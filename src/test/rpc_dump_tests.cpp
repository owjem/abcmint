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

BOOST_AUTO_TEST_SUITE(rpc_dump_tests)


BOOST_AUTO_TEST_CASE(dumpRpc)
{
    Array tests = read_json("privkey.json");
    Value retValue;

    int i=0;
    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        // std::cout<<"====>: "<<test[0].get_str()<<std::endl;
        // std::cout<<"====>: "<<strTest<<std::endl;
        string importAccount = "wiaa"+to_string(i);
        BOOST_CHECK_NO_THROW(CallRPC("importprivkey "+test[0].get_str()+" "+importAccount +" false"));

        BOOST_CHECK_NO_THROW(retValue = CallRPC("getaddressesbyaccount " + importAccount));
        Array arr = retValue.get_array();
        BOOST_CHECK_MESSAGE(true, importAccount+" \t"+arr[0].get_str());

        std::string strTest = write_string(retValue, false);
        std::cout<<"====>: "<<strTest<<std::endl;

        BOOST_CHECK_NO_THROW(retValue = CallRPC("dumpprivkey " + arr[0].get_str()));

        // std::cout<<"====>: "<<retValue.get_str()<<std::endl;

        // BOOST_CHECK(retValue.get_str() == test[0].get_str());
    }

    i=0;
    BOOST_FOREACH(Value& tv, tests)
    {
        Array test = tv.get_array();
        // std::cout<<"====>: "<<test[0].get_str()<<std::endl;
        // std::cout<<"====>: "<<strTest<<std::endl;
        string importAccount = "wiab"+to_string(i);
        BOOST_CHECK_NO_THROW(CallRPC("importkey "+test[0].get_str()+"|"+test[1].get_str()+" "+importAccount +" false"));

        BOOST_CHECK_NO_THROW(retValue = CallRPC("getaddressesbyaccount " + importAccount));
        Array arr = retValue.get_array();
        BOOST_CHECK_MESSAGE(true, importAccount+" \t"+arr[0].get_str());
        BOOST_CHECK_EQUAL(arr[0].get_str().c_str(), "8348w1QCQJfSNv24Bfe9h4oXGQF14kBspusWmspq5VnCPs4c2");

        std::string strTest = write_string(retValue, false);
        std::cout<<"====>: "<<strTest<<std::endl;

    }

    // BOOST_CHECK_THROW(CallRPC("dumpkey"), std::runtime_error);
    // BOOST_CHECK_THROW(CallRPC("importkey"), std::runtime_error);

    // BOOST_CHECK_NO_THROW(CallRPC("getaccountaddress keyaccount1"));
    // json_spirit::Value result;
    // result = CallRPC("getaddressesbyaccount keyaccount1");
    // BOOST_CHECK_EQUAL(result.type(), json_spirit::array_type);


    // json_spirit::Array ar = result.get_array();
    // BOOST_CHECK_EQUAL(ar.size(), (unsigned int)1);
    // BOOST_CHECK_EQUAL(ar[0].type(), json_spirit::str_type);
    // result = CallRPC(std::string("dumpkey ")+ ar[0].get_str());

    // std::vector<std::string> vRet;
    // boost::split(vRet, result.get_str(), boost::is_any_of("|"));
    // BOOST_CHECK_EQUAL(vRet.size(), (unsigned int)3);
    // BOOST_CHECK_EQUAL("ffffffff00000000", vRet[2].c_str());

    // FILE * pFile;
    // pFile = fopen ("./test/data/importkey.txt","r");
    // BOOST_CHECK(pFile!=NULL);
    // fseek (pFile , 0 , SEEK_SET);
    // char* buffer = (char*) malloc (sizeof(char)*344611);
    // BOOST_CHECK(buffer!=NULL);
    // memset(buffer, 0, sizeof(char)*344611);
    // size_t ret = fread(buffer,sizeof(char),344610,pFile);
    // BOOST_CHECK_EQUAL(ret, (unsigned int)344610);
    // BOOST_CHECK_NO_THROW(CallRPC(std::string(buffer)));

    // result = CallRPC("getaddressesbyaccount win_8");
    // BOOST_CHECK_EQUAL(result.type(), json_spirit::array_type);
    // ar = result.get_array();
    // BOOST_CHECK_EQUAL(ar.size(), (unsigned int)1);
    // BOOST_CHECK_EQUAL(ar[0].type(), json_spirit::str_type);

    // BOOST_CHECK_EQUAL(ar[0].get_str().c_str(), "8348w1QCQJfSNv24Bfe9h4oXGQF14kBspusWmspq5VnCPs4c2");

    // free(buffer);

}


BOOST_AUTO_TEST_SUITE_END()
