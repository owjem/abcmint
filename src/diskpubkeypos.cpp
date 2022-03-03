// Copyright (c) 2018 The Bitcoin developers

#include "diskpubkeypos.h"
#include "base58.h"
#include "util.h"
#include "init.h"
#include "wallet.h"

CMemPos* pmemPos;


bool CMemPos::AddPubKeyPos2Map(const CKeyID& keyID, const CDiskPubKeyPos& pubKeyPos)
{
    {
        LOCK(cs_CMemPos);
        if(!mapPubKeyPos[keyID].IsNull() && pubKeyPos.GetHeight() <= mapPubKeyPos[keyID].GetHeight())
            return false;

        mapPubKeyPos[keyID] = pubKeyPos;
    }
    return true;
}

bool CMemPos::GetPubKeyPos(const CKeyID& keyID, CDiskPubKeyPos& posOut) const
{
    {
        LOCK(cs_CMemPos);
        CDiskPubKeyPos pos;
        if(!mapPubKeyPos.empty()){
            PubKeyPosMap::const_iterator mi = mapPubKeyPos.find(keyID);
            if (mi != mapPubKeyPos.end()) {
                LogPrintf(" ===> GetPubKeyPos 1  ");
                const CDiskPubKeyPos* pos = &(mi->second);
                posOut.SetHeight(pos->GetHeight());
                posOut.SetPubKeyOffset(pos->GetPubKeyOffset());
                return true;
            }
                LogPrintf(" ===> GetPubKeyPos 2 ");
        }
    }
    return false;
}

bool CMemPos::GetPossbyHeight(const unsigned int& nHeight, std::set<CDiskPubKeyPos> &Poss) const
{
    if (nHeight >= 0) {
        Poss.clear();
        PubKeyPosMap::const_iterator mi = mapPubKeyPos.begin();
        while (mi != mapPubKeyPos.end()) {
            if (mi->second.GetHeight() == nHeight)
                Poss.insert((*mi).second);
            mi++;
        }
        if (Poss.size() > 0)
            return true;
    }
    return false;
}

unsigned int CMemPos::GetPosCount() const
{
    // LOCK(cs_CMemPos);
    return mapPubKeyPos.size();
}


// bool CMemPos::CachePubKeyPos(const CBlockIndex* pBlockIndex)
// {
//     FILE* file = OpenBlockFile(pBlockIndex->GetBlockPos(), true);
//     if (NULL == file)
//         return error("%s() : open file blk%d.dat at %u error", __PRETTY_FUNCTION__, pBlockIndex->nFile, pBlockIndex->nDataPos);

//     CAutoFile filein(file, SER_DISK, CLIENT_VERSION);

//     // Read block
//     CBlock block;
//     try {
//         filein >> block;
//     }
//     catch (const std::exception& e) {
//         return error("%s: Deserialize or I/O error - %s, file blk%d.dat at %u", __func__, e.what(),
//                         pBlockIndex->nFile, pBlockIndex->nDataPos);
//     }

//     unsigned int offset = sizeof(CBlockHeader); //block header
//     unsigned int nTxCount = block.vtx.size();
//     offset += GetSizeOfCompactSize(nTxCount);

//     for (unsigned int i=0; i<nTxCount; i++)
//     {
//         const CTransaction &tx = block.vtx[i];
//         unsigned int offsetVin = sizeof(tx.nVersion); // the vin offset

//         unsigned int nVinSize = tx.vin.size();
//         offsetVin += GetSizeOfCompactSize(nVinSize);

//         for (unsigned int i = 0; i < nVinSize; i++) {
//             CTxIn txIn = tx.vin[i];

//             unsigned int nScriptSigSize = txIn.scriptSig.size();

//             if(splitScript(txIn.scriptSig, i, pBlockIndex->nHeight))
//             {

//             }else{
//                 offsetVin += ::GetSerializeSize(txIn, SER_DISK, CLIENT_VERSION);
//             }
//             //compare the pubkey in the scripts with the input pubKeyIn
//             std::string strScripts = HexStr(txIn.scriptSig);
//             std::size_t found = strScripts.find(pubKeyIn);
//             if (found!=std::string::npos) {

//                 pubKeyPos.nHeight = pBlockIndex->nHeight;

//                 offsetVin += sizeof(COutPoint); //transation hash length + prevout index length
//                 offsetVin += GetSizeOfCompactSize(nScriptSigSize);
//                 pubKeyPos.nPubKeyOffset = offset + offsetVin + found/2; // the offset in the current block, 2 hex char for one byte
//                 unsigned int npubKeyLength = GetSizeOfCompactSize(pubKeyIn.length());
//                 LogPrintf("public key found: offset:%u, offsetVin:%u, found:%lu, npubKeyLength:%u. \n",
//                         offset, offsetVin, found, npubKeyLength);
//                 pubKeyPos.nPubKeyOffset -= npubKeyLength;
//                 return true;
//             } else {
//                 offsetVin += ::GetSerializeSize(txIn, SER_DISK, CLIENT_VERSION);
//             }
//         }

//         offset += GetSerializeSize(tx, SER_DISK, CLIENT_VERSION); //CTransaction length
//     }

//     //still no transation contain this pubkey in the block chain
//     return false;
// }


// bool CMemPos::splitScript(const CScript& script, unsigned int nIn, unsigned int nHeight) {
//     CScript::const_iterator pc = script.begin();
//     CScript::const_iterator pend = script.end();

//     vector<bool> vfExec;
//     opcodetype opcode;
//     vector<unsigned char> vchPushValue;
//     if (script.size() > 1000000)
//         return false;

//     try
//     {
//         while (pc < pend)
//         {
//             if (!script.GetOp(pc, opcode, vchPushValue))
//                 return false;
//             if (vchPushValue.size() != RAINBOW_PUBLIC_KEY_SIZE)
//                 return false;
//             if (0 <= opcode && opcode <= OP_PUSHDATA4){

//                 CPubKey pubKey;
//                 unsigned int vchsize = vchPushValue.size();

//                 vector<unsigned char> vch;

//                 CDiskPubKeyPos pos(nHeight, 0);
//                 vch = pos.Raw();

//                 // LogPrintf(" ===> [%s][%d] [%s] \n", __func__, i, HexStr(vch));
//                 // stack.insert(make_pair(vch, vchPushValue));


//             }
//         }
//     }
//     catch (...)
//     {
//         return false;
//     }

//     return true;
// }

// bool FindPubKeyPos(std::string& pubKeyIn, CDiskPubKeyPos& pubKeyPos)
// {
//     CBlockIndex* pBlockIndex = chainActive.Genesis();
//     CBlockIndex* pindexState = chainActive.Tip();
//     while (pBlockIndex != pindexState) {
//         if (pindexState->nHeight - pBlockIndex->nHeight + 1 < COINBASE_MATURITY+20) {
//             LogPrintf("%s: block not maturity, height:%u\n", __func__, pindexState->nHeight - pBlockIndex->nHeight + 1);
//             return false;
//         }
//         FILE* file = OpenBlockFile(pBlockIndex->GetBlockPos(), true);
//         if (NULL == file)
//             return error("%s() : open file blk%d.dat at %u error", __PRETTY_FUNCTION__, pBlockIndex->nFile, pBlockIndex->nDataPos);

//         CAutoFile filein(file, SER_DISK, CLIENT_VERSION);

//         // Read block
//         CBlock block;
//         try {
//             filein >> block;
//         }
//         catch (const std::exception& e) {
//             return error("%s: Deserialize or I/O error - %s, file blk%d.dat at %u", __func__, e.what(),
//                          pBlockIndex->nFile, pBlockIndex->nDataPos);
//         }

//         unsigned int offset = sizeof(CBlockHeader); //block header
//         unsigned int nTxCount = block.vtx.size();
//         offset += GetSizeOfCompactSize(nTxCount);

//         for (unsigned int i=0; i<nTxCount; i++)
//         {
//             const CTransaction &tx = block.vtx[i];
//             unsigned int offsetVin = sizeof(tx.nVersion); // the vin offset

//             unsigned int nVinSize = tx.vin.size();
//             offsetVin += GetSizeOfCompactSize(nVinSize);

//             for (unsigned int i = 0; i < nVinSize; i++) {
//                 CTxIn txIn = tx.vin[i];

//                 unsigned int nScriptSigSize = txIn.scriptSig.size();

//                 //compare the pubkey in the scripts with the input pubKeyIn
//                 std::string strScripts = HexStr(txIn.scriptSig);
//                 std::size_t found = strScripts.find(pubKeyIn);
//                 if (found!=std::string::npos) {
//                     pubKeyPos.nHeight = pBlockIndex->nHeight;

//                     offsetVin += sizeof(COutPoint); //transation hash length + prevout index length
//                     offsetVin += GetSizeOfCompactSize(nScriptSigSize);
//                     pubKeyPos.nPubKeyOffset = offset + offsetVin + found/2; // the offset in the current block, 2 hex char for one byte
//                     unsigned int npubKeyLength = GetSizeOfCompactSize(pubKeyIn.length());
//                     LogPrintf("public key found: offset:%u, offsetVin:%u, found:%lu, npubKeyLength:%u. \n",
//                             offset, offsetVin, found, npubKeyLength);
//                     pubKeyPos.nPubKeyOffset -= npubKeyLength;
//                     return true;
//                 } else {
//                     offsetVin += ::GetSerializeSize(txIn, SER_DISK, CLIENT_VERSION);
//                 }
//             }

//             offset += GetSerializeSize(tx, SER_DISK, CLIENT_VERSION); //CTransaction length
//         }

//         pBlockIndex = chainActive.Next(pBlockIndex);
//     }

//     //still no transation contain this pubkey in the block chain
//     return false;
// }

// bool GetPubKeyByPos(CDiskPubKeyPos pos, CPubKey& pubKey)
// {
//     CBlockIndex* pblockindex = NULL;
//     if(pos.nHeight >0){
//         pblockindex = chainActive[pos.nHeight];
//     }
//     if (!pblockindex) {
//         return error("%s() : can't find block at height: %u", __PRETTY_FUNCTION__, pos.nHeight);
//     }

//     //4e (21 52 02 00) (e2 b8 8a 76 1b 0d d7 8e b3...)--4e is the opcode, (21 52 02 00) is the length
//     CDiskBlockPos blockPos(pblockindex->nFile , pblockindex->nDataPos + pos.nPubKeyOffset);
//     FILE* pFile = OpenBlockFile(blockPos, true);
//     if (NULL == pFile)
//         return error("%s() : open file blk%d.dat error", __PRETTY_FUNCTION__, pblockindex->nFile);

//     CAutoFile file(pFile, SER_DISK, CLIENT_VERSION);
//     try {
//         unsigned char opcode;
//         READDATA(file, opcode);
//         unsigned int nSize = 0;

//         switch(opcode)
//         {
//             case OP_PUSHDATA1:
//             case OP_PUSHDATA2:
//             {
//                 unsigned short chSize;
//                 READDATA(file, chSize);
//                 nSize = chSize;
//             }
//             break;
//             case OP_PUSHDATA4:
//             {
//                 READDATA(file, nSize);
//             }
//             break;
//             default:
//                 nSize = opcode;
//             break;
//         }

//         //currently rainbow public key size is fixed, maybe change in future, change this
//         if (nSize != RAINBOW_PUBLIC_KEY_SIZE)
//             return error("%s() : invalid opcode=[%x], value[%d] or I/O error", __PRETTY_FUNCTION__, opcode, nSize);

//         unsigned int i = 0;
//         while (i < nSize)
//         {
//             unsigned int blk = std::min(nSize - i, (unsigned int)(1 + 4999999 / sizeof(unsigned char)));

//             // pubKey.vchPubKey.resize(i + blk);
//             file.read((char*)&pubKey[i], blk * sizeof(unsigned char));
//             i += blk;
//         }

//     } catch (std::exception &e) {
//         return error("%s() : deserialize or I/O error", __PRETTY_FUNCTION__);
//     }

//     return true;
// }

// bool UpdatePubKeyPos(CPubKey& pubKey, const std::string& address, const CKeyID &keyID)
// {
//     CDiskPubKeyPos pos;
//     bool found = false;
//     std::string strPubKey = HexStr(pubKey.begin(),pubKey.end());

//     if (!pmemPos->GetPubKeyPos(keyID, pos)) {
//         if (!FindPubKeyPos(strPubKey, pos)) {
//             LogPrintf("[%s] public key %s not found in block chain! \n", __func__, address);
//             found = false;
//         } else {
//             LogPrintf("[%s] public key %s found at height=%d, offset=%u. \n", __func__, address, pos.nHeight, pos.nPubKeyOffset);
//             pmemPos->AddPubKeyPos2Map(keyID, pos);
//             found = true;
//         }
//     } else {
//         //read the public key by position and compare, if not match, find it again
//         CPubKey diskPubKey;
//         LogPrintf("[%s] public key pos is not null, height=%u, offset=%u. checking! \n", __func__, pos.nHeight, pos.nPubKeyOffset);
//         if (!GetPubKeyByPos(pos, diskPubKey)) {
//             LogPrintf("[%s] can't get public key %s at height=%u, offset=%u. will find again! \n", __func__,
//                 address, pos.nHeight, pos.nPubKeyOffset);
//             pos.SetNull();
//         } else if (diskPubKey != pubKey) {
//             LogPrintf("[%s] public key foud but not equal, will find again! \n", __func__);
//             pos.SetNull();
//         }

//         if (pos.IsNull()) {
//             //find again
//             if (!FindPubKeyPos(strPubKey, pos)) {
//                 LogPrintf("[%s] find again, public key %s not found in block chain! \n", __func__, address);
//                 found = false;
//             } else {
//                 LogPrintf("[%s] find again, public key %s found at height=%d, offset=%u. \n", __func__,
//                     address, pos.nHeight, pos.nPubKeyOffset);
//                 pmemPos->AddPubKeyPos2Map(keyID, pos);
//                 found = true;
//             }
//         } else {
//             LogPrintf("[%s] public key %s found and match at height=%d, offset=%u. \n", __func__,
//                 address, pos.nHeight, pos.nPubKeyOffset);
//             found = true;
//         }
//     }

//     return found;
// }

// void PubKeyScanner(CWallet* pwalletMain)
// {
//     LogPrintf("PubKeyScanner started\n");
//     RenameThread("PubKeyScanner");
//     bool allPosFound = true;
//     SetThreadPriority(THREAD_PRIORITY_NORMAL);

//     try {
//         while(true) {

//             std::set<CKeyID> setAddress;
//             pwalletMain->GetKeys(setAddress);
//             for (std::set<CKeyID>::iterator it = setAddress.begin(); it != setAddress.end(); ++it) {
//                 std::string address = CBitcoinAddress(*it).ToString();
//                 const CKeyID& keyID = *it;

//                 CPubKey pubKey;
//                 if (!pwalletMain->GetPubKey(keyID, pubKey)) {
//                     LogPrintf("[%s] address %s not found in wallet.\n", __func__, address);
//                     continue;
//                 }

//                 if(!UpdatePubKeyPos(pubKey, address, keyID)) {
//                     LogPrintf("[%s] address %s update public key position is not in active chain util you create a transaction.\n", __func__, address);
//                     allPosFound = false;
//                     continue;
//                 } else {
//                     std::map<CTxDestination, CAddressBookData>::iterator mi = pwalletMain->mapAddressBook.find(keyID);
//                     if (mi != pwalletMain->mapAddressBook.end())
//                         pwalletMain->NotifyAddressBookChanged(pwalletMain, keyID, "", true, mi->second.name, CT_UPDATED);
//                 }

//             }

//             if (allPosFound) {
//                 SetThreadPriority(THREAD_PRIORITY_LOWEST);
//             } else
//                 SetThreadPriority(THREAD_PRIORITY_NORMAL);

//             MilliSleep(allPosFound ? 120*60*1000 : 10*60*1000);
//         }
//     }
//     catch (boost::thread_interrupted)
//     {
//         LogPrintf("PubKeyScanner terminated\n");
//         throw;
//     }
// }

//more logic extention (offer rpc command to user to control this thread)
// void SearchPubKeyPos(boost::thread_group& threadGroup)
// {
//     threadGroup.create_thread(boost::bind(&PubKeyScanner, pwalletMain));
// }


void CMemPosInit(){
    pmemPos = new CMemPos();
}
