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

#ifndef AXIS2_WSDL_ENDPOINT_H
#define AXIS2_WSDL_ENDPOINT_H

/**
 * @file axis2_wsdl_endpoint.h
 * @brief Axis2 Wsdl endpoint interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_wsdl_component.h>
#include <axis2_qname.h>
#include <axis2_wsdl_binding.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_binding;
typedef struct axis2_wsdl_endpoint axis2_wsdl_endpoint_t;
typedef struct axis2_wsdl_endpoint_ops axis2_wsdl_endpoint_ops_t;

/** @defgroup axis2_wsdl_endpoint Wsdl Feature
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl End point operations struct
 * Encapsulator struct for operations of axis2_wsdl_endpoint
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_endpoint_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(axis2_wsdl_endpoint_t *wsdl_endpoint,
										axis2_env_t **env);
                               
    /**
     * Method getName
     *
     * @return
     */
    axis2_qname_t *(AXIS2_CALL *
    get_name) (axis2_wsdl_endpoint_t *wsdl_endpoint,
                                    axis2_env_t **env);
                                    
    /**
     * Method setName
     *
     * @param qname
     */
    axis2_status_t (AXIS2_CALL *
    set_name) (axis2_wsdl_endpoint_t *wsdl_endpoint,
                                    axis2_env_t **env,
                                    axis2_qname_t *qname);
    
    
    /**
     * Method get_binding
     *
     * @return
     */
    struct axis2_wsdl_binding *(AXIS2_CALL *
    get_binding) (axis2_wsdl_endpoint_t *wsdl_endpoint,
                                    axis2_env_t **env);
                                    
    /**
     * Method set_binding
     *
     * @param wsdl_binding
     */
    axis2_status_t (AXIS2_CALL *
    set_binding) (axis2_wsdl_endpoint_t *wsdl_endpoint,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_binding *wsdl_binding);  


};

/** 
 * @brief Wsdl endpoint struct
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_endpoint
{
	axis2_wsdl_endpoint_ops_t *ops;
    axis2_wsdl_component_t *wsdl_component;
};

/**
 * Creates wsdl endpoint struct
 * @return pointer to newly created wsdl endpoint
 */
AXIS2_DECLARE(axis2_wsdl_endpoint_t *) 
axis2_wsdl_endpoint_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_ENDPOINT_FREE(wsdl_endpoint, env) \
		((wsdl_endpoint->ops)->free (wsdl_endpoint, env))

#define AXIS2_WSDL_ENDPOINT_GET_NAME(wsdl_endpoint, env) \
		((wsdl_endpoint->ops)->get_name (wsdl_endpoint, env))

#define AXIS2_WSDL_ENDPOINT_SET_NAME(wsdl_endpoint, env, qname) \
		((wsdl_endpoint->ops)->set_name (wsdl_endpoint, env, qname))
        
#define AXIS2_WSDL_ENDPOINT_GET_BINDING(wsdl_endpoint, env) \
		((wsdl_endpoint->ops)->get_binding (wsdl_endpoint, env))

#define AXIS2_WSDL_ENDPOINT_SET_BINDING(wsdl_endpoint, env, wsdl_binding) \
		((wsdl_endpoint->ops)->set_binding (wsdl_endpoint, env, wsdl_binding))        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_ENDPOINT_H */
