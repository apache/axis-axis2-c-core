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

#ifndef AXIS2_TRANSPORT_RECEIVER_H
#define AXIS2_TRANSPORT_RECEIVER_H

/**
 * @file axis2_transport_receiver.h
 * @brief Axis2 description transport receiver interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_endpoint_ref.h>
#include <axis2_ctx.h>
#include <axis2_conf_ctx.h>



#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_conf_ctx;
struct axis2_transport_in_desc;
typedef struct axis2_transport_receiver axis2_transport_receiver_t;
typedef struct axis2_transport_receiver_ops axis2_transport_receiver_ops_t;

/** @defgroup axis2_transport_receiver Transport Receiver
 * @ingroup axis2_description
 * @{
 */

/** 
 * @brief Description Transport Receiver ops struct
 * Encapsulator struct for ops of axis2_transport_receiver
 */
AXIS2_DECLARE_DATA struct axis2_transport_receiver_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_transport_receiver_t *transport_receiver,
        const axis2_env_t *env);
    
	axis2_status_t (AXIS2_CALL *
    start)(axis2_transport_receiver_t *transport_receiver,
	        const axis2_env_t *env);
                    
	axis2_status_t (AXIS2_CALL *
    stop)(axis2_transport_receiver_t *transport_receiver,
	        const axis2_env_t *env);
                    
	axis2_endpoint_ref_t* (AXIS2_CALL *
    get_reply_to_epr)(axis2_transport_receiver_t *transport_receiver,
			            const axis2_env_t *env,
                        axis2_char_t *svc_name);

    axis2_status_t (AXIS2_CALL *
    init) (axis2_transport_receiver_t *transport_receiver,
		    const axis2_env_t *env,
			struct axis2_conf_ctx *conf_ctx,
			struct axis2_transport_in_desc *transport_in);


    struct axis2_conf_ctx* (AXIS2_CALL *
    get_conf_ctx) (axis2_transport_receiver_t *server, 
                    	const axis2_env_t *env);
	
	
    axis2_bool_t (AXIS2_CALL *
    is_running) (axis2_transport_receiver_t *server, 
                    	const axis2_env_t *env);

};

/** 
 * @brief Transport Reciever  struct  
 */  
AXIS2_DECLARE_DATA struct axis2_transport_receiver
{
	axis2_transport_receiver_ops_t *ops;
};


/*************************** Function macros **********************************/

#define AXIS2_TRANSPORT_RECEIVER_FREE(transport_receiver, env) \
		((transport_receiver->ops)->free (transport_receiver, env))

#define AXIS2_TRANSPORT_RECEIVER_INIT(transport_receiver, env, axisconf, transport_in) \
		((transport_receiver->ops)->init (transport_receiver, env, axisconf, transport_in))

#define AXIS2_TRANSPORT_RECEIVER_START(transport_receiver, env) \
		((transport_receiver->ops)->start (transport_receiver, env))

#define AXIS2_TRANSPORT_RECEIVER_STOP(transport_receiver, env) \
		((transport_receiver->ops)->stop (transport_receiver, env))

#define AXIS2_TRANSPORT_RECEIVER_GET_REPLY_TO_EPR(transport_receiver, env, svc_name) \
		((transport_receiver->ops)->get_reply_to_epr (transport_receiver, env, svc_name))

#define AXIS2_TRANSPORT_RECEIVER_GET_CONF_CTX(transport_receiver, env) \
                        ((transport_receiver)->ops->get_conf_ctx(transport_receiver, env))
                        
#define AXIS2_TRANSPORT_RECEIVER_IS_RUNNING(transport_receiver, env) \
                                ((transport_receiver)->ops->is_running(transport_receiver, env))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_RECEIVER_H */
