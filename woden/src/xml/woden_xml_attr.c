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

#include <woden_xml_attr.h>
#include <axis2_uri.h>
#include <axis2_hash.h>
#include <axiom_element.h>
#include <axiom_node.h>

typedef struct woden_xml_attr_impl woden_xml_attr_impl_t;

/**
 * @brief Xml Attribute Struct Impl
 *   Axis2 Xml Attribute
 */
struct woden_xml_attr_impl
{
    woden_xml_attr_t xml_attr;
    axis2_qname_t *f_attr_type;
    axis2_generic_obj_t *f_content;
    axis2_char_t *f_external_form;
    axis2_bool_t f_valid;
};

#define INTF_TO_IMPL(xml_attr) ((woden_xml_attr_impl_t *) xml_attr)

axis2_status_t AXIS2_CALL
woden_xml_attr_free(
    void *xml_attr,
    const axis2_env_t *envv);

axis2_status_t AXIS2_CALL
woden_xml_attr_init(
    void *xml_attr,
    const axis2_env_t *env,
    axiom_element_t *owner_el,
    axiom_node_t *owner_node,
    axis2_qname_t *attr_type,
    axis2_char_t *attr_value);

axis2_qname_t *AXIS2_CALL
woden_xml_attr_get_attribute_type(
    void *xml_attr,
    const axis2_env_t *env);

axis2_generic_obj_t *AXIS2_CALL
woden_xml_attr_get_content(
    void *xml_attr,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
woden_xml_attr_to_external_form(
    void *xml_attr,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
woden_xml_attr_is_valid(
    void *xml_attr,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_xml_attr_set_valid(
    void *xml_attr,
    const axis2_env_t *env,
    axis2_bool_t validity);

AXIS2_EXTERN woden_xml_attr_t * AXIS2_CALL
woden_xml_attr_create(
    const axis2_env_t *env,
    axiom_element_t *owner_el,
    axiom_node_t *owner_node,
    axis2_qname_t *attr_type,
    axis2_char_t *attr_value)
{
    woden_xml_attr_impl_t *xml_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    xml_attr_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_xml_attr_impl_t));

    xml_attr_impl->f_attr_type = NULL;
    xml_attr_impl->f_content = NULL;
    xml_attr_impl->f_external_form = NULL;
    xml_attr_impl->f_valid = AXIS2_TRUE;

    xml_attr_impl->xml_attr.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_xml_attr_ops_t));

    woden_xml_attr_init(&(xml_attr_impl->xml_attr), env, owner_el,
            owner_node, attr_type, attr_value);

    xml_attr_impl->xml_attr.ops->free = woden_xml_attr_free;
    xml_attr_impl->xml_attr.ops->init = woden_xml_attr_init;
    xml_attr_impl->xml_attr.ops->get_attribute_type =
        woden_xml_attr_get_attribute_type;
    xml_attr_impl->xml_attr.ops->get_content = woden_xml_attr_get_content;
    xml_attr_impl->xml_attr.ops->to_external_form =
        woden_xml_attr_to_external_form;
    xml_attr_impl->xml_attr.ops->is_valid = woden_xml_attr_is_valid;
    xml_attr_impl->xml_attr.ops->set_valid = woden_xml_attr_set_valid;

    return &(xml_attr_impl->xml_attr);
}

axis2_status_t AXIS2_CALL
woden_xml_attr_free(
    void *xml_attr,
    const axis2_env_t *env)
{
    woden_xml_attr_impl_t *xml_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    xml_attr_impl = INTF_TO_IMPL(xml_attr);

    if (xml_attr_impl->f_attr_type)
    {
        axis2_qname_free(xml_attr_impl->f_attr_type, env);
        xml_attr_impl->f_attr_type = NULL;
    }

    if (xml_attr_impl->f_external_form)
    {
        AXIS2_FREE(env->allocator, xml_attr_impl->f_external_form);
        xml_attr_impl->f_external_form = NULL;
    }

    if ((&(xml_attr_impl->xml_attr))->ops)
    {
        AXIS2_FREE(env->allocator, (&(xml_attr_impl->xml_attr))->ops);
        (&(xml_attr_impl->xml_attr))->ops = NULL;
    }

    if (xml_attr_impl)
    {
        AXIS2_FREE(env->allocator, xml_attr_impl);
        xml_attr_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_xml_attr_resolve_methods(
    woden_xml_attr_t *xml_attr,
    const axis2_env_t *env,
    axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);

    xml_attr->ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_xml_attr_ops_t));
    xml_attr->ops->free = axis2_hash_get(methods, "free", AXIS2_HASH_KEY_STRING);
    xml_attr->ops->type = axis2_hash_get(methods, "type", AXIS2_HASH_KEY_STRING);
    xml_attr->ops->init = axis2_hash_get(methods,
            "init", AXIS2_HASH_KEY_STRING);
    xml_attr->ops->get_attribute_type = axis2_hash_get(methods,
            "get_attribute_type", AXIS2_HASH_KEY_STRING);
    xml_attr->ops->get_content = axis2_hash_get(methods,
            "get_content", AXIS2_HASH_KEY_STRING);
    xml_attr->ops->to_external_form = axis2_hash_get(methods,
            "to_external_form", AXIS2_HASH_KEY_STRING);
    xml_attr->ops->is_valid = axis2_hash_get(methods,
            "is_valid", AXIS2_HASH_KEY_STRING);
    xml_attr->ops->convert = axis2_hash_get(methods,
            "convert", AXIS2_HASH_KEY_STRING);
    xml_attr->ops->set_valid = axis2_hash_get(methods,
            "set_valid", AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_xml_attr_init(
    void *xml_attr,
    const axis2_env_t *env,
    axiom_element_t *owner_el,
    axiom_node_t *owner_node,
    axis2_qname_t *attr_type,
    axis2_char_t *attr_value)
{
    woden_xml_attr_impl_t *xml_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, owner_el, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, owner_node, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attr_type, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attr_value, AXIS2_FAILURE);
    xml_attr_impl = INTF_TO_IMPL(xml_attr);

    if (xml_attr_impl->f_attr_type)
    {
        axis2_qname_free(xml_attr_impl->f_attr_type, env);
        xml_attr_impl->f_attr_type = NULL;
    }
    xml_attr_impl->f_attr_type = axis2_qname_clone(attr_type, env);
    if (xml_attr_impl->f_attr_type)
    {
        axis2_qname_free(xml_attr_impl->f_attr_type, env);
        xml_attr_impl->f_attr_type = NULL;
    }
    xml_attr_impl->f_external_form = axis2_strdup(attr_value, env);
    xml_attr_impl->f_content = xml_attr_impl->xml_attr.ops->
            convert(&(xml_attr_impl->xml_attr), env, owner_el, owner_node,
                    attr_value);
    if (!xml_attr_impl->f_content)
        xml_attr_impl->f_valid = AXIS2_FALSE;
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_xml_attr_get_attribute_type(
    void *xml_attr,
    const axis2_env_t *env)
{
    woden_xml_attr_impl_t *xml_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    xml_attr_impl = INTF_TO_IMPL(xml_attr);

    return xml_attr_impl->f_attr_type;
}

axis2_generic_obj_t *AXIS2_CALL
woden_xml_attr_get_content(
    void *xml_attr,
    const axis2_env_t *env)
{
    woden_xml_attr_impl_t *xml_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    xml_attr_impl = INTF_TO_IMPL(xml_attr);
    return xml_attr_impl->f_content;
}

axis2_char_t *AXIS2_CALL
woden_xml_attr_to_external_form(
    void *xml_attr,
    const axis2_env_t *env)
{
    woden_xml_attr_impl_t *xml_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    xml_attr_impl = INTF_TO_IMPL(xml_attr);
    return xml_attr_impl->f_external_form;
}

axis2_bool_t AXIS2_CALL
woden_xml_attr_is_valid(
    void *xml_attr,
    const axis2_env_t *env)
{
    woden_xml_attr_impl_t *xml_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    xml_attr_impl = INTF_TO_IMPL(xml_attr);
    return xml_attr_impl->f_valid;
}

axis2_status_t AXIS2_CALL
woden_xml_attr_set_valid(
    void *xml_attr,
    const axis2_env_t *env,
    axis2_bool_t validity)
{
    woden_xml_attr_impl_t *xml_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    xml_attr_impl = INTF_TO_IMPL(xml_attr);
    xml_attr_impl->f_valid = validity;
    return AXIS2_SUCCESS;
}

