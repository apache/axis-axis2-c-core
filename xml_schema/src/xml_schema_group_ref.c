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

#include <xml_schema/axis2_xml_schema_group_ref.h>
#include <xml_schema/axis2_xml_schema_group_base.h>
#include <axis2_utils.h>

typedef struct axis2_xml_schema_group_ref_impl 
                axis2_xml_schema_group_ref_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
 */ 
struct axis2_xml_schema_group_ref_impl
{
    axis2_xml_schema_group_ref_t group_ref;
    
    axis2_xml_schema_particle_t *base;
    
    axis2_xml_schema_types_t obj_type;
    
    axis2_hash_t *ht_super;
    
    axis2_hash_t *methods;
    
    axis2_qname_t *ref_qname;
    
    axis2_xml_schema_group_base_t *particle;
};

#define AXIS2_INTF_TO_IMPL(group_ref) \
        ((axis2_xml_schema_group_ref_impl_t *) group_ref)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_group_ref_free(void *group_ref,
                        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_group_ref_super_objs(
        void *group_ref,
        const axis2_env_t *env);

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_group_ref_type(
        void *group_ref,
        const axis2_env_t *env);

axis2_xml_schema_particle_t *AXIS2_CALL
axis2_xml_schema_group_ref_get_base_impl(void *group_ref,
                                        const axis2_env_t *env);

axis2_qname_t *AXIS2_CALL
axis2_xml_schema_group_ref_get_ref_qname(void *group_ref,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_group_ref_set_ref_qname(void *group_ref,
                                const axis2_env_t *env,
                                axis2_qname_t *ref_qname);

axis2_xml_schema_group_base_t *AXIS2_CALL
axis2_xml_schema_group_ref_get_particle(void *group_ref,
                                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_group_ref_set_particle(void *group_ref,
                                    const axis2_env_t *env,
                                    axis2_xml_schema_group_base_t *particle);
/************************** end function pointers **************************/

AXIS2_EXTERN axis2_xml_schema_group_ref_t * AXIS2_CALL
axis2_xml_schema_group_ref_create(const axis2_env_t *env)
{
    axis2_xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    axis2_xml_schema_annotated_t *annotated = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    group_ref_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_group_ref_impl_t));
    if(!group_ref_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    group_ref_impl->group_ref.base.ops = NULL;
    group_ref_impl->base = NULL;
    group_ref_impl->obj_type = AXIS2_XML_SCHEMA_GROUP_REF;
    group_ref_impl->ht_super = NULL;
    group_ref_impl->methods = NULL;
    group_ref_impl->ref_qname = NULL;
    group_ref_impl->particle = NULL;
    group_ref_impl->group_ref.ops = NULL;
    
    group_ref_impl->group_ref.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_group_ref_ops_t));
    
    if(!group_ref_impl->group_ref.ops)
    {
        axis2_xml_schema_group_ref_free(&(group_ref_impl->group_ref), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    group_ref_impl->group_ref.ops->free = 
            axis2_xml_schema_group_ref_free;
    group_ref_impl->group_ref.ops->super_objs = 
            axis2_xml_schema_group_ref_super_objs;
    group_ref_impl->group_ref.ops->type = 
            axis2_xml_schema_group_ref_type;
    
    group_ref_impl->group_ref.ops->get_base_impl = 
            axis2_xml_schema_group_ref_get_base_impl;
    
    group_ref_impl->group_ref.ops->get_ref_qname = 
            axis2_xml_schema_group_ref_get_ref_qname;
    
    group_ref_impl->group_ref.ops->set_ref_qname = 
            axis2_xml_schema_group_ref_set_ref_qname;
    
    group_ref_impl->group_ref.ops->get_particle = 
            axis2_xml_schema_group_ref_get_particle;
            
    group_ref_impl->group_ref.ops->set_particle =
            axis2_xml_schema_group_ref_set_particle;            
   
    group_ref_impl->methods = axis2_hash_make(env);
    if(!group_ref_impl->methods)
    {
        axis2_xml_schema_group_ref_free(&(group_ref_impl->group_ref), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(group_ref_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_group_ref_free);
    axis2_hash_set(group_ref_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_group_ref_super_objs);
    axis2_hash_set(group_ref_impl->methods, "type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_group_ref_type);
    axis2_hash_set(group_ref_impl->methods, "get_ref_qname", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_group_ref_get_ref_qname);
    axis2_hash_set(group_ref_impl->methods, "set_ref_qname", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_group_ref_set_ref_qname);
    axis2_hash_set(group_ref_impl->methods, "get_particle", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_group_ref_get_particle);
    
    group_ref_impl->base = axis2_xml_schema_particle_create(env);
     if(!group_ref_impl->base)
    {
        axis2_xml_schema_group_ref_free(&(group_ref_impl->group_ref), env);
        return NULL;
    }
    group_ref_impl->ht_super = axis2_hash_make(env);
    if(!group_ref_impl->ht_super)
    {
        axis2_xml_schema_group_ref_free(&(group_ref_impl->group_ref), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(group_ref_impl->ht_super, "AXIS2_XML_SCHEMA_GROUP_REF", 
            AXIS2_HASH_KEY_STRING, &(group_ref_impl->group_ref) );
    axis2_hash_set(group_ref_impl->ht_super, "AXIS2_XML_SCHEMA_PARTICLE", 
            AXIS2_HASH_KEY_STRING, group_ref_impl->particle );
    
    annotated = AXIS2_XML_SCHEMA_PARTICLE_GET_BASE_IMPL(group_ref_impl->particle, env);
    if(NULL != annotated)
    {
        axis2_hash_set(group_ref_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED",
            AXIS2_HASH_KEY_STRING, annotated);
        axis2_hash_set(group_ref_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ",
            AXIS2_HASH_KEY_STRING, AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env));            
    }
 
    status = axis2_xml_schema_particle_resolve_methods(
            &(group_ref_impl->group_ref.base), env, group_ref_impl->base, 
            group_ref_impl->methods);
    
    return &(group_ref_impl->group_ref);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_group_ref_free(void *group_ref,
                                    const axis2_env_t *env)
{
    axis2_xml_schema_group_ref_impl_t *group_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_ref_impl = AXIS2_INTF_TO_IMPL(group_ref);

    if(NULL != group_ref_impl->ref_qname)
    {
        AXIS2_FREE(env->allocator, group_ref_impl->ref_qname);
        group_ref_impl->ref_qname = NULL;
    }

    if(NULL != group_ref_impl->particle)
    {
        AXIS2_XML_SCHEMA_GROUP_BASE_FREE(group_ref_impl->particle, env);
        group_ref_impl->particle = NULL;
    }
 
    if(NULL != group_ref_impl->ht_super)
    {
        axis2_hash_free(group_ref_impl->ht_super, env);
        group_ref_impl->ht_super = NULL;
    }
    
    if(NULL != group_ref_impl->methods)
    {
        axis2_hash_free(group_ref_impl->methods, env);
        group_ref_impl->methods = NULL;
    }

    if(NULL != group_ref_impl->base)
    {
        AXIS2_XML_SCHEMA_PARTICLE_FREE(group_ref_impl->base, env);
        group_ref_impl->base = NULL;
    }
    
    if(NULL != group_ref_impl->group_ref.base.ops)
    {
        AXIS2_FREE(env->allocator, group_ref_impl->group_ref.base.ops);
        group_ref_impl->group_ref.base.ops = NULL;
    }
    if(NULL != group_ref_impl->group_ref.ops)
    {
        AXIS2_FREE(env->allocator, group_ref_impl->group_ref.ops);
        group_ref_impl->group_ref.ops = NULL;
    }
    AXIS2_FREE(env->allocator, group_ref_impl);
    group_ref_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_group_ref_super_objs(
        void *group_ref,
        const axis2_env_t *env)
{
    axis2_xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    group_ref_impl = AXIS2_INTF_TO_IMPL(group_ref);
    return group_ref_impl->ht_super;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_group_ref_type(
        void *group_ref,
        const axis2_env_t *env)
{
    axis2_xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_ref_impl = AXIS2_INTF_TO_IMPL(group_ref);
    return group_ref_impl->obj_type;
}

axis2_xml_schema_particle_t *AXIS2_CALL
axis2_xml_schema_group_ref_get_base_impl(void *group_ref,
                                const axis2_env_t *env)
{
    axis2_xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    group_ref_impl = AXIS2_INTF_TO_IMPL(group_ref);
    return group_ref_impl->base;
}

/*
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_group_ref_resolve_methods(
                                axis2_xml_schema_group_ref_t *group_ref,
                                const axis2_env_t *env,
                                axis2_xml_schema_group_ref_t *group_ref_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_group_ref_impl_t *group_ref_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, group_ref_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    group_ref_impl_l = (axis2_xml_schema_group_ref_impl_t *) group_ref_impl;
    
    group_ref->ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_xml_schema_group_ref_ops_t));
    group_ref->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    group_ref->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    group_ref->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    group_ref->ops->get_ref_qname = axis2_hash_get(methods, "get_ref_qname", 
            AXIS2_HASH_KEY_STRING);
    if(!group_ref->ops->get_ref_qname)
    group_ref->ops->get_ref_qname = 
            group_ref_impl_l->group_ref.ops->get_ref_qname;
    
    group_ref->ops->set_ref_qname = axis2_hash_get(methods, "set_ref_qname", 
            AXIS2_HASH_KEY_STRING); 
    if(!group_ref->ops->set_ref_qname)
    group_ref->ops->set_ref_qname = 
            group_ref_impl_l->group_ref.ops->set_ref_qname;
    
    group_ref->ops->get_particle = axis2_hash_get(methods, "get_particle", 
            AXIS2_HASH_KEY_STRING);
    if(!group_ref->ops->get_particle)
    group_ref->ops->get_particle = 
            group_ref_impl_l->group_ref.ops->get_particle;
    
    return axis2_xml_schema_particle_resolve_methods(&(group_ref->base), 
            env, group_ref_impl_l->base, methods);
}
*/

axis2_qname_t *AXIS2_CALL
axis2_xml_schema_group_ref_get_ref_qname(void *group_ref,
                                const axis2_env_t *env)
{
/*    axis2_xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_XML_SCHEMA_GROUP_REF_SUPER_OBJS(group_ref, env); 
    group_ref_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_XML_SCHEMA_GROUP_REF", AXIS2_HASH_KEY_STRING));
*/
    
    return AXIS2_INTF_TO_IMPL(group_ref)->ref_qname;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_group_ref_set_ref_qname(void *group_ref,
                                const axis2_env_t *env,
                                axis2_qname_t *ref_qname)
{
    axis2_xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    /*
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_XML_SCHEMA_GROUP_REF_SUPER_OBJS(group_ref, env); 
    group_ref_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_XML_SCHEMA_GROUP_REF", AXIS2_HASH_KEY_STRING));
    
    */
    group_ref_impl = AXIS2_INTF_TO_IMPL(group_ref);
    if(group_ref_impl->ref_qname)
    {
        AXIS2_QNAME_FREE(group_ref_impl->ref_qname, env);
        group_ref_impl->ref_qname = NULL;
    }
    group_ref_impl->ref_qname = AXIS2_QNAME_CLONE(ref_qname, env);
    if(!group_ref_impl->ref_qname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_group_base_t *AXIS2_CALL
axis2_xml_schema_group_ref_get_particle(void *group_ref,
                                        const axis2_env_t *env)
{
/*
    axis2_xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_XML_SCHEMA_GROUP_REF_SUPER_OBJS(group_ref, env); 
    group_ref_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_XML_SCHEMA_GROUP_REF", AXIS2_HASH_KEY_STRING));
*/    
    return AXIS2_INTF_TO_IMPL(group_ref)->particle;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_group_ref_set_particle(void *group_ref,
                                    const axis2_env_t *env,
                                    axis2_xml_schema_group_base_t *particle)
{
    axis2_xml_schema_group_ref_impl_t *grp_ref_impl = NULL;
    
    grp_ref_impl = AXIS2_INTF_TO_IMPL(group_ref);
    
    if(grp_ref_impl->particle)
    {
        /** TODO free */
    }
    grp_ref_impl->particle = particle;
    return AXIS2_SUCCESS;
}                                    
