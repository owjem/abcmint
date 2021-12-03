package=libpng
$(package)_version=1.6.37
$(package)_download_path=https://sourceforge.net/projects/libpng/files/libpng16/$($(package)_version)/
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=daeb2620d829575513e35fecc83f0d3791a620b9b93d800b763542ece9390fb4

define $(package)_preprocess_cmds

endef

define $(package)_config_cmds
cmake -DCMAKE_INSTALL_PREFIX=$(host_prefix) -DBUILD_SHARED_AND_STATIC_LIBS=On -DPNG_TESTS=Off
endef

define $(package)_build_cmds
$(MAKE)
endef

define $(package)_stage_cmds
$(MAKE) PREFIX=$(host_prefix) DESTDIR=$($(package)_staging_dir) install
endef

define $(package)_postprocess_cmds
  rm -rf bin
endef
