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

#ifndef AXIS2_WSDL_SOAP_OP_H
#define AXIS2_WSDL_SOAP_OP_H

/**
 * @file axis2_wsdl_soap_op.h
 * @brief Axis2 Wsdl soap op interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_wsdl_extensible_element.h>
#include <axis2_wsdl.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_wsdl_soap_op axis2_wsdl_soap_op_t;
typedef struct axis2_wsdl_soap_op_ops axis2_wsdl_soap_op_ops_t;

/** @defgroup axis2_wsdl_soap_op Wsdl Extensible Element
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Extensible Element ops struct
 * Encapsulator struct for ops of axis2_wsdl_soap_op
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_soap_op_ops
{
   /** De-allocate memory
      * @return status code
      */
   axis2_status_t (AXIS2_CALL *free)(
                    axis2_wsdl_soap_op_t *wsdl_soap_op,
                 const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_style) (axis2_wsdl_soap_op_t *soap_op,
                                        const axis2_env_t *env);
        
    axis2_status_t (AXIS2_CALL *
    set_style) (axis2_wsdl_soap_op_t *soap_op,
                                        const axis2_env_t *env,
                                        axis2_char_t *style);
    
    axis2_char_t * (AXIS2_CALL *
    get_soap_action) (axis2_wsdl_soap_op_t *soap_op,
                                        const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_soap_action) (axis2_wsdl_soap_op_t *soap_op,
                                        const axis2_env_t *env,
                                        axis2_char_t *soap_action);
};

/** 
 * @brief Wsdl container struct
 *   Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_soap_op
{
   axis2_wsdl_soap_op_ops_t *ops;
    axis2_wsdl_extensible_element_t *extensible_element;
};

/**
 * Creates wsdl property struct
 * @return pointer to newly created wsdl property
 */
AXIS2_EXTERN axis2_wsdl_soap_op_t * AXIS2_CALL 
axis2_wsdl_soap_op_create (const axis2_env_t *env);

/**
 * Creates wsdl property struct
 * @param type 
 * @return pointer to newly created wsdl property
 */
AXIS2_EXTERN axis2_wsdl_soap_op_t * AXIS2_CALL 
axis2_wsdl_soap_op_create_with_type (const axis2_env_t *env, axis2_qname_t *type);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_SOAP_OP_FREE(soap_op, env) \
      ((soap_op)->ops->free (soap_op, env))

#define AXIS2_WSDL_SOAP_OP_GET_STYLE(soap_op, env) \
      ((soap_op)->ops->get_style (soap_op, env))

#define AXIS2_WSDL_SOAP_OP_SET_STYLE(soap_op, env, style) \
      ((soap_op)->ops->set_style (soap_op, env, style))

#define AXIS2_WSDL_SOAP_OP_GET_SOAP_ACTION(soap_op, env) \
      ((soap_op)->ops->get_soap_action (soap_op, env))

#define AXIS2_WSDL_SOAP_OP_SET_SOAP_ACTION(soap_op, env, soap_action) \
      ((soap_op)->ops->set_soap_action (soap_op, env, soap_action))       

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_SOAP_OP_H */
