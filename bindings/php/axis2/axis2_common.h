/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


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

