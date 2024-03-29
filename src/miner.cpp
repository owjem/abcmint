// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "miner.h"

#include "core.h"
#include "main.h"
#include "net.h"
#ifdef ENABLE_WALLET
#include "wallet.h"
#endif
#include "miner/common.h"

//////////////////////////////////////////////////////////////////////////////
//
// BitcoinMiner
//

#ifdef USE_GPU
#include <cuda.h>
#include "miner/gpumining.h"

static uint256 GPUMinerSearchSolution(uint256 hash, unsigned int nBits, uint256 randomNonce, CBlockIndex* pindexPrev, int deviceID, int deviceCount, uint64_t& solm, char* threadname)
{

    LogPrintf("[%s] =============> hash: %s diff: %d, randomNonce: %s, height: %d, deviceID: %d, deviceCount: %d, solm: %ld \n", threadname, hash.ToString(), nBits, randomNonce.ToString(), pindexPrev->nHeight , deviceID, deviceCount, solm);

    unsigned int mEquations = nBits;
    unsigned int nUnknowns = nBits + 8;
    unsigned int nTerms = 1 + (nUnknowns + 1) * (nUnknowns) / 2;
    std::vector<uint8_t> coeffMatrix;
    coeffMatrix.resize(mEquations * nTerms);
    if (pindexPrev->nHeight < 25216) {
        GenCoeffMatrix(hash, nBits, coeffMatrix);
    } else {
        NewGenCoeffMatrix(hash, nBits, coeffMatrix);
    }
    uint8_t maskArray[nUnknowns];
    Uint256ToSolutionBits(maskArray, nUnknowns, randomNonce);
    uint8_t*** Eqs = CreateEquationsUint8(nUnknowns, mEquations);
    uint64_t startPoint[4];
    for (int width = 0; width < 4; width++) {
        startPoint[width] = randomNonce.Get64(width);
    }
    TransformTo3D(nUnknowns, mEquations, coeffMatrix, Eqs);
    for (int i = 0; i < mEquations; i++) {
        for (int j = 0; j < nUnknowns; j++)
            Eqs[i][0][0] ^= Eqs[i][1][j] & TM(startPoint, j);
        int offset = 0;
        for (int j = 0; j < nUnknowns - 1; j++)
            for (int k = j + 1; k < nUnknowns; k++) {
                Eqs[i][0][0] ^= Eqs[i][2][offset] & TM(startPoint, j) & TM(startPoint, k);
                Eqs[i][1][j] ^= Eqs[i][2][offset] & TM(startPoint, k);
                Eqs[i][1][k] ^= Eqs[i][2][offset] & TM(startPoint, j);
                offset += 1;
            }
    }
    uint8_t*** EqsCopy = CreateEquationsUint8(nUnknowns, mEquations);
    int nFix = 8;
    if (nBits <= 40) {
        nFix = 8;
    } else {
        nFix = nUnknowns - 40;
    }
    uint64_t balance = (uint64_t)ceil((uint64_t)(1 << nFix) / (uint64_t)deviceCount);
    uint64_t upBound = std::min((uint64_t)(deviceID + 1) * balance, (uint64_t)(1 << nFix));
    uint64_t downBound = deviceID * balance;

    LogPrintf("[%s] =============> balance: %ld, upBound: %ld, downBound: %ld  \n", threadname, balance, upBound, downBound);

    int p = nUnknowns - nFix;
    int npartial;
    uint8_t fixvalue;
    uint256 nonce = 0;
    uint8_t w[nUnknowns];
    // uint64_t solm = 0;
    solm = 0;
    unsigned int newNumVariables = nUnknowns - nFix;
    unsigned int newNumEquations = mEquations;
    unsigned int newNumTerms = 1 + (newNumVariables + 1) * (newNumVariables) / 2;
    uint64_t foundSolution = 0;
    uint32_t* coefficients = (uint32_t*)malloc((mEquations * (newNumTerms + newNumVariables)) * sizeof(uint32_t));
    if (coefficients == NULL){
        throw boost::thread_interrupted();
        LogPrintf("ERROR: SearchSolution malloc failure!");
    }

    LogPrintf("[%s] =============> newNumVariables: %d, newNumEquations: %d, newNumTerms: %d  \n", threadname, newNumVariables, newNumEquations, newNumTerms);
    // int searchTimes = 0;
    for (solm = downBound; solm < (uint64_t)upBound; solm++) {
        if (pindexPrev != chainActive.Tip())
            break;
        npartial = nUnknowns;
        for (int i = 0; i < mEquations; i++)
            for (int j = 0; j < 3; j++)
                for (int k = 0; k < Binomial(nUnknowns, j); k++)
                    EqsCopy[i][j][k] = Eqs[i][j][k];
        while (npartial != p) {
            fixvalue = (uint8_t)((solm >> (nUnknowns - npartial)) & 1);
            for (int i = 0; i < mEquations; i++) {
                for (int j = 0; j < npartial - 1; j++)
                    EqsCopy[i][1][j + 1] ^= EqsCopy[i][2][j] & fixvalue;
                EqsCopy[i][0][0] ^= EqsCopy[i][1][0] & fixvalue;
                for (int j = 0; j < npartial - 1; j++)
                    EqsCopy[i][1][j] = EqsCopy[i][1][j + 1];
                for (int j = 0; j < Binomial(npartial - 1, 2); j++)
                    EqsCopy[i][2][j] = EqsCopy[i][2][j + npartial - 1];
            }
            npartial -= 1;
        }
        std::vector<uint8_t> dest;
        dest.resize(mEquations * newNumTerms);
        TransTo1D(newNumVariables, newNumEquations, dest, EqsCopy);
        std::vector<uint32_t> withQudraticTerms;
        withQudraticTerms.resize(mEquations * (newNumTerms + newNumVariables));
        AddQuadraticTerms(dest, newNumVariables, mEquations, withQudraticTerms);
        LexToGradeReverseLex(withQudraticTerms, newNumVariables, mEquations, coefficients);
        foundSolution = GPUSearchSolution(coefficients, newNumVariables, mEquations);
        if(foundSolution==0) continue;

        LogPrintf("[%s] =============> solm: %ld \n", threadname, solm);
        LogPrintf("[%s] =============> foundSolution: %ld \n", threadname, foundSolution);
        nonce = uint256(foundSolution);
        LogPrintf("[%s] =============> nonce: %s\n", threadname, nonce.ToString());
        uint256 fixSolution = nonce;
        uint8_t x[newNumVariables];
        Uint256ToSolutionBits(x, newNumVariables, fixSolution);
        uint8_t z[nFix];
        uint64_t fixv[1];
        fixv[0] = solm;
        for (unsigned int i = 0; i < nFix; i++) {
            z[i] = TM(fixv, i);
        }
        for (unsigned int i = 0; i < nUnknowns; i++) {
            if (i < nFix) {
                w[i] = z[i] ^ maskArray[i];
            } else {
                w[i] = x[i - nFix] ^ maskArray[i];
            }
        }
        nonce = 0;
        SolutionBitsToUint256(w, nUnknowns, nonce);
        uint256 prevblockhash = 0;
        if (pindexPrev->GetBlockHash() == Params().HashGenesisBlock() || pindexPrev->pprev->GetBlockHash() == Params().HashGenesisBlock()) {
            prevblockhash = 0;
        } else {
            prevblockhash = pindexPrev->GetBlockHash();
        }
        if (CheckSolution(hash, nBits, prevblockhash, pindexPrev->nVersion, nonce))
            break;
    }
    FreeEquationsUint8(mEquations, EqsCopy);
    FreeEquationsUint8(mEquations, Eqs);
    free(coefficients);
    return nonce;
}
#else
#include "miner/solution.h"

uint256 SerchSolution(uint256 hash, unsigned int nBits, uint256 randomNonce, CBlockIndex* pindexPrev)
{
    uint256 nonce = 0;

    unsigned int mEquations = nBits;
    unsigned int nUnknowns = nBits + 8;
    unsigned int nTerms = 1 + (nUnknowns + 1) * (nUnknowns) / 2;
    std::vector<uint8_t> coeffMatrix;
    coeffMatrix.resize(mEquations * nTerms);
    if (pindexPrev->nHeight < 25216) {
        GenCoeffMatrix(hash, nBits, coeffMatrix);
    } else {
        NewGenCoeffMatrix(hash, nBits, coeffMatrix);
    }
    int*** Eqs = CreateEquations(nUnknowns, mEquations);
    uint64_t maxsol = 1; // The solver only returns maxsol solutions. Other solutions will be discarded.
    uint64_t** SolArray = CreateArray(maxsol); // Set all array elements to zero.
    // serch
    uint64_t startPoint[4];
    for (int width = 0; width < 4; width++) {
        startPoint[width] = randomNonce.Get64(width);
    }
    int nSearchVariables = 0;
    if (nUnknowns > 62) {
        nSearchVariables = nUnknowns - 62;
    } else {
        nSearchVariables = 0;
    }
    TransformDataStructure(nUnknowns, mEquations, coeffMatrix, Eqs); // Transform coefficientsMatrix into the structure required by exfes.
    exfes(nSearchVariables, nUnknowns, mEquations, startPoint, maxsol, Eqs, SolArray, pindexPrev); // Solve equations by exfes.
    ReportSolution(maxsol, SolArray, nonce); // Report obtained solutions in uint256 format.
    FreeEquations(mEquations, Eqs);
    FreeArray(maxsol, SolArray);

    return nonce;
}

#endif


int static FormatHashBlocks(void* pbuffer, unsigned int len)
{
    unsigned char* pdata = (unsigned char*)pbuffer;
    unsigned int blocks = 1 + ((len + 8) / 64);
    unsigned char* pend = pdata + 64 * blocks;
    memset(pdata + len, 0, 64 * blocks - len);
    pdata[len] = 0x80;
    unsigned int bits = len * 8;
    pend[-1] = (bits >> 0) & 0xff;
    pend[-2] = (bits >> 8) & 0xff;
    pend[-3] = (bits >> 16) & 0xff;
    pend[-4] = (bits >> 24) & 0xff;
    return blocks;
}

static const unsigned int pSHA256InitState[8] =
{0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

void SHA256Transform(void* pstate, void* pinput, const void* pinit)
{
    SHA256_CTX ctx;
    unsigned char data[64];

    SHA256_Init(&ctx);

    for (int i = 0; i < 16; i++)
        ((uint32_t*)data)[i] = ByteReverse(((uint32_t*)pinput)[i]);

    for (int i = 0; i < 8; i++)
        ctx.h[i] = ((uint32_t*)pinit)[i];

    SHA256_Update(&ctx, data, sizeof(data));
    for (int i = 0; i < 8; i++)
        ((uint32_t*)pstate)[i] = ctx.h[i];
}

// Some explaining would be appreciated
class COrphan
{
public:
    const CTransaction* ptx;
    set<uint256> setDependsOn;
    double dPriority;
    double dFeePerKb;

    COrphan(const CTransaction* ptxIn)
    {
        ptx = ptxIn;
        dPriority = dFeePerKb = 0;
    }

    void print() const
    {
        LogPrintf("COrphan(hash=%s, dPriority=%.1f, dFeePerKb=%.1f)\n",
               ptx->GetHash().ToString(), dPriority, dFeePerKb);
        BOOST_FOREACH(uint256 hash, setDependsOn)
            LogPrintf("   setDependsOn %s\n", hash.ToString());
    }
};


uint64_t nLastBlockTx = 0;
uint64_t nLastBlockSize = 0;

// We want to sort transactions by priority and fee, so:
typedef boost::tuple<double, double, const CTransaction*> TxPriority;
class TxPriorityCompare
{
    bool byFee;
public:
    TxPriorityCompare(bool _byFee) : byFee(_byFee) { }
    bool operator()(const TxPriority& a, const TxPriority& b)
    {
        if (byFee)
        {
            if (a.get<1>() == b.get<1>())
                return a.get<0>() < b.get<0>();
            return a.get<1>() < b.get<1>();
        }
        else
        {
            if (a.get<0>() == b.get<0>())
                return a.get<1>() < b.get<1>();
            return a.get<0>() < b.get<0>();
        }
    }
};

CBlockTemplate* CreateNewBlock(const CScript& scriptPubKeyIn)
{
    // Create new block
    auto_ptr<CBlockTemplate> pblocktemplate(new CBlockTemplate());
    if(!pblocktemplate.get())
        return NULL;
    CBlock *pblock = &pblocktemplate->block; // pointer for convenience

    // Create coinbase tx
    CTransaction txNew;
    txNew.vin.resize(1);
    txNew.vin[0].prevout.SetNull();
    txNew.vout.resize(1);
    txNew.vout[0].scriptPubKey = scriptPubKeyIn;

    // Add our coinbase tx as first transaction
    pblock->vtx.push_back(txNew);
    pblocktemplate->vTxFees.push_back(-1); // updated at end
    pblocktemplate->vTxSigOps.push_back(-1); // updated at end

    // Largest block you're willing to create:
    unsigned int nBlockMaxSize = GetArg("-blockmaxsize", DEFAULT_BLOCK_MAX_SIZE);
    // Limit to betweeen 2000K and MAX_BLOCK_SIZE-2K for sanity:
    nBlockMaxSize = std::max((unsigned int)2000*1024, std::min((unsigned int)(MAX_BLOCK_SIZE-200*1024), nBlockMaxSize));

    // How much of the block should be dedicated to high-priority transactions,
    // included regardless of the fees they pay
    unsigned int nBlockPrioritySize = GetArg("-blockprioritysize", DEFAULT_BLOCK_PRIORITY_SIZE);
    nBlockPrioritySize = std::min(nBlockMaxSize, nBlockPrioritySize);

    // Minimum block size you want to create; block will be filled with free transactions
    // until there are no more or the block reaches this size:
    unsigned int nBlockMinSize = GetArg("-blockminsize", DEFAULT_BLOCK_MIN_SIZE);
    nBlockMinSize = std::min(nBlockMaxSize, nBlockMinSize);

    // Collect memory pool transactions into the block
    int64_t nFees = 0;
    {
        LOCK2(cs_main, mempool.cs);
        CBlockIndex* pindexPrev = chainActive.Tip();
        CCoinsViewCache view(*pcoinsTip, true);

        // Priority order to process transactions
        list<COrphan> vOrphan; // list memory doesn't move
        map<uint256, vector<COrphan*> > mapDependers;
        bool fPrintPriority = GetBoolArg("-printpriority", false);

        // This vector will be sorted into a priority queue:
        vector<TxPriority> vecPriority;
        vecPriority.reserve(mempool.mapTx.size());
        for (map<uint256, CTxMemPoolEntry>::iterator mi = mempool.mapTx.begin();
             mi != mempool.mapTx.end(); ++mi)
        {
            const CTransaction& tx = mi->second.GetTx();
            if (tx.IsCoinBase() || !IsFinalTx(tx, pindexPrev->nHeight + 1))
                continue;

            COrphan* porphan = NULL;
            double dPriority = 0;
            int64_t nTotalIn = 0;
            bool fMissingInputs = false;
            BOOST_FOREACH(const CTxIn& txin, tx.vin)
            {
                // Read prev transaction
                if (!view.HaveCoins(txin.prevout.hash))
                {
                    // This should never happen; all transactions in the memory
                    // pool should connect to either transactions in the chain
                    // or other transactions in the memory pool.
                    if (!mempool.mapTx.count(txin.prevout.hash))
                    {
                        LogPrintf("ERROR: mempool transaction missing input\n");
                        if (fDebug) assert("mempool transaction missing input" == 0);
                        fMissingInputs = true;
                        if (porphan)
                            vOrphan.pop_back();
                        break;
                    }

                    // Has to wait for dependencies
                    if (!porphan)
                    {
                        // Use list for automatic deletion
                        vOrphan.push_back(COrphan(&tx));
                        porphan = &vOrphan.back();
                    }
                    mapDependers[txin.prevout.hash].push_back(porphan);
                    porphan->setDependsOn.insert(txin.prevout.hash);
                    nTotalIn += mempool.mapTx[txin.prevout.hash].GetTx().vout[txin.prevout.n].nValue;
                    continue;
                }
                const CCoins &coins = view.GetCoins(txin.prevout.hash);

                int64_t nValueIn = coins.vout[txin.prevout.n].nValue;
                nTotalIn += nValueIn;

                int nConf = pindexPrev->nHeight - coins.nHeight + 1;

                dPriority += (double)nValueIn * nConf;
            }
            if (fMissingInputs) continue;

            // Priority is sum(valuein * age) / modified_txsize
            unsigned int nTxSize = ::GetSerializeSize(tx, SER_NETWORK, PROTOCOL_VERSION);
            dPriority = tx.ComputePriority(dPriority, nTxSize);

            // This is a more accurate fee-per-kilobyte than is used by the client code, because the
            // client code rounds up the size to the nearest 1K. That's good, because it gives an
            // incentive to create smaller transactions.
            double dFeePerKb =  double(nTotalIn-tx.GetValueOut()) / (double(nTxSize)/1000.0);

            if (porphan)
            {
                porphan->dPriority = dPriority;
                porphan->dFeePerKb = dFeePerKb;
            }
            else
                vecPriority.push_back(TxPriority(dPriority, dFeePerKb, &mi->second.GetTx()));
        }

        // Collect transactions into block
        uint64_t nBlockSize = 1000;
        uint64_t nBlockTx = 0;
        int nBlockSigOps = 100;
        bool fSortedByFee = (nBlockPrioritySize <= 0);

        TxPriorityCompare comparer(fSortedByFee);
        std::make_heap(vecPriority.begin(), vecPriority.end(), comparer);

        while (!vecPriority.empty())
        {
            // Take highest priority transaction off the priority queue:
            double dPriority = vecPriority.front().get<0>();
            double dFeePerKb = vecPriority.front().get<1>();
            const CTransaction& tx = *(vecPriority.front().get<2>());

            std::pop_heap(vecPriority.begin(), vecPriority.end(), comparer);
            vecPriority.pop_back();

            // Size limits
            unsigned int nTxSize = ::GetSerializeSize(tx, SER_NETWORK, PROTOCOL_VERSION);
            if (nBlockSize + nTxSize >= nBlockMaxSize)
                continue;

            // Legacy limits on sigOps:
            unsigned int nTxSigOps = GetLegacySigOpCount(tx);
            if (nBlockSigOps + nTxSigOps >= MAX_BLOCK_SIGOPS)
                continue;

            // Skip free transactions if we're past the minimum block size:
            if (fSortedByFee && (dFeePerKb < CTransaction::nMinRelayTxFee) && (nBlockSize + nTxSize >= nBlockMinSize))
                continue;

            // Prioritize by fee once past the priority size or we run out of high-priority
            // transactions:
            if (!fSortedByFee &&
                ((nBlockSize + nTxSize >= nBlockPrioritySize) || !AllowFree(dPriority)))
            {
                fSortedByFee = true;
                comparer = TxPriorityCompare(fSortedByFee);
                std::make_heap(vecPriority.begin(), vecPriority.end(), comparer);
            }

            if (!view.HaveInputs(tx))
                continue;

            int64_t nTxFees = view.GetValueIn(tx)-tx.GetValueOut();

            nTxSigOps += GetP2SHSigOpCount(tx, view);
            if (nBlockSigOps + nTxSigOps >= MAX_BLOCK_SIGOPS)
                continue;

            CValidationState state;
            if (!CheckInputs(tx, state, view, true, SCRIPT_VERIFY_P2SH))
                continue;

            CTxUndo txundo;
            uint256 hash = tx.GetHash();
            UpdateCoins(tx, state, view, txundo, pindexPrev->nHeight+1, hash);

            // Added
            pblock->vtx.push_back(tx);
            pblocktemplate->vTxFees.push_back(nTxFees);
            pblocktemplate->vTxSigOps.push_back(nTxSigOps);
            nBlockSize += nTxSize;
            ++nBlockTx;
            nBlockSigOps += nTxSigOps;
            nFees += nTxFees;

            if (fPrintPriority)
            {
                LogPrintf("priority %.1f feeperkb %.1f txid %s\n",
                       dPriority, dFeePerKb, tx.GetHash().ToString());
            }

            // Add transactions that depend on this one to the priority queue
            if (mapDependers.count(hash))
            {
                BOOST_FOREACH(COrphan* porphan, mapDependers[hash])
                {
                    if (!porphan->setDependsOn.empty())
                    {
                        porphan->setDependsOn.erase(hash);
                        if (porphan->setDependsOn.empty())
                        {
                            vecPriority.push_back(TxPriority(porphan->dPriority, porphan->dFeePerKb, porphan->ptx));
                            std::push_heap(vecPriority.begin(), vecPriority.end(), comparer);
                        }
                    }
                }
            }
        }

        nLastBlockTx = nBlockTx;
        nLastBlockSize = nBlockSize;
        LogPrintf("CreateNewBlock(): total size %u\n", nBlockSize);

        pblock->vtx[0].vout[0].nValue = GetBlockValue(pindexPrev->nHeight+1, nFees);
        pblocktemplate->vTxFees[0] = -nFees;

        // Fill in header
        pblock->hashPrevBlock  = pindexPrev->GetBlockHash();
        UpdateTime(*pblock, pindexPrev);
        pblock->nBits          = GetNextWorkRequired(pindexPrev, pblock);
        pblock->nNonce         = 0;
        pblock->vtx[0].vin[0].scriptSig = CScript() << OP_0 << OP_0;
        pblocktemplate->vTxSigOps[0] = GetLegacySigOpCount(pblock->vtx[0]);

        CBlockIndex indexDummy(*pblock);
        indexDummy.pprev = pindexPrev;
        indexDummy.nHeight = pindexPrev->nHeight + 1;
        CCoinsViewCache viewNew(*pcoinsTip, true);
        CValidationState state;
        if (!ConnectBlock(*pblock, state, &indexDummy, viewNew, true))
            throw std::runtime_error("CreateNewBlock() : ConnectBlock failed");
    }

    return pblocktemplate.release();
}

void IncrementExtraNonce(CBlock* pblock, CBlockIndex* pindexPrev, unsigned int& nExtraNonce)
{
    // Update nExtraNonce
    static uint256 hashPrevBlock;
    if (hashPrevBlock != pblock->hashPrevBlock)
    {
        nExtraNonce = 0;
        hashPrevBlock = pblock->hashPrevBlock;
    }
    ++nExtraNonce;
    unsigned int nHeight = pindexPrev->nHeight+1; // Height first in coinbase required for block.version=2
    // pblock->vtx[0].vin[0].scriptSig = (CScript() << nHeight << CBigNum(nExtraNonce)) + COINBASE_FLAGS;
    pblock->vtx[0].vin[0].scriptSig = (CScript() << nHeight << CScriptNum(nExtraNonce));
    assert(pblock->vtx[0].vin[0].scriptSig.size() <= 200 * 1024);

    pblock->hashMerkleRoot = pblock->BuildMerkleTree();
}


void FormatHashBuffers(CBlock* pblock, char* pmidstate, char* pdata, char* phash1)
{
    //
    // Pre-build hash buffers
    //
    struct
    {
        struct unnamed2
        {
            int nVersion;
            uint256 hashPrevBlock;
            uint256 hashMerkleRoot;
            unsigned int nTime;
            unsigned int nBits;
            uint256 nNonce;
        }
        block;
        unsigned char pchPadding0[64];
        uint256 hash1;
        unsigned char pchPadding1[64];
    }
    tmp;
    memset(&tmp, 0, sizeof(tmp));

    tmp.block.nVersion       = pblock->nVersion;
    tmp.block.hashPrevBlock  = pblock->hashPrevBlock;
    tmp.block.hashMerkleRoot = pblock->hashMerkleRoot;
    tmp.block.nTime          = pblock->nTime;
    tmp.block.nBits          = pblock->nBits;
    tmp.block.nNonce         = pblock->nNonce;

    FormatHashBlocks(&tmp.block, sizeof(tmp.block));
    FormatHashBlocks(&tmp.hash1, sizeof(tmp.hash1));

    // Byte swap all the input buffer
    for (unsigned int i = 0; i < sizeof(tmp)/4; i++)
        ((unsigned int*)&tmp)[i] = ByteReverse(((unsigned int*)&tmp)[i]);

    // Precalc the first half of the first hash, which stays constant
    SHA256Transform(pmidstate, &tmp.block, pSHA256InitState);

    memcpy(pdata, &tmp.block, 128);
    memcpy(phash1, &tmp.hash1, 64);
}

#ifdef ENABLE_WALLET
//////////////////////////////////////////////////////////////////////////////
//
// Internal miner
//
double dHashesPerSec = 0.0;
int64_t nHPSTimerStart = 0;

CBlockTemplate* CreateNewBlockWithKey(CReserveKey& reservekey)
{
    CPubKey pubkey;
    if (!reservekey.GetReservedKey(pubkey))
        return NULL;

    CScript scriptPubKey = CScript() << OP_DUP << OP_HASH256 << pubkey.GetID() << OP_EQUALVERIFY << OP_CHECKSIG;

    LogPrint("miner", "[MINER] [%s] So[%s] \n", __func__, scriptPubKey.ToString().c_str());

    return CreateNewBlock(scriptPubKey);
}

bool CheckWork(CBlock* pblock, CWallet& wallet, CReserveKey& reservekey)
{
    if (pblock->hashPrevBlock != chainActive.Tip()->GetBlockHash())
        return false;

    //// debug print
    LogPrintf("BitcoinMiner:\n");
    pblock->print();
    LogPrintf("generated %s\n", FormatMoney(pblock->vtx[0].vout[0].nValue));

    // Found a solution
    {
        LOCK(cs_main);
        if (pblock->hashPrevBlock != chainActive.Tip()->GetBlockHash()) {
            LogPrintf("\n\n ***********hashBestChain****************** %s \n\n", chainActive.Tip()->GetBlockHash().ToString());
            LogPrintf("\n\n ***********pblock->hashPrevBlock****************** %s \n\n", pblock->hashPrevBlock.ToString());
            return error("BitcoinMiner : generated block is stale");
        }

        // Remove key from key pool
        reservekey.KeepKey();

        // Track how many getdata requests this block gets
        {
            LOCK(wallet.cs_wallet);
            wallet.mapRequestCount[pblock->GetHash()] = 0;
        }

        // Process this block the same as if we had received it from another node
        CValidationState state;
        if (!ProcessBlock(state, NULL, pblock))
            return error("BitcoinMiner : ProcessBlock, block not accepted");
    }

    return true;
}

void static BitcoinMiner(CWallet* pwallet, int threadNum, int deviceID, int deviceCount)
{
    LogPrintf("BitcoinMiner started\n");
    char threadname[16];
    SetThreadPriority(THREAD_PRIORITY_LOWEST);

    snprintf(threadname, sizeof(threadname), "%d/%d/Miner", deviceID, threadNum);
    RenameThread(threadname);

#ifdef USE_GPU
    SetDevice(deviceID);
#endif

    // Each thread has its own key and counter
    CReserveKey reservekey(pwallet, fUsedDefaultKey);
    unsigned int nExtraNonce = 0;

    try { while (true) {
        if (Params().NetworkID() != CChainParams::REGTEST) {
            // Busy-wait for the network to come online so we don't waste time mining
            // on an obsolete chain. In regtest mode we expect to fly solo.
            while (vNodes.empty())
                MilliSleep(1000);
        }

        //
        // Create new block
        //
        unsigned int nTransactionsUpdatedLast = mempool.GetTransactionsUpdated();
        CBlockIndex* pindexPrev = chainActive.Tip();

        auto_ptr<CBlockTemplate> pblocktemplate(CreateNewBlockWithKey(reservekey));
        if (!pblocktemplate.get())
            return;
        CBlock *pblock = &pblocktemplate->block;

        pblock->print();
        LogPrint("miner", "[MINER] [%s] new jop start ===========================================\n", __func__);

        IncrementExtraNonce(pblock, pindexPrev, nExtraNonce);

        LogPrintf("[%s] Running BitcoinMiner with %u transactions in block (%u bytes)\n", threadname, pblock->vtx.size(),
               ::GetSerializeSize(*pblock, SER_NETWORK, PROTOCOL_VERSION));

        //
        // Search
        //
        uint256 randomNonce = 0;
        if (pblock->nBits + 8 <= 48) {
            randomNonce = GetRandHash(4);
        } else if ((pblock->nBits + 8 > 48) && (pblock->nBits + 8 <= 80)) {
            randomNonce = GetRandHash(8);
        } else {
            randomNonce = GetRandHash();
        }
        uint64_t nSolm = 0;
        // uint64_t nOldSolm = 0;

        int64_t nStart = GetTime();
        uint256 tempHash = pblock->hashPrevBlock ^ pblock->hashMerkleRoot;
        uint256 seedHash = Hash(BEGIN(tempHash), END(tempHash));
        uint256 prevblockhash = 0;
        if (chainActive.Tip()->GetBlockHash() == Params().HashGenesisBlock()|| pindexPrev->pprev->GetBlockHash() == Params().HashGenesisBlock()) {
            prevblockhash = 0;
        } else {
            prevblockhash = pindexPrev->GetBlockHash();
        }
        while (true) {
            uint256 nNonceFound;

            // Solve the multivariable quadratic polynomial equations.

#ifdef USE_GPU
            nNonceFound = GPUMinerSearchSolution(seedHash, pblock->nBits, randomNonce, pindexPrev, deviceID, deviceCount, nSolm, threadname);
#else
            nNonceFound = SerchSolution(seedHash, pblock->nBits, randomNonce, pindexPrev);
#endif

            // uint32_t nHashesDone = nSolm - nOldSolm;
            // nOldSolm = nSolm;

            // Check if something found
            if (nNonceFound != uint256("0xffffffffffffffffffffffffffffffff")) {
                if (CheckSolution(seedHash, pblock->nBits, prevblockhash, pblock->nVersion, nNonceFound)) {
                    // Found a solution
                    pblock->nNonce = nNonceFound;
                SetThreadPriority(THREAD_PRIORITY_NORMAL);
                CheckWork(pblock, *pwallet, reservekey);
                SetThreadPriority(THREAD_PRIORITY_LOWEST);

                // In regression test mode, stop mining after a block is found. This
                // allows developers to controllably generate a block on demand.
                if (Params().NetworkID() == CChainParams::REGTEST)
                    throw boost::thread_interrupted();

                break;
                }
            }

            // Check for stop or if block needs to be rebuilt
            boost::this_thread::interruption_point();
            if (vNodes.empty() && Params().NetworkID() != CChainParams::REGTEST)
                break;
            if (mempool.GetTransactionsUpdated() != nTransactionsUpdatedLast && GetTime() - nStart > 60)
                break;
            if (pindexPrev != chainActive.Tip())
                break;

            // Update nTime every few seconds
            UpdateTime(*pblock, pindexPrev);
            // nBlockTime = ByteReverse(pblock->nTime);
            if (TestNet())
            {
                // Changing pblock->nTime can change work required on testnet:
                // nBlockBits = ByteReverse(pblock->nBits);
                // hashTarget = CBigNum().SetCompact(pblock->nBits).getuint256();
            }
        }
    } }
    catch (boost::thread_interrupted)
    {
        LogPrintf("[%s] BitcoinMiner terminated\n", threadname);
        throw;
    } catch (const std::runtime_error& e) {
        LogPrintf("[%s] BitcoinMiner runtime error: %s\n", threadname, e.what());
        return;
    }
}

void GenerateBitcoins(bool fGenerate, CWallet* pwallet, int nThreads)
{
    static boost::thread_group* minerThreads = NULL;

    if(!fGenerate)
        return;

#ifdef USE_GPU
    int deviceCount = GetDeviceCount();
    LogPrintf("The number of devices is %d \n", deviceCount);

    if (nThreads < 0)
        nThreads = deviceCount;
#else
    int deviceCount = 1;
    if (nThreads < 0)
        nThreads = boost::thread::hardware_concurrency();
#endif

    if (minerThreads != NULL)
    {
        minerThreads->interrupt_all();
        delete minerThreads;
        minerThreads = NULL;
    }

    if (nThreads == 0)
        return;

    minerThreads = new boost::thread_group();
    for (int i = 0; i < nThreads; i++)
        minerThreads->create_thread(boost::bind(&BitcoinMiner, pwallet, i, i % deviceCount, deviceCount));

    // minerThreads->interrupt_all();
    // minerThreads->join_all();
}

#endif
