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

#ifndef AXIS2_DESCRIPTION_OPERATION_H
#define AXIS2_DESCRIPTION_OPERATION_H

/**
  * @file axis2_description_operation.h
  * @brief axis2 DESCRIPTION CORE operation
  */

#include <axis2_core.h>
#include <axis2_description_param_include.h>
#include <axis2_description_service.h>
#include <axis2_engine_msg_receiver.h>

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
 * @defgroup axis2_description_operation DESCRIPTION Operation
 * @ingroup axis2_description 
 * @{
 */

/************************** Start of function macros **************************/

#define axis2_description_operation_free(operation_desc, env) \
		(axis2_description_operation_get_ops(operation_desc \
		, env)->free (operation_desc, env));

#define axis2_description_operation_add_param(operation_desc, env, param) \
		(axis2_description_operation_get_ops(operation_desc \
		, env)->add_param (operation_desc, env, param));

#define axis2_description_operation_get_param get_param(operation_desc, env) \
		(axis2_description_operation_get_ops(operation_desc \
		, env)->get_param (operation_desc, env));

#define axis2_description_operation_get_params get_params(operation_desc, env) \
		(axis2_description_operation_get_ops(operation_desc \
		, env)->get_params (operation_desc, env));

#define axis2_description_operation_is_param_locked(operation_desc, env \
		, param_name) (axis2_description_operation_get_ops(operation_desc \
		, env)->is_param_locked(operation_desc, env, param_name));

#define axis2_description_operation_set_parent(operation_desc, env, \
		service_desc) (axis2_description_operation_get_ops(operation_desc \
		, env)->set_parent (operation_desc, env, service_desc));

#define axis2_description_operation_get_parent(operation_desc, env) \
		(axis2_description_operation_get_ops(operation_desc \
		, env)->get_parent (operation_desc, env));

#define axis2_description_operation_get_name(operation_desc, env) \
		(axis2_description_operation_get_ops(operation_desc, env)->get_name \
		(operation_desc, env));

#define axis2_description_operation_set_msg_exchange_pattern(operation_desc \
		, env, msg_exchange_pattern) \
		(axis2_description_operation_get_ops(operation_desc \
		, env)->set_msg_exchange_pattern (operation_desc \
		, env, msg_exchange_pattern));

#define axis2_description_operation_get_msg_exchange_pattern(operation_desc, env) \
		(axis2_description_operation_get_ops(operation_desc \
		, env)->get_msg_exchange_pattern (operation_desc, env));

#define axis2_description_operation_set_msg_receiver(operation_desc \
		, env, msg_receiver) (axis2_description_operation_get_ops(operation_desc \
		, env)->set_msg_receiver (operation_desc, env, msg_receiver));

#define axis2_description_operation_get_msg_receiver(operation_desc, env) \
		(axis2_description_operation_get_ops(operation_desc \
		, env)->get_msg_receiver (operation_desc, env));

/************************** End of function macros ****************************/

/************************** Start of function pointers ************************/

typedef axis2_status_t (*axis2_description_operation_free_t)
        (axis2_description_operation_t *operation_desc, axis2_env_t *env);

typedef axis2_status_t (*axis2_description_operation_add_param_t)
        (axis2_description_operation_t *operation_desc, axis2_env_t *env
        , axis2_description_param_t *param);

typedef axis2_description_param_t *(*axis2_description_operation_get_param_t) 
		(axis2_description_operation_t *operation_desc, axis2_env_t *env
        , const axis2_char_t *name);

typedef axis2_hash_t *(*axis2_description_operation_get_params_t)
        (axis2_description_operation_t *operation_desc, axis2_env_t *env);
		 
typedef axis2_bool_t (*axis2_description_operation_is_param_locked_t)
		(axis2_description_operation_t *operation_desc, axis2_env_t *env
		, const axis2_char_t *param_name);
		 

typedef axis2_status_t (*axis2_description_operation_set_parent_t)
        (axis2_description_operation_t *operation_desc, axis2_env_t *env
         , axis2_description_service_t *service_desc);

typedef axis2_description_service_t *(*axis2_description_operation_get_parent_t) 
		(axis2_description_operation_t *operation_desc, axis2_env_t *env);

typedef axis2_qname_t *(*axis2_description_operation_get_name_t)
        (axis2_description_operation_t *operation_desc, axis2_env_t *env);

typedef axis2_status_t (*axis2_description_operation_set_msg_exchange_pattern_t)
        (axis2_description_operation_t *operation_desc, axis2_env_t *env
         , axis2_char_t *pattern);

typedef axis2_char_t *(*axis2_description_operation_get_msg_exchange_pattern_t)
        (axis2_description_operation_t *operation_desc, axis2_env_t *env);

typedef axis2_status_t (*axis2_description_operation_set_msg_receiver_t)
        (axis2_description_operation_t *operation_desc, axis2_env_t *env,
         axis2_engine_msg_receiver_t *msg_receiver);

typedef axis2_engine_msg_receiver_t *(*axis2_description_operation_get_msg_receiver_t)
        (axis2_description_operation_t *operation_desc, axis2_env_t *env);

/**************************** End of function pointers ************************/

struct axis2_description_operation_ops_s
{
	axis2_description_operation_free_t free;

	axis2_description_operation_add_param_t add_param;

	axis2_description_operation_get_param_t get_param;

	axis2_description_operation_get_params_t get_params;

	axis2_description_operation_is_param_locked_t is_param_locked;
	
	axis2_description_operation_set_parent_t set_parent;

	axis2_description_operation_get_parent_t get_parent;

	axis2_description_operation_get_name_t get_name;

	axis2_description_operation_set_msg_exchange_pattern_t
		set_msg_exchange_pattern;

	axis2_description_operation_get_msg_exchange_pattern_t
		get_msg_exchange_pattern;

	axis2_description_operation_set_msg_receiver_t set_msg_receiver;

	axis2_description_operation_get_msg_receiver_t get_msg_receiver;
};

axis2_description_operation_ops_t *axis2_description_operation_get_ops
		(axis2_description_operation_t *operation_desc, axis2_env_t *env);

axis2_description_operation_t *axis2_description_operation_create 
		(axis2_env_t *env);

axis2_description_operation_t *axis2_description_operation_create_with_name 
		(axis2_env_t *env, axis2_qname_t *name);

/** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_DESCRIPTION_OPERATION_H */
