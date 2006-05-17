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

#include <xml_schema/axis2_xml_schema_choice.h>
#include <xml_schema/axis2_xml_schema_group_base.h>

typedef struct axis2_xml_schema_choice_impl 
                axis2_xml_schema_choice_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_choice_impl
{
    axis2_xml_schema_choice_t choice;
    
    axis2_xml_schema_group_base_t *base;
    
    axis2_hash_t *methods;
    
    axis2_xml_schema_obj_collection_t *items;
};

#define AXIS2_INTF_TO_IMPL(choice) \
        ((axis2_xml_schema_choice_impl_t *) choice)

/********************* function prototypes ************************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_choice_free(void *choice,
                        axis2_env_t **env);

axis2_xml_schema_group_base_t *AXIS2_CALL
axis2_xml_schema_choice_get_base_impl(void *choice,
                                        axis2_env_t **env);

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_choice_get_items(void *choice,
                                axis2_env_t **env);

/******************** end function prototypes ********************************/

AXIS2_DECLARE(axis2_xml_schema_choice_t *)
axis2_xml_schema_choice_create(axis2_env_t **env)
{
    axis2_xml_schema_choice_impl_t *choice_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    choice_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_choice_impl_t));
    if(!choice_impl)
    {
        AXIS2_ERROR_SET((*env)->error , 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    choice_impl->base = NULL;
    choice_impl->methods = NULL;
    choice_impl->items = NULL;
    choice_impl->choice.base.ops = NULL;
    choice_impl->choice.ops = NULL;
    
    choice_impl->choice.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_choice_ops_t));
    if(!choice_impl->choice.ops)
    {
        axis2_xml_schema_choice_free(&(choice_impl->choice), env);
        AXIS2_ERROR_SET((*env)->error , 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    choice_impl->choice.ops->free = 
        axis2_xml_schema_choice_free;
    choice_impl->choice.ops->get_base_impl = 
        axis2_xml_schema_choice_get_base_impl;
    choice_impl->choice.ops->get_items = 
        axis2_xml_schema_choice_get_items;
   
    choice_impl->methods = axis2_hash_make(env);
    if(!choice_impl->methods)
    {
        axis2_xml_schema_choice_free(&(choice_impl->choice), env);
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    axis2_hash_set(choice_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
       axis2_xml_schema_choice_free);
    axis2_hash_set(choice_impl->methods, "get_items", 
       AXIS2_HASH_KEY_STRING, axis2_xml_schema_choice_get_items);
    
    choice_impl->base = axis2_xml_schema_group_base_create(env);
    if(!choice_impl->base)
    {
        axis2_xml_schema_choice_free(&(choice_impl->choice), env);
        return NULL;
    }
    status = axis2_xml_schema_group_base_resolve_methods(
            &(choice_impl->choice.base), env, choice_impl->base, 
            choice_impl->methods);
    return &(choice_impl->choice);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_choice_free(void *choice,
                                    axis2_env_t **env)
{
    axis2_xml_schema_choice_impl_t *choice_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    choice_impl = AXIS2_INTF_TO_IMPL(choice);

    if(NULL != choice_impl->items)
    {
        AXIS2_XML_SCHEMA_OBJ_COLLECTION_FREE(choice_impl->items, env);
        choice_impl->items = NULL;
    }
    
    if(NULL != choice_impl->methods)
    {
        axis2_hash_free(choice_impl->methods, env);
        choice_impl->methods = NULL;
    }

    if(NULL != choice_impl->base)
    {
        AXIS2_XML_SCHEMA_GROUP_BASE_FREE(choice_impl->base, env);
        choice_impl->base = NULL;
    }
    
    if(NULL != choice_impl->choice.ops)
    {
        AXIS2_FREE((*env)->allocator, choice_impl->choice.ops);
        choice_impl->choice.ops = NULL;
    }
    if(NULL != choice_impl->choice.base.ops)
    {
        AXIS2_FREE((*env)->allocator, choice_impl->choice.base.ops);
        choice_impl->choice.base.ops = NULL;
    }
    if(NULL != choice_impl)
    {
        AXIS2_FREE((*env)->allocator, choice_impl);
        choice_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_group_base_t *AXIS2_CALL
axis2_xml_schema_choice_get_base_impl(void *choice,
                                axis2_env_t **env)
{
    axis2_xml_schema_choice_impl_t *choice_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    choice_impl = AXIS2_INTF_TO_IMPL(choice);
    return choice_impl->base;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_choice_resolve_methods(
                                axis2_xml_schema_choice_t *choice,
                                axis2_env_t **env,
                                axis2_xml_schema_choice_t *choice_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_choice_impl_t *choice_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, choice_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    choice_impl_l = (axis2_xml_schema_choice_impl_t *) choice_impl;
    
    choice->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_choice_ops_t));
    choice->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    choice->ops->get_base_impl = 
            choice_impl_l->choice.ops->get_base_impl;
    choice->ops->get_items = 
            choice_impl_l->choice.ops->get_items;
    
    return axis2_xml_schema_group_base_resolve_methods(&(choice->base), 
            env, choice_impl_l->base, methods);
}

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_choice_get_items(void *choice,
                                    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(choice)->items;
}

