package=jansson
$(package)_version=2.9
$(package)_download_path=https://github.com/akheron/jansson/archive/
$(package)_file_name=v$($(package)_version).tar.gz
$(package)_sha256_hash=952fa714b399e71c1c3aa020e32e899f290c82126ca4d0d14cff5d10af457656

define $(package)_preprocess_cmds

endef


define $(package)_config_cmds
autoreconf -i && $($(package)_autoconf) --prefix=$(host_prefix)
endef

define $(package)_build_cmds
$(MAKE)
endef

define $(package)_stage_cmds
$(MAKE) DESTDIR=$($(package)_staging_dir) install
endef

define $(package)_postprocess_cmds
  rm -rf bin
endef

