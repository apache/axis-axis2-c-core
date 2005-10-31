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

#ifndef AXIS2_WSDL_SERVICE_H
#define AXIS2_WSDL_SERVICE_H

/**
 * @file axis2_wsdl_service.h
 * @brief axis2 WSDL  service
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>

#include <axis2_wsdl_component.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @cond */
struct axis2_wsdl_service_ops_s;
struct axis2_wsdl_service_s;
/** @endcond */
typedef struct axis2_wsdl_service_ops_s axis2_wsdl_service_ops_t;
typedef struct axis2_wsdl_service_s axis2_wsdl_service_t;
	
/** @defgroup axis2_wsdl WSDL (Axis2 wsdl)
  * @ingroup axis2
  * @{
  */

/** @} */

/**
 * @defgroup axis2_wsdl_service WSDL Service
 * @ingroup axis2_wsdl 
 * @{
 */

/**************************** Start of function macros ************************/

#define AXIS2_WSDL_SERVICE_FREE(env, wsdl_srv) ((wsdl_srv->ops)->free (env, \
		wsdl_srv))


	
		
/**************************** End of function macros **************************/
/**************************** Function pointers *******************************/

/** Deallocate memory
  * @return status code
  */
typedef axis2_status_t (*axis2_wsdl_service_free_t) (axis2_environment_t *env
		, axis2_wsdl_service_t *wsdl_srv);


		
/*************************** End of function pointers *************************/

struct axis2_wsdl_service_ops_s
{
	axis2_wsdl_service_free_t free;

};

struct axis2_wsdl_service_s
{
	axis2_wsdl_service_ops_t *ops;
};

axis2_status_t axis2_wsdl_service_create
		(axis2_environment_t *env, axis2_wsdl_service_t **wsdl_srv);

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WSDL_SERVICE_H  */
