dnl require autoconf 2.60 (AS_ECHO/AS_ECHO_N)
AC_PREREQ([2.60])
define(_CLIENT_VERSION_MAJOR, 1)
define(_CLIENT_VERSION_MINOR, 9)
define(_CLIENT_VERSION_REVISION, 5)
define(_CLIENT_VERSION_BUILD, 5)
define(_CLIENT_VERSION_IS_RELEASE, true)
define(_COPYRIGHT_YEAR, 2018)
AC_INIT([Bitcoin Core],[_CLIENT_VERSION_MAJOR._CLIENT_VERSION_MINOR._CLIENT_VERSION_REVISION],[info@bitcoin.org],[bitcoin])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([src/config/bitcoin-config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])

dnl Unless the user specified ARFLAGS, force it to be cr
AC_ARG_VAR([ARFLAGS], [Flags for the archiver, defaults to <cr> if not set])
if test "${ARFLAGS+set}" != "set"; then
  ARFLAGS="cr"
fi

# AC_SUBST([ARFLAGS], ["cr"])
# AC_SUBST([ARFLAGS], ["rcs"])

AC_CANONICAL_HOST

AH_TOP([#ifndef BITCOIN_CONFIG_H])
AH_TOP([#define BITCOIN_CONFIG_H])
AH_BOTTOM([#endif //BITCOIN_CONFIG_H])

dnl faketime breaks configure and is only needed for make. Disable it here.
unset FAKETIME

dnl Automake init set-up and checks
AM_INIT_AUTOMAKE([no-define subdir-objects foreign])

dnl faketime messes with timestamps and causes configure to be re-run.
dnl --disable-maintainer-mode can be used to bypass this.
AM_MAINTAINER_MODE([enable])

dnl make the compilation flags quiet unless V=1 is used
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Compiler checks (here before libtool).
if test "x${CXXFLAGS+set}" = "xset"; then
  CXXFLAGS_overridden=yes
else
  CXXFLAGS_overridden=no
fi
AC_PROG_CXX
m4_ifdef([AC_PROG_OBJCXX],[AC_PROG_OBJCXX])

dnl By default, libtool for mingw refuses to link static libs into a dll for
dnl fear of mixing pic/non-pic objects, and import/export complications. Since
dnl we have those under control, re-enable that functionality.
case $host in
  *mingw*)
     lt_cv_deplibs_check_method="pass_all"
  ;;
esac
dnl Libtool init checks.
LT_INIT([pic-only])

dnl Check/return PATH for base programs.
AC_PATH_TOOL(AR, ar)
AC_PATH_TOOL(RANLIB, ranlib)
AC_PATH_TOOL(STRIP, strip)
AC_PATH_TOOL(GCOV, gcov)
AC_PATH_PROG(LCOV, lcov)
AC_PATH_PROG(JAVA, java)
AC_PATH_PROG(GENHTML, genhtml)
AC_PATH_PROG([GIT], [git])
AC_PATH_PROG(CCACHE,ccache)
AC_PATH_PROG(XGETTEXT,xgettext)
AC_PATH_PROG(HEXDUMP,hexdump)

dnl pkg-config check.
PKG_PROG_PKG_CONFIG

# Enable wallet
AC_ARG_ENABLE([wallet],
  [AS_HELP_STRING([--enable-wallet],
  [enable wallet (default is yes)])],
  [enable_wallet=$enableval],
  [enable_wallet=yes])

AC_ARG_WITH([miniupnpc],
  [AS_HELP_STRING([--with-miniupnpc],
  [enable UPNP (default is yes if libminiupnpc is found)])],
  [use_upnp=$withval],
  [use_upnp=auto])

AC_ARG_ENABLE([upnp-default],
  [AS_HELP_STRING([--enable-upnp-default],
  [if UPNP is enabled, turn it on at startup (default is no)])],
  [use_upnp_default=$enableval],
  [use_upnp_default=yes])

dnl enable ipv6 support
AC_ARG_ENABLE([ipv6],
  [AS_HELP_STRING([--enable-ipv6],
  [enable ipv6 (default is yes)])],
  [use_ipv6=$enableval],
  [use_ipv6=yes])

dnl enable qt support
AC_ARG_WITH([qt],
  [AS_HELP_STRING([--with-qt],
  [enable qt (default is yes)])],
  [use_qt=$withval],
  [use_qt=yes])

dnl enable gpu miner support
AC_ARG_ENABLE(gpu,
    AS_HELP_STRING([--enable-gpu],[enable gpu miner support (default is yes)]),
    [use_gpu=$enableval],
    [use_gpu=yes])

dnl compile with full warnings and debugging symbols
AC_ARG_ENABLE(debug,
    AS_HELP_STRING([--enable-debug],[compile with debug symbols (default is yes)]),
    [use_debug=$enableval],
    [use_debug=yes])
    AC_MSG_CHECKING([if compiling with debugging symbols])

AC_ARG_ENABLE(tests,
    AS_HELP_STRING([--enable-tests],[compile tests (default is yes)]),
    [use_tests=$enableval],
    [use_tests=yes])

AC_ARG_WITH([comparison-tool],
    AS_HELP_STRING([--with-comparison-tool],[path to java comparison tool (requires --enable-tests)]),
    [use_comparison_tool=$withval],
    [use_comparison_tool=no])

AC_ARG_ENABLE([comparison-tool-reorg-tests],
    AS_HELP_STRING([--enable-comparison-tool-reorg-tests],[enable expensive reorg tests in the comparison tool (default no)]),
    [use_comparison_tool_reorg_tests=$enableval],
    [use_comparison_tool_reorg_tests=no])

AC_ARG_WITH([qrencode],
  [AS_HELP_STRING([--with-qrencode],
  [enable QR code support (default is yes if qt is enabled and libqrencode is found)])],
  [use_qr=$withval],
  [use_qr=auto])

AC_ARG_WITH([qtdbus],
  [AS_HELP_STRING([--with-qtdbus],
  [enable DBus support (default is yes if qt is enabled and QtDBus is found)])],
  [use_dbus=$withval],
  [use_dbus=auto])

AC_ARG_ENABLE([hardening],
  [AS_HELP_STRING([--enable-hardening],
  [attempt to harden the resulting executables (default is yes)])],
  [use_hardening=$enableval],
  [use_hardening=yes])

AC_ARG_ENABLE([reduce-exports],
  [AS_HELP_STRING([--enable-reduce-exports],
  [attempt to reduce exported symbols in the resulting executables (default is yes)])],
  [use_reduce_exports=$enableval],
  [use_reduce_exports=auto])

AC_ARG_ENABLE([ccache],
  [AS_HELP_STRING([--enable-ccache],
  [use ccache for building (default is yes if ccache is found)])],
  [use_ccache=$enableval],
  [use_ccache=auto])

AC_ARG_ENABLE([lcov],
  [AS_HELP_STRING([--enable-lcov],
  [enable lcov testing (default is no)])],
  [use_lcov=yes],
  [use_lcov=no])

AC_ARG_WITH([qt-incdir],[AS_HELP_STRING([--with-qt-incdir=INC_DIR],[specify qt include path (overridden by pkgconfig)])], [qt_include_path=$withval], [])
AC_ARG_WITH([qt-libdir],[AS_HELP_STRING([--with-qt-libdir=LIB_DIR],[specify qt lib path (overridden by pkgconfig)])], [qt_lib_path=$withval], [])
AC_ARG_WITH([qt-bindir],[AS_HELP_STRING([--with-qt-bindir=BIN_DIR],[specify qt bin path])], [qt_bin_path=$withval], [])
AC_ARG_WITH([qt-plugindir],[AS_HELP_STRING([--with-qt-plugindir=PLUGIN_DIR],[specify qt plugin path (overridden by pkgconfig)])], [qt_plugin_path=$withval], [])
AC_ARG_WITH([protoc-bindir],[AS_HELP_STRING([--with-protoc-bindir=BIN_DIR],[specify protoc bin path])], [protoc_bin_path=$withval], [])

# Enable debug
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
                    [use debug compiler flags and macros (default is no)])],
    [enable_debug=$enableval],
    [enable_debug=no])

if test "x$enable_debug" = xyes; then
    if test "x$GCC" = xyes; then
        CFLAGS="-g3 -O0 -DDEBUG"
    fi

    if test "x$GXX" = xyes; then
        CXXFLAGS="-g3 -O0 -DDEBUG"
    fi
fi

## TODO: Remove these hard-coded paths and flags. They are here for the sake of
##       compatibility with the legacy buildsystem.
##
if test "x$CXXFLAGS_overridden" = "xno"; then
  CXXFLAGS="$CXXFLAGS -Wall -Wextra -Wformat -Wformat-security -Wno-unused-parameter"
fi
CPPFLAGS="$CPPFLAGS -DBOOST_SPIRIT_THREADSAFE -DHAVE_BUILD_INFO -D__STDC_FORMAT_MACROS"


AC_ARG_WITH([daemon],
  [AS_HELP_STRING([--with-daemon],
  [build bitcoind daemon (default=yes)])],
  [build_bitcoind=$withval],
  [build_bitcoind=yes])

AC_LANG_PUSH([C++])

use_pkgconfig=yes
case $host in
  *mingw*)

     #pkgconfig does more harm than good with MinGW
     use_pkgconfig=no

     TARGET_OS=windows
     AC_CHECK_LIB([mingwthrd],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([kernel32],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([user32],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([gdi32],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([comdlg32],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([winspool],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([winmm],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([shell32],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([comctl32],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([ole32],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([oleaut32],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([uuid],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([rpcrt4],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([advapi32],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([ws2_32],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([mswsock],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([shlwapi],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([iphlpapi],      [main],, AC_MSG_ERROR(lib missing))
     AC_CHECK_LIB([crypt32],      [main],, AC_MSG_ERROR(lib missing))

     # -static is interpreted by libtool, where it has a different meaning.
     # In libtool-speak, it's -all-static.
     AX_CHECK_LINK_FLAG([[-static]],[LIBTOOL_APP_LDFLAGS="$LIBTOOL_APP_LDFLAGS -all-static"])

     AC_PATH_PROG([MAKENSIS], [makensis], none)
     if test x$MAKENSIS = xnone; then
       AC_MSG_WARN("makensis not found. Cannot create installer.")
     fi

     AC_PATH_TOOL(WINDRES, windres, none)
     if test x$WINDRES = xnone; then
       AC_MSG_ERROR("windres not found")
     fi

     CPPFLAGS="$CPPFLAGS -D_MT -DWIN32 -D_WINDOWS -DBOOST_THREAD_USE_LIB"
     LEVELDB_TARGET_FLAGS="TARGET_OS=OS_WINDOWS_CROSSCOMPILE"
     if test "x$CXXFLAGS_overridden" = "xno"; then
       CXXFLAGS="$CXXFLAGS -w"
     fi
     case $host in
       i?86-*) WINDOWS_BITS=32 ;;
       x86_64-*) WINDOWS_BITS=64 ;;
       *) AC_MSG_ERROR("Could not determine win32/win64 for installer") ;;
     esac
     AC_SUBST(WINDOWS_BITS)

     dnl libtool insists upon adding -nostdlib and a list of objects/libs to link against.
     dnl That breaks our ability to build dll's with static libgcc/libstdc++/libssp. Override
     dnl its command here, with the predeps/postdeps removed, and -static inserted. Postdeps are
     dnl also overridden to prevent their insertion later.
     dnl This should only affect dll's.
     archive_cmds_CXX="\$CC -shared \$libobjs \$deplibs \$compiler_flags -static -o \$output_objdir/\$soname \${wl}--enable-auto-image-base -Xlinker --out-implib -Xlinker \$lib"
     postdeps_CXX=

     ;;
  *darwin*)
     TARGET_OS=darwin
     LEVELDB_TARGET_FLAGS="TARGET_OS=Darwin"
     if  test x$cross_compiling != xyes; then
       BUILD_OS=darwin
       AC_CHECK_PROG([PORT],port, port)
       if test x$PORT = xport; then
         dnl add default macports paths
         CPPFLAGS="$CPPFLAGS -isystem /opt/local/include"
         LIBS="$LIBS -L/opt/local/lib"
         if test -d /opt/local/include/db48; then
           CPPFLAGS="$CPPFLAGS -I/opt/local/include/db48"
           LIBS="$LIBS -L/opt/local/lib/db48"
         fi
       fi

       AC_CHECK_PROG([BREW],brew, brew)
       if test x$BREW = xbrew; then
         dnl These Homebrew packages may be keg-only, meaning that they won't be found
         dnl in expected paths because they may conflict with system files. Ask
         dnl Homebrew where each one is located, then adjust paths accordingly.
         dnl It's safe to add these paths even if the functionality is disabled by
         dnl the user (--without-wallet or --without-gui for example).

         openssl_prefix=`$BREW --prefix openssl 2>/dev/null`
         bdb_prefix=`$BREW --prefix berkeley-db4 2>/dev/null`
         qt5_prefix=`$BREW --prefix qt5 2>/dev/null`
         if test x$openssl_prefix != x; then
           PKG_CONFIG_PATH="$openssl_prefix/lib/pkgconfig:$PKG_CONFIG_PATH"
           export PKG_CONFIG_PATH
         fi
         if test x$bdb_prefix != x; then
           CPPFLAGS="$CPPFLAGS -I$bdb_prefix/include"
           LIBS="$LIBS -L$bdb_prefix/lib"
         fi
         if test x$qt5_prefix != x; then
           PKG_CONFIG_PATH="$qt5_prefix/lib/pkgconfig:$PKG_CONFIG_PATH"
           export PKG_CONFIG_PATH
         fi
       fi
     else
       case $build_os in
         *darwin*)
           BUILD_OS=darwin
           ;;
         *)
           AC_PATH_TOOL([INSTALLNAMETOOL], [install_name_tool], install_name_tool)
           AC_PATH_TOOL([OTOOL], [otool], otool)
           AC_PATH_PROGS([GENISOIMAGE], [genisoimage mkisofs],genisoimage)

           dnl libtool will try to strip the static lib, which is a problem for
           dnl cross-builds because strip attempts to call a hard-coded ld,
           dnl which may not exist in the path. Stripping the .a is not
           dnl necessary, so just disable it.
           old_striplib=
           ;;
       esac
     fi

     AX_CHECK_LINK_FLAG([[-Wl,-headerpad_max_install_names]], [LDFLAGS="$LDFLAGS -Wl,-headerpad_max_install_names"])
     CPPFLAGS="$CPPFLAGS -DMAC_OSX"
     ;;
   *linux*)
     TARGET_OS=linux
     ;;
   *)
     ;;
esac

enable_sse42=no
enable_sse41=no
enable_avx2=no
enable_x86_shani=no
use_asm=no

if test x$use_comparison_tool != xno; then
  AC_SUBST(JAVA_COMPARISON_TOOL, $use_comparison_tool)
fi

if test x$use_comparison_tool_reorg_tests != xno; then
  if test x$use_comparison_tool == x; then
    AC_MSG_ERROR("comparison tool reorg tests but comparison tool was not specified")
  fi
  AC_SUBST(COMPARISON_TOOL_REORG_TESTS, 1)
else
  AC_SUBST(COMPARISON_TOOL_REORG_TESTS, 0)
fi

if test x$use_lcov = xyes; then
  if test x$LCOV = x; then
    AC_MSG_ERROR("lcov testing requested but lcov not found")
  fi
  if test x$GCOV = x; then
    AC_MSG_ERROR("lcov testing requested but gcov not found")
  fi
  if test x$JAVA = x; then
    AC_MSG_ERROR("lcov testing requested but java not found")
  fi
  if test x$GENHTML = x; then
    AC_MSG_ERROR("lcov testing requested but genhtml not found")
  fi
  if test x$use_comparison_tool = x; then
    AC_MSG_ERROR("lcov testing requested but comparison tool was not specified")
  fi
  LCOV="$LCOV --gcov-tool=$GCOV"
  AX_CHECK_COMPILE_FLAG([--coverage],[CXXFLAGS="$CXXFLAGS --coverage"],
    [AC_MSG_ERROR("lcov testing requested but --coverage flag does not work")])
fi

dnl Require little endian
AC_C_BIGENDIAN([AC_MSG_ERROR("Big Endian not supported")])

dnl Check for pthread compile/link requirements
AX_PTHREAD

# The following macro will add the necessary defines to bitcoin-config.h, but
# they also need to be passed down to any subprojects. Pull the results out of
# the cache and add them to CPPFLAGS.
AC_SYS_LARGEFILE
# detect POSIX or GNU variant of strerror_r
AC_FUNC_STRERROR_R

if test x$ac_cv_sys_file_offset_bits != x &&
   test x$ac_cv_sys_file_offset_bits != xno &&
   test x$ac_cv_sys_file_offset_bits != xunknown; then
  CPPFLAGS="$CPPFLAGS -D_FILE_OFFSET_BITS=$ac_cv_sys_file_offset_bits"
fi

if test x$ac_cv_sys_large_files != x &&
   test x$ac_cv_sys_large_files != xno &&
   test x$ac_cv_sys_large_files != xunknown; then
  CPPFLAGS="$CPPFLAGS -D_LARGE_FILES=$ac_cv_sys_large_files"
fi

AX_CHECK_LINK_FLAG([[-Wl,--large-address-aware]], [LDFLAGS="$LDFLAGS -Wl,--large-address-aware"])

AX_GCC_FUNC_ATTRIBUTE([visibility])
AX_GCC_FUNC_ATTRIBUTE([dllexport])
AX_GCC_FUNC_ATTRIBUTE([dllimport])

if test x$use_glibc_compat != xno; then

  #__fdelt_chk's params and return type have changed from long unsigned int to long int.
  # See which one is present here.
  AC_MSG_CHECKING(__fdelt_chk type)
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#ifdef _FORTIFY_SOURCE
                    #undef _FORTIFY_SOURCE
                  #endif
                  #define _FORTIFY_SOURCE 2
                  #include <sys/select.h>
     extern "C" long unsigned int __fdelt_warn(long unsigned int);]],[[]])],
    [ fdelt_type="long unsigned int"],
    [ fdelt_type="long int"])
  AC_MSG_RESULT($fdelt_type)
  AC_DEFINE_UNQUOTED(FDELT_TYPE, $fdelt_type,[parameter and return value type for __fdelt_chk])

fi

if test x$use_hardening != xno; then
  AX_CHECK_COMPILE_FLAG([-Wstack-protector],[HARDENED_CXXFLAGS="$HARDENED_CXXFLAGS -Wstack-protector"])
  AX_CHECK_COMPILE_FLAG([-fstack-protector-all],[HARDENED_CXXFLAGS="$HARDENED_CXXFLAGS -fstack-protector-all"])

  AX_CHECK_PREPROC_FLAG([-D_FORTIFY_SOURCE=2],[
    AX_CHECK_PREPROC_FLAG([-U_FORTIFY_SOURCE],[
      HARDENED_CPPFLAGS="$HARDENED_CPPFLAGS -U_FORTIFY_SOURCE"
    ])
    HARDENED_CPPFLAGS="$HARDENED_CPPFLAGS -D_FORTIFY_SOURCE=2"
  ])

  AX_CHECK_LINK_FLAG([[-Wl,--dynamicbase]], [HARDENED_LDFLAGS="$HARDENED_LDFLAGS -Wl,--dynamicbase"])
  AX_CHECK_LINK_FLAG([[-Wl,--nxcompat]], [HARDENED_LDFLAGS="$HARDENED_LDFLAGS -Wl,--nxcompat"])
  AX_CHECK_LINK_FLAG([[-Wl,-z,relro]], [HARDENED_LDFLAGS="$HARDENED_LDFLAGS -Wl,-z,relro"])
  AX_CHECK_LINK_FLAG([[-Wl,-z,now]], [HARDENED_LDFLAGS="$HARDENED_LDFLAGS -Wl,-z,now"])

  if test x$TARGET_OS != xwindows; then
    # All windows code is PIC, forcing it on just adds useless compile warnings
    AX_CHECK_COMPILE_FLAG([-fPIE],[HARDENED_CXXFLAGS="$HARDENED_CXXFLAGS -fPIE"])
    AX_CHECK_LINK_FLAG([[-pie]], [HARDENED_LDFLAGS="$HARDENED_LDFLAGS -pie"])
  fi

  case $host in
    *mingw*)
       AC_CHECK_LIB([ssp],      [main],, AC_MSG_ERROR(lib missing))
    ;;
  esac

  CXXFLAGS="$CXXFLAGS $HARDENED_CXXFLAGS"
  CPPFLAGS="$CPPFLAGS $HARDENED_CPPFLAGS"
  LDFLAGS="$LDFLAGS $HARDENED_LDFLAGS"
  OBJCXXFLAGS="$CXXFLAGS"
fi

dnl this flag screws up non-darwin gcc even when the check fails. special-case it.
if test x$TARGET_OS = xdarwin; then
  AX_CHECK_LINK_FLAG([[-Wl,-dead_strip]], [LDFLAGS="$LDFLAGS -Wl,-dead_strip"])
fi

AC_CHECK_HEADERS([endian.h sys/endian.h byteswap.h stdio.h stdlib.h unistd.h strings.h sys/types.h sys/stat.h sys/select.h sys/prctl.h])
AC_SEARCH_LIBS([getaddrinfo_a], [anl], [AC_DEFINE(HAVE_GETADDRINFO_A, 1, [Define this symbol if you have getaddrinfo_a])])
AC_SEARCH_LIBS([inet_pton], [nsl resolv], [AC_DEFINE(HAVE_INET_PTON, 1, [Define this symbol if you have inet_pton])])

AC_CHECK_DECLS([strnlen])

AC_CHECK_DECLS([le16toh, le32toh, le64toh, htole16, htole32, htole64, be16toh, be32toh, be64toh, htobe16, htobe32, htobe64],,,
		[#if HAVE_ENDIAN_H
                 #include <endian.h>
                 #elif HAVE_SYS_ENDIAN_H
                 #include <sys/endian.h>
                 #endif])

AC_CHECK_DECLS([bswap_16, bswap_32, bswap_64],,,
		[#if HAVE_BYTESWAP_H
                 #include <byteswap.h>
                 #endif])

dnl Check for MSG_NOSIGNAL
AC_MSG_CHECKING(for MSG_NOSIGNAL)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/socket.h>]],
 [[ int f = MSG_NOSIGNAL; ]])],
 [ AC_MSG_RESULT(yes); AC_DEFINE(HAVE_MSG_NOSIGNAL, 1,[Define this symbol if you have MSG_NOSIGNAL]) ],
 [ AC_MSG_RESULT(no)]
)

AC_SEARCH_LIBS([clock_gettime],[rt])

AC_MSG_CHECKING([for visibility attribute])
AC_LINK_IFELSE([AC_LANG_SOURCE([
  int foo_def( void ) __attribute__((visibility("default")));
  int main(){}
  ])],
  [
    AC_DEFINE(HAVE_VISIBILITY_ATTRIBUTE,1,[Define if the visibility attribute is supported.])
    AC_MSG_RESULT(yes)
  ],
  [
    AC_MSG_RESULT(no)
    if test x$use_reduce_exports = xyes; then
      AC_MSG_ERROR([Cannot find a working visibility attribute. Use --disable-reduce-exports.])
    fi
      AC_MSG_WARN([Cannot find a working visibility attribute. Disabling reduced exports.])
      use_reduce_exports=no
  ]
)

if test x$use_reduce_exports != xno; then
  AX_CHECK_COMPILE_FLAG([-fvisibility=hidden],[RE_CXXFLAGS="-fvisibility=hidden"],
  [
    if test x$use_reduce_exports = xyes; then
      AC_MSG_ERROR([Cannot set default symbol visibility. Use --disable-reduced-exports.])
    fi
    AC_MSG_WARN([Cannot set default symbol visibility. Disabling reduced exports.])
    use_reduce_exports=no
  ])
fi

LEVELDB_CPPFLAGS=
LIBLEVELDB=
LIBMEMENV=
AM_CONDITIONAL([EMBEDDED_LEVELDB],[true])
AC_SUBST(LEVELDB_CPPFLAGS)
AC_SUBST(LIBLEVELDB)
AC_SUBST(LIBMEMENV)

if test x$enable_wallet != xno; then
    dnl Check for libdb_cxx only if wallet enabled
    BITCOIN_FIND_BDB48
fi

dnl Check for libminiupnpc (optional)
if test "$use_upnp" != "no"; then
  AC_CHECK_HEADERS(
    [miniupnpc/miniupnpc.h miniupnpc/upnpcommands.h miniupnpc/upnperrors.h],
    [AC_CHECK_LIB([miniupnpc], [upnpDiscover], [MINIUPNPC_LIBS="-L${depends_prefix}/lib -lminiupnpc"], [have_miniupnpc=yes])],
    [have_miniupnpc=yes]
  )
dnl The minimum supported miniUPnPc API version is set to 10. This keeps compatibility
dnl with Ubuntu 16.04 LTS and Debian 8 libminiupnpc-dev packages.
if test "$have_miniupnpc" != "no"; then
  AC_MSG_CHECKING([whether miniUPnPc API version is supported])
  AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
      @%:@include <miniupnpc/miniupnpc.h>
    ]], [[
      #if MINIUPNPC_API_VERSION >= 10
      // Everything is okay
      #else
      #  error miniUPnPc API version is too old
      #endif
    ]])],[
      AC_MSG_RESULT([yes])
    ],[
    AC_MSG_RESULT([no])
    AC_MSG_WARN([miniUPnPc API version < 10 is unsupported, disabling UPnP support.])
    have_miniupnpc=no
  ])
fi
fi

if test x$build_bitcoin_utils$build_bitcoind$bitcoin_enable_qt$use_tests = xnononono; then
    use_boost=no
else
    use_boost=yes
fi

if test x$use_boost = xyes; then

dnl Check for boost libs
AX_BOOST_BASE
AX_BOOST_SYSTEM
AX_BOOST_FILESYSTEM
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_THREAD
AX_BOOST_CHRONO


if test x$use_reduce_exports != xno; then
  AC_MSG_CHECKING([for working boost reduced exports])
  TEMP_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$BOOST_CPPFLAGS $CPPFLAGS"
  AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
      @%:@include <boost/version.hpp>
    ]], [[
      #if BOOST_VERSION >= 104900
      // Everything is okay
      #else
      #  error Boost version is too old
      #endif
    ]])],[
      AC_MSG_RESULT(yes)
    ],[:
    if test x$use_reduce_exports = xauto; then
      use_reduce_exports=no
    else
      if test x$use_reduce_exports = xyes; then
        AC_MSG_ERROR([boost versions < 1.49 are known to be broken with reduced exports. Use --disable-reduced-exports.])
      fi
    fi
    AC_MSG_RESULT(no)
    AC_MSG_WARN([boost versions < 1.49 are known to have symbol visibility issues. Disabling reduced exports.])
  ])
  CPPFLAGS="$TEMP_CPPFLAGS"
fi

elif test x$use_reduce_exports = xauto; then
    use_reduce_exports=yes
fi

if test x$use_reduce_exports != xno; then
    CXXFLAGS="$CXXFLAGS $RE_CXXFLAGS"
    AX_CHECK_LINK_FLAG([[-Wl,--exclude-libs,ALL]], [RELDFLAGS="-Wl,--exclude-libs,ALL"])
fi

if test x$use_tests = xyes; then

  if test x$HEXDUMP = x; then
    AC_MSG_ERROR(hexdump is required for tests)
  fi


  if test x$use_boost = xyes; then

  AX_BOOST_UNIT_TEST_FRAMEWORK

  dnl Determine if -DBOOST_TEST_DYN_LINK is needed
  AC_MSG_CHECKING([for dynamic linked boost test])
  TEMP_LIBS="$LIBS"
  LIBS="$LIBS $BOOST_LDFLAGS $BOOST_UNIT_TEST_FRAMEWORK_LIB"
  TEMP_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
  AC_LINK_IFELSE([AC_LANG_SOURCE([
       #define BOOST_TEST_DYN_LINK
       #define BOOST_TEST_MAIN
        #include <boost/test/unit_test.hpp>

       ])],
    [AC_MSG_RESULT(yes)]
    [TESTDEFS="$TESTDEFS -DBOOST_TEST_DYN_LINK"],
    [AC_MSG_RESULT(no)])
  LIBS="$TEMP_LIBS"
  CPPFLAGS="$TEMP_CPPFLAGS"

  fi
fi

if test x$use_boost = xyes; then

BOOST_LIBS="$BOOST_LDFLAGS $BOOST_SYSTEM_LIB $BOOST_FILESYSTEM_LIB $BOOST_PROGRAM_OPTIONS_LIB $BOOST_THREAD_LIB $BOOST_CHRONO_LIB"

dnl Boost >= 1.50 uses sleep_for rather than the now-deprecated sleep, however
dnl it was broken from 1.50 to 1.52 when backed by nanosleep. Use sleep_for if
dnl a working version is available, else fall back to sleep. sleep was removed
dnl after 1.56.
dnl If neither is available, abort.
TEMP_LIBS="$LIBS"
LIBS="$BOOST_LIBS $LIBS"
TEMP_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
  #include <boost/thread/thread.hpp>
  #include <boost/version.hpp>
  ]],[[
  #if BOOST_VERSION >= 105000 && (!defined(BOOST_HAS_NANOSLEEP) || BOOST_VERSION >= 105200)
      boost::this_thread::sleep_for(boost::chrono::milliseconds(0));
  #else
   choke me
  #endif
  ]])],
  [boost_sleep=yes;
     AC_DEFINE(HAVE_WORKING_BOOST_SLEEP_FOR, 1, [Define this symbol if boost sleep_for works])],
  [boost_sleep=no])
LIBS="$TEMP_LIBS"
CPPFLAGS="$TEMP_CPPFLAGS"

if test x$boost_sleep != xyes; then
TEMP_LIBS="$LIBS"
LIBS="$BOOST_LIBS $LIBS"
TEMP_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
  #include <boost/version.hpp>
  #include <boost/thread.hpp>
  #include <boost/date_time/posix_time/posix_time_types.hpp>
  ]],[[
  #if BOOST_VERSION <= 105600
      boost::this_thread::sleep(boost::posix_time::milliseconds(0));
  #else
   choke me
  #endif
  ]])],
  [boost_sleep=yes; AC_DEFINE(HAVE_WORKING_BOOST_SLEEP, 1, [Define this symbol if boost sleep works])],
  [boost_sleep=no])
LIBS="$TEMP_LIBS"
CPPFLAGS="$TEMP_CPPFLAGS"
fi

if test x$boost_sleep != xyes; then
  AC_MSG_ERROR(No working boost sleep implementation found.)
fi

fi

if test x$use_pkgconfig = xyes; then

  if test x"$PKG_CONFIG" = "x"; then
    AC_MSG_ERROR(pkg-config not found.)
  fi

  : #NOP
  m4_ifdef(
    [PKG_CHECK_MODULES],
    [
      PKG_CHECK_MODULES([SSL], [libssl],, [AC_MSG_ERROR(openssl  not found.)])
      PKG_CHECK_MODULES([CRYPTO], [libcrypto],,[AC_MSG_ERROR(libcrypto  not found.)])

      if test x$use_qt = xyes; then
        PKG_CHECK_MODULES([QT], [QtCore QtGui QtNetwork], [QT_INCLUDES="$QT_CFLAGS"; have_qt=yes], [have_qt=no])
        if test x$use_tests = xyes; then
          PKG_CHECK_MODULES([QT_TEST], [QtTest], [QT_TEST_INCLUDES="$QT_TEST_CFLAGS"; have_qt_test=yes], [have_qt_test=no])
        fi
        if test x$use_dbus != xno; then
          PKG_CHECK_MODULES([QT_DBUS], [QtDBus], [QT_DBUS_INCLUDES="$QT_DBUS_CFLAGS"; have_qt_dbus=yes], [have_qt_dbus=no])
        fi
        if test x$use_qr != xno; then
          PKG_CHECK_MODULES([QR], [libqrencode], [have_qrencode=yes], [have_qrencode=no])
        fi
# PKG_CHECK_MODULES([PROTOBUF], [protobuf], [have_protobuf=yes], [AC_MSG_ERROR(libprotobuf not found.)])
      fi
    ]
  )
else
  AC_CHECK_HEADER([openssl/crypto.h],,AC_MSG_ERROR(libcrypto headers missing))
  AC_CHECK_LIB([crypto],      [main],CRYPTO_LIBS=-lcrypto, AC_MSG_ERROR(libcrypto missing))

  AC_CHECK_HEADER([openssl/ssl.h],, AC_MSG_ERROR(libssl headers missing),)
  AC_CHECK_LIB([ssl],         [main],SSL_LIBS=-lssl, AC_MSG_ERROR(libssl missing))

  if test x$use_qt = xyes; then
    TEMP_LIBS="$LIBS"
    LIBS=
    if test x$qt_lib_path != x; then
      QT_LIBS="$QT_LIBS -L$qt_lib_path"
      LIBS="$QT_LIBS"
    fi
    if test x$qt_plugin_path != x; then
      QT_LIBS="$QT_LIBS -L$qt_plugin_path/codecs"
      LIBS="$QT_LIBS"
    fi

    if test x$TARGET_OS == xwindows; then
      AC_CHECK_LIB([imm32],      [main],, AC_MSG_ERROR(libimm32 not found. Install it or use --without-qt.))
    fi

    #TODO: These are only needed when they're linked directly to parent libs. It really has nothing to do with windows.
    #Instead, check for missing functions in parent libs and assume static if they're absent.
    if test x$TARGET_OS == xwindows; then
      AC_CHECK_LIB([qcncodecs],      [main],, AC_MSG_ERROR(libqcncodecs not found. Install it or use --without-qt.))
      AC_CHECK_LIB([qjpcodecs],      [main],, AC_MSG_ERROR(libqjpcodecs not found. Install it or use --without-qt.))
      AC_CHECK_LIB([qkrcodecs],      [main],, AC_MSG_ERROR(libqkrcodecs not found. Install it or use --without-qt.))
      AC_CHECK_LIB([qtwcodecs],      [main],, AC_MSG_ERROR(libqtwcodecs not found. Install it or use --without-qt.))
    fi

    AC_CHECK_LIB([QtCore],      [main],, AC_MSG_ERROR(libQtCore not found. Install it or use --without-qt.))
    AC_CHECK_LIB([QtGui],      [main],, AC_MSG_ERROR(libQtGui not found. Install it or use --without-qt.))
    AC_CHECK_LIB([QtNetwork],      [main],, AC_MSG_ERROR(libQtNetwork not found. Install it or use --without-qt.))
    AC_CHECK_LIB([protobuf],      [main],, AC_MSG_ERROR(libprotobuf not found. Install it or use --without-qt.))
    QT_LIBS="$LIBS"
    LIBS="$TEMP_LIBS"

    TEMP_CPPFLAGS="$CPPFLAGS"
    if test x$qt_include_path != x; then
      QT_INCLUDES="-I$qt_include_path -I$qt_include_path/QtCore -I$qt_include_path/QtGui -I$qt_include_path/QtNetwork -I$qt_include_path/QtTest -I$qt_include_path/QtDBus"
      CPPFLAGS="$CPPFLAGS $QT_INCLUDES"
    fi
    AC_CHECK_HEADER([QtPlugin],, AC_MSG_ERROR(QtCore headers missing. Install them or use --without-qt.),)
    AC_CHECK_HEADER([QApplication],, AC_MSG_ERROR(QtGUI headers missing. Install them or use --without-qt.),)
    AC_CHECK_HEADER([QLocalSocket],, AC_MSG_ERROR(QtNetwork headers missing. Install them or use --without-qt.),)

    if test x$use_tests = xyes; then
      TEMP_LIBS="$LIBS"
      LIBS=
      if test x$qt_lib_path != x; then
        LIBS="-L$qt_lib_path"
      fi
      AC_CHECK_LIB([QtTest],      [main],, have_qt_test=no)
      AC_CHECK_HEADER([QTest],, have_qt_test=no)
      QT_TEST_LIBS="$LIBS"
      LIBS="$TEMP_LIBS"
    fi
    if test x$use_dbus != xno; then
      TEMP_LIBS="$LIBS"
      LIBS=
      if test x$qt_lib_path != x; then
        LIBS="-L$qt_lib_path"
      fi
      AC_CHECK_LIB([QtDBus],      [main],, have_qt_dbus=no)
      AC_CHECK_HEADER([QtDBus],, have_qt_dbus=no)
      QT_DBUS_LIBS="$LIBS"
      LIBS="$TEMP_LIBS"
    fi
    CPPFLAGS="$TEMP_CPPFLAGS"
    if test x$use_qr != xno; then
      AC_CHECK_LIB([qrencode], [main],, [have_qrencode=no])
      AC_CHECK_HEADER([qrencode.h],, have_qrencode=no)
    fi
  fi
fi

CFLAGS_TEMP="$CFLAGS"
LIBS_TEMP="$LIBS"
CFLAGS="$CFLAGS $SSL_CFLAGS $CRYPTO_CFLAGS"
LIBS="$LIBS $SSL_LIBS $CRYPTO_LIBS"
AC_CHECK_HEADER([openssl/ec.h],, AC_MSG_ERROR(OpenSSL ec header missing),)
CFLAGS="$CFLAGS_TEMP"
LIBS="$LIBS_TEMP"

AC_MSG_CHECKING([whether to build bitcoind])
AM_CONDITIONAL([BUILD_BITCOIND], [test x$build_bitcoind = xyes])
AC_MSG_RESULT($build_bitcoind)

if test x$use_ipv6 = xyes; then
  dnl Check for ipv6 build requirements
  AC_MSG_CHECKING(for IPV6 build support)
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[
    #if defined(_WINDOWS)
    #include <winsock2.h>
    #else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #endif
  ]],[[
    #if !defined(_WINDOWS)
    struct in6_addr ipv6Addr;
    struct sockaddr_in6 addr;
    #endif
    int temp = socket(AF_INET6, SOCK_STREAM, 0);
  ]])],
  [AC_MSG_RESULT(yes); have_ipv6=yes; AC_DEFINE(HAVE_IPV6, 1, [Define this symbol if you have ipv6 build support])],
  [AC_MSG_RESULT(no)]; have_ipv6=no)
fi
AC_LANG_POP

if test "x$use_ccache" != "xno"; then
  AC_MSG_CHECKING(if ccache should be enabled)
  if test x$CCACHE = x; then
    if test "x$use_ccache" = "xyes"; then
      AC_MSG_ERROR([ccache not found.]);
    else
      use_ccache=no
    fi
  else
    use_ccache=yes
    CC="$ac_cv_path_CCACHE $CC"
    CXX="$ac_cv_path_CCACHE $CXX"
  fi
  AC_MSG_RESULT($use_ccache)
fi

dnl enable wallet
AC_MSG_CHECKING([if wallet should be enabled])
if test x$enable_wallet != xno; then
  AC_MSG_RESULT(yes)
  AC_DEFINE_UNQUOTED([ENABLE_WALLET],[1],[Define to 1 to enable wallet functions])

else
  AC_MSG_RESULT(no)

  if test "x$use_qt" != "xno"; then
    AC_MSG_ERROR([Cannot currently build Qt GUI with wallet disabled. Use --without-qt.])
  fi
fi
if test "x$use_ccache" = "xyes"; then
    AX_CHECK_PREPROC_FLAG([-Qunused-arguments],[CPPFLAGS="-Qunused-arguments $CPPFLAGS"])
fi


dnl enable ipv6 support
AC_MSG_CHECKING([if ipv6 should be enabled])
if test x$have_ipv6 = xno; then
  if test x$use_ipv6 = xyes; then
     AC_MSG_ERROR("ipv6 requested but cannot be built. use --disable-ipv6")
  fi
  AC_MSG_RESULT(no)
else
  if test x$use_ipv6 = xyes; then
      AC_MSG_RESULT(yes)
      AC_DEFINE([USE_IPV6],[1],[Define if ipv6 support should be compiled in])
  else
      AC_MSG_RESULT(no)
  fi
fi

dnl enable upnp support
AC_MSG_CHECKING([whether to build with support for UPnP])
if test "$have_miniupnpc" = "no"; then
  if test "$use_upnp" = "yes"; then
     AC_MSG_ERROR([UPnP requested but cannot be built. Use --without-miniupnpc])
  fi
  AC_MSG_RESULT([no])
  use_upnp=no
else
  if test "$use_upnp" != "no"; then
    AC_MSG_RESULT([yes])
    AC_MSG_CHECKING([whether to build with UPnP enabled by default])
    use_upnp=yes
    upnp_setting=0
    if test "$use_upnp_default" != "no"; then
      use_upnp_default=yes
      upnp_setting=1
    fi
    AC_MSG_RESULT([$use_upnp_default])
    AC_DEFINE_UNQUOTED([USE_UPNP],[$upnp_setting],[UPnP support not compiled if undefined, otherwise value (0 or 1) determines default state])
    # if test "$TARGET_OS" = "windows"; then
      MINIUPNPC_CPPFLAGS="-DSTATICLIB -DMINIUPNP_STATICLIB"
    # fi
  else
    AC_MSG_RESULT([no])
  fi
fi

dnl enable upnp support
AC_MSG_CHECKING([if GPU should be enabled])
if test x$use_gpu = xyes; then
  AC_MSG_RESULT([yes])
  AC_MSG_CHECKING([if gpu miner should be on by default])
  AC_DEFINE_UNQUOTED([USE_GPU],[1],[Define to 1 for gpu miner])
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
fi

dnl enable qt support
AC_MSG_CHECKING([if qt should be enabled])
if test x$use_qt = xyes; then
  if test x$have_qt = xno; then
     AC_MSG_ERROR("qt support requested but qt could not be located. use --without-qt")
  fi
  if test x$MOC = x; then
    AC_MSG_ERROR("qt support requested but moc was not found. use --without-qt")
  fi
  if test x$PROTOC = x; then
    AC_MSG_ERROR("qt support requested but protoc was not found. use --without-qt")
  fi
  if test x$UIC = x; then
    AC_MSG_ERROR("qt support requested but uic was not found. use --without-qt")
  fi
  if test x$RCC = x; then
    AC_MSG_ERROR("qt support requested but rcc was not found. use --without-qt")
  fi
  if test x$LRELEASE = x; then
    AC_MSG_ERROR("qt support requested but lrelease was not found. use --without-qt")
  fi
  if test x$use_tests = xyes; then
    if test x$have_qt_test = xno; then
      AC_MSG_ERROR("libQtTest not found. Use --disable-tests or --without-qt.")
    fi
  fi
  if test x$have_qt_dbus = xno;  then
    use_dbus=no
    if test x$use_dbus = xyes; then
      AC_MSG_ERROR("libQtDBus not found. Use --without-qtdbus.")
    fi
  fi
  BUILD_QT=qt
else
  use_qt=no
fi

AC_MSG_RESULT($use_qt)

dnl these are only used when qt is enabled
if test x$use_qt = xyes; then

  dnl enable dbus support
  AC_MSG_CHECKING([if dbus should be enabled])
  if test x$use_dbus != xno; then
    use_dbus=yes
    AC_DEFINE([USE_DBUS],[1],[Define if dbus support should be compiled in])
  else
    use_dbus=no
  fi
  AC_MSG_RESULT($use_dbus)

  dnl enable qr support
  AC_MSG_CHECKING([if qr should be enabled])
  if test x$have_qrencode = xno; then
    if test x$use_qr == xyes; then
     AC_MSG_ERROR("QR support requested but cannot be built. use --without-qrencode")
    fi
    AC_MSG_RESULT(no)
  else
    if test x$use_qr != xno; then
      AC_MSG_RESULT(yes)
      AC_DEFINE([USE_QRCODE],[1],[Define if QR support should be compiled in])
      use_qr=yes
    else
      AC_MSG_RESULT(no)
    fi
  fi

  if test x$use_tests = xyes; then
    BUILD_TEST_QT="test"
  fi
fi

AC_MSG_CHECKING([whether to build test_bitcoin])
if test x$use_tests = xyes; then
  AC_MSG_RESULT([yes])
  BUILD_TEST="test"
else
  AC_MSG_RESULT([no])
fi


AC_MSG_CHECKING([if CUDA should be enabled])
if test x$use_gpu = xyes; then
  AC_MSG_RESULT([yes])
# Checks for libraries.
#CUDA_PATH="/usr/local/cuda"
#AC_SUBST([CUDA_PATH], ["/usr/local/cuda-5.0"])
AC_ARG_WITH([cuda-path],
	    [AS_HELP_STRING([--with-cuda-path], [location of cuda sdk])],
            [CUDA_PATH="$withval"],
            [CUDA_PATH="/usr/local/cuda"])
AC_SUBST([CUDA_PATH])
AC_SUBST([CUDA_CFLAGS], ["-I${CUDA_PATH}/include"])
AC_SUBST([CUDA_LDFLAGS], ["-L${CUDA_PATH}/lib64 -lcudadevrt -lcudart"])
AC_SUBST([CUDA_ARCH], ["-arch=sm_52 -gencode=arch=compute_52,code=sm_52 -gencode=arch=compute_60,code=sm_60 -gencode=arch=compute_61,code=sm_61 -gencode=arch=compute_70,code=sm_70 -gencode=arch=compute_75,code=sm_75 -gencode=arch=compute_80,code=sm_80 -gencode=arch=compute_86,code=sm_86 -gencode=arch=compute_86,code=compute_86"])

cuda_k=20
cuda_unroll=8
AC_SUBST([CUDA_K], ["${cuda_k}"])
AC_SUBST([CUDA_UNROLL], ["${cuda_unroll}"])
AC_SUBST([NVCC], ["${CUDA_PATH}/bin/nvcc"])
AC_SUBST([NVCC_CFLAGS], ["--resource-usage -lineinfo -DPARK=${cuda_k} ${CUDA_ARCH}"])


AC_DEFINE_UNQUOTED([PARK], [$cuda_k], [Define this symbol for NVCC])
AC_SUBST([NVCC_OMPFLAGS], ["-fopenmp -lomp"])
else
  AC_MSG_RESULT([no])
fi

AM_CONDITIONAL([TARGET_DARWIN], [test x$TARGET_OS = xdarwin])
AM_CONDITIONAL([BUILD_DARWIN], [test x$BUILD_OS = xdarwin])
AM_CONDITIONAL([TARGET_WINDOWS], [test x$TARGET_OS = xwindows])
AM_CONDITIONAL([ENABLE_WALLET],[test x$enable_wallet == xyes])
AM_CONDITIONAL([ENABLE_TESTS],[test x$use_tests = xyes])
AM_CONDITIONAL([USE_QRCODE], [test x$use_qr = xyes])
AM_CONDITIONAL([USE_LCOV],[test x$use_lcov = xyes])
AM_CONDITIONAL([USE_GPU],[test x$use_gpu = xyes])
AM_CONDITIONAL([USE_COMPARISON_TOOL],[test x$use_comparison_tool != xno])
AM_CONDITIONAL([USE_COMPARISON_TOOL_REORG_TESTS],[test x$use_comparison_tool_reorg_test != xno])
AM_CONDITIONAL([GLIBC_BACK_COMPAT],[test x$use_glibc_compat = xyes])

AM_CONDITIONAL([ENABLE_SSE42], [test "$enable_sse42" = "yes"])
AM_CONDITIONAL([ENABLE_SSE41], [test "$enable_sse41" = "yes"])
AM_CONDITIONAL([ENABLE_AVX2], [test "$enable_avx2" = "yes"])
AM_CONDITIONAL([ENABLE_X86_SHANI], [test "$enable_x86_shani" = "yes"])
AM_CONDITIONAL([ENABLE_ARM_CRC], [test "$enable_arm_crc" = "yes"])
AM_CONDITIONAL([ENABLE_ARM_SHANI], [test "$enable_arm_shani" = "yes"])
AM_CONDITIONAL([USE_ASM], [test "$use_asm" = "yes"])

AC_DEFINE(CLIENT_VERSION_MAJOR, _CLIENT_VERSION_MAJOR, [Major version])
AC_DEFINE(CLIENT_VERSION_MINOR, _CLIENT_VERSION_MINOR, [Minor version])
AC_DEFINE(CLIENT_VERSION_REVISION, _CLIENT_VERSION_REVISION, [Build revision])
AC_DEFINE(CLIENT_VERSION_BUILD, _CLIENT_VERSION_BUILD, [Version Build])
AC_DEFINE(CLIENT_VERSION_IS_RELEASE, _CLIENT_VERSION_IS_RELEASE, [Version is release])
AC_DEFINE(COPYRIGHT_YEAR, _COPYRIGHT_YEAR, [Version is release])
AC_SUBST(CLIENT_VERSION_MAJOR, _CLIENT_VERSION_MAJOR)
AC_SUBST(CLIENT_VERSION_MINOR, _CLIENT_VERSION_MINOR)
AC_SUBST(CLIENT_VERSION_REVISION, _CLIENT_VERSION_REVISION)
AC_SUBST(CLIENT_VERSION_BUILD, _CLIENT_VERSION_BUILD)
AC_SUBST(CLIENT_VERSION_IS_RELEASE, _CLIENT_VERSION_IS_RELEASE)
AC_SUBST(COPYRIGHT_YEAR, _COPYRIGHT_YEAR)

AC_SUBST(COPYRIGHT_HOLDERS, "_COPYRIGHT_HOLDERS")
AC_SUBST(COPYRIGHT_HOLDERS_SUBSTITUTION, "_COPYRIGHT_HOLDERS_SUBSTITUTION")
AC_SUBST(COPYRIGHT_HOLDERS_FINAL, "_COPYRIGHT_HOLDERS_FINAL")
AC_SUBST(BITCOIN_DAEMON_NAME)
AC_SUBST(BITCOIN_GUI_NAME)
AC_SUBST(BITCOIN_CLI_NAME)
AC_SUBST(BITCOIN_TX_NAME)
AC_SUBST(BITCOIN_UTIL_NAME)
AC_SUBST(BITCOIN_CHAINSTATE_NAME)
AC_SUBST(BITCOIN_WALLET_TOOL_NAME)
AC_SUBST(BITCOIN_MP_NODE_NAME)
AC_SUBST(BITCOIN_MP_GUI_NAME)

AC_SUBST(RELDFLAGS)
AC_SUBST(LIBTOOL_APP_LDFLAGS)
AC_SUBST(USE_UPNP)
AC_SUBST(USE_QRCODE)
AC_SUBST(USE_IPV6)
# AC_SUBST(INCLUDES)
AC_SUBST(BOOST_LIBS)
AC_SUBST(MOC_DEFS)
AC_SUBST(QT_INCLUDES)
AC_SUBST(QT_TEST_LIBS)
AC_SUBST(QT_LIBS)
AC_SUBST(QT_DBUS_LIBS)
AC_SUBST(QT_DBUS_INCLUDES)
AC_SUBST(QT_TEST_INCLUDES)
AC_SUBST(TESTDEFS)
AC_SUBST(MINIUPNPC_CPPFLAGS)
AC_SUBST(MINIUPNPC_LIBS)
AC_SUBST(LEVELDB_TARGET_FLAGS)
AC_SUBST(BUILD_TEST)
AC_SUBST(BUILD_QT)
AC_SUBST(BUILD_TEST_QT)
AC_CONFIG_FILES([Makefile src/Makefile])
# AC_CONFIG_FILES([qa/pull-tester/run-bitcoind-for-test.sh],[chmod +x qa/pull-tester/run-bitcoind-for-test.sh])
# AC_CONFIG_FILES([qa/pull-tester/build-tests.sh],[chmod +x qa/pull-tester/build-tests.sh])
AC_OUTPUT


dnl Replace the BUILDDIR path with the correct Windows path if compiling on Native Windows
case ${OS} in
   *Windows*)
     sed  's/BUILDDIR="\/\([[a-z]]\)/BUILDDIR="\1:/'  test/config.ini > test/config-2.ini
     mv test/config-2.ini test/config.ini
   ;;
esac

echo
echo "Options used to compile and link:"
echo "  external signer = $use_external_signer"
echo "  multiprocess    = $build_multiprocess"
echo "  with experimental syscall sandbox support = $use_syscall_sandbox"
echo "  with libs       = $build_bitcoin_libs"
echo "  with wallet     = $enable_wallet"
if test "$enable_wallet" != "no"; then
    echo "    with sqlite   = $use_sqlite"
    echo "    with bdb      = $use_bdb"
fi
# echo "  with gui / qt   = $bitcoin_enable_qt"
# if test $bitcoin_enable_qt != "no"; then
#     echo "    with qr       = $use_qr"
# fi
# echo "  with zmq        = $use_zmq"
# if test $enable_fuzz = "no"; then
#     echo "  with test       = $use_tests"
# else
#     echo "  with test       = not building test_bitcoin because fuzzing is enabled"
#     echo "    with fuzz     = $enable_fuzz"
# fi
echo "  with gpu        = $use_gpu"
echo "  with bench      = $use_bench"
echo "  with upnp       = $use_upnp"
echo "  with natpmp     = $use_natpmp"
echo "  use asm         = $use_asm"
echo "  ebpf tracing    = $have_sdt"
echo "  sanitizers      = $use_sanitizers"
echo "  debug enabled   = $use_debug"
echo "  gprof enabled   = $enable_gprof"
echo "  werror          = $enable_werror"
echo "  LTO             = $enable_lto"
echo
echo "  target os       = $host_os"
echo "  build os        = $build_os"
echo
echo "  CC              = $CC"
echo "  CFLAGS          = $PTHREAD_CFLAGS $CFLAGS"
echo "  CPPFLAGS        = $DEBUG_CPPFLAGS $HARDENED_CPPFLAGS $CPPFLAGS"
echo "  CXX             = $CXX"
echo "  CXXFLAGS        = $LTO_CXXFLAGS $DEBUG_CXXFLAGS $HARDENED_CXXFLAGS $WARN_CXXFLAGS $NOWARN_CXXFLAGS $ERROR_CXXFLAGS $GPROF_CXXFLAGS $CXXFLAGS"
echo "  LDFLAGS         = $LTO_LDFLAGS $PTHREAD_LIBS $HARDENED_LDFLAGS $GPROF_LDFLAGS $LDFLAGS"
echo "  ARFLAGS         = $ARFLAGS"
echo "  TESTDEFS        = $TESTDEFS"
echo "  NVXX            = $NVCC"
echo "  NVXXCFLAGS      = $NVCC_CFLAGS"
echo "  CUDA_CFLAGS     = $CUDA_CFLAGS"
echo "  CUDA_LDFLAGS    = $CUDA_LDFLAGS"
echo
