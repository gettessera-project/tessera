// Copyright (c) 2026 tessera core
// See COPYING for license.

#include <key.h>

#include <crypto/sha3_512.h>
#include <hash.h>
#include <random.h>
#include <util/check.h>

extern "C" {
#include "api.h"
#include "randombytes.h"
}

#include <cassert>
#include <span>
#include <string>

bool CKey::Check(const unsigned char* vch)
{
    // ML-DSA secret keys have no cheap validity predicate — unlike a secp256k1
    // scalar, which Check confirms lies in [1, n-1]. Any non-null buffer of the
    // right length (enforced by the caller) is structurally acceptable; semantic
    // validity is established by VerifyPubKey's sign+verify round-trip.
    return vch != nullptr;
}

void CKey::MakeNewKey()
{
    MakeKeyData();
    std::array<unsigned char, PUBLIC_KEY_SIZE> pk;
    // PQClean draws its own OS entropy (randombytes); on failure it aborts, so a
    // return here is always success. The secret key lands directly in our locked
    // buffer; the public key is captured alongside it (it cannot be recomputed).
    const int ret = PQCLEAN_MLDSA44_CLEAN_crypto_sign_keypair(pk.data(), keydata->data());
    assert(ret == 0);
    (void)ret;
    m_pubkey = CPubKey(SigScheme::ML_DSA_44, pk);
    assert(m_pubkey.IsValid());
}

bool CKey::MakeKeyFromSeed(std::span<const unsigned char> seed)
{
    if (seed.size() != SEED_SIZE) {
        ClearKeyData();
        return false;
    }
    MakeKeyData();
    std::array<unsigned char, PUBLIC_KEY_SIZE> pk;
    // Drive the UNMODIFIED crypto_sign_keypair from a fixed seed by switching the
    // ML-DSA entropy source into deterministic mode for the duration of this one
    // call (see randombytes.h). The lattice crypto is byte-for-byte verbatim.
    mldsa_set_deterministic_seed(seed.data());
    const int ret = PQCLEAN_MLDSA44_CLEAN_crypto_sign_keypair(pk.data(), keydata->data());
    mldsa_clear_deterministic_seed();
    assert(ret == 0);
    (void)ret;
    m_pubkey = CPubKey(SigScheme::ML_DSA_44, pk);
    return m_pubkey.IsValid();
}

CPrivKey CKey::GetPrivKey() const
{
    assert(keydata);
    return CPrivKey(keydata->begin(), keydata->end());
}

bool CKey::Sign(const uint256& hash, std::vector<unsigned char>& vchSig) const
{
    if (!keydata) return false;
    vchSig.resize(SIGNATURE_SIZE);
    size_t siglen = 0;
    const int ret = PQCLEAN_MLDSA44_CLEAN_crypto_sign_signature(
        vchSig.data(), &siglen, hash.begin(), hash.size(), keydata->data());
    if (ret != 0) {
        vchSig.clear();
        return false;
    }
    vchSig.resize(siglen);
    return true;
}

bool CKey::VerifyPubKey(const CPubKey& pubkey) const
{
    if (!IsValid() || !pubkey.IsValid()) return false;
    if (pubkey.GetScheme() != GetScheme()) return false;

    // Prove the secret matches the candidate public key: sign a random challenge
    // and check it verifies under that key. (ML-DSA's public key is not derivable
    // from the secret, so a sign+verify round-trip is the available equality test.)
    const uint256 hash = GetRandHash();
    std::vector<unsigned char> vchSig;
    if (!Sign(hash, vchSig)) return false;
    return pubkey.Verify(hash, vchSig);
}

bool CKey::Load(const CPrivKey& privkey, const CPubKey& vchPubKey, bool fSkipCheck)
{
    if (privkey.size() != SIZE || !vchPubKey.IsValid() ||
        vchPubKey.GetScheme() != SigScheme::ML_DSA_44) {
        return false;
    }
    if (fSkipCheck) {
        MakeKeyData();
        std::memcpy(keydata->data(), privkey.data(), SIZE);
        m_pubkey = vchPubKey;
        return true;
    }
    Set(privkey.begin(), privkey.end(), vchPubKey);
    return IsValid();
}

CKey GenerateRandomKey() noexcept
{
    CKey key;
    key.MakeNewKey();
    return key;
}

bool MLDSA_InitSanityCheck()
{
    const CKey key = GenerateRandomKey();
    if (!key.IsValid()) return false;
    const CPubKey pubkey = key.GetPubKey();
    if (!pubkey.IsValid()) return false;

    static const std::string test{"Tessera ML-DSA sanity check"};
    const uint256 hash = Hash(std::span<const unsigned char>(
        reinterpret_cast<const unsigned char*>(test.data()), test.size()));
    std::vector<unsigned char> sig;
    if (!key.Sign(hash, sig)) return false;
    return pubkey.Verify(hash, sig) && key.VerifyPubKey(pubkey);
}

// --- CExtKey: hardened-only hierarchical-deterministic derivation ------------

void CExtKey::SetSeedFromTag(std::span<const unsigned char> seed, const unsigned char* tag, size_t taglen)
{
    // BIP32 requires a seed of 128 to 512 bits, and the bound is not decoration: a
    // short seed yields a master key with only as much entropy as was fed in, and
    // nothing downstream would report it. Enforced as a programming invariant --
    // every caller in this tree passes a fixed-width seed, so a violation is a bug
    // rather than bad input.
    Assert(16 <= seed.size() && seed.size() <= 64);

    // Master node: I = SHA3-512(tag || seed); the first 32 bytes are the master node seed
    // (the ML-DSA KeyGen seed), the rest the chain code.
    unsigned char out[SHA3_512::OUTPUT_SIZE];
    SHA3_512().Write(tag, taglen).Write(seed.data(), seed.size()).Finalize(out);

    nDepth = 0;
    nChild = 0;
    std::memset(vchFingerprint, 0, sizeof(vchFingerprint));
    nodeseed = uint256(std::span<const unsigned char>(out, CKey::SEED_SIZE));
    chaincode = ChainCode(std::span<const unsigned char>(out + CKey::SEED_SIZE, 32));
    key.MakeKeyFromSeed(std::span<const unsigned char>(nodeseed.begin(), CKey::SEED_SIZE));
}

void CExtKey::SetSeed(std::span<const unsigned char> seed)
{
    static const unsigned char hashkey[] = {
        'T', 'e', 's', 's', 'e', 'r', 'a', ' ',
        'H', 'D', ' ', 'm', 'a', 's', 't', 'e', 'r'};
    SetSeedFromTag(seed, hashkey, sizeof(hashkey));
}


bool CExtKey::Derive(CExtKey& out, unsigned int _nChild) const
{
    if (!key.IsValid()) return false;

    out.nDepth = nDepth + 1;
    const CKeyID id = key.GetPubKey().GetID();
    std::memcpy(out.vchFingerprint, id.begin(), sizeof(out.vchFingerprint));
    out.nChild = _nChild;

    // Hardened-only: I = SHA3-512(chaincode || nodeseed || ser32BE(nChild)). The
    // parent's secret node seed is always part of the preimage, so a child cannot
    // be derived from public material alone.
    unsigned char data[32 + CKey::SEED_SIZE + 4];
    std::memcpy(data, chaincode.begin(), 32);
    std::memcpy(data + 32, nodeseed.begin(), CKey::SEED_SIZE);
    data[64] = static_cast<unsigned char>(_nChild >> 24);
    data[65] = static_cast<unsigned char>(_nChild >> 16);
    data[66] = static_cast<unsigned char>(_nChild >> 8);
    data[67] = static_cast<unsigned char>(_nChild);

    unsigned char out64[SHA3_512::OUTPUT_SIZE];
    SHA3_512().Write(data, sizeof(data)).Finalize(out64);
    out.nodeseed = uint256(std::span<const unsigned char>(out64, CKey::SEED_SIZE));
    out.chaincode = ChainCode(std::span<const unsigned char>(out64 + CKey::SEED_SIZE, 32));
    return out.key.MakeKeyFromSeed(std::span<const unsigned char>(out.nodeseed.begin(), CKey::SEED_SIZE));
}

void CExtKey::Encode(unsigned char code[SIZE]) const
{
    code[0] = nDepth;
    std::memcpy(code + 1, vchFingerprint, 4);
    code[5] = static_cast<unsigned char>(nChild >> 24);
    code[6] = static_cast<unsigned char>(nChild >> 16);
    code[7] = static_cast<unsigned char>(nChild >> 8);
    code[8] = static_cast<unsigned char>(nChild);
    std::memcpy(code + 9, chaincode.begin(), 32);
    std::memcpy(code + 41, nodeseed.begin(), CKey::SEED_SIZE);
}

void CExtKey::Decode(const unsigned char code[SIZE])
{
    nDepth = code[0];
    std::memcpy(vchFingerprint, code + 1, 4);
    nChild = (static_cast<unsigned int>(code[5]) << 24) | (static_cast<unsigned int>(code[6]) << 16) |
             (static_cast<unsigned int>(code[7]) << 8) | static_cast<unsigned int>(code[8]);
    chaincode = ChainCode(std::span<const unsigned char>(code + 9, 32));
    nodeseed = uint256(std::span<const unsigned char>(code + 41, CKey::SEED_SIZE));
    key.MakeKeyFromSeed(std::span<const unsigned char>(nodeseed.begin(), CKey::SEED_SIZE));
}
