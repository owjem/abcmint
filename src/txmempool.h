// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_TXMEMPOOL_H
#define BITCOIN_TXMEMPOOL_H

#include "coins.h"
#include "core.h"
#include "sync.h"

/** Fake height value used in CCoins to signify they are only in the memory pool (since 0.8) */
static const unsigned int MEMPOOL_HEIGHT = 0x7FFFFFFF;

/*
 * CTxMemPool stores valid-according-to-the-current-best-chain
 * transactions that may be included in the next block.
 *
 * Transactions are added when they are seen on the network
 * (or created by the local node), but not all transactions seen
 * are added to the pool: if a new transaction double-spends
 * an input of a transaction in the pool, it is dropped,
 * as are non-standard transactions.
 */
class CTxMemPool
{
private:
    bool fSanityCheck; // Normally false, true if -checkmempool or -regtest
    unsigned int nTransactionsUpdated;

public:
    mutable CCriticalSection cs;
    std::map<uint256, CTransaction> mapTx;
    std::map<COutPoint, CInPoint> mapNextTx;

    CTxMemPool();

    /*
     * If sanity-checking is turned on, check makes sure the pool is
     * consistent (does not contain two transactions that spend the same inputs,
     * all inputs are in the mapNextTx array). If sanity-checking is turned off,
     * check does nothing.
     */
    void check(CCoinsViewCache *pcoins) const;
    void setSanityCheck(bool _fSanityCheck) { fSanityCheck = _fSanityCheck; }

    bool addUnchecked(const uint256& hash, const CTransaction &tx);
    bool remove(const CTransaction &tx, bool fRecursive = false);
    bool removeConflicts(const CTransaction &tx);
    void clear();
    void queryHashes(std::vector<uint256>& vtxid);
    void pruneSpent(const uint256& hash, CCoins &coins);
    unsigned int GetTransactionsUpdated() const;
    void AddTransactionsUpdated(unsigned int n);

    unsigned long size()
    {
        LOCK(cs);
        return mapTx.size();
    }

    bool exists(uint256 hash)
    {
        LOCK(cs);
        return (mapTx.count(hash) != 0);
    }

    bool lookup(uint256 hash, CTransaction& result) const;
};

/** CCoinsView that brings transactions from a memorypool into view.
    It does not check for spendings by memory pool transactions. */
class CCoinsViewMemPool : public CCoinsViewBacked
{
protected:
    CTxMemPool &mempool;

public:
    CCoinsViewMemPool(CCoinsView &baseIn, CTxMemPool &mempoolIn);
    bool GetCoins(const uint256 &txid, CCoins &coins);
    bool HaveCoins(const uint256 &txid);
};

#endif /* BITCOIN_TXMEMPOOL_H */
