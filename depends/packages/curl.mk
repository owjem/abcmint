package=curl
$(package)_version=7_70_0
$(package)_download_path=https://github.com/curl/curl/archive/
$(package)_file_name=curl-$($(package)_version).tar.gz
$(package)_sha256_hash=011f13dce1b713a8577bbb706d45025ac5ff2a479af21907681cda6950535a7e

define $(package)_preprocess_cmds

endef

# define $(package)_set_vars
# $(package)_config_opts=--disable-shared --without-ssl
# endef

# define $(package)_config_cmds
#   $($(package)_autoconf)
# endef

define $(package)_build_cmds
  $($(package)_cmake) -DBUILD_CURL_EXE=OFF -DCMAKE_USE_OPENSSL=OFF && $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef

define $(package)_postprocess_cmds
  rm -rf bin
endef

