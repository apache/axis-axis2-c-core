dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(axis2c-src, M0.5)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_PREFIX_DEFAULT(/usr/local/axis2c/samples)

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

CFLAGS="$CFLAGS -ansi -Wall -D_LARGEFILE64_SOURCE"
LDFLAGS="-lpthread"

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

PKG_CHECK_MODULES(LIBXML2, libxml-2.0)
CFLAGS="$CFLAGS $LIBXML2_CFLAGS $GUTHTHILA_CFLAGS"

AC_MSG_CHECKING(whether to build dynamic invocation client)
AC_ARG_ENABLE(diclient, [  --enable-diclient    build diclient. default=no],
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
  DICLIENT_DIR=""
)

AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_LIBS)
AC_SUBST(LIBXML2_DIR)
AC_SUBST(GUTHTHILA_DIR)
AC_SUBST(GUTHTHILA_LIBS)
AC_SUBST(DICLIENT_DIR)

AC_CONFIG_FILES([Makefile \
    server/Makefile \
    server/echo/Makefile \
    server/math/Makefile \
    server/notify/Makefile \
    client/Makefile \
    client/echo/Makefile \
    client/math/Makefile \
    client/addr_echo/Makefile \
    client/diclient/Makefile \
    client/echo_non_blocking/Makefile \
    client/notify/Makefile \
    client/google/Makefile \
    ])
    
AC_OUTPUT
