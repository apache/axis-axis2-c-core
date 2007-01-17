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

#ifndef AXIS2_PROPERTY_H
#define AXIS2_PROPERTY_H

#include <axis2_utils_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>
#include <axis2_utils.h>


#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axis2_property property
     * @ingroup axis2_util
     * @{
     */

    typedef struct axis2_property axis2_property_t;

    /**
     * create new property
     * @return property newly created property
     */
    AXIS2_EXTERN axis2_property_t * AXIS2_CALL
    axis2_property_create(
        const axis2_env_t *env);

    /**
     * create new property
     * @param env axis2 environment
     * @param scope scope can be one of following
     *              AXIS2_SCOPE_REQUEST
     *              AXIS2_SCOPE_SESSION
     *              AXIS2_SCOPE_APPLICATION
     *              pass 0 to use default scope of AXIS2_SCOPE_REQUEST
     * @param free_func free function for the value freeing. Pass 0 if
     *              param value is a string
     * @param value value of the property
     * @return property newly created property
     */
    AXIS2_EXTERN axis2_property_t * AXIS2_CALL
    axis2_property_create_with_args(
        const axis2_env_t *env,
        axis2_scope_t scope,
        axis2_bool_t own_value,
        AXIS2_FREE_VOID_ARG free_func,
        void *value);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_property_free(axis2_property_t *property,
        const axis2_env_t *env);

    /**
     * Default scope is AXIS2_SCOPE_REQUEST
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_property_set_scope(axis2_property_t *property,
        const axis2_env_t *env,
        axis2_scope_t scope);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_property_set_free_func(axis2_property_t *property,
        const axis2_env_t *env,
        AXIS2_FREE_VOID_ARG free_func);

/*************************** Function macros **********************************/
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_property_set_value(axis2_property_t *property,
        const axis2_env_t *env,
        void *value);
        
    AXIS2_EXTERN void * AXIS2_CALL
    axis2_property_get_value(axis2_property_t *property,
        const axis2_env_t *env);

    axis2_status_t AXIS2_CALL
    axis2_property_set_own_value(
        axis2_property_t *property,
        const axis2_env_t *env,
        axis2_bool_t own_value);

    AXIS2_EXTERN axis2_property_t* AXIS2_CALL
    axis2_property_clone(axis2_property_t *property,
        const axis2_env_t *env);

#define AXIS2_PROPERTY_FREE(property, env) \
      axis2_property_free (property, env)

#define AXIS2_PROPERTY_SET_FREE_FUNC(property, env, free_func) \
      axis2_property_set_free_func (property, env, free_func)

#define AXIS2_PROPERTY_SET_SCOPE(property, env, scope) \
      axis2_property_set_scope (property, env, scope)

#define AXIS2_PROPERTY_SET_VALUE(property, env, value) \
      axis2_property_set_value (property, env, value)

#define AXIS2_PROPERTY_GET_VALUE(property, env) \
        axis2_property_get_value(property, env)

#define AXIS2_PROPERTY_CLONE(property, env) \
        axis2_property_clone(property, env)

/*************************** End of function macros ***************************/

    /** @} */

#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_PROPERTY_H */
