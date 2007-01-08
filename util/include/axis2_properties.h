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

    /**
     * @defgroup axis2_properties properties
     * @ingroup axis2_util
     * @{
     */

    typedef struct axis2_properties axis2_properties_t;

    /**
     * create new properties
     * @return properties newly created properties
     */
    AXIS2_EXTERN axis2_properties_t * AXIS2_CALL
    axis2_properties_create(const axis2_env_t *env);

    /**
     * free w2c_properties.
     * @param  properties pointer to properties struct
     * @param  env Environment. MUST NOT be NULL
     * @return status of the op. AXIS2_SUCCESS on success
     *     else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_properties_free(axis2_properties_t *properties,
        const axis2_env_t *env);

    /**
     * get string value for property with specified key.
     * @param  properties pointer to properties struct
     * @param  env Environment. MUST NOT be NULL
     * @param  key MUST NOT be NULL
     * @return value of the property
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_properties_get_property(axis2_properties_t *properties,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_properties_set_property(axis2_properties_t *properties,
        const axis2_env_t *env,
        axis2_char_t *key,
        axis2_char_t *value);
    /**
     * retrieve the hash with all the properties
     * @param  properties pointer to properties struct
     * @param  env Environment. MUST NOT be NULL
     * @return hash (key,value)
     */
    AXIS2_EXTERN axis2_hash_t* AXIS2_CALL
    axis2_properties_get_all(axis2_properties_t *properties,
        const axis2_env_t *env);

    /**
     * load properties
     * @param  properties pointer to properties struct
     * @param  env Environment. MUST NOT be NULL
     * @param  input Input Stream. MUST NOT be NULL
     * @return status of the op. AXIS2_SUCCESS on success
     *     else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_properties_load(axis2_properties_t *properties,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_properties_store(axis2_properties_t *properites,
        const axis2_env_t *env,
        FILE *output);

#define AXIS2_PROPERTIES_FREE(properties, env) \
      axis2_properties_free (properties, env)

#define AXIS2_PROPERTIES_GET_PROPERTY(properties, env, key) \
      axis2_properties_get_property(properties, env, key)

#define AXIS2_PROPERTIES_SET_PROPERTY(properties, env, key, value) \
      axis2_properties_set_property(properties, env, key, value)

#define AXIS2_PROPERTIES_GET_ALL(properties, env) \
      axis2_properties_get_all(properties, env)

#define AXIS2_PROPERTIES_LOAD(properties, env, input) \
      axis2_properties_load(properties, env, input)

#define AXIS2_PROPERTIES_STORE(properties, env, output) \
      axis2_properties_store(properties, env, output)

    /*************************** End of function macros ***************************/



    /** @} */

#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_PROPERTIES_H */
