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

#ifndef AXIS2_LISTNER_MANAGER_H
#define AXIS2_LISTNER_MANAGER_H


/**
  * @file axis2_listener_manager.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_conf_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_listener_manager Message Context 
 * @ingroup axis2_core_context
 * @{
 */
    
typedef struct axis2_listener_manager_ops axis2_listener_manager_ops_t;
typedef struct axis2_listener_manager axis2_listener_manager_t; 

    
/** 
 * @brief Message Context ops struct
 * Encapsulator struct for ops of axis2_listener_manager
 */  
struct axis2_listener_manager_ops
{
    /**
     * This Method starts a Listenet if it is not already started. But only a one listener started for
     * a given one transport.
     * @param transport
     * @param conf_ctx
     * @throws AxisFault
     */
    axis2_status_t (AXIS2_CALL *
    make_sure_started)(struct axis2_listener_manager *listener_manager, 
                       axis2_env_t **env, 
                       axis2_char_t *transport,
                       axis2_conf_ctx_t *conf_ctx);
                       
    axis2_status_t (AXIS2_CALL *
    stop)(struct axis2_listener_manager *listener_manager, 
          axis2_env_t **env, 
          axis2_char_t *transport);
    /**
     * @param serviceName
     * @param transport
     * @return reply to URL return will direct the response for the message to the given transport listener
     * @throws AxisFault
     */
    axis2_endpoint_ref_t* (AXIS2_CALL *
    reply_to_epr)(struct axis2_listener_manager *listener_manager, 
                  axis2_env_t **env, 
                  axis2_char_t *svc_name,
                  axis2_char_t *transport);
    
    
    axis2_conf_ctx_t *(AXIS2_CALL *
    get_conf_ctx)(axis2_listener_manager_t *listener_manager,
                  axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    free)(struct axis2_listener_manager *listener_manager, 
          axis2_env_t **env);
          
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_listener_manager
{
    axis2_listener_manager_ops_t *ops;    
};

AXIS2_DECLARE(axis2_listener_manager_t*) 
axis2_listener_manager_create(axis2_env_t **env);
    
/************************** Start of function macros **************************/

#define AXIS2_LISTNER_MANAGER_MAKE_SURE_STARTED(listener_manager, env, transport, conf_ctx)\
        ((listener_manager)->ops->make_sure_started(listener_manager, env, transport, conf_ctx))
        
#define AXIS2_LISTENER_MANAGER_STOP(listener_manager, env, transport)\
        ((listener_manager)->ops->stop(listener_manager, env, transport))
        
#define AXIS2_LISTNER_MANAGER_REPLY_TO_EPR(listener_manager, env, svc_name, transport) \
        ((listener_manager)->ops->reply_to_epr(listener_manager, env, svc_name, transport))
        
#define AXIS2_LISTNER_MANAGER_GET_CONF_CTX(listener_manager, env) \
        ((listener_manager)->ops->get_conf_ctx(listener_manager, env))
        
#define AXIS2_LISTNER_MANAGER_FREE(listener_manager, env) \
        ((listener_manager)->ops->free(listener_manager, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_LISTNER_MANAGER_H */
