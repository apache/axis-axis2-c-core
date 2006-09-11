dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(xml_schema-src, 0.93)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([tar-ustar])
AC_PREFIX_DEFAULT(/usr/local/xml_schema)

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

CFLAGS="$CFLAGS -D_LARGEFILE64_SOURCE"
if test "$GCC" = "yes"; then
    CFLAGS="$CFLAGS -ansi -Wall -Werror -Wno-implicit-function-declaration"
fi
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

AC_MSG_CHECKING(whether to build tests)
AC_ARG_ENABLE(tests, [  --enable-tests    build tests. default=yes],
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

AC_CHECK_LIB(cutest, CuTestInit, [], [TESTDIR=""])

UTILINC=$axis2_utilinc
AXIOMINC=$axiominc

AC_SUBST(UTILINC)
AC_SUBST(AXIOMINC)
AC_SUBST(TESTDIR)

AC_CONFIG_FILES([Makefile
    src/Makefile \
    test/Makefile \
    samples/Makefile
    ])
    
AC_OUTPUT
