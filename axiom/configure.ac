dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(axis2_axiom-src, 0.92)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
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
AC_CHECK_LIB(cutest, CuTestInit)
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
AC_CHECK_HEADERS([linux/if.h],[],[],
[
#include <sys/socket.h>
])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
#AC_CHECK_FUNCS([memmove])

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
    dnl else find the axis2_util include dir in $(AXIS2C_HOME)/include/util/include
    elif test -d '$(AXIS2C_HOME)/include/util/include'; then
        axis2_utilinc="-I$(AXIS2C_HOME)/include/util/include"
    else
        AC_MSG_ERROR(could not find axis2_util. stop)
    fi
    ;;
  esac ],
  AC_MSG_RESULT(no)
)


UTILINC=$axis2_utilinc

AC_SUBST(UTILINC)

AC_CONFIG_FILES([Makefile \
    parser/Makefile \
    parser/libxml2/Makefile \
    soap/Makefile \
    om/Makefile \
    attachments/Makefile
    ])
    
AC_OUTPUT
