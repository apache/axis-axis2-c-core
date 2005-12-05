dnl $Id$
dnl config.m4 for extension axis2

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(axis2, for axis2 support,
dnl Make sure that the comment is aligned:
dnl [  --with-axis2             Include axis2 support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(axis2, whether to enable axis2 support,
dnl Make sure that the comment is aligned:
dnl [  --enable-axis2           Enable axis2 support])

if test "$PHP_AXIS2" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-axis2 -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/axis2.h"  # you most likely want to change this
  dnl if test -r $PHP_AXIS2/$SEARCH_FOR; then # path given as parameter
  dnl   AXIS2_DIR=$PHP_AXIS2
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for axis2 files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       AXIS2_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$AXIS2_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the axis2 distribution])
  dnl fi

  dnl # --with-axis2 -> add include path
  dnl PHP_ADD_INCLUDE($AXIS2_DIR/include)

  dnl # --with-axis2 -> check for lib and symbol presence
  dnl LIBNAME=axis2 # you may want to change this
  dnl LIBSYMBOL=axis2 # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $AXIS2_DIR/lib, AXIS2_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_AXIS2LIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong axis2 lib version or lib not found])
  dnl ],[
  dnl   -L$AXIS2_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(AXIS2_SHARED_LIBADD)

  PHP_NEW_EXTENSION(axis2, axis2.c, $ext_shared)
fi
