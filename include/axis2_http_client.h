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

#ifndef AXIS2_HTTP_CLIENT_H
#define AXIS2_HTTP_CLIENT_H


/**
  * @file axis2_http_client.h
  * @brief axis2 HTTP Header name:value pair implementation
  */
  
#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_http_simple_response.h>
#include <axis2_http_simple_request.h>
#include <axis2_url.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

typedef struct axis2_http_client_ops axis2_http_client_ops_t;
typedef struct axis2_http_client axis2_http_client_t;

    
/** 
 * @brief HTTP Client ops struct
 * Encapsulator struct for ops of axis2_http_client
 */  
AXIS2_DECLARE_DATA struct axis2_http_client_ops
{
    axis2_status_t (AXIS2_CALL *send)
						(axis2_http_client_t *client, axis2_env_t **env,
						axis2_http_simple_request_t *request);
	int (AXIS2_CALL *recieve_header)
						(axis2_http_client_t *client, axis2_env_t **env);
	axis2_http_simple_response_t* (AXIS2_CALL *get_response)
						(axis2_http_client_t *client, axis2_env_t **env);
    
	axis2_status_t (AXIS2_CALL *set_url) (axis2_http_client_t *client, 
                    axis2_env_t **env, axis2_url_t *url);
	
	axis2_url_t* (AXIS2_CALL *get_url) (axis2_http_client_t *client, 
                    axis2_env_t **env);
	
    axis2_status_t (AXIS2_CALL *set_timeout) (axis2_http_client_t *client, 
                    axis2_env_t **env, int timeout_ms);
	
    int (AXIS2_CALL *get_timeout) (axis2_http_client_t *client, 
                    axis2_env_t **env);
	
    axis2_status_t (AXIS2_CALL *set_proxy) (axis2_http_client_t *client, 
                    axis2_env_t **env, axis2_char_t *proxy_host,
                    int proxy_port);
	
    axis2_char_t* (AXIS2_CALL *get_proxy) (axis2_http_client_t *client, 
                    axis2_env_t **env);
	
    axis2_status_t (AXIS2_CALL *free) (axis2_http_client_t *client, 
                    axis2_env_t **env);
};

/** 
 * @brief HTTP Client struct
 *    Axis2 HTTP Client
 */
AXIS2_DECLARE_DATA struct axis2_http_client
{
    axis2_http_client_ops_t *ops;    
};


AXIS2_DECLARE(axis2_http_client_t *) 
axis2_http_client_create (axis2_env_t **env, axis2_url_t *url);

/**
 * Free http_client passed as void pointer. This will be
 * cast into appropriate type and then pass the cast object
 * into the http_client structure's free method
 */
axis2_status_t AXIS2_CALL
axis2_http_client_free_void_arg (void *client, axis2_env_t **env);

/************************** Start of function macros **************************/

#define AXIS2_HTTP_CLIENT_SEND(client, env, request) \
                                ((client)->ops->send(client, env, request))
#define AXIS2_HTTP_CLIENT_RECIEVE_HEADER(client, env) \
                                ((client)->ops->recieve_header(client, env))
#define AXIS2_HTTP_CLIENT_GET_RESPONSE(client, env) \
                                ((client)->ops->get_response(client, env))
#define AXIS2_HTTP_CLIENT_SET_URL(client, env, url) \
                                ((client)->ops->set_url(client, env, url))
#define AXIS2_HTTP_CLIENT_GET_URL(client, env) \
                                ((client)->ops->get_url(client, env))
#define AXIS2_HTTP_CLIENT_SET_TIMEOUT(client, env, timeout) \
                                ((client)->ops->set_timeout(client, env,\
								timeout))
#define AXIS2_HTTP_CLIENT_GET_TIMEOUT(client, env) \
                                ((client)->ops->get_url(client, env))
#define AXIS2_HTTP_CLIENT_SET_PROXY(client, env, proxy_host, proxy_port) \
                                ((client)->ops->set_proxy(client, env,\
								proxy_host, proxy_port))
#define AXIS2_HTTP_CLIENT_GET_PROXY(client, env) \
                                ((client)->ops->get_proxy(client, env))
#define AXIS2_HTTP_CLIENT_FREE(client, env) \
                                ((client)->ops->free(client, env))
/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_CLIENT_H */
