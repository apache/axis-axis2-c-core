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
 
#ifndef AXIS2_ARRAY_LIST_H
#define AXIS2_ARRAY_LIST_H

/**
 * @file axis2_array_list.h
 * @brief Axis2 array_list interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_array_list;
struct axis2_array_list_ops;
    
static const int AXIS2_ARRAY_LIST_DEFAULT_CAPACITY = 16;
    
/**
 * @defgroup axis2_array_list Array List
 * @ingroup axis2_util
 * @{
 */

   /**
    *   \brief Array List ops struct
    */
    AXIS2_DECLARE_DATA typedef struct axis2_array_list_ops
    {
       /**
        * Guarantees that this list will have at least enough capacity to
        * hold min_capacity elements. This implementation will grow the list to
        * max(current * 2, min_capacity)
        *
        * @param min_capacity the minimum guaranteed capacity
        */
        axis2_status_t (AXIS2_CALL *ensure_capacity)(struct axis2_array_list *array_list, axis2_env_t **env, int min_capacity);
        
       /**
        * Returns the number of elements in this list.
        *
        * @return the list size
        */
        int (AXIS2_CALL *size)(struct axis2_array_list *array_list, axis2_env_t **env);
        
       /**
        * Checks if the list is empty.
        *
        * @return true if there are no elements, else false
        */
        axis2_bool_t (AXIS2_CALL *is_empty)(struct axis2_array_list *array_list, axis2_env_t **env);
        
       /**
        * Returns true iff element is in this array_list.
        *
        * @param e the element whose inclusion in the List is being tested
        * @return true if the list contains e
        */
        axis2_bool_t (AXIS2_CALL *contains)(struct axis2_array_list *array_list, axis2_env_t **env, void *e);
            
       /**
        * Returns the lowest index at which element appears in this List, or
        * -1 if it does not appear. This looks for the pointer value equality only, 
        * does not look into pointer content
        *
        * @param e the element whose inclusion in the List is being tested
        * @return the index where e was found
        */
        int (AXIS2_CALL *index_of)(struct axis2_array_list *array_list, axis2_env_t **env, void *e);
        
       /**
        * Returns the highest index at which element appears in this List, or
        * -1 if it does not appear. This looks for the pointer value equality only, 
        * does not look into pointer content
        *
        * @param e the element whose inclusion in the List is being tested
        * @return the index where e was found
        */
        int (AXIS2_CALL *last_index_of)(struct axis2_array_list *array_list, axis2_env_t **env, void *e);
        
       /**
        * Returns a void* array containing all of the elements in this array_list.
        * The array is not independent of this list.
        *
        * @return array of elements in this list
        */
        void** (AXIS2_CALL *to_array)(struct axis2_array_list *array_list, axis2_env_t **env);
        
       /**
        * Retrieves the element at the user-supplied index.
        *
        * @param index the index of the element we are fetching
        * @return element at the given index
        */
        void* (AXIS2_CALL *get)(struct axis2_array_list *array_list, axis2_env_t **env, int index);
        
       /**
        * Sets the element at the specified index.  The new element, e,
        * can be an object of any type or null.
        *
        * @param index the index at which the element is being set
        * @param e the element to be set
        * @return the element previously at the specified index
        */
        void* (AXIS2_CALL *set)(struct axis2_array_list *array_list, axis2_env_t **env, int index, void* e);
        
       /**
        * Appends the supplied element to the end of this list.
        * The element, e, can be a pointer of any type or NULL.
        *
        * @param e the element to be appended to this list
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *add)(struct axis2_array_list *array_list, axis2_env_t **env, void* e);
        
       /**
        * Adds the supplied element at the specified index, shifting all
        * elements currently at that index or higher one to the right.
        * The element, e, can be a pointer of any type or NULL.
        *
        * @param index the index at which the element is being added
        * @param e the item being added
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *add_at)(struct axis2_array_list *array_list, axis2_env_t **env, int index, void* e);
        
       /**
        * Removes the element at the user-supplied index.
        *
        * @param index the index of the element to be removed
        * @return the removed void* pointer
        */
        void* (AXIS2_CALL *remove)(struct axis2_array_list *array_list, axis2_env_t **env, int index);
        
       /**
        * Checks that the index is in the range of possible elements (inclusive).
        *
        * @param index the index to check
        * @return AXIS2_FALSE if index &gt; size or index &lt; 0, else AXIS2_TRUE
        */
        axis2_bool_t (AXIS2_CALL *check_bound_inclusive)(struct axis2_array_list *array_list, axis2_env_t **env, int index);
        
       /**
        * Checks that the index is in the range of existing elements (exclusive).
        *
        * @param index the index to check
        * @return AXIS2_FALSE if index &gt;= size or index &lt; 0, else AXIS2_TRUE
        */
        axis2_bool_t (AXIS2_CALL *check_bound_exclusive)(struct axis2_array_list *array_list, axis2_env_t **env, int index);
            
        axis2_status_t (AXIS2_CALL *free)(struct axis2_array_list *array_list, axis2_env_t **env);

    } axis2_array_list_ops_t;

   /** 
    * \brief Array List struct
    */
     
    typedef struct axis2_array_list
    {
        /** Handler Description related ops */
        axis2_array_list_ops_t *ops;
    } axis2_array_list_t;

   /**
    * Constructs a new array list with the supplied initial capacity.
    * If capacity is invalid (<= 0) then default capacity is used
    * @param capacity initial capacity of this array_list
    */

    AXIS2_DECLARE(axis2_array_list_t*) axis2_array_list_create(axis2_env_t **env, int capacity);
    /**
     * Free array passed as void pointer.
     */
    AXIS2_DECLARE(axis2_status_t)
    axis2_array_list_free_void_arg(void *array_list, 
                                axis2_env_t **env);


#define AXIS2_ARRAY_LIST_FREE(array_list, env) ((array_list)->ops->free(array_list, env))
#define AXIS2_ARRAY_LIST_SIZE(array_list, env) ((array_list)->ops->size(array_list, env))
#define AXIS2_ARRAY_LIST_IS_EMPTY(array_list, env) ((array_list)->ops->is_empty(array_list, env))
#define AXIS2_ARRAY_LIST_CONTAINS(array_list, env, e) ((array_list)->ops->contains(array_list, env, e))
#define AXIS2_ARRAY_LIST_INDEX_OF(array_list, env, e) ((array_list)->ops->index_of(array_list, env, e))
#define AXIS2_ARRAY_LIST_LAST_INDEX_OF(array_list, env, e) ((array_list)->ops->last_index_of(array_list, env, e))    
#define AXIS2_ARRAY_LIST_TO_ARRAY(array_list, env) ((array_list)->ops->index_of(array_list, env))
#define AXIS2_ARRAY_LIST_GET(array_list, env, index) ((array_list)->ops->get(array_list, env, index))
#define AXIS2_ARRAY_LIST_SET(array_list, env, index, e) ((array_list)->ops->set(array_list, env, index, e))
#define AXIS2_ARRAY_LIST_ADD(array_list, env, e) ((array_list)->ops->add(array_list, env, e))
#define AXIS2_ARRAY_LIST_ADD_AT(array_list, env, index, e) ((array_list)->ops->add_at(array_list, env, index, e))
#define AXIS2_ARRAY_LIST_REMOVE(array_list, env, index) ((array_list)->ops->remove(array_list, env, index))

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_ARRAY_LIST_H */
