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
     * To create a Axis Service for a given WSDL and the created client is most 
     * suitable for client side invocation not for server side invocation, since 
     * all the soap acction and wsa action is added to operations
     * @param env pointer to environment struct
     * @param wsdl_url location of the WSDL
     * @param wsdl_svc_qname name of the service to be invoke , if it is null 
     *     then the first one will
     *     be selected if there are more than one
     * @param ep_name name of the endpoint , if there are more than one , 
     *     if it is null then the
     *     first one in the  iterator will be selected
     * @param doc_base_uri doc base uri
     * @param options Service client options, to set the target EPR
     * @return axis2_svc ,      the created servie will be return
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
