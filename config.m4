dnl $Id$
dnl config.m4 for extension pdcp

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(pdcp, for pdcp support,
dnl Make sure that the comment is aligned:
dnl [  --with-pdcp             Include pdcp support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(pdcp, whether to enable pdcp support,
dnl Make sure that the comment is aligned:
dnl [  --enable-pdcp           Enable pdcp support])

if test "$PHP_PDCP" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-pdcp -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/pdcp.h"  # you most likely want to change this
  dnl if test -r $PHP_PDCP/$SEARCH_FOR; then # path given as parameter
  dnl   PDCP_DIR=$PHP_PDCP
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for pdcp files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PDCP_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PDCP_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the pdcp distribution])
  dnl fi

  dnl # --with-pdcp -> add include path
  dnl PHP_ADD_INCLUDE($PDCP_DIR/include)

  dnl # --with-pdcp -> check for lib and symbol presence
  dnl LIBNAME=pdcp # you may want to change this
  dnl LIBSYMBOL=pdcp # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PDCP_DIR/lib, PDCP_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_PDCPLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong pdcp lib version or lib not found])
  dnl ],[
  dnl   -L$PDCP_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(PDCP_SHARED_LIBADD)

  PHP_NEW_EXTENSION(pdcp, pdcp.c, $ext_shared)
fi
 