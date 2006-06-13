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

#include <woden/wsdl20/extensions/soap/axis2_woden_soap_module_deserializer.h>
#include <woden/wsdl20/extensions/soap/axis2_woden_soap_module.h>
#include <woden/wsdl20/extensions/soap/axis2_woden_soap_module_element.h>
#include <woden/wsdl20/axis2_woden_desc.h>
#include <woden/wsdl20/xml/axis2_woden_desc_element.h>
#include <woden/wsdl20/axis2_woden_documentation.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include "../../woden_constants.h"
#include <axiom_element.h>
#include <axiom_util.h>


typedef struct axis2_woden_soap_module_deserializer_impl 
        axis2_woden_soap_module_deserializer_impl_t;

/** 
 * @brief Soap Module Deserializer Struct Impl
 *   Axis2 Soap Module Deserializer  
 */ 
struct axis2_woden_soap_module_deserializer_impl
{
    axis2_woden_soap_module_deserializer_t mod_deser;
    axis2_woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
};

#define INTF_TO_IMPL(mod_deser) \
    ((axis2_woden_soap_module_deserializer_impl_t *) mod_deser)

axis2_status_t AXIS2_CALL 
axis2_woden_soap_module_deserializer_free(
        void *mod_deser,
        const axis2_env_t *env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_soap_module_deserializer_type(
        void *mod_deser,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_soap_module_deserializer_super_objs(
        void *mod_deser,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_soap_module_deserializer_marshall(
        void *mod_deser,
        const axis2_env_t *env,
        axis2_char_t *parent_type,
        void *parent,
        axis2_qname_t *element_type,
        axiom_node_t *el_node,
        void *desc,
        axis2_woden_ext_registry_t *ext_reg);

static void *
parse_documentation(
        void *reader,
        const axis2_env_t *env,
        axiom_node_t *doc_el_node,
        void *desc);

static axis2_woden_soap_module_deserializer_t *
create(const axis2_env_t *env);

static axis2_status_t
axis2_woden_soap_module_deserializer_free_ops(
        void *mod_deser,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/

AXIS2_EXTERN axis2_woden_soap_module_deserializer_t * AXIS2_CALL
axis2_woden_soap_module_deserializer_to_ext_deserializer(
        void *mod_deser,
        const axis2_env_t *env)
{
    axis2_woden_soap_module_deserializer_impl_t *mod_deser_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!mod_deser)
    {
        mod_deser_impl = (axis2_woden_soap_module_deserializer_impl_t *) 
            create(env);
    }
    else
        mod_deser_impl = (axis2_woden_soap_module_deserializer_impl_t *) 
            mod_deser;

    axis2_woden_soap_module_deserializer_free_ops(mod_deser, env);

    mod_deser_impl->mod_deser.ext_deserializer.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_ext_deserializer_ops_t));
    axis2_woden_ext_deserializer_resolve_methods(&(mod_deser_impl->mod_deser.
            ext_deserializer), env, mod_deser_impl->methods);
    return mod_deser;
}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_soap_module_deserializer_t *
create(const axis2_env_t *env)
{
    axis2_woden_soap_module_deserializer_impl_t *mod_deser_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mod_deser_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_soap_module_deserializer_impl_t));

    mod_deser_impl->obj_type= AXIS2_WODEN_SOAP_MODULE_DESERIALIZER;
    mod_deser_impl->super = NULL;
    mod_deser_impl->methods = NULL;
    
    mod_deser_impl->mod_deser.ext_deserializer.ops = NULL;
    
    mod_deser_impl->mod_deser.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_woden_soap_module_deserializer_ops_t));

    mod_deser_impl->mod_deser.ops->free = 
        axis2_woden_soap_module_deserializer_free;
    mod_deser_impl->mod_deser.ops->super_objs = 
        axis2_woden_soap_module_deserializer_super_objs;
    mod_deser_impl->mod_deser.ops->type = 
        axis2_woden_soap_module_deserializer_type;
    
    mod_deser_impl->mod_deser.ops->marshall = 
        axis2_woden_soap_module_deserializer_marshall;
 
    mod_deser_impl->methods = axis2_hash_make(env);
    if(!mod_deser_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(mod_deser_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_module_deserializer_free);
    axis2_hash_set(mod_deser_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_module_deserializer_super_objs);
    axis2_hash_set(mod_deser_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_soap_module_deserializer_type);

    axis2_hash_set(mod_deser_impl->methods, "marshall", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_soap_module_deserializer_marshall);

    return &(mod_deser_impl->mod_deser);
}

AXIS2_EXTERN axis2_woden_soap_module_deserializer_t * AXIS2_CALL
axis2_woden_soap_module_deserializer_create(const axis2_env_t *env)
{
    axis2_woden_soap_module_deserializer_impl_t *mod_deser_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mod_deser_impl = (axis2_woden_soap_module_deserializer_impl_t *) create(env);

    mod_deser_impl->super = axis2_hash_make(env);
    if(!mod_deser_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(mod_deser_impl->super, "AXIS2_WODEN_SOAP_MODULE_DESERIALIZER", 
            AXIS2_HASH_KEY_STRING, &(mod_deser_impl->mod_deser));
 
    return &(mod_deser_impl->mod_deser);
}

static axis2_status_t
axis2_woden_soap_module_deserializer_free_ops(
        void *mod_deser,
        const axis2_env_t *env)
{
    axis2_woden_soap_module_deserializer_impl_t *mod_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mod_deser_impl = INTF_TO_IMPL(mod_deser);

    if(mod_deser_impl->mod_deser.ext_deserializer.ops)
    {
        AXIS2_FREE(env->allocator, mod_deser_impl->mod_deser.
                ext_deserializer.ops);
        mod_deser_impl->mod_deser.ext_deserializer.ops = 
            NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_woden_soap_module_deserializer_free(
        void *mod_deser,
        const axis2_env_t *env)
{
    axis2_woden_soap_module_deserializer_impl_t *mod_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mod_deser_impl = INTF_TO_IMPL(mod_deser);

    if(mod_deser_impl->super)
    {
        axis2_hash_free(mod_deser_impl->super, env);
        mod_deser_impl->super = NULL;
    }
    
    if(mod_deser_impl->methods)
    {
        axis2_hash_free(mod_deser_impl->methods, env);
        mod_deser_impl->methods = NULL;
    }

    axis2_woden_soap_module_deserializer_free_ops(mod_deser, env);

    if((&(mod_deser_impl->mod_deser))->ops)
    {
        AXIS2_FREE(env->allocator, (&(mod_deser_impl->mod_deser))->ops);
        (&(mod_deser_impl->mod_deser))->ops = NULL;
    }
    
    if(mod_deser_impl)
    {
        AXIS2_FREE(env->allocator, mod_deser_impl);
        mod_deser_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_soap_module_deserializer_super_objs(
        void *mod_deser,
        const axis2_env_t *env)
{
    axis2_woden_soap_module_deserializer_impl_t *mod_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mod_deser_impl = INTF_TO_IMPL(mod_deser);

    return mod_deser_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_soap_module_deserializer_type(
        void *mod_deser,
        const axis2_env_t *env)
{
    axis2_woden_soap_module_deserializer_impl_t *mod_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mod_deser_impl = INTF_TO_IMPL(mod_deser);

    return mod_deser_impl->obj_type;
}

axis2_status_t AXIS2_CALL
axis2_woden_soap_module_deserializer_resolve_methods(
        axis2_woden_soap_module_deserializer_t *mod_deser,
        const axis2_env_t *env,
        axis2_woden_soap_module_deserializer_t *mod_deser_impl,
        axis2_hash_t *methods)
{
    axis2_woden_soap_module_deserializer_impl_t *mod_deser_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    mod_deser_impl_l = INTF_TO_IMPL(mod_deser_impl);
    
    mod_deser->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    mod_deser->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    mod_deser->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    mod_deser->ops->marshall = axis2_hash_get(methods, 
            "marshall", AXIS2_HASH_KEY_STRING);
    if(!mod_deser->ops->marshall && mod_deser_impl_l)
            mod_deser->ops->marshall = 
            mod_deser_impl_l->mod_deser.ops->marshall;
    
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_soap_module_deserializer_marshall(
        void *mod_deser,
        const axis2_env_t *env,
        axis2_char_t *parent_type,
        void *parent,
        axis2_qname_t *element_type,
        axiom_node_t *el_node,
        void *desc,
        axis2_woden_ext_registry_t *ext_reg)
{
    axis2_woden_soap_module_deserializer_impl_t *mod_deser_impl = NULL;
    axis2_hash_t *super = NULL;
    void *soap_mod = NULL;
    axis2_char_t *ref = NULL;
    axis2_char_t *req = NULL;
    axiom_element_t *el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_bool_t required = AXIS2_FALSE;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SOAP_MODULE_DESERIALIZER_SUPER_OBJS(mod_deser, env);
    mod_deser_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SOAP_MODULE_DESERIALIZER", AXIS2_HASH_KEY_STRING));
   
    soap_mod = AXIS2_WODEN_EXT_REGISTRY_QUERY_EXT_ELEMENT_TYPE(ext_reg, env, 
            parent_type, element_type);

    soap_mod = axis2_woden_soap_module_to_ext_element(soap_mod, env);
    AXIS2_WODEN_EXT_ELEMENT_SET_EXT_TYPE(soap_mod, env, element_type);
    soap_mod = axis2_woden_soap_module_to_soap_module_element(soap_mod, env);
    AXIS2_WODEN_SOAP_MODULE_ELEMENT_SET_PARENT_ELEMENT(soap_mod, env, parent);

    ref = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(el, env, 
            WODEN_ATTR_REF); 
    if(NULL != ref)
    {
        axis2_uri_t *uri = axis2_uri_parse_string(env, ref);
        
        soap_mod = axis2_woden_soap_module_to_soap_module_element(soap_mod, env);
        AXIS2_WODEN_SOAP_MODULE_ELEMENT_SET_REF(soap_mod, env, uri);
    }
    req = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(el, env, 
            WODEN_ATTR_REQUIRED);
    if(NULL != req && 0 == AXIS2_STRCMP(WODEN_VALUE_TRUE, req))
            required = AXIS2_TRUE;
    else
            required = AXIS2_FALSE;
    soap_mod = axis2_woden_soap_module_to_ext_element(soap_mod, env);
    AXIS2_WODEN_EXT_ELEMENT_SET_REQUIRED(soap_mod, env, required);

    el = AXIOM_NODE_GET_DATA_ELEMENT(el_node, env);
    temp_el = axiom_util_get_first_child_element(el, env, el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_DOCUMENTATION);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(mod_deser, env, temp_el_node, desc);
            soap_mod = axis2_woden_soap_module_to_soap_module_element(soap_mod, env);
            AXIS2_WODEN_SOAP_MODULE_ELEMENT_ADD_DOCUMENTATION_ELEMENT(soap_mod, env, 
                    documentation);
        }
        else
        {
            /* TODO Parse ext elements */
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return soap_mod;
}

static void *
parse_documentation(
        void *mod_deser,
        const axis2_env_t *env,
        axiom_node_t *doc_el_node,
        void *desc)
{
    void *documentation = NULL;
    
    desc = axis2_woden_desc_to_desc_element(desc, env);
    documentation = AXIS2_WODEN_DESC_ELEMENT_CREATE_DOCUMENTATION_ELEMENT(desc, 
            env);
    
    documentation = axis2_woden_documentation_to_documentation_element(
            documentation, env);
    AXIS2_WODEN_DOCUMENTATION_ELEMENT_SET_CONTENT(documentation, env, doc_el_node);
    
    return documentation;
}


