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

#ifndef AXIS2_WSDL_EXTENSIBLE_ELEMENT_H
#define AXIS2_WSDL_EXTENSIBLE_ELEMENT_H

/**
 * @file axis2_wsdl_extensible_element.h
 * @brief Axis2 Wsdl extensible component interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_wsdl_extensible_element axis2_wsdl_extensible_element_t;
typedef struct axis2_wsdl_extensible_element_ops axis2_wsdl_extensible_element_ops_t;

/** @defgroup axis2_wsdl_extensible_element Wsdl Extensible Element
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Extensible Element ops struct
 * Encapsulator struct for ops of axis2_wsdl_extensible_element
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_extensible_element_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_wsdl_extensible_element_t *wsdl_extensible_element,
			        axis2_env_t **env);

    axis2_bool_t (AXIS2_CALL *
    is_required) (axis2_wsdl_extensible_element_t *extensible_element,
                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_required) (axis2_wsdl_extensible_element_t *extensible_element,
                   axis2_env_t **env,
                   axis2_bool_t required);
    
    axis2_qname_t *(AXIS2_CALL *
    get_type) (axis2_wsdl_extensible_element_t *extensible_element,
               axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_type) (axis2_wsdl_extensible_element_t *extensible_element,
               axis2_env_t **env,
               axis2_qname_t *type);
};

/** 
 * @brief Wsdl container struct
 *	Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_extensible_element
{
	axis2_wsdl_extensible_element_ops_t *ops;
};

/**
 * Creates wsdl property struct
 * @return pointer to newly created wsdl property
 */
AXIS2_DECLARE(axis2_wsdl_extensible_element_t *) 
axis2_wsdl_extensible_element_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_EXTENSIBLE_ELEMENT_FREE(extensible_element, env) \
		((extensible_element)->ops->free (extensible_element, env))

#define AXIS2_WSDL_EXTENSIBLE_ELEMENT_IS_REQUIRED(extensible_element, env) \
		((extensible_element)->ops->is_required (extensible_element, env))

#define AXIS2_WSDL_EXTENSIBLE_ELEMENT_SET_REQUIRED(extensible_element, env, required) \
		((extensible_element)->ops->set_required (extensible_element, env, required))

#define AXIS2_WSDL_EXTENSIBLE_ELEMENT_GET_TYPE(extensible_element, env) \
		(extensible_element->ops->get_type (extensible_element, env))

#define AXIS2_WSDL_EXTENSIBLE_ELEMENT_SET_TYPE(extensible_element, env, type) \
		((extensible_element)->ops->set_type (extensible_element, env, type))       

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_EXTENSIBLE_ELEMENT_H */
