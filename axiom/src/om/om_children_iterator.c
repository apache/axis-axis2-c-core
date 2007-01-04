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

#include <axiom_children_iterator.h>

/************** function prototypes *************************************/

/*************** end function prototypes **********************************/

typedef struct axiom_children_iterator_impl_t
{
    axiom_children_iterator_t iterator;
    axiom_node_t *first_child;
    axiom_node_t *current_child;
    axiom_node_t *last_child;
    axis2_bool_t next_called;
    axis2_bool_t remove_called;
}
axiom_children_iterator_impl_t;

static const axiom_children_iterator_ops_t axiom_children_iterator_ops_var = {
    0
};

/***************** end impl struct ****************************************/

#define AXIS2_INTF_TO_IMPL(iterator) \
        ((axiom_children_iterator_impl_t *)iterator)

/**************** end macro ***********************************************/



AXIS2_EXTERN  axiom_children_iterator_t * AXIS2_CALL
axiom_children_iterator_create(const axis2_env_t *env,
        axiom_node_t *current_child)
{
    axiom_children_iterator_impl_t *iterator_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    iterator_impl = (axiom_children_iterator_impl_t *)AXIS2_MALLOC(
                env->allocator,
                sizeof(axiom_children_iterator_impl_t));

    if (!iterator_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    iterator_impl->current_child = NULL;
    iterator_impl->last_child    = NULL;
    iterator_impl->first_child    = NULL;

    iterator_impl->next_called = AXIS2_FALSE;
    iterator_impl->remove_called = AXIS2_FALSE;

    iterator_impl->first_child = current_child;
    iterator_impl->current_child = current_child;

    iterator_impl->iterator.ops = &axiom_children_iterator_ops_var;

    return &(iterator_impl->iterator);
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_children_iterator_free(axiom_children_iterator_t *iterator,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(iterator));
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_children_iterator_remove(axiom_children_iterator_t *iterator,
        const axis2_env_t *env)
{
    axiom_children_iterator_impl_t *iterator_impl = NULL;
    axiom_node_t *om_node = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);

    if (!(iterator_impl->next_called))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_ITERATOR_NEXT_METHOD_HAS_NOT_YET_BEEN_CALLED, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    if (iterator_impl->remove_called)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_ITERATOR_REMOVE_HAS_ALREADY_BEING_CALLED, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    iterator_impl->remove_called = AXIS2_TRUE;

    if (!(iterator_impl->last_child))
        return AXIS2_FAILURE;
    om_node = AXIOM_NODE_DETACH(iterator_impl->last_child, env);
    if (om_node)
    {
        AXIOM_NODE_FREE_TREE(om_node, env);
        om_node = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_children_iterator_has_next(axiom_children_iterator_t *iterator,
        const axis2_env_t *env)
{
    axiom_children_iterator_impl_t *iterator_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);

    return (iterator_impl->current_child) ? AXIS2_TRUE : AXIS2_FALSE;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_children_iterator_next(axiom_children_iterator_t *iterator,
        const axis2_env_t *env)
{
    axiom_children_iterator_impl_t *iterator_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);

    iterator_impl->next_called = AXIS2_TRUE;
    iterator_impl->remove_called = AXIS2_FALSE;
    if (iterator_impl->current_child)
    {
        iterator_impl->last_child = iterator_impl->current_child;
        iterator_impl->current_child = AXIOM_NODE_GET_NEXT_SIBLING(
                    iterator_impl->current_child, env);
        return iterator_impl->last_child;
    }
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_children_iterator_reset(axiom_children_iterator_t *iterator,
        const axis2_env_t *env)
{
    axiom_children_iterator_impl_t *iterator_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);

    iterator_impl->current_child = iterator_impl->first_child;
    return AXIS2_SUCCESS;
}

