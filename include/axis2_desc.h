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

/** 
 * @defgroup axis2_description description
 * @ingroup axis2_desc
 * Base struct of description hierarchy. Encapsulates common data and functions
 * of the description hierarchy.
 * @{
 */

/**
  * @file axis2_desc.h
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
     * description ops struct.
     * Encapsulator struct for ops of axis2_desc.
     */
     struct axis2_desc_ops
    {
        /** 
         * Frees description struct.
         * @param desc pointer to description
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_desc_t *desc,
                    const axis2_env_t *env);

        /**
         * Adds given parameter to the list of parameters.
         * @param desc pointer to description
         * @param env pointer to environment struct
         * @param param pointer to parameter
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_desc_t *desc,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * Gets named parameter.
         * @param desc pointer to description
         * @param env pointer to environment struct
         * @param param_name parameter name string
         * @return pointer to named parameter, NULL if it does not exist
         */
        axis2_param_t *(AXIS2_CALL *
                get_param) (
                    const axis2_desc_t *desc,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

        /**
         * Gets all parameters stored in description.
         * @param desc pointer to description
         * @param env pointer to environment struct
         * @return pointer to array list containing the list of parameters
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_params)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env);

        /**
         * Checks if a named parameter is locked.
         * @param desc pointer to description
         * @param env pointer to environment struct
         * @param param_name parameter name string
         * @return AXIS2_TRUE if parameter is locked, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

        /**
         * Adds child to the description. The type of children is based on the
         * level of the description hierarchy. As an example, service has 
         * children of type operation, service group has children of type 
         * service
         * @param desc pointer to description
         * @param env pointer to environment struct
         * @param key key with which the child is to be added
         * @param child child to be added
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_child)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env,
                    const axis2_char_t *key,
                    const void *child);

        /**
         * Gets all children. 
         * @param desc pointer to description
         * @param env pointer to environment struct
         * @return pointer to hash map containing children
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_children)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env);

        /**
         * Gets child with given key.
         * @param desc pointer to description
         * @param env pointer to environment struct
         * @param key key with which the child is stored
         * @return pointer to child, returned as a void* value, need to cast to 
         * correct type
         */
        void *(AXIS2_CALL *
                get_child)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env,
                    const axis2_char_t *key);

        /**
         * Removes the name child.
         * @param desc pointer to description
         * @param env pointer to environment struct
         * @param key key that represents the child to be removed
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                remove_child)(
                    const axis2_desc_t *desc,
                    const axis2_env_t *env,
                    const axis2_char_t *key);
    };

    /**
     * description struct.
     */
     struct axis2_desc
    {
        /** operations of message */
        axis2_desc_ops_t *ops;

    };

    /**
     * Creates a description struct instance.    
     * @param env pointer to environment struct
     * @return pointer to newly created description
     */
    AXIS2_EXTERN axis2_desc_t *AXIS2_CALL
    axis2_desc_create (
        const axis2_env_t *env);

/** Frees the desc.
    @sa axis2_desc_ops#free */
#define AXIS2_DESC_FREE(desc, env) \
        ((desc)->ops->free (desc, env))

/** Adds given parameter.
    @sa axis2_desc_ops#add_param*/
#define AXIS2_DESC_ADD_PARAM(desc, env, param) \
      ((desc)->ops->add_param (desc, env, param))

/** Gets named parameter.
    @sa axis2_desc_ops#get_param */
#define AXIS2_DESC_GET_PARAM(desc, env, key) \
      ((desc)->ops->get_param (desc, env, key))

/** Gets the map of all parameters.
    @sa axis2_desc_ops#get_all_params */
#define AXIS2_DESC_GET_ALL_PARAMS(desc, env) \
      ((desc)->ops->get_all_params (desc, env))

/** Checks if named parameter is locked.
    @sa axis2_desc_ops#is_param_locked */
#define AXIS2_DESC_IS_PARAM_LOCKED(desc, env, param_name) \
        ((desc)->ops->is_param_locked(desc, env, param_name))

/** Adds child with given key.
    @sa axis2_desc_ops#add_child */
#define AXIS2_DESC_ADD_CHILD(desc, env, key, child) \
    ((desc)->ops->add_child(desc, env, key, child))

/** Gets the map of all children.
    @sa axis2_desc_ops#get_all_children */
#define AXIS2_DESC_GET_ALL_CHILDREN(desc, env) \
    ((desc)->ops->get_all_children(desc, env))

/** Gets child with given key.
    @sa axis2_desc_ops#get_child */
#define AXIS2_DESC_GET_CHILD(desc, env, key) \
    ((desc)->ops->get_child(desc, env, key))

/** Removes child with given key.
    @sa axis2_desc_ops#remove_child */
#define AXIS2_DESC_REMOVE_CHILD(desc, env, key) \
    ((desc)->ops->remove_child(desc, env, key))

/** @} */
#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_DESC_H */
