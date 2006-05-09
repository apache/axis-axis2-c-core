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

#ifndef AXIS2_XML_SCHEMA_CONTENT_TYPE_H
#define AXIS2_XML_SCHEMA_CONTENT_TYPE_H

/**
 * @file axis2_xml_schema_content_type.h
 * @brief Axis2 Xml Schema Form Interface
 *          Indicates if attributes or elements need to be qualified or left 
 *          unqualified.
 *
 */

#include <xml_schema/axis2_xml_schema_enum.h>

/** @defgroup axis2_xml_schema_content_type Xml Schema Form
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_content_type 
                    axis2_xml_schema_content_type_t;
typedef struct axis2_xml_schema_content_type_ops 
                    axis2_xml_schema_content_type_ops_t;

#define AXIS2_XML_SCHEMA_QUALIFIED "qualified"
#define AXIS2_XML_SCHEMA_UNQUALIFIED "unqualified"
#define AXIS2_XML_SCHEMA_NONE "none"

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_content_type_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *content_type,
            axis2_env_t **env);

    axis2_xml_schema_enum_t *(AXIS2_CALL *
    get_base_impl) (void *content_type,
                    axis2_env_t **env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_values)(void *content_type,
                axis2_env_t **env);
    
};

struct axis2_xml_schema_content_type
{
    axis2_xml_schema_enum_t base;
    axis2_xml_schema_content_type_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_content_type_t *)
axis2_xml_schema_content_type_create(axis2_env_t **env,
                                    axis2_char_t* value);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_content_type_resolve_methods(
                                axis2_xml_schema_content_type_t *content_type,
                                axis2_env_t **env,
                                axis2_xml_schema_content_type_t *content_type_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_CONTENT_TYPE_FREE(content_type, env) \
		(((axis2_xml_schema_content_type_t *) content_type)->ops->free(content_type, env))

#define AXIS2_XML_SCHEMA_CONTENT_TYPE_GET_BASE_IMPL(content_type, env) \
		(((axis2_xml_schema_content_type_t *) content_type)->ops->get_base_impl(content_type, env))

#define AXIS2_XML_SCHEMA_CONTENT_TYPE_GET_VALUES(content_type, env) \
		(((axis2_xml_schema_content_type_t *) content_type)->ops->values(content_type, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_CONTENT_TYPE_H */
