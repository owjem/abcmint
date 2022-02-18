// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_MINER_H
#define BITCOIN_MINER_H

#include "miner/idxlut.h"
#include "uint256.h"
#include <stdint.h>

class CBlock;
class CBlockIndex;
struct CBlockTemplate;
class CReserveKey;
class CScript;
class CWallet;


extern uint64_t nLastBlockTx;
extern uint64_t nLastBlockSize;

// int64_t GetBlockValue(int nHeight, int64_t nFees);

/** Calculate the minimum amount of work a received block needs, without knowing its direct parent */
// unsigned int ComputeMinWork(unsigned int nBase, int64_t nTime);

uint256 SerchSolution(uint256 hash, unsigned int nBits, uint256 randomNonce, CBlockIndex* pindexPrev, int deviceID, int deviceCount);

// bool CheckSolution(uint256 hash, unsigned int nBits, uint256 preblockhash,int nblockversion, uint256 nNonce) ;

/** Check whether a block hash satisfies the proof-of-work requirement specified by nBits */
// bool CheckProofOfWork(uint256 hash, unsigned int nBits, uint256 preblockhash, int nblockversion, uint256 nNonce);



/** Run the miner threads */
void GenerateBitcoins(bool fGenerate, CWallet* pwallet, int nThreads);
/** Generate a new block, without valid proof-of-work */
CBlockTemplate* CreateNewBlock(const CScript& scriptPubKeyIn);
CBlockTemplate* CreateNewBlockWithKey(CReserveKey& reservekey);
/** Modify the extranonce in a block */
void IncrementExtraNonce(CBlock* pblock, CBlockIndex* pindexPrev, unsigned int& nExtraNonce);
/** Do mining precalculation */
void FormatHashBuffers(CBlock* pblock, char* pmidstate, char* pdata, char* phash1);
/** Check mined block */
bool CheckWork(CBlock* pblock, CWallet& wallet, CReserveKey& reservekey);

extern double dHashesPerSec;
extern int64_t nHPSTimerStart;

#endif // BITCOIN_MINER_H
