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

#ifndef AXIS2_XML_TOKENIZED_TYPE_H
#define AXIS2_XML_TOKENIZED_TYPE_H

/**
 * @file axis2_xml_tokenized_type.h
 * @brief Axis2 Xml Tokenized Type Interface
 *          Indicator of how the attribute is used.
 *
 */

#include <axis2_xml_schema_enum.h>

/** @defgroup axis2_xml_tokenized_type Xml Tokenized Type
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_tokenized_type axis2_xml_tokenized_type_t;
typedef struct axis2_xml_tokenized_type_ops 
        axis2_xml_tokenized_type_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_tokenized_type_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *tokenized_type,
            axis2_env_t **env);

    axis2_xml_schema_enum_t *(AXIS2_CALL *
    get_base_impl) (void *tokenized_type,
                    axis2_env_t **env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_values)(void *tokenized_type,
                axis2_env_t **env);
    
};

struct axis2_xml_tokenized_type
{
    axis2_xml_schema_enum_t base;
    axis2_xml_tokenized_type_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_tokenized_type_t *)
axis2_xml_tokenized_type_create(axis2_env_t **env,
                                    axis2_char_t* value);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_tokenized_type_resolve_methods(
                                axis2_xml_tokenized_type_t *tokenized_type,
                                axis2_env_t **env,
                                axis2_xml_tokenized_type_t *tokenized_type_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_TOKENIZED_TYPE_FREE(tokenized_type, env) \
		(((axis2_xml_tokenized_type_t *) tokenized_type)->ops->free(tokenized_type, env))

#define AXIS2_XML_TOKENIZED_TYPE_GET_BASE_IMPL(tokenized_type, env) \
		(((axis2_xml_tokenized_type_t *) tokenized_type)->ops->get_base_impl(tokenized_type, env))

#define AXIS2_XML_TOKENIZED_TYPE_GET_VALUES(tokenized_type, env) \
		(((axis2_xml_tokenized_type_t *) tokenized_type)->ops->values(tokenized_type, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_TOKENIZED_TYPE_H */
