#include "boost/test/unit_test_log.hpp"
#include "script.h"

#include "data/script_invalid.json.h"
#include "data/script_invalid_long.json.h"
#include "data/script_valid.json.h"

#include "key.h"
#include "keystore.h"
#include "main.h"

#include <fstream>
#include <stdint.h>
#include <string>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>
#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_utils.h"
#include "json/json_spirit_writer_template.h"

using namespace std;
using namespace json_spirit;
using namespace boost::algorithm;

extern uint256 SignatureHash(const CScript &scriptCode, const CTransaction& txTo, unsigned int nIn, int nHashType);

static const unsigned int flags = SCRIPT_VERIFY_P2SH | SCRIPT_VERIFY_STRICTENC;

CScript
ParseScript(string s)
{
    CScript result;

    static map<string, opcodetype> mapOpNames;

    if (mapOpNames.size() == 0)
    {
        for (int op = 0; op <= OP_NOP10; op++)
        {
            // Allow OP_RESERVED to get into mapOpNames
            if (op < OP_NOP && op != OP_RESERVED)
                continue;

            const char* name = GetOpName((opcodetype)op);
            if (strcmp(name, "OP_UNKNOWN") == 0)
                continue;
            string strName(name);
            mapOpNames[strName] = (opcodetype)op;
            // Convenience: OP_ADD and just ADD are both recognized:
            replace_first(strName, "OP_", "");
            mapOpNames[strName] = (opcodetype)op;
        }
    }

    vector<string> words;
    split(words, s, is_any_of(" \t\n"), token_compress_on);

    BOOST_FOREACH(string w, words)
    {
        if (all(w, is_digit()) ||
            (starts_with(w, "-") && all(string(w.begin()+1, w.end()), is_digit())))
        {
            // Number
            int64_t n = atoi64(w);
            result << n;
        }
        else if (starts_with(w, "0x") && IsHex(string(w.begin()+2, w.end())))
        {
            // Raw hex data, inserted NOT pushed onto stack:
            std::vector<unsigned char> raw = ParseHex(string(w.begin()+2, w.end()));
            result.insert(result.end(), raw.begin(), raw.end());
        }
        else if (w.size() >= 2 && starts_with(w, "'") && ends_with(w, "'"))
        {
            // Single-quoted string, pushed as data. NOTE: this is poor-man's
            // parsing, spaces/tabs/newlines in single-quoted strings won't work.
            std::vector<unsigned char> value(w.begin()+1, w.end()-1);
            result << value;
        }
        else if (mapOpNames.count(w))
        {
            // opcode, e.g. OP_ADD or ADD:
            result << mapOpNames[w];
        }
        else
        {
            BOOST_ERROR("Parse error: " << s);
            return CScript();
        }
    }

    return result;
}

Array
read_json(const std::string& jsondata)
{
    Value v;

    if (!read_string(jsondata, v) || v.type() != array_type)
    {
        BOOST_ERROR("Parse error.");
        return Array();
    }
    return v.get_array();
}

BOOST_AUTO_TEST_SUITE(script_tests)

BOOST_AUTO_TEST_CASE(script_valid)
{
    // Read tests from test/data/script_valid.json
    // Format is an array of arrays
    // Inner arrays are [ "scriptSig", "scriptPubKey" ]
    // ... where scriptSig and scriptPubKey are stringified
    // scripts.
    Array tests = read_json(std::string(json_tests::script_valid, json_tests::script_valid + sizeof(json_tests::script_valid)));

    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        string strTest = write_string(tv, false);
        if (test.size() < 2) // Allow size > 2; extra stuff ignored (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        string scriptSigString = test[0].get_str();
        CScript scriptSig = ParseScript(scriptSigString);
        string scriptPubKeyString = test[1].get_str();
        CScript scriptPubKey = ParseScript(scriptPubKeyString);

        // int flagsNow = flags;
        // if (test.size() > 3 && ("," + test[2].get_str() + ",").find(",DERSIG,") != string::npos) {
        //     flagsNow |= SCRIPT_VERIFY_DERSIG;
        // }

        CTransaction tx;
        map<vector<unsigned char>, vector<unsigned char>> mapPubKey;
        BOOST_CHECK_MESSAGE(VerifyScript(scriptSig, scriptPubKey, tx, 0, flags, SIGHASH_NONE, mapPubKey), strTest);
    }
}

BOOST_AUTO_TEST_CASE(script_invalid_long)
{
    // Scripts that should evaluate as invalid
    Array tests = read_json(std::string(json_tests::script_invalid_long, json_tests::script_invalid_long + sizeof(json_tests::script_invalid_long)));

    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        string strTest = write_string(tv, false);
        if (test.size() < 2) // Allow size > 2; extra stuff ignored (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        string scriptSigString = test[0].get_str();
        CScript scriptSig = ParseScript(scriptSigString);
        string scriptPubKeyString = test[1].get_str();
        CScript scriptPubKey = ParseScript(scriptPubKeyString);
        strTest = test[2].get_str();

        CTransaction tx;
        map<vector<unsigned char>, vector<unsigned char>> mapPubKey;
        BOOST_CHECK_MESSAGE(!VerifyScript(scriptSig, scriptPubKey, tx, 0, flags, SIGHASH_NONE, mapPubKey), strTest);
    }
}

BOOST_AUTO_TEST_CASE(script_invalid)
{
    // Scripts that should evaluate as invalid
    Array tests = read_json(std::string(json_tests::script_invalid, json_tests::script_invalid + sizeof(json_tests::script_invalid)));

    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        string strTest = write_string(tv, false);
        if (test.size() < 2) // Allow size > 2; extra stuff ignored (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        string scriptSigString = test[0].get_str();
        CScript scriptSig = ParseScript(scriptSigString);
        string scriptPubKeyString = test[1].get_str();
        CScript scriptPubKey = ParseScript(scriptPubKeyString);

        // int flagsNow = flags;
        // if (test.size() > 3 && ("," + test[2].get_str() + ",").find(",DERSIG,") != string::npos) {
        //     flagsNow |= SCRIPT_VERIFY_DERSIG;
        // }

        CTransaction tx;
        map<vector<unsigned char>, vector<unsigned char>> mapPubKey;
        BOOST_CHECK_MESSAGE(!VerifyScript(scriptSig, scriptPubKey, tx, 0, flags, SIGHASH_NONE, mapPubKey), strTest);
    }
}


#if 0
CScript
sign_multisig(CScript scriptPubKey, std::vector<CKey> keys, CTransaction transaction, map<vector<unsigned char>, vector<unsigned char> >& mapstack)
{
    uint256 hash = SignatureHash(scriptPubKey, transaction, 0, SIGHASH_ALL);

    CScript result;
    //
    // NOTE: CHECKMULTISIG has an unfortunate bug; it requires
    // one extra item on the stack, before the signatures.
    // Putting OP_0 on the stack is the workaround;
    // fixing the bug would mean splitting the block chain (old
    // clients would not accept new CHECKMULTISIG transactions,
    // and vice-versa)
    //
    result << OP_0;
    BOOST_FOREACH(const CKey &key, keys)
    {
        vector<unsigned char> vchSig;
        BOOST_CHECK(key.Sign(hash, vchSig));
        vchSig.push_back((unsigned char)SIGHASH_ALL);
        result << vchSig;
    }
    return result;
}
CScript
sign_multisig(CScript scriptPubKey, const CKey &key, CTransaction transaction, map<vector<unsigned char>, vector<unsigned char> >& mapstack)
{
    std::vector<CKey> keys;
    keys.push_back(key);
    return sign_multisig(scriptPubKey, keys, transaction, mapstack);
}

BOOST_AUTO_TEST_CASE(script_CHECKMULTISIG12)
{
    map<vector<unsigned char>, vector<unsigned char>> mapPubKey;
    CKey key1, key2, key3;
    key1.MakeNewKey(true);
    key2.MakeNewKey(false);
    key3.MakeNewKey(true);

    CScript scriptPubKey12;
    scriptPubKey12 << OP_1 << key1.GetPubKey() << key2.GetPubKey() << OP_2 << OP_CHECKMULTISIG;

    CTransaction txFrom12;
    txFrom12.vout.resize(1);
    txFrom12.vout[0].scriptPubKey = scriptPubKey12;

    CTransaction txTo12;
    txTo12.vin.resize(1);
    txTo12.vout.resize(1);
    txTo12.vin[0].prevout.n = 0;
    txTo12.vin[0].prevout.hash = txFrom12.GetHash();
    txTo12.vout[0].nValue = 1;

    CScript goodsig1 = sign_multisig(scriptPubKey12, key1, txTo12, mapPubKey);
    BOOST_CHECK(VerifyScript(goodsig1, scriptPubKey12, txTo12, 0, flags, 0, mapPubKey));
    txTo12.vout[0].nValue = 2;
    BOOST_CHECK(!VerifyScript(goodsig1, scriptPubKey12, txTo12, 0, flags, 0, mapPubKey));

    CScript goodsig2 = sign_multisig(scriptPubKey12, key2, txTo12, mapPubKey);
    BOOST_CHECK(VerifyScript(goodsig2, scriptPubKey12, txTo12, 0, flags, 0, mapPubKey));

    CScript badsig1 = sign_multisig(scriptPubKey12, key3, txTo12, mapPubKey);
    BOOST_CHECK(!VerifyScript(badsig1, scriptPubKey12, txTo12, 0, flags, 0, mapPubKey));
}


BOOST_AUTO_TEST_CASE(script_CHECKMULTISIG23)
{
    map<vector<unsigned char>, vector<unsigned char>> mapPubKey;
    CKey key1, key2, key3, key4;
    key1.MakeNewKey(true);
    key2.MakeNewKey(false);
    key3.MakeNewKey(true);
    key4.MakeNewKey(false);

    CScript scriptPubKey23;
    scriptPubKey23 << OP_2 << key1.GetPubKey() << key2.GetPubKey() << key3.GetPubKey() << OP_3 << OP_CHECKMULTISIG;

    CTransaction txFrom23;
    txFrom23.vout.resize(1);
    txFrom23.vout[0].scriptPubKey = scriptPubKey23;

    CTransaction txTo23;
    txTo23.vin.resize(1);
    txTo23.vout.resize(1);
    txTo23.vin[0].prevout.n = 0;
    txTo23.vin[0].prevout.hash = txFrom23.GetHash();
    txTo23.vout[0].nValue = 1;

    std::vector<CKey> keys;
    keys.push_back(key1); keys.push_back(key2);
    CScript goodsig1 = sign_multisig(scriptPubKey23, keys, txTo23);
    BOOST_CHECK(VerifyScript(goodsig1, scriptPubKey23, txTo23, 0, flags, 0, mapPubKey));

    keys.clear();
    keys.push_back(key1); keys.push_back(key3);
    CScript goodsig2 = sign_multisig(scriptPubKey23, keys, txTo23);
    BOOST_CHECK(VerifyScript(goodsig2, scriptPubKey23, txTo23, 0, flags, 0, mapPubKey));

    keys.clear();
    keys.push_back(key2); keys.push_back(key3);
    CScript goodsig3 = sign_multisig(scriptPubKey23, keys, txTo23);
    BOOST_CHECK(VerifyScript(goodsig3, scriptPubKey23, txTo23, 0, flags, 0, mapPubKey));

    keys.clear();
    keys.push_back(key2); keys.push_back(key2); // Can't re-use sig
    CScript badsig1 = sign_multisig(scriptPubKey23, keys, txTo23);
    BOOST_CHECK(!VerifyScript(badsig1, scriptPubKey23, txTo23, 0, flags, 0, mapPubKey));

    keys.clear();
    keys.push_back(key2); keys.push_back(key1); // sigs must be in correct order
    CScript badsig2 = sign_multisig(scriptPubKey23, keys, txTo23);
    BOOST_CHECK(!VerifyScript(badsig2, scriptPubKey23, txTo23, 0, flags, 0, mapPubKey));

    keys.clear();
    keys.push_back(key3); keys.push_back(key2); // sigs must be in correct order
    CScript badsig3 = sign_multisig(scriptPubKey23, keys, txTo23);
    BOOST_CHECK(!VerifyScript(badsig3, scriptPubKey23, txTo23, 0, flags, 0, mapPubKey));

    keys.clear();
    keys.push_back(key4); keys.push_back(key2); // sigs must match pubkeys
    CScript badsig4 = sign_multisig(scriptPubKey23, keys, txTo23);
    BOOST_CHECK(!VerifyScript(badsig4, scriptPubKey23, txTo23, 0, flags, 0, mapPubKey));

    keys.clear();
    keys.push_back(key1); keys.push_back(key4); // sigs must match pubkeys
    CScript badsig5 = sign_multisig(scriptPubKey23, keys, txTo23);
    BOOST_CHECK(!VerifyScript(badsig5, scriptPubKey23, txTo23, 0, flags, 0, mapPubKey));

    keys.clear(); // Must have signatures
    CScript badsig6 = sign_multisig(scriptPubKey23, keys, txTo23);
    BOOST_CHECK(!VerifyScript(badsig6, scriptPubKey23, txTo23, 0, flags, 0, mapPubKey));
}

BOOST_AUTO_TEST_CASE(script_combineSigs)
{
    // Test the CombineSignatures function
    CBasicKeyStore keystore;
    vector<CKey> keys;
    vector<CPubKey> pubkeys;
    for (int i = 0; i < 3; i++)
    {
        CKey key;
        key.MakeNewKey(i%2 == 1);
        keys.push_back(key);
        pubkeys.push_back(key.GetPubKey());
        keystore.AddKey(key);
    }
#endif

BOOST_AUTO_TEST_SUITE_END()
