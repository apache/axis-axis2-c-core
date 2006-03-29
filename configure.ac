dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(axis2c-src, 0.90)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
#AC_PREFIX_DEFAULT(/usr/local/axis2)
AC_PREFIX_DEFAULT(/usr/local/axis2c)

#AC_CONFIG_SRCDIR([modules/common/src/axis2.c])

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AM_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl Checks for libraries.
AC_CHECK_LIB(dl, dlopen)
AC_CHECK_LIB(cutest, CuTestInit)

#CFLAGS="$CFLAGS -ansi -Wall -D_LARGEFILE64_SOURCE -g3 -O2"
CFLAGS="$CFLAGS -ansi -Wall -D_LARGEFILE64_SOURCE"
#CFLAGS="$CFLAGS -Wall -ansi -Wshadow -DLINUX=2 -D_REENTRANT -D_GNU_SOURCE \
#    -D_LARGEFILE64_SOURCE -g -O2 -pthread"
LDFLAGS="-lpthread"

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
#AC_CHECK_FUNCS([memmove])

AC_MSG_CHECKING(whether to use apache2 as server transport)
AC_ARG_WITH(apache2,
[  --with-apache2[=PATH]      use apache2 as server transport.],
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
[  --with-apr[=PATH]      use apr.],
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
    else
        AC_MSG_ERROR(could not find apr. stop)
    fi
    ;;
  esac ],
  AC_MSG_RESULT(no)
)

AC_MSG_CHECKING(whether to build guththila xml parser library)
AC_ARG_ENABLE(guththila, [  --enable-guththila    build guththila xml parser library wrapper. default=no],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    GUTHTHILA_DIR=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    GUTHTHILA_DIR="guththila"

    ;;
  esac ],
  AC_MSG_RESULT(no)
  GUTHTHILA_DIR=""
)

AC_MSG_CHECKING(whether to build libxml2 xml parser library)
AC_ARG_ENABLE(libxml2, [  --enable-libxml2    build libxml2 xml parser library wrapper. default=yes],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    LIBXML2_DIR=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    LIBXML2_DIR="libxml2"

    ;;
  esac ],
  AC_MSG_RESULT(yes)
  LIBXML2_DIR="libxml2"
)

AC_MSG_CHECKING(whether to build tests)
AC_ARG_ENABLE(tests, [  --enable-tests    build tests. default=no],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    TESTDIR=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    TESTDIR="test"

    ;;
  esac ],
  AC_MSG_RESULT(no)
  TESTDIR=""
)

AC_MSG_CHECKING(whether to enable trace)
AC_ARG_ENABLE(tests, [  --enable-trace    enable trace. default=no],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    CFLAGS="$CFLAGS"
    ;;
  *)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -DAXIS2_TRACE"
    ;;
  esac ],
  AC_MSG_RESULT(no)
  CFLAGS="$CFLAGS"
)

AC_ARG_ENABLE(tests, [  --enable-multi_threaded  enable trace. default=yes],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    CFLAGS="$CFLAGS"
    ;;
  *)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -DAXIS2_SVR_MULTI_THREADED"
    ;;
  esac ],
  AC_MSG_RESULT(no)
  CFLAGS="$CFLAGS"
)

AC_MSG_CHECKING(whether to build dynamic invocation client library)
AC_ARG_ENABLE(diclient, [  --enable-diclient    build diclient library wrapper. default=no],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    DICLIENT_DIR=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    DICLIENT_DIR="diclient"

    ;;
  esac ],
  AC_MSG_RESULT(yes)
  DICLIENT_DIR="diclient"
)

PKG_CHECK_MODULES(LIBXML2, libxml-2.0)
CFLAGS="$CFLAGS $LIBXML2_CFLAGS $GUTHTHILA_CFLAGS"

APACHE2INC=$apache2inc
APRINC=$aprinc

AC_SUBST(APACHE2INC)
AC_SUBST(APRINC)
AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_LIBS)
AC_SUBST(LIBXML2_DIR)
AC_SUBST(DICLIENT_DIR)
AC_SUBST(GUTHTHILA_DIR)
AC_SUBST(GUTHTHILA_LIBS)
AC_SUBST(TESTDIR)
AC_SUBST(SAMPLES)
AC_SUBST(APACHE2BUILD)

AC_CONFIG_FILES([Makefile \
    modules/Makefile \
    modules/core/Makefile \
    modules/core/description/Makefile \
    modules/core/context/Makefile \
    modules/core/engine/Makefile \
    modules/core/addr/Makefile \
    modules/core/phaseresolver/Makefile \
    modules/core/transport/Makefile \
    modules/core/transport/http/Makefile \
    modules/core/transport/http/sender/Makefile \
    modules/core/transport/http/receiver/Makefile \
    modules/core/transport/http/server/simple_axis2_server/Makefile \
    modules/core/transport/http/server/Makefile \
    modules/core/transport/http/server/apache2/Makefile \
    modules/core/deployment/Makefile \
    modules/core/clientapi/Makefile \
    modules/core/clientapi/diclient/Makefile \
    modules/core/receivers/Makefile \
    modules/core/util/Makefile \
    modules/xml/Makefile \
    modules/xml/parser/Makefile \
    modules/xml/parser/libxml2/Makefile \
    modules/xml/soap/Makefile \
    modules/xml/om/Makefile \
    modules/xml/attachments/Makefile \
    modules/util/Makefile \
    modules/platforms/unix/Makefile \
    modules/wsdl/Makefile \
    modules/wsdl/builder/Makefile \
    modules/mod_addr/Makefile \
    test/Makefile \
    test/xml/Makefile \
    test/xml/om/Makefile \
    test/xml/soap/Makefile \
    test/wsdl/Makefile \
    test/wsdl/builder/Makefile \
    test/wsdl/builder/print/Makefile \
    test/core/Makefile \
    test/core/description/Makefile \
    test/core/deployment/Makefile \
    test/core/context/Makefile \
    test/core/clientapi/Makefile \
    test/core/engine/Makefile \
    test/core/addr/Makefile \
    test/util/Makefile \
    test/unit/Makefile \
    test/unit/core/Makefile \
    test/unit/core/description/Makefile \
    test/unit/core/engine/Makefile \
    test/unit/core/context/Makefile \
    test/unit/core/phaseresolver/Makefile \
    test/unit/core/deployment/Makefile \
    test/unit/util/Makefile \
    test/unit/wsdl/Makefile \
    test/unit/xml/Makefile \
    test/unit/xml/om/Makefile\
    test/core/transport/Makefile\
    test/core/transport/http/Makefile \
    test/itest/Makefile \
    test/itest/whitemesa/Makefile \
    test/itest/whitemesa/round1/Makefile \
    test/itest/whitemesa/round2/Makefile \
    test/tools/Makefile \
    ides/Makefile \
    ides/anjuta/Makefile
    include/Makefile \
    ])
    
AC_OUTPUT
