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

#include <xml_schema_tokenized_type.h>
#include <axis2_utils.h>

typedef struct xml_schema_tokenized_type_impl xml_schema_tokenized_type_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
 */ 
struct xml_schema_tokenized_type_impl
{
    xml_schema_tokenized_type_t tokenized_type;
    
    xml_schema_enum_t *schema_enum;
    
    axis2_array_list_t *members;
    
    axis2_hash_t *ht_super;
    
    xml_schema_types_t obj_type;
};

#define AXIS2_INTF_TO_IMPL(tokenized_type) \
        ((xml_schema_tokenized_type_impl_t *) tokenized_type)

/********************* function prototypes ***********************************/

axis2_status_t AXIS2_CALL 
xml_schema_tokenized_type_free(void *tokenized_type,
                        const axis2_env_t *env);

xml_schema_enum_t *AXIS2_CALL
xml_schema_tokenized_type_get_base_impl(void *tokenized_type,
                                const axis2_env_t *env);

axis2_array_list_t * AXIS2_CALL
xml_schema_tokenized_type_get_values(void *tokenized_type,
                        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_tokenized_type_get_type(void *tokenized_type,
                        const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL
xml_schema_tokenized_type_super_objs(void *tokenized_type,
                                    const axis2_env_t *env);

/***************************** end macros **************************************/

AXIS2_EXTERN xml_schema_tokenized_type_t * AXIS2_CALL
xml_schema_tokenized_type_create(const axis2_env_t *env,
                            axis2_char_t *value)
{
    xml_schema_tokenized_type_impl_t *tokenized_type_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    tokenized_type_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_tokenized_type_impl_t));
    if(!tokenized_type_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    tokenized_type_impl->schema_enum = NULL;
    tokenized_type_impl->members = NULL;
    tokenized_type_impl->obj_type = XML_SCHEMA_TOKENIZED_TYPE;
    tokenized_type_impl->tokenized_type.ops = NULL;
    tokenized_type_impl->ht_super = NULL;
    
    tokenized_type_impl->tokenized_type.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_tokenized_type_ops_t));
    if(!tokenized_type_impl->tokenized_type.ops)
    {
        xml_schema_tokenized_type_free(&(tokenized_type_impl->tokenized_type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    tokenized_type_impl->tokenized_type.ops->free =
        xml_schema_tokenized_type_free;
        
    tokenized_type_impl->tokenized_type.ops->get_base_impl = 
        xml_schema_tokenized_type_get_base_impl;
        
    tokenized_type_impl->tokenized_type.ops->get_values = 
            xml_schema_tokenized_type_get_values;
   
    tokenized_type_impl->tokenized_type.ops->get_type = 
            xml_schema_tokenized_type_get_type;
            
    tokenized_type_impl->tokenized_type.ops->super_objs = 
            xml_schema_tokenized_type_super_objs; 
                       

    tokenized_type_impl->members = axis2_array_list_create(env, 0);
    if(!tokenized_type_impl->members)
        return NULL;
        
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, 
        AXIS2_STRDUP("CDATA", env));
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, 
        AXIS2_STRDUP("IDREF", env));
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env,
        AXIS2_STRDUP("IDREFS", env));
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env,
        AXIS2_STRDUP("ENTITY", env));
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env,
        AXIS2_STRDUP("ENTITIES", env));
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, 
        AXIS2_STRDUP("NMTOKEN", env));
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env,
        AXIS2_STRDUP("NMTOKENS", env));
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, 
        AXIS2_STRDUP("NOTATION", env));
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, 
        AXIS2_STRDUP("ENUMERATION", env));
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, 
        AXIS2_STRDUP("QName", env));
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, 
        AXIS2_STRDUP("NCName", env));
    AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, 
        AXIS2_STRDUP("None", env));

    tokenized_type_impl->schema_enum = xml_schema_enum_create(env, value);

    tokenized_type_impl->ht_super = axis2_hash_make(env);
    if(!tokenized_type_impl->ht_super)
    {
        xml_schema_tokenized_type_free(&(tokenized_type_impl->tokenized_type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    axis2_hash_set(tokenized_type_impl->ht_super , "XML_SCHEMA_TOKENIZED_TYPE",
        AXIS2_HASH_KEY_STRING, &(tokenized_type_impl->tokenized_type));
    
    axis2_hash_set(tokenized_type_impl->ht_super, "XML_SCHEMA_ENUM",
        AXIS2_HASH_KEY_STRING, tokenized_type_impl->schema_enum);
    
    status = xml_schema_enum_resolve_methods(
            &(tokenized_type_impl->tokenized_type.base), env, tokenized_type_impl->schema_enum, 
            xml_schema_tokenized_type_super_objs,
            xml_schema_tokenized_type_get_type,
            xml_schema_tokenized_type_free); 

    return &(tokenized_type_impl->tokenized_type);
}

axis2_status_t AXIS2_CALL
xml_schema_tokenized_type_free(void *tokenized_type,
                              const axis2_env_t *env)
{
    xml_schema_tokenized_type_impl_t *tokenized_type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    tokenized_type_impl = AXIS2_INTF_TO_IMPL(tokenized_type);

    if(tokenized_type_impl->members)
    {
        AXIS2_ARRAY_LIST_FREE(tokenized_type_impl->members, env);
        tokenized_type_impl->members = NULL;
    }
    
    if(tokenized_type_impl->schema_enum)
    {
        int size = 0;
        int i    = 0;
        size = AXIS2_ARRAY_LIST_SIZE(tokenized_type_impl->members, env);
        for(i = 0; i < size ; i++)
        {
            axis2_char_t *value = NULL;
            value =(axis2_char_t* ) AXIS2_ARRAY_LIST_GET(tokenized_type_impl->members, env, i);
            if(NULL != value)
            {
                AXIS2_FREE(env->allocator, value);
                value = NULL;
            }
        }
        XML_SCHEMA_ENUM_FREE(tokenized_type_impl->schema_enum, env);
        tokenized_type_impl->schema_enum = NULL;
    }
    
    if(NULL != tokenized_type_impl->tokenized_type.ops)
    {
        AXIS2_FREE(env->allocator, tokenized_type_impl->tokenized_type.ops);
        tokenized_type_impl->tokenized_type.ops = NULL;
    }

    if(NULL != tokenized_type_impl)
    {
        AXIS2_FREE(env->allocator, tokenized_type_impl);
        tokenized_type_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

xml_schema_enum_t *AXIS2_CALL
xml_schema_tokenized_type_get_base_impl(void *tokenized_type,
                                const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(tokenized_type)->schema_enum;
}

axis2_array_list_t *AXIS2_CALL
xml_schema_tokenized_type_get_values(void *tokenized_type,
                                        const axis2_env_t *env)
{
    /*
    xml_schema_tokenized_type_impl_t *tokenized_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    ht_super = XML_SCHEMA_TOKENIZED_TYPE_SUPER_OBJS(tokenized_type, env);
    if(NULL != ht_super)
    {
        tokenized_impl = (xml_schema_tokenized_type_impl_t*)
            axis2_hash_get(tokenized_impl->ht_super, "XML_SCHEMA_TOKENIZED_TYPE",
                AXIS2_HASH_KEY_STRING);
        if(NULL != tokenized_impl)
        {
            return tokenized_impl->members;
        }                
    }
    */
    return AXIS2_INTF_TO_IMPL(tokenized_type)->members;
}

xml_schema_types_t AXIS2_CALL
xml_schema_tokenized_type_get_type(void *tokenized_type,
                        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(tokenized_type)->obj_type;
}                        

axis2_hash_t* AXIS2_CALL
xml_schema_tokenized_type_super_objs(void *tokenized_type,
                                    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(tokenized_type)->ht_super;
}                                    
