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
LDFLAGS="-lpthread"

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for libraries
AC_CHECK_LIB(dl, dlopen)
AC_CHECK_LIB(z, inflate)
AC_CHECK_LIB(cutest, CuTestInit)
AC_CHECK_LIB(xslt, xsltApplyStylesheet)

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove])

dnl Checks for library informations
PKG_CHECK_MODULES(LIBXML2, libxml-2.0)
PKG_CHECK_MODULES(LIBXSLT, libxslt)


AC_MSG_CHECKING(To Use Axis2/C Util. This is a compulsory module to build Axis2/C)
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

AC_MSG_CHECKING(To Use AXIOM. This is a compulsory module to build Axis2/C)
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

AC_MSG_CHECKING(To Use Xml Schema. This is a compulsory module to build Woden C and Codegen)
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

AC_MSG_CHECKING(To Use Woden C. This is a compulsory module to build C Codegen)
AC_ARG_WITH(axis2_woden,
[  --with-axis2_woden[=PATH]      use axis2_woden.],
[ case "$withval" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    dnl Find axis2_woden include dir in the path
    if test -d $withval; then
        axis2_woden="-I$withval"
    dnl else find the axis2_woden include dir in $(AXIS2C_HOME)/include
    elif test -d '$(AXIS2C_HOME)/include'; then
        axis2_woden="-I$(AXIS2C_HOME)/include"
    else
        AC_MSG_ERROR(could not find axis2_woden. stop)
    fi
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
    test/unit/codegen/Makefile \
    samples/client/calculator/Makefile \
    samples/server/calculator/Makefile \
    samples/client/interop_test_svc/Makefile \
    samples/server/interop_test_svc/Makefile \
    ])
    
AC_OUTPUT
