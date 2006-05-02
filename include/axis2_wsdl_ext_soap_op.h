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

#ifndef AXIS2_WSDL_EXT_SOAP_OP_H
#define AXIS2_WSDL_EXT_SOAP_OP_H

/**
 * @file axis2_wsdl_ext_soap_op.h
 * @brief Axis2 Wsdl ext_soap_op interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_wsdl_component.h>
#include <axis2_wsdl_ext.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_component;    
typedef struct axis2_wsdl_ext_soap_op axis2_wsdl_ext_soap_op_t;
typedef struct axis2_wsdl_ext_soap_op_ops axis2_wsdl_ext_soap_op_ops_t;

/** @defgroup axis2_wsdl_ext_soap_op Wsdl Ext Soap Operation
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Ext Soap Operation ops struct
 * Encapsulator struct for ops of axis2_wsdl_ext_soap_op
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_ext_soap_op_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_ext_soap_op_t *ext_soap_op,
	        axis2_env_t **env);

    axis2_char_t *(AXIS2_CALL *
    get_soap_action) (axis2_wsdl_ext_soap_op_t *ext_soap_op,
                                    axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_soap_action) (axis2_wsdl_ext_soap_op_t *ext_soap_op,
                                    axis2_env_t **env,
                                    axis2_char_t *soap_action);

    axis2_char_t *(AXIS2_CALL *
    get_style) (axis2_wsdl_ext_soap_op_t *ext_soap_op,
                                    axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_style) (axis2_wsdl_ext_soap_op_t *ext_soap_op,
                                    axis2_env_t **env,
                                    axis2_char_t *style);
     
};

/** 
 * @brief Wsdl container struct
 *	Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_ext_soap_op
{
    axis2_wsdl_ext_t base;
	axis2_wsdl_ext_soap_op_ops_t *ops;
    struct axis2_wsdl_extensible_element *ext_element;
};

/**
 * Creates wsdl ext_soap_op struct
 * @return pointer to newly created wsdl ext_soap_op
 */
AXIS2_DECLARE(axis2_wsdl_ext_soap_op_t *) 
axis2_wsdl_ext_soap_op_create (axis2_env_t **env,
                                    axis2_qname_t *qtype);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_EXT_SOAP_OP_FREE(ext_soap_op, env) \
		((ext_soap_op)->ops->free (ext_soap_op, env))

#define AXIS2_WSDL_EXT_SOAP_OP_GET_SOAP_ACTION(ext_soap_op, env) \
		((ext_soap_op)->ops->get_soap_action (ext_soap_op, env))

#define AXIS2_WSDL_EXT_SOAP_OP_SET_SOAP_ACTION(ext_soap_op, env, soap_action) \
		((ext_soap_op)->ops->set_soap_action (ext_soap_op, env, soap_action))
        
#define AXIS2_WSDL_EXT_SOAP_OP_GET_STYLE(ext_soap_op, env) \
		((ext_soap_op)->ops->get_style (ext_soap_op, env))

#define AXIS2_WSDL_EXT_SOAP_OP_SET_STYLE(ext_soap_op, env, style) \
		((ext_soap_op)->ops->set_style (ext_soap_op, env, style))        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_EXT_SOAP_OP_H */
