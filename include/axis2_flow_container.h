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

#include <axis2_const.h>
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
   
typedef struct axis2_flow_container_ops axis2_flow_container_ops_t;
typedef struct axis2_flow_container axis2_flow_container_t;   
   

/** @defgroup axis2_flow_container Flow Container
  * @ingroup axis2_flow_container
  * @{
  */

/** 
 * @brief Flow Container ops struct
 * Encapsulator struct for ops of axis2_flow_container
 */
struct axis2_flow_container_ops
{
   /** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
   free) (axis2_flow_container_t *flow_container,
           const axis2_env_t *env);

    /**
     * Get fault out flow
     * @return in flow
     */
    axis2_flow_t *(AXIS2_CALL *
   get_inflow)(axis2_flow_container_t *flow_container,
                const axis2_env_t *env);
    
    /**
     * Set in flow
     * @param inflow in flow
     */
    axis2_status_t (AXIS2_CALL *
   set_inflow)(axis2_flow_container_t *flow_container,
                const axis2_env_t *env,
                axis2_flow_t *inflow);
    
    /**
     * Get out flow
     * @return out flow
     */
    axis2_flow_t *(AXIS2_CALL *
   get_outflow)(axis2_flow_container_t *flow_container,
                 const axis2_env_t *env);
    
    /**
     * Set out flow
     * @param outflow out flow
     */
    axis2_status_t (AXIS2_CALL *
   set_outflow)(axis2_flow_container_t *flow_container,
                 const axis2_env_t *env,
                 axis2_flow_t *outflow);
    
    /**
     * Get fault in flow
     * @return fault in flow
     */
    axis2_flow_t * (AXIS2_CALL *
   get_fault_inflow)(axis2_flow_container_t *flow_container,
                      const axis2_env_t *env);
    
    /**
     * set fault in flow
     * @param falut_inflow falut in flow
     */
    axis2_status_t (AXIS2_CALL *
   set_fault_inflow)(axis2_flow_container_t *flow_container,
                      const axis2_env_t *env,
                      axis2_flow_t *falut_inflow);
    /**
     * Get fault out flow
     * @return fault out flow
     */
    axis2_flow_t *(AXIS2_CALL *
   get_fault_outflow)(axis2_flow_container_t *flow_container,
                       const axis2_env_t *env);
    
    /**
     * Set fault out flow
     * @param fault_outflow fault out flow
     */
    axis2_status_t (AXIS2_CALL *
   set_fault_outflow)(axis2_flow_container_t *flow_container,
                       const axis2_env_t *env,
                       axis2_flow_t *fault_outflow);  
};

/** 
 * @brief Flow container struct
 *   Container for flows  
 */ 
struct axis2_flow_container
{
   axis2_flow_container_ops_t *ops;
};

/**
 * Creates flow container struct
 * @return pointer to newly created flow container
 */
AXIS2_EXTERN axis2_flow_container_t * AXIS2_CALL 
axis2_flow_container_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_FLOW_CONTAINER_FREE(flow_container, env) \
        ((flow_container)->ops->free (flow_container, env))

#define AXIS2_FLOW_CONTAINER_GET_INFLOW(flow_container, env) \
        ((flow_container)->ops->get_inflow (flow_container, env))

#define AXIS2_FLOW_CONTAINER_SET_INFLOW(flow_container, env, inflow) \
        ((flow_container)->ops->set_inflow (flow_container, env, inflow))

#define AXIS2_FLOW_CONTAINER_GET_OUTFLOW(flow_container, env) \
        ((flow_container)->ops->get_outflow (flow_container, env))
        
#define AXIS2_FLOW_CONTAINER_SET_OUTFLOW(flow_container, env, outflow) \
        ((flow_container)->ops->set_outflow (flow_container, env, outflow))

#define AXIS2_FLOW_CONTAINER_GET_FAULT_INFLOW(flow_container, env) \
        ((flow_container)->ops->get_fault_inflow (flow_container, env))
        
#define AXIS2_FLOW_CONTAINER_SET_FAULT_INFLOW(flow_container, env, fault_inflow) \
        ((flow_container)->ops->set_fault_inflow (flow_container, env, fault_inflow))

#define AXIS2_FLOW_CONTAINER_GET_FAULT_OUTFLOW(flow_container, env) \
        ((flow_container)->ops->get_fault_outflow (flow_container, env))
        
#define AXIS2_FLOW_CONTAINER_SET_FAULT_OUTFLOW(flow_container, env, fault_outflow) \
        ((flow_container)->ops->set_fault_outflow (flow_container, env, fault_outflow))        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_FLOW_CONTAINER_H */
