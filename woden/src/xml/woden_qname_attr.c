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

#include <woden_qname_attr.h>
#include <woden_xml_attr.h>
#include <axiom_element.h>
#include <axiom_node.h>

typedef struct woden_qname_attr_impl woden_qname_attr_impl_t;

/**
 * @brief Qname Attribute Struct Impl
 *   Axis2 Qname Attribute
 */
struct woden_qname_attr_impl
{
    woden_qname_attr_t qname_attr;
    woden_xml_attr_t *xml_attr;
    woden_obj_types_t obj_type;
    axutil_hash_t *methods;
};

#define INTF_TO_IMPL(qname_attr) \
    ((woden_qname_attr_impl_t *) qname_attr)

axis2_status_t AXIS2_CALL
woden_qname_attr_free(
    void *qname_attr,
    const axutil_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_qname_attr_type(
    void *qname_attr,
    const axutil_env_t *env);

woden_xml_attr_t *AXIS2_CALL
woden_qname_attr_get_base_impl(
    void *qname_attr,
    const axutil_env_t *env);

axis2_qname_t *AXIS2_CALL
woden_qname_attr_get_qname(
    void *qname_attr,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_qname_attr_convert(
    void *qname_attr,
    const axutil_env_t *env,
    axiom_element_t *owner_el,
    axiom_node_t *owner_node,
    axis2_char_t *attr_value);

/*
 * TODO This constructor is not used for extension attributes, but may be useful if
 * parsing of native WSDL attributes is changed to use the XMLAttr interface.
 */
AXIS2_EXTERN woden_qname_attr_t * AXIS2_CALL
woden_qname_attr_create(
    const axutil_env_t *env,
    axiom_element_t *owner_el,
    axiom_node_t *owner_node,
    axis2_qname_t *attr_type,
    axis2_char_t *attr_value)
{
    woden_qname_attr_impl_t *qname_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    qname_attr_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_qname_attr_impl_t));

    qname_attr_impl->obj_type = WODEN_QNAME_ATTR;
    qname_attr_impl->xml_attr = NULL;
    qname_attr_impl->methods = NULL;
    qname_attr_impl->qname_attr.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_qname_attr_ops_t));

    qname_attr_impl->qname_attr.ops->free =
        woden_qname_attr_free;
    qname_attr_impl->qname_attr.ops->type =
        woden_qname_attr_type;
    qname_attr_impl->qname_attr.ops->get_base_impl =
        woden_qname_attr_get_base_impl;
    qname_attr_impl->qname_attr.ops->get_qname =
        woden_qname_attr_get_qname;
    qname_attr_impl->qname_attr.ops->convert =
        woden_qname_attr_convert;


    qname_attr_impl->methods = axutil_hash_make(env);
    if (!qname_attr_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(qname_attr_impl->methods, "free",
            AXIS2_HASH_KEY_STRING, woden_qname_attr_free);
    axutil_hash_set(qname_attr_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_qname_attr_type);
    axutil_hash_set(qname_attr_impl->methods, "get_qname",
            AXIS2_HASH_KEY_STRING, woden_qname_attr_get_qname);
    axutil_hash_set(qname_attr_impl->methods, "convert",
            AXIS2_HASH_KEY_STRING, woden_qname_attr_convert);

    qname_attr_impl->xml_attr = woden_xml_attr_create(env, owner_el,
            owner_node, attr_type, attr_value);

    return &(qname_attr_impl->qname_attr);
}

woden_obj_types_t AXIS2_CALL
woden_qname_attr_type(
    void *qname_attr,
    const axutil_env_t *env)
{
    woden_qname_attr_impl_t *qname_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    qname_attr_impl = INTF_TO_IMPL(qname_attr);

    return qname_attr_impl->obj_type;
}

axis2_status_t AXIS2_CALL
woden_qname_attr_free(
    void *qname_attr,
    const axutil_env_t *env)
{
    woden_qname_attr_impl_t *qname_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    qname_attr_impl = INTF_TO_IMPL(qname_attr);

    if (qname_attr_impl->methods)
    {
        axutil_hash_free(qname_attr_impl->methods, env);
        qname_attr_impl->methods = NULL;
    }

    if (qname_attr_impl->xml_attr)
    {
        WODEN_XML_ATTR_FREE(qname_attr_impl->xml_attr, env);
        qname_attr_impl->xml_attr = NULL;
    }

    if ((&(qname_attr_impl->qname_attr))->ops)
    {
        AXIS2_FREE(env->allocator, (&(qname_attr_impl->qname_attr))->ops);
        (&(qname_attr_impl->qname_attr))->ops = NULL;
    }

    if (qname_attr_impl)
    {
        AXIS2_FREE(env->allocator, qname_attr_impl);
        qname_attr_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

woden_xml_attr_t *AXIS2_CALL
woden_qname_attr_get_base_impl(
    void *qname_attr,
    const axutil_env_t *env)
{
    woden_qname_attr_impl_t *qname_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    qname_attr_impl = INTF_TO_IMPL(qname_attr);

    return qname_attr_impl->xml_attr;
}

axis2_status_t AXIS2_CALL
woden_qname_attr_resolve_methods(
    woden_qname_attr_t *qname_attr,
    const axutil_env_t *env,
    axutil_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);

    qname_attr->ops->free = axutil_hash_get(methods,
            "free", AXIS2_HASH_KEY_STRING);
    qname_attr->ops->to_qname_attr_free = axutil_hash_get(methods,
            "to_qname_attr_free", AXIS2_HASH_KEY_STRING);
    qname_attr->ops->type = axutil_hash_get(methods,
            "type", AXIS2_HASH_KEY_STRING);
    qname_attr->ops->get_qname = axutil_hash_get(methods,
            "get_qname", AXIS2_HASH_KEY_STRING);
    qname_attr->ops->convert = axutil_hash_get(methods,
            "convert", AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_qname_attr_get_qname(
    void *qname_attr,
    const axutil_env_t *env)
{
    woden_qname_attr_impl_t *qname_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    qname_attr_impl = INTF_TO_IMPL(qname_attr);
    return (axis2_qname_t *) WODEN_XML_ATTR_GET_CONTENT(
                qname_attr_impl->xml_attr, env);
}

void *AXIS2_CALL
woden_qname_attr_convert(
    void *qname_attr,
    const axutil_env_t *env,
    axiom_element_t *owner_el,
    axiom_node_t *owner_node,
    axis2_char_t *attr_value)
{
    woden_qname_attr_impl_t *qname_attr_impl = NULL;
    axis2_qname_t *qn = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    qname_attr_impl = INTF_TO_IMPL(qname_attr);

    if (attr_value)
    {
        /* TODO qn = axis2_dom_utils_get_qname(attr_value, owner_el, owner_node); */
    }

    if (!qn)
    {
        WODEN_XML_ATTR_SET_VALID(qname_attr_impl->xml_attr, env, AXIS2_FALSE);
        /* TODO handler error */
    }

    return qn;
}

