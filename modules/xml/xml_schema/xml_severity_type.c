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

#include <xml_schema/axis2_xml_severity_type.h>

typedef struct axis2_xml_severity_type_impl 
                axis2_xml_severity_type_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_severity_type_impl
{
    axis2_xml_severity_type_t severity_type;
    axis2_xml_schema_enum_t *schema_enum;
    axis2_hash_t *methods;
    axis2_array_list_t *members;
};

#define AXIS2_INTF_TO_IMPL(severity_type) \
        ((axis2_xml_severity_type_impl_t *) severity_type)

axis2_status_t AXIS2_CALL 
axis2_xml_severity_type_free(void *severity_type,
                        axis2_env_t **env);

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_severity_type_get_base_impl(void *severity_type,
                                axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_xml_severity_type_resolve_methods(
                                axis2_xml_severity_type_t *severity_type,
                                axis2_env_t **env,
                                axis2_xml_severity_type_t *severity_type_impl,
                                axis2_hash_t *methods);

axis2_array_list_t *AXIS2_CALL
axis2_xml_severity_type_get_values(void *severity_type,
                        axis2_env_t **env);


AXIS2_DECLARE(axis2_xml_severity_type_t *)
axis2_xml_severity_type_create(axis2_env_t **env,
                            axis2_char_t *value)
{
    axis2_xml_severity_type_impl_t *severity_type_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    severity_type_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_severity_type_impl_t));
    if(!severity_type_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                    

    severity_type_impl->schema_enum = NULL;
    severity_type_impl->methods = NULL;
    severity_type_impl->members = NULL;
    severity_type_impl->severity_type.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_severity_type_ops_t));
    if(!severity_type_impl->severity_type.ops)
    {
        axis2_xml_severity_type_free(&(severity_type_impl->severity_type), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
                        
    severity_type_impl->severity_type.ops->free = 
            axis2_xml_severity_type_free;
    severity_type_impl->severity_type.ops->get_base_impl = 
            axis2_xml_severity_type_get_base_impl;
    severity_type_impl->severity_type.ops->get_values = 
            axis2_xml_severity_type_get_values;

    severity_type_impl->members = axis2_array_list_create(env, 0);
    if(!severity_type_impl->members)
    {    
        axis2_xml_severity_type_free(&(severity_type_impl->severity_type), env);
        return NULL;
    }       
    AXIS2_ARRAY_LIST_ADD(severity_type_impl->members, env, "qualified");
    AXIS2_ARRAY_LIST_ADD(severity_type_impl->members, env, "unqualified");
    AXIS2_ARRAY_LIST_ADD(severity_type_impl->members, env, "none");

    severity_type_impl->methods = axis2_hash_make(env);
    if(!severity_type_impl->methods)
    {
        axis2_xml_severity_type_free(&(severity_type_impl->severity_type), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(severity_type_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_severity_type_free);
    axis2_hash_set(severity_type_impl->methods, "get_values", AXIS2_HASH_KEY_STRING, 
            axis2_xml_severity_type_get_values);

    severity_type_impl->schema_enum = axis2_xml_schema_enum_create(env, value);
    if(!severity_type_impl->schema_enum)
    {
        axis2_xml_severity_type_free(&(severity_type_impl->severity_type), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    status = axis2_xml_schema_enum_resolve_methods(
            &(severity_type_impl->severity_type.base), env, severity_type_impl->schema_enum, 
            severity_type_impl->methods); 

    return &(severity_type_impl->severity_type);
}

axis2_status_t AXIS2_CALL
axis2_xml_severity_type_free(void *severity_type,
                                    axis2_env_t **env)
{
    axis2_xml_severity_type_impl_t *severity_type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    severity_type_impl = AXIS2_INTF_TO_IMPL(severity_type);

    if(severity_type_impl->members)
    {
        int size = 0;
        int i    = 0;
        size = AXIS2_ARRAY_LIST_SIZE(severity_type_impl->members, env);
        for(i = 0; i < size ; i++)
        {
            axis2_char_t *value = NULL;
            value =(axis2_char_t* ) AXIS2_ARRAY_LIST_GET(severity_type_impl->members, env, i);
            if(NULL != value)
            {
                AXIS2_FREE((*env)->allocator, value);
                value = NULL;
            }
        }
        AXIS2_ARRAY_LIST_FREE(severity_type_impl->members, env);
        severity_type_impl->members = NULL;
    }
    
    if(severity_type_impl->schema_enum)
    {
        AXIS2_XML_SCHEMA_ENUM_FREE(severity_type_impl->schema_enum, env);
        severity_type_impl->schema_enum = NULL;
    }
    
    if((&(severity_type_impl->severity_type))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(severity_type_impl->severity_type))->ops);
        (&(severity_type_impl->severity_type))->ops = NULL;
    }

    if(severity_type_impl)
    {
        AXIS2_FREE((*env)->allocator, severity_type_impl);
        severity_type_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_severity_type_get_base_impl(void *severity_type,
                                axis2_env_t **env)
{
    axis2_xml_severity_type_impl_t *severity_type_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    severity_type_impl = AXIS2_INTF_TO_IMPL(severity_type);
    return severity_type_impl->schema_enum;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_severity_type_resolve_methods(
                                axis2_xml_severity_type_t *severity_type,
                                axis2_env_t **env,
                                axis2_xml_severity_type_t *severity_type_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_severity_type_impl_t *severity_type_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, severity_type_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    severity_type_impl_l = (axis2_xml_severity_type_impl_t *) severity_type_impl;
    
    severity_type->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_severity_type_ops_t));
    severity_type->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    severity_type->ops->get_base_impl = 
            severity_type_impl_l->severity_type.ops->get_base_impl;
    severity_type->ops->get_values = 
            severity_type_impl_l->severity_type.ops->get_values;
    
    return axis2_xml_schema_enum_resolve_methods(&(severity_type->base), 
            env, severity_type_impl_l->schema_enum, methods);
}

axis2_array_list_t* AXIS2_CALL
axis2_xml_severity_type_get_values(void *severity_type,
                                axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(severity_type)->members;
}

