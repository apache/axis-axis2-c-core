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

#ifndef AXIS2_XML_SCHEMA_VALIDATION_EVENT_HANDLER_H
#define AXIS2_XML_SCHEMA_VALIDATION_EVENT_HANDLER_H

/**
 * @file axis2_xml_schema_validation_event_handler.h
 * @brief Axis2 Xml Schema Validation Event Handler  interface
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_validation_event_handler axis2_xml_schema_validation_event_handler_t;
typedef struct axis2_xml_schema_validation_event_handler_ops axis2_xml_schema_validation_event_handler_ops_t;

/** @defgroup axis2_xml_schema_validation_event_handler Xml Schema Validation Event Handler
  * @ingroup axis2_xml_schema
  * @{
  */

struct axis2_xml_schema_validation_event_handler_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *axis2_validation_event_handler,
            axis2_env_t **env);

};

struct axis2_xml_schema_validation_event_handler
{
    axis2_xml_schema_validation_event_handler_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_validation_event_handler_t *)
axis2_xml_schema_validation_event_handler_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_validation_event_handler_resolve_methods(axis2_xml_schema_validation_event_handler_t *schema_axis2_validation_event_handler,
                                        axis2_env_t **env,
                                        axis2_xml_schema_validation_event_handler_t *schema_axis2_validation_event_handler_impl,
                                        axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_VALIDATION_EVENT_HANDLER_FREE(axis2_validation_event_handler, env) \
		(((axis2_xml_schema_validation_event_handler_t *) axis2_validation_event_handler)->ops->free (axis2_validation_event_handler, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_VALIDATION_EVENT_HANDLER_H */
