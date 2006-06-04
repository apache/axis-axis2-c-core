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

#ifndef AXIS2_WSDL_FAULT_REF_H
#define AXIS2_WSDL_FAULT_REF_H

/**
 * @file axis2_wsdl_fault_ref.h
 * @brief Axis2 Wsdl bindig message reference interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_wsdl_component.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_component;    
typedef struct axis2_wsdl_fault_ref axis2_wsdl_fault_ref_t;
typedef struct axis2_wsdl_fault_ref_ops axis2_wsdl_fault_ref_ops_t;

/** @defgroup axis2_wsdl_fault_ref Wsdl Binding Fault
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Binding Message Reference ops struct
 * Encapsulator struct for ops of axis2_wsdl_fault_ref
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_fault_ref_ops
{
   /** De-allocate memory
      * @return status code
      */
   axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_fault_ref_t *wsdl_fault_ref,
          const axis2_env_t *env);

    /**
     * Returns the direction of the Fault according the MEP
     *
     * @return
     */
    axis2_char_t * (AXIS2_CALL *
    get_direction) (axis2_wsdl_fault_ref_t *fault_ref,
                                                const axis2_env_t *env);
    
    /**
     * Sets the direction of the Fault.
     *
     * @param direction
     */
    axis2_status_t (AXIS2_CALL *
    set_direction) (axis2_wsdl_fault_ref_t *fault_ref,
                                                const axis2_env_t *env,
                                                axis2_char_t *direction);
    
    /**
     * Method getMessageLabel
     *
     * @return
     */
    axis2_char_t * (AXIS2_CALL *
    get_msg_label) (axis2_wsdl_fault_ref_t *fault_ref,
                                                const axis2_env_t *env);
    
    /**
     * Method setMessageLabel
     *
     * @param messageLabel
     */
    axis2_status_t (AXIS2_CALL *
    set_msg_label) (axis2_wsdl_fault_ref_t *fault_ref,
                                                const axis2_env_t *env,
                                                axis2_char_t *msg_label);
                                                
    /**
     * Returns the Fault reference.
     *
     * @return
     */
    axis2_qname_t * (AXIS2_CALL *
    get_ref) (axis2_wsdl_fault_ref_t *fault_ref,
                                    const axis2_env_t *env);
    
    /**
     * Sets the Fault reference.
     *
     * @param ref
     */
    axis2_status_t (AXIS2_CALL *
    set_ref) (axis2_wsdl_fault_ref_t *fault_ref,
                                    const axis2_env_t *env,
                                    axis2_qname_t *ref);                                              
};

/** 
 * @brief Wsdl  message reference struct  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_fault_ref
{
   axis2_wsdl_fault_ref_ops_t *ops;
    struct axis2_wsdl_component *wsdl_component;
};

/**
 * Creates wsdl  message reference struct
 * @return pointer to newly created wsdl  message reference
 */
AXIS2_EXTERN axis2_wsdl_fault_ref_t * AXIS2_CALL 
axis2_wsdl_fault_ref_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_FAULT_REF_FREE(wsdl_fault_ref, env) \
      ((wsdl_fault_ref)->ops->free (wsdl_fault_ref, env))

#define AXIS2_WSDL_FAULT_REF_GET_DIRECTION(wsdl_fault_ref, env) \
      ((wsdl_fault_ref)->ops->get_direction (wsdl_fault_ref, env))

#define AXIS2_WSDL_FAULT_REF_SET_DIRECTION(wsdl_fault_ref, env, direction) \
      ((wsdl_fault_ref)->ops->set_direction (wsdl_fault_ref, env, direction))       

#define AXIS2_WSDL_FAULT_REF_GET_MSG_LABEL(wsdl_fault_ref, env) \
      ((wsdl_fault_ref)->ops->get_msg_label (wsdl_fault_ref, env))

#define AXIS2_WSDL_FAULT_REF_SET_MSG_LABEL(wsdl_fault_ref, env, msg_label) \
      ((wsdl_fault_ref)->ops->set_msg_label (wsdl_fault_ref, env, msg_label))
        
#define AXIS2_WSDL_FAULT_REF_GET_REF(wsdl_fault_ref, env) \
      ((wsdl_fault_ref)->ops->get_ref (wsdl_fault_ref, env))

#define AXIS2_WSDL_FAULT_REF_SET_REF(wsdl_fault_ref, env, ref) \
      ((wsdl_fault_ref)->ops->set_ref (wsdl_fault_ref, env, ref))        
        
/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_FAULT_REF_H */
