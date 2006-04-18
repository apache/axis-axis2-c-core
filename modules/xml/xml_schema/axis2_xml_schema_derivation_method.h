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

#ifndef AXIS2_XML_SCHEMA_DERIVATION_METHOD_H
#define AXIS2_XML_SCHEMA_DERIVATION_METHOD_H

/**
 * @file axis2_xml_schema_use.h
 * @brief Axis2 Xml Schema Derivation Method Interface
 *          Provides different methods for preventing derivation.
 *
 */

#include <axis2_xml_schema_enum.h>

/** @defgroup axis2_xml_schema_derivation_method Xml Schema Derivation Method
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_derivation_method axis2_xml_schema_derivation_method_t;
typedef struct axis2_xml_schema_derivation_method_ops axis2_xml_schema_derivation_method_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_derivation_method_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *derivation_method,
            axis2_env_t **env);

    axis2_xml_schema_enum_t *(AXIS2_CALL *
    get_base_impl) (void *derivation_method,
                    axis2_env_t **env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_values)(void *derivation_method,
                axis2_env_t **env);
    
};

struct axis2_xml_schema_derivation_method
{
    axis2_xml_schema_enum_t base;
    axis2_xml_schema_derivation_method_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_derivation_method_t *)
axis2_xml_schema_derivation_method_create(axis2_env_t **env,
                                    axis2_char_t* value);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_derivation_method_resolve_methods(
                                axis2_xml_schema_derivation_method_t *derivation_method,
                                axis2_env_t **env,
                                axis2_xml_schema_derivation_method_t *derivation_method_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_DERIVATION_METHOD_FREE(derivation_method, env) \
		(((axis2_xml_schema_derivation_method_t *) derivation_method)->ops->free(derivation_method, env))

#define AXIS2_XML_SCHEMA_DERIVATION_METHOD_GET_BASE_IMPL(derivation_method, env) \
		(((axis2_xml_schema_derivation_method_t *) derivation_method)->ops->get_base_impl(derivation_method, env))

#define AXIS2_XML_SCHEMA_DERIVATION_METHOD_GET_VALUES(derivation_method, env) \
		(((axis2_xml_schema_derivation_method_t *) derivation_method)->ops->values(derivation_method, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_DERIVATION_METHOD_H */
