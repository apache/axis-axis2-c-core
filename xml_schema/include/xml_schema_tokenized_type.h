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

#ifndef XML_SCHEMA_TOKENIZED_TYPE_H
#define XML_SCHEMA_TOKENIZED_TYPE_H

/**
 * @file xml_schema_tokenized_type.h
 * @brief Axis2 Xml Tokenized Type Interface
 *          Indicator of how the attribute is used.
 *
 */

#include <xml_schema_enum.h>

/** @defgroup xml_schema_tokenized_type Xml Tokenized Type
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_tokenized_type xml_schema_tokenized_type_t;
typedef struct xml_schema_tokenized_type_ops 
        xml_schema_tokenized_type_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_tokenized_type_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *tokenized_type,
            const axis2_env_t *env);

    xml_schema_enum_t *(AXIS2_CALL *
    get_base_impl) (void *tokenized_type,
                    const axis2_env_t *env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_values)(void *tokenized_type,
                const axis2_env_t *env);
                
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(void *tokenized_type,
                const axis2_env_t *env);
                
    xml_schema_types_t (AXIS2_CALL *
    get_type)(void *tokenized_type,
          const axis2_env_t *env);                                
    
};

struct xml_schema_tokenized_type
{
    xml_schema_enum_t base;
    xml_schema_tokenized_type_ops_t *ops;
};

AXIS2_EXTERN xml_schema_tokenized_type_t * AXIS2_CALL
xml_schema_tokenized_type_create(const axis2_env_t *env,
                                    axis2_char_t* value);

/********************** Macros **************************************************************/

#define XML_SCHEMA_TOKENIZED_TYPE_FREE(tokenized_type, env) \
      (((xml_schema_tokenized_type_t *) tokenized_type)->ops->free(tokenized_type, env))

#define XML_SCHEMA_TOKENIZED_TYPE_GET_BASE_IMPL(tokenized_type, env) \
      (((xml_schema_tokenized_type_t *) tokenized_type)->ops->get_base_impl(tokenized_type, env))

#define XML_SCHEMA_TOKENIZED_TYPE_GET_VALUES(tokenized_type, env) \
      (((xml_schema_tokenized_type_t *) tokenized_type)->ops->values(tokenized_type, env))
      
#define XML_SCHEMA_TOKENIZED_TYPE_SUPER_OBJS(tokenized_type, env) \
      (((xml_schema_tokenized_type_t *) tokenized_type)->ops->super_objs(tokenized_type, env))      

#define XML_SCHEMA_TOKENIZED_TYPE_GET_TYPE(tokenized_type, env) \
      (((xml_schema_tokenized_type_t *) tokenized_type)->ops->\
        get_type(tokenized_type, env))
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_TOKENIZED_TYPE_H */
