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

#ifndef AXIS2_TRANSPORT_IN_DESC_H
#define AXIS2_TRANSPORT_IN_DESC_H

/**
 * @file axis2_transport_in_desc.h
 * @brief Axis2 description transport in interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_array_list.h>
#include <axis2_transport_listener.h>
#include <axis2_phase_meta.h>
#include <axis2_phase.h>
#include <axis2_flow.h>
#include <axis2_wsdl.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_phase;
struct axis2_transport_listener;    
typedef struct axis2_transport_in_desc axis2_transport_in_desc_t;
typedef struct axis2_transport_in_desc_ops axis2_transport_in_desc_ops_t;

/** @defgroup axis2_transport_in_desc Transport in
 * @ingroup axis2_descriptions
 * @{
 */

/** 
 * @brief Description Transport In ops struct
 * Encapsulator struct for ops of axis2_transport_in_desc
 */
AXIS2_DECLARE_DATA struct axis2_transport_in_desc_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_transport_in_desc_t *transport_in_desc,
			        axis2_env_t **env);
    
    /**
     * @return
     */
    axis2_qname_t *(AXIS2_CALL *
    get_qname) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env);
    
    /**
     * @param name
     */
    axis2_status_t (AXIS2_CALL *
    set_qname) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname);
    
    struct axis2_flow *(AXIS2_CALL *
    get_inflow) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_inflow) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env,
                                            struct axis2_flow *inflow);
    
    struct axis2_flow *(AXIS2_CALL *
    get_faultflow) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_faultflow) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env,
                                            struct axis2_flow *faultflow);
    
    /**
     * @return
     */
    struct axis2_transport_listener * (AXIS2_CALL *
    get_recv) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env);
    
    /**
     * @param receiver
     */
    axis2_status_t (AXIS2_CALL *
    set_recv) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env,
                                            struct axis2_transport_listener *recv);
    
    struct axis2_phase * (AXIS2_CALL *
    get_in_phase) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_in_phase) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env,
                                            struct axis2_phase *in_phase);
    
    struct axis2_phase *(AXIS2_CALL *
    get_faultphase) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_faultphase) (struct axis2_transport_in_desc *transport_in,
                                            axis2_env_t **env,
                                            struct axis2_phase *faultphase);
        

};

/** 
 * @brief Transport In struct  
 */  
AXIS2_DECLARE_DATA struct axis2_transport_in_desc
{
	axis2_transport_in_desc_ops_t *ops;
    axis2_param_container_t *param_container;
};

/**
 * Creates phase holder struct
 * @return pointer to newly created phase holder
 */
AXIS2_DECLARE(axis2_transport_in_desc_t *) 
axis2_transport_in_desc_create_with_qname (axis2_env_t **env, axis2_qname_t *qname);

/*************************** Function macros **********************************/

#define AXIS2_TRANSPORT_IN_DESC_FREE(transport_in_desc, env) \
		((transport_in_desc)->ops->free (transport_in_desc, env))

#define AXIS2_TRANSPORT_IN_DESC_GET_QNAME(transport_in_desc, env) \
		((transport_in_desc)->ops->get_qname (transport_in_desc, env))

#define AXIS2_TRANSPORT_IN_DESC_SET_QNAME(transport_in_desc, env, qname) \
		((transport_in_desc)->ops->set_qname (transport_in_desc, env, qname))

#define AXIS2_TRANSPORT_IN_DESC_GET_INFLOW(transport_in_desc, env) \
		((transport_in_desc)->ops->get_inflow (transport_in_desc, env))

#define AXIS2_TRANSPORT_IN_DESC_SET_INFLOW(transport_in_desc, env, inflow) \
		((transport_in_desc)->ops->set_inflow (transport_in_desc, env, inflow))       

#define AXIS2_TRANSPORT_IN_DESC_GET_FAULTFLOW(transport_in_desc, env) \
		((transport_in_desc)->ops->get_faultflow (transport_in_desc, env)) 
        
#define AXIS2_TRANSPORT_IN_DESC_SET_FAULTFLOW(transport_in_desc, env, faultflow) \
		((transport_in_desc)->ops->set_faultflow (transport_in_desc, env, faultflow)) 

#define AXIS2_TRANSPORT_IN_DESC_GET_RECV(transport_in_desc, env) \
		((transport_in_desc)->ops->get_recv (transport_in_desc, env)) 
        
#define AXIS2_TRANSPORT_IN_DESC_SET_RECV(transport_in_desc, env, recv) \
		((transport_in_desc)->ops->set_recv (transport_in_desc, env, recv)) 

#define AXIS2_TRANSPORT_IN_DESC_GET_IN_PHASE(transport_in_desc, env) \
		((transport_in_desc)->ops->get_in_phase (transport_in_desc, env)) 
        
#define AXIS2_TRANSPORT_IN_DESC_SET_IN_PHASE(transport_in_desc, env, in_phase) \
		((transport_in_desc)->ops->set_in_phase (transport_in_desc, env, in_phase)) 

#define AXIS2_TRANSPORT_IN_DESC_GET_FAULTPHASE(transport_in_desc, env) \
		((transport_in_desc)->ops->get_faultphase (transport_in_desc, env)) 
        
#define AXIS2_TRANSPORT_IN_DESC_SET_FAULTPHASE(transport_in_desc, env, faultphase) \
		((transport_in_desc)->ops->set_faultphase (transport_in_desc, env, faultphase))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_IN_DESC_H */
