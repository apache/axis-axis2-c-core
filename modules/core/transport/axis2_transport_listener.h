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
/*#include <axis2_endpoint_ref.h>*/
#include <axis2_engine_config.h>

#include <wsdl.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_transport_listener;
typedef struct axis2_transport_listener axis2_transport_listener_t;
typedef struct axis2_transport_listener_ops axis2_transport_listener_ops_t;

/** @defgroup axis2_transport_listener Transport Listener
 * @ingroup axis2_description
 * @{
 */

/** 
 * @brief Description Transport Listener operations struct
 * Encapsulator struct for operations of axis2_transport_listener
 */
AXIS2_DECLARE_DATA struct axis2_transport_listener_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_transport_listener_t *transport_listener,
			        axis2_env_t **env);
    

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

#define AXIS2_TRANSPORT_LISTENER_START(transport_listener, env, handler) \
		((transport_listener->ops)->start (transport_listener, env, handler))

#define AXIS2_TRANSPORT_LISTENER_STOP(transport_listener, env, phase_name) \
		((transport_listener->ops)->stop (transport_listener, env, phase_name))

#define AXIS2_TRANSPORT_LISTENER_REPLY_TO_EPR(transport_listener, env, svc_name) \
		((transport_listener->ops)->reply_to_epr (transport_listener, env, svc_name))       

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_LISTENER_H */
