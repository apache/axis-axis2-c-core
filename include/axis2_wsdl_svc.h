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

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>
#include <axis2_qname.h>

#include <axis2_wsdl_component.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_wsdl_svc_ops_s axis2_wsdl_svc_ops_t;
typedef struct axis2_wsdl_svc_s axis2_wsdl_svc_t;
	
/** @defgroup axis2_wsdl_svc Wsdl Service
  * @ingroup axis2_wsdl
  * @{
  */

/** 
 * @brief Wsdl Service operations struct
 * Encapsulator struct for operations of axis2_wsdl_service
 */
struct axis2_wsdl_svc_ops_s
{
    /** Deallocate memory
     * @return status code
     */
	axis2_status_t (AXIS2_CALL *free) (axis2_wsdl_svc_t *wsdl_svc, 
                                        axis2_env_t **env);

    /**
     * Get wsdl service name
     * @return wsdl service name
     */
    axis2_qname_t * (AXIS2_CALL *get_name)(axis2_wsdl_svc_t *wsdl_svc,
                                            axis2_env_t **env);

    /**
     * Set wsdl serivce name
     * @param set wsdl service name
     */
    axis2_status_t (AXIS2_CALL *set_name) (axis2_wsdl_svc_t *wsdl_svc,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname);
};

/**
 * @brief Wsdl Service struct
 * Axis2 Wsdl Service
 */
struct axis2_wsdl_svc_s
{
	axis2_wsdl_svc_ops_t *ops;
    
};

/** create Wsdl Service struct
 * @return pointer to newly created wsdl service
 */
AXIS2_DECLARE(axis2_wsdl_svc_t *) 
axis2_wsdl_svc_create (axis2_env_t **env);

/**************************** Start of function macros ************************/

#define AXIS2_WSDL_SERVICE_FREE(wsdl_svc, env) ((wsdl_svc->ops)->free (wsdl_svc, \
		env))

#define AXIS2_WSDL_SERVICE_GET_NAME(wsdl_svc, env) \
        ((wsdl_svc->ops)->get_name (wsdl_svc, env))

#define AXIS2_WSDL_SERVICE_SET_NAME(wsdl_svc, env, qname) \
        ((wsdl_svc->ops)->set_name (wsdl_svc, env, qname))
	
		
/**************************** End of function macros **************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WSDL_SVC_H  */
