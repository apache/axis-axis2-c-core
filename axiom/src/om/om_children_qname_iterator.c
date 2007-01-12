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

#include <axiom_children_qname_iterator.h>
#include <axiom_element.h>

static axis2_bool_t AXIS2_CALL
axiom_children_qname_iterator_qname_matches(const axis2_env_t *env,
        axis2_qname_t *element_qname,
                axis2_qname_t *qname_to_match);

struct axiom_children_qname_iterator
{
    axiom_node_t *current_child;
    axiom_node_t *last_child;
    axis2_bool_t next_called;
    axis2_bool_t remove_called;
    axis2_qname_t *given_qname;
    axis2_bool_t need_to_move_forward;
    axis2_bool_t matching_node_found;

};

AXIS2_EXTERN axiom_children_qname_iterator_t * AXIS2_CALL
axiom_children_qname_iterator_create(const axis2_env_t *env,
        axiom_node_t *current_child,
        axis2_qname_t *given_qname)
{
    axiom_children_qname_iterator_t *iterator = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, current_child, NULL);

    iterator = (axiom_children_qname_iterator_t*)AXIS2_MALLOC(
                env->allocator,
                sizeof(axiom_children_qname_iterator_t));

    if (!iterator)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    iterator->current_child = NULL;
    iterator->last_child = NULL;
    iterator->need_to_move_forward = AXIS2_TRUE;
    iterator->matching_node_found = AXIS2_FALSE;
    iterator->next_called = AXIS2_FALSE;
    iterator->remove_called = AXIS2_FALSE;
    iterator->given_qname = NULL;

    iterator->current_child = current_child;
    if (given_qname)
    {
        iterator->given_qname = AXIS2_QNAME_CLONE(given_qname, env);
        if (!(iterator->given_qname))
        {
            AXIOM_CHILDREN_QNAME_ITERATOR_FREE(iterator, env);
            return NULL;
        }
    }
    
    return iterator;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_children_qname_iterator_free(axiom_children_qname_iterator_t *iterator,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (iterator->given_qname)
    {
        AXIS2_QNAME_FREE(iterator->given_qname, env);
        iterator->given_qname = NULL;
    }
    AXIS2_FREE(env->allocator, iterator);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_children_qname_iterator_remove(axiom_children_qname_iterator_t *iterator,
        const axis2_env_t *env)
{
    axiom_node_t *last_child = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (!(iterator->next_called))
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_ITERATOR_NEXT_METHOD_HAS_NOT_YET_BEEN_CALLED, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    if (iterator->remove_called)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_ITERATOR_REMOVE_HAS_ALREADY_BEING_CALLED, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    iterator->remove_called = AXIS2_TRUE;

    if (!(iterator->last_child))
        return AXIS2_FAILURE;
    last_child = AXIOM_NODE_DETACH(iterator->last_child, env);
    if (last_child)
    {
        AXIOM_NODE_FREE_TREE(last_child, env);
        last_child = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_children_qname_iterator_has_next
(axiom_children_qname_iterator_t *iterator,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    while (iterator->need_to_move_forward)
    {
        if (iterator->current_child)
        {
            axiom_element_t *om_element = NULL;
            if (AXIOM_NODE_GET_NODE_TYPE(iterator->current_child, env) == AXIOM_ELEMENT)
            {
                om_element = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                            iterator->current_child, env);
            }

            if (om_element &&
                    axiom_children_qname_iterator_qname_matches(env,
                            AXIOM_ELEMENT_GET_QNAME(om_element, env, iterator->current_child),
                            iterator->given_qname))
            {
                iterator->matching_node_found = AXIS2_TRUE;
                iterator->need_to_move_forward = AXIS2_FALSE;

            }
            else
            {
                iterator->current_child = AXIOM_NODE_GET_NEXT_SIBLING(iterator->current_child, env);
                if (iterator->current_child)
                {
                    iterator->need_to_move_forward = AXIS2_TRUE;
                    iterator->matching_node_found = AXIS2_TRUE;
                }
                else
                {
                    iterator->need_to_move_forward = AXIS2_FALSE;
                    iterator->matching_node_found = AXIS2_FALSE;
                }
            }
        }
        else
        {
            iterator->need_to_move_forward = AXIS2_FALSE;
        }
    }
    return iterator->matching_node_found;

}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_children_qname_iterator_next(axiom_children_qname_iterator_t *iterator,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    iterator->need_to_move_forward = AXIS2_TRUE;
    iterator->matching_node_found = AXIS2_FALSE;
    iterator->next_called = AXIS2_TRUE;
    iterator->remove_called = AXIS2_FALSE;

    iterator->last_child = iterator->current_child;
    if (iterator->current_child)
    {
        iterator->current_child =
            AXIOM_NODE_GET_NEXT_SIBLING(iterator->current_child, env);
    }
    return iterator->last_child;
}

static axis2_bool_t AXIS2_CALL
axiom_children_qname_iterator_qname_matches(const axis2_env_t *env,
        axis2_qname_t *element_qname,
        axis2_qname_t *qname_to_match)
{
    int lparts_match =  0;
    int uris_match = 0;
    axis2_char_t *ele_lpart = NULL;
    axis2_char_t *match_lpart = NULL;
    axis2_char_t *ele_nsuri = NULL;
    axis2_char_t *match_nsuri = NULL;

    if (!(qname_to_match))
        return AXIS2_TRUE;
    if (qname_to_match)
    {
        match_lpart = AXIS2_QNAME_GET_LOCALPART(qname_to_match, env);
        match_nsuri = AXIS2_QNAME_GET_URI(qname_to_match, env);
    }
    if (element_qname)
    {
        ele_lpart = AXIS2_QNAME_GET_LOCALPART(element_qname, env);
        ele_nsuri = AXIS2_QNAME_GET_URI(element_qname, env);
    }

    lparts_match = (!match_lpart ||
            (AXIS2_STRCMP(match_lpart, "") == 0) ||
            (element_qname && (AXIS2_STRCMP(ele_lpart, match_lpart) == 0)));


    uris_match = (!match_nsuri || (AXIS2_STRCMP(match_nsuri, "") == 0) ||
            (element_qname && (AXIS2_STRCMP(ele_nsuri, match_nsuri) == 0)));

    return lparts_match && uris_match;
}


