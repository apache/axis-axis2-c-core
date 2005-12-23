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

#ifndef AXIS2_WS_INFO_LIST_H
#define AXIS2_WS_INFO_LIST_H

/**
 * @file axis2_ws_info_list.h
 * @brief Axis2 Ws Info List interface
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
    
typedef struct axis2_ws_info_list axis2_ws_info_list_t;
typedef struct axis2_ws_info_list_ops axis2_ws_info_list_ops_t;

/** @defgroup axis2_ws_info_list Ws Info List
 * @ingroup axis2_deployment
 * @{
 */

/** 
 * @brief Ws Info List ops struct
 * Encapsulator struct for ops of axis2_ws_info_list
 */
AXIS2_DECLARE_DATA struct axis2_ws_info_list_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_ws_info_list_t *ws_info_list,
	        axis2_env_t **env);
    
   
                               
 

};

/** 
 * @brief Ws Info List struct 
 */  
AXIS2_DECLARE_DATA struct axis2_ws_info_list
{
	axis2_ws_info_list_ops_t *ops;
};

/**
 * Creates ws info list struct
 * @param repos_path
 * @return pointer to newly created ws info list
 */
AXIS2_DECLARE(axis2_ws_info_list_t *) 
axis2_ws_info_list_create_with_repos_name (
                                        axis2_env_t **env, 
                                        axis2_char_t *repos_path);


/*************************** Function macros **********************************/

#define AXIS2_WS_INFO_LIST_FREE(ws_info_list, env) \
		((ws_info_list->ops)->free (ws_info_list, env))  



/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WS_INFO_LIST_H */
