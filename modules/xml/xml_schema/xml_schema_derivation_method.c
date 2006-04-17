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

#include <axis2_xml_schema_derivation_method.h>

typedef struct axis2_xml_schema_derivation_method_impl axis2_xml_schema_derivation_method_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_derivation_method_impl
{
    axis2_xml_schema_derivation_method_t derivation_method;
    axis2_xml_schema_enum_t *schema_enum;
    axis2_hash_t *methods;
    axis2_array_list_t *members;
};

#define INTF_TO_IMPL(derivation_method) ((axis2_xml_schema_derivation_method_impl_t *) derivation_method)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_derivation_method_free(void *derivation_method,
                        axis2_env_t **env);

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_derivation_method_get_base_impl(void *derivation_method,
                                axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_derivation_method_resolve_methods(
                                axis2_xml_schema_derivation_method_t *derivation_method,
                                axis2_env_t **env,
                                axis2_xml_schema_derivation_method_t *derivation_method_impl,
                                axis2_hash_t *methods);

axis2_array_list_t *
axis2_xml_schema_derivation_method_get_values(void *derivation_method,
                        axis2_env_t **env);


AXIS2_DECLARE(axis2_xml_schema_derivation_method_t *)
axis2_xml_schema_derivation_method_create(axis2_env_t **env,
                            axis2_char_t *value)
{
    axis2_xml_schema_derivation_method_impl_t *derivation_method_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    derivation_method_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_derivation_method_impl_t));

    derivation_method_impl->schema_enum = NULL;
    derivation_method_impl->methods = NULL;
    derivation_method_impl->members = NULL;
    derivation_method_impl->derivation_method.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_derivation_method_ops_t));

    derivation_method_impl->derivation_method.ops->free = axis2_xml_schema_derivation_method_free;
    derivation_method_impl->derivation_method.ops->get_base_impl = axis2_xml_schema_derivation_method_get_base_impl;
    derivation_method_impl->derivation_method.ops->get_values = 
            axis2_xml_schema_derivation_method_get_values;
   
    if(!derivation_method_impl->members)
    {
        derivation_method_impl->members = axis2_array_list_create(env, 0);
        if(!derivation_method_impl->members)
            return NULL;
        AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env, "All");
        AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env, "Empty");
        AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env, "Extension");
        AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env, "List");
        AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env, "None");
        AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env, "Restriction");
        AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env, "Substitution");
        AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env, "Union");
    }
    derivation_method_impl->methods = axis2_hash_make(env);
    if(!derivation_method_impl->methods)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(derivation_method_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_derivation_method_free);
    axis2_hash_set(derivation_method_impl->methods, "get_values", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_derivation_method_get_values);

    derivation_method_impl->schema_enum = axis2_xml_schema_enum_create(env, value);
    status = axis2_xml_schema_enum_resolve_methods(
            &(derivation_method_impl->derivation_method.base), env, derivation_method_impl->schema_enum, 
            derivation_method_impl->methods); 

    return &(derivation_method_impl->derivation_method);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_derivation_method_free(void *derivation_method,
                                    axis2_env_t **env)
{
    axis2_xml_schema_derivation_method_impl_t *derivation_method_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    derivation_method_impl = INTF_TO_IMPL(derivation_method);

    if(derivation_method_impl->members)
    {
        AXIS2_ARRAY_LIST_FREE(derivation_method_impl->members, env);
        derivation_method_impl->members = NULL;
    }
    
    if(derivation_method_impl->schema_enum)
    {
        AXIS2_XML_SCHEMA_ENUM_FREE(derivation_method_impl->schema_enum, env);
        derivation_method_impl->schema_enum = NULL;
    }
    
    if((&(derivation_method_impl->derivation_method))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(derivation_method_impl->derivation_method))->ops);
        (&(derivation_method_impl->derivation_method))->ops = NULL;
    }

    if(derivation_method_impl)
    {
        AXIS2_FREE((*env)->allocator, derivation_method_impl);
        derivation_method_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_derivation_method_get_base_impl(void *derivation_method,
                                axis2_env_t **env)
{
    axis2_xml_schema_derivation_method_impl_t *derivation_method_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    derivation_method_impl = INTF_TO_IMPL(derivation_method);

    return derivation_method_impl->schema_enum;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_derivation_method_resolve_methods(
                                axis2_xml_schema_derivation_method_t *derivation_method,
                                axis2_env_t **env,
                                axis2_xml_schema_derivation_method_t *derivation_method_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_derivation_method_impl_t *derivation_method_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, derivation_method_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    derivation_method_impl_l = (axis2_xml_schema_derivation_method_impl_t *) derivation_method_impl;
    
    derivation_method->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_derivation_method_ops_t));
    derivation_method->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    derivation_method->ops->get_base_impl = 
            derivation_method_impl_l->derivation_method.ops->get_base_impl;
    derivation_method->ops->get_values = 
            derivation_method_impl_l->derivation_method.ops->get_values;
    
    return axis2_xml_schema_enum_resolve_methods(&(derivation_method->base), 
            env, derivation_method_impl_l->schema_enum, methods);
}

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_derivation_method_get_values(void *derivation_method,
                                        axis2_env_t **env)
{
    return INTF_TO_IMPL(derivation_method)->members;
}

