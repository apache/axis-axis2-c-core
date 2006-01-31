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

#ifndef AXIS2_HTTP_TRANSPORT_UTILS_H
#define AXIS2_HTTP_TRANSPORT_UTILS_H


/**
  * @file axis2_http_transport_utils.h
  * @brief axis2 HTTP Transport Utility class
  */

#include <axis2.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_om_stax_builder.h>
#include <axis2_msg_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_hash.h>
#include <axis2_om_element.h>
#include <axis2_stream.h>
#include <axis2_soap_envelope.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

/* 
 * struct to hold the callback information
 */	
struct axis2_callback_info
{
	axis2_env_t *env;
	axis2_stream_t *in_stream;
	int content_length;
	int unread_len;
};
typedef struct axis2_callback_info axis2_callback_info_t;

axis2_status_t AXIS2_CALL 
axis2_http_transport_utils_process_http_post_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
                        axis2_stream_t *in_stream, axis2_stream_t *out_stream,
						axis2_char_t *content_type, int content_length, 
                        axis2_char_t *soap_action_header,
                        axis2_char_t *request_uri);
    
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_process_http_get_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx,
                        axis2_stream_t *in_stream, axis2_stream_t *out_stream,
						axis2_char_t *content_type,
                        axis2_char_t *soap_action_header, 
                        axis2_char_t *request_uri, axis2_conf_ctx_t *conf_ctx, 
                        axis2_hash_t *request_params);
    
axis2_soap_envelope_t* AXIS2_CALL 
axis2_http_transport_utils_create_envelope_from_get_request
                        (axis2_env_t **env, axis2_char_t *request_uri,
                        axis2_hash_t *request_params);
    
axis2_om_stax_builder_t* AXIS2_CALL 
axis2_http_transport_utils_select_builder_for_mime
                        (axis2_env_t **env, axis2_char_t *request_uri,
                        axis2_msg_ctx_t *msg_ctx, axis2_stream_t *in_stream,
                        axis2_char_t *content_type);
   
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_do_write_mtom(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx);
                                                
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_doing_rest(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx);
                                                
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_doing_rest_through_post
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
						
axis2_hash_t *AXIS2_CALL
axis2_http_transport_utils_get_request_params(axis2_env_t **env, 
						axis2_char_t *request_uri);
						
axis2_char_t* AXIS2_CALL
axis2_http_transport_utils_get_services_html(axis2_env_t **env, 
							axis2_conf_ctx_t *conf_ctx);

axis2_hash_t *AXIS2_CALL
axis2_http_transport_utils_get_request_params(axis2_env_t **env, 
						axis2_char_t *request_uri);

axis2_soap_envelope_t* AXIS2_CALL
axis2_http_transport_utils_create_soap_msg(axis2_env_t **env, 
                        axis2_msg_ctx_t *msg_ctx, axis2_char_t *soap_ns_uri);


/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_TRANSPORT_UTILS_H */
