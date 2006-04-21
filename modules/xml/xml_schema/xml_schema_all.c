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

#include <axis2_xml_schema_all.h>
#include <axis2_xml_schema_group_base.h>

typedef struct axis2_xml_schema_all_impl axis2_xml_schema_all_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_all_impl
{
    axis2_xml_schema_all_t all;
    axis2_xml_schema_group_base_t *base;
    axis2_hash_t *methods;
    axis2_xml_schema_obj_collection_t *items;
};

#define AXIS2_INTF_TO_IMPL(all) ((axis2_xml_schema_all_impl_t *) all)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_all_free(void *all,
                        axis2_env_t **env);

axis2_xml_schema_group_base_t *AXIS2_CALL
axis2_xml_schema_all_get_base_impl(void *all,
                                        axis2_env_t **env);

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_all_get_items(void *all,
                                axis2_env_t **env);


AXIS2_DECLARE(axis2_xml_schema_all_t *)
axis2_xml_schema_all_create(axis2_env_t **env)
{
    axis2_xml_schema_all_impl_t *all_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    all_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_all_impl_t));

    all_impl->base = NULL;
    all_impl->methods = NULL;
    all_impl->items = NULL;
    all_impl->all.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_all_ops_t));

    all_impl->all.ops->free = axis2_xml_schema_all_free;
    all_impl->all.ops->get_base_impl = 
            axis2_xml_schema_all_get_base_impl;
    all_impl->all.ops->get_items = 
            axis2_xml_schema_all_get_items;
   
    all_impl->methods = axis2_hash_make(env);
    if(!all_impl->methods)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(all_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_all_free);
    axis2_hash_set(all_impl->methods, "get_items", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_all_get_items);
    
    all_impl->base = axis2_xml_schema_group_base_create(env);
    status = axis2_xml_schema_group_base_resolve_methods(
            &(all_impl->all.base), env, all_impl->base, 
            all_impl->methods);
    
    return &(all_impl->all);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_all_free(void *all,
                                    axis2_env_t **env)
{
    axis2_xml_schema_all_impl_t *all_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    all_impl = AXIS2_INTF_TO_IMPL(all);

    if(all_impl->items)
    {
        AXIS2_XML_SCHEMA_OBJ_COLLECTION_FREE(all_impl->items, env);
        all_impl->items = NULL;
    }
    
    if(all_impl->methods)
    {
        axis2_hash_free(all_impl->methods, env);
        all_impl->methods = NULL;
    }

    if(all_impl->base)
    {
        AXIS2_XML_SCHEMA_GROUP_BASE_FREE(all_impl->base, env);
        all_impl->base = NULL;
    }
    
    if((&(all_impl->all))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(all_impl->all))->ops);
        (&(all_impl->all))->ops = NULL;
    }

    if(all_impl)
    {
        AXIS2_FREE((*env)->allocator, all_impl);
        all_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_group_base_t *AXIS2_CALL
axis2_xml_schema_all_get_base_impl(void *all,
                                axis2_env_t **env)
{
    axis2_xml_schema_all_impl_t *all_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    all_impl = AXIS2_INTF_TO_IMPL(all);

    return all_impl->base;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_all_resolve_methods(
                                axis2_xml_schema_all_t *all,
                                axis2_env_t **env,
                                axis2_xml_schema_all_t *all_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_all_impl_t *all_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, all_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    all_impl_l = (axis2_xml_schema_all_impl_t *) all_impl;
    
    all->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_all_ops_t));
    all->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    all->ops->get_base_impl = 
            all_impl_l->all.ops->get_base_impl;
    all->ops->get_items = 
            all_impl_l->all.ops->get_items;
    
    return axis2_xml_schema_group_base_resolve_methods(&(all->base), 
            env, all_impl_l->base, methods);
}

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_all_get_items(void *all,
                                    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(all)->items;
}

