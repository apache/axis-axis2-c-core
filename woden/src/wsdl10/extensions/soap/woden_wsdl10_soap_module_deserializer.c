/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <woden_wsdl10_soap_module_deserializer.h>
#include <woden_wsdl10_soap_module.h>
#include <woden_wsdl10_soap_module_element.h>
#include <woden_wsdl10_desc.h>
#include <woden_wsdl10_desc_element.h>
#include <woden_wsdl10_soap_binding_op_exts.h>
#include <woden_wsdl10_soap_address_exts.h>
#include <woden_documentation.h>
#include <woden_documentation_element.h>
#include "woden_wsdl10_soap_constants.h"
#include "../../woden_wsdl10_constants.h"
#include <axiom_element.h>
#include <axiom_util.h>
#include <woden_qname_util.h>
#include <woden_wsdl10_ext_deserializer.h>
#include <woden_wsdl10_soap_address_exts.h>

typedef struct woden_wsdl10_soap_module_deserializer_impl
            woden_wsdl10_soap_module_deserializer_impl_t;

/**
 * @brief Soap Module Deserializer Struct Impl
 *   Axis2 Soap Module Deserializer
 */
struct woden_wsdl10_soap_module_deserializer_impl
{
    woden_wsdl10_soap_module_deserializer_t mod_deser;
    woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
};

#define INTF_TO_IMPL(mod_deser) \
    ((woden_wsdl10_soap_module_deserializer_impl_t *) mod_deser)

axis2_status_t AXIS2_CALL
woden_wsdl10_soap_module_deserializer_free(
    void *mod_deser,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_wsdl10_soap_module_deserializer_type(
    void *mod_deser,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_wsdl10_soap_module_deserializer_super_objs(
    void *mod_deser,
    const axis2_env_t *env);

void *AXIS2_CALL
woden_wsdl10_soap_module_deserializer_unmarshall(
    void *mod_deser,
    const axis2_env_t *env,
    axis2_char_t *parent_type,
    void *parent,
    axis2_qname_t *element_type,
    axiom_node_t *el_node,
    void *desc,
    woden_wsdl10_ext_registry_t *ext_reg);

static void *
parse_documentation(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *doc_el_node,
    void *desc);

static woden_wsdl10_soap_module_deserializer_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_wsdl10_soap_module_deserializer_free_ops(
    void *mod_deser,
    const axis2_env_t *env);

/************************Woden C Internal Methods******************************/

AXIS2_EXTERN woden_wsdl10_soap_module_deserializer_t * AXIS2_CALL
woden_wsdl10_soap_module_deserializer_to_ext_deserializer(
    void *mod_deser,
    const axis2_env_t *env)
{
    woden_wsdl10_soap_module_deserializer_impl_t *mod_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!mod_deser)
    {
        mod_deser_impl = (woden_wsdl10_soap_module_deserializer_impl_t *)
                create(env);
    }
    else
        mod_deser_impl = (woden_wsdl10_soap_module_deserializer_impl_t *)
                mod_deser;

    woden_wsdl10_soap_module_deserializer_free_ops(mod_deser, env);

    mod_deser_impl->mod_deser.ext_deserializer.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_wsdl10_ext_deserializer_ops_t));
    woden_wsdl10_ext_deserializer_resolve_methods(&(mod_deser_impl->mod_deser.
            ext_deserializer), env, mod_deser_impl->methods);
    return mod_deser;
}

/************************End of Woden C Internal Methods***********************/
static woden_wsdl10_soap_module_deserializer_t *
create(const axis2_env_t *env)
{
    woden_wsdl10_soap_module_deserializer_impl_t *mod_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    mod_deser_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl10_soap_module_deserializer_impl_t));

    mod_deser_impl->obj_type = WODEN_WSDL10_SOAP_MODULE_DESERIALIZER;
    mod_deser_impl->super = NULL;
    mod_deser_impl->methods = NULL;

    mod_deser_impl->mod_deser.ext_deserializer.ops = NULL;

    mod_deser_impl->mod_deser.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl10_soap_module_deserializer_ops_t));

    mod_deser_impl->mod_deser.ops->free =
        woden_wsdl10_soap_module_deserializer_free;
    mod_deser_impl->mod_deser.ops->super_objs =
        woden_wsdl10_soap_module_deserializer_super_objs;
    mod_deser_impl->mod_deser.ops->type =
        woden_wsdl10_soap_module_deserializer_type;

    mod_deser_impl->mod_deser.ops->unmarshall =
        woden_wsdl10_soap_module_deserializer_unmarshall;

    mod_deser_impl->methods = axis2_hash_make(env);
    if (!mod_deser_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(mod_deser_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_wsdl10_soap_module_deserializer_free);
    axis2_hash_set(mod_deser_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            woden_wsdl10_soap_module_deserializer_super_objs);
    axis2_hash_set(mod_deser_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_wsdl10_soap_module_deserializer_type);

    axis2_hash_set(mod_deser_impl->methods, "unmarshall",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_soap_module_deserializer_unmarshall);

    return &(mod_deser_impl->mod_deser);
}

AXIS2_EXTERN woden_wsdl10_soap_module_deserializer_t * AXIS2_CALL
woden_wsdl10_soap_module_deserializer_create(const axis2_env_t *env)
{
    woden_wsdl10_soap_module_deserializer_impl_t *mod_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    mod_deser_impl = (woden_wsdl10_soap_module_deserializer_impl_t *) create(env);

    mod_deser_impl->super = axis2_hash_make(env);
    if (!mod_deser_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(mod_deser_impl->super, "WODEN_WSDL10_SOAP_MODULE_DESERIALIZER",
            AXIS2_HASH_KEY_STRING, &(mod_deser_impl->mod_deser));

    return &(mod_deser_impl->mod_deser);
}

static axis2_status_t
woden_wsdl10_soap_module_deserializer_free_ops(
    void *mod_deser,
    const axis2_env_t *env)
{
    woden_wsdl10_soap_module_deserializer_impl_t *mod_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mod_deser_impl = INTF_TO_IMPL(mod_deser);

    if (mod_deser_impl->mod_deser.ext_deserializer.ops)
    {
        AXIS2_FREE(env->allocator, mod_deser_impl->mod_deser.
                ext_deserializer.ops);
        mod_deser_impl->mod_deser.ext_deserializer.ops =
            NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_wsdl10_soap_module_deserializer_free(
    void *mod_deser,
    const axis2_env_t *env)
{
    woden_wsdl10_soap_module_deserializer_impl_t *mod_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mod_deser_impl = INTF_TO_IMPL(mod_deser);

    if (mod_deser_impl->super)
    {
        axis2_hash_free(mod_deser_impl->super, env);
        mod_deser_impl->super = NULL;
    }

    if (mod_deser_impl->methods)
    {
        axis2_hash_free(mod_deser_impl->methods, env);
        mod_deser_impl->methods = NULL;
    }

    woden_wsdl10_soap_module_deserializer_free_ops(mod_deser, env);

    if ((&(mod_deser_impl->mod_deser))->ops)
    {
        AXIS2_FREE(env->allocator, (&(mod_deser_impl->mod_deser))->ops);
        (&(mod_deser_impl->mod_deser))->ops = NULL;
    }

    if (mod_deser_impl)
    {
        AXIS2_FREE(env->allocator, mod_deser_impl);
        mod_deser_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_wsdl10_soap_module_deserializer_super_objs(
    void *mod_deser,
    const axis2_env_t *env)
{
    woden_wsdl10_soap_module_deserializer_impl_t *mod_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    mod_deser_impl = INTF_TO_IMPL(mod_deser);

    return mod_deser_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_wsdl10_soap_module_deserializer_type(
    void *mod_deser,
    const axis2_env_t *env)
{
    woden_wsdl10_soap_module_deserializer_impl_t *mod_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mod_deser_impl = INTF_TO_IMPL(mod_deser);

    return mod_deser_impl->obj_type;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_soap_module_deserializer_resolve_methods(
    woden_wsdl10_soap_module_deserializer_t *mod_deser,
    const axis2_env_t *env,
    woden_wsdl10_soap_module_deserializer_t *mod_deser_impl,
    axis2_hash_t *methods)
{
    woden_wsdl10_soap_module_deserializer_impl_t *mod_deser_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    mod_deser_impl_l = INTF_TO_IMPL(mod_deser_impl);

    mod_deser->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    mod_deser->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    mod_deser->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    mod_deser->ops->unmarshall = axis2_hash_get(methods,
            "unmarshall", AXIS2_HASH_KEY_STRING);
    if (!mod_deser->ops->unmarshall && mod_deser_impl_l)
        mod_deser->ops->unmarshall =
            mod_deser_impl_l->mod_deser.ops->unmarshall;

    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_wsdl10_soap_module_deserializer_unmarshall(
    void *mod_deser,
    const axis2_env_t *env,
    axis2_char_t *parent_type,
    void *parent,
    axis2_qname_t *element_type,
    axiom_node_t *el_node,
    void *desc,
    woden_wsdl10_ext_registry_t *ext_reg)
{
    woden_wsdl10_soap_module_deserializer_impl_t *mod_deser_impl = NULL;
    axis2_hash_t *super = NULL;
    void *soap_mod = NULL;
    axis2_char_t *ref = NULL;
    axis2_char_t *req = NULL;
    axiom_element_t *el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_bool_t required = AXIS2_FALSE;
    axis2_qname_t *element_type_l = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL10_SOAP_MODULE_DESERIALIZER_SUPER_OBJS(mod_deser, env);
    mod_deser_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_WSDL10_SOAP_MODULE_DESERIALIZER", AXIS2_HASH_KEY_STRING));

    /*soap_mod = WODEN_WSDL10_EXT_REGISTRY_QUERY_EXT_ELEMENT_TYPE(ext_reg, env,
            parent_type, element_type);*/
    soap_mod = woden_wsdl10_soap_module_create(env);
    if (!soap_mod)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    soap_mod = woden_wsdl10_soap_module_to_ext_element(soap_mod, env);
    WODEN_EXT_ELEMENT_SET_EXT_TYPE(soap_mod, env, element_type);
    soap_mod = woden_wsdl10_soap_module_to_soap_module_element(soap_mod, env);
    WODEN_WSDL10_SOAP_MODULE_ELEMENT_SET_PARENT_ELEMENT(soap_mod, env, parent);

    el = AXIOM_NODE_GET_DATA_ELEMENT(el_node, env);

    ref = axiom_element_get_attribute_value_by_name(el, env,
            WODEN_WSDL10_ATTR_REF);
    if (ref)
    {
        axis2_uri_t *uri = axis2_uri_parse_string(env, ref);

        soap_mod = woden_wsdl10_soap_module_to_soap_module_element(soap_mod, env);
        WODEN_WSDL10_SOAP_MODULE_ELEMENT_SET_REF(soap_mod, env, uri);
    }
    req = axiom_element_get_attribute_value_by_name(el, env,
            WODEN_WSDL10_ATTR_REQUIRED);
    if (req && 0 == axis2_strcmp(WODEN_WSDL10_VALUE_TRUE, req))
        required = AXIS2_TRUE;
    else
        required = AXIS2_FALSE;

    soap_mod = woden_wsdl10_soap_module_to_ext_element(soap_mod, env);
    WODEN_EXT_ELEMENT_SET_REQUIRED(soap_mod, env, required);
    element_type_l = axis2_qname_create(env, "operation",
            "http://schemas.xmlsoap.org/wsdl/soap/", NULL);
    if (AXIS2_TRUE == axis2_qname_equals(element_type, env, element_type_l))
    {
        axis2_char_t *action_str = NULL;
        axis2_uri_t *soap_action = NULL;
        void *binding_op_exts = NULL;

        action_str = axiom_element_get_attribute_value_by_name(el, env,
                WODEN_WSDL10_ATTR_ACTION);
        soap_action = axis2_uri_parse_string(env, action_str);
        binding_op_exts = woden_wsdl10_soap_binding_op_exts_create(env);
        WODEN_WSDL10_SOAP_BINDING_OP_EXTS_SET_SOAP_ACTION(binding_op_exts,
                env, soap_action);
        if( soap_action != NULL)
        {
            axis2_uri_free(soap_action, env);
        }


        soap_mod = woden_wsdl10_soap_module_to_soap_module_element(soap_mod,
                env);
        WODEN_WSDL10_SOAP_MODULE_ELEMENT_SET_SOAP_BINDING_OP_EXTS(
            soap_mod, env, binding_op_exts);

    }
    element_type_l = axis2_qname_create(env, "address",
            "http://schemas.xmlsoap.org/wsdl/soap/", NULL);
    if (AXIS2_TRUE == axis2_qname_equals(element_type, env, element_type_l))
    {
        axis2_char_t *address_str = NULL;
        axis2_uri_t *soap_address = NULL;
        void *address_exts = NULL;

        address_str = axiom_element_get_attribute_value_by_name(el, env,
                WODEN_WSDL10_ATTR_LOCATION);
        soap_address = axis2_uri_parse_string(env, address_str);
        address_exts = woden_wsdl10_soap_address_exts_create(env);
        WODEN_WSDL10_SOAP_ADDRESS_EXTS_SET_SOAP_ADDRESS(address_exts,
                env, soap_address);
        axis2_uri_free(soap_address, env);

        soap_mod = woden_wsdl10_soap_module_to_soap_module_element(soap_mod,
                env);
        WODEN_WSDL10_SOAP_MODULE_ELEMENT_SET_SOAP_ADDRESS_EXTS(
            soap_mod, env, address_exts);

    }

    temp_el = axiom_util_get_first_child_element(el, env, el_node,
            &temp_el_node);

    while (temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_DOCUMENTATION);

        if (AXIS2_TRUE == axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(mod_deser, env, temp_el_node, desc);
            soap_mod = woden_wsdl10_soap_module_to_soap_module_element(soap_mod, env);
            WODEN_WSDL10_SOAP_MODULE_ELEMENT_ADD_DOCUMENTATION_ELEMENT(soap_mod, env,
                    documentation);
        }
        else
        {
            /* TODO Parse ext elements */
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }
    soap_mod = woden_wsdl10_soap_module_to_ext_element(soap_mod, env);
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

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    documentation = WODEN_WSDL10_DESC_ELEMENT_CREATE_DOCUMENTATION_ELEMENT(desc,
            env);

    documentation = woden_documentation_to_documentation_element(
                documentation, env);
    WODEN_DOCUMENTATION_ELEMENT_SET_CONTENT(documentation, env, doc_el_node);

    return documentation;
}

