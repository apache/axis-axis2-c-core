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


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_STUB_H */
