// Copyright (c) 2026 tessera core
// See COPYING for license.
//
// Tests for the ML-DSA-44 private key (key.cpp): fresh key generation yields a
// valid secret + matching public key; signing a sighash produces a signature
// that verifies under that public key (and only that one); a tampered signature
// or message is rejected; GetPrivKey/Load round-trips a key; and VerifyPubKey
// accepts the matching public key while rejecting a foreign one.

#include <key.h>
#include <pubkey.h>
#include <uint256.h>
#include <hash.h>

#include <cstdio>
#include <vector>

namespace {
int g_fail = 0;
void Check(const char* name, bool ok)
{
    std::printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) ++g_fail;
}
} // namespace

int main()
{
    // A freshly generated key is valid and exposes a valid, well-formed pubkey.
    CKey key = GenerateRandomKey();
    Check("generated key is valid", key.IsValid());
    Check("key scheme is ML-DSA-44", key.GetScheme() == SigScheme::ML_DSA_44);
    Check("secret key size == 2560", key.size() == CKey::SIZE);

    CPubKey pub = key.GetPubKey();
    Check("public key is valid", pub.IsValid());
    Check("public key size == 1 + 1312", pub.size() == 1 + CKey::PUBLIC_KEY_SIZE);
    Check("public key scheme tag is ML-DSA-44", pub.GetScheme() == SigScheme::ML_DSA_44);

    // Sign a sighash and verify it under the public key.
    const uint256 hash = Hash(std::span<const unsigned char>(pub.data(), pub.size()));
    std::vector<unsigned char> sig;
    bool signed_ok = key.Sign(hash, sig);
    Check("Sign succeeds", signed_ok);
    Check("signature size == 2420", sig.size() == CKey::SIGNATURE_SIZE);
    Check("signature verifies under own pubkey", pub.Verify(hash, sig));

    // A different message must not verify against that signature.
    {
        uint256 other = hash;
        other.begin()[0] ^= 0x01;
        Check("signature does not verify over a different hash", !pub.Verify(other, sig));
    }

    // A tampered signature is rejected.
    {
        std::vector<unsigned char> bad = sig;
        bad[bad.size() / 2] ^= 0xff;
        Check("tampered signature is rejected", !pub.Verify(hash, bad));
    }

    // Signing is the hedged (randomized) ML-DSA variant: two signatures over the
    // same message differ, yet both verify.
    {
        std::vector<unsigned char> sig2;
        bool ok2 = key.Sign(hash, sig2);
        Check("second Sign succeeds", ok2);
        Check("two signatures over same msg differ (hedged)", sig2 != sig);
        Check("second signature also verifies", pub.Verify(hash, sig2));
    }

    // A second, independent key has a different pubkey, and cross-verification
    // fails both ways.
    CKey key2 = GenerateRandomKey();
    CPubKey pub2 = key2.GetPubKey();
    Check("two fresh keys have different pubkeys", pub2 != pub);
    Check("key1 signature does not verify under key2's pubkey", !pub2.Verify(hash, sig));
    {
        std::vector<unsigned char> sig2;
        key2.Sign(hash, sig2);
        Check("key2 signature does not verify under key1's pubkey", !pub.Verify(hash, sig2));
    }

    // GetPrivKey / Load round-trips: the rebuilt key equals the original, exposes
    // the same pubkey, and still produces verifiable signatures.
    {
        CPrivKey priv = key.GetPrivKey();
        Check("serialized private key size == 2560", priv.size() == CKey::SIZE);
        CKey loaded;
        bool load_ok = loaded.Load(priv, pub, /*fSkipCheck=*/false);
        Check("Load (with check) succeeds", load_ok);
        Check("loaded key equals original", loaded == key);
        Check("loaded key exposes same pubkey", loaded.GetPubKey() == pub);
        std::vector<unsigned char> sig3;
        Check("loaded key signs", loaded.Sign(hash, sig3));
        Check("loaded key's signature verifies", pub.Verify(hash, sig3));
    }

    // Load with a mismatched pubkey is rejected by the round-trip check.
    {
        CPrivKey priv = key.GetPrivKey();
        CKey loaded;
        Check("Load with foreign pubkey is rejected", !loaded.Load(priv, pub2, /*fSkipCheck=*/false));
    }

    // VerifyPubKey: accepts the matching key, rejects a foreign one.
    Check("VerifyPubKey accepts own pubkey", key.VerifyPubKey(pub));
    Check("VerifyPubKey rejects foreign pubkey", !key.VerifyPubKey(pub2));

    // Set() rebuilds a key from raw secret bytes + the matching public key, and
    // rejects a mismatched public key (leaving the key invalid).
    {
        CPrivKey priv = key.GetPrivKey();
        CKey via_set;
        via_set.Set(priv.begin(), priv.end(), pub);
        Check("Set(secret, matching pubkey) yields a valid key", via_set.IsValid() && via_set == key);
        CKey via_set_bad;
        via_set_bad.Set(priv.begin(), priv.end(), pub2);
        Check("Set(secret, foreign pubkey) is rejected", !via_set_bad.IsValid());
    }

    // Load with fSkipCheck=true trusts the caller and skips the round-trip.
    {
        CPrivKey priv = key.GetPrivKey();
        CKey fast;
        Check("Load(fSkipCheck=true) succeeds", fast.Load(priv, pub, /*fSkipCheck=*/true));
        Check("fSkipCheck-loaded key equals original", fast == key);
        std::vector<unsigned char> s;
        Check("fSkipCheck-loaded key signs verifiably", fast.Sign(hash, s) && pub.Verify(hash, s));
    }

    // Startup self-test of the signature primitive.
    Check("MLDSA_InitSanityCheck passes", MLDSA_InitSanityCheck());

    std::printf("%s\n", g_fail == 0 ? "ALL PASS" : "FAILURES");
    return g_fail == 0 ? 0 : 1;
}
