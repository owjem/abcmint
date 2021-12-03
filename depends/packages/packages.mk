# packages:=python mysql jsoncpp openssl curl boost
# packages:=python mysql jsoncpp openssl curl boost zlib cares gflags abseil benchmark grpc

# packages:=jansson cjson ncurses
# packages:=ncurses

# protobuf_native_packages = native_protobuf jansson
# protobuf_packages = protobuf

# darwin_native_packages = native_biplist native_ds_store native_mac_alias

# ifneq ($(build_os),darwin)
# darwin_native_packages += native_cctools native_cdrkit native_libdmg-hfsplus
# endif


# packages:=openssl boost miniupnpc bdb qrencode zlib libpng


packages:=boost libevent openssl

qrencode_packages = qrencode

qt_linux_packages:=qt expat libxcb xcb_proto libXau xproto freetype fontconfig libxkbcommon
qt_android_packages=qt

qt_darwin_packages=qt
qt_mingw32_packages=qt

bdb_packages=bdb
# sqlite_packages=sqlite

# zmq_packages=zeromq

upnp_packages=miniupnpc
# natpmp_packages=libnatpmp

# multiprocess_packages = libmultiprocess capnp
# multiprocess_native_packages = native_libmultiprocess native_capnp

darwin_native_packages = native_ds_store native_mac_alias

# deps boots 1.71
# $(host_arch)_$(host_os)_native_packages += native_b2

ifneq ($(build_os),darwin)
darwin_native_packages += native_cctools native_libtapi native_libdmg-hfsplus

ifeq ($(strip $(FORCE_USE_SYSTEM_CLANG)),)
darwin_native_packages+= native_clang
endif

endif
