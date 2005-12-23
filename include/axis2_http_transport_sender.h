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

#ifndef AXIS2_HTTP_TRANSPORT_SENDER_H
#define AXIS2_HTTP_TRANSPORT_SENDER_H


/**
  * @file axis2_http_transport_sender.h
  * @brief axis2 HTTP Transport Sender (Handler) implementation
  */
  
#include <axis2.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_msg_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_transport_out_desc.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

    typedef struct axis2_http_transport_sender_ops 
							axis2_http_transport_sender_ops_t;
    typedef struct axis2_http_transport_sender axis2_http_transport_sender_t;

    
/** 
 * @brief HTTP Transport Sender ops struct
 * Encapsulator struct for ops of axis2_http_transport_sender
 */  
AXIS2_DECLARE_DATA struct axis2_http_transport_sender_ops
{
    axis2_status_t (AXIS2_CALL *invoke) 
							(axis2_http_transport_sender_t *transport_sender, 
                    		axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
    
    axis2_status_t (AXIS2_CALL *clean_up) 
							(axis2_http_transport_sender_t *transport_sender, 
                    		axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
    
    axis2_status_t (AXIS2_CALL *init) 
							(axis2_http_transport_sender_t *transport_sender, 
                    		axis2_env_t **env, axis2_conf_ctx_t *conf_ctx, 
							axis2_transport_out_desc_t *out_desc);
    
    axis2_status_t (AXIS2_CALL *free) 
							(axis2_http_transport_sender_t *transport_sender, 
                    		axis2_env_t **env);
};

/** 
 * @brief HTTP Transport Sender
 *    Axis2 HTTP Transport Sender
 */
AXIS2_DECLARE_DATA struct axis2_http_transport_sender
{
    axis2_http_transport_sender_ops_t *ops;    
};


AXIS2_DECLARE(axis2_http_transport_sender_t *) AXIS2_CALL
axis2_http_transport_sender_create (axis2_env_t **env);

/************************** Start of function macros **************************/

#define AXIS2_HTTP_TRANSPORT_SENDER_INVOKE(transport_sender, env, msg_ctx) \
								((transport_sender)->ops->invoke(\
								transport_sender, env, msg_ctx))
#define AXIS2_HTTP_TRANSPORT_SENDER_CLEAN_UP(transport_sender, env,msg_ctx) \
                                ((transport_sender)->ops->clean_up(\
								transport_sender, env, msg_ctx))
#define AXIS2_HTTP_TRANSPORT_SENDER_INIT(transport_sender, env, conf_ctx, \
								out_desc) ((transport_sender)->ops->init(\
								transport_sender, env, conf_ctx, out_desc))
#define AXIS2_HTTP_TRANSPORT_SENDER_FREE(transport_sender, env) \
                                ((transport_sender)->ops->free(\
								transport_sender, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_TRANSPORT_SENDER_H */
