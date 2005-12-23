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

#ifndef AXIS2_WS_INFO_H
#define AXIS2_WS_INFO_H

/**
 * @file axis2_ws_info.h
 * @brief Axis2 Ws Info interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_conf.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_conf;
    
typedef struct axis2_ws_info axis2_ws_info_t;
typedef struct axis2_ws_info_ops axis2_ws_info_ops_t;

/** @defgroup axis2_ws_info Ws Info
 * @ingroup axis2_deployment
 * @{
 */

/** 
 * @brief Ws Info ops struct
 * Encapsulator struct for ops of axis2_ws_info
 */
AXIS2_DECLARE_DATA struct axis2_ws_info_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_ws_info_t *ws_info,
	        axis2_env_t **env);
    
    struct axis2_module_desc *(AXIS2_CALL *
    get_module) (axis2_ws_info_t *ws_info,
                                axis2_env_t **env,
                                axis2_qname_t *module_name); 
                               
 

};

/** 
 * @brief Ws Info struct 
 */  
AXIS2_DECLARE_DATA struct axis2_ws_info
{
	axis2_ws_info_ops_t *ops;
};

/**
 * Creates description builder struct
 * @param repos_path
 * @return pointer to newly created ws info
 */
AXIS2_DECLARE(axis2_ws_info_t *) 
axis2_ws_info_create(axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_WS_INFO_FREE(ws_info, env) \
		((ws_info->ops)->free (ws_info, env))  

#define AXIS2_WS_INFO_GET_MODULE(ws_info, env, module_name) \
		((ws_info->ops)->get_module (ws_info, env, module_name))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WS_INFO_H */
