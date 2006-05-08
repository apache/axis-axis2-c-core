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

#ifndef AXIS2_XML_SCHEMA_CONTENT_PROCESSING_H
#define AXIS2_XML_SCHEMA_CONTENT_PROCESSING_H

/**
 * @file axis2_xml_schema_content_processing.h
 * @brief Axis2 Xml Schema Content Processing Interface
 *          Indicator of how the attribute is content_processingd.
 *
 */

#include <xml_schema/axis2_xml_schema_enum.h>

/** @defgroup axis2_xml_schema_content_processing Xml Schema Content Processing
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_content_processing 
        axis2_xml_schema_content_processing_t;
        
typedef struct axis2_xml_schema_content_processing_ops 
        axis2_xml_schema_content_processing_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_content_processing_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *content_processing,
            axis2_env_t **env);

    axis2_xml_schema_enum_t *(AXIS2_CALL *
    get_base_impl) (void *content_processing,
                    axis2_env_t **env);

    axis2_array_list_t *(AXIS2_CALL *
    get_values)(void *content_processing,
                axis2_env_t **env);
    
};

struct axis2_xml_schema_content_processing
{
    axis2_xml_schema_enum_t base;
    axis2_xml_schema_content_processing_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_content_processing_t *)
axis2_xml_schema_content_processing_create(axis2_env_t **env,
                                    axis2_char_t* value);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_content_processing_resolve_methods(
                axis2_xml_schema_content_processing_t *content_processing,
                axis2_env_t **env,
                axis2_xml_schema_content_processing_t *content_processing_impl,
                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_CONTENT_PROCESSING_FREE(content_processing, env) \
		(((axis2_xml_schema_content_processing_t *) content_processing)->ops->\
         free(content_processing, env))


#define AXIS2_XML_SCHEMA_CONTENT_PROCESSING_GET_BASE_IMPL(content_processing, env) \
		(((axis2_xml_schema_content_processing_t *) content_processing)->ops->\
         get_base_impl(content_processing, env))

#define AXIS2_XML_SCHEMA_CONTENT_PROCESSING_GET_VALUES(content_processing, env) \
		(((axis2_xml_schema_content_processing_t *) content_processing)->ops->\
         get_values(content_processing, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_CONTENT_PROCESSING_H */
