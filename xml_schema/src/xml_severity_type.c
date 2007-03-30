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

#include <xml_schema_severity_type.h>
#include <axis2_utils.h>

typedef struct xml_schema_severity_type_impl
            xml_schema_severity_type_impl_t;

/**
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension
 */
struct xml_schema_severity_type_impl
{
    xml_schema_severity_type_t severity_type;

    xml_schema_enum_t *schema_enum;

    axutil_array_list_t *members;

    axis2_hash_t *ht_super;

    xml_schema_types_t type;
};

#define AXIS2_INTF_TO_IMPL(severity_type) \
        ((xml_schema_severity_type_impl_t *) severity_type)

axis2_status_t AXIS2_CALL
xml_schema_severity_type_free(void *severity_type,
        const axis2_env_t *env);

xml_schema_enum_t *AXIS2_CALL
xml_schema_severity_type_get_base_impl(void *severity_type,
        const axis2_env_t *env);

axutil_array_list_t *AXIS2_CALL
xml_schema_severity_type_get_values(void *severity_type,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
xml_schema_severity_type_super_objs(void *severity_type,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_severity_type_get_type(void *severity_type,
        const axis2_env_t *env);

AXIS2_EXTERN xml_schema_severity_type_t * AXIS2_CALL
xml_schema_severity_type_create(const axis2_env_t *env,
        axis2_char_t *value)
{
    xml_schema_severity_type_impl_t *severity_type_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    severity_type_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_severity_type_impl_t));
    if (!severity_type_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    severity_type_impl->schema_enum = NULL;
    severity_type_impl->members = NULL;
    severity_type_impl->ht_super = NULL;
    severity_type_impl->type = XML_SCHEMA_SEVERITY_TYPE;

    severity_type_impl->severity_type.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_severity_type_ops_t));
    if (!severity_type_impl->severity_type.ops)
    {
        xml_schema_severity_type_free(&(severity_type_impl->severity_type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    severity_type_impl->severity_type.ops->free =
        xml_schema_severity_type_free;
    severity_type_impl->severity_type.ops->get_base_impl =
        xml_schema_severity_type_get_base_impl;
    severity_type_impl->severity_type.ops->get_values =
        xml_schema_severity_type_get_values;
    severity_type_impl->severity_type.ops->get_type =
        xml_schema_severity_type_get_type;
    severity_type_impl->severity_type.ops->super_objs =
        xml_schema_severity_type_super_objs;

    severity_type_impl->members = axutil_array_list_create(env, 0);
    if (!severity_type_impl->members)
    {
        xml_schema_severity_type_free(&(severity_type_impl->severity_type), env);
        return NULL;
    }
    axutil_array_list_add(severity_type_impl->members, env,
            axis2_strdup(env, XML_SCHEMA_CONST_ERROR));
    axutil_array_list_add(severity_type_impl->members, env,
            axis2_strdup(env, XML_SCHEMA_CONST_WARNING));

    severity_type_impl->schema_enum = xml_schema_enum_create(env, value);
    if (!severity_type_impl->schema_enum)
    {
        xml_schema_severity_type_free(&(severity_type_impl->severity_type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    severity_type_impl->ht_super = axis2_hash_make(env);
    if (!severity_type_impl->ht_super)
    {
        xml_schema_severity_type_free(&(severity_type_impl->severity_type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(severity_type_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_SEVERITY_TYPE"),
            AXIS2_HASH_KEY_STRING,  &(severity_type_impl->severity_type));

    axis2_hash_set(severity_type_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_ENUM"),
            AXIS2_HASH_KEY_STRING, severity_type_impl->schema_enum);

    status = xml_schema_enum_resolve_methods(
                &(severity_type_impl->severity_type.base), env, severity_type_impl->schema_enum,
                xml_schema_severity_type_super_objs,
                xml_schema_severity_type_get_type,
                xml_schema_severity_type_free);

    return &(severity_type_impl->severity_type);
}

axis2_status_t AXIS2_CALL
xml_schema_severity_type_free(void *severity_type,
        const axis2_env_t *env)
{
    xml_schema_severity_type_impl_t *severity_type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    severity_type_impl = AXIS2_INTF_TO_IMPL(severity_type);

    if (severity_type_impl->members)
    {
        int size = 0;
        int i    = 0;
        size = axutil_array_list_size(severity_type_impl->members, env);
        for (i = 0; i < size ; i++)
        {
            axis2_char_t *value = NULL;
            value = (axis2_char_t*) axutil_array_list_get(severity_type_impl->members, env, i);
            if (value)
            {
                AXIS2_FREE(env->allocator, value);
                value = NULL;
            }
        }
        axutil_array_list_free(severity_type_impl->members, env);
        severity_type_impl->members = NULL;
    }

    if (severity_type_impl->schema_enum)
    {
        XML_SCHEMA_ENUM_FREE(severity_type_impl->schema_enum, env);
        severity_type_impl->schema_enum = NULL;
    }

    if (severity_type_impl->severity_type.ops)
    {
        AXIS2_FREE(env->allocator, severity_type_impl->severity_type.ops);
        severity_type_impl->severity_type.ops = NULL;
    }
    if (severity_type_impl->severity_type.base.ops)
    {
        AXIS2_FREE(env->allocator, severity_type_impl->severity_type.base.ops);
        severity_type_impl->severity_type.base.ops = NULL;
    }
    if (severity_type_impl)
    {
        AXIS2_FREE(env->allocator, severity_type_impl);
        severity_type_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

xml_schema_enum_t *AXIS2_CALL
xml_schema_severity_type_get_base_impl(void *severity_type,
        const axis2_env_t *env)
{
    xml_schema_severity_type_impl_t *severity_type_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    severity_type_impl = AXIS2_INTF_TO_IMPL(severity_type);
    return severity_type_impl->schema_enum;
}

axutil_array_list_t* AXIS2_CALL
xml_schema_severity_type_get_values(void *severity_type,
        const axis2_env_t *env)
{
    /*
    xml_schema_severity_type_impl_t *severity_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    ht_super = XML_SCHEMA_SEREVITY_TYPE_SUPER_OBJS(severity_type, env);
    if( ht_super)
    {
        severity_impl = (xml_schema_severity_type_impl_t*)
            axis2_hash_get(ht_super, "XML_SCHEMA_SEVERITY_TYPE", AXIS2_HASH_KEY_STRING);
        return severity_impl->members;    
    }
    */
    return AXIS2_INTF_TO_IMPL(severity_type)->members;
}


axis2_hash_t *AXIS2_CALL
xml_schema_severity_type_super_objs(void *severity_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(severity_type)->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_severity_type_get_type(void *severity_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(severity_type)->type;
}
