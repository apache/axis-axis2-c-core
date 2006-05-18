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
 * @file axis2_rest_sender.h
 * @brief axis2 REST sender
 */

#include <axis2_const.h>
#include <axis2_utils_defines.h>
#include <axis2_env.h>
#include <axis2_msg_ctx.h>
#include <axis2_om_output.h>
#include <axis2_http_simple_response.h>
#include <axis2_soap_envelope.h>


#ifdef __cplusplus
extern "C" 
{
#endif
/** 
  * @ingroup axis2_core_transport_http
  * @{
  */

typedef struct axis2_rest_sender_ops axis2_rest_sender_ops_t;
typedef struct axis2_rest_sender axis2_rest_sender_t;
    
    
/** 
 * @brief REST sender ops struct
 * Encapsulator struct for ops of axis2_rest_sender
 */
AXIS2_DECLARE_DATA struct axis2_rest_sender_ops
{
	axis2_status_t (AXIS2_CALL *send) 
                                (axis2_rest_sender_t *sender, 
                                axis2_env_t **env, axis2_msg_ctx_t *msg_ctx,
								axis2_om_node_t *out, 
								axis2_char_t *str_url);
	
	axis2_status_t (AXIS2_CALL *set_chunked) 
                                (axis2_rest_sender_t *sender, 
                                axis2_env_t **env, axis2_bool_t chunked);
	
	axis2_status_t (AXIS2_CALL *set_om_output)
                                (axis2_rest_sender_t *sender, 
                                axis2_env_t **env, axis2_om_output_t *om_output);
	
	axis2_status_t (AXIS2_CALL *set_http_version)
								(axis2_rest_sender_t *sender, 
								axis2_env_t **env, axis2_char_t *version);
	
	axis2_status_t (AXIS2_CALL *free) 
                                (axis2_rest_sender_t *sender, 
                                axis2_env_t **env);
};

    
axis2_status_t AXIS2_CALL 
axis2_rest_sender_get_header_info 
								(axis2_rest_sender_t *sender, 
                                axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
								axis2_http_simple_response_t *response);

axis2_status_t AXIS2_CALL
axis2_rest_sender_process_response 
								(axis2_rest_sender_t *sender, 
                                axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
								axis2_http_simple_response_t *response);

axis2_status_t AXIS2_CALL
axis2_rest_sender_get_timeout_values 
								(axis2_rest_sender_t *sender, 
                                axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);

/**
 * @brief REST sender struct
 * Axis2 REST sender
 */
AXIS2_DECLARE_DATA struct axis2_rest_sender
{
	axis2_rest_sender_ops_t *ops;
};


AXIS2_DECLARE(axis2_rest_sender_t *) 
axis2_rest_sender_create(axis2_env_t **env);

/********************* Start of function macros	***************************/

#define AXIS2_REST_SENDER_SEND(sender, env, msg_ctx, output, url)\
                        ((sender)->ops->send (sender, env, msg_ctx,output, url))
#define AXIS2_REST_SENDER_SET_CHUNKED(sender, env, chunked) \
                        ((sender)->ops->set_chunked(sender, env, chunked))
#define AXIS2_REST_SENDER_SET_OM_OUTPUT(sender, env, om_output) \
                        ((sender)->ops->set_om_output (sender, env, om_output))
#define AXIS2_REST_SENDER_SET_HTTP_VERSION(sender, env, version)\
                        ((sender)->ops->set_http_version (sender, env, version))
#define AXIS2_REST_SENDER_FREE(sender, env) \
                        ((sender)->ops->free(sender, env))

/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_REST_SENDER_H */
