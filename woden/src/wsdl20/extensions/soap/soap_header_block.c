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

#include <woden/wsdl20/extensions/soap/axis2_woden_soap_header_block.h>
#include <woden/wsdl20/xml/axis2_woden_wsdl_element.h>
#include <woden/xml/axis2_woden_string_attr.h>
#include <woden/xml/axis2_woden_uri_attr.h>
#include <woden/wsdl20/axis2_woden_types.h>
#include "axis2_woden_soap_constants.h"

typedef struct axis2_woden_soap_header_block_impl axis2_woden_soap_header_block_impl_t;

/** 
 * @brief Soap Header Blocks Struct Impl
 *	Axis2 Soap Header Blocks  
 */ 
struct axis2_woden_soap_header_block_impl
{
    axis2_woden_soap_header_block_t header_block;
    axis2_hash_t *methods;
    axis2_hash_t *super;
    axis2_woden_obj_types_t obj_type;
    
    void *f_parent;
    axis2_array_list_t *f_documentation_elements;
    axis2_qname_t *f_ext_element_type;
    axis2_bool_t f_required;
    void *f_attr_ext;
    void *f_elem_ext;
    axis2_qname_t *f_element_decl_qn;
    void *f_element_decl;
    axis2_bool_t f_must_understand;
    void *f_types;
};

#define INTF_TO_IMPL(header_block) ((axis2_woden_soap_header_block_impl_t *) header_block)

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_free(
        void *header_block,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_soap_header_block_super_objs(
        void *header_block,
        const axis2_env_t *env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_soap_header_block_type(
        void *header_block,
        const axis2_env_t *env);

/* ***********************************************************************
 *  Component model methods (SOAPHeaderBlock interface), some shared with Element model
 * ***********************************************************************/

void *AXIS2_CALL
axis2_woden_soap_header_block_get_element_decl(
        void *header_block,
        axis2_env_t *env);

axis2_bool_t AXIS2_CALL 
axis2_woden_soap_header_block_must_understand(
        void *header_block,
        axis2_env_t *env);

axis2_bool_t AXIS2_CALL 
axis2_woden_soap_header_block_is_required(
        void *header_block,
        axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_soap_header_block_get_parent(
        void *header_block,
        axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_soap_header_block_to_element(
        void *header_block,
        axis2_env_t *env);

/* ***********************************************************************
 *  Element model-only methods (SOAPHeaderBlockElement interface)
 * ***********************************************************************/

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_element_qname(
        void *header_block,
        axis2_env_t *env,
        axis2_qname_t *qname); 

axis2_qname_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_element_qname(
        void *header_block,
        axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_soap_header_block_get_element(
        void *header_block,
        axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_must_understand(
        void *header_block,
        axis2_env_t *env,
        axis2_bool_t understood);

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_parent_element(
        void *header_block,
        axis2_env_t *env,
        void *wsdl_el);

void *AXIS2_CALL
axis2_woden_soap_header_block_get_parent_element(
        void *header_block,
        axis2_env_t *env); 

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_add_documentation_element(
        void *header_block,
        axis2_env_t *env,
        void *doc_el);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_documentation_elements(
        void *header_block,
        axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_extension_type(
        void *header_block,
        axis2_env_t *env,
        axis2_qname_t *qname); 

axis2_qname_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_extension_type(
        void *header_block,
        axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_required(
        void *header_block,
        axis2_env_t *env,
        axis2_bool_t required);

/* ***********************************************************************
 *  Extensibility methods
 * ***********************************************************************/

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_ext_attr(
        void *header_block,
        axis2_env_t *env,
        axis2_qname_t *attr_type, 
        void *attr);

void *AXIS2_CALL
axis2_woden_soap_header_block_get_ext_attr(
        void *header_block,
        axis2_env_t *env,
        axis2_qname_t *attr_type);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_ext_attrs_for_namespace(
        void *header_block,
        axis2_env_t *env,
        axis2_url_t *namespc);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_ext_attrs(
        void *header_block,
        axis2_env_t *env);

axis2_bool_t AXIS2_CALL 
axis2_woden_soap_header_block_has_ext_attrs_for_namespace(
        void *header_block,
        axis2_env_t *env,
        axis2_url_t *namespc);

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_add_ext_element(
        void *header_block,
        axis2_env_t *env,
        void *ext_el);

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_remove_ext_element(
        void *header_block,
        axis2_env_t *env,
        void *ext_el);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_ext_elements(
        void *header_block,
        axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_ext_elements_of_type(
        void *header_block,
        axis2_env_t *env,
        axis2_qname_t *ext_type); 

axis2_bool_t AXIS2_CALL 
axis2_woden_soap_header_block_has_ext_elements_for_namespace(
        void *header_block,
        axis2_env_t *env,
        axis2_url_t *namespc); 

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_element_decl(
        void *header_block,
        axis2_env_t *env,
        void *element_decl);

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_types(
        void *header_block,
        axis2_env_t *env,
        void *types);

static axis2_woden_soap_header_block_t *
create(const axis2_env_t *env);

static axis2_status_t
axis2_woden_soap_header_block_free_ops(
        void *header_block,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_woden_soap_header_block_t * AXIS2_CALL
axis2_woden_soap_header_block_to_soap_header_block_element(
        void *header_block,
        const axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!header_block)
    {
        header_block_impl = (axis2_woden_soap_header_block_impl_t *) create(env);
    }
    else
        header_block_impl = (axis2_woden_soap_header_block_impl_t *) header_block;
    axis2_woden_soap_header_block_free_ops(header_block, env);

    header_block_impl->header_block.header_block_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_soap_header_block_element_ops_t));
    axis2_woden_soap_header_block_element_resolve_methods(&(header_block_impl->header_block.
            header_block_element), env, header_block_impl->methods);
    return header_block;
}

AXIS2_EXTERN axis2_woden_soap_header_block_t * AXIS2_CALL
axis2_woden_soap_header_block_to_ext_element(
        void *header_block,
        const axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!header_block)
    {
        header_block_impl = (axis2_woden_soap_header_block_impl_t *) create(env);
    }
    else
        header_block_impl = (axis2_woden_soap_header_block_impl_t *) header_block;
    axis2_woden_soap_header_block_free_ops(header_block, env);

    header_block_impl->header_block.header_block_element.base.ext_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_ext_element_ops_t));
    axis2_woden_ext_element_resolve_methods(&(header_block_impl->header_block.
            header_block_element.base.ext_element), env, header_block_impl->methods);
    return header_block;
}

AXIS2_EXTERN axis2_woden_soap_header_block_t * AXIS2_CALL
axis2_woden_soap_header_block_to_attr_extensible(
        void *header_block,
        const axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!header_block)
    {
        header_block_impl = (axis2_woden_soap_header_block_impl_t *) create(env);
    }
    else
        header_block_impl = (axis2_woden_soap_header_block_impl_t *) header_block;
    axis2_woden_soap_header_block_free_ops(header_block, env);

    header_block_impl->header_block.header_block_element.base.attr_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_attr_extensible_ops_t));
    axis2_woden_attr_extensible_resolve_methods(&(header_block_impl->header_block.
            header_block_element.base.attr_extensible), env, NULL, header_block_impl->methods);
    return header_block;
}

AXIS2_EXTERN axis2_woden_soap_header_block_t * AXIS2_CALL
axis2_woden_soap_header_block_to_element_extensible(
        void *header_block,
        const axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!header_block)
    {
        header_block_impl = (axis2_woden_soap_header_block_impl_t *) create(env);
    }
    else
        header_block_impl = (axis2_woden_soap_header_block_impl_t *) header_block;
    axis2_woden_soap_header_block_free_ops(header_block, env);

    header_block_impl->header_block.header_block_element.base.element_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_element_extensible_ops_t));
    axis2_woden_element_extensible_resolve_methods(&(header_block_impl->header_block.
            header_block_element.base.element_extensible), env, NULL, header_block_impl->methods);
    return header_block;
}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_soap_header_block_t *
create(const axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    header_block_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_soap_header_block_impl_t));

    header_block_impl->obj_type= AXIS2_WODEN_SOAP_HEADER_BLOCK;
    header_block_impl->super = NULL;
    header_block_impl->methods = NULL;

    header_block_impl->f_parent = NULL;
    header_block_impl->f_documentation_elements = NULL;
    header_block_impl->f_ext_element_type = NULL;
    header_block_impl->f_required = AXIS2_FALSE;
    header_block_impl->f_attr_ext = NULL;
    header_block_impl->f_elem_ext = NULL;
    header_block_impl->f_element_decl_qn = NULL;
    header_block_impl->f_element_decl = NULL;
    header_block_impl->f_must_understand = AXIS2_FALSE;
    header_block_impl->f_types = NULL;
    
    header_block_impl->header_block.header_block_element.ops = NULL;
    header_block_impl->header_block.header_block_element.base.ext_element.ops = NULL;
    header_block_impl->header_block.header_block_element.base.attr_extensible.ops = NULL;
    header_block_impl->header_block.header_block_element.base.element_extensible.ops = NULL;
    
    header_block_impl->header_block.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_woden_soap_header_block_ops_t));

    header_block_impl->header_block.ops->free = axis2_woden_soap_header_block_free;
    header_block_impl->header_block.ops->super_objs = 
        axis2_woden_soap_header_block_super_objs;
    header_block_impl->header_block.ops->type = 
        axis2_woden_soap_header_block_type;
    
    header_block_impl->header_block.ops->get_element_decl = 
        axis2_woden_soap_header_block_get_element_decl;
    
    header_block_impl->header_block.ops->must_understand = 
        axis2_woden_soap_header_block_must_understand;
    
    header_block_impl->header_block.ops->is_required = 
        axis2_woden_soap_header_block_is_required;
    
    header_block_impl->header_block.ops->get_parent = 
        axis2_woden_soap_header_block_get_parent;
    
    header_block_impl->header_block.ops->to_element = 
        axis2_woden_soap_header_block_to_element;
    
    header_block_impl->header_block.ops->set_element_decl = 
        axis2_woden_soap_header_block_set_element_decl;
    
    header_block_impl->header_block.ops->set_types = 
        axis2_woden_soap_header_block_set_types;
    
    header_block_impl->methods = axis2_hash_make(env);
    if(!header_block_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(header_block_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_free);
    axis2_hash_set(header_block_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_woden_soap_header_block_super_objs);
    axis2_hash_set(header_block_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_soap_header_block_type);

    axis2_hash_set(header_block_impl->methods, "get_element_decl", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_element_decl);

    axis2_hash_set(header_block_impl->methods, "must_understand", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_must_understand);

    axis2_hash_set(header_block_impl->methods, "is_required", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_is_required);

    axis2_hash_set(header_block_impl->methods, "get_parent", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_parent);

    axis2_hash_set(header_block_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_to_element);

    axis2_hash_set(header_block_impl->methods, "set_element_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_set_element_qname);

    axis2_hash_set(header_block_impl->methods, "get_element_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_element_qname);

    axis2_hash_set(header_block_impl->methods, "get_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_element);

    axis2_hash_set(header_block_impl->methods, "set_must_understand", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_set_must_understand);

    axis2_hash_set(header_block_impl->methods, "set_parent_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_set_parent_element);

    axis2_hash_set(header_block_impl->methods, "get_parent_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_parent_element);

    axis2_hash_set(header_block_impl->methods, "add_documentation_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_add_documentation_element);

    axis2_hash_set(header_block_impl->methods, "get_documentation_elements", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_documentation_elements);

    axis2_hash_set(header_block_impl->methods, "set_extension_type", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_set_extension_type);

    axis2_hash_set(header_block_impl->methods, "get_extension_type", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_extension_type);

    axis2_hash_set(header_block_impl->methods, "set_required", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_set_required);

    axis2_hash_set(header_block_impl->methods, "set_ext_attr", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_set_ext_attr);

    axis2_hash_set(header_block_impl->methods, "get_ext_attr", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_ext_attr);

    axis2_hash_set(header_block_impl->methods, "get_ext_attrs_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_ext_attrs_for_namespace);

    axis2_hash_set(header_block_impl->methods, "get_ext_attrs", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_ext_attrs);

    axis2_hash_set(header_block_impl->methods, "has_ext_attrs_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_has_ext_attrs_for_namespace);

    axis2_hash_set(header_block_impl->methods, "add_ext_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_add_ext_element);

    axis2_hash_set(header_block_impl->methods, "remove_ext_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_remove_ext_element);

    axis2_hash_set(header_block_impl->methods, "get_ext_elements", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_ext_elements);

    axis2_hash_set(header_block_impl->methods, "get_ext_elements_of_type", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_get_ext_elements_of_type);

    axis2_hash_set(header_block_impl->methods, "has_ext_elements_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_has_ext_elements_for_namespace);

    axis2_hash_set(header_block_impl->methods, "set_element_decl", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_set_element_decl);

    axis2_hash_set(header_block_impl->methods, "set_types", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_header_block_set_types);

    return &(header_block_impl->header_block);
}

AXIS2_EXTERN axis2_woden_soap_header_block_t * AXIS2_CALL
axis2_woden_soap_header_block_create(const axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    header_block_impl = (axis2_woden_soap_header_block_impl_t *) create(env);

    header_block_impl->super = axis2_hash_make(env);
    if(!header_block_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(header_block_impl->super, "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING, 
            &(header_block_impl->header_block));
 
    return &(header_block_impl->header_block);
}

static axis2_status_t
axis2_woden_soap_header_block_free_ops(
        void *header_block,
        const axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    header_block_impl = INTF_TO_IMPL(header_block);

    if(header_block_impl->header_block.header_block_element.ops)
    {
        AXIS2_FREE(env->allocator, header_block_impl->header_block.
                header_block_element.ops);
        header_block_impl->header_block.header_block_element.ops = NULL;
    }

    if(header_block_impl->header_block.header_block_element.base.ext_element.ops)
    {
        AXIS2_FREE(env->allocator, header_block_impl->header_block.
                header_block_element.base.ext_element.ops);
        header_block_impl->header_block.header_block_element.base.ext_element.ops = NULL;
    }

    if(header_block_impl->header_block.header_block_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, header_block_impl->header_block.
                header_block_element.base.attr_extensible.ops);
        header_block_impl->header_block.header_block_element.base.attr_extensible.ops = NULL;
    }

    if(header_block_impl->header_block.header_block_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, header_block_impl->header_block.
                header_block_element.base.element_extensible.ops);
        header_block_impl->header_block.header_block_element.base.element_extensible.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_woden_soap_header_block_free(void *header_block,
                        const axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    header_block_impl = INTF_TO_IMPL(header_block);

    if(header_block_impl->super)
    {
        axis2_hash_free(header_block_impl->super, env);
        header_block_impl->super = NULL;
    }
    
    if(header_block_impl->methods)
    {
        axis2_hash_free(header_block_impl->methods, env);
        header_block_impl->methods = NULL;
    }

    if(header_block_impl->f_parent)
    {
        AXIS2_WODEN_WSDL_ELEMENT_FREE(header_block_impl->f_parent, env);
        header_block_impl->f_parent = NULL;
    }

    if(header_block_impl->f_documentation_elements)
    {
        AXIS2_ARRAY_LIST_FREE(header_block_impl->f_documentation_elements, env);
        header_block_impl->f_documentation_elements = NULL;
    }

    if(header_block_impl->f_ext_element_type)
    {
        AXIS2_QNAME_FREE(header_block_impl->f_ext_element_type, env);
        header_block_impl->f_ext_element_type = NULL;
    }

    if(header_block_impl->f_attr_ext)
    {
        AXIS2_WODEN_ATTR_EXTENSIBLE_FREE(header_block_impl->f_attr_ext, env);
        header_block_impl->f_attr_ext = NULL;
    }

    if(header_block_impl->f_elem_ext)
    {
        AXIS2_WODEN_ELEMENT_EXTENSIBLE_FREE(header_block_impl->f_elem_ext, env);
        header_block_impl->f_elem_ext = NULL;
    }

    if(header_block_impl->f_element_decl_qn)
    {
        AXIS2_QNAME_FREE(header_block_impl->f_element_decl_qn, env);
        header_block_impl->f_element_decl_qn = NULL;
    }

    if(header_block_impl->f_element_decl)
    {
        AXIS2_WODEN_ELEMENT_DECL_FREE(header_block_impl->f_element_decl, env);
        header_block_impl->f_element_decl = NULL;
    }

    if(header_block_impl->f_types)
    {
        AXIS2_WODEN_TYPES_FREE(header_block_impl->f_types, env);
        header_block_impl->f_types = NULL;
    }

    axis2_woden_soap_header_block_free_ops(header_block, env);

    if((&(header_block_impl->header_block))->ops)
    {
        AXIS2_FREE(env->allocator, (&(header_block_impl->header_block))->ops);
        (&(header_block_impl->header_block))->ops = NULL;
    }
    
    if(header_block_impl)
    {
        AXIS2_FREE(env->allocator, header_block_impl);
        header_block_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_soap_header_block_super_objs(
        void *header_block,
        const axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    header_block_impl = INTF_TO_IMPL(header_block);

    return header_block_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_soap_header_block_type(
        void *header_block,
        const axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    header_block_impl = INTF_TO_IMPL(header_block);

    return header_block_impl->obj_type;
}

axis2_status_t AXIS2_CALL
axis2_woden_soap_header_block_resolve_methods(
        axis2_woden_soap_header_block_t *header_block,
        const axis2_env_t *env,
        axis2_woden_soap_header_block_t *header_block_impl,
        axis2_hash_t *methods)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    header_block_impl_l = INTF_TO_IMPL(header_block_impl);
    
    header_block->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    header_block->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    header_block->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    header_block->ops->get_element_decl = axis2_hash_get(methods, 
            "get_element_decl", AXIS2_HASH_KEY_STRING);
    if(!header_block->ops->get_element_decl && header_block_impl_l)
            header_block->ops->get_element_decl = 
            header_block_impl_l->header_block.ops->get_element_decl;
    
    header_block->ops->must_understand = axis2_hash_get(methods, 
            "must_understand", AXIS2_HASH_KEY_STRING);
    if(!header_block->ops->must_understand && header_block_impl_l)
            header_block->ops->must_understand = 
            header_block_impl_l->header_block.ops->must_understand;
    
    header_block->ops->is_required = axis2_hash_get(methods, 
            "is_required", AXIS2_HASH_KEY_STRING);
    if(!header_block->ops->is_required && header_block_impl_l)
            header_block->ops->is_required = 
            header_block_impl_l->header_block.ops->is_required;
    
    header_block->ops->get_parent = axis2_hash_get(methods, 
            "get_parent", AXIS2_HASH_KEY_STRING);
    if(!header_block->ops->get_parent && header_block_impl_l)
            header_block->ops->get_parent = 
            header_block_impl_l->header_block.ops->get_parent;
    
    header_block->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!header_block->ops->to_element && header_block_impl_l)
            header_block->ops->to_element = 
            header_block_impl_l->header_block.ops->to_element;
    
    header_block->ops->set_element_decl = axis2_hash_get(methods, 
            "set_element_decl", AXIS2_HASH_KEY_STRING);
    if(!header_block->ops->set_element_decl && header_block_impl_l)
            header_block->ops->set_element_decl = 
            header_block_impl_l->header_block.ops->set_element_decl;
    
    header_block->ops->set_types = axis2_hash_get(methods, 
            "set_types", AXIS2_HASH_KEY_STRING);
    if(!header_block->ops->set_types && header_block_impl_l)
            header_block->ops->set_types = 
            header_block_impl_l->header_block.ops->set_types;
    
    return AXIS2_SUCCESS;
}



/* ***********************************************************************
 *  Component model methods (SOAPHeaderBlock interface), some shared with Element model
 * ***********************************************************************/

void *AXIS2_CALL
axis2_woden_soap_header_block_get_element_decl(
        void *header_block,
        axis2_env_t *env) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return header_block_impl->f_element_decl;
}

axis2_bool_t AXIS2_CALL 
axis2_woden_soap_header_block_must_understand(
        void *header_block,
        axis2_env_t *env) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return header_block_impl->f_must_understand;
}

axis2_bool_t AXIS2_CALL 
axis2_woden_soap_header_block_is_required(
        void *header_block,
        axis2_env_t *env) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return header_block_impl->f_required;
}

void *AXIS2_CALL
axis2_woden_soap_header_block_get_parent(
        void *header_block,
        axis2_env_t *env) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return header_block_impl->f_parent;
}

void *AXIS2_CALL
axis2_woden_soap_header_block_to_element(
        void *header_block,
        axis2_env_t *env) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return &(header_block_impl->header_block);
}


/* ***********************************************************************
 *  Element model-only methods (SOAPHeaderBlockElement interface)
 * ***********************************************************************/

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_element_qname(
        void *header_block,
        axis2_env_t *env,
        axis2_qname_t *qname) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    if(header_block_impl->f_element_decl_qn)
    {
        AXIS2_QNAME_FREE(header_block_impl->f_element_decl_qn, env);
        header_block_impl->f_element_decl_qn = NULL;
    }

    header_block_impl->f_element_decl_qn = AXIS2_QNAME_CLONE(qname, env);

    return AXIS2_SUCCESS;

}

axis2_qname_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_element_qname(
        void *header_block,
        axis2_env_t *env) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return header_block_impl->f_element_decl_qn;
}

void *AXIS2_CALL
axis2_woden_soap_header_block_get_element(
        void *header_block,
        axis2_env_t *env) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;
    void *xse = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    if(header_block_impl->f_types != NULL) 
    {
        xse = AXIS2_WODEN_TYPES_GET_ELEMENT_DECLARATION(
                header_block_impl->f_types, env, header_block_impl->
                f_element_decl_qn);
    }
    return xse;
}

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_must_understand(
        void *header_block,
        axis2_env_t *env,
        axis2_bool_t understood) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    header_block_impl->f_must_understand = understood;

    return AXIS2_SUCCESS;

}

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_parent_element(
        void *header_block,
        axis2_env_t *env,
        void *wsdl_el) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    if(header_block_impl->f_parent)
    {
        /* TODO */
        header_block_impl->f_parent = NULL;
    }
    header_block_impl->f_parent = wsdl_el;

    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_soap_header_block_get_parent_element(
        void *header_block,
        axis2_env_t *env) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return header_block_impl->f_parent;
}

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_add_documentation_element(
        void *header_block,
        axis2_env_t *env,
        void *doc_el) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, doc_el, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    if(!header_block_impl->f_documentation_elements)
    {
        header_block_impl->f_documentation_elements = 
            axis2_array_list_create(env, 0);
        if(!header_block_impl->f_documentation_elements)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(header_block_impl->f_documentation_elements, env, doc_el);
    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_documentation_elements(
        void *header_block,
        axis2_env_t *env) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return header_block_impl->f_documentation_elements;
}

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_extension_type(
        void *header_block,
        axis2_env_t *env,
        axis2_qname_t *qname) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    if(header_block_impl->f_ext_element_type)
    {
        AXIS2_QNAME_FREE(header_block_impl->f_ext_element_type, env);
        header_block_impl->f_ext_element_type = NULL;
    }

    header_block_impl->f_ext_element_type = AXIS2_QNAME_CLONE(qname, env);

    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_extension_type(
        void *header_block,
        axis2_env_t *env) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return header_block_impl->f_ext_element_type;
}

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_required(
        void *header_block,
        axis2_env_t *env,
        axis2_bool_t required) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    header_block_impl->f_required = required;

    return AXIS2_SUCCESS;
}

/* ***********************************************************************
 *  Extensibility methods
 * ***********************************************************************/

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_ext_attr(
        void *header_block,
        axis2_env_t *env,
        axis2_qname_t *attr_type, 
        void *attr) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attr_type, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attr, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return AXIS2_WODEN_ATTR_EXTENSIBLE_SET_EXT_ATTRIBUTE(
            header_block_impl->f_attr_ext, env, attr_type, attr);
}

void *AXIS2_CALL
axis2_woden_soap_header_block_get_ext_attr(
        void *header_block,
        axis2_env_t *env,
        axis2_qname_t *attr_type) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, attr_type, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTR(
            header_block_impl->f_attr_ext, env, attr_type);
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_ext_attrs_for_namespace(
        void *header_block,
        axis2_env_t *env,
        axis2_url_t *namespc) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, namespc, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS_FOR_NAMESPACE(
            header_block_impl->f_attr_ext, env, namespc);
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_ext_attrs(
        void *header_block,
        axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS(
            header_block_impl->f_attr_ext, env);
}

axis2_bool_t AXIS2_CALL 
axis2_woden_soap_header_block_has_ext_attrs_for_namespace(
        void *header_block,
        axis2_env_t *env,
        axis2_url_t *namespc) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, namespc, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return AXIS2_WODEN_ATTR_EXTENSIBLE_HAS_EXT_ATTRS_FOR_NAMESPACE(
            header_block_impl->f_attr_ext, env, namespc);
}

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_add_ext_element(
        void *header_block,
        axis2_env_t *env,
        void *ext_el) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ext_el, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(
            header_block_impl->f_elem_ext, env, ext_el);
}

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_remove_ext_element(
        void *header_block,
        axis2_env_t *env,
        void *ext_el) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ext_el, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return AXIS2_WODEN_ELEMENT_EXTENSIBLE_REMOVE_EXT_ELEMENT(
            header_block_impl->f_elem_ext, env, ext_el);

    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_ext_elements(
        void *header_block,
        axis2_env_t *env)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return AXIS2_WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(
            header_block_impl->f_elem_ext, env);
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_soap_header_block_get_ext_elements_of_type(
        void *header_block,
        axis2_env_t *env,
        axis2_qname_t *ext_type) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ext_type, NULL);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return AXIS2_WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS_OF_TYPE(
            header_block_impl->f_elem_ext, env, ext_type);
}

axis2_bool_t AXIS2_CALL 
axis2_woden_soap_header_block_has_ext_elements_for_namespace(
        void *header_block,
        axis2_env_t *env,
        axis2_url_t *namespc) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, namespc, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    return AXIS2_WODEN_ELEMENT_EXTENSIBLE_HAS_EXT_ELEMENTS_FOR_NAMESPACE(
            header_block_impl->f_elem_ext, env, namespc);
}

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_element_decl(
        void *header_block,
        axis2_env_t *env,
        void *element_decl)
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element_decl, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    if(header_block_impl->f_element_decl)
    {
        /* TODO */
        header_block_impl->f_element_decl = NULL;
    }

    header_block_impl->f_element_decl = element_decl;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_woden_soap_header_block_set_types(
        void *header_block,
        axis2_env_t *env,
        void *types) 
{
    axis2_woden_soap_header_block_impl_t *header_block_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, types, AXIS2_FAILURE);
    super = AXIS2_WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(header_block, env);
    header_block_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_HEADER_BLOCK", AXIS2_HASH_KEY_STRING));

    if(header_block_impl->f_types)
    {
        /* TODO */
        header_block_impl->f_types = NULL;
    }

    header_block_impl->f_types = types;

    return AXIS2_SUCCESS;
}

