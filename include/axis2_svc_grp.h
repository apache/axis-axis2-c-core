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

#ifndef AXIS2_DESCRIPTION_SERVICEGROUP_H
#define AXIS2_DESCRIPTION_SERVICEGROUP_H

/**
 * @file axis2_description_servicegroup.h
 * @brief axis2 DESCRIPTION CORE servicegroup
 */

#include <axis2_core.h>
#include <axis2_param_container.h>
#include <axis2_svc.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_description DESCRIPTION (Axis2 Information model)
  * @ingroup axis2
  * @{
  */

/** @} */

/**
 * @defgroup axis2_description_servicegroup DESCRIPTION Servicegroup
 * @ingroup axis2_description 
 * @{
 */

/**************************** Start of function macros ************************/

#define axis2_description_servicegroup_free(srvgrp_desc, env) \
		(axis2_description_servicegroup_get_ops(srvgrp_desc, env)->free (srvgrp_desc \
		, env));
	
#define axis2_description_servicegroup_set_name(srvgrp_desc, env \
		, servicegroup_name) (axis2_description_servicegroup_get_ops(srvgrp_desc \
		, env)->set_name (srvgrp_desc, env, servicegroup_name));
		
#define axis2_description_servicegroup_get_name(srvgrp_desc, env) \
		(axis2_description_servicegroup_get_ops(srvgrp_desc \
		, env)->get_name(srvgrp_desc, env));

#define axis2_description_servicegroup_add_service(srvgrp_desc, env \
		, service_desc) (axis2_description_servicegroup_get_ops(srvgrp_desc \
		, env)->add_service (srvgrp_desc, env, service_desc));

#define axis2_description_servicegroup_get_service(srvgrp_desc, env \
		, service_desc) (axis2_description_servicegroup_get_ops(srvgrp_desc \
		, env)->get_service(srvgrp_desc, env, service_desc));
		
#define axis2_description_servicegroup_remove_service(srvgrp_desc, env, srvc_name) \
		(axis2_description_servicegroup_get_ops(srvgrp_desc \
		, env)->remove_service(srvgrp_desc, env, srvc_name));		

#define axis2_description_servicegroup_add_param(srvgrp_desc, env, param) \
		(axis2_description_servicegroup_get_ops(srvgrp_desc \
		, env)->add_param(srvgrp_desc, env, param));
		
		

#define axis2_description_servicegroup_get_param(srvgrp_desc, env, name) \
		(axis2_description_servicegroup_get_ops(srvgrp_desc \
		, env)->get_param(srvgrp_desc, env, name));
		
#define axis2_description_servicegroup_get_params(srvgrp_desc, env) \
		(axis2_description_servicegroup_get_ops(srvgrp_desc \
		, env)->get_params(srvgrp_desc, env));
		
		
#define axis2_description_servicegroup_is_param_locked(srvgrp_desc, env \
		, param_name)(axis2_description_servicegroup_get_ops(srvgrp_desc \
		, env)->is_param_locked(srvgrp_desc, env, param_name));
		
/**************************** End of function macros **************************/
/**************************** Function pointers *******************************/

/** Deallocate memory
  * @return status code
  */
typedef axis2_status_t (*axis2_description_servicegroup_free_t)
		(axis2_svc_grp_t *srvgrp_desc, axis2_env_t *env);

/** Set name of the service group_member
  * @param servicegroup_name
  * @return status_code
  */
typedef axis2_status_t (*axis2_description_servicegroup_set_name_t)
		(axis2_svc_grp_t *srvgrp_desc, axis2_env_t *env
		, axis2_char_t *servicegroup_name);
		
/** get the service group name
  * @return service group name
  */
typedef axis2_char_t *(*axis2_description_servicegroup_get_name_t)
		(axis2_svc_grp_t *srvgrp_desc, axis2_env_t *env);

/** Add a service to the serivce group
  * @param service to be added
  * @return status code
  */
typedef axis2_status_t (*axis2_description_servicegroup_add_service_t)
		(axis2_svc_grp_t *srvgrp_desc, axis2_env_t *env
	 	, axis2_svc_t *service_desc);

/** Get the service from service group_member
  * @param service_name
  * @return service description
  */
typedef axis2_svc_t 
		*(*axis2_description_servicegroup_get_service_t)
		(axis2_svc_grp_t *srvgrp_desc, axis2_env_t *env
		, const axis2_qname_t* service_name);

typedef axis2_status_t (*axis2_description_servicegroup_remove_service_t)
		(axis2_svc_grp_t *srvgrp_desc, axis2_env_t *env
		, const axis2_qname_t *service_name);
		
typedef axis2_status_t (*axis2_description_servicegroup_add_param_t)
		(axis2_svc_grp_t *srvgrp_desc, axis2_env_t *env
		, axis2_param_t *param);
		

typedef axis2_param_t *(*axis2_description_servicegroup_get_param_t)(
		axis2_svc_grp_t *srvgrp_desc, axis2_env_t *env
		, const axis2_char_t *name);
		
typedef axis2_hash_t *(*axis2_description_servicegroup_get_params_t)
		(axis2_svc_grp_t *srvgrp_desc, axis2_env_t *env);
		
typedef axis2_bool_t (*axis2_description_servicegroup_is_param_locked_t)
		(axis2_svc_grp_t *srvgrp_desc, axis2_env_t *env
		, const axis2_char_t *param_name);
		
/*************************** End of function pointers *************************/

struct axis2_description_servicegroup_ops_s
{
	axis2_description_servicegroup_free_t free;
	axis2_description_servicegroup_set_name_t set_name;
	axis2_description_servicegroup_get_name_t get_name;
	axis2_description_servicegroup_add_service_t add_service;
	axis2_description_servicegroup_get_service_t get_service;
	axis2_description_servicegroup_remove_service_t remove_service;
	axis2_description_servicegroup_add_param_t add_param;
	axis2_description_servicegroup_get_param_t get_param;
	axis2_description_servicegroup_get_params_t get_params;
	axis2_description_servicegroup_is_param_locked_t is_param_locked;
};

axis2_description_servicegroup_ops_t *axis2_description_servicegroup_get_ops 
		(axis2_svc_grp_t *srvgrp_desc, axis2_env_t *env);

axis2_svc_grp_t *axis2_description_servicegroup_create
		(axis2_env_t *env);

/** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_DESCRIPTION_SERVICEGROUP_H  */
