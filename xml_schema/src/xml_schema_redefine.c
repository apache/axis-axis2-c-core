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

#include  <xml_schema/axis2_xml_schema_redefine.h>
#include  <xml_schema/axis2_xml_schema.h>

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
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
    
    axis2_hash_t *ht_super;
    
    axis2_xml_schema_types_t obj_type;

}axis2_xml_schema_redefine_impl_t;

#define AXIS2_INTF_TO_IMPL(redefine) \
        ((axis2_xml_schema_redefine_impl_t *) redefine)

/********************* function prototypes ***********************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_redefine_free(void *redefine,
                        const axis2_env_t *env);

axis2_xml_schema_external_t *AXIS2_CALL
axis2_xml_schema_redefine_get_base_impl(void *redefine,
                                        const axis2_env_t *env);
                                        
axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_redefine_type(void *redefine,
                                const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_redefine_super_objs(void *redefine,
                                     const axis2_env_t *env);                                                                                

axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_attribute_group(void *redefine,
                                              const axis2_env_t *env);
                
axis2_status_t AXIS2_CALL
axis2_xml_schema_redefine_set_attribute_group(void *redefine,
                const axis2_env_t *env,
                axis2_xml_schema_obj_table_t *group);

   
                   
axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_redefine_get_items(void *redefine,
               const axis2_env_t *env);
               
axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_schema_types(void *redefine,
                      const axis2_env_t *env);
                      
axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_group(void *redefine, 
               const axis2_env_t *env);                      
                      
    
/********************* end function prototypes ********************************/

AXIS2_EXTERN axis2_xml_schema_redefine_t * AXIS2_CALL
axis2_xml_schema_redefine_create(const axis2_env_t *env)
{
    axis2_xml_schema_redefine_impl_t *redefine_impl = NULL;
    axis2_xml_schema_annotated_t *annotated = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    redefine_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_redefine_impl_t));
    if(!redefine_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
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
    redefine_impl->obj_type = AXIS2_XML_SCHEMA_REDEFINE;
    redefine_impl->ht_super = NULL;
    
    redefine_impl->redefine.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_redefine_ops_t));

    if(!redefine_impl->redefine.ops)
    {
        axis2_xml_schema_redefine_free(&(redefine_impl->redefine), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    redefine_impl->redefine.ops->free = 
        axis2_xml_schema_redefine_free;
    redefine_impl->redefine.ops->get_base_impl = 
        axis2_xml_schema_redefine_get_base_impl;
    redefine_impl->redefine.ops->type =
        axis2_xml_schema_redefine_type;
    redefine_impl->redefine.ops->super_objs =
        axis2_xml_schema_redefine_super_objs;
    redefine_impl->redefine.ops->get_items = 
        axis2_xml_schema_redefine_get_items;
    redefine_impl->redefine.ops->get_group = 
        axis2_xml_schema_redefine_get_group;
    redefine_impl->redefine.ops->get_attribute_group =
        axis2_xml_schema_redefine_get_attribute_group;
    redefine_impl->redefine.ops->set_attribute_group =
        axis2_xml_schema_redefine_set_attribute_group;
    redefine_impl->redefine.ops->get_schema_types =
        axis2_xml_schema_redefine_get_schema_types;                            

    redefine_impl->methods = axis2_hash_make(env);
    redefine_impl->ht_super = axis2_hash_make(env);
    if(!redefine_impl->methods || !redefine_impl->ht_super)
    {
        axis2_xml_schema_redefine_free(&(redefine_impl->redefine), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(redefine_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_redefine_free);
    axis2_hash_set(redefine_impl->methods, "type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_redefine_type);
    axis2_hash_set(redefine_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_redefine_super_objs);
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
    
    
    axis2_hash_set(redefine_impl->ht_super, "AXIS2_XML_SCHEMA_REDEFINE", 
            AXIS2_HASH_KEY_STRING, &(redefine_impl->redefine));
    axis2_hash_set(redefine_impl->ht_super, "AXIS2_XML_SCHEMA_EXTERNAL", 
            AXIS2_HASH_KEY_STRING, redefine_impl->external);
            
    annotated = AXIS2_XML_SCHEMA_EXTERNAL_GET_BASE_IMPL(redefine_impl->external, env);
    if(NULL != redefine_impl->external)
    {
        
        axis2_hash_set(redefine_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED", 
            AXIS2_HASH_KEY_STRING, annotated);
        axis2_hash_set(redefine_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", 
            AXIS2_HASH_KEY_STRING, 
            AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env));        
    }            

    status = axis2_xml_schema_external_resolve_methods(
            &(redefine_impl->redefine.base), env, redefine_impl->external, 
            redefine_impl->methods);

    return &(redefine_impl->redefine);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_redefine_free(void *redefine,
                                    const axis2_env_t *env)
{
    axis2_xml_schema_redefine_impl_t *redefine_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    redefine_impl = AXIS2_INTF_TO_IMPL(redefine);

    if(NULL != redefine_impl->methods)
    {
        axis2_hash_free(redefine_impl->methods, env);
        redefine_impl->methods = NULL;
    }
    if(NULL != redefine_impl->ht_super)
    {
        axis2_hash_free(redefine_impl->ht_super, env);
        redefine_impl->ht_super = NULL;
    }

    if(NULL != redefine_impl->external)
    {
        AXIS2_XML_SCHEMA_EXTERNAL_FREE(redefine_impl->external, env);
        redefine_impl->external = NULL;
    }
    
    if(NULL != redefine_impl->redefine.ops)
    {
        AXIS2_FREE(env->allocator, redefine_impl->redefine.ops);
        redefine_impl->redefine.ops = NULL;
    }
    if(NULL != redefine_impl->redefine.base.ops)
    {
        AXIS2_FREE(env->allocator, redefine_impl->redefine.base.ops);
        redefine_impl->redefine.base.ops = NULL;        
    }
    AXIS2_FREE(env->allocator, redefine_impl);
    redefine_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_external_t *AXIS2_CALL
axis2_xml_schema_redefine_get_base_impl(void *redefine,
                                const axis2_env_t *env)
{
    axis2_xml_schema_redefine_impl_t *redefine_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    redefine_impl = AXIS2_INTF_TO_IMPL(redefine);
    return redefine_impl->external;
}


/*

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_redefine_resolve_methods(
                                axis2_xml_schema_redefine_t *redefine,
                                const axis2_env_t *env,
                                axis2_xml_schema_redefine_t *redefine_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_redefine_impl_t *redefine_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, redefine_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    redefine_impl_l = (axis2_xml_schema_redefine_impl_t *) redefine_impl;
    
    redefine->ops = AXIS2_MALLOC(env->allocator, 
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
*/
axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_attribute_group(void *redefine,
                                              const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(redefine)->attr_groups;
}
                
axis2_status_t AXIS2_CALL
axis2_xml_schema_redefine_set_attribute_group(void *redefine,
                const axis2_env_t *env,
                axis2_xml_schema_obj_table_t *group)
{
    axis2_xml_schema_redefine_impl_t *red_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, group, AXIS2_FAILURE);
    red_impl = AXIS2_INTF_TO_IMPL(redefine);
    if(NULL != red_impl->groups)
    {
    
    }
    red_impl->groups = group;
    return AXIS2_SUCCESS;
}

   
                   
axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_redefine_get_items(void *redefine,
               const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(redefine)->items;
}
               
axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_schema_types(void *redefine,
                      const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(redefine)->schema_types;
}
                      
axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_redefine_get_group(void *redefine, 
               const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(redefine)->groups;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_redefine_type(void *redefine,
                                const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(redefine)->obj_type;
}
                                

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_redefine_super_objs(void *redefine,
                                     const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(redefine)->ht_super;
}

