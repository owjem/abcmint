// Copyright (c) 2018 The Bitcoin developers

#ifndef BITCOIN_MINER_H
#define BITCOIN_MINER_H


#include "miner/idxlut.h"
#include "main.h"



extern uint64 nLastBlockTx;
extern uint64 nLastBlockSize;

int64 GetBlockValue(int nHeight, int64 nFees);

/** Calculate the minimum amount of work a received block needs, without knowing its direct parent */
// unsigned int ComputeMinWork(unsigned int nBase, int64 nTime);

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock);

uint256 SerchSolution(uint256 hash, unsigned int nBits, uint256 randomNonce, CBlockIndex* pindexPrev, int deviceID, int deviceCount);

// bool CheckSolution(uint256 hash, unsigned int nBits, uint256 preblockhash,int nblockversion, uint256 nNonce) ;

/** Check whether a block hash satisfies the proof-of-work requirement specified by nBits */
// bool CheckProofOfWork(uint256 hash, unsigned int nBits, uint256 preblockhash, int nblockversion, uint256 nNonce);

/** Modify the extranonce in a block */
void IncrementExtraNonce(CBlock* pblock, CBlockIndex* pindexPrev, unsigned int& nExtraNonce);

/** Check mined block */
bool CheckWork(CBlock* pblock, CWallet& wallet, CReserveKey& reservekey);

/** Generate a new block, without valid proof-of-work */
CBlockTemplate* CreateNewBlock(const CScript& scriptPubKeyIn);
CBlockTemplate* CreateNewBlockWithKey(CReserveKey& reservekey);

/** Do mining precalculation */
void FormatHashBuffers(CBlock* pblock, char* pmidstate, char* pdata, char* phash1);

/** Run the miner threads */
void GenerateBitcoins(bool fGenerate, CWallet* pwallet);

extern double dHashesPerSec;
extern int64 nHPSTimerStart;
#endif
