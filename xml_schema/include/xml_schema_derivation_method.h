/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef XML_SCHEMA_DERIVATION_METHOD_H
#define XML_SCHEMA_DERIVATION_METHOD_H

/**
 * @file xml_schema_use.h
 * @brief Axis2 Xml Schema Derivation Method Interface
 *          Provides different methods for preventing derivation.
 *
 */

#include <xml_schema_enum.h>

/** @defgroup xml_schema_derivation_method Xml Schema Derivation Method
  * @ingroup xml_schema
  * @{
  */



#ifdef __cplusplus
extern "C"
{
#endif

typedef struct xml_schema_derivation_method 
                xml_schema_derivation_method_t;
typedef struct xml_schema_derivation_method_ops
                xml_schema_derivation_method_ops_t;

struct xml_schema_derivation_method_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *derivation_method,
            const axutil_env_t *env);

    xml_schema_enum_t *(AXIS2_CALL *
    get_base_impl) (void *derivation_method,
                    const axutil_env_t *env);
    
    axutil_array_list_t *(AXIS2_CALL *
    get_values)(void *derivation_method,
                const axutil_env_t *env);
                
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(void *derivation_method,
                const axutil_env_t *env);
    
    xml_schema_types_t (AXIS2_CALL *
    get_type)(void *derivation_method,
          const axutil_env_t *env);                                
    
};

struct xml_schema_derivation_method
{
    xml_schema_enum_t base;
    xml_schema_derivation_method_ops_t *ops;
};

AXIS2_EXTERN xml_schema_derivation_method_t * AXIS2_CALL
xml_schema_derivation_method_create(const axutil_env_t *env,
                                    axis2_char_t* value);


#define XML_SCHEMA_DERIVATION_METHOD_FREE(derivation_method, env) \
      (((xml_schema_derivation_method_t *) \
       derivation_method)->ops->free(derivation_method, env))

#define XML_SCHEMA_DERIVATION_METHOD_GET_BASE_IMPL(derivation_method, env) \
      (((xml_schema_derivation_method_t *) \
      derivation_method)->ops->get_base_impl(derivation_method, env))

#define XML_SCHEMA_DERIVATION_METHOD_GET_VALUES(derivation_method, env) \
      (((xml_schema_derivation_method_t *) \
      derivation_method)->ops->values(derivation_method, env))

#define XML_SCHEMA_DERIVATION_METHOD_GET_TYPE(derivation_method, env) \
      (((xml_schema_derivation_method_t *) \
      derivation_method)->ops->get_type(derivation_method, env))
      
#define XML_SCHEMA_DERIVATION_METHOD_SUPER_OBJS(derivation_method, env) \
        (((xml_schema_derivation_method_t *) \
      derivation_method)->ops->super_objs(derivation_method, env))
      
      
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_DERIVATION_METHOD_H */
