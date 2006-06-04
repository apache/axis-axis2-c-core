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
 
#include <axis2_simple_http_svr_conn.h>
#include <axis2_http_transport.h>
#include <axis2_string.h>
#include <axis2_network_handler.h>
#include <axis2_http_chunked_stream.h>
#include <platforms/axis2_platform_auto_sense.h>

/** 
 * @brief Simple HTTP Server Connection struct impl
 *    Axis2 Simple HTTP Server Connection impl  
 */
typedef struct axis2_simple_http_svr_conn_impl 
                                            axis2_simple_http_svr_conn_impl_t;
  
struct axis2_simple_http_svr_conn_impl
{
    axis2_simple_http_svr_conn_t svr_conn;
    int socket;
    axis2_stream_t *stream;
    axis2_bool_t keep_alive;
};

#define AXIS2_INTF_TO_IMPL(svr_conn)\
                ((axis2_simple_http_svr_conn_impl_t *)(svr_conn))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_close(axis2_simple_http_svr_conn_t *svr_conn, 
                  const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_simple_http_svr_conn_is_open(axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_set_keep_alive 
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env, axis2_bool_t keep_alive);

axis2_bool_t AXIS2_CALL 
axis2_simple_http_svr_conn_is_keep_alive(axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env);
                                        
axis2_stream_t* AXIS2_CALL 
axis2_simple_http_svr_conn_get_stream(axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env);
                                        
axis2_http_response_writer_t* AXIS2_CALL 
axis2_simple_http_svr_conn_get_writer(axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env);
                                        
axis2_http_simple_request_t* AXIS2_CALL 
axis2_simple_http_svr_conn_read_request(axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env);
                                        
axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_write_response
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env, 
                        axis2_http_simple_response_t *response);
                  
axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_set_rcv_timeout
                      (axis2_simple_http_svr_conn_t *svr_conn, 
                      const axis2_env_t *env, int timeout);
                  
axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_set_snd_timeout
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env, int timeout);
int AXIS2_CALL 
axis2_simple_http_svr_conn_read_request_body
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env, axis2_char_t *buffer, int size);
                        
axis2_char_t * AXIS2_CALL
axis2_simple_http_svr_conn_get_svr_ip
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env);
                                
axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_free(axis2_simple_http_svr_conn_t *svr_conn, 
                                const axis2_env_t *env);
                                
/***************************** End of function headers ************************/

axis2_simple_http_svr_conn_t* AXIS2_CALL
axis2_simple_http_svr_conn_create (const axis2_env_t *env, int sockfd)
{
    axis2_simple_http_svr_conn_impl_t *svr_conn_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    svr_conn_impl = (axis2_simple_http_svr_conn_impl_t *)
                        AXIS2_MALLOC (env->allocator,
                            sizeof(axis2_simple_http_svr_conn_impl_t));
    
    if(NULL == svr_conn_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    svr_conn_impl->socket = sockfd;
    svr_conn_impl->stream = NULL;
    svr_conn_impl->keep_alive = AXIS2_FALSE;
   
   if(-1 != svr_conn_impl->socket)
   {
      svr_conn_impl->stream = axis2_stream_create_socket(env, 
                  svr_conn_impl->socket);
      if(NULL == svr_conn_impl->stream)
      {
         axis2_simple_http_svr_conn_free((axis2_simple_http_svr_conn_t *)
                                        svr_conn_impl, env);
         return NULL;
      }
   }
    svr_conn_impl->svr_conn.ops = AXIS2_MALLOC(env->allocator,
        sizeof(axis2_simple_http_svr_conn_ops_t));
    if(NULL == svr_conn_impl->svr_conn.ops)
    {
        axis2_simple_http_svr_conn_free((axis2_simple_http_svr_conn_t *)
                                        svr_conn_impl, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    svr_conn_impl->svr_conn.ops->close = axis2_simple_http_svr_conn_close;
    svr_conn_impl->svr_conn.ops->is_open = axis2_simple_http_svr_conn_is_open;
    svr_conn_impl->svr_conn.ops->set_keep_alive =
                        axis2_simple_http_svr_conn_set_keep_alive;
    svr_conn_impl->svr_conn.ops->is_keep_alive = 
                        axis2_simple_http_svr_conn_is_keep_alive;
    svr_conn_impl->svr_conn.ops->get_stream = 
                        axis2_simple_http_svr_conn_get_stream;
    svr_conn_impl->svr_conn.ops->get_writer = 
                        axis2_simple_http_svr_conn_get_writer;
    svr_conn_impl->svr_conn.ops->read_request = 
                        axis2_simple_http_svr_conn_read_request;
    svr_conn_impl->svr_conn.ops->write_response =
                        axis2_simple_http_svr_conn_write_response;
   svr_conn_impl->svr_conn.ops->set_rcv_timeout = 
                  axis2_simple_http_svr_conn_set_rcv_timeout;
   svr_conn_impl->svr_conn.ops->set_snd_timeout = 
                  axis2_simple_http_svr_conn_set_snd_timeout;
    svr_conn_impl->svr_conn.ops->get_svr_ip =
                        axis2_simple_http_svr_conn_get_svr_ip;
    svr_conn_impl->svr_conn.ops->free = axis2_simple_http_svr_conn_free;
    return &(svr_conn_impl->svr_conn);
}


axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_free(axis2_simple_http_svr_conn_t *svr_conn, 
                                const axis2_env_t *env)
{
    axis2_simple_http_svr_conn_impl_t *svr_conn_impl = 
                                    AXIS2_INTF_TO_IMPL(svr_conn);
    
    axis2_simple_http_svr_conn_close(svr_conn, env);
   if(NULL != svr_conn->ops)
    {
        AXIS2_FREE(env->allocator, svr_conn->ops);
        svr_conn->ops = NULL;
    }
    
    AXIS2_FREE(env->allocator, svr_conn_impl);
    svr_conn_impl = NULL;
    
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_close(axis2_simple_http_svr_conn_t *svr_conn, 
                                    const axis2_env_t *env)
{
    axis2_simple_http_svr_conn_impl_t *svr_conn_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svr_conn_impl = AXIS2_INTF_TO_IMPL(svr_conn);
   
   AXIS2_STREAM_FREE(svr_conn_impl->stream, env);
    if(-1 != svr_conn_impl->socket)
    {
      axis2_network_handler_close_socket(env, svr_conn_impl->socket);
        svr_conn_impl->socket = -1;
    }
    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL
axis2_simple_http_svr_conn_is_open(axis2_simple_http_svr_conn_t *svr_conn, 
                                    const axis2_env_t *env)
{
    axis2_simple_http_svr_conn_impl_t *svr_conn_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svr_conn_impl = AXIS2_INTF_TO_IMPL(svr_conn);
    if(-1 != svr_conn_impl->socket)
    {
        return AXIS2_TRUE;
    }
    else
    {
        return AXIS2_FALSE;
    }
    return AXIS2_FALSE;
}


axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_set_keep_alive 
                                    (axis2_simple_http_svr_conn_t *svr_conn, 
                                    const axis2_env_t *env, axis2_bool_t keep_alive)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(svr_conn)->keep_alive = keep_alive;
    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL 
axis2_simple_http_svr_conn_is_keep_alive(axis2_simple_http_svr_conn_t *svr_conn, 
                                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    return AXIS2_INTF_TO_IMPL(svr_conn)->keep_alive;
}


axis2_stream_t* AXIS2_CALL 
axis2_simple_http_svr_conn_get_stream(axis2_simple_http_svr_conn_t *svr_conn, 
                                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svr_conn)->stream;
}


axis2_http_response_writer_t* AXIS2_CALL 
axis2_simple_http_svr_conn_get_writer(axis2_simple_http_svr_conn_t *svr_conn, 
                                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return axis2_http_response_writer_create(env, 
                                        AXIS2_INTF_TO_IMPL(svr_conn)->stream);
    
}


axis2_http_simple_request_t* AXIS2_CALL 
axis2_simple_http_svr_conn_read_request(axis2_simple_http_svr_conn_t *svr_conn, 
                                        const axis2_env_t *env)
{
   axis2_char_t str_line[1024];
   axis2_char_t tmp_buf[3];
   axis2_simple_http_svr_conn_impl_t *svr_conn_impl = NULL;
   int read = -1;
   axis2_bool_t end_of_line = AXIS2_FALSE;
   axis2_bool_t end_of_headers = AXIS2_FALSE;
   axis2_http_request_line_t *request_line = NULL;
   axis2_http_simple_request_t *request = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);
   
    svr_conn_impl = AXIS2_INTF_TO_IMPL(svr_conn);
   memset(str_line, 0, 1024);
   while((read = AXIS2_STREAM_READ(svr_conn_impl->stream, env, tmp_buf, 
                  1)) > 0)
   {
      tmp_buf[read] = '\0';
      strcat(str_line, tmp_buf);
      if(0 != strstr(str_line, AXIS2_HTTP_CRLF))
      {
         end_of_line = AXIS2_TRUE;
      }
      if(AXIS2_TRUE == end_of_line)
      {
         break;
      }
   }
    if(NULL != str_line)
    {
        if(0 != AXIS2_STRNCASECMP(str_line, "GET", 3) && 0 != 
                        AXIS2_STRNCASECMP(str_line, "POST", 4))
        {
            char write_buf[512];
            sprintf(write_buf, "%s %s\r\n%s: close\r\n\r\n", 
                        AXIS2_HTTP_HEADER_PROTOCOL_11, 
                        AXIS2_HTTP_RESPONSE_BAD_REQUEST, 
                        AXIS2_HTTP_HEADER_CONNECTION);
            AXIS2_STREAM_WRITE(svr_conn_impl->stream, env, write_buf,
                        AXIS2_STRLEN(write_buf) + 1);
            return NULL;
        }
    }
   request_line = axis2_http_request_line_parse_line(env, str_line);
   if(NULL == request_line)
   {
      AXIS2_ERROR_SET(env->error, 
                  AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE, 
                  AXIS2_FAILURE);
      return NULL;
   }
   request = axis2_http_simple_request_create(env, request_line, NULL , 0, 
                  svr_conn_impl->stream);
   /* now read the headers */
   memset(str_line, 0, 1024);
   end_of_line = AXIS2_FALSE;
   while(AXIS2_FALSE == end_of_headers)
   {
      while((read = AXIS2_STREAM_READ(svr_conn_impl->stream, env, tmp_buf, 
                     1)) > 0)
      {
         tmp_buf[read] = '\0';
         strcat(str_line, tmp_buf);
         if(0 != strstr(str_line, AXIS2_HTTP_CRLF))
         {
            end_of_line = AXIS2_TRUE;
            break;
         }
      }
      if(AXIS2_TRUE == end_of_line)
      {
         if(0 == AXIS2_STRCMP(str_line, AXIS2_HTTP_CRLF))
         {
            end_of_headers = AXIS2_TRUE;
         }
         else
         {
            axis2_http_header_t *tmp_header = 
                  axis2_http_header_create_by_str(env, str_line);
            memset(str_line, 0, 1024);
            if(NULL != tmp_header)
            {
               AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(request, env, 
                  tmp_header);
            }
         }
      }
      end_of_line = AXIS2_FALSE;
   }
    return request;
}


axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_write_response
                  (axis2_simple_http_svr_conn_t *svr_conn, 
                  const axis2_env_t *env, 
                  axis2_http_simple_response_t *response)
{
   axis2_http_response_writer_t *response_writer = NULL;
   axis2_simple_http_svr_conn_impl_t *svr_conn_impl = NULL;
   axis2_array_list_t *headers = NULL;
   axis2_stream_t *response_stream = NULL;
   axis2_char_t *response_body = NULL;
   int body_size = 0;
   int i = 0;
   axis2_http_header_t *enc_header = NULL;
   axis2_bool_t chuked_encoding = AXIS2_FALSE;
   axis2_char_t *status_line = NULL;
   
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK(env->error, response, AXIS2_FAILURE);
   
   svr_conn_impl = AXIS2_INTF_TO_IMPL(svr_conn);
   response_writer = axis2_http_response_writer_create(env, 
                  svr_conn_impl->stream);
   if(NULL == response_writer)
   {
      return AXIS2_FAILURE;
   }
   enc_header = AXIS2_HTTP_SIMPLE_RESPONSE_GET_FIRST_HEADER(response, env, 
                  AXIS2_HTTP_HEADER_TRANSFER_ENCODING);
   if(NULL != enc_header)
   {
      axis2_char_t *enc_value = AXIS2_HTTP_HEADER_GET_VALUE(enc_header, env);
      if(NULL != enc_value)
      {
         if(0 == AXIS2_STRCMP(enc_value, 
                  AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
         {
            chuked_encoding = AXIS2_TRUE;
            /* remove the content length header */
            if(AXIS2_TRUE == AXIS2_HTTP_SIMPLE_RESPONSE_CONTAINS_HEADER(
                  response,env, AXIS2_HTTP_HEADER_CONTENT_LENGTH))
            {
               AXIS2_HTTP_SIMPLE_RESPONSE_REMOVE_HEADERS(response, env, 
                  AXIS2_HTTP_HEADER_CONTENT_LENGTH);
            }
         }
      }
   }
   status_line = AXIS2_HTTP_SIMPLE_RESPONSE_GET_STATUS_LINE(response, env);
   if(NULL == status_line)
   {
      AXIS2_ERROR_SET(env->error, 
                  AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE, 
                  AXIS2_FAILURE);
      AXIS2_HTTP_RESPONSE_WRITER_FREE(response_writer, env);
      response_writer = NULL;
      return AXIS2_FAILURE;
   }
   AXIS2_HTTP_RESPONSE_WRITER_PRINT_STR(response_writer, env, status_line);
   headers = AXIS2_HTTP_SIMPLE_RESPONSE_GET_HEADERS(response, env);
   
   if(NULL != headers)
   {
      for(i = 0; i < AXIS2_ARRAY_LIST_SIZE(headers, env); i++)
      {
         axis2_http_header_t *header = NULL;
         header = (axis2_http_header_t *)AXIS2_ARRAY_LIST_GET(headers, env, 
                     i);
         if(NULL != header)
         {
                axis2_char_t *header_ext_form =  
                        AXIS2_HTTP_HEADER_TO_EXTERNAL_FORM(
                        (axis2_http_header_t*)header, env);

            AXIS2_HTTP_RESPONSE_WRITER_PRINT_STR(response_writer, env, 
                     header_ext_form);
                AXIS2_FREE(env->allocator, header_ext_form);
         }
       }
    }
   AXIS2_HTTP_RESPONSE_WRITER_PRINTLN(response_writer, env);
   
   response_stream = AXIS2_HTTP_SIMPLE_RESPONSE_GET_BODY(response, env);
   body_size = AXIS2_HTTP_SIMPLE_RESPONSE_GET_BODY_BYTES(response, env, 
                  &response_body);
   if(body_size <= 0)
   {
        AXIS2_HTTP_RESPONSE_WRITER_FREE(response_writer, env);
      return AXIS2_SUCCESS;
   }
   if(AXIS2_FALSE == chuked_encoding)
   {
      axis2_status_t write_stat = AXIS2_FAILURE;
      write_stat = AXIS2_HTTP_RESPONSE_WRITER_PRINTLN_STR(response_writer, 
                  env, response_body);
      if(AXIS2_SUCCESS != write_stat)
      {
         AXIS2_ERROR_SET(env->error, AXIS2_ERROR_WRITING_RESPONSE, 
                  AXIS2_FAILURE);
            AXIS2_HTTP_RESPONSE_WRITER_FREE(response_writer, env);
         return AXIS2_FAILURE;
      }
   }
   else
   {
      axis2_http_chunked_stream_t *chunked_stream = NULL;
      int left = body_size;
      chunked_stream = axis2_http_chunked_stream_create(env, 
                  svr_conn_impl->stream);
      while(left > 0)
      {
         left -= AXIS2_HTTP_CHUNKED_STREAM_WRITE(chunked_stream, env, 
                  response_body, body_size);
      }
      AXIS2_HTTP_CHUNKED_STREAM_WRITE_LAST_CHUNK(chunked_stream, env);
      AXIS2_HTTP_CHUNKED_STREAM_FREE(chunked_stream, env);
   }
    AXIS2_FREE(env->allocator, response_body);
    AXIS2_HTTP_RESPONSE_WRITER_FREE(response_writer, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_set_rcv_timeout
                      (axis2_simple_http_svr_conn_t *svr_conn, 
                      const axis2_env_t *env, int timeout)
{
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   return axis2_network_handler_set_sock_option(env, 
                  AXIS2_INTF_TO_IMPL(svr_conn)->socket, SO_RCVTIMEO, 
                  timeout);
   
   
}
                  
axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_set_snd_timeout
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env, int timeout)
{
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   return axis2_network_handler_set_sock_option(env, 
                  AXIS2_INTF_TO_IMPL(svr_conn)->socket, SO_SNDTIMEO, 
                  timeout);
}

axis2_char_t* AXIS2_CALL 
axis2_simple_http_svr_conn_get_svr_ip(
                        axis2_simple_http_svr_conn_t *svr_conn, 
                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return axis2_network_handler_get_svr_ip(env, 
                        AXIS2_INTF_TO_IMPL(svr_conn)->socket);
}
