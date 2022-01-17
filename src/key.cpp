// Copyright (c) 2018 The Bitcoin developers


#include <map>

#include "pqcrypto/api.h"
#include "key.h"




// void CKey::Reset() {
//     privKey.resize(RAINBOW_PRIVATE_KEY_SIZE);
// }

// CKey::CKey(): pubKey() {
//     privKey.resize(RAINBOW_PRIVATE_KEY_SIZE);
//     fValid = false;
// }

// CKey::CKey(const CKey& b) : pubKey() {
//     privKey.resize(RAINBOW_PRIVATE_KEY_SIZE);
//     if (b.privKey.size() == RAINBOW_PRIVATE_KEY_SIZE)
//         memcpy(privKey.data(), b.privKey.data(), RAINBOW_PRIVATE_KEY_SIZE);
//     else
//         throw key_error("CKey::CKey: invalid key size.");
//     fValid = b.fSet;
// }

// CKey& CKey::operator=(const CKey& b) {
//     if (b.privKey.size() == RAINBOW_PRIVATE_KEY_SIZE)
//         memcpy(privKey.data(), b.privKey.data(), RAINBOW_PRIVATE_KEY_SIZE);
//     else
//         throw key_error("CKey::CKey: invalid key size.");
//     pubKey = b.pubKey;
//     fValid = b.fSet;
//     return (*this);
// }


bool CKey::Check(const unsigned char *vch) {
    // for (unsigned int i=0; i<RAINBOW_PRIVATE_KEY_SIZE; i++) {
    //     if (vch[i] == '\0')
    //         return false;
    //     if (vch[i] != 0)
    //         return false;
    // }
    return true;
}

void CKey::MakeNewKey(bool fCompressedIn) {
    int status;
    unsigned char vchPubKey[RAINBOW_PUBLIC_KEY_SIZE];
    status = crypto_sign_keypair(&vchPubKey[0], &vch[0]);
    if (status != 0) {
        throw key_error("CKey::MakeNewKey, make key pair failure.");
    }
    pubKey.Set(&vchPubKey[0], &vchPubKey[RAINBOW_PUBLIC_KEY_SIZE]);
    fValid = true;
    fCompressed = false;
}

bool CKey::SetPrivKey(const CPrivKey &privkey, bool fCompressedIn) {
    if(privkey.size()!= RAINBOW_PRIVATE_KEY_SIZE){
        fValid = false;
        return fValid;
    }
    if (Check(&privkey[0])) {
        memcpy(vch, &privkey[0], RAINBOW_PRIVATE_KEY_SIZE);
        fValid = true;
        fCompressed = false;
    }
    return fValid;
}

bool CKey::SetPubKey(const CPubKey &vchPubKey, bool fCompressed) {
    pubKey.Set(&vchPubKey[0], &vchPubKey[RAINBOW_PUBLIC_KEY_SIZE]);
    return true;
}

CPrivKey CKey::GetPrivKey() const {
    assert(fValid);
    // CPrivKey privkey;
    // memcpy(&privkey[0], &vch[0], RAINBOW_PRIVATE_KEY_SIZE);
    // return privkey;
    return std::vector<unsigned char, secure_allocator<unsigned char>> (vch, vch + size());
}

CPubKey CKey::GetPubKey() const {
    assert(fValid);
    return pubKey;
}

bool CKey::Sign(const uint256 &hash, std::vector<unsigned char>& vchSig) const {
    int status = 0;
    vchSig.resize(RAINBOW_SIGNATURE_SIZE);
    status = rainbow_sign(&vchSig[0], vch, (unsigned char*)&hash);
    if (status != 0) {
        vchSig.clear();
        return false;
    }
    return true;
}

bool CKey::Load(CPrivKey &privkey, CPubKey &vchPubKey, bool fSkipCheck=false) {
    if (!SetPrivKey(privkey, fSkipCheck))
        return false;

    fCompressed = false;
    fValid = true;

    pubKey = vchPubKey;
    // if (fSkipCheck)
    //     return true;

    // if (GetPubKey() != vchPubKey)
    //     return false;

    return true;
}

bool CPubKey::Verify(const uint256 &hash, const std::vector<unsigned char>& vchSig) const {
    if (!IsValid())
        return false;
    if (vchSig.empty())
        return false;
    int status = -1;
    status = rainbow_verify((unsigned char*)&hash, &vchSig[0], vch);
    if (status != 0) {
        return false;
    }
    return true;
}
