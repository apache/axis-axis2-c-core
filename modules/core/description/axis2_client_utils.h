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

#ifndef AXIS2_CLIENT_UTILS_H
#define AXIS2_CLIENT_UTILS_H

/** @defgroup axis2_client_utils Client Utils
 * @ingroup axis2_description
 * @{
 */

/**
 * @file axis2_client_utils.h
 * @brief Axis2 Client Utils interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Creates service for a given WSDL. 
     * @param env pointer to environment struct
     * @param wsdl_url location of the WSDL
     * @param wsdl_svc_qname QName of the service to be invoked, if it is NULL 
     * then the first service in the WSDL will be selected by default if there
     * are any services in the WSDL
     * @param ep_name name of the endpoint, if it is NULL then the first one, 
     * if any, in the WSDL would be selected
     * @param doc_base_uri document base URI
     * @param options service client options
     * @return axis2_svc pointer to newly created service
     */
    AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
    axis2_client_utils_create_axis2_svc(
        const axis2_env_t *env,
        const axis2_uri_t *wsdl_uri,
        const axis2_qname_t *wsdl_svc_qname,
        const axis2_char_t *ep_name,
        const axis2_char_t *doc_base_uri,
        axis2_options_t *options);

    /** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_CLIENT_UTILS_H */
