/*
 * Licensedo to the Apache Software Foundation (ASF) under one or more
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

#ifndef AXIS2_HTTP_TRANSPORT_UTILS_H
#define AXIS2_HTTP_TRANSPORT_UTILS_H

#define AXIS2_MTOM_OUTPUT_BUFFER_SIZE 1024

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

/**
  * @file axis2_http_transport_utils.h
  * @brief axis2 HTTP Transport Utility functions
  * This file includes functions that handles soap and rest request
  * that comes to the engine via HTTP protocol.
  */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axutil_env.h>
#include <axiom_stax_builder.h>
#include <axis2_msg_ctx.h>
#include <axis2_conf_ctx.h>
#include <axutil_hash.h>
#include <axiom_element.h>
#include <axutil_stream.h>
#include <axiom_soap_envelope.h>
#include <axutil_http_chunked_stream.h>
#include <axis2_http_out_transport_info.h>
#include <axutil_url.h>
#include <axiom_mtom_sending_callback.h>

#ifdef __cplusplus
extern "C"
{
#endif
	
	typedef enum axis2_http_method_types
	{
		AXIS2_HTTP_METHOD_GET = 0,
		AXIS2_HTTP_METHOD_POST,
		AXIS2_HTTP_METHOD_HEAD,
		AXIS2_HTTP_METHOD_PUT,
		AXIS2_HTTP_METHOD_DELETE
	}axis2_http_method_types_t;

	
	typedef struct axis2_http_transport_in
	{
		/** HTTP Content type */
		axis2_char_t *content_type;
		/** HTTP Content length */
		int content_length;
		/** Input message context */
		axis2_msg_ctx_t *msg_ctx;
		
		/** soap action */
		axis2_char_t *soap_action;
		
		/** complete request uri */
		axis2_char_t *request_uri;
		
		/** Input stream */
		axutil_stream_t *in_stream;
		
		/** remote request ip  corresponds to CGI header REMOTE_ADDR */
		axis2_char_t *remote_ip;
		
		/** server port */
		axis2_char_t *svr_port;
		
		/** HTTP transfer encoding header value */
		axis2_char_t *transfer_encoding;
		
		/** HTTP Accept header */
		axis2_char_t *accept_header;
		
		/** HTTP Accept language header */
		axis2_char_t *accept_language_header;
		
		/** HTTP accept charset header */
		axis2_char_t *accept_charset_header;
		/** H&TTP METHOD  Should be one of AXIS2_HTTP_METHOD_GET | AXIS2_HTTP_METHOD_POST |"
		AXIS2_HTTP_METHOD_HEAD | AXIS2_HTTP_METHOD_PUT | AXIS2_HTTP_METHOD_DELETE" */
		int request_method;
		/** out transport */
		axis2_http_out_transport_info_t *out_transport_info;
		/** this is for serving services html */
		axis2_char_t *request_url_prefix;

	}axis2_http_transport_in_t;

	typedef struct axis2_http_transport_out
	{
		/** HTTP Status code string */
		axis2_char_t *http_status_code_name;
		/** HTTP Status code value */
		int http_status_code;
		/** Out message context */
		axis2_msg_ctx_t *msg_ctx;
		/** Response data */
		void *response_data;
		/** HTTP content type */
		axis2_char_t *content_type;
		/** Response data length */
		int response_data_length;
		/** content language */
		axis2_char_t *content_language;
		/** output headers list */
		axutil_array_list_t *output_headers;
		
	}axis2_http_transport_out_t;


	/** 
	 * Initialize the axis2_http_tranport_in_t. Before using this structure users should 
	 * initialize it using this method.
	 * @param in a pointer to a axis2_http_tranport_in_t 
	 * @param env, environments 
	 */
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	axis2_http_transport_utils_transport_in_init(axis2_http_transport_in_t *in, 
												const axutil_env_t *env);

	/** 
	 * Uninitialize the axis2_http_tranport_in_t. Before using this structure users should 
	 * initialize it using this method.
	 * @param in a pointer to a axis2_http_tranport_in_t 
	 * @param env, environments 
	 */
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	axis2_http_transport_utils_transport_in_uninit(axis2_http_transport_in_t *request, 
												   const axutil_env_t *env);

	/** 
	 * Initialize the axis2_http_tranport_out_t. Before using this structure users should 
	 * initialize it using this method.
	 * @param out a pointer to a axis2_http_tranport_out_t 
	 * @param env, environments 
	 */
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	axis2_http_transport_utils_transport_out_init(axis2_http_transport_out_t *out, 
												const axutil_env_t *env);


	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	axis2_http_transport_utils_transport_out_uninit(axis2_http_transport_out_t *response, 
												const axutil_env_t *env);
	/**
	* This methods provides the HTTP request handling functionality using axis2 for server side 
	* HTTP modules.
	* @param env, environments 
	* @param conf_ctx, Instance of axis2_conf_ctx_t
	* @param request, populated instance of axis2_http_transport_in_t struct
	* @param response, an instance of axis2_http_transport_out_t struct
	* @returns AXIS2_SUCCESS on success, AXIS2_FAILURE Otherwise
	*/
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	axis2_http_transport_utils_process_request(
		const axutil_env_t *env,
		axis2_conf_ctx_t *conf_ctx,
		axis2_http_transport_in_t *request,
		axis2_http_transport_out_t *response);



    /**
    * This function handles the HTTP POST request that comes to the axis2 engine.
	* The request can be either a SOAP request OR a REST request. 
    * @param env, axutil_env_t instance
    * @param msg_ctx, Input message context. (an instance of axis2_msg_ctx_t struct.) 
    * @param in_stream, This is the input message content represented as an axutil_stream instance.
	*  A callback function will be used to read as required from the stream with in the engine.
    * @param out_stream, This is the output stream. The outgoing message contents is represented as
	*                 an instance of axutil_stream
    * @param content_type, HTTP content type. This value should not be null.
    * @param content_length, HTTP Content length value.
    * @param soap_action_header, SOAPAction header value. This is only required in case of SOAP 1.1.
	*         For SOAP 1.2 , the action header will be within the ContentType header and
	*         this method is able to obtain the extract the action header value from content type.
    * @param request_uri, This is the HTTP request uri. Should not be null.
    * @returns AXIS2_SUCCESS on success, AXIS2_FAILURE Otherwise
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_transport_utils_process_http_post_request(
        const axutil_env_t * env,
        axis2_msg_ctx_t * msg_ctx,
        axutil_stream_t * in_stream,
        axutil_stream_t * out_stream,
        const axis2_char_t * content_type,
        const int content_length,
        axutil_string_t * soap_action_header,
        const axis2_char_t * request_uri);


    /**
    * This method handles the HTTP put request. Parameters are similar to that of post_request
	* method.
    * @param env, environment
    * @param msg_ctx, in message context.
    * @param in_stream, input stream
    * @param out_stream, output stream.
    * @param content_type, HTTP ContentType header value
    * @param content_length, HTTP Content length value
    * @param soap_action_header, SOAP Action header value
    * @param request_uri, request uri
    * @returns AXIS2_SUCCESS on success, AXIS2_FAILURE Otherwise
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_transport_utils_process_http_put_request(
        const axutil_env_t * env,
        axis2_msg_ctx_t * msg_ctx,
        axutil_stream_t * in_stream,
        axutil_stream_t * out_stream,
        const axis2_char_t * content_type,
        const int content_length,
        axutil_string_t * soap_action_header,
        const axis2_char_t * request_uri);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_http_transport_utils_process_http_get_request(
        const axutil_env_t * env,
        axis2_msg_ctx_t * msg_ctx,
        axutil_stream_t * in_stream,
        axutil_stream_t * out_stream,
        const axis2_char_t * content_type,
        axutil_string_t * soap_action_header,
        const axis2_char_t * request_uri,
        axis2_conf_ctx_t * conf_ctx,
        axutil_hash_t * request_params);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL

    axis2_http_transport_utils_process_http_head_request(
        const axutil_env_t * env,
        axis2_msg_ctx_t * msg_ctx,
        axutil_stream_t * in_stream,
        axutil_stream_t * out_stream,
        const axis2_char_t * content_type,
        axutil_string_t * soap_action_header,
        const axis2_char_t * request_uri,
        axis2_conf_ctx_t * conf_ctx,
        axutil_hash_t * request_params);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL

    axis2_http_transport_utils_process_http_delete_request(
        const axutil_env_t * env,
        axis2_msg_ctx_t * msg_ctx,
        axutil_stream_t * in_stream,
        axutil_stream_t * out_stream,
        const axis2_char_t * content_type,
        axutil_string_t * soap_action_header,
        const axis2_char_t * request_uri,
        axis2_conf_ctx_t * conf_ctx,
        axutil_hash_t * request_params);

    AXIS2_EXTERN axiom_stax_builder_t *AXIS2_CALL

    axis2_http_transport_utils_select_builder_for_mime(
        const axutil_env_t * env,
        axis2_char_t * request_uri,
        axis2_msg_ctx_t * msg_ctx,
        axutil_stream_t * in_stream,
        axis2_char_t * content_type);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL

    axis2_http_transport_utils_do_write_mtom(
        const axutil_env_t * env,
        axis2_msg_ctx_t * msg_ctx);

    AXIS2_EXTERN axutil_hash_t *AXIS2_CALL

    axis2_http_transport_utils_get_request_params(
        const axutil_env_t * env,
        axis2_char_t * request_uri);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_not_found(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_not_implemented(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_method_not_allowed(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_not_acceptable(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_bad_request(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_request_timeout(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_conflict(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_gone(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_precondition_failed(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_request_entity_too_large(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_service_unavailable(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_transport_utils_get_internal_server_error(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL

    axis2_http_transport_utils_get_services_html(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL

    axis2_http_transport_utils_get_services_static_wsdl(
        const axutil_env_t * env,
        axis2_conf_ctx_t * conf_ctx,
        axis2_char_t * request_url);

    AXIS2_EXTERN axutil_hash_t *AXIS2_CALL

    axis2_http_transport_utils_get_request_params(
        const axutil_env_t * env,
        axis2_char_t * request_uri);

    AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL

    axis2_http_transport_utils_create_soap_msg(
        const axutil_env_t * env,
        axis2_msg_ctx_t * msg_ctx,
        const axis2_char_t * soap_ns_uri);

	AXIS2_EXTERN axutil_array_list_t* AXIS2_CALL
	axis2_http_transport_utils_process_accept_headers(
		const axutil_env_t *env,
		axis2_char_t *accept_value);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_transport_utils_send_mtom_message(
        axutil_http_chunked_stream_t * chunked_stream,
        const axutil_env_t * env,
        axutil_array_list_t *mime_parts,
        axis2_char_t *sending_callback_name);

    AXIS2_EXTERN void AXIS2_CALL 
    axis2_http_transport_utils_destroy_mime_parts(
        axutil_array_list_t *mime_parts,
        const axutil_env_t *env);

    AXIS2_EXTERN void *AXIS2_CALL 
        axis2_http_transport_utils_initiate_callback(
        const axutil_env_t *env,
        axis2_char_t *callback_name,
        void *user_param,
        axiom_mtom_sending_callback_t **callback);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL axis2_http_transport_utils_is_callback_required(
        const axutil_env_t *env,
        axutil_array_list_t *mime_parts);



    /** @} */
#ifdef __cplusplus
}
#endif

#endif       /* AXIS2_HTTP_TRANSPORT_UTILS_H */
