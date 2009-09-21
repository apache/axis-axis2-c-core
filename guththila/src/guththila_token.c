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

#include <guththila_token.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TOK_LIST_FREE(tok_list) \
    (if (tok_list) { AXIS2_FREE(tok_list)} )

#define TOK_LIST_SIZE(tok_list) (tok_list->size)

int GUTHTHILA_CALL
guththila_tok_list_grow(
    guththila_tok_list_t * tok_list,
    const axutil_env_t * env)
{
    int i = 0;

    if(tok_list->cur_list < tok_list->no_list - 1)
    {
        int cur = ++tok_list->cur_list;
        int cur_cap = tok_list->capacity[cur - 1] * 2;
        guththila_token_t *list = (guththila_token_t *)AXIS2_MALLOC(env->allocator,
            sizeof(guththila_token_t) * cur_cap);
        for(i = 0; i < cur_cap; ++i)
        {
            guththila_stack_push(&tok_list->fr_stack, &list[i], env);
        }
        tok_list->capacity[cur] = cur_cap;
        tok_list->list[cur] = list;
        return GUTHTHILA_SUCCESS;
    }
    else
    {
        guththila_token_t ** list = NULL;
        int *capacity = NULL;
        list = (guththila_token_t **)AXIS2_MALLOC(env->allocator, sizeof(guththila_token_t *)
            * tok_list->no_list * 2);
        capacity = (int *)AXIS2_MALLOC(env->allocator, sizeof(int) * tok_list->no_list * 2);
        if(list)
        {
            int cur_list = tok_list->cur_list;
            for(i = 0; i <= cur_list; ++i)
            {
                list[i] = tok_list->list[i];
                capacity[i] = tok_list->capacity[i];
            }
            tok_list->no_list = tok_list->no_list * 2;
            AXIS2_FREE(env->allocator, tok_list->list);
            tok_list->list = list;
            AXIS2_FREE(env->allocator, tok_list->capacity);
            tok_list->capacity = capacity;
            guththila_tok_list_grow(tok_list, env);
        }
    }
    return GUTHTHILA_FAILURE;
}

int GUTHTHILA_CALL
guththila_tok_list_init(
    guththila_tok_list_t * tok_list,
    const axutil_env_t * env)
{
    int i = 0;
    tok_list->list = (guththila_token_t **)AXIS2_MALLOC(env->allocator, sizeof(guththila_token_t *)
        * GUTHTHILA_TOK_DEF_LIST_SIZE);
    if(tok_list->list && guththila_stack_init(&tok_list->fr_stack, env))
    {
        tok_list->capacity = (int *)AXIS2_MALLOC(env->allocator, sizeof(int)
            * GUTHTHILA_TOK_DEF_LIST_SIZE);
        if(tok_list->capacity)
        {
            tok_list->no_list = GUTHTHILA_TOK_DEF_LIST_SIZE;
            tok_list->list[0] = (guththila_token_t *)AXIS2_MALLOC(env->allocator,
                sizeof(guththila_token_t) * GUTHTHILA_TOK_DEF_SIZE);
            for(i = 0; i < GUTHTHILA_TOK_DEF_SIZE; i++)
            {
                guththila_stack_push(&tok_list->fr_stack, &tok_list->list[0][i], env);
            }
            tok_list->capacity[0] = GUTHTHILA_TOK_DEF_SIZE;
            tok_list->cur_list = 0;
            tok_list->no_list = GUTHTHILA_TOK_DEF_LIST_SIZE;
            return GUTHTHILA_SUCCESS;
        }
    }
    return GUTHTHILA_FAILURE;
}

void GUTHTHILA_CALL
guththila_tok_list_free(
    guththila_tok_list_t * tok_list,
    const axutil_env_t * env)
{
    int i = 0;
    guththila_stack_un_init(&tok_list->fr_stack, env);
    for(; i <= tok_list->cur_list; i++)
    {
        AXIS2_FREE(env->allocator, tok_list->list[i]);
    }
    AXIS2_FREE(env->allocator, tok_list->list);
    AXIS2_FREE(env->allocator, tok_list->capacity);
    AXIS2_FREE(env->allocator, tok_list);
}

void GUTHTHILA_CALL
guththila_tok_list_free_data(
    guththila_tok_list_t * tok_list,
    const axutil_env_t * env)
{
    int i = 0;
    guththila_stack_un_init(&tok_list->fr_stack, env);
    for(; i <= tok_list->cur_list; i++)
    {
        AXIS2_FREE(env->allocator, tok_list->list[i]);
    }
    AXIS2_FREE(env->allocator, tok_list->capacity);
    AXIS2_FREE(env->allocator, tok_list->list);
}

guththila_token_t *GUTHTHILA_CALL
guththila_tok_list_get_token(
    guththila_tok_list_t * tok_list,
    const axutil_env_t * env)
{
    if(tok_list->fr_stack.top > 0 || guththila_tok_list_grow(tok_list, env))
    {
        return guththila_stack_pop(&tok_list->fr_stack, env);
    }
    return NULL;
}

int GUTHTHILA_CALL
guththila_tok_list_release_token(
    guththila_tok_list_t * tok_list,
    guththila_token_t * token,
    const axutil_env_t * env)
{
    return guththila_stack_push(&tok_list->fr_stack, token, env);
}

int GUTHTHILA_CALL
guththila_tok_str_cmp(
    guththila_token_t * tok,
    guththila_char_t *str,
    size_t str_len,
    const axutil_env_t * env)
{
    unsigned int i = 0;
    if(tok->size != str_len)
        return -1;
    for(; i < tok->size; i++)
    {
        if(tok->start[i] != str[i])
        {
            return -1;
        }
    }
    return 0;
}

int GUTHTHILA_CALL
guththila_tok_tok_cmp(
    guththila_token_t * tok1,
    guththila_token_t * tok2,
    const axutil_env_t * env)
{
    unsigned int i = 0;

    if(tok1 && tok2)
    {
        if(tok1->size != tok2->size)
            return -1;
        for(; i < tok1->size; i++)
        {
            if(tok1->start[i] != tok2->start[i])
            {
                return -1;
            }
        }
        return 0;
    }
    return -1;
}

void GUTHTHILA_CALL
guththila_set_token(
    guththila_token_t* tok,
    guththila_char_t* start,
    short type,
    int size,
    int _start,
    int last,
    int ref,
    const axutil_env_t* env)
{
    if(start)
    {
        tok->start = start;
        tok->type = type;
        tok->_start = _start;
        tok->size = size;
        tok->last = last;
        tok->ref = ref;
    }
}
