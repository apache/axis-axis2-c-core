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
#ifdef AXIS2_LIBCURL_ENABLED

#include "axis2_libcurl.h"
#include <axiom_soap.h>
#include <axutil_string.h>
#include <axis2_http_transport.h>
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
#include <axis2_const.h>
#include <axis2_util.h>
#include <stdlib.h>
#include <axis2_http_sender.h>
#include <axis2_http_transport.h>
#include "libcurl_stream.h"

static int ref = 0;

struct axis2_libcurl
{
    axis2_char_t *memory;
    axutil_array_list_t *alist;
    unsigned int size;
    const axutil_env_t *env;
    char errorbuffer[CURL_ERROR_SIZE];
    CURL *handler;
    axis2_bool_t cookies;
};

static size_t 
axis2_libcurl_write_memory_callback(
    void *ptr,
    size_t size,
    size_t nmemb,
    void *data);

static size_t 
axis2_libcurl_header_callback(
    void *ptr,
    size_t size,
    size_t nmemb,
    void *data);

static axis2_char_t *
axis2_libcurl_get_content_type(
    axis2_libcurl_t *curl,
    const axutil_env_t * env);

static int
axis2_libcurl_get_content_length(
    axis2_libcurl_t *curl,
    const axutil_env_t * env);

static axis2_http_header_t *
axis2_libcurl_get_first_header(
    axis2_libcurl_t *curl,
    const axutil_env_t * env,
    const axis2_char_t * str);

static void
axis2_libcurl_free_headers(    
    axis2_libcurl_t *curl,
    const axutil_env_t * env);

axis2_status_t AXIS2_CALL
axis2_libcurl_send(
    axis2_libcurl_t *data,
    axiom_output_t * om_output,
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axiom_soap_envelope_t * out,
    const axis2_char_t * str_url,
    const axis2_char_t * soap_action)
{
    struct curl_slist *headers = NULL;
    axiom_soap_body_t *soap_body;
    axis2_bool_t is_soap = AXIS2_TRUE;
    axis2_bool_t send_via_get = AXIS2_FALSE;
    axis2_bool_t send_via_head = AXIS2_FALSE;
    axis2_bool_t send_via_put = AXIS2_FALSE;
    axis2_bool_t send_via_delete = AXIS2_FALSE;
    axis2_bool_t doing_mtom = AXIS2_FALSE;
    axiom_node_t *body_node = NULL;
    axiom_node_t *data_out = NULL;
    axutil_property_t *method = NULL;
    axis2_char_t *method_value = NULL;
    axiom_xml_writer_t *xml_writer = NULL;
    axis2_char_t *buffer = NULL;
    unsigned int buffer_size = 0;
    int content_length = -1;
    axis2_char_t *content_type;
    axis2_char_t *content_len = AXIS2_HTTP_HEADER_CONTENT_LENGTH_;
    const axis2_char_t *char_set_enc = NULL;
    axis2_char_t *content = AXIS2_HTTP_HEADER_CONTENT_TYPE_;
    axis2_char_t *soap_action_header = AXIS2_HTTP_HEADER_SOAP_ACTION_;
    axutil_stream_t *in_stream;
    axutil_property_t *trans_in_property;
    axutil_string_t *char_set_enc_str;
    axis2_byte_t *output_stream = NULL;
    int output_stream_size = 0;
    CURL *handler;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    axis2_transport_out_desc_t *trans_desc = NULL;
    axutil_param_t *write_xml_declaration_param = NULL;
    axutil_hash_t *transport_attrs = NULL;
    axis2_bool_t write_xml_declaration = AXIS2_FALSE;
    axutil_property_t *property;
    int *response_length = NULL;
    axis2_http_status_line_t *status_line = NULL;
    axis2_char_t *status_line_str = NULL;
    axis2_char_t *tmp_strcat = NULL;
    int status_code = 0;

    AXIS2_PARAM_CHECK(env->error, data, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, data->handler, AXIS2_FAILURE);

    handler = data->handler;
    curl_easy_reset(handler);
    curl_easy_setopt(handler, CURLOPT_ERRORBUFFER, &data->errorbuffer);
    headers = curl_slist_append(headers, AXIS2_HTTP_HEADER_USER_AGENT_AXIS2C);
    headers = curl_slist_append(headers, AXIS2_HTTP_HEADER_ACCEPT_);
    headers = curl_slist_append(headers, AXIS2_HTTP_HEADER_EXPECT_);

    if (AXIS2_TRUE == axis2_msg_ctx_get_doing_rest(msg_ctx, env))
    {
        is_soap = AXIS2_FALSE;
    }
    else
    {
        is_soap = AXIS2_TRUE;
    }

    if (!is_soap)
    {
        soap_body = axiom_soap_envelope_get_body(out, env);
        if (!soap_body)
        {
            AXIS2_HANDLE_ERROR(env,
                            AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL,
                            AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }

        body_node = axiom_soap_body_get_base_node(soap_body, env);
        if (!body_node)
        {
            AXIS2_HANDLE_ERROR(env,
                               AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL,
                               AXIS2_FAILURE);

            return AXIS2_FAILURE;
        }
        data_out = axiom_node_get_first_element(body_node, env);

        method = (axutil_property_t *) axis2_msg_ctx_get_property(msg_ctx, env,
                                                                  AXIS2_HTTP_METHOD);

        if (method)
        {
            method_value =
                (axis2_char_t *) axutil_property_get_value(method, env);
        }

        /* The default is POST */
        if (method_value && 0 == axutil_strcmp(method_value, AXIS2_HTTP_GET))
        {
            send_via_get = AXIS2_TRUE;
        }
        else if (method_value && 0 == axutil_strcmp(method_value, AXIS2_HTTP_HEAD))
        {
            send_via_head = AXIS2_TRUE;
        }
        else if (method_value && 0 == axutil_strcmp(method_value, AXIS2_HTTP_PUT))
        {
            send_via_put = AXIS2_TRUE;
        }
        else if (method_value && 0 == axutil_strcmp(method_value, AXIS2_HTTP_DELETE))
        {
            send_via_delete = AXIS2_TRUE;
        }
    }

    conf_ctx = axis2_msg_ctx_get_conf_ctx (msg_ctx, env);
    if (conf_ctx)
    {
        conf = axis2_conf_ctx_get_conf (conf_ctx, env);
    }

    if (conf)
    {
        trans_desc = axis2_conf_get_transport_out (conf,
                                                   env, AXIS2_TRANSPORT_ENUM_HTTP);
    }

    if (trans_desc)
    {
        write_xml_declaration_param =
            axutil_param_container_get_param
            (axis2_transport_out_desc_param_container (trans_desc, env), env,
             AXIS2_XML_DECLARATION);
    }


    if (write_xml_declaration_param)
    {
        transport_attrs = 
            axutil_param_get_attributes (write_xml_declaration_param, env);
        if (transport_attrs)
        {
            axutil_generic_obj_t *obj = NULL;
            axiom_attribute_t *write_xml_declaration_attr = NULL;
            axis2_char_t *write_xml_declaration_attr_value = NULL;

            obj = axutil_hash_get (transport_attrs, AXIS2_ADD_XML_DECLARATION,
                           AXIS2_HASH_KEY_STRING);
            if (obj)
            {
                write_xml_declaration_attr = (axiom_attribute_t *) 
                    axutil_generic_obj_get_value (obj,
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
        axiom_output_write_xml_version_encoding (om_output, env);
    }

    if (!send_via_get && !send_via_head && !send_via_delete)
    {
        xml_writer = axiom_output_get_xml_writer(om_output, env);

        char_set_enc_str = axis2_msg_ctx_get_charset_encoding(msg_ctx, env);

        if (!char_set_enc_str)
        {
            char_set_enc = AXIS2_DEFAULT_CHAR_SET_ENCODING;
        }
        else
        {
            char_set_enc = axutil_string_get_buffer(char_set_enc_str, env);
        }

        if (!send_via_put && is_soap)
        {
            doing_mtom = axis2_msg_ctx_get_doing_mtom(msg_ctx, env);

            axiom_output_set_do_optimize(om_output, env, doing_mtom);
            axiom_soap_envelope_serialize(out, env, om_output, AXIS2_FALSE);
            if (AXIS2_TRUE == axis2_msg_ctx_get_is_soap_11(msg_ctx, env))
            {
                if (AXIS2_ESC_DOUBLE_QUOTE != *soap_action)
                {
                    axis2_char_t *tmp_soap_action = NULL;
                    tmp_soap_action =
                        AXIS2_MALLOC(env->allocator,
                                     (axutil_strlen(soap_action) +
                                      5) * sizeof(axis2_char_t));
                    sprintf(tmp_soap_action, "\"%s\"", soap_action);
                    tmp_strcat = axutil_stracat(env, soap_action_header,tmp_soap_action);
                    headers = curl_slist_append(headers, tmp_strcat);
                    AXIS2_FREE(env->allocator, tmp_strcat);
                    AXIS2_FREE(env->allocator, tmp_soap_action);
                }
                else
                {
                    tmp_strcat = axutil_stracat(env, soap_action_header, soap_action);
                    headers = curl_slist_append(headers, tmp_strcat );
                    AXIS2_FREE(env->allocator, tmp_strcat);
                }
            }

            if (doing_mtom)
            {
                /*axiom_output_flush(om_output, env, &output_stream,
                                   &output_stream_size);*/
                axiom_output_flush(om_output, env);
                content_type =
                    (axis2_char_t *) axiom_output_get_content_type(om_output,
                                                                   env);
                if (AXIS2_TRUE != axis2_msg_ctx_get_is_soap_11(msg_ctx, env))
                {
                    if (axutil_strcmp(soap_action, ""))
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
                }
            }
            else if (AXIS2_TRUE == axis2_msg_ctx_get_is_soap_11(msg_ctx, env))
            {
                axis2_char_t *temp_content_type = NULL;
                content_type =
                    (axis2_char_t *) AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML;
                content_type = axutil_stracat(env, content_type, 
                                              AXIS2_CONTENT_TYPE_CHARSET);
                temp_content_type =
                    axutil_stracat(env, content_type, char_set_enc);
                AXIS2_FREE(env->allocator, content_type);
                content_type = temp_content_type;
            }
            else
            {
                axis2_char_t *temp_content_type = NULL;
                content_type =
                    (axis2_char_t *) AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP;
                content_type = axutil_stracat(env, content_type, 
                                              AXIS2_CONTENT_TYPE_CHARSET);
                temp_content_type =
                    axutil_stracat(env, content_type, char_set_enc);
                AXIS2_FREE(env->allocator, content_type);
                content_type = temp_content_type;
                if (axutil_strcmp(soap_action, ""))
                {
                    temp_content_type =
                        axutil_stracat(env, content_type, 
                                       AXIS2_CONTENT_TYPE_ACTION);
                    AXIS2_FREE(env->allocator, content_type);
                    content_type = temp_content_type;
                    temp_content_type =
                        axutil_stracat(env, content_type, soap_action);
                    AXIS2_FREE(env->allocator, content_type);
                    content_type = temp_content_type;
                }
                temp_content_type = axutil_stracat(env, content_type, 
                                                   AXIS2_SEMI_COLON_STR);
                AXIS2_FREE(env->allocator, content_type);
                content_type = temp_content_type;
            }
        }
        else if (is_soap)
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Attempt to send SOAP"
                             "message using HTTP PUT failed");
            return AXIS2_FAILURE;
        }
        else
        {
            axutil_property_t *content_type_property = NULL;
            axutil_hash_t *content_type_hash = NULL;
            axis2_char_t *content_type_value = NULL;

            axiom_node_serialize(data_out, env, om_output);
            content_type_property =
                (axutil_property_t *) 
                axis2_msg_ctx_get_property(msg_ctx, env,
                                           AXIS2_USER_DEFINED_HTTP_HEADER_CONTENT_TYPE);

            if (content_type_property)
            {
                content_type_hash =
                    (axutil_hash_t *)
                    axutil_property_get_value(content_type_property, env);

                if (content_type_hash)
                {
                    content_type_value =
                        (char *) axutil_hash_get(content_type_hash,
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

        }

        buffer = axiom_xml_writer_get_xml(xml_writer, env);
        if (!doing_mtom)
        {
            buffer_size = axiom_xml_writer_get_xml_size(xml_writer, env);
        }
        else
            buffer_size = output_stream_size;
        {
            char tmp_buf[10];
            sprintf(tmp_buf, "%d", buffer_size);
            tmp_strcat = axutil_stracat(env, content_len, tmp_buf);
            headers = curl_slist_append(headers, tmp_strcat);
            AXIS2_FREE(env->allocator, tmp_strcat);
            tmp_strcat = NULL;

            tmp_strcat = axutil_stracat(env, content, content_type);
            headers = curl_slist_append(headers, tmp_strcat);
            AXIS2_FREE(env->allocator, tmp_strcat);
            tmp_strcat = NULL;
        }

        if (!doing_mtom)
        {
            curl_easy_setopt(handler, CURLOPT_POSTFIELDSIZE, buffer_size);
            curl_easy_setopt(handler, CURLOPT_POSTFIELDS, buffer);
        }
        else
        {
            curl_easy_setopt(handler, CURLOPT_POSTFIELDSIZE,
                             output_stream_size);
            curl_easy_setopt(handler, CURLOPT_POSTFIELDS, output_stream);
        }


        if (send_via_put)
        {
            curl_easy_setopt(handler, CURLOPT_CUSTOMREQUEST, AXIS2_HTTP_PUT);
        }
        curl_easy_setopt(handler, CURLOPT_URL, str_url);
    }
    else
    {
        axis2_char_t *request_param;
        axis2_char_t *url_encode;
        request_param =
            (axis2_char_t *) axis2_http_sender_get_param_string(NULL, env,
                                                                msg_ctx);
        url_encode = axutil_strcat(env, str_url, AXIS2_Q_MARK_STR, 
                                   request_param, NULL);
        if (send_via_get)
        {
            curl_easy_setopt(handler, CURLOPT_HTTPGET, 1);
        }
        else if (send_via_head)
        {
            curl_easy_setopt(handler, CURLOPT_NOBODY, 1);
        }
        else if (send_via_delete)
        {
            curl_easy_setopt(handler, CURLOPT_CUSTOMREQUEST, AXIS2_HTTP_DELETE);
        }
        curl_easy_setopt(handler, CURLOPT_URL, url_encode);
    }

    {
        axis2_bool_t manage_session;
        manage_session = axis2_msg_ctx_get_manage_session(msg_ctx, env);
        if (manage_session == AXIS2_TRUE) 
        {
            if (data->cookies == AXIS2_FALSE)
            {
                /* Ensure cookies enabled to manage session */
                /* Pass empty cookie string to enable cookies */
                curl_easy_setopt(handler, CURLOPT_COOKIEFILE, " ");	
                data->cookies = AXIS2_TRUE;
            }
        }
        else if (data->cookies == AXIS2_TRUE)
        {
            /* Pass special string ALL to reset cookies if any have been enabled. */
            /* If cookies have ever been enabled, we reset every time as long as 
               manage_session is false, as there is no clear curl option to 
               turn off the cookie engine once enabled. */
            curl_easy_setopt(handler, CURLOPT_COOKIELIST, AXIS2_ALL);	
        }
    }

    curl_easy_setopt(handler, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(handler, CURLOPT_WRITEFUNCTION,
                     axis2_libcurl_write_memory_callback);
    curl_easy_setopt(handler, CURLOPT_WRITEDATA, data);

    curl_easy_setopt (handler, CURLOPT_HEADERFUNCTION, axis2_libcurl_header_callback);

    curl_easy_setopt (handler, CURLOPT_WRITEHEADER, data);

    /* Free response data from previous request */
    if( data->size )
    {
        if (data->memory)
        {
            AXIS2_FREE(data->env->allocator, data->memory);
        }
        data->size = 0;
    }

    if (curl_easy_perform(handler)) 
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "%s", &data->errorbuffer);
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR,
                           AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    in_stream = axutil_stream_create_libcurl(env, data->memory, data->size);
    trans_in_property = axutil_property_create(env);
    axutil_property_set_scope(trans_in_property, env, AXIS2_SCOPE_REQUEST);
    axutil_property_set_free_func(trans_in_property, env,
                                  libcurl_stream_free);
    axutil_property_set_value(trans_in_property, env, in_stream);
    axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_TRANSPORT_IN,
                               trans_in_property);

    if (axutil_array_list_size(data->alist, env) > 0)
    {
        status_line_str = axutil_array_list_get(data->alist, env, 0);
        if (status_line_str)
        {
            status_line = axis2_http_status_line_create(env, status_line_str);
        }
    }

    if (status_line)
    {
        status_code = axis2_http_status_line_get_status_code(status_line, env);
    }

    axis2_msg_ctx_set_status_code (msg_ctx, env, status_code);
    AXIS2_FREE(data->env->allocator, content_type);
    content_type = axis2_libcurl_get_content_type(data, env); 

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

    content_length = axis2_libcurl_get_content_length(data, env);
    if (content_length >= 0)
    {
        response_length = AXIS2_MALLOC (env->allocator, sizeof (int));
        memcpy (response_length, &content_length, sizeof (int));
        property = axutil_property_create (env);
        axutil_property_set_scope (property, env, AXIS2_SCOPE_REQUEST);
        axutil_property_set_value (property, env, response_length);
        axis2_msg_ctx_set_property (msg_ctx, env,
                                    AXIS2_HTTP_HEADER_CONTENT_LENGTH, property);
    }

    curl_slist_free_all (headers);
    /* release the read http headers. */
    /* (commenting out the call below is a clever way to force a premature EOF 
       condition in subsequent messages, as they will be read using the content-length
       of the first message.) */
    axis2_libcurl_free_headers(data, env);
    AXIS2_FREE(data->env->allocator, content_type);
    axis2_http_status_line_free( status_line, env);

    return AXIS2_SUCCESS;
}

static size_t
axis2_libcurl_write_memory_callback(
    void *ptr,
    size_t size,
    size_t nmemb,
    void *data)
{
    size_t realsize = size * nmemb;
    axis2_libcurl_t *curl = (axis2_libcurl_t *) data;
    axis2_char_t *buffer =
        (axis2_char_t *) AXIS2_MALLOC(curl->env->allocator,
                                      curl->size + realsize + 1);
    if (buffer)
    {
        if (curl->size)
        {
            memcpy(&(buffer[0]), curl->memory, curl->size);
            AXIS2_FREE(curl->env->allocator, curl->memory);
        }

        memcpy(&(buffer[curl->size]), ptr, realsize);
        curl->size += (int)realsize;
        /* We are sure that the difference lies within the int range */
        buffer[curl->size] = 0;
        curl->memory = buffer;
    }
    return realsize;
}

static size_t
axis2_libcurl_header_callback(
    void *ptr,
    size_t size,
    size_t nmemb,
    void *data)
{
    axis2_char_t *memory;
    size_t realsize = size * nmemb;
    axis2_libcurl_t *curl = (axis2_libcurl_t *) data;
    memory = (axis2_char_t *)AXIS2_MALLOC(curl->env->allocator, realsize + 1);
    if (memory)
    {
        memcpy(&(memory[0]), ptr, realsize);
        memory[realsize] = 0;
        axutil_array_list_add(curl->alist, curl->env, memory);
    }
    return realsize;
}

axis2_libcurl_t * AXIS2_CALL
axis2_libcurl_create(
    const axutil_env_t * env)
{
    axis2_libcurl_t *curl = NULL;
    CURLcode code;


    if (!ref)
    {
        /* curl_global_init is not thread-safe so it would be better
           to do this, as well as the test and increment of ref, under
           mutex if one is available, or as part of an
           axis2_initialize() if a global initialize is created.
           Otherwise the client application should perform the the
           curl_global_init itself in a thread-safe fashion. 
        */
        code = curl_global_init(CURL_GLOBAL_ALL);
        if (code)	
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                            "libcurl curl_global_init failed, error: %d", code); 
            return NULL;
        }
        ref++;
    }

    curl =
        (axis2_libcurl_t *) AXIS2_MALLOC(env->allocator,
                                         sizeof(axis2_libcurl_t));
    if (curl) 
    {
        curl->memory = 0;
        curl->size = 0;
        curl->alist = axutil_array_list_create(env, 15);
        curl->env = env;
        curl->handler = curl_easy_init();
        curl->cookies = AXIS2_FALSE;
        if ((!curl->alist) || (!curl->handler))
        {
            axis2_libcurl_free(curl, env);
            curl = 0;
        }
    }
    return curl;
}

void AXIS2_CALL
axis2_libcurl_free(
    axis2_libcurl_t *curl,
    const axutil_env_t * env)
{
    if (!curl)
    {
        return;
    }

    if (curl->handler)
    {
        curl_easy_cleanup (curl->handler); 
    }
    if (curl->alist)
    {
        axis2_libcurl_free_headers(curl, env);
        axutil_array_list_free(curl->alist, env);
        curl->alist = NULL;
    }
    if (curl->memory) 
    {
        AXIS2_FREE(env->allocator, curl->memory);
    }
    
    AXIS2_FREE(env->allocator, curl);
}

static void
axis2_libcurl_free_headers(    
    axis2_libcurl_t *curl,
    const axutil_env_t * env)
{
    int count = 0;
    axutil_array_list_t *header_group = curl->alist;
    if (header_group)
    {
        while ((count = axutil_array_list_size(header_group, env)) > 0)
        {
            axis2_char_t *header = axutil_array_list_remove(header_group, env, count-1);
            AXIS2_FREE(env->allocator, header);
        }
    }
}
 
static axis2_http_header_t *
axis2_libcurl_get_first_header(
    axis2_libcurl_t *curl,
    const axutil_env_t * env,
    const axis2_char_t * str)
{
    axis2_http_header_t *tmp_header = NULL;
    axis2_char_t *tmp_header_str = NULL;
    axis2_char_t *tmp_name = NULL;
    int i = 0;
    int count = 0;
    axutil_array_list_t *header_group = NULL;
 
    AXIS2_PARAM_CHECK(env->error, curl, NULL);
    AXIS2_PARAM_CHECK(env->error, str, NULL);

    header_group = curl->alist;
    if (!header_group)
    {
        return NULL;
    }

    if (0 == axutil_array_list_size(header_group, env))
    {
        return NULL;
    }

    count = axutil_array_list_size(header_group, env);

    for (i = 0; i < count; i++)
    {
        tmp_header_str = (axis2_char_t *) axutil_array_list_get(header_group,
                                                                   env, i);
        if(!tmp_header_str)
        {
            continue;
        }
        tmp_header = (axis2_http_header_t *) axis2_http_header_create_by_str(env, tmp_header_str);
        if(!tmp_header)
        {
            continue;
        }

        tmp_name = axis2_http_header_get_name(tmp_header, env);
        if (0 == axutil_strcasecmp(str, tmp_name))
        {
            return tmp_header;
        }
        else
        {
            axis2_http_header_free( tmp_header, env );
        }

    }
    return NULL;
}

static int
axis2_libcurl_get_content_length(
    axis2_libcurl_t *curl,
    const axutil_env_t * env)
{
    axis2_http_header_t *tmp_header;
    int rtn_value = -1;

    tmp_header = axis2_libcurl_get_first_header
        (curl, env, AXIS2_HTTP_HEADER_CONTENT_LENGTH);
    if (tmp_header)
    {
        rtn_value = AXIS2_ATOI(axis2_http_header_get_value(tmp_header, env));
        axis2_http_header_free( tmp_header, env );
    }
    return rtn_value;
}


static axis2_char_t *
axis2_libcurl_get_content_type(
    axis2_libcurl_t *curl,
    const axutil_env_t * env)
{
    axis2_http_header_t *tmp_header;
    axis2_char_t *rtn_value	= NULL;

    tmp_header = axis2_libcurl_get_first_header
        (curl, env, AXIS2_HTTP_HEADER_CONTENT_TYPE);
    if (tmp_header)
    {
        rtn_value = axutil_strdup (env, axis2_http_header_get_value(tmp_header, env) );
        axis2_http_header_free( tmp_header, env );
    }
    else
    {
        rtn_value = axutil_strdup (env, AXIS2_HTTP_HEADER_ACCEPT_TEXT_PLAIN);
    }

    return rtn_value;
}

#endif                          /* AXIS2_LIBCURL_ENABLED */



