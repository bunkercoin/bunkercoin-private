// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2022 The Dogecoin Core developers
// Copyright (c) 2022 The Bunkercoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
	
    CMutableTransaction txNew;
    txNew.nVersion = 1;
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
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1386325540, nBits=0x1e0ffff0, nNonce=99943, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Times of Malta First coronavirus cases are girl and parents 7/Mar/21";
    const CScript genesisOutputScript = CScript() << ParseHex("04536744d5a57623b6602d8325383a580b1c00076a9e68fd75ff1f07240fb7408916e919cec8b8750a6e6aff08339db104106a75f976d5817490b9a1bdc2e0c727") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
    Consensus::Params improvedrewardsConsensus;
public:
    CMainParams() {
        strNetworkID = "main";

        // Blocks 0 - 10 are conventional difficulty calculation
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.nMajorityEnforceBlockUpgrade = 1500;
        consensus.nMajorityRejectBlockOutdated = 1900;
        consensus.nMajorityWindow = 2000;
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.nCoinbaseMaturity = 30;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 9576; // 95% of 10,080
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week
        consensus.fAllowLegacyBlocks = true;

		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        // XXX: BIP heights and hashes all need to be updated to Bunkercoin values
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1645000000; // Feb 16 2022
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1670000000; // Dec 02 2022

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1670000000; // Mar 11 2022
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // Disabled



		//BIP are deployed gradually at 85000 blocks 
        consensus.BIP34Height = 83000;
        consensus.BIP65Height = 84000;
        consensus.BIP66Height = 85000;
		
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000002a112b111a1bcab");

 

        // Blocks 10 - 100000 are Digishield without AuxPoW
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 10;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.nPowTargetTimespan = 60; // post-digishield: 1 minute
        digishieldConsensus.nCoinbaseMaturity = 30;
		consensus.fAllowLegacyBlocks = true;

        // Blocks 100000+ are AuxPoW or PoW
        auxpowConsensus = digishieldConsensus;
		auxpowConsensus.nAuxpowChainId = 0x0042; // 73s
        auxpowConsensus.nHeightEffective = 100000;
        auxpowConsensus.fAllowLegacyBlocks = false;

        // Blocks 600000+ on mainnet are AuxPoW (or PoW) with improved rewards.
        improvedrewardsConsensus = auxpowConsensus;
        improvedrewardsConsensus.fSimplifiedRewards = false;
        improvedrewardsConsensus.nHeightEffective = 600000;

        // Assemble the binary search tree of consensus parameters
        // Reminder (IMPORTANT!!): Remember to add a root here in case of future hardforks!
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;
        auxpowConsensus.pRight = &improvedrewardsConsensus;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xe8;
        pchMessageStart[1] = 0xc9;
        pchMessageStart[2] = 0xf0;
        pchMessageStart[3] = 0xa1;
        vAlertPubKey = ParseHex("04536744d5a57623b6602d8325383a580b1c00076a9e68fd75ff1f07240fb7408916e919cec8b8750a6e6aff08339db104106a75f976d5817490b9a1bdc2e0c727");
        nDefaultPort = 14200;
        //nPruneAfterHeight = 100;

        genesis = CreateGenesisBlock(1637347145, 467930, 0x1e0ffff0, 1, 88 * COIN);


        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        improvedrewardsConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0x405d1f6dda6196fc4fc4f2d28a8a199a6206149556cc30ddfaa0a26c04c6c9c2"));
        assert(genesis.hashMerkleRoot == uint256S("0xd80699e741a6ad2478044ad7f71642f6263b0b3d9c0af2c531ca79c7f5648fec"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        //vSeeds.push_back(CDNSSeedData("multidoge.org", "seed.multidoge.org", true));
        //vSeeds.push_back(CDNSSeedData("multidoge.org", "seed2.multidoge.org"));
		vSeeds.push_back(CDNSSeedData("seed.bunkercoin.xyz", "seed.bunkercoin.xyz"));
		vSeeds.push_back(CDNSSeedData("seed.bunkerseed.tk", "seed.bunkerseed.tk"));
		vSeeds.push_back(CDNSSeedData("84.87.9.249", "84.87.9.249"));
		
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,22);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,158);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xc4)(0x2e).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xe1)(0xf4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
			(     0, uint256S("0x405d1f6dda6196fc4fc4f2d28a8a199a6206149556cc30ddfaa0a26c04c6c9c2"))
			(     1, uint256S("0x9cbb556c9abae1eff31e1b25cd1948f63955e35f7e10d30704d245518773c3ea"))
			(     15, uint256S("0x65e33ff135539d9458462cf0c7c49c2cecc7438c080213dc0c4f578161139e32"))
			(     30, uint256S("0x8dea692c06c81cb5365f1dc7038618475d380ff38ba631a899d1dea9316ad5f9"))
			(     50, uint256S("0x61881dacc6ffce7427cf033fc33dcc8cd13372d3517219cf5e16f2419c5d6c11"))
			(     115, uint256S("0x945a2e3a0afcb3737fea470a4b8702830e145f7cd45205a52441825a532f9a11"))
			(     60000, uint256S("0xd17d161d15ab4cd982bc3789cf44c257cadd2aa9975e8ea0fd599c098c27dc8a"))
			(     80000, uint256S("0x38c5277d693b59030a73ec375cccc2769b7a21fd31d8d85fbbe9f9cec82edf94"))
			(     100000, uint256S("0xffcd62ddf782a04e1f9b51c537b5e080ee0f99030803465eecaeb653c41c01fb"))
            (     239930, uint256S("0x58b8b23586a2e05f0c62b731cc5bb5f30754979a550c097220142278ed3c39a1"))
            (     239961, uint256S("0x7fd21781aa2d1e4d1887790f9c60e6350b95ed1aafaccbd767d40e52de0d89db"))
            (     270000, uint256S("0x6504b341c2f47fefffa448facb4131627f256e3ac0105ff3a17fd1684a752b6e"))
            (     300000, uint256S("0xe4d3262d8b29cb1892cfe1af0b5bd049df8c554a11188cf4555bda35e76860af"))
            (     350000, uint256S("0xa40bbfaf23ba6f43573b754b5446eb7facd84bfcd80966d8138c78baccd914a9"))
            (     400000, uint256S("0x2ffd920cc8ec3d618ce2592f0a8ce474464b818770aa2cca048eaa89818650a6"))
            (     450000, uint256S("0xd16725fc99e329edae781c0a8665e0c2f6bdc9a6275692efdb2e04aa7e287288"))
            (     464413, uint256S("0xb4ace6d2ed4368ee566028ff6faf884aef8e45bb0bd48ab9ad6261b34a0da151"))
        };

        chainTxData = ChainTxData{
            // Data as of block 0xffcd62ddf782a04e1f9b51c537b5e080ee0f99030803465eecaeb653c41c01fb (height 100000).
            // Tx estimate based on average of year 2022 (~40k transactions per day)
            1668211686, // * UNIX timestamp of last checkpoint block
            477579,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.04        // * estimated number of transactions per second after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
    Consensus::Params minDifficultyConsensus;
    Consensus::Params improvedrewardsConsensus;
public:
    CTestNetParams() {
        strNetworkID = "test";

        // Blocks 0 - 10 are conventional difficulty calculation
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 1;
        consensus.nMajorityRejectBlockOutdated = 2;
        consensus.nMajorityWindow = 100;
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.nCoinbaseMaturity = 30;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 95; // 95% of 10,080
        consensus.nMinerConfirmationWindow = 108; // 60 * 24 * 7 = 10,080 blocks, or one week
        consensus.fAllowLegacyBlocks = true;

		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        // XXX: BIP heights and hashes all need to be updated to Bunkercoin values
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // Dec 20 2021
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1970000000; // Dec 02 2022

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0; // Mar 11 2022
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1970000000; // Disabled



		//BIP are deployed gradually at 50000 blocks 
        consensus.BIP34Height = 110;
        consensus.BIP65Height = 120;
        consensus.BIP66Height = 130;
		
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

 

        // Blocks 10 - 100000 are Digishield without AuxPoW
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 10;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.nPowTargetTimespan = 60; // post-digishield: 1 minute
        digishieldConsensus.nCoinbaseMaturity = 30;
	    consensus.fAllowLegacyBlocks = true;
        consensus.fSimplifiedRewards = false;

        // Blocks 100000+ are AuxPoW or PoW
        auxpowConsensus = digishieldConsensus;
		auxpowConsensus.nAuxpowChainId = 0x0031; // 66 - B
        auxpowConsensus.nHeightEffective = 20;
        auxpowConsensus.fAllowLegacyBlocks = false;
        auxpowConsensus.fSimplifiedRewards = false;

        // Blocks 169+ on testnet are AuxPoW with improved rewards.
        improvedrewardsConsensus = auxpowConsensus;
        improvedrewardsConsensus.fSimplifiedRewards = true;
        improvedrewardsConsensus.nHeightEffective = 169;

        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;
        auxpowConsensus.pRight = &improvedrewardsConsensus;

        pchMessageStart[0] = 0xf2;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb9;
        pchMessageStart[3] = 0xd7;
        vAlertPubKey = ParseHex("04536744d5a57623b6602d8325383a580b1c00076a9e68fd75ff1f07240fb7408916e919cec8b8750a6e6aff08339db104106a75f976d5817490b9a1bdc2e0c727");
        nDefaultPort = 28200;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1651427697, 707474, 0x1e0ffff0, 1, 88 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        minDifficultyConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        improvedrewardsConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0xf9bbe878f5d839abc89cddd05e9716070542bf4c13cd46fb275b494d76a17161"));
        assert(genesis.hashMerkleRoot == uint256S("0xd80699e741a6ad2478044ad7f71642f6263b0b3d9c0af2c531ca79c7f5648fec"));

        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        //vSeeds.push_back(CDNSSeedData("jrn.me.uk", "testseed.jrn.me.uk"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,85); // 0x71
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196); // 0xc4
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,110); // 0xf1
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0xD1)(0xDF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x75)(0xA4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = true;
		
        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0xf9bbe878f5d839abc89cddd05e9716070542bf4c13cd46fb275b494d76a17161"))
            ( 1, uint256S("0x52ea6009b51622d311d09d9ab1469badc4e5177478b5011a4c017dc3dd025cc5"))
            ( 5, uint256S("0x32353b4f278dca8350660bd1fea21e0f0a94565d35a176e8eb54a6a4c9eaf888"))
            ( 10, uint256S("0x79bdeefbb3f4b90d8aaf69f021b3314f2bf5bcdb9a0d346d1b80cd2be86d60a4"))
            ( 15, uint256S("0xf9a146f4284d458b8ecd7d358c6a23dfe91f2bbed90436bcb850a8082e3c4dc9"))
            ( 20, uint256S("0x19e6261875990052e3db47342aeeb60e45e36d40a7d4cf915c02f0ef9b10b80f"))
            ( 25, uint256S("0x68d87209a76e91a7fb64ca49a2b2b2a6e6121d62a0ba6ab2d3acd293afa4f063"))
            ( 26, uint256S("0xacefbe5f29292d053221539933d3dfdb98754803b6f2c699cd83656b71ed42b3"))
        };
		
        chainTxData = ChainTxData{
            // Data as of block 07fef07a255d510297c9189dc96da5f4e41a8184bc979df8294487f07fee1cf3 (height 3286675)
            1659702317, // * UNIX timestamp of last checkpoint block
            27,    // * total number of transactions between genesis and last checkpoint
            0.01       // * estimated number of transactions per second after that timestamp
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
    Consensus::Params improvedrewardsConsensus;
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 3;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 1; // regtest: 1 second blocks
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 540; // 75% for testchains
        consensus.nMinerConfirmationWindow = 720; // Faster than normal for regtest (2,520 instead of 10,080)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // AuxPow parameters
        consensus.nAuxpowChainId = 0x0062; // 98 - Josh Wise!
        consensus.fStrictChainId = true;
        consensus.fAllowLegacyBlocks = true;

        // Dogecoin parameters
        consensus.fSimplifiedRewards = false; // Testing for the hardfork
        consensus.nCoinbaseMaturity = 60; // For easier testability in RPC tests

        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 1;
        digishieldConsensus.nPowTargetTimespan = 1; // regtest: also retarget every second in digishield mode, for conformity
        digishieldConsensus.fDigishieldDifficultyCalculation = true;

        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.fAllowLegacyBlocks = false;
        auxpowConsensus.nHeightEffective = 3;

        // Blocks 169+ on testnet are AuxPoW with improved rewards.
        improvedrewardsConsensus = auxpowConsensus;
        improvedrewardsConsensus.fSimplifiedRewards = true;
        improvedrewardsConsensus.nHeightEffective = 6;

        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;
        auxpowConsensus.pRight = &improvedrewardsConsensus;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1637347268, 2, 0x207fffff, 1, 88 * COIN); //hash e08a885e4191632633276ee13f8627849340187fc267610038aa9fe478bc7fab
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        // This fucker. This was why the older code never worked.
        improvedrewardsConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
		
		assert(consensus.hashGenesisBlock == uint256S("0xe08a885e4191632633276ee13f8627849340187fc267610038aa9fe478bc7fab"));
        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.


        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
		
		
        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0xe08a885e4191632633276ee13f8627849340187fc267610038aa9fe478bc7fab"))
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);  // 0x6f
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);  // 0xc4
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);  // 0xef
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

const Consensus::Params *Consensus::Params::GetConsensus(uint32_t nTargetHeight) const {
    if (nTargetHeight < this -> nHeightEffective && this -> pLeft != NULL) {
        return this -> pLeft -> GetConsensus(nTargetHeight);
    } else if (nTargetHeight > this -> nHeightEffective && this -> pRight != NULL) {
        const Consensus::Params *pCandidate = this -> pRight -> GetConsensus(nTargetHeight);
        if (pCandidate->nHeightEffective <= nTargetHeight) {
            return pCandidate;
        }
    }

    // No better match below the target height
    return this;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}