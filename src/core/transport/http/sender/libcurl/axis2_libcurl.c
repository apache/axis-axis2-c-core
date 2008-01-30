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

typedef struct axis2_libcurl_header
{
    axutil_array_list_t *alist;
    const axutil_env_t *env;
} axis2_libcurl_header_t;

static CURL *handler;

typedef struct axis2_libcurl
{
    axis2_char_t *memory;
    axutil_array_list_t *alist;
    unsigned int size;
    axis2_libcurl_header_t *header;
    const axutil_env_t *env;
    char errorbuffer[CURL_ERROR_SIZE];
} axis2_libcurl_t;

size_t axis2_libcurl_write_memory_callback(
    void *ptr,
    size_t size,
    size_t nmemb,
    void *data);

size_t axis2_libcurl_header_callback(
    void *ptr,
    size_t size,
    size_t nmemb,
    void *data);

axis2_libcurl_t *axis2_libcurl_create(
    const axutil_env_t * env);

void axis2_libcurl_free(
    axis2_libcurl_t *curl,
    const axutil_env_t * env);

axis2_char_t *
axis2_libcurl_get_content_type(
    axis2_libcurl_header_t * header,
    const axutil_env_t * env);

axis2_ssize_t
axis2_libcurl_get_content_length(
    axis2_libcurl_header_t * header,
    const axutil_env_t * env);

axis2_http_header_t *AXIS2_CALL
axis2_libcurl_get_first_header(
    axis2_libcurl_header_t * header,
    const axutil_env_t * env,
    const axis2_char_t * str);


axis2_status_t AXIS2_CALL
axis2_libcurl_send(
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
    axis2_char_t *content_type;
    axis2_char_t *content_len = AXIS2_HTTP_HEADER_CONTENT_LENGTH_;
    const axis2_char_t *char_set_enc = NULL;
    axis2_char_t *content = AXIS2_HTTP_HEADER_CONTENT_TYPE_;
    axis2_char_t *soap_action_header = AXIS2_HTTP_HEADER_SOAP_ACTION_;
    axis2_libcurl_t *data;
    axutil_stream_t *in_stream;
    axutil_property_t *trans_in_property;
    axutil_string_t *char_set_enc_str;
    axis2_byte_t *output_stream = NULL;
    int output_stream_size = 0;
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
    int status_code = 0;

    data = axis2_libcurl_create(env);
    if (!data) 
    {
        return AXIS2_FAILURE;
    }
    if (!ref)
    {
        handler = curl_easy_init();
        ref++;
    }
    else
    {
        curl_easy_reset(handler);
    }
    curl_easy_setopt(handler, CURLOPT_ERRORBUFFER, &data->errorbuffer);
    headers = curl_slist_append(headers, AXIS2_HTTP_HEADER_USER_AGENT_AXIS2C);
    headers = curl_slist_append(headers, AXIS2_HTTP_HEADER_ACCEPT_);
    headers = curl_slist_append(headers, AXIS2_HTTP_HEADER_EXPECT_);

    if (AXIS2_TRUE == axis2_msg_ctx_get_doing_rest(msg_ctx, env))
        is_soap = AXIS2_FALSE;
    else
        is_soap = AXIS2_TRUE;

    if (!is_soap)
    {
        soap_body = axiom_soap_envelope_get_body(out, env);
        if (!soap_body)
        {
            AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "%s",
                            AXIS2_ERROR_GET_MESSAGE(env->error));
            axis2_libcurl_free(data, env);
            return AXIS2_FAILURE;
        }
        body_node = axiom_soap_body_get_base_node(soap_body, env);
        if (!body_node)
        {
            axis2_libcurl_free(data, env);
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
                write_xml_declaration_attr = (axiom_attribute_t *) axutil_generic_obj_get_value (obj,
                                                                                         env);
            }
            if (write_xml_declaration_attr)
            {
                write_xml_declaration_attr_value = axiom_attribute_get_value (write_xml_declaration_attr, env);
            }
            if (write_xml_declaration_attr_value && 0 == axutil_strcasecmp (write_xml_declaration_attr_value, AXIS2_VALUE_TRUE))
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
                if ('\"' != *soap_action)
                {
                    axis2_char_t *tmp_soap_action = NULL;
                    tmp_soap_action =
                        AXIS2_MALLOC(env->allocator,
                                     (axutil_strlen(soap_action) +
                                      5) * sizeof(axis2_char_t));
                    sprintf(tmp_soap_action, "\"%s\"", soap_action);
                    headers = curl_slist_append(headers,
                                                axutil_stracat(env,
                                                               soap_action_header,
                                                               tmp_soap_action));
                    AXIS2_FREE(env->allocator, tmp_soap_action);
                }
                else
                {
                    headers = curl_slist_append(headers,
                                                axutil_stracat(env,
                                                               soap_action_header,
                                                               soap_action));
                }
            }

            if (doing_mtom)
            {
                axiom_output_flush(om_output, env, &output_stream,
                                   &output_stream_size);
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
                                                           ";action=\"");
                        content_type = temp_content_type;
                        temp_content_type = axutil_stracat (env,
                                                            content_type,
                                                            soap_action);
                        AXIS2_FREE (env->allocator, content_type);
                        content_type = temp_content_type;
                        temp_content_type =
                            axutil_stracat (env, content_type, "\"");
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
                content_type = axutil_stracat(env, content_type, ";charset=");
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
                content_type = axutil_stracat(env, content_type, ";charset=");
                temp_content_type =
                    axutil_stracat(env, content_type, char_set_enc);
                AXIS2_FREE(env->allocator, content_type);
                content_type = temp_content_type;
                if (axutil_strcmp(soap_action, ""))
                {
                    temp_content_type =
                        axutil_stracat(env, content_type, ";action=");
                    AXIS2_FREE(env->allocator, content_type);
                    content_type = temp_content_type;
                    temp_content_type =
                        axutil_stracat(env, content_type, soap_action);
                    AXIS2_FREE(env->allocator, content_type);
                    content_type = temp_content_type;
                }
                temp_content_type = axutil_stracat(env, content_type, ";");
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
                (axutil_property_t *) axis2_msg_ctx_get_property(msg_ctx, env,
                                                                 AXIS2_USER_DEFINED_HTTP_HEADER_CONTENT_TYPE);

            if (content_type_property)
            {
                content_type_hash =
                    (axutil_hash_t *)
                    axutil_property_get_value(content_type_property, env);
                if (content_type_hash)
                    content_type_value =
                        (char *) axutil_hash_get(content_type_hash,
                                                 AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                                 AXIS2_HASH_KEY_STRING);
            }
            if (content_type_value)
                content_type = content_type_value;
            else
                content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML;

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
            headers =
                curl_slist_append(headers,
                                  axutil_stracat(env, content_len, tmp_buf));
            headers =
                curl_slist_append(headers,
                                  axutil_stracat(env, content, content_type));
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
            /*curl_easy_setopt(handler, CURLOPT_PUT, 1);*/
            curl_easy_setopt(handler, CURLOPT_CUSTOMREQUEST, "PUT");
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
        url_encode = axutil_strcat(env, str_url, "?", request_param, NULL);
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
            curl_easy_setopt(handler, CURLOPT_CUSTOMREQUEST, "DELETE");
        }
        curl_easy_setopt(handler, CURLOPT_URL, url_encode);
    }

/* 	curl_easy_setopt (handler, CURLOPT_VERBOSE, 1); */
    curl_easy_setopt(handler, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(handler, CURLOPT_WRITEFUNCTION,
                     axis2_libcurl_write_memory_callback);
    curl_easy_setopt(handler, CURLOPT_WRITEDATA, data);

    curl_easy_setopt (handler, CURLOPT_HEADERFUNCTION, axis2_libcurl_header_callback);

    curl_easy_setopt (handler, CURLOPT_WRITEHEADER, data->header);
    if (curl_easy_perform(handler)) 
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "%s", &data->errorbuffer);
        AXIS2_ERROR_SET (env->error, AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR,
                         AXIS2_FAILURE);
        axis2_libcurl_free(data, env);
        return AXIS2_FAILURE;
    }

/* 	curl_slist_free_all (headers); */

/* 	curl_easy_cleanup (handler); */

    in_stream = axutil_stream_create_libcurl(env, data->memory, data->size);
    trans_in_property = axutil_property_create(env);
    axutil_property_set_scope(trans_in_property, env, AXIS2_SCOPE_REQUEST);
    axutil_property_set_free_func(trans_in_property, env,
                                  axutil_stream_free_void_arg);
    axutil_property_set_value(trans_in_property, env, in_stream);
    axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_TRANSPORT_IN,
                               trans_in_property);

    if (axutil_array_list_size((data->header)->alist, env) > 0)
    {
        status_line_str = axutil_array_list_get((data->header)->alist, env, 0);
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
    content_type = axis2_libcurl_get_content_type(data->header, env); 
    if (content_type)
    {    
        if (strstr (content_type, AXIS2_HTTP_HEADER_ACCEPT_MULTIPART_RELATED)
            && strstr (content_type, AXIS2_HTTP_HEADER_XOP_XML))
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

    buffer_size = (unsigned int) axis2_libcurl_get_content_length(data->header, env);
    response_length = AXIS2_MALLOC (env->allocator, sizeof (int));
    memcpy (response_length, &buffer_size, sizeof (int));
    property = axutil_property_create (env);
    axutil_property_set_scope (property, env, AXIS2_SCOPE_REQUEST);
    axutil_property_set_value (property, env, response_length);
    axis2_msg_ctx_set_property (msg_ctx, env,
                                AXIS2_HTTP_HEADER_CONTENT_LENGTH, property);

    curl_slist_free_all (headers);
    ref--;
    if (ref == 0)
    {
        curl_easy_cleanup (handler);
    }

    axis2_libcurl_free(data, env);
    return AXIS2_SUCCESS;
}

size_t
axis2_libcurl_write_memory_callback(
    void *ptr,
    size_t size,
    size_t nmemb,
    void *data)
{
    size_t realsize = size * nmemb;
    axis2_libcurl_t *mem = (axis2_libcurl_t *) data;
    axis2_char_t *buffer =
        (axis2_char_t *) AXIS2_MALLOC(mem->env->allocator,
                                      mem->size + realsize + 1);
    if (buffer)
    {
        if (mem->size)
            memcpy(&(buffer[0]), mem->memory, mem->size);

        memcpy(&(buffer[mem->size]), ptr, realsize);
        mem->size += realsize;
        buffer[mem->size] = 0;
        mem->memory = buffer;
    }
    return realsize;
}

size_t
axis2_libcurl_header_callback(
    void *ptr,
    size_t size,
    size_t nmemb,
    void *data)
{
    axis2_char_t *memory;
    size_t realsize = size * nmemb;
    axis2_libcurl_header_t *mem = (axis2_libcurl_header_t *) data;
    memory = (axis2_char_t *)AXIS2_MALLOC(mem->env->allocator, realsize + 1);
    if (memory)
    {
        memcpy(&(memory[0]), ptr, realsize);
        memory[realsize] = 0;
        axutil_array_list_add(mem->alist, mem->env,
                              axutil_strdup(mem->env, memory));
        AXIS2_FREE(mem->env->allocator, memory);
    }
    return realsize;
}

axis2_libcurl_t *
axis2_libcurl_create(
    const axutil_env_t * env)
{
    axis2_libcurl_t *curl = NULL;
    curl =
        (axis2_libcurl_t *) AXIS2_MALLOC(env->allocator,
                                         sizeof(axis2_libcurl_t));
    if (curl) 
    {
        curl->size = 0;
        curl->alist = axutil_array_list_create(env, 7);
        curl->header = (axis2_libcurl_header_t *) AXIS2_MALLOC(env->allocator,
                                         sizeof(axis2_libcurl_header_t));
        if (curl->header)
        {
            (curl->header)->alist = axutil_array_list_create(env, 15);
            (curl->header)->env = env;
        }
        curl->env = env;
    }
    return curl;
}

void 
axis2_libcurl_free(
    axis2_libcurl_t *curl,
    const axutil_env_t * env)
{
    if (curl->alist)
    {
        axutil_array_list_free(curl->alist, env);
        curl->alist = NULL;
    }

    if (curl->header)
    {
       if((curl->header)->alist)
       {
           axutil_array_list_free((curl->header)->alist, env);
           (curl->header)->alist = NULL;
       }
       AXIS2_FREE(env->allocator, curl->header);
    }
    
    AXIS2_FREE(env->allocator, curl);
}
 
axis2_http_header_t *
axis2_libcurl_get_first_header(
    axis2_libcurl_header_t * payload,
    const axutil_env_t * env,
    const axis2_char_t * str)
{
    axis2_http_header_t *tmp_header = NULL;
    axis2_char_t *tmp_header_str = NULL;
    axis2_char_t *tmp_name = NULL;
    int i = 0;
    int count = 0;
    axutil_array_list_t *header_group = NULL;
 
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, str, NULL);

    header_group = payload->alist;
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
    }
    return NULL;

}

axis2_ssize_t
axis2_libcurl_get_content_length(
    axis2_libcurl_header_t * payload,
    const axutil_env_t * env)
{
    axis2_http_header_t *tmp_header = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    tmp_header = axis2_libcurl_get_first_header
        (payload, env, AXIS2_HTTP_HEADER_CONTENT_LENGTH);
    if (tmp_header)
    {
        return AXIS2_ATOI(axis2_http_header_get_value(tmp_header, env));
    }
    return -1;
}

axis2_char_t *
axis2_libcurl_get_content_type(
    axis2_libcurl_header_t * payload,
    const axutil_env_t * env)
{
    axis2_http_header_t *tmp_header = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    tmp_header = axis2_libcurl_get_first_header
        (payload, env, AXIS2_HTTP_HEADER_CONTENT_TYPE);
    if (tmp_header)
        return axis2_http_header_get_value(tmp_header, env);

    return AXIS2_HTTP_HEADER_ACCEPT_TEXT_PLAIN;
}
