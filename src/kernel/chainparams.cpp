// Copyright (c) 2026 tessera core
// See COPYING for license.
//
// Tessera network parameters. Unlike Bitcoin Core's kernel/chainparams.cpp
// (which defines the Bitcoin network), this file defines Tessera's networks
// and genesis blocks. The structure mirrors Core; the values are Tessera's.
//
// The genesis blocks were mined with SHA3-256d proof of work (grind nNonce until
// SHA3-256d of the 80-byte header is at or under the nBits target). The nNonce /
// hashGenesisBlock values below are the mining output and are reproduced +
// validated by test/test_chainparams.
//
// Tessera identity values (network magic, ports, address prefixes, bech32
// HRPs) are PROVISIONAL — chosen here so the chain is runnable, easy to revise
// before mainnet freeze.

#include <kernel/chainparams.h>

#include <arith_uint256.h>
#include <consensus/amount.h>
#include <consensus/merkle.h>
#include <consensus/params.h>
#include <primitives/block.h>
#include <primitives/transaction.h>
#include <script/script.h>
#include <uint256.h>
#include <util/chaintype.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <vector>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.version = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Its coinbase carries the headline — Tessera's
 * post-quantum analogue of the bank-bailout line Satoshi put in Bitcoin's
 * genesis. Its single output is provably unspendable (OP_RETURN) and, like
 * every genesis coinbase, is never added to the UTXO set.
 */
// nTime MUST be in the past. With only genesis in the chain its timestamp is the
// median time past, so block 1 inherits it; a future-dated genesis makes every
// first block fail the two-hour rule and the network cannot start at all. The
// error surfaces as "time-too-new" against the NEW block, which points away from
// the real cause.
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Google Quantum AI 30/Mar/2026 ECDLP-256 in minutes on under half a million physical qubits";
    const CScript genesisOutputScript = CScript() << OP_RETURN;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::ApplyDeploymentOptions(const DeploymentOptions& opts)
{
    for (const auto& [dep, height] : opts.activation_heights) {
        switch (dep) {
        case Consensus::BuriedDeployment::DEPLOYMENT_SEGWIT:
            consensus.SegwitHeight = int{height};
            break;
        case Consensus::BuriedDeployment::DEPLOYMENT_HEIGHTINCB:
            consensus.BIP34Height = int{height};
            break;
        case Consensus::BuriedDeployment::DEPLOYMENT_DERSIG:
            consensus.BIP66Height = int{height};
            break;
        case Consensus::BuriedDeployment::DEPLOYMENT_CLTV:
            consensus.BIP65Height = int{height};
            break;
        case Consensus::BuriedDeployment::DEPLOYMENT_CSV:
            consensus.CSVHeight = int{height};
            break;
        }
    }

    for (const auto& [deployment_pos, version_bits_params] : opts.version_bits_parameters) {
        consensus.vDeployments[deployment_pos].nStartTime = version_bits_params.start_time;
        consensus.vDeployments[deployment_pos].nTimeout = version_bits_params.timeout;
        consensus.vDeployments[deployment_pos].min_activation_height = version_bits_params.min_activation_height;
    }
}

/**
 * Main network on which people trade goods and services.
 */
class CMainParams : public CChainParams {
public:
    CMainParams(const MainNetOptions& opts) {
        m_chain_type = ChainType::MAIN;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 210000;
        // Tessera enforces its rules from the first block it can, which is one
        // and not zero.
        //
        // Genesis is exempt, and not as a concession: it is accepted by a
        // hardcoded hash, so no rule applied to it can change which chain is
        // valid, and the contextual checks read a parent it does not have. Zero
        // here makes ContextualCheckBlock assert on genesis, which aborts
        // -reindex. Core uses one for this same reason on every chain whose
        // rules start at the beginning.
        //
        // Nothing else moves. For the block at height one the test is 1 >= 0
        // against 1 >= 1, true either way, and it stays true for every block
        // above it. Genesis is the only block whose treatment changes.
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        // Segwit stays at zero: its check never reads the parent.
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256{"00000000ffff0000000000000000000000000000000000000000000000000000"};
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;            // ten minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.enforce_BIP94 = false;
        consensus.fPowNoRetargeting = false;
        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY] = Consensus::BIP9Deployment{};

        // Provisional Tessera identity (revise before mainnet freeze).
        pchMessageStart[0] = 0x32;
        pchMessageStart[1] = 0x2c;
        pchMessageStart[2] = 0x9b;
        pchMessageStart[3] = 0xb3;
        nDefaultPort = 11333;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock(1786492800, 3204703079, 0x1d00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256{"033d419a2fc78583172e44f3f3681f1c6f83cf78e3e9becb06a255e1290f4a6e"});
        assert(genesis.hashMerkleRoot == uint256{"c6360e1eed547b847a7b71d35ba209eafe6f24af5f33142c5ee9006b4e83978a"});

        vSeeds.clear();
        // DNS seeds — queried for peer addresses on first start. A subdomain, so
        // the apex stays free for the project site; it must resolve directly to
        // node addresses, which means it cannot sit behind a CDN proxy.
        vSeeds.emplace_back("seed.gettessera.org.");
        // No fixed seeds: the hardcoded address list is generated from a running
        // network, and there is not one yet.
        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 65);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 75);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 193);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x36, 0x48, 0x00};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x36, 0x44, 0x00};
        bech32_hrp = "tsr";

        fDefaultConsistencyChecks = false;
        m_is_mockable_chain = false;

        chainTxData = ChainTxData{0, 0, 0};

        ApplyDeploymentOptions(opts.dep_opts);
    }
};

/**
 * Testnet: a public test network with its own genesis.
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams(const TestNetOptions& opts) {
        m_chain_type = ChainType::TESTNET;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 210000;
        // One and not zero, for the reason spelled out in CMainParams.
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256{"00000000ffff0000000000000000000000000000000000000000000000000000"};
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.enforce_BIP94 = false;
        consensus.fPowNoRetargeting = false;
        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY] = Consensus::BIP9Deployment{};

        pchMessageStart[0] = 0x40;
        pchMessageStart[1] = 0x29;
        pchMessageStart[2] = 0xd9;
        pchMessageStart[3] = 0xec;
        nDefaultPort = 21333;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock(1786496400, 49524069, 0x1d00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256{"8b7344b7e963140f3ecbbe3899642635c60316661b14de323da8f60b92626094"});
        assert(genesis.hashMerkleRoot == uint256{"c6360e1eed547b847a7b71d35ba209eafe6f24af5f33142c5ee9006b4e83978a"});

        vSeeds.clear();
        // DNS seeds for testnet — testnet-prefixed so they resolve to testnet
        // nodes, separate from the mainnet seed records.
        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 127);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 137);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 255);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x9A, 0x00};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x96, 0x00};
        bech32_hrp = "ttsr";

        fDefaultConsistencyChecks = false;
        m_is_mockable_chain = false;

        chainTxData = ChainTxData{0, 0, 0};

        ApplyDeploymentOptions(opts.dep_opts);
    }
};

/**
 * Regression test: intended for private networks, which are not intended to be
 * publicly accessible and is used for testing. Difficulty never retargets.
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const RegTestOptions& opts) {
        m_chain_type = ChainType::REGTEST;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 150;
        // One and not zero, for the reason spelled out in CMainParams.
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256{"7fffff0000000000000000000000000000000000000000000000000000000000"};
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.enforce_BIP94 = opts.enforce_bip94;
        consensus.fPowNoRetargeting = true;
        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY] = Consensus::BIP9Deployment{};

        pchMessageStart[0] = 0x55;
        pchMessageStart[1] = 0x7c;
        pchMessageStart[2] = 0x94;
        pchMessageStart[3] = 0x3f;
        nDefaultPort = 21444;
        nPruneAfterHeight = opts.fastprune ? 100 : 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1786500000, 1, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256{"3d62c24130edb6183dfe0b0dae30ddcdb707bba6202138567ef7e5fe6fdfae4a"});
        assert(genesis.hashMerkleRoot == uint256{"c6360e1eed547b847a7b71d35ba209eafe6f24af5f33142c5ee9006b4e83978a"});

        vSeeds.clear();
        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 127);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 137);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 255);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x9A, 0x00};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x96, 0x00};
        bech32_hrp = "tsrrt";

        fDefaultConsistencyChecks = true;
        m_is_mockable_chain = true;

        chainTxData = ChainTxData{0, 0, 0};

        ApplyDeploymentOptions(opts.dep_opts);
    }
};

std::vector<int> CChainParams::GetAvailableSnapshotHeights() const
{
    std::vector<int> heights;
    heights.reserve(m_assumeutxo_data.size());

    for (const auto& data : m_assumeutxo_data) {
        heights.push_back(data.height);
    }
    return heights;
}

std::optional<ChainType> GetNetworkForMagic(const MessageStartChars& message)
{
    const auto mainnet_msg = CChainParams::Main()->MessageStart();
    const auto testnet_msg = CChainParams::TestNet()->MessageStart();
    const auto regtest_msg = CChainParams::RegTest()->MessageStart();

    if (std::ranges::equal(message, mainnet_msg)) {
        return ChainType::MAIN;
    } else if (std::ranges::equal(message, testnet_msg)) {
        return ChainType::TESTNET;
    } else if (std::ranges::equal(message, regtest_msg)) {
        return ChainType::REGTEST;
    }
    return std::nullopt;
}

std::unique_ptr<const CChainParams> CChainParams::RegTest(const RegTestOptions& options)
{
    return std::make_unique<const CRegTestParams>(options);
}

std::unique_ptr<const CChainParams> CChainParams::Main(const MainNetOptions& options)
{
    return std::make_unique<const CMainParams>(options);
}

std::unique_ptr<const CChainParams> CChainParams::TestNet(const TestNetOptions& options)
{
    return std::make_unique<const CTestNetParams>(options);
}

// SigNet and TestNet4 are Bitcoin-specific networks (BIP325 signed blocks /
// the 2024 testnet reset). Tessera does not define them yet; they will be
// added with their own mined genesis if/when needed.
std::unique_ptr<const CChainParams> CChainParams::SigNet(const SigNetOptions&)
{
    throw std::runtime_error("Tessera: signet is not defined yet");
}

std::unique_ptr<const CChainParams> CChainParams::TestNet4(const TestNetOptions&)
{
    throw std::runtime_error("Tessera: testnet4 is not defined yet");
}
