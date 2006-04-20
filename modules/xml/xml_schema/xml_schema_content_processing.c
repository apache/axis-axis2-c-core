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

#include <xml_schema/axis2_xml_schema_content_processing.h>

typedef struct axis2_xml_schema_content_processing_impl 
        axis2_xml_schema_content_processing_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_content_processing_impl
{
    axis2_xml_schema_content_processing_t content_processing;
    axis2_xml_schema_enum_t *schema_enum;
    axis2_hash_t *methods;
    axis2_array_list_t *members;
};

#define INTF_TO_IMPL(content_processing) \
        ((axis2_xml_schema_content_processing_impl_t *) content_processing)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_content_processing_free(void *content_processing,
                        axis2_env_t **env);

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_content_processing_get_base_impl(void *content_processing,
                                                    axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_content_processing_get_values(void *content_processing,
                        axis2_env_t **env);


AXIS2_DECLARE(axis2_xml_schema_content_processing_t *)
axis2_xml_schema_content_processing_create(axis2_env_t **env,
                            axis2_char_t *value)
{
    axis2_xml_schema_content_processing_impl_t *content_processing_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    content_processing_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_content_processing_impl_t));

    content_processing_impl->schema_enum = NULL;
    content_processing_impl->methods = NULL;
    content_processing_impl->members = NULL;
    content_processing_impl->content_processing.ops = 
            AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_content_processing_ops_t));

    content_processing_impl->content_processing.ops->free = 
            axis2_xml_schema_content_processing_free;
    content_processing_impl->content_processing.ops->get_base_impl = 
            axis2_xml_schema_content_processing_get_base_impl;
    content_processing_impl->content_processing.ops->get_values = 
            axis2_xml_schema_content_processing_get_values;
   
    if(!content_processing_impl->members)
    {
        content_processing_impl->members = axis2_array_list_create(env, 0);
        if(!content_processing_impl->members)
            return NULL;
        AXIS2_ARRAY_LIST_ADD(content_processing_impl->members, env, "Lax");
        AXIS2_ARRAY_LIST_ADD(content_processing_impl->members, env, "None");
        AXIS2_ARRAY_LIST_ADD(content_processing_impl->members, env, "Skip");
        AXIS2_ARRAY_LIST_ADD(content_processing_impl->members, env, "Strict");
    }
    
    content_processing_impl->methods = axis2_hash_make(env);
    if(!content_processing_impl->methods)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(content_processing_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_content_processing_free);
    axis2_hash_set(content_processing_impl->methods, "get_values", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_content_processing_get_values);
    
    content_processing_impl->schema_enum = axis2_xml_schema_enum_create(env, NULL);
    status = axis2_xml_schema_enum_resolve_methods(
            &(content_processing_impl->content_processing.base), env, 
            content_processing_impl->schema_enum, 
            content_processing_impl->methods);
    
    return &(content_processing_impl->content_processing);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_content_processing_free(void *content_processing,
                                    axis2_env_t **env)
{
    axis2_xml_schema_content_processing_impl_t *content_processing_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_processing_impl = INTF_TO_IMPL(content_processing);

    if(content_processing_impl->members)
    {
        AXIS2_ARRAY_LIST_FREE(content_processing_impl->members, env);
        content_processing_impl->members = NULL;
    }
    
    if(content_processing_impl->methods)
    {
        axis2_hash_free(content_processing_impl->methods, env);
        content_processing_impl->methods = NULL;
    }
    if(content_processing_impl->schema_enum)
    {
        AXIS2_XML_SCHEMA_ENUM_FREE(content_processing_impl->schema_enum, env);
        content_processing_impl->schema_enum = NULL;
    }
    
    if((&(content_processing_impl->content_processing))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(content_processing_impl->
                        content_processing))->ops);
        (&(content_processing_impl->content_processing))->ops = NULL;
    }

    if(content_processing_impl)
    {
        AXIS2_FREE((*env)->allocator, content_processing_impl);
        content_processing_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_content_processing_get_base_impl(void *content_processing,
                                axis2_env_t **env)
{
    axis2_xml_schema_content_processing_impl_t *content_processing_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    content_processing_impl = INTF_TO_IMPL(content_processing);

    return content_processing_impl->schema_enum;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_content_processing_resolve_methods(
                axis2_xml_schema_content_processing_t *content_processing,
                axis2_env_t **env,
                axis2_xml_schema_content_processing_t *content_processing_impl,
                axis2_hash_t *methods)
{
    axis2_xml_schema_content_processing_impl_t *content_processing_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, content_processing_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    content_processing_impl_l = (axis2_xml_schema_content_processing_impl_t *) 
            content_processing_impl;
    
    content_processing->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_content_processing_ops_t));
    content_processing->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    content_processing->ops->get_base_impl = 
            content_processing_impl_l->content_processing.ops->get_base_impl;
    content_processing->ops->get_values = 
            content_processing_impl_l->content_processing.ops->get_values;
    
    return axis2_xml_schema_enum_resolve_methods(
            &(content_processing->base), 
            env, content_processing_impl_l->schema_enum, methods);
}

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_content_processing_get_values(void *content_processing,
                                        axis2_env_t **env)
{
    return INTF_TO_IMPL(content_processing)->members;
}

