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

#include <xml_schema/axis2_xml_schema_group_base.h>

typedef struct axis2_xml_schema_group_base_impl 
                axis2_xml_schema_group_base_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_group_base_impl
{
    axis2_xml_schema_group_base_t group_base;
    
    axis2_xml_schema_particle_t *particle;
    
    axis2_xml_schema_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    
    axis2_xml_schema_obj_collection_t *items;
};

#define AXIS2_INTF_TO_IMPL(group_base) \
        ((axis2_xml_schema_group_base_impl_t *) group_base)

/****************** *****************************************************/
axis2_status_t AXIS2_CALL
axis2_xml_schema_group_base_free(
        void *group_base,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_group_base_super_objs(
        void *group_base,
        axis2_env_t **env);

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_group_base_type(
        void *group_base,
        axis2_env_t **env);

axis2_xml_schema_particle_t *AXIS2_CALL
axis2_xml_schema_group_base_get_base_impl(void *group_base,
                                    axis2_env_t **env);

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_group_base_get_items(void *group_base,
                                        axis2_env_t **env);

/***********************************************************************/
 
AXIS2_DECLARE(axis2_xml_schema_group_base_t *)
axis2_xml_schema_group_base_create(axis2_env_t **env)
{
    axis2_xml_schema_group_base_impl_t *group_base_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    group_base_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_group_base_impl_t));
    if(NULL != group_base_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                    

    group_base_impl->particle = NULL;
    group_base_impl->obj_type = AXIS2_XML_SCHEMA_GROUP_BASE;
    group_base_impl->super = NULL;
    group_base_impl->methods = NULL;
    group_base_impl->items = NULL;
    group_base_impl->group_base.ops = NULL;
    group_base_impl->group_base.base.ops = NULL;
    
    group_base_impl->group_base.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_group_base_ops_t));
    if(!group_base_impl->group_base.ops)
    {
        axis2_xml_schema_group_base_free(&(group_base_impl->group_base), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    group_base_impl->group_base.ops->free = 
        axis2_xml_schema_group_base_free;
    group_base_impl->group_base.ops->super_objs = 
        axis2_xml_schema_group_base_super_objs;
    group_base_impl->group_base.ops->type = 
        axis2_xml_schema_group_base_type;
    group_base_impl->group_base.ops->get_base_impl = 
        axis2_xml_schema_group_base_get_base_impl;
    group_base_impl->group_base.ops->get_items = 
        axis2_xml_schema_group_base_get_items;
   
    group_base_impl->methods = axis2_hash_make(env);
    if(!group_base_impl->methods)
    {
        axis2_xml_schema_group_base_free(&(group_base_impl->group_base), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(group_base_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_group_base_free);
    axis2_hash_set(group_base_impl->methods, "type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_group_base_type);
    axis2_hash_set(group_base_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_group_base_super_objs);
    axis2_hash_set(group_base_impl->methods, "get_items", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_group_base_get_items);
    
    group_base_impl->particle = axis2_xml_schema_particle_create(env);
    if(!group_base_impl->particle)
    {
        axis2_xml_schema_group_base_free(&(group_base_impl->group_base), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    group_base_impl->super = axis2_hash_make(env);
    if(!group_base_impl->super)
    {
        axis2_xml_schema_group_base_free(&(group_base_impl->group_base), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(group_base_impl->super, "AXIS2_XML_SCHEMA_GROUP_BASE", 
            AXIS2_HASH_KEY_STRING, &(group_base_impl->group_base) );
    axis2_hash_set(group_base_impl->super, "AXIS2_XML_SCHEMA_PARTICLE", 
            AXIS2_HASH_KEY_STRING, group_base_impl->particle );
 
    status = axis2_xml_schema_particle_resolve_methods(
            &(group_base_impl->group_base.base), env, group_base_impl->particle, 
            group_base_impl->methods); 
    return &(group_base_impl->group_base);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_group_base_free(void *group_base,
                            axis2_env_t **env)
{
    axis2_xml_schema_group_base_impl_t *group_base_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_base_impl = AXIS2_INTF_TO_IMPL(group_base);

    if(NULL != group_base_impl->items)
    {
        AXIS2_XML_SCHEMA_OBJ_COLLECTION_FREE(group_base_impl->items, env);
        group_base_impl->items = NULL;
    }
 
    if(NULL != group_base_impl->super)
    {
        axis2_hash_free(group_base_impl->super, env);
        group_base_impl->super = NULL;
    }
 
    if(NULL != group_base_impl->methods)
    {
        axis2_hash_free(group_base_impl->methods, env);
        group_base_impl->methods = NULL;
    }
   
    if(NULL != group_base_impl->particle)
    {
        AXIS2_XML_SCHEMA_PARTICLE_FREE(group_base_impl->particle, env);
        group_base_impl->particle = NULL;
    }
    
    if(NULL != group_base_impl->group_base.ops)
    {
        AXIS2_FREE((*env)->allocator, group_base_impl->group_base.ops);
        group_base_impl->group_base.ops = NULL;
    }
    if(NULL != group_base_impl->group_base.base.ops)
    {
        AXIS2_FREE((*env)->allocator, group_base_impl->group_base.base.ops);
        group_base_impl->group_base.base.ops = NULL;
    }
    if(group_base_impl)
    {
        AXIS2_FREE((*env)->allocator, group_base_impl);
        group_base_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_group_base_super_objs(
        void *group_base,
        axis2_env_t **env)
{
    axis2_xml_schema_group_base_impl_t *group_base_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    group_base_impl = AXIS2_INTF_TO_IMPL(group_base);

    return group_base_impl->super;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_group_base_type(
        void *group_base,
        axis2_env_t **env)
{
    axis2_xml_schema_group_base_impl_t *group_base_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_base_impl = AXIS2_INTF_TO_IMPL(group_base);

    return group_base_impl->obj_type;
}

axis2_xml_schema_particle_t *AXIS2_CALL
axis2_xml_schema_group_base_get_base_impl(
        void *group_base,
        axis2_env_t **env)
{
    axis2_xml_schema_group_base_impl_t *group_base_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    group_base_impl = AXIS2_INTF_TO_IMPL(group_base);

    return group_base_impl->particle;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_group_base_resolve_methods(
       axis2_xml_schema_group_base_t *group_base,
       axis2_env_t **env,
       axis2_xml_schema_group_base_t *group_base_impl,
       axis2_hash_t *methods)
{
    axis2_xml_schema_group_base_impl_t *group_base_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, group_base_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    group_base_impl_l = (axis2_xml_schema_group_base_impl_t *) group_base_impl;
    
    group_base->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_group_base_ops_t));
    if(!group_base->ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
            
    group_base->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    group_base->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    group_base->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    group_base->ops->get_items = axis2_hash_get(methods, "get_items", 
            AXIS2_HASH_KEY_STRING);
    if(!group_base->ops->get_items)
            group_base->ops->get_items = 
            group_base_impl_l->group_base.ops->get_items;
    
    return axis2_xml_schema_particle_resolve_methods(&(group_base->base), 
            env, group_base_impl_l->particle, methods);
}

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_group_base_get_items(void *group_base,
                                        axis2_env_t **env)
{
    axis2_xml_schema_group_base_impl_t *group_base_impl = NULL;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_XML_SCHEMA_GROUP_BASE_SUPER_OBJS(group_base, env); 
    group_base_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_XML_SCHEMA_GROUP_BASE", AXIS2_HASH_KEY_STRING));
    
    return AXIS2_INTF_TO_IMPL(group_base)->items;
}

