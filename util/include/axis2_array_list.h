/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_ARRAY_LIST_H
#define AXIS2_ARRAY_LIST_H

/**
 * @defgroup axis2_array_list array list
 * @ingroup axis2_util
 * Description.
 * @{
 */

/**
 * @file axis2_array_list.h
 * @brief Axis2 array_list interface
 */

#include <axis2_utils_defines.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_array_list;
    struct axis2_array_list_ops;

    static const int AXIS2_ARRAY_LIST_DEFAULT_CAPACITY = 16;

    /**
     * Array List struct
     */
    typedef struct axis2_array_list
    {
        int ref;
    }
    axis2_array_list_t;

    /**
     * Constructs a new array list with the supplied initial capacity.
     * If capacity is invalid (<= 0) then default capacity is used
     * @param env pointer to environment struct
     * @param capacity initial capacity of this array_list
     */
    AXIS2_EXTERN axis2_array_list_t* AXIS2_CALL axis2_array_list_create(
        const axis2_env_t *env,
        int capacity);

    /**
     * Free array passed as void pointer.
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_array_list_free_void_arg(
        void *array_list,
        const axis2_env_t *env);

    /**
     * Guarantees that this list will have at least enough capacity to
     * hold min_capacity elements. This implementation will grow the list to
     * max(current * 2, min_capacity)
     * @param array_list pointer to array_list
     * @param env pointer to environment struct
     * @param min_capacity the minimum guaranteed capacity
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axis2_array_list_ensure_capacity(
                struct axis2_array_list *array_list,
                const axis2_env_t *env,
                int min_capacity);

    /**
     * Returns the number of elements in this list.
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @return the list size
     */
    AXIS2_EXTERN int AXIS2_CALL 
    axis2_array_list_size(
                struct axis2_array_list *array_list,
                const axis2_env_t *env);

    /**
     * Checks if the list is empty.
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @return true if there are no elements, else false
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL 
    axis2_array_list_is_empty(
                struct axis2_array_list *array_list,
                const axis2_env_t *env);

    /**
     * Returns true iff element is in this array_list.
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @param e the element whose inclusion in the List is being tested
     * @return true if the list contains e
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL 
    axis2_array_list_contains(
                struct axis2_array_list *array_list,
                const axis2_env_t *env,
                void *e);

    /**
     * Returns the lowest index at which element appears in this List, or
     * -1 if it does not appear. This looks for the pointer value equality only, 
     * does not look into pointer content
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @param e the element whose inclusion in the List is being tested
     * @return the index where e was found
     */
    AXIS2_EXTERN int AXIS2_CALL 
    axis2_array_list_index_of(
                struct axis2_array_list *array_list,
                const axis2_env_t *env,
                void *e);

    /**
     * Returns the highest index at which element appears in this List, or
     * -1 if it does not appear. This looks for the pointer value equality only, 
     * does not look into pointer content
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @param e the element whose inclusion in the List is being tested
     * @return the index where e was found
     */
    AXIS2_EXTERN int AXIS2_CALL 
    axis2_array_list_last_index_of(
                struct axis2_array_list *array_list,
                const axis2_env_t *env,
                void *e);

    /**
     * Returns a void* array containing all of the elements in this array_list.
     * The array is not independent of this list.
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @return array of elements in this list
     */
    AXIS2_EXTERN void** AXIS2_CALL 
    axis2_array_list_to_array(
                struct axis2_array_list *array_list,
                const axis2_env_t *env);

    /**
     * Retrieves the element at the user-supplied index.
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @param index the index of the element we are fetching
     * @return element at the given index
     */
    AXIS2_EXTERN void* AXIS2_CALL 
    axis2_array_list_get(struct axis2_array_list *array_list,
                        const axis2_env_t *env,
                        int index);

    /**
     * Sets the element at the specified index.  The new element, e,
     * can be an object of any type or null.
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @param index the index at which the element is being set
     * @param e the element to be set
     * @return the element previously at the specified index
     */
    AXIS2_EXTERN void* AXIS2_CALL 
    axis2_array_list_set(
                    struct axis2_array_list *array_list,
                    const axis2_env_t *env,
                    int index,
                    void* e);

    /**
     * Appends the supplied element to the end of this list.
     * The element, e, can be a pointer of any type or NULL.
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @param e the element to be appended to this list
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axis2_array_list_add(
                    struct axis2_array_list *array_list,
                    const axis2_env_t *env,
                    const void* e);

    /**
     * Adds the supplied element at the specified index, shifting all
     * elements currently at that index or higher one to the right.
     * The element, e, can be a pointer of any type or NULL.
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @param index the index at which the element is being added
     * @param e the item being added
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axis2_array_list_add_at(
                struct axis2_array_list *array_list,
                const axis2_env_t *env,
                const int index,
                const void* e);

    /**
     * Removes the element at the user-supplied index.
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @param index the index of the element to be removed
     * @return the removed void* pointer
     */
    AXIS2_EXTERN void* AXIS2_CALL 
    axis2_array_list_remove(struct axis2_array_list *array_list,
                        const axis2_env_t *env,
                        int index);

    /**
     * Checks that the index is in the range of possible elements (inclusive).
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @param index the index to check
     * @return AXIS2_FALSE if index &gt; size or index &lt; 0, else AXIS2_TRUE
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL 
    axis2_array_list_check_bound_inclusive(
                struct axis2_array_list *array_list,
                const axis2_env_t *env,
                int index);

    /**
     * Checks that the index is in the range of existing elements (exclusive).
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @param index the index to check
     * @return AXIS2_FALSE if index &gt;= size or index &lt; 0, else AXIS2_TRUE
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL 
    axis2_array_list_check_bound_exclusive(
                struct axis2_array_list *array_list,
                const axis2_env_t *env,
                int index);

    /**
     * @param array_list pointer to array list
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axis2_array_list_free(
                struct axis2_array_list *array_list,
                const axis2_env_t *env);


    /** Frees the axis2 array list.
        @sa axis2_array_list_ops#free */
#define AXIS2_ARRAY_LIST_FREE(array_list, env) \
        axis2_array_list_free(array_list, env)

    /** Size.
        @sa axis2_array_list_ops#size */
#define AXIS2_ARRAY_LIST_SIZE(array_list, env) \
        axis2_array_list_size(array_list, env)

    /** Is empty.
        @sa axis2_array_list_ops#is_empty */
#define AXIS2_ARRAY_LIST_IS_EMPTY(array_list, env) \
        axis2_array_list_is_empty(array_list, env)

    /** Contains.
        @sa axis2_array_list_ops#contains */
#define AXIS2_ARRAY_LIST_CONTAINS(array_list, env, e) \
        axis2_array_list_contains(array_list, env, e)

    /** Index of.
        @sa axis2_array_list_ops#index_of */
#define AXIS2_ARRAY_LIST_INDEX_OF(array_list, env, e) \
        axis2_array_list_index_of(array_list, env, e)

    /** Last index of.
        @sa axis2_array_list_ops#last_index_of */
#define AXIS2_ARRAY_LIST_LAST_INDEX_OF(array_list, env, e) \
        axis2_array_list_last_index_of(array_list, env, e)

    /** To array.
        @sa axis2_array_list_ops#to_array */
#define AXIS2_ARRAY_LIST_TO_ARRAY(array_list, env) \
        axis2_array_list_index_of(array_list, env)

    /** Get.
        @sa axis2_array_list_ops#get */
#define AXIS2_ARRAY_LIST_GET(array_list, env, index) \
        axis2_array_list_get(array_list, env, index)

    /** Set.
        @sa axis2_array_list_ops#set */
#define AXIS2_ARRAY_LIST_SET(array_list, env, index, e) \
        axis2_array_list_set(array_list, env, index, e)

    /** Add.
        @sa axis2_array_list_ops#add */
#define AXIS2_ARRAY_LIST_ADD(array_list, env, e) \
        axis2_array_list_add(array_list, env, e)

    /** Add at.
        @sa axis2_array_list_ops#add_at */
#define AXIS2_ARRAY_LIST_ADD_AT(array_list, env, index, e) \
        axis2_array_list_add_at(array_list, env, index, e)

    /** Remove.
        @sa axis2_array_list_ops#remove */
#define AXIS2_ARRAY_LIST_REMOVE(array_list, env, index) \
        axis2_array_list_remove(array_list, env, index)

    /** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_ARRAY_LIST_H */
