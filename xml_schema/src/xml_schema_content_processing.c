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

#include <xml_schema_content_processing.h>
#include <xml_schema_use.h>
#include <axis2_utils.h>

typedef struct xml_schema_content_processing_impl 
        xml_schema_content_processing_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
 */ 
struct xml_schema_content_processing_impl
{
    xml_schema_content_processing_t content_processing;
    
    xml_schema_enum_t *schema_enum;
    
    xml_schema_types_t obj_type;
    
    axis2_hash_t *ht_super;
    
    axis2_hash_t *methods;
    
    axis2_array_list_t *members;
};

#define AXIS2_INTF_TO_IMPL(content_processing) \
        ((xml_schema_content_processing_impl_t *) content_processing)

axis2_status_t AXIS2_CALL 
xml_schema_content_processing_free(
        void *content_processing,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
xml_schema_content_processing_super_objs(
        void *content_processing,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL 
xml_schema_content_processing_get_type(
        void *content_processing,
        const axis2_env_t *env);

xml_schema_enum_t *AXIS2_CALL
xml_schema_content_processing_get_base_impl(void *content_processing,
                                                    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
xml_schema_content_processing_get_values(void *content_processing,
                        const axis2_env_t *env);


AXIS2_EXTERN xml_schema_content_processing_t * AXIS2_CALL
xml_schema_content_processing_create(const axis2_env_t *env,
                            axis2_char_t *value)
{
    xml_schema_content_processing_impl_t *content_processing_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    content_processing_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_content_processing_impl_t));
    if(!content_processing_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    content_processing_impl->schema_enum = NULL;
    content_processing_impl->content_processing.base.ops = NULL;
    content_processing_impl->obj_type = XML_SCHEMA_CONTENT_PROCESSING;
    content_processing_impl->ht_super = NULL;
    content_processing_impl->methods = NULL;
    content_processing_impl->members = NULL;
    content_processing_impl->content_processing.ops = NULL;
    
    content_processing_impl->content_processing.ops = 
            AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_content_processing_ops_t));
    if(!content_processing_impl->content_processing.ops)
    {
        xml_schema_content_processing_free(
            &(content_processing_impl->content_processing), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;            
    }
    
    content_processing_impl->content_processing.ops->free = 
            xml_schema_content_processing_free;
    content_processing_impl->content_processing.ops->super_objs = 
            xml_schema_content_processing_super_objs;
    content_processing_impl->content_processing.ops->get_type = 
            xml_schema_content_processing_get_type;
    content_processing_impl->content_processing.ops->get_base_impl = 
            xml_schema_content_processing_get_base_impl;
    content_processing_impl->content_processing.ops->get_values = 
            xml_schema_content_processing_get_values;
   
    content_processing_impl->members = axis2_array_list_create(env, 0);

    if(!content_processing_impl->members)
    {
         xml_schema_content_processing_free(
            &(content_processing_impl->content_processing), env);
        return NULL;
    }   

    AXIS2_ARRAY_LIST_ADD(content_processing_impl->members, env, 
        AXIS2_STRDUP(XML_SCHEMA_CONST_LAX, env));
    AXIS2_ARRAY_LIST_ADD(content_processing_impl->members, env,
        AXIS2_STRDUP(XML_SCHEMA_CONST_NONE, env));
    AXIS2_ARRAY_LIST_ADD(content_processing_impl->members, env, 
        AXIS2_STRDUP(XML_SCHEMA_CONST_SKIP, env));
    AXIS2_ARRAY_LIST_ADD(content_processing_impl->members, env,
        AXIS2_STRDUP(XML_SCHEMA_CONST_STRICT, env));
    
    content_processing_impl->methods = axis2_hash_make(env);
    if(!content_processing_impl->methods)
    {
         xml_schema_content_processing_free(
            &(content_processing_impl->content_processing), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(content_processing_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, xml_schema_content_processing_free);
    axis2_hash_set(content_processing_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, xml_schema_content_processing_super_objs);
    axis2_hash_set(content_processing_impl->methods, "get_type", 
            AXIS2_HASH_KEY_STRING, xml_schema_content_processing_get_type);
            
    axis2_hash_set(content_processing_impl->methods, "get_values", 
            AXIS2_HASH_KEY_STRING, xml_schema_content_processing_get_values);
    
    content_processing_impl->schema_enum = xml_schema_enum_create(env, value);
    
    content_processing_impl->ht_super = axis2_hash_make(env);
    if(!content_processing_impl->ht_super)
    {
         xml_schema_content_processing_free(
            &(content_processing_impl->content_processing), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(content_processing_impl->ht_super, "XML_SCHEMA_CONTENT_PROCESSING", 
            AXIS2_HASH_KEY_STRING, &(content_processing_impl->content_processing));
    
    axis2_hash_set(content_processing_impl->ht_super, "XML_SCHEMA_ENUM", 
            AXIS2_HASH_KEY_STRING, content_processing_impl->schema_enum);

    status = xml_schema_enum_resolve_methods(
            &(content_processing_impl->content_processing.base), env, 
            content_processing_impl->schema_enum, 
            content_processing_impl->methods);
    
    return &(content_processing_impl->content_processing);
}

axis2_status_t AXIS2_CALL
xml_schema_content_processing_free(void *content_processing,
                                    const axis2_env_t *env)
{
    xml_schema_content_processing_impl_t *content_processing_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_processing_impl = AXIS2_INTF_TO_IMPL(content_processing);

    if(NULL != content_processing_impl->members)
    {   
        int size = 0;
        int i    = 0;
        size = AXIS2_ARRAY_LIST_SIZE(content_processing_impl->members, env);
        for(i = 0; i < size ; i++)
        {
            axis2_char_t *value = NULL;
            value =(axis2_char_t* ) 
                AXIS2_ARRAY_LIST_GET(content_processing_impl->members, env, i);
            if(NULL != value)
            {
                AXIS2_FREE(env->allocator, value);
                value = NULL;
            }
        }
        AXIS2_ARRAY_LIST_FREE(content_processing_impl->members, env);
        content_processing_impl->members = NULL;
    }
    
    if(NULL != content_processing_impl->ht_super)
    {
        axis2_hash_free(content_processing_impl->ht_super, env);
        content_processing_impl->ht_super = NULL;
    }
    
    if(NULL != content_processing_impl->methods)
    {
        axis2_hash_free(content_processing_impl->methods, env);
        content_processing_impl->methods = NULL;
    }

    if(NULL != content_processing_impl->schema_enum)
    {
        XML_SCHEMA_ENUM_FREE(content_processing_impl->schema_enum, env);
        content_processing_impl->schema_enum = NULL;
    }
    
    if(NULL != content_processing_impl->content_processing.ops)
    {
        AXIS2_FREE(env->allocator, content_processing_impl->content_processing.ops);
        content_processing_impl->content_processing.ops = NULL;
    }
    if(NULL != content_processing_impl->content_processing.base.ops)
    {
        AXIS2_FREE(env->allocator, content_processing_impl->content_processing.base.ops);
        content_processing_impl->content_processing.base.ops = NULL;
    }
    if(NULL != content_processing_impl)
    {
        AXIS2_FREE(env->allocator, content_processing_impl);
        content_processing_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
xml_schema_content_processing_super_objs(
        void *content_processing,
        const axis2_env_t *env)
{
    xml_schema_content_processing_impl_t *content_processing_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    content_processing_impl = AXIS2_INTF_TO_IMPL(content_processing);

    return content_processing_impl->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_content_processing_get_type(
        void *content_processing,
        const axis2_env_t *env)
{
    xml_schema_content_processing_impl_t *content_processing_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_processing_impl = AXIS2_INTF_TO_IMPL(content_processing);

    return content_processing_impl->obj_type;
}

xml_schema_enum_t *AXIS2_CALL
xml_schema_content_processing_get_base_impl(
        void *content_processing,
        const axis2_env_t *env)
{
    xml_schema_content_processing_impl_t *content_processing_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    content_processing_impl = AXIS2_INTF_TO_IMPL(content_processing);

    return content_processing_impl->schema_enum;
}

axis2_array_list_t *AXIS2_CALL
xml_schema_content_processing_get_values(void *content_processing,
                                        const axis2_env_t *env)
{
    xml_schema_content_processing_impl_t *content_processing_impl = NULL;
    axis2_hash_t *super = NULL;

    super = XML_SCHEMA_USE_SUPER_OBJS(content_processing, env);
    content_processing_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "XML_SCHEMA_CONTENT_PROCESSING", AXIS2_HASH_KEY_STRING));

    return content_processing_impl->members;
}

