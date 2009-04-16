dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(md5-src, 1.6.0)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_PREFIX_DEFAULT(/usr/local/md5)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET


CFLAGS="$CFLAGS -D_LARGEFILE64_SOURCE -g"
if test "$GCC" = "yes"; then
    CFLAGS="$CFLAGS -ansi -Wall -Wno-implicit-function-declaration"
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
AC_CHECK_FUNCS([memmove])

AC_CHECK_LIB(z, inflate)
dnl AC_CHECK_LIB(cutest, CuTestInit)


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


UTILINC=$axis2_utilinc

AC_SUBST(UTILINC)
AC_SUBST(TESTDIR)
AC_SUBST(WRAPPER_DIR)

CFLAGS="$CFLAGS"

AC_CONFIG_FILES([Makefile \
    src/Makefile \
    ])
    
AC_OUTPUT
