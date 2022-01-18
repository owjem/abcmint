#include <boost/test/unit_test.hpp>
#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_writer_template.h"
#include "json/json_spirit_utils.h"
#include "data/base58_encode_decode.json.h"
#include "data/base58_keys_invalid.json.h"
#include "data/base58_keys_valid.json.h"
#include "data/wallet_decode_check.json.h"

#include "base58.h"
#include "util.h"
#include "pqcrypto/random.h"
#include "pqcrypto/pqcrypto.h"
#include "pqcrypto/sha512.h"
#include "key.h"

using namespace json_spirit;
extern Array read_json(const std::string& jsondata);

BOOST_AUTO_TEST_SUITE(base58_tests)

// Goal: test low-level base58 encoding functionality
BOOST_AUTO_TEST_CASE(base58_EncodeBase58)
{
    Array tests = read_json(std::string(json_tests::base58_encode_decode, json_tests::base58_encode_decode + sizeof(json_tests::base58_encode_decode)));
    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        std::string strTest = write_string(tv, false);
        if (test.size() < 2) // Allow for extra stuff (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        std::vector<unsigned char> sourcedata = ParseHex(test[0].get_str());
        std::string base58string = test[1].get_str();
        BOOST_CHECK_MESSAGE(
                    EncodeBase58(&sourcedata[0], &sourcedata[sourcedata.size()]) == base58string,
                    strTest);
    }
}

// Goal: test low-level base58 decoding functionality
BOOST_AUTO_TEST_CASE(base58_DecodeBase58)
{
    Array tests = read_json(std::string(json_tests::base58_encode_decode, json_tests::base58_encode_decode + sizeof(json_tests::base58_encode_decode)));
    std::vector<unsigned char> result;

    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        std::string strTest = write_string(tv, false);
        if (test.size() < 2) // Allow for extra stuff (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        std::vector<unsigned char> expected = ParseHex(test[0].get_str());
        std::string base58string = test[1].get_str();
        BOOST_CHECK_MESSAGE(DecodeBase58(base58string, result), strTest);
        BOOST_CHECK_MESSAGE(result.size() == expected.size() && std::equal(result.begin(), result.end(), expected.begin()), strTest);
    }

    BOOST_CHECK(!DecodeBase58("invalid", result));
}

BOOST_AUTO_TEST_CASE(decodeBase58Check) {
    Array tests = read_json(std::string(json_tests::wallet_decode_check, json_tests::wallet_decode_check + sizeof(json_tests::wallet_decode_check)));
    std::vector<unsigned char> result;

    CBitcoinAddress address;
	CKeyID keyID;
    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        std::string strTest = write_string(tv, false);
        if (test.size() < 2) // Allow for extra stuff (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }
        std::string sourcedata = test[0].get_str();
        std::string base58string = test[1].get_str();
        if(sourcedata.size() < 50){
            BOOST_CHECK_MESSAGE(address.SetString(sourcedata), sourcedata);
            address.GetKeyID(keyID);
            BOOST_CHECK_MESSAGE(keyID.ToString() == base58string, strTest);
        }else{
            BOOST_CHECK_MESSAGE(!address.SetString(sourcedata), sourcedata);
            address.GetKeyID(keyID);
            BOOST_CHECK_MESSAGE(!(keyID.ToString() == base58string), address.ToString());
        }
    }

}




#if 0

BOOST_AUTO_TEST_CASE(base58_public_key_address)
{
    CKey key;
	key.MakeNewKey();
	CPrivKey sk;
	sk = key.GetPrivKey();
    CPubKey pk;
	pk = key.GetPubKey();
    CKeyID id = pk.GetID();
	CBitcoinAddress addr;
	BOOST_CHECK(addr.Set(id));
	std::cout<<"key id : "<<id.ToString()<<std::endl;
    std::cout<<"key address : "<<addr.ToString()<<std::endl;
	//BOOST_CHECK(addr.SetString(addr.ToString()));
	BOOST_CHECK(addr.IsValid());
	BOOST_CHECK(!addr.IsScript());
	CKeyID getkeyID;
	BOOST_CHECK(addr.GetKeyID(getkeyID));
	std::cout<<"get key id : "<<getkeyID.ToString()<<std::endl;
	BOOST_CHECK_EQUAL(id.ToString(),  getkeyID.ToString());

	//std::string longstr = HexStr(sk);
    //std::vector<unsigned char> skhex = ParseHex(longstr);
//	std::string encodedata = EncodeBase58Check(sk);
//    CBitcoinSecret secret;
//    BOOST_CHECK(secret.SetString(encodedata));

}


// TEST(base58Test , test)
// {
   // CKey key;
	//key.MakeNewKey();
	//CPrivKey sk;
	//sk = key.GetPrivKey();
	//std::string longstr = HexStr(sk);
	//std::vector<unsigned char> skhex = ParseHex(longstr);

	//int64_t nStart = GetTimeMicros();
	//std::string encodedata = EncodeBase58Check(sk);
    //int64_t nTime = GetTimeMicros() - nStart;
	//std::cout<<"encodeTime: "<<nTime<<std::endl;
	//std::vector<unsigned char> result;
	//nStart = GetTimeMicros();
	//BOOST_CHECK(DecodeBase58(encodedata, result));
	//nTime = GetTimeMicros() - nStart;
	//std::cout<<"decodeTime: "<<nTime<<std::endl;
	//CBitcoinSecret secret;
	//bool b = secret.SetString(encodedata);

// }
#endif

BOOST_AUTO_TEST_CASE(base58_pqcSha256)
{
    Array tests = read_json(std::string(json_tests::base58_encode_decode, json_tests::base58_encode_decode + sizeof(json_tests::base58_encode_decode)));
    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        std::string strTest = write_string(tv, false);
        if (test.size() < 2) // Allow for extra stuff (useful for comments)
        {
            BOOST_ERROR("Bad test: " << strTest);
            continue;
        }

        // SHA256
        std::vector<unsigned char> sourcedata =  ParseHex(test[0].get_str());

        std::vector<unsigned char> hash1(32);
        std::vector<unsigned char> hash2(32);

        SHA256(&sourcedata[0], sourcedata.size(), &hash1[0]);
        pqcSha256(&sourcedata[0], sourcedata.size(), &hash2[0]);

        std::string string1 = HexStr(hash1.begin(),hash1.end());
        std::string string2 = HexStr(hash2.begin(),hash2.end());
        BOOST_CHECK_MESSAGE(
                    string1 == string2,
                    strTest + " HexStr:"+ string1);
    }
}

BOOST_AUTO_TEST_SUITE_END()
