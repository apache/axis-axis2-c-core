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

#include <xml_schema/axis2_xml_schema_constants.h>
#include <xml_schema/axis2_xml_schema_derivation_method.h>
#include <axis2_utils.h>

typedef struct axis2_xml_schema_derivation_method_impl 
               axis2_xml_schema_derivation_method_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
 */ 
struct axis2_xml_schema_derivation_method_impl
{
    axis2_xml_schema_derivation_method_t derivation_method;
    
    axis2_xml_schema_enum_t *schema_enum;
    
    axis2_hash_t *methods;
    
    axis2_xml_schema_types_t type;
    
    axis2_array_list_t *members;
    
    axis2_hash_t *ht_super;
};

#define AXIS2_INTF_TO_IMPL(derivation_method) \
        ((axis2_xml_schema_derivation_method_impl_t *) derivation_method)

/************************* function prototypes ************************************/
axis2_hash_t* AXIS2_CALL
axis2_xml_schema_derivation_method_super_objs(void *derivation_method,
                                              const axis2_env_t *env);

axis2_xml_schema_types_t AXIS2_CALL 
axis2_xml_schema_derivation_method_type(void *derivation_method,
                                        const axis2_env_t *env); 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_derivation_method_free(void *derivation_method,
                        const axis2_env_t *env);

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_derivation_method_get_base_impl(void *derivation_method,
                                const axis2_env_t *env);

axis2_array_list_t * AXIS2_CALL
axis2_xml_schema_derivation_method_get_values(void *derivation_method,
                        const axis2_env_t *env);


/************************** end function prototypes ********************************/

AXIS2_EXTERN axis2_xml_schema_derivation_method_t * AXIS2_CALL
axis2_xml_schema_derivation_method_create(const axis2_env_t *env,
                            axis2_char_t *value)
{
    axis2_xml_schema_derivation_method_impl_t *derivation_method_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    derivation_method_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_derivation_method_impl_t));
    if(!derivation_method_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    derivation_method_impl->schema_enum = NULL;
    derivation_method_impl->methods = NULL;
    derivation_method_impl->members = NULL;
    derivation_method_impl->ht_super = NULL;
    derivation_method_impl->type = AXIS2_XML_SCHEMA_DERIVATION_METHOD;
    derivation_method_impl->derivation_method.base.ops = NULL;
    derivation_method_impl->derivation_method.ops = NULL;
    
    derivation_method_impl->derivation_method.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_derivation_method_ops_t));
    if(!derivation_method_impl->derivation_method.ops)
    {
        axis2_xml_schema_derivation_method_free(
            &(derivation_method_impl->derivation_method), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);            
        return NULL;
    }

    derivation_method_impl->derivation_method.ops->free = 
        axis2_xml_schema_derivation_method_free;
    derivation_method_impl->derivation_method.ops->get_base_impl =
        axis2_xml_schema_derivation_method_get_base_impl;
    derivation_method_impl->derivation_method.ops->get_values = 
            axis2_xml_schema_derivation_method_get_values;
    derivation_method_impl->derivation_method.ops->super_objs = 
            axis2_xml_schema_derivation_method_super_objs;
    derivation_method_impl->derivation_method.ops->type =
            axis2_xml_schema_derivation_method_type;
                                    
    derivation_method_impl->members = axis2_array_list_create(env, 0);
    
    if(!derivation_method_impl->members)
    {
        axis2_xml_schema_derivation_method_free(
            &(derivation_method_impl->derivation_method), env);
        return NULL;
    }   
    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_ALL, env));
        
    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env, 
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_EMPTY, env));
        
    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env, 
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_EXTENSION, env));
        
    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_LIST, env));
        
    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env, 
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_NONE, env));
        
    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_RESTRICTION, env));
        
    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_SUBSTITUTION, env));
        
    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_UNION, env));

    derivation_method_impl->methods = axis2_hash_make(env);

    if(!derivation_method_impl->methods)
    {
        axis2_xml_schema_derivation_method_free(
            &(derivation_method_impl->derivation_method), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    axis2_hash_set(derivation_method_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_derivation_method_free);
            
    axis2_hash_set(derivation_method_impl->methods, "get_values", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_derivation_method_get_values);
    
    axis2_hash_set(derivation_method_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_derivation_method_super_objs);
            
    axis2_hash_set(derivation_method_impl->methods, "type", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_derivation_method_type);
    
    derivation_method_impl->schema_enum = 
        axis2_xml_schema_enum_create(env, value);
    if(!derivation_method_impl->schema_enum)
    {
        axis2_xml_schema_derivation_method_free(
            &(derivation_method_impl->derivation_method), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }   
    
    derivation_method_impl->ht_super = axis2_hash_make(env);
    axis2_hash_set(derivation_method_impl->ht_super, "AXIS2_XML_SCHEMA_DERIVATION_METHOD",
                    AXIS2_HASH_KEY_STRING, &(derivation_method_impl->derivation_method));
                    
    axis2_hash_set(derivation_method_impl->ht_super, "AXIS2_XML_SCHEMA_ENUM",
                    AXIS2_HASH_KEY_STRING, derivation_method_impl->schema_enum);                       
    status = axis2_xml_schema_enum_resolve_methods(
            &(derivation_method_impl->derivation_method.base), 
            env, derivation_method_impl->schema_enum, 
            derivation_method_impl->methods); 

    return &(derivation_method_impl->derivation_method);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_derivation_method_free(void *derivation_method,
                                    const axis2_env_t *env)
{
    axis2_xml_schema_derivation_method_impl_t *derivation_method_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    derivation_method_impl = AXIS2_INTF_TO_IMPL(derivation_method);

    if(NULL != derivation_method_impl->members)
    {
        int size = 0;
        int i    = 0;
        size = AXIS2_ARRAY_LIST_SIZE(derivation_method_impl->members, env);
        for(i = 0; i < size ; i++)
        {
            axis2_char_t *value = NULL;
            value =(axis2_char_t* ) AXIS2_ARRAY_LIST_GET(derivation_method_impl->members, env, i);
            if(NULL != value)
            {
                AXIS2_FREE(env->allocator, value);
                value = NULL;
            }
        }
        AXIS2_ARRAY_LIST_FREE(derivation_method_impl->members, env);
        derivation_method_impl->members = NULL;
    }
    
    if(NULL != derivation_method_impl->schema_enum)
    {
        AXIS2_XML_SCHEMA_ENUM_FREE(derivation_method_impl->schema_enum, env);
        derivation_method_impl->schema_enum = NULL;
    }
    
    if(NULL != derivation_method_impl->derivation_method.ops)
    {
        AXIS2_FREE(env->allocator, derivation_method_impl->derivation_method.ops);
        derivation_method_impl->derivation_method.ops = NULL;
    }
    if(NULL != derivation_method_impl->derivation_method.base.ops)
    {
        AXIS2_FREE(env->allocator, derivation_method_impl->derivation_method.base.ops);
        derivation_method_impl->derivation_method.base.ops = NULL;
    }
    if(NULL != derivation_method_impl)
    {
        AXIS2_FREE(env->allocator, derivation_method_impl);
        derivation_method_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_derivation_method_get_base_impl(void *derivation_method,
                                const axis2_env_t *env)
{
    axis2_xml_schema_derivation_method_impl_t *derivation_method_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    derivation_method_impl = AXIS2_INTF_TO_IMPL(derivation_method);

    return derivation_method_impl->schema_enum;
}

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_derivation_method_get_values(void *derivation_method,
                                        const axis2_env_t *env)
{
/*
    axis2_xml_schema_derivation_method_impl_t *deri_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    ht_super = AXIS2_XML_SCHEMA_DERIVATION_METHOD_SUPER_OBJS(derivation_method, env);
    if(NULL != ht_super)
    {
        deri_impl = (axis2_xml_schema_derivation_method_impl_t*)
            axis2_hash_get(ht_super, "AXIS2_XML_SCHEMA_DERIVATION_METHOD", AXIS2_HASH_KEY_STRING);
        if(NULL != deri_impl)
            return deri_impl->members;            
    }
*/    
    return AXIS2_INTF_TO_IMPL(derivation_method)->members;
}

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_derivation_method_super_objs(void *derivation_method,
                                              const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(derivation_method)->ht_super;
}                                              

axis2_xml_schema_types_t AXIS2_CALL 
axis2_xml_schema_derivation_method_type(void *derivation_method,
                                        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(derivation_method)->type;
}                                        
