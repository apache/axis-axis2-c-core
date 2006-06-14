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

#include <woden_soap_module.h>
#include <woden_wsdl_element.h>
#include <woden_string_attr.h>
#include <woden_uri_attr.h>
#include <woden_types.h>
#include "woden_soap_constants.h"

typedef struct woden_soap_module_impl woden_soap_module_impl_t;

/** 
 * @brief Soap Modules Struct Impl
 *   Axis2 Soap Modules  
 */ 
struct woden_soap_module_impl
{
    woden_soap_module_t module;
    axis2_hash_t *methods;
    axis2_hash_t *super;
    woden_obj_types_t obj_type;
    
    void *f_parent;
    axis2_array_list_t *f_documentation_elements;
    axis2_qname_t *f_ext_element_type;
    axis2_bool_t f_required;
    void *f_attr_ext;
    void *f_elem_ext;
    axis2_uri_t *f_ref;
};

#define INTF_TO_IMPL(module) ((woden_soap_module_impl_t *) module)

axis2_status_t AXIS2_CALL 
woden_soap_module_free(
        void *module,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
woden_soap_module_super_objs(
        void *module,
        const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL 
woden_soap_module_type(
        void *module,
        const axis2_env_t *env);

/* ***********************************************************************
 *  Component model methods (SOAPHeaderBlock interface), some shared with Element model
 * ***********************************************************************/

axis2_uri_t *AXIS2_CALL
woden_soap_module_get_ref(
        void *module,
        axis2_env_t *env);

axis2_bool_t AXIS2_CALL 
woden_soap_module_is_required(
        void *module,
        axis2_env_t *env);

void *AXIS2_CALL
woden_soap_module_get_parent(
        void *module,
        axis2_env_t *env);

void *AXIS2_CALL
woden_soap_module_to_element(
        void *module,
        axis2_env_t *env);

/* ***********************************************************************
 *  Element model-only methods (SOAPHeaderBlockElement interface)
 * ***********************************************************************/

axis2_status_t AXIS2_CALL 
woden_soap_module_set_ref(
        void *module,
        axis2_env_t *env,
        axis2_uri_t *uri); 

axis2_status_t AXIS2_CALL 
woden_soap_module_set_parent_element(
        void *module,
        axis2_env_t *env,
        void *wsdl_el);

void *AXIS2_CALL
woden_soap_module_get_parent_element(
        void *module,
        axis2_env_t *env); 

axis2_status_t AXIS2_CALL 
woden_soap_module_add_documentation_element(
        void *module,
        axis2_env_t *env,
        void *doc_el);

axis2_array_list_t *AXIS2_CALL 
woden_soap_module_get_documentation_elements(
        void *module,
        axis2_env_t *env);

axis2_status_t AXIS2_CALL 
woden_soap_module_set_extension_type(
        void *module,
        axis2_env_t *env,
        axis2_qname_t *qname); 

axis2_qname_t *AXIS2_CALL 
woden_soap_module_get_extension_type(
        void *module,
        axis2_env_t *env);

axis2_status_t AXIS2_CALL 
woden_soap_module_set_required(
        void *module,
        axis2_env_t *env,
        axis2_bool_t required);

/* ***********************************************************************
 *  Extensibility methods
 * ***********************************************************************/

axis2_status_t AXIS2_CALL 
woden_soap_module_set_ext_attr(
        void *module,
        axis2_env_t *env,
        axis2_qname_t *attr_type, 
        void *attr);

void *AXIS2_CALL
woden_soap_module_get_ext_attr(
        void *module,
        axis2_env_t *env,
        axis2_qname_t *attr_type);

axis2_array_list_t *AXIS2_CALL 
woden_soap_module_get_ext_attrs_for_namespace(
        void *module,
        axis2_env_t *env,
        axis2_uri_t *namespc);

axis2_array_list_t *AXIS2_CALL 
woden_soap_module_get_ext_attrs(
        void *module,
        axis2_env_t *env);

axis2_bool_t AXIS2_CALL 
woden_soap_module_has_ext_attrs_for_namespace(
        void *module,
        axis2_env_t *env,
        axis2_uri_t *namespc);

axis2_status_t AXIS2_CALL 
woden_soap_module_add_ext_element(
        void *module,
        axis2_env_t *env,
        void *ext_el);

axis2_status_t AXIS2_CALL 
woden_soap_module_remove_ext_element(
        void *module,
        axis2_env_t *env,
        void *ext_el);

axis2_array_list_t *AXIS2_CALL 
woden_soap_module_get_ext_elements(
        void *module,
        axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL 
woden_soap_module_get_ext_elements_of_type(
        void *module,
        axis2_env_t *env,
        axis2_qname_t *ext_type); 

axis2_bool_t AXIS2_CALL 
woden_soap_module_has_ext_elements_for_namespace(
        void *module,
        axis2_env_t *env,
        axis2_uri_t *namespc); 

static woden_soap_module_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_soap_module_free_ops(
        void *module,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_soap_module_t * AXIS2_CALL
woden_soap_module_to_soap_module_element(
        void *module,
        const axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!module)
    {
        module_impl = (woden_soap_module_impl_t *) create(env);
    }
    else
        module_impl = (woden_soap_module_impl_t *) module;
    woden_soap_module_free_ops(module, env);

    module_impl->module.module_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_soap_module_element_ops_t));
    woden_soap_module_element_resolve_methods(&(module_impl->module.
            module_element), env, module_impl->methods);
    return module;
}

AXIS2_EXTERN woden_soap_module_t * AXIS2_CALL
woden_soap_module_to_ext_element(
        void *module,
        const axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!module)
    {
        module_impl = (woden_soap_module_impl_t *) create(env);
    }
    else
        module_impl = (woden_soap_module_impl_t *) module;
    woden_soap_module_free_ops(module, env);

    module_impl->module.module_element.base.ext_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_ext_element_ops_t));
    woden_ext_element_resolve_methods(&(module_impl->module.
            module_element.base.ext_element), env, module_impl->methods);
    return module;
}

AXIS2_EXTERN woden_soap_module_t * AXIS2_CALL
woden_soap_module_to_attr_extensible(
        void *module,
        const axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!module)
    {
        module_impl = (woden_soap_module_impl_t *) create(env);
    }
    else
        module_impl = (woden_soap_module_impl_t *) module;
    woden_soap_module_free_ops(module, env);

    module_impl->module.module_element.base.attr_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(module_impl->module.
            module_element.base.attr_extensible), env, NULL, module_impl->methods);
    return module;
}

AXIS2_EXTERN woden_soap_module_t * AXIS2_CALL
woden_soap_module_to_element_extensible(
        void *module,
        const axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!module)
    {
        module_impl = (woden_soap_module_impl_t *) create(env);
    }
    else
        module_impl = (woden_soap_module_impl_t *) module;
    woden_soap_module_free_ops(module, env);

    module_impl->module.module_element.base.element_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(module_impl->module.
            module_element.base.element_extensible), env, NULL, module_impl->methods);
    return module;
}

/************************End of Woden C Internal Methods***********************/
static woden_soap_module_t *
create(const axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    module_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_soap_module_impl_t));

    module_impl->obj_type= WODEN_SOAP_MODULE;
    module_impl->super = NULL;
    module_impl->methods = NULL;

    module_impl->f_parent = NULL;
    module_impl->f_documentation_elements = NULL;
    module_impl->f_ext_element_type = NULL;
    module_impl->f_required = AXIS2_FALSE;
    module_impl->f_attr_ext = NULL;
    module_impl->f_elem_ext = NULL;
    module_impl->f_ref = NULL;
    
    module_impl->module.module_element.ops = NULL;
    module_impl->module.module_element.base.ext_element.ops = NULL;
    module_impl->module.module_element.base.attr_extensible.ops = NULL;
    module_impl->module.module_element.base.element_extensible.ops = NULL;
    
    module_impl->module.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(woden_soap_module_ops_t));

    module_impl->module.ops->free = woden_soap_module_free;
    module_impl->module.ops->super_objs = 
        woden_soap_module_super_objs;
    module_impl->module.ops->type = 
        woden_soap_module_type;
    
    module_impl->module.ops->get_ref = 
        woden_soap_module_get_ref;
    
    module_impl->module.ops->is_required = 
        woden_soap_module_is_required;
    
    module_impl->module.ops->get_parent = 
        woden_soap_module_get_parent;
    
    module_impl->module.ops->to_element = 
        woden_soap_module_to_element;
    
    module_impl->methods = axis2_hash_make(env);
    if(!module_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(module_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            woden_soap_module_free);
    axis2_hash_set(module_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, woden_soap_module_super_objs);
    axis2_hash_set(module_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, woden_soap_module_type);

    axis2_hash_set(module_impl->methods, "get_ref", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_get_ref);

    axis2_hash_set(module_impl->methods, "is_required", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_is_required);

    axis2_hash_set(module_impl->methods, "get_parent", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_get_parent);

    axis2_hash_set(module_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_to_element);

    axis2_hash_set(module_impl->methods, "set_ref", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_set_ref);

    axis2_hash_set(module_impl->methods, "set_parent_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_set_parent_element);

    axis2_hash_set(module_impl->methods, "get_parent_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_get_parent_element);

    axis2_hash_set(module_impl->methods, "add_documentation_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_add_documentation_element);

    axis2_hash_set(module_impl->methods, "get_documentation_elements", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_get_documentation_elements);

    axis2_hash_set(module_impl->methods, "set_extension_type", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_set_extension_type);

    axis2_hash_set(module_impl->methods, "get_extension_type", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_get_extension_type);

    axis2_hash_set(module_impl->methods, "set_required", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_set_required);

    axis2_hash_set(module_impl->methods, "set_ext_attr", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_set_ext_attr);

    axis2_hash_set(module_impl->methods, "get_ext_attr", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_get_ext_attr);

    axis2_hash_set(module_impl->methods, "get_ext_attrs_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_get_ext_attrs_for_namespace);

    axis2_hash_set(module_impl->methods, "get_ext_attrs", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_get_ext_attrs);

    axis2_hash_set(module_impl->methods, "has_ext_attrs_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_has_ext_attrs_for_namespace);

    axis2_hash_set(module_impl->methods, "add_ext_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_add_ext_element);

    axis2_hash_set(module_impl->methods, "remove_ext_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_remove_ext_element);

    axis2_hash_set(module_impl->methods, "get_ext_elements", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_get_ext_elements);

    axis2_hash_set(module_impl->methods, "get_ext_elements_of_type", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_get_ext_elements_of_type);

    axis2_hash_set(module_impl->methods, "has_ext_elements_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            woden_soap_module_has_ext_elements_for_namespace);

    return &(module_impl->module);
}

AXIS2_EXTERN woden_soap_module_t * AXIS2_CALL
woden_soap_module_create(const axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    module_impl = (woden_soap_module_impl_t *) create(env);

    module_impl->super = axis2_hash_make(env);
    if(!module_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(module_impl->super, "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING, 
            &(module_impl->module));
 
    return &(module_impl->module);
}

static axis2_status_t
woden_soap_module_free_ops(
        void *module,
        const axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    module_impl = INTF_TO_IMPL(module);

    if(module_impl->module.module_element.ops)
    {
        AXIS2_FREE(env->allocator, module_impl->module.
                module_element.ops);
        module_impl->module.module_element.ops = NULL;
    }

    if(module_impl->module.module_element.base.ext_element.ops)
    {
        AXIS2_FREE(env->allocator, module_impl->module.
                module_element.base.ext_element.ops);
        module_impl->module.module_element.base.ext_element.ops = NULL;
    }

    if(module_impl->module.module_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, module_impl->module.
                module_element.base.attr_extensible.ops);
        module_impl->module.module_element.base.attr_extensible.ops = NULL;
    }

    if(module_impl->module.module_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, module_impl->module.
                module_element.base.element_extensible.ops);
        module_impl->module.module_element.base.element_extensible.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_soap_module_free(void *module,
                        const axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    module_impl = INTF_TO_IMPL(module);

    if(module_impl->super)
    {
        axis2_hash_free(module_impl->super, env);
        module_impl->super = NULL;
    }
    
    if(module_impl->methods)
    {
        axis2_hash_free(module_impl->methods, env);
        module_impl->methods = NULL;
    }

    if(module_impl->f_parent)
    {
        WODEN_WSDL_ELEMENT_FREE(module_impl->f_parent, env);
        module_impl->f_parent = NULL;
    }

    if(module_impl->f_documentation_elements)
    {
        AXIS2_ARRAY_LIST_FREE(module_impl->f_documentation_elements, env);
        module_impl->f_documentation_elements = NULL;
    }

    if(module_impl->f_ext_element_type)
    {
        AXIS2_QNAME_FREE(module_impl->f_ext_element_type, env);
        module_impl->f_ext_element_type = NULL;
    }

    if(module_impl->f_attr_ext)
    {
        WODEN_ATTR_EXTENSIBLE_FREE(module_impl->f_attr_ext, env);
        module_impl->f_attr_ext = NULL;
    }

    if(module_impl->f_elem_ext)
    {
        WODEN_ELEMENT_EXTENSIBLE_FREE(module_impl->f_elem_ext, env);
        module_impl->f_elem_ext = NULL;
    }

    if(module_impl->f_ref)
    {
        AXIS2_URI_FREE(module_impl->f_ref, env);
        module_impl->f_ref = NULL;
    }

    woden_soap_module_free_ops(module, env);

    if((&(module_impl->module))->ops)
    {
        AXIS2_FREE(env->allocator, (&(module_impl->module))->ops);
        (&(module_impl->module))->ops = NULL;
    }
    
    if(module_impl)
    {
        AXIS2_FREE(env->allocator, module_impl);
        module_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_soap_module_super_objs(
        void *module,
        const axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    module_impl = INTF_TO_IMPL(module);

    return module_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_soap_module_type(
        void *module,
        const axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    module_impl = INTF_TO_IMPL(module);

    return module_impl->obj_type;
}

axis2_status_t AXIS2_CALL
woden_soap_module_resolve_methods(
        woden_soap_module_t *module,
        const axis2_env_t *env,
        woden_soap_module_t *module_impl,
        axis2_hash_t *methods)
{
    woden_soap_module_impl_t *module_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    module_impl_l = INTF_TO_IMPL(module_impl);
    
    module->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    module->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    module->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    module->ops->get_ref = axis2_hash_get(methods, 
            "get_ref", AXIS2_HASH_KEY_STRING);
    if(!module->ops->get_ref && module_impl_l)
            module->ops->get_ref = 
            module_impl_l->module.ops->get_ref;
    
    module->ops->is_required = axis2_hash_get(methods, 
            "is_required", AXIS2_HASH_KEY_STRING);
    if(!module->ops->is_required && module_impl_l)
            module->ops->is_required = 
            module_impl_l->module.ops->is_required;
    
    module->ops->get_parent = axis2_hash_get(methods, 
            "get_parent", AXIS2_HASH_KEY_STRING);
    if(!module->ops->get_parent && module_impl_l)
            module->ops->get_parent = 
            module_impl_l->module.ops->get_parent;
    
    module->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!module->ops->to_element && module_impl_l)
            module->ops->to_element = 
            module_impl_l->module.ops->to_element;
    
    return AXIS2_SUCCESS;
}

/* ***********************************************************************
 *  Component model methods (SOAPHeaderBlock interface), some shared with Element model
 * ***********************************************************************/

axis2_uri_t *AXIS2_CALL
woden_soap_module_get_ref(
        void *module,
        axis2_env_t *env) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return module_impl->f_ref;
}

axis2_bool_t AXIS2_CALL 
woden_soap_module_is_required(
        void *module,
        axis2_env_t *env) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return module_impl->f_required;
}

void *AXIS2_CALL
woden_soap_module_get_parent(
        void *module,
        axis2_env_t *env) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return module_impl->f_parent;
}

void *AXIS2_CALL
woden_soap_module_to_element(
        void *module,
        axis2_env_t *env) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return &(module_impl->module);
}


/* ***********************************************************************
 *  Element model-only methods (SOAPHeaderBlockElement interface)
 * ***********************************************************************/

axis2_status_t AXIS2_CALL 
woden_soap_module_set_ref(
        void *module,
        axis2_env_t *env,
        axis2_uri_t *uri) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, uri, AXIS2_FAILURE);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    if(module_impl->f_ref)
    {
        AXIS2_URI_FREE(module_impl->f_ref, env);
        module_impl->f_ref = NULL;
    }

    module_impl->f_ref = AXIS2_URI_CLONE(uri, env);

    return AXIS2_SUCCESS;

}

axis2_status_t AXIS2_CALL 
woden_soap_module_set_parent_element(
        void *module,
        axis2_env_t *env,
        void *wsdl_el) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    if(module_impl->f_parent)
    {
        /* TODO */
        module_impl->f_parent = NULL;
    }
    module_impl->f_parent = wsdl_el;

    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_soap_module_get_parent_element(
        void *module,
        axis2_env_t *env) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return module_impl->f_parent;
}

axis2_status_t AXIS2_CALL 
woden_soap_module_add_documentation_element(
        void *module,
        axis2_env_t *env,
        void *doc_el) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, doc_el, AXIS2_FAILURE);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    if(!module_impl->f_documentation_elements)
    {
        module_impl->f_documentation_elements = 
            axis2_array_list_create(env, 0);
        if(!module_impl->f_documentation_elements)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(module_impl->f_documentation_elements, env, doc_el);
    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL 
woden_soap_module_get_documentation_elements(
        void *module,
        axis2_env_t *env) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return module_impl->f_documentation_elements;
}

axis2_status_t AXIS2_CALL 
woden_soap_module_set_extension_type(
        void *module,
        axis2_env_t *env,
        axis2_qname_t *qname) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    if(module_impl->f_ext_element_type)
    {
        AXIS2_QNAME_FREE(module_impl->f_ext_element_type, env);
        module_impl->f_ext_element_type = NULL;
    }

    module_impl->f_ext_element_type = AXIS2_QNAME_CLONE(qname, env);

    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL 
woden_soap_module_get_extension_type(
        void *module,
        axis2_env_t *env) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return module_impl->f_ext_element_type;
}

axis2_status_t AXIS2_CALL 
woden_soap_module_set_required(
        void *module,
        axis2_env_t *env,
        axis2_bool_t required) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    module_impl->f_required = required;

    return AXIS2_SUCCESS;
}

/* ***********************************************************************
 *  Extensibility methods
 * ***********************************************************************/

axis2_status_t AXIS2_CALL 
woden_soap_module_set_ext_attr(
        void *module,
        axis2_env_t *env,
        axis2_qname_t *attr_type, 
        void *attr) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attr_type, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attr, AXIS2_FAILURE);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return WODEN_ATTR_EXTENSIBLE_SET_EXT_ATTR(
            module_impl->f_attr_ext, env, attr_type, attr);
}

void *AXIS2_CALL
woden_soap_module_get_ext_attr(
        void *module,
        axis2_env_t *env,
        axis2_qname_t *attr_type) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, attr_type, NULL);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTR(
            module_impl->f_attr_ext, env, attr_type);
}

axis2_array_list_t *AXIS2_CALL 
woden_soap_module_get_ext_attrs_for_namespace(
        void *module,
        axis2_env_t *env,
        axis2_uri_t *namespc) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, namespc, NULL);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS_FOR_NAMESPACE(
            module_impl->f_attr_ext, env, namespc);
}

axis2_array_list_t *AXIS2_CALL 
woden_soap_module_get_ext_attrs(
        void *module,
        axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS(
            module_impl->f_attr_ext, env);
}

axis2_bool_t AXIS2_CALL 
woden_soap_module_has_ext_attrs_for_namespace(
        void *module,
        axis2_env_t *env,
        axis2_uri_t *namespc) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, namespc, AXIS2_FAILURE);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return WODEN_ATTR_EXTENSIBLE_HAS_EXT_ATTRS_FOR_NAMESPACE(
            module_impl->f_attr_ext, env, namespc);
}

axis2_status_t AXIS2_CALL 
woden_soap_module_add_ext_element(
        void *module,
        axis2_env_t *env,
        void *ext_el) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ext_el, AXIS2_FAILURE);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(
            module_impl->f_elem_ext, env, ext_el);
}

axis2_status_t AXIS2_CALL 
woden_soap_module_remove_ext_element(
        void *module,
        axis2_env_t *env,
        void *ext_el) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ext_el, AXIS2_FAILURE);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return WODEN_ELEMENT_EXTENSIBLE_REMOVE_EXT_ELEMENT(
            module_impl->f_elem_ext, env, ext_el);

    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL 
woden_soap_module_get_ext_elements(
        void *module,
        axis2_env_t *env)
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(
            module_impl->f_elem_ext, env);
}

axis2_array_list_t *AXIS2_CALL 
woden_soap_module_get_ext_elements_of_type(
        void *module,
        axis2_env_t *env,
        axis2_qname_t *ext_type) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ext_type, NULL);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS_OF_TYPE(
            module_impl->f_elem_ext, env, ext_type);
}

axis2_bool_t AXIS2_CALL 
woden_soap_module_has_ext_elements_for_namespace(
        void *module,
        axis2_env_t *env,
        axis2_uri_t *namespc) 
{
    woden_soap_module_impl_t *module_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, namespc, AXIS2_FAILURE);
    super = WODEN_SOAP_MODULE_SUPER_OBJS(module, env);
    module_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_SOAP_MODULE", AXIS2_HASH_KEY_STRING));

    return WODEN_ELEMENT_EXTENSIBLE_HAS_EXT_ELEMENTS_FOR_NAMESPACE(
            module_impl->f_elem_ext, env, namespc);
}

