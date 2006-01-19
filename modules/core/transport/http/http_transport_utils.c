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
#include <axis2_soap.h>

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL 
axis2_http_transport_utils_process_http_post_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
                        axis2_stream_t *in_stream, axis2_stream_t *out_stream,
						axis2_char_t *content_type, 
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
axis2_http_transport_utils_is_optimized(axis2_env_t **env, 
                                        axis2_om_element_t *om_element);
                                                
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_do_write_mtom(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx);
                                                
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_doing_rest(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx);
                                                
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_doing_rest_through_post
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
						
axis2_status_t AXIS2_CALL
axis2_http_transport_utils_strdecode(axis2_env_t **env, axis2_char_t *dest, 
						axis2_char_t *src);
						
int AXIS2_CALL
axis2_http_transport_utils_hexit(axis2_char_t c);

axis2_char_t* AXIS2_CALL
axis2_http_transport_utils_get_services_html(axis2_env_t **env, 
							axis2_conf_ctx_t *conf_ctx);
axis2_char_t* AXIS2_CALL
axis2_http_transport_utils_get_charset_enc(axis2_env_t **env, 
						axis2_char_t *content_type);
int
axis2_http_transport_utils_on_data_request(char *buffer, int size, void *ctx);						
/***************************** End of function headers ************************/

axis2_status_t AXIS2_CALL 
axis2_http_transport_utils_process_http_post_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
                        axis2_stream_t *in_stream, axis2_stream_t *out_stream,
						axis2_char_t *content_type, 
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
	
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, in_stream, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, out_stream, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, content_type, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, request_uri, AXIS2_FAILURE);
	
	conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
	
	callback_ctx.in_stream = in_stream;
	callback_ctx.env = *env;
	
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
	AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, 
					AXIS2_TRANSPORT_OUT, out_stream, AXIS2_FALSE);
	AXIS2_MSG_CTX_SET_SERVER_SIDE(msg_ctx, env, AXIS2_TRUE);
	
	xml_reader = axis2_xml_reader_create_for_memory(env, 
						axis2_http_transport_utils_on_data_request, 
						(void *)&callback_ctx, NULL);
	char_set = axis2_http_transport_utils_get_charset_enc(env,content_type);
	/* TODO set the charset of the stream before (at least default)
	 *	we read them
	 */
	AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, AXIS2_CHARACTER_SET_ENCODING,
					char_set, AXIS2_TRUE);
	om_builder = axis2_om_stax_builder_create(env, xml_reader);
	if(NULL != strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP))
	{
		is_soap11 = AXIS2_FALSE;
		soap_builder = axis2_soap_builder_create(env, om_builder, 
                    AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI);
		soap_envelope = AXIS2_SOAP_BUILDER_GET_SOAP_ENVELOPE(soap_builder,
					env);
	}
	else if(NULL != strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML))
	{
		is_soap11 = AXIS2_TRUE;
		if(NULL != soap_action_header && AXIS2_STRLEN(soap_action_header)
					> 0)
		{
			soap_builder = axis2_soap_builder_create(env, om_builder, 
						AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI);
			soap_envelope = AXIS2_SOAP_BUILDER_GET_SOAP_ENVELOPE(
					soap_builder, env);
		}
		/* REST support
		 * else
		 * {
		 *		envelope = AXIS2_SOAP_ENVELOPE_GET_DEFAULT_SOAP_ENVELOPE(
		 *			env);
		 *		AXIS2_SOAP_BODY_ADD_CHILD(AXIS2_SOAP_ENVELOPE_GET_BODY(
		 *			envelope, env), AXIS2_OM_STAX_BUILDER_GET_DOCUMENT(
		 *			om_builder, env));
		 */			
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
	axis2_engine_t *engine = NULL;
	engine = axis2_engine_create(env, conf_ctx);
    
    if (!soap_envelope)
        return AXIS2_FAILURE;
    
    axis2_soap_body_t *soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    
    if (!soap_body)
        return AXIS2_FAILURE;
    
	if(AXIS2_TRUE == AXIS2_SOAP_BODY_HAS_FAULT(soap_body, env))
	{
		AXIS2_ENGINE_RECEIVE_FAULT(engine, env, msg_ctx);
	}
	else
	{
		AXIS2_ENGINE_RECEIVE(engine, env, msg_ctx);
	}
	if(NULL == AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env) && 
						AXIS2_FALSE == is_soap11)
	{
		axis2_soap_envelope_t *def_envelope = axis2_soap_envelope_create_null
						(env);
		AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, def_envelope);
	}
	return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL 
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
	AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, 
					AXIS2_TRANSPORT_OUT, out_stream, AXIS2_FALSE);
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


axis2_soap_envelope_t* AXIS2_CALL 
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
	envelope = axis2_soap_envelope_create_null(env);
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
		tmp_ele = axis2_om_element_create(env, op_node, (axis2_char_t*)name, def_om_ns, 
						&tmp_node);
		AXIS2_OM_ELEMENT_SET_TEXT(tmp_ele, env, (axis2_char_t*)value, tmp_node);
	}
    return envelope;
}


axis2_om_stax_builder_t* AXIS2_CALL 
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


axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_optimized(axis2_env_t **env, 
                                        axis2_om_element_t *om_element)
{
	/*
        TODO implement when MTOM support is added
    */
    return AXIS2_FALSE;
}


axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_do_write_mtom(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx)
{
   /*
        TODO implement when MTOM support is added
    */
    return AXIS2_FALSE;
}


axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_doing_rest(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx)
{
    /*
        TODO implement when REST support is added
    */
    return AXIS2_FALSE;
}


axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_doing_rest_through_post
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    /*
        TODO implement when REST support is added
    */
    return AXIS2_FALSE;
}

axis2_hash_t *AXIS2_CALL
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


axis2_status_t AXIS2_CALL
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


int AXIS2_CALL
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

axis2_char_t* AXIS2_CALL
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
				
				ret = AXIS2_STRACAT(tmp2, "Available Operations <ul>", env);
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
		ret = AXIS2_STRACAT(tmp2, "<hr><h2><font color=\"blue\">Faulty Services</font></h2>"
							, env);
		AXIS2_FREE((*env)->allocator, tmp2);
        tmp2 = ret;
		
		for(hi = axis2_hash_first(errorneous_svc_map, env); NULL != hi;
							axis2_hash_next(env, hi))
		{
			axis2_hash_this(hi, (const void **)&fsname, NULL, NULL);
			ret = AXIS2_STRACAT(tmp2, "<h3><font color=\"blue\">", env);
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
	ret = AXIS2_STRACAT("<html><head><title>Axis2C :: Services</title></head><body>"
							, tmp2, env);
	/*AXIS2_FREE((*env)->allocator, tmp2);*/
    tmp2 = ret;
	ret = AXIS2_STRACAT(tmp2, "</body></html>\r\n", env);
	/*AXIS2_FREE((*env)->allocator, tmp);*/
	
	return ret;
}

axis2_char_t* AXIS2_CALL
axis2_http_transport_utils_get_charset_enc(axis2_env_t **env, 
						axis2_char_t *content_type)
{
	axis2_char_t *tmp = NULL;
		
	AXIS2_ENV_CHECK(env, NULL);
	AXIS2_PARAM_CHECK((*env)->error, content_type, NULL);
	
	tmp = strstr(content_type, AXIS2_HTTP_CHAR_SET_ENCODING);
	if(NULL == tmp)
	{
		return AXIS2_STRDUP(AXIS2_HTTP_HEADER_DEFAULT_CHAR_ENCODING, env);
	}
	tmp = strchr(tmp, '=');
	if(NULL == tmp)
	{
		return AXIS2_STRDUP(AXIS2_HTTP_HEADER_DEFAULT_CHAR_ENCODING, env);
	}
	return AXIS2_STRDUP(tmp, env);
}

int
axis2_http_transport_utils_on_data_request(char *buffer, int size, void *ctx)
{
	axis2_stream_t *in_stream = NULL;
	axis2_env_t **env = NULL;
	if(NULL == buffer || NULL == ctx)
	{
		return 0;
	}
	in_stream = ((axis2_callback_info_t*)ctx)->in_stream;
	env = &((axis2_callback_info_t*)ctx)->env;
	return AXIS2_STREAM_READ(in_stream, env, buffer, size);
}
