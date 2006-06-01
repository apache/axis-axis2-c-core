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

#ifndef AXIS2_TRANSPORT_OUT_DESC_H
#define AXIS2_TRANSPORT_OUT_DESC_H

/**
 * @file axis2_transport_out_desc.h
 * @brief Axis2 description transport out interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_array_list.h>
#include <axis2_phase_meta.h>
#include <axis2_phase.h>
#include <axis2_flow.h>
#include <axis2_transport_sender.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_phase;
struct axis2_transport_sender;    
typedef struct axis2_transport_out_desc axis2_transport_out_desc_t;
typedef struct axis2_transport_out_desc_ops axis2_transport_out_desc_ops_t;

/** @defgroup axis2_transport_out_desc Transport in
 * @ingroup axis2_descriptions
 * @{
 */

/** 
 * @brief Description Transport Out ops struct
 * Encapsulator struct for ops of axis2_transport_out_desc
 */
AXIS2_DECLARE_DATA struct axis2_transport_out_desc_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_transport_out_desc_t *transport_out_desc,
			        const axis2_env_t *env);
    
    /**
     * @return
     */
    axis2_qname_t *(AXIS2_CALL *
    get_qname) (struct axis2_transport_out_desc *transport_out,
                const axis2_env_t *env);
    
    /**
     * @param name
     */
    axis2_status_t (AXIS2_CALL *
    set_qname) (struct axis2_transport_out_desc *transport_out,
                const axis2_env_t *env,
                axis2_qname_t *qname);
    
    struct axis2_flow *(AXIS2_CALL *
    get_outflow) (struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_outflow) (struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    struct axis2_flow *outflow);
    
    struct axis2_flow *(AXIS2_CALL *
    get_faultflow) (struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_faultflow) (struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    struct axis2_flow *faultflow);
    
    /**
     * @return
     */
    axis2_transport_sender_t * (AXIS2_CALL *
    get_sender) (struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env);
    
    /**
     * @param receiver
     */
    axis2_status_t (AXIS2_CALL *
    set_sender) (struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    axis2_transport_sender_t *sender);
    
    struct axis2_phase * (AXIS2_CALL *
    get_out_phase) (struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_out_phase) (struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    struct axis2_phase *out_phase);
    
    struct axis2_phase *(AXIS2_CALL *
    get_faultphase) (struct axis2_transport_out_desc *transport_out,
                        const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_faultphase) (struct axis2_transport_out_desc *transport_out,
                        const axis2_env_t *env,
                        struct axis2_phase *faultphase);
        
    axis2_status_t (AXIS2_CALL *
    add_param) (axis2_transport_out_desc_t *transport_out_desc,
                const axis2_env_t *env,
                axis2_param_t *param);

    axis2_param_t *(AXIS2_CALL *
    get_param) (axis2_transport_out_desc_t *transport_out_desc,
                const axis2_env_t *env,
                axis2_char_t *param_name);

    axis2_bool_t (AXIS2_CALL *
    is_param_locked) (axis2_transport_out_desc_t *transport_out_desc,
                      const axis2_env_t *env,
                      axis2_char_t *param_name);

};

/** 
 * @brief Transport Out struct  
 */  
AXIS2_DECLARE_DATA struct axis2_transport_out_desc
{
	axis2_transport_out_desc_ops_t *ops;
    axis2_param_container_t *param_container;
};

/**
 * Creates transport out struct
 * @return pointer to newly created transport out
 */
AXIS2_EXTERN axis2_transport_out_desc_t * AXIS2_CALL 
axis2_transport_out_desc_create_with_qname (const axis2_env_t *env, axis2_qname_t *qname);

/*************************** Function macros **********************************/

#define AXIS2_TRANSPORT_OUT_DESC_FREE(transport_out_desc, env) \
		((transport_out_desc)->ops->free (transport_out_desc, env))

#define AXIS2_TRANSPORT_OUT_DESC_GET_QNAME(transport_out_desc, env) \
		((transport_out_desc)->ops->get_qname (transport_out_desc, env))

#define AXIS2_TRANSPORT_OUT_DESC_SET_QNAME(transport_out_desc, env, qname) \
		((transport_out_desc)->ops->set_qname (transport_out_desc, env, qname))

#define AXIS2_TRANSPORT_OUT_DESC_GET_OUTFLOW(transport_out_desc, env) \
		((transport_out_desc)->ops->get_outflow (transport_out_desc, env))

#define AXIS2_TRANSPORT_OUT_DESC_SET_OUTFLOW(transport_out_desc, env, outflow) \
		((transport_out_desc)->ops->set_outflow (transport_out_desc, env, outflow))       

#define AXIS2_TRANSPORT_OUT_DESC_GET_FAULTFLOW(transport_out_desc, env) \
		((transport_out_desc)->ops->get_faultflow (transport_out_desc, env)) 
        
#define AXIS2_TRANSPORT_OUT_DESC_SET_FAULTFLOW(transport_out_desc, env, faultflow) \
		((transport_out_desc)->ops->set_faultflow (transport_out_desc, env, faultflow)) 

#define AXIS2_TRANSPORT_OUT_DESC_GET_SENDER(transport_out_desc, env) \
		((transport_out_desc)->ops->get_sender (transport_out_desc, env)) 
        
#define AXIS2_TRANSPORT_OUT_DESC_SET_SENDER(transport_out_desc, env, sender) \
		((transport_out_desc)->ops->set_sender (transport_out_desc, env, sender)) 

#define AXIS2_TRANSPORT_OUT_DESC_GET_OUT_PHASE(transport_out_desc, env) \
		((transport_out_desc)->ops->get_out_phase (transport_out_desc, env)) 
        
#define AXIS2_TRANSPORT_OUT_DESC_SET_OUT_PHASE(transport_out_desc, env, out_phase) \
		((transport_out_desc)->ops->set_out_phase (transport_out_desc, env, out_phase)) 

#define AXIS2_TRANSPORT_OUT_DESC_GET_FAULTPHASE(transport_out_desc, env) \
		((transport_out_desc)->ops->get_faultphase (transport_out_desc, env)) 
        
#define AXIS2_TRANSPORT_OUT_DESC_SET_FAULTPHASE(transport_out_desc, env, faultphase) \
		((transport_out_desc)->ops->set_faultphase (transport_out_desc, env, faultphase))

#define AXIS2_TRANSPORT_OUT_DESC_ADD_PARAM(transport_out_desc, env, param) \
		((transport_out_desc)->ops->add_param (transport_out_desc, env, param)) 

#define AXIS2_TRANSPORT_OUT_DESC_GET_PARAM(transport_out_desc, env, param_name) \
		((transport_out_desc)->ops->get_param (transport_out_desc, env, param_name)) 

#define AXIS2_TRANSPORT_OUT_DESC_IS_PARAM_LOCKED(transport_out_desc, env, param_name) \
		((transport_out_desc)->ops->is_param_locked (transport_out_desc, env, param_name)) 

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_OUT_DESC_H */
