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

#ifndef AXIS2_XML_SCHEMA_DATA_TYPE_H
#define AXIS2_XML_SCHEMA_DATA_TYPE_H

/**
 * @file axis2_xml_schema_data_type.h
 * @brief Axis2 Xml Schema Data Type  interface
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_data_type axis2_xml_schema_data_type_t;
typedef struct axis2_xml_schema_data_type_ops axis2_xml_schema_data_type_ops_t;
struct axis2_xml_tokenized_type;

/** @defgroup axis2_xml_schema_data_type Xml Schema Data Type
  * @ingroup axis2_xml_schema
  * @{
  */

struct axis2_xml_schema_data_type_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *data_type,
            const axis2_env_t *env);

    void * (AXIS2_CALL *
    parse_value) (void *data_type,
                    const axis2_env_t *env,
                    void *input);

    void *(AXIS2_CALL *
    value_type) (void *data_type,
                    const axis2_env_t *env);

    struct axis2_xml_tokenized_type *(AXIS2_CALL *
    tokenized_type) (void *data_type,
                        const axis2_env_t *env);

};

struct axis2_xml_schema_data_type
{
    axis2_xml_schema_data_type_ops_t *ops;
};

AXIS2_EXTERN axis2_xml_schema_data_type_t * AXIS2_CALL
axis2_xml_schema_data_type_create(const axis2_env_t *env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_data_type_resolve_methods(
                            axis2_xml_schema_data_type_t *schema_data_type,
                            const axis2_env_t *env,
                            axis2_xml_schema_data_type_t *schema_data_type_impl,
                            axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_DATA_TYPE_FREE(data_type, env) \
		(((axis2_xml_schema_data_type_t *) data_type)->ops->free (data_type, env))

#define AXIS2_XML_SCHEMA_DATA_TYPE_PARSE_VALUE(data_type, env, input) \
		(((axis2_xml_schema_data_type_t *) data_type)->ops->parse_value (\
        data_type, env, input))

#define AXIS2_XML_SCHEMA_DATA_TYPE_VALUE_TYPE(data_type, env, line_num) \
		(((axis2_xml_schema_data_type_t *) data_type)->ops->value_type (\
        data_type, env, line_num))

#define AXIS2_XML_SCHEMA_DATA_TYPE_TOKENIZED_TYPE(data_type, env) \
		(((axis2_xml_schema_data_type_t *) data_type)->ops->tokenized_type (\
        data_type, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_DATA_TYPE_H */
