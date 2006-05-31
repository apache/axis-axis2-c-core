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
#include <xml_schema/axis2_xml_schema_defines.h>
#include <xml_schema/axis2_xml_schema_content_type.h>
#include <axis2_utils.h>

#include <axis2_string.h>

typedef struct axis2_xml_schema_content_type_impl 
                axis2_xml_schema_content_type_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_content_type_impl
{
    axis2_xml_schema_content_type_t content_type;
    
    axis2_xml_schema_enum_t *schema_enum;

    axis2_hash_t *methods;
    
    axis2_xml_schema_types_t obj_type;

    axis2_array_list_t *members;

    axis2_hash_t *ht_super;    
};

#define AXIS2_INTF_TO_IMPL(content_type) \
        ((axis2_xml_schema_content_type_impl_t *) content_type)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_content_type_free(void *content_type,
                        axis2_env_t **env);

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_content_type_get_base_impl(void *content_type,
                                axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_content_type_resolve_methods(
                                axis2_xml_schema_content_type_t *content_type,
                                axis2_env_t **env,
                                axis2_xml_schema_content_type_t *content_type_impl,
                                axis2_hash_t *methods);

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_content_type_get_values(void *content_type,
                        axis2_env_t **env);
                        
axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_content_type_type(void *content_type,
                                   axis2_env_t **env);
                                   
axis2_hash_t* AXIS2_CALL
axis2_xml_schema_content_type_super_objs(void *content_type, 
                                         axis2_env_t **env);
                                         
                                                                            


AXIS2_DECLARE(axis2_xml_schema_content_type_t *)
axis2_xml_schema_content_type_create(axis2_env_t **env,
                            axis2_char_t *value)
{
    axis2_xml_schema_content_type_impl_t *content_type_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    content_type_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_content_type_impl_t));
    if(!content_type_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                    

    content_type_impl->schema_enum = NULL;
    content_type_impl->methods = NULL;
    content_type_impl->members = NULL;
    content_type_impl->obj_type = AXIS2_XML_SCHEMA_CONTENT_TYPE;
    content_type_impl->ht_super = NULL;
    
    content_type_impl->content_type.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_content_type_ops_t));
    if(!content_type_impl->content_type.ops)
    {
        axis2_xml_schema_content_type_free(&(content_type_impl->content_type), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
                        
    content_type_impl->content_type.ops->free = 
            axis2_xml_schema_content_type_free;
    content_type_impl->content_type.ops->get_base_impl = 
            axis2_xml_schema_content_type_get_base_impl;
    content_type_impl->content_type.ops->get_values = 
            axis2_xml_schema_content_type_get_values;
    content_type_impl->content_type.ops->type =
            axis2_xml_schema_content_type_type;
    content_type_impl->content_type.ops->super_objs =
            axis2_xml_schema_content_type_super_objs;                
                
    content_type_impl->members = axis2_array_list_create(env, 0);
    if(!content_type_impl->members)
    {    
        axis2_xml_schema_content_type_free(&(content_type_impl->content_type), env);
        return NULL;
    }       

    AXIS2_ARRAY_LIST_ADD(content_type_impl->members, env, 
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_ELEMENT_ONLY, env));
    AXIS2_ARRAY_LIST_ADD(content_type_impl->members, env,
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_EMPTY, env));
        
    AXIS2_ARRAY_LIST_ADD(content_type_impl->members, env,
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_MIXED, env));

    AXIS2_ARRAY_LIST_ADD(content_type_impl->members, env,
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_TEXT_ONLY, env));        

    content_type_impl->methods = axis2_hash_make(env);
    if(!content_type_impl->methods)
    {
        axis2_xml_schema_content_type_free(&(content_type_impl->content_type), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(content_type_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_content_type_free);
    axis2_hash_set(content_type_impl->methods, "get_values", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_content_type_get_values);
    axis2_hash_set(content_type_impl->methods, "type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_content_type_type);
    axis2_hash_set(content_type_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_content_type_super_objs);                        
            
    content_type_impl->schema_enum = axis2_xml_schema_enum_create(env, value);
    if(!content_type_impl->schema_enum)
    {
        axis2_xml_schema_content_type_free(&(content_type_impl->content_type), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    content_type_impl->ht_super = axis2_hash_make(env);
     if(!content_type_impl->ht_super)
    {
        axis2_xml_schema_content_type_free(&(content_type_impl->content_type), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    axis2_hash_set(content_type_impl->ht_super, "AXIS2_XML_SCHEMA_CONTENT_TYPE",
         AXIS2_HASH_KEY_STRING, &(content_type_impl->content_type));
    axis2_hash_set(content_type_impl->ht_super, "AXIS2_XML_SCHEMA_ENUM", AXIS2_HASH_KEY_STRING, 
            content_type_impl->schema_enum);
    
    status = axis2_xml_schema_enum_resolve_methods(
            &(content_type_impl->content_type.base), env, content_type_impl->schema_enum, 
            content_type_impl->methods); 

    return &(content_type_impl->content_type);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_content_type_free(void *content_type,
                                    axis2_env_t **env)
{
    axis2_xml_schema_content_type_impl_t *content_type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_type_impl = AXIS2_INTF_TO_IMPL(content_type);

    if(content_type_impl->members)
    {
        int size = 0;
        int i    = 0;
        size = AXIS2_ARRAY_LIST_SIZE(content_type_impl->members, env);
        for(i = 0; i < size ; i++)
        {
            axis2_char_t *value = NULL;
            value =(axis2_char_t* ) AXIS2_ARRAY_LIST_GET(content_type_impl->members, env, i);
            if(NULL != value)
            {
                AXIS2_FREE((*env)->allocator, value);
                value = NULL;
            }
        }
        AXIS2_ARRAY_LIST_FREE(content_type_impl->members, env);
        content_type_impl->members = NULL;
    }
    
    if(content_type_impl->schema_enum)
    {
        AXIS2_XML_SCHEMA_ENUM_FREE(content_type_impl->schema_enum, env);
        content_type_impl->schema_enum = NULL;
    }
    
    if((&(content_type_impl->content_type))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(content_type_impl->content_type))->ops);
        (&(content_type_impl->content_type))->ops = NULL;
    }

    if(content_type_impl)
    {
        AXIS2_FREE((*env)->allocator, content_type_impl);
        content_type_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_content_type_get_base_impl(void *content_type,
                                axis2_env_t **env)
{
    axis2_xml_schema_content_type_impl_t *content_type_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    content_type_impl = AXIS2_INTF_TO_IMPL(content_type);
    return content_type_impl->schema_enum;
}

axis2_array_list_t* AXIS2_CALL
axis2_xml_schema_content_type_get_values(void *content_type,
                                axis2_env_t **env)
{
/*    axis2_xml_schema_content_type_impl_t *schema_impl = NULL;
    axis2_hash_t *super = NULL;
    super = AXIS2_XML_SCHEMA_CONTENT_TYPE_SUPER_OBJS(content_type, env);
    if(NULL != super)
        schema_impl = axis2_hash_get(super,
         "AXIS2_XML_SCHEMA_CONTENT_TYPE", AXIS2_HASH_KEY_STRING); */
    return AXIS2_INTF_TO_IMPL(content_type)->members;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_content_type_type(void *content_type,
                                   axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(content_type)->obj_type;
}                                   
                                   
axis2_hash_t* AXIS2_CALL
axis2_xml_schema_content_type_super_objs(void *content_type, 
                                         axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(content_type)->ht_super;
}                                         
