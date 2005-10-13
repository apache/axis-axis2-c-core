# run autogen.sh to generate the configure script.

AC_PREREQ(2.59)
AC_INIT(modules/common/src/axis2.c)
AC_CONFIG_SRCDIR([include/axis2.h])
AC_CONFIG_AUX_DIR(conftools)
AC_CONFIG_HEADER(axis2_config.h)
AM_INIT_AUTOMAKE(axis2,p2)

# Checks for programs.
#AC_PROG_C
#using libtool
AM_PROG_LIBTOOL

# Checks for libraries.

CFLAGS="$CFLAGS -ansi -Wall -D_LARGEFILE64_SOURCE -g"
#CFLAGS="$CFLAGS -Wall -ansi -Wshadow -DLINUX=2 -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -g -O2 -pthread"
LDFLAGS="$LDFLAGS -L${CUTEST_HOME}/lib/ -lcutest -L${APR_HOME}/lib -lapr-1"
# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
#AC_CHECK_FUNCS([memmove])

AC_OUTPUT([Makefile \
    modules/Makefile \
    modules/core/Makefile \
    modules/core/context/Makefile \
    modules/core/context/src/Makefile \
    modules/core/description/Makefile \
    modules/core/description/src/Makefile \
    modules/core/engine/Makefile \
    modules/core/engine/src/Makefile \
    modules/xml/Makefile \
    modules/xml/guththila/Makefile \
    modules/xml/guththila/src/Makefile \
    modules/xml/soap/Makefile \
    modules/xml/soap/src/Makefile \
    modules/xml/om/Makefile \
    modules/xml/om/src/Makefile \
    modules/common/Makefile \
    modules/common/src/Makefile \
    modules/common/test/Makefile \
    modules/util/Makefile \
    modules/util/src/Makefile \
    modules/util/test/Makefile \
    modules/test/Makefile \
    modules/test/om/Makefile \
    modules/test/om/src/Makefile \
    modules/test/common/Makefile \
    modules/test/util/Makefile \
    modules/test/unit/Makefile])
