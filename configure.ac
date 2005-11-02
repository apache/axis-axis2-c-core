# run autogen.sh to generate the configure script.

AC_PREREQ(2.59)
AC_INIT(axis2, 1.0.0)
AC_CONFIG_SRCDIR([modules/common/src/axis2.c])
AC_CONFIG_AUX_DIR(conftools)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE
AC_CONFIG_HEADER(axis2_config.h)
AC_PROG_LIBTOOL
sinclude(conftools/libtool.m4)
sinclude(conftools/ac_c_bigendian_cross.m4)
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB(dl, dlopen)

#using libtool
AM_PROG_LIBTOOL

CFLAGS="$CFLAGS -ansi -Wall -D_LARGEFILE64_SOURCE -g"
#CFLAGS="$CFLAGS -Wall -ansi -Wshadow -DLINUX=2 -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -g -O2 -pthread"
LDFLAGS="$LDFLAGS -L${CUTEST_HOME}/lib/"
LIBS="-lcutest -ldl"

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
#AC_CHECK_FUNCS([memmove])

#Following two lines are temporary and will be removed
want_xml=yes
want_core=yes

AC_MSG_CHECKING(whether to build tests)
AC_ARG_ENABLE(tests, [  --enable-tests    build tests. default=yes],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_test=no
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_test=yes
    
    ;;
  esac ],
  AC_MSG_RESULT(no)
  want_test=yes
)

AC_MSG_CHECKING(whether to build guththila)
AC_ARG_ENABLE(guththila, [  --enable-guththila    build guththila. default=yes],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    want_guththila=no
    ;;
  *)
    AC_MSG_RESULT(yes)
    want_guththila=yes
    
    ;;
  esac ],
  AC_MSG_RESULT(yes)
  want_guththila=yes
)

#Following two lines are temporary and will be removed
AM_CONDITIONAL([COND_XML], [test "$want_xml" = yes])
AM_CONDITIONAL([COND_CORE], [test "$want_core" = yes])
AM_CONDITIONAL([COND_TEST], [test "$want_test" = yes])
AM_CONDITIONAL([COND_GUTHTHILA], [test "$want_guththila" = yes])

AC_CONFIG_FILES([Makefile \
    modules/Makefile \
    modules/core/Makefile \
    modules/core/context/Makefile \
    modules/core/context/src/Makefile \
    modules/core/description/Makefile \
    modules/core/description/src/Makefile \
    modules/core/description/test/Makefile \
    modules/core/engine/Makefile \
    modules/core/engine/src/Makefile \
    modules/core/handlers/Makefile \
    modules/core/handlers/src/Makefile \
    modules/xml/Makefile \
    modules/xml/guththila/Makefile \
    modules/xml/guththila/src/Makefile \
    modules/xml/soap/Makefile \
    modules/xml/soap/src/Makefile \
    modules/xml/om/Makefile \
    modules/xml/om/src/Makefile \
    modules/xml/om/test/Makefile \
    modules/common/Makefile \
    modules/common/src/Makefile \
    modules/common/test/Makefile \
    modules/util/Makefile \
    modules/util/src/Makefile \
    modules/util/test/Makefile \
    modules/wsdl/Makefile \
    modules/wsdl/src/Makefile \
    modules/wsdl/test/Makefile \
    modules/test/Makefile \
    modules/test/om/Makefile \
    modules/test/om/src/Makefile \
    modules/test/common/Makefile \
    modules/test/util/Makefile \
    modules/test/unit/Makefile])
    
AC_OUTPUT
