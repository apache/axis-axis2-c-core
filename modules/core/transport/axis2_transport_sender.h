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

#ifndef AXIS2_TRANSPORT_SENDER_H
#define AXIS2_TRANSPORT_SENDER_H

/**
 * @file axis2_transport_sender.h
 * @brief Axis2 description transport sender interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_transport_out_desc.h>
#include <axis2_ctx.h>
#include <axis2_msg_ctx.h>

#include <axis2_wsdl.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_transport_out_desc;    
struct axis2_ctx;
struct axis2_msg_ctx;    
struct axis2_handler;    
typedef struct axis2_transport_sender axis2_transport_sender_t;
typedef struct axis2_transport_sender_ops axis2_transport_sender_ops_t;

/** @defgroup axis2_transport_sender Transport Sender
 * @ingroup axis2_description
 * @{
 */

/** 
 * @brief Description Transport Sender ops struct
 * Encapsulator struct for ops of axis2_transport_sender
 */
AXIS2_DECLARE_DATA struct axis2_transport_sender_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_transport_sender_t *transport_sender,
			        axis2_env_t **env);
    /**
     * Initialize
     * @param confContext
     * @param transportOut
     * @throws org.apache.axis2.AxisFault
     */
    axis2_status_t (AXIS2_CALL * 
    init) (axis2_transport_sender_t *transport_sender,
                                axis2_env_t **env,
                                struct axis2_conf_ctx *conf_ctx,
                                struct axis2_transport_out_desc *transport_out);
    /**
     * Clean up
     * @param msgContext
     * @throws org.apache.axis2.AxisFault
     */
    axis2_status_t (AXIS2_CALL *                          
    cleanup) (axis2_transport_sender_t *transport_sender,
                                axis2_env_t **env,
                                struct axis2_msg_ctx *msg_ctx);
									
	/**
     * Invoke
     * @param msgContext
     * @throws org.apache.axis2.AxisFault
     */
    axis2_status_t (AXIS2_CALL *                          
    invoke) (axis2_transport_sender_t *transport_sender,
                                axis2_env_t **env,
                                struct axis2_msg_ctx *msg_ctx);

};

/** 
 * @brief Transport Sender struct
 * This send the SOAP Message to other SOAP nodes and this alone write the SOAP 
 * Message to the wire. Out flow must be end with one of this kind
 */  
AXIS2_DECLARE_DATA struct axis2_transport_sender
{
	axis2_transport_sender_ops_t *ops;
};

/**
 * Creates phase holder struct
 * @return pointer to newly created transport sender
 */
AXIS2_DECLARE(axis2_transport_sender_t *) 
axis2_transport_sender_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_TRANSPORT_SENDER_FREE(transport_sender, env) \
		((transport_sender->ops)->free (transport_sender, env))

#define AXIS2_TRANSPORT_SENDER_INIT(transport_sender, env, conf_context, transport_out) \
		((transport_sender->ops)->init (transport_sender, env, conf_context, transport_out))

#define AXIS2_TRANSPORT_INVOKE(transport_sender, env, msg_ctx) \
		((transport_sender->ops)->invoke (transport_sender, env, msg_ctx))      

#define AXIS2_TRANSPORT_CLEANUP(transport_sender, env, msg_ctx) \
		((transport_sender->ops)->cleanup (transport_sender, env, msg_ctx))      

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_SENDER_H */
