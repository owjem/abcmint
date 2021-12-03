package=cJSON
$(package)_version=1.7.7
$(package)_download_path=https://github.com/DaveGamble/cJSON/archive/
$(package)_file_name=v$($(package)_version).tar.gz
$(package)_sha256_hash=6eb9d852a97ffbe149e747f54d63e39a674fa248bb24902a14c079803067949a

define $(package)_preprocess_cmds

endef

define $(package)_config_cmds
cmake -DCMAKE_INSTALL_PREFIX=$(host_prefix) -DCMAKE_C_COMPILER=$($(package)_cc) -DENABLE_CJSON_UTILS=Off -DBUILD_SHARED_AND_STATIC_LIBS=On -DENABLE_CJSON_TEST=Off
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
