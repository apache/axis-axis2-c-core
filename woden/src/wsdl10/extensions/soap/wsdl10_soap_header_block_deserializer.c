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

#include <woden_wsdl10_soap_header_block_deserializer.h>
#include <woden_wsdl10_soap_header_block.h>
#include <woden_wsdl10_soap_header_block_element.h>
#include <woden_wsdl10_desc.h>
#include <woden_wsdl10_desc_element.h>
#include <woden_documentation.h>
#include <woden_documentation_element.h>
#include "../../../util/woden_om_util.h"
#include "../../woden_wsdl10_constants.h"
#include "woden_wsdl10_soap_constants.h"
#include <axiom_element.h>
#include <axiom_util.h>
#include <woden_qname_util.h>
#include <woden_wsdl10_ext_deserializer.h>


typedef struct woden_wsdl10_soap_header_block_deserializer_impl
            woden_wsdl10_soap_header_block_deserializer_impl_t;

/**
 * @brief Soap Header Block Deserializer Struct Impl
 *   Axis2 Soap Header Block Deserializer
 */
struct woden_wsdl10_soap_header_block_deserializer_impl
{
    woden_wsdl10_soap_header_block_deserializer_t header_deser;
    woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
};

#define INTF_TO_IMPL(header_deser) \
    ((woden_wsdl10_soap_header_block_deserializer_impl_t *) header_deser)

axis2_status_t AXIS2_CALL
woden_wsdl10_soap_header_block_deserializer_free(
    void *header_deser,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_wsdl10_soap_header_block_deserializer_type(
    void *header_deser,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_wsdl10_soap_header_block_deserializer_super_objs(
    void *header_deser,
    const axis2_env_t *env);

void *AXIS2_CALL
woden_wsdl10_soap_header_block_deserializer_marshall(
    void *header_deser,
    const axis2_env_t *env,
    axis2_char_t *parent_type,
    void *parent,
    axis2_qname_t *ext_type,
    axiom_node_t *ext_el_node,
    void *desc,
    woden_wsdl10_ext_registry_t *ext_reg);

static void *
parse_documentation(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *doc_el_node,
    void *desc);

static woden_wsdl10_soap_header_block_deserializer_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_wsdl10_soap_header_block_deserializer_free_ops(
    void *header_deser,
    const axis2_env_t *env);

/************************Woden C Internal Methods******************************/

AXIS2_EXTERN woden_wsdl10_soap_header_block_deserializer_t * AXIS2_CALL
woden_wsdl10_soap_header_block_deserializer_to_ext_deserializer(
    void *header_deser,
    const axis2_env_t *env)
{
    woden_wsdl10_soap_header_block_deserializer_impl_t *header_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!header_deser)
    {
        header_deser_impl = (woden_wsdl10_soap_header_block_deserializer_impl_t *)
                create(env);
    }
    else
        header_deser_impl = (woden_wsdl10_soap_header_block_deserializer_impl_t *)
                header_deser;

    woden_wsdl10_soap_header_block_deserializer_free_ops(header_deser, env);

    header_deser_impl->header_deser.ext_deserializer.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_wsdl10_ext_deserializer_ops_t));
    woden_wsdl10_ext_deserializer_resolve_methods(&(header_deser_impl->header_deser.
            ext_deserializer), env, header_deser_impl->methods);
    return header_deser;
}

/************************End of Woden C Internal Methods***********************/
static woden_wsdl10_soap_header_block_deserializer_t *
create(const axis2_env_t *env)
{
    woden_wsdl10_soap_header_block_deserializer_impl_t *header_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    header_deser_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl10_soap_header_block_deserializer_impl_t));

    header_deser_impl->obj_type = WODEN_WSDL10_SOAP_HEADER_BLOCK_DESERIALIZER;
    header_deser_impl->super = NULL;
    header_deser_impl->methods = NULL;

    header_deser_impl->header_deser.ext_deserializer.ops = NULL;

    header_deser_impl->header_deser.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl10_soap_header_block_deserializer_ops_t));

    header_deser_impl->header_deser.ops->free =
        woden_wsdl10_soap_header_block_deserializer_free;
    header_deser_impl->header_deser.ops->super_objs =
        woden_wsdl10_soap_header_block_deserializer_super_objs;
    header_deser_impl->header_deser.ops->type =
        woden_wsdl10_soap_header_block_deserializer_type;

    header_deser_impl->header_deser.ops->marshall =
        woden_wsdl10_soap_header_block_deserializer_marshall;

    header_deser_impl->methods = axis2_hash_make(env);
    if (!header_deser_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(header_deser_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_wsdl10_soap_header_block_deserializer_free);
    axis2_hash_set(header_deser_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            woden_wsdl10_soap_header_block_deserializer_super_objs);
    axis2_hash_set(header_deser_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_wsdl10_soap_header_block_deserializer_type);

    axis2_hash_set(header_deser_impl->methods, "marshall",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_soap_header_block_deserializer_marshall);

    return &(header_deser_impl->header_deser);
}

AXIS2_EXTERN woden_wsdl10_soap_header_block_deserializer_t * AXIS2_CALL
woden_wsdl10_soap_header_block_deserializer_create(const axis2_env_t *env)
{
    woden_wsdl10_soap_header_block_deserializer_impl_t *header_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    header_deser_impl = (woden_wsdl10_soap_header_block_deserializer_impl_t *) create(env);

    header_deser_impl->super = axis2_hash_make(env);
    if (!header_deser_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(header_deser_impl->super, "WODEN_WSDL10_SOAP_HEADER_BLOCK_DESERIALIZER",
            AXIS2_HASH_KEY_STRING, &(header_deser_impl->header_deser));

    return &(header_deser_impl->header_deser);
}

static axis2_status_t
woden_wsdl10_soap_header_block_deserializer_free_ops(
    void *header_deser,
    const axis2_env_t *env)
{
    woden_wsdl10_soap_header_block_deserializer_impl_t *header_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    header_deser_impl = INTF_TO_IMPL(header_deser);

    if (header_deser_impl->header_deser.ext_deserializer.ops)
    {
        AXIS2_FREE(env->allocator, header_deser_impl->header_deser.
                ext_deserializer.ops);
        header_deser_impl->header_deser.ext_deserializer.ops =
            NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_wsdl10_soap_header_block_deserializer_free(
    void *header_deser,
    const axis2_env_t *env)
{
    woden_wsdl10_soap_header_block_deserializer_impl_t *header_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    header_deser_impl = INTF_TO_IMPL(header_deser);

    if (header_deser_impl->super)
    {
        axis2_hash_free(header_deser_impl->super, env);
        header_deser_impl->super = NULL;
    }

    if (header_deser_impl->methods)
    {
        axis2_hash_free(header_deser_impl->methods, env);
        header_deser_impl->methods = NULL;
    }

    woden_wsdl10_soap_header_block_deserializer_free_ops(header_deser, env);

    if ((&(header_deser_impl->header_deser))->ops)
    {
        AXIS2_FREE(env->allocator, (&(header_deser_impl->header_deser))->ops);
        (&(header_deser_impl->header_deser))->ops = NULL;
    }

    if (header_deser_impl)
    {
        AXIS2_FREE(env->allocator, header_deser_impl);
        header_deser_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_wsdl10_soap_header_block_deserializer_super_objs(
    void *header_deser,
    const axis2_env_t *env)
{
    woden_wsdl10_soap_header_block_deserializer_impl_t *header_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    header_deser_impl = INTF_TO_IMPL(header_deser);

    return header_deser_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_wsdl10_soap_header_block_deserializer_type(
    void *header_deser,
    const axis2_env_t *env)
{
    woden_wsdl10_soap_header_block_deserializer_impl_t *header_deser_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    header_deser_impl = INTF_TO_IMPL(header_deser);

    return header_deser_impl->obj_type;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_soap_header_block_deserializer_resolve_methods(
    woden_wsdl10_soap_header_block_deserializer_t *header_deser,
    const axis2_env_t *env,
    woden_wsdl10_soap_header_block_deserializer_t *header_deser_impl,
    axis2_hash_t *methods)
{
    woden_wsdl10_soap_header_block_deserializer_impl_t *header_deser_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    header_deser_impl_l = INTF_TO_IMPL(header_deser_impl);

    header_deser->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    header_deser->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    header_deser->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    header_deser->ops->marshall = axis2_hash_get(methods,
            "marshall", AXIS2_HASH_KEY_STRING);
    if (!header_deser->ops->marshall && header_deser_impl_l)
        header_deser->ops->marshall =
            header_deser_impl_l->header_deser.ops->marshall;

    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_wsdl10_soap_header_block_deserializer_marshall(
    void *header_deser,
    const axis2_env_t *env,
    axis2_char_t *parent_type,
    void *parent,
    axis2_qname_t *ext_type,
    axiom_node_t *ext_el_node,
    void *desc,
    woden_wsdl10_ext_registry_t *ext_reg)
{
    woden_wsdl10_soap_header_block_deserializer_impl_t *header_deser_impl = NULL;
    axis2_hash_t *super = NULL;
    void *soap_hdr = NULL;
    axis2_char_t *element_decl_qn = NULL;
    axis2_char_t *must_understand = NULL;
    axis2_char_t *req = NULL;
    axiom_element_t *ext_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_bool_t understood = AXIS2_FALSE;
    axis2_bool_t required = AXIS2_FALSE;
    void *types = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL10_SOAP_HEADER_BLOCK_DESERIALIZER_SUPER_OBJS(header_deser, env);
    header_deser_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_WSDL10_SOAP_HEADER_BLOCK_DESERIALIZER", AXIS2_HASH_KEY_STRING));

    soap_hdr = WODEN_WSDL10_EXT_REGISTRY_QUERY_EXT_ELEMENT_TYPE(ext_reg, env,
            parent_type, ext_type);

    soap_hdr = woden_wsdl10_soap_header_block_to_ext_element(soap_hdr, env);
    WODEN_EXT_ELEMENT_SET_EXT_TYPE(soap_hdr, env, ext_type);
    soap_hdr = woden_wsdl10_soap_header_block_to_soap_header_block_element(soap_hdr, env);
    WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_SET_PARENT_ELEMENT(soap_hdr, env, parent);

    element_decl_qn = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ext_el, env,
            WODEN_WSDL10_ATTR_ELEMENT);
    if (element_decl_qn)
    {
        axis2_qname_t *qname = NULL;
        axis2_hash_t *namespcs = NULL;

        desc = woden_wsdl10_desc_to_desc_element(desc, env);
        namespcs = WODEN_WSDL10_DESC_ELEMENT_GET_NAMESPACES(desc, env);
        qname = woden_om_util_get_qname(env, ext_el_node,
                element_decl_qn, namespcs);

        soap_hdr = woden_wsdl10_soap_header_block_to_soap_header_block_element(soap_hdr, env);
        WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_SET_ELEMENT_QNAME(soap_hdr, env, qname);
    }
    must_understand = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ext_el, env,
            WODEN_WSDL10_ATTR_MUSTUNDERSTAND);
    if (must_understand && 0 == AXIS2_STRCMP(WODEN_WSDL10_VALUE_TRUE, must_understand))
        understood = AXIS2_TRUE;
    else
        understood = AXIS2_FALSE;
    soap_hdr = woden_wsdl10_soap_header_block_to_soap_header_block_element(soap_hdr, env);
    WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_SET_MUST_UNDERSTAND(soap_hdr, env, understood);

    req = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ext_el, env,
            WODEN_WSDL10_ATTR_REQUIRED);
    if (req && 0 == AXIS2_STRCMP(WODEN_WSDL10_VALUE_TRUE, req))
        required = AXIS2_TRUE;
    else
        required = AXIS2_FALSE;
    soap_hdr = woden_wsdl10_soap_header_block_to_ext_element(soap_hdr, env);
    WODEN_EXT_ELEMENT_SET_REQUIRED(soap_hdr, env, required);

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    types = WODEN_WSDL10_DESC_ELEMENT_GET_TYPES_ELEMENT(desc, env);
    WODEN_WSDL10_SOAP_HEADER_BLOCK_SET_TYPES(soap_hdr, env, types);

    ext_el = AXIOM_NODE_GET_DATA_ELEMENT(ext_el_node, env);
    temp_el = axiom_util_get_first_child_element(ext_el, env, ext_el_node,
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

            documentation = parse_documentation(header_deser, env, temp_el_node, desc);
            soap_hdr = woden_wsdl10_soap_header_block_to_soap_header_block_element(soap_hdr, env);
            WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_ADD_DOCUMENTATION_ELEMENT(soap_hdr, env,
                    documentation);
        }
        else
        {
            /* TODO Parse ext elements */
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return soap_hdr;
}

static void *
parse_documentation(
    void *header_deser,
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


