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

#include <axis2_linked_list.h>
#include <axis2_utils.h>

struct axis2_linked_list
{
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
};

AXIS2_EXTERN axis2_linked_list_t *AXIS2_CALL
axis2_linked_list_create(const axis2_env_t *env)
{
    axis2_linked_list_t *linked_list = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    linked_list = AXIS2_MALLOC(env->allocator, sizeof(axis2_linked_list_t));
    if (!linked_list)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    linked_list->size = 0;
    linked_list->mod_count = 0;
    linked_list->first = NULL;
    linked_list->last = NULL;

    return linked_list;
}

static entry_t *
axis2_linked_list_create_entry(const axis2_env_t *env, void *data)
{
    entry_t *entry = (entry_t *) AXIS2_MALLOC(env->allocator, sizeof(entry_t));
    if (!entry)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    entry->data = data;
    entry->previous = NULL;
    entry->next = NULL;
    return entry;
}

static axis2_status_t
axis2_linked_list_add_last_entry(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        entry_t *e)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, e, AXIS2_FAILURE);

    linked_list->mod_count++;
    if (linked_list->size == 0)
    {
        linked_list->first = linked_list->last = e;
    }
    else
    {
        e->previous = linked_list->last;
        linked_list->last->next = e;
        linked_list->last = e;
    }
    linked_list->size++;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL axis2_linked_list_free(
    axis2_linked_list_t *linked_list,
    const axis2_env_t *env)
{
    entry_t *current = NULL, *next = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    current = linked_list->first;
    while (current)
    {
        next = current->next;
        AXIS2_FREE(env->allocator, current);
        current = next;
    }
    AXIS2_FREE(env->allocator, linked_list);
    linked_list = NULL;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN entry_t * AXIS2_CALL
axis2_linked_list_get_entry(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int n)
{
    entry_t *e = NULL;
    if (n < linked_list->size / 2)
    {
        e = linked_list->first;
        /* n less than size/2, iterate from start */
        while (n > 0)
        {
            e = e->next;
            n = n - 1;
        }
    }
    else
    {
        e = linked_list->last;
        /* n greater than size/2, iterate from end */
        while ((n = n + 1) < linked_list->size)
        {
            e = e->previous;
        }
    }

    return e;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_linked_list_remove_entry(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        entry_t *e)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, e, AXIS2_FAILURE);
    linked_list->mod_count++;
    linked_list->size--;
    if (linked_list->size == 0)
    {
        linked_list->first = linked_list->last = NULL;
    }
    else
    {
        if (e == linked_list->first)
        {
            linked_list->first = e->next;
            e->next->previous = NULL;
        }
        else if (e == linked_list->last)
        {
            linked_list->last = e->previous;
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

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_linked_list_check_bounds_inclusive(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    if (index < 0 || index > linked_list->size)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INDEX_OUT_OF_BOUNDS, AXIS2_FAILURE);
        return AXIS2_FALSE;
    }
    return AXIS2_TRUE;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_linked_list_check_bounds_exclusive(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    if (index < 0 || index >= linked_list->size)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INDEX_OUT_OF_BOUNDS, AXIS2_FAILURE);
        return AXIS2_FALSE;
    }
    return AXIS2_TRUE;
}

AXIS2_EXTERN void * AXIS2_CALL
axis2_linked_list_get_first(axis2_linked_list_t *linked_list,
        const axis2_env_t *env)
{
    if (linked_list->size == 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_SUCH_ELEMENT, AXIS2_FAILURE);
        return NULL;
    }

    return linked_list->first->data;
}

AXIS2_EXTERN void * AXIS2_CALL
axis2_linked_list_get_last(axis2_linked_list_t *linked_list,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    if (linked_list->size == 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_SUCH_ELEMENT, AXIS2_FAILURE);
        return NULL;
    }

    return linked_list->last->data;
}

AXIS2_EXTERN void * AXIS2_CALL
axis2_linked_list_remove_first(axis2_linked_list_t *linked_list,
        const axis2_env_t *env)
{
    void *r;
    AXIS2_ENV_CHECK(env, NULL);

    if (linked_list->size == 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_SUCH_ELEMENT, AXIS2_FAILURE);
        return NULL;
    }

    linked_list->mod_count++;
    linked_list->size--;
    r = linked_list->first->data;

    if (linked_list->first->next)
    {
        linked_list->first->next->previous = NULL;
    }
    else
    {
        linked_list->last = NULL;
    }

    linked_list->first = linked_list->
            first->next;

    return r;
}

AXIS2_EXTERN void * AXIS2_CALL
axis2_linked_list_remove_last(axis2_linked_list_t *linked_list,
        const axis2_env_t *env)
{
    void *r = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    if (linked_list->size == 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_SUCH_ELEMENT, AXIS2_FAILURE);
        return NULL;
    }

    linked_list->mod_count++;
    linked_list->size--;
    r = linked_list->last->data;

    if (linked_list->last->previous)
    {
        linked_list->last->previous->next = NULL;
    }
    else
    {
        linked_list->first = NULL;
    }

    linked_list->last = linked_list->last->previous;

    return r;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_linked_list_add_first(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o)
{
    entry_t *e ;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, o, AXIS2_FAILURE);

    e = axis2_linked_list_create_entry(env, o);

    linked_list->mod_count++;
    if (linked_list->size == 0)
    {
        linked_list->first = linked_list->last = e;
    }
    else
    {
        e->next = linked_list->first;
        linked_list->first->previous = e;
        linked_list->first = e;
    }
    linked_list->size++;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_linked_list_add_last(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o)
{
    entry_t *e;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, o, AXIS2_FAILURE);

    e = axis2_linked_list_create_entry(env, o);
    return axis2_linked_list_add_last_entry(linked_list, env, e);
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_linked_list_contains(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o)
{
    entry_t *e;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, o, AXIS2_FALSE);

    e = linked_list->first;
    while (e)
    {
        if (o == e->data)
            return AXIS2_TRUE;
        e = e->next;
    }
    return AXIS2_FALSE;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_linked_list_size(axis2_linked_list_t *linked_list,
        const axis2_env_t *env)
{
    return linked_list->size;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_linked_list_add(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o)
{
    entry_t *e;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, o, AXIS2_FALSE);
    e = axis2_linked_list_create_entry(env, o);
    return axis2_linked_list_add_last_entry(linked_list, env, e);
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_linked_list_remove(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o)
{
    entry_t *e;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, o, AXIS2_FALSE);

    e = linked_list->first;
    while (e)
    {
        if (o == e->data)
        {
            return axis2_linked_list_remove_entry(linked_list, env, e);
        }
        e = e->next;
    }
    return AXIS2_FALSE;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_linked_list_clear(axis2_linked_list_t *linked_list,
        const axis2_env_t *env)
{
    if (linked_list->size > 0)
    {
        linked_list->mod_count++;
        linked_list->first = NULL;
        linked_list->last = NULL;
        linked_list->size = 0;
    }

    return AXIS2_SUCCESS;
}


AXIS2_EXTERN void * AXIS2_CALL
axis2_linked_list_get(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index)
{
    AXIS2_ENV_CHECK(env, NULL);
    axis2_linked_list_check_bounds_exclusive(linked_list, env, index);
    return axis2_linked_list_get_entry(linked_list, env, index)->data;
}


AXIS2_EXTERN void * AXIS2_CALL
axis2_linked_list_set(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index, void *o)
{
    entry_t *e;
    void *old;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, o, NULL);
    axis2_linked_list_check_bounds_exclusive(linked_list, env, index);
    e = axis2_linked_list_get_entry(linked_list, env, index);
    old = e->data;
    e->data = o;
    return old;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_linked_list_add_at_index(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index,
        void *o)
{
    entry_t *after = NULL;
    entry_t *e;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, o, AXIS2_FAILURE);

    axis2_linked_list_check_bounds_inclusive(linked_list, env, index);
    e = axis2_linked_list_create_entry(env, o);

    if (index < linked_list->size)
    {
        linked_list->mod_count++;
        after = axis2_linked_list_get_entry(linked_list, env, index);
        e->next = after;
        e->previous = after->previous;
        if (after->previous == NULL)
            linked_list->first = e;
        else
            after->previous->next = e;
        after->previous = e;
        linked_list->size++;
    }
    else
    {
        axis2_linked_list_add_last_entry(linked_list, env, e);
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN void * AXIS2_CALL
axis2_linked_list_remove_at_index(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        int index)
{
    entry_t *e;
    AXIS2_ENV_CHECK(env, NULL);
    axis2_linked_list_check_bounds_exclusive(linked_list, env, index);
    e = axis2_linked_list_get_entry(linked_list, env, index);
    axis2_linked_list_remove_entry(linked_list, env, e);
    return e->data;
}


AXIS2_EXTERN int AXIS2_CALL
axis2_linked_list_index_of(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o)
{
    int index = 0;
    entry_t *e;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, o, AXIS2_FAILURE);

    e = linked_list->first;
    while (e)
    {
        if (o == e->data)
            return index;
        index++;
        e = e->next;
    }
    return -1;
}


AXIS2_EXTERN int AXIS2_CALL
axis2_linked_list_last_index_of(axis2_linked_list_t *linked_list,
        const axis2_env_t *env,
        void *o)
{
    int index;
    entry_t *e;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, o, AXIS2_FAILURE);

    index = linked_list->size - 1;
    e = linked_list->last;
    while (e)
    {
        if (o == e->data)
            return index;
        index--;
        e = e->previous;
    }
    return -1;
}

AXIS2_EXTERN void ** AXIS2_CALL
axis2_linked_list_to_array(axis2_linked_list_t *linked_list,
        const axis2_env_t *env)
{
    int i = 0;
    void **array;
    entry_t *e;
    AXIS2_ENV_CHECK(env, NULL);
    array = (void **) AXIS2_MALLOC(env->allocator,
                linked_list->size * sizeof(void *));
    e = linked_list->first;
    for (i = 0; i < linked_list->size; i++)
    {
        array[i] = e->data;
        e = e->next;
    }
    return array;
}
