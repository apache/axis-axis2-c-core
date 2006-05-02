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

#ifndef AXIS2_WSDL_EXTENSIBLE_ATTRIBUTE_H
#define AXIS2_WSDL_EXTENSIBLE_ATTRIBUTE_H

/**
 * @file axis2_wsdl_extensible_attribute.h
 * @brief Axis2 Wsdl extensible component interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_wsdl_extensible_attribute axis2_wsdl_extensible_attribute_t;
typedef struct axis2_wsdl_extensible_attribute_ops axis2_wsdl_extensible_attribute_ops_t;

/** @defgroup axis2_wsdl_extensible_attribute Wsdl Extensible Attribute
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Extensible Attribute ops struct
 * Encapsulator struct for ops of axis2_wsdl_extensible_attribute
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_extensible_attribute_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_wsdl_extensible_attribute_t *wsdl_extensible_attribute,
			        axis2_env_t **env);

    axis2_qname_t *(AXIS2_CALL *
    get_key) (axis2_wsdl_extensible_attribute_t *extensible_attribute,
               axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_key) (axis2_wsdl_extensible_attribute_t *extensible_attribute,
               axis2_env_t **env,
               axis2_qname_t *key);
    
    axis2_qname_t *(AXIS2_CALL *
    get_value) (axis2_wsdl_extensible_attribute_t *extensible_attribute,
               axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_value) (axis2_wsdl_extensible_attribute_t *extensible_attribute,
               axis2_env_t **env,
               axis2_qname_t *value);
};

/** 
 * @brief Wsdl container struct
 *	Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_extensible_attribute
{
	axis2_wsdl_extensible_attribute_ops_t *ops;
};

/**
 * Creates wsdl property struct
 * @return pointer to newly created wsdl property
 */
AXIS2_DECLARE(axis2_wsdl_extensible_attribute_t *) 
axis2_wsdl_extensible_attribute_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_EXTENSIBLE_ATTRIBUTE_FREE(extensible_attribute, env) \
		((extensible_attribute)->ops->free (extensible_attribute, env))

#define AXIS2_WSDL_EXTENSIBLE_ATTRIBUTE_GET_KEY(extensible_attribute, env) \
		(extensible_attribute->ops->get_key (extensible_attribute, env))

#define AXIS2_WSDL_EXTENSIBLE_ATTRIBUTE_SET_KEY(extensible_attribute, env, key) \
		((extensible_attribute)->ops->set_key (extensible_attribute, env, key))       

#define AXIS2_WSDL_EXTENSIBLE_ATTRIBUTE_GET_VALUE(extensible_attribute, env) \
		(extensible_attribute->ops->get_value (extensible_attribute, env))

#define AXIS2_WSDL_EXTENSIBLE_ATTRIBUTE_SET_VALUE(extensible_attribute, env, value) \
		((extensible_attribute)->ops->set_value (extensible_attribute, env, value))       


/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_EXTENSIBLE_ATTRIBUTE_H */
