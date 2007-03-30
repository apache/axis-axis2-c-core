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

#include <xml_schema_complex_content.h>
#include <axis2_utils.h>

typedef struct xml_schema_complex_content_impl
            xml_schema_complex_content_impl_t;

/**
 * @brief  xml_schema_complex_content impl
 *   Axis2 Other Extension
 */
struct xml_schema_complex_content_impl
{
    xml_schema_complex_content_t complex_content;

    xml_schema_annotated_t *annotated;

    axis2_hash_t *methods;

    void *content;

    xml_schema_types_t obj_type;

    axis2_hash_t *ht_super;

    axis2_bool_t mixed;
};

#define AXIS2_INTF_TO_IMPL(complex_content) \
        ((xml_schema_complex_content_impl_t *) complex_content)

axis2_status_t AXIS2_CALL
xml_schema_complex_content_free(
    void *complex_content,
    const axutil_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_complex_content_get_base_impl(
    void *complex_content,
    const axutil_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_complex_content_get_type(
    void *complex_content,
    const axutil_env_t *env);

axis2_hash_t* AXIS2_CALL
xml_schema_complex_content_super_objs(
    void *complex_content,
    const axutil_env_t *env);


void* AXIS2_CALL
xml_schema_complex_content_get_content(
    void *complex_content,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_complex_content_set_content(
    void *complex_content,
    const axutil_env_t *env,
    void* content);

axis2_char_t* AXIS2_CALL
xml_schema_complex_content_to_string(
    void *complex_content,
    const axutil_env_t *env,
    axis2_char_t *prefix,
    int tab);

axis2_bool_t AXIS2_CALL
xml_schema_complex_content_is_mixed(
    void *complex_content,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_complex_content_set_mixed(
    void *complex_content,
    const axutil_env_t *env,
    axis2_bool_t mixed);


AXIS2_EXTERN xml_schema_complex_content_t * AXIS2_CALL
xml_schema_complex_content_create(const axutil_env_t *env)
{
    xml_schema_complex_content_impl_t *complex_content_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    complex_content_impl = (xml_schema_complex_content_impl_t*)
            AXIS2_MALLOC(env->allocator, sizeof(xml_schema_complex_content_impl_t));

    if (!complex_content_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    complex_content_impl->annotated = NULL;
    complex_content_impl->methods = NULL;
    complex_content_impl->content = NULL;
    complex_content_impl->obj_type = XML_SCHEMA_COMPLEX_CONTENT;
    complex_content_impl->ht_super = NULL;
    complex_content_impl->mixed = AXIS2_FALSE;
    complex_content_impl->complex_content.base.ops = NULL;
    complex_content_impl->complex_content.ops = NULL;

    complex_content_impl->complex_content.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_complex_content_ops_t));
    if (!complex_content_impl->complex_content.ops)
    {
        xml_schema_complex_content_free(&(complex_content_impl->complex_content), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    complex_content_impl->complex_content.ops->free =
        xml_schema_complex_content_free;
    complex_content_impl->complex_content.ops->get_base_impl =
        xml_schema_complex_content_get_base_impl;
    complex_content_impl->complex_content.ops->get_type =
        xml_schema_complex_content_get_type;
    complex_content_impl->complex_content.ops->super_objs =
        xml_schema_complex_content_super_objs;
    complex_content_impl->complex_content.ops->set_content =
        xml_schema_complex_content_set_content;
    complex_content_impl->complex_content.ops->get_content =
        xml_schema_complex_content_get_content;
    complex_content_impl->complex_content.ops->to_string =
        xml_schema_complex_content_to_string;
    complex_content_impl->complex_content.ops->set_mixed =
        xml_schema_complex_content_set_mixed;

    complex_content_impl->methods = axis2_hash_make(env);
    complex_content_impl->ht_super = axis2_hash_make(env);

    if (!complex_content_impl->methods || !complex_content_impl->ht_super)
    {
        xml_schema_complex_content_free(&(complex_content_impl->complex_content), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(complex_content_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            xml_schema_complex_content_free);
    axis2_hash_set(complex_content_impl->methods, "get_content",
            AXIS2_HASH_KEY_STRING, xml_schema_complex_content_get_content);
    axis2_hash_set(complex_content_impl->methods, "set_content",
            AXIS2_HASH_KEY_STRING, xml_schema_complex_content_set_content);
    axis2_hash_set(complex_content_impl->methods, "get_type",
            AXIS2_HASH_KEY_STRING, xml_schema_complex_content_get_type);
    axis2_hash_set(complex_content_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, xml_schema_complex_content_super_objs);
    axis2_hash_set(complex_content_impl->methods, "to_string",
            AXIS2_HASH_KEY_STRING, xml_schema_complex_content_to_string);

    complex_content_impl->annotated = xml_schema_annotated_create(env);
    if (!complex_content_impl->annotated)
    {
        xml_schema_complex_content_free(&(complex_content_impl->complex_content), env);
        return NULL;
    }
    axis2_hash_set(complex_content_impl->ht_super,
            "XML_SCHEMA_COMPLEX_CONTENT", AXIS2_HASH_KEY_STRING,
            &(complex_content_impl->complex_content));
    axis2_hash_set(complex_content_impl->ht_super,
            "XML_SCHEMA_ANNOTATED", AXIS2_HASH_KEY_STRING,
            (complex_content_impl->annotated));
    axis2_hash_set(complex_content_impl->ht_super,
            "XML_SCHEMA_OBJ", AXIS2_HASH_KEY_STRING,
            XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(complex_content_impl->annotated, env));


    status = xml_schema_annotated_resolve_methods(
                &(complex_content_impl->complex_content.base),
                env, complex_content_impl->annotated,
                xml_schema_complex_content_super_objs,
                xml_schema_complex_content_get_type,
                xml_schema_complex_content_free);

    return &(complex_content_impl->complex_content);
}

axis2_status_t AXIS2_CALL
xml_schema_complex_content_free(void *complex_content,
        const axutil_env_t *env)
{
    xml_schema_complex_content_impl_t *complex_content_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    complex_content_impl = AXIS2_INTF_TO_IMPL(complex_content);

    if (complex_content_impl->content)
    {
        /** TODO */
    }
    if (complex_content_impl->ht_super)
    {
        axis2_hash_free(complex_content_impl->ht_super, env);
        complex_content_impl->ht_super = NULL;
    }
    if (complex_content_impl->methods)
    {
        axis2_hash_free(complex_content_impl->methods, env);
        complex_content_impl->methods = NULL;
    }
    if (complex_content_impl->annotated)
    {
        XML_SCHEMA_ANNOTATED_FREE(complex_content_impl->annotated, env);
        complex_content_impl->annotated = NULL;
    }
    if (complex_content_impl->complex_content.base.ops)
    {
        AXIS2_FREE(env->allocator, complex_content_impl->complex_content.base.ops);
        complex_content_impl->complex_content.base.ops = NULL;
    }
    if (complex_content_impl->complex_content.ops)
    {
        AXIS2_FREE(env->allocator, complex_content_impl->complex_content.ops);
        complex_content_impl->complex_content.ops = NULL;
    }

    AXIS2_FREE(env->allocator, complex_content_impl);
    complex_content_impl = NULL;
    return AXIS2_SUCCESS;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_complex_content_get_base_impl(
    void *complex_content,
    const axutil_env_t *env)
{
    xml_schema_complex_content_impl_t *complex_content_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    complex_content_impl = AXIS2_INTF_TO_IMPL(complex_content);
    return complex_content_impl->annotated;
}

void* AXIS2_CALL
xml_schema_complex_content_get_content(
    void *complex_content,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(complex_content)->content;
}

axis2_status_t AXIS2_CALL
xml_schema_complex_content_set_content(
    void *complex_content,
    const axutil_env_t *env,
    void *content)
{
    xml_schema_complex_content_impl_t *complex_content_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, content, AXIS2_FAILURE);

    complex_content_impl = AXIS2_INTF_TO_IMPL(complex_content);

    if (complex_content_impl->content)
    {
        /** TODO */

    }
    complex_content_impl->content = content;
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
xml_schema_complex_content_to_string(
    void *complex_content,
    const axutil_env_t *env,
    axis2_char_t *prefix,
    int tab)
{
    axis2_char_t *xml = NULL;
    axis2_char_t *temp_xml = NULL;
    axis2_char_t *temp_prefix = NULL;
    int i = 0;

    AXIS2_ENV_CHECK(env, NULL);
    for (i = 0; i < tab; i++)
    {
        if (xml == NULL)
        {
            xml = axis2_strdup(env, "\t");
        }
        else
        {
            temp_xml = xml;
            xml = axis2_stracat(env, xml, "\t");
            if (temp_xml)
                AXIS2_FREE(env->allocator, temp_xml);
            temp_xml = NULL;
        }
    }

    temp_prefix = prefix;

    if (prefix && axis2_strcmp(prefix, "") != 0 && strchr(prefix, ':') == NULL)
    {
        temp_prefix = axis2_stracat(env, prefix, ":");
    }

    temp_xml = axis2_stracat(env, "<", prefix);
    xml = axis2_stracat(env, temp_xml, "complexContent>\n");
    if (temp_xml)
    {
        AXIS2_FREE(env->allocator, temp_xml);
        temp_xml = NULL;
    }

    /** TODO implement swith to get string of content */


    return xml;
}

axis2_bool_t AXIS2_CALL
xml_schema_complex_content_is_mixed(
    void *complex_content,
    const axutil_env_t *env)
{
    xml_schema_complex_content_impl_t *complex_content_impl = NULL;
    complex_content_impl = AXIS2_INTF_TO_IMPL(complex_content);
    return complex_content_impl->mixed;
}

axis2_status_t AXIS2_CALL
xml_schema_complex_content_set_mixed(
    void *complex_content,
    const axutil_env_t *env,
    axis2_bool_t mixed)
{
    xml_schema_complex_content_impl_t *complex_content_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    complex_content_impl = AXIS2_INTF_TO_IMPL(complex_content);
    complex_content_impl->mixed = mixed;
    return AXIS2_SUCCESS;
}

xml_schema_types_t AXIS2_CALL
xml_schema_complex_content_get_type(
    void *complex_content,
    const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(complex_content)->obj_type;
}

axis2_hash_t* AXIS2_CALL
xml_schema_complex_content_super_objs(
    void *complex_content,
    const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(complex_content)->ht_super;
}

