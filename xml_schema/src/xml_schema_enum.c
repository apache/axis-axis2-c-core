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

#include <xml_schema_enum.h>
#include <axis2_hash.h>
#include <axis2_utils.h>

typedef struct xml_schema_enum_impl
            xml_schema_enum_impl_t;

/**
 * @brief Xml Schema Enum Impl
 *   Axis2 Xml Schema Enum
 */
struct xml_schema_enum_impl
{
    xml_schema_enum_t schema_enum;

    xml_schema_types_t obj_type;

    axis2_hash_t *ht_super;

    axutil_array_list_t *values;

    axis2_char_t *value;
};

#define AXIS2_INTF_TO_IMPL(schema_enum) \
        ((xml_schema_enum_impl_t *) schema_enum)

axis2_status_t AXIS2_CALL
xml_schema_enum_free(
    void *schema_enum,
    const axutil_env_t *env);

axis2_hash_t *AXIS2_CALL
xml_schema_enum_super_objs(
    void *schema_enum,
    const axutil_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_enum_get_type(
    void *schema_enum,
    const axutil_env_t *env);

axis2_char_t * AXIS2_CALL
xml_schema_enum_get_value(
    void *schema_enum,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_enum_set_value(
    void *schema_enum,
    const axutil_env_t *env,
    const axis2_char_t *value);

axis2_bool_t AXIS2_CALL
xml_schema_enum_equals(
    void *schema_enum,
    const axutil_env_t *env,
    void *obj);

axutil_array_list_t *AXIS2_CALL
xml_schema_enum_get_values(
    void *schema_enum,
    const axutil_env_t *env);

/************************** function impl ************************************/

AXIS2_EXTERN xml_schema_enum_t * AXIS2_CALL
xml_schema_enum_create(const axutil_env_t *env,
        const axis2_char_t *value)
{
    xml_schema_enum_impl_t *schema_enum_impl = NULL;

    schema_enum_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_enum_impl_t));
    if (!schema_enum_impl)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    schema_enum_impl->obj_type = XML_SCHEMA_ENUM;
    schema_enum_impl->ht_super = NULL;
    schema_enum_impl->values = NULL;
    schema_enum_impl->value = NULL;
    schema_enum_impl->schema_enum.ops = NULL;

    schema_enum_impl->schema_enum.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_enum_ops_t));

    if (!(schema_enum_impl->schema_enum.ops))
    {
        AXIS2_FREE(env->allocator, schema_enum_impl);
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    schema_enum_impl->schema_enum.ops->free =
        xml_schema_enum_free;

    schema_enum_impl->schema_enum.ops->super_objs =
        xml_schema_enum_super_objs;

    schema_enum_impl->schema_enum.ops->get_type =
        xml_schema_enum_get_type;

    schema_enum_impl->schema_enum.ops->get_value =
        xml_schema_enum_get_value;

    schema_enum_impl->schema_enum.ops->set_value =
        xml_schema_enum_set_value;

    schema_enum_impl->schema_enum.ops->equals =
        xml_schema_enum_equals;

    schema_enum_impl->schema_enum.ops->get_values =
        xml_schema_enum_get_values;

    schema_enum_impl->ht_super = axis2_hash_make(env);

    if (!schema_enum_impl->ht_super)
    {
        xml_schema_enum_free(&(schema_enum_impl->schema_enum), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(schema_enum_impl->ht_super, "XML_SCHEMA_ENUM",
            AXIS2_HASH_KEY_STRING, &(schema_enum_impl->schema_enum));

    xml_schema_enum_set_value(&(schema_enum_impl->schema_enum),
            env, value);

    return &(schema_enum_impl->schema_enum);
}

axis2_status_t AXIS2_CALL
xml_schema_enum_free(void *schema_enum,
        const axutil_env_t *env)
{
    xml_schema_enum_impl_t *schema_enum_impl = NULL;

    schema_enum_impl = AXIS2_INTF_TO_IMPL(schema_enum);

    if (schema_enum_impl->value)
    {
        AXIS2_FREE(env->allocator, schema_enum_impl->value);
        schema_enum_impl->value = NULL;
    }

    if (schema_enum_impl->values)
    {
        axutil_array_list_free(schema_enum_impl->values, env);
        schema_enum_impl->values = NULL;
    }

    if (schema_enum_impl->ht_super)
    {
        axis2_hash_free(schema_enum_impl->ht_super, env);
        schema_enum_impl->ht_super = NULL;
    }

    if (schema_enum_impl->schema_enum.ops)
    {
        AXIS2_FREE(env->allocator, schema_enum_impl->schema_enum.ops);
        schema_enum_impl->schema_enum.ops = NULL;
    }

    if (schema_enum_impl)
    {
        AXIS2_FREE(env->allocator, (schema_enum_impl));
        schema_enum_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
xml_schema_enum_super_objs(
    void *schema_enum,
    const axutil_env_t *env)
{
    xml_schema_enum_impl_t *schema_enum_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    schema_enum_impl = AXIS2_INTF_TO_IMPL(schema_enum);
    return schema_enum_impl->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_enum_get_type(
    void *schema_enum,
    const axutil_env_t *env)
{
    xml_schema_enum_impl_t *schema_enum_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_enum_impl = AXIS2_INTF_TO_IMPL(schema_enum);
    return schema_enum_impl->obj_type;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_enum_resolve_methods(
    xml_schema_enum_t *schema_enum,
    const axutil_env_t *env,
    xml_schema_enum_t *schema_enum_impl,
    XML_SCHEMA_SUPER_OBJS_FN super_objs,
    XML_SCHEMA_GET_TYPE_FN get_type,
    XML_SCHEMA_FREE_FN free_fn)

{
    xml_schema_enum_impl_t *this_sch_enum_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schema_enum_impl, AXIS2_FAILURE);

    this_sch_enum_impl = (xml_schema_enum_impl_t*) schema_enum_impl;

    schema_enum->ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_enum_ops_t));
    if (!schema_enum->ops)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    schema_enum->ops->free = free_fn;

    schema_enum->ops->super_objs = super_objs;
    schema_enum->ops->get_type = get_type;

    schema_enum->ops->get_value =
        this_sch_enum_impl->schema_enum.ops->get_value;

    schema_enum->ops->set_value =
        this_sch_enum_impl->schema_enum.ops->set_value;

    schema_enum->ops->equals =
        this_sch_enum_impl->schema_enum.ops->equals;

    schema_enum->ops->get_values =
        this_sch_enum_impl->schema_enum.ops->get_values;
    return AXIS2_SUCCESS;
}


axis2_char_t * AXIS2_CALL
xml_schema_enum_get_value(void *schema_enum,
        const axutil_env_t *env)
{
    xml_schema_enum_impl_t *schema_enum_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    ht_super = XML_SCHEMA_ENUM_SUPER_OBJS(schema_enum, env);
    schema_enum_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super,
            "XML_SCHEMA_ENUM", AXIS2_HASH_KEY_STRING));

    return schema_enum_impl->value;
}

axis2_status_t AXIS2_CALL
xml_schema_enum_set_value(void *schema_enum,
        const axutil_env_t *env,
        const axis2_char_t *value)
{
    xml_schema_enum_impl_t *schema_enum_impl = NULL;
    axis2_hash_t *super = NULL;
    /** TODO fix this method to match the list of values
        in sub class with 'value' */
    super = XML_SCHEMA_ENUM_SUPER_OBJS(schema_enum, env);
    schema_enum_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super,
            "XML_SCHEMA_ENUM", AXIS2_HASH_KEY_STRING));

    if (schema_enum_impl->value)
    {
        AXIS2_FREE(env->allocator, (schema_enum_impl->value));
        schema_enum_impl->value = NULL;
    }
    schema_enum_impl->value = axis2_strdup(env, value);
    if (!schema_enum_impl->value)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
xml_schema_enum_equals(void *schema_enum,
        const axutil_env_t *env,
        void *obj)
{
    xml_schema_enum_impl_t *schema_enum_impl = NULL;
    axis2_hash_t *super = NULL;

    super = XML_SCHEMA_ENUM_SUPER_OBJS(schema_enum, env);
    schema_enum_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super,
            "XML_SCHEMA_ENUM", AXIS2_HASH_KEY_STRING));


    /** TODO */
    return AXIS2_TRUE;
}

axutil_array_list_t *AXIS2_CALL
xml_schema_enum_get_values(void *schema_enum,
        const axutil_env_t *env)
{
    xml_schema_enum_impl_t *schema_enum_impl = NULL;
    axis2_hash_t *super = NULL;

    super = XML_SCHEMA_ENUM_SUPER_OBJS(schema_enum, env);
    schema_enum_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super,
            "XML_SCHEMA_ENUM", AXIS2_HASH_KEY_STRING));

    return schema_enum_impl->values;
}

AXIS2_EXTERN int AXIS2_CALL
xml_schema_enum_index(axis2_char_t *value,
        const axutil_env_t *env,
        axutil_array_list_t *values)
{
    int size = 0;
    int i    = 0;
    AXIS2_ENV_CHECK(env, -1);
    AXIS2_PARAM_CHECK(env->error, value, -1);
    AXIS2_PARAM_CHECK(env->error, values, -1);
    size = axutil_array_list_size(values, env);
    for (i = 0; i < size; i++)
    {
        axis2_char_t *mem_val = NULL;
        mem_val = (axis2_char_t *)axutil_array_list_get(values,  env, i);
        if (mem_val && axis2_strcmp(mem_val, value) == 0)
        {
            return i;
        }
    }
    return -1;
}

