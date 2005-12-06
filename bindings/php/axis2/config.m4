dnl $Id$
dnl config.m4 for extension axis2

PHP_ARG_WITH(axis2, for axis2 support,
 [  --with-axis2             Include axis2 support])

if test "$PHP_AXIS2" != "no"; then
  if test -z "$AXIS2C_HOME"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR(Cannot find Axis2C home. Plese set AXIS2C_HOME env variable.)
  fi

  dnl # --with-axis2 -> add include path
  PHP_ADD_INCLUDE($AXIS2C_HOME/include)
  PHP_ADD_INCLUDE($AXIS2C_HOME/modules/util/src)
  PHP_ADD_INCLUDE($AXIS2C_HOME/modules/xml/parser)


  dnl # --with-axis2 -> check for lib and symbol presence

  PHP_ADD_LIBRARY_WITH_PATH(axis2_libxml2, $AXIS2C_HOME/install/lib, AXIS2_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(axis2_om, $AXIS2C_HOME/install/lib, AXIS2_SHARED_LIBADD)

  PHP_SUBST(AXIS2_SHARED_LIBADD)

  PHP_NEW_EXTENSION(axis2, [axis2.c om_doctype.c om_namespace.c om_processing_instruction.c qname.c \
                            om_attribute.c om_document.c om_node.c om_stax_builder.c xml_reader.c \
                            om_comment.c om_element.c om_output.c om_text.c xml_writer.c], $ext_shared)
fi
