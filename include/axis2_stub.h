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
  
#ifndef AXIS2_STUB_H
#define AXIS2_STUB_H

#include <axis2_endpoint_ref.h>
#include <axis2_call.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 * @file axis2_stub.h
 * @brief Axis2c service skeleton that should be implemented by actual service
 * wrappers
 */

/** @defgroup axis2_stub Axis2 Stub
 * @ingroup axis2_core_clientapi
 * @{
 */    

typedef struct axis2_stub_ops axis2_stub_ops_t;   
typedef struct axis2_stub axis2_stub_t;
  
AXIS2_DECLARE_DATA struct axis2_stub_ops
{
    axis2_status_t (AXIS2_CALL * 
    free)(axis2_stub_t *stub,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_endpoint_ref) (axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_endpoint_ref_t *endpoint_ref);
    
    axis2_status_t (AXIS2_CALL *
    set_endpoint_uri) (axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_char_t *endpoint_uri);
    
    axis2_status_t (AXIS2_CALL *
    invoke_blocking) (axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_msg_ctx_t *msg_ctx);
    
    axis2_status_t (AXIS2_CALL *
    invoke_non_blocking) (axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_msg_ctx_t *msg_ctx,
                        axis2_callback_t *callback);
                        
    /**
     * Invoke the blocking/Synchronous call
     *
     * @param op - this will be used to identify the operation in the client 
     *              side, without dispatching
     * @param toSend - This should be OM Element (payload)
     * @return
     */
    axis2_om_node_t* (AXIS2_CALL *
    invoke_blocking_with_om)(struct axis2_stub *stub, 
                                axis2_env_t **env,
                                axis2_char_t *op_name, 
                                axis2_om_node_t *om_node_to_send);
    /**
     * Invoke the blocking/Synchronous call
     *
     * @param op_name - this will be used to identify the operation in the client
     *                      side, without dispatching
     * @param envelope - This should be SOAPEnvelope
     * @return
     */
    axis2_soap_envelope_t* (AXIS2_CALL *
    invoke_blocking_with_soap)(struct axis2_stub *stub, 
                                axis2_env_t **env,
                                axis2_char_t *op_name, 
                                axis2_soap_envelope_t *envelope);
    
    /**
     * Invoke the nonblocking/Asynchronous call
     *
     * @param op_name
     * @param om_node_to_send   -  This should be OM Element (payload)
     *                 invocation behaves accordingly
     * @param callback
     */
    axis2_status_t (AXIS2_CALL *
    invoke_non_blocking_with_om)(struct axis2_stub *stub, 
                                    axis2_env_t **env,
                                    axis2_char_t *op_name,
                                    axis2_om_node_t *om_node_to_send,
                                    axis2_callback_t *callback);
    /**
     * Invoke the nonblocking/Asynchronous call
     *
     * @param op_name
     * @param envelope   -  This should be a SOAP Envelope
     *                 invocation behaves accordingly
     * @param callback
     */
    
    axis2_status_t (AXIS2_CALL *
    invoke_non_blocking_with_soap)(struct axis2_stub *call, 
                                    axis2_env_t **env,
                                    axis2_char_t *op_name,
                                    axis2_soap_envelope_t *envelope,
                                    axis2_callback_t * callback);
} ;

AXIS2_DECLARE_DATA struct axis2_stub 
{
    axis2_stub_ops_t *ops; 
    axis2_array_list_t *func_array;
};

/**
 * Creates axis2_stub struct
 * @param endpoint reference
 * @return pointer to newly created axis2_stub struct
 */
AXIS2_DECLARE(axis2_stub_t *)
axis2_stub_create_with_endpoint_ref(axis2_env_t **env,
                                        axis2_endpoint_ref_t *endpoint_ref);

/**
 * Creates axis2_stub struct
 * @param endpoint uri
 * @return pointer to newly created axis2_stub struct
 */
AXIS2_DECLARE(axis2_stub_t *)
axis2_stub_create_with_endpoint_uri(axis2_env_t **env,
                                        axis2_char_t *endpoint_uri);

/*************************** Function macros **********************************/

#define AXIS2_STUB_FREE(stub, env) \
		((stub)->ops->free (stub, env))

#define AXIS2_STUB_SET_ENDPOINT_REF(stub, env, endpoint_ref) \
		((stub)->ops->set_endpoint_ref (stub, env, endpoint_ref))

#define AXIS2_STUB_SET_ENDPOINT_URI(stub, env, endpoint_uri) \
		((stub)->ops->set_endpoint_uri (stub, env, endpoint_uri))
        
#define AXIS2_STUB_INVOKE_BLOCKING(stub, env, msg_ctx) \
		((stub)->ops->invoke_blocking (stub, env, msg_ctx))  

#define AXIS2_STUB_INVOKE_NON_BLOCKING(stub, env, msg_ctx, callback) \
		((stub)->ops->invoke_non_blocking (stub, env, msg_ctx, callback))

#define AXIS2_STUB_INVOKE_BLOCKING_WITH_OM(stub, env, op_name, om_node_to_send) \
		((stub)->ops->invoke_blocking_with_om (stub, env, op_name, om_node_to_send))  

#define AXIS2_STUB_INVOKE_NON_BLOCKING_WITH_OM(stub, env, op_name, om_node_to_send, callback) \
		((stub)->ops->invoke_non_blocking_with_om (stub, env, op_name, om_node_to_send, callback))
        
#define AXIS2_STUB_INVOKE_BLOCKING_WITH_SOAP(stub, env, op_name, envelope) \
		((stub)->ops->invoke_blocking_with_soap (stub, env, op_name, envelope))  

#define AXIS2_STUB_INVOKE_NON_BLOCKING_WITH_SOAP(stub, env, op_name, envelope, callback) \
		((stub)->ops->invoke_non_blocking_with_soap (stub, env, op_name, envelope, callback))
        
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_STUB_H */
