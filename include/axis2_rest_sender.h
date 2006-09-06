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

#ifndef AXIS2_REST_SENDER_H
#define AXIS2_REST_SENDER_H

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

/**
 * @file axis2_rest_sender.h
 * @brief axis2 REST sender
 */

#include <axis2_const.h>
#include <axis2_utils_defines.h>
#include <axis2_env.h>
#include <axis2_msg_ctx.h>
#include <axiom_output.h>
#include <axis2_http_simple_response.h>
#include <axiom_soap_envelope.h>


#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_rest_sender_ops */
    typedef struct axis2_rest_sender_ops axis2_rest_sender_ops_t;
    /** Type name for struct axis2_rest_sender */
    typedef struct axis2_rest_sender axis2_rest_sender_t;


    /**
     * @brief REST sender ops struct
     * Encapsulator struct for ops of axis2_rest_sender
     */
    struct axis2_rest_sender_ops
    {
        /**
         * @param sender pointer to sender
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @param out out
         * @param str_url str_url
         */
        axis2_status_t (AXIS2_CALL *
                send)(
                    axis2_rest_sender_t *sender,
                    const axis2_env_t *env, 
                    axis2_msg_ctx_t *msg_ctx,
                    axiom_node_t *out,
                    const axis2_char_t *str_url);

        /**
         * @param sender pointer to sender
         * @param env pointer to environment struct
         * @param chuncked chuncked
         */
        axis2_status_t (AXIS2_CALL *
                set_chunked)(
                    axis2_rest_sender_t *sender,
                    const axis2_env_t *env, 
                    axis2_bool_t chunked);

        /**
         * @param sender pointer to sender
         * @param env pointer to environment struct
         * @param om_output om_output
         */
        axis2_status_t (AXIS2_CALL *
                set_om_output)(
                    axis2_rest_sender_t *sender,
                    const axis2_env_t *env, 
                    axiom_output_t *om_output);

        /**
         * @param sender pointer to sender
         * @param env pointer to environment struct
         * @param version pointer to version
         */
        axis2_status_t (AXIS2_CALL *
                set_http_version)(
                    axis2_rest_sender_t *sender,
                    const axis2_env_t *env, 
                    axis2_char_t *version);

        /**
         * @param sender pointer to sender
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_rest_sender_t *sender,
                    const axis2_env_t *env);
    };


    /**
     * @param sender pointer to sender
     * @param env pointer to environment struct
     * @param msg_ctx pointer to message context
     * @param respoinse pointer to response
     */
    axis2_status_t AXIS2_CALL
    axis2_rest_sender_get_header_info(
        axis2_rest_sender_t *sender,
        const axis2_env_t *env, 
        axis2_msg_ctx_t *msg_ctx,
        axis2_http_simple_response_t *response);

    /**
     * @param sender pointer to sender
     * @param env pointer to environment struct
     * @param msg_ctx pointer to message context
     * @param response response
     */
    axis2_status_t AXIS2_CALL
    axis2_rest_sender_process_response(
        axis2_rest_sender_t *sender,
        const axis2_env_t *env, 
        axis2_msg_ctx_t *msg_ctx,
        axis2_http_simple_response_t *response);

    /**
     * @param sender pointer to sender
     * @param env pointer to environment struct
     * @param msg_ctx pointer to message context struct
     */
    axis2_status_t AXIS2_CALL
    axis2_rest_sender_get_timeout_values(
        axis2_rest_sender_t *sender,
        const axis2_env_t *env, 
        axis2_msg_ctx_t *msg_ctx);

    /**
     * Axis2 REST sender
     */
    struct axis2_rest_sender
    {
        /** Operations of REST sender */
        axis2_rest_sender_ops_t *ops;
    };


    /** 
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axis2_rest_sender_t *AXIS2_CALL
    axis2_rest_sender_create(
        const axis2_env_t *env);

/********************* Start of function macros   ***************************/

/** Send.
    @sa axis2_rest_sender_ops#send */
#define AXIS2_REST_SENDER_SEND(sender, env, msg_ctx, output, url)\
                        ((sender)->ops->send (sender, env, msg_ctx,output, url))

/** Set chunked.
    @sa axis2_rest_sender_ops#set_chunked */
#define AXIS2_REST_SENDER_SET_CHUNKED(sender, env, chunked) \
                        ((sender)->ops->set_chunked(sender, env, chunked))

/** Set om output.
    @sa axis2_rest_sender_ops#set_om_output */
#define AXIS2_REST_SENDER_SET_OM_OUTPUT(sender, env, om_output) \
                        ((sender)->ops->set_om_output (sender, env, om_output))

/** Set http version.
    @sa axis2_rest_sender_ops#set_http_version */
#define AXIS2_REST_SENDER_SET_HTTP_VERSION(sender, env, version)\
                        ((sender)->ops->set_http_version (sender, env, version))

/** Frees the rest sender.
    @sa axis2_rest_sender_ops#free */
#define AXIS2_REST_SENDER_FREE(sender, env) \
                        ((sender)->ops->free(sender, env))

/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_REST_SENDER_H */
