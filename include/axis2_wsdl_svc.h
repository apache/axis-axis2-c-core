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

#ifndef AXIS2_WSDL_SVC_H
#define AXIS2_WSDL_SVC_H

/**
 * @file axis2_wsdl_svc.h
 * @brief axis2 wsdl service interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_wsdl_interface.h>
#include <axis2_wsdl_component.h>
#include <axis2_wsdl_endpoint.h>

#ifdef __cplusplus
extern "C"
{
#endif
	
/** @defgroup axis2_wsdl_svc Wsdl Service
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_wsdl_endpoint; 
struct axis2_wsdl_interface; 
struct axis2_wsdl_component;    
typedef struct axis2_wsdl_svc_ops axis2_wsdl_svc_ops_t;
typedef struct axis2_wsdl_svc axis2_wsdl_svc_t;

#define AXIS2_STYLE_RPC  "rpc"
#define AXIS2_STYLE_DOC  "doc"
#define AXIS2_STYLE_MSG  "msg"
/** 
 * @brief Wsdl Service ops struct
 * Encapsulator struct for ops of axis2_wsdl_service
 */
struct axis2_wsdl_svc_ops
{
    /** Deallocate memory
     * @return status code
     */
	axis2_status_t (AXIS2_CALL *
	free) (axis2_wsdl_svc_t *wsdl_svc, 
           axis2_env_t **env);

    /**
     * Get wsdl service name
     * @return wsdl service name
     */
    axis2_qname_t * (AXIS2_CALL *
	get_qname)(axis2_wsdl_svc_t *wsdl_svc,
               axis2_env_t **env);

    /**
     * Set wsdl serivce name
     * @param set wsdl service name
     */
    axis2_status_t (AXIS2_CALL *
	set_qname) (axis2_wsdl_svc_t *wsdl_svc,
                axis2_env_t **env,
                axis2_qname_t *qname);
    
    /**
     * Method getEndpoints
     *
     * @return
     */
    axis2_hash_t *(AXIS2_CALL *
    get_endpoints) (axis2_wsdl_svc_t *wsdl_svc,
                    axis2_env_t **env);
    
    /**
     * Method setEndpoints
     *
     * @param endpoints
     */
    axis2_status_t (AXIS2_CALL *
    set_endpoints) (axis2_wsdl_svc_t *wsdl_svc,
                    axis2_env_t **env,
                    axis2_hash_t *endpoints);
    
    /**
     * Will add a WSDLEndpoint object to the WOM keyed with qname;
     *
     * @param endpoint
     */
    axis2_status_t (AXIS2_CALL *
    set_endpoint) (axis2_wsdl_svc_t *wsdl_svc,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_endpoint *endpoint);
    
    /**
     * Endpoint will be retrived by its qname.
     *
     * @param qName qname of the Service
     * @return <code>WSDL Endpoint</code> Object.
     */
    struct axis2_wsdl_endpoint * (AXIS2_CALL *
    get_endpoint) (axis2_wsdl_svc_t *wsdl_svc,
                                    axis2_env_t **env,
                                    axis2_qname_t *qname);

    /**
     * If the Name of the <code>WSDLService</code> is not set a
     * Error code will be set
     *
     * @return Target Namespace as a <code>axis2_char_t</code>
     */
    axis2_char_t *(AXIS2_CALL *
    get_namespace) (axis2_wsdl_svc_t *wsdl_svc,
                                    axis2_env_t **env);

    /**
     * Method get_svc_interface
     *
     * @return
     */
    struct axis2_wsdl_interface * (AXIS2_CALL *
    get_svc_interface) (axis2_wsdl_svc_t *wsdl_svc,
                                        axis2_env_t **env);
    
    /**
     * Method setServiceInterface
     *
     * @param svc_interface
     */
    axis2_status_t (AXIS2_CALL *
    set_svc_interface) (axis2_wsdl_svc_t *wsdl_svc,
                                        axis2_env_t **env,
                                        struct axis2_wsdl_interface *svc_interface);

};

/**
 * @brief Wsdl Service struct
 * Axis2 Wsdl Service
 */
struct axis2_wsdl_svc
{
	axis2_wsdl_svc_ops_t *ops;
    struct axis2_wsdl_component *wsdl_component;
};

/** create Wsdl Service struct
 * @return pointer to newly created wsdl service
 */
AXIS2_DECLARE(axis2_wsdl_svc_t *) 
axis2_wsdl_svc_create (axis2_env_t **env);

/**************************** Start of function macros ************************/

#define AXIS2_WSDL_SVC_FREE(wsdl_svc, env) ((wsdl_svc)->ops->free (wsdl_svc, \
		env))

#define AXIS2_WSDL_SVC_GET_QNAME(wsdl_svc, env) \
        ((wsdl_svc)->ops->get_qname (wsdl_svc, env))

#define AXIS2_WSDL_SVC_SET_QNAME(wsdl_svc, env, qname) \
        ((wsdl_svc)->ops->set_qname (wsdl_svc, env, qname))

#define AXIS2_WSDL_SVC_GET_ENDPOINTS(wsdl_svc, env) \
        ((wsdl_svc)->ops->get_endpoints(wsdl_svc, env))

#define AXIS2_WSDL_SVC_SET_ENDPOINTS(wsdl_svc, env, endpoints) \
        ((wsdl_svc)->ops->set_endpoints(wsdl_svc, env, endpoints))

#define AXIS2_WSDL_SVC_SET_ENDPOINT(wsdl_svc, env, endpoint) \
        ((wsdl_svc)->ops->set_endpoint(wsdl_svc, env, endpoint))

#define AXIS2_WSDL_SVC_GET_ENDPOINT(wsdl_svc, env, qname) \
        ((wsdl_svc)->ops->get_endpoint(wsdl_svc, env, qname))

#define AXIS2_WSDL_SVC_GET_NAMESPACE(wsdl_svc, env) \
        ((wsdl_svc)->ops->get_namespace(wsdl_svc, env))
        
#define AXIS2_WSDL_SVC_GET_SVC_INTERFACE(wsdl_svc, env) \
        ((wsdl_svc)->ops->get_svc_interface(wsdl_svc, env))        

#define AXIS2_WSDL_SVC_SET_SVC_INTERFACE(wsdl_svc, env, svc_interface) \
        ((wsdl_svc)->ops->set_svc_interface(wsdl_svc, env, svc_interface))
        
/**************************** End of function macros **************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WSDL_SVC_H  */
