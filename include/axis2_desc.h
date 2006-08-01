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

#ifndef AXIS2_DESC_H
#define AXIS2_DESC_H

/** @defgroup axis2_description description
 * @ingroup axis2_desc
 * Description.
 * @{
 */

/**
  * @file axis2_desc.h
  * @brief axis2 desc interface
  */

#include <axis2_param_container.h>
#include <axis2_hash.h>
#include <axis2_description.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name of struct axis2_desc_ops */
    typedef struct axis2_desc_ops axis2_desc_ops_t;
    /** Type name of struct axis2_desc */
    typedef struct axis2_desc axis2_desc_t;

    /**
     * message ops struct
     * Encapsulator struct for ops of axis2_desc
     */
    AXIS2_DECLARE_DATA struct axis2_desc_ops
    {
	/** 
	 * @param desc pointer to desc
	 * @param env pointer to environment struct
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_desc_t *desc,
                    const axis2_env_t *env);

	/**
	 * @param desc pointer to desc
	 * @param env pointer to environment struct
	 * @param param pointer to param
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_desc_t *desc,
                    const axis2_env_t *env,
                    axis2_param_t *param);

	/**
	 * @param desc pointer to desc
	 * @param env pointer to environment struct
	 * @param param_name pointer to param name
	 */
        axis2_param_t *(AXIS2_CALL *
                get_param) (
                    const axis2_desc_t *desc,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

	/**
	 * @param desc pointer to desc
	 * @param env pointer to environment struct
	 */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_params)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env);

	/**
	 * @param desc pointer to desc
	 * @param env pointer to environment struct
	 * @param param_name pointer to param name
	 */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

        /*axis2_status_t (AXIS2_CALL *
        set_policy_container)(
            axis2_desc_t *desc, 
            const axis2_env_t *env,
            axis2_policy_container_t *policy_container);

        axis2_policy_container_t *(AXIS2_CALL *
        get_policy_container)(
            const axis2_desc_t *desc, 
            const axis2_env_t *env);*/

	/**
	 * @param desc pointer to desc
	 * @param env pointer to environment struct
	 * @param key pointer to key
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                add_child)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env,
                    const axis2_char_t *key,
                    const void* child);

	/**
	 * @param desc pointer to desc
	 * @param env pointer to environment struct
	 */
        axis2_hash_t *(AXIS2_CALL *
                get_all_children)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env);

	/**
	 * @param desc pointer to desc
	 * @param env pointer to environment struct
	 * @param key pointer to key
	 */
        void *(AXIS2_CALL *
                get_child)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env,
                    const axis2_char_t *key);

	/**
	 * @param desc pointer to desc
	 * @param env pointer to environment struct
	 * @param key pointer to key
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                remove_child)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env,
                    const axis2_char_t *key);
    };

    /**
     * message struct
     */
    AXIS2_DECLARE_DATA struct axis2_desc
    {
        /** operations of message */
        axis2_desc_ops_t *ops;

    };

    /**
     * Creates desc struct
     * @param env pointer to environment struct
     * @return pointer to newly created desc
     */
    AXIS2_EXTERN axis2_desc_t *AXIS2_CALL
            axis2_desc_create (
                const axis2_env_t *env);

/************************** Start of function macros **************************/

/** Frees the desc.
    @sa axis2_desc_ops#free */
#define AXIS2_DESC_FREE(desc, env) \
        ((desc)->ops->free (desc, env))

/** Adds the param.
    @sa axis2_desc_ops#add_param*/
#define AXIS2_DESC_ADD_PARAM(desc, env, param) \
      ((desc)->ops->add_param (desc, env, param))

/** Gets the param.
    @sa axis2_desc_ops#get_param */
#define AXIS2_DESC_GET_PARAM(desc, env, key) \
      ((desc)->ops->get_param (desc, env, key))

/** Gets the all params.
    @sa axis2_desc_ops#get_all_params */
#define AXIS2_DESC_GET_ALL_PARAMS(desc, env) \
      ((desc)->ops->get_all_params (desc, env))

/** Is param locked.
    @sa axis2_desc_ops#is_param_locked */
#define AXIS2_DESC_IS_PARAM_LOCKED(desc, env, param_name) \
        ((desc)->ops->is_param_locked(desc, env, param_name))

/** Set policy include.
    @sa axis2_desc_ops#set_policy_include */
#define AXIS2_DESC_SET_POLICY_INCLUDE(desc, env, policy_container) \
    ((desc)->ops->set_policy_include(desc, env, policy_container))

/** Get policy include.
    @sa axis2_desc_ops#get_policy_include */
#define AXIS2_DESC_GET_POLICY_INCLUDE(desc, env) \
    ((desc)->ops->get_policy_container(desc, env))

/** Adds the child.
    @sa axis2_desc_ops#add_child */
#define AXIS2_DESC_ADD_CHILD(desc, env, key, child) \
    ((desc)->ops->add_child(desc, env, key, child))

/** Gets the all children.
    @sa axis2_desc_ops#get_all_children */
#define AXIS2_DESC_GET_ALL_CHILDREN(desc, env) \
    ((desc)->ops->get_all_children(desc, env))

/** Gets the child.
    @sa axis2_desc_ops#get_child */
#define AXIS2_DESC_GET_CHILD(desc, env, key) \
    ((desc)->ops->get_child(desc, env, key))

/** Removes the child.
    @sa axis2_desc_ops#remove_child */
#define AXIS2_DESC_REMOVE_CHILD(desc, env, key) \
    ((desc)->ops->remove_child(desc, env, key))

/** @} */
#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_DESC_H */
