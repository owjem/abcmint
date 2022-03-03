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
                const CDiskPubKeyPos* pos = &(mi->second);
                posOut.SetHeight(pos->GetHeight());
                posOut.SetPubKeyOffset(pos->GetPubKeyOffset());
                return true;
            }
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

void CMemPosInit(){
    pmemPos = new CMemPos();
}
