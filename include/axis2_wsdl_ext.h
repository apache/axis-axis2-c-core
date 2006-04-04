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

#ifndef AXIS2_WSDL_EXT_H
#define AXIS2_WSDL_EXT_H

/**
 * @file axis2_wsdl_ext.h
 * @brief Axis2 Wsdl ext interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_wsdl_component.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_component;    
typedef struct axis2_wsdl_ext axis2_wsdl_ext_t;
typedef struct axis2_wsdl_ext_ops axis2_wsdl_ext_ops_t;

/** @defgroup axis2_wsdl_ext Wsdl Ext Soap Address
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Ext Soap Address ops struct
 * Encapsulator struct for ops of axis2_wsdl_ext
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_ext_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_ext_t *ext,
	        axis2_env_t **env);

};

/** 
 * @brief Wsdl container struct
 *	Property for params  
 * This Extensibility Element is extended to handle particularly the
 * SOAP Adress or the Endpoint URL.
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_ext
{
	axis2_wsdl_ext_ops_t *ops;
    axis2_char_t *namespc;
};

/**
 * Creates wsdl ext struct
 * @return pointer to newly created wsdl ext
 */
AXIS2_DECLARE(axis2_wsdl_ext_t *) 
axis2_wsdl_ext_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_EXT_FREE(ext, env) \
		((ext)->ops->free (ext, env))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_EXT_H */
