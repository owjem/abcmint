include $(top_srcdir)/src/Makefile.include

AM_CPPFLAGS =  $(INCLUDES) -I$(top_builddir)/src/obj \
  -I$(top_srcdir)/src/leveldb/include -I$(top_srcdir)/src/leveldb/helpers \
  -I$(top_srcdir)/src $(BOOST_INCLUDES) $(BDB_CPPFLAGS) \
  -I/usr/local/cuda/include/ -I$(top_srcdir)/src/miner \
  -I$(top_srcdir)/src/pqcrypto

AM_LDFLAGS = $(PTHREAD_CFLAGS)

bin_PROGRAMS = test_bitcoin

TESTS = test_bitcoin

JSON_TEST_FILES= data/script_valid.json \
  data/base58_keys_valid.json data/sig_canonical.json \
  data/sig_noncanonical.json \
  data/base58_encode_decode.json \
  data/base58_keys_invalid.json \
  data/script_invalid.json data/tx_invalid.json \
  data/tx_valid.json \
  data/wallet_decode_check.json

RAW_TEST_FILES = data/alertTests.raw

BUILT_SOURCES = $(JSON_TEST_FILES:.json=.json.h) $(RAW_TEST_FILES:.raw=.raw.h)

# test_bitcoin binary #
test_bitcoin_CPPFLAGS = $(AM_CPPFLAGS) $(TESTDEFS)
test_bitcoin_LDADD = $(LIBBITCOIN) $(LIBLEVELDB) $(LIBMEMENV) \
  $(BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(BDB_LIBS) \
  $(MINIUPNPC_LIBS) $(LIBMINER) $(LIBPQCRYPTO)

test_bitcoin_SOURCES = base58_tests.cpp script_tests.cpp test_bitcoin.cpp \
$(JSON_TEST_FILES) $(RAW_TEST_FILES)

nodist_test_bitcoin_SOURCES = $(BUILT_SOURCES)

CLEANFILES = *.gcda *.gcno $(BUILT_SOURCES)
