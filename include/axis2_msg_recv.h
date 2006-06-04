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

#ifndef AXIS2_MSG_RECV_H
#define AXIS2_MSG_RECV_H

/**
  * @file axis2_msg_recv.h
  * @brief Axis Message Receiver interface. Message Receiver struct. 
  *        This interface is extended by custom message receivers
  */

#ifdef __cplusplus
extern "C"
{
#endif
    
/** @defgroup axis2_msg_recv Message Receiver
  * @ingroup axis2_core_receivers
  * @{
  */

#include <axis2_defines.h>
#include <axis2_const.h>
#include <axis2_svc_skeleton.h>
#include <axis2_msg_ctx.h>
#include <axis2_op_ctx.h>
#include <axis2_svr_callback.h>    

#define AXIS2_SUCCESS 1
#define AXIS2_FAILURE 0

struct axis2_msg_ctx;
typedef struct axis2_msg_recv axis2_msg_recv_t;
typedef struct axis2_msg_recv_ops axis2_msg_recv_ops_t;

/** 
 * @brief Message Receiver ops struct.
 * Encapsulator struct for ops of axis2_msg_recv
 */     
struct axis2_msg_recv_ops
{
    /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_msg_recv_t *msg_recv,
            const axis2_env_t *env);

    /**
     * This method is called from axis2_engine_receive method. This method's
     * actual implementation is decided from the create method of the 
     * extended message receiver object. There depending on the synchronous or
     * asynchronous type, receive metho is assigned with the synchronous or
     * asynchronous implementation of receive.
     * @see raw_xml_in_out_msg_recv_create method where receive is assigned
     *      to receive_sync
     * @ param in_msg_ctx
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    receive) (axis2_msg_recv_t *msg_recv,
                const axis2_env_t *env,
                struct axis2_msg_ctx *in_msg_ctx,
                void *callback_recv_param);
    /** 
     * This contain synchronous receiving logic.
     * @param in_msg_ctx
     * @return status code
     */                
    axis2_status_t (AXIS2_CALL *
    receive_sync) (axis2_msg_recv_t *msg_recv,
                const axis2_env_t *env,
                struct axis2_msg_ctx *in_msg_ctx,
                void *callback_recv_param);

    /**
     * This contain asynchronous receiving logic.
     * @param in_msg_ctx
     * @return status code
     */                
    axis2_status_t (AXIS2_CALL *
    receive_async) (axis2_msg_recv_t *msg_recv,
                const axis2_env_t *env,
                struct axis2_msg_ctx *in_msg_ctx,
                void *callback_recv_param);                
    
    /** 
     * This contain in only synchronous business invoke logic
     * @param in_msg_ctx
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    invoke_in_business_logic_sync) (axis2_msg_recv_t *msg_recv,
                                            const axis2_env_t *env,
                                            struct axis2_msg_ctx *in_msg_ctx);
     
    /** 
     * This contain in only asynchronous business invoke logic
     * @param in_msg_ctx
     * @param svr_callback
     * @return status code
     */                                            
    axis2_status_t (AXIS2_CALL *
    invoke_in_business_logic_async) (axis2_msg_recv_t *msg_recv,
                                            const axis2_env_t *env,
                                            struct axis2_msg_ctx *in_msg_ctx,
                                            struct axis2_svr_callback *svr_callback);
     
    /** 
     * This contain in out synchronous business invoke logic
     * @param in_msg_ctx
     * @param out_msg_ctx
     * @return status code
     */                                            
    axis2_status_t (AXIS2_CALL *
    invoke_in_out_business_logic_sync) (axis2_msg_recv_t *msg_recv,
                                            const axis2_env_t *env,
                                            struct axis2_msg_ctx *in_msg_ctx,
                                            struct axis2_msg_ctx *out_msg_ctx);
    
    /** 
     * This contain in out asynchronous business invoke logic
     * @param in_msg_ctx
     * @param out_msg_ctx
     * @param callback
     * @return status code
     */                                              
    axis2_status_t (AXIS2_CALL *
    invoke_in_out_business_logic_async) (axis2_msg_recv_t *msg_recv,
                                            const axis2_env_t *env,
                                            struct axis2_msg_ctx *in_msg_ctx,
                                            struct axis2_msg_ctx *out_msg_ctx,
                                            struct axis2_svr_callback *callback);                                            
    

    /**
     * this will create a new service skeleton object
     * @param msg_ctx
     * @return service skeleton object
     */
    axis2_svc_skeleton_t * (AXIS2_CALL *
    make_new_svc_obj) (axis2_msg_recv_t *msg_recv,
                        const axis2_env_t *env,
                        struct axis2_msg_ctx *msg_ctx);

    /**
     * This will return the service skeleton object
     * @param msg_ctx
     * @return service skeleton object
     */
    axis2_svc_skeleton_t * (AXIS2_CALL *
    get_impl_obj) (axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx); 

    /**
     * Set the application scope
     * @param scope
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    set_scope) (axis2_msg_recv_t *msg_recv,
                                const axis2_env_t *env,
                                axis2_char_t *scope);

    
    /**
     * Get the application scope
     * @param scope
     * @return scope
     */                    
    axis2_char_t * (AXIS2_CALL *
    get_scope) (axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env);
     
    /**
     * Delete the service skeleton object created by make_new_svc_obj
     * @param msg_ctx
     * @return status code
     */                    
    axis2_status_t (AXIS2_CALL *
    delete_svc_obj) (axis2_msg_recv_t *msg_recv,
                        const axis2_env_t *env,
                        axis2_msg_ctx_t *msg_ctx);                    
};

/** 
 * @brief Message Receiver struct.
 * This is the base message receiver struct extended by other message receiver
 * structs.
 * @see axis2_raw_xml_in_out_msg_recv struct
 */  
struct axis2_msg_recv
{
    axis2_msg_recv_ops_t *ops;
    void* derived;
};

/** 
 * Create new message receiver object. usually this will be called from the
 * extended message receiver object.
 * @see create method of raw_xml_in_out_msg_recv
 * @return newly created message receiver object
 **/
AXIS2_EXTERN axis2_msg_recv_t * AXIS2_CALL
axis2_msg_recv_create (const axis2_env_t *env);

/************************** Start of function macros **************************/

#define AXIS2_MSG_RECV_FREE(msg_recv, env) ((msg_recv)->ops->free (msg_recv, env))

#define AXIS2_MSG_RECV_RECEIVE(msg_recv, env, msg_ctx, callback_recv_param) \
      ((msg_recv)->ops->receive (msg_recv, env, msg_ctx, callback_recv_param))

#define AXIS2_MSG_RECV_RECEIVE_SYNC(msg_recv, env, msg_ctx, callback_recv_param) \
      ((msg_recv)->ops->receive_sync (msg_recv, env, msg_ctx, callback_recv_param))

#define AXIS2_MSG_RECV_RECEIVE_ASYNC(msg_recv, env, msg_ctx, callback_recv_param) \
      ((msg_recv)->ops->receive_async (msg_recv, env, msg_ctx, callback_recv_param))

#define AXIS2_MSG_RECV_INVOKE_IN_BUSINESS_LOGIC_SYNC(msg_recv, env, in_msg_ctx) \
        ((msg_recv)->ops->invoke_in_business_logic_sync(msg_recv, env, in_msg_ctx))

#define AXIS2_MSG_RECV_INVOKE_IN_BUSINESS_LOGIC_ASYNC(msg_recv, env, \
            in_msg_ctx, callback) \
        ((msg_recv)->ops->invoke_in_business_logic_async(msg_recv, env, \
            in_msg_ctx, callback))
        
#define AXIS2_MSG_RECV_INVOKE_IN_OUT_BUSINESS_LOGIC_SYNC(msg_recv, env, \
            in_msg_ctx, out_msg_ctx) \
        ((msg_recv)->ops->invoke_in_out_business_logic_sync (msg_recv, env, \
            in_msg_ctx, out_msg_ctx))
            
#define AXIS2_MSG_RECV_INVOKE_IN_OUT_BUSINESS_LOGIC_ASYNC(msg_recv, env, \
            in_msg_ctx, out_msg_ctx, callback) \
        ((msg_recv)->ops->invoke_in_out_business_logic_async(msg_recv, env, \
            in_msg_ctx, out_msg_ctx, callback))            

#define AXIS2_MSG_RECV_MAKE_NEW_SVC_OBJ(msg_recv, env, msg_ctx) \
        ((msg_recv)->ops->make_new_svc_obj(msg_recv, env, msg_ctx))

#define AXIS2_MSG_RECV_GET_IMPL_OBJ(msg_recv, env, msg_ctx) \
      ((msg_recv)->ops->get_impl_obj(msg_recv, env, msg_ctx))
        
#define AXIS2_MSG_RECV_SET_SCOPE(msg_recv, env, scope) \
      ((msg_recv)->ops->set_scope(msg_recv, env, scope))

#define AXIS2_MSG_RECV_GET_SCOPE(msg_recv, env) \
        ((msg_recv)->ops->get_scope(msg_recv, env))

#define AXIS2_MSG_RECV_DELETE_SVC_OBJ(msg_recv, env, msg_ctx) \
        ((msg_recv)->ops->delete_svc_obj(msg_recv, env, msg_ctx))

/************************** End of function macros ****************************/
    
/** @} */
#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_MSG_RECV_H */
