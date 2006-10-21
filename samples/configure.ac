dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(axis2c-sample-src, 0.95)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([tar-ustar])
AC_PREFIX_DEFAULT(/usr/local/axis2c)

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

CFLAGS="$CFLAGS -ansi -Wall -D_LARGEFILE64_SOURCE -Werror"
LDFLAGS="$LDFLAGS -lpthread"

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

AC_MSG_CHECKING(To Use Axis2 C AXIOM. This is a compulsory module to build Axis2 C)
AC_ARG_WITH(axiom,
[  --with-axiom[=PATH]      use axiom.],
[ case "$withval" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    dnl Find axiom include dir in the path
    if test -d $withval; then
        axiominc="-I$withval"
    dnl else find the axiom include dir in $(AXIS2C_HOME)/include
    elif test -d '$(AXIS2C_HOME)/include'; then
        axiominc="-I$(AXIS2C_HOME)/include"
    else
        AC_MSG_ERROR(could not find axiom. stop)
    fi
    ;;
  esac ],
  AC_MSG_RESULT(no)
)

AC_MSG_CHECKING(To Use Axis2 C Util. This is a compulsory module to build Axis2 C)
AC_ARG_WITH(axis2_util,
[  --with-axis2_util[=PATH]      use axis2_util.],
[ case "$withval" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    dnl Find axis2_util include dir in the path
    if test -d $withval; then
        axis2_utilinc="-I$withval"
    dnl else find the axis2_util include dir in $(AXIS2C_HOME)/include
    elif test -d '$(AXIS2C_HOME)/include'; then
        axis2_utilinc="-I$(AXIS2C_HOME)/include"
    else
        AC_MSG_ERROR(could not find axis2_util. stop)
    fi
    ;;
  esac ],
  AC_MSG_RESULT(no)
)


APACHE2INC=$apache2inc
AXIOMINC=$axiominc
UTILINC=$axis2_utilinc
APRINC=$aprinc

AC_SUBST(APACHE2INC)
AC_SUBST(APRINC)
AC_SUBST(AXIOMINC)
AC_SUBST(UTILINC)
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
    server/Calculator/Makefile \
    server/notify/Makefile \
    server/sg_math/Makefile \
    server/mtom/Makefile \
    client/Makefile \
    client/echo/Makefile \
    client/math/Makefile \
    client/dynamic_client/Makefile \
    client/mtom/Makefile \
    client/mtom/resources/Makefile \
    client/notify/Makefile \
    client/google/Makefile \
    user_guide/Makefile \
    user_guide/clients/Makefile \
    ])
    
AC_OUTPUT

