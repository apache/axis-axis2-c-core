dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(wodenc-src, 0.96)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([tar-ustar])
AC_PREFIX_DEFAULT(/usr/local/wodenc)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AM_PROG_LIBTOOL
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

CFLAGS="$CFLAGS -D_LARGEFILE64_SOURCE"
if test "$GCC" = "yes"; then
    CFLAGS="$CFLAGS -ansi -Wall -Wno-implicit-function-declaration -Werror"
fi

LDFLAGS="$LDFLAGS -lpthread"

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
#AC_CHECK_FUNCS([memmove])

AC_MSG_CHECKING(whether to build libxml2 xml parser library)
AC_ARG_ENABLE(libxml2, [  --enable-libxml2    build libxml2 xml parser library wrapper. default=yes],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    WRAPPER_DIR=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    WRAPPER_DIR="libxml2"

    ;;
  esac ],
  AC_MSG_RESULT(yes)
  WRAPPER_DIR="libxml2"
)

AC_MSG_CHECKING(whether to build guththila xml parser library)
AC_ARG_ENABLE(guththila, [  --enable-guththila    build guththila xml parser library wrapper. default=no],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    WRAPPER_DIR="guththila"

    ;;
  esac ],
  AC_MSG_RESULT(no)
)

dnl AC_MSG_CHECKING(whether to build tests)
dnl AC_ARG_ENABLE(tests, [  --enable-tests    build tests. default=no],
dnl [ case "${enableval}" in
dnl   yes)
dnl     AC_MSG_RESULT(yes)
dnl     TESTDIR="test"
dnl     ;;
dnl   *)
dnl     AC_MSG_RESULT(no)
dnl     TESTDIR=""
dnl 
dnl     ;;
dnl   esac ],
dnl   AC_MSG_RESULT(no)
dnl   TESTDIR=""
dnl )


AC_MSG_CHECKING(To Use Xml Schema. This is a compulsory module to build Woden C)
AC_ARG_WITH(axis2_xml_schema,
[  --with-axis2_xml_schema[=PATH]      use axis2_xml_schema.],
[ case "$withval" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    dnl Find axis2_xml_schema include dir in the path
    if test -d $withval; then
        axis2_xml_schemainc="-I$withval"
    dnl else find the axis2_xml_schema include dir in $(AXIS2C_HOME)/include
    elif test -d '$(AXIS2C_HOME)/include'; then
        axis2_xml_schemainc="-I$(AXIS2C_HOME)/include"
    else
        AC_MSG_ERROR(could not find axis2_xml_schema. stop)
    fi
    ;;
  esac ],
  AC_MSG_RESULT(no)
)


AXIOMINC=$axiominc
UTILINC=$axis2_utilinc
XMLSCHEMAINC=$axis2_xml_schemainc

AC_SUBST(AXIOMINC)
AC_SUBST(UTILINC)
AC_SUBST(XMLSCHEMAINC)
AC_SUBST(TESTDIR)
AC_SUBST(WRAPPER_DIR)

AC_CONFIG_FILES([Makefile \
    src/Makefile \
    src/util/Makefile \
    src/schema/Makefile \
    src/xml/Makefile \
    src/types/Makefile \
    src/wsdl/Makefile \
    src/wsdl20/Makefile \
    src/wsdl20/xml/Makefile \
    src/wsdl20/extensions/Makefile \
    src/wsdl20/extensions/soap/Makefile \
    src/wsdl10/Makefile \
    src/wsdl10/xml/Makefile \
    src/wsdl10/extensions/Makefile \
    src/wsdl10/extensions/soap/Makefile \
    src/builder/Makefile \
    src/builder/wsdl10/Makefile 
    ])
    
AC_OUTPUT
