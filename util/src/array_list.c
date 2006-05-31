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

#include <axis2_array_list.h>
#include <axis2_utils.h>

typedef struct axis2_array_list_impl
{
    /** handler description */
	axis2_array_list_t array_list;
    /**The number of elements in this list. */
    int size;
    /**Current capacity of this list. */
    int capacity;
    /** Where the data is stored. */
    void** data;
} axis2_array_list_impl_t;
  
/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(array_list) ((axis2_array_list_impl_t *)array_list)

axis2_status_t AXIS2_CALL 
axis2_array_list_ensure_capacity(struct axis2_array_list *array_list, 
                                 const axis2_env_t *env, 
                                 int min_capacity);
                                 
int AXIS2_CALL 
axis2_array_list_size(struct axis2_array_list *array_list, 
                      const axis2_env_t *env);
                      
axis2_bool_t AXIS2_CALL 
axis2_array_list_is_empty(struct axis2_array_list *array_list, 
                          const axis2_env_t *env);
                          
axis2_bool_t AXIS2_CALL 
axis2_array_list_contains(struct axis2_array_list *array_list, 
                          const axis2_env_t *env, 
                          void *e);
                          
int AXIS2_CALL 
axis2_array_list_index_of(struct axis2_array_list *array_list, 
                          const axis2_env_t *env, 
                          void *e);
int AXIS2_CALL 
axis2_array_list_last_index_of(struct axis2_array_list *array_list, 
                                const axis2_env_t *env, 
                                void *e);
                                
void** AXIS2_CALL 
axis2_array_list_to_array(struct axis2_array_list *array_list, 
                          const axis2_env_t *env);
                          
void* AXIS2_CALL 
axis2_array_list_get(struct axis2_array_list *array_list, 
                     const axis2_env_t *env, 
                    int index);
                    
void* AXIS2_CALL 
axis2_array_list_set(struct axis2_array_list *array_list, 
                     const axis2_env_t *env, 
                     int index, 
                     void* e);
                     
axis2_status_t AXIS2_CALL 
axis2_array_list_add(struct axis2_array_list *array_list, 
                     const axis2_env_t *env, 
                     void* e);
                     
axis2_status_t AXIS2_CALL 
axis2_array_list_add_at(struct axis2_array_list *array_list, 
                        const axis2_env_t *env, 
                        int index, 
                        void* e);    
                        
void* AXIS2_CALL 
axis2_array_list_remove(struct axis2_array_list *array_list, 
                        const axis2_env_t *env, 
                        int index);

axis2_bool_t AXIS2_CALL 
axis2_array_list_check_bound_inclusive(struct axis2_array_list *array_list, 
                                       const axis2_env_t *env, 
                                       int index);
                                       
axis2_bool_t AXIS2_CALL 
axis2_array_list_check_bound_exclusive(struct axis2_array_list *array_list, 
                                       const axis2_env_t *env, 
                                      int index);
                                      
axis2_status_t AXIS2_CALL 
axis2_array_list_free(struct axis2_array_list *array_list, 
                      const axis2_env_t *env);

struct axis2_array_list* 
AXIS2_CALL axis2_array_list_create(const axis2_env_t *env, int capacity)
{
    axis2_array_list_impl_t *array_list_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    array_list_impl = AXIS2_MALLOC( env->allocator, sizeof(axis2_array_list_impl_t) );
    if (!array_list_impl)
    { 
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    array_list_impl->size = 0;
    array_list_impl->capacity = 0;
    array_list_impl->data = NULL;
	
    /* Check capacity, and set the default if error */
    if (capacity <= 0)
        capacity = AXIS2_ARRAY_LIST_DEFAULT_CAPACITY;
    array_list_impl->data = AXIS2_MALLOC(env->allocator, sizeof(void*) * capacity );
    if (!array_list_impl->data)
    {
        axis2_array_list_free(&(array_list_impl->array_list), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    array_list_impl->capacity = capacity;    
    
    /* initialize ops */
    array_list_impl->array_list.ops = NULL;
    array_list_impl->array_list.ops  = AXIS2_MALLOC( env->allocator, sizeof(axis2_array_list_ops_t) );
    if (!array_list_impl->array_list.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_array_list_free(&(array_list_impl->array_list), env);
        return NULL;        
    }

    array_list_impl->array_list.ops->ensure_capacity = axis2_array_list_ensure_capacity;
    array_list_impl->array_list.ops->size = axis2_array_list_size;
    array_list_impl->array_list.ops->is_empty = axis2_array_list_is_empty;
    array_list_impl->array_list.ops->contains = axis2_array_list_contains;
    array_list_impl->array_list.ops->index_of = axis2_array_list_index_of;
    array_list_impl->array_list.ops->last_index_of = axis2_array_list_last_index_of;
    array_list_impl->array_list.ops->to_array = axis2_array_list_to_array;
    array_list_impl->array_list.ops->get = axis2_array_list_get;
    array_list_impl->array_list.ops->set = axis2_array_list_set;
    array_list_impl->array_list.ops->add = axis2_array_list_add;
    array_list_impl->array_list.ops->add_at = axis2_array_list_add_at;
    array_list_impl->array_list.ops->remove = axis2_array_list_remove;
    array_list_impl->array_list.ops->check_bound_inclusive = axis2_array_list_check_bound_inclusive;
    array_list_impl->array_list.ops->check_bound_exclusive = axis2_array_list_check_bound_exclusive;            
    array_list_impl->array_list.ops->free = axis2_array_list_free;

    return &(array_list_impl->array_list);
}


axis2_status_t AXIS2_CALL axis2_array_list_ensure_capacity(struct axis2_array_list *array_list, const axis2_env_t *env, int min_capacity)
{
    axis2_array_list_impl_t *array_list_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    array_list_impl = AXIS2_INTF_TO_IMPL(array_list);
    
    if (min_capacity > array_list_impl->capacity)
    {
        int new_capacity = (array_list_impl->capacity * 2 > min_capacity) ? 
                                (array_list_impl->capacity * 2) : min_capacity;
        array_list_impl->data = AXIS2_REALLOC(env->allocator, array_list_impl->data, sizeof(void*) * new_capacity);
        if (!array_list_impl->data)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        array_list_impl->capacity = new_capacity;
    }
    return AXIS2_SUCCESS;
}

int AXIS2_CALL axis2_array_list_size(struct axis2_array_list *array_list, const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(array_list)->size;
}

axis2_bool_t AXIS2_CALL axis2_array_list_is_empty(struct axis2_array_list *array_list, const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(array_list)->size == 0;
}

axis2_bool_t AXIS2_CALL axis2_array_list_contains(struct axis2_array_list *array_list, const axis2_env_t *env, void *e)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return axis2_array_list_index_of(array_list, env, e) != -1;
}

int AXIS2_CALL axis2_array_list_index_of(struct axis2_array_list *array_list, const axis2_env_t *env, void *e)
{
    axis2_array_list_impl_t *array_list_impl = NULL;
    int i = 0;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    array_list_impl = AXIS2_INTF_TO_IMPL(array_list);    
    
    for (i = 0; i < array_list_impl->size; i++)
        if (e == array_list_impl->data[i])
            return i;
    return -1;
}

int AXIS2_CALL axis2_array_list_last_index_of(struct axis2_array_list *array_list, const axis2_env_t *env, void *e)
{
    axis2_array_list_impl_t *array_list_impl = NULL;
    int i = 0;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    array_list_impl = AXIS2_INTF_TO_IMPL(array_list);    

    for (i = array_list_impl->size - 1; i >= 0; i--)
        if (e == array_list_impl->data[i])
            return i;
    return -1;
}

void** AXIS2_CALL axis2_array_list_to_array(struct axis2_array_list *array_list, const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(array_list)->data;
}

void* AXIS2_CALL axis2_array_list_get(struct axis2_array_list *array_list, const axis2_env_t *env, int index)
{
    AXIS2_ENV_CHECK(env, NULL);
    if (axis2_array_list_check_bound_exclusive(array_list, env, index) )
        return AXIS2_INTF_TO_IMPL(array_list)->data[index];
    else
        return NULL;
}

void* AXIS2_CALL axis2_array_list_set(struct axis2_array_list *array_list, const axis2_env_t *env, int index, void* e)
{
    void* result = NULL;
    axis2_array_list_impl_t *array_list_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    array_list_impl = AXIS2_INTF_TO_IMPL(array_list);    
    
    if (axis2_array_list_check_bound_exclusive(array_list, env, index))
    {
        result = array_list_impl->data[index];
        array_list_impl->data[index] = e;
    }
    
    return result;
}

axis2_status_t AXIS2_CALL axis2_array_list_add(struct axis2_array_list *array_list, const axis2_env_t *env, void* e)
{
    axis2_array_list_impl_t *array_list_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    array_list_impl = AXIS2_INTF_TO_IMPL(array_list);    
   
    if (array_list_impl->size == array_list_impl->capacity)
      if (axis2_array_list_ensure_capacity(array_list, env, array_list_impl->size + 1) != AXIS2_SUCCESS )
          return AXIS2_FAILURE;
    array_list_impl->data[array_list_impl->size++] = e;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_array_list_add_at(struct axis2_array_list *array_list, const axis2_env_t *env, int index, void* e)
{
    int i = 0;
    axis2_array_list_impl_t *array_list_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    array_list_impl = AXIS2_INTF_TO_IMPL(array_list);    
    
    if (!axis2_array_list_check_bound_inclusive(array_list, env, index))
        return AXIS2_FAILURE;
    
    if (array_list_impl->size == array_list_impl->capacity)
    {
        if (axis2_array_list_ensure_capacity(array_list, env, array_list_impl->size + 1) != AXIS2_SUCCESS )
            return AXIS2_FAILURE;
    }
      
    if (index != array_list_impl->size)
    {
        for (i = array_list_impl->size; i > index; i--)
            array_list_impl->data[i] = array_list_impl->data[i - 1];
    }
    
    array_list_impl->data[index] = e;
    array_list_impl->size++;
    return AXIS2_SUCCESS;
}

void* AXIS2_CALL axis2_array_list_remove(struct axis2_array_list *array_list, const axis2_env_t *env, int index)
{
    void* result = NULL;
    int i = 0;
    axis2_array_list_impl_t *array_list_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    array_list_impl = AXIS2_INTF_TO_IMPL(array_list);    
    
    
    if (axis2_array_list_check_bound_exclusive(array_list, env, index))
    {
        result = array_list_impl->data[index];
        for (i = index; i < array_list_impl->size - 1; i++)
            array_list_impl->data[i] = array_list_impl->data[i + 1];
    }
    
    return result;
}

axis2_bool_t AXIS2_CALL axis2_array_list_check_bound_inclusive(struct axis2_array_list *array_list, const axis2_env_t *env, int index)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    if (index < 0 || index > AXIS2_INTF_TO_IMPL(array_list)->size)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INDEX_OUT_OF_BOUNDS, AXIS2_FAILURE);
        return AXIS2_FALSE;
    }
    return AXIS2_TRUE;
}

axis2_bool_t AXIS2_CALL axis2_array_list_check_bound_exclusive(struct axis2_array_list *array_list, const axis2_env_t *env, int index)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    if (index < 0 || index >= AXIS2_INTF_TO_IMPL(array_list)->size)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INDEX_OUT_OF_BOUNDS, AXIS2_FAILURE);
        return AXIS2_FALSE;
    }
    return AXIS2_TRUE;
}

axis2_status_t AXIS2_CALL axis2_array_list_free(struct axis2_array_list *array_list, const axis2_env_t *env)
{
    axis2_array_list_impl_t *array_list_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    array_list_impl = AXIS2_INTF_TO_IMPL(array_list);
    
    if (array_list_impl->array_list.ops)
    {
        AXIS2_FREE(env->allocator, array_list_impl->array_list.ops);
        array_list_impl->array_list.ops = NULL;
    }
    
    if (array_list_impl->data)
    {
        AXIS2_FREE(env->allocator, array_list_impl->data);
        array_list_impl->data = NULL;
    }
    
    AXIS2_FREE(env->allocator, array_list_impl);
    array_list_impl = NULL;
    
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_status_t) 
axis2_array_list_free_void_arg(void *array_list, 
                                const axis2_env_t *env)
{
    axis2_array_list_t *array_list_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    array_list_l = (axis2_array_list_t *) array_list;
    return axis2_array_list_free(array_list_l, env);
}
