// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "assert.h"
#include "core.h"
#include "protocol.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

//
// Main network
//

unsigned int pnSeed[] =
{
    0xD3471A78,0x60B66827, 0x02316827, 0xC3326827
};

// SeedSpec6 pnSeed6_main[] = {
//     {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x78,0x1A,0x47,0xD3}, 8888},
//     {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x27,0x68,0xB6,0x60}, 8888},
//     {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x27,0x68,0x31,0x02}, 8888},
//     {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x27,0x68,0x32,0xC3}, 8888}
// };

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xc1;
        pchMessageStart[1] = 0xf7;
        pchMessageStart[2] = 0xfb;
        pchMessageStart[3] = 0xfd;
        vAlertPubKey = ParseHex("04fc9702847840aaf195de8442ebecedf5b095cdbb9bc716bda9110971b28a49e0ead8564ff0db22209e0374782c093bb899692d524e9d6a6956e7c5ecbcd68284");
        nDefaultPort = 8888;
        nRPCPort = 8889;
        bnProofOfWorkLimit = 256;
        bnProofOfWorkLimitMin = 41;
        nSubsidyHalvingInterval = 210000;

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //
        // CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
        //   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
        //     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
        //     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
        //   vMerkleTree: 4a5e1e
        const char* pszTimestamp = "Surely you're joking, Mr.Feynman!";
        CTransaction txNew;
        txNew.vin.resize(1);
		txNew.vin[0].prevout.SetNull();
        txNew.vout.resize(12);
        txNew.vin[0].scriptSig = CScript()<< vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));

        txNew.vout[0].nValue = 20000000*COIN;
		txNew.vout[0].scriptPubKey.SetDestination(CKeyID("e6b765cf4efeb5519bcb47ca35738355588e44284398d58c4ac33116ab7e8cef"));
		txNew.vout[1].nValue = 20000000*COIN ;
		txNew.vout[1].scriptPubKey.SetDestination(CKeyID("ebbb5a58e2914878d6f6670a53bdd5e6d0210a3d0b3fa8656bfeb8dd61fb8b81"));
		txNew.vout[2].nValue = 20000000*COIN ;
		txNew.vout[2].scriptPubKey.SetDestination(CKeyID("76068b3357435c9306fcb2b54547b298b5f19e4819dd62e31f84558219e3bd8e"));
		txNew.vout[3].nValue = 20000000*COIN;
		txNew.vout[3].scriptPubKey.SetDestination(CKeyID("be17335dbc8aeaf6aeb3c4ae62886491dab4a3c38c79a5e86bac95214a49b001"));
		txNew.vout[4].nValue = 20000000*COIN ;
		txNew.vout[4].scriptPubKey.SetDestination(CKeyID("f6b7bee668d7125d8a5502ccf9f5927b8866dc180d20642e368d9a936e287e39"));
		txNew.vout[5].nValue = 20000000*COIN ;
		txNew.vout[5].scriptPubKey.SetDestination(CKeyID("706d4053c3794cc3312a7c09b6dd21dd1da3f8c8e56f23c76554ddbf585bccf4"));
		txNew.vout[6].nValue = 20000000*COIN;
		txNew.vout[6].scriptPubKey.SetDestination(CKeyID("bdb56bbe25f5db3df8a3dbfbd5480f390665603b0bc1f6120d0a36370d4d5416"));
		txNew.vout[7].nValue = 20000000*COIN ;
		txNew.vout[7].scriptPubKey.SetDestination(CKeyID("8603abbecfdcaa59545ba185caf684a75075d4147afba303b2c318ef58f6c33e"));
		txNew.vout[8].nValue = 20000000*COIN ;
		txNew.vout[8].scriptPubKey.SetDestination(CKeyID("ebe47a2e627e8c015534830474268becf1ea37da47c8a426b2336ed72abc3257"));
		txNew.vout[9].nValue = 20000000*COIN;
		txNew.vout[9].scriptPubKey.SetDestination(CKeyID("ec97680cfe5c7f30b6356329d463b2e00ed09c22a37ca8010a9c84e36b70f674"));
		txNew.vout[10].nValue = 7374182.7*COIN ;
		txNew.vout[10].scriptPubKey.SetDestination(CKeyID("210ed7d715c525c5bf6b638c29428c70bc4040f248750d5241371d00d84f22ef"));
		txNew.vout[11].nValue = 7374182*COIN ;
		txNew.vout[11].scriptPubKey.SetDestination(CKeyID("b4cb22b62f91f6a36554bdfce5b860d6905c54217fd4543d8eaf16bb20d6ccf7"));

        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1529291280;
        genesis.nBits    = 41;
        genesis.nNonce   = uint256("0x0000000000000000000000000000000000000000000000000001ee7340a9a1d6");


        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xcea89aa6adb81572f8b9e5f9b5d0184cbbc25208164cb1547decf3655da9dc77"));
        assert(genesis.hashMerkleRoot == uint256("0xaef6a6cb3767fa5d965b10f9d1e3e183ddea21a5f7ffce9bd7a86c065e7c6865"));

        // uint256 hash = uint256("0xcea89aa6adb81572f8b9e5f9b5d0184cbbc25208164cb1547decf3655da9dc77");
        // LogPrintf("%s\n", hash.ToString());
        // LogPrintf("%s\n", hashGenesisBlock.ToString());
        // LogPrintf("%s\n", genesis.hashMerkleRoot.ToString());

        vSeeds.push_back(CDNSSeedData("abcmint-bohr.com", "abcmint-bohr.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-einstein.com", "abcmint-einstein.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-euclid.com", "abcmint-euclid.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-feynman.com", "abcmint-feynman.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-gauss.com", "abcmint-gauss.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-newton.com", "abcmint-newton.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-planck.com", "abcmint-planck.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-turing.com", "abcmint-turing.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        // Convert the pnSeeds array into usable address objects.
        for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++)
        {
            // It'll only connect to one or two seed nodes because once it connects,
            // it'll get a pile of addresses with newer timestamps.
            // Seed nodes are given a random 'last seen time' of between one and two
            // weeks ago.
            const int64_t nOneWeek = 7*24*60*60;
            struct in_addr ip;
            memcpy(&ip, &pnSeed[i], sizeof(ip));
            CAddress addr(CService(ip, GetDefaultPort()));
            addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
            vFixedSeeds.push_back(addr);
        }
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet (v3)
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        vAlertPubKey = ParseHex("04302390343f91cc401d56d68b123028bf52e5fca1939df127f63c6467cdf9c8e2c14b61104cf817d0b780da337893ecc4aaff1309e536162dabbdb45200ca2b0a");
        nDefaultPort = 18333;
        nRPCPort = 18332;
        strDataDir = "testnet3";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1529291280;
        genesis.nNonce = uint256("0x0000000000000000000000000000000000000000000000000001ee7340a9a1d6");
        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("cea89aa6adb81572f8b9e5f9b5d0184cbbc25208164cb1547decf3655da9dc77"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("abcmint-bohr.com", "abcmint-bohr.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-einstein.com", "abcmint-einstein.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-euclid.com", "abcmint-euclid.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-feynman.com", "abcmint-feynman.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-gauss.com", "abcmint-gauss.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-newton.com", "abcmint-newton.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-planck.com", "abcmint-planck.com"));
        vSeeds.push_back(CDNSSeedData("abcmint-turing.com", "abcmint-turing.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nSubsidyHalvingInterval = 150;
        bnProofOfWorkLimit = 256;
        bnProofOfWorkLimitMin = 41;
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        strDataDir = "regtest";
        assert(hashGenesisBlock == uint256("0444e67245baf5a1461d799910d353d86776fd4896beee622db01b72efd04a37"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
