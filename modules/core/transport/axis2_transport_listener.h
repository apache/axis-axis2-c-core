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

#ifndef AXIS2_TRANSPORT_LISTENER_H
#define AXIS2_TRANSPORT_LISTENER_H

/**
 * @file axis2_transport_listener.h
 * @brief Axis2 description transport listener interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_endpoint_ref.h>
#include <axis2_ctx.h>

#include <axis2_wsdl.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_transport_listener;
struct axis2_ctx;    
typedef struct axis2_transport_listener axis2_transport_listener_t;
typedef struct axis2_transport_listener_ops axis2_transport_listener_ops_t;
struct axis2_conf_ctx;
struct axis2_transport_in_desc;

/** @defgroup axis2_transport_listener Transport Listener
 * @ingroup axis2_description
 * @{
 */

/** 
 * @brief Description Transport Listener ops struct
 * Encapsulator struct for ops of axis2_transport_listener
 */
AXIS2_DECLARE_DATA struct axis2_transport_listener_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_transport_listener_t *transport_listener,
			        axis2_env_t **env);
    
	axis2_status_t (AXIS2_CALL *start)(
                    axis2_transport_listener_t *transport_listener,
			        axis2_env_t **env);
	axis2_status_t (AXIS2_CALL *stop)(
                    axis2_transport_listener_t *transport_listener,
			        axis2_env_t **env);
	axis2_endpoint_ref_t* (AXIS2_CALL *reply_to_epr)(
                    axis2_transport_listener_t *transport_listener,
			        axis2_env_t **env,
                    axis2_char_t *svc_name);

    axis2_status_t (AXIS2_CALL * 
    init) (axis2_transport_listener_t *transport_listener,
                                axis2_env_t **env,
                                struct axis2_conf_ctx *conf_ctx,
                                struct axis2_transport_in_desc *transport_in);
};

/** 
 * @brief Transport Listener struct  
 */  
AXIS2_DECLARE_DATA struct axis2_transport_listener
{
	axis2_transport_listener_ops_t *ops;
};

/**
 * Creates phase holder struct
 * @return pointer to newly created transport listener
 */
AXIS2_DECLARE(axis2_transport_listener_t *) 
axis2_transport_listener_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_TRANSPORT_LISTENER_FREE(transport_listener, env) \
		((transport_listener->ops)->free (transport_listener, env))

#define AXIS2_TRANSPORT_LISTENER_INIT(transport_listener, env, axisconf, transport_in) \
		((transport_listener->ops)->init (transport_listener, env, axisconf, transport_in))

#define AXIS2_TRANSPORT_LISTENER_START(transport_listener, env) \
		((transport_listener->ops)->start (transport_listener, env))

#define AXIS2_TRANSPORT_LISTENER_STOP(transport_listener, env) \
		((transport_listener->ops)->stop (transport_listener, env))

#define AXIS2_TRANSPORT_LISTENER_REPLY_TO_EPR(transport_listener, env, svc_name) \
		((transport_listener->ops)->reply_to_epr (transport_listener, env, svc_name))       

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_LISTENER_H */
