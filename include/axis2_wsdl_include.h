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

#ifndef AXIS2_WSDL_INCLUDE_H
#define AXIS2_WSDL_INCLUDE_H

/**
 * @file axis2_wsdl_include.h
 * @brief Axis2 Wsdl include interface
 */

#include <axis2_const.h>
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
typedef struct axis2_wsdl_include axis2_wsdl_include_t;
typedef struct axis2_wsdl_include_ops axis2_wsdl_include_ops_t;

/** @defgroup axis2_wsdl_include Wsdl Include
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Include ops struct
 * Encapsulator struct for ops of axis2_wsdl_include
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_include_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_include_t *wsdl_include,
	        const axis2_env_t *env);
                               
    /**
     * Method get_namespace
     *
     * @return namespace
     */
    axis2_char_t *(AXIS2_CALL *
    get_namespace) (axis2_wsdl_include_t *wsdl_include,
                                    const axis2_env_t *env);
                                    
    /**
     * Method set_namespace
     *
     * @param namespace
     */
    axis2_status_t (AXIS2_CALL *
    set_namespace) (axis2_wsdl_include_t *wsdl_include,
                                    const axis2_env_t *env,
                                    axis2_char_t *namespace);
    
    
    /**
     * Method isRequired
     *
     * @return
     */
    axis2_char_t *(AXIS2_CALL *
    get_location) (axis2_wsdl_include_t *wsdl_include,
                                    const axis2_env_t *env);
                                    
    /**
     * Method set_location
     *
     * @param location
     */
    axis2_status_t (AXIS2_CALL *
    set_location) (axis2_wsdl_include_t *wsdl_include,
                                    const axis2_env_t *env,
                                    axis2_char_t *location);  


};

/** 
 * @brief Wsdl container struct
 *	Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_include
{
	axis2_wsdl_include_ops_t *ops;
    struct axis2_wsdl_component *wsdl_component;
};

/**
 * Creates wsdl include struct
 * @return pointer to newly created wsdl include
 */
AXIS2_DECLARE(axis2_wsdl_include_t *) axis2_wsdl_include_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_INCLUDE_FREE(wsdl_include, env) \
		((wsdl_include)->ops->free (wsdl_include, env))

#define AXIS2_WSDL_INCLUDE_GET_NAMESPACE(wsdl_include, env) \
		((wsdl_include)->ops->get_namespace (wsdl_include, env))

#define AXIS2_WSDL_INCLUDE_SET_NAMESPACE(wsdl_include, env, namespace) \
		((wsdl_include)->ops->set_namespace (wsdl_include, env, namespace))
        
#define AXIS2_WSDL_INCLUDE_GET_LOCATION(wsdl_include, env) \
		((wsdl_include)->ops->is_location (wsdl_include, env))

#define AXIS2_WSDL_INCLUDE_SET_LOCATION(wsdl_include, env, location) \
		((wsdl_include)->ops->set_location (wsdl_include, env, location))        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_INCLUDE_H */
