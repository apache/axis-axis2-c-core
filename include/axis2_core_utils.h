
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

#ifndef AXIS2_CORE_UTILS_H
#define AXIS2_CORE_UTILS_H

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axutil_error.h>
#include <axutil_env.h>
#include <axis2_msg_ctx.h>
#include <axis2_op.h>
#include <axutil_qname.h>
#include <axis2_core_dll_desc.h>

    /** Forward declaration; including axis2_conf_ctx.h here would be circular. */
    typedef struct axis2_conf_ctx axis2_conf_ctx_t;

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_conf;

    /**
     * @defgroup axis2_core_utils Core Utils
     * @ingroup axis2_core_utils
     * @{
     */
    AXIS2_EXTERN axis2_msg_ctx_t *AXIS2_CALL

    axis2_core_utils_create_out_msg_ctx(
        const axutil_env_t * env,
        axis2_msg_ctx_t * in_msg_ctx);

    AXIS2_EXTERN void AXIS2_CALL
    axis2_core_utils_reset_out_msg_ctx(
        const axutil_env_t * env,
        axis2_msg_ctx_t * out_msg_ctx);

    AXIS2_EXTERN axutil_qname_t *AXIS2_CALL

    axis2_core_utils_get_module_qname(
        const axutil_env_t * env,
        const axis2_char_t * name,
        const axis2_char_t * version);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL

    axis2_core_utils_calculate_default_module_version(
        const axutil_env_t * env,
        axutil_hash_t * modules_map,
        struct axis2_conf *axis_conf);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_core_utils_get_module_name(
        const axutil_env_t * env,
        axis2_char_t * module_name);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL

    axis2_core_utils_get_module_version(
        const axutil_env_t * env,
        axis2_char_t * module_name);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_core_utils_is_latest_mod_ver(
        const axutil_env_t * env,
        axis2_char_t * module_ver,
        axis2_char_t * current_def_ver);

    AXIS2_EXTERN axis2_op_t *AXIS2_CALL                                                                    
    axis2_core_utils_get_rest_op_with_method_and_location(axis2_svc_t *svc,
        const axutil_env_t *env,
        const axis2_char_t *method,
        const axis2_char_t *location,
        axutil_array_list_t *param_keys,
        axutil_array_list_t *param_values);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_core_utils_prepare_rest_mapping (
        const axutil_env_t * env,
        axis2_char_t * url,
        axutil_hash_t *rest_map,
        axis2_op_t *op_desc);

    /**
     * Whether a wsa:ReplyTo or wsa:FaultTo taken off an inbound message may be
     * used as the destination of a server-initiated send.
     *
     * A non-anonymous response endpoint makes the server open a connection to
     * an address the caller chose. Unless WS-Security is engaged to bind that
     * endpoint reference to a trusted issuer, the WS-Addressing specification
     * leaves it to the receiver to decide whether to honour it, so this
     * declines by default.
     *
     * Controlled by three parameters, read from the service then axis2.xml:
     *  - allowNonAnonymousResponseEndpoints (default false) refuses every
     *    non-anonymous response endpoint outright.
     *  - allowedResponseEndpointSchemes (default "https") restricts which
     *    schemes may carry a reply once they are permitted at all.
     *  - blockPrivateNetworkResponseEndpoints (default false) additionally
     *    refuses loopback and private literal addresses. Link-local, wildcard
     *    and multicast literals are refused whatever this is set to.
     *
     * @param env pointer to environment struct
     * @param conf_ctx configuration context, for parameter lookup
     * @param address the endpoint reference address, or NULL
     * @return AXIS2_TRUE if the server may send to this address
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_core_utils_is_response_endpoint_allowed(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx,
        const axis2_char_t * address);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_core_utils_free_rest_map (
        const axutil_env_t * env,
        axutil_hash_t *rest_map);



    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CORE_UTILS_H */
