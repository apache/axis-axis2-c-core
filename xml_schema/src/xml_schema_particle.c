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

#include <xml_schema_particle.h>
#include <axis2_utils.h>

typedef struct xml_schema_particle_impl 
                    xml_schema_particle_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
 */ 
struct xml_schema_particle_impl
{
    xml_schema_particle_t particle;
    
    xml_schema_annotated_t *annotated;
    
    xml_schema_types_t obj_type;
    
    axis2_hash_t *ht_super;
    
    long max_occurs;
    
    axis2_char_t *max_occurs_string;
    
    long min_occurs;
    
    axis2_char_t *min_occurs_string;
    
};

#define AXIS2_INTF_TO_IMPL(particle) \
        ((xml_schema_particle_impl_t *) particle)
        
/*************** function prototypes *****************************************/

axis2_status_t AXIS2_CALL 
xml_schema_particle_free(
        void *particle,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
xml_schema_particle_super_objs(
        void *particle,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL 
xml_schema_particle_get_type(
        void *particle,
        const axis2_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_particle_get_base_impl(void *particle,
                                        const axis2_env_t *env);

long AXIS2_CALL
xml_schema_particle_get_max_occurs(void *particle,
                                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_particle_set_max_occurs(void *particle,
                                            const axis2_env_t *env,
                                            long max_occurs);

long AXIS2_CALL
xml_schema_particle_get_min_occurs(void *particle,
                                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_particle_set_min_occurs(void *particle,
                                            const axis2_env_t *env,
                                            long min_occurs);

/*************** function prototypes *****************************************/

AXIS2_EXTERN xml_schema_particle_t * AXIS2_CALL
xml_schema_particle_create(const axis2_env_t *env)
{
    xml_schema_particle_impl_t *particle_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    particle_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_particle_impl_t));
    
    if(!particle_impl)
    {
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    particle_impl->annotated = NULL;
    particle_impl->obj_type = XML_SCHEMA_PARTICLE;
    particle_impl->ht_super = NULL;
    particle_impl->particle.ops = NULL;
    particle_impl->particle.base.ops = NULL;
    particle_impl->max_occurs_string = NULL;
    particle_impl->min_occurs_string = NULL;
    
    
    particle_impl->particle.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_particle_ops_t));

    if(!particle_impl->particle.ops)
    {
        xml_schema_particle_free(&(particle_impl->particle), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    particle_impl->particle.ops->free = 
            xml_schema_particle_free;
    
    particle_impl->particle.ops->super_objs = 
            xml_schema_particle_super_objs;
    
    particle_impl->particle.ops->get_type = 
            xml_schema_particle_get_type;
    
    particle_impl->particle.ops->get_base_impl = 
            xml_schema_particle_get_base_impl;
    
    particle_impl->particle.ops->get_max_occurs = 
            xml_schema_particle_get_max_occurs;
    
    particle_impl->particle.ops->set_max_occurs = 
            xml_schema_particle_set_max_occurs;
    
    particle_impl->particle.ops->get_min_occurs = 
            xml_schema_particle_get_min_occurs;
    
    particle_impl->particle.ops->set_min_occurs = 
            xml_schema_particle_set_min_occurs;
   
    particle_impl->annotated = xml_schema_annotated_create(env);
    if(!particle_impl->annotated)
    {
        xml_schema_particle_free(&(particle_impl->particle), env);
        return NULL;        
    }

    particle_impl->ht_super = axis2_hash_make(env);
    if(!particle_impl->ht_super)
    {
        xml_schema_particle_free(&(particle_impl->particle), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(particle_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_PARTICLE", env), AXIS2_HASH_KEY_STRING, 
            &(particle_impl->particle));
    axis2_hash_set(particle_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_ANNOTATED", env), AXIS2_HASH_KEY_STRING, 
            particle_impl->annotated);
            
    axis2_hash_set(particle_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_OBJ", env), AXIS2_HASH_KEY_STRING,
            XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(particle_impl->annotated, env));           
            
    status = xml_schema_annotated_resolve_methods(
            &(particle_impl->particle.base), env, particle_impl->annotated, 
            xml_schema_particle_super_objs,
            xml_schema_particle_get_type,
            xml_schema_particle_free);
    
    return &(particle_impl->particle);
}

axis2_status_t AXIS2_CALL
xml_schema_particle_free(void *particle,
                                    const axis2_env_t *env)
{
    xml_schema_particle_impl_t *particle_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    particle_impl = AXIS2_INTF_TO_IMPL(particle);

    if(NULL != particle_impl->ht_super)
    {
        axis2_hash_free(particle_impl->ht_super, env);
        particle_impl->ht_super = NULL;
    }
    
   

    if(NULL != particle_impl->annotated)
    {
        XML_SCHEMA_ANNOTATED_FREE(particle_impl->annotated, env);
        particle_impl->annotated = NULL;
    }
    
    if(NULL != particle_impl->particle.ops)
    {
        AXIS2_FREE(env->allocator, particle_impl->particle.ops);
        particle_impl->particle.ops = NULL;
    }
    if(NULL != particle_impl->particle.base.ops)
    {
        AXIS2_FREE(env->allocator, particle_impl->particle.base.ops);
        particle_impl->particle.base.ops = NULL;
    }
    
    if(NULL != particle_impl)
    {
        AXIS2_FREE(env->allocator, particle_impl);
        particle_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
xml_schema_particle_super_objs(void *particle,
                                const axis2_env_t *env)
{
    xml_schema_particle_impl_t *particle_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    particle_impl = AXIS2_INTF_TO_IMPL(particle);
    return particle_impl->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_particle_get_type(void *particle,
                                const axis2_env_t *env)
{
    xml_schema_particle_impl_t *particle_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    particle_impl = AXIS2_INTF_TO_IMPL(particle);
    return particle_impl->obj_type;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_particle_get_base_impl(void *particle,
                                const axis2_env_t *env)
{
    xml_schema_particle_impl_t *particle_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    particle_impl = AXIS2_INTF_TO_IMPL(particle);
    return particle_impl->annotated;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_particle_resolve_methods(
                                xml_schema_particle_t *particle,
                                const axis2_env_t *env,
                                xml_schema_particle_t *particle_impl,
                                XML_SCHEMA_SUPER_OBJS_FN super_objs,
                                XML_SCHEMA_GET_TYPE_FN get_type,
                                XML_SCHEMA_FREE_FN free_fn)
{
    xml_schema_particle_impl_t *sch_particle_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, particle_impl, AXIS2_FAILURE);
    
    sch_particle_impl = (xml_schema_particle_impl_t *) particle_impl;
    
    particle->ops = AXIS2_MALLOC(env->allocator, 
            sizeof(xml_schema_particle_ops_t));
    if(!particle->ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }            
    
    particle->ops->free = free_fn;
    particle->ops->super_objs = super_objs;
    
    particle->ops->get_type = get_type;

    particle->ops->get_max_occurs = 
        sch_particle_impl->particle.ops->get_max_occurs;
    
    particle->ops->set_max_occurs = 
        sch_particle_impl->particle.ops->set_max_occurs;
    
    particle->ops->get_min_occurs = 
        sch_particle_impl->particle.ops->get_min_occurs;
    
    particle->ops->set_min_occurs = 
        sch_particle_impl->particle.ops->set_min_occurs;
    
    return xml_schema_annotated_resolve_methods(&(particle->base), 
            env, sch_particle_impl->annotated, super_objs, get_type, free_fn);
}

long AXIS2_CALL
xml_schema_particle_get_max_occurs(void *particle,
                                            const axis2_env_t *env)
{
    xml_schema_particle_impl_t *particle_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_PARTICLE_SUPER_OBJS(particle, env);
    if(NULL != ht_super)
    {
        particle_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                "XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING));
        if(!particle_impl)
            return AXIS2_FAILURE;                
    }
    return particle_impl->max_occurs;
}

axis2_status_t AXIS2_CALL
xml_schema_particle_set_max_occurs(void *particle,
                                            const axis2_env_t *env,
                                            long max_occurs)
{
    xml_schema_particle_impl_t *particle_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_PARTICLE_SUPER_OBJS(particle, env);
    if(NULL != ht_super)
    {
        particle_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                "XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING));
        if(!particle_impl)
            return AXIS2_FAILURE;                
    }

    particle_impl->max_occurs = max_occurs;
    return AXIS2_SUCCESS;
}

long AXIS2_CALL
xml_schema_particle_get_min_occurs(void *particle,
                                            const axis2_env_t *env)
{
    xml_schema_particle_impl_t *particle_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_PARTICLE_SUPER_OBJS(particle, env);
    if(NULL != ht_super)
    {
        particle_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                "XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING));
        if(!particle_impl)
            return AXIS2_FAILURE;                
    }

    return particle_impl->min_occurs;
}

axis2_status_t AXIS2_CALL
xml_schema_particle_set_min_occurs(void *particle,
                                            const axis2_env_t *env,
                                            long min_occurs)
{
    xml_schema_particle_impl_t *particle_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_PARTICLE_SUPER_OBJS(particle, env);
    if(NULL != ht_super)
    {
        particle_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, 
                "XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING));
        if(!particle_impl)
            return AXIS2_FAILURE;                
    }
    particle_impl->min_occurs = min_occurs;
    return AXIS2_SUCCESS;
}

