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

#ifndef AXIS2_WSDL_SOAP_OPERATION_H
#define AXIS2_WSDL_SOAP_OPERATION_H

/**
 * @file axis2_wsdl_soap_operation.h
 * @brief Axis2 Wsdl soap operation interface
 */

#include <axis2.h>
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

typedef struct axis2_wsdl_soap_operation axis2_wsdl_soap_operation_t;
typedef struct axis2_wsdl_soap_operation_ops axis2_wsdl_soap_operation_ops_t;

/** @defgroup axis2_wsdl_soap_operation Wsdl Extensible Element
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Extensible Element operations struct
 * Encapsulator struct for operations of axis2_wsdl_soap_operation
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_soap_operation_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_wsdl_soap_operation_t *wsdl_soap_operation,
			        axis2_env_t **env);

    axis2_char_t *(AXIS2_CALL *
    get_style) (axis2_wsdl_soap_operation_t *soap_operation,
                                        axis2_env_t **env);
        
    axis2_status_t (AXIS2_CALL *
    set_style) (axis2_wsdl_soap_operation_t *soap_operation,
                                        axis2_env_t **env,
                                        axis2_char_t *style);
    
    axis2_char_t * (AXIS2_CALL *
    get_soap_action) (axis2_wsdl_soap_operation_t *soap_operation,
                                        axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_soap_action) (axis2_wsdl_soap_operation_t *soap_operation,
                                        axis2_env_t **env,
                                        axis2_char_t *soap_action);
};

/** 
 * @brief Wsdl container struct
 *	Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_soap_operation
{
	axis2_wsdl_soap_operation_ops_t *ops;
    axis2_wsdl_extensible_element_t *extensible_element;
};

/**
 * Creates wsdl property struct
 * @return pointer to newly created wsdl property
 */
AXIS2_DECLARE(axis2_wsdl_soap_operation_t *) 
axis2_wsdl_soap_operation_create (axis2_env_t **env);

/**
 * Creates wsdl property struct
 * @param type 
 * @return pointer to newly created wsdl property
 */
AXIS2_DECLARE(axis2_wsdl_soap_operation_t *) 
axis2_wsdl_soap_operation_create_with_type (axis2_env_t **env, axis2_qname_t *type);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_SOAP_OPERATION_FREE(extensible_element, env) \
		((extensible_element->ops)->free (extensible_element, env))

#define AXIS2_WSDL_SOAP_OPERATION_GET_STYLE(extensible_element, env) \
		((extensible_element->ops)->get_style (extensible_element, env))

#define AXIS2_WSDL_SOAP_OPERATION_SET_STYLE(extensible_element, env, style) \
		((extensible_element->ops)->set_style (extensible_element, env, style))

#define AXIS2_WSDL_SOAP_OPERATION_GET_SOAP_ACTION(extensible_element, env) \
		((extensible_element->ops)->get_soap_action (extensible_element, env))

#define AXIS2_WSDL_SOAP_OPERATION_SET_SOAP_ACTION(extensible_element, env, soap_action) \
		((extensible_element->ops)->set_soap_action (extensible_element, env, soap_action))       

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_SOAP_OPERATION_H */
