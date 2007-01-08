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

#ifndef AXIS2_LINKED_LIST_H
#define AXIS2_LINKED_LIST_H

/**
 * @file axis2_linked_list.h
 * @brief Axis2 linked_list interface
 */

#include <axis2_utils_defines.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_linked_list axis2_linked_list_t;

    /**
     * @defgroup axis2_linked_list linked list
     * @ingroup axis2_util
     * @{
     */

    /**
     * Struct to represent an entry in the list. Holds a single element.
     */
    typedef struct entry_s
    {
        /** The element in the list. */
        void *data;

        /** The next list entry, null if this is last. */
        struct entry_s *next;

        /** The previous list entry, null if this is first. */
        struct entry_s *previous;

    }
    entry_t; /* struct entry */

    /**
    * Create an empty linked list.
    */
    AXIS2_EXTERN axis2_linked_list_t* AXIS2_CALL 
    axis2_linked_list_create(const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_linked_list_free(axis2_linked_list_t *linked_list,
        const axis2_env_t *env);

    /**
    * Obtain the Entry at a given position in a list. This method of course
    * takes linear time, but it is intelligent enough to take the shorter of the
    * paths to get to the Entry required. This implies that the first or last
    * entry in the list is obtained in constant time, which is a very desirable
    * property.
    * For speed and flexibility, range checking is not done in this method:
    * Incorrect values will be returned if (n &lt; 0) or (n &gt;= size).
    *
    * @param n the number of the entry to get
    * @return the entry at position n
    */
    AXIS2_EXTERN entry_t * AXIS2_CALL
    axis2_linked_list_get_entry(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int n);

    /**
     * Remove an entry from the list. This will adjust size and deal with
     *  `first' and  `last' appropriatly.
     *
     * @param e the entry to remove
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_linked_list_remove_entry(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        entry_t *e);


    /**
     * Checks that the index is in the range of possible elements (inclusive).
     *
     * @param index the index to check
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_linked_list_check_bounds_inclusive(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index);

    /**
    * Checks that the index is in the range of existing elements (exclusive).
    *
    * @param index the index to check
    */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_linked_list_check_bounds_exclusive(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index);

    /**
    * Returns the first element in the list.
    *
    * @return the first list element
    */
    AXIS2_EXTERN void * AXIS2_CALL
    axis2_linked_list_get_first(axis2_linked_list_t *linked_list,
        const axis2_env_t *env);

    /**
    * Returns the last element in the list.
    *
    * @return the last list element
    */
    AXIS2_EXTERN void * AXIS2_CALL
    axis2_linked_list_get_last(axis2_linked_list_t *linked_list,
        const axis2_env_t *env);

    /**
    * Remove and return the first element in the list.
    *
    * @return the former first element in the list
    */
    AXIS2_EXTERN void * AXIS2_CALL
    axis2_linked_list_remove_first(axis2_linked_list_t *linked_list,
        const axis2_env_t *env);

    /**
    * Remove and return the last element in the list.
    *
    * @return the former last element in the list
    */
    AXIS2_EXTERN void * AXIS2_CALL
    axis2_linked_list_remove_last(axis2_linked_list_t *linked_list,
        const axis2_env_t *env);

    /**
    * Insert an element at the first of the list.
    *
    * @param o the element to insert
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_linked_list_add_first(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o);

    /**
    * Insert an element at the last of the list.
    *
    * @param o the element to insert
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_linked_list_add_last(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o);

    /**
    * Returns true if the list contains the given object. Comparison is done by
    * <code>o == null ? e = null : o.equals(e)</code>.
    *
    * @param o the element to look for
    * @return true if it is found
    */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_linked_list_contains(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o);

    /**
    * Returns the size of the list.
    *
    * @return the list size
    */
    AXIS2_EXTERN int AXIS2_CALL
    axis2_linked_list_size(axis2_linked_list_t *linked_list,
        const axis2_env_t *env);

    /**
    * Adds an element to the end of the list.
    *
    * @param e the entry to add
    * @return true, as it always succeeds
    */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_linked_list_add(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o);

    /**
    * Removes the entry at the lowest index in the list that matches the given
    * object, comparing by <code>o == null ? e = null : o.equals(e)</code>.
    *
    * @param o the object to remove
    * @return true if an instance of the object was removed
    */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_linked_list_remove(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o);

    /**
    * Remove all elements from this list.
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_linked_list_clear(axis2_linked_list_t *linked_list,
        const axis2_env_t *env);

    /**
    * Return the element at index.
    *
    * @param index the place to look
    * @return the element at index
    */
    AXIS2_EXTERN void * AXIS2_CALL
    axis2_linked_list_get(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index);

    /**
    * Replace the element at the given location in the list.
    *
    * @param index which index to change
    * @param o the new element
    * @return the prior element
    */
    AXIS2_EXTERN void * AXIS2_CALL
    axis2_linked_list_set(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index, 
        void *o);

    /**
    * Inserts an element in the given position in the list.
    *
    * @param index where to insert the element
    * @param o the element to insert
    */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_linked_list_add_at_index(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index,
        void *o);

    /**
    * Removes the element at the given position from the list.
    *
    * @param index the location of the element to remove
    * @return the removed element
    */
    AXIS2_EXTERN void * AXIS2_CALL
    axis2_linked_list_remove_at_index(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index);

    /**
    * Returns the first index where the element is located in the list, or -1.
    *
    * @param o the element to look for
    * @return its position, or -1 if not found
    */
    AXIS2_EXTERN int AXIS2_CALL
    axis2_linked_list_index_of(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o);

    /**
    * Returns the last index where the element is located in the list, or -1.
    *
    * @param o the element to look for
    * @return its position, or -1 if not found
    */
    AXIS2_EXTERN int AXIS2_CALL
    axis2_linked_list_last_index_of(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o);

    /**
    * Returns an array which contains the elements of the list in order.
    *
    * @return an array containing the list elements
    */
    AXIS2_EXTERN void ** AXIS2_CALL
    axis2_linked_list_to_array(axis2_linked_list_t *linked_list,
        const axis2_env_t *env);


#define AXIS2_LINKED_LIST_FREE(linked_list, env) \
        axis2_linked_list_free(linked_list, env)

#define AXIS2_LINKED_LIST_GET_ENTRY(linked_list, env, n) \
        axis2_linked_list_get_entry(linked_list, env, n)

#define AXIS2_LINKED_LIST_REMOVE_ENTRY(linked_list, env, e) \
        axis2_linked_list_remove_entry(linked_list, env, e)

#define AXIS2_LINKED_LIST_CHECK_BOUNDS_INCLUSIVE(linked_list, env, index) \
        axis2_linked_list_check_bounds_inclusive(linked_list, env, index)

#define AXIS2_LINKED_LIST_CHECK_BOUNDS_EXCLUSIVE(linked_list, env, index) \
        axis2_linked_list_check_bounds_exclusive(linked_list, env, index)

#define AXIS2_LINKED_LIST_GET_FIRST(linked_list, env) \
        axis2_linked_list_get_first(linked_list, env)

#define AXIS2_LINKED_LIST_GET_LAST(linked_list, env) \
        axis2_linked_list_get_last(linked_list, env)

#define AXIS2_LINKED_LIST_REMOVE_FIRST(linked_list, env) \
        axis2_linked_list_remove_first(linked_list, env)

#define AXIS2_LINKED_LIST_REMOVE_LAST(linked_list, env) \
        axis2_linked_list_remove_last(linked_list, env)

#define AXIS2_LINKED_LIST_ADD_FIRST(linked_list, env, o) \
        axis2_linked_list_add_first(linked_list, env, o)

#define AXIS2_LINKED_LIST_ADD_LAST(linked_list, env, o) \
        axis2_linked_list_add_last(linked_list, env, o)

#define AXIS2_LINKED_LIST_CONTAINS(linked_list, env, o) \
        axis2_linked_list_contains(linked_list, env, o)

#define AXIS2_LINKED_LIST_SIZE(linked_list, env) \
        axis2_linked_list_size(linked_list, env)

#define AXIS2_LINKED_LIST_ADD(linked_list, env, o) \
        axis2_linked_list_add(linked_list, env, o)

#define AXIS2_LINKED_LIST_REMOVE(linked_list, env, o) \
        axis2_linked_list_remove(linked_list, env, o)

#define AXIS2_LINKED_LIST_CLEAR(linked_list, env) \
        axis2_linked_list_clear(linked_list, env, index)

#define AXIS2_LINKED_LIST_GET(linked_list, env, index) \
        axis2_linked_list_get(linked_list, env, index)

#define AXIS2_LINKED_LIST_SET(linked_list, env, index, o) \
        axis2_linked_list_set(linked_list, env, index, o)

#define AXIS2_LINKED_LIST_ADD_AT_INDEX(linked_list, env, index, o) \
        axis2_linked_list_add_at_index(linked_list, env, index, o)

#define AXIS2_LINKED_LIST_REMOVE_AT_INDEX(linked_list, env, index) \
        axis2_linked_list_remove_at_index(linked_list, env, index)

#define AXIS2_LINKED_LIST_INDEX_OF(linked_list, env, o) \
        axis2_linked_list_index_of(linked_list, env, o)

#define AXIS2_LINKED_LIST_LAST_INDEX_OF(linked_list, env, o) \
        axis2_linked_list_last_index_of(linked_list, env, o)

#define AXIS2_LINKED_LIST_TO_ARRAY(linked_list, env) \
        axis2_linked_list_to_array(linked_list, env)

    /** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_LINKED_LIST_H */
