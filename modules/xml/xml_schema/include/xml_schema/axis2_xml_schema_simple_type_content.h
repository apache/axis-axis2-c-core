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

#ifndef AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT_H
#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT_H

/**
 * @file axis2_xml_schema_simple_type_content.h
 * @brief Axis2 Xml Schema Particle Interface
 *          Base class for all sim_type_cont types.
 */

#include <axis2_xml_schema_annotated.h>
#include <axis2_hash.h>

/** @defgroup axis2_xml_schema_simple_type_content Xml Schema Particle
  * @ingroup axis2_xml_schema
  * @{
  */
#ifdef __cplusplus
extern "C"
{
#endif

typedef enum axis2_xml_simple_content_types_t
{
    AXIS2_XML_SCHEMA_SIMPLE_CONTENT_TYPE = 0,
    AXIS2_XML_SCHEMA_SIMPLE_TYPE_LIST,
    AXIS2_XML_SCHEMA_SIMPLE_TYPE_RESTRICTION,
    AXIS2_XML_SCHEMA_SIMPLE_TYPE_UNION

}axis2_xml_simple_content_types_t;


typedef struct axis2_xml_schema_simple_type_content 
                    axis2_xml_schema_simple_type_content_t;
typedef struct axis2_xml_schema_simple_type_content_ops 
                    axis2_xml_schema_simple_type_content_ops_t;


struct axis2_xml_schema_simple_type_content_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *sim_type_cont,
            axis2_env_t **env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *sim_type_cont,
                    axis2_env_t **env);

    int (AXIS2_CALL *
    get_type)(void *sim_type_cont,
              axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_type)(void *sim_type_cont,
              axis2_env_t **env,
              int type);
    
};

struct axis2_xml_schema_simple_type_content
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_simple_type_content_ops_t *ops;
};

/**
 * Creates new Xml Schema Particle
 * Particle types are usually interchangeable.
 * A local element declaration or reference to a global element
 * declaration (element), a compositor ( sequence, choice, or all),
 * a reference to a named content model group (group), or an element wildcard 
 * (any).
 */
AXIS2_DECLARE(axis2_xml_schema_simple_type_content_t *)
axis2_xml_schema_simple_type_content_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_simple_type_content_resolve_methods(
                                axis2_xml_schema_simple_type_content_t *sim_type_cont,
                                axis2_env_t **env,
                                axis2_xml_schema_simple_type_content_t *sim_type_cont_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT_FREE(sim_type_cont, env) \
		(((axis2_xml_schema_simple_type_content_t *) sim_type_cont)->ops->\
            free(sim_type_cont, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT_GET_BASE_IMPL(sim_type_cont, env) \
		(((axis2_xml_schema_simple_type_content_t *) sim_type_cont)->ops->\
            get_base_impl(sim_type_cont, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT_SET_TYPE(sim_type_cont, env, type) \
		(((axis2_xml_schema_simple_type_content_t *) sim_type_cont)->ops->\
            set_type(sim_type_cont, env, type))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT_GET_TYPE(sim_type_cont, env) \
		(((axis2_xml_schema_simple_type_content_t *) sim_type_cont)->ops->\
            get_type(sim_type_cont, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_SIMPLE_TYPE_CONTENT_H */
