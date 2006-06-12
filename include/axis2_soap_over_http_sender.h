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
 
#ifndef AXIOM_SOAP_OVER_HTTP_SENDER_H
#define AXIOM_SOAP_OVER_HTTP_SENDER_H

/**
 * @file axiom_soap_over_http_sender.h
 * @brief axis2 SOAP over HTTP sender
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_msg_ctx.h>
#include <axiom_output.h>
#include <axis2_http_simple_response.h>
#include <axiom_soap_envelope.h>


#ifdef __cplusplus
extern "C" 
{
#endif
/** 
  * @ingroup axis2_core_transport_http
  * @{
  */

typedef struct axiom_soap_over_http_sender_ops 
                     axiom_soap_over_http_sender_ops_t;
typedef struct axiom_soap_over_http_sender axiom_soap_over_http_sender_t;
    
    
/** 
 * @brief SOAP over HTTP sender ops struct
 * Encapsulator struct for ops of axiom_soap_over_http_sender
 */
AXIS2_DECLARE_DATA struct axiom_soap_over_http_sender_ops
{
   axis2_status_t (AXIS2_CALL *send) 
                                (axiom_soap_over_http_sender_t *sender, 
                                const axis2_env_t *env, axis2_msg_ctx_t *msg_ctx,
                        axiom_soap_envelope_t *out, 
                        const axis2_char_t *str_url, 
                        const axis2_char_t *soap_action);
   
   axis2_status_t (AXIS2_CALL *set_chunked) 
                                (axiom_soap_over_http_sender_t *sender, 
                                const axis2_env_t *env, axis2_bool_t chunked);
   
   axis2_status_t (AXIS2_CALL *set_om_output)
                                (axiom_soap_over_http_sender_t *sender, 
                                const axis2_env_t *env, axiom_output_t *om_output);
   
   axis2_status_t (AXIS2_CALL *set_http_version)
                        (axiom_soap_over_http_sender_t *sender, 
                        const axis2_env_t *env, axis2_char_t *version);
   
   axis2_status_t (AXIS2_CALL *free) 
                                (axiom_soap_over_http_sender_t *sender, 
                                const axis2_env_t *env);
};

    
axis2_status_t AXIS2_CALL 
axiom_soap_over_http_sender_get_header_info 
                        (axiom_soap_over_http_sender_t *sender, 
                                const axis2_env_t *env, axis2_msg_ctx_t *msg_ctx, 
                        axis2_http_simple_response_t *response);

axis2_status_t AXIS2_CALL
axiom_soap_over_http_sender_process_response 
                        (axiom_soap_over_http_sender_t *sender, 
                                const axis2_env_t *env, axis2_msg_ctx_t *msg_ctx, 
                        axis2_http_simple_response_t *response);

axis2_status_t AXIS2_CALL
axiom_soap_over_http_sender_get_timeout_values 
                        (axiom_soap_over_http_sender_t *sender, 
                                const axis2_env_t *env, axis2_msg_ctx_t *msg_ctx);

/**
 * @brief HTTP SOAP over HTTP sender struct
 * Axis2 SOAP over HTTP sender
 */
AXIS2_DECLARE_DATA struct axiom_soap_over_http_sender
{
   axiom_soap_over_http_sender_ops_t *ops;
};


AXIS2_EXTERN axiom_soap_over_http_sender_t * AXIS2_CALL 
axiom_soap_over_http_sender_create(const axis2_env_t *env);

/********************* Start of function macros   ***************************/

#define AXIOM_SOAP_OVER_HTTP_SENDER_SEND(sender, env, msg_ctx, output, url, \
                  soap_action) ((sender)->ops->send (sender, env, msg_ctx\
                  ,output, url, soap_action))
#define AXIOM_SOAP_OVER_HTTP_SENDER_SET_CHUNKED(sender, env, chunked) \
                        ((sender)->ops->set_chunked(sender, env, chunked))
#define AXIOM_SOAP_OVER_HTTP_SENDER_SET_OM_OUTPUT(sender, env, om_output) \
                        ((sender)->ops->set_om_output (sender, env, om_output))
#define AXIOM_SOAP_OVER_SENDER_SET_HTTP_VERSION(sender, env, version)\
                        ((sender)->ops->set_http_version (sender, env, version))
#define AXIOM_SOAP_OVER_HTTP_SENDER_FREE(sender, env) \
                        ((sender)->ops->free(sender, env))

/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIOM_SOAP_OVER_HTTP_SENDER_H */
