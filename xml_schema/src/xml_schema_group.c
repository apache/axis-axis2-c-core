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

#include <xml_schema/axis2_xml_schema_group.h>
#include <xml_schema/axis2_xml_schema_group_base.h>
#include <axis2_utils.h>

typedef struct axis2_xml_schema_group_impl axis2_xml_schema_group_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_group_impl
{
    axis2_xml_schema_group_t group;
    axis2_xml_schema_annotated_t *annotated;
    axis2_xml_schema_types_t obj_type;
    axis2_hash_t *ht_super;
    axis2_hash_t *methods;
    axis2_char_t *name;
    axis2_xml_schema_group_base_t *particle;
};

#define AXIS2_INTF_TO_IMPL(group) ((axis2_xml_schema_group_impl_t *) group)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_group_free(
        void *group,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
axis2_xml_schema_group_super_objs(
        void *group,
        const axis2_env_t *env);

axis2_xml_schema_types_t AXIS2_CALL 
axis2_xml_schema_group_type(
        void *group,
        const axis2_env_t *env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_group_get_base_impl(void *group,
                                        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_xml_schema_group_get_name(void *group,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_group_set_name(void *group,
                                const axis2_env_t *env,
                                axis2_char_t *name);

axis2_xml_schema_group_base_t *AXIS2_CALL
axis2_xml_schema_group_get_particle(void *group,
                                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_group_set_particle(void *group,
                                    const axis2_env_t *env,
                                    axis2_xml_schema_group_base_t *particle);

AXIS2_EXTERN axis2_xml_schema_group_t * AXIS2_CALL
axis2_xml_schema_group_create(const axis2_env_t *env)
{
    axis2_xml_schema_group_impl_t *group_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    group_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_group_impl_t));
    if(!group_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    group_impl->annotated = NULL;
    group_impl->obj_type = AXIS2_XML_SCHEMA_GROUP;
    group_impl->ht_super = NULL;
    group_impl->methods = NULL;
    group_impl->name = NULL;
    group_impl->particle = NULL;
    group_impl->group.base.ops = NULL;
    group_impl->group.ops = NULL;
    
    group_impl->group.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_group_ops_t));
    if(!group_impl->group.ops)
    {
        axis2_xml_schema_group_free(&(group_impl->group), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    group_impl->group.ops->free = 
            axis2_xml_schema_group_free;
    group_impl->group.ops->super_objs = 
            axis2_xml_schema_group_super_objs;
    group_impl->group.ops->type = 
            axis2_xml_schema_group_type;
    group_impl->group.ops->get_base_impl = 
            axis2_xml_schema_group_get_base_impl;
    group_impl->group.ops->get_name = 
            axis2_xml_schema_group_get_name;
    group_impl->group.ops->set_name = 
            axis2_xml_schema_group_set_name;
    group_impl->group.ops->get_particle = 
            axis2_xml_schema_group_get_particle;
    group_impl->group.ops->set_particle = 
            axis2_xml_schema_group_set_particle;
   
    group_impl->methods = axis2_hash_make(env);
    if(!group_impl->methods)
    {
        axis2_xml_schema_group_free(&(group_impl->group), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(group_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_group_free);
    axis2_hash_set(group_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_group_super_objs);
    axis2_hash_set(group_impl->methods, "type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_group_type);
    axis2_hash_set(group_impl->methods, "get_name", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_group_get_name);
    axis2_hash_set(group_impl->methods, "set_name", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_group_set_name);
    axis2_hash_set(group_impl->methods, "get_particle", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_group_get_particle);
    axis2_hash_set(group_impl->methods, "set_particle", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_group_set_particle);
    
    group_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!group_impl->annotated)
    {
        axis2_xml_schema_group_free(&(group_impl->group), env);
        return NULL;
    }
    group_impl->ht_super = axis2_hash_make(env);
    if(!group_impl->ht_super)
    {
        axis2_xml_schema_group_free(&(group_impl->group), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(group_impl->ht_super, "AXIS2_XML_SCHEMA_GROUP", AXIS2_HASH_KEY_STRING, 
            &(group_impl->group));
    axis2_hash_set(group_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED", AXIS2_HASH_KEY_STRING, 
            group_impl->annotated);
    axis2_hash_set(group_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", AXIS2_HASH_KEY_STRING, 
            AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(group_impl->annotated, env));
                

    status = axis2_xml_schema_annotated_resolve_methods(
            &(group_impl->group.base), env, group_impl->annotated, 
            group_impl->methods);
    return &(group_impl->group);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_group_free(void *group,
                                    const axis2_env_t *env)
{
    axis2_xml_schema_group_impl_t *group_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_impl = AXIS2_INTF_TO_IMPL(group);

    if(NULL != group_impl->name)
    {
        AXIS2_FREE(env->allocator, group_impl->name);
        group_impl->name = NULL;
    }

    if(NULL != group_impl->particle)
    {
        AXIS2_XML_SCHEMA_PARTICLE_FREE(group_impl->particle, env);
        group_impl->particle = NULL;
    }
    
    if(NULL != group_impl->ht_super)
    {
        axis2_hash_free(group_impl->ht_super, env);
        group_impl->ht_super = NULL;
    }
    
    if(NULL != group_impl->methods)
    {
        axis2_hash_free(group_impl->methods, env);
        group_impl->methods = NULL;
    }

    if(NULL != group_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(group_impl->annotated, env);
        group_impl->annotated = NULL;
    }
    
    if(NULL != group_impl->group.ops)
    {
        AXIS2_FREE(env->allocator, group_impl->group.ops);
        group_impl->group.ops = NULL;
    }
    if(NULL != group_impl->group.base.ops)
    {
        AXIS2_FREE(env->allocator, group_impl->group.base.ops);
        group_impl->group.base.ops = NULL;  
    }
    AXIS2_FREE(env->allocator, group_impl);
    group_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_group_super_objs(void *group,
                                const axis2_env_t *env)
{
    axis2_xml_schema_group_impl_t *group_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    group_impl = AXIS2_INTF_TO_IMPL(group);
    return group_impl->ht_super;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_group_type(void *group,
                                const axis2_env_t *env)
{
    axis2_xml_schema_group_impl_t *group_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_impl = AXIS2_INTF_TO_IMPL(group);
    return group_impl->obj_type;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_group_get_base_impl(void *group,
                                const axis2_env_t *env)
{
    axis2_xml_schema_group_impl_t *group_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    group_impl = AXIS2_INTF_TO_IMPL(group);
    return group_impl->annotated;
}

/*
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_group_resolve_methods(
                                axis2_xml_schema_group_t *group,
                                const axis2_env_t *env,
                                axis2_xml_schema_group_t *group_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_group_impl_t *group_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, group_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, group, AXIS2_FAILURE);
    
    group_impl_l = (axis2_xml_schema_group_impl_t *) group_impl;
    
    group->ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_xml_schema_group_ops_t));
    if(!group->ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    group->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    group->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    group->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    group->ops->get_name = axis2_hash_get(methods, "name", 
            AXIS2_HASH_KEY_STRING);
    if(!group->ops->get_name)
            group->ops->get_name = 
            group_impl_l->group.ops->get_name;

    group->ops->set_name = axis2_hash_get(methods, "set_name", 
            AXIS2_HASH_KEY_STRING);
    if(!group->ops->set_name)
            group->ops->set_name = 
            group_impl_l->group.ops->set_name;

    group->ops->get_particle = axis2_hash_get(methods, "get_particle", 
            AXIS2_HASH_KEY_STRING);
    if(!group->ops->get_particle)
    group->ops->get_particle = 
            group_impl_l->group.ops->get_particle;

    group->ops->set_particle = axis2_hash_get(methods, "set_particle", 
            AXIS2_HASH_KEY_STRING);
    if(!group->ops->set_particle)
    group->ops->set_particle = 
            group_impl_l->group.ops->set_particle;
    
    return axis2_xml_schema_annotated_resolve_methods(&(group->base), 
            env, group_impl_l->annotated, methods);
}

*/

axis2_char_t *AXIS2_CALL
axis2_xml_schema_group_get_name(void *group,
                                const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(group)->name;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_group_set_name(void *group,
                                const axis2_env_t *env,
                                axis2_char_t *name)
{
    axis2_xml_schema_group_impl_t *group_impl = NULL;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
/*    super = AXIS2_XML_SCHEMA_GROUP_SUPER_OBJS(group, env); 
    group_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_XML_SCHEMA_GROUP", AXIS2_HASH_KEY_STRING));
*/

    group_impl = AXIS2_INTF_TO_IMPL(group);
    if(NULL != group_impl->name)
    {
        AXIS2_FREE(env->allocator, group_impl->name);
        group_impl->name = NULL;
    }
    group_impl->name = AXIS2_STRDUP(name, env);
    if(!group_impl->name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_group_base_t *AXIS2_CALL
axis2_xml_schema_group_get_particle(void *group,
                                        const axis2_env_t *env)
{
    /*
    axis2_xml_schema_group_impl_t *group_impl = NULL;
  
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_XML_SCHEMA_GROUP_SUPER_OBJS(group, env); 
    group_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_XML_SCHEMA_GROUP", AXIS2_HASH_KEY_STRING));
    */
    return AXIS2_INTF_TO_IMPL(group)->particle;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_group_set_particle(void *group,
                                    const axis2_env_t *env,
                                    axis2_xml_schema_group_base_t *particle)
{
    axis2_xml_schema_group_impl_t *group_impl = NULL;
    /*
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_impl = AXIS2_INTF_TO_IMPL(group);
    AXIS2_PARAM_CHECK(env->error , particle, AXIS2_FAILURE);
    super = AXIS2_XML_SCHEMA_GROUP_SUPER_OBJS(group, env); 
    group_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_XML_SCHEMA_GROUP", AXIS2_HASH_KEY_STRING));
    */
    group_impl = AXIS2_INTF_TO_IMPL(group);
    
    if(NULL != group_impl->particle)
    {
        /** TODO Free particle */
        group_impl->particle = NULL;
    }
    group_impl->particle = particle;
    return AXIS2_SUCCESS;
}

