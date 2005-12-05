/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2004 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.15 2004/01/08 16:46:52 sniper Exp $ */

#ifndef AXIS2_COMMON_H
#define AXIS2_COMMON_H
#include <axis2_om_node.h>
#include <axis2_om_document.h>
#include <axis2_om_stax_builder.h>

typedef struct _om_node {
    void *ptr;
    axis2_om_document_t *doc;
    axis2_om_stax_builder_t *builder;
    int node_type;
    int ref_count;
}om_node_t;

typedef struct _om_object{
    void *ptr;
    int obj_type;
    int ref_count;
}om_object;

typedef om_node_t* om_node_ptr;
typedef om_object* om_object_ptr;


#define REGISTER_AXIS2_CLASS(ce, name, parent_ce, funcs, entry) \
INIT_CLASS_ENTRY(ce, name, funcs); \
ce.create_object = axis2_objects_new; \
entry = zend_register_internal_class_ex(&ce, parent_ce, NULL TSRMLS_CC);

#define AXIS2_GET_THIS(zval) \
        if(NULL == (zval = getThis())) \
        { \
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "object missing ");  \
            RETURN_FALSE; \
        } 
        
#define AXIS2_GET_OBJ(__ptr, __id, __ptr_type, __intern) \
        { \
            __intern = (axis2_object *)zend_object_store_get_object(__id TSRMLS_CC); \
            if((__intern->ptr)) \
            { \
                __ptr = (__ptr_type)(__intern->ptr); \
            } \
        }

#endif /* AXIS2_COMMON_H */