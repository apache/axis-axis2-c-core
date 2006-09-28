dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(wodenc-samples-src, 0.94)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([tar-ustar])
AC_PREFIX_DEFAULT(/usr/local/wodenc/samples)

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
    CFLAGS="$CFLAGS -ansi -Wall -Wno-implicit-function-declaration"
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


AXIOMINC=$axiominc
UTILINC=$axis2_utilinc
XMLSCHEMAINC=$axis2_xml_schemainc

AC_SUBST(AXIOMINC)
AC_SUBST(UTILINC)
AC_SUBST(XMLSCHEMAINC)
AC_SUBST(TESTDIR)
AC_SUBST(PARSER_DIR)

AC_CONFIG_FILES([Makefile \
        callback/Makefile \
        client/Makefile
        client/enc_echo/Makefile \
        client/enc_echo/data/Makefile \
        client/enc_echo/keys/Makefile \
        client/echo/Makefile \
        client/echo/data/Makefile

    ])
    
AC_OUTPUT
