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
 
#include <axis2_http_transport_utils.h>
#include <string.h>
#include <ctype.h>
#include <axis2_conf.h>
#include <axis2_op.h>
#include <axis2_qname.h>
#include <axis2_http_transport.h>
#include <axis2_soap_builder.h>
#include <axis2_engine.h>
#include <axis2_soap_body.h>
#include <axis2_utils.h>
#include <axis2_om_namespace.h>
#include <axis2_om_node.h>
#include <axis2_hash.h>
#include <axis2_soap_const.h>
#include <axis2_http_header.h>
#include <axis2_property.h>

#define AXIS2_MIME_BOUNDARY_BYTE 45

/***************************** Function headers *******************************/

AXIS2_DECLARE(axis2_status_t)
axis2_http_transport_utils_process_http_post_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
                        axis2_stream_t *in_stream, axis2_stream_t *out_stream,
						axis2_char_t *content_type, int content_length,
                        axis2_char_t *soap_action_header,
                        axis2_char_t *request_uri);
    
AXIS2_DECLARE(axis2_bool_t) 
axis2_http_transport_utils_process_http_get_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx,
                        axis2_stream_t *in_stream, axis2_stream_t *out_stream,
						axis2_char_t *content_type,
                        axis2_char_t *soap_action_header, 
                        axis2_char_t *request_uri, axis2_conf_ctx_t *conf_ctx, 
                        axis2_hash_t *request_params);
    
AXIS2_DECLARE(axis2_soap_envelope_t*)
axis2_http_transport_utils_create_envelope_from_get_request
                        (axis2_env_t **env, axis2_char_t *request_uri,
                        axis2_hash_t *request_params);
    
AXIS2_DECLARE(axis2_om_stax_builder_t* )
axis2_http_transport_utils_select_builder_for_mime
                        (axis2_env_t **env, axis2_char_t *request_uri,
                        axis2_msg_ctx_t *msg_ctx, axis2_stream_t *in_stream,
                        axis2_char_t *content_type);
    
AXIS2_DECLARE(axis2_bool_t ) 
axis2_http_transport_utils_is_optimized(axis2_env_t **env, 
                                        axis2_om_element_t *om_element);
                                                
AXIS2_DECLARE(axis2_bool_t ) 
axis2_http_transport_utils_do_write_mtom(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx);
                                                
AXIS2_DECLARE(axis2_bool_t ) 
axis2_http_transport_utils_is_doing_rest(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx);
                                                
AXIS2_DECLARE(axis2_bool_t) 
axis2_http_transport_utils_is_doing_rest_through_post
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
						
AXIS2_DECLARE(axis2_status_t)
axis2_http_transport_utils_strdecode(axis2_env_t **env, axis2_char_t *dest, 
						axis2_char_t *src);
						
AXIS2_DECLARE(int)
axis2_http_transport_utils_hexit(axis2_char_t c);

AXIS2_DECLARE(axis2_char_t* )
axis2_http_transport_utils_get_services_html(axis2_env_t **env, 
							axis2_conf_ctx_t *conf_ctx);
AXIS2_DECLARE(axis2_char_t*)
axis2_http_transport_utils_get_charset_enc(axis2_env_t **env, 
						axis2_char_t *content_type);
int
axis2_http_transport_utils_on_data_request(char *buffer, int size, void *ctx);

AXIS2_DECLARE(axis2_soap_envelope_t*)
axis2_http_transport_utils_create_soap_msg(axis2_env_t **env, 
                        axis2_msg_ctx_t *msg_ctx, axis2_char_t *soap_ns_uri);
AXIS2_DECLARE(axis2_char_t*)
axis2_http_transport_utils_get_value_from_content_type(axis2_env_t **env, 
						axis2_char_t *content_type, axis2_char_t *key);
/***************************** End of function headers ************************/

AXIS2_DECLARE(axis2_status_t ) 
axis2_http_transport_utils_process_http_post_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
                        axis2_stream_t *in_stream, axis2_stream_t *out_stream,
						axis2_char_t *content_type, int content_length, 
                        axis2_char_t *soap_action_header,
                        axis2_char_t *request_uri)
{
	axis2_soap_envelope_t *soap_envelope = NULL;
	axis2_soap_builder_t *soap_builder = NULL;
	axis2_om_stax_builder_t *om_builder = NULL;
	axis2_bool_t is_soap11 = AXIS2_FALSE;
	axis2_xml_reader_t *xml_reader = NULL;
	axis2_char_t *char_set = NULL;
	/*axis2_char_t *xml_char_set = NULL;*/
	axis2_conf_ctx_t *conf_ctx = NULL;
	axis2_callback_info_t callback_ctx;
	axis2_hash_t *headers = NULL;
	axis2_engine_t *engine = NULL;
	axis2_soap_body_t *soap_body = NULL;
    axis2_property_t *property = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_hash_t *binary_data_map = NULL;
	
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, in_stream, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, out_stream, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, content_type, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, request_uri, AXIS2_FAILURE);
	
	conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
	
	callback_ctx.in_stream = in_stream;
	callback_ctx.env = *env;
	callback_ctx.content_length = content_length;
	callback_ctx.unread_len = content_length;
	callback_ctx.chunked_stream = NULL;
	
	if(NULL != soap_action_header && (strlen(soap_action_header) > 0) )	
	{
		/* remove leading and trailing " s */
		if('"' == soap_action_header[0])
		{
			memmove(soap_action_header, soap_action_header+sizeof(axis2_char_t),
						strlen(soap_action_header) + sizeof(axis2_char_t));
		}
		if('"' == soap_action_header[strlen(soap_action_header) -1])
		{
			soap_action_header[strlen(soap_action_header) -1] = '\0';
		}
	}
	property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, AXIS2_TRANSPORT_HEADERS, 
						AXIS2_FALSE);
    if(property)
    {
        headers = AXIS2_PROPERTY_GET_VALUE(property, env);
        property = NULL;
    }
	if(NULL != headers)
	{
		axis2_http_header_t *encoding_header = NULL;
		encoding_header = (axis2_http_header_t*)axis2_hash_get(headers, 
						AXIS2_HTTP_HEADER_TRANSFER_ENCODING,
						AXIS2_HASH_KEY_STRING);
		if(NULL != encoding_header)
		{
			axis2_char_t *encoding_value = NULL;
			encoding_value = AXIS2_HTTP_HEADER_GET_VALUE(encoding_header, env);
			if(NULL != encoding_value && 0 == AXIS2_STRCASECMP(encoding_value, 
						AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
			{
				callback_ctx.chunked_stream = axis2_http_chunked_stream_create(
							env, in_stream);
				if(NULL == callback_ctx.chunked_stream)
				{
					AXIS2_LOG_ERROR((*env)->log, AXIS2_LOG_SI, "Error occured in"
							" creating in chunked stream.");
					return AXIS2_FAILURE;				
				}
				AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "HTTP"
						" stream chunked");
			}
		}
	}
    
    if (NULL != strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_MULTIPART_RELATED))
    {
        /* get mime boundry */
        axis2_char_t *mime_boundary = 
            axis2_http_transport_utils_get_value_from_content_type(env, 
                content_type, AXIS2_HTTP_HEADER_CONTENT_TYPE_MIME_BOUNDARY);

        if (mime_boundary)
        {
            axis2_char_t *buffer = NULL;
            int size = 1024;
            int len = 0;
            axis2_char_t *root_mime = NULL;
            int root_mime_len = 0;
            axis2_char_t *soap_body_str = NULL;
            int soap_body_len = 0;
            axis2_char_t *body_mime = NULL;
            int body_mime_len = 0;
            axis2_char_t *mime_binary = NULL;
            int mime_binary_len = 0;
            axis2_char_t *pos = NULL;
            
            buffer = AXIS2_MALLOC((*env)->allocator, sizeof(axis2_char_t) * (size + 1));
           
            do 
            {    
                len = axis2_http_transport_utils_on_data_request(buffer, size, (void*)&callback_ctx);
                if (len > 0)
                {
                    axis2_char_t *temp_root_mime = root_mime;
                    root_mime = AXIS2_MALLOC((*env)->allocator, 
                        sizeof(char) * (root_mime_len + len + 1));
                    if (root_mime)
                    {
                        if (temp_root_mime)
                        {
                            memcpy(root_mime, temp_root_mime, root_mime_len);
                            AXIS2_FREE((*env)->allocator, temp_root_mime);
                            temp_root_mime = NULL;
                        }
                        
                        memcpy(root_mime + root_mime_len, buffer, len);
                        root_mime_len += len;
                        root_mime[root_mime_len] = '\0';

                        pos = AXIS2_STRSTR(root_mime, "\r\n\r\n");
                        if (pos)
                        {
                            if (root_mime_len > (pos - root_mime + 4))
                            {
                                soap_body_len = root_mime_len - (pos - root_mime + 4);
                                soap_body_str = AXIS2_MALLOC((*env)->allocator,
                                                sizeof(char) * (soap_body_len + 1));
                                memcpy(soap_body_str, pos + 4, soap_body_len);
                                soap_body_str[soap_body_len] = '\0';
                                *pos = '\0';
                            }
                        }
                    }
                }
                
            } while(!pos && len > 0);

            pos = NULL;
            len = 0;

            do 
            {
                if (soap_body_str)
                {
                    pos = AXIS2_STRSTR(soap_body_str, mime_boundary);
                }
                
                if (pos)
                {
                    pos -= 2;
                    body_mime_len = soap_body_len - (pos - soap_body_str);
                    body_mime = AXIS2_MALLOC((*env)->allocator,
                                    sizeof(char) * (body_mime_len + 1));
                    memcpy(body_mime, pos, body_mime_len);
                    body_mime[body_mime_len] = '\0';
                    
                    *(pos)  = '\0';
                    soap_body_len = (pos - soap_body_str);
                }
                else
                {
                    len = axis2_http_transport_utils_on_data_request(buffer, size, (void*)&callback_ctx);
                    if (len > 0)
                    {
                        axis2_char_t *temp_soap_body = soap_body_str;
                        soap_body_str = AXIS2_MALLOC((*env)->allocator, 
                            sizeof(char) * (soap_body_len + len + 1));
                        if (soap_body_str)
                        {
                            if (temp_soap_body)
                            {
                                memcpy(soap_body_str, temp_soap_body, soap_body_len);
                                AXIS2_FREE((*env)->allocator, temp_soap_body);
                                temp_soap_body = NULL;
                            }
                            
                            memcpy(soap_body_str + soap_body_len, buffer, len);
                            soap_body_len += len;
                            soap_body_str[soap_body_len] = '\0';
                        }
                     }
                 }
            } while(!pos && len > 0);
            
            pos = NULL;
            len = 0;

            do 
            {
                if (body_mime)
                {
                    pos = AXIS2_STRSTR(body_mime, "\r\n\r\n");
                }
                
                if (pos)
                {
                    if (body_mime_len > (pos - body_mime + 4))
                    {
                        mime_binary_len = body_mime_len - (pos - body_mime + 4);
                        mime_binary = AXIS2_MALLOC((*env)->allocator,
                                        sizeof(char) * (mime_binary_len + 1));
                        memcpy(mime_binary, pos + 4, mime_binary_len);
                        mime_binary[mime_binary_len] = '\0';
                        *pos = '\0';
                    }
                }
                else
                {
                    len = axis2_http_transport_utils_on_data_request(buffer, size, (void*)&callback_ctx);
                    if (len > 0)
                    {
                        axis2_char_t *temp_body_mime = body_mime;
                        body_mime = AXIS2_MALLOC((*env)->allocator, 
                            sizeof(char) * (body_mime_len + len + 1));
                        if (body_mime)
                        {
                            if (temp_body_mime)
                            {
                                memcpy(body_mime, temp_body_mime, body_mime_len);
                                AXIS2_FREE((*env)->allocator, temp_body_mime);
                                temp_body_mime = NULL;
                            }
                            
                            memcpy(body_mime + body_mime_len, buffer, len);
                            body_mime_len += len;
                            body_mime[body_mime_len] = '\0';
                        }
                     }
                }
            } while(!pos && len > 0);


            /* TODO: need to address the ultiple attachment case */
            pos = NULL;
            len = 0;
            do 
            {
                axis2_char_t *old_pos = NULL;
                if (mime_binary)
                {
                    old_pos = mime_binary;
                    axis2_char_t *temp_pos = NULL;
                    do
                    {
                        
                        pos = memchr(old_pos, AXIS2_MIME_BOUNDARY_BYTE, 
                            (mime_binary_len - (old_pos - mime_binary)));
                        if (!pos)
                            break;
                        else
                        {
                            old_pos = pos + 1;
                            if (old_pos - mime_binary > mime_binary_len)
                            {
                                pos = NULL;
                                break;
                            }
                            temp_pos = AXIS2_STRSTR(pos + 1, mime_boundary);
                        }
                    } while ( *(pos + 1) != AXIS2_MIME_BOUNDARY_BYTE || temp_pos != pos + 2);

                    if ( pos && *(pos + 1) == AXIS2_MIME_BOUNDARY_BYTE && temp_pos == pos + 2)
                    {
                        mime_binary_len = pos - mime_binary;
                    }
                    else
                        pos = NULL;
                }
                
                if (!pos)
                {
                    len = axis2_http_transport_utils_on_data_request(buffer, size, (void*)&callback_ctx);
                    if (len > 0)
                    {
                        axis2_char_t *temp_mime_binary = mime_binary;
                        mime_binary = AXIS2_MALLOC((*env)->allocator, 
                            sizeof(char) * (mime_binary_len + len + 1));
                        if (mime_binary)
                        {
                            if (temp_mime_binary)
                            {
                                memcpy(mime_binary, temp_mime_binary, mime_binary_len);
                                AXIS2_FREE((*env)->allocator, temp_mime_binary);
                                temp_mime_binary = NULL;
                            }
                            
                            memcpy(mime_binary + mime_binary_len, buffer, len);
                            mime_binary_len += len;
                            mime_binary[mime_binary_len] = '\0';
                        }
                     }
                }
            
            } while(!pos && len > 0);

            if (soap_body_str) 
            {
                /* create a basic stream with soap string to pull SOAP */
                axis2_stream_t *stream = axis2_stream_create_basic(env);
                if (stream)
                {
                    AXIS2_STREAM_WRITE(stream, env, soap_body_str, soap_body_len);
                    callback_ctx.in_stream = stream;
                    callback_ctx.chunked_stream = NULL;
                    callback_ctx.content_length = soap_body_len;
                    callback_ctx.unread_len = soap_body_len;
                }
                
                /* data handler hash map */
                binary_data_map = axis2_hash_make(env);
                if (binary_data_map)
                {
                    /* get MIME ID */
                    axis2_char_t *id = NULL;
                    id = AXIS2_STRSTR(body_mime, "content-id");
                    if (id)
                    {
                        id += AXIS2_STRLEN("content-id");
                        while (id && *id && *id != ':')
                            id++;
                        if (id)
                        {
                            while (id && *id && *id != '<')
                                id++;
                            id++;
                            if (id)
                            {
                                axis2_char_t *pos = NULL;
                                pos = AXIS2_STRSTR(id, ">");
                                if (pos)
                                {
                                    axis2_char_t *mime_id = NULL;
                                    int mime_id_len = 0;
                                    mime_id_len = pos - id;
                                    mime_id = AXIS2_MALLOC((*env)->allocator, 
                                        sizeof(axis2_char_t) * mime_id_len + 1);
                                    if (mime_id)
                                    {
                                        axis2_data_handler_t *data_handler = NULL;
                                        memcpy(mime_id, id, mime_id_len);
                                        mime_id[mime_id_len] = '\0';
                                        data_handler = axis2_data_handler_create(env, NULL, NULL);
                                        AXIS2_DATA_HANDLER_SET_BINARY_DATA(data_handler, env,
                                            mime_binary, mime_binary_len);                                        
                                        axis2_hash_set(binary_data_map, mime_id,
                                            AXIS2_HASH_KEY_STRING, data_handler);                                        
                                    }
                                }
                            }
                        }
                    }
                }
                
                
            }
        }
    }
    

    /*TODO for MTOM:create a basic stream
    set callback_ctx.in_stream to this basic stream
    make callback_ctx.chunked_stream null but keep the referance */

	AXIS2_MSG_CTX_SET_WSA_ACTION(msg_ctx, env, soap_action_header);
	AXIS2_MSG_CTX_SET_SOAP_ACTION(msg_ctx, env, soap_action_header);
	AXIS2_MSG_CTX_SET_TO(msg_ctx, env, axis2_endpoint_ref_create(env, 
					request_uri));
    
	AXIS2_MSG_CTX_SET_SERVER_SIDE(msg_ctx, env, AXIS2_TRUE);
	
	char_set = axis2_http_transport_utils_get_charset_enc(env,content_type);
	xml_reader = axis2_xml_reader_create_for_memory(env, 
						axis2_http_transport_utils_on_data_request,NULL, 
						(void *)&callback_ctx, char_set);

	if(NULL == xml_reader)
	{
		return AXIS2_FAILURE;
	}

    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
    AXIS2_PROPERTY_SET_VALUE(property, env, char_set);
	AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, AXIS2_CHARACTER_SET_ENCODING,
					property, AXIS2_TRUE);

	om_builder = axis2_om_stax_builder_create(env, xml_reader);
	if(NULL == om_builder)
	{
		AXIS2_XML_READER_FREE(xml_reader, env);
		xml_reader = NULL;
		return AXIS2_FAILURE;
	}

	if(NULL != strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP))
	{
		is_soap11 = AXIS2_FALSE;
		soap_builder = axis2_soap_builder_create(env, om_builder, 
                    AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI);
		if(NULL == soap_builder)
		{
			/* We should not be freeing om_builder here as it is done by
            axis2_soap_builder_create in case of error - Samisa*/
            /*AXIS2_OM_STAX_BUILDER_FREE(om_builder, env);*/
			om_builder = NULL;
			xml_reader = NULL;
			return AXIS2_FAILURE;
		}

		soap_envelope = AXIS2_SOAP_BUILDER_GET_SOAP_ENVELOPE(soap_builder,
					env);
		if(NULL == soap_envelope)
		{
			AXIS2_OM_STAX_BUILDER_FREE(om_builder, env);
			om_builder = NULL;
			xml_reader = NULL;
			AXIS2_SOAP_BUILDER_FREE(soap_builder, env);
			soap_builder = NULL;
			return AXIS2_FAILURE;
		}
	}
	else if(NULL != strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML))
	{
		is_soap11 = AXIS2_TRUE;
		if(NULL != soap_action_header)
		{
			soap_builder = axis2_soap_builder_create(env, om_builder, 
						AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI);
			if(NULL == soap_builder)
			{
			    /* We should not be freeing om_builder here as it is done by
                axis2_soap_builder_create in case of error - Samisa*/
				/*AXIS2_OM_STAX_BUILDER_FREE(om_builder, env);*/
				om_builder = NULL;
				xml_reader = NULL;
				return AXIS2_FAILURE;
			}
			soap_envelope = AXIS2_SOAP_BUILDER_GET_SOAP_ENVELOPE(
					soap_builder, env);
			if(NULL == soap_envelope)
			{
				AXIS2_SOAP_BUILDER_FREE(soap_builder, env);
				om_builder = NULL;
				xml_reader = NULL;
				soap_builder = NULL;
				return AXIS2_FAILURE;
			}
		}
        else
        {
            /* REST support */
            axis2_param_t *rest_param = AXIS2_MSG_CTX_GET_PARAMETER(msg_ctx, env
                        , AXIS2_ENABLE_REST);
            if(NULL != rest_param && 0 == AXIS2_STRCMP(AXIS2_VALUE_TRUE, 
                        AXIS2_PARAM_GET_VALUE(rest_param, env)))
            {
                /* TODO we have to check for NULLs */
                axis2_soap_body_t *def_body = NULL;
                axis2_om_document_t *om_doc = NULL;
                axis2_om_node_t *root_node = NULL;
                soap_envelope = axis2_soap_envelope_create_default_soap_envelope
                            (env, AXIS2_SOAP11);
                def_body = AXIS2_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
                om_doc = AXIS2_OM_STAX_BUILDER_GET_DOCUMENT(om_builder, env);
                root_node = AXIS2_OM_DOCUMENT_BUILD_ALL(om_doc, env);
                AXIS2_SOAP_BODY_ADD_CHILD(def_body, env, root_node);
                AXIS2_MSG_CTX_SET_DOING_REST(msg_ctx, env, AXIS2_TRUE);
            }
        }
		 		
	}
    
    if (binary_data_map)
    {
        AXIS2_SOAP_BUILDER_SET_MIME_BODY_PARTS(soap_builder, env, 
            binary_data_map);
    }
        
	/* xml_char_set = AXIS2_OM_DOCUMENT_GET_CHARSET_ENC(
	 *					AXIS2_OM_STAX_BUILDER_GET_DOCUMENT(env om_builder),
	 *					env);
	 *
	 *if(0 != AXIS2_STRCMP(char_set, xml_char_set))
	 *{
	 *	AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CHARSET_MISMATCH, 
	 *					AXIS2_FAILURE);
	 *	AXIS2_SOAP_ENVELOPE_FREE(envelope, env);
	 *	envelope = NULL;
	 *	AXIS2_XML_READER_FREE(xml_reader, env);
	 *	xml_reader = NULL;
	 *	AXIS2_OM_STAX_BUILDER_FREE(om_builder, env);
	 *	om_builder = NULL;
	 *	if(NULL != soap_builder)
	 *	{
	 *	 	AXIS2_SOAP_BUILDER_FREE(soap_builder, env);
	 *	 	soap_builder = NULL;
	 *	}
	 *	return AXIS2_FAILURE;
	 *}
	 */
	
	AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, soap_envelope);
	
	engine = axis2_engine_create(env, conf_ctx);
    
    if (!soap_envelope)
        return AXIS2_FAILURE;
    
    soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(soap_envelope, 
						env);
    
    if (NULL == soap_body)
        return AXIS2_FAILURE;
    
	if(AXIS2_TRUE == AXIS2_SOAP_BODY_HAS_FAULT(soap_body, env))
	{
		status = AXIS2_ENGINE_RECEIVE_FAULT(engine, env, msg_ctx);
	}
	else
	{
		status = AXIS2_ENGINE_RECEIVE(engine, env, msg_ctx);
	}
	if(NULL == AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env) && 
						AXIS2_FALSE == is_soap11)
	{
		axis2_soap_envelope_t *def_envelope = 
            axis2_soap_envelope_create_default_soap_envelope(env, AXIS2_SOAP12);
		AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, def_envelope);
	}
    if(NULL != engine)
    {
        AXIS2_ENGINE_FREE(engine, env);
    }
	return status;
}


AXIS2_DECLARE(axis2_bool_t) 
axis2_http_transport_utils_process_http_get_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx,
                        axis2_stream_t *in_stream, axis2_stream_t *out_stream,
						axis2_char_t *content_type,
                        axis2_char_t *soap_action_header, 
                        axis2_char_t *request_uri, axis2_conf_ctx_t *conf_ctx, 
                        axis2_hash_t *request_params)
{
	axis2_soap_envelope_t *soap_envelope = NULL;
	axis2_engine_t *engine = NULL;
    axis2_property_t *property = NULL;
		
	AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FALSE);
	AXIS2_PARAM_CHECK((*env)->error, in_stream, AXIS2_FALSE);
	AXIS2_PARAM_CHECK((*env)->error, out_stream, AXIS2_FALSE);
	AXIS2_PARAM_CHECK((*env)->error, content_type, AXIS2_FALSE);
	AXIS2_PARAM_CHECK((*env)->error, request_uri, AXIS2_FALSE);
	AXIS2_PARAM_CHECK((*env)->error, request_params, AXIS2_FALSE);
	
	if(NULL != soap_action_header)	
	{
		/* remove leading and trailing " s */
		if('"' == soap_action_header[0])
		{
			memmove(soap_action_header, soap_action_header+sizeof(axis2_char_t),
						strlen(soap_action_header) + sizeof(axis2_char_t));
		}
		if('"' == soap_action_header[strlen(soap_action_header) -1])
		{
			soap_action_header[strlen(soap_action_header) -1] = '\0';
		}
	}
	AXIS2_MSG_CTX_SET_WSA_ACTION(msg_ctx, env, soap_action_header);
	AXIS2_MSG_CTX_SET_SOAP_ACTION(msg_ctx, env, soap_action_header);
	AXIS2_MSG_CTX_SET_TO(msg_ctx, env, axis2_endpoint_ref_create(env, 
					request_uri));
    
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_stream_free_void_arg);
    AXIS2_PROPERTY_SET_VALUE(property, env, out_stream);
	AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, 
					AXIS2_TRANSPORT_OUT, property, AXIS2_FALSE);

	AXIS2_MSG_CTX_SET_SERVER_SIDE(msg_ctx, env, AXIS2_TRUE);
	
	soap_envelope = axis2_http_transport_utils_create_envelope_from_get_request(
					env, request_uri, request_params);
	if(NULL == soap_envelope)
	{
		return AXIS2_FALSE;
	}
	else
	{
		AXIS2_MSG_CTX_SET_DOING_REST(msg_ctx, env, AXIS2_TRUE);
		AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, soap_envelope);
		engine = axis2_engine_create(env, conf_ctx);
		AXIS2_ENGINE_RECEIVE(engine, env, msg_ctx);
		return AXIS2_TRUE;
	}
    return AXIS2_FALSE;
}


AXIS2_DECLARE(axis2_soap_envelope_t*)
axis2_http_transport_utils_create_envelope_from_get_request
                        (axis2_env_t **env, axis2_char_t *request_uri,
                        axis2_hash_t *request_params)
{
	axis2_char_t **values = NULL;
	axis2_soap_envelope_t *envelope = NULL;
	axis2_om_namespace_t *om_ns = NULL;
	axis2_om_namespace_t *def_om_ns = NULL;
	axis2_om_node_t *document_node = NULL;
	axis2_om_node_t *op_node = NULL;
	axis2_om_element_t *op_ele = NULL;
	axis2_hash_index_t *hi = NULL;
	AXIS2_PARAM_CHECK((*env)->error, request_uri, NULL);
	AXIS2_PARAM_CHECK((*env)->error, request_params, NULL);
	
	values = axis2_parse_request_url_for_svc_and_op(env, request_uri);
	if(NULL == values)
	{
		return NULL;
	}
	if(NULL == values[0] && NULL != values[1])
	{
		AXIS2_FREE((*env)->allocator, values[1]);
		AXIS2_FREE((*env)->allocator, values);
		return NULL;
	}
	if(NULL != values[0] && NULL == values[1])
	{
		AXIS2_FREE((*env)->allocator, values[0]);
		AXIS2_FREE((*env)->allocator, values);
		return NULL;
	}
	if(NULL == values[0] && NULL == values[1])
	{
		AXIS2_FREE((*env)->allocator, values);
		return NULL;
	}
	envelope = axis2_soap_envelope_create_default_soap_envelope(env, AXIS2_SOAP12);
    
	om_ns = axis2_om_namespace_create(env, values[0], "services");
	
    def_om_ns = axis2_om_namespace_create(env, "", NULL);
	
    document_node = AXIS2_SOAP_BODY_GET_BASE_NODE(AXIS2_SOAP_ENVELOPE_GET_BODY(
						envelope, env), env);
	op_ele = axis2_om_element_create(env, document_node, values[1], om_ns, 
						&op_node);
	
	for(hi = axis2_hash_first(request_params, env); NULL != hi; 
						axis2_hash_next(env, hi))
	{
		void *name = NULL;
		void *value = NULL;
		axis2_om_element_t *tmp_ele = NULL;
		axis2_om_node_t *tmp_node = NULL;
		axis2_hash_this(hi, (const void **)&name, NULL, (void**)&value);
		tmp_ele = axis2_om_element_create(env, op_node, (axis2_char_t*)name,
						def_om_ns, &tmp_node);
		AXIS2_OM_ELEMENT_SET_TEXT(tmp_ele, env, (axis2_char_t*)value, tmp_node);
	}
    return envelope;
}


AXIS2_DECLARE(axis2_om_stax_builder_t* )
axis2_http_transport_utils_select_builder_for_mime
                        (axis2_env_t **env, axis2_char_t *request_uri,
                        axis2_msg_ctx_t *msg_ctx, axis2_stream_t *in_stream,
                        axis2_char_t *content_type)
{
	/*
        TODO implement when MTOM support is added
    */
    return NULL;
}


AXIS2_DECLARE(axis2_bool_t ) 
axis2_http_transport_utils_is_optimized(axis2_env_t **env, 
                                        axis2_om_element_t *om_element)
{
	/*
        TODO implement when MTOM support is added
    */
    return AXIS2_FALSE;
}


AXIS2_DECLARE(axis2_bool_t) 
axis2_http_transport_utils_do_write_mtom(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx)
{
    axis2_property_t *property = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);    
    property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env,
        AXIS2_ENABLE_MTOM, AXIS2_FALSE);
    if(property)
    {
        axis2_char_t *value = (axis2_char_t *)AXIS2_PROPERTY_GET_VALUE(property, env);

        if (value)
        {
            return (AXIS2_STRCMP(value, AXIS2_VALUE_TRUE) == 0);
        }
    }

    return AXIS2_FALSE;
}


AXIS2_DECLARE(axis2_bool_t) 
axis2_http_transport_utils_is_doing_rest(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx)
{
    /*
        TODO implement when REST support is added
    */
    return AXIS2_FALSE;
}


AXIS2_DECLARE(axis2_bool_t) 
axis2_http_transport_utils_is_doing_rest_through_post
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    /*
        TODO implement when REST support is added
    */
    return AXIS2_FALSE;
}

AXIS2_DECLARE(axis2_hash_t *)
axis2_http_transport_utils_get_request_params(axis2_env_t **env, 
						axis2_char_t *request_uri)
{
	
	
	axis2_char_t *query_str = NULL;
	axis2_char_t *tmp = strchr(request_uri, '?');
	axis2_char_t *tmp2 = NULL;
	axis2_char_t *tmp_name = NULL;
	axis2_char_t *tmp_value = NULL;
	axis2_hash_t *ret = NULL;
	
	AXIS2_PARAM_CHECK((*env)->error, request_uri, AXIS2_FAILURE);
	
	if(NULL == tmp || '\0' == *(tmp +1))
	{
		return NULL;
	}
	query_str = AXIS2_STRDUP(tmp+1, env);
		
	for(tmp2 = tmp = query_str; *tmp != '\0'; ++tmp)
	{
		if('=' == *tmp)
		{
			*tmp = '\0';
			tmp_name = AXIS2_STRDUP(tmp2, env);
			axis2_http_transport_utils_strdecode(env, tmp_name, tmp_name);
			tmp2 = tmp +1;			
		}
		if('&' == *tmp)
		{
			*tmp = '\0';
			tmp_value = AXIS2_STRDUP(tmp2, env);
			axis2_http_transport_utils_strdecode(env, tmp_value, tmp_value);
			tmp2 = tmp +1;
		}
		if(NULL != tmp_name && NULL != tmp_value)
		{
			if(NULL == ret)
			{
				ret = axis2_hash_make(env);
			}
			axis2_hash_set(ret, tmp_name, AXIS2_HASH_KEY_STRING, tmp_value);
			tmp_name = NULL;
			tmp_value = NULL;
		}
	}
	if(NULL != tmp_name && '\0' != *tmp2)
	{
		if(NULL == ret)
		{
			ret = axis2_hash_make(env);
		}
		tmp_value = AXIS2_STRDUP(tmp2, env);
		axis2_http_transport_utils_strdecode(env, tmp_value, tmp_value);
		axis2_hash_set(ret, tmp_name, AXIS2_HASH_KEY_STRING, tmp_value);
	}
	
	return ret;
}


AXIS2_DECLARE(axis2_status_t)
axis2_http_transport_utils_strdecode(axis2_env_t **env, axis2_char_t *dest, 
						axis2_char_t *src)
{
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, dest, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, src, AXIS2_FAILURE);

    for ( ; *src != '\0'; ++dest, ++src )
    {
		if ( src[0] == '%' && isxdigit( src[1] ) && isxdigit( src[2] ) )
		{
			*dest = axis2_http_transport_utils_hexit( src[1] ) * 16 + 
							axis2_http_transport_utils_hexit( src[2] );
			src += 2;
		}
		else
		{
			*dest = *src;
		}
    }
    *dest = '\0';
	
	return AXIS2_SUCCESS;
}


AXIS2_DECLARE(int )
axis2_http_transport_utils_hexit(axis2_char_t c)
{
	if ( c >= '0' && c <= '9' )
	{
		return c - '0';
	}
	if ( c >= 'a' && c <= 'f' )
	{
		return c - 'a' + 10;
	}
	if ( c >= 'A' && c <= 'F' )
	{
		return c - 'A' + 10;
	}
	return 0;           /* shouldn't happen, we're guarded by isxdigit() */
}

AXIS2_DECLARE(axis2_char_t*)
axis2_http_transport_utils_get_services_html(axis2_env_t **env, 
							axis2_conf_ctx_t *conf_ctx)
{
	axis2_hash_t *services_map = NULL;
	axis2_hash_t *errorneous_svc_map = NULL;
	axis2_char_t *ret = NULL;
	axis2_char_t *tmp2 = "<h2>Deployed Services</h2>";
	axis2_hash_index_t *hi = NULL;
	axis2_bool_t svcs_exists = AXIS2_FALSE;
	
	AXIS2_ENV_CHECK(env, NULL);
	AXIS2_PARAM_CHECK((*env)->error, conf_ctx, NULL);
	
	services_map = AXIS2_CONF_GET_SVCS(AXIS2_CONF_CTX_GET_CONF(conf_ctx,env),
							env);
	errorneous_svc_map = AXIS2_CONF_GET_FAULTY_SVCS(AXIS2_CONF_CTX_GET_CONF(
							conf_ctx,env), env);
	if(NULL != services_map && 0 != axis2_hash_count(services_map))
	{
		void *service = NULL;
		axis2_char_t *sname = NULL;
		axis2_hash_t *ops = NULL;
		svcs_exists = AXIS2_TRUE;
		
		for (hi = axis2_hash_first (services_map, env);
             				NULL != hi; hi = axis2_hash_next (env, hi))
		{
			axis2_hash_this(hi, NULL, NULL, &service);
			sname= AXIS2_QNAME_GET_LOCALPART(AXIS2_SVC_GET_QNAME(
							((axis2_svc_t *)service), env), env);
			ret = AXIS2_STRACAT(tmp2, "<h3>", env);
            tmp2 = ret;
			ret = AXIS2_STRACAT(tmp2, sname, env);
			AXIS2_FREE((*env)->allocator, tmp2);
            tmp2 = ret;
			ret  = AXIS2_STRACAT(tmp2, "</h3>", env);
			AXIS2_FREE((*env)->allocator, tmp2);
            tmp2 = ret;
			ops = AXIS2_SVC_GET_OPS(((axis2_svc_t *)service), env);
			if(NULL != ops && 0 != axis2_hash_count(ops))
			{
				axis2_hash_index_t *hi2 = NULL;
				void *op = NULL;
				axis2_char_t *oname = NULL;
				
				ret = AXIS2_STRACAT(tmp2, "<i>Available Operations</i> <ul>", 
						env);
				AXIS2_FREE((*env)->allocator, tmp2);
                tmp2 = ret;
				for(hi2 = axis2_hash_first(ops, env); NULL != hi2;
							hi2 = axis2_hash_next(env, hi2))
				{
					axis2_hash_this(hi2, NULL, NULL, &op);
					oname = AXIS2_QNAME_GET_LOCALPART(AXIS2_OP_GET_QNAME(
							((axis2_op_t *)op), env), env);
					ret = AXIS2_STRACAT(tmp2, "<li>", env);
					AXIS2_FREE((*env)->allocator, tmp2);
                    tmp2 = ret;

					ret = AXIS2_STRACAT(tmp2, oname, env);
					AXIS2_FREE((*env)->allocator, tmp2);
                    tmp2 = ret;
					ret = AXIS2_STRACAT(tmp2, "</li>", env);
					AXIS2_FREE((*env)->allocator, tmp2);                    
					tmp2 = ret;
				}
				ret = AXIS2_STRACAT(tmp2, "</ul>", env);
				AXIS2_FREE((*env)->allocator, tmp2);
                tmp2 = ret;
			}
			else
			{
				ret = AXIS2_STRACAT(tmp2, "No operations Available", env);
				/*AXIS2_FREE((*env)->allocator, tmp);*/
                tmp2 = ret;
			}			
		}
	}
	if(NULL != errorneous_svc_map && 0 != axis2_hash_count(errorneous_svc_map))
	{
		void *fsname = NULL;
		svcs_exists = AXIS2_TRUE;
		ret = AXIS2_STRACAT(tmp2, "<hr><h2><font color=\"red\">Faulty \
						Services</font></h2>"
							, env);
		AXIS2_FREE((*env)->allocator, tmp2);
        tmp2 = ret;
		
		for(hi = axis2_hash_first(errorneous_svc_map, env); NULL != hi;
							axis2_hash_next(env, hi))
		{
			axis2_hash_this(hi, (const void **)&fsname, NULL, NULL);
			ret = AXIS2_STRACAT(tmp2, "<h3><font color=\"red\">", env);
			AXIS2_FREE((*env)->allocator, tmp2);
            tmp2 = ret;
			ret = AXIS2_STRACAT(tmp2, (axis2_char_t*)fsname, env);
			AXIS2_FREE((*env)->allocator, tmp2);
            tmp2 = ret;
			ret = AXIS2_STRACAT(tmp2, "</font></h3>", env);
			AXIS2_FREE((*env)->allocator, tmp2);
            tmp2 = ret;
		}
	}
	if(AXIS2_FALSE == svcs_exists)
	{
		ret = AXIS2_STRDUP("<h2>There are no services deployed</h2>", env);
	}
	ret = AXIS2_STRACAT("<html><head><title>Axis2C :: Services</title></head>"
						"<body><font face=\"courier\">" 
						, tmp2, env);
	/*AXIS2_FREE((*env)->allocator, tmp2);*/
    tmp2 = ret;
	ret = AXIS2_STRACAT(tmp2, "</font></body></html>\r\n", env);
	/*AXIS2_FREE((*env)->allocator, tmp);*/
	
	return ret;
}

AXIS2_DECLARE(axis2_char_t*)
axis2_http_transport_utils_get_charset_enc(axis2_env_t **env, 
						axis2_char_t *content_type)
{
	axis2_char_t *tmp = NULL;
	axis2_char_t *tmp_content_type = NULL;
	axis2_char_t *tmp2 = NULL;
	
	AXIS2_ENV_CHECK(env, NULL);
	AXIS2_PARAM_CHECK((*env)->error, content_type, NULL);
	
	tmp_content_type = AXIS2_STRDUP(content_type, env);
	if(NULL == tmp_content_type)
	{
		return AXIS2_STRDUP(AXIS2_HTTP_HEADER_DEFAULT_CHAR_ENCODING, env);
	}
	tmp = strstr(tmp_content_type, AXIS2_HTTP_CHAR_SET_ENCODING);
	if(NULL == tmp)
	{
        AXIS2_FREE((*env)->allocator, tmp_content_type);
		return AXIS2_STRDUP(AXIS2_HTTP_HEADER_DEFAULT_CHAR_ENCODING, env);
	}
	tmp = strchr(tmp, '=');
	tmp2 = strchr(tmp, ';');
	if(NULL != tmp2)
	{
        if('\'' == *(tmp2 - sizeof(axis2_char_t)) ||
                        '\"' == *(tmp2 - sizeof(axis2_char_t)))
        {
           tmp2 -= sizeof(axis2_char_t); 
        }
		*tmp2 = '\0';
	}
	if(NULL == tmp)
	{
        AXIS2_FREE((*env)->allocator, tmp_content_type);
		return AXIS2_STRDUP(AXIS2_HTTP_HEADER_DEFAULT_CHAR_ENCODING, env);
	}
    /* Following formats are acceptable
     * charset="UTF-8"
     * charser='UTF-8'
     * charset=UTF-8
     * But for our requirements charset we get should be UTF-8
     */
    if('\'' == *(tmp + sizeof(axis2_char_t)) || '\"' == *(tmp + 
                        sizeof(axis2_char_t)))
    {
       tmp += 2*sizeof(axis2_char_t); 
    }
    else
    {
        tmp += sizeof(axis2_char_t);
    }
	tmp2 =  AXIS2_STRDUP(tmp, env);
	AXIS2_FREE((*env)->allocator, tmp_content_type);
	return tmp2;
}

int
axis2_http_transport_utils_on_data_request(char *buffer, int size, void *ctx)
{
	axis2_env_t **env = NULL;
	int len = -1;
    axis2_callback_info_t *cb_ctx = (axis2_callback_info_t*)ctx;
    
	if(NULL == buffer || NULL == ctx)
	{
		return 0;
	}
	env = &((axis2_callback_info_t*)ctx)->env;
	if(cb_ctx->unread_len <= 0 && -1 != cb_ctx->content_length)
	{
		return 0;
	}
	if(cb_ctx->chunked_stream != NULL)
	{
		len = AXIS2_HTTP_CHUNKED_STREAM_READ(cb_ctx->chunked_stream, env, 
						buffer, size);
        buffer[len] = '\0';
		return len;
	}
	else
	{
		axis2_stream_t *in_stream = NULL;
        int read_len = 0;
        in_stream = (axis2_stream_t *)((axis2_callback_info_t*)ctx)->in_stream;
        /* For managed streams such as Apache2 streams we do not need to 
         * calculate lenghts
         */
        if(AXIS2_STREAM_MANAGED == AXIS2_STREAM_GET_TYPE(in_stream, env))
        {
            read_len = size;
        }
        else
        {
            if(size > ((axis2_callback_info_t*)ctx)->unread_len && 
                        -1 != ((axis2_callback_info_t*)ctx)->unread_len)
            {
                read_len = ((axis2_callback_info_t*)ctx)->unread_len;
            }
            else
            {
                read_len = size;
            }
        }
        len = AXIS2_STREAM_READ(in_stream, env, buffer, read_len);
        if(len > 0)
        {
            buffer[len] = '\0';
            if(AXIS2_STREAM_MANAGED != AXIS2_STREAM_GET_TYPE(in_stream, env))
            {
                ((axis2_callback_info_t*)ctx)->unread_len -= len;
            }            
        }
        return len;
    }
	return 0;	
}

AXIS2_DECLARE(axis2_soap_envelope_t*)
axis2_http_transport_utils_create_soap_msg(axis2_env_t **env, 
                        axis2_msg_ctx_t *msg_ctx, axis2_char_t *soap_ns_uri)
{
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_char_t *char_set_enc = NULL;
    axis2_char_t *content_type = NULL;
    axis2_stream_t *in_stream = NULL;
    axis2_callback_info_t *callback_ctx = NULL;
	axis2_char_t *trans_enc = NULL;
	int *content_length = NULL;
	axis2_property_t *property = NULL;
    AXIS2_ENV_CHECK(env, NULL);
	AXIS2_PARAM_CHECK((*env)->error, msg_ctx, NULL);
    AXIS2_PARAM_CHECK((*env)->error, soap_ns_uri, NULL);
    
    
    property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, 
        AXIS2_TRANSPORT_IN, AXIS2_FALSE);
    if(property)
    {
        in_stream = AXIS2_PROPERTY_GET_VALUE(property, env);
        property = NULL;
    }
    /* TODO free this when xml pulling is over */
    callback_ctx = AXIS2_MALLOC((*env)->allocator, sizeof(axis2_callback_info_t));
    if(NULL == callback_ctx)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
	callback_ctx->in_stream = in_stream;
	callback_ctx->env = *env;
	callback_ctx->content_length = -1;
	callback_ctx->unread_len = -1;
	callback_ctx->chunked_stream = NULL;
	
	property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, 
						AXIS2_HTTP_HEADER_CONTENT_LENGTH, AXIS2_FALSE);
    if(property)
    {
        content_length = AXIS2_PROPERTY_GET_VALUE(property, env);
        property = NULL;
    }
	if(content_length != NULL)
	{
		callback_ctx->content_length = *content_length;
		callback_ctx->unread_len = *content_length;
	}
    if(NULL == in_stream)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NULL_IN_STREAM_IN_MSG_CTX,
                        AXIS2_FAILURE);
        return NULL;
    }
    property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, 
						AXIS2_HTTP_HEADER_TRANSFER_ENCODING, AXIS2_FALSE);
    if(property)
    {
        trans_enc = AXIS2_PROPERTY_GET_VALUE(property, env);
        property = NULL;
    }
	if(NULL != trans_enc && 0 == AXIS2_STRCMP(trans_enc, 
						AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
	{
		callback_ctx->chunked_stream = axis2_http_chunked_stream_create(env, 
						in_stream);
		if(NULL == callback_ctx->chunked_stream)
		{
			return NULL;
		}
	}
	
    
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    if(NULL != op_ctx)
    {
        axis2_ctx_t *ctx = AXIS2_OP_CTX_GET_BASE(op_ctx, env);
        if (NULL != ctx)
        {
            property = AXIS2_CTX_GET_PROPERTY(ctx, env,
                        AXIS2_CHARACTER_SET_ENCODING, AXIS2_FALSE);
            if(property)
            {
                char_set_enc = AXIS2_PROPERTY_GET_VALUE(property, env);
                property = NULL;
            }
            property = AXIS2_CTX_GET_PROPERTY(ctx, env, 
                        MTOM_RECIVED_CONTENT_TYPE, AXIS2_FALSE);
            if(property)
            {
                content_type = AXIS2_PROPERTY_GET_VALUE(property, env);
                property = NULL;
            }
            
        }
    }
    if(NULL == char_set_enc)
    {
        char_set_enc = AXIS2_DEFAULT_CHAR_SET_ENCODING;
    }
    if(NULL != content_type)
    {
        AXIS2_MSG_CTX_SET_DOING_MTOM(msg_ctx, env, AXIS2_TRUE);
        /**
         * TODO MTOM stuff - create builder and get envelope
         */
    }
    else if(AXIS2_TRUE != AXIS2_MSG_CTX_GET_DOING_REST(msg_ctx, env))
    {
        axis2_xml_reader_t *xml_reader = NULL;
        axis2_om_stax_builder_t *om_builder = NULL;
        axis2_soap_builder_t *soap_builder = NULL;
        axis2_soap_envelope_t *soap_envelope = NULL;
        
        xml_reader = axis2_xml_reader_create_for_memory(env,
                        axis2_http_transport_utils_on_data_request,NULL,
                        (void *)callback_ctx, char_set_enc);
        if(NULL == xml_reader)
        {
            return NULL;
        }
        om_builder = axis2_om_stax_builder_create(env, xml_reader);
        if(NULL == om_builder)
        {
            AXIS2_XML_READER_FREE(xml_reader, env);
            xml_reader = NULL;
            return NULL;
        }
        soap_builder = axis2_soap_builder_create(env, om_builder,
                        soap_ns_uri);
        if(NULL == soap_builder)
        {
			/* We should not be freeing om_builder here as it is done by
            axis2_soap_builder_create in case of error - Samisa*/
            /*AXIS2_OM_STAX_BUILDER_FREE(om_builder, env);*/
            om_builder = NULL;
            xml_reader = NULL;
            return NULL;
        }
        soap_envelope = AXIS2_SOAP_BUILDER_GET_SOAP_ENVELOPE(soap_builder, env);
        return soap_envelope;
    }
    /**
     * TODO REST handling
     * else
     * {
     * }
     */
    return NULL;
}


AXIS2_DECLARE(axis2_char_t*)
axis2_http_transport_utils_get_value_from_content_type(axis2_env_t **env, 
						axis2_char_t *content_type, axis2_char_t *key)
{
	axis2_char_t *tmp = NULL;
	axis2_char_t *tmp_content_type = NULL;
	axis2_char_t *tmp2 = NULL;
	
	AXIS2_ENV_CHECK(env, NULL);
	AXIS2_PARAM_CHECK((*env)->error, content_type, NULL);
    AXIS2_PARAM_CHECK((*env)->error, key, NULL);
	
	tmp_content_type = AXIS2_STRDUP(content_type, env);
	if(NULL == tmp_content_type)
	{
		return NULL;
	}
	tmp = strstr(tmp_content_type, key);
	if(NULL == tmp)
	{
        AXIS2_FREE((*env)->allocator, tmp_content_type);
		return NULL;
	}

	tmp = strchr(tmp, '=');
	tmp2 = strchr(tmp, ';');
	
    if(NULL != tmp2)
	{
		*tmp2 = '\0';
	}
	if(NULL == tmp)
	{
        AXIS2_FREE((*env)->allocator, tmp_content_type);
		return NULL;
	}
	tmp2 =  AXIS2_STRDUP(tmp + 1, env);
	AXIS2_FREE((*env)->allocator, tmp_content_type);
	return tmp2;
}
