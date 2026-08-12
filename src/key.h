// Copyright (c) 2026 tessera core
// See COPYING for license.
//
// Private key + signing, modeled on Bitcoin Core's key.h (CKey / CPrivKey).
// Tessera is crypto-agile and post-quantum: where Core's CKey wraps a 32-byte
// secp256k1 scalar, ours wraps the vendored ML-DSA-44 secret key (PQClean /
// FIPS 204, the standardized Dilithium2) — the analog of Core's secp256k1 subtree.
//
// One structural difference is forced by the scheme: a secp256k1 public key is
// cheaply derivable from its private scalar, so Core's CKey stores only the
// secret and recomputes the CPubKey on demand. ML-DSA cannot do that — the
// secret key packs t0 while the public key packs t1, and recovering t1 means
// re-expanding the matrix A and recomputing t = A*s1 + s2 (work the PQClean API
// does not expose). So a CKey here honestly carries BOTH the secret key and its
// matching CPubKey, captured together at key-generation time.
//
// This header reproduces the full meaningful surface of Core's CKey. The parts
// of Core's key.h that are pure elliptic-curve / BIP machinery with no lattice
// analog are NOT silently dropped — they are enumerated, with the reason, in the
// "No ML-DSA analog" block at the bottom of the CKey class, mirroring how
// pubkey.h documents the secp256k1-only public-key types it omits. The one piece
// that is genuinely wanted but blocked is hierarchical-deterministic derivation
// (Core's CExtKey / BIP32): ML-DSA has no additive key tweak, so HD would need a
// deterministic keypair-from-seed primitive that the vendored PQClean build does
// not expose. It is deferred as a design decision rather than invented here.

#ifndef TESSERA_KEY_H
#define TESSERA_KEY_H

#include <pubkey.h>
#include <serialize.h>
#include <support/allocators/secure.h>
#include <uint256.h>

#include <array>
#include <cstring>
#include <span>
#include <vector>

/**
 * CPrivKey is a serialized private key, with all parameters included.
 * For ML-DSA-44 this is simply the raw 2560-byte secret key; the matching
 * public key (which carries the scheme tag) is stored alongside it in a CKey.
 */
typedef std::vector<unsigned char, secure_allocator<unsigned char>> CPrivKey;

/** An encapsulated post-quantum (ML-DSA-44) private key. */
class CKey
{
public:
    /**
     * ML-DSA-44 sizes (PQClean / FIPS 204). SIZE is the secret key, which is the
     * secure material a CKey owns. Core additionally exposes COMPRESSED_SIZE and
     * COMPACT_SIGNATURE_SIZE; neither has a meaning for ML-DSA (keys are not
     * point-compressed and there is no public-key-recovery "compact" signature).
     */
    static constexpr unsigned int SIZE = 2560;            // CRYPTO_SECRETKEYBYTES
    static constexpr unsigned int PUBLIC_KEY_SIZE = 1312; // CRYPTO_PUBLICKEYBYTES
    static constexpr unsigned int SIGNATURE_SIZE = 2420;  // CRYPTO_BYTES
    static constexpr unsigned int SEED_SIZE = 32;         // SEEDBYTES (KeyGen seed xi)

private:
    //! Internal data container for the secret key material.
    using KeyType = std::array<unsigned char, SIZE>;

    //! The actual secret-key bytes, in locked+cleansed memory. nullptr == invalid.
    secure_unique_ptr<KeyType> keydata;

    //! The matching public key (ML-DSA cannot cheaply derive it from the secret).
    CPubKey m_pubkey;

    //! Check whether the secret-key material is structurally well-formed. Unlike
    //! secp256k1 (where Check enforces the scalar is in [1, n-1]), an ML-DSA secret
    //! is not cheaply validatable; semantic validity is established by a sign+verify
    //! round-trip against the paired public key (see VerifyPubKey).
    static bool Check(const unsigned char* vch);

    void MakeKeyData()
    {
        if (!keydata) keydata = make_secure_unique<KeyType>();
    }

    void ClearKeyData()
    {
        keydata.reset();
        m_pubkey = CPubKey();
    }

public:
    CKey() noexcept = default;
    CKey(CKey&&) noexcept = default;
    CKey& operator=(CKey&&) noexcept = default;

    CKey& operator=(const CKey& other)
    {
        if (other.keydata) {
            MakeKeyData();
            *keydata = *other.keydata;
        } else {
            ClearKeyData();
        }
        m_pubkey = other.m_pubkey;
        return *this;
    }

    CKey(const CKey& other) { *this = other; }

    friend bool operator==(const CKey& a, const CKey& b)
    {
        return a.size() == b.size() &&
               (a.size() == 0 || std::memcmp(a.data(), b.data(), a.size()) == 0);
    }

    /**
     * Initialize from a secret/public key pair given as byte iterators over the
     * raw secret (SIZE bytes) and the serialized public key. This is the ML-DSA
     * analog of Core's `Set(pbegin, pend, fCompressed)`: because the public key
     * cannot be recomputed from the secret, it must be supplied rather than
     * derived, so the compression flag is replaced by the public-key bytes. The
     * key is left invalid if the lengths are wrong or the pair does not match.
     */
    template <typename T>
    void Set(const T sk_begin, const T sk_end, const CPubKey& pubkey)
    {
        if (size_t(sk_end - sk_begin) != SIZE || !Check(UCharCast(&sk_begin[0]))) {
            ClearKeyData();
            return;
        }
        MakeKeyData();
        std::memcpy(keydata->data(), (const unsigned char*)&sk_begin[0], SIZE);
        m_pubkey = pubkey;
        if (!VerifyPubKey(pubkey)) ClearKeyData();
    }

    //! Simple read-only vector-like interface over the secret-key bytes.
    unsigned int size() const { return keydata ? static_cast<unsigned int>(keydata->size()) : 0; }
    const unsigned char* data() const { return keydata ? keydata->data() : nullptr; }
    const unsigned char* begin() const { return data(); }
    const unsigned char* end() const { return data() + size(); }

    //! Check whether this private key is valid.
    bool IsValid() const { return !!keydata; }

    //! The signature scheme of this key. Single scheme for now (ML-DSA-44).
    SigScheme GetScheme() const { return SigScheme::ML_DSA_44; }

    //! Generate a new fresh ML-DSA-44 keypair (secret + matching public key).
    void MakeNewKey();

    //! Deterministically generate the keypair from a SEED_SIZE-byte seed (FIPS 204
    //! ML-DSA.KeyGen_internal(xi)). Backs hierarchical-deterministic derivation
    //! (see CExtKey); the key is left invalid if the seed is the wrong length.
    bool MakeKeyFromSeed(std::span<const unsigned char> seed);

    //! Convert the private key to a serialized (raw secret-key byte) form.
    CPrivKey GetPrivKey() const;

    //! Compute the public key from a private key (here: return the stored key).
    CPubKey GetPubKey() const { return m_pubkey; }

    /**
     * Create an ML-DSA signature over the 32-byte message `hash` (a sighash).
     * The produced signature is the bare scheme signature; callers that build a
     * scriptSig/witness append the one-byte sighash type themselves.
     *
     * Core's signature carries `grind` / `test_case` parameters; those drive
     * low-R grinding of a DER ECDSA signature and have no ML-DSA analog (the
     * scheme is a hedged Fiat-Shamir lattice signature of fixed length).
     */
    bool Sign(const uint256& hash, std::vector<unsigned char>& vchSig) const;

    //! Verify that a candidate public key is the one matching this private key,
    //! by signing a random challenge and checking it verifies under that key.
    //! (ML-DSA's public key is not derivable from the secret, so a sign+verify
    //! round-trip is the available equality test.)
    bool VerifyPubKey(const CPubKey& vchPubKey) const;

    //! Load a private key and (re)build the CKey. The public key must be supplied
    //! because it cannot be recomputed from the ML-DSA secret alone.
    bool Load(const CPrivKey& privkey, const CPubKey& vchPubKey, bool fSkipCheck);

    // --- Surface of Core's CKey with no ML-DSA analog (intentionally absent) ---
    //
    //   IsCompressed()                    secp256k1 points can be stored
    //                                     compressed/uncompressed; ML-DSA keys are
    //                                     a single fixed serialization.
    //   Negate()                          EC scalar negation (for BIP32 / Taproot
    //                                     tweaks); a lattice secret has no such op.
    //   SignCompact()                     65-byte recoverable ECDSA signature for
    //                                     public-key recovery; ML-DSA has no key
    //                                     recovery from a signature.
    //   SignSchnorr() / ComputeKeyPair()  BIP-340 Schnorr / Taproot key-path; not
    //                                     part of Tessera's witness-only ML-DSA
    //                                     script model (option A).
    //   Derive() (BIP32) / CExtKey        HD child derivation by additive EC tweak;
    //                                     impossible for ML-DSA. A PQC HD scheme
    //                                     would need deterministic keypair-from-seed
    //                                     (not exposed by the vendored PQClean) and
    //                                     is a separate design decision — deferred.
    //   EllSwiftCreate() /                BIP-324 v2 transport ElligatorSwift + ECDH;
    //   ComputeBIP324ECDHSecret()         a curve key-exchange with no lattice analog.
};

//! Produce a fresh random ML-DSA-44 private key (with its matching public key).
CKey GenerateRandomKey() noexcept;

/**
 * Self-test the signature primitive at startup: generate a key, sign a fixed
 * message, and confirm it verifies under the derived public key. This is the
 * ML-DSA analog of Core's ECC_InitSanityCheck(); unlike secp256k1 there is no
 * global signing context to start/stop (ECC_Start / ECC_Stop / ECC_Context),
 * so the scheme needs no init/teardown — only this round-trip check.
 */
bool MLDSA_InitSanityCheck();

//! A BIP32-style chain code: 256 bits of derivation entropy carried alongside a key.
using ChainCode = uint256;

/**
 * A hierarchical-deterministic extended private key, the Tessera analog of
 * Core's CExtKey (BIP32). Because ML-DSA has no additive key tweak, the chainable
 * secret is NOT the (2560-byte) signing key but the 32-byte key-generation seed
 * that deterministically expands to it (FIPS 204 KeyGen_internal). A child is
 * derived by mixing the parent's secret node seed into a SHA3-512 PRF keyed by
 * the chain code, so derivation is HARDENED-ONLY: a parent extended *public* key
 * cannot derive child public keys (there is no key homomorphism for a lattice
 * scheme), which is why there is no Neuter() / CExtPubKey here. One master seed
 * still backs up and recovers the whole tree.
 */
struct CExtKey {
    unsigned char nDepth{0};
    unsigned char vchFingerprint[4]{0, 0, 0, 0};
    unsigned int nChild{0};
    ChainCode chaincode;
    uint256 nodeseed;  //!< the 32-byte ML-DSA key-generation seed (the chainable secret)
    CKey key;          //!< signing key, expanded from nodeseed

    //! Serialized size: depth(1) + fingerprint(4) + child(4) + chaincode(32) + nodeseed(32).
    static constexpr unsigned int SIZE = 1 + 4 + 4 + 32 + 32;

    friend bool operator==(const CExtKey& a, const CExtKey& b)
    {
        return a.nDepth == b.nDepth &&
               std::memcmp(a.vchFingerprint, b.vchFingerprint, sizeof(vchFingerprint)) == 0 &&
               a.nChild == b.nChild &&
               a.chaincode == b.chaincode &&
               a.nodeseed == b.nodeseed &&
               a.key == b.key;
    }

    //! Initialize as the master extended key from a wallet seed (any length).
    void SetSeed(std::span<const unsigned char> seed);


    //! Compute the master node from a seed and a personalization tag (shared by the two above).
    void SetSeedFromTag(std::span<const unsigned char> seed, const unsigned char* tag, size_t taglen);

    //! Derive the hardened child at index nChild. Returns false if this key is invalid.
    [[nodiscard]] bool Derive(CExtKey& out, unsigned int nChild) const;

    //! Serialize / deserialize the extended key to a fixed SIZE-byte form. Decode
    //! re-expands the signing key from the stored node seed.
    void Encode(unsigned char code[SIZE]) const;
    void Decode(const unsigned char code[SIZE]);

    // No Neuter() / CExtPubKey: child public keys cannot be derived from a parent
    // public key under ML-DSA (no key homomorphism). HD here is hardened-only.
};

#endif // TESSERA_KEY_H
