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
 * @defgroup axis2_http_client http client
 * @ingroup axis2_core_trans_http
 * Description
 * @{
 */

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

    /** Type name for struct axis2_http_client_ops */
    typedef struct axis2_http_client_ops axis2_http_client_ops_t;
    /** Type name for struct axis2_http_client */
    typedef struct axis2_http_client axis2_http_client_t;


    /**
     * HTTP Client ops struct
     * Encapsulator struct for ops of axis2_http_client
     */
    AXIS2_DECLARE_DATA struct axis2_http_client_ops
    {
        /**
	 * @param client pointer to client
	 * @param env pointer to environment struct
	 * @param request pointer to request
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
	        send)(
                    axis2_http_client_t *client, 
		    const axis2_env_t *env,
                    axis2_http_simple_request_t *request);

        /**
	 * @param client pointer to client
	 * @param env pointer to environment struct
	 */
        int (AXIS2_CALL *
                recieve_header)(
                    axis2_http_client_t *client, 
		    const axis2_env_t *env);

        /**
	 * @param client pointer to client
	 * @param env pointer to environment struct
	 */
        axis2_http_simple_response_t* (AXIS2_CALL *
                get_response)(
                    axis2_http_client_t *client, 
		    const axis2_env_t *env);

        /**
	 * @param client pointer to client
	 * @param env pointer to environment struct
	 * @param url pointer to url
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_url)(
                    axis2_http_client_t *client,
                    const axis2_env_t *env, 
		    axis2_url_t *url);

        /**
	 * @param client pointer to client
	 * @param env pointer to environment struct
	 */
        axis2_url_t* (AXIS2_CALL *
                get_url)(
                    axis2_http_client_t *client,
                    const axis2_env_t *env);

        /**
	 * @param client pointer to client
	 * @param env pointer to environment struct
	 * @param timeout_ms
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_timeout)(
                    axis2_http_client_t *client,
                    const axis2_env_t *env, 
		    int timeout_ms);

        /**
	 * @param client pointer to client
	 * @param env pointer to environment struct
	 */
        int (AXIS2_CALL *
                get_timeout)(
                    axis2_http_client_t *client,
                    const axis2_env_t *env);

        /**
	 * @param client pointer to client
	 * @param env pointer to environment struct
	 * @param proxy_host pointer to proxy host
	 * @param proxy_port
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_proxy)(
                    axis2_http_client_t *client,
                    const axis2_env_t *env, 
		    axis2_char_t *proxy_host,
                    int proxy_port);

        /**
	 * @param client pointer to client
	 * @param env pointer to environment struct
	 */
        axis2_char_t* (AXIS2_CALL *
                get_proxy)(
                    axis2_http_client_t *client,
                    const axis2_env_t *env);

        /**
	 * @param client pointer to client
	 * @param env pointer to environment struct
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                free)(
		axis2_http_client_t *client,
                const axis2_env_t *env);
    };

    /**
     * axis2 http client
     */
    AXIS2_DECLARE_DATA struct axis2_http_client
    {
        /** operations of axis2 http client */
        axis2_http_client_ops_t *ops;
    };

    /**
     * @param env pointer to environment struct
     * @param url pointer to url
     */
    AXIS2_EXTERN axis2_http_client_t * AXIS2_CALL
            axis2_http_client_create (
                const axis2_env_t *env, 
		axis2_url_t *url);

    /**
     * Free http_client passed as void pointer. This will be
     * cast into appropriate type and then pass the cast object
     * into the http_client structure's free method
     * @param client
     * @param env pointer to environment struct
     */
    axis2_status_t AXIS2_CALL
            axis2_http_client_free_void_arg (
	    void *client, 
	    const axis2_env_t *env);

/************************** Start of function macros **************************/

/** Send.
    @sa axis2_http_client_ops#send */
#define AXIS2_HTTP_CLIENT_SEND(client, env, request) \
                                ((client)->ops->send(client, env, request))

/** Receive header.
    @sa axis2_http_client_ops#receive_header */
#define AXIS2_HTTP_CLIENT_RECIEVE_HEADER(client, env) \
                                ((client)->ops->recieve_header(client, env))

/** Gets the response.
    @sa axis2_http_client_ops#get_response */
#define AXIS2_HTTP_CLIENT_GET_RESPONSE(client, env) \
                                ((client)->ops->get_response(client, env))

/** Sets the url.
    @sa axis2_http_client_ops#set_url */
#define AXIS2_HTTP_CLIENT_SET_URL(client, env, url) \
                                ((client)->ops->set_url(client, env, url))

/** Gets the url.
    @sa axis2_http_client_ops#get_url */
#define AXIS2_HTTP_CLIENT_GET_URL(client, env) \
                                ((client)->ops->get_url(client, env))

/** Sets the timeout.
    @sa axis2_http_client_ops#set_timeout */
#define AXIS2_HTTP_CLIENT_SET_TIMEOUT(client, env, timeout) \
                                ((client)->ops->set_timeout(client, env,\
                        timeout))

/** Gets the timeout.
    @sa axis2_http_client_ops#get_timeout */
#define AXIS2_HTTP_CLIENT_GET_TIMEOUT(client, env) \
                                ((client)->ops->get_url(client, env))

/** Sets the proxy.
    @sa axis2_http_client_ops#set_proxy */
#define AXIS2_HTTP_CLIENT_SET_PROXY(client, env, proxy_host, proxy_port) \
                                ((client)->ops->set_proxy(client, env,\
                        proxy_host, proxy_port))

/** Gets the proxy.
    @sa axis2_http_client_ops#get_proxy */
#define AXIS2_HTTP_CLIENT_GET_PROXY(client, env) \
                                ((client)->ops->get_proxy(client, env))

/** Frees the http client.
    @sa axis2_http_client_ops#free */
#define AXIS2_HTTP_CLIENT_FREE(client, env) \
                                ((client)->ops->free(client, env)) 

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_CLIENT_H */
