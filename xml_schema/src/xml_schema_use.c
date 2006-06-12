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

#include <axis2_xml_schema_use.h>
#include <axis2_utils.h>

typedef struct axis2_xml_schema_use_impl axis2_xml_schema_use_impl_t;

/** 
 * @brief axis2_xml_schema_use_impl
 */ 
struct axis2_xml_schema_use_impl
{
    axis2_xml_schema_use_t use;
    
    axis2_xml_schema_enum_t *schema_enum;
    
    axis2_xml_schema_types_t obj_type; 
    
    axis2_hash_t *ht_super;
    
    axis2_hash_t *methods;
    
    axis2_array_list_t *members;
};

#define AXIS2_INTF_TO_IMPL(use) ((axis2_xml_schema_use_impl_t *) use)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_use_free(void *use,
                        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
axis2_xml_schema_use_super_objs(
        void *use,
        const axis2_env_t *env);

axis2_xml_schema_types_t AXIS2_CALL 
axis2_xml_schema_use_get_type(
        void *use,
        const axis2_env_t *env);

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_use_get_base_impl(void *use,
                                const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_use_resolve_methods(
                                axis2_xml_schema_use_t *use,
                                const axis2_env_t *env,
                                axis2_xml_schema_use_t *use_impl,
                                axis2_hash_t *methods);

axis2_array_list_t * AXIS2_CALL
axis2_xml_schema_use_get_values(void *use,
                        const axis2_env_t *env);


AXIS2_EXTERN axis2_xml_schema_use_t * AXIS2_CALL
axis2_xml_schema_use_create(const axis2_env_t *env,
                            axis2_char_t *value)
{
    axis2_xml_schema_use_impl_t *use_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    use_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_use_impl_t));
    if(!use_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                    

    use_impl->schema_enum = NULL;
    use_impl->obj_type = AXIS2_XML_SCHEMA_USE;
    use_impl->ht_super = NULL;
    use_impl->methods = NULL;
    use_impl->members = NULL;
    use_impl->use.base.ops = NULL;
    use_impl->use.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_use_ops_t));
    if(!use_impl->use.ops)
    {
        axis2_xml_schema_use_free(&(use_impl->use), env);
        
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    use_impl->use.ops->free = axis2_xml_schema_use_free;
    use_impl->use.ops->super_objs = axis2_xml_schema_use_super_objs;
    use_impl->use.ops->get_type = axis2_xml_schema_use_get_type;
    use_impl->use.ops->get_base_impl = axis2_xml_schema_use_get_base_impl;
    use_impl->use.ops->get_values = 
            axis2_xml_schema_use_get_values;
   
    use_impl->members = axis2_array_list_create(env, 0);
    if(!use_impl->members)
        return NULL;
    AXIS2_ARRAY_LIST_ADD(use_impl->members, env, 
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_NONE, env));
    AXIS2_ARRAY_LIST_ADD(use_impl->members, env, 
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_OPTIONAL, env));
    AXIS2_ARRAY_LIST_ADD(use_impl->members, env, 
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_PROHIBITED, env));
    AXIS2_ARRAY_LIST_ADD(use_impl->members, env, 
        AXIS2_STRDUP(AXIS2_XML_SCHEMA_CONST_REQUIRED, env));

    use_impl->methods = axis2_hash_make(env);
    if(!use_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(use_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_use_free);

    axis2_hash_set(use_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_use_super_objs);

    axis2_hash_set(use_impl->methods, "get_type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_use_get_type);

    axis2_hash_set(use_impl->methods, "get_values", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_use_get_values);

    use_impl->schema_enum = axis2_xml_schema_enum_create(env, value);

    use_impl->ht_super = axis2_hash_make(env);
    if(!use_impl->ht_super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(use_impl->ht_super, "AXIS2_XML_SCHEMA_USE", AXIS2_HASH_KEY_STRING, 
            &(use_impl->use));
    axis2_hash_set(use_impl->ht_super, "AXIS2_XML_SCHEMA_ENUM", AXIS2_HASH_KEY_STRING, 
            use_impl->schema_enum);

    status = axis2_xml_schema_enum_resolve_methods(
            &(use_impl->use.base), env, use_impl->schema_enum, 
            use_impl->methods); 

    return &(use_impl->use);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_use_free(void *use,
                                    const axis2_env_t *env)
{
    axis2_xml_schema_use_impl_t *use_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    use_impl = AXIS2_INTF_TO_IMPL(use);

    if(use_impl->members)
    {
        int size = 0;
        int i    = 0;
        size = AXIS2_ARRAY_LIST_SIZE(use_impl->members, env);
        for(i = 0; i < size ; i++)
        {
            axis2_char_t *value = NULL;
            value =(axis2_char_t* ) AXIS2_ARRAY_LIST_GET(use_impl->members, env, i);
            if(NULL != value)
            {
                AXIS2_FREE(env->allocator, value);
                value = NULL;
            }
        }
        AXIS2_ARRAY_LIST_FREE(use_impl->members, env);
        use_impl->members = NULL;
    }
    
    if(use_impl->schema_enum)
    {
        AXIS2_XML_SCHEMA_ENUM_FREE(use_impl->schema_enum, env);
        use_impl->schema_enum = NULL;
    }
    
    if(NULL != use_impl->ht_super)
    {
        axis2_hash_free(use_impl->ht_super, env);
        use_impl->ht_super = NULL;
    }
    
    if(NULL != use_impl->methods)
    {
        axis2_hash_free(use_impl->methods, env);
        use_impl->methods = NULL;
    }
    if(NULL != use_impl->use.base.ops)
    {
        AXIS2_FREE(env->allocator, use_impl->use.base.ops);
        use_impl->use.base.ops = NULL;
    }

    if(NULL != use_impl->use.ops)
    {
        AXIS2_FREE(env->allocator, use_impl->use.ops);
        use_impl->use.ops = NULL;
    }

    AXIS2_FREE(env->allocator, use_impl);
    use_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_use_super_objs(
        void *use,
        const axis2_env_t *env)
{
    axis2_xml_schema_use_impl_t *use_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    use_impl = AXIS2_INTF_TO_IMPL(use);
    return use_impl->ht_super;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_use_get_type(
        void *use,
        const axis2_env_t *env)
{
    axis2_xml_schema_use_impl_t *use_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    use_impl = AXIS2_INTF_TO_IMPL(use);
    return use_impl->obj_type;
}

axis2_xml_schema_enum_t *AXIS2_CALL
axis2_xml_schema_use_get_base_impl(void *use,
                                const axis2_env_t *env)
{
    axis2_xml_schema_use_impl_t *use_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    use_impl = AXIS2_INTF_TO_IMPL(use);

    return use_impl->schema_enum;
}

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_use_get_values(void *use,
                                        const axis2_env_t *env)
{
    axis2_xml_schema_use_impl_t *use_impl = NULL;
    axis2_hash_t *super = NULL;

    super = AXIS2_XML_SCHEMA_USE_SUPER_OBJS(use, env);
    use_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_USE",
                AXIS2_HASH_KEY_STRING));
    if(NULL != use_impl)                
        return use_impl->members;
    return NULL;        
}

