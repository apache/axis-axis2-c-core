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

#include <axiom_children_with_specific_attribute_iterator.h>
#include <axiom_attribute.h>
#include <axiom_element.h>

/************** function prototypes *************************************/

axis2_status_t AXIS2_CALL
axiom_children_with_specific_attribute_iterator_free(
    axiom_children_with_specific_attribute_iterator_t *iterator,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axiom_children_with_specific_attribute_iterator_remove(
    axiom_children_with_specific_attribute_iterator_t *iterator,
    const axis2_env_t *env);


axis2_bool_t AXIS2_CALL
axiom_children_with_specific_attribute_iterator_has_next(
    axiom_children_with_specific_attribute_iterator_t *iterator,
    const axis2_env_t *env);

axiom_node_t* AXIS2_CALL
axiom_children_with_specific_attribute_iterator_next(
    axiom_children_with_specific_attribute_iterator_t *iterator,
    const axis2_env_t *env);

/*************** end function prototypes **********************************/

typedef struct axiom_children_with_specific_attribute_iterator_impl_t
{
    axiom_children_with_specific_attribute_iterator_t iterator;
    axiom_node_t *current_child;
    axiom_node_t *last_child;
    axis2_bool_t next_called;
    axis2_bool_t remove_called;
    axis2_qname_t *attr_qname;
    axis2_char_t *attr_value;
    axis2_bool_t detach;
}
axiom_children_with_specific_attribute_iterator_impl_t;

/***************** end impl struct ****************************************/

#define AXIS2_INTF_TO_IMPL(iterator) \
        ((axiom_children_with_specific_attribute_iterator_impl_t *)iterator)

/**************** end macro ***********************************************/


AXIS2_EXTERN  axiom_children_with_specific_attribute_iterator_t * AXIS2_CALL
axiom_children_with_specific_attribute_iterator_create(
    const axis2_env_t *env,
    axiom_node_t *current_child,
    axis2_qname_t *attr_qname,
    axis2_char_t *attr_value,
    axis2_bool_t detach)
{
    axiom_children_with_specific_attribute_iterator_impl_t *iterator_impl = NULL;

    AXIS2_PARAM_CHECK(env->error, current_child, NULL);
    AXIS2_PARAM_CHECK(env->error, attr_qname, NULL);
    AXIS2_PARAM_CHECK(env->error, attr_value, NULL);
    iterator_impl = (axiom_children_with_specific_attribute_iterator_impl_t *)
            AXIS2_MALLOC(env->allocator,
                    sizeof(axiom_children_with_specific_attribute_iterator_impl_t));

    if (!iterator_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    iterator_impl->current_child = NULL;
    iterator_impl->last_child    = NULL;

    iterator_impl->next_called = AXIS2_FALSE;
    iterator_impl->remove_called = AXIS2_FALSE;

    iterator_impl->attr_qname = AXIS2_QNAME_CLONE(attr_qname, env);
    iterator_impl->attr_value = attr_value;
    iterator_impl->detach = detach;


    iterator_impl->iterator.ops = NULL;
    iterator_impl->iterator.ops =
        (axiom_children_with_specific_attribute_iterator_ops_t*)
        AXIS2_MALLOC(env->allocator,
                sizeof(axiom_children_with_specific_attribute_iterator_ops_t));

    if (!(iterator_impl->iterator.ops))
    {
        AXIOM_CHILDREN_WITH_SPECIFIC_ATTRIBUTE_ITERATOR_FREE(&(iterator_impl->iterator), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    iterator_impl->current_child = current_child;
    iterator_impl->iterator.ops->free_fn =
        axiom_children_with_specific_attribute_iterator_free;
    iterator_impl->iterator.ops->remove =
        axiom_children_with_specific_attribute_iterator_remove;
    iterator_impl->iterator.ops->has_next =
        axiom_children_with_specific_attribute_iterator_has_next;
    iterator_impl->iterator.ops->next =
        axiom_children_with_specific_attribute_iterator_next;
    return &(iterator_impl->iterator);
}


axis2_status_t AXIS2_CALL
axiom_children_with_specific_attribute_iterator_free(
    axiom_children_with_specific_attribute_iterator_t *iterator,
    const axis2_env_t *env)
{
    axiom_children_with_specific_attribute_iterator_impl_t *qname_iter_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    qname_iter_impl = AXIS2_INTF_TO_IMPL(iterator);
    if (qname_iter_impl->attr_qname)
    {
        AXIS2_QNAME_FREE(qname_iter_impl->attr_qname, env);
        qname_iter_impl->attr_qname = NULL;
    }
    if (iterator->ops)
    {
        AXIS2_FREE(env->allocator, iterator->ops);
        iterator->ops = NULL;
    }
    AXIS2_FREE(env->allocator, qname_iter_impl);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axiom_children_with_specific_attribute_iterator_remove(
    axiom_children_with_specific_attribute_iterator_t *iterator,
    const axis2_env_t *env)
{
    axiom_children_with_specific_attribute_iterator_impl_t *iterator_impl = NULL;
    axiom_node_t *last_child = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);

    if (!(iterator_impl->next_called))
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_ITERATOR_NEXT_METHOD_HAS_NOT_YET_BEEN_CALLED, AXIS2_FAILURE);
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
    last_child = AXIOM_NODE_DETACH(iterator_impl->last_child, env);
    if (last_child)
    {
        AXIOM_NODE_FREE_TREE(last_child, env);
        last_child = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axiom_children_with_specific_attribute_iterator_has_next(
    axiom_children_with_specific_attribute_iterator_t *iterator,
    const axis2_env_t *env)
{
    axis2_bool_t matching_node_found = AXIS2_FALSE;
    axis2_bool_t need_to_move_forward = AXIS2_TRUE;


    axiom_children_with_specific_attribute_iterator_impl_t *iterator_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);
    if (!(iterator_impl->current_child))
    {
        return AXIS2_FALSE;
    }
    while (need_to_move_forward)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(iterator_impl->current_child, env)
                == AXIOM_ELEMENT)
        {
            axiom_attribute_t *om_attr = NULL;
            axiom_element_t *om_ele = NULL;
            om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                        iterator_impl->current_child, env);
            om_attr = AXIOM_ELEMENT_GET_ATTRIBUTE(om_ele, env,
                    iterator_impl->attr_qname);
            break;
            if (om_attr &&
                    (AXIS2_STRCMP(axiom_attribute_get_value(om_attr, env),
                            iterator_impl->attr_value) == 0))
            {
                matching_node_found = AXIS2_TRUE;
                need_to_move_forward = AXIS2_FALSE;
            }
            else
            {
                iterator_impl->current_child =
                    AXIOM_NODE_GET_NEXT_SIBLING(
                        iterator_impl->current_child, env);
                need_to_move_forward = (iterator_impl->current_child != NULL);

            }
        }
        else
        {

            iterator_impl->current_child =
                AXIOM_NODE_GET_NEXT_SIBLING(
                    iterator_impl->current_child, env);
            need_to_move_forward = (iterator_impl->current_child != NULL);
        }

    }
    return matching_node_found;
}

axiom_node_t* AXIS2_CALL
axiom_children_with_specific_attribute_iterator_next(
    axiom_children_with_specific_attribute_iterator_t *iterator,
    const axis2_env_t *env)
{
    axiom_children_with_specific_attribute_iterator_impl_t *iterator_impl = NULL;
    axiom_node_t *last_child = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    iterator_impl = AXIS2_INTF_TO_IMPL(iterator);

    iterator_impl->next_called = AXIS2_TRUE;
    iterator_impl->remove_called = AXIS2_FALSE;
    iterator_impl->last_child = iterator_impl->current_child;
    iterator_impl->current_child = AXIOM_NODE_GET_NEXT_SIBLING(
                iterator_impl->current_child, env);
    if (iterator_impl->last_child && iterator_impl->detach
            && (AXIOM_NODE_GET_PARENT(iterator_impl->last_child, env)))
    {
        last_child = AXIOM_NODE_DETACH(iterator_impl->last_child, env);
        if (last_child)
            AXIOM_NODE_FREE_TREE(last_child, env);
    }
    return iterator_impl->last_child;
}
