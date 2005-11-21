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

#ifndef AXIS2_FLOW_H
#define AXIS2_FLOW_H

/**
 * @file axis2_flow.h
 * @brief axis2 flow interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_handler_desc.h>


#ifdef __cplusplus
extern "C"
{
#endif
	
typedef struct axis2_flow_ops axis2_flow_ops_t;
typedef struct axis2_flow axis2_flow_t;	
	

/** @defgroup axis2_flow Flow
  * @ingroup axis2_flow
  * @{
  */

/** 
 * @brief Flow operations struct
 * Encapsulator struct for operations of axis2_flow
 */
struct axis2_flow_ops
{
	/** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *free) (axis2_flow_t *flow,
                            axis2_env_t **env);

    /**
     * Add handler description
     * @param handler handler description
     */
    axis2_status_t (AXIS2_CALL *add_handler) (axis2_flow_t *flow,
                                                axis2_env_t **env,
                                                axis2_handler_desc_t *handler);
    
    /**
     * Get handler
     * @param index index of the handler
     * @return handler description
     */
    axis2_handler_desc_t * (AXIS2_CALL *get_handler) (axis2_flow_t *flow,
                                                        axis2_env_t **env,
                                                        int index);
    
    /**
     * Get handler count
     * @return handler count
     */
    int (AXIS2_CALL *get_handler_count) (axis2_flow_t *flow,
                                            axis2_env_t **env);  
};

/** 
 * @brief Flow struct
 *	Flow  
 */ 
struct axis2_flow
{
	axis2_flow_ops_t *ops;
};

/**
 * Creates flow struct
 * @return pointer to newly created flow
 */
AXIS2_DECLARE(axis2_flow_t *) 
axis2_flow_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_FLOW_FREE(flow, env) ((flow->ops)->free (flow, env))

#define AXIS2_FLOW_ADD_HANDLER(flow, env, param) \
		((flow->ops)->add_handler (flow, env, handler))

#define AXIS2_FLOW_GET_HANDLER(flow, env, index) \
		((flow->ops)->get_handler (flow, env, index))

#define AXIS2_FLOW_GET_HANDLER_COUNT(flow, env) \
		((flow->ops)->get_handler_count (flow, env))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_FLOW_H */
