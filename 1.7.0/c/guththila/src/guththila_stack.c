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
#include <guththila_stack.h>

int GUTHTHILA_CALL
guththila_stack_init(
    guththila_stack_t * stack,
    const axutil_env_t * env)
{
    stack->top = 0;
    stack->data = (void **)AXIS2_MALLOC(env->allocator, sizeof(void **) * GUTHTHILA_STACK_DEFAULT);
    if(!stack->data)
    {
        return GUTHTHILA_FAILURE;
    }
    else
    {
        stack->max = GUTHTHILA_STACK_DEFAULT;
        return GUTHTHILA_SUCCESS;
    }
}

void GUTHTHILA_CALL
guththila_stack_free(
    guththila_stack_t * stack,
    const axutil_env_t * env)
{
    if(stack->data)
        AXIS2_FREE(env->allocator, stack->data);
    AXIS2_FREE(env->allocator, stack);
}

void GUTHTHILA_CALL
guththila_stack_un_init(
    guththila_stack_t * stack,
    const axutil_env_t * env)
{
    if(stack->data)
        AXIS2_FREE(env->allocator, stack->data);
}

void *GUTHTHILA_CALL
guththila_stack_pop(
    guththila_stack_t * stack,
    const axutil_env_t * env)
{
    if(stack->top > 0)
    {
        return stack->data[--(stack->top)];
    }
    return NULL;
}

int GUTHTHILA_CALL
guththila_stack_push(
    guththila_stack_t * stack,
    void *data,
    const axutil_env_t * env)
{
    int top = stack->top++;
    if(top >= stack->max)
    {
        void **temp = NULL;
        int i = 0;
        temp = (void **)AXIS2_MALLOC(env->allocator, sizeof(void **) * (stack->max *= 2));
        for(i = 0; i < top; ++i)
        {
            temp[i] = stack->data[i];
        }
        AXIS2_FREE(env->allocator, stack->data);
        stack->data = temp;
    }
    stack->data[top] = data;
    return top;
}

void *GUTHTHILA_CALL
guththila_stack_peek(
    guththila_stack_t * stack,
    const axutil_env_t * env)
{
    if(stack->top > 0)
    {
        return stack->data[stack->top - 1];
    }
    else
    {
        return NULL;
    }
}

int GUTHTHILA_CALL
guththila_stack_del_top(
    guththila_stack_t * stack,
    const axutil_env_t * env)
{
    if(stack->top > 0)
    {
        AXIS2_FREE(env->allocator, stack->data[stack->top]);
        return GUTHTHILA_SUCCESS;
    }
    return GUTHTHILA_FAILURE;
}

int GUTHTHILA_CALL
guththila_stack_is_empty(
    guththila_stack_t * stack,
    const axutil_env_t * env)
{
    return stack->top == 0 ? 1 : 0;
}

void *GUTHTHILA_CALL
guththila_stack_get_by_index(
    guththila_stack_t * stack,
    int index,
    const axutil_env_t * env)
{
    return index < stack->top ? stack->data[index] : NULL;
}

