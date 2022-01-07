# include Makefile.include
AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS)
# AM_CPPFLAGS =  $(INCLUDES) -I$(top_builddir)/src/obj \
#   -I$(top_srcdir)/src/leveldb/include -I$(top_srcdir)/src/leveldb/helpers \
#   -I$(builddir) $(BOOST_INCLUDES) $(BDB_CPPFLAGS) \
#   -I/usr/local/cuda/include/
# AM_LDFLAGS = $(PTHREAD_CFLAGS)


if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(CXXFLAGS) $(CPPFLAGS)"
endif

BITCOIN_CONFIG_INCLUDES=-I$(builddir)/config
BITCOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS) $(CUDA_CFLAGS)

LIBBITCOIN=libbitcoin.a
LIBBITCOINQT=qt/libbitcoinqt.a
LIBPQCRYPTO=pqcrypto/libpqcrypto.a
LIBMINER=miner/libminer.a
# LIBGPUMINING=obj/libgpumining.a

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  libbitcoin.a \
  miner/libminer.a \
  pqcrypto/libpqcrypto.a
BITCOIN_INCLUDES += $(BDB_CPPFLAGS)

# noinst_LIBRARIES = libbitcoin.a libpqcrypto.a libminer.a libgpumining.a

bin_PROGRAMS =
TESTS =
  bin_PROGRAMS += bitcoind

# SUBDIRS = . $(BUILD_QT) $(BUILD_TEST)
# DIST_SUBDIRS = . qt
.PHONY: FORCE
# bitcoin core #
BITCOIN_CORE_H = \
  addrman.h \
  alert.h \
  allocators.h \
  base58.h \
  bignum.h \
  bitcoinrpc.h \
  bloom.h \
  chainparams.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  compat.h \
  core.h \
  crypter.h \
  db.h \
  hash.h \
  init.h \
  key.h \
  keystore.h \
  leveldb.h \
  limitedmap.h \
  main.h \
  miner.h \
  mruset.h \
  netbase.h \
  net.h \
  protocol.h \
  script.h \
  serialize.h \
  sync.h \
  threadsafety.h \
  txdb.h \
  ui_interface.h \
  uint256.h \
  util.h \
  version.h \
  wallet.h \
  walletdb.h \
  idxlut.h \
  diskpubkeypos.h

JSON_H = \
  json/json_spirit.h \
  json/json_spirit_error_position.h \
  json/json_spirit_reader.h \
  json/json_spirit_reader_template.h \
  json/json_spirit_stream_reader.h \
  json/json_spirit_utils.h \
  json/json_spirit_value.h \
  json/json_spirit_writer.h \
  json/json_spirit_writer_template.h

PQCRYPTO_H = \
  pqcrypto/aes.h \
  pqcrypto/api.h \
  pqcrypto/blas_config.h \
  pqcrypto/blas.h \
  pqcrypto/blas_u64.h \
  pqcrypto/ctaes.h \
  pqcrypto/gf16.h \
  pqcrypto/hash_len_config.h \
  pqcrypto/hash_utils.h \
  pqcrypto/mpkc_config.h \
  pqcrypto/mpkc.h \
  pqcrypto/pqcrypt_argchk.h \
  pqcrypto/pqcrypt_cfg.h \
  pqcrypto/pqcrypt_cipher.h \
  pqcrypto/pqcrypt_macros.h \
  pqcrypto/pqcrypto.h \
  pqcrypto/pqcrypt_prng.h \
  pqcrypto/pqcrypt_util.h \
  pqcrypto/rainbow_16.h \
  pqcrypto/rainbow_config.h \
  pqcrypto/random.h \
  pqcrypto/rng.h \
  pqcrypto/sha256.h \
  pqcrypto/sha512.h

MINER_H = \
  miner/common.h \
  miner/gpumining.h \
  miner/idxlut.h \
  miner/solution.h \
  miner.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libbitcoin_a-version.$(OBJEXT): obj/build.h

libbitcoin_a_LIBADD = $(LIBMINER)

libbitcoin_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bitcoinrpc.cpp \
  bloom.cpp \
  chainparams.cpp \
  checkpoints.cpp \
  core.cpp \
  crypter.cpp \
  db.cpp \
  hash.cpp \
  init.cpp \
  key.cpp \
  keystore.cpp \
  leveldb.cpp \
  main.cpp \
  netbase.cpp \
  net.cpp \
  noui.cpp \
  protocol.cpp \
  rpcblockchain.cpp \
  rpcdump.cpp \
  rpcmining.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcwallet.cpp \
  script.cpp \
  sync.cpp \
  txdb.cpp \
  util.cpp \
  version.cpp \
  wallet.cpp \
  walletdb.cpp \
  diskpubkeypos.cpp \
  $(JSON_H) \
  $(BITCOIN_CORE_H) $(PQCRYPTO_H) $(MINER_H) $(GPUMINING_H)

pqcrypto_libpqcrypto_a_SOURCES = \
  pqcrypto/aes.cpp \
  pqcrypto/ctaes.cpp \
  pqcrypto/fortuna.cpp \
  pqcrypto/hash_utils.cpp \
  pqcrypto/pqcrypt_argchk.cpp \
  pqcrypto/rainbow_16.cpp \
  pqcrypto/random.cpp \
  pqcrypto/rijndael.cpp \
  pqcrypto/rijndael_tab.cpp \
  pqcrypto/rng.cpp \
  pqcrypto/sha256.cpp \
  pqcrypto/sign.cpp \
  $(PQCRYPTO_H)

miner_libminer_a_SOURCES = \
  miner/common.cpp \
  miner/idxlut.cpp \
  miner/solution.cpp \
  miner.cpp \
  $(MINER_H)


# nodist_libbitcoin_a_SOURCES = $(srcdir)/obj/build.h
#


if USE_GPU

# K = 20
# UNROLL = 8
# NVCC = /usr/local/cuda/bin/nvcc
# NVCFLAGS =  --resource-usage -lineinfo -DPARK=$(K)
# OMPFLAGS ?= -fopenmp -lomp
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

# GPUMINING_LIBS = -L/usr/local/cuda/lib64/ -lcudadevrt -lcudart

# miner/kernel.inc: miner/gen_kernel.py
# 	./miner/gen_kernel.py -k $(K) --unroll=$(UNROLL) > ./miner/kernel.inc
# miner/gpumining.o: miner/gpumining.cu  miner/kernel.inc
# 	$(NVCC) -c $(NVCFLAGS) $< -o $@
# libgpumining.a:miner/gpumining.o
# 	ar rcs $@ $^

# AM_CPPFLAGS=$(CUDA_CFLAGS)
# deviceQuery_LDADD=$(CUDA_LDFLAGS)
# obj/build.h: FORCE
# 	@$(MKDIR_P) $(builddir)/obj
# 	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
# 	  $(abs_top_srcdir)
# libbitcoin_a-clientversion.$(OBJEXT): obj/build.h

GPUMINING_H = miner/gpumining.h

miner/kernel.inc: miner/gen_kernel.py FORCE
	@$(top_srcdir)/src/miner/gen_kernel.py -k $(CUDA_K) --unroll=$(CUDA_UNROLL) > $(abs_top_builddir)/src/miner/kernel.inc
obj/gpumining.o: miner/gpumining.cu miner/kernel.inc
	$(NVCC) -c $(NVCC_CFLAGS) $(BITCOIN_CONFIG_INCLUDES) $< -o $@
# obj/libgpumining.a:obj/gpumining.o
# 	ar rcs $@ $^
# miner_libminer_a-miner.$(OBJEXT): obj/gpumining.o obj/libgpumining.a

# miner_libgpumining_a-gpumining.$(OBJEXT): miner/gpumining.cu
# 	$(NVCC) -c $(NVCC_CFLAGS) $< -o $@

# miner_libgpumining_a_CXX = $(NVCC)

# miner_libgpumining_a_SOURCES = miner/gpumining.cu obj/kernel.inc $(GPUMINING_H)

# miner_libgpumining_a_CPPFLAGS = $(GPUMINING_LIBS) $(CUDA_CFLAGS) $(CUDA_LDFLAGS)
# miner_libgpumining_a_LDFLAGS = $(CUDA_LDFLAGS)

# libminer_a_CPPFLAGS = $(GPUMINING_LIBS)
miner_libminer_a_LIBADD = obj/gpumining.o
miner_libminer_a_CPPFLAGS = $(CUDA_CFLAGS) $(CUDA_LDFLAGS)

endif

# bitcoind binary #
bitcoind_LDADD = \
  $(LIBBITCOIN) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBMINER) \
  $(LIBPQCRYPTO)

bitcoind_SOURCES = bitcoind.cpp
#

if TARGET_WINDOWS
bitcoind_SOURCES += bitcoin-res.rc
endif

bitcoind_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) obj/gpumining.o
libbitcoin_a_CPPFLAGS = $(BITCOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(CUDA_CFLAGS) $(CUDA_LDFLAGS)
bitcoind_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)  $(CUDA_CFLAGS) $(CUDA_LDFLAGS)










CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a *.gcda *.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	rm -f leveldb/*/*.gcno leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(CXXFLAGS)  -c -o $@ $<

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)


include Makefile.test.include
