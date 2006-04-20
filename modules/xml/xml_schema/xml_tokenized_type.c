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

#include <xml_schema/axis2_xml_tokenized_type.h>

typedef struct axis2_xml_tokenized_type_impl axis2_xml_tokenized_type_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_tokenized_type_impl
{
    axis2_xml_tokenized_type_t tokenized_type;
    axis2_xml_schema_enum_t *schema_enum;
    axis2_hash_t *methods;
    axis2_array_list_t *members;
};

#define INTF_TO_IMPL(tokenized_type) ((axis2_xml_tokenized_type_impl_t *) tokenized_type)

axis2_status_t AXIS2_CALL 
axis2_xml_tokenized_type_free(void *tokenized_type,
                        axis2_env_t **env);

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_tokenized_type_get_base_impl(void *tokenized_type,
                                axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_xml_tokenized_type_resolve_methods(
                                axis2_xml_tokenized_type_t *tokenized_type,
                                axis2_env_t **env,
                                axis2_xml_tokenized_type_t *tokenized_type_impl,
                                axis2_hash_t *methods);

axis2_array_list_t * AXIS2_CALL
axis2_xml_tokenized_type_get_values(void *tokenized_type,
                        axis2_env_t **env);


AXIS2_DECLARE(axis2_xml_tokenized_type_t *)
axis2_xml_tokenized_type_create(axis2_env_t **env,
                            axis2_char_t *value)
{
    axis2_xml_tokenized_type_impl_t *tokenized_type_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    tokenized_type_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_tokenized_type_impl_t));

    tokenized_type_impl->schema_enum = NULL;
    tokenized_type_impl->methods = NULL;
    tokenized_type_impl->members = NULL;
    tokenized_type_impl->tokenized_type.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_tokenized_type_ops_t));

    tokenized_type_impl->tokenized_type.ops->free = axis2_xml_tokenized_type_free;
    tokenized_type_impl->tokenized_type.ops->get_base_impl = axis2_xml_tokenized_type_get_base_impl;
    tokenized_type_impl->tokenized_type.ops->get_values = 
            axis2_xml_tokenized_type_get_values;
   
    if(!tokenized_type_impl->members)
    {
        tokenized_type_impl->members = axis2_array_list_create(env, 0);
        if(!tokenized_type_impl->members)
            return NULL;
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "CDATA");
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "IDREF");
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "IDREFS");
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "ENTITY");
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "ENTITIES");
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "NMTOKEN");
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "NMTOKENS");
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "NOTATION");
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "ENUMERATION");
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "QName");
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "NCName");
        AXIS2_ARRAY_LIST_ADD(tokenized_type_impl->members, env, "None");
    }
    tokenized_type_impl->methods = axis2_hash_make(env);
    if(!tokenized_type_impl->methods)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(tokenized_type_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_tokenized_type_free);
    axis2_hash_set(tokenized_type_impl->methods, "get_values", AXIS2_HASH_KEY_STRING, 
            axis2_xml_tokenized_type_get_values);

    tokenized_type_impl->schema_enum = axis2_xml_schema_enum_create(env, value);
    status = axis2_xml_schema_enum_resolve_methods(
            &(tokenized_type_impl->tokenized_type.base), env, tokenized_type_impl->schema_enum, 
            tokenized_type_impl->methods); 

    return &(tokenized_type_impl->tokenized_type);
}

axis2_status_t AXIS2_CALL
axis2_xml_tokenized_type_free(void *tokenized_type,
                                        axis2_env_t **env)
{
    axis2_xml_tokenized_type_impl_t *tokenized_type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    tokenized_type_impl = INTF_TO_IMPL(tokenized_type);

    if(tokenized_type_impl->members)
    {
        AXIS2_ARRAY_LIST_FREE(tokenized_type_impl->members, env);
        tokenized_type_impl->members = NULL;
    }
    
    if(tokenized_type_impl->schema_enum)
    {
        AXIS2_XML_SCHEMA_ENUM_FREE(tokenized_type_impl->schema_enum, env);
        tokenized_type_impl->schema_enum = NULL;
    }
    
    if((&(tokenized_type_impl->tokenized_type))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(tokenized_type_impl->tokenized_type))->ops);
        (&(tokenized_type_impl->tokenized_type))->ops = NULL;
    }

    if(tokenized_type_impl)
    {
        AXIS2_FREE((*env)->allocator, tokenized_type_impl);
        tokenized_type_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_tokenized_type_get_base_impl(void *tokenized_type,
                                axis2_env_t **env)
{
    axis2_xml_tokenized_type_impl_t *tokenized_type_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    tokenized_type_impl = INTF_TO_IMPL(tokenized_type);

    return tokenized_type_impl->schema_enum;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_tokenized_type_resolve_methods(
                        axis2_xml_tokenized_type_t *tokenized_type,
                        axis2_env_t **env,
                        axis2_xml_tokenized_type_t *tokenized_type_impl,
                        axis2_hash_t *methods)
{
    axis2_xml_tokenized_type_impl_t *tokenized_type_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, tokenized_type_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    tokenized_type_impl_l = (axis2_xml_tokenized_type_impl_t *) 
        tokenized_type_impl;
    
    tokenized_type->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_tokenized_type_ops_t));
    tokenized_type->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    tokenized_type->ops->get_base_impl = 
            tokenized_type_impl_l->tokenized_type.ops->get_base_impl;
    tokenized_type->ops->get_values = 
            tokenized_type_impl_l->tokenized_type.ops->get_values;
    
    return axis2_xml_schema_enum_resolve_methods(&(tokenized_type->base), 
            env, tokenized_type_impl_l->schema_enum, methods);
}

axis2_array_list_t *AXIS2_CALL
axis2_xml_tokenized_type_get_values(void *tokenized_type,
                                        axis2_env_t **env)
{
    return INTF_TO_IMPL(tokenized_type)->members;
}

