dnl Licensed to the Apache Software Foundation (ASF) under one or more
dnl contributor license agreements.  See the NOTICE file distributed with
dnl this work for additional information regarding copyright ownership.
dnl The ASF licenses this file to You under the Apache License, Version 2.0
dnl (the "License"); you may not use this file except in compliance with
dnl the License.  You may obtain a copy of the License at
dnl 
dnl   http://www.apache.org/licenses/LICENSE-2.0
dnl 
dnl Unless required by applicable law or agreed to in writing, software
dnl distributed under the License is distributed on an "AS IS" BASIS,
dnl WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
dnl See the License for the specific language governing permissions and
dnl limitations under the License.
dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(guththilac-src, 2.0.0)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([tar-ustar subdir-objects foreign])
AC_PREFIX_DEFAULT(/usr/local/guththila)

m4_ifdef([_A][M_PROG_TAR],[_A][M_SET_OPTION([tar-ustar])])

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl check for flavours of varargs macros (test from GLib)
AC_MSG_CHECKING(for ISO C99 varargs macros in C)
AC_TRY_COMPILE([],[
int a(int p1, int p2, int p3);
#define call_a(...) a(1,__VA_ARGS__)
call_a(2,3);
],axis2c_have_iso_c_varargs=yes,axis2c_have_iso_c_varargs=no)
AC_MSG_RESULT($axis2c_have_iso_c_varargs)

AC_MSG_CHECKING(for GNUC varargs macros)
AC_TRY_COMPILE([],[
int a(int p1, int p2, int p3);
#define call_a(params...) a(1,params)
call_a(2,3);
],axis2c_have_gnuc_varargs=yes,axis2c_have_gnuc_varargs=no)
AC_MSG_RESULT($axis2c_have_gnuc_varargs)

dnl Output varargs tests
if test x$axis2c_have_iso_c_varargs = xyes; then
    AC_DEFINE(HAVE_ISO_VARARGS,1,[Have ISO C99 varargs macros])
fi
if test x$axis2c_have_gnuc_varargs = xyes; then
    AC_DEFINE(HAVE_GNUC_VARARGS,1,[Have GNU-style varargs macros])
fi

dnl Checks for libraries.
AC_CHECK_LIB(dl, dlopen)

CFLAGS="$CFLAGS -D_LARGEFILE64_SOURCE -DAXIS2_GUTHTHILA_ENABLED"

dnl Detect Android target for conditional compilation
AC_MSG_CHECKING([for Android target])
case "$host" in
    *-linux-android*|*-android*)
        android_build=yes
        ;;
    *)
        android_build=no
        ;;
esac
AC_MSG_RESULT([$android_build])

if test "$GCC" = "yes"; then
    dnl Use gnu99 for json-c compatibility
    CFLAGS="$CFLAGS -std=gnu99 -ggdb3 -Wall -Wno-implicit-function-declaration "
fi

AC_MSG_CHECKING(whether to set -Werror)
AC_ARG_ENABLE(werror, [  --enable-werror  treat warnings as errors during build],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    ;;
  *)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -Werror"
    ;;
  esac ],
  AC_MSG_RESULT(no)
)


AC_MSG_CHECKING(whether to enable code coverage statistics)
AC_ARG_ENABLE(coverage, [  --enable-coverage       enable code coverage statistics (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    CFLAGS="$CFLAGS"
    CPPFLAGS="$CPPFLAGS"
    ;;
  *)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -fprofile-arcs -ftest-coverage"
    CPPFLAGS="$CPPFLAGS -fprofile-arcs -ftest-coverage"
    CXXFLAGS="$CXXFLAGS -fprofile-arcs -ftest-coverage"
    LDFLAGS="$LDFLAGS -lgcov --coverage"
    ;;
  esac ],[
  AC_MSG_RESULT(no)
  CFLAGS="$CFLAGS"
  CPPFLAGS="$CPPFLAGS"]
)

AC_MSG_CHECKING(whether to enable address sanitizer)
AC_ARG_ENABLE(asan, [  --enable-asan
                          enable address sanitizer (default=no)],
[ case "${enableval}" in
  no)
    AC_MSG_RESULT(no)
    enable_asan=no
    CFLAGS="$CFLAGS"
    CPPFLAGS="$CPPFLAGS"
    ;;
  *)
    AC_MSG_RESULT(yes)
    enable_asan=yes
    CFLAGS="$CFLAGS -fsanitize=address -fno-omit-frame-pointer"
    CPPFLAGS="$CPPFLAGS -fsanitize=address -fno-omit-frame-pointer"
    CXXFLAGS="$CXXFLAGS -fsanitize=address -fno-omit-frame-pointer"
    LDFLAGS="$LDFLAGS -fsanitize=address"
    ;;
  esac ],[
  AC_MSG_RESULT(no)
  enable_asan=no
  CFLAGS="$CFLAGS"
  CPPFLAGS="$CPPFLAGS"]
)

dnl Create automake conditional for ASAN (used by test Makefile.am for LD_PRELOAD)
AM_CONDITIONAL([ENABLE_ASAN], [test "x$enable_asan" = "xyes"])

dnl Detect ASAN library path for test preloading (libtool-wrapped tests need this)
ASAN_PRELOAD=""
if test "x$enable_asan" = "xyes"; then
    AC_MSG_CHECKING([for ASAN library path])
    for asan_path in /lib/x86_64-linux-gnu/libasan.so.8 \
                      /lib/x86_64-linux-gnu/libasan.so.6 \
                      /lib64/libasan.so.8 \
                      /lib64/libasan.so.6 \
                      /usr/lib/libasan.so; do
        if test -f "$asan_path"; then
            ASAN_PRELOAD="$asan_path"
            break
        fi
    done
    if test -n "$ASAN_PRELOAD"; then
        AC_MSG_RESULT([$ASAN_PRELOAD])
    else
        AC_MSG_RESULT([not found - tests may fail with libtool wrappers])
    fi
fi
AC_SUBST([ASAN_PRELOAD])


AC_MSG_CHECKING(whether to use the Google test framework)
AC_ARG_WITH(gtest,
            [  --with-gtest[=PATH]      Find the gtest source files in 'PATH'.],
[ case "$withval" in
  no)
    AC_MSG_RESULT(no)
    USE_GTEST=""
    GTEST_DIR=""
    GTEST=""
    ;;
  *)
    AC_MSG_RESULT(yes)
    GTEST_DIR="$withval"
    GTEST="gtest"
    CXXFLAGS="$CXXFLAGS -g -Wall -Wextra -pthread"
    ;;
  esac ],
  AC_MSG_RESULT(no)
)

AC_MSG_CHECKING(whether to build tests)
AC_ARG_ENABLE(tests, [  --enable-tests    build tests. default=no],
[ case "${enableval}" in
  yes)
    AC_MSG_RESULT(yes)
    TESTDIR="tests"
    ;;
  *)
    AC_MSG_RESULT(no)
    TESTDIR=""

    ;;
  esac ],
  AC_MSG_RESULT(no)
  TESTDIR=""
)




dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
#AC_CHECK_FUNCS([memmove])


UTILINC=$axis2_utilinc
VERSION_NO="7:0:7"

AC_SUBST(UTILINC)
AC_SUBST(VERSION_NO)
AC_SUBST(GTEST_DIR)
AC_SUBST(GTEST)
AC_SUBST(TESTDIR)

dnl Set GTEST_AVAILABLE conditional based on whether gtest is configured
dnl For guththila subproject, just check if GTEST is set (--with-gtest was provided)
AM_CONDITIONAL(GTEST_AVAILABLE, test -n "$GTEST" && test -n "$GTEST_DIR")

AC_CONFIG_FILES([Makefile \
    src/Makefile \
    gtest/Makefile \
    tests/Makefile \
    ])

AC_OUTPUT
