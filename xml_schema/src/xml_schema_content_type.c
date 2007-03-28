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
#include <xml_schema_defines.h>
#include <xml_schema_content_type.h>
#include <axis2_utils.h>

#include <axis2_string.h>

typedef struct xml_schema_content_type_impl
            xml_schema_content_type_impl_t;

/**
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension
 */
struct xml_schema_content_type_impl
{
    xml_schema_content_type_t content_type;

    xml_schema_enum_t *schema_enum;

    xml_schema_types_t obj_type;

    axis2_array_list_t *members;

    axis2_hash_t *ht_super;
};

#define AXIS2_INTF_TO_IMPL(content_type) \
        ((xml_schema_content_type_impl_t *) content_type)

axis2_status_t AXIS2_CALL
xml_schema_content_type_free(void *content_type,
        const axis2_env_t *env);

xml_schema_enum_t *AXIS2_CALL
xml_schema_content_type_get_base_impl(void *content_type,
        const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
xml_schema_content_type_get_values(void *content_type,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_content_type_get_type(void *content_type,
        const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL
xml_schema_content_type_super_objs(void *content_type,
        const axis2_env_t *env);




AXIS2_EXTERN xml_schema_content_type_t * AXIS2_CALL
xml_schema_content_type_create(const axis2_env_t *env,
        axis2_char_t *value)
{
    xml_schema_content_type_impl_t *content_type_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    content_type_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_content_type_impl_t));
    if (!content_type_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    content_type_impl->schema_enum = NULL;
    content_type_impl->members = NULL;
    content_type_impl->obj_type = XML_SCHEMA_CONTENT_TYPE;
    content_type_impl->ht_super = NULL;

    content_type_impl->content_type.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_content_type_ops_t));
    if (!content_type_impl->content_type.ops)
    {
        xml_schema_content_type_free(&(content_type_impl->content_type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    content_type_impl->content_type.ops->free =
        xml_schema_content_type_free;
    content_type_impl->content_type.ops->get_base_impl =
        xml_schema_content_type_get_base_impl;
    content_type_impl->content_type.ops->get_values =
        xml_schema_content_type_get_values;
    content_type_impl->content_type.ops->get_type =
        xml_schema_content_type_get_type;
    content_type_impl->content_type.ops->super_objs =
        xml_schema_content_type_super_objs;

    content_type_impl->members = axis2_array_list_create(env, 0);
    if (!content_type_impl->members)
    {
        xml_schema_content_type_free(&(content_type_impl->content_type), env);
        return NULL;
    }

    axis2_array_list_add(content_type_impl->members, env,
            axis2_strdup(env, XML_SCHEMA_CONST_ELEMENT_ONLY));
    axis2_array_list_add(content_type_impl->members, env,
            axis2_strdup(env, XML_SCHEMA_CONST_EMPTY));

    axis2_array_list_add(content_type_impl->members, env,
            axis2_strdup(env, XML_SCHEMA_CONST_MIXED));

    axis2_array_list_add(content_type_impl->members, env,
            axis2_strdup(env, XML_SCHEMA_CONST_TEXT_ONLY));

    content_type_impl->schema_enum = xml_schema_enum_create(env, value);
    if (!content_type_impl->schema_enum)
    {
        xml_schema_content_type_free(&(content_type_impl->content_type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    content_type_impl->ht_super = axis2_hash_make(env);
    if (!content_type_impl->ht_super)
    {
        xml_schema_content_type_free(&(content_type_impl->content_type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(content_type_impl->ht_super, axis2_strdup("XML_SCHEMA_CONTENT_TYPE", env),
            AXIS2_HASH_KEY_STRING, &(content_type_impl->content_type));
    axis2_hash_set(content_type_impl->ht_super, axis2_strdup("XML_SCHEMA_ENUM", env), AXIS2_HASH_KEY_STRING,
            content_type_impl->schema_enum);

    status = xml_schema_enum_resolve_methods(
                &(content_type_impl->content_type.base), env, content_type_impl->schema_enum,
                xml_schema_content_type_super_objs,
                xml_schema_content_type_get_type,
                xml_schema_content_type_free);

    return &(content_type_impl->content_type);
}

axis2_status_t AXIS2_CALL
xml_schema_content_type_free(void *content_type,
        const axis2_env_t *env)
{
    xml_schema_content_type_impl_t *content_type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_type_impl = AXIS2_INTF_TO_IMPL(content_type);

    if (content_type_impl->members)
    {
        int size = 0;
        int i    = 0;
        size = axis2_array_list_size(content_type_impl->members, env);
        for (i = 0; i < size ; i++)
        {
            axis2_char_t *value = NULL;
            value = (axis2_char_t*) axis2_array_list_get(content_type_impl->members, env, i);
            if (value)
            {
                AXIS2_FREE(env->allocator, value);
                value = NULL;
            }
        }
        axis2_array_list_free(content_type_impl->members, env);
        content_type_impl->members = NULL;
    }

    if (content_type_impl->schema_enum)
    {
        XML_SCHEMA_ENUM_FREE(content_type_impl->schema_enum, env);
        content_type_impl->schema_enum = NULL;
    }

    if ((&(content_type_impl->content_type))->ops)
    {
        AXIS2_FREE(env->allocator, (&(content_type_impl->content_type))->ops);
        (&(content_type_impl->content_type))->ops = NULL;
    }

    if (content_type_impl)
    {
        AXIS2_FREE(env->allocator, content_type_impl);
        content_type_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

xml_schema_enum_t *AXIS2_CALL
xml_schema_content_type_get_base_impl(void *content_type,
        const axis2_env_t *env)
{
    xml_schema_content_type_impl_t *content_type_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    content_type_impl = AXIS2_INTF_TO_IMPL(content_type);
    return content_type_impl->schema_enum;
}

axis2_array_list_t* AXIS2_CALL
xml_schema_content_type_get_values(void *content_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(content_type)->members;
}

xml_schema_types_t AXIS2_CALL
xml_schema_content_type_get_type(void *content_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(content_type)->obj_type;
}

axis2_hash_t* AXIS2_CALL
xml_schema_content_type_super_objs(void *content_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(content_type)->ht_super;
}
