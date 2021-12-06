#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>
#include <string>

#include "base58.h"
#include "util.h"
#include "bitcoinrpc.h"
#include "wallet.h"

using namespace std;
using namespace json_spirit;

extern Array createArgs(int nRequired, const char* address1 = NULL, const char* address2 = NULL);
extern Value CallRPC(string args);
extern Array read_json(const std::string& filename);

extern CWallet* pwalletMain;

BOOST_AUTO_TEST_SUITE(rpc_wallet_tests)

// BOOST_AUTO_TEST_CASE(rpc_addmultisig)
// {
//     LOCK(pwalletMain->cs_wallet);

//     rpcfn_type addmultisig = tableRPC["addmultisigaddress"]->actor;

//     // old, 65-byte-long:
//     const char address1Hex[] = "0434e3e09f49ea168c5bbf53f877ff4206923858aab7c7e1df25bc263978107c95e35065a27ef6f1b27222db0ec97e0e895eaca603d3ee0d4c060ce3d8a00286c8";
//     // new, compressed:
//     const char address2Hex[] = "0388c2037017c62240b6b72ac1a2a5f94da790596ebd06177c8572752922165cb4";

//     Value v;
//     CBitcoinAddress address;
//     BOOST_CHECK_NO_THROW(v = addmultisig(createArgs(1, address1Hex), false));
//     address.SetString(v.get_str());
//     BOOST_CHECK(address.IsValid() && address.IsScript());

//     BOOST_CHECK_NO_THROW(v = addmultisig(createArgs(1, address1Hex, address2Hex), false));
//     address.SetString(v.get_str());
//     BOOST_CHECK(address.IsValid() && address.IsScript());

//     BOOST_CHECK_NO_THROW(v = addmultisig(createArgs(2, address1Hex, address2Hex), false));
//     address.SetString(v.get_str());
//     BOOST_CHECK(address.IsValid() && address.IsScript());

//     BOOST_CHECK_THROW(addmultisig(createArgs(0), false), runtime_error);
//     BOOST_CHECK_THROW(addmultisig(createArgs(1), false), runtime_error);
//     BOOST_CHECK_THROW(addmultisig(createArgs(2, address1Hex), false), runtime_error);

//     BOOST_CHECK_THROW(addmultisig(createArgs(1, ""), false), runtime_error);
//     BOOST_CHECK_THROW(addmultisig(createArgs(1, "NotAValidPubkey"), false), runtime_error);

//     string short1(address1Hex, address1Hex + sizeof(address1Hex) - 2); // last byte missing
//     BOOST_CHECK_THROW(addmultisig(createArgs(2, short1.c_str()), false), runtime_error);

//     string short2(address1Hex + 1, address1Hex + sizeof(address1Hex)); // first byte missing
//     BOOST_CHECK_THROW(addmultisig(createArgs(2, short2.c_str()), false), runtime_error);
// }

BOOST_AUTO_TEST_CASE(rpc_wallet)
{
    // Test RPC calls for various wallet statistics
    Value r;

    LOCK2(cs_main, pwalletMain->cs_wallet);

    CPubKey demoPubkey = pwalletMain->GenerateNewKey();
    CBitcoinAddress demoAddress = CBitcoinAddress(CTxDestination(demoPubkey.GetID()));
    Value retValue;
    string strAccount = "walletDemoAccount";
    // string strPurpose = "receive";
    BOOST_CHECK_NO_THROW({ /*Initialize Wallet with an account */
        CWalletDB walletdb(pwalletMain->strWalletFile);
        CAccount account;
        account.vchPubKey = demoPubkey;
        pwalletMain->SetAddressBookName(account.vchPubKey.GetID(), strAccount);
        walletdb.WriteAccount(strAccount, account);
    });

    CPubKey setaccountDemoPubkey = pwalletMain->GenerateNewKey();
    CBitcoinAddress setaccountDemoAddress = CBitcoinAddress(CTxDestination(setaccountDemoPubkey.GetID()));

    /*********************************
     * 			setaccount
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("setaccount " + setaccountDemoAddress.ToString() + " nullaccount"));
    /* 8LGFCoETPkrArFP9aR78rnHoVvausbAc6DM4CYNCC6dHtji3p is not owned by the test wallet. */
    // BOOST_CHECK_THROW(CallRPC("setaccount 8LGFCoETPkrArFP9aR78rnHoVvausbAc6DM4CYNCC6dHtji3p nullaccount"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("setaccount"), runtime_error);
    /* 8LGFCoETPkrArFP9aR78rnHoVvausbAc6DM4CYNCC6dHtji3 (48 chars) is an illegal address (should be 49 chars) */
    BOOST_CHECK_THROW(CallRPC("setaccount 8LGFCoETPkrArFP9aR78rnHoVvausbAc6DM4CYNCC6dHtji3 nullaccount"), runtime_error);


    /*********************************
     *                  getbalance
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("getbalance"));
    BOOST_CHECK_NO_THROW(CallRPC("getbalance " + demoAddress.ToString()));

    /*********************************
     * 			listunspent
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("listunspent"));
    BOOST_CHECK_THROW(CallRPC("listunspent string"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("listunspent 0 string"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("listunspent 0 1 not_array"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("listunspent 0 1 [] extra"), runtime_error);
    BOOST_CHECK_NO_THROW(r = CallRPC("listunspent 0 1 []"));
    BOOST_CHECK(r.get_array().empty());

    /*********************************
     * 		listreceivedbyaddress
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("listreceivedbyaddress"));
    BOOST_CHECK_NO_THROW(CallRPC("listreceivedbyaddress 0"));
    BOOST_CHECK_THROW(CallRPC("listreceivedbyaddress not_int"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("listreceivedbyaddress 0 not_bool"), runtime_error);
    BOOST_CHECK_NO_THROW(CallRPC("listreceivedbyaddress 0 true"));
    BOOST_CHECK_THROW(CallRPC("listreceivedbyaddress 0 true extra"), runtime_error);

    /*********************************
     * 		listreceivedbyaccount
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("listreceivedbyaccount"));
    BOOST_CHECK_NO_THROW(CallRPC("listreceivedbyaccount 0"));
    BOOST_CHECK_THROW(CallRPC("listreceivedbyaccount not_int"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("listreceivedbyaccount 0 not_bool"), runtime_error);
    BOOST_CHECK_NO_THROW(CallRPC("listreceivedbyaccount 0 true"));
    BOOST_CHECK_THROW(CallRPC("listreceivedbyaccount 0 true extra"), runtime_error);

    /*********************************
     *          listsinceblock
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("listsinceblock"));

    /*********************************
     *          listtransactions
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("listtransactions"));
    BOOST_CHECK_NO_THROW(CallRPC("listtransactions " + demoAddress.ToString()));
    BOOST_CHECK_NO_THROW(CallRPC("listtransactions " + demoAddress.ToString() + " 20"));
    BOOST_CHECK_NO_THROW(CallRPC("listtransactions " + demoAddress.ToString() + " 20 0"));
    BOOST_CHECK_THROW(CallRPC("listtransactions " + demoAddress.ToString() + " not_int"), runtime_error);

    /*********************************
     *          listlockunspent
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("listlockunspent"));

    /*********************************
     *          listaccounts
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("listaccounts"));

    /*********************************
     *          listaddressgroupings
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("listaddressgroupings"));

    /*********************************
     * 		getrawchangeaddress
     *********************************/
    // BOOST_CHECK_NO_THROW(CallRPC("getrawchangeaddress"));

    /*********************************
     * 		getnewaddress
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("getnewaddress"));
    BOOST_CHECK_NO_THROW(CallRPC("getnewaddress getnewaddress_demoaccount"));

    /*********************************
     * 		getaccountaddress
     *********************************/
    BOOST_CHECK_NO_THROW(CallRPC("getaccountaddress \"\""));
    BOOST_CHECK_NO_THROW(CallRPC("getaccountaddress accountThatDoesntExists")); // Should generate a new account
    BOOST_CHECK_NO_THROW(retValue = CallRPC("getaccountaddress " + strAccount));
    BOOST_CHECK(CBitcoinAddress(retValue.get_str()).Get() == demoAddress.Get());

    /*********************************
     * 			getaccount
     *********************************/
    BOOST_CHECK_THROW(CallRPC("getaccount"), runtime_error);
    BOOST_CHECK_NO_THROW(CallRPC("getaccount " + demoAddress.ToString()));

    /*********************************
     * 	signmessage + verifymessage
     *********************************/
    BOOST_CHECK_NO_THROW(retValue = CallRPC("signmessage " + demoAddress.ToString() + " mymessage"));
    BOOST_CHECK_THROW(CallRPC("signmessage"), runtime_error);
    /* Should throw error because this address is not loaded in the wallet */
    BOOST_CHECK_THROW(CallRPC("signmessage 8LGFCoETPkrArFP9aR78rnHoVvausbAc6DM4CYNCC6dHtji3p mymessage"), runtime_error);

    /* missing arguments */
    BOOST_CHECK_THROW(CallRPC("verifymessage " + demoAddress.ToString()), runtime_error);
    BOOST_CHECK_THROW(CallRPC("verifymessage " + demoAddress.ToString() + " " + retValue.get_str()), runtime_error);
    /* Illegal address */
    BOOST_CHECK_THROW(CallRPC("verifymessage 8LGFCoETPkrArFP9aR78rnHoVvausbAc6DM4CYNCC6dHtji3p " + retValue.get_str() + " mymessage"), runtime_error);
    /* wrong address */
    // BOOST_CHECK(CallRPC("verifymessage 8LGFCoETPkrArFP9aR78rnHoVvausbAc6DM4CYNCC6dHtji3px " + retValue.get_str() + " mymessage").get_bool() == false);
    /* Correct address and signature but wrong message */
    BOOST_CHECK(CallRPC("verifymessage " + demoAddress.ToString() + " " + retValue.get_str() + " wrongmessage").get_bool() == false);
    /* Correct address, message and signature*/
    BOOST_CHECK(CallRPC("verifymessage " + demoAddress.ToString() + " " + retValue.get_str() + " mymessage").get_bool() == true);

    /*********************************
     * 		getaddressesbyaccount
     *********************************/
    BOOST_CHECK_THROW(CallRPC("getaddressesbyaccount"), runtime_error);
    BOOST_CHECK_NO_THROW(retValue = CallRPC("getaddressesbyaccount " + strAccount));
    Array arr = retValue.get_array();
    BOOST_CHECK(arr.size() > 0);
    BOOST_CHECK(CBitcoinAddress(arr[0].get_str()).Get() == demoAddress.Get());

}


BOOST_AUTO_TEST_CASE(rpc_rawparams)
{
    // Test raw transaction API argument handling
    Value r;

    BOOST_CHECK_THROW(CallRPC("getrawtransaction"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("getrawtransaction not_hex"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("getrawtransaction a3b807410df0b60fcb9736768df5823938b2f838694939ba45f3c0a1bff150ed not_int"), runtime_error);

    BOOST_CHECK_NO_THROW(CallRPC("listunspent"));
    BOOST_CHECK_THROW(CallRPC("listunspent string"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("listunspent 0 string"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("listunspent 0 1 not_array"), runtime_error);
    BOOST_CHECK_NO_THROW(r=CallRPC("listunspent 0 1 []"));
    BOOST_CHECK_THROW(r=CallRPC("listunspent 0 1 [] extra"), runtime_error);
    BOOST_CHECK(r.get_array().empty());
    BOOST_CHECK_THROW(CallRPC("createrawtransaction"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("createrawtransaction null null"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("createrawtransaction not_array"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("createrawtransaction [] []"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("createrawtransaction {} {}"), runtime_error);
    BOOST_CHECK_NO_THROW(CallRPC("createrawtransaction [] {}"));
    BOOST_CHECK_THROW(CallRPC("createrawtransaction [] {} extra"), runtime_error);

    BOOST_CHECK_THROW(CallRPC("decoderawtransaction"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("decoderawtransaction null"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("decoderawtransaction DEADBEEF"), runtime_error);
    string rawtx = "01000000010b30d2994962b03b48cfceb18b4bdc519974d3d29d6cf6ed31854de85b6cc9e50000000000ffffffff0100e40b54020000002576aa20d1ac643e112b6f1d9539bb943104d00b932808a3073783108490051635f976ea88ac00000000";
    BOOST_CHECK_NO_THROW(r = CallRPC(string("decoderawtransaction ")+rawtx));
    BOOST_CHECK_EQUAL(find_value(r.get_obj(), "version").get_int(), 1);
    BOOST_CHECK_EQUAL(find_value(r.get_obj(), "locktime").get_int(), 0);
    BOOST_CHECK_THROW(r = CallRPC(string("decoderawtransaction ")+rawtx+" extra"), runtime_error);

    BOOST_CHECK_THROW(CallRPC("signrawtransaction"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("signrawtransaction null"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("signrawtransaction ff00"), runtime_error);
    BOOST_CHECK_NO_THROW(CallRPC(string("signrawtransaction ")+rawtx));
    BOOST_CHECK_NO_THROW(CallRPC(string("signrawtransaction ")+rawtx+" null null NONE|ANYONECANPAY"));
    BOOST_CHECK_NO_THROW(CallRPC(string("signrawtransaction ")+rawtx+" [] [] NONE|ANYONECANPAY"));
    BOOST_CHECK_THROW(CallRPC(string("signrawtransaction ")+rawtx+" null null badenum"), runtime_error);

    // Only check failure cases for sendrawtransaction, there's no network to send to...
    BOOST_CHECK_THROW(CallRPC("sendrawtransaction"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("sendrawtransaction null"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("sendrawtransaction DEADBEEF"), runtime_error);
    BOOST_CHECK_THROW(CallRPC(string("sendrawtransaction ")+rawtx+" extra"), runtime_error);
}

#if 0
BOOST_AUTO_TEST_CASE(rpc_rawsign)
{
    Value r;
    // input is a 1-of-2 multisig (so is output):
    string prevout =
      "[{\"txid\":\"b4cc287e58f87cdae59417329f710f3ecd75a4ee1d2872b7248f50977c8493f3\","
      "\"vout\":1,\"scriptPubKey\":\"a914b10c9df5f7edf436c697f02f1efdba4cf399615187\","
      "\"redeemScript\":\"512103debedc17b3df2badbcdd86d5feb4562b86fe182e5998abd8bcd4f122c6155b1b21027e940bb73ab8732bfdf7f9216ecefca5b94d6df834e77e108f68e66f126044c052ae\"}]";
    r = CallRPC(string("createrawtransaction ")+prevout+" "+
      "{\"3HqAe9LtNBjnsfM4CyYaWTnvCaUYT7v4oZ\":11}");
    string notsigned = r.get_str();
    string privkey1 = "\"KzsXybp9jX64P5ekX1KUxRQ79Jht9uzW7LorgwE65i5rWACL6LQe\"";
    string privkey2 = "\"Kyhdf5LuKTRx4ge69ybABsiUAWjVRK4XGxAKk2FQLp2HjGMy87Z4\"";
    r = CallRPC(string("signrawtransaction ")+notsigned+" "+prevout+" "+"[]");
    BOOST_CHECK(find_value(r.get_obj(), "complete").get_bool() == false);
    r = CallRPC(string("signrawtransaction ")+notsigned+" "+prevout+" "+"["+privkey1+","+privkey2+"]");
    BOOST_CHECK(find_value(r.get_obj(), "complete").get_bool() == true);
}

TEST(rpcTest, network)
{
    ASSERT_NO_THROW(CallRPC("getpeerinfo"));
    ASSERT_THROW(CallRPC("getpeerinfo false"), std::runtime_error);

    json_spirit::Value result;
    result = CallRPC("getconnectioncount");
    ASSERT_EQ(result.get_int(), 0);

    ASSERT_NO_THROW(CallRPC("addnode 192.168.1.1 add"));

    result = CallRPC("getaddednodeinfo false");
    ASSERT_STREQ(find_value(result.get_obj(), "addednode").get_str().c_str(), "192.168.1.1");

    ASSERT_THROW(CallRPC("addnode 192.168.1.1 add"), std::runtime_error);
    ASSERT_NO_THROW(CallRPC("addnode 192.168.1.1 remove"));

}

TEST(rpcTest, mining)
{
    json_spirit::Value result;
    result = CallRPC("getgenerate");
    ASSERT_EQ(result.get_bool(), false);

    ASSERT_THROW(CallRPC("setgenerate"), std::runtime_error);
    ASSERT_NO_THROW(CallRPC("setgenerate true"));

    ASSERT_THROW(CallRPC("getgenerate 1"), std::runtime_error);
    result = CallRPC("getgenerate");
    ASSERT_EQ(result.get_bool(), true);

    ASSERT_THROW(CallRPC("getmininginfo extra"), std::runtime_error);
    result = CallRPC("getmininginfo");
    ASSERT_EQ(find_value(result.get_obj(), "generate").get_bool(), true);
    ASSERT_EQ(find_value(result.get_obj(), "difficulty").get_int(), 41);

    ASSERT_THROW(CallRPC("getwork true"), std::runtime_error);
    ASSERT_THROW(CallRPC("getblocktemplate true"), std::runtime_error);

    ASSERT_THROW(CallRPC("getwork"), std::runtime_error); //not in server mode, should receive runtime_error
    ASSERT_THROW(CallRPC("getblocktemplate"), std::runtime_error); //not in server mode, should receive runtime_error
}
/*
TEST(rpcTest, publickeypos)
{
    setupWalletTest();
    //an address not exist
    ASSERT_THROW(CallRPC("getpublickeypos 8A778HtteQY3PeZs8J9w2KJGVMd9M4jpFSv2QNZNXspaM2EGt"), std::runtime_error);

    //create a new key/address
    ASSERT_NO_THROW(CallRPC("getaccountaddress publickeypos"));
    json_spirit::Value result;
    result = CallRPC("getaddressesbyaccount publickeypos");
    ASSERT_EQ(result.type(), json_spirit::array_type);

    json_spirit::Array ar = result.get_array();
    ASSERT_EQ(ar.size(), (unsigned int)1);
    ASSERT_EQ(ar[0].type(), json_spirit::str_type);
    //position is null
    ASSERT_THROW(CallRPC(std::string("getpublickeypos ")+ ar[0].get_str()), std::runtime_error);

}

*/
#endif

BOOST_AUTO_TEST_SUITE_END()
