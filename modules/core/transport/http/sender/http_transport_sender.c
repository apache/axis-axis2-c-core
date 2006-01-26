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
 
#include <axis2_http_transport_sender.h>
#include <axis2_string.h>
#include <axis2_endpoint_ref.h>
#include <axis2_addr.h>
#include <axis2_xml_writer.h>
#include <axis2_om_output.h>
#include <axis2_http_transport_utils.h>
#include <axis2_http_out_transport_info.h>
#include <axis2_http_transport.h>
#include <axis2_soap_over_http_sender.h>

#define AXIS2_TRANSPORT_URL "TransportURL"

/** 
 * @brief HTTP Transport Sender struct impl
 *	Axis2 HTTP Transport Sender impl  
 */
typedef struct axis2_http_transport_sender_impl 
							axis2_http_transport_sender_impl_t;  
  
struct axis2_http_transport_sender_impl
{
	axis2_transport_sender_t transport_sender;
	axis2_char_t *http_version;
	axis2_bool_t chunked;
	int connection_timeout;
	int so_timeout;
	
};

#define AXIS2_INTF_TO_IMPL(transport_sender) \
                			((axis2_http_transport_sender_impl_t *)\
							(transport_sender))

/***************************** Function headers *******************************/
axis2_status_t AXIS2_CALL 
axis2_http_transport_sender_invoke
							(axis2_transport_sender_t *transport_sender, 
                    		axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
    
axis2_status_t AXIS2_CALL 
axis2_http_transport_sender_clean_up
							(axis2_transport_sender_t *transport_sender, 
                    		axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
    
axis2_status_t AXIS2_CALL 
axis2_http_transport_sender_init
							(axis2_transport_sender_t *transport_sender, 
                    		axis2_env_t **env, axis2_conf_ctx_t *conf_ctx, 
							axis2_transport_out_desc_t *out_desc);
axis2_status_t AXIS2_CALL
axis2_http_transport_sender_write_message
							(axis2_transport_sender_t *transport_sender, 
                    		axis2_env_t **env, axis2_msg_ctx_t *msg_ctx,
							axis2_endpoint_ref_t *epr, 
							axis2_soap_envelope_t *out, 
							axis2_om_output_t *om_output);
    
axis2_status_t AXIS2_CALL 
axis2_http_transport_sender_free 
							(axis2_transport_sender_t *transport_sender, 
                    		axis2_env_t **env);
/***************************** End of function headers ************************/

axis2_transport_sender_t* AXIS2_CALL
axis2_http_transport_sender_create(axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
        
    axis2_http_transport_sender_impl_t *transport_sender_impl = 
                        (axis2_http_transport_sender_impl_t *)AXIS2_MALLOC 
                        ((*env)->allocator, sizeof(
                        axis2_http_transport_sender_impl_t));
	
    if(NULL == transport_sender_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return NULL;
	}

    transport_sender_impl->http_version = AXIS2_STRDUP(
						AXIS2_HTTP_HEADER_PROTOCOL_11, env);
	transport_sender_impl->chunked = AXIS2_TRUE;
	transport_sender_impl->connection_timeout = 
						AXIS2_HTTP_DEFAULT_CONNECTION_TIMEOUT;
	transport_sender_impl->so_timeout = AXIS2_HTTP_DEFAULT_SO_TIMEOUT;
    transport_sender_impl->transport_sender.ops = AXIS2_MALLOC((*env)->allocator
						,sizeof(axis2_transport_sender_ops_t));
    if(NULL == transport_sender_impl->transport_sender.ops)
	{
		axis2_http_transport_sender_free((axis2_transport_sender_t*)
						transport_sender_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return NULL;
	}
    
    transport_sender_impl->transport_sender.ops->invoke = 
                        axis2_http_transport_sender_invoke;
    transport_sender_impl->transport_sender.ops->cleanup = 
						axis2_http_transport_sender_clean_up;
    transport_sender_impl->transport_sender.ops->init = 
						axis2_http_transport_sender_init;
    transport_sender_impl->transport_sender.ops->free = 
						axis2_http_transport_sender_free;
                        
	return &(transport_sender_impl->transport_sender);
}


axis2_status_t AXIS2_CALL 
axis2_http_transport_sender_free 
						(axis2_transport_sender_t *transport_sender, 
						axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(transport_sender, env, AXIS2_FAILURE);
    axis2_http_transport_sender_impl_t *transport_sender_impl =
                        AXIS2_INTF_TO_IMPL(transport_sender);

    if(NULL != transport_sender->ops)
        AXIS2_FREE((*env)->allocator, transport_sender->ops);
    
	AXIS2_FREE((*env)->allocator, transport_sender_impl);
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_http_transport_sender_invoke
							(axis2_transport_sender_t *transport_sender, 
                    		axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    axis2_char_t *char_set_enc = NULL;
	axis2_endpoint_ref_t *epr = NULL;
	axis2_char_t *transport_url = NULL;
	axis2_xml_writer_t *xml_writer = NULL;
	axis2_om_output_t *om_output = NULL;
	axis2_char_t *buffer = NULL;
	axis2_soap_envelope_t *soap_data_out = NULL;
	
	AXIS2_FUNC_PARAM_CHECK(transport_sender, env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    
	xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0);
	om_output = axis2_om_output_create(env, xml_writer);
	
	char_set_enc = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, 
							AXIS2_CHARACTER_SET_ENCODING, AXIS2_FALSE);
	if(NULL != char_set_enc)
	{
		axis2_op_ctx_t *op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
		if(NULL != op_ctx)
		{
            axis2_ctx_t *ctx = AXIS2_OP_CTX_GET_BASE(AXIS2_MSG_CTX_GET_OP_CTX(
                                msg_ctx, env), env);
            if (ctx)
			    char_set_enc = AXIS2_CTX_GET_PROPERTY(ctx, env, 
							AXIS2_CHARACTER_SET_ENCODING, AXIS2_FALSE);
		}
	}
	/**
	 * If we still can't find the char set enc we will
	 * use default
	 */
	if(NULL == char_set_enc)
	{
		char_set_enc = AXIS2_DEFAULT_CHAR_SET_ENCODING;
	}
		
	axis2_bool_t do_mtom = axis2_http_transport_utils_do_write_mtom(env,
							msg_ctx);
	AXIS2_MSG_CTX_SET_DOING_MTOM(msg_ctx, env, do_mtom);
	AXIS2_MSG_CTX_SET_DOING_REST(msg_ctx, 
							env, axis2_http_transport_utils_is_doing_rest(env, 
							msg_ctx));
	transport_url = (axis2_char_t*)AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, 
							AXIS2_TRANSPORT_URL, AXIS2_FALSE);
	if(NULL != transport_url)
	{
		epr = axis2_endpoint_ref_create(env, transport_url);
	}
	else
	{
		axis2_endpoint_ref_t *ctx_epr = AXIS2_MSG_CTX_GET_TO(msg_ctx, env);
		if(NULL !=  ctx_epr && 0 == AXIS2_STRCMP(
							AXIS2_WSA_ANONYMOUS_URL_SUBMISSION, 
							AXIS2_ENDPOINT_REF_GET_ADDRESS(ctx_epr, env)) &&
							0 == AXIS2_STRCMP(AXIS2_WSA_ANONYMOUS_URL, 
							AXIS2_ENDPOINT_REF_GET_ADDRESS(ctx_epr, env)))
		{
			epr = ctx_epr;
		}
	}
	
	/*if(AXIS2_TRUE == AXIS2_MSG_CTX_IS_DOING_REST(msg_ctx, env))
	 *{
	 *	data_out = AXIS2_SOAP_BODY_GET_FIRST_ELEMENT(AXIS2_SOAP_ENVELOPE_GET_BODY(
							AXIS2_MSG_CTX_GET_ENVELOPE(msg_ctx, env), env),env);
	 *}
	 * else
	 */
	{
		soap_data_out = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
	}
	if(NULL != epr)
	{
		axis2_http_transport_sender_write_message(transport_sender, env, msg_ctx
							, epr, soap_data_out, om_output);
	}
	else
	{
		axis2_stream_t *out_stream = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env,
							AXIS2_TRANSPORT_OUT, AXIS2_FALSE);
		if(AXIS2_TRUE == AXIS2_MSG_CTX_GET_SERVER_SIDE(msg_ctx, env))
		{
            axis2_op_ctx_t *op_ctx = NULL;
            axis2_ctx_t *ctx = NULL;

			axis2_http_out_transport_info_t *out_info = 
							(axis2_http_out_transport_info_t *)
							AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, 
							AXIS2_HTTP_OUT_TRANSPORT_INFO, AXIS2_FALSE);
			axis2_bool_t is_soap11 = AXIS2_FALSE;
			
			if(NULL == out_info)
			{
				AXIS2_ERROR_SET((*env)->error, 
							AXIS2_ERROR_OUT_TRNSPORT_INFO_NULL, AXIS2_FAILURE);
				return AXIS2_FAILURE;
			}
			is_soap11 = AXIS2_MSG_CTX_GET_IS_SOAP_11(msg_ctx, env);
			/* AXIS2_OM_OUTPUT_SET_SOAP11(om_output, env, is_soap_11);
			 */
			AXIS2_HTTP_OUT_TRANSPORT_INFO_SET_CHAR_ENCODING(out_info, env, 
							char_set_enc);
			/*TODO: uncomment this
            AXIS2_HTTP_OUT_TRANSPORT_INFO_SET_CONTENT_TYPE(out_info, env, 
							AXIS2_OM_OUTPUT_GET_CONTENT_TYPE(om_output, env));*/
			/* AXIS2_OM_OUTPUT_SET_DO_OPTIMIZE(om_output, env, 
			 *				AXIS2_MSG_CTX_GET_IS_DOING_MTOM(msg_ctx, env);
			 */
			AXIS2_SOAP_ENVELOPE_SERIALIZE (soap_data_out, env, om_output, 
						AXIS2_FALSE);
			buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);
			AXIS2_STREAM_WRITE(out_stream, env, buffer, AXIS2_STRLEN(buffer));				
			AXIS2_FREE((*env)->allocator, buffer);
	
            op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
            if (op_ctx)
            {
                ctx = AXIS2_OP_CTX_GET_BASE(op_ctx, env);
                if (ctx)
                {
                    AXIS2_CTX_SET_PROPERTY(ctx, env, AXIS2_RESPONSE_WRITTEN,
                                                "TRUE", AXIS2_FALSE);
                }
            }
            
		}
	}
	/*
	 * TODO handle errors
	 */	
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_http_transport_sender_clean_up
							(axis2_transport_sender_t *transport_sender, 
                    		axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    AXIS2_FUNC_PARAM_CHECK(transport_sender, env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
	/*
	 * Clean up is not used. If the http sender needs
	 * to be cleaned up it should be done here.
	 */
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_http_transport_sender_init
							(axis2_transport_sender_t *transport_sender, 
                    		axis2_env_t **env, axis2_conf_ctx_t *conf_ctx, 
							axis2_transport_out_desc_t *out_desc)
{
    axis2_char_t *version = NULL;
	axis2_char_t *temp = NULL;
	
	AXIS2_FUNC_PARAM_CHECK(transport_sender, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, conf_ctx, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, out_desc, AXIS2_FAILURE);
	
	version = (axis2_char_t *)AXIS2_PARAM_CONTAINER_GET_PARAM(
							out_desc->param_container, env, 
							AXIS2_HTTP_PROTOCOL_VERSION);
	if(NULL != version)
	{
		if(0 == AXIS2_STRCMP(version, AXIS2_HTTP_HEADER_PROTOCOL_11))
		{
			axis2_char_t *encoding = NULL;
			if(NULL != AXIS2_INTF_TO_IMPL(transport_sender)->http_version)
			{
				AXIS2_FREE((*env)->allocator, 
							AXIS2_INTF_TO_IMPL(transport_sender)->http_version);
			}
			AXIS2_INTF_TO_IMPL(transport_sender)->http_version = AXIS2_STRDUP(
							version, env);
			encoding = (axis2_char_t *)AXIS2_PARAM_CONTAINER_GET_PARAM(
							out_desc->param_container, env, 
							AXIS2_HTTP_HEADER_TRANSFER_ENCODING);
			if(NULL != encoding && 0 == AXIS2_STRCMP(encoding, 
							AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
			{
				AXIS2_INTF_TO_IMPL(transport_sender)->chunked = AXIS2_TRUE;
			}
		}
		else if(0 == AXIS2_STRCMP(version, AXIS2_HTTP_HEADER_PROTOCOL_10))
		{
			if(NULL != AXIS2_INTF_TO_IMPL(transport_sender)->http_version)
			{
				AXIS2_FREE((*env)->allocator, 
							AXIS2_INTF_TO_IMPL(transport_sender)->http_version);
			}
			AXIS2_INTF_TO_IMPL(transport_sender)->http_version = AXIS2_STRDUP(
							version, env);
			AXIS2_INTF_TO_IMPL(transport_sender)->chunked = AXIS2_FALSE;
		}
		else
		{
			AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NULL_HTTP_VERSION, 
							AXIS2_FAILURE);
			return AXIS2_FAILURE;
			
		}
		
		temp = (axis2_char_t *)AXIS2_PARAM_CONTAINER_GET_PARAM(
							out_desc->param_container, env, 
							AXIS2_HTTP_SO_TIMEOUT);
		if(NULL != temp)
		{
			AXIS2_INTF_TO_IMPL(transport_sender)->so_timeout = atoi(temp);
		}
		temp = (axis2_char_t *)AXIS2_PARAM_CONTAINER_GET_PARAM(
							out_desc->param_container, env, 
							AXIS2_HTTP_CONNECTION_TIMEOUT);
		if(NULL != temp)
		{
			AXIS2_INTF_TO_IMPL(transport_sender)->connection_timeout=atoi(temp);
		}
	}
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_transport_sender_write_message
							(axis2_transport_sender_t *transport_sender, 
                    		axis2_env_t **env, axis2_msg_ctx_t *msg_ctx,
							axis2_endpoint_ref_t *epr, 
							axis2_soap_envelope_t *out, 
							axis2_om_output_t *om_output)
{
	axis2_char_t *soap_action = NULL;
	axis2_char_t *url = NULL;
	
	AXIS2_FUNC_PARAM_CHECK(transport_sender, env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, epr, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, om_output, AXIS2_FAILURE);

	url = AXIS2_ENDPOINT_REF_GET_ADDRESS(epr, env);
	soap_action = AXIS2_MSG_CTX_GET_SOAP_ACTION(msg_ctx, env);
	if(NULL == soap_action || 0 == AXIS2_STRLEN(soap_action))
	{
		soap_action = AXIS2_MSG_CTX_GET_WSA_ACTION(msg_ctx, env);
	}
	if(NULL == soap_action)
	{
		soap_action = "";
	}
	/*
	 * TODO : when REST comes
	 * if(AXIS2_TRUE == AXIS2_MSG_CTX_IS_DOING_REST(msg_ctx, env)
	 * {
	 * 		axis2_rest_sender_t *sender = axis2_rest_sender_create(env)
	 *		AXIS2_REST_SENDER_SET_FORMAT(sender, env,
	 *					AXIS2_INTF_TO_IMPL(transport_sender)->format);
	 *		AXIS2_REST_SENDER_SEND(sender, env, msg_ctx, data_out, url, 
	 *					soap_action);
	 *	}
	 */
	axis2_soap_over_http_sender_t *sender = axis2_soap_over_http_sender_create
							(env);
	AXIS2_SOAP_OVER_HTTP_SENDER_SET_OM_OUTPUT(sender, env, om_output);
	AXIS2_SOAP_OVER_HTTP_SENDER_SEND(sender, env, msg_ctx, out, url,
						soap_action);
	/*
	 * TODO check for errors
	 */
	return AXIS2_SUCCESS;
}

/**
 * Following block distinguish the exposed part of the dll.
 */
int axis2_get_instance(struct axis2_transport_sender **inst,
                        axis2_env_t **env)
{
    *inst = axis2_http_transport_sender_create(env);
    if(!(*inst))
    {
        printf("transport sender load not success\n");
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

int axis2_remove_instance(axis2_transport_sender_t *inst,
                            axis2_env_t **env)
{
    axis2_status_t status = AXIS2_FAILURE;
    if (inst)
    {
        status = AXIS2_TRANSPORT_SENDER_FREE(inst, env);
    }
    return status;
}
