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

#ifndef AXIS2_GENERIC_OBJ_H
#define AXIS2_GENERIC_OBJ_H

#include <axis2_utils_defines.h>
#include <axutil_error.h>
#include <axutil_env.h>
#include <axis2_utils.h>


#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_generic_obj axis2_generic_obj_t;

    /**
     * @defgroup axis2_generic_obj generic object
     * @ingroup axis2_util
     * @{
     */

    /**
     * create new generic_obj
     * @return generic_obj newly created generic_obj
     */
    AXIS2_EXTERN axis2_generic_obj_t * AXIS2_CALL
    axis2_generic_obj_create(const axutil_env_t *env);

    AXIS2_EXTERN void AXIS2_CALL
    axis2_generic_obj_free(axis2_generic_obj_t *generic_obj,
        const axutil_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_generic_obj_set_free_func(axis2_generic_obj_t *generic_obj,
        const axutil_env_t *env,
        AXIS2_FREE_VOID_ARG free_func);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_generic_obj_set_value(axis2_generic_obj_t *generic_obj,
        const axutil_env_t *env,
        void *value);

    AXIS2_EXTERN void * AXIS2_CALL
    axis2_generic_obj_get_value(axis2_generic_obj_t *generic_obj,
        const axutil_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_generic_obj_set_type(axis2_generic_obj_t *generic_obj,
        const axutil_env_t *env,
        int type);

    AXIS2_EXTERN int AXIS2_CALL
    axis2_generic_obj_get_type(axis2_generic_obj_t *generic_obj,
        const axutil_env_t *env);


#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_GENERIC_OBJ_H */


