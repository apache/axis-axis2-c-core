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

#ifndef AXIS2_WSDL_OP_H
#define AXIS2_WSDL_OP_H

/**
 * @file axis2_wsdl_op.h
 * @brief axis2 wsdl op interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>

#include <axis2_qname.h>
#include <axis2_wsdl_component.h>
#include <axis2_linked_list.h>
#include <axis2_wsdl_msg_ref.h>
#include <axis2_wsdl_fault_ref.h>
#include <axis2_wsdl_svc.h>

#ifdef __cplusplus
extern "C"
{
#endif
struct axis2_wsdl_extensible_component;   
struct axis2_wsdl_fault_ref;    
typedef struct axis2_wsdl_op_ops axis2_wsdl_op_ops_t;
typedef struct axis2_wsdl_op axis2_wsdl_op_t;   
   

/** @defgroup axis2_wsdl_op Wsdl Operation
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_wsdl_op_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_wsdl_op_t *wsdl_op,
            const axis2_env_t *env);
        
   /** 
     * Deallocate memory
     * @param void wsdl_op to be freed
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free_void_arg) (void *wsdl_op,
                    const axis2_env_t *env);
            
    /**
     * set the message exchange pattern
     * @param axis2_char_t message exchange pattern
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    set_msg_exchange_pattern) (axis2_wsdl_op_t *wsdl_op, 
                                const axis2_env_t *env, 
                                const axis2_char_t *msg_exchange_pattern);
  
    /**
     * get the message exchange pattern
     * @return axis2_char_t message exchange pattern
     */
    axis2_char_t *(AXIS2_CALL *
    get_msg_exchange_pattern) (axis2_wsdl_op_t *wsdl_op, 
                                const axis2_env_t *env);

    /**
     * Set the wsdl op name
     * @param axis2_qname_t* op name
     */
    axis2_status_t (AXIS2_CALL *
    set_qname) (axis2_wsdl_op_t *wsdl_op, 
                const axis2_env_t *env,
                const axis2_qname_t *name);
                                        
    /**
     * Get the name of wsdl op
     * @return axis2_qname_t wsdl op name
     */
    const axis2_qname_t *(AXIS2_CALL *
    get_qname) (void *wsdl_op,
                const axis2_env_t *env);
    

    /**
     * set style
     * @param axis2_char_t style
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    set_style) (axis2_wsdl_op_t *wsdl_op,
                const axis2_env_t *env,
                const axis2_char_t *style);
    
    /**
     * Get style
     * @return axis2_char_t style
     */
    axis2_char_t *(AXIS2_CALL *
    get_style) (axis2_wsdl_op_t *wsdl_op,
                const axis2_env_t *env);
    
    
    /**
     * Method getInfaults
     *
     * @return
     */
    axis2_linked_list_t * (AXIS2_CALL *
    get_in_faults) (axis2_wsdl_op_t *wsdl_op,
                   const axis2_env_t *env);
    
    /**
     * Method setInfaults
     *
     * @param in_faults
     */
    axis2_status_t (AXIS2_CALL *
    set_in_faults) (axis2_wsdl_op_t *wsdl_op,
                    const axis2_env_t *env,
                    axis2_linked_list_t *in_faults);
    
    /**
     * Method getInputMessage
     *
     * @return
     */
    struct axis2_wsdl_msg_ref *(AXIS2_CALL *
    get_input_msg) (axis2_wsdl_op_t *wsdl_op,
                                        const axis2_env_t *env);
    
    /**
     * Method setInputMessage
     *
     * @param inputMessage
     */
    axis2_status_t (AXIS2_CALL *
    set_input_msg) (axis2_wsdl_op_t *wsdl_op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_msg_ref *input_msg);
    
    /**
     * Method getOutfaults
     *
     * @return
     */
    axis2_linked_list_t *(AXIS2_CALL *
    get_out_faults)(axis2_wsdl_op_t *wsdl_op,
                                        const axis2_env_t *env);
    
    /**
     * Method setOutfaults
     *
     * @param out_faults
     */
    axis2_status_t (AXIS2_CALL *
    set_out_faults) (axis2_wsdl_op_t *wsdl_op,
                                        const axis2_env_t *env,
                                        axis2_linked_list_t *out_faults);
    
    /**
     * Method getOutputMessage
     *
     * @return
     */
    struct axis2_wsdl_msg_ref *(AXIS2_CALL *
    get_output_msg) (axis2_wsdl_op_t *wsdl_op,
                                        const axis2_env_t *env);
    
    /**
     * Method setOutputMessage
     *
     * @param outputMessage
     */
    axis2_status_t (AXIS2_CALL *
    set_output_msg) (axis2_wsdl_op_t *wsdl_op,
                        const axis2_env_t *env,
                        struct axis2_wsdl_msg_ref *output_msg);
    
    /**
     * Method isSafe
     *
     * @return
     */
    axis2_bool_t (AXIS2_CALL *
    is_safe) (axis2_wsdl_op_t *wsdl_op,
                const axis2_env_t *env);
    
    /**
     * Method setSafety
     *
     * @param safe
     */
    axis2_status_t (AXIS2_CALL *
    set_safety) (axis2_wsdl_op_t *wsdl_op,
                    const axis2_env_t *env,
                    axis2_bool_t safe);
    
    /**
     * Will return the Namespace of the QName of this <code>WSDLOperation</code>. Will return null if not set.
     *
     * @return
     */
    axis2_char_t *(AXIS2_CALL *
    get_target_namespace) (axis2_wsdl_op_t *wsdl_op,
                            const axis2_env_t *env);
    
    /**
     * Add the InFault to the Components InFaults
     *
     * @param inFault
     */
    axis2_status_t (AXIS2_CALL *
    add_in_fault) (axis2_wsdl_op_t *wsdl_op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_fault_ref *in_fault);
    
    /**
     * Add the OutFault to the Component OutFaults
     *
     * @param outFault
     */
    axis2_status_t (AXIS2_CALL *
    add_out_fault) (axis2_wsdl_op_t *wsdl_op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_fault_ref *out_fault);
};

struct axis2_wsdl_op
{
   axis2_wsdl_op_ops_t *ops;
    struct axis2_wsdl_extensible_component *extensible_component;
};

AXIS2_EXTERN axis2_wsdl_op_t * AXIS2_CALL 
axis2_wsdl_op_create (const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_wsdl_op_free_void_arg(void *wsdl_op,
                        const axis2_env_t *env);
                        
/**************************** Start of function macros ************************/

#define AXIS2_WSDL_OP_FREE(wsdl_op, env) \
      ((wsdl_op)->ops->free (wsdl_op, env))

#define AXIS2_WSDL_OP_FREE_VOID_ARG(wsdl_op, env) \
      (((axis2_wsdl_op_t *) wsdl_op)->ops->free_void_arg(wsdl_op, env))
   
#define AXIS2_WSDL_OP_GET_MSG_EXCHANGE_PATTERN(wsdl_op, env) \
      ((wsdl_op)->ops->get_msg_exchange_pattern(wsdl_op, env))
      
#define AXIS2_WSDL_OP_SET_MSG_EXCHANGE_PATTERN(wsdl_op, env, pattern) \
      ((wsdl_op)->ops->set_msg_exchange_pattern(wsdl_op, env, pattern))
      
#define AXIS2_WSDL_OP_GET_QNAME(wsdl_op, env) \
      (((axis2_wsdl_op_t *) wsdl_op)->ops->get_qname(wsdl_op, env))
      
#define AXIS2_WSDL_OP_SET_QNAME(wsdl_op, env, name) \
      ((wsdl_op)->ops->set_qname(wsdl_op, env, name))
      
#define AXIS2_WSDL_OP_SET_STYLE(wsdl_op, env, style) \
      ((wsdl_op)->ops->set_style(wsdl_op, env, style))
      
#define AXIS2_WSDL_OP_GET_STYLE(wsdl_op, env) \
      ((wsdl_op)->ops->get_style(wsdl_op, env))
      
#define AXIS2_WSDL_OP_GET_IN_FAULTS(wsdl_op, env) \
      ((wsdl_op)->ops->get_in_faults(wsdl_op, env))
      
#define AXIS2_WSDL_OP_SET_IN_FAULTS(wsdl_op, env, in_faults) \
      ((wsdl_op)->ops->set_in_faults(wsdl_op, env, in_faults))

#define AXIS2_WSDL_OP_GET_INPUT_MSG(wsdl_op, env) \
      ((wsdl_op)->ops->get_input_msg(wsdl_op, env))
      
#define AXIS2_WSDL_OP_SET_INPUT_MSG(wsdl_op, env, input_msg) \
      ((wsdl_op)->ops->set_input_msg(wsdl_op, env, input_msg))      

#define AXIS2_WSDL_OP_GET_OUT_FAULTS(wsdl_op, env) \
      ((wsdl_op)->ops->get_out_faults(wsdl_op, env))
      
#define AXIS2_WSDL_OP_SET_OUT_FAULTS(wsdl_op, env, out_faults) \
      ((wsdl_op)->ops->set_out_faults(wsdl_op, env, out_faults))

#define AXIS2_WSDL_OP_GET_OUTPUT_MSG(wsdl_op, env) \
      ((wsdl_op)->ops->get_output_msg(wsdl_op, env))
      
#define AXIS2_WSDL_OP_SET_OUTPUT_MSG(wsdl_op, env, output_msg) \
      ((wsdl_op)->ops->set_output_msg(wsdl_op, env, output_msg))

#define AXIS2_WSDL_OP_IS_SAFE(wsdl_op, env) \
      ((wsdl_op)->ops->is_safe(wsdl_op, env))
      
#define AXIS2_WSDL_OP_SET_SAFETY(wsdl_op, env, safe) \
      ((wsdl_op)->ops->set_safety(wsdl_op, env, safe))
        
#define AXIS2_WSDL_OP_GET_TARGET_NAMESPACE(wsdl_op, env) \
      ((wsdl_op)->ops->get_target_namespace(wsdl_op, env))        

#define AXIS2_WSDL_OP_ADD_IN_FAULT(wsdl_op, env, in_fault) \
      ((wsdl_op)->ops->add_in_fault(wsdl_op, env, in_fault))
      
#define AXIS2_WSDL_OP_ADD_OUT_FAULT(wsdl_op, env, out_fault) \
      ((wsdl_op)->ops->add_out_fault(wsdl_op, env, out_fault))

/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WSDL_OP_H  */
