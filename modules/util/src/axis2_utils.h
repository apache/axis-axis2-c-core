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

#ifndef AXIS2_UTILS_H
#define AXIS2_UTILS_H

#include <axis2.h>
#include <axis2_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_utils Utils
 * @ingroup axis2_utils
 * @{
 */
#define AXIS2_REQUEST_URL_PREFIX "/services"
    /**
     * This function allows users to reolve the service and op from the 
     * url. It returns an array of 2 elements of axis2_char_t arrays (strings).
     * The caller is responsible to free the memory allocated by the function
     * for the return value.
     *
     * @param request url
     * @return axis2_char_t ** <code>axis2_char_t **<code>
     */
    AXIS2_DECLARE(axis2_char_t**)
    axis2_parse_request_url_for_svc_and_op(axis2_env_t **env, axis2_char_t *request);

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_UTILS_H */
