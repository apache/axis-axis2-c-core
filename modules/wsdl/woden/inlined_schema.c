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

#include <woden/axis2_woden_inlined_schema.h>

typedef struct axis2_woden_inlined_schema_impl axis2_woden_inlined_schema_impl_t;

/** 
 * @brief Inlined Schema Struct Impl
 *	Axis2 Inlined Schema  
 */ 
struct axis2_woden_inlined_schema_impl
{
    axis2_woden_inlined_schema_t inlined_schema;
    axis2_woden_schema_t *schema;
    axis2_hash_t *methods;
    axis2_char_t *f_id;
};

#define INTF_TO_IMPL(schema) \
    ((axis2_woden_inlined_schema_impl_t *) schema)

axis2_status_t AXIS2_CALL 
axis2_woden_inlined_schema_free(void *schema,
                        axis2_env_t **env);

axis2_woden_schema_t *AXIS2_CALL
axis2_woden_inlined_schema_get_base_impl(void *schema,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_woden_inlined_schema_set_id(void *schema,
                                            axis2_env_t **env,
                                            axis2_char_t *id);

axis2_char_t *AXIS2_CALL
axis2_woden_inlined_schema_get_id(void *schema,
                                            axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_inlined_schema_t *)
axis2_woden_inlined_schema_create(axis2_env_t **env)
{
    axis2_woden_inlined_schema_impl_t *schema_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
     
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_inlined_schema_impl_t));

    schema_impl->schema = NULL;
    schema_impl->methods = NULL;
    schema_impl->f_id = NULL;
    schema_impl->inlined_schema.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_inlined_schema_ops_t));

    schema_impl->inlined_schema.ops->free = 
        axis2_woden_inlined_schema_free;
    schema_impl->inlined_schema.ops->get_base_impl = 
        axis2_woden_inlined_schema_get_base_impl;
    schema_impl->inlined_schema.ops->set_id = 
        axis2_woden_inlined_schema_set_id;
    schema_impl->inlined_schema.ops->get_id = 
        axis2_woden_inlined_schema_get_id;
    
    
    schema_impl->methods = axis2_hash_make(env);
    if(!schema_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(schema_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, axis2_woden_inlined_schema_free);
    axis2_hash_set(schema_impl->methods, "set_id", 
            AXIS2_HASH_KEY_STRING, axis2_woden_inlined_schema_set_id);
    axis2_hash_set(schema_impl->methods, "get_id", 
            AXIS2_HASH_KEY_STRING, axis2_woden_inlined_schema_get_id);

    schema_impl->schema = axis2_woden_schema_create(env);
    status = axis2_woden_schema_resolve_methods(&(schema_impl->
                inlined_schema.base), env, schema_impl->
                schema, schema_impl->methods);
    if(AXIS2_SUCCESS != status) return NULL;
    return &(schema_impl->inlined_schema);
}

axis2_status_t AXIS2_CALL
axis2_woden_inlined_schema_free(void *schema,
                        axis2_env_t **env)
{
    axis2_woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    if(schema_impl->f_id)
    {
        AXIS2_URL_FREE(schema_impl->f_id, env);
        schema_impl->f_id = NULL;
    }
 
    if(schema_impl->methods)
    {
        axis2_hash_free(schema_impl->methods, env);
        schema_impl->methods = NULL;
    }  

    if(schema_impl->schema)
    {
        AXIS2_WODEN_SCHEMA_FREE(schema_impl->schema, env);
        schema_impl->schema = NULL;
    }
    
    if((&(schema_impl->inlined_schema))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(schema_impl->inlined_schema))->ops);
        (&(schema_impl->inlined_schema))->ops = NULL;
    }

    if(schema_impl)
    {
        AXIS2_FREE((*env)->allocator, schema_impl);
        schema_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_woden_schema_t *AXIS2_CALL
axis2_woden_inlined_schema_get_base_impl(void *schema,
                                axis2_env_t **env)
{
    axis2_woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->schema;
}

axis2_status_t AXIS2_CALL
axis2_woden_inlined_schema_resolve_methods(
                    axis2_woden_inlined_schema_t *schema,
                    axis2_env_t **env,
                    axis2_woden_inlined_schema_t *schema_impl,
                    axis2_hash_t *methods)
{
    axis2_woden_inlined_schema_impl_t *schema_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, schema_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    schema_impl_l = (axis2_woden_inlined_schema_impl_t *) schema_impl;
    
    schema->ops = AXIS2_MALLOC((*env)->allocator, 
                            sizeof(axis2_woden_inlined_schema_ops_t));
    schema->ops->free = 
                axis2_hash_get(methods, "free", AXIS2_HASH_KEY_STRING);
    schema->ops->get_base_impl = 
                schema_impl_l->inlined_schema.ops->get_base_impl;
    schema->ops->set_id = schema_impl_l->inlined_schema.ops->set_id;
    schema->ops->get_id = schema_impl_l->inlined_schema.ops->get_id;
    
    return axis2_wsdl_ext_resolve_methods(&(schema->base), 
            env, schema_impl_l->schema, methods);
}

axis2_status_t AXIS2_CALL 
axis2_woden_inlined_schema_set_id(void *schema,
                                            axis2_env_t **env,
                                            axis2_char_t *id)
{
    axis2_woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    schema_impl->f_id = id;
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL 
axis2_woden_inlined_schema_get_id(void *schema,
                                            axis2_env_t **env)
{
    axis2_woden_inlined_schema_impl_t *schema_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    schema_impl = INTF_TO_IMPL(schema);

    return schema_impl->f_id;
}

