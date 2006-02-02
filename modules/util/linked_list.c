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

#include <axis2_linked_list.h>
#include <axis2.h>

typedef struct axis2_linked_list_impl
{
    /** handler description */
	axis2_linked_list_t linked_list;
    /**The number of elements in this list. */
    int size;
    
    /**
     * The first element in the list.
     */
    entry_t *first;

    /**
    * The last element in the list.
    */
    entry_t *last;
    
    /**
    * A count of the number of structural modifications that have been made to
    * the list (that is, insertions and removals). Structural modifications
    * are ones which change the list size or affect how iterations would
    * behave. This field is available for use by Iterator and ListIterator,
    * in order to set an error code in response
    * to the next op on the iterator. This <i>fail-fast</i> behavior
    * saves the user from many subtle bugs otherwise possible from concurrent
    * modification during iteration.
    * <p>
    *
    * To make lists fail-fast, increment this field by just 1 in the
    * <code>add(int, Object)</code> and <code>remove(int)</code> methods.
    * Otherwise, this field may be ignored.
    */
    int mod_count;
    
} axis2_linked_list_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(linked_list) ((axis2_linked_list_impl_t *)linked_list)

/********************************Function headers******************************/

axis2_status_t AXIS2_CALL axis2_linked_list_free(
                                        axis2_linked_list_t *linked_list, 
                                        axis2_env_t **env);
 
entry_t * AXIS2_CALL
axis2_linked_list_get_entry(axis2_linked_list_t *linked_list, 
                            axis2_env_t **env, 
                            int n);
                                        
axis2_status_t AXIS2_CALL 
axis2_linked_list_remove_entry(axis2_linked_list_t *linked_list, 
                                axis2_env_t **env, 
                                entry_t *e);
                                        
axis2_bool_t AXIS2_CALL 
axis2_linked_list_check_bounds_inclusive(axis2_linked_list_t *linked_list, 
                                            axis2_env_t **env, 
                                            int index);


axis2_bool_t AXIS2_CALL
axis2_linked_list_check_bounds_exclusive(axis2_linked_list_t *linked_list,
                                            axis2_env_t **env,
                                            int index);


void * AXIS2_CALL
axis2_linked_list_get_first(axis2_linked_list_t *linked_list,
                            axis2_env_t **env);

void * AXIS2_CALL
axis2_linked_list_get_last(axis2_linked_list_t *linked_list,
                            axis2_env_t **env);

void * AXIS2_CALL
axis2_linked_list_remove_first(axis2_linked_list_t *linked_list,
                                axis2_env_t **env);
                                
/**
* Remove and return the last element in the list.
*
* @return the former last element in the list
*/
void * AXIS2_CALL
axis2_linked_list_remove_last(axis2_linked_list_t *linked_list,
                                axis2_env_t **env);


axis2_status_t AXIS2_CALL
axis2_linked_list_add_first(axis2_linked_list_t *linked_list,
                            axis2_env_t **env,
                            void *o);
                            
axis2_status_t AXIS2_CALL
axis2_linked_list_add_last(axis2_linked_list_t *linked_list,
                            axis2_env_t **env,
                            void *o);

static axis2_status_t
add_last_entry(axis2_linked_list_t *linked_list,
                axis2_env_t **env,
                entry_t *e);
                
axis2_bool_t AXIS2_CALL
axis2_linked_list_contains(axis2_linked_list_t *linked_list,
                            axis2_env_t ** env,
                            void *o);

/**
* Returns the size of the list.
*
* @return the list size
*/
int AXIS2_CALL
axis2_linked_list_size(axis2_linked_list_t *linked_list,
                        axis2_env_t **env);
                        

axis2_bool_t AXIS2_CALL
axis2_linked_list_add(axis2_linked_list_t *linked_list, 
                        axis2_env_t **env,
                        void *o);

axis2_bool_t AXIS2_CALL
axis2_linked_list_remove(axis2_linked_list_t *linked_list,
                            axis2_env_t **env,
                            void *o);
                            
axis2_status_t AXIS2_CALL
axis2_linked_list_clear(axis2_linked_list_t *linked_list,
                            axis2_env_t **env);



void * AXIS2_CALL
axis2_linked_list_get(axis2_linked_list_t *linked_list,
                        axis2_env_t **env,
                        int index);

void * AXIS2_CALL
axis2_linked_list_set(axis2_linked_list_t *linked_list,
                        axis2_env_t **env,
                        int index, 
                        void *o);

axis2_status_t AXIS2_CALL
axis2_linked_list_add_at_index(axis2_linked_list_t *linked_list,
                        axis2_env_t **env,
                        int index, 
                        void *o);
                        
void * AXIS2_CALL
axis2_linked_list_remove_at_index(axis2_linked_list_t *linked_list,
                                    axis2_env_t **env,
                                    int index);

int AXIS2_CALL
axis2_linked_list_index_of(axis2_linked_list_t *linked_list,
                            axis2_env_t **env,
                            void *o);
                            
int AXIS2_CALL
axis2_linked_list_last_index_of(axis2_linked_list_t *linked_list,
                                axis2_env_t **env,
                                void *o);

void ** AXIS2_CALL
axis2_linked_list_to_array(axis2_linked_list_t *linked_list,
                            axis2_env_t **env);

/********************************End of function headers***********************/

axis2_linked_list_t *AXIS2_CALL 
axis2_linked_list_create(axis2_env_t **env)
{
    axis2_linked_list_impl_t *linked_list_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    linked_list_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_linked_list_impl_t) );
    if (!linked_list_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
    /* initialize ops */
    linked_list_impl->linked_list.ops = NULL;
    linked_list_impl->linked_list.ops  = AXIS2_MALLOC( (*env)->allocator, 
        sizeof(axis2_linked_list_ops_t) );
    if (!linked_list_impl->linked_list.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_linked_list_free(&(linked_list_impl->linked_list), env);
        return NULL;        
    }

    linked_list_impl->linked_list.ops->free = axis2_linked_list_free;
    linked_list_impl->linked_list.ops->get_entry = axis2_linked_list_get_entry;
    linked_list_impl->linked_list.ops->remove_entry = axis2_linked_list_remove_entry;
    linked_list_impl->linked_list.ops->check_bounds_inclusive = axis2_linked_list_check_bounds_inclusive;
    linked_list_impl->linked_list.ops->check_bounds_exclusive = axis2_linked_list_check_bounds_exclusive;
    linked_list_impl->linked_list.ops->get_first = axis2_linked_list_get_first;
    linked_list_impl->linked_list.ops->get_last = axis2_linked_list_get_last;
    linked_list_impl->linked_list.ops->remove_first = axis2_linked_list_remove_first;
    linked_list_impl->linked_list.ops->remove_last = axis2_linked_list_remove_last;
    linked_list_impl->linked_list.ops->add_first = axis2_linked_list_add_first;
    linked_list_impl->linked_list.ops->add_last = axis2_linked_list_add_last;
    linked_list_impl->linked_list.ops->contains = axis2_linked_list_contains;
    linked_list_impl->linked_list.ops->size = axis2_linked_list_size;            
    linked_list_impl->linked_list.ops->add = axis2_linked_list_add;
    linked_list_impl->linked_list.ops->remove = axis2_linked_list_remove;
    linked_list_impl->linked_list.ops->clear = axis2_linked_list_clear;
    linked_list_impl->linked_list.ops->get = axis2_linked_list_get;
    linked_list_impl->linked_list.ops->set = axis2_linked_list_set;
    linked_list_impl->linked_list.ops->add_at_index = axis2_linked_list_add_at_index;
    linked_list_impl->linked_list.ops->remove_at_index = axis2_linked_list_remove_at_index;
    linked_list_impl->linked_list.ops->index_of = axis2_linked_list_index_of;
    linked_list_impl->linked_list.ops->last_index_of = axis2_linked_list_last_index_of;
    linked_list_impl->linked_list.ops->to_array = axis2_linked_list_to_array;
    
    linked_list_impl->size = 0;
    linked_list_impl->mod_count = 0;
    linked_list_impl->first = NULL;
    linked_list_impl->last = NULL;
    
    return &(linked_list_impl->linked_list);
}
/***************************Function implementation****************************/

/**
 * Construct an entry.
 * @param data the list element
 */
static entry_t *
create_entry(axis2_env_t **env, void *data)
{
    entry_t *entry = (entry_t *) AXIS2_MALLOC((*env)->allocator, sizeof(entry_t));
    if(NULL == entry)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    entry->data = data;
    entry->previous = NULL;
    entry->next = NULL;
    return entry;    
}

static axis2_status_t 
free_entry(entry_t *e,
            axis2_env_t **env)
{
    if(NULL != e->data)
        AXIS2_FREE((*env)->allocator, e->data);
    if(NULL != e->previous)
        AXIS2_FREE((*env)->allocator, e->previous);
    
    if(NULL != e->next)
        AXIS2_FREE((*env)->allocator, e->next);
    
    e = NULL;
    
    return AXIS2_SUCCESS;
}

/**
* Inserts an element at the end of the list.
*
* @param e the entry to add
*/
static axis2_status_t
add_last_entry(axis2_linked_list_t *linked_list,
                axis2_env_t **env,
                entry_t *e)
{
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, e, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(linked_list)->mod_count++;
    if (AXIS2_INTF_TO_IMPL(linked_list)->size == 0)
        AXIS2_INTF_TO_IMPL(linked_list)->first = 
            AXIS2_INTF_TO_IMPL(linked_list)->last = e;
    else
    {
        e->previous = AXIS2_INTF_TO_IMPL(linked_list)->last;
        AXIS2_INTF_TO_IMPL(linked_list)->last->next = e;
        AXIS2_INTF_TO_IMPL(linked_list)->last = e;
    }
    AXIS2_INTF_TO_IMPL(linked_list)->size++;
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_linked_list_free(
                                        axis2_linked_list_t *linked_list, 
                                        axis2_env_t **env)
{
    axis2_linked_list_impl_t *linked_list_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FAILURE);
    
    linked_list_impl = AXIS2_INTF_TO_IMPL(linked_list);
    
    if (NULL != linked_list_impl->linked_list.ops)
    {
        AXIS2_FREE((*env)->allocator, linked_list_impl->linked_list.ops);
        linked_list_impl->linked_list.ops = NULL;
    }
    
    if(NULL != linked_list_impl->first)
    {
        free_entry(linked_list_impl->first, env);
    }
    
    if(NULL != linked_list_impl->last)
    {
        free_entry(linked_list_impl->last, env);
    }
    
    AXIS2_FREE((*env)->allocator, linked_list_impl);
    linked_list_impl = NULL;
    
    return AXIS2_SUCCESS;
}

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
entry_t * AXIS2_CALL
axis2_linked_list_get_entry(axis2_linked_list_t *linked_list, 
                            axis2_env_t **env, 
                            int n)
{
    entry_t *e = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, NULL);
    
    if (n < AXIS2_INTF_TO_IMPL(linked_list)->size / 2)
    {
        e = AXIS2_INTF_TO_IMPL(linked_list)->first;
        /* n less than size/2, iterate from start */
        while (n > 0)
        {
            e = e->next;
            n = n - 1;
        }
    }
    else
    {
        e = AXIS2_INTF_TO_IMPL(linked_list)->last;
        /* n greater than size/2, iterate from end */
        while ((n = n + 1) < AXIS2_INTF_TO_IMPL(linked_list)->size)
          e = e->previous;
    }
    
    return e;
}

/**
 * Remove an entry from the list. This will adjust size and deal with
 *  `first' and  `last' appropriatly.
 *
 * @param e the entry to remove
 */
axis2_status_t AXIS2_CALL 
axis2_linked_list_remove_entry(axis2_linked_list_t *linked_list, 
                                axis2_env_t **env, 
                                entry_t *e)
{
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, e, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(linked_list)->mod_count++;
    AXIS2_INTF_TO_IMPL(linked_list)->size--;
    if (AXIS2_INTF_TO_IMPL(linked_list)->size == 0)
    AXIS2_INTF_TO_IMPL(linked_list)->first = AXIS2_INTF_TO_IMPL(linked_list)->
        last = NULL;
    else
    {
        if (e == AXIS2_INTF_TO_IMPL(linked_list)->first)
        {
            AXIS2_INTF_TO_IMPL(linked_list)->first = e->next;
            e->next->previous = NULL;
        }
        else if (e == AXIS2_INTF_TO_IMPL(linked_list)->last)
        {
            AXIS2_INTF_TO_IMPL(linked_list)->last = e->previous;
            e->previous->next = NULL;
        }
        else
        {
            e->next->previous = e->previous;
            e->previous->next = e->next;
        }
    }
    return AXIS2_SUCCESS;
}

/**
 * Checks that the index is in the range of possible elements (inclusive).
 *
 * @param index the index to check
 */
axis2_bool_t AXIS2_CALL 
axis2_linked_list_check_bounds_inclusive(axis2_linked_list_t *linked_list, 
                                            axis2_env_t **env, 
                                            int index)
{
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FALSE);
    
    if (index < 0 || index > AXIS2_INTF_TO_IMPL(linked_list)->size)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INDEX_OUT_OF_BOUNDS, AXIS2_FAILURE);
        return AXIS2_FALSE;
    }
    return AXIS2_TRUE;
}

/**
* Checks that the index is in the range of existing elements (exclusive).
*
* @param index the index to check
*/
axis2_bool_t AXIS2_CALL
axis2_linked_list_check_bounds_exclusive(axis2_linked_list_t *linked_list,
                                            axis2_env_t **env,
                                            int index)
{
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FALSE);
    if (index < 0 || index >= AXIS2_INTF_TO_IMPL(linked_list)->size)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INDEX_OUT_OF_BOUNDS, AXIS2_FAILURE);
        return AXIS2_FALSE;
    }
    return AXIS2_TRUE;
}

/**
* Returns the first element in the list.
*
* @return the first list element
*/
void * AXIS2_CALL
axis2_linked_list_get_first(axis2_linked_list_t *linked_list,
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, NULL);
    if (AXIS2_INTF_TO_IMPL(linked_list)->size == 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_SUCH_ELEMENT, AXIS2_FAILURE);
        return NULL;
    }
    
    return AXIS2_INTF_TO_IMPL(linked_list)->first->data;
}

/**
* Returns the last element in the list.
*
* @return the last list element
*/
void * AXIS2_CALL
axis2_linked_list_get_last(axis2_linked_list_t *linked_list,
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, NULL);
    
    if (AXIS2_INTF_TO_IMPL(linked_list)->size == 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_SUCH_ELEMENT, AXIS2_FAILURE);
        return NULL;
    }
      
    return AXIS2_INTF_TO_IMPL(linked_list)->last->data;
}

/**
* Remove and return the first element in the list.
*
* @return the former first element in the list
*/
void * AXIS2_CALL
axis2_linked_list_remove_first(axis2_linked_list_t *linked_list,
                                axis2_env_t **env)
{
    void *r;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, NULL);
    
    if (AXIS2_INTF_TO_IMPL(linked_list)->size == 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_SUCH_ELEMENT, AXIS2_FAILURE);
        return NULL;
    }
    
    AXIS2_INTF_TO_IMPL(linked_list)->mod_count++;
    AXIS2_INTF_TO_IMPL(linked_list)->size--;
    r = AXIS2_INTF_TO_IMPL(linked_list)->first->data;
    
    if (AXIS2_INTF_TO_IMPL(linked_list)->first->next != NULL)
        AXIS2_INTF_TO_IMPL(linked_list)->first->next->previous = NULL;
    else
        AXIS2_INTF_TO_IMPL(linked_list)->last = NULL;
    
    AXIS2_INTF_TO_IMPL(linked_list)->first = AXIS2_INTF_TO_IMPL(linked_list)->
        first->next;
    
    return r;
}

/**
* Remove and return the last element in the list.
*
* @return the former last element in the list
*/
void * AXIS2_CALL
axis2_linked_list_remove_last(axis2_linked_list_t *linked_list,
                                axis2_env_t **env)
{
    void *r = NULL;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, NULL);
    
    if (AXIS2_INTF_TO_IMPL(linked_list)->size == 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_SUCH_ELEMENT, AXIS2_FAILURE);
        return NULL;
    }

    AXIS2_INTF_TO_IMPL(linked_list)->mod_count++;
    AXIS2_INTF_TO_IMPL(linked_list)->size--;
    r = AXIS2_INTF_TO_IMPL(linked_list)->last->data;
    
    if (AXIS2_INTF_TO_IMPL(linked_list)->last->previous != NULL)
        AXIS2_INTF_TO_IMPL(linked_list)->last->previous->next = NULL;
    else
        AXIS2_INTF_TO_IMPL(linked_list)->first = NULL;

    AXIS2_INTF_TO_IMPL(linked_list)->last = AXIS2_INTF_TO_IMPL(linked_list)->
        last->previous;

    return r;
}

/**
* Insert an element at the first of the list.
*
* @param o the element to insert
*/
axis2_status_t AXIS2_CALL
axis2_linked_list_add_first(axis2_linked_list_t *linked_list,
                            axis2_env_t **env,
                            void *o)
{
    entry_t *e ;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, o, AXIS2_FAILURE);
    
    e = create_entry(env, o);
    
    AXIS2_INTF_TO_IMPL(linked_list)->mod_count++;
    if (AXIS2_INTF_TO_IMPL(linked_list)->size == 0)
        AXIS2_INTF_TO_IMPL(linked_list)->first = 
            AXIS2_INTF_TO_IMPL(linked_list)->last = e;
    else
    {
        e->next = AXIS2_INTF_TO_IMPL(linked_list)->first;
        AXIS2_INTF_TO_IMPL(linked_list)->first->previous = e;
        AXIS2_INTF_TO_IMPL(linked_list)->first = e;
    }
    AXIS2_INTF_TO_IMPL(linked_list)->size++;
    
    return AXIS2_SUCCESS;
}

/**
* Insert an element at the last of the list.
*
* @param o the element to insert
*/
axis2_status_t AXIS2_CALL
axis2_linked_list_add_last(axis2_linked_list_t *linked_list,
                            axis2_env_t **env,
                            void *o)
{   
    entry_t *e;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, o, AXIS2_FAILURE);
    
    e = create_entry(env, o);
    return add_last_entry(linked_list, env, e);
}

/**
 * Returns true if the list contains the given object. Comparison is done by
 * <code>o == null ? e = null : o.equals(e)</code>.
 *
 * @param o the element to look for
 * @return true if it is found
 */
axis2_bool_t AXIS2_CALL
axis2_linked_list_contains(axis2_linked_list_t *linked_list,
                            axis2_env_t **env,
                            void *o)
{
    entry_t *e;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, o, AXIS2_FALSE);
    
    e = AXIS2_INTF_TO_IMPL(linked_list)->first;
    while (e != NULL)
    {
        if (o == e->data)
            return AXIS2_TRUE;
        e = e->next;
    }
    return AXIS2_FALSE;
}

/**
* Returns the size of the list.
*
* @return the list size
*/
int AXIS2_CALL
axis2_linked_list_size(axis2_linked_list_t *linked_list,
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(linked_list)->size;
}

/**
* Adds an element to the end of the list.
*
* @param e the entry to add
* @return true, as it always succeeds
*/
axis2_bool_t AXIS2_CALL
axis2_linked_list_add(axis2_linked_list_t *linked_list, 
                        axis2_env_t **env,
                        void *o)
{   
    entry_t *e;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, o, AXIS2_FALSE);
    e = create_entry(env, o);
    return add_last_entry(linked_list, env, e);
}

/**
* Removes the entry at the lowest index in the list that matches the given
* object, comparing by <code>o == null ? e = null : o.equals(e)</code>.
*
* @param o the object to remove
* @return true if an instance of the object was removed
*/
axis2_bool_t AXIS2_CALL
axis2_linked_list_remove(axis2_linked_list_t *linked_list,
                            axis2_env_t **env,
                            void *o)
{
    entry_t *e;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, o, AXIS2_FALSE);
    
    e = AXIS2_INTF_TO_IMPL(linked_list)->first;
    while (e != NULL)
    {
        if(o == e->data)
        {
            return axis2_linked_list_remove_entry(linked_list, env, e);
        }
        e = e->next;
    }
    return AXIS2_FALSE;
}



/**
* Remove all elements from this list.
*/
axis2_status_t AXIS2_CALL
axis2_linked_list_clear(axis2_linked_list_t *linked_list,
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FAILURE);
    if (AXIS2_INTF_TO_IMPL(linked_list)->size > 0)
    {
        AXIS2_INTF_TO_IMPL(linked_list)->mod_count++;
        AXIS2_INTF_TO_IMPL(linked_list)->first = NULL;
        AXIS2_INTF_TO_IMPL(linked_list)->last = NULL;
        AXIS2_INTF_TO_IMPL(linked_list)->size = 0;
    }
    
    return AXIS2_SUCCESS;
}

/**
* Return the element at index.
*
* @param index the place to look
* @return the element at index
*/
void * AXIS2_CALL
axis2_linked_list_get(axis2_linked_list_t *linked_list,
                        axis2_env_t **env,
                        int index)
{
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, NULL);
    axis2_linked_list_check_bounds_exclusive(linked_list, env, index);
    return axis2_linked_list_get_entry(linked_list, env, index)->data;
}

/**
* Replace the element at the given location in the list.
*
* @param index which index to change
* @param o the new element
* @return the prior element
*/
void * AXIS2_CALL
axis2_linked_list_set(axis2_linked_list_t *linked_list,
                        axis2_env_t **env,
                        int index, void *o)
{
    entry_t *e;
    void *old;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, o, NULL);
    axis2_linked_list_check_bounds_exclusive(linked_list, env, index);
    e = axis2_linked_list_get_entry(linked_list, env, index);
    old = e->data;
    e->data = o;
    return old;
}

/**
* Inserts an element in the given position in the list.
*
* @param index where to insert the element
* @param o the element to insert
*/
axis2_status_t AXIS2_CALL
axis2_linked_list_add_at_index(axis2_linked_list_t *linked_list,
                                axis2_env_t **env,
                                int index, 
                                void *o)
{
    entry_t *after = NULL;
    entry_t *e;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, o, AXIS2_FAILURE);
    
    axis2_linked_list_check_bounds_inclusive(linked_list, env, index);
    e = create_entry(env, o);
    
    if (index < AXIS2_INTF_TO_IMPL(linked_list)->size)
    {
        AXIS2_INTF_TO_IMPL(linked_list)->mod_count++;
        after = axis2_linked_list_get_entry(linked_list, env, index);
        e->next = after;
        e->previous = after->previous;
        if (after->previous == NULL)
            AXIS2_INTF_TO_IMPL(linked_list)->first = e;
        else
            after->previous->next = e;
        after->previous = e;
        AXIS2_INTF_TO_IMPL(linked_list)->size++;
    }
    else
        add_last_entry(linked_list, env, e);
    
    return AXIS2_SUCCESS;
}

/**
* Removes the element at the given position from the list.
*
* @param index the location of the element to remove
* @return the removed element
*/
void * AXIS2_CALL
axis2_linked_list_remove_at_index(axis2_linked_list_t *linked_list,
                                    axis2_env_t **env,
                                    int index)
{
    entry_t *e;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, NULL);
    axis2_linked_list_check_bounds_exclusive(linked_list, env, index);
    e = axis2_linked_list_get_entry(linked_list, env, index);
    axis2_linked_list_remove_entry(linked_list, env, e);
    return e->data;
}

/**
* Returns the first index where the element is located in the list, or -1.
*
* @param o the element to look for
* @return its position, or -1 if not found
*/
int AXIS2_CALL
axis2_linked_list_index_of(axis2_linked_list_t *linked_list,
                            axis2_env_t **env,
                            void *o)
{
    int index = 0;
    entry_t *e;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, o, AXIS2_FAILURE);
    
    e = AXIS2_INTF_TO_IMPL(linked_list)->first;
    while (e != NULL)
    {
    if (o == e->data)
      return index;
    index++;
    e = e->next;
    }
    return -1;
}

/**
* Returns the last index where the element is located in the list, or -1.
*
* @param o the element to look for
* @return its position, or -1 if not found
*/
int AXIS2_CALL
axis2_linked_list_last_index_of(axis2_linked_list_t *linked_list,
                                axis2_env_t **env,
                                void *o)
{
    int index;
    entry_t *e;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, o, AXIS2_FAILURE);
    
    index = AXIS2_INTF_TO_IMPL(linked_list)->size - 1;
    e = AXIS2_INTF_TO_IMPL(linked_list)->last;
    while (e != NULL)
    {
        if (o == e->data)
            return index;
        index--;
        e = e->previous;
    }
    return -1;
}

/**
* Returns an array which contains the elements of the list in order.
*
* @return an array containing the list elements
*/
void ** AXIS2_CALL
axis2_linked_list_to_array(axis2_linked_list_t *linked_list,
                            axis2_env_t **env)
{
    int i = 0;
    void **array;
    entry_t *e;
    AXIS2_FUNC_PARAM_CHECK(linked_list, env, NULL);
    array = (void **) AXIS2_MALLOC((*env)->allocator, 
        AXIS2_INTF_TO_IMPL(linked_list)->size * sizeof(void *));
    e = AXIS2_INTF_TO_IMPL(linked_list)->first;
    for (i = 0; i < AXIS2_INTF_TO_IMPL(linked_list)->size; i++)
    {
        array[i] = e->data;
        e = e->next;
    }
    return array;
}
