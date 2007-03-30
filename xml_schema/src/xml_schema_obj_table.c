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

#include <xml_schema_obj_table.h>
#include <axis2_hash.h>

typedef struct xml_schema_obj_table_impl
            xml_schema_obj_table_impl_t;

/**
 * @brief xml_schema_obj_table_impl
 */
struct xml_schema_obj_table_impl
{
    xml_schema_obj_table_t obj_table;

    axis2_hash_t *collection;

    axutil_array_list_t *names;

    axutil_array_list_t *values;
};

#define AXIS2_INTF_TO_IMPL(obj_table) \
        ((xml_schema_obj_table_impl_t *) obj_table)

/******************** function prototypes **************************************/

axis2_status_t AXIS2_CALL
xml_schema_obj_table_free(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env);

int AXIS2_CALL
xml_schema_obj_table_get_count(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env);

void *AXIS2_CALL
xml_schema_obj_table_get_item(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env,
        const axis2_qname_t *qname);

axutil_array_list_t *AXIS2_CALL
xml_schema_obj_table_get_names(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env);

axutil_array_list_t *AXIS2_CALL
xml_schema_obj_table_get_values(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env);

axis2_bool_t AXIS2_CALL
xml_schema_obj_table_contains(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env,
        axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
xml_schema_obj_table_add(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env,
        axis2_qname_t *qname,
        void *value);

axis2_status_t AXIS2_CALL
xml_schema_obj_table_put(
    xml_schema_obj_table_t *obj_table,
    const axutil_env_t *env,
    axis2_char_t *key,
    void *value);

void* AXIS2_CALL
xml_schema_obj_table_get(
    xml_schema_obj_table_t *obj_table,
    const axutil_env_t *env,
    axis2_char_t *key);

axis2_hash_t* AXIS2_CALL
xml_schema_obj_table_get_hash_table(
    xml_schema_obj_table_t *obj_table,
    const axutil_env_t *env);

/******************** end function pointers ***********************************/

AXIS2_EXTERN xml_schema_obj_table_t * AXIS2_CALL
xml_schema_obj_table_create(const axutil_env_t *env)
{
    xml_schema_obj_table_impl_t *obj_table_impl = NULL;

    obj_table_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_obj_table_impl_t));

    if (!obj_table_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    obj_table_impl->collection = NULL;
    obj_table_impl->names = NULL;
    obj_table_impl->values = NULL;
    obj_table_impl->obj_table.ops = NULL;

    obj_table_impl->obj_table.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_obj_table_ops_t));
    if (!obj_table_impl->obj_table.ops)
    {
        xml_schema_obj_table_free(&(obj_table_impl->obj_table), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    obj_table_impl->collection = axis2_hash_make(env);

    if (!obj_table_impl->collection)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    obj_table_impl->obj_table.ops->free =
        xml_schema_obj_table_free;
    obj_table_impl->obj_table.ops->put =
        xml_schema_obj_table_put;
    obj_table_impl->obj_table.ops->get_count =
        xml_schema_obj_table_get_count;
    obj_table_impl->obj_table.ops->get_item =
        xml_schema_obj_table_get_item;
    obj_table_impl->obj_table.ops->get_names =
        xml_schema_obj_table_get_names;
    obj_table_impl->obj_table.ops->get_values =
        xml_schema_obj_table_get_values;
    obj_table_impl->obj_table.ops->contains =
        xml_schema_obj_table_contains;
    obj_table_impl->obj_table.ops->add =
        xml_schema_obj_table_add;
    obj_table_impl->obj_table.ops->get_hash_table =
        xml_schema_obj_table_get_hash_table;

    return &(obj_table_impl->obj_table);
}


axis2_status_t AXIS2_CALL
xml_schema_obj_table_free(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env)
{
    xml_schema_obj_table_impl_t *obj_table_impl = NULL;

    obj_table_impl = AXIS2_INTF_TO_IMPL(obj_table);

    if (obj_table_impl->names)
    {
        axutil_array_list_free(obj_table_impl->names, env);
        obj_table_impl->names = NULL;
    }

    if (obj_table_impl->values)
    {
        axutil_array_list_free(obj_table_impl->values, env);
        obj_table_impl->values = NULL;
    }

    if (obj_table_impl->collection)
    {
        axis2_hash_free(obj_table_impl->collection, env);
        obj_table_impl->collection = NULL;
    }

    if (obj_table_impl->obj_table.ops)
    {
        AXIS2_FREE(env->allocator, obj_table_impl->obj_table.ops);
        obj_table_impl->obj_table.ops = NULL;
    }

    AXIS2_FREE(env->allocator, obj_table_impl);
    obj_table_impl = NULL;
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
xml_schema_obj_table_get_count(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env)
{
    xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    obj_table_impl = AXIS2_INTF_TO_IMPL(obj_table);
    if (obj_table_impl->collection)
    {
        return axis2_hash_count(obj_table_impl->collection);
    }
    else
        return -1;
}

void *AXIS2_CALL
xml_schema_obj_table_get_item(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env,
        const axis2_qname_t *qname)
{
    xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    axis2_char_t *name = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);

    obj_table_impl = AXIS2_INTF_TO_IMPL(obj_table);

    name = axis2_qname_to_string((axis2_qname_t *)qname, env);
    if (name)
    {
        return axis2_hash_get(obj_table_impl->collection, name,
                AXIS2_HASH_KEY_STRING);
    }
    return NULL;
}

axutil_array_list_t *AXIS2_CALL
xml_schema_obj_table_get_names(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env)
{
    xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    axis2_hash_index_t *hi = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    obj_table_impl = AXIS2_INTF_TO_IMPL(obj_table);

    if (obj_table_impl->names)
    {
        axutil_array_list_free(obj_table_impl->names, env);
        obj_table_impl->names = NULL;
    }

    obj_table_impl->names = axutil_array_list_create(env, 0);
    if (!obj_table_impl->names)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    for (hi = axis2_hash_first(obj_table_impl->collection, env); hi;
            hi = axis2_hash_next(env, hi))
    {
        const void *val = NULL;
        axis2_qname_t *qname = NULL;
        /*axis2_hash_this (hi, NULL, NULL, &val);*/
        axis2_hash_this(hi, &val, NULL, NULL);
        qname = axis2_qname_create_from_string(env, val);
        axutil_array_list_add(obj_table_impl->names, env, qname);
        val = NULL;
    }
    return obj_table_impl->names;
}

axutil_array_list_t *AXIS2_CALL
xml_schema_obj_table_get_values(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env)
{
    xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    obj_table_impl = AXIS2_INTF_TO_IMPL(obj_table);

    if (obj_table_impl->values)
    {
        axutil_array_list_free(obj_table_impl->values, env);
        obj_table_impl->values = NULL;
    }

    obj_table_impl->values = axutil_array_list_create(env, 0);
    if (!obj_table_impl->values)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    for (hi = axis2_hash_first(obj_table_impl->collection, env); hi;
            hi = axis2_hash_next(env, hi))
    {
        void *val = NULL;
        axis2_hash_this(hi, NULL, NULL, &val);
        axutil_array_list_add(obj_table_impl->values, env, val);
        val = NULL;
    }
    return obj_table_impl->values;
}

axis2_bool_t AXIS2_CALL
xml_schema_obj_table_contains(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env,
        axis2_qname_t *qname)
{
    xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    void *value = NULL;
    axis2_char_t *name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    obj_table_impl = AXIS2_INTF_TO_IMPL(obj_table);
    name = axis2_qname_to_string(qname, env);
    if (name)
    {
        value = axis2_hash_get(obj_table_impl->collection, name,
                AXIS2_HASH_KEY_STRING);
        if (value)
            return AXIS2_TRUE;
    }

    return AXIS2_FALSE;
}

axis2_status_t AXIS2_CALL
xml_schema_obj_table_add(xml_schema_obj_table_t *obj_table,
        const axutil_env_t *env,
        axis2_qname_t *qname,
        void *value)
{
    xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    axis2_char_t *name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, value, AXIS2_FAILURE);
    obj_table_impl = AXIS2_INTF_TO_IMPL(obj_table);
    name = axis2_qname_to_string(qname, env);
    if (name)
    {
        axis2_hash_set(obj_table_impl->collection,
                name, AXIS2_HASH_KEY_STRING, value);
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL
xml_schema_obj_table_put(
    xml_schema_obj_table_t *obj_table,
    const axutil_env_t *env,
    axis2_char_t *key,
    void *value)
{
    xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, value, AXIS2_FAILURE);

    obj_table_impl = AXIS2_INTF_TO_IMPL(obj_table);
    axis2_hash_set(obj_table_impl->collection, key, AXIS2_HASH_KEY_STRING,
            value);
    return AXIS2_SUCCESS;
}

void* AXIS2_CALL
xml_schema_obj_table_get(
    xml_schema_obj_table_t *obj_table,
    const axutil_env_t *env,
    axis2_char_t *key)
{
    xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, key, NULL);
    obj_table_impl = AXIS2_INTF_TO_IMPL(obj_table);
    return axis2_hash_get(obj_table_impl->collection, key, AXIS2_HASH_KEY_STRING);
}

axis2_hash_t* AXIS2_CALL
xml_schema_obj_table_get_hash_table(
    xml_schema_obj_table_t *obj_table,
    const axutil_env_t *env)
{
    xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    obj_table_impl = AXIS2_INTF_TO_IMPL(obj_table);
    return obj_table_impl->collection;
}
