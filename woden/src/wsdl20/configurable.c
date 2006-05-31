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

#include <woden/wsdl20/axis2_woden_configurable.h>
#include <woden/wsdl20/axis2_woden_documentable.h>


typedef struct axis2_woden_configurable_impl axis2_woden_configurable_impl_t;

/** 
 * @brief Documentable Struct Impl
 *	Axis2 Documentable  
 */ 
struct axis2_woden_configurable_impl
{
    axis2_woden_configurable_t configurable;
    axis2_woden_documentable_t *documentable;
    axis2_hash_t *super;
    axis2_array_list_t *f_features;
    axis2_array_list_t *f_properties;
};

#define INTF_TO_IMPL(configurable) ((axis2_woden_configurable_impl_t *) configurable)

axis2_status_t AXIS2_CALL 
axis2_woden_configurable_free(
        void *configurable,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_configurable_super_objs(
        void *configurable,
        const axis2_env_t *env);

axis2_woden_documentable_t *AXIS2_CALL
axis2_woden_configurable_get_base_impl(
        void *configurable,
        const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
axis2_woden_configurable_get_features(
        void *configurable,
        const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
axis2_woden_configurable_get_properties(
        void *configurable,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_woden_configurable_add_feature_element(
        void *configurable,
        const axis2_env_t *env,
        void *feature);

axis2_array_list_t *AXIS2_CALL
axis2_woden_configurable_get_feature_elements(
        void *configurable,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_woden_configurable_add_property_element(
        void *configurable,
        const axis2_env_t *env,
        void *property);

axis2_array_list_t *AXIS2_CALL
axis2_woden_configurable_get_property_elements(
        void *configurable,
        const axis2_env_t *env);


static axis2_woden_configurable_t *
create(
        const axis2_env_t *env)
{
    axis2_woden_configurable_impl_t *configurable_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    configurable_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_configurable_impl_t));

    configurable_impl->super = NULL;
    configurable_impl->f_features = NULL;
    configurable_impl->f_properties = NULL;
    
    configurable_impl->configurable.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_configurable_ops_t));

    configurable_impl->configurable.ops->free = axis2_woden_configurable_free;
    configurable_impl->configurable.ops->super_objs = axis2_woden_configurable_super_objs;
    configurable_impl->configurable.ops->get_base_impl = 
        axis2_woden_configurable_get_base_impl;

    return &(configurable_impl->configurable);
}

AXIS2_DECLARE(axis2_woden_configurable_t *)
axis2_woden_configurable_create(
        const axis2_env_t *env)
{
    axis2_woden_configurable_impl_t *configurable_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    configurable_impl = (axis2_woden_configurable_impl_t *) create(env);

    configurable_impl->documentable = axis2_woden_documentable_create(env);

    configurable_impl->super = axis2_hash_make(env);
    if(!configurable_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(configurable_impl->super, "AXIS2_WODEN_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, &(configurable_impl->configurable));
    axis2_hash_set(configurable_impl->super, "AXIS2_WODEN_DOCUMENTABLE", 
            AXIS2_HASH_KEY_STRING, configurable_impl->documentable);
         
    return &(configurable_impl->configurable);
}

axis2_status_t AXIS2_CALL
axis2_woden_configurable_free(
        void *configurable,
        const axis2_env_t *env)
{
    axis2_woden_configurable_impl_t *configurable_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    configurable_impl = INTF_TO_IMPL(configurable);

    if(configurable_impl->f_features)
    {
        AXIS2_ARRAY_LIST_FREE(configurable_impl->f_features, env);
        configurable_impl->f_features = NULL;
    }
    
    if(configurable_impl->f_properties)
    {
        AXIS2_ARRAY_LIST_FREE(configurable_impl->f_properties, env);
        configurable_impl->f_properties = NULL;
    }
    
    if(configurable_impl->documentable)
    {
        AXIS2_WODEN_DOCUMENTABLE_FREE(configurable_impl->documentable, env);
        configurable_impl->documentable = NULL;
    }

    if(configurable_impl->super)
    {
        axis2_hash_free(configurable_impl->super, env);
        configurable_impl->super = NULL;
    }
    
    if(configurable_impl->configurable.ops)
    {
        AXIS2_FREE(env->allocator, configurable_impl->configurable.ops);
        configurable_impl->configurable.ops = NULL;
    }
    
    if(configurable_impl)
    {
        AXIS2_FREE(env->allocator, configurable_impl);
        configurable_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_configurable_super_objs(
        void *configurable,
        const axis2_env_t *env)
{
    axis2_woden_configurable_impl_t *configurable_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    configurable_impl = INTF_TO_IMPL(configurable);

    return configurable_impl->super;
}

axis2_woden_documentable_t *AXIS2_CALL
axis2_woden_configurable_get_base_impl(
        void *configurable,
        const axis2_env_t *env)
{
    axis2_woden_configurable_impl_t *configurable_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    configurable_impl = INTF_TO_IMPL(configurable);

    return configurable_impl->documentable;
}

axis2_status_t AXIS2_CALL
axis2_woden_configurable_resolve_methods(
        axis2_woden_configurable_t *configurable,
        const axis2_env_t *env,
        axis2_woden_configurable_t *configurable_impl,
        axis2_hash_t *methods)
{
    axis2_woden_configurable_impl_t *configurable_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    configurable_impl_l = INTF_TO_IMPL(configurable_impl);
    
    configurable->ops = AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_configurable_ops_t));
    configurable->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    configurable->ops->to_configurable_free = axis2_hash_get(methods, 
            "to_configurable_free", AXIS2_HASH_KEY_STRING);
    configurable->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_configurable_get_features(
        void *configurable,
        const axis2_env_t *env)
{
    axis2_woden_configurable_impl_t *configurable_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_CONFIGURABLE_SUPER_OBJS(configurable, env);
    configurable_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_CONFIGURABLE", AXIS2_HASH_KEY_STRING));

    return configurable_impl->f_features;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_configurable_get_properties(
        void *configurable,
        const axis2_env_t *env)
{
    axis2_woden_configurable_impl_t *configurable_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_CONFIGURABLE_SUPER_OBJS(configurable, env);
    configurable_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_CONFIGURABLE", AXIS2_HASH_KEY_STRING));

    return configurable_impl->f_properties;
}

axis2_status_t AXIS2_CALL
axis2_woden_configurable_add_feature_element(
        void *configurable,
        const axis2_env_t *env,
        void *feature)
{
    axis2_woden_configurable_impl_t *configurable_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, feature, AXIS2_FAILURE);
    super = AXIS2_WODEN_CONFIGURABLE_SUPER_OBJS(configurable, env);
    configurable_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_CONFIGURABLE", AXIS2_HASH_KEY_STRING));
   
    if(!configurable_impl->f_features)
    {
        configurable_impl->f_features = axis2_array_list_create(env, 0);
        if(!configurable_impl->f_features)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_ARRAY_LIST_ADD(configurable_impl->f_features, env, 
            feature);
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_configurable_get_feature_elements(
        void *configurable,
        const axis2_env_t *env)
{
    axis2_woden_configurable_impl_t *configurable_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_CONFIGURABLE_SUPER_OBJS(configurable, env);
    configurable_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_CONFIGURABLE", AXIS2_HASH_KEY_STRING));

    return configurable_impl->f_features;
}

axis2_status_t AXIS2_CALL
axis2_woden_configurable_add_property_element(
        void *configurable,
        const axis2_env_t *env,
        void *property)
{
    axis2_woden_configurable_impl_t *configurable_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, property, AXIS2_FAILURE);
    super = AXIS2_WODEN_CONFIGURABLE_SUPER_OBJS(configurable, env);
    configurable_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_CONFIGURABLE", AXIS2_HASH_KEY_STRING));
   
    if(!configurable_impl->f_properties)
    {
        configurable_impl->f_properties = axis2_array_list_create(env, 0);
        if(!configurable_impl->f_properties)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_ARRAY_LIST_ADD(configurable_impl->f_properties, env, 
            property);
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_configurable_get_property_elements(
        void *configurable,
        const axis2_env_t *env)
{
    axis2_woden_configurable_impl_t *configurable_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_CONFIGURABLE_SUPER_OBJS(configurable, env);
    configurable_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_CONFIGURABLE", AXIS2_HASH_KEY_STRING));

    return configurable_impl->f_properties;
}
