dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(axis2, 1.0.0)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

AC_CONFIG_SRCDIR([modules/common/src/axis2.c])

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

#CFLAGS="$CFLAGS -ansi -Wall -D_LARGEFILE64_SOURCE -g3 -O2"
CFLAGS="$CFLAGS -ansi -Wall -D_LARGEFILE64_SOURCE -g"
#CFLAGS="$CFLAGS -Wall -ansi -Wshadow -DLINUX=2 -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -g -O2 -pthread"

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
#AC_CHECK_FUNCS([memmove])

AC_MSG_CHECKING(whether to build guththila xml parser library)
AC_ARG_ENABLE(guththila, [  --enable-guththila    build guththila xml parser library wrapper. default=no],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    GUTHTHILA=""
    GUTHTHILA_LIBNAME=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    GUTHTHILA="guththila"
    GUTHTHILA_LIBNAME="-lguththila"

    ;;
  esac ],
  AC_MSG_RESULT(no)
  GUTHTHILA=""
  GUTHTHILA_LIBNAME=""
)

AC_MSG_CHECKING(whether to build libxml2 xml parser library)
AC_ARG_ENABLE(libxml2, [  --enable-libxml2    build libxml2 xml parser library wrapper. default=yes],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    LIBXML2=""
    LIBXML2_LIBNAME=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    LIBXML2="libxml2"
    LIBXML2_LIBNAME="-lxml2"

    ;;
  esac ],
  AC_MSG_RESULT(yes)
  LIBXML2="libxml2"
  LIBXML2_LIBNAME="-lxml2"
)

AC_SUBST(LIBXML2)
AC_SUBST(GUTHTHILA)
AC_SUBST(LIBXML2_LIBNAME)
AC_SUBST(GUTHTHILA_LIBNAME)

AC_CONFIG_FILES([Makefile \
    modules/Makefile \
    modules/core/Makefile \
    modules/core/context/Makefile \
    modules/core/description/Makefile \
    modules/core/engine/Makefile \
    modules/core/handlers/Makefile \
    modules/core/addr/Makefile \
    modules/core/phaseresolver/Makefile \
    modules/core/transport/Makefile \
    modules/core/transport/http/Makefile \
    modules/core/deployment/Makefile \
    modules/core/clientapi/Makefile \
    modules/core/receivers/Makefile \
    modules/xml/Makefile \
    modules/xml/parser/Makefile \
    modules/xml/parser/guththila/Makefile \
    modules/xml/parser/guththila/impl/Makefile \
    modules/xml/parser/guththila/impl/src/Makefile \
    modules/xml/parser/guththila/src/Makefile \
    modules/xml/parser/libxml2/Makefile \
    modules/xml/parser/libxml2/src/Makefile \
    modules/xml/soap/Makefile \
    modules/xml/om/Makefile \
    modules/util/Makefile \
    modules/wsdl/Makefile \
    test/Makefile \
    test/xml/Makefile \
    test/xml/om/Makefile \
    test/core/Makefile \
    test/core/handlers/Makefile \
    test/util/Makefile \
    test/unit/Makefile \
    test/unit/core/Makefile \
    test/unit/core/description/Makefile \
    test/unit/core/engine/Makefile \
    test/unit/core/context/Makefile \
    test/unit/core/phaseresolver/Makefile \
    test/unit/core/deployment/Makefile \
    test/unit/util/Makefile \
    test/unit/xml/Makefile \
    test/unit/xml/om/Makefile
    ])
    
AC_OUTPUT
