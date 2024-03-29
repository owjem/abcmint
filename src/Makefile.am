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
          OPT="$(CXXFLAGS) $(CPPFLAGS) -D__STDC_LIMIT_MACROS"
endif

BITCOIN_CONFIG_INCLUDES=-I$(builddir)/config
BITCOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS) $(CUDA_CFLAGS)

# LIBBITCOIN_CRYPTO=libbitcoin_crypto.a
LIBBITCOIN_SERVER=libbitcoin_server.a
LIBBITCOIN_COMMON=libbitcoin_common.a
LIBBITCOIN_CLI=libbitcoin_cli.a
LIBBITCOIN_CRYPTO_BASE=crypto/libbitcoin_crypto_base.a
LIBBITCOINQT=qt/libbitcoinqt.a
LIBPQCRYPTO=pqcrypto/libpqcrypto.a
LIBMINER=miner/libminer.a
# LIBGPUMINING=obj/libgpumining.a

if ENABLE_WALLET
LIBBITCOIN_WALLET=libbitcoin_wallet.a
# LIBBITCOIN_WALLET_TOOL=libbitcoin_wallet_tool.a
endif

LIBBITCOIN_CRYPTO= $(LIBBITCOIN_CRYPTO_BASE)
if ENABLE_SSE41
LIBBITCOIN_CRYPTO_SSE41 = crypto/libbitcoin_crypto_sse41.a
LIBBITCOIN_CRYPTO += $(LIBBITCOIN_CRYPTO_SSE41)
endif
if ENABLE_AVX2
LIBBITCOIN_CRYPTO_AVX2 = crypto/libbitcoin_crypto_avx2.a
LIBBITCOIN_CRYPTO += $(LIBBITCOIN_CRYPTO_AVX2)
endif
if ENABLE_X86_SHANI
LIBBITCOIN_CRYPTO_X86_SHANI = crypto/libbitcoin_crypto_x86_shani.a
LIBBITCOIN_CRYPTO += $(LIBBITCOIN_CRYPTO_X86_SHANI)
endif
if ENABLE_ARM_SHANI
LIBBITCOIN_CRYPTO_ARM_SHANI = crypto/libbitcoin_crypto_arm_shani.a
LIBBITCOIN_CRYPTO += $(LIBBITCOIN_CRYPTO_ARM_SHANI)
endif

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  $(LIBBITCOIN_CRYPTO) \
  libbitcoin_common.a \
  libbitcoin_wallet.a \
  libbitcoin_server.a \
  libbitcoin_cli.a \
  miner/libminer.a \
  pqcrypto/libpqcrypto.a
BITCOIN_INCLUDES += $(BDB_CPPFLAGS)

# noinst_LIBRARIES = libbitcoin.a libpqcrypto.a libminer.a libgpumining.a

bin_PROGRAMS =
TESTS =

  bin_PROGRAMS += abcmintd

  bin_PROGRAMS += abcmint-cli

# SUBDIRS = . $(BUILD_QT) $(BUILD_TEST)
# DIST_SUBDIRS = . qt
.PHONY: FORCE
# bitcoin core #
BITCOIN_CORE_H = \
  addrman.h \
  alert.h \
  allocators.h \
  base58.h bignum.h \
  bloom.h \
  chainparams.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  core.h \
  crypter.h \
  db.h \
  hash.h \
  init.h \
  key.h \
  keystore.h \
  leveldbwrapper.h \
  limitedmap.h \
  main.h \
  miner.h \
  mruset.h \
  netbase.h \
  net.h \
  noui.h \
  protocol.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  script.h \
  serialize.h \
  sync.h \
  threadsafety.h \
  tinyformat.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  util.h \
  version.h \
  walletdb.h \
  wallet.h \
  miner/common.h \
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
  pqcrypto/blas_config.h \
  pqcrypto/blas.h \
  pqcrypto/blas_u64.h \
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
  miner/idxlut.h \
  miner/solution.h \
  miner.h

GPUMINING_H = miner/gpumining.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libbitcoin_common_a-version.$(OBJEXT): obj/build.h

libbitcoin_server_a_LIBADD = $(LIBMINER)
# server: shared between bitcoind and bitcoin-qt
libbitcoin_server_a_CPPFLAGS = $(BITCOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS)
libbitcoin_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  checkpoints.cpp \
  coins.cpp \
  init.cpp \
  keystore.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  rpcblockchain.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  txdb.cpp \
  txmempool.cpp \
  $(JSON_H) \
  $(BITCOIN_CORE_H) $(PQCRYPTO_H) $(MINER_H) $(GPUMINING_H)

libbitcoin_wallet_a_SOURCES = \
  db.cpp \
  crypter.cpp \
  rpcdump.cpp \
  rpcwallet.cpp \
  wallet.cpp \
  walletdb.cpp \
  $(BITCOIN_CORE_H)

# crypto primitives library
crypto_libbitcoin_crypto_base_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libbitcoin_crypto_base_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libbitcoin_crypto_base_a_SOURCES = \
  crypto/aes.cpp \
  crypto/aes.h \
  crypto/common.h \
  crypto/hkdf_sha256_32.cpp \
  crypto/hkdf_sha256_32.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/poly1305.h \
  crypto/poly1305.cpp \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

if USE_ASM
crypto_libbitcoin_crypto_base_a_SOURCES += crypto/sha256_sse4.cpp
endif

crypto_libbitcoin_crypto_sse41_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libbitcoin_crypto_sse41_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libbitcoin_crypto_sse41_a_CXXFLAGS += $(SSE41_CXXFLAGS)
crypto_libbitcoin_crypto_sse41_a_CPPFLAGS += -DENABLE_SSE41
crypto_libbitcoin_crypto_sse41_a_SOURCES = crypto/sha256_sse41.cpp

crypto_libbitcoin_crypto_avx2_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libbitcoin_crypto_avx2_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libbitcoin_crypto_avx2_a_CXXFLAGS += $(AVX2_CXXFLAGS)
crypto_libbitcoin_crypto_avx2_a_CPPFLAGS += -DENABLE_AVX2
crypto_libbitcoin_crypto_avx2_a_SOURCES = crypto/sha256_avx2.cpp

crypto_libbitcoin_crypto_x86_shani_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libbitcoin_crypto_x86_shani_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libbitcoin_crypto_x86_shani_a_CXXFLAGS += $(X86_SHANI_CXXFLAGS)
crypto_libbitcoin_crypto_x86_shani_a_CPPFLAGS += -DENABLE_X86_SHANI
crypto_libbitcoin_crypto_x86_shani_a_SOURCES = crypto/sha256_x86_shani.cpp

crypto_libbitcoin_crypto_arm_shani_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libbitcoin_crypto_arm_shani_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libbitcoin_crypto_arm_shani_a_CXXFLAGS += $(ARM_SHANI_CXXFLAGS)
crypto_libbitcoin_crypto_arm_shani_a_CPPFLAGS += -DENABLE_ARM_SHANI
crypto_libbitcoin_crypto_arm_shani_a_SOURCES = crypto/sha256_arm_shani.cpp

# common: shared between bitcoind, and bitcoin-qt and non-server tools
# libbitcoin_a_CPPFLAGS = $(BITCOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(CUDA_CFLAGS) $(CUDA_LDFLAGS)
libbitcoin_common_a_CPPFLAGS = $(BITCOIN_INCLUDES)
libbitcoin_common_a_SOURCES = \
  base58.cpp \
  allocators.cpp \
  chainparams.cpp \
  core.cpp \
  miner/common.cpp \
  diskpubkeypos.cpp \
  hash.cpp \
  key.cpp \
  netbase.cpp \
  protocol.cpp \
  rpcprotocol.cpp \
  script.cpp \
  sync.cpp \
  util.cpp \
  version.cpp \
  $(BITCOIN_CORE_H)

nodist_libbitcoin_common_a_SOURCES = $(srcdir)/obj/build.h

# cli: shared between bitcoin-cli and bitcoin-qt
libbitcoin_cli_a_CPPFLAGS = $(BITCOIN_INCLUDES)
libbitcoin_cli_a_SOURCES = \
  rpcclient.cpp \
  $(BITCOIN_CORE_H)


pqcrypto_libpqcrypto_a_SOURCES = \
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
  miner/idxlut.cpp \
  miner/solution.cpp \
  miner.cpp \
  $(MINER_H) $(GPUMINING_H)


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

miner/kernel.inc: miner/gen_kernel.py
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
abcmintd_SOURCES = bitcoind.cpp
abcmintd_CPPFLAGS = $(BITCOIN_INCLUDES)
abcmintd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
abcmintd_SOURCES += bitcoind-res.rc
endif

abcmintd_LDADD = \
  $(LIBBITCOIN_CLI) \
  $(LIBBITCOIN_SERVER) \
  $(LIBBITCOIN_COMMON) \
  $(LIBBITCOIN_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBMINER) \
  $(LIBPQCRYPTO)


abcmintd_LDADD += $(LIBBITCOIN_WALLET)

abcmintd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS)
#

# bitcoin-cli binary #
abcmint_cli_SOURCES = bitcoin-cli.cpp
abcmint_cli_CPPFLAGS = $(BITCOIN_INCLUDES)
abcmint_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
abcmint_cli_SOURCES += bitcoin-cli-res.rc
endif

abcmint_cli_LDADD = \
  $(LIBBITCOIN_CLI) \
  $(LIBBITCOIN_COMMON) \
  $(LIBBITCOIN_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBPQCRYPTO)

abcmint_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS)
abcmint_cli_LDADD += $(BDB_LIBS) $(MINIUPNPC_LIBS)
#


if USE_GPU
abcmintd_LDADD += obj/gpumining.o
abcmintd_LDFLAGS += $(CUDA_CFLAGS) $(CUDA_LDFLAGS)
# abcmint_cli_LDADD += obj/gpumining.o
# abcmint_cli_LDFLAGS += $(CUDA_CFLAGS) $(CUDA_LDFLAGS)
endif

CTAES_DIST =  crypto/ctaes/bench.c
CTAES_DIST += crypto/ctaes/ctaes.c
CTAES_DIST += crypto/ctaes/ctaes.h
CTAES_DIST += crypto/ctaes/README.md
CTAES_DIST += crypto/ctaes/test.c





CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a *.gcda *.gcno obj/*.o

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb
EXTRA_DIST += $(CTAES_DIST)

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
