/*
 *   Copyright 2004,2005 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 *	
 * @author Dinesh Premalal (xydinesh@gmail.com, premalwd@cse.mrt.ac.lk)	
 */

#include "guththila_environment.h"
#include "guththila_stack.h"

GUTHTHILA_DECLARE (guththila_stack_t *)
guththila_stack_create (guththila_environment_t * environment)
{
    guththila_stack_t *st =
        (guththila_stack_t *) GUTHTHILA_MALLOC (environment->allocator,
                                                sizeof (guththila_stack_t));
    if (st)
    {
        st->pointer = 0;
        st->current_pos = 0;
        st->tail = NULL;
        st->current = NULL;
        return st;
    }
    else
        return NULL;
}


GUTHTHILA_DECLARE (int)
guththila_stack_push (guththila_environment_t * environment,
                      guththila_stack_t * stack, guththila_token_t * tok,
                      guththila_attribute_t * attr)
{
    if (stack)
    {
        guththila_element_t *e = 
                (guththila_element_t *) GUTHTHILA_MALLOC (environment->allocator,
                                                     sizeof (guththila_element_t));
                                                                  
        e->token = tok;
        e->attribute = attr;
        if (stack->pointer == 0)
        {
            e->prev = NULL;
            stack->tail = e;
            stack->current = stack->tail;
            stack->pointer++;
            stack->current_pos = stack->pointer;
        }
        else
        {
            e->prev = stack->tail;
            stack->tail = e;
            stack->current = stack->tail;
            stack->pointer++;
            stack->current_pos = stack->pointer;
        }
    }
    else
        guththila_token_exception ();
    return 0;
}


GUTHTHILA_DECLARE (int)
guththila_stack_size (guththila_environment_t * environment,
                      guththila_stack_t * stack)
{
    if (stack->pointer)
        return stack->pointer;
    else
        return 0;
}


GUTHTHILA_DECLARE (void)
guththila_stack_free (guththila_environment_t * environment,
                      guththila_stack_t * stack)
{
    if (stack && (stack->pointer > 0))
      {
        guththila_element_t *ele = stack->tail;
        guththila_stack_free_rec (environment, stack, ele);
        GUTHTHILA_FREE (environment->allocator, stack);
    }
    else
      GUTHTHILA_FREE (environment->allocator, stack);
}


GUTHTHILA_DECLARE (void)
guththila_stack_free_rec (guththila_environment_t * environment,
                          guththila_stack_t * stack,
                          guththila_element_t * elem)
{
    if (elem->prev == NULL)
    {
        GUTHTHILA_FREE (environment->allocator, elem);
    }
    else
    {

/*         elem = elem->prev; */
        guththila_stack_free_rec (environment, stack, elem->prev);
        GUTHTHILA_FREE (environment->allocator, elem);
    }
}


GUTHTHILA_DECLARE (guththila_element_t *)
guththila_stack_last (guththila_environment_t * environment,
                      guththila_stack_t * stack)
{
    if (stack)
        return stack->tail;
    else
        return NULL;
}


GUTHTHILA_DECLARE (guththila_element_t *)
guththila_stack_pull (guththila_environment_t * environment,
                      guththila_stack_t * stack)
{
    guththila_element_t *e = NULL;
    if (stack)
    {
        e = stack->tail;
        if (stack->pointer != 0)
        {
            if (e->prev)
            {
                stack->tail = e->prev;
                stack->current = stack->tail;
                e->prev = NULL;
                stack->pointer--;
                stack->current_pos = stack->pointer;
            }
            else
            {
                stack->tail = e;
                stack->current = stack->tail;
                stack->pointer = 0;
                stack->current_pos = stack->pointer;
            }
        }
        return e;
    }
    else
        return NULL;
}


GUTHTHILA_DECLARE (int)
guththila_stack_push_namespace (guththila_environment_t * environment,
                                guththila_stack_t * stack,
                                guththila_namespace_t * ns)
{
    if (stack)
    {
        guththila_element_t *e =
            (guththila_element_t *) GUTHTHILA_MALLOC (environment->allocator,
                                               sizeof(guththila_element_t));
        e->_namespace = ns;
        e->attribute = NULL;
        e->token = NULL;
        if (stack->pointer == 0)
        {
            e->prev = NULL;
            stack->tail = e;
            stack->current = stack->tail;
            stack->pointer++;
            stack->current_pos = stack->pointer;
        }
        else
        {
            e->prev = stack->tail;
            stack->tail = e;

            stack->current = stack->tail;
            stack->pointer++;
            stack->current_pos = stack->pointer;
        }
    }
    else
        guththila_token_exception ();
    return 0;
}


GUTHTHILA_DECLARE (guththila_element_t *)
guththila_stack_pull_current (guththila_environment_t * environment,
                              guththila_stack_t * stack)
{
    guththila_element_t *e = NULL;
    e = stack->current;
    if (stack->current_pos != 0)
    {
        if (e->prev)
        {
            stack->current = e->prev;
            stack->current_pos--;
        }
        else
        {
            stack->current = e;
            stack->current_pos = 0;
        }
    }
    return e;
}


GUTHTHILA_DECLARE (int)
guththila_stack_push_depth (guththila_environment_t * environment,
                            guththila_stack_t * stack, guththila_depth_t * d)
{
    if (stack)
    {
        guththila_element_t *e =
            (guththila_element_t *) GUTHTHILA_MALLOC (environment->allocator,
                                            sizeof (guththila_element_t));
        e->_namespace = NULL;
        e->attribute = NULL;
        e->token = NULL;
        e->depth = d;
        if (stack->pointer == 0)
        {
            e->prev = NULL;
            stack->tail = e;
            stack->current = stack->tail;
            stack->pointer++;
            stack->current_pos = stack->pointer;
        }
        else
        {
            e->prev = stack->tail;
            stack->tail = e;
            stack->current = stack->tail;
            stack->pointer++;
            stack->current_pos = stack->pointer;
        }
    }
    else
        guththila_token_exception ();
    return 0;
}


GUTHTHILA_DECLARE (void)
guththila_stack_clear (guththila_environment_t * environment,
                       guththila_stack_t * stack)
{
    guththila_element_t *e = NULL;
    e = stack->tail;
    if (e)
    {
        guththila_stack_pull (environment, stack);
        e = stack->tail;
    }
    stack->pointer = 0;
}


GUTHTHILA_DECLARE (guththila_element_t *)
guththila_stack_get (guththila_environment_t * environment,
                     guththila_stack_t * stack, int i)
{
    if (i)
    {
        if (i == 1)
            return stack->tail;
        else
        {
            guththila_element_t *e = NULL;
            int ix = stack->pointer;
            e = stack->tail;
            for (; ix > ((stack->pointer + 1) - i); ix--)
                e = e->prev;
            return e;
        }
    }
    else
        return NULL;
}
