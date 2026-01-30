dnl Licensed to the Apache Software Foundation (ASF) under one or more
dnl contributor license agreements.  See the NOTICE file distributed with
dnl this work for additional information regarding copyright ownership.
dnl The ASF licenses this file to You under the Apache License, Version 2.0
dnl (the "License"); you may not use this file except in compliance with
dnl the License.  You may obtain a copy of the License at
dnl 
dnl   http://www.apache.org/licenses/LICENSE-2.0
dnl 
dnl Unless required by applicable law or agreed to in writing, software
dnl distributed under the License is distributed on an "AS IS" BASIS,
dnl WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
dnl See the License for the specific language governing permissions and
dnl limitations under the License.
dnl run autogen.sh to generate the configure script.
AC_PREREQ(2.59)

AC_INIT(axis2c-src, 2.0.0)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([tar-ustar subdir-objects foreign])
AC_PREFIX_DEFAULT(/usr/local/axis2c)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET


dnl check for flavours of varargs macros (test from GLib)
AC_MSG_CHECKING(for ISO C99 varargs macros in C)
AC_TRY_COMPILE([],[
int a(int p1, int p2, int p3);
#define call_a(...) a(1,__VA_ARGS__)
call_a(2,3);
],axis2c_have_iso_c_varargs=yes,axis2c_have_iso_c_varargs=no)
AC_MSG_RESULT($axis2c_have_iso_c_varargs)

AC_MSG_CHECKING(for GNUC varargs macros)
AC_TRY_COMPILE([],[
int a(int p1, int p2, int p3);
#define call_a(params...) a(1,params)
call_a(2,3);
],axis2c_have_gnuc_varargs=yes,axis2c_have_gnuc_varargs=no)
AC_MSG_RESULT($axis2c_have_gnuc_varargs)

dnl Output varargs tests
if test x$axis2c_have_iso_c_varargs = xyes; then
    AC_DEFINE(HAVE_ISO_VARARGS,1,[Have ISO C99 varargs macros])
fi
if test x$axis2c_have_gnuc_varargs = xyes; then
    AC_DEFINE(HAVE_GNUC_VARARGS,1,[Have GNU-style varargs macros])
fi

dnl Checks for libraries.
AC_CHECK_LIB(dl, dlopen)
AC_CHECK_LIB(z, inflate)
AC_CHECK_LIB(socket, socket)

if test -d $srcdir/util; then
    AC_CONFIG_SUBDIRS(util)
fi

if test -d $srcdir/axiom; then
    AC_CONFIG_SUBDIRS(axiom)
fi

if test -d $srcdir/neethi; then
    AC_CONFIG_SUBDIRS(neethi)
fi

CFLAGS="$CFLAGS -D_LARGEFILE64_SOURCE"
CPPFLAGS="$CPPFLAGS -D_LARGEFILE64_SOURCE"

dnl Detect Android target for conditional compilation
AC_MSG_CHECKING([for Android target])
case "$host" in
    *-linux-android*|*-android*)
        android_build=yes
        AC_DEFINE([AXIS2_ANDROID_BUILD], [1], [Building for Android])
        ;;
    *)
        android_build=no
        ;;
esac
AC_MSG_RESULT([$android_build])
AM_CONDITIONAL([BUILD_FOR_ANDROID], [test "x$android_build" = "xyes"])

if test "$GCC" = "yes"; then
    dnl Use gnu99 mode for compatibility with json-c and other modern libraries
    dnl The -ansi flag conflicts with json-c headers which use C99 inline
    CFLAGS="$CFLAGS -std=gnu99 -Wall -Wno-implicit-function-declaration -g -D_GNU_SOURCE"
    AC_MSG_NOTICE([Using -std=gnu99 for compatibility])
fi

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([net/if.h], [], [],
[#include <stdio.h>
#if STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# if HAVE_STDLIB_H
# include <stdlib.h>
# endif
#endif
#if HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
])
AC_CHECK_HEADERS([linux/if.h],[],[],
[
#if HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
])
AC_CHECK_HEADERS([net/if_types.h])
AC_CHECK_HEADERS([net/if_dl.h])

dnl This is a check to see if we are running MacOS X
dnl It may be better to do a Darwin check
AC_CHECK_HEADERS([sys/appleapiopts.h]) 
dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.
dnl AC_FUNC_MALLOC
dnl AC_FUNC_REALLOC
#AC_CHECK_FUNCS([memmove])

AC_PATH_TOOL(PKGCONFIG, pkg-config)

AC_MSG_CHECKING(whether to use archive)
AC_ARG_WITH(archive,
[  --with-archive[=PATH]      Find the zlib header files in 'PATH'.
    If you omit the '=PATH' part completely, the configure script will search
    '/usr/include/' for zlib headers.],
[ case "$withval" in
  no)
    AC_MSG_RESULT(no)
    ZLIBBUILD=""
    zliblibs=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    zliblibs="minizip/libaxis2_minizip.la"
    CFLAGS="$CFLAGS -DAXIS2_ARCHIVE_ENABLED"
    CPPFLAGS="$CPPFLAGS -DAXIS2_ARCHIVE_ENABLED"
    if test -d $withval; then
        zlibinc="-I$withval"
    elif test -d '/usr/include'; then
        zlibinc="-I/usr/include"
    else
        AC_MSG_ERROR(could not find zlib stop)
    fi
    ZLIBBUILD="minizip"
    ;;
  esac ],
  AC_MSG_RESULT(no)
)

AC_MSG_CHECKING(whether to set -Werror)
AC_ARG_ENABLE(werror, [  --enable-werror  treat warnings as errors during build],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -Werror"
    ;;
  esac ],
  AC_MSG_RESULT(no)
)


AC_MSG_CHECKING(whether to build xpath)
AC_ARG_ENABLE(xpath, [  --enable-xpath    build xpath. default=yes],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    XPATH_DIR="xpath"

    ;;
  esac ],
  AC_MSG_RESULT(yes)
  XPATH_DIR="xpath"
)

GUTHTHILA_LIBS=
GUTHTHILA_DIR=

AC_MSG_CHECKING(whether to build guththila xml parser library)
AC_ARG_ENABLE(guththila, [  --enable-guththila
                          build Guththila XML parser library wrapper (default=yes)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    guththilla_enabled=no
    ;;
  *)
    AC_MSG_RESULT(yes)
    guththilla_enabled=yes
    ;;
  esac ], [
  AC_MSG_RESULT(yes)
  guththilla_enabled=yes
])

AS_IF([test $guththilla_enabled = yes], [
  CFLAGS="$CFLAGS -DAXIS2_GUTHTHILA_ENABLED"
  CPPFLAGS="$CPPFLAGS -DAXIS2_GUTHTHILA_ENABLED"
  WRAPPER_DIR="guththila"
  AC_CONFIG_SUBDIRS(guththila)
  GUTHTHILA_LIBS="/guththila/src/"
  GUTHTHILA_DIR="guththila"
])
AS_UNSET(guththilla_enabled)

AC_MSG_CHECKING(whether to build libxml2 xml parser library)
AC_ARG_ENABLE(libxml2, [  --enable-libxml2    
                          build Libxml2 XML parser library wrapper (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    WRAPPER_DIR="libxml2"
    PKG_CHECK_MODULES(LIBXML2, libxml-2.0)
    CFLAGS="$CFLAGS -DAXIS2_LIBXML2_ENABLED"
    CPPFLAGS="$CPPFLAGS $PARSER_CFLAGS -DAXIS2_LIBXML2_ENABLED"
    LDFLAGS="$LDFLAGS $PARSER_LIBS"
    ;;
  esac ],
  AC_MSG_RESULT(no)
)

AC_MSG_CHECKING(whether to build heimdal ntlm wrapper library)
AC_ARG_ENABLE(heimdal, [  --enable-heimdal    
                          build heimdal ntlm library wrapper (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    NTLM_WRAPPER_DIR=""
    NTLM_DIR=""
    NTLM_LIB=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    NTLM_WRAPPER_DIR="heimdal"
    NTLM_DIR="ntlm"
    #PKG_CHECK_MODULES(HEIMDAL, heimdal-1.4)
    CFLAGS="$CFLAGS -DAXIS2_HEIMDAL_ENABLED -DAXIS2_NTLM_ENABLED"
    CPPFLAGS="$CPPFLAGS $NTLM_CFLAGS -DAXIS2_HEIMDAL_ENABLED -DAXIS2_NTLM_ENABLED"
    LDFLAGS="$LDFLAGS $NTLM_LDFLAGS"
    NTLM_LIB="./ntlm/heimdal/libaxis2_ntlm.la"
    ;;
  esac ],
  AC_MSG_RESULT(no)
)

AC_MSG_CHECKING(whether to build libntlm ntlm wrapper library)
AC_ARG_ENABLE(libntlm, [  --enable-libntlm    
                          build libntlm ntlm library wrapper (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    NTLM_WRAPPER_DIR=""
    NTLM_DIR=""
    NTLM_LIB=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    NTLM_WRAPPER_DIR="libntlm"
    NTLM_DIR="ntlm"
    CFLAGS="$CFLAGS -DAXIS2_LIBNTLM_ENABLED -DAXIS2_NTLM_ENABLED"
    CPPFLAGS="$CPPFLAGS $NTLM_CFLAGS -DAXIS2_LIBNTLM_ENABLED -DAXIS2_NTLM_ENABLED -DLIBNTLM_CFLAGS"
    LDFLAGS="$LDFLAGS $NTLM_LDFLAGS $LIBNTLM_LIBS"
    NTLM_LIB="./ntlm/libntlm/libaxis2_ntlm.la"
    ;;
  esac ],
  AC_MSG_RESULT(no)
)


AC_MSG_CHECKING(whether to build tcp transport)
AC_ARG_ENABLE(tcp, [  --enable-tcp
                          build tcp transport (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    TCP_DIR="tcp"

    ;;
  esac ],
  AC_MSG_RESULT(no)
)

AC_MSG_CHECKING(whether to use cgi transport)
AC_ARG_ENABLE(cgi, [  --enable-cgi
                          build cgi transport (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    CGI_DIR="CGI"

    ;;
  esac ],
  AC_MSG_RESULT(no)
)

AC_MSG_CHECKING(whether to use apache2 as server transport)
AC_ARG_WITH(apache2,
[  --with-apache2[=PATH]      Find the Apache2 HTTP Web server header files in 'PATH'.
    If this option is given, the Apache2 httpd module would be built.
    'PATH' should point to Apache2 httpd include files location.
    If you omit the '=PATH' part completely, the configure script will search
    '/usr/include/apache2' or '/usr/include/httpd' for Apache2 headers.],
[ case "$withval" in
  no)
    AC_MSG_RESULT(no)
    APACHE2BUILD=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    dnl Find apache2 include dir in the path pointed by APACHE2_HOME env variable
    if test -d $withval; then
        apache2inc="-I$withval"
    dnl else find the apache2 include dir in /usr/local/apache2
    elif test -d '/usr/include/apache2'; then
        apache2inc="-I/usr/include/apache2"
    elif test -d '/usr/include/httpd'; then
        apache2inc="-I/usr/include/httpd"
    else
        AC_MSG_ERROR(could not find apache2. stop)
    fi
    APACHE2BUILD="apache2"
    ;;
  esac ],
  AC_MSG_RESULT(no)
)

AC_MSG_CHECKING(whether to use apr. Note that if you build with apache2 you might need to do this.)
AC_ARG_WITH(apr,
[  --with-apr[=PATH]      Find the APR header files in 'PATH'.
    Some Apache2 distributions, specially development versions, install APR 
    (Apache Portable Run-time) include files in a separate location. 
    In that case, to build Apache2 httpd module, this option is also required.
    'PATH' should point to APR include files location.
    If you omit the '=PATH' part completely, the configure script will search
    '/usr/include/apr-0' or '/usr/include/apr-1' for APR headers.],
[ case "$withval" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    dnl Find apr include dir in the path
    if test -d $withval; then
        aprinc="-I$withval"
    dnl else find the apache2 include dir in /usr/local/apache2
    elif test -d '/usr/include/apr-0'; then
        aprinc="-I/usr/include/apr-0"
    elif test -d '/usr/include/apr-1'; then
        aprinc="-I/usr/include/apr-1"
    elif test -d '/usr/include/apr-1.0'; then
        aprinc="-I/usr/include/apr-1.0"
    else
        AC_MSG_ERROR(could not find apr. stop)
    fi
    ;;
  esac ],
  [
  AC_MSG_RESULT(no, checking for APR automatically)
  dnl Auto-detect APR when --with-apr is not specified
  if test -d '/usr/include/apr-0'; then
      aprinc="-I/usr/include/apr-0"
      AC_MSG_NOTICE([APR found: /usr/include/apr-0])
  elif test -d '/usr/include/apr-1'; then
      aprinc="-I/usr/include/apr-1"
      AC_MSG_NOTICE([APR found: /usr/include/apr-1])
  elif test -d '/usr/include/apr-1.0'; then
      aprinc="-I/usr/include/apr-1.0"
      AC_MSG_NOTICE([APR found: /usr/include/apr-1.0])
  else
      AC_MSG_NOTICE([APR not found in standard locations, Apache2 module may not build])
  fi
  ]
)

AC_MSG_CHECKING(whether to use use the Google test framework)
AC_ARG_WITH(gtest,
            [  --with-gtest[=PATH]      Find the gtest source files in 'PATH'.],
[ case "$withval" in
  no)
    AC_MSG_RESULT(no)
    USE_GTEST=""
    GTEST_DIR=""
    GTEST=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    GTEST_DIR="$withval"
    GTEST="gtest"
    CXXFLAGS="$CXXFLAGS -g -Wall -Wextra -pthread"
    ;;
  esac ],
  AC_MSG_RESULT(no)
)


AC_MSG_CHECKING(whether to build tests)
AC_ARG_ENABLE(tests, [  --enable-tests    
                          build tests (default=no)],
[ case "${enableval}" in
  yes)
    AC_MSG_RESULT(yes)
    TESTDIR="test"
    ;;
  *)
    AC_MSG_RESULT(no)
    TESTDIR=""

    ;;
  esac ],
  AC_MSG_RESULT(no)
  TESTDIR=""
)


AC_MSG_CHECKING(whether to enable trace)
AC_ARG_ENABLE(trace, [  --enable-trace    
                          enable logging trace messages useful when debugging (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    CFLAGS="$CFLAGS"
    CPPFLAGS="$CPPFLAGS"
    ;;
  *)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -DAXIS2_TRACE"
    CPPFLAGS="$CPPFLAGS -DAXIS2_TRACE"
    ;;
  esac ],
  AC_MSG_RESULT(no)
  CFLAGS="$CFLAGS"
  CPPFLAGS="$CPPFLAGS"
)

AC_MSG_CHECKING(whether to enable code coverage statistics)
AC_ARG_ENABLE(coverage, [  --enable-coverage  
                          enable code coverage statistics (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    CFLAGS="$CFLAGS"
    CPPFLAGS="$CPPFLAGS"
    ;;
  *)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -fprofile-arcs -ftest-coverage"
    CPPFLAGS="$CPPFLAGS -fprofile-arcs -ftest-coverage"
    CXXFLAGS="$CXXFLAGS -fprofile-arcs -ftest-coverage"
    LDFLAGS="$LDFLAGS -lgcov --coverage"
    ;;
  esac ],[
  AC_MSG_RESULT(no)
  CFLAGS="$CFLAGS"
  CPPFLAGS="$CPPFLAGS"]
)

AC_MSG_CHECKING(whether to enable address sanitizer)
AC_ARG_ENABLE(asan, [  --enable-asan
                          enable address sanitizer (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    CFLAGS="$CFLAGS"
    CPPFLAGS="$CPPFLAGS"
    enable_asan=no
    ;;
  *)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -fsanitize=address -fno-omit-frame-pointer"
    CPPFLAGS="$CPPFLAGS -fsanitize=address -fno-omit-frame-pointer"
    CXXFLAGS="$CXXFLAGS -fsanitize=address -fno-omit-frame-pointer"
    dnl LDFLAGS must include -fsanitize=address for executables to link correctly
    LDFLAGS="$LDFLAGS -fsanitize=address"
    enable_asan=yes
    ;;
  esac ],[
  AC_MSG_RESULT(no)
  CFLAGS="$CFLAGS"
  CPPFLAGS="$CPPFLAGS"
  enable_asan=no]
)

dnl Create automake conditional for ASAN
AM_CONDITIONAL([ENABLE_ASAN], [test "x$enable_asan" = "xyes"])

dnl Detect ASAN library path for test preloading (only when ASAN enabled)
dnl This is needed because libtool-wrapped tests need LD_PRELOAD to work with ASAN
ASAN_PRELOAD=""
if test "x$enable_asan" = "xyes"; then
    AC_MSG_CHECKING([for ASAN library path])
    dnl Try common locations for libasan
    for asan_path in /lib/x86_64-linux-gnu/libasan.so.8 \
                     /lib/x86_64-linux-gnu/libasan.so.6 \
                     /lib64/libasan.so.8 \
                     /lib64/libasan.so.6 \
                     /usr/lib/libasan.so; do
        if test -f "$asan_path"; then
            ASAN_PRELOAD="$asan_path"
            break
        fi
    done
    if test -n "$ASAN_PRELOAD"; then
        AC_MSG_RESULT([$ASAN_PRELOAD])
    else
        AC_MSG_RESULT([not found - tests may fail])
    fi
fi
AC_SUBST([ASAN_PRELOAD])


AC_MSG_CHECKING(whether to enable multi threading)
AC_ARG_ENABLE(multi-thread, [  --enable-multi-thread  
                          enable multi threaded axis2 worker (default=yes)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    CFLAGS="$CFLAGS"
    CPPFLAGS="$CPPFLAGS"
    ;;
  *)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -DAXIS2_SVR_MULTI_THREADED"
    CPPFLAGS="$CPPFLAGS -DAXIS2_SVR_MULTI_THREADED"
    ;;
  esac ],[
  AC_MSG_RESULT(yes)
  CFLAGS="$CFLAGS -DAXIS2_SVR_MULTI_THREADED"
  CPPFLAGS="$CPPFLAGS -DAXIS2_SVR_MULTI_THREADED"]
)

AC_MSG_CHECKING(whether to use openssl)
AC_ARG_WITH(openssl,
[  --with-openssl[=PATH]      Find the openssl header files in 'PATH'.
    If this option is given, Openssl support in transport will be enabled.
    'PATH' should point to openssl installation location.
    If you omit the '=PATH' part completely, the configure script will search
    '/usr/include/openssl' for openssl headers.],
[ case "$withval" in
  no)
    AC_MSG_RESULT(no)
    ssl_enabled=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    dnl Find openssl dir in the path
    if test -d $withval; then
        opensslinc="-I$withval/include"
		openssllib="-L$withval/lib"
		CFLAGS="$CFLAGS -DAXIS2_SSL_ENABLED"
		CPPFLAGS="$CPPFLAGS -DAXIS2_SSL_ENABLED"
		ssl_enabled=true
    dnl else find the include dir in /usr/local/openssl
    elif test -d '/usr/include/openssl'; then
		CFLAGS="$CFLAGS -DAXIS2_SSL_ENABLED"
		CPPFLAGS="$CPPFLAGS -DAXIS2_SSL_ENABLED"
        opensslinc="-I/usr/include/openssl"
		openssllib=""
		ssl_enabled=true
    else
        AC_MSG_ERROR(could not find openssl. stop)
    fi
    ;;
  esac ],
  AC_MSG_RESULT(no)
)

AC_MSG_CHECKING(whether to use libcurl)
AC_ARG_ENABLE(libcurl, [  --enable-libcurl  
                          enable libcurl based client transport (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    CFLAGS="$CFLAGS"
    CPPFLAGS="$CPPFLAGS"
    libcurl_enabled=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -DAXIS2_LIBCURL_ENABLED"
    CPPFLAGS="$CPPFLAGS -DAXIS2_LIBCURL_ENABLED"
    libcurl_enabled=true
    ;;
  esac ],
  AC_MSG_RESULT(no)
  CFLAGS="$CFLAGS"
  CPPFLAGS="$CPPFLAGS"
)

AC_MSG_CHECKING(whether to use JSON)
AC_ARG_ENABLE(json, [  --enable-json
                          enable json support (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    CFLAGS="$CFLAGS"
    CPPFLAGS="$CPPFLAGS"
    json_enabled=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS $JSON_CFLAGS -DAXIS2_JSON_ENABLED"
    CPPFLAGS="$CPPFLAGS $JSON_LIBS -DAXIS2_JSON_ENABLED"
    json_enabled=true
    PKG_CHECK_MODULES([JSON], [json-c >= 0.18])
    ;;
  esac ],
  AC_MSG_RESULT(no)
  CFLAGS="$CFLAGS"
  CPPFLAGS="$CPPFLAGS"
)

AC_MSG_CHECKING(whether to enable HTTP/2 transport)
AC_ARG_ENABLE(http2, [  --enable-http2
                          enable HTTP/2 transport support (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    http2_enabled=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    http2_enabled=true

    dnl Check for nghttp2 library
    PKG_CHECK_MODULES([NGHTTP2], [libnghttp2 >= 1.0.0], [
        CFLAGS="$CFLAGS $NGHTTP2_CFLAGS -DWITH_NGHTTP2"
        CPPFLAGS="$CPPFLAGS $NGHTTP2_CFLAGS -DWITH_NGHTTP2"
        LDFLAGS="$LDFLAGS $NGHTTP2_LIBS"
    ], [
        AC_MSG_ERROR([
HTTP/2 support requires libnghttp2 >= 1.0.0 but it was not found.

Please install the required packages:

Ubuntu/Debian:
    sudo apt install libnghttp2-dev libnghttp2-14 libssl-dev

RedHat/CentOS:
    sudo yum install libnghttp2-devel openssl-devel

Fedora:
    sudo dnf install libnghttp2-devel openssl-devel

Alternatively, disable HTTP/2 support by removing --enable-http2 from configure.

For more help, see: HTTP2_TROUBLESHOOTING.md
        ])
    ])

    dnl Check for OpenSSL (required for HTTPS/HTTP2)
    PKG_CHECK_MODULES([OPENSSL], [openssl >= 1.0.0], [
        CFLAGS="$CFLAGS $OPENSSL_CFLAGS"
        CPPFLAGS="$CPPFLAGS $OPENSSL_CFLAGS"
        LDFLAGS="$LDFLAGS $OPENSSL_LIBS"
    ], [
        AC_CHECK_LIB([ssl], [SSL_library_init], [], [
            AC_MSG_ERROR([
HTTP/2 support requires OpenSSL >= 1.0.0 for HTTPS and ALPN negotiation.

Please install OpenSSL development packages:

Ubuntu/Debian:
    sudo apt install libssl-dev

RedHat/CentOS:
    sudo yum install openssl-devel

Fedora:
    sudo dnf install openssl-devel

For more help, see: HTTP2_TROUBLESHOOTING.md
            ])
        ])
        AC_CHECK_LIB([crypto], [EVP_EncryptInit], [], [
            AC_MSG_ERROR([OpenSSL crypto library is required for HTTP/2 support.])
        ])
    ])

    AC_MSG_NOTICE([HTTP/2 transport enabled with nghttp2 and OpenSSL support])
    ;;
  esac ],
  AC_MSG_RESULT(no)
  http2_enabled=false
)


AC_MSG_CHECKING(whether to build native WSDL2C generator)
AC_ARG_ENABLE(native-codegen, [  --enable-native-codegen
                          build native C WSDL2C code generator (default=yes)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    NATIVE_CODEGEN_DIR=""
    native_codegen_enabled=false
    ;;
  *)
    AC_MSG_RESULT(yes)
    NATIVE_CODEGEN_DIR="tools/codegen/native"
    native_codegen_enabled=true
    ;;
  esac ],
  AC_MSG_RESULT(yes)
  NATIVE_CODEGEN_DIR="tools/codegen/native"
  native_codegen_enabled=true
)

# Check for libxml2 when native codegen is enabled
AS_IF([test x$native_codegen_enabled = xtrue], [
  PKG_CHECK_MODULES([LIBXML2], [libxml-2.0 >= 2.6.0])
])


APACHE2INC=$apache2inc
OPENSSLINC=$opensslinc
OPENSSLLIB=$openssllib
IKSEMELINC=$iksemelinc
APRINC=$aprinc
VERSION_NO="7:0:7"
QPID_HOME=$qpidhome

AC_SUBST(VERSION_NO)
AC_SUBST(APACHE2INC)
AC_SUBST(OPENSSLINC)
AC_SUBST(OPENSSLLIB)
AC_SUBST(APRINC)
AC_SUBST(DICLIENT_DIR)
AC_SUBST(TESTDIR)
AC_SUBST(GTEST_DIR)
AC_SUBST(GTEST)

dnl Check if gtest should be included as a subdirectory
dnl Only include it if we have --with-gtest and the directory has a proper Makefile
gtest_available=false
if test -n "$GTEST" && test -d "$srcdir/gtest" && test -f "$srcdir/gtest/Makefile.am"; then
    dnl Check if gtest will be properly configured
    if test -n "$GTEST_DIR" && test -d "$GTEST_DIR"; then
        gtest_available=true
        AC_MSG_NOTICE([GTest will be built as subdirectory])
    else
        AC_MSG_NOTICE([GTest configured but source directory not found: $GTEST_DIR])
    fi
else
    AC_MSG_NOTICE([GTest not configured as subdirectory])
fi

AM_CONDITIONAL(GTEST_AVAILABLE, test x$gtest_available = xtrue)
AC_SUBST(SAMPLES)
AC_SUBST(APACHE2BUILD)
AC_SUBST(NTLM_CFLAGS)
AC_SUBST(NTLM_LDFLAGS)
AC_SUBST(WRAPPER_DIR)
AC_SUBST(NTLM_WRAPPER_DIR)
AC_SUBST(NTLM_LIB)
AC_SUBST(NTLM_DIR)
AC_SUBST(TCP_DIR)
AC_SUBST(CGI_DIR)
AC_SUBST(GUTHTHILA_DIR)
AC_SUBST(GUTHTHILA_LIBS)
AC_SUBST(ZLIBBUILD)
AC_SUBST(NATIVE_CODEGEN_DIR)
AM_CONDITIONAL(AXIS2_SSL_ENABLED, test x$ssl_enabled = xtrue)
AM_CONDITIONAL(AXIS2_LIBCURL_ENABLED, test x$libcurl_enabled = xtrue)
AM_CONDITIONAL(AXIS2_JSON_ENABLED, test x$json_enabled = xtrue)
AM_CONDITIONAL(AXIS2_HTTP2_ENABLED, test x$http2_enabled = xtrue)
AM_CONDITIONAL(WITH_NGHTTP2, test x$http2_enabled = xtrue)
AM_CONDITIONAL(AXIS2_APACHE2_ENABLED, test x$APACHE2BUILD = xapache2)
AM_CONDITIONAL(AXIS2_NATIVE_CODEGEN_ENABLED, test x$native_codegen_enabled = xtrue)

export NTLM_LDFLAGS
export WRAPPER_DIR
export NTLM_WRAPPER_DIR
export NTLM_LIB
export NTLM_DIR
export prefix 

AC_CONFIG_FILES([Makefile \
    src/Makefile \
    src/core/Makefile \
    src/core/description/Makefile \
    src/core/context/Makefile \
    src/core/engine/Makefile \
    src/core/addr/Makefile \
    src/core/phaseresolver/Makefile \
    src/core/transport/Makefile \
    src/core/transport/http/Makefile \
    src/core/transport/http/common/Makefile \
    src/core/transport/http/util/Makefile \
    src/core/transport/http/sender/Makefile \
    src/core/transport/http/sender/ssl/Makefile \
    src/core/transport/http/sender/libcurl/Makefile \
    src/core/transport/http/sender/ntlm/Makefile \
    src/core/transport/http/sender/ntlm/heimdal/Makefile \
    src/core/transport/http/sender/ntlm/libntlm/Makefile \
    src/core/transport/http/receiver/Makefile \
    src/core/transport/http/server/simple_axis2_server/Makefile \
    src/core/transport/http/server/Makefile \
    src/core/transport/http/server/apache2/Makefile \
    src/core/transport/tcp/Makefile \
    src/core/transport/tcp/sender/Makefile \
    src/core/transport/tcp/receiver/Makefile \
    src/core/transport/tcp/server/Makefile \
    src/core/transport/tcp/server/simple_tcp_server/Makefile \
	src/core/transport/http/server/CGI/Makefile \
    src/core/deployment/Makefile \
    src/core/clientapi/Makefile \
    src/core/receivers/Makefile \
    src/core/util/Makefile \
    src/modules/Makefile \
    src/modules/mod_addr/Makefile \
    src/modules/mod_log/Makefile \
    test/Makefile \
    test/core/Makefile \
    test/core/description/Makefile \
    test/core/clientapi/Makefile \
    test/core/deployment/Makefile \
    test/core/context/Makefile \
    test/core/engine/Makefile \
    test/core/addr/Makefile \
    test/core/util/Makefile \
    test/core/security/Makefile \
    test/core/transport/Makefile\
    test/core/transport/http/Makefile \
    tools/tcpmon/Makefile \
    tools/tcpmon/src/Makefile \
    tools/md5/Makefile \
    tools/md5/src/Makefile \
    tools/codegen/Makefile \
    tools/codegen/native/Makefile \
    tools/codegen/native/test/Makefile \
    ides/Makefile \
    include/Makefile \
    axis2c.pc \
    gtest/Makefile \
	dist.sh \
	bindist.sh \
	axis2c_build.sh
    ])

dnl gtest/Makefile is now configured unconditionally above
dnl The GTEST_AVAILABLE conditional only controls whether it's included in SUBDIRS

dnl Conditionally configure HTTP/2 transport subdirectories
if test x$http2_enabled = xtrue; then
    AC_CONFIG_FILES([
        src/core/transport/h2/Makefile
        src/core/transport/h2/util/Makefile
        src/core/transport/h2/sender/Makefile
        src/core/transport/h2/server/Makefile
        src/core/transport/h2/server/apache2/Makefile
        src/core/transport/h2/test/Makefile
    ])
fi

dnl Conditionally configure HTTP/2 JSON sample services (including camera control and financial benchmark)
if test x$http2_enabled = xtrue && test x$json_enabled = xtrue; then
    AC_CONFIG_FILES([
        samples/user_guide/Makefile
        samples/user_guide/clients/Makefile
        samples/user_guide/bigdata-h2-service/Makefile
        samples/user_guide/login-service/Makefile
        samples/user_guide/testws-service/Makefile
        samples/user_guide/camera-control-service/Makefile
        samples/user_guide/financial-benchmark-service/Makefile
    ])
fi

AC_OUTPUT
