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

#ifndef AXIS2_DI_CLIENT_H
#define AXIS2_DI_CLIENT_H


/**
  * @file axis2_di_client.h
  * @brief axis2 Dynamic Invocation Client interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_error.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_di_client Dynamic Invocation Client 
 * @ingroup axis2_core_clientapi
 * @{
 */
    
typedef struct axis2_di_client_ops axis2_di_client_ops_t;
typedef struct axis2_di_client axis2_di_client_t; 

struct axis2_om_node;
    
/** 
 * @brief Dynamic Invocation Client ops struct
 * Encapsulator struct for ops of axis2_di_client
 */  
struct axis2_di_client_ops
{
    axis2_status_t (AXIS2_CALL *
	free)(struct axis2_di_client *di_client, 
          axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    invoke) (axis2_di_client_t *di_client,
             axis2_env_t **env,
             axis2_om_node_t *node);
    
    axis2_status_t (AXIS2_CALL *
    init) (axis2_di_client_t *di_client,
           axis2_env_t **env,
		   axis2_char_t *wsdl_file_name);
};

/** 
 * @brief Dynamic Invocation Client struct
  *	Axis2 Dynamic Invocation Client
 */
struct axis2_di_client
{
    axis2_di_client_ops_t *ops;    
};

AXIS2_DECLARE(axis2_di_client_t*) 
axis2_di_client_create(axis2_env_t **env);

/************************** Start of function macros **************************/

#define AXIS2_DI_CLIENT_FREE(di_client, env) \
		((di_client)->ops->free (di_client, env))
		
#define AXIS2_DI_CLIENT_INVOKE(di_client, env, node) \
		((di_client)->ops->invoke (di_client, env, node))

#define AXIS2_DI_CLIENT_INIT(di_client, env, wsdl_file_name) \
		((di_client)->ops->init (di_client, env, wsdl_file_name))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_DI_CLIENT_H */

