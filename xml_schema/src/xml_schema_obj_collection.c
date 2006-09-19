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

#include <xml_schema_obj_collection.h>
#include <axis2_hash.h>
#include <axis2_utils.h>

typedef struct xml_schema_obj_collection_impl
            xml_schema_obj_collection_impl_t;

/**
 * @brief Xml Schema Obj Struct Impl
 *   Axis2 Xml Schema Obj
 */
struct xml_schema_obj_collection_impl
{
    xml_schema_obj_collection_t obj_collection;
    axis2_array_list_t *objs;
};

#define AXIS2_INTF_TO_IMPL(obj_collection) \
        ((xml_schema_obj_collection_impl_t *) obj_collection)

/**************************** end function prototypes ************************/

axis2_status_t AXIS2_CALL
xml_schema_obj_collection_free(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env);

int AXIS2_CALL
xml_schema_obj_collection_get_count(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env);

xml_schema_obj_t *AXIS2_CALL
xml_schema_obj_collection_get_item(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    int i);

axis2_status_t AXIS2_CALL
xml_schema_obj_collection_set_item(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    int i,
    void *item);

axis2_status_t AXIS2_CALL
xml_schema_obj_collection_add(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    void *item);

axis2_bool_t AXIS2_CALL
xml_schema_obj_collection_contains(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    void *item);

int AXIS2_CALL
xml_schema_obj_collection_index_of(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    void *item);

void* AXIS2_CALL
xml_schema_obj_collection_remove_at(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    int index);

axis2_array_list_t* AXIS2_CALL
xml_schema_obj_collection_to_array(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env);



AXIS2_EXTERN xml_schema_obj_collection_t * AXIS2_CALL
xml_schema_obj_collection_create(const axis2_env_t *env)
{
    xml_schema_obj_collection_impl_t *obj_collection_impl = NULL;

    obj_collection_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_obj_collection_impl_t));
    if (!obj_collection_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    obj_collection_impl->objs = NULL;
    obj_collection_impl->obj_collection.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_obj_collection_ops_t));

    if (!obj_collection_impl->obj_collection.ops)
    {
        XML_SCHEMA_OBJ_COLLECTION_FREE(&(obj_collection_impl->obj_collection), env);
        return NULL;
    }

    obj_collection_impl->objs = axis2_array_list_create(env, 0);
    if (!obj_collection_impl->objs)
    {
        xml_schema_obj_collection_free(&(obj_collection_impl->obj_collection), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    obj_collection_impl->obj_collection.ops->free =
        xml_schema_obj_collection_free;
    obj_collection_impl->obj_collection.ops->get_count =
        xml_schema_obj_collection_get_count;
    obj_collection_impl->obj_collection.ops->get_item =
        xml_schema_obj_collection_get_item;
    obj_collection_impl->obj_collection.ops->set_item =
        xml_schema_obj_collection_set_item;
    obj_collection_impl->obj_collection.ops->add =
        xml_schema_obj_collection_add;
    obj_collection_impl->obj_collection.ops->contains =
        xml_schema_obj_collection_contains;
    obj_collection_impl->obj_collection.ops->index_of =
        xml_schema_obj_collection_index_of;
    obj_collection_impl->obj_collection.ops->remove_at =
        xml_schema_obj_collection_remove_at;

    return &(obj_collection_impl->obj_collection);
}


axis2_status_t AXIS2_CALL
xml_schema_obj_collection_free(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env)
{
    xml_schema_obj_collection_impl_t *obj_collection_impl = NULL;

    obj_collection_impl = AXIS2_INTF_TO_IMPL(obj_collection);

    if (obj_collection_impl->objs)
    {
        AXIS2_ARRAY_LIST_FREE(obj_collection_impl->objs, env);
        obj_collection_impl->objs = NULL;
    }

    if (obj_collection_impl->obj_collection.ops)
    {
        AXIS2_FREE(env->allocator, obj_collection_impl->obj_collection.ops);
        obj_collection_impl->obj_collection.ops = NULL;
    }

    AXIS2_FREE(env->allocator, obj_collection_impl);
    obj_collection_impl = NULL;
    return AXIS2_SUCCESS;
}


int AXIS2_CALL
xml_schema_obj_collection_get_count(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env)
{
    xml_schema_obj_collection_impl_t *obj_collection_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    obj_collection_impl = AXIS2_INTF_TO_IMPL(obj_collection);
    return AXIS2_ARRAY_LIST_SIZE(obj_collection_impl->objs, env);
}

xml_schema_obj_t *AXIS2_CALL
xml_schema_obj_collection_get_item(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    int i)
{
    xml_schema_obj_collection_impl_t *obj_collection_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    obj_collection_impl = AXIS2_INTF_TO_IMPL(obj_collection);
    return AXIS2_ARRAY_LIST_GET(obj_collection_impl->objs, env, i);
}

axis2_status_t AXIS2_CALL
xml_schema_obj_collection_set_item(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    int i,
    void *item)
{
    xml_schema_obj_collection_impl_t *obj_collection_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, item, AXIS2_FAILURE);
    obj_collection_impl = AXIS2_INTF_TO_IMPL(obj_collection);

    AXIS2_ARRAY_LIST_SET(obj_collection_impl->objs, env, i, item);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
xml_schema_obj_collection_add(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    void *item)
{
    xml_schema_obj_collection_impl_t *obj_collection_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, item, AXIS2_FAILURE);
    obj_collection_impl = AXIS2_INTF_TO_IMPL(obj_collection);
    return AXIS2_ARRAY_LIST_ADD(obj_collection_impl->objs, env, item);
}

axis2_bool_t AXIS2_CALL
xml_schema_obj_collection_contains(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    void *item)
{
    xml_schema_obj_collection_impl_t *obj_collection_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, item, AXIS2_FAILURE);
    obj_collection_impl = AXIS2_INTF_TO_IMPL(obj_collection);

    return AXIS2_ARRAY_LIST_CONTAINS(obj_collection_impl->objs, env, item);
}

int AXIS2_CALL
xml_schema_obj_collection_index_of(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    void *item)
{
    xml_schema_obj_collection_impl_t *obj_collection_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, item, AXIS2_FAILURE);
    obj_collection_impl = AXIS2_INTF_TO_IMPL(obj_collection);
    return AXIS2_ARRAY_LIST_INDEX_OF(obj_collection_impl->objs, env, item);
}

void* AXIS2_CALL
xml_schema_obj_collection_remove_at(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env,
    int index)
{
    xml_schema_obj_collection_impl_t *obj_collection_impl = NULL;
    void *item = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    obj_collection_impl = AXIS2_INTF_TO_IMPL(obj_collection);
    item = AXIS2_ARRAY_LIST_REMOVE(obj_collection_impl->objs, env, index);
    return item;
}

axis2_array_list_t* AXIS2_CALL
xml_schema_obj_collection_to_array(
    xml_schema_obj_collection_t *obj_collection,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(obj_collection)->objs;
}
