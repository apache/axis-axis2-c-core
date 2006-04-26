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

#ifndef AXIS2_XML_SCHEMA_FACET_H
#define AXIS2_XML_SCHEMA_FACET_H

/**
 * @file axis2_xml_schema_facet.h
 * @brief Axis2 Xml Schema Facet Interface
 *          Abstract class for all facets that are used when simple types are
 *          derived by restriction.
 */

#include <axis2_xml_schema_annotated.h>
#include <axis2_om_element.h>
#include <axis2_om_node.h>
#include <axis2_hash.h>

/** @defgroup axis2_xml_schema_facet Xml Schema Facet
  * @ingroup axis2_xml_schema
  * @{
  */
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_facet axis2_xml_schema_facet_t;
typedef struct axis2_xml_schema_facet_ops axis2_xml_schema_facet_ops_t;

typedef enum axis2_xml_schema_facet_type
{
    AXIS2_XML_SCHEMA_FACET = 0,
    AXIS2_XML_SCHEMA_ENUMARATION_FACET,
    AXIS2_XML_SCHEMA_FRACTION_DIGITS_FACET,
    AXIS2_XML_SCHEMA_LENGTH_FACET,
    AXIS2_XML_SCHEMA_MAX_EXCLUSIVE_FACET,
    AXIS2_XML_SCHEMA_MAX_INCLUSIVE_FACET,
    AXIS2_XML_SCHEMA_MAX_LENGTH_FACET,
    AXIS2_XML_SCHEMA_MIN_EXCLUSIVE_FACET,
    AXIS2_XML_SCHEMA_MIN_INCLUSIVE_FACET,
    AXIS2_XML_SCHEMA_MIN_LENGTH_FACET,
    AXIS2_XML_SCHEMA_NUMERAIC_FACET,
    AXIS2_XML_SCHEMA_PATTERN_FACET,
    AXIS2_XML_SCHEMA_TATAL_DIGITS_FACET,
    AXIS2_XML_SCHEMA_WHITE_SPACE_FACET
    
}axis2_xml_schema_facet_type_t; 

struct axis2_xml_schema_facet_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *facet,
            axis2_env_t **env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *facet,
                    axis2_env_t **env);

    axis2_bool_t (AXIS2_CALL *
    is_fixed)(void *facet,
                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_fixed)(void *facet,
                    axis2_env_t **env,
                    axis2_bool_t fixed);
    
    void *(AXIS2_CALL *
    get_value)(void *facet,
                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_value)(void *facet,
                    axis2_env_t **env,
                    void *value);
    
    int (AXIS2_CALL *
    get_facet_type)(void *facet,
                    axis2_env_t **env);
                    
    axis2_status_t (AXIS2_CALL *
    set_facet_type)(void *facet,
                    axis2_env_t **env,
                    int facet_type);                    
    
};

struct axis2_xml_schema_facet
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_facet_ops_t *ops;
};

/**
 * Creates new Xml Schema Facet
 */
AXIS2_DECLARE(axis2_xml_schema_facet_t *)
axis2_xml_schema_facet_create(axis2_env_t **env,
                                void *value,
                                axis2_bool_t fixed);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_facet_resolve_methods(
                                axis2_xml_schema_facet_t *facet,
                                axis2_env_t **env,
                                axis2_xml_schema_facet_t *facet_impl,
                                axis2_hash_t *methods);
                                
AXIS2_DECLARE(axis2_xml_schema_facet_t *)
axis2_xml_schema_facet_construct(axis2_env_t **env,
                                 axis2_om_node_t *node);
                                                                 

#define AXIS2_XML_SCHEMA_FACET_FREE(facet, env) \
		(((axis2_xml_schema_facet_t *) facet)->ops->\
            free(facet, env))

#define AXIS2_XML_SCHEMA_FACET_GET_BASE_IMPL(facet, env) \
		(((axis2_xml_schema_facet_t *) facet)->ops->\
            get_base_impl(facet, env))

#define AXIS2_XML_SCHEMA_FACET_IS_FIXED(facet, env) \
		(((axis2_xml_schema_facet_t *) facet)->ops->\
            is_fixed(facet, env))

#define AXIS2_XML_SCHEMA_FACET_SET_FIXED(facet, env, fixed) \
		(((axis2_xml_schema_facet_t *) facet)->ops->\
            set_fixed(facet, env, fixed))

#define AXIS2_XML_SCHEMA_FACET_GET_VALUE(facet, env) \
		(((axis2_xml_schema_facet_t *) facet)->ops->\
            get_value(facet, env))

#define AXIS2_XML_SCHEMA_FACET_SET_VALUE(facet, env, value) \
		(((axis2_xml_schema_facet_t *) facet)->ops->\
            set_value(facet, env, value))
            
#define AXIS2_XML_SCHEMA_FACET_SET_FACET_TYPE(facet, env, facet_type) \
        (((axis2_xml_schema_facet_t *) facet)->ops->\
            set_facet_type(facet, env, facet_type))
            
#define AXIS2_XML_SCHEMA_FACET_GET_FACET_TYPE(facet, env) \
        (((axis2_xml_schema_facet_t *) facet)->ops->\
            get_facet_type(facet, env))                        

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_FACET_H */
