// Copyright (c) 2018 The Bitcoin developers

#ifndef BITCOIN_DISKPUBKEYPOS_H
#define BITCOIN_DISKPUBKEYPOS_H

#include <vector>
#include <boost/thread.hpp>
#include "key.h"
#include "serialize.h"
#include "sync.h"
#include "util.h"

static const unsigned int RAINBOW_PUBLIC_KEY_POS_SIZE         = 8;
static const unsigned int RAINBOW_PUBLIC_KEY_REUSED_SIZE      = 4;

// nHeight is different from CDiskBlockPos.nFile, because it need to cast to
// unsigned char in CScript
class CDiskPubKeyPos {
private:
  unsigned int nHeight;
  unsigned int nPubKeyOffset;
  bool fIsMe;

public:

  CDiskPubKeyPos(unsigned int nHeightIn, unsigned int nPubKeyOffsetIn, bool fIsMeIn =false)
      : nHeight(nHeightIn), nPubKeyOffset(nPubKeyOffsetIn), fIsMe(fIsMeIn) {}

  CDiskPubKeyPos() { SetNull(); }

  void SetNull() {
    nHeight = 0xFFFFFFFF;
    nPubKeyOffset = 0;
    fIsMe = false;
  }

  bool IsNull() const { return (nHeight == 0xFFFFFFFF); }

  unsigned int GetHeight() const { return nHeight; }

  void SetHeight(const unsigned int nHeightIn) { nHeight = nHeightIn; }

  unsigned int GetPubKeyOffset() const { return nPubKeyOffset; }

  void SetPubKeyOffset(const unsigned int nPubKeyOffsetIn) { nPubKeyOffset = nPubKeyOffsetIn; }

  void SetPubKeyOffset(const std::vector<unsigned char> nPubKeyOffsetIn) {
    memcpy(&nPubKeyOffset, &nPubKeyOffsetIn[0], nPubKeyOffsetIn.size() * sizeof(nPubKeyOffsetIn[0]));
    bswap_32(nPubKeyOffset);
  }

  void UpdatePubKeyOffset() { nPubKeyOffset = nPubKeyOffset + 1; }

  unsigned int size() const { return sizeof(nHeight)+sizeof(nHeight); }

  friend bool operator==(const CDiskPubKeyPos &a, const CDiskPubKeyPos &b) {
    return (a.nHeight == b.nHeight && a.nPubKeyOffset == b.nPubKeyOffset);
  }

  friend bool operator!=(const CDiskPubKeyPos &a, const CDiskPubKeyPos &b) {
    return !(a == b);
  }

  friend bool operator<(const CDiskPubKeyPos &a, const CDiskPubKeyPos &b) {
    return false;
  }
  friend bool operator>(const CDiskPubKeyPos &a, const CDiskPubKeyPos &b) {
    return false;
  }

  CDiskPubKeyPos &operator<<(const std::vector<unsigned char> &v) {
    if (v.size() < RAINBOW_PUBLIC_KEY_POS_SIZE) {
      nHeight = 0xFFFFFFFF;
      memcpy(&nPubKeyOffset, &v[0], sizeof(nPubKeyOffset));
      bswap_32(nPubKeyOffset);
      return *this;
    }
    memcpy(&nHeight, &v[0], sizeof(nHeight));
    memcpy(&nPubKeyOffset, &v[4], sizeof(nPubKeyOffset));
    bswap_32(nHeight);
    bswap_32(nPubKeyOffset);

    return *this;
  }

 std::vector<unsigned char> Raw() const {
    std::vector<unsigned char> v;
    v.insert(v.end(), (unsigned char*)&nHeight, (unsigned char*)&nHeight+sizeof(nHeight));
    v.insert(v.end(), (unsigned char*)&nPubKeyOffset, (unsigned char*)&nPubKeyOffset+sizeof(nPubKeyOffset));
    return v;
  }
  std::vector<unsigned char> PubKeyOffsetRaw() const {
    std::vector<unsigned char> v;
    v.insert(v.end(), (unsigned char*)&nPubKeyOffset, (unsigned char*)&nPubKeyOffset+sizeof(nPubKeyOffset));
    return v;
  }
};

typedef std::map<CKeyID, CDiskPubKeyPos> PubKeyPosMap;
class CMemPos {
protected:
    mutable CCriticalSection cs_CMemPos;

public:
    PubKeyPosMap mapPubKeyPos;

    CMemPos(){}

    bool AddPubKeyPos2Map(const CKeyID& keyID, const CDiskPubKeyPos& pos);

    bool GetPubKeyPos(const CKeyID& keyID, CDiskPubKeyPos& posOut) const;

    bool GetPossbyHeight(const unsigned int& nHeight, std::set<CDiskPubKeyPos>& Poss) const;

    unsigned int GetPosCount() const;
    // bool splitScript(const CScript& script, unsigned int nIn, unsigned int nHeight);
};


void CMemPosInit();

#endif
