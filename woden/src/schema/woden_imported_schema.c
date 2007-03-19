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

#include <woden_imported_schema.h>
#include <xml_schema.h>
#include <axis2_uri.h>

typedef struct woden_imported_schema_impl woden_imported_schema_impl_t;

/**
 * @brief Imported Schema Struct Impl
 *   Axis2 Imported Schema
 */
struct woden_imported_schema_impl
{
    woden_imported_schema_t imported_schema;
    woden_schema_t *schema;
    woden_obj_types_t obj_type;
    axis2_hash_t *methods;
    axis2_hash_t *super;
    axis2_uri_t *f_schema_location;
};

#define INTF_TO_IMPL(schema) \
    ((woden_imported_schema_impl_t *) schema)

axis2_status_t AXIS2_CALL
woden_imported_schema_free(
    void *schema,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_imported_schema_super_objs(
    void *schema,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_imported_schema_type(
    void *schema,
    const axis2_env_t *env);

woden_schema_t *AXIS2_CALL
woden_imported_schema_get_base_impl(
    void *schema,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_imported_schema_set_location(
    void *schema,
    const axis2_env_t *env,
    axis2_uri_t *location);

axis2_uri_t *AXIS2_CALL
woden_imported_schema_get_location(
    void *schema,
    const axis2_env_t *env);

static woden_imported_schema_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_imported_schema_free_ops(
    void *schema,
    const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_imported_schema_t * AXIS2_CALL
woden_imported_schema_to_schema(
    void *schema,
    const axis2_env_t *env)
{
    woden_imported_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!schema)
    {
	  schema = (void *) create (env);
	  schema_impl = (woden_imported_schema_impl_t *)schema;
    }
    else
        schema_impl = (woden_imported_schema_impl_t *) schema;

    if (schema)
	  woden_imported_schema_free_ops(schema, env);

    schema_impl->imported_schema.schema.ops =
        AXIS2_MALLOC(env->allocator, sizeof(woden_schema_ops_t));
    woden_schema_resolve_methods(&(schema_impl->imported_schema.schema),
            env, schema_impl->schema, schema_impl->methods);

    return schema;
}

/************************End of Woden C Internal Methods***********************/

static woden_imported_schema_t *
create(const axis2_env_t *env)
{
    woden_imported_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_imported_schema_impl_t));

    schema_impl->schema = NULL;
    schema_impl->obj_type = WODEN_IMPORTED_SCHEMA;
    schema_impl->methods = NULL;
    schema_impl->super = NULL;
    schema_impl->f_schema_location = NULL;

    schema_impl->imported_schema.schema.ops = NULL;

    schema_impl->imported_schema.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_imported_schema_ops_t));

    schema_impl->imported_schema.ops->free =
        woden_imported_schema_free;
    schema_impl->imported_schema.ops->super_objs =
        woden_imported_schema_super_objs;
    schema_impl->imported_schema.ops->type =
        woden_imported_schema_type;
    schema_impl->imported_schema.ops->get_base_impl =
        woden_imported_schema_get_base_impl;

    schema_impl->imported_schema.ops->set_location =
        woden_imported_schema_set_location;
    schema_impl->imported_schema.ops->get_location =
        woden_imported_schema_get_location;


    schema_impl->super = axis2_hash_make(env);
    if (!schema_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    schema_impl->methods = axis2_hash_make(env);
    if (!schema_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(schema_impl->methods, "free",
            AXIS2_HASH_KEY_STRING, woden_imported_schema_free);
    axis2_hash_set(schema_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_imported_schema_super_objs);
    axis2_hash_set(schema_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_imported_schema_type);

    axis2_hash_set(schema_impl->methods, "set_location",
            AXIS2_HASH_KEY_STRING, woden_imported_schema_set_location);
    axis2_hash_set(schema_impl->methods, "get_location",
            AXIS2_HASH_KEY_STRING, woden_imported_schema_get_location);

    return &(schema_impl->imported_schema);
}
AXIS2_EXTERN woden_imported_schema_t * AXIS2_CALL
woden_imported_schema_create(
    const axis2_env_t *env)
{
    woden_imported_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = (woden_imported_schema_impl_t *) create(env);

    schema_impl->schema = woden_schema_create(env);
    axis2_hash_set(schema_impl->super, "WODEN_IMPORTED_SCHEMA", AXIS2_HASH_KEY_STRING,
            &(schema_impl->schema));
    axis2_hash_set(schema_impl->super, "WODEN_SCHEMA", AXIS2_HASH_KEY_STRING,
            schema_impl->schema);
    return &(schema_impl->imported_schema);
}

woden_obj_types_t AXIS2_CALL
woden_imported_schema_type(
    void *schema,
    const axis2_env_t *env)
{
    woden_imported_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->obj_type;
}

static axis2_status_t
woden_imported_schema_free_ops(
    void *schema,
    const axis2_env_t *env)
{
    woden_imported_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    if (schema_impl->imported_schema.schema.ops)
    {
        AXIS2_FREE(env->allocator, schema_impl->imported_schema.schema.ops);
        schema_impl->imported_schema.schema.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_imported_schema_free(
    void *schema,
    const axis2_env_t *env)
{
    woden_imported_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    if (schema_impl->f_schema_location)
    {
        axis2_uri_free(schema_impl->f_schema_location, env);
        schema_impl->f_schema_location = NULL;
    }

    if (schema_impl->methods)
    {
        axis2_hash_free(schema_impl->methods, env);
        schema_impl->methods = NULL;
    }

    if (schema_impl->schema)
    {
        WODEN_SCHEMA_FREE(schema_impl->schema, env);
        schema_impl->schema = NULL;
    }

    woden_imported_schema_free_ops(schema, env);

    if ((&(schema_impl->imported_schema))->ops)
    {
        AXIS2_FREE(env->allocator, (&(schema_impl->imported_schema))->ops);
        (&(schema_impl->imported_schema))->ops = NULL;
    }

    if (schema_impl)
    {
        AXIS2_FREE(env->allocator, schema_impl);
        schema_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_imported_schema_super_objs(
    void *schema,
    const axis2_env_t *env)
{
    woden_imported_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->super;
}

woden_schema_t *AXIS2_CALL
woden_imported_schema_get_base_impl(
    void *schema,
    const axis2_env_t *env)
{
    woden_imported_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->schema;
}

axis2_status_t AXIS2_CALL
woden_imported_schema_resolve_methods(
    woden_imported_schema_t *schema,
    const axis2_env_t *env,
    woden_imported_schema_t *schema_impl,
    axis2_hash_t *methods)
{
    woden_imported_schema_impl_t *schema_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    schema_impl_l = INTF_TO_IMPL(schema_impl);

    schema->ops->free =
        axis2_hash_get(methods, "free", AXIS2_HASH_KEY_STRING);
    schema->ops->type =
        axis2_hash_get(methods, "type", AXIS2_HASH_KEY_STRING);

    schema->ops->set_location = axis2_hash_get(methods,
            "set_location", AXIS2_HASH_KEY_STRING);
    if (!schema->ops->set_location && schema_impl_l)
        schema->ops->set_location =
            schema_impl_l->imported_schema.ops->set_location;

    schema->ops->get_location = axis2_hash_get(methods,
            "get_location", AXIS2_HASH_KEY_STRING);
    if (!schema->ops->get_location && schema_impl_l)
        schema->ops->get_location =
            schema_impl_l->imported_schema.ops->get_location;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_imported_schema_set_location(
    void *schema,
    const axis2_env_t *env,
    axis2_uri_t *location)
{
    woden_imported_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    if (schema_impl->f_schema_location)
    {
        axis2_uri_free(schema_impl->f_schema_location, env);
        schema_impl->f_schema_location = NULL;
    }
    schema_impl->f_schema_location = axis2_uri_clone(location, env);
    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL
woden_imported_schema_get_location(
    void *schema,
    const axis2_env_t *env)
{
    woden_imported_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->f_schema_location;
}

