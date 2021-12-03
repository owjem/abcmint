
package=python
$(package)_version=2.7.18
$(package)_download_path=https://www.python.org/ftp/python/$($(package)_version)/
# https://www.python.org/ftp/python/2.7.18/Python-2.7.18.tgz
$(package)_file_name=Python-$($(package)_version).tgz
$(package)_sha256_hash=da3080e3b488f648a3d7a4560ddee895284c3380b11d6de75edb986526b9a814

define $(package)_preprocess_cmds

endef

define $(package)_set_vars
$(package)_config_opts=--enable-shared --build=$(build)
# $(package)_config_opts=--enable-shared --enable-optimizations --build=$(build)
endef

define $(package)_config_cmds
  $($(package)_autoconf)
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef

define $(package)_postprocess_cmds
  rm -rf bin share
endef
