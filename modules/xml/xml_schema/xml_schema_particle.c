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

#include <xml_schema/axis2_xml_schema_particle.h>

typedef struct axis2_xml_schema_particle_impl 
                    axis2_xml_schema_particle_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_particle_impl
{
    axis2_xml_schema_particle_t particle;
    axis2_xml_schema_annotated_t *annotated;
    axis2_xml_schema_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    long max_occurs;
    axis2_char_t *max_occurs_string;
    long min_occurs;
    axis2_char_t *min_occurs_string;
    
};

#define AXIS2_INTF_TO_IMPL(particle) \
        ((axis2_xml_schema_particle_impl_t *) particle)
        
/*************** function prototypes *****************************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_particle_free(
        void *particle,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_xml_schema_particle_super_objs(
        void *particle,
        axis2_env_t **env);

axis2_xml_schema_types_t AXIS2_CALL 
axis2_xml_schema_particle_type(
        void *particle,
        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_particle_get_base_impl(void *particle,
                                        axis2_env_t **env);

long AXIS2_CALL
axis2_xml_schema_particle_get_max_occurs(void *particle,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_particle_set_max_occurs(void *particle,
                                            axis2_env_t **env,
                                            long max_occurs);

long AXIS2_CALL
axis2_xml_schema_particle_get_min_occurs(void *particle,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_particle_set_min_occurs(void *particle,
                                            axis2_env_t **env,
                                            long min_occurs);

/*************** function prototypes *****************************************/

AXIS2_DECLARE(axis2_xml_schema_particle_t *)
axis2_xml_schema_particle_create(axis2_env_t **env)
{
    axis2_xml_schema_particle_impl_t *particle_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    particle_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_particle_impl_t));
    
    if(!particle_impl)
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    particle_impl->annotated = NULL;
    particle_impl->obj_type = AXIS2_XML_SCHEMA_PARTICLE;
    particle_impl->super = NULL;
    particle_impl->particle.ops = NULL;
    particle_impl->particle.base.ops = NULL;
    particle_impl->methods = NULL;
    particle_impl->max_occurs_string = NULL;
    particle_impl->min_occurs_string = NULL;
    
    
    particle_impl->particle.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_particle_ops_t));

    if(!particle_impl->particle.ops)
    {
        axis2_xml_schema_particle_free(&(particle_impl->particle), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    particle_impl->particle.ops->free = 
            axis2_xml_schema_particle_free;
    particle_impl->particle.ops->super_objs = 
            axis2_xml_schema_particle_super_objs;
    particle_impl->particle.ops->type = 
            axis2_xml_schema_particle_type;
    particle_impl->particle.ops->get_base_impl = 
            axis2_xml_schema_particle_get_base_impl;
    particle_impl->particle.ops->get_max_occurs = 
            axis2_xml_schema_particle_get_max_occurs;
    particle_impl->particle.ops->set_max_occurs = 
            axis2_xml_schema_particle_set_max_occurs;
    particle_impl->particle.ops->get_min_occurs = 
            axis2_xml_schema_particle_get_min_occurs;
    particle_impl->particle.ops->set_min_occurs = 
            axis2_xml_schema_particle_set_min_occurs;
   
    particle_impl->methods = axis2_hash_make(env);
    if(!particle_impl->methods)
    {
        axis2_xml_schema_particle_free(&(particle_impl->particle), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(particle_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_particle_free);
    axis2_hash_set(particle_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_particle_super_objs);
    axis2_hash_set(particle_impl->methods, "type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_particle_type);
    axis2_hash_set(particle_impl->methods, "get_max_occurs", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_particle_get_max_occurs);
    axis2_hash_set(particle_impl->methods, "set_max_occurs", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_particle_set_max_occurs);
    axis2_hash_set(particle_impl->methods, "get_min_occurs", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_particle_get_min_occurs);
    axis2_hash_set(particle_impl->methods, "set_min_occurs", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_particle_set_min_occurs);
    
    particle_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!particle_impl->annotated)
    {
        axis2_xml_schema_particle_free(&(particle_impl->particle), env);
        return NULL;        
    }

    particle_impl->super = axis2_hash_make(env);
    if(!particle_impl->super)
    {
        axis2_xml_schema_particle_free(&(particle_impl->particle), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(particle_impl->super, "AXIS2_XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING, 
            &(particle_impl->particle));
    axis2_hash_set(particle_impl->super, "AXIS2_XML_SCHEMA_ANNOTATED", AXIS2_HASH_KEY_STRING, 
            &(particle_impl->annotated));

    status = axis2_xml_schema_annotated_resolve_methods(
            &(particle_impl->particle.base), env, particle_impl->annotated, 
            particle_impl->methods);
    
    return &(particle_impl->particle);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_particle_free(void *particle,
                                    axis2_env_t **env)
{
    axis2_xml_schema_particle_impl_t *particle_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    particle_impl = AXIS2_INTF_TO_IMPL(particle);

    if(NULL != particle_impl->super)
    {
        axis2_hash_free(particle_impl->super, env);
        particle_impl->super = NULL;
    }
    
    if(NULL != particle_impl->methods)
    {
        axis2_hash_free(particle_impl->methods, env);
        particle_impl->methods = NULL;
    }

    if(NULL != particle_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(particle_impl->annotated, env);
        particle_impl->annotated = NULL;
    }
    
    if(NULL != particle_impl->particle.ops)
    {
        AXIS2_FREE((*env)->allocator, particle_impl->particle.ops);
        particle_impl->particle.ops = NULL;
    }
    if(NULL != particle_impl->particle.base.ops)
    {
        AXIS2_FREE((*env)->allocator, particle_impl->particle.base.ops);
        particle_impl->particle.base.ops = NULL;
    }
    
    if(NULL != particle_impl)
    {
        AXIS2_FREE((*env)->allocator, particle_impl);
        particle_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_particle_super_objs(void *particle,
                                axis2_env_t **env)
{
    axis2_xml_schema_particle_impl_t *particle_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    particle_impl = AXIS2_INTF_TO_IMPL(particle);

    return particle_impl->super;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_particle_type(void *particle,
                                axis2_env_t **env)
{
    axis2_xml_schema_particle_impl_t *particle_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    particle_impl = AXIS2_INTF_TO_IMPL(particle);

    return particle_impl->obj_type;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_particle_get_base_impl(void *particle,
                                axis2_env_t **env)
{
    axis2_xml_schema_particle_impl_t *particle_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    particle_impl = AXIS2_INTF_TO_IMPL(particle);

    return particle_impl->annotated;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_particle_resolve_methods(
                                axis2_xml_schema_particle_t *particle,
                                axis2_env_t **env,
                                axis2_xml_schema_particle_t *particle_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_particle_impl_t *particle_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, particle_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    particle_impl_l = (axis2_xml_schema_particle_impl_t *) particle_impl;
    
    particle->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_particle_ops_t));
    if(NULL != particle->ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }            
    
    particle->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    particle->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    particle->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    particle->ops->get_max_occurs = axis2_hash_get(methods, "get_max_occurs", 
            AXIS2_HASH_KEY_STRING);
    if(!particle->ops->get_max_occurs)
            particle->ops->get_max_occurs = 
            particle_impl_l->particle.ops->get_max_occurs;
    
    particle->ops->set_max_occurs = axis2_hash_get(methods, "set_max_occurs", 
            AXIS2_HASH_KEY_STRING);
    if(!particle->ops->set_max_occurs)
            particle->ops->set_max_occurs = 
            particle_impl_l->particle.ops->set_max_occurs;
    
    particle->ops->get_min_occurs = axis2_hash_get(methods, "get_min_occurs", 
            AXIS2_HASH_KEY_STRING);
    if(!particle->ops->get_min_occurs)
            particle->ops->get_min_occurs = 
            particle_impl_l->particle.ops->get_min_occurs;
    
    particle->ops->set_min_occurs = axis2_hash_get(methods, "set_min_occurs", 
            AXIS2_HASH_KEY_STRING);
    if(!particle->ops->set_min_occurs)
            particle->ops->set_min_occurs = 
            particle_impl_l->particle.ops->set_min_occurs;
    
    return axis2_xml_schema_annotated_resolve_methods(&(particle->base), 
            env, particle_impl_l->annotated, methods);
}

long AXIS2_CALL
axis2_xml_schema_particle_get_max_occurs(void *particle,
                                            axis2_env_t **env)
{
    axis2_xml_schema_particle_impl_t *particle_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_XML_SCHEMA_PARTICLE_SUPER_OBJS(particle, env);
    particle_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING));

    return particle_impl->max_occurs;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_particle_set_max_occurs(void *particle,
                                            axis2_env_t **env,
                                            long max_occurs)
{
    axis2_xml_schema_particle_impl_t *particle_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_XML_SCHEMA_PARTICLE_SUPER_OBJS(particle, env);
    particle_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING));

    particle_impl->max_occurs = max_occurs;
    return AXIS2_SUCCESS;
}

long AXIS2_CALL
axis2_xml_schema_particle_get_min_occurs(void *particle,
                                            axis2_env_t **env)
{
    axis2_xml_schema_particle_impl_t *particle_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_XML_SCHEMA_PARTICLE_SUPER_OBJS(particle, env);
    particle_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING));

    return particle_impl->min_occurs;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_particle_set_min_occurs(void *particle,
                                            axis2_env_t **env,
                                            long min_occurs)
{
    axis2_xml_schema_particle_impl_t *particle_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_XML_SCHEMA_PARTICLE_SUPER_OBJS(particle, env);
    particle_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_XML_SCHEMA_PARTICLE", AXIS2_HASH_KEY_STRING));

    particle_impl->min_occurs = min_occurs;
    return AXIS2_SUCCESS;
}

