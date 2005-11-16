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

#ifndef AXIS2_FLOW_CONTAINER_H
#define AXIS2_FLOW_CONTAINER_H

/**
 * @file axis2_flow_container.h
 * @brief axis2 flow container interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_flow.h>


#ifdef __cplusplus
extern "C"
{
#endif
	
typedef struct axis2_flow_container_ops_s axis2_flow_container_ops_t;
typedef struct axis2_flow_container_s axis2_flow_container_t;	
	

/** @defgroup axis2_flow_container Flow Container
  * @ingroup axis2_flow_container
  * @{
  */

/** 
 * @brief Flow Container operations struct
 * Encapsulator struct for operations of axis2_flow_container
 */
struct axis2_flow_container_ops_s
{
	/** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *free) (axis2_flow_container_t *flow_container,
                                        axis2_env_t **env);

    /**
     * Get fault out flow
     * @return in flow
     */
    axis2_flow_t *(AXIS2_CALL *get_in_flow)(axis2_flow_container_t *flow_container,
                                                axis2_env_t **env);
    
    /**
     * Set in flow
     * @param in_flow in flow
     */
    axis2_status_t (AXIS2_CALL *set_in_flow)(
                                        axis2_flow_container_t *flow_container,
                                        axis2_env_t **env,
                                        axis2_flow_t *in_flow);
    
    /**
     * Get out flow
     * @return out flow
     */
    axis2_flow_t *(AXIS2_CALL *get_out_flow)(
                                        axis2_flow_container_t *flow_container,
                                        axis2_env_t **env);
    
    /**
     * Set out flow
     * @param out_flow out flow
     */
    axis2_status_t (AXIS2_CALL *set_out_flow)(
                                        axis2_flow_container_t *flow_container,
                                        axis2_env_t **env,
                                        axis2_flow_t *out_flow);
    
    /**
     * Get fault in flow
     * @return fault in flow
     */
    axis2_flow_t * (AXIS2_CALL *get_fault_in_flow)(
                                        axis2_flow_container_t *flow_container,
                                        axis2_env_t **env);
    
    /**
     * set fault in flow
     * @param falut_in_flow falut in flow
     */
    axis2_status_t (AXIS2_CALL *set_fault_in_flow)(
                                        axis2_flow_container_t *flow_container,
                                        axis2_env_t **env,
                                        axis2_flow_t *falut_in_flow);
    /**
     * Get fault out flow
     * @return fault out flow
     */
    axis2_flow_t *(AXIS2_CALL *get_fault_out_flow)(
                                        axis2_flow_container_t *flow_container,
                                        axis2_env_t **env);
    
    /**
     * Set fault out flow
     * @param fault_out_flow fault out flow
     */
    axis2_status_t (AXIS2_CALL *set_fault_out_flow)(
                                        axis2_flow_container_t *flow_container,
                                        axis2_env_t **env,
                                        axis2_flow_t *fault_out_flow);  
};

/** 
 * @brief Flow container struct
 *	Container for flows  
 */ 
struct axis2_flow_container_s
{
	axis2_flow_container_ops_t *ops;
};

/**
 * Creates flow container struct
 * @return pointer to newly created flow container
 */
AXIS2_DECLARE(axis2_flow_container_t *) 
axis2_flow_container_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_FLOW_CONTAINER_FREE(flow_container, env) \
        ((flow_container->ops)->free (flow_container, env))

#define AXIS2_FLOW_CONTAINER_GET_IN_FLOW(flow_container, env) \
        ((flow_container->ops)->get_in_flow (flow_container, env))

#define AXIS2_FLOW_CONTAINER_SET_IN_FLOW(flow_container, env, in_flow) \
        ((flow_container->ops)->set_in_flow (flow_container, env, in_flow))

#define AXIS2_FLOW_CONTAINER_GET_OUT_FLOW(flow_container, env) \
        ((flow_container->ops)->get_out_flow (flow_container, env))
        
#define AXIS2_FLOW_CONTAINER_SET_OUT_FLOW(flow_container, env, out_flow) \
        ((flow_container->ops)->set_out_flow (flow_container, env, out_flow))

#define AXIS2_FLOW_CONTAINER_GET_FAULT_IN_FLOW(flow_container, env) \
        ((flow_container->ops)->get_fault_in_flow (flow_container, env))
        
#define AXIS2_FLOW_CONTAINER_SET_FAULT_IN_FLOW(flow_container, env, fault_in_flow) \
        ((flow_container->ops)->set_fault_in_flow (flow_container, env, fault_in_flow))

#define AXIS2_FLOW_CONTAINER_GET_FAULT_OUT_FLOW(flow_container, env) \
        ((flow_container->ops)->get_fault_out_flow (flow_container, env))
        
#define AXIS2_FLOW_CONTAINER_SET_FAULT_OUT_FLOW(flow_container, env, fault_out_flow) \
        ((flow_container->ops)->set_fault_out_flow (flow_container, env, fault_out_flow))        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_FLOW_CONTAINER_H */
