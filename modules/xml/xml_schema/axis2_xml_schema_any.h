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

#ifndef AXIS2_XML_SCHEMA_ANY_H
#define AXIS2_XML_SCHEMA_ANY_H

/**
 * @file axis2_xml_schema_any.h
 * @brief Axis2 Xml Schema Any Interface
 *          Enables any element from the specified namespace or namespaces
 *          to appear in the containing complexType element. Represents the
 *          World Wide Web Consortium (W3C) any element.
 *
 */

#include <xml_schema/axis2_xml_schema_particle.h>
#include <xml_schema/axis2_xml_schema_content_processing.h>

/** @defgroup axis2_xml_schema_any Xml Schema Any
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_any axis2_xml_schema_any_t;
typedef struct axis2_xml_schema_any_ops axis2_xml_schema_any_ops_t;
struct axis2_xml_schema_content_processing_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_any_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *any,
            axis2_env_t **env);

    axis2_xml_schema_particle_t *(AXIS2_CALL *
    get_base_impl) (void *any,
                    axis2_env_t **env);
    
    axis2_char_t *(AXIS2_CALL *
    get_namespace)(void *any,
                    axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_namespace) (void *any,
                    axis2_env_t **env,
                    axis2_char_t *ns);

    struct axis2_xml_schema_content_processing *(AXIS2_CALL *
    get_process_content) (void *any,
                          axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_process_content)(void *any,
                        axis2_env_t **env,
                        struct axis2_xml_schema_content_processing *process_content);
};

struct axis2_xml_schema_any
{
    axis2_xml_schema_particle_t base;
    axis2_xml_schema_any_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_any_t *)
axis2_xml_schema_any_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_any_resolve_methods(
                                axis2_xml_schema_any_t *any,
                                axis2_env_t **env,
                                axis2_xml_schema_any_t *any_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_ANY_FREE(any, env) \
		(((axis2_xml_schema_any_t *) any)->ops->free(any, env))

#define AXIS2_XML_SCHEMA_ANY_GET_BASE_IMPL(any, env) \
		(((axis2_xml_schema_any_t *) any)->ops->get_base_impl(any, env))

#define AXIS2_XML_SCHEMA_ANY_GET_NAMESPACE(any, env) \
		(((axis2_xml_schema_any_t *) any)->ops->get_namespace(any, env))

#define AXIS2_XML_SCHEMA_ANY_SET_NAMESPACE(any, env, namespc) \
		(((axis2_xml_schema_any_t *) any)->ops->set_namespace(any, env, namespc))

#define AXIS2_XML_SCHEMA_ANY_GET_PROCESS_CONTENT(any, env) \
		(((axis2_xml_schema_any_t *) any)->ops->get_process_content(any, env))

#define AXIS2_XML_SCHEMA_ANY_SET_PROCESS_CONTENT(any, env, process_content) \
		(((axis2_xml_schema_any_t *) any)->ops->set_process_content(any, env, process_content))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_ANY_H */
