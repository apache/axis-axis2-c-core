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

#include <axis2_http_sender.h>
#include <axutil_string.h>
#include <axis2_http_transport.h>
#include <string.h>
#include <axiom_output.h>
#include <axis2_op_ctx.h>
#include <axis2_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_http_client.h>
#include <axiom_xml_writer.h>
#include <axutil_property.h>
#include <axutil_param.h>
#include <axutil_types.h>
#include <axutil_generic_obj.h>
#include <axutil_uuid_gen.h>
#include <axutil_digest_calc.h>
#include <axis2_const.h>
#include <axis2_util.h>
#include <axiom_soap.h>
#include <axutil_version.h>

#ifdef AXIS2_LIBCURL_ENABLED
#include "libcurl/axis2_libcurl.h"
#else
#define CLIENT_NONCE_LENGTH 8
#endif

struct axis2_http_sender
{
    axis2_char_t *http_version;
    axis2_bool_t chunked;
    int so_timeout;
    axiom_output_t *om_output;
    axis2_http_client_t *client;
    axis2_bool_t is_soap;
};



#ifndef AXIS2_LIBCURL_ENABLED
static void axis2_http_sender_add_header_list (axis2_http_simple_request_t *
                                               request,
                                               const axutil_env_t * env,
                                               axutil_array_list_t *
                                               array_list);

static axis2_status_t axis2_http_sender_configure_proxy (axis2_http_sender_t *
                                                         sender,
                                                         const axutil_env_t *
                                                         env,
                                                         axis2_msg_ctx_t *
                                                         msg_ctx);

static axis2_status_t
axis2_http_sender_configure_server_cert (axis2_http_sender_t * sender,
                                         const axutil_env_t * env,
                                         axis2_msg_ctx_t * msg_ctx);

static axis2_status_t
axis2_http_sender_configure_key_file (axis2_http_sender_t * sender,
                                      const axutil_env_t * env,
                                      axis2_msg_ctx_t * msg_ctx);

static axis2_status_t
axis2_http_sender_configure_http_auth (axis2_http_sender_t * sender,
                                       const axutil_env_t * env,
                                       axis2_msg_ctx_t * msg_ctx,
                                       axis2_http_simple_request_t * request);

static axis2_status_t
axis2_http_sender_configure_proxy_auth (axis2_http_sender_t * sender,
                                        const axutil_env_t * env,
                                        axis2_msg_ctx_t * msg_ctx,
                                        axis2_http_simple_request_t * request);

static axis2_status_t
axis2_http_sender_set_http_auth_type (axis2_http_sender_t * sender,
                                      const axutil_env_t * env,
                                      axis2_msg_ctx_t * msg_ctx,
                                      axis2_http_simple_request_t * request);

static axis2_status_t
axis2_http_sender_set_proxy_auth_type (axis2_http_sender_t * sender,
                                       const axutil_env_t * env,
                                       axis2_msg_ctx_t * msg_ctx,
                                       axis2_http_simple_request_t * request);

static axis2_status_t
axis2_http_sender_configure_http_basic_auth (axis2_http_sender_t * sender,
                                             const axutil_env_t * env,
                                             axis2_msg_ctx_t * msg_ctx,
                                             axis2_http_simple_request_t *
                                             request);

static axis2_status_t
axis2_http_sender_configure_proxy_basic_auth (axis2_http_sender_t * sender,
                                              const axutil_env_t * env,
                                              axis2_msg_ctx_t * msg_ctx,
                                              axis2_http_simple_request_t *
                                              request);

static axis2_status_t
axis2_http_sender_configure_http_digest_auth (axis2_http_sender_t * sender,
                                              const axutil_env_t * env,
                                              axis2_msg_ctx_t * msg_ctx,
                                              axis2_http_simple_request_t * request,
                                              axis2_char_t * header_data);

static axis2_status_t
axis2_http_sender_configure_proxy_digest_auth (axis2_http_sender_t * sender,
                                               const axutil_env_t * env,
                                               axis2_msg_ctx_t * msg_ctx,
                                               axis2_http_simple_request_t * request,
                                               axis2_char_t * header_data);
#endif

AXIS2_EXTERN axis2_http_sender_t *AXIS2_CALL
axis2_http_sender_create (const axutil_env_t * env)
{
    axis2_http_sender_t *sender = NULL;

    sender = (axis2_http_sender_t *) AXIS2_MALLOC
        (env->allocator, sizeof (axis2_http_sender_t));

    if (!sender)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset (sender, 0, sizeof (axis2_http_sender_t));
    sender->http_version = (axis2_char_t *) AXIS2_HTTP_HEADER_PROTOCOL_11;
    sender->so_timeout = AXIS2_HTTP_DEFAULT_SO_TIMEOUT;
    /* unlike the java impl we don't have a default om output
     * it should be explicitly set and it's a MUST
     */
    sender->om_output = NULL;
    sender->chunked = AXIS2_FALSE;
    sender->client = NULL;

    return sender;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_http_sender_free (axis2_http_sender_t * sender,
                        const axutil_env_t * env)
{
    if (sender->http_version)
    {
        AXIS2_FREE (env->allocator, sender->http_version);
    }

    /* Do not free this here since it will be required in later processing
     * of the response soap message
     */
    sender->client = NULL;
    AXIS2_FREE (env->allocator, sender);
    return;
}

#ifndef AXIS2_LIBCURL_ENABLED
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_send (axis2_http_sender_t * sender,
                        const axutil_env_t * env,
                        axis2_msg_ctx_t * msg_ctx,
                        axiom_soap_envelope_t * out,
                        const axis2_char_t * str_url,
                        const axis2_char_t * soap_action)
{
    axis2_http_simple_request_t *request = NULL;
    axis2_http_request_line_t *request_line = NULL;

    /* url is to hold url given in str_url */
    axutil_url_t *url = NULL;
    axiom_xml_writer_t *xml_writer = NULL;
    axis2_char_t *buffer = NULL;
    unsigned int buffer_size = 0;
    const axis2_char_t *char_set_enc = NULL;
    axutil_string_t *char_set_enc_str = NULL;
    int status_code = -1;
    axis2_http_simple_response_t *response = NULL;
    axis2_char_t *content_type = NULL;
    axis2_bool_t content_type_deepl_copy = AXIS2_TRUE;
    axis2_byte_t *output_stream = NULL;
    int output_stream_size = 0;
    axis2_bool_t doing_mtom = AXIS2_FALSE;
    axutil_property_t *dump_property = NULL;
    axutil_param_t *ssl_pp_param = NULL;	
    /* ssl passphrase */
    axis2_char_t *ssl_pp = NULL;
    axutil_property_t *ssl_pp_property = NULL;
    axutil_property_t *test_auth_property = NULL;
    axis2_char_t *test_auth_property_value = NULL;
    axis2_bool_t test_proxy_auth = AXIS2_FALSE;
    axis2_bool_t test_http_auth = AXIS2_FALSE;

    /* http proxy authentication */
    axutil_property_t *proxy_auth_property = NULL;
    axis2_char_t *proxy_auth_property_value = NULL;
    axis2_bool_t force_proxy_auth = AXIS2_FALSE;
    axis2_bool_t force_proxy_auth_with_head = AXIS2_FALSE;
    axutil_property_t *http_auth_property = NULL;
    axis2_char_t *http_auth_property_value = NULL;
    axis2_bool_t force_http_auth = AXIS2_FALSE;
    axis2_bool_t force_http_auth_with_head = AXIS2_FALSE; 
    axutil_property_t *content_type_property = NULL;
    axutil_hash_t *content_type_hash = NULL;
    axis2_char_t *content_type_value = NULL;
    axutil_property_t *method = NULL;
    axis2_char_t *method_value = NULL;

    /* handling REST requests */
    axis2_bool_t send_via_get = AXIS2_FALSE;
    axis2_bool_t send_via_head = AXIS2_FALSE;
    axis2_bool_t send_via_delete = AXIS2_FALSE;
    axis2_bool_t send_via_put = AXIS2_FALSE;
    axiom_node_t *data_out = NULL;
    axiom_node_t *body_node = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axis2_bool_t is_soap = AXIS2_TRUE;

    /* http header property to get HTTP headers from msg_ctx and give
     * it to http_sender */
    axutil_property_t *http_property = NULL;
    axutil_array_list_t *array_list;
    axis2_bool_t http_auth_header_added = AXIS2_FALSE;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    axis2_transport_out_desc_t *trans_desc = NULL;
    axutil_param_t *write_xml_declaration_param = NULL;
    axutil_hash_t *transport_attrs = NULL;
    axis2_bool_t write_xml_declaration = AXIS2_FALSE;
    axutil_property_t *property = NULL;/* Property for holding http client */

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Entry:axis2_http_sender_send");
    soap_body = axiom_soap_envelope_get_body (out, env);

    AXIS2_PARAM_CHECK (env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, out, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, str_url, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, soap_action, AXIS2_FAILURE);

    if (AXIS2_TRUE == axis2_msg_ctx_get_doing_rest (msg_ctx, env))
    {
        is_soap = AXIS2_FALSE;
    }
    else
    {
        is_soap = AXIS2_TRUE;
    }

    url = axutil_url_parse_string (env, str_url);

    if (!is_soap)
    {
        if(soap_body)
        {
            body_node = axiom_soap_body_get_base_node (soap_body, env);
        }
        if(body_node)
        {
            data_out = axiom_node_get_first_element (body_node, env);
        }

        method = (axutil_property_t *) axis2_msg_ctx_get_property (msg_ctx, env,
                                                                   AXIS2_HTTP_METHOD);
        if (method)
        {
            method_value = (axis2_char_t *) axutil_property_get_value (method,
                                                                       env);
        }

        /* The default is POST */
        if (method_value && 
            0 == axutil_strcmp (method_value, AXIS2_HTTP_GET))
        {
            send_via_get = AXIS2_TRUE;
        }
        else if (method_value && 
                 0 == axutil_strcmp (method_value, AXIS2_HTTP_HEAD))
        {
            send_via_head = AXIS2_TRUE;
        }
        else if (method_value && 
                 0 == axutil_strcmp (method_value, AXIS2_HTTP_PUT))
        {
            send_via_put = AXIS2_TRUE;
        }
        else if (method_value && 
                 0 == axutil_strcmp (method_value, AXIS2_HTTP_DELETE))
        {
            send_via_delete = AXIS2_TRUE;
        }
    }

    if (!url)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                         "url is null for string %s", str_url);
        return AXIS2_FAILURE;
    }

    if (sender->client)
    {
        axis2_http_client_free (sender->client, env);
        sender->client = NULL;
    }

    sender->client = axis2_http_client_create (env, url);
    if (!sender->client)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                         "sender->client creation failed for url %s", url);
        return AXIS2_FAILURE;
    }
   
    /* We put the client into msg_ctx so that we can free it once the processing
     * is done at client side
     */
    property = axutil_property_create (env);
    axutil_property_set_scope (property, env, AXIS2_SCOPE_REQUEST);
    axutil_property_set_free_func (property, env,
                                   axis2_http_client_free_void_arg);
    axutil_property_set_value (property, env, sender->client);
    axis2_msg_ctx_set_property (msg_ctx, env, AXIS2_HTTP_CLIENT, property);

    /* configure proxy settings if we have set so
     */

    axis2_http_sender_configure_proxy (sender, env, msg_ctx);
    
    conf_ctx = axis2_msg_ctx_get_conf_ctx (msg_ctx, env);
    if (conf_ctx)
    {
        conf = axis2_conf_ctx_get_conf (conf_ctx, env);
    }

    if (conf)
    {
        trans_desc = axis2_conf_get_transport_out (conf,
                                                   env, 
                                                   AXIS2_TRANSPORT_ENUM_HTTP);
    }

    if (trans_desc)
    {
        /* get xml declaration details from axis2.xml */
        write_xml_declaration_param =
            axutil_param_container_get_param
            (axis2_transport_out_desc_param_container (trans_desc, env), env,
             AXIS2_XML_DECLARATION);
    }


    if (write_xml_declaration_param)
    {
        /* accessing attributes of the HTTP transport's , xml
         * declaration element from axis2.xml*/

        transport_attrs = axutil_param_get_attributes (write_xml_declaration_param, env);
        if (transport_attrs)
        {
            axutil_generic_obj_t *obj = NULL;
            axiom_attribute_t *write_xml_declaration_attr = NULL;
            axis2_char_t *write_xml_declaration_attr_value = NULL;

            obj = axutil_hash_get (transport_attrs, AXIS2_ADD_XML_DECLARATION,
                                   AXIS2_HASH_KEY_STRING);
            if (obj)
            {
                write_xml_declaration_attr = 
                    (axiom_attribute_t *) axutil_generic_obj_get_value (obj,
                                                                        env);
            }

            if (write_xml_declaration_attr)
            {
                write_xml_declaration_attr_value = 
                    axiom_attribute_get_value (write_xml_declaration_attr, env);
            }

            if (write_xml_declaration_attr_value && 
                0 == axutil_strcasecmp (write_xml_declaration_attr_value, 
                                        AXIS2_VALUE_TRUE))
            {
                write_xml_declaration = AXIS2_TRUE;
            }
        }
    }

    if (write_xml_declaration)
    {
        axiom_output_write_xml_version_encoding (sender->om_output, env);
    }

    if (!send_via_get && !send_via_head && !send_via_delete)
    {
        /* processing POST and PUT methods */
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "msg_ctx_id:%s", 
            axis2_msg_ctx_get_msg_id(msg_ctx,env));

        doing_mtom = axis2_msg_ctx_get_doing_mtom (msg_ctx, env);

        if (!sender->om_output)
        {
            AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NULL_OM_OUTPUT,
                               AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "%s",
                AXIS2_ERROR_GET_MESSAGE(env->error));
            return AXIS2_FAILURE;
        }

        xml_writer = axiom_output_get_xml_writer (sender->om_output, env);

        char_set_enc_str = axis2_msg_ctx_get_charset_encoding (msg_ctx, env);

        if (!char_set_enc_str)
        {
            /* if there is no character encoding details available
             * use default one.  
             * #define AXIS2_DEFAULT_CHAR_SET_ENCODING "UTF-8"
             */
            char_set_enc = AXIS2_DEFAULT_CHAR_SET_ENCODING;
        }
        else
        {
            char_set_enc = axutil_string_get_buffer (char_set_enc_str, env);
        }

        if (!send_via_put && is_soap)
        {
            /* HTTP POST case */
            /* dump property use to dump message without sending */
            dump_property = axis2_msg_ctx_get_property (msg_ctx, env,
                                                        AXIS2_DUMP_INPUT_MSG_TRUE);
            if (dump_property)
            {
                axis2_char_t *dump_true =
                    axutil_property_get_value (dump_property, env);
                if (0 == axutil_strcmp (dump_true, AXIS2_VALUE_TRUE))
                {
                    axis2_http_client_set_dump_input_msg (sender->client,
                                                          env, AXIS2_TRUE);
                }
            }

            axiom_output_set_do_optimize (sender->om_output, env, doing_mtom);
            axiom_soap_envelope_serialize (out, env, sender->om_output,
                                           AXIS2_FALSE);
        }
        else if (is_soap)
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                             "Attempt to send SOAP message using HTTP PUT failed");
            return AXIS2_FAILURE;
        }
        else
        {
            if (!data_out)
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                    "body node payload is NULL");
                return AXIS2_FAILURE;
            }
            axiom_node_serialize (data_out, env, sender->om_output);
        }

        if (doing_mtom)
        {
            axutil_param_t *callback_name_param = NULL;
            axis2_status_t mtom_status = AXIS2_FAILURE;
            axutil_array_list_t *mime_parts = NULL;
            axis2_char_t *mtom_sending_callback_name = NULL;

            /* Getting the sender callback name paramter if it is 
             * specified in the configuration file */

            callback_name_param = axis2_msg_ctx_get_parameter(msg_ctx, env ,
                AXIS2_MTOM_SENDING_CALLBACK);
            if(callback_name_param)
            {
                mtom_sending_callback_name =
                    (axis2_char_t *) axutil_param_get_value (callback_name_param, env);
                if(mtom_sending_callback_name)
                {
                    axis2_http_client_set_mtom_sending_callback_name(
                        sender->client, env, mtom_sending_callback_name);
                }
            }

            /* Here we put all the attachment related stuff in a array_list
               After this method we have the message in parts */

            mtom_status = axiom_output_flush (sender->om_output, env);
            if(mtom_status == AXIS2_FAILURE)
            {
                return mtom_status;
            }
            /* HTTP client should distinguish an MTOM invocation because the way
               of sending the message in MTOM case is different */
            axis2_http_client_set_doing_mtom(sender->client, env, doing_mtom);

            /* HTTP client will keep this mime_parts, which it will send in chunks at
               th end */
            mime_parts = axiom_output_get_mime_parts(sender->om_output, env);
            if(mime_parts)
            {
                axis2_http_client_set_mime_parts(sender->client, env, mime_parts);    
            }
        }
        else
        {
            buffer = axiom_xml_writer_get_xml (xml_writer, env);
        }

        if (!(buffer || doing_mtom))
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                             "NULL xml returned from xml writer");
            return AXIS2_FAILURE;
        }

        if (!send_via_put)
        {
            /* HTTP POST */
            axis2_char_t *path_with_query = NULL;

            path_with_query = axutil_strcat(env, axutil_url_get_path (url, env),
                                                 axutil_url_get_query(url, env), NULL);
            request_line =
                axis2_http_request_line_create (env, AXIS2_HTTP_POST,
                                                path_with_query,
                                                sender->http_version);
            AXIS2_FREE(env->allocator, path_with_query);
        }
        else
        {
            /* HTTP PUT */
            request_line =
                axis2_http_request_line_create (env, AXIS2_HTTP_PUT,
                                                axutil_url_get_path (url, env),
                                                sender->http_version);
        }
    }
    else
    {
        /* Processing HTTP GET, HEAD and DELETE */

        axis2_char_t *request_params = NULL;
        axis2_char_t *path = NULL;

        request_params = axis2_http_sender_get_param_string (sender,
                                                             env, msg_ctx);

        if(request_params)
        {
            /* substituting AXIS2_Q_MARK for "?" */
            path = axutil_strcat (env,
                              axutil_url_get_path (url, env),
                              AXIS2_Q_MARK_STR, request_params, NULL);
			AXIS2_FREE(env->allocator, request_params);
			request_params = NULL;
        }
        else
        {
            path = axutil_strdup(env,axutil_url_get_path(url, env));
        }

        if (send_via_get)
        {
            request_line = axis2_http_request_line_create (env, 
                                                           AXIS2_HTTP_GET, 
                                                           path,
                                                           sender->http_version);
        }
        else if (send_via_head)
        {
            request_line = axis2_http_request_line_create (env, 
                                                           AXIS2_HTTP_HEAD, 
                                                           path,
                                                           sender->http_version);
        }
        else if (send_via_delete)
        {
            request_line = axis2_http_request_line_create (env, 
                                                           AXIS2_HTTP_DELETE,
                                                           path,
                                                           sender->http_version);
        }

		if(path)
		{
			AXIS2_FREE(env->allocator, path);
			path = NULL;
		}
    }

    request = axis2_http_simple_request_create (env, request_line, NULL, 0,
                                                NULL);

    /* User-Agent:Axis2/C header */
    axis2_http_sender_util_add_header (env,
                                       request,
                                       AXIS2_HTTP_HEADER_USER_AGENT, 
                                       AXIS2_USER_AGENT);


    http_property =
        axis2_msg_ctx_get_property (msg_ctx,
                                    env, AXIS2_TRANSPORT_HEADER_PROPERTY);
    if (http_property)
    {
        array_list = (axutil_array_list_t *)
            axutil_property_get_value (http_property, env);
        axis2_http_sender_add_header_list (request, env, array_list);
    }

    if (!send_via_get && !send_via_head && !send_via_put && !send_via_delete &&
        AXIS2_TRUE == axis2_msg_ctx_get_is_soap_11 (msg_ctx, env))
    {
        if (AXIS2_ESC_DOUBLE_QUOTE != *soap_action)
        {
            axis2_char_t *tmp_soap_action = NULL;
            tmp_soap_action =
                AXIS2_MALLOC (env->allocator,
                              (axutil_strlen (soap_action) +
                               5) * sizeof (axis2_char_t));
            sprintf (tmp_soap_action, "\"%s\"", soap_action);
            axis2_http_sender_util_add_header (env,
                                               request,
                                               AXIS2_HTTP_HEADER_SOAP_ACTION,
                                               tmp_soap_action);
            AXIS2_FREE (env->allocator, tmp_soap_action);
        }
        else
        {
            axis2_http_sender_util_add_header (env,
                                               request,
                                               AXIS2_HTTP_HEADER_SOAP_ACTION,
                                               (const axis2_char_t *)
                                               soap_action);
        }
    }
    else if (AXIS2_TRUE == axis2_msg_ctx_get_is_soap_11 (msg_ctx, env))
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                         "Adding of SOAP Action Failed for REST request");

        return AXIS2_FAILURE;
    }

    if (!send_via_get && !send_via_head && !send_via_delete)
    {
        /* processing PUT and POST */
        buffer_size = axiom_xml_writer_get_xml_size (xml_writer, env);

        if (AXIS2_FALSE == sender->chunked)
        {
            axis2_char_t tmp_buf[10];
            if (!buffer)
            {
                buffer_size = output_stream_size;
            }

            if (buffer_size)
            {
                sprintf (tmp_buf, "%d", buffer_size);
                axis2_http_sender_util_add_header (env,
                                                   request,
                                                   AXIS2_HTTP_HEADER_CONTENT_LENGTH,
                                                   tmp_buf);
            }
        }
        else
        {
            axis2_http_sender_util_add_header (env,
                                               request,
                                               AXIS2_HTTP_HEADER_TRANSFER_ENCODING,
                                               AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED);
        }

        if (!send_via_put && is_soap)
        {
            /* HTTP POST */
            if (doing_mtom)
            {
                content_type =
                    (axis2_char_t *) axiom_output_get_content_type (sender->
                                                                    om_output,
                                                                    env);
                if (AXIS2_TRUE != axis2_msg_ctx_get_is_soap_11 (msg_ctx, env) &&
                    axutil_strcmp (soap_action, ""))
                {
                    /* handle SOAP action for SOAP 1.2 case */
                    axis2_char_t *temp_content_type = NULL;
                    temp_content_type = axutil_stracat (env,
                                                        content_type,
                                                        AXIS2_CONTENT_TYPE_ACTION);
                    content_type = temp_content_type;
                    temp_content_type = axutil_stracat (env,
                                                        content_type,
                                                        soap_action);
                    AXIS2_FREE (env->allocator, content_type);
                    content_type = temp_content_type;
                    temp_content_type =
                        axutil_stracat (env, content_type, 
                                        AXIS2_ESC_DOUBLE_QUOTE_STR);
                    AXIS2_FREE (env->allocator, content_type);
                    content_type = temp_content_type;
                }
                else
                {
                    content_type_deepl_copy = AXIS2_FALSE;
                }
            }
            else if (AXIS2_TRUE == axis2_msg_ctx_get_is_soap_11 (msg_ctx, env))
            {
                /* SOAP 1.1 without MTOM */
                axis2_char_t *temp_content_type = NULL;
                content_type =
                    (axis2_char_t *) AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML;
                content_type = axutil_stracat (env, content_type, 
                                               AXIS2_CONTENT_TYPE_CHARSET);
                temp_content_type = axutil_stracat (env,
                                                    content_type, char_set_enc);
                AXIS2_FREE (env->allocator, content_type);
                content_type = temp_content_type;
            }
            else
            {
                /* SOAP 1.2 without MTOM */
                axis2_char_t *temp_content_type = NULL;
                content_type =
                    (axis2_char_t *) AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP;
                content_type = axutil_stracat (env, content_type, 
                                               AXIS2_CONTENT_TYPE_CHARSET);
                temp_content_type = axutil_stracat (env,
                                                    content_type, char_set_enc);
                AXIS2_FREE (env->allocator, content_type);
                content_type = temp_content_type;
                if (axutil_strcmp (soap_action, ""))
                {
                    temp_content_type = axutil_stracat (env,
                                                        content_type,
                                                        AXIS2_CONTENT_TYPE_ACTION);
                    AXIS2_FREE (env->allocator, content_type);
                    content_type = temp_content_type;
                    temp_content_type = axutil_stracat (env,
                                                        content_type,
                                                        soap_action);
                    AXIS2_FREE (env->allocator, content_type);
                    content_type = temp_content_type;
                    temp_content_type =
                        axutil_stracat (env, content_type, 
                                        AXIS2_ESC_DOUBLE_QUOTE_STR);
                    AXIS2_FREE (env->allocator, content_type);
                    content_type = temp_content_type;
                }
            }
        }
        else if (is_soap)
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                             "Attempt to send SOAP message using HTTP PUT failed");
            return AXIS2_FAILURE;
        }
        else
        {
            content_type_property = (axutil_property_t *)
                axis2_msg_ctx_get_property (msg_ctx,
                                            env,
                                            AXIS2_USER_DEFINED_HTTP_HEADER_CONTENT_TYPE);

            if (content_type_property)
            {
                content_type_hash = (axutil_hash_t *)
                    axutil_property_get_value (content_type_property, env);
                if (content_type_hash)
                {
                    content_type_value =
                        (char *) axutil_hash_get (content_type_hash,
                                                  AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                                  AXIS2_HASH_KEY_STRING);
                }
            }

            if (content_type_value)
            {
                content_type = content_type_value;
            }
            else
            {
                content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML;
            }

            content_type_deepl_copy = AXIS2_FALSE;
        }

        axis2_http_sender_util_add_header (env,
                                           request,
                                           AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                           content_type);

        if (content_type_deepl_copy && content_type)
        {
            AXIS2_FREE (env->allocator, content_type);
            content_type = NULL;
        }
        /* Finished Processing PUT and POST */
    }

    if (0 ==
        axutil_strcasecmp (sender->http_version, AXIS2_HTTP_HEADER_PROTOCOL_11))
    {
        /* HTTP 1.1 */
        axis2_char_t *header = NULL;
        int host_len = 0;
        host_len = axutil_strlen (axutil_url_get_host (url, env));
        header = AXIS2_MALLOC (env->allocator,
                               host_len + 10 * sizeof (axis2_char_t));
        sprintf (header, "%s:%d",
                 axutil_url_get_host (url, env), axutil_url_get_port (url,
                                                                        env));
        axis2_http_sender_util_add_header (env, request, AXIS2_HTTP_HEADER_HOST,
                                           header);
        AXIS2_FREE (env->allocator, header);
        header = NULL;
    }

    /* If this is a normal invocation the buffer has the full SOAP message
       which needs to be send. In the MTOM case instead of this buffer 
       it has the mime_parts array_list */

    if(!doing_mtom)
    {
        axis2_http_simple_request_set_body_string (request,
                                                   env, buffer, buffer_size);
    }
    

    /* HTTPS request processing */
    axis2_http_sender_configure_server_cert (sender, env, msg_ctx);

    axis2_http_sender_configure_key_file (sender, env, msg_ctx);
    
    axis2_http_sender_get_timeout_values (sender, env, msg_ctx);
    axis2_http_client_set_timeout (sender->client, env, sender->so_timeout);

    ssl_pp_property = axis2_msg_ctx_get_property(msg_ctx, env,
                                                 AXIS2_SSL_PASSPHRASE);
    
    if (ssl_pp_property)
    {
            ssl_pp = (axis2_char_t *)
                     axutil_property_get_value(ssl_pp_property, env);
    }
    else
    {
        ssl_pp_param = axis2_msg_ctx_get_parameter(msg_ctx,
                                                   env, AXIS2_SSL_PASSPHRASE);

        if (ssl_pp_param)
        {
            ssl_pp = axutil_param_get_value(ssl_pp_param, env);
        }
    }

    test_auth_property = (axutil_property_t *)
        axis2_msg_ctx_get_property (msg_ctx, env,
                                    AXIS2_TEST_PROXY_AUTH);
    if (test_auth_property)
    {
        test_auth_property_value = 
            (axis2_char_t *) axutil_property_get_value (test_auth_property,
                                                        env);
    }

    if (test_auth_property_value && 
        0 == axutil_strcmp (test_auth_property_value, AXIS2_VALUE_TRUE))
    {
        test_proxy_auth = AXIS2_TRUE;
    }
    
    test_auth_property = NULL;
    test_auth_property_value = NULL;

    test_auth_property = 
        (axutil_property_t *) axis2_msg_ctx_get_property (msg_ctx, env,
                                                          AXIS2_TEST_HTTP_AUTH);
    if (test_auth_property)
    {
        test_auth_property_value = 
            (axis2_char_t *) axutil_property_get_value (test_auth_property,
                                                        env);
    }

    if (test_auth_property_value && 
        0 == axutil_strcmp (test_auth_property_value,
                            AXIS2_VALUE_TRUE))
    {
        test_http_auth = AXIS2_TRUE;
    }

    if (!test_proxy_auth)
    {
        proxy_auth_property = 
            (axutil_property_t *) axis2_msg_ctx_get_property (msg_ctx, env,
                                                              AXIS2_FORCE_PROXY_AUTH);
    }
    if (proxy_auth_property)
    {
        proxy_auth_property_value = (axis2_char_t *) 
            axutil_property_get_value (proxy_auth_property,
                                       env); 
    }

    if (proxy_auth_property_value && 
        0 == axutil_strcmp (proxy_auth_property_value, AXIS2_VALUE_TRUE))
    {
        force_proxy_auth = AXIS2_TRUE;
    }

    proxy_auth_property = NULL;
    proxy_auth_property_value = NULL;
    
    if (force_proxy_auth)
    {
        proxy_auth_property = (axutil_property_t *) 
            axis2_msg_ctx_get_property (msg_ctx, env,
                                        AXIS2_PROXY_AUTH_TYPE);
    }

    if (proxy_auth_property)
    {
        proxy_auth_property_value = (axis2_char_t *) 
            axutil_property_get_value (proxy_auth_property,
                                       env);
    }

    if (proxy_auth_property_value && 
        0 == axutil_strcmp (proxy_auth_property_value, 
                            AXIS2_PROXY_AUTH_TYPE_DIGEST))
    {
        force_proxy_auth = AXIS2_FALSE;
        force_proxy_auth_with_head = AXIS2_TRUE;
    }

    if (!test_http_auth)
    {
        http_auth_property = (axutil_property_t *) 
            axis2_msg_ctx_get_property (msg_ctx, env,
                                        AXIS2_FORCE_HTTP_AUTH);
    }

    if (http_auth_property)
    {
        http_auth_property_value = (axis2_char_t *) 
            axutil_property_get_value (http_auth_property,
                                       env);
    }

    if (http_auth_property_value && 
        0 == axutil_strcmp (http_auth_property_value, AXIS2_VALUE_TRUE))
    {
        force_http_auth = AXIS2_TRUE;
    }

    http_auth_property = NULL;
    http_auth_property_value = NULL;
    
    if (force_http_auth)
    {
        http_auth_property = (axutil_property_t *) 
            axis2_msg_ctx_get_property (msg_ctx, env,
                                        AXIS2_HTTP_AUTH_TYPE);
    }

    if (http_auth_property)
    {
        http_auth_property_value = (axis2_char_t *) 
            axutil_property_get_value (http_auth_property,
                                       env);
    }

    if (http_auth_property_value && 
        0 == axutil_strcmp (http_auth_property_value, 
                            AXIS2_HTTP_AUTH_TYPE_DIGEST))
    {
        force_http_auth = AXIS2_FALSE;
        force_http_auth_with_head = AXIS2_TRUE;
    }

    axis2_msg_ctx_set_auth_type(msg_ctx, env, NULL);

    if (force_proxy_auth || force_proxy_auth_with_head)
    {
        status_code = 
            AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL;
    }
    else
    {
        /* NOT forcing proxy authentication  */
        if (force_http_auth)
        {
            axis2_status_t auth_status;
            auth_status = 
                axis2_http_sender_configure_http_auth (sender,
                                                       env,
                                                       msg_ctx, request);

            if (auth_status != AXIS2_SUCCESS)
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                                 "Error in setting HTTP Authentication header");
            }
            http_auth_header_added = AXIS2_TRUE;

            status_code = axis2_http_client_send (sender->client, 
                                                  env, request, ssl_pp);

            status_code = axis2_http_client_recieve_header (sender->client, 
                                                            env); 
        }

        if (force_http_auth_with_head)
        {
            axis2_http_request_line_t *head_request_line = NULL;
            axis2_http_request_line_t *temp = NULL;

            temp = axis2_http_simple_request_get_request_line(request, env);
            head_request_line = axis2_http_request_line_create(env, 
                                                               AXIS2_HTTP_HEAD,
                                 axis2_http_request_line_get_uri(temp, env),
                                 axis2_http_request_line_get_http_version(temp, 
                                                                          env));
            axis2_http_simple_request_set_request_line(request, env, 
                                                       head_request_line);

            status_code = axis2_http_client_send (sender->client, env, 
                                                  request, ssl_pp);

            status_code = axis2_http_client_recieve_header (sender->client, 
                                                            env);
            axis2_http_simple_request_set_request_line(request, env, temp);
            axis2_http_request_line_free(head_request_line, env);

            if (status_code == AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL)
            {
                axis2_status_t auth_status;
                auth_status = axis2_http_sender_configure_http_auth (sender,
                                                                     env,
                                                                     msg_ctx, 
                                                                     request);

                if (auth_status != AXIS2_SUCCESS)
                {
                    AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                                     "Error in setting HTTP Authentication\
 header");
                }
                http_auth_header_added = AXIS2_TRUE;
            }

            if (status_code != 
                AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL)
            {

                status_code = axis2_http_client_send (sender->client, env, 
                                                      request, ssl_pp);

                status_code = axis2_http_client_recieve_header (sender->client, 
                                                                env);

                if (status_code == 
                    AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL)
                {
                    http_auth_header_added = AXIS2_FALSE;
                    force_http_auth_with_head = AXIS2_FALSE;
                }
            }
        }
        else 
        {
            status_code = axis2_http_client_send (sender->client, env, 
                                                  request, ssl_pp);

            status_code = axis2_http_client_recieve_header (sender->client, 
                                                            env);
        }
    }

    if (AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL == 
        status_code && !test_proxy_auth)
    {
        if (force_proxy_auth_with_head)
        {
            axis2_http_request_line_t *head_request_line = NULL;
            axis2_http_request_line_t *temp = NULL;

            temp = axis2_http_simple_request_get_request_line(request, env);
            head_request_line = axis2_http_request_line_create(env, 
                                                               AXIS2_HTTP_HEAD,
                                 axis2_http_request_line_get_uri(temp, env),
                                 axis2_http_request_line_get_http_version(temp, 
                                                                          env));
            axis2_http_simple_request_set_request_line(request, env, 
                                                       head_request_line);

            status_code = axis2_http_client_send (sender->client, env, 
                                                  request, ssl_pp);

            status_code = axis2_http_client_recieve_header (sender->client, 
                                                            env);
            axis2_http_simple_request_set_request_line(request, env, 
                                                       temp);
            axis2_http_request_line_free(head_request_line, env);

            if (status_code == 
                AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL)
            {
                axis2_status_t auth_status;
                auth_status = axis2_http_sender_configure_proxy_auth (sender,
                                                                      env,
                                                                      msg_ctx, 
                                                                      request);

                if (auth_status != AXIS2_SUCCESS)
                {
                    AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                                     "Error in setting Proxy Authentication\
 header");
                }

                if ((force_http_auth_with_head || force_http_auth) && 
                    !http_auth_header_added)
                {
                     status_code = 
                         AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL;
                }
                else
                {
                    status_code = axis2_http_client_send (sender->client, env, 
                                                          request, ssl_pp);

                    status_code = 
                        axis2_http_client_recieve_header (sender->client, env);
                }
            }
            else if (status_code != 
                     AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL)
            {

                status_code = axis2_http_client_send (sender->client, env, 
                                                      request, ssl_pp);

                status_code = axis2_http_client_recieve_header (sender->client,
                                                                env);
            }

            /* Proxies have no idea about HTTP Methods therefore, if
             * it fails no need to re-check */ 
            if (AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL == 
                status_code)
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                                 "Proxy Authentication failed");
                axis2_msg_ctx_set_auth_failed(msg_ctx, env, AXIS2_TRUE);
                axis2_msg_ctx_set_required_auth_is_http(msg_ctx, env, 
                                                        AXIS2_FALSE);
            }
        }
        else
        {

            /* not forcing proxy auth with head */
            axis2_status_t auth_status;
            auth_status = axis2_http_sender_configure_proxy_auth (sender,
                                                                  env,
                                                                  msg_ctx, 
                                                                  request);

            if (auth_status != AXIS2_SUCCESS)
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                                 "Error in setting Proxy Authentication \
header");
            }

            if ((force_http_auth_with_head || force_http_auth) && 
                !http_auth_header_added)
            {
                status_code = AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL;
            }
            else
            {
                status_code = axis2_http_client_send (sender->client, env, 
                                                      request, ssl_pp);
    
                status_code = axis2_http_client_recieve_header (sender->client, 
                                                                env);

                if (AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL ==
                    status_code)
                {
                    AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                                     "Proxy Authentication failed");
                    axis2_msg_ctx_set_auth_failed(msg_ctx, env, AXIS2_TRUE);
                    axis2_msg_ctx_set_required_auth_is_http(msg_ctx, env, 
                                                            AXIS2_FALSE);
                }
            }
        }
    }
    else if (AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL == 
             status_code)
    {
        axis2_msg_ctx_set_auth_failed(msg_ctx, env, AXIS2_TRUE);
        axis2_msg_ctx_set_required_auth_is_http(msg_ctx, env, AXIS2_FALSE);
        axis2_http_sender_set_proxy_auth_type (sender, env, msg_ctx, request);
    }
 
    if (AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL == status_code && 
        !test_http_auth)
    {
        if (!http_auth_header_added)
        {
            if (force_proxy_auth_with_head)
            {
                axis2_http_request_line_t *head_request_line = NULL;
                axis2_http_request_line_t *temp = NULL;

                temp = axis2_http_simple_request_get_request_line(request, env);
                head_request_line = axis2_http_request_line_create(env, 
                                                                   AXIS2_HTTP_HEAD,
                                     axis2_http_request_line_get_uri(temp, env),
                                     axis2_http_request_line_get_http_version(
                                         temp, env));
                axis2_http_simple_request_set_request_line(request, env, 
                                                           head_request_line);

                status_code = axis2_http_client_send (sender->client, env, 
                                                      request, ssl_pp);
 
                status_code = axis2_http_client_recieve_header (sender->client,
                                                                env);
                axis2_http_simple_request_set_request_line(request, env, 
                                                           temp);
                axis2_http_request_line_free(head_request_line, env);

                if (status_code == 
                    AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL)
                {
                    axis2_status_t auth_status;
                    auth_status = axis2_http_sender_configure_http_auth (sender,
                                                                         env,
                                                                         msg_ctx, 
                                                                         request);

                    if (auth_status != AXIS2_SUCCESS)
                    {
                        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                                         "Error in setting HTTP Authentication \
header");
                    }

                    status_code = axis2_http_client_send (sender->client,
                                                          env, request, ssl_pp);

                    status_code = 
                        axis2_http_client_recieve_header (sender->client, 
                                                          env);
                    if (status_code == 
                        AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL)
                    {
                        axis2_status_t auth_status;
                        auth_status = 
                            axis2_http_sender_configure_http_auth (sender,
                                                                   env,
                                                                   msg_ctx, 
                                                                   request);

                        if (auth_status != AXIS2_SUCCESS)
                        {
                            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                                             "Error in setting HTTP \
Authentication header");
                        }

                        status_code = axis2_http_client_send (sender->client,
                                                              env, request, 
                                                              ssl_pp);

                        status_code = 
                            axis2_http_client_recieve_header (sender->client, env);
                    }
 
                }
            }
            else
            {
                axis2_status_t auth_status;
                auth_status = axis2_http_sender_configure_http_auth (sender,
                                                                     env,
                                                                     msg_ctx,
                                                                     request);

                if (auth_status != AXIS2_SUCCESS)
                {
                    AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                                     "Error in setting HTTP Authentication \
header");
                }

                status_code = axis2_http_client_send (sender->client,
                                                      env, request, ssl_pp);

                status_code = axis2_http_client_recieve_header (sender->client, 
                                                                env);
            }

            if (AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL == status_code)
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                                 "HTTP Authentication failed");
                axis2_msg_ctx_set_auth_failed(msg_ctx, env, AXIS2_TRUE);
                axis2_msg_ctx_set_required_auth_is_http(msg_ctx, env, 
                                                        AXIS2_TRUE);
            }

            if (AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL == 
                status_code)
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                                 "Proxy Authentication failed");
                axis2_msg_ctx_set_auth_failed(msg_ctx, env, AXIS2_TRUE);
                axis2_msg_ctx_set_required_auth_is_http(msg_ctx, env, 
                                                        AXIS2_FALSE);
            }
        }
        else
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                             "HTTP Authentication failed");
            axis2_msg_ctx_set_auth_failed(msg_ctx, env, AXIS2_TRUE);
            axis2_msg_ctx_set_required_auth_is_http(msg_ctx, env, AXIS2_TRUE);
        }
    }
    else if (AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL == 
             status_code)
    {
        axis2_msg_ctx_set_auth_failed(msg_ctx, env, AXIS2_TRUE);
        axis2_msg_ctx_set_required_auth_is_http(msg_ctx, env, AXIS2_TRUE);
        axis2_http_sender_set_http_auth_type (sender, env, msg_ctx, request);
    }

    axis2_http_simple_request_free (request, env);
    request = NULL;

    AXIS2_FREE (env->allocator, output_stream);
    output_stream = NULL;

    if (status_code < 0)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "status_code < 0");
        return AXIS2_FAILURE;
    }

    axis2_msg_ctx_set_status_code (msg_ctx, env, status_code);

    /* Start processing response */
    response = axis2_http_client_get_response (sender->client, env);
    if (!is_soap)
    {
        return axis2_http_sender_process_response (sender, env,
                                                   msg_ctx, response);
    }
    else if (AXIS2_HTTP_RESPONSE_OK_CODE_VAL == status_code ||
        AXIS2_HTTP_RESPONSE_ACK_CODE_VAL == status_code)
    {
        return axis2_http_sender_process_response (sender, env,
                                                   msg_ctx, response);
    }
    else if (AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_VAL == 
             status_code)
    {
        axis2_http_header_t *tmp_header = NULL;
        axis2_char_t *tmp_header_val = NULL;
        axis2_op_t *op = NULL;

        op = axis2_msg_ctx_get_op (msg_ctx, env);
        if (op)
        {
            const axis2_char_t *mep = axis2_op_get_msg_exchange_pattern (op, env);
            AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR, AXIS2_FAILURE);
            /* handle one way case */

            if (!axutil_strcmp (mep, AXIS2_MEP_URI_OUT_ONLY))
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "mep is AXIS2_MEP_URI_OUT_ONLY");
                return AXIS2_FAILURE;
            }
        }

        /* set an error to indicate error code status */
        tmp_header =
            axis2_http_simple_response_get_first_header (response,
                                                         env,
                                                         AXIS2_HTTP_HEADER_CONTENT_TYPE);
        if (tmp_header)
        {
            tmp_header_val = axis2_http_header_get_value (tmp_header, env);
        }

        if (tmp_header_val && (axutil_strstr (tmp_header_val,
                                              AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP)
                               || axutil_strstr (tmp_header_val,
                                                 AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML)))
        {
            return axis2_http_sender_process_response (sender,
                                                       env, msg_ctx, response);
        }
    }

    AXIS2_HANDLE_ERROR(env,
                       AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR, AXIS2_FAILURE);
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Exit:axis2_http_sender_send");
    return AXIS2_FAILURE;
}
#endif

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_set_chunked (axis2_http_sender_t * sender,
                               const axutil_env_t * env, axis2_bool_t chunked)
{
    sender->chunked = chunked;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_set_om_output (axis2_http_sender_t * sender,
                                 const axutil_env_t * env,
                                 axiom_output_t * om_output)
{
    sender->om_output = om_output;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_get_header_info (axis2_http_sender_t * sender,
                                   const axutil_env_t * env,
                                   axis2_msg_ctx_t * msg_ctx,
                                   axis2_http_simple_response_t * response)
{
    axutil_array_list_t *headers = NULL;
    axis2_char_t *charset = NULL;
    int i = 0;
    axis2_bool_t response_chunked = AXIS2_FALSE;
    int *content_length = NULL;
    axutil_property_t *property = NULL;
    axis2_char_t *content_type = NULL;
    int status_code = 0;

    AXIS2_PARAM_CHECK (env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, response, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, sender, AXIS2_FAILURE);

    headers = axis2_http_simple_response_get_headers (response, env);
    if (headers == NULL)
    {
        return AXIS2_SUCCESS;
    }

    for (i = 0; i < axutil_array_list_size (headers, env); i++)
    {
        axis2_http_header_t *header = axutil_array_list_get (headers, env, i);
        axis2_char_t *name = axis2_http_header_get_name ((axis2_http_header_t *)
                                                         header, env);
        if (name)
        {
            if (0 == axutil_strcasecmp (name, AXIS2_HTTP_HEADER_TRANSFER_ENCODING)
                && 0 ==
                axutil_strcasecmp (axis2_http_header_get_value (header, env),
                               AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
            {
                axis2_char_t *transfer_encoding = NULL;
                transfer_encoding =
                    axutil_strdup (env,
                                   AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED);
                response_chunked = AXIS2_TRUE;
                axis2_msg_ctx_set_transfer_encoding (msg_ctx,
                                                     env, transfer_encoding);

            }
            if (0 != axutil_strcasecmp (name, AXIS2_HTTP_HEADER_CONTENT_TYPE))
            {
                axis2_char_t *tmp_charset = NULL;
                axis2_char_t *content_type =
                    axis2_http_header_get_value (header,
                                                 env);
                tmp_charset = strstr (content_type,
                                      AXIS2_HTTP_CHAR_SET_ENCODING);
                if (charset)
                {
                    charset = axutil_strdup (env, tmp_charset);
                    break;
                }
            }
        }
    }

    content_type =
        (axis2_char_t *) axis2_http_simple_response_get_content_type (response,
                                                                      env);
    if (content_type)
    {
        if (strstr (content_type, AXIS2_HTTP_HEADER_ACCEPT_MULTIPART_RELATED)
            && strstr (content_type, AXIS2_HTTP_HEADER_ACCEPT_XOP_XML))
        {
            axis2_ctx_t *axis_ctx =
                axis2_op_ctx_get_base (axis2_msg_ctx_get_op_ctx (msg_ctx, env),
                                       env);
            property = axutil_property_create (env);
            axutil_property_set_scope (property, env, AXIS2_SCOPE_REQUEST);
            axutil_property_set_value (property,
                                       env, axutil_strdup (env, content_type));
            axis2_ctx_set_property (axis_ctx,
                                    env, MTOM_RECIVED_CONTENT_TYPE, property);
        }
    }

    if (charset)
    {
        axis2_ctx_t *axis_ctx =
            axis2_op_ctx_get_base (axis2_msg_ctx_get_op_ctx (msg_ctx, env), env);
        if (axis_ctx)
        {
            property = axutil_property_create (env);
            axutil_property_set_scope (property, env, AXIS2_SCOPE_REQUEST);
            axutil_property_set_value (property, env, charset);
            axis2_ctx_set_property (axis_ctx, env, AXIS2_CHARACTER_SET_ENCODING,
                                    property);
        }
    }


    if (AXIS2_FALSE == response_chunked)
    {
        int tmp_len = 0;
        content_length = AXIS2_MALLOC (env->allocator, sizeof (int));
        if (!content_length)
        {
            return AXIS2_FAILURE;
        }
        tmp_len = axis2_http_simple_response_get_content_length (response, env);
        memcpy (content_length, &tmp_len, sizeof (int));
        property = axutil_property_create (env);
        axutil_property_set_scope (property, env, AXIS2_SCOPE_REQUEST);
        axutil_property_set_value (property, env, content_length);
        axis2_msg_ctx_set_property (msg_ctx, env,
                                    AXIS2_HTTP_HEADER_CONTENT_LENGTH, property);
    }

    status_code = axis2_http_simple_response_get_status_code (response, env);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_process_response (axis2_http_sender_t * sender,
                                    const axutil_env_t * env,
                                    axis2_msg_ctx_t * msg_ctx,
                                    axis2_http_simple_response_t * response)
{
    axutil_stream_t *in_stream = NULL;
    axutil_property_t *property = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_http_sender_process_response");
    AXIS2_PARAM_CHECK (env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, response, AXIS2_FAILURE);

    in_stream = axis2_http_simple_response_get_body (response, env);
    if (!in_stream)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NULL_STREAM_IN_RESPONSE_BODY,
                           AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    axis2_http_sender_get_header_info (sender, env, msg_ctx, response);
    axis2_msg_ctx_set_http_output_headers(msg_ctx, env,
        axis2_http_simple_response_extract_headers(response, env));
    property = axutil_property_create (env);
    axutil_property_set_scope (property, env, AXIS2_SCOPE_REQUEST);
    axutil_property_set_free_func (property, env, axutil_stream_free_void_arg);
    axutil_property_set_value (property, env, in_stream);
    axis2_msg_ctx_set_property (msg_ctx, env, AXIS2_TRANSPORT_IN, property);
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_http_sender_process_response");
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_get_timeout_values (axis2_http_sender_t * sender,
                                      const axutil_env_t * env,
                                      axis2_msg_ctx_t * msg_ctx)
{
    axis2_char_t *so_str = NULL;
    axis2_char_t *connection_str = NULL;
    axutil_param_t *tmp_param = NULL;
    axutil_property_t *property = NULL;

    AXIS2_PARAM_CHECK (env->error, sender, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, msg_ctx, AXIS2_FAILURE);
    /* check if timeout has been set by user using options 
     * with axis2_options_set_timeout_in_milli_seconds
     */
    property =
        axis2_msg_ctx_get_property(msg_ctx, env, AXIS2_HTTP_CONNECTION_TIMEOUT);
    if (property)
    {
        axis2_char_t *value = axutil_property_get_value(property, env);
        if (value) 
        {
            sender->so_timeout = AXIS2_ATOI(value);
            return AXIS2_SUCCESS;
        }
    }

    tmp_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                             env, AXIS2_HTTP_SO_TIMEOUT);

    if (tmp_param)
    {
        so_str = (axis2_char_t *) axutil_param_get_value (tmp_param, env);
        if (so_str)
        {
            sender->so_timeout = AXIS2_ATOI (so_str);            
            return AXIS2_SUCCESS;
        }
    }

    tmp_param = axis2_msg_ctx_get_parameter (msg_ctx, env,
                                             AXIS2_HTTP_CONNECTION_TIMEOUT);
    if (tmp_param)
    {
        connection_str =
            (axis2_char_t *) axutil_param_get_value (tmp_param, env);
        if (connection_str)
        {
            sender->so_timeout = AXIS2_ATOI (connection_str);
            return AXIS2_SUCCESS;
        }
    }
    return AXIS2_FAILURE;
}

	AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_set_http_version (axis2_http_sender_t * sender,
                                    const axutil_env_t * env,
                                    axis2_char_t * version)
{
    sender->http_version = axutil_strdup (env, version);
    if (!sender->http_version)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

#ifndef AXIS2_LIBCURL_ENABLED
static void
axis2_http_sender_add_header_list (axis2_http_simple_request_t * request,
                                   const axutil_env_t * env,
                                   axutil_array_list_t * array_list)
{
    int ii = 0;
    int kk = 0;
    axis2_http_header_t *http_header = NULL;
    ii = axutil_array_list_size (array_list, env);
    for (; kk < ii; kk++)
    {
        http_header =
            (axis2_http_header_t *) axutil_array_list_get (array_list, env, kk);
        axis2_http_simple_request_add_header (request, env, http_header);
    }
}

static axis2_status_t
axis2_http_sender_configure_proxy (axis2_http_sender_t * sender,
                                   const axutil_env_t * env,
                                   axis2_msg_ctx_t * msg_ctx)
{
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    axis2_transport_out_desc_t *trans_desc = NULL;
    axutil_param_t *proxy_param = NULL;
    axutil_hash_t *transport_attrs = NULL;
    axis2_char_t *proxy_host = NULL;
    axis2_char_t *proxy_port = NULL;

    AXIS2_PARAM_CHECK (env->error, msg_ctx, AXIS2_FAILURE);

    conf_ctx = axis2_msg_ctx_get_conf_ctx (msg_ctx, env);
    if (!conf_ctx)
    {
        return AXIS2_FAILURE;
    }
    conf = axis2_conf_ctx_get_conf (conf_ctx, env);
    if (!conf)
    {
        return AXIS2_FAILURE;
    }

    trans_desc = axis2_conf_get_transport_out (conf,
                                               env, AXIS2_TRANSPORT_ENUM_HTTP);
    if (!trans_desc)
    {
        return AXIS2_FAILURE;
    }

    proxy_param =
        axutil_param_container_get_param
        (axis2_transport_out_desc_param_container (trans_desc, env), env,
         AXIS2_HTTP_PROXY_API);
    if (!proxy_param)
    {
        proxy_param =
            axutil_param_container_get_param
            (axis2_transport_out_desc_param_container (trans_desc, env), env,
             AXIS2_HTTP_PROXY);
    }
    if (proxy_param)
    {
        transport_attrs = axutil_param_get_attributes (proxy_param, env);
        if (transport_attrs)
        {
            axutil_generic_obj_t *obj = NULL;
            axiom_attribute_t *host_attr = NULL;
            axiom_attribute_t *port_attr = NULL;

            obj = axutil_hash_get (transport_attrs, AXIS2_HTTP_PROXY_HOST,
                                   AXIS2_HASH_KEY_STRING);
            if (!obj)
            {
                return AXIS2_FAILURE;
            }
            host_attr = (axiom_attribute_t *) axutil_generic_obj_get_value (obj,
                                                                            env);
            if (!host_attr)
            {
                return AXIS2_FAILURE;
            }
            proxy_host = axiom_attribute_get_value (host_attr, env);
            if (!proxy_host)
            {
                return AXIS2_FAILURE;
            }
            /* Now we get the port */
            obj = NULL;

            obj = axutil_hash_get (transport_attrs, AXIS2_HTTP_PROXY_PORT,
                                   AXIS2_HASH_KEY_STRING);
            port_attr = (axiom_attribute_t *) axutil_generic_obj_get_value (obj,
                                                                            env);
            if (!port_attr)
            {
                return AXIS2_FAILURE;
            }
            proxy_port = axiom_attribute_get_value (port_attr, env);
            if (!proxy_port)
            {
                return AXIS2_FAILURE;
            }
        }
    }
    if (proxy_port && proxy_host)
    {
        axis2_http_client_set_proxy (sender->client, env, proxy_host,
                                     AXIS2_ATOI (proxy_port));
    }
    return AXIS2_SUCCESS;
}
#endif

#ifndef AXIS2_LIBCURL_ENABLED
static axis2_status_t
axis2_http_sender_configure_server_cert (axis2_http_sender_t * sender,
                                         const axutil_env_t * env,
                                         axis2_msg_ctx_t * msg_ctx)
{
    axutil_property_t *server_cert_property = NULL;
    axutil_param_t *server_cert_param = NULL;
    axis2_char_t *server_cert = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_PARAM_CHECK (env->error, msg_ctx, AXIS2_FAILURE);

    server_cert_property = axis2_msg_ctx_get_property (msg_ctx, env,
                                                       AXIS2_SSL_SERVER_CERT);
    if (server_cert_property)
    {
        server_cert =
            (axis2_char_t *) axutil_property_get_value (server_cert_property,
                                                        env);
    }
    else
    {
        server_cert_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                         env,
                                                         AXIS2_SSL_SERVER_CERT);
        if (server_cert_param)
        {
            server_cert =
                (axis2_char_t *) axutil_param_get_value (server_cert_param, env);
        }
    }

    if (server_cert)
    {
        status = axis2_http_client_set_server_cert (sender->client,
                                                    env, server_cert);
    }

    return status;
}
#endif

#ifndef AXIS2_LIBCURL_ENABLED
static axis2_status_t
axis2_http_sender_configure_key_file (axis2_http_sender_t * sender,
                                      const axutil_env_t * env,
                                      axis2_msg_ctx_t * msg_ctx)
{
    axutil_property_t *key_file_property = NULL;
    axutil_param_t *key_file_param = NULL;
    axis2_char_t *key_file = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_PARAM_CHECK (env->error, msg_ctx, AXIS2_FAILURE);

    key_file_property = axis2_msg_ctx_get_property (msg_ctx, env,
                                                    AXIS2_SSL_KEY_FILE);
    if (key_file_property)
    {
        key_file =
            (axis2_char_t *) axutil_property_get_value (key_file_property, env);
    }
    else
    {
        key_file_param = axis2_msg_ctx_get_parameter (msg_ctx, env,
                                                      AXIS2_SSL_KEY_FILE);
        if (key_file_param)
        {
            key_file =
                (axis2_char_t *) axutil_param_get_value (key_file_param, env);
        }
    }

    if (key_file)
    {
        status = axis2_http_client_set_key_file (sender->client, env, key_file);
    }

    return status;
}
#endif

#ifndef AXIS2_LIBCURL_ENABLED
static axis2_status_t
axis2_http_sender_configure_http_basic_auth (axis2_http_sender_t * sender,
                                             const axutil_env_t * env,
                                             axis2_msg_ctx_t * msg_ctx,
                                             axis2_http_simple_request_t *
                                             request)
{
    axutil_property_t *http_auth_un = NULL;
    axutil_property_t *http_auth_pw = NULL;
    axis2_char_t *uname = NULL;
    axis2_char_t *passwd = NULL;

    http_auth_un = axis2_msg_ctx_get_property (msg_ctx, env,
                                               AXIS2_HTTP_AUTH_UNAME);
    http_auth_pw = axis2_msg_ctx_get_property (msg_ctx, env,
                                               AXIS2_HTTP_AUTH_PASSWD);
    if (http_auth_un && http_auth_pw)
    {
        uname = (axis2_char_t *) axutil_property_get_value (http_auth_un, env);
        passwd = (axis2_char_t *) axutil_property_get_value (http_auth_pw, env);
    }
    if (!uname || !passwd)
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        axis2_conf_t *conf = NULL;
        axis2_transport_out_desc_t *trans_desc = NULL;
        axutil_param_t *http_auth_param = NULL;
        axutil_hash_t *transport_attrs = NULL;

        conf_ctx = axis2_msg_ctx_get_conf_ctx (msg_ctx, env);
        if (conf_ctx)
        {
            conf = axis2_conf_ctx_get_conf (conf_ctx, env);
            if (conf)
            {
                trans_desc = axis2_conf_get_transport_out (conf,
                                                           env, AXIS2_TRANSPORT_ENUM_HTTP);
            }
        }
        if (trans_desc)
        {
            http_auth_param =
                axutil_param_container_get_param
                (axis2_transport_out_desc_param_container (trans_desc, env), env,
                AXIS2_HTTP_AUTHENTICATION);
            if (http_auth_param)
            {
                transport_attrs = axutil_param_get_attributes (http_auth_param, env);
                if (transport_attrs)
                {
                    axutil_generic_obj_t *obj = NULL;
                    axiom_attribute_t *username_attr = NULL;
                    axiom_attribute_t *password_attr = NULL;

                    obj = axutil_hash_get (transport_attrs, 
                                           AXIS2_HTTP_AUTHENTICATION_USERNAME,
                                           AXIS2_HASH_KEY_STRING);
                    if (obj)
                    {
                        username_attr = 
                            (axiom_attribute_t *) axutil_generic_obj_get_value 
                            (obj, env);
                    }
                    if (username_attr)
                    {
                        uname = axiom_attribute_get_value (username_attr, env);
                    }
                    obj = NULL;

                    obj = axutil_hash_get (transport_attrs, 
                                           AXIS2_HTTP_AUTHENTICATION_PASSWORD,
                                           AXIS2_HASH_KEY_STRING);
                    if (obj)
                    {
                        password_attr = 
                            (axiom_attribute_t *) axutil_generic_obj_get_value 
                            (obj, env);
                    }
                    if (password_attr)
                    {
                        passwd = axiom_attribute_get_value (password_attr, env);
                    }
                }
            }
        }
    }
    if (uname && passwd)
    {
        int elen;
        int plen = axutil_strlen (uname) + axutil_strlen (passwd) + 1;
        axis2_char_t *to_encode =
            (axis2_char_t *) (AXIS2_MALLOC (env->allocator, 
                                            sizeof (axis2_char_t) * plen + 1));
        axis2_char_t *encoded = NULL;
        axis2_char_t *auth_str = NULL;
        sprintf (to_encode, "%s:%s", uname, passwd);
        elen = axutil_base64_encode_len (plen);
        encoded = (axis2_char_t *) (AXIS2_MALLOC (env->allocator,
                                                  sizeof (axis2_char_t) *
                                                  elen));
        auth_str =
            (axis2_char_t
             *) (AXIS2_MALLOC (env->allocator,
                               sizeof (axis2_char_t) * (elen + 6)));
        axutil_base64_encode (encoded, to_encode, plen);
        sprintf (auth_str, "%s %s", AXIS2_HTTP_AUTH_TYPE_BASIC, encoded);
        axis2_http_sender_util_add_header (env, request,
                                           AXIS2_HTTP_HEADER_AUTHORIZATION,
                                           auth_str);

        AXIS2_FREE (env->allocator, to_encode);
        to_encode = NULL;
        AXIS2_FREE (env->allocator, encoded);
        encoded = NULL;
        AXIS2_FREE (env->allocator, auth_str);
        auth_str = NULL;

        return AXIS2_SUCCESS;
    }

    return AXIS2_FAILURE;

}

static axis2_status_t
axis2_http_sender_configure_proxy_basic_auth (axis2_http_sender_t * sender,
                                              const axutil_env_t * env,
                                              axis2_msg_ctx_t * msg_ctx,
                                              axis2_http_simple_request_t *
                                              request)
{
    axutil_property_t *proxy_auth_un = NULL;
    axutil_property_t *proxy_auth_pw = NULL;
    axis2_char_t *uname = NULL;
    axis2_char_t *passwd = NULL;

    proxy_auth_un = axis2_msg_ctx_get_property (msg_ctx, env,
                                               AXIS2_PROXY_AUTH_UNAME);
    proxy_auth_pw = axis2_msg_ctx_get_property (msg_ctx, env,
                                               AXIS2_PROXY_AUTH_PASSWD);

    if (proxy_auth_un && proxy_auth_pw)
    {
        uname = (axis2_char_t *) axutil_property_get_value (proxy_auth_un, env);
        passwd = (axis2_char_t *) axutil_property_get_value (proxy_auth_pw, env);
    }
    if (!uname || !passwd)
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        axis2_conf_t *conf = NULL;
        axis2_transport_out_desc_t *trans_desc = NULL;
        axutil_param_t *proxy_param = NULL;
        axutil_hash_t *transport_attrs = NULL;

        conf_ctx = axis2_msg_ctx_get_conf_ctx (msg_ctx, env);
        if (conf_ctx)
        {
            conf = axis2_conf_ctx_get_conf (conf_ctx, env);
            if (conf)
            {
                trans_desc = 
                    axis2_conf_get_transport_out (conf,
                                                  env, 
                                                  AXIS2_TRANSPORT_ENUM_HTTP);
            }
        }
        if (trans_desc)
        {
            proxy_param =
                axutil_param_container_get_param
                (axis2_transport_out_desc_param_container (trans_desc, env), env,
                AXIS2_HTTP_PROXY_API);
            if (!proxy_param)
            {
                proxy_param =
                    axutil_param_container_get_param
                    (axis2_transport_out_desc_param_container (trans_desc, env), env,
                    AXIS2_HTTP_PROXY);
            } 
            if (proxy_param)
            {
                transport_attrs = axutil_param_get_attributes (proxy_param, env);
                if (transport_attrs)
                {
                    axutil_generic_obj_t *obj = NULL;
                    axiom_attribute_t *username_attr = NULL;
                    axiom_attribute_t *password_attr = NULL;

                    obj = axutil_hash_get (transport_attrs, 
                                           AXIS2_HTTP_PROXY_USERNAME,
                                           AXIS2_HASH_KEY_STRING);
                    if (obj)
                    {
                        username_attr = (axiom_attribute_t *) 
                            axutil_generic_obj_get_value (obj,
                                                          env);
                    }
                    if (username_attr)
                    {
                        uname = axiom_attribute_get_value (username_attr, env);
                    }
                    obj = NULL;

                    obj = axutil_hash_get (transport_attrs, 
                                           AXIS2_HTTP_PROXY_PASSWORD,
                                           AXIS2_HASH_KEY_STRING);
                    if (obj)
                    {
                        password_attr = (axiom_attribute_t *) 
                            axutil_generic_obj_get_value (obj,
                                                          env);
                    }
                    if (password_attr)
                    {
                        passwd = axiom_attribute_get_value (password_attr, env);
                    }
                }
            }
        }
    }
    if (uname && passwd)
    {
        int elen;
        int plen = axutil_strlen (uname) + axutil_strlen (passwd) + 1;
        axis2_char_t *to_encode =
            (axis2_char_t
             *) (AXIS2_MALLOC (env->allocator, 
                               sizeof (axis2_char_t) * plen + 1));
        axis2_char_t *encoded = NULL;
        axis2_char_t *auth_str = NULL;
        sprintf (to_encode, "%s:%s", uname, passwd);
        elen = axutil_base64_encode_len (plen);
        encoded = (axis2_char_t *) (AXIS2_MALLOC (env->allocator,
                                                  sizeof (axis2_char_t) *
                                                  elen));
        auth_str =
            (axis2_char_t
             *) (AXIS2_MALLOC (env->allocator,
                               sizeof (axis2_char_t) * (elen + 6)));
        axutil_base64_encode (encoded, to_encode, plen);
        sprintf (auth_str, "%s %s", AXIS2_PROXY_AUTH_TYPE_BASIC, encoded);
        axis2_http_sender_util_add_header (env, request,
                                           AXIS2_HTTP_HEADER_PROXY_AUTHORIZATION,
                                           auth_str);

        AXIS2_FREE (env->allocator, to_encode);
        to_encode = NULL;
        AXIS2_FREE (env->allocator, encoded);
        encoded = NULL;
        AXIS2_FREE (env->allocator, auth_str);
        auth_str = NULL;
 
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}
#endif

#ifndef AXIS2_LIBCURL_ENABLED
static axis2_status_t
axis2_http_sender_configure_http_digest_auth (axis2_http_sender_t * sender,
                                              const axutil_env_t * env,
                                              axis2_msg_ctx_t * msg_ctx,
                                              axis2_http_simple_request_t * request,
                                              axis2_char_t * header_data)
{
    axutil_property_t *http_auth_un = NULL;
    axutil_property_t *http_auth_pw = NULL;
    axis2_char_t *uname = NULL;
    axis2_char_t *passwd = NULL;

    if (!header_data || !*header_data)
        return AXIS2_FAILURE;

    http_auth_un = axis2_msg_ctx_get_property (msg_ctx, env,
                                               AXIS2_HTTP_AUTH_UNAME);
    http_auth_pw = axis2_msg_ctx_get_property (msg_ctx, env,
                                               AXIS2_HTTP_AUTH_PASSWD);
    if (http_auth_un && http_auth_pw)
    {
        uname = (axis2_char_t *) axutil_property_get_value (http_auth_un, env);
        passwd = (axis2_char_t *) axutil_property_get_value (http_auth_pw, env);
    }
    if (!uname || !passwd)
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        axis2_conf_t *conf = NULL;
        axis2_transport_out_desc_t *trans_desc = NULL;
        axutil_param_t *http_auth_param = NULL;
        axutil_hash_t *transport_attrs = NULL;

        conf_ctx = axis2_msg_ctx_get_conf_ctx (msg_ctx, env);
        if (conf_ctx)
        {
            conf = axis2_conf_ctx_get_conf (conf_ctx, env);
            if (conf)
            {
                trans_desc = axis2_conf_get_transport_out (conf,
                                                           env, 
                                                           AXIS2_TRANSPORT_ENUM_HTTP);
            }
        }
        if (trans_desc)
        {
            http_auth_param =
                axutil_param_container_get_param
                (axis2_transport_out_desc_param_container (trans_desc, env), env,
                AXIS2_HTTP_AUTHENTICATION);
            if (http_auth_param)
            {
                transport_attrs = axutil_param_get_attributes (http_auth_param, env);
                if (transport_attrs)
                {
                    axutil_generic_obj_t *obj = NULL;
                    axiom_attribute_t *username_attr = NULL;
                    axiom_attribute_t *password_attr = NULL;

                    obj = axutil_hash_get (transport_attrs, 
                                           AXIS2_HTTP_AUTHENTICATION_USERNAME,
                                           AXIS2_HASH_KEY_STRING);
                    if (obj)
                    {
                        username_attr = (axiom_attribute_t *) 
                            axutil_generic_obj_get_value (obj,
                                                          env);
                    }
                    if (username_attr)
                    {
                        uname = axiom_attribute_get_value (username_attr, env);
                    }
                    obj = NULL;

                    obj = axutil_hash_get (transport_attrs, 
                                           AXIS2_HTTP_AUTHENTICATION_PASSWORD,
                                           AXIS2_HASH_KEY_STRING);
                    if (obj)
                    {
                        password_attr = (axiom_attribute_t *) 
                            axutil_generic_obj_get_value (obj,
                                                          env);
                    }
                    if (password_attr)
                    {
                        passwd = axiom_attribute_get_value (password_attr, env);
                    }
                }
            }
        }
    }
    if (uname && passwd)
    {
        int elen = 0; /* length of header content */
        int print_const = 5; /* constant accounts for printing the
                                quoatation marks, comma, and space */
        int response_length = 32;
        axis2_char_t *temp = NULL;
        axis2_char_t *alloc_temp = NULL;
        axis2_char_t *algo = AXIS2_HTTP_AUTHORIZATION_REQUEST_ALGORITHM_MD5;
        axis2_char_t *realm = NULL;
        axis2_char_t *qop = NULL;
        axis2_char_t *nonce = NULL;
        axis2_char_t *opaque = NULL;
        axis2_char_t *cnonce = NULL;
        axis2_char_t *nc = NULL;
        axutil_digest_hash_hex_t h_a1;
        axutil_digest_hash_hex_t h_a2;
        axutil_digest_hash_hex_t response;
        axis2_char_t *auth_str = NULL;
        axutil_property_t *method = NULL;
        axis2_char_t *method_value = NULL;
        axis2_char_t *url = NULL;

        url = axis2_http_request_line_get_uri(
                       axis2_http_simple_request_get_request_line(request, env),
                       env);

        if (!url)
        {
            return AXIS2_FAILURE;
        }

        elen += print_const + 
            axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_URI) + 
            axutil_strlen(url);

        elen += print_const + 
            axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_USERNAME) +
            axutil_strlen(uname);

        method = (axutil_property_t *) axis2_msg_ctx_get_property (msg_ctx, 
                                                                   env,
                                                                   AXIS2_HTTP_METHOD);
        if (method)
        {
            method_value = (axis2_char_t *) axutil_property_get_value (method,
                                                                       env);
        }
        else
        {
            method_value = AXIS2_HTTP_POST;
        }

        temp = axutil_strstr(header_data, 
                             AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_REALM);
        if (temp)
        {
            realm = axutil_strchr(temp, AXIS2_ESC_DOUBLE_QUOTE);
            if (realm)
            {
                realm++;
                temp = axutil_strchr(realm, AXIS2_ESC_DOUBLE_QUOTE);
                alloc_temp = (axis2_char_t
                              *) (AXIS2_MALLOC (env->allocator,
                              sizeof (axis2_char_t) * (temp-realm + 1)));
                strncpy(alloc_temp, realm, (temp-realm));
                if (alloc_temp)
                    alloc_temp[temp-realm] = AXIS2_ESC_NULL;
                realm = alloc_temp;
                alloc_temp = NULL;
                elen += print_const + 
                    axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_REALM)+
                    axutil_strlen(realm);
            }
            else
            {
                return AXIS2_FAILURE;
            }
        }

        temp = axutil_strstr(header_data, 
                             AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_QOP);
        if (temp)
        {
            qop = axutil_strchr(temp, AXIS2_ESC_DOUBLE_QUOTE);
            if (qop)
            {
                qop++;
                temp = axutil_strchr(qop, AXIS2_ESC_DOUBLE_QUOTE);
                alloc_temp = (axis2_char_t
                              *) (AXIS2_MALLOC (env->allocator,
                              sizeof (axis2_char_t) * (temp-qop + 1)));
                strncpy(alloc_temp, qop, (temp-qop));
                if (alloc_temp)
                    alloc_temp[temp-qop] = AXIS2_ESC_NULL;
                qop = alloc_temp;
                alloc_temp = NULL;
            }
        }

        temp = axutil_strstr(header_data, 
                             AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE);
        if (temp)
        {
            nonce = axutil_strchr(temp, AXIS2_ESC_DOUBLE_QUOTE);
            if (nonce)
            {
                nonce++;
                temp = axutil_strchr(nonce, AXIS2_ESC_DOUBLE_QUOTE);
                alloc_temp = (axis2_char_t
                              *) (AXIS2_MALLOC (env->allocator,
                              sizeof (axis2_char_t) * (temp-nonce + 1)));
                strncpy(alloc_temp, nonce, (temp-nonce));
                if (alloc_temp)
                    alloc_temp[temp-nonce] = AXIS2_ESC_NULL;
                nonce = alloc_temp;
                alloc_temp = NULL;

                elen += print_const + 
                    axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE)+
                    axutil_strlen(nonce);
            }
            else
            {
                if (realm)
                    AXIS2_FREE (env->allocator, realm);
                return AXIS2_FAILURE;
            }
        }

        temp = axutil_strstr(header_data, 
                             AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_OPAQUE);
        if (temp)
        {
            opaque = axutil_strchr(temp, AXIS2_ESC_DOUBLE_QUOTE);
            if (opaque)
            {
                opaque++;
                temp = axutil_strchr(opaque, AXIS2_ESC_DOUBLE_QUOTE);
                alloc_temp = (axis2_char_t
                              *) (AXIS2_MALLOC (env->allocator,
                              sizeof (axis2_char_t) * (temp-opaque + 1)));
                strncpy(alloc_temp, opaque, (temp-opaque));
                if (alloc_temp)
                    alloc_temp[temp-opaque] = AXIS2_ESC_NULL;
                opaque = alloc_temp;
                alloc_temp = NULL;
                elen += print_const + 
                    axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_OPAQUE)
                    + axutil_strlen(opaque);
            }
            else
            {
                if (realm)
                    AXIS2_FREE (env->allocator, realm);
                if (nonce)
                    AXIS2_FREE (env->allocator, nonce);
                return AXIS2_FAILURE;
            }
        }

        if (qop)
        {
            nc = AXIS2_HTTP_AUTHORIZATION_REQUEST_DEFAULT_CLIENT_NONCE;
            temp = qop;
            if (!axutil_strstr(temp, 
                               AXIS2_HTTP_AUTHORIZATION_REQUEST_QOP_OPTION_AUTH))
            {
                return AXIS2_FAILURE;
            }
            AXIS2_FREE (env->allocator, qop);
            qop = AXIS2_HTTP_AUTHORIZATION_REQUEST_QOP_OPTION_AUTH;
            temp = axutil_uuid_gen(env);
            cnonce = temp;
            temp += CLIENT_NONCE_LENGTH;
            if (temp)
                *temp = AXIS2_ESC_NULL;
            elen += 11 
                + axutil_strlen(
                    AXIS2_HTTP_AUTHORIZATION_REQUEST_DEFAULT_CLIENT_NONCE)
                + axutil_strlen(
                    AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE_COUNT)
                + axutil_strlen(
                    AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_CLIENT_NONCE)
                + axutil_strlen(
                    AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_QOP)
                + CLIENT_NONCE_LENGTH + axutil_strlen(qop);
        }
        
        axutil_digest_calc_get_h_a1(env, algo, uname,
                                    realm, passwd, cnonce,
                                    nonce, h_a1);
        axutil_digest_calc_get_response(env, h_a1, nonce, nc,
                                        cnonce, qop, method_value,
                                        url, h_a2, response);

        elen += 4 
            + axutil_strlen(
                AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_RESPONSE)
            + axutil_strlen(AXIS2_HTTP_AUTH_TYPE_DIGEST) + response_length;

        auth_str =
            (axis2_char_t
             *) (AXIS2_MALLOC (env->allocator,
                               sizeof (axis2_char_t) * (elen + 1)));
        temp = auth_str;
        sprintf (temp, "%s %s=\"%s\", ",
                 AXIS2_HTTP_AUTH_TYPE_DIGEST,
                 AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_USERNAME, uname);
        temp += ((int)strlen(AXIS2_HTTP_AUTH_TYPE_DIGEST) +
                 (int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_USERNAME) +
                 (int)strlen(uname) + 6);
        if (realm)
        {
            sprintf (temp, "%s=\"%s\", ",
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_REALM, realm);
            temp += ((int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_REALM) +
                     (int)strlen(realm) + 5);
        }
        if (nonce)
        {
            sprintf (temp, "%s=\"%s\", ",
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE, nonce);
            temp += ((int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE) +
                     (int)strlen(nonce) + 5);
        }
        sprintf (temp, "%s=\"%s\", ",  
                 AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_URI, url);
        temp += ((int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_URI) +
                 (int)strlen(url) + 5);
        if (qop)
        {
            sprintf (temp, "%s=%s, %s=%s, %s=\"%s\", ",
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_QOP, qop,
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE_COUNT, nc,
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_CLIENT_NONCE,
                     cnonce);
            temp += ((int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_QOP) +
                     (int)strlen(qop) +
                     (int)strlen(
                         AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE_COUNT) +
                     (int)strlen(nc) +
                     (int)strlen(
                         AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_CLIENT_NONCE) +
                     (int)strlen(cnonce) + 11);
        }
        if (opaque)
        {
            sprintf (temp, "%s=\"%s\", ",
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_OPAQUE, opaque);
            temp += ((int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_OPAQUE)+
                     (int)strlen(opaque) + 5);
        }
        sprintf (temp, "%s=\"%s\"",
                 AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_RESPONSE, response);

        axis2_http_sender_util_add_header (env, request,
                                           AXIS2_HTTP_HEADER_AUTHORIZATION,
                                           auth_str);
        if (realm)
            AXIS2_FREE (env->allocator, realm);
        if (nonce)
            AXIS2_FREE (env->allocator, nonce);
        if (cnonce)
            AXIS2_FREE (env->allocator, cnonce);
        if (opaque)
            AXIS2_FREE (env->allocator, opaque);
        if (auth_str)
            AXIS2_FREE (env->allocator, auth_str);
        auth_str = NULL;
 
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

static axis2_status_t
axis2_http_sender_configure_proxy_digest_auth (axis2_http_sender_t * sender,
                                               const axutil_env_t * env,
                                               axis2_msg_ctx_t * msg_ctx,
                                               axis2_http_simple_request_t * request,
                                               axis2_char_t * header_data)
{
    axutil_property_t *proxy_auth_un = NULL;
    axutil_property_t *proxy_auth_pw = NULL;
    axis2_char_t *uname = NULL;
    axis2_char_t *passwd = NULL;

    if (!header_data || !*header_data)
        return AXIS2_FAILURE;

    proxy_auth_un = axis2_msg_ctx_get_property (msg_ctx, env,
                                               AXIS2_PROXY_AUTH_UNAME);
    proxy_auth_pw = axis2_msg_ctx_get_property (msg_ctx, env,
                                               AXIS2_PROXY_AUTH_PASSWD);

    if (proxy_auth_un && proxy_auth_pw)
    {
        uname = (axis2_char_t *) axutil_property_get_value (proxy_auth_un, env);
        passwd = (axis2_char_t *) axutil_property_get_value (proxy_auth_pw, env);
    }
    if (!uname || !passwd)
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        axis2_conf_t *conf = NULL;
        axis2_transport_out_desc_t *trans_desc = NULL;
        axutil_param_t *proxy_param = NULL;
        axutil_hash_t *transport_attrs = NULL;

        conf_ctx = axis2_msg_ctx_get_conf_ctx (msg_ctx, env);
        if (conf_ctx)
        {
            conf = axis2_conf_ctx_get_conf (conf_ctx, env);
            if (conf)
            {
                trans_desc = axis2_conf_get_transport_out (conf,
                                                           env, 
                                                           AXIS2_TRANSPORT_ENUM_HTTP);
            }
        }
        if (trans_desc)
        {
            proxy_param =
                axutil_param_container_get_param
                (axis2_transport_out_desc_param_container (trans_desc, env), 
                 env,
                 AXIS2_HTTP_PROXY_API);
            if (!proxy_param)
            {
                proxy_param =
                    axutil_param_container_get_param
                    (axis2_transport_out_desc_param_container (trans_desc, env), 
                     env,
                     AXIS2_HTTP_PROXY);
            } 
            if (proxy_param)
            {
                transport_attrs = axutil_param_get_attributes (proxy_param, env);

                if (transport_attrs)
                {
                    axutil_generic_obj_t *obj = NULL;
                    axiom_attribute_t *username_attr = NULL;
                    axiom_attribute_t *password_attr = NULL;

                    obj = axutil_hash_get (transport_attrs, 
                                           AXIS2_HTTP_PROXY_USERNAME,
                                           AXIS2_HASH_KEY_STRING);
                    if (obj)
                    {
                        username_attr = (axiom_attribute_t *) 
                            axutil_generic_obj_get_value (obj,
                                                          env);
                    }
                    if (username_attr)
                    {
                        uname = axiom_attribute_get_value (username_attr, env);
                    }

                    obj = NULL;
                    obj = axutil_hash_get (transport_attrs, 
                                           AXIS2_HTTP_PROXY_PASSWORD,
                                           AXIS2_HASH_KEY_STRING);
                    if (obj)
                    {
                        password_attr = (axiom_attribute_t *) 
                            axutil_generic_obj_get_value (obj,
                                                          env);
                    }
                    if (password_attr)
                    {
                        passwd = axiom_attribute_get_value (password_attr, env);
                    }
                }
            }
        }
    }
    if (uname && passwd)
    {
        int elen = 0; /* length of header content */
        int print_const = 5; /* constant accounts for printing the
                                quoatation marks, comma, and space */
        int response_length = 32;
        axis2_char_t *temp = NULL;
        axis2_char_t *alloc_temp = NULL;
        axis2_char_t *algo = AXIS2_HTTP_AUTHORIZATION_REQUEST_ALGORITHM_MD5;
        axis2_char_t *realm = NULL;
        axis2_char_t *qop = NULL;
        axis2_char_t *nonce = NULL;
        axis2_char_t *opaque = NULL;
        axis2_char_t *cnonce = NULL;
        axis2_char_t *nc = NULL;
        axutil_digest_hash_hex_t h_a1;
        axutil_digest_hash_hex_t h_a2;
        axutil_digest_hash_hex_t response;
        axis2_char_t *auth_str = NULL;
        axutil_property_t *method = NULL;
        axis2_char_t *method_value = NULL;
        axis2_char_t *url = NULL;

        url = axis2_http_request_line_get_uri(
                       axis2_http_simple_request_get_request_line(request, env), env);

        if (!url)
        {
            return AXIS2_FAILURE;
        }

        elen += print_const + 
            axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_URI) 
            + axutil_strlen(url);

        elen += print_const + 
            axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_USERNAME) 
            + axutil_strlen(uname);

        method = (axutil_property_t *) 
            axis2_msg_ctx_get_property (msg_ctx, env,
                                        AXIS2_HTTP_METHOD);
        if (method)
        {
            method_value = (axis2_char_t *) axutil_property_get_value (method,
                                                                       env);
        }
        else
        {
            method_value = AXIS2_HTTP_POST;
        }

        temp = axutil_strstr(header_data, 
                             AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_REALM);
        if (temp)
        {
            realm = axutil_strchr(temp, AXIS2_ESC_DOUBLE_QUOTE);
            if (realm)
            {
                realm++;
                temp = axutil_strchr(realm, AXIS2_ESC_DOUBLE_QUOTE);
                alloc_temp = (axis2_char_t
                              *) (AXIS2_MALLOC (env->allocator,
                              sizeof (axis2_char_t) * (temp-realm + 1)));
                strncpy(alloc_temp, realm, (temp-realm));
                if (alloc_temp)
                    alloc_temp[temp-realm] = AXIS2_ESC_NULL;
                realm = alloc_temp;
                alloc_temp = NULL;
                elen += print_const + 
                    axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_REALM) 
                    + axutil_strlen(realm);
            }
            else
            {
                return AXIS2_FAILURE;
            }
        }

        temp = axutil_strstr(header_data, 
                             AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_QOP);
        if (temp)
        {
            qop = axutil_strchr(temp, AXIS2_ESC_DOUBLE_QUOTE);
            if (qop)
            {
                qop++;
                temp = axutil_strchr(qop, AXIS2_ESC_DOUBLE_QUOTE);
                alloc_temp = (axis2_char_t
                              *) (AXIS2_MALLOC (env->allocator,
                              sizeof (axis2_char_t) * (temp-qop + 1)));
                strncpy(alloc_temp, qop, (temp-qop));
                if (alloc_temp)
                    alloc_temp[temp-qop] = AXIS2_ESC_NULL;
                qop = alloc_temp;
                alloc_temp = NULL;
            }
        }

        temp = axutil_strstr(header_data, AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE);
        if (temp)
        {
            nonce = axutil_strchr(temp, AXIS2_ESC_DOUBLE_QUOTE);
            if (nonce)
            {
                nonce++;
                temp = axutil_strchr(nonce, AXIS2_ESC_DOUBLE_QUOTE);
                alloc_temp = (axis2_char_t
                              *) (AXIS2_MALLOC (env->allocator,
                              sizeof (axis2_char_t) * (temp-nonce + 1)));
                strncpy(alloc_temp, nonce, (temp-nonce));
                if (alloc_temp)
                    alloc_temp[temp-nonce] = AXIS2_ESC_NULL;
                nonce = alloc_temp;
                alloc_temp = NULL;

                elen += print_const + 
                    axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE)
                    + axutil_strlen(nonce);
            }
            else
            {
                if (realm)
                    AXIS2_FREE (env->allocator, realm);
                return AXIS2_FAILURE;
            }
        }

        temp = axutil_strstr(header_data, 
                             AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_OPAQUE);
        if (temp)
        {
            opaque = axutil_strchr(temp, AXIS2_ESC_DOUBLE_QUOTE);
            if (opaque)
            {
                opaque++;
                temp = axutil_strchr(opaque, AXIS2_ESC_DOUBLE_QUOTE);
                alloc_temp = (axis2_char_t
                              *) (AXIS2_MALLOC (env->allocator,
                              sizeof (axis2_char_t) * (temp-opaque + 1)));
                strncpy(alloc_temp, opaque, (temp-opaque));
                if (alloc_temp)
                    alloc_temp[temp-opaque] = AXIS2_ESC_NULL;
                opaque = alloc_temp;
                alloc_temp = NULL;
                elen += print_const + 
                    axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_OPAQUE)
                    + axutil_strlen(opaque);
            }
            else
            {
                if (realm)
                    AXIS2_FREE (env->allocator, realm);
                if (nonce)
                    AXIS2_FREE (env->allocator, nonce);
                return AXIS2_FAILURE;
            }
        }

        if (qop)
        {
            nc = AXIS2_HTTP_AUTHORIZATION_REQUEST_DEFAULT_CLIENT_NONCE;
            temp = qop;
            if (!axutil_strstr(temp, 
                               AXIS2_HTTP_AUTHORIZATION_REQUEST_QOP_OPTION_AUTH))
            {
                return AXIS2_FAILURE;
            }
            AXIS2_FREE (env->allocator, qop);
            qop = AXIS2_HTTP_AUTHORIZATION_REQUEST_QOP_OPTION_AUTH;
            temp = axutil_uuid_gen(env);
            cnonce = temp;
            temp += CLIENT_NONCE_LENGTH;
            if (temp)
                *temp = AXIS2_ESC_NULL;
            elen += 11 + axutil_strlen(
                AXIS2_HTTP_AUTHORIZATION_REQUEST_DEFAULT_CLIENT_NONCE)
                + axutil_strlen(
                    AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE_COUNT)
                + axutil_strlen(
                    AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_CLIENT_NONCE)
                + axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_QOP)
                + CLIENT_NONCE_LENGTH + axutil_strlen(qop);
        }
        
        axutil_digest_calc_get_h_a1(env, algo, uname,
                                    realm, passwd, cnonce,
                                    nonce, h_a1);
        axutil_digest_calc_get_response(env, h_a1, nonce, nc,
                                        cnonce, qop, method_value,
                                        url, h_a2, response);

        elen += 4 + 
            axutil_strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_RESPONSE)
            + axutil_strlen(AXIS2_PROXY_AUTH_TYPE_DIGEST) + response_length;

        auth_str =
            (axis2_char_t
             *) (AXIS2_MALLOC (env->allocator,
                               sizeof (axis2_char_t) * (elen + 1)));
        temp = auth_str;
        sprintf (temp, "%s %s=\"%s\", ",
                 AXIS2_PROXY_AUTH_TYPE_DIGEST,
                 AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_USERNAME, uname);
        temp += ((int)strlen(AXIS2_HTTP_AUTH_TYPE_DIGEST) +
                 (int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_USERNAME) +
                 (int)strlen(uname) + 6);
        if (realm)
        {
            sprintf (temp, "%s=\"%s\", ",
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_REALM, realm);
            temp += ((int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_REALM) +
                     (int)strlen(realm) + 5);
        }
        if (nonce)
        {
            sprintf (temp, "%s=\"%s\", ",
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE, nonce);
            temp += ((int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE) +
                     (int)strlen(nonce) + 5);
        }
        sprintf (temp, "%s=\"%s\", ",
                 AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_URI, url);
        temp += ((int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_URI) +
                 (int)strlen(url) + 5);
        if (qop)
        {
            sprintf (temp, "%s=%s, %s=%s, %s=\"%s\", ",
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_QOP, qop,
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE_COUNT, nc,
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_CLIENT_NONCE, cnonce);
            temp += ((int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_QOP) +
                     (int)strlen(qop) +
                     (int)strlen(
                         AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE_COUNT) +
                     (int)strlen(nc) +
                     (int)strlen(
                         AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_CLIENT_NONCE) +
                     (int)strlen(cnonce) + 11);
        }
        if (opaque)
        {
            sprintf (temp, "%s=\"%s\", ",
                     AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_OPAQUE, opaque);
            temp += ((int)strlen(AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_OPAQUE)+
                     (int)strlen(opaque) + 5);
        }
        sprintf (temp, "%s=\"%s\"",
                 AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_RESPONSE, response);
        axis2_http_sender_util_add_header (env, request,
                                           AXIS2_HTTP_HEADER_PROXY_AUTHORIZATION,
                                           auth_str);
        if (realm)
        {
            AXIS2_FREE (env->allocator, realm);
        }

        if (nonce)
        {
            AXIS2_FREE (env->allocator, nonce);
        }
        
        if (cnonce)
        {
            AXIS2_FREE (env->allocator, cnonce);
        }

        if (opaque)
        {
            AXIS2_FREE (env->allocator, opaque);
        }

        if (auth_str)
        {
            AXIS2_FREE (env->allocator, auth_str);
        }

        auth_str = NULL;
 
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}
#endif

#ifndef AXIS2_LIBCURL_ENABLED
static axis2_status_t
axis2_http_sender_configure_http_auth (axis2_http_sender_t * sender,
                                       const axutil_env_t * env,
                                       axis2_msg_ctx_t * msg_ctx,
                                       axis2_http_simple_request_t * request)
{
    axis2_char_t *auth_type = NULL;
    axis2_status_t status = AXIS2_FALSE;
    axutil_property_t *http_auth_property = NULL;
    axis2_char_t *http_auth_property_value = NULL;
    axis2_bool_t force_http_auth = AXIS2_FALSE;
    axutil_property_t *http_auth_type_property = NULL;
    axis2_char_t *http_auth_type_property_value = NULL;
    axis2_char_t *auth_type_end = NULL;

    http_auth_property = 
        (axutil_property_t *)axis2_msg_ctx_get_property (msg_ctx, env,
                                                         AXIS2_FORCE_HTTP_AUTH);
    if (http_auth_property)
    {
        http_auth_property_value = 
            (axis2_char_t *) axutil_property_get_value (http_auth_property,
                                                        env);
    }

    if (http_auth_property_value && 
        0 == axutil_strcmp (http_auth_property_value, AXIS2_VALUE_TRUE))
    {
        force_http_auth = AXIS2_TRUE;
    }

    if (force_http_auth)
    {
        http_auth_type_property = 
            (axutil_property_t *) axis2_msg_ctx_get_property (msg_ctx, env,
                                                              AXIS2_HTTP_AUTH_TYPE);
        if (http_auth_type_property)
        {
            http_auth_type_property_value = 
                (axis2_char_t *) axutil_property_get_value (http_auth_type_property,
                                                            env);
        }

        if (http_auth_type_property_value)
        {
            auth_type = http_auth_type_property_value;
        }
    }
    if (!force_http_auth || 
        axutil_strcasecmp (auth_type, AXIS2_HTTP_AUTH_TYPE_DIGEST) == 0)
    {
        axis2_http_header_t *auth_header = NULL;
        axis2_http_simple_response_t *response = NULL;

        response = axis2_http_client_get_response (sender->client, env);

        if (response)
        {
            auth_header = 
                axis2_http_simple_response_get_first_header (
                    response, env,
                    AXIS2_HTTP_HEADER_WWW_AUTHENTICATE);
        }

        if (auth_header)
        {
            auth_type = axis2_http_header_get_value (auth_header, env);
        }
 
        if (auth_type)
        {
            auth_type_end = axutil_strchr (auth_type, ' ');
            *auth_type_end = AXIS2_ESC_NULL;
            auth_type_end++;
            /*Read the realm and the rest stuff now from auth_type_end */
        }
        if (force_http_auth && 
            axutil_strcasecmp (auth_type, AXIS2_HTTP_AUTH_TYPE_DIGEST) != 0)
        {
            auth_type = NULL;
        }
    }
    if (auth_type)
    {
        if (axutil_strcasecmp (auth_type, AXIS2_HTTP_AUTH_TYPE_BASIC) == 0)
        {
            status = axis2_http_sender_configure_http_basic_auth (sender, env,
                                                                  msg_ctx,
                                                                  request);
        }
        else if (axutil_strcasecmp (auth_type, AXIS2_HTTP_AUTH_TYPE_DIGEST) == 0)
        {
            status = axis2_http_sender_configure_http_digest_auth (sender, env,
                                                                   msg_ctx,
                                                                   request,
                                                                   auth_type_end);
        }
        else
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Authtype %s is not"
                             "supported", auth_type);
        }
    }
    else
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR,
                         AXIS2_FAILURE);
    }

    return status;
}

static axis2_status_t
axis2_http_sender_configure_proxy_auth (axis2_http_sender_t * sender,
                                        const axutil_env_t * env,
                                        axis2_msg_ctx_t * msg_ctx,
                                        axis2_http_simple_request_t * request)
{
    axis2_char_t *auth_type = NULL;
    axis2_status_t status = AXIS2_FALSE;
    axutil_property_t *proxy_auth_property = NULL;
    axis2_char_t *proxy_auth_property_value = NULL;
    axis2_bool_t force_proxy_auth = AXIS2_FALSE;
    axutil_property_t *proxy_auth_type_property = NULL;
    axis2_char_t *proxy_auth_type_property_value = NULL;
    axis2_char_t *auth_type_end = NULL;

    proxy_auth_property = 
        (axutil_property_t *) axis2_msg_ctx_get_property (msg_ctx, env,
                                                          AXIS2_FORCE_PROXY_AUTH);
    if (proxy_auth_property)
    {
        proxy_auth_property_value = 
            (axis2_char_t *) axutil_property_get_value (proxy_auth_property,
                                                        env);
    }

    if (proxy_auth_property_value && 
        0 == axutil_strcmp (proxy_auth_property_value, AXIS2_VALUE_TRUE))
    {
        force_proxy_auth = AXIS2_TRUE;
    }

    if (force_proxy_auth)
    {
        proxy_auth_type_property = 
            (axutil_property_t *) axis2_msg_ctx_get_property (msg_ctx, env,
                                                              AXIS2_PROXY_AUTH_TYPE);
        if (proxy_auth_type_property)
        {
            proxy_auth_type_property_value = 
                (axis2_char_t *) axutil_property_get_value (proxy_auth_type_property,
                                                            env);
        }

        if (proxy_auth_type_property_value)
        {
            auth_type = proxy_auth_type_property_value;
        }
    }
    else
    {
        axis2_http_header_t *auth_header = NULL;
        axis2_http_simple_response_t *response = NULL;
    
        response = axis2_http_client_get_response (sender->client, env);

        if (response)
        {
            auth_header = 
                axis2_http_simple_response_get_first_header (
                    response, env,
                    AXIS2_HTTP_HEADER_PROXY_AUTHENTICATE);
        }

        if (auth_header)
        {
            auth_type = axis2_http_header_get_value (auth_header, env);
        }

        if (auth_type)
        {
            auth_type_end = axutil_strchr (auth_type, ' ');
            *auth_type_end = AXIS2_ESC_NULL;
            auth_type_end++;
            /*Read the realm and the rest stuff now from auth_type_end */
        }
    }
    if (auth_type)
    {
        if (axutil_strcasecmp (auth_type, AXIS2_PROXY_AUTH_TYPE_BASIC) == 0)
        {
            status = axis2_http_sender_configure_proxy_basic_auth (sender, env,
                                                                   msg_ctx,
                                                                   request);
        }
        else if (axutil_strcasecmp (auth_type, 
                                    AXIS2_PROXY_AUTH_TYPE_DIGEST) == 0)
        {
            status = 
                axis2_http_sender_configure_proxy_digest_auth (sender, env,
                                                               msg_ctx,
                                                               request,
                                                               auth_type_end);
        }
        else
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                             "Authtype %s is not supported", auth_type);
        }
    }
    else
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR,
                         AXIS2_FAILURE);
    }

    return status;
}
#endif

#ifndef AXIS2_LIBCURL_ENABLED
static axis2_status_t
axis2_http_sender_set_http_auth_type (axis2_http_sender_t * sender,
                                      const axutil_env_t * env,
                                      axis2_msg_ctx_t * msg_ctx,
                                      axis2_http_simple_request_t * request)
{
    axis2_char_t *auth_type = NULL;
    axis2_status_t status = AXIS2_FALSE;
    axis2_char_t *auth_type_end = NULL;
    axis2_http_header_t *auth_header = NULL;
    axis2_http_simple_response_t *response = NULL;

    response = axis2_http_client_get_response (sender->client, env);
 
    if (response)
    {
        auth_header = 
            axis2_http_simple_response_get_first_header (
                response, env,
                AXIS2_HTTP_HEADER_WWW_AUTHENTICATE);
    }

    if (auth_header)
    {
        auth_type = axis2_http_header_get_value (auth_header, env);
    }

    if (auth_type)
    {
        auth_type_end = axutil_strchr (auth_type, ' ');
        *auth_type_end = AXIS2_ESC_NULL;
        auth_type_end++;
            /*Read the realm and the rest stuff now from auth_type_end */
    }

    if (auth_type)
    {
        if (axutil_strcasecmp (auth_type, AXIS2_HTTP_AUTH_TYPE_BASIC) == 0)
        {
            status = 
                axis2_msg_ctx_set_auth_type (msg_ctx, env, 
                                             AXIS2_HTTP_AUTH_TYPE_BASIC);
        }
        else if (axutil_strcasecmp (auth_type, AXIS2_HTTP_AUTH_TYPE_DIGEST) == 0)
        {
            status = 
                axis2_msg_ctx_set_auth_type (msg_ctx, env, 
                                             AXIS2_HTTP_AUTH_TYPE_DIGEST);
        }
        else
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                             "Authtype %s is not supported", auth_type);
        }
    }
    else
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR,
                         AXIS2_FAILURE);
    }
    return status;
}

static axis2_status_t
axis2_http_sender_set_proxy_auth_type (axis2_http_sender_t * sender,
                                       const axutil_env_t * env,
                                       axis2_msg_ctx_t * msg_ctx,
                                       axis2_http_simple_request_t * request)
{
    axis2_char_t *auth_type = NULL;
    axis2_status_t status = AXIS2_FALSE;
    axis2_char_t *auth_type_end = NULL;
    axis2_http_header_t *auth_header = NULL;
    axis2_http_simple_response_t *response = NULL;

    response = axis2_http_client_get_response (sender->client, env);
 
    if (response)
    {
        auth_header = axis2_http_simple_response_get_first_header (
            response, env,
            AXIS2_HTTP_HEADER_PROXY_AUTHENTICATE);
    }

    if (auth_header)
    {
        auth_type = axis2_http_header_get_value (auth_header, env);
    }

    if (auth_type)
    {
        auth_type_end = axutil_strchr (auth_type, ' ');
        *auth_type_end = AXIS2_ESC_NULL;
        auth_type_end++;
            /*Read the realm and the rest stuff now from auth_type_end */
    }

    if (auth_type)
    {
        if (axutil_strcasecmp (auth_type, AXIS2_PROXY_AUTH_TYPE_BASIC) == 0)
        {
            status = axis2_msg_ctx_set_auth_type (msg_ctx, 
                                                  env, 
                                                  AXIS2_PROXY_AUTH_TYPE_BASIC);
        }
        else if (axutil_strcasecmp (auth_type, AXIS2_PROXY_AUTH_TYPE_DIGEST) == 0)
        {
            status = axis2_msg_ctx_set_auth_type (msg_ctx, env, 
                                                  AXIS2_PROXY_AUTH_TYPE_DIGEST);
        }
        else
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                             "Authtype %s is not supported", auth_type);
        }
    }
    else
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR,
                         AXIS2_FAILURE);
    }
    return status;
}
#endif

#ifdef AXIS2_LIBCURL_ENABLED
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_libcurl_http_send (axis2_libcurl_t * curl,
                         axis2_http_sender_t * sender,
                         const axutil_env_t * env,
                         axis2_msg_ctx_t * msg_ctx,
                         axiom_soap_envelope_t * out,
                         const axis2_char_t * str_url,
                         const axis2_char_t * soap_action)
{
    return axis2_libcurl_send (curl, sender->om_output,
                               env, msg_ctx, out, str_url, soap_action);
}
#endif

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_sender_get_param_string (axis2_http_sender_t * sender,
                                    const axutil_env_t * env,
                                    axis2_msg_ctx_t * msg_ctx)
{
    axiom_soap_envelope_t *soap_env = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_node_t *body_node = NULL;
    axiom_node_t *data_node = NULL;
    axiom_element_t *data_element = NULL;
    axiom_child_element_iterator_t *iterator = NULL;
    axutil_array_list_t *param_list = NULL;
    axis2_char_t *param_string = NULL;
    int i = 0;

    AXIS2_PARAM_CHECK (env->error, msg_ctx, NULL);

    soap_env = axis2_msg_ctx_get_soap_envelope (msg_ctx, env);
    if (!soap_env)
    {
        return NULL;
    }
    soap_body = axiom_soap_envelope_get_body(soap_env, env);
    body_node =
        axiom_soap_body_get_base_node(soap_body, env);
    if(!body_node)
    {
        /* This could be the situation where service client does not provide
         * a xml payload and instead add url parameters to the endpoint url
         */
        return NULL;
    }
    data_node = axiom_node_get_first_child (body_node, env);
    if (!data_node)
    {
        return NULL;
    }

    param_list =
        axutil_array_list_create (env, AXIS2_ARRAY_LIST_DEFAULT_CAPACITY);

    data_element = axiom_node_get_data_element (data_node, env);

    iterator = axiom_element_get_child_elements (data_element, env, data_node);

    if (iterator)
    {
        while (AXIS2_TRUE ==
               AXIOM_CHILD_ELEMENT_ITERATOR_HAS_NEXT (iterator, env))
        {
            axiom_node_t *node = NULL;
            axiom_element_t *element = NULL;
            axis2_char_t *name = NULL;
            axis2_char_t *value = NULL;
            axis2_char_t *encoded_value = NULL;

            node = AXIOM_CHILD_ELEMENT_ITERATOR_NEXT (iterator, env);
            element = axiom_node_get_data_element (node, env);
            name = axiom_element_get_localname (element, env);
            value = axiom_element_get_text (element, env, node);
            if (value)
            {
                encoded_value =
                    (axis2_char_t *) AXIS2_MALLOC (env->allocator,
                                                   strlen (value));
                memset (encoded_value, 0, strlen (value));
                encoded_value =
                    axutil_url_encode (env, encoded_value, value, 
                                       (int)strlen (value));
                /* We are sure that the difference lies within the int range */

                axutil_array_list_add (param_list, env,
                                       axutil_strcat (env, name, "=",
                                                      encoded_value, NULL));
				AXIS2_FREE(env->allocator, encoded_value);
				encoded_value = NULL;
            }
        }
    }
    for (i = 0; i < axutil_array_list_size (param_list, env); i++)
    {
        axis2_char_t *tmp_string = NULL;
        axis2_char_t *pair = NULL;

        pair = axutil_array_list_get (param_list, env, i);
        if (i == 0)
        {
            tmp_string = axutil_stracat (env, param_string, pair);
        }
        else
        {
            tmp_string = axutil_strcat (env, param_string, AXIS2_AND_SIGN,
                                        pair, NULL);
        }

        if (param_string)
        {
            AXIS2_FREE (env->allocator, param_string);
            param_string = NULL;
        }
        AXIS2_FREE (env->allocator, pair);
        param_string = tmp_string;
    }
    axutil_array_list_free (param_list, env);
    return param_string;
}

void AXIS2_CALL
axis2_http_sender_util_add_header (const axutil_env_t * env,
                                   axis2_http_simple_request_t * request,
                                   axis2_char_t * header_name,
                                   const axis2_char_t * header_value)
{
    axis2_http_header_t *http_header;
    http_header = axis2_http_header_create (env, header_name, header_value);
    axis2_http_simple_request_add_header (request, env, http_header);
}
