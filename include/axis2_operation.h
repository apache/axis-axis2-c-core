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

#ifndef AXIS2_OPERATION_H
#define AXIS2_OPERATION_H

/**
  * @file axis2_operation.h
  * @brief axis2 operation interface
  */

#include <axis2_core.h>
#include <axis2_param_container.h>
#include <axis2_svc.h>
#include <axis2_msg_recv.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_operation Operation Description
  * @ingroup axis2_core_description
  * @{
  */

struct axis2_svc_s;
struct axis2_msg_recv_s;
typedef struct axis2_operation_ops_s axis2_operation_ops_t;    
typedef struct axis2_operation_s axis2_operation_t;    
    
/** 
 * @brief Operation operations struct
 * Encapsulator struct for operations of axis2_operation
 */    
AXIS2_DECLARE_DATA struct axis2_operation_ops_s
{
    /** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free) (axis2_operation_t *operation, 
                                        axis2_env_t **env);

    /**
     * Method addParameter
     *
     * @param param Parameter that will be added
     */
	axis2_status_t (AXIS2_CALL *add_param)(axis2_operation_t *operation, 
                                            axis2_env_t **env, 
                                            axis2_param_t *param);
    
    /**
     * Method getParameter
     *
     * @param name Name of the parameter
     */
	axis2_param_t *(AXIS2_CALL *get_param)(axis2_operation_t *operation, 
                                            axis2_env_t **env,
                                            const axis2_char_t *name);


	axis2_hash_t *(AXIS2_CALL *get_params) (axis2_operation_t *operation, 
                                                axis2_env_t **env);

    /**
     * To check whether a given paramter is locked
     */
	axis2_bool_t (AXIS2_CALL *is_param_locked) (axis2_operation_t *operation, 
                                                axis2_env_t **env,
		                                        const axis2_char_t *param_name);
	
	axis2_status_t (AXIS2_CALL *set_parent) (axis2_operation_t *operation, 
                                     axis2_env_t **env,
                                     struct axis2_svc_s *service_desc);

	struct axis2_svc_s *(AXIS2_CALL *get_parent) (axis2_operation_t *operation, 
                                            axis2_env_t **env);

	axis2_qname_t *(AXIS2_CALL *get_name) (axis2_operation_t *operation, 
                                            axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *set_msg_exchange_pattern) 
                                                (axis2_operation_t *operation, 
                                                axis2_env_t **env,
                                                axis2_char_t *pattern);

	axis2_char_t *(AXIS2_CALL *get_msg_exchange_pattern)
                                                (axis2_operation_t *operation, 
                                                axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *set_msg_recv) (axis2_operation_t *operation, 
                                        axis2_env_t **env,
                                        struct axis2_msg_recv_s *msg_recv);

	struct axis2_msg_recv_s *(AXIS2_CALL *get_msg_recv) (axis2_operation_t *operation, 
                                                    axis2_env_t **env);
};

/** 
 * @brief Operaton struct
 *	Axis2 Operation   
 */  
AXIS2_DECLARE_DATA struct axis2_operation_s
{
	axis2_operation_ops_t *ops;
};

/** 
 * Creates operation struct
 * @return pointer to newly created operation
 */
AXIS2_DECLARE(axis2_operation_t *)
axis2_operation_create (axis2_env_t **env);

/** 
 * Creates operation struct with name
 * @param name operation name
 * @return pointer to newly created operation
 */
AXIS2_DECLARE(axis2_operation_t *)
axis2_operation_create_with_name (axis2_env_t **env, 
                                                axis2_qname_t *name);


/************************** Start of function macros **************************/

#define AXIS2_OPERATION_FREE(operation, env) \
        ((operation->ops)->free (operation, env));

#define AXIS2_OPERATION_ADD_PARAM(operation, env, param) \
		((operation->ops)->add_param (operation, env, param));

#define AXIS2_OPERATION_GET_PARAM(operation, env) \
		((operation->ops)->get_param (operation, env));

#define AXIS2_OPERATION_GET_PARAMS(operation, env) \
		((operation->ops)->get_params (operation, env));

#define AXIS2_OPERATION_IS_PARAM_LOCKED(operation, env, param_name) \
        ((operation->ops)->is_param_locked(operation, env, param_name));

#define AXIS2_OPERATION_SET_PARENT(operation, env, service_desc) \
        ((operation->ops)->set_parent (operation, env, service_desc));

#define AXIS2_OPERATION_GET_PARENT(operation, env) \
		((operation->ops)->get_parent (operation, env));

#define AXIS2_OPERATION_GET_NAME(operation, env) \
		((operation->ops)->get_name (operation, env));

#define AXIS2_OPERATION_SET_MSG_EXCHANGE_PATTERN(operation , env, \
        msg_exchange_pattern) \
		((operation->ops)->set_msg_exchange_pattern (operation, env, \
        msg_exchange_pattern));

#define AXIS2_OPERATION_GET_MSG_EXCHANGE_PATTERN(operation, env) \
		((operation->ops)->get_msg_exchange_pattern (operation, env));

#define AXIS2_OPERATION_SET_MSG_RECEIVER(operation, env, msg_recv) \
        ((operation->ops)->set_msg_recv (operation, env, msg_recv));

#define AXIS2_OPERATION_GET_MSG_RECEIVER(operation, env) \
		((operation->ops)->get_msg_recv (operation, env));

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_OPERATION_H */
