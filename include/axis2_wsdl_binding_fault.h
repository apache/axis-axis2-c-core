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

#ifndef AXIS2_BINDING_FAULT_H
#define AXIS2_BINDING_FAULT_H

/**
 * @file axis2_binding_fault.h
 * @brief Axis2 Wsdl property interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_wsdl_extensible_component.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_extensible_component;    
typedef struct axis2_wsdl_binding_fault axis2_wsdl_binding_fault_t;
typedef struct axis2_wsdl_binding_fault_ops axis2_wsdl_binding_fault_ops_t;

/** @defgroup axis2_binding_fault Wsdl Binding Fault
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Binding Fault ops struct
 * Encapsulator struct for ops of axis2_binding_fault
 */
 struct axis2_wsdl_binding_fault_ops
{
   /** De-allocate memory
      * @return status code
      */
   axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_binding_fault_t *binding_fault,
          const axis2_env_t *env);

    /**
     * Method getRef
     *
     * @return
     */
    axis2_qname_t *(AXIS2_CALL *
    get_ref) (axis2_wsdl_binding_fault_t *binding_fault,
                                const axis2_env_t *env);
    
    /**
     * Method setRef
     *
     * @param ref
     */
    axis2_status_t (AXIS2_CALL *
    set_ref)(axis2_wsdl_binding_fault_t *binding_fault,
                                const axis2_env_t *env,
                                axis2_qname_t *ref);
};

/** 
 * @brief Wsdl binding fault struct  
 */  
 struct axis2_wsdl_binding_fault
{
   axis2_wsdl_binding_fault_ops_t *ops;
    struct axis2_wsdl_extensible_component *extensible_component;
};

/**
 * Creates wsdl binding fault struct
 * @return pointer to newly created wsdl binding fault
 */
AXIS2_EXTERN axis2_wsdl_binding_fault_t * AXIS2_CALL 
axis2_binding_fault_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_BINDING_FAULT_FREE(binding_fault, env) \
      ((binding_fault)->ops->free (binding_fault, env))

#define AXIS2_WSDL_BINDING_FAULT_GET_REF(binding_fault, env) \
      ((binding_fault)->ops->get_ref (binding_fault, env))

#define AXIS2_WSDL_BINDING_FAULT_SET_REF(binding_fault, env, ref) \
      ((binding_fault)->ops->set_ref (binding_fault, env, ref))       

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_BINDING_FAULT_H */
