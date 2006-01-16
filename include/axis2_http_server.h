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

#ifndef AXIS2_HTTP_SERVER_H
#define AXIS2_HTTP_SERVER_H


/**
  * @file axis2_http_server.h
  * @brief axis2 HTTP Server implementation
  */
  
#include <axis2.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_conf_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

typedef struct axis2_http_server_ops axis2_http_server_ops_t;
typedef struct axis2_http_server axis2_http_server_t;

    
/** 
 * @brief HTTP Server ops struct
 * Encapsulator struct for ops of axis2_http_server
 */  
AXIS2_DECLARE_DATA struct axis2_http_server_ops
{
    axis2_status_t (AXIS2_CALL *init)
						(axis2_http_server_t *server, axis2_env_t **env,
						axis2_conf_ctx_t *conf_ctx, 
						axis2_transport_in_desc_t *in_desc);
	axis2_status_t (AXIS2_CALL *start)
						(axis2_http_server_t *server, axis2_env_t **env);
	axis2_status_t (AXIS2_CALL *stop)
						(axis2_http_server_t *server, axis2_env_t **env);
    
	axis2_conf_ctx_t* (AXIS2_CALL *get_conf_ctx) (axis2_http_server_t *server, 
                    	axis2_env_t **env);
	
	axis2_endpoint_ref_t* (AXIS2_CALL *get_reply_to_epr) 
						(axis2_http_server_t *server, axis2_env_t **env, 
						axis2_char_t *svc_name);
	
    axis2_bool_t (AXIS2_CALL *is_running) (axis2_http_server_t *server, 
                    	axis2_env_t **env);
	
	axis2_status_t (AXIS2_CALL *free) (axis2_http_server_t *server, 
                    axis2_env_t **env);
};

/** 
 * @brief HTTP Server struct
 *    Axis2 HTTP Server
 */
AXIS2_DECLARE_DATA struct axis2_http_server
{
    axis2_http_server_ops_t *ops;    
};


AXIS2_DECLARE(axis2_transport_receiver_t*) 
axis2_http_server_create (axis2_env_t **env, axis2_char_t *repo, int port);

/************************** Start of function macros **************************/

#define AXIS2_HTTP_SERVER_INIT(server, env, conf_ctx, in_desc) \
                        ((server)->ops->init(server, env, conf_ctx, in_desc))
#define AXIS2_HTTP_SERVER_START(server, env) ((server)->ops->start(server, env))
#define AXIS2_HTTP_SERVER_STOP(server, env) ((server)->ops->stop(server, env))
#define AXIS2_HTTP_SERVER_GET_CONF_CTX(server, env) \
                        ((server)->ops->get_conf_ctx(server, env))
#define AXIS2_HTTP_SERVER_GET_REPLY_TO_EPR(server, env, svc_name) \
                        ((server)->ops->get_reply_to_epr(server, env, svc_name))
#define AXIS2_HTTP_SERVER_IS_RUNNING(server, env) \
                                ((server)->ops->is_running(server, env))
/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_SERVER_H */
