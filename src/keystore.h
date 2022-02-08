// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_KEYSTORE_H
#define BITCOIN_KEYSTORE_H

#include "key.h"
#include "sync.h"

#include <boost/signals2/signal.hpp>
// #include "diskpubkeypos.h"

class CScript;

/** A virtual base class for key stores */
class CKeyStore
{
protected:
    mutable CCriticalSection cs_KeyStore;

public:
    virtual ~CKeyStore() {}

    // Add a key to the store.
    virtual bool AddKeyPubKey(const CKey &key, const CPubKey &pubkey) =0;
    virtual bool AddKey(const CKey &key);

    // Check whether a key corresponding to a given address is present in the store.
    virtual bool HaveKey(const CKeyID &address) const =0;
    virtual bool GetKey(const CKeyID &address, CKey& keyOut) const =0;
    virtual void GetKeys(std::set<CKeyID> &setAddress) const =0;
    virtual bool GetPubKey(const CKeyID &address, CPubKey& vchPubKeyOut) const;

    // Support for BIP 0013 : see https://en.bitcoin.it/wiki/BIP_0013
    virtual bool AddCScript(const CScript& redeemScript) =0;
    virtual bool HaveCScript(const CScriptID &hash) const =0;
    virtual bool GetCScript(const CScriptID &hash, CScript& redeemScriptOut) const =0;

    // virtual bool GetPubKeyPos(const CKeyID &address, CDiskPubKeyPos& posOut) const =0;
};

typedef std::map<CKeyID, CKey> KeyMap;
typedef std::map<CScriptID, CScript > ScriptMap;
// typedef std::map<CKeyID, CDiskPubKeyPos> PubKeyPosMap;

/** Basic key store, that keeps keys in an address->secret map */
class CBasicKeyStore : public CKeyStore
{
protected:
    KeyMap mapKeys;
    ScriptMap mapScripts;
    // PubKeyPosMap mapPubKeyPos;

public:
    bool AddKeyPubKey(const CKey& key, const CPubKey &pubkey);
    bool HaveKey(const CKeyID &address) const
    {
        bool result;
        {
            LOCK(cs_KeyStore);
            result = (mapKeys.count(address) > 0);
        }
        return result;
    }
    void GetKeys(std::set<CKeyID> &setAddress) const
    {
        setAddress.clear();
        {
            LOCK(cs_KeyStore);
            KeyMap::const_iterator mi = mapKeys.begin();
            while (mi != mapKeys.end())
            {
                setAddress.insert((*mi).first);
                mi++;
            }
        }
    }
    bool GetKey(const CKeyID &address, CKey &keyOut) const
    {
        {
            LOCK(cs_KeyStore);
            KeyMap::const_iterator mi = mapKeys.find(address);
            if (mi != mapKeys.end())
            {
                const CKey* pKey = &(mi->second);
                keyOut.pubKey = pKey->GetPubKey();
                keyOut.SetPrivKey(pKey->GetPrivKey());
                return true;
            }
        }
        return false;
    }

    // bool AddPubKeyPos2Map(const CKeyID &keyID, const CDiskPubKeyPos& pos)
    // {
    // 	{
    //         LOCK(cs_KeyStore);
    //         mapPubKeyPos[keyID] = pos;
    // 	}
    //     return true;
    // }

    // bool GetPubKeyPos(const CKeyID &keyID, CDiskPubKeyPos& posOut) const
    // {
    //     {
    //         LOCK(cs_KeyStore);
    //         PubKeyPosMap::const_iterator mi = mapPubKeyPos.find(keyID);
    //         if (mi != mapPubKeyPos.end()) {
    //             const CDiskPubKeyPos* pos = &(mi->second);
    //             posOut.nHeight = pos->nHeight;
    //             posOut.nPubKeyOffset = pos->nPubKeyOffset;
    //             return true;
    //         }
    //     }
    //     return false;
    // }

    virtual bool AddCScript(const CScript& redeemScript);
    virtual bool HaveCScript(const CScriptID &hash) const;
    virtual bool GetCScript(const CScriptID &hash, CScript& redeemScriptOut) const;
};

typedef std::vector<unsigned char, secure_allocator<unsigned char> > CKeyingMaterial;
typedef std::map<CKeyID, std::pair<CPubKey, std::vector<unsigned char> > > CryptedKeyMap;

#endif
