include Makefile.include
AM_CPPFLAGS =  $(INCLUDES) -I$(top_builddir)/src/obj \
  -I$(top_srcdir)/src/leveldb/include -I$(top_srcdir)/src/leveldb/helpers \
  -I$(builddir) $(BOOST_INCLUDES) $(BDB_CPPFLAGS) \
  -I/usr/local/cuda/include/
AM_LDFLAGS = $(PTHREAD_CFLAGS)

noinst_LIBRARIES = libbitcoin.a libpqcrypto.a libminer.a libgpumining.a

bin_PROGRAMS = bitcoind

SUBDIRS = . $(BUILD_QT) $(BUILD_TEST)
DIST_SUBDIRS = . qt test
.PHONY: FORCE
# bitcoin core #
BITCOIN_CORE_H = addrman.h alert.h allocators.h base58.h bignum.h \
  bitcoinrpc.h bloom.h chainparams.h checkpoints.h checkqueue.h \
  clientversion.h compat.h core.h crypter.h db.h hash.h init.h \
  key.h keystore.h leveldb.h limitedmap.h main.h miner.h mruset.h \
  netbase.h net.h protocol.h script.h serialize.h sync.h threadsafety.h \
  txdb.h ui_interface.h uint256.h util.h version.h walletdb.h wallet.h \
  idxlut.h diskpubkeypos.h

JSON_H = json/json_spirit.h json/json_spirit_error_position.h \
  json/json_spirit_reader.h json/json_spirit_reader_template.h \
  json/json_spirit_stream_reader.h json/json_spirit_utils.h \
  json/json_spirit_value.h json/json_spirit_writer.h \
  json/json_spirit_writer_template.h

PQCRYPTO_H = pqcrypto/aes.h pqcrypto/api.h pqcrypto/blas_config.h \
  pqcrypto/blas.h pqcrypto/blas_u64.h pqcrypto/ctaes.h \
  pqcrypto/gf16.h pqcrypto/hash_len_config.h pqcrypto/hash_utils.h \
  pqcrypto/mpkc_config.h pqcrypto/mpkc.h pqcrypto/pqcrypt_argchk.h \
  pqcrypto/pqcrypt_cfg.h pqcrypto/pqcrypt_cipher.h pqcrypto/pqcrypt_macros.h \
  pqcrypto/pqcrypto.h pqcrypto/pqcrypt_prng.h pqcrypto/pqcrypt_util.h \
  pqcrypto/rainbow_16.h pqcrypto/rainbow_config.h pqcrypto/random.h \
  pqcrypto/rng.h pqcrypto/sha256.h pqcrypto/sha512.h

MINER_H = miner/common.h miner/gpumining.h  miner/idxlut.h miner/solution.h

obj/build.h: FORCE
	@$(MKDIR_P) $(abs_top_builddir)/src/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
version.o: obj/build.h

libbitcoin_a_SOURCES = addrman.cpp alert.cpp bitcoinrpc.cpp bloom.cpp \
  chainparams.cpp checkpoints.cpp core.cpp crypter.cpp db.cpp hash.cpp \
  init.cpp key.cpp keystore.cpp leveldb.cpp main.cpp miner.cpp \
  netbase.cpp net.cpp noui.cpp protocol.cpp rpcblockchain.cpp rpcdump.cpp \
  rpcmining.cpp rpcnet.cpp rpcrawtransaction.cpp rpcwallet.cpp script.cpp \
  sync.cpp txdb.cpp util.cpp version.cpp wallet.cpp walletdb.cpp \
  diskpubkeypos.cpp $(JSON_H) \
  $(BITCOIN_CORE_H) $(PQCRYPTO_H) $(MINER_H) $(GPUMINING_H)

libpqcrypto_a_SOURCES = pqcrypto/aes.cpp pqcrypto/ctaes.cpp \
  pqcrypto/fortuna.cpp pqcrypto/hash_utils.cpp pqcrypto/pqcrypt_argchk.cpp \
  pqcrypto/rainbow_16.cpp pqcrypto/random.cpp pqcrypto/rijndael.cpp \
  pqcrypto/rijndael_tab.cpp pqcrypto/rng.cpp pqcrypto/sha256.cpp pqcrypto/sign.cpp $(PQCRYPTO_H)

libminer_a_SOURCES = miner/common.cpp miner/idxlut.cpp miner/solution.cpp $(MINER_H)


nodist_libbitcoin_a_SOURCES = $(top_srcdir)/src/obj/build.h
#


if USE_GPU

K = 20
UNROLL = 8
NVCC = /usr/local/cuda/bin/nvcc
NVCFLAGS =  --resource-usage -lineinfo -DPARK=$(K)
OMPFLAGS ?= -fopenmp -lomp
#LIBS += -L/usr/local/cuda/lib64 -l cudart
#LIBS +=-l cudart_static.a
# DEFS += -I/usr/local/cuda/include/
# LIBS += -lrt /usr/local/cuda-9.2/lib64/libcudart_static.a
# LIBS += -L/usr/local/cuda/lib64/
#dynamic
# LIBS += -lcudadevrt -lcudart
# static
# LIBS += -lcudadevrt -lcudart_static -lrt -pthread -ldl -lc -lpthread -Bstatic -lgcc -lstdc++
#LIBS += $$PWD/obj/libgpumining.a

GPUMINING_LIBS = -L/usr/local/cuda/lib64/ -lcudadevrt -lcudart
GPUMINING_H = miner/gpumining.h

miner/kernel.inc: miner/gen_kernel.py
	./miner/gen_kernel.py -k $(K) --unroll=$(UNROLL) > ./miner/kernel.inc
obj/gpumining.o: miner/gpumining.cu  miner/kernel.inc
	$(NVCC) -c $(NVCFLAGS) $< -o $@
libgpumining.a:obj/gpumining.o
	ar rcs $@ $^
endif

# bitcoind binary #
bitcoind_LDADD = libbitcoin.a libpqcrypt.a libminer.a libgpumining.a leveldb/libleveldb.a leveldb/libmemenv.a \
  $(BOOST_LIBS) $(GPUMINING_LIBS)
bitcoind_SOURCES = bitcoind.cpp
#

if TARGET_WINDOWS
bitcoind_SOURCES += bitcoin-res.rc
endif

AM_CPPFLAGS += $(BDB_CPPFLAGS)
bitcoind_LDADD += $(BDB_LIBS)

AM_CPPFLAGS += $(MINIUPNPC_CPPFLAGS)
bitcoind_LDADD += $(MINIUPNPC_LIBS)

leveldb/libleveldb.a: leveldb/libmemenv.a

leveldb/%.a:
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
	  OPT="$(CXXFLAGS) $(CPPFLAGS)"

qt/bitcoinstrings.cpp: $(libbitcoin_a_SOURCES)
	@test -n $(XGETTEXT) || echo "xgettext is required for updating translations"
	@cd $(top_srcdir); XGETTEXT=$(XGETTEXT) share/qt/extract_strings_qt.py

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a *.gcda *.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb Makefile.include

clean-local:
	-$(MAKE) -C leveldb clean
	rm -f leveldb/port/*.gcno leveldb/db/*.gcno leveldb/table/*.gcno leveldb/helpers/*.gcno
	rm -f leveldb/util/*.gcno leveldb/helpers/memenv/*.gcno
