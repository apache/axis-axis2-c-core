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

#include <axiom_navigator.h>

/*************************** function prototypes ******************************************/
static void
axiom_navigator_update_next_node(axiom_navigator_t *om_navigator,
        const axis2_env_t *env);
/************************************************************************************/

typedef struct axiom_navigator_impl
{
    axiom_navigator_t navigator;

    axiom_node_t *node;

    axis2_bool_t visited;

    axiom_node_t *next;

    axiom_node_t *root;

    axis2_bool_t backtracked;

    axis2_bool_t end;

    axis2_bool_t start;

}
axiom_navigator_impl_t;

static const axiom_navigator_ops_t axiom_navigator_ops_var = {
};

/****************************** Macro  ***************************************/

#define AXIS2_INTF_TO_IMPL(navigator) ((axiom_navigator_impl_t *)navigator)

/*****************************************************************************/

AXIS2_EXTERN axiom_navigator_t * AXIS2_CALL
axiom_navigator_create(const axis2_env_t *env,
        axiom_node_t *om_node)
{
    axiom_navigator_impl_t *navigator_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, om_node, NULL);

    navigator_impl = (axiom_navigator_impl_t *)
            AXIS2_MALLOC(env->allocator, sizeof(axiom_navigator_impl_t));
    if (!navigator_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    navigator_impl->node = NULL;
    navigator_impl->visited = AXIS2_FALSE;
    navigator_impl->next = NULL;
    navigator_impl->root = NULL;
    navigator_impl->end = AXIS2_FALSE;
    navigator_impl->start = AXIS2_TRUE;
    navigator_impl->backtracked = AXIS2_FALSE;

    navigator_impl->next = om_node;
    navigator_impl->root = om_node;

    navigator_impl->navigator.ops = &axiom_navigator_ops_var;

    return &(navigator_impl->navigator);
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_navigator_free(axiom_navigator_t *om_navigator,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env , AXIS2_FAILURE);

    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(om_navigator));
    om_navigator = NULL;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_navigator_is_navigable(axiom_navigator_t *om_navigator,
        const axis2_env_t *env)
{
    axiom_navigator_impl_t *navigator_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    navigator_impl = AXIS2_INTF_TO_IMPL(om_navigator);
    if (AXIS2_TRUE == navigator_impl->end)
        return AXIS2_FALSE;
    else
    {
        if (navigator_impl->next)
            return AXIS2_TRUE;
    }
    return AXIS2_FALSE;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_navigator_is_completed(axiom_navigator_t *om_navigator,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(om_navigator)->end;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_navigator_visited(axiom_navigator_t *om_navigator,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(om_navigator)->visited;
}


AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_navigator_next(axiom_navigator_t *om_navigator,
        const axis2_env_t *env)
{
    axiom_navigator_impl_t *navigator_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    navigator_impl = AXIS2_INTF_TO_IMPL(om_navigator);

    if (NULL == navigator_impl->next)
        return NULL;

    navigator_impl->node = navigator_impl->next;
    navigator_impl->visited = navigator_impl->backtracked;
    navigator_impl->backtracked = AXIS2_FALSE;

    axiom_navigator_update_next_node(om_navigator, env);


    /** set the starting and ending flags */
    if (navigator_impl->root == navigator_impl->next)
    {
        if (!(navigator_impl->start))
        {
            navigator_impl->end = AXIS2_TRUE;
        }
        else
        {
            navigator_impl->start = AXIS2_FALSE;
        }
    }
    return navigator_impl->node;
}


/** this method encapsulate searching logic */
static void
axiom_navigator_update_next_node(axiom_navigator_t *om_navigator,
        const axis2_env_t *env)
{
    axiom_navigator_impl_t *navigator_impl = NULL;
    if (!om_navigator)
        return;

    navigator_impl = AXIS2_INTF_TO_IMPL(om_navigator);

    if (!navigator_impl->next)
        return;

    if ((AXIOM_ELEMENT == AXIOM_NODE_GET_NODE_TYPE(navigator_impl->next, env)) &&
            !(navigator_impl->visited))
    {
        if (AXIOM_NODE_GET_FIRST_CHILD(navigator_impl->next, env))
        {
            navigator_impl->next =    AXIOM_NODE_GET_FIRST_CHILD(navigator_impl->next, env);
        }
        else if (AXIS2_TRUE == AXIOM_NODE_IS_COMPLETE(navigator_impl->next, env))
        {
            navigator_impl->backtracked = AXIS2_TRUE;
        }
        else
        {
            navigator_impl->next = NULL;
        }
    }
    else
    {
        axiom_node_t  *parent = NULL;
        axiom_node_t *next_sibling = NULL;

        next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(navigator_impl->next, env);

        parent = AXIOM_NODE_GET_PARENT(navigator_impl->next, env);

        if (next_sibling)
        {
            navigator_impl->next = next_sibling;
        }
        else if ((parent) && AXIOM_NODE_IS_COMPLETE(parent, env))
        {
            navigator_impl->next = parent;
            navigator_impl->backtracked = AXIS2_TRUE;
        }
        else
        {
            navigator_impl->next = NULL;
        }
    }
}
