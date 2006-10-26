dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(codegen-src, 0.92)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_PREFIX_DEFAULT(/usr/local/codegen)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AM_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET


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

dnl Checks for libraries
AC_CHECK_LIB(dl, dlopen)
AC_CHECK_LIB(z, inflate)
dnl AC_CHECK_LIB(cutest, CuTestInit)
AC_CHECK_LIB(xslt, xsltApplyStylesheet)

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove])

dnl Checks for library informations
PKG_CHECK_MODULES(LIBXML2, libxml-2.0)
PKG_CHECK_MODULES(LIBXSLT, libxslt)

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

AC_MSG_CHECKING(whether to build tests)
AC_ARG_ENABLE(tests,
[  --enable-tests    build tests. default=no],
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


AXIOMINC=$axiominc
UTILINC=$axis2_utilinc
XMLSCHEMAINC=$axis2_xml_schemainc
WODENINC=$axis2_wodeninc

AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_LIBS)
AC_SUBST(LIBXML2_DIR)

AC_SUBST(LIBXSLT_CFLAGS)
AC_SUBST(LIBXSLT_LIBS)
AC_SUBST(LIBXSLT_DIR)

AC_SUBST(AXIOMINC)
AC_SUBST(UTILINC)
AC_SUBST(XMLSCHEMAINC)
AC_SUBST(WODENINC)
AC_SUBST(TESTDIR)
AC_SUBST(WRAPPER_DIR)

CFLAGS="$CFLAGS $LIBXML2_CFLAGS $LIBXSLT_CFLAGS"

AC_CONFIG_FILES([Makefile \
    src/Makefile \
    src/util/Makefile \
    src/wsdl/Makefile \
    src/wsdl/writer/Makefile \
    src/wsdl/emitter/Makefile \
    src/wsdl/extensions/Makefile \
    src/templates/Makefile \
    src/templates/c/Makefile \
    src/typemappers/Makefile \
    src/schema/Makefile \
    src/schema/compiler/Makefile \
    src/schema/writer/Makefile \
    test/Makefile \
    test/unit/Makefile \
    test/unit/util/Makefile \
    test/unit/codegen/Makefile
    ])
    
AC_OUTPUT
