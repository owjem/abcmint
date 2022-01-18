// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_KEY_H
#define BITCOIN_KEY_H

#include "allocators.h"
#include "hash.h"
#include "serialize.h"
#include "uint256.h"

#include <stdexcept>
#include <vector>


/**
 * RAINBOW signature:
 */
static const unsigned int RAINBOW_PUBLIC_KEY_SIZE = 152097;
static const unsigned int RAINBOW_SIGNATURE_SIZE = 64;
const static unsigned int RAINBOW_PRIVATE_KEY_SIZE = 100209;
static const unsigned int HASH_LEN_BYTES = 32;

class key_error : public std::runtime_error
{
public:
    explicit key_error(const std::string& str) : std::runtime_error(str) {}
};

/** A reference to a CKey: the Hash of its serialized public key */
class CKeyID : public uint256
{
public:
    CKeyID() : uint256(0) { }
    CKeyID(const uint256& in) : uint256(in) { }
    CKeyID(const std::string& in) : uint256(in) { }
};

/** A reference to a CScript: the Hash of its serialization (see script.h) */
class CScriptID : public uint256
{
public:
    CScriptID() : uint256(0) { }
    CScriptID(const uint256& in) : uint256(in) { }
};

/** An encapsulated public key. */
class CPubKey {
private:
    // Just store the serialized data.
    // Its length can very cheaply be computed from the first byte.
    unsigned char vch[RAINBOW_PUBLIC_KEY_SIZE];
    // friend class CKey;
    // Compute the length of a pubkey with a given first byte.
    unsigned int static GetLen(unsigned char chHeader) {
        return RAINBOW_PUBLIC_KEY_SIZE;
    }

    // Set this key data to be invalid
    void Invalidate() {
        // vch[0] = 0xff;
        memset(&vch[0], 0xff, 3);
    }

public:
    // Construct an invalid public key.
    CPubKey() {
        Invalidate();
    }

    // Initialize a public key using begin/end iterators to byte data.
    template<typename T>
    void Set(const T pbegin, const T pend) {
        int len = pend == pbegin ? 0 : GetLen(pbegin[0]);
        if (len && len == (pend-pbegin))
            memcpy(vch, (unsigned char*)&pbegin[0], len);
        else
            Invalidate();
    }

    // Construct a public key using begin/end iterators to byte data.
    template<typename T>
    CPubKey(const T pbegin, const T pend) {
        Set(pbegin, pend);
    }

    // Construct a public key from a byte vector.
    CPubKey(const std::vector<unsigned char> &vch) {
        Set(vch.begin(), vch.end());
    }

    // Simple read-only vector-like interface to the pubkey data.
    unsigned int size() const { return GetLen(vch[0]); }
    const unsigned char *begin() const { return vch; }
    const unsigned char *end() const { return vch+size(); }
    const unsigned char &operator[](unsigned int pos) const { return vch[pos]; }

    // Comparator implementation.
    friend bool operator==(const CPubKey &a, const CPubKey &b) {
        return a.vch[0] == b.vch[0] &&
               memcmp(a.vch, b.vch, a.size()) == 0;
    }
    friend bool operator!=(const CPubKey &a, const CPubKey &b) {
        return !(a == b);
    }
    friend bool operator<(const CPubKey &a, const CPubKey &b) {
        return a.vch[0] < b.vch[0] ||
               (a.vch[0] == b.vch[0] && memcmp(a.vch, b.vch, a.size()) < 0);
    }

    // Implement serialization, as if this was a byte vector.
    unsigned int GetSerializeSize(int nType, int nVersion) const {
        return size() + 1;
    }
    template<typename Stream> void Serialize(Stream &s, int nType, int nVersion) const {
        unsigned int len = size();
        ::WriteCompactSize(s, len);
        s.write((char*)vch, len);
    }
    template<typename Stream> void Unserialize(Stream &s, int nType, int nVersion) {
        unsigned int len = ::ReadCompactSize(s);
        if (len <= RAINBOW_PUBLIC_KEY_SIZE) {
            s.read((char*)vch, len);
        } else {
            // invalid pubkey, skip available data
            char dummy;
            while (len--)
                s.read(&dummy, 1);
            Invalidate();
        }
    }

    // Get the KeyID of this public key (hash of its serialization)
    CKeyID GetID() const {
        return CKeyID(Hash(vch, vch+size()));
    }

    // Get the 256-bit hash of this public key.
    uint256 GetHash() const {
        return Hash(vch, vch+size());
    }

    // just check syntactic correctness.
    bool IsValid() const
    {
        if(size() != RAINBOW_PUBLIC_KEY_SIZE) return false;
        if(vch[0] == 0xff && vch[1] == 0xff && vch[2] == 0xff && vch[3] == 0) return false;

        // for (unsigned int i=0; i<8; i++)
        //     printf(" ===> x %d vch[%02x]", i, vch[i] );

        return true;
    }
    // fully validate whether this is a valid public key (more expensive than IsValid())
    bool IsFullyValid() const;

    // Check whether this is a compressed public key.
    bool IsCompressed() const {
        return false;
    }

    // Verify a DER signature (~72 bytes).
    // If this public key is not fully valid, the return value will be false.
    bool Verify(const uint256 &hash, const std::vector<unsigned char>& vchSig) const;

    std::vector<unsigned char> Raw() const
    {
        return std::vector<unsigned char>(vch, vch + size());
    }

};


// secure_allocator is defined in allocators.h
// CPrivKey is a serialized private key, with all parameters included (279 bytes)
typedef std::vector<unsigned char, secure_allocator<unsigned char> > CPrivKey;
// CSecret is a serialization of just the secret parameter (32 bytes)
typedef std::vector<unsigned char, secure_allocator<unsigned char>> CSecret;

/** An encapsulated private key. */
class CKey {
private:
    // Whether this private key is valid. We check for correctness when modifying the key
    // data, so fValid should always correspond to the actual state.
    bool fValid;

    // Whether the public key corresponding to this private key is (to be) compressed.
    bool fCompressed;

    // The actual byte data
    unsigned char vch[RAINBOW_PRIVATE_KEY_SIZE];

    // Check whether the 32-byte array pointed to be vch is valid keydata.
    bool static Check(const unsigned char *vch);
public:
    // The public key associate to private key
    CPubKey pubKey;

    // Construct an invalid private key.
    CKey() : fValid(false) {
        LockObject(vch);
    }

    // Copy constructor. This is necessary because of memlocking.
    CKey(const CKey &secret) : fValid(secret.fValid), fCompressed(secret.fCompressed) {
        LockObject(vch);
        memcpy(vch, secret.vch, sizeof(vch));
    }

    // Destructor (again necessary because of memlocking).
    ~CKey() {
        UnlockObject(vch);
    }

    friend bool operator==(const CKey &a, const CKey &b) {
        return a.fCompressed == b.fCompressed && a.size() == b.size() &&
               memcmp(&a.vch[0], &b.vch[0], a.size()) == 0;
    }

    // Initialize using begin and end iterators to byte data.
    template<typename T>
    void Set(const T pbegin, const T pend, bool fCompressedIn=false) {
        if (pend - pbegin != RAINBOW_PRIVATE_KEY_SIZE) {
            fValid = false;
            return;
        }
        if (Check(&pbegin[0])) {
            memcpy(vch, (unsigned char*)&pbegin[0], RAINBOW_PRIVATE_KEY_SIZE);
            fValid = true;
            fCompressed = fCompressedIn;
        } else {
            fValid = false;
        }
    }

    // Simple read-only vector-like interface.
    unsigned int size() const { return (fValid ? RAINBOW_PRIVATE_KEY_SIZE : 0); }
    const unsigned char *begin() const { return vch; }
    const unsigned char *end() const { return vch + size(); }

    // Check whether this private key is valid.
    bool IsValid() const { return fValid; }

    // Check whether the public key corresponding to this private key is (to be) compressed.
    bool IsCompressed() const { return false; }

    // Initialize from a CPrivKey (serialized OpenSSL private key data).
    bool SetPrivKey(const CPrivKey &vchPrivKey, bool fCompressed=false);

    // Initialize from a CPrivKey (serialized OpenSSL private key data).
    bool SetPubKey(const CPubKey &vchPubKey, bool fCompressed=false);

    // Generate a new private key using a cryptographic PRNG.
    void MakeNewKey(bool fCompressed=false);

    // Convert the private key to a CPrivKey (serialized OpenSSL private key data).
    // This is expensive.
    CPrivKey GetPrivKey() const;

    // Compute the public key from a private key.
    // This is expensive.
    CPubKey GetPubKey() const;

    // Create a DER-serialized signature.
    bool Sign(const uint256 &hash, std::vector<unsigned char>& vchSig) const;

    // Load private key and check that public key matches.
    bool Load(CPrivKey &privkey, CPubKey &vchPubKey, bool fSkipCheck);

};

#endif
