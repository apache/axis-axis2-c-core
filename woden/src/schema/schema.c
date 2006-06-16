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
 
#include <woden_schema.h>
#include <xml_schema.h>

typedef struct woden_schema_impl woden_schema_impl_t;

/** 
 * @brief Schema Struct Impl
 *   Axis2 Schema  
 */ 
struct woden_schema_impl
{
    woden_schema_t schema;
    axis2_uri_t *f_namespc;
    xml_schema_t *f_schema_def;
    axis2_bool_t f_is_referenceable;
};

#define INTF_TO_IMPL(schema) ((woden_schema_impl_t *) schema)

axis2_status_t AXIS2_CALL 
woden_schema_free(
        void *schema,
        const axis2_env_t *envv);

axis2_status_t AXIS2_CALL 
woden_schema_set_namespace(
        void *schema,
        const axis2_env_t *env,
        axis2_uri_t *namespc);

axis2_uri_t *AXIS2_CALL 
woden_schema_get_namespace(
        void *schema,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
woden_schema_set_schema_def(
        void *schema,
        const axis2_env_t *env,
        xml_schema_t *schema_def);

xml_schema_t *AXIS2_CALL 
woden_schema_get_schema_def(
        void *schema,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
woden_schema_set_referenceable(
        void *schema,
        const axis2_env_t *env,
        axis2_bool_t referenceable);

axis2_bool_t AXIS2_CALL 
woden_schema_is_referenceable(
        void *schema,
        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL 
woden_schema_get_namespace_as_string(
        void *schema,
        const axis2_env_t *env);

AXIS2_EXTERN woden_schema_t * AXIS2_CALL
woden_schema_create(
        const axis2_env_t *env)
{
    woden_schema_impl_t *schema_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_schema_impl_t));

    schema_impl->f_namespc = NULL;
    schema_impl->f_schema_def = NULL;
    schema_impl->f_is_referenceable = AXIS2_TRUE;

    schema_impl->schema.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_schema_ops_t)); 
    
    schema_impl->schema.ops->free = woden_schema_free;
    schema_impl->schema.ops->set_namespace = woden_schema_set_namespace;
    schema_impl->schema.ops->get_namespace = woden_schema_get_namespace;
    schema_impl->schema.ops->set_schema_def = woden_schema_set_schema_def;
    schema_impl->schema.ops->get_schema_def = woden_schema_get_schema_def;
    schema_impl->schema.ops->set_referenceable = 
        woden_schema_set_referenceable;
    schema_impl->schema.ops->is_referenceable = 
        woden_schema_is_referenceable;
    schema_impl->schema.ops->get_namespace_as_string = 
        woden_schema_get_namespace_as_string;
    
    return &(schema_impl->schema);
}

axis2_status_t AXIS2_CALL
woden_schema_free(
        void *schema,
        const axis2_env_t *env)
{
    woden_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    if(schema_impl->f_namespc)
    {
        AXIS2_URI_FREE(schema_impl->f_namespc, env);
        schema_impl->f_namespc = NULL;
    }
    
    if(schema_impl->f_schema_def)
    {
        XML_SCHEMA_FREE(schema_impl->f_schema_def, env);
        schema_impl->f_schema_def = NULL;
    }
    
    if((&(schema_impl->schema))->ops)
    {
        AXIS2_FREE(env->allocator, (&(schema_impl->schema))->ops);
        (&(schema_impl->schema))->ops = NULL;
    }

    if(schema_impl)
    {
        AXIS2_FREE(env->allocator, schema_impl);
        schema_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_schema_resolve_methods(
        woden_schema_t *schema,
        const axis2_env_t *env,
        woden_schema_t *schema_impl,
        axis2_hash_t *methods)
{
    woden_schema_impl_t *schema_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    schema_impl_l = INTF_TO_IMPL(schema_impl);
    
    schema->ops->free = axis2_hash_get(methods, "free", AXIS2_HASH_KEY_STRING);

    schema->ops->set_namespace = axis2_hash_get(methods, 
            "set_namespace", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->set_namespace && schema_impl_l)
            schema->ops->set_namespace = 
            schema_impl_l->schema.ops->set_namespace;
    
    schema->ops->get_namespace = axis2_hash_get(methods, 
            "get_namespace", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_namespace && schema_impl_l)
            schema->ops->get_namespace = 
            schema_impl_l->schema.ops->get_namespace;
    
    schema->ops->set_schema_def = axis2_hash_get(methods, 
            "set_schema_def", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->set_schema_def && schema_impl_l)
            schema->ops->set_schema_def = 
            schema_impl_l->schema.ops->set_schema_def;
    
    schema->ops->get_schema_def = axis2_hash_get(methods, 
            "get_schema_def", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_schema_def && schema_impl_l)
            schema->ops->get_schema_def = 
            schema_impl_l->schema.ops->get_schema_def;
    
    schema->ops->set_referenceable = axis2_hash_get(methods, 
            "set_referenceable", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->set_referenceable && schema_impl_l)
            schema->ops->set_referenceable = 
            schema_impl_l->schema.ops->set_referenceable;
    
    schema->ops->is_referenceable = axis2_hash_get(methods, 
            "is_referenceable", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->is_referenceable && schema_impl_l)
            schema->ops->is_referenceable = 
            schema_impl_l->schema.ops->is_referenceable;
    
    schema->ops->get_namespace_as_string = axis2_hash_get(methods, 
            "get_namespace_as_string", AXIS2_HASH_KEY_STRING);
    if(!schema->ops->get_namespace_as_string && schema_impl_l)
            schema->ops->get_namespace_as_string = 
            schema_impl_l->schema.ops->get_namespace_as_string;

    return AXIS2_SUCCESS;    
}

axis2_status_t AXIS2_CALL 
woden_schema_set_namespace(
        void *schema,
        const axis2_env_t *env,
        axis2_uri_t *namespc)
{
    woden_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, namespc, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    if(schema_impl->f_namespc)
    {
        AXIS2_URI_FREE(schema_impl->f_namespc, env);
        schema_impl->f_namespc = NULL;
    }
    schema_impl->f_namespc = namespc;
    
    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL 
woden_schema_get_namespace(
        void *schema,
        const axis2_env_t *env)
{
    woden_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->f_namespc;
}

axis2_status_t AXIS2_CALL 
woden_schema_set_schema_def(
        void *schema,
        const axis2_env_t *env,
        xml_schema_t *schema_def)
{
    woden_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, schema_def, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);
    
    if(schema_impl->f_schema_def)
    {
        XML_SCHEMA_FREE(schema_impl->f_schema_def, env);
        schema_impl->f_schema_def = NULL;
    }
    schema_impl->f_schema_def = schema_def;
    
    return AXIS2_SUCCESS;
}

xml_schema_t *AXIS2_CALL 
woden_schema_get_schema_def(
        void *schema,
        const axis2_env_t *env)
{
    woden_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->f_schema_def;
}

axis2_status_t AXIS2_CALL 
woden_schema_set_referenceable(
        void *schema,
        const axis2_env_t *env,
        axis2_bool_t referenceable)
{
    woden_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    schema_impl->f_is_referenceable = referenceable;
    
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL 
woden_schema_is_referenceable(
        void *schema,
        const axis2_env_t *env)
{
    woden_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->f_is_referenceable;
}

axis2_char_t *AXIS2_CALL 
woden_schema_get_namespace_as_string(
        void *schema,
        const axis2_env_t *env)
{
    woden_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->f_namespc != NULL ? 
        AXIS2_URI_TO_STRING(schema_impl->f_namespc, env, AXIS2_URI_UNP_OMITUSERINFO) : NULL;
}

