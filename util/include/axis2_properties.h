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

#ifndef AXIS2_PROPERTIES_H
#define AXIS2_PROPERTIES_H

#include <axis2_utils_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>
#include <axis2_hash.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
typedef struct axis2_properties axis2_properties_t;
typedef struct axis2_properties_ops axis2_properties_ops_t;
    
/**
 * @defgroup axis2_properties properties
 * @ingroup axis2_util 
 * @{
 */

/** 
 * @brief properties ops struct
 * Encapsulator struct for ops of axis2_properties_ops
 */
 struct axis2_properties_ops
{
   /**
    * free w2c_properties.
    * @param  properties pointer to properties struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_properties_t *properties, 
            const axis2_env_t *env);

   /**
    * get string value for property with specified key.
    * @param  properties pointer to properties struct
    * @param  env Environment. MUST NOT be NULL
    * @param  key MUST NOT be NULL
    * @return value of the property
    */
    axis2_char_t* (AXIS2_CALL *
    get_property) (axis2_properties_t *properties,
                const axis2_env_t *env,
                axis2_char_t *key);
   /**
    * set a property ( key, value) pair.
    * @param  properties pointer to properties struct
    * @param  env Environment. MUST NOT be NULL
    * @param  key Property Key. MUST NOT be NULL
    * @param  value Property Value
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_property) (axis2_properties_t *properties,
                 const axis2_env_t *env,
                 axis2_char_t *key,
                 axis2_char_t *value);
   /**
    * retrieve the hash with all the properties
    * @param  properties pointer to properties struct
    * @param  env Environment. MUST NOT be NULL
    * @return hash (key,value)
    */
    axis2_hash_t* (AXIS2_CALL *
    get_all)( axis2_properties_t *properties,
                const axis2_env_t *env);

   /**
    * load properties
    * @param  properties pointer to properties struct
    * @param  env Environment. MUST NOT be NULL
    * @param  input Input Stream. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    load)(axis2_properties_t *properties,
                 const axis2_env_t *env,
                 axis2_char_t *input_filename);
  
   /**
    * store properties
    * @param  properties pointer to properties struct
    * @param  env Environment. MUST NOT be NULL
    * @param  ouput Output Stream. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    store)(axis2_properties_t *properites,
                const axis2_env_t *env,
                FILE *output);
};
    
     
/** 
 * @brief
 */ 
 struct axis2_properties
{
   axis2_properties_ops_t *ops;
};

/**
 * create new properties
 * @return properties newly created properties
 */
AXIS2_EXTERN axis2_properties_t * AXIS2_CALL
axis2_properties_create(const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_PROPERTIES_FREE(properties, env) \
      ((properties)->ops->free (properties, env))

#define AXIS2_PROPERTIES_GET_PROPERTY(properties, env, key) \
      ((properties)->ops->get_property(properties, env, key))  

#define AXIS2_PROPERTIES_SET_PROPERTY(properties, env, key, value) \
      ((properties)->ops->set_property(properties, env, key, value))  

#define AXIS2_PROPERTIES_GET_ALL(properties, env) \
      ((properties)->ops->get_all(properties, env))  

#define AXIS2_PROPERTIES_LOAD(properties, env, input) \
      ((properties)->ops->load(properties, env, input)) 

#define AXIS2_PROPERTIES_STORE(properties, env, output) \
      ((properties)->ops->store(properties, env, output))
         
/*************************** End of function macros ***************************/



/** @} */
    
#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_PROPERTIES_H */
