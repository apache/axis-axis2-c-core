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
#include <unistd.h>

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
                                    axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_simple_http_svr_conn_is_open(axis2_simple_http_svr_conn_t *svr_conn, 
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_set_keep_alive 
                                    (axis2_simple_http_svr_conn_t *svr_conn, 
                                    axis2_env_t **env, axis2_bool_t keep_alive);

axis2_bool_t AXIS2_CALL 
axis2_simple_http_svr_conn_is_keep_alive(axis2_simple_http_svr_conn_t *svr_conn, 
                                        axis2_env_t **env);
                                        
axis2_stream_t* AXIS2_CALL 
axis2_simple_http_svr_conn_get_stream(axis2_simple_http_svr_conn_t *svr_conn, 
                                        axis2_env_t **env);
                                        
axis2_http_response_writer_t* AXIS2_CALL 
axis2_simple_http_svr_conn_get_writer(axis2_simple_http_svr_conn_t *svr_conn, 
                                        axis2_env_t **env);
                                        
axis2_http_simple_request_t* AXIS2_CALL 
axis2_simple_http_svr_conn_read_request(axis2_simple_http_svr_conn_t *svr_conn, 
                                        axis2_env_t **env);
                                        
axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_write_response
                                (axis2_simple_http_svr_conn_t *svr_conn, 
                                axis2_env_t **env, 
                                const axis2_http_simple_response_t *response);
                                
axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_free(axis2_simple_http_svr_conn_t *svr_conn, 
                                axis2_env_t **env);
                                
/***************************** End of function headers ************************/

axis2_simple_http_svr_conn_t* AXIS2_CALL
axis2_simple_http_svr_conn_create (axis2_env_t **env, int sockfd)
{
    AXIS2_ENV_CHECK(env, NULL);
    axis2_simple_http_svr_conn_impl_t *svr_conn_impl = 
                                (axis2_simple_http_svr_conn_impl_t *)
                                AXIS2_MALLOC ((*env)->allocator, 
                                sizeof(axis2_simple_http_svr_conn_impl_t));
    
    if(NULL == svr_conn_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    svr_conn_impl->socket = sockfd;
    svr_conn_impl->stream = NULL;
    svr_conn_impl->keep_alive = AXIS2_FALSE;
    
    svr_conn_impl->svr_conn.ops = AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_simple_http_svr_conn_ops_t));
    if(NULL == svr_conn_impl->svr_conn.ops)
    {
        axis2_simple_http_svr_conn_free((axis2_simple_http_svr_conn_t *)
                                        svr_conn_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
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
    svr_conn_impl->svr_conn.ops->free = axis2_simple_http_svr_conn_free;
    return &(svr_conn_impl->svr_conn);
}


axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_free(axis2_simple_http_svr_conn_t *svr_conn, 
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svr_conn, env, AXIS2_FAILURE);
    axis2_simple_http_svr_conn_impl_t *svr_conn_impl = 
                                    AXIS2_INTF_TO_IMPL(svr_conn);
    if(NULL != svr_conn_impl->stream)
    {
        /* TODO free the stream*/
        AXIS2_FREE((*env)->allocator, svr_conn_impl->stream);
        svr_conn_impl->stream = NULL;
    }
    if(-1 != svr_conn_impl->socket)
    {
        close(svr_conn_impl->socket);
        svr_conn_impl->socket = -1;
    }
    if(NULL != svr_conn->ops)
    {
        AXIS2_FREE((*env)->allocator, svr_conn->ops);
        svr_conn->ops = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, svr_conn_impl);
    svr_conn_impl = NULL;
    
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_close(axis2_simple_http_svr_conn_t *svr_conn, 
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svr_conn, env, AXIS2_FAILURE);
    axis2_simple_http_svr_conn_impl_t *svr_conn_impl = 
                                    AXIS2_INTF_TO_IMPL(svr_conn);
    if(-1 != svr_conn_impl->socket)
    {
        close(svr_conn_impl->socket);
        svr_conn_impl->socket = -1;
    }
    /*
        TODO close the stream
    */
    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL
axis2_simple_http_svr_conn_is_open(axis2_simple_http_svr_conn_t *svr_conn, 
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svr_conn, env, AXIS2_FAILURE);
    axis2_simple_http_svr_conn_impl_t *svr_conn_impl = 
                                    AXIS2_INTF_TO_IMPL(svr_conn);
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
                                    axis2_env_t **env, axis2_bool_t keep_alive)
{
    AXIS2_FUNC_PARAM_CHECK(svr_conn, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(svr_conn)->keep_alive = keep_alive;
    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL 
axis2_simple_http_svr_conn_is_keep_alive(axis2_simple_http_svr_conn_t *svr_conn, 
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svr_conn, env, AXIS2_FAILURE);
    
    return AXIS2_INTF_TO_IMPL(svr_conn)->keep_alive;
}


axis2_stream_t* AXIS2_CALL 
axis2_simple_http_svr_conn_get_stream(axis2_simple_http_svr_conn_t *svr_conn, 
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svr_conn, env, NULL);
    return AXIS2_INTF_TO_IMPL(svr_conn)->stream;
}


axis2_http_response_writer_t* AXIS2_CALL 
axis2_simple_http_svr_conn_get_writer(axis2_simple_http_svr_conn_t *svr_conn, 
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svr_conn, env, NULL);
    return axis2_http_response_writer_create(env, 
                                        AXIS2_INTF_TO_IMPL(svr_conn)->stream);
    
}


axis2_http_simple_request_t* AXIS2_CALL 
axis2_simple_http_svr_conn_read_request(axis2_simple_http_svr_conn_t *svr_conn, 
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svr_conn, env, NULL);
    /*
        TODO stream dependant code
    */
    return NULL;
}


axis2_status_t AXIS2_CALL 
axis2_simple_http_svr_conn_write_response
                                (axis2_simple_http_svr_conn_t *svr_conn, 
                                axis2_env_t **env, 
                                const axis2_http_simple_response_t *response)
{
    AXIS2_FUNC_PARAM_CHECK(svr_conn, env, AXIS2_FAILURE);
    /*
        TODO stream dependant code
    */
    return AXIS2_SUCCESS;
}
