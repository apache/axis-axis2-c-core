dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(axis2_axiom-src, 0.95)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([tar-ustar])
AC_PREFIX_DEFAULT(/usr/local/axis2_axiom)

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
AC_CHECK_LIB(z, inflate)

#CFLAGS="$CFLAGS -ansi -Wall -D_LARGEFILE64_SOURCE -Wno-implicit-function-declaration"
CFLAGS="$CFLAGS -D_LARGEFILE64_SOURCE"
if test "$GCC" = "yes"; then
    CFLAGS="$CFLAGS -ansi -Wall -Wno-implicit-function-declaration -Werror"
fi

LDFLAGS="$LDFLAGS -lpthread"

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([linux/if.h],[],[],
[
#if HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
])

dnl This is a check to see if we are running MacOS X
dnl It may be better to do a Darwin check
AC_CHECK_HEADER([sys/appleapiopts.h],
               [AC_DEFINE([IS_MACOSX],[1],[Define to 1 if compiling on MacOS X])],
               [])

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

AC_MSG_CHECKING(whether to build tests)
AC_ARG_ENABLE(tests, [  --enable-tests    build tests. default=no],
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


PKG_CHECK_MODULES(LIBXML2, libxml-2.0)
CFLAGS="$CFLAGS $LIBXML2_CFLAGS $GUTHTHILA_CFLAGS"

AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_LIBS)
AC_SUBST(WRAPPER_DIR)
AC_SUBST(GUTHTHILA_LIBS)
AC_SUBST(TESTDIR)

AC_CONFIG_FILES([Makefile \
    src/Makefile \
    src/parser/Makefile \
    src/parser/libxml2/Makefile \
    src/parser/guththila/Makefile \
    src/soap/Makefile \
    src/om/Makefile \
    src/util/Makefile \
    src/attachments/Makefile \
    test/Makefile \
    test/om/Makefile \
    test/soap/Makefile \
    include/Makefile
    ])
    
AC_OUTPUT
