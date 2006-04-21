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

#ifndef AXIS2_XML_SCHEMA_EXTERNAL_H
#define AXIS2_XML_SCHEMA_EXTERNAL_H

/**
 * @file axis2_xml_schema_external.h
 * @brief Axis2 Xml Schema Particle Interface
 *          Base class for all external types.
 */

#include <axis2_xml_schema_annotated.h>
#include <axis2_hash.h>

/** @defgroup axis2_xml_schema_external Xml Schema Particle
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_external 
                    axis2_xml_schema_external_t;
typedef struct axis2_xml_schema_external_ops 
                    axis2_xml_schema_external_ops_t;
                    
struct axis2_xml_schema;                    

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_external_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *external,
            axis2_env_t **env);

    struct axis2_xml_schema* (AXIS2_CALL *
    get_schema)(void *external,
                axis2_env_t **env);
                
    axis2_status_t (AXIS2_CALL *
    set_schema)(void *external,
                axis2_env_t **env,
                struct axis2_xml_schema* schema);

    axis2_xml_schema_annotated_t* (AXIS2_CALL *
    get_base_impl)(void *external,
                   axis2_env_t **env);
};

struct axis2_xml_schema_external
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_external_ops_t *ops;
};

/**
 * Creates new Xml Schema external
 */
AXIS2_DECLARE(axis2_xml_schema_external_t *)
axis2_xml_schema_external_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_external_resolve_methods(
                                axis2_xml_schema_external_t *external,
                                axis2_env_t **env,
                                axis2_xml_schema_external_t *external_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_EXTERNAL_FREE(external, env) \
		(((axis2_xml_schema_external_t *) external)->ops->\
            free(external, env))

#define AXIS2_XML_SCHEMA_EXTERNAL_GET_BASE_IMPL(external, env) \
		(((axis2_xml_schema_external_t *) external)->ops->\
            get_base_impl(external, env))

#define AXIS2_XML_SCHEMA_EXTERNAL_GET_SCHEMA(external, env) \
		(((axis2_xml_schema_external_t *) external)->ops->\
            get_schema(external, env))

#define AXIS2_XML_SCHEMA_EXTERNAL_SET_SCHEMA(external, env, max_occurs) \
		(((axis2_xml_schema_external_t *) external)->ops->\
            set_schema(external, env, max_occurs))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_EXTERNAL_H */
