dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(axis2_util-src, 0.95)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([tar-ustar])
AC_PREFIX_DEFAULT(/usr/local/axis2_util)

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
    CFLAGS="$CFLAGS -ansi -Wall -Wno-implicit-function-declaration"
fi
LDFLAGS="-lpthread"

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
AC_CHECK_HEADERS([net/if_types.h net/if_dl.h])

dnl This is a check to see if we are running MacOS X
dnl It may be better to do a Darwin check
AC_CHECK_HEADER([sys/appleapiopts.h],
               [AC_DEFINE([IS_MACOSX],[1],[Define to 1 if compiling on MacOS X])],
               [])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_CHECK_TYPES([struct lifreq])
AC_CHECK_TYPES([struct sockaddr_dl], [], [],
[
#if HAVE_NET_IF_DL_H
# include <net/if_dl.h>
#endif
])

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
#AC_CHECK_FUNCS([memmove])

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

CFLAGS="$CFLAGS $GUTHTHILA_CFLAGS"

UTILINC=$axis2_utilinc

AC_SUBST(UTILINC)
AC_SUBST(GUTHTHILA_DIR)
AC_SUBST(GUTHTHILA_LIBS)
AC_SUBST(TESTDIR)

AC_CONFIG_FILES([Makefile \
    src/Makefile \
    src/platforms/unix/Makefile \
    src/minizip/Makefile \
    include/Makefile \
    test/Makefile \
    test/util/Makefile \
    test/unit/Makefile \
    test/unit/util/Makefile
    ])
    
AC_OUTPUT
