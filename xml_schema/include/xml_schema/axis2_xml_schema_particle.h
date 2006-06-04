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

#ifndef AXIS2_XML_SCHEMA_PARTICLE_H
#define AXIS2_XML_SCHEMA_PARTICLE_H

/**
 * @file axis2_xml_schema_particle.h
 * @brief Axis2 Xml Schema Particle Interface
 *          Base class for all particle types.
 */

#include <xml_schema/axis2_xml_schema_defines.h>
#include <xml_schema/axis2_xml_schema_annotated.h>
#include <axis2_hash.h>

/** @defgroup axis2_xml_schema_particle Xml Schema Particle
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_particle 
                    axis2_xml_schema_particle_t;
typedef struct axis2_xml_schema_particle_ops 
                    axis2_xml_schema_particle_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_particle_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *particle,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *particle,
            const axis2_env_t *env);

    axis2_xml_schema_types_t (AXIS2_CALL *
    type) (
            void *particle,
            const axis2_env_t *env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *particle,
                    const axis2_env_t *env);

    long (AXIS2_CALL *
    get_max_occurs)(void *particle,
                const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_max_occurs)(void *particle,
                    const axis2_env_t *env,
                    long max_occurs);
    
    long (AXIS2_CALL *
    get_min_occurs)(void *particle,
                const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_min_occurs)(void *particle,
                    const axis2_env_t *env,
                    long min_occurs);
};

struct axis2_xml_schema_particle
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_particle_ops_t *ops;
};

/**
 * Creates new Xml Schema Particle
 * Particle types are usually interchangeable.
 * A local element declaration or reference to a global element
 * declaration (element), a compositor ( sequence, choice, or all),
 * a reference to a named content model group (group), or an element wildcard 
 * (any).
 */
AXIS2_EXTERN axis2_xml_schema_particle_t * AXIS2_CALL
axis2_xml_schema_particle_create(const axis2_env_t *env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_particle_resolve_methods(
                                axis2_xml_schema_particle_t *particle,
                                const axis2_env_t *env,
                                axis2_xml_schema_particle_t *particle_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_PARTICLE_FREE(particle, env) \
      (((axis2_xml_schema_particle_t *) particle)->ops->\
            free(particle, env))

#define AXIS2_XML_SCHEMA_PARTICLE_SUPER_OBJS(particle, env) \
      (((axis2_xml_schema_particle_t *) particle)->ops->\
            super_objs(particle, env))

#define AXIS2_XML_SCHEMA_PARTICLE_TYPE(particle, env) \
      (((axis2_xml_schema_particle_t *) particle)->ops->\
            type(particle, env))

#define AXIS2_XML_SCHEMA_PARTICLE_GET_BASE_IMPL(particle, env) \
      (((axis2_xml_schema_particle_t *) particle)->ops->\
            get_base_impl(particle, env))

#define AXIS2_XML_SCHEMA_PARTICLE_GET_MAX_OCCURS(particle, env) \
      (((axis2_xml_schema_particle_t *) particle)->ops->\
            get_max_occurs(particle, env))

#define AXIS2_XML_SCHEMA_PARTICLE_SET_MAX_OCCURS(particle, env, max_occurs) \
      (((axis2_xml_schema_particle_t *) particle)->ops->\
            set_max_occurs(particle, env, max_occurs))

#define AXIS2_XML_SCHEMA_PARTICLE_GET_MIN_OCCURS(particle, env) \
      (((axis2_xml_schema_particle_t *) particle)->ops->\
            get_min_occurs(particle, env))

#define AXIS2_XML_SCHEMA_PARTICLE_SET_MIN_OCCURS(particle, env, min_occurs) \
      (((axis2_xml_schema_particle_t *) particle)->ops->\
            set_min_occurs(particle, env, min_occurs))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_PARTICLE_H */
