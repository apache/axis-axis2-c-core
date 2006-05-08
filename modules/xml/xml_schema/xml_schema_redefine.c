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

#include  <axis2_xml_schema_redefine.h>
#include  <axis2_xml_schema.h>

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
typedef struct axis2_xml_schema_redefine_impl
{
    axis2_xml_schema_redefine_t redefine;
    
    axis2_xml_schema_external_t *external;
        
    axis2_xml_schema_obj_collection_t *items;
    
    axis2_xml_schema_obj_table_t *attr_groups;
    
    axis2_xml_schema_obj_table_t *groups;
    
    axis2_xml_schema_obj_table_t *schema_types;
        
    axis2_hash_t *methods;

}axis2_xml_schema_redefine_impl_t;

#define AXIS2_INTF_TO_IMPL(redefine) \
        ((axis2_xml_schema_redefine_impl_t *) redefine)

/********************* function prototypes ***********************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_redefine_free(void *redefine,
                        axis2_env_t **env);

axis2_xml_schema_external_t *AXIS2_CALL
axis2_xml_schema_redefine_get_base_impl(void *redefine,
                                        axis2_env_t **env);

axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_attribute_group(void *redefine,
                                              axis2_env_t **env);
                
axis2_status_t AXIS2_CALL
axis2_xml_schema_redefine_set_attribute_group(void *redefine,
                axis2_env_t **env,
                axis2_xml_schema_obj_table_t *group);

   
                   
axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_redefine_get_items(void *redefine,
               axis2_env_t **env);
               
axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_schema_types(void *redefine,
                      axis2_env_t **env);
                      
axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_group(void *redefine, 
               axis2_env_t **env);                      
                      
    
/********************* end function prototypes ********************************/

AXIS2_DECLARE(axis2_xml_schema_redefine_t *)
axis2_xml_schema_redefine_create(axis2_env_t **env)
{
    axis2_xml_schema_redefine_impl_t *redefine_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    redefine_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_redefine_impl_t));
    if(!redefine_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    redefine_impl->external = NULL;
    redefine_impl->methods = NULL;
    redefine_impl->redefine.ops = NULL;
    redefine_impl->redefine.base.ops = NULL;
    redefine_impl->attr_groups = NULL;
    redefine_impl->groups = NULL;
    redefine_impl->schema_types = NULL;
    redefine_impl->items = NULL;
    
    redefine_impl->redefine.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_redefine_ops_t));

    if(!redefine_impl->redefine.ops)
    {
        axis2_xml_schema_redefine_free(&(redefine_impl->redefine), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    redefine_impl->redefine.ops->free = 
        axis2_xml_schema_redefine_free;
    redefine_impl->redefine.ops->get_base_impl = 
        axis2_xml_schema_redefine_get_base_impl;
        
    redefine_impl->redefine.ops->get_items = 
            axis2_xml_schema_redefine_get_items;
            
    redefine_impl->redefine.ops->get_group = 
            axis2_xml_schema_redefine_get_group;

    redefine_impl->methods = axis2_hash_make(env);
    if(!redefine_impl->methods)
    {
        axis2_xml_schema_redefine_free(&(redefine_impl->redefine), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(redefine_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_redefine_free);

    axis2_hash_set(redefine_impl->methods, "get_items", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_redefine_get_items);
    axis2_hash_set(redefine_impl->methods, "get_group", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_redefine_get_group);

    redefine_impl->external = axis2_xml_schema_external_create(env);
     if(!redefine_impl->external)
    {
        axis2_xml_schema_redefine_free(&(redefine_impl->redefine), env);
        return NULL;
    }
    status = axis2_xml_schema_external_resolve_methods(
            &(redefine_impl->redefine.base), env, redefine_impl->external, 
            redefine_impl->methods);

    return &(redefine_impl->redefine);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_redefine_free(void *redefine,
                                    axis2_env_t **env)
{
    axis2_xml_schema_redefine_impl_t *redefine_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    redefine_impl = AXIS2_INTF_TO_IMPL(redefine);

    if(NULL != redefine_impl->methods)
    {
        axis2_hash_free(redefine_impl->methods, env);
        redefine_impl->methods = NULL;
    }

    if(NULL != redefine_impl->external)
    {
        AXIS2_XML_SCHEMA_EXTERNAL_FREE(redefine_impl->external, env);
        redefine_impl->external = NULL;
    }
    
    if(NULL != redefine_impl->redefine.ops)
    {
        AXIS2_FREE((*env)->allocator, redefine_impl->redefine.ops);
        redefine_impl->redefine.ops = NULL;
    }
    if(NULL != redefine_impl->redefine.base.ops)
    {
        AXIS2_FREE((*env)->allocator, redefine_impl->redefine.base.ops);
        redefine_impl->redefine.base.ops = NULL;        
    }
    AXIS2_FREE((*env)->allocator, redefine_impl);
    redefine_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_external_t *AXIS2_CALL
axis2_xml_schema_redefine_get_base_impl(void *redefine,
                                axis2_env_t **env)
{
    axis2_xml_schema_redefine_impl_t *redefine_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    redefine_impl = AXIS2_INTF_TO_IMPL(redefine);

    return redefine_impl->external;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_redefine_resolve_methods(
                                axis2_xml_schema_redefine_t *redefine,
                                axis2_env_t **env,
                                axis2_xml_schema_redefine_t *redefine_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_redefine_impl_t *redefine_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, redefine_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    redefine_impl_l = (axis2_xml_schema_redefine_impl_t *) redefine_impl;
    
    redefine->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_redefine_ops_t));
            
    redefine->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
            
    redefine->ops->get_base_impl = 
        redefine_impl_l->redefine.ops->get_base_impl;
    redefine->ops->get_group = 
        redefine_impl_l->redefine.ops->get_group;
    redefine->ops->get_items = 
        redefine_impl_l->redefine.ops->get_items;
        
    return axis2_xml_schema_external_resolve_methods(&(redefine->base), 
            env, redefine_impl_l->external, methods);
}

axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_attribute_group(void *redefine,
                                              axis2_env_t **env){}
                
axis2_status_t AXIS2_CALL
axis2_xml_schema_redefine_set_attribute_group(void *redefine,
                axis2_env_t **env,
                axis2_xml_schema_obj_table_t *group){}

   
                   
axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_redefine_get_items(void *redefine,
               axis2_env_t **env){}
               
axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_schema_types(void *redefine,
                      axis2_env_t **env){}
                      
axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_group(void *redefine, 
               axis2_env_t **env){}