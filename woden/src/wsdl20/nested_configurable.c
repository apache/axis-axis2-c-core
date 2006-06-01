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

#include <woden/wsdl20/axis2_woden_nested_configurable.h>
#include <woden/wsdl20/axis2_woden_documentable.h>


typedef struct axis2_woden_nested_configurable_impl axis2_woden_nested_configurable_impl_t;

/** 
 * @brief Documentable Struct Impl
 *	Axis2 Documentable  
 */ 
struct axis2_woden_nested_configurable_impl
{
    axis2_woden_nested_configurable_t nested_configurable;
    axis2_woden_configurable_t *configurable;
    axis2_hash_t *super;
    axis2_array_list_t *f_parent;
};

#define INTF_TO_IMPL(nested_configurable) ((axis2_woden_nested_configurable_impl_t *) nested_configurable)

axis2_status_t AXIS2_CALL 
axis2_woden_nested_configurable_free(
        void *nested_configurable,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_nested_configurable_super_objs(
        void *nested_configurable,
        const axis2_env_t *env);

axis2_woden_configurable_t *AXIS2_CALL
axis2_woden_nested_configurable_get_base_impl(
        void *nested_configurable,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_nested_configurable_get_parent(
        void *nested_configurable,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_woden_nested_configurable_set_parent_element(
        void *nested_configurable,
        const axis2_env_t *env,
        void *parent);

void *AXIS2_CALL
axis2_woden_nested_configurable_get_parent_element(
        void *nested_configurable,
        const axis2_env_t *env);

static axis2_woden_nested_configurable_t *
create(
        const axis2_env_t *env)
{
    axis2_woden_nested_configurable_impl_t *nested_configurable_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    nested_configurable_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_nested_configurable_impl_t));

    nested_configurable_impl->super = NULL;
    nested_configurable_impl->f_parent = NULL;
    
    nested_configurable_impl->nested_configurable.ops = 
            AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_woden_nested_configurable_ops_t));

    nested_configurable_impl->nested_configurable.ops->free = 
        axis2_woden_nested_configurable_free;
    nested_configurable_impl->nested_configurable.ops->super_objs = 
        axis2_woden_nested_configurable_super_objs;
    nested_configurable_impl->nested_configurable.ops->get_base_impl = 
        axis2_woden_nested_configurable_get_base_impl;

    return &(nested_configurable_impl->nested_configurable);
}

AXIS2_EXTERN axis2_woden_nested_configurable_t * AXIS2_CALL
axis2_woden_nested_configurable_create(
        const axis2_env_t *env)
{
    axis2_woden_nested_configurable_impl_t *nested_configurable_impl = NULL;
    void *documentable = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    nested_configurable_impl = (axis2_woden_nested_configurable_impl_t *) create(env);

    nested_configurable_impl->configurable = axis2_woden_configurable_create(env);

    nested_configurable_impl->super = axis2_hash_make(env);
    if(!nested_configurable_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(nested_configurable_impl->super, "AXIS2_WODEN_NESTED_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, &(nested_configurable_impl->nested_configurable));
    axis2_hash_set(nested_configurable_impl->super, "AXIS2_WODEN_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, nested_configurable_impl->configurable);
    documentable = AXIS2_WODEN_CONFIGURABLE_GET_BASE_IMPL(
            nested_configurable_impl->configurable, env);
    axis2_hash_set(nested_configurable_impl->super, "AXIS2_WODEN_DOCUMENTABLE", 
            AXIS2_HASH_KEY_STRING, documentable);
         
    return &(nested_configurable_impl->nested_configurable);
}

axis2_status_t AXIS2_CALL
axis2_woden_nested_configurable_free(
        void *nested_configurable,
        const axis2_env_t *env)
{
    axis2_woden_nested_configurable_impl_t *nested_configurable_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    nested_configurable_impl = INTF_TO_IMPL(nested_configurable);

    if(nested_configurable_impl->f_parent)
    {
        /* TODO free */
        nested_configurable_impl->f_parent = NULL;
    }
    
    if(nested_configurable_impl->configurable)
    {
        AXIS2_WODEN_CONFIGURABLE_FREE(nested_configurable_impl->configurable, 
                env);
        nested_configurable_impl->configurable = NULL;
    }

    if(nested_configurable_impl->super)
    {
        axis2_hash_free(nested_configurable_impl->super, env);
        nested_configurable_impl->super = NULL;
    }
    
    if(nested_configurable_impl->nested_configurable.ops)
    {
        AXIS2_FREE(env->allocator, nested_configurable_impl->nested_configurable.ops);
        nested_configurable_impl->nested_configurable.ops = NULL;
    }
    
    if(nested_configurable_impl)
    {
        AXIS2_FREE(env->allocator, nested_configurable_impl);
        nested_configurable_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_nested_configurable_super_objs(
        void *nested_configurable,
        const axis2_env_t *env)
{
    axis2_woden_nested_configurable_impl_t *nested_configurable_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    nested_configurable_impl = INTF_TO_IMPL(nested_configurable);

    return nested_configurable_impl->super;
}

axis2_woden_configurable_t *AXIS2_CALL
axis2_woden_nested_configurable_get_base_impl(
        void *nested_configurable,
        const axis2_env_t *env)
{
    axis2_woden_nested_configurable_impl_t *nested_configurable_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    nested_configurable_impl = INTF_TO_IMPL(nested_configurable);

    return nested_configurable_impl->configurable;
}

axis2_status_t AXIS2_CALL
axis2_woden_nested_configurable_resolve_methods(
        axis2_woden_nested_configurable_t *nested_configurable,
        const axis2_env_t *env,
        axis2_woden_nested_configurable_t *nested_configurable_impl,
        axis2_hash_t *methods)
{
    axis2_woden_nested_configurable_impl_t *nested_configurable_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    nested_configurable_impl_l = INTF_TO_IMPL(nested_configurable_impl);
    
    nested_configurable->ops = AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_nested_configurable_ops_t));
    nested_configurable->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    nested_configurable->ops->to_nested_configurable_free = axis2_hash_get(methods, 
            "to_nested_configurable_free", AXIS2_HASH_KEY_STRING);
    nested_configurable->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_nested_configurable_get_parent(
        void *nested_configurable,
        const axis2_env_t *env)
{
    axis2_woden_nested_configurable_impl_t *nested_configurable_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_NESTED_CONFIGURABLE_SUPER_OBJS(nested_configurable, env);
    nested_configurable_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_NESTED_CONFIGURABLE", AXIS2_HASH_KEY_STRING));

    return nested_configurable_impl->f_parent;
}

axis2_status_t AXIS2_CALL
axis2_woden_nested_configurable_set_parent_element(
        void *nested_configurable,
        const axis2_env_t *env,
        void *parent)
{
    axis2_woden_nested_configurable_impl_t *nested_configurable_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, parent, AXIS2_FAILURE);
    super = AXIS2_WODEN_NESTED_CONFIGURABLE_SUPER_OBJS(nested_configurable, env);
    nested_configurable_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_NESTED_CONFIGURABLE", AXIS2_HASH_KEY_STRING));
   
    if(nested_configurable_impl->f_parent)
    {
        /* TODO free parent */
    }
    nested_configurable_impl->f_parent = parent;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_nested_configurable_get_parent_element(
        void *nested_configurable,
        const axis2_env_t *env)
{
    axis2_woden_nested_configurable_impl_t *nested_configurable_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_NESTED_CONFIGURABLE_SUPER_OBJS(nested_configurable, env);
    nested_configurable_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_NESTED_CONFIGURABLE", AXIS2_HASH_KEY_STRING));

    return nested_configurable_impl->f_parent;
}
