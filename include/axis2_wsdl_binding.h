/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_WSDL_BINDING_H
#define AXIS2_WSDL_BINDING_H

/**
 * @file axis2_wsdl_binding.h
 * @brief Axis2 Wsdl binding interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_hash.h>
#include <axis2_wsdl_binding_op.h>
#include <axis2_wsdl_binding_fault.h>
#include <axis2_qname.h>
#include <axis2_wsdl_interface.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_interface;
struct axis2_wsdl_binding_op;
struct axis2_wsdl_binding_fault;
struct axis2_wsdl_extensible_component;    
typedef struct axis2_wsdl_binding axis2_wsdl_binding_t;
typedef struct axis2_wsdl_binding_ops axis2_wsdl_binding_ops_t;

/** @defgroup axis2_wsdl_binding Wsdl Binding
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Binding ops struct
 * Encapsulator struct for ops of axis2_wsdl_binding
 */
 struct axis2_wsdl_binding_ops
{
   /** De-allocate memory
      * @return status code
      */
   axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_binding_t *wsdl_binding,
          const axis2_env_t *env);
                               
    /**
     * Method getBoundInterface
     *
     * @return  WSDLInterface
     */
    struct axis2_wsdl_interface * (AXIS2_CALL *
    get_bound_interface) (axis2_wsdl_binding_t *binding,
                                            const axis2_env_t *env);
    
    /**
     * Method setBoundInterface
     *
     * @param boundInterface
     */
    axis2_status_t (AXIS2_CALL *
    set_bound_interface) (axis2_wsdl_binding_t *binding,
                                            const axis2_env_t *env,
                                            struct axis2_wsdl_interface *bound_interface);
    
    /**
     * Method getName
     *
     * @return  QName
     */
    axis2_qname_t * (AXIS2_CALL *
    get_name) (axis2_wsdl_binding_t *binding,
                                const axis2_env_t *env);
    
    /**
     * Method setName
     *
     * @param name
     */
    axis2_status_t (AXIS2_CALL *
    set_name) (axis2_wsdl_binding_t *binding,
                                const axis2_env_t *env,
                                axis2_qname_t *qname);
    
    /**
     * Method getTargetNameSpace
     *
     * @return String
     */
    axis2_char_t * (AXIS2_CALL *
    get_target_namespace) (axis2_wsdl_binding_t *binding,
                                                const axis2_env_t *env);
    
    /**
     * Method getBindingFaults
     *
     * @return  Hashmap
     */
    axis2_hash_t * (AXIS2_CALL *
    get_binding_faults) (axis2_wsdl_binding_t *binding,
                                            const axis2_env_t *env);
    
    /**
     * Method setBindingFaults
     *
     * @param bindingFaults
     */
    axis2_status_t (AXIS2_CALL *
    set_binding_faults) (axis2_wsdl_binding_t *binding,
                                            const axis2_env_t *env,
                                            axis2_hash_t *binding_faults);
    
    /**
     * Method getBindingOperations
     *
     * @return Hashmap
     */
    axis2_hash_t * (AXIS2_CALL *
    get_binding_ops) (axis2_wsdl_binding_t *binding,
                                                const axis2_env_t *env);
    
    /**
     * Method setBindingOperations
     *
     * @param bindingOperations
     */
    axis2_status_t (AXIS2_CALL *
    set_binding_ops) (axis2_wsdl_binding_t *binding,
                                                const axis2_env_t *env,
                                                axis2_hash_t *binding_ops);
    
    /**
     * Method add_binding_op
     *
     * @param Binding Operation
     */
    axis2_status_t (AXIS2_CALL *
    add_binding_op) (
                            axis2_wsdl_binding_t *binding,
                            const axis2_env_t *env,
                            struct axis2_wsdl_binding_op *binding_op);
    
    /**
     * Method get_binding_op
     *
     * @param qName
     * @return WSDL Binding Operation
     */
    struct axis2_wsdl_binding_op *(AXIS2_CALL *
    get_binding_op) (axis2_wsdl_binding_t *binding,
                                                const axis2_env_t *env,
                                                axis2_qname_t *qname);
    
    /**
     * Method addBindingFaults
     *
     * @param bindingFault
     */
    axis2_status_t (AXIS2_CALL *
    add_binding_fault) (axis2_wsdl_binding_t *binding,
                                            const axis2_env_t *env,
                                            struct axis2_wsdl_binding_fault *binding_fault);
    
    /**
     * Method getBindingFault
     *
     * @param ref
     * @return WSDLBindingFault
     */
    struct axis2_wsdl_binding_fault * (AXIS2_CALL *
    get_binding_fault) (axis2_wsdl_binding_t *binding,
                                            const axis2_env_t *env,
                                            axis2_qname_t *ref);

};

/** 
 * @brief Wsdl binding struct  
 */  
 struct axis2_wsdl_binding
{
   axis2_wsdl_binding_ops_t *ops;
    struct axis2_wsdl_extensible_component *extensible_component;
};

/**
 * Creates wsdl binding struct
 * @return pointer to newly created wsdl binding
 */
AXIS2_EXTERN axis2_wsdl_binding_t *AXIS2_CALL axis2_wsdl_binding_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_BINDING_FREE(wsdl_binding, env) \
      ((wsdl_binding)->ops->free (wsdl_binding, env))

#define AXIS2_WSDL_BINDING_GET_BOUND_INTERFACE(wsdl_binding, env) \
      ((wsdl_binding)->ops->get_bound_interface (wsdl_binding, env))

#define AXIS2_WSDL_BINDING_SET_BOUND_INTERFACE(wsdl_binding, env, interface) \
      ((wsdl_binding)->ops->set_bound_interface (wsdl_binding, env, interface))
        
#define AXIS2_WSDL_BINDING_GET_NAME(wsdl_binding, env) \
      ((wsdl_binding)->ops->get_name (wsdl_binding, env))

#define AXIS2_WSDL_BINDING_SET_NAME(wsdl_binding, env, qname) \
      ((wsdl_binding)->ops->set_name (wsdl_binding, env, qname))        

#define AXIS2_WSDL_BINDING_GET_TARGET_NAMESPACE(wsdl_binding, env) \
      ((wsdl_binding)->ops->get_target_namespace (wsdl_binding, env))

#define AXIS2_WSDL_BINDING_GET_BINDING_FAULTS(wsdl_binding, env) \
      ((wsdl_binding)->ops->get_binding_faults (wsdl_binding, env)) 
        
#define AXIS2_WSDL_BINDING_SET_BINDING_FAULTS(wsdl_binding, env, faults) \
      ((wsdl_binding)->ops->set_binding_faults (wsdl_binding, env, faults))

#define AXIS2_WSDL_BINDING_GET_BINDING_OPS(wsdl_binding, env) \
      ((wsdl_binding)->ops->get_binding_ops (wsdl_binding, env))

#define AXIS2_WSDL_BINDING_SET_BINDING_OPS(wsdl_binding, env, ops) \
      ((wsdl_binding)->ops->set_binding_ops (wsdl_binding, env, ops)) 
        
#define AXIS2_WSDL_BINDING_ADD_BINDING_OP(wsdl_binding, env, op) \
      ((wsdl_binding)->ops->add_binding_op (wsdl_binding, env, op))

#define AXIS2_WSDL_BINDING_GET_BINDING_OP(wsdl_binding, env, qname) \
      ((wsdl_binding)->ops->get_binding_op (wsdl_binding, env, qname)) 

#define AXIS2_WSDL_BINDING_ADD_BINDING_FAULT(wsdl_binding, env, op) \
      ((wsdl_binding)->ops->add_binding_fault (wsdl_binding, env, fault))

#define AXIS2_WSDL_BINDING_GET_BINDING_FAULT(wsdl_binding, env) \
      ((wsdl_binding)->ops->get_binding_fault (wsdl_binding, env)) 
/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_BINDING_H */
