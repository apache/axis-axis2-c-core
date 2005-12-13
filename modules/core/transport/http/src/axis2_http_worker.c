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
 
#include <axis2_http_worker.h>
#include <axis2_string.h>
#include <axis2_http_transport.h>

/** 
 * @brief HTTP Worker struct impl
 *	Axis2 HTTP Worker impl  
 */
typedef struct axis2_http_worker_impl axis2_http_worker_impl_t;  
  
struct axis2_http_worker_impl
{
	axis2_http_worker_t http_worker;
	axis2_conf_ctx_t *conf_ctx;
};

#define AXIS2_INTF_TO_IMPL(http_worker) ((axis2_http_worker_impl_t *)\
                                                                (http_worker))

/***************************** Function headers *******************************/

axis2_bool_t AXIS2_CALL 
axis2_http_worker_process_request(axis2_http_worker_t *http_worker, 
				axis2_env_t **env, 
				axis2_simple_http_svr_conn_t *svr_conn, 
				axis2_http_simple_request_t *simple_request);
    
axis2_status_t AXIS2_CALL 
axis2_http_worker_set_response_headers(axis2_http_worker_t *http_worker, 
						axis2_env_t **env, 
						axis2_simple_http_svr_conn_t *svr_conn, 
						axis2_http_simple_request_t *simple_request,
                        axis2_http_simple_response_t *simple_response,
						axis2_ssize_t content_length);

axis2_status_t AXIS2_CALL 
axis2_http_worker_set_trasnport_out_config(axis2_http_worker_t *http_worker, 
						axis2_env_t **env, 
						axis2_conf_ctx_t *conf_ctx, 
						axis2_http_simple_response_t *simple_response);
	
axis2_status_t AXIS2_CALL 
axis2_http_worker_free(axis2_http_worker_t *http_worker, 
								axis2_env_t **env);
								
/***************************** End of function headers ************************/

axis2_http_worker_t * AXIS2_CALL 
axis2_http_worker_create (axis2_env_t **env, axis2_conf_ctx_t *conf_ctx)
{
    AXIS2_ENV_CHECK(env, NULL);
    axis2_http_worker_impl_t *http_worker_impl = (axis2_http_worker_impl_t *)
        AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_http_worker_impl_t));
	
    if(NULL == http_worker_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    http_worker_impl->conf_ctx = conf_ctx;
    
    http_worker_impl->http_worker.ops = AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_http_worker_ops_t));
    if(NULL == http_worker_impl->http_worker.ops)
	{
		axis2_http_worker_free((axis2_http_worker_t*)http_worker_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    http_worker_impl->http_worker.ops->process_request = 
                                    axis2_http_worker_process_request;
    http_worker_impl->http_worker.ops->set_response_headers = 
                                    axis2_http_worker_set_response_headers;
    http_worker_impl->http_worker.ops->set_trasnport_out_config =
                                    axis2_http_worker_set_trasnport_out_config;
    http_worker_impl->http_worker.ops->free = axis2_http_worker_free;
    
	return &(http_worker_impl->http_worker);
}

axis2_status_t AXIS2_CALL
axis2_http_worker_free (axis2_http_worker_t *http_worker, axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(http_worker, env, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(http_worker)->conf_ctx = NULL;
    
    if(NULL != http_worker->ops)
        AXIS2_FREE((*env)->allocator, http_worker->ops);
    
	AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(http_worker));
    
	return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL 
axis2_http_worker_process_request(axis2_http_worker_t *http_worker, 
				axis2_env_t **env, 
				axis2_simple_http_svr_conn_t *svr_conn, 
				axis2_http_simple_request_t *simple_request)
{
    AXIS2_FUNC_PARAM_CHECK(http_worker, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svr_conn, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, simple_request, AXIS2_FAILURE);
    /*
        TODO create the engine and process the request
    */
    return AXIS2_TRUE;
}


axis2_status_t AXIS2_CALL 
axis2_http_worker_set_response_headers(axis2_http_worker_t *http_worker, 
						axis2_env_t **env, 
						axis2_simple_http_svr_conn_t *svr_conn, 
						axis2_http_simple_request_t *simple_request,
                        axis2_http_simple_response_t *simple_response,
						axis2_ssize_t content_length)
{
    AXIS2_FUNC_PARAM_CHECK(http_worker, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svr_conn, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, simple_request, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, simple_response, AXIS2_FAILURE);
    if(AXIS2_FALSE == AXIS2_HTTP_SIMPLE_RESPONSE_CONTAINS_HEADER
                    (simple_response, env, AXIS2_HTTP_HEADER_CONNECTION))
    {
        axis2_http_header_t *conn_header = 
                    AXIS2_HTTP_SIMPLE_REQUEST_GET_FIRST_HEADER(simple_request,
                    env, AXIS2_HTTP_HEADER_CONNECTION);
        if(NULL != conn_header)
        {
            if(0 == AXIS2_STRCASECMP(AXIS2_HTTP_HEADER_GET_VALUE(conn_header, 
                    env), AXIS2_HTTP_HEADER_CONNECTION_KEEPALIVE))
            {
                axis2_http_header_t *header = axis2_http_header_create(env,
                    AXIS2_HTTP_HEADER_CONNECTION, 
                    AXIS2_HTTP_HEADER_CONNECTION_KEEPALIVE);
                AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(simple_response, env, 
                    header);
                AXIS2_SIMPLE_HTTP_SVR_CONN_SET_KEEP_ALIVE(svr_conn, env, 
                    AXIS2_TRUE);
            }
            if(0 == AXIS2_STRCASECMP(AXIS2_HTTP_HEADER_GET_VALUE(conn_header, 
                    env), AXIS2_HTTP_HEADER_CONNECTION_CLOSE))
            {
                axis2_http_header_t *header = axis2_http_header_create(env,
                    AXIS2_HTTP_HEADER_CONNECTION, 
                    AXIS2_HTTP_HEADER_CONNECTION_CLOSE);
                AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(simple_response, env, 
                    header);
                AXIS2_SIMPLE_HTTP_SVR_CONN_SET_KEEP_ALIVE(svr_conn, env, 
                    AXIS2_FALSE);
            }
        }
        else
        {
            if(AXIS2_STRCASECMP(AXIS2_HTTP_SIMPLE_RESPONSE_GET_HTTP_VERSION(
                    simple_response, env), AXIS2_HTTP_HEADER_PROTOCOL_11))
            {
                AXIS2_SIMPLE_HTTP_SVR_CONN_SET_KEEP_ALIVE(svr_conn, env, 
                    AXIS2_TRUE);               
            }
            else
            {
                AXIS2_SIMPLE_HTTP_SVR_CONN_SET_KEEP_ALIVE(svr_conn, env, 
                    AXIS2_FALSE);
            }
        }
        if(AXIS2_TRUE == AXIS2_HTTP_SIMPLE_RESPONSE_CONTAINS_HEADER(
                    simple_response, env, AXIS2_HTTP_HEADER_TRANSFER_ENCODING))
        {
            if(0 != content_length)
            {
                axis2_char_t content_len_str[10];
                sprintf(content_len_str, "%10d", content_length);
                axis2_http_header_t *content_len_hdr = axis2_http_header_create(
                    env, AXIS2_HTTP_HEADER_CONTENT_LENGTH, content_len_str);
                AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(simple_response, env,
                    content_len_hdr);
            }
        }
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_http_worker_set_trasnport_out_config(axis2_http_worker_t *http_worker, 
						axis2_env_t **env, 
						axis2_conf_ctx_t *conf_ctx, 
						axis2_http_simple_response_t *simple_response)
{
    AXIS2_FUNC_PARAM_CHECK(http_worker, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, conf_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, simple_response, AXIS2_FAILURE);

    /*
        TODO implement the method
    */
    return AXIS2_SUCCESS;
}
