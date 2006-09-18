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

#include <woden_inlined_schema.h>
#include <xml_schema.h>
#include <axis2_uri.h>

typedef struct woden_inlined_schema_impl woden_inlined_schema_impl_t;

/**
 * @brief Inlined Schema Struct Impl
 *   Axis2 Inlined Schema
 */
struct woden_inlined_schema_impl
{
    woden_inlined_schema_t inlined_schema;
    woden_schema_t *schema;
    woden_obj_types_t obj_type;
    axis2_hash_t *methods;
    axis2_hash_t *super;
    axis2_char_t *f_schema_id;
};

#define INTF_TO_IMPL(schema) \
    ((woden_inlined_schema_impl_t *) schema)

axis2_status_t AXIS2_CALL
woden_inlined_schema_free(
    void *schema,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_inlined_schema_super_objs(
    void *schema,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_inlined_schema_type(
    void *schema,
    const axis2_env_t *env);

woden_schema_t *AXIS2_CALL
woden_inlined_schema_get_base_impl(
    void *schema,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_inlined_schema_set_id(
    void *schema,
    const axis2_env_t *env,
    axis2_char_t *id);

axis2_char_t *AXIS2_CALL
woden_inlined_schema_get_id(
    void *schema,
    const axis2_env_t *env);

static woden_inlined_schema_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_inlined_schema_free_ops(
    void *schema,
    const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_inlined_schema_t * AXIS2_CALL
woden_inlined_schema_to_schema(
    void *schema,
    const axis2_env_t *env)
{
    woden_inlined_schema_impl_t *schema_impl = NULL;
    void *base_schema = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!schema)
    {
        schema_impl = (woden_inlined_schema_impl_t *) create(env);
    }
    else
        schema_impl = (woden_inlined_schema_impl_t *) schema;

    woden_inlined_schema_free_ops(schema, env);

    schema_impl->inlined_schema.schema.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_schema_ops_t));
    base_schema = WODEN_INLINED_SCHEMA_GET_BASE_IMPL(schema, env);
    woden_schema_resolve_methods(&(schema_impl->inlined_schema.schema),
            env, base_schema, schema_impl->methods);

    return schema;
}

/************************End of Woden C Internal Methods***********************/

static woden_inlined_schema_t *
create(
    const axis2_env_t *env)
{
    woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_inlined_schema_impl_t));

    schema_impl->schema = NULL;
    schema_impl->obj_type = WODEN_INLINED_SCHEMA;
    schema_impl->methods = NULL;
    schema_impl->super = NULL;
    schema_impl->f_schema_id = NULL;

    schema_impl->inlined_schema.schema.ops = NULL;

    schema_impl->inlined_schema.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_inlined_schema_ops_t));

    schema_impl->inlined_schema.ops->free =
        woden_inlined_schema_free;
    schema_impl->inlined_schema.ops->super_objs =
        woden_inlined_schema_super_objs;
    schema_impl->inlined_schema.ops->type =
        woden_inlined_schema_type;
    schema_impl->inlined_schema.ops->get_base_impl =
        woden_inlined_schema_get_base_impl;

    schema_impl->inlined_schema.ops->set_id =
        woden_inlined_schema_set_id;
    schema_impl->inlined_schema.ops->get_id =
        woden_inlined_schema_get_id;

    schema_impl->methods = axis2_hash_make(env);
    if (!schema_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(schema_impl->methods, "free",
            AXIS2_HASH_KEY_STRING, woden_inlined_schema_free);
    axis2_hash_set(schema_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_inlined_schema_super_objs);
    axis2_hash_set(schema_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_inlined_schema_type);

    axis2_hash_set(schema_impl->methods, "set_id",
            AXIS2_HASH_KEY_STRING, woden_inlined_schema_set_id);
    axis2_hash_set(schema_impl->methods, "get_id",
            AXIS2_HASH_KEY_STRING, woden_inlined_schema_get_id);

    return &(schema_impl->inlined_schema);
}
AXIS2_EXTERN woden_inlined_schema_t * AXIS2_CALL
woden_inlined_schema_create(
    const axis2_env_t *env)
{
    woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = (woden_inlined_schema_impl_t *) create(env);

    schema_impl->schema = woden_schema_create(env);
    schema_impl->super = axis2_hash_make(env);
    if (!schema_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(schema_impl->super, "WODEN_INLINED_SCHEMA", AXIS2_HASH_KEY_STRING,
            &(schema_impl->schema));
    axis2_hash_set(schema_impl->super, "WODEN_SCHEMA", AXIS2_HASH_KEY_STRING,
            schema_impl->schema);

    return &(schema_impl->inlined_schema);
}

woden_obj_types_t AXIS2_CALL
woden_inlined_schema_type(
    void *schema,
    const axis2_env_t *env)
{
    woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->obj_type;
}

static axis2_status_t
woden_inlined_schema_free_ops(
    void *schema,
    const axis2_env_t *env)
{
    woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    if (schema_impl->inlined_schema.schema.ops)
    {
        AXIS2_FREE(env->allocator, schema_impl->inlined_schema.schema.ops);
        schema_impl->inlined_schema.schema.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_inlined_schema_free(
    void *schema,
    const axis2_env_t *env)
{
    woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    if (schema_impl->f_schema_id)
    {
        AXIS2_FREE(env->allocator, schema_impl->f_schema_id);
        schema_impl->f_schema_id = NULL;
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

    woden_inlined_schema_free_ops(schema, env);

    if ((&(schema_impl->inlined_schema))->ops)
    {
        AXIS2_FREE(env->allocator, (&(schema_impl->inlined_schema))->ops);
        (&(schema_impl->inlined_schema))->ops = NULL;
    }

    if (schema_impl)
    {
        AXIS2_FREE(env->allocator, schema_impl);
        schema_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

woden_schema_t *AXIS2_CALL
woden_inlined_schema_get_base_impl(
    void *schema,
    const axis2_env_t *env)
{
    woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->schema;
}

axis2_hash_t *AXIS2_CALL
woden_inlined_schema_super_objs(
    void *schema,
    const axis2_env_t *env)
{
    woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->super;
}

axis2_status_t AXIS2_CALL
woden_inlined_schema_resolve_methods(
    woden_inlined_schema_t *schema,
    const axis2_env_t *env,
    woden_inlined_schema_t *schema_impl,
    axis2_hash_t *methods)
{
    woden_inlined_schema_impl_t *schema_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    schema_impl_l = INTF_TO_IMPL(schema_impl);

    schema->ops->free =
        axis2_hash_get(methods, "free", AXIS2_HASH_KEY_STRING);
    schema->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    schema->ops->type =
        axis2_hash_get(methods, "type", AXIS2_HASH_KEY_STRING);

    schema->ops->set_id = axis2_hash_get(methods,
            "set_id", AXIS2_HASH_KEY_STRING);
    if (!schema->ops->set_id && schema_impl_l)
        schema->ops->set_id =
            schema_impl_l->inlined_schema.ops->set_id;

    schema->ops->get_id = axis2_hash_get(methods,
            "get_id", AXIS2_HASH_KEY_STRING);
    if (!schema->ops->get_id && schema_impl_l)
        schema->ops->get_id =
            schema_impl_l->inlined_schema.ops->get_id;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_inlined_schema_set_id(
    void *schema,
    const axis2_env_t *env,
    axis2_char_t *id)
{
    woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    if (schema_impl->f_schema_id)
    {
        AXIS2_FREE(env->allocator, schema_impl->f_schema_id);
        schema_impl->f_schema_id = NULL;
    }
    schema_impl->f_schema_id = AXIS2_STRDUP(id, env);
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
woden_inlined_schema_get_id(
    void *schema,
    const axis2_env_t *env)
{
    woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->f_schema_id;
}


