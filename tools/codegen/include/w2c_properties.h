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
  
#ifndef W2C_PROPERTIES_PARSER_H
#define W2C_PROPERTIES_PARSER_H

/**
 * @file w2c_properties_parser.h
 * @brief handles Axis2/C w2c parse .properties file
 */

#include <stdio.h>
#include <axis2_utils.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_properties represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_properties w2c_properties_t;
typedef struct w2c_properties_ops w2c_properties_ops_t;

AXIS2_DECLARE_DATA  struct w2c_properties_ops
{
    /**
     * free w2c_properties.
     * @param  properties pointer to properties struct
     * @param  env Environment. MUST NOT be NULL
     * @return status of the op. AXIS2_SUCCESS on success
     *     else AXIS2_FAILURE
     */
 
     axis2_status_t (AXIS2_CALL *
     free)(w2c_properties_t *properties,
          const axis2_env_t *env);
    
    /**
     * parse the properties file and return hash
     * @param env double pointer to environment struct. MUST NOT be NULL
     * return hash ( key, array list of strings)
     */
     axis2_hash_t* (AXIS2_CALL *
     get_hash)(w2c_properties_t *properties,
           const axis2_env_t *env);
};

AXIS2_DECLARE_DATA  struct w2c_properties
{
     struct w2c_properties_ops *ops;
};

/**
 * create a properties obj with aonther properties
 * @param env Environment. MUST NOT be NULL
 * @param properties copying properties
 * @param filename filename of .properties
 * @param seprator sep
 * @return newly created w2c_properties object..
 */
AXIS2_EXTERN w2c_properties_t* AXIS2_CALL
w2c_properties_create( const axis2_env_t *env,
                    axis2_char_t *filename,
                    axis2_char_t seperator);

/*************************** Function macros **********************************/

#define W2C_PROPERTIES_FREE(properties, env) \
      ((properties)->ops->free (properties, env))

#define W2C_PROPERTIES_GET_HASH(properties, env) \
      ((properties)->ops->get_hash(properties, env))
   
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_PROPERTIES_PARSER_H */
