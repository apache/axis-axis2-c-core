
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

#include <axiom_xpath.h>
#include "xpath_internals.h"
#include "xpath_internals_engine.h"
#include "xpath_internals_iterators.h"


/* child */
int axiom_xpath_child_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes_tot = 0;
    int n_nodes;
    axiom_xpath_operation_t * node_test_op;
    axiom_node_t *cur = NULL;
    axiom_node_t *context_node = NULL;
    /* For streaming */
    axiom_node_t *prev = NULL;
    /*axiom_xpath_operation_t * next_operation = NULL;*/

    AXIOM_XPATH_ITERATOR_INITIALIZE;
    cur = axiom_node_get_first_child(context->node, context->env);

    while (cur != NULL)
    {
        n_nodes = 0;
        context->node = cur;
        prev = cur;
        cur = axiom_node_get_next_sibling(cur, context->env);
        if (axiom_xpath_node_test_match(
                    context, (axiom_xpath_node_test_t *)node_test_op->par1))
        {
            n_nodes =
                axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
            n_nodes_tot += n_nodes;
        }
/*
        if (context->streaming)
        {
            if (n_nodes)
            {
                next_operation = AXIOM_XPATH_OPR_GET(op_next);
            }

            if (!n_nodes
                    || next_operation->opr != AXIOM_XPATH_OPERATION_RESULT)
            {
                axiom_node_detach(prev, context->env);
                axiom_node_free_tree(prev, context->env);
            }
        }
*/
    }

    /* Change the context node back to what it was */
    context->node = context_node;

    return n_nodes_tot;
}

/* descendant */
int axiom_xpath_descendant_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_node_t *child = NULL;
    axiom_node_t *context_node = NULL;
    axutil_stack_t *stack;

    AXIOM_XPATH_ITERATOR_INITIALIZE;

    /* Setting up the stack */
    stack = axutil_stack_create(context->env);

    child = axiom_node_get_first_child(context->node, context->env);
    while (child)
    {
        axutil_stack_push(stack, context->env, child);
        child = axiom_node_get_first_child(child, context->env);
    }

    /* Processing nodes */
    while (axutil_stack_size(stack, context->env) > 0)
    {
        child = (axiom_node_t *)axutil_stack_pop(stack, context->env);

        context->node = child;
        if (axiom_xpath_node_test_match(
                    context, (axiom_xpath_node_test_t *)node_test_op->par1))
        {
            n_nodes +=
                axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
        }

        child = axiom_node_get_next_sibling(child, context->env);
        while (child)
        {
            axutil_stack_push(stack, context->env, child);
            child = axiom_node_get_first_child(child, context->env);
        }
    }

    context->node = context_node;

    return n_nodes;
}

/* parent */
int axiom_xpath_parent_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_node_t *parent = NULL;
    axiom_node_t *context_node = NULL;

    AXIOM_XPATH_ITERATOR_INITIALIZE;

    parent = axiom_node_get_parent(context->node, context->env);

    if (parent != NULL)
    {
        context->node = parent;
        if (axiom_xpath_node_test_match(
                    context, (axiom_xpath_node_test_t *)node_test_op->par1))
        {
            n_nodes =
                axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
        }
    }

    /* Change the context node back to what it was */
    context->node = context_node;

    return n_nodes;
}

/* ancestor axis */
int axiom_xpath_ancestor_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_node_t *cur = NULL;
    axiom_node_t *context_node = NULL;

    AXIOM_XPATH_ITERATOR_INITIALIZE;
    cur = axiom_node_get_parent(context->node, context->env);

    while (cur != NULL)
    {
        context->node = cur;
        if (axiom_xpath_node_test_match(
                    context, (axiom_xpath_node_test_t *)node_test_op->par1))
        {
            n_nodes +=
                axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
        }

        cur = axiom_node_get_parent(cur, context->env);
    }

    /* Change the context node back to what it was */
    context->node = context_node;

    return n_nodes;
}

/* following-sibling axis */
int axiom_xpath_following_sibling_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_node_t *cur = NULL;
    axiom_node_t *context_node = NULL;

    AXIOM_XPATH_ITERATOR_INITIALIZE;
    cur = axiom_node_get_next_sibling(context->node, context->env);

    while (cur != NULL)
    {
        context->node = cur;
        if (axiom_xpath_node_test_match(
                    context, (axiom_xpath_node_test_t *)node_test_op->par1))
        {
            n_nodes +=
                axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
        }

        cur = axiom_node_get_next_sibling(cur, context->env);
    }

    /* Change the context node back to what it was */
    context->node = context_node;

    return n_nodes;
}

/* preceding-sibling axis */
int axiom_xpath_preceding_sibling_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_node_t *cur = NULL;
    axiom_node_t *context_node = NULL;

    AXIOM_XPATH_ITERATOR_INITIALIZE;
    cur = axiom_node_get_previous_sibling(context->node, context->env);

    while (cur != NULL)
    {
        context->node = cur;
        if (axiom_xpath_node_test_match(
                    context, (axiom_xpath_node_test_t *)node_test_op->par1))
        {
            n_nodes +=
                axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
        }

        cur = axiom_node_get_previous_sibling(cur, context->env);
    }

    /* Change the context node back to what it was */
    context->node = context_node;

    return n_nodes;
}

/* following  */
int axiom_xpath_following_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_node_t *child = NULL, *parent = NULL;
    axiom_node_t *context_node = NULL;
    axutil_stack_t *stack;

    AXIOM_XPATH_ITERATOR_INITIALIZE;

    /* Setting up the stack */
    stack = axutil_stack_create(context->env);
    axutil_stack_push(stack, context->env, context->node);

    parent = context->node;

    while (parent)
    {
        axutil_stack_push(stack, context->env, parent);

        while (axutil_stack_size(stack, context->env) > 0)
        {
            child = (axiom_node_t *)axutil_stack_pop(stack, context->env);

            child = axiom_node_get_next_sibling(child, context->env);

            while (child)
            {
                context->node = child;
                if (axiom_xpath_node_test_match(
                            context, (axiom_xpath_node_test_t *)node_test_op->par1))
                {
                    n_nodes +=
                        axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
                }

                axutil_stack_push(stack, context->env, child);
                child = axiom_node_get_first_child(child, context->env);
            }
        }
        parent = axiom_node_get_parent(parent, context->env);
    }

    /* Change the context node back to what it was */
    context->node = context_node;

    return n_nodes;
}

/* preceding */
int axiom_xpath_preceding_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_node_t *child = NULL, *parent = NULL;
    axiom_node_t *context_node = NULL;
    axutil_stack_t *stack;

    AXIOM_XPATH_ITERATOR_INITIALIZE;

    /* Setting up the stack */
    stack = axutil_stack_create(context->env);

    parent = context->node;

    while (parent)
    {
        axutil_stack_push(stack, context->env, parent);

        while (axutil_stack_size(stack, context->env) > 0)
        {
            child = (axiom_node_t *)axutil_stack_pop(stack, context->env);

            child = axiom_node_get_previous_sibling(child, context->env);

            while (child)
            {
                context->node = child;
                if (axiom_xpath_node_test_match(
                            context, (axiom_xpath_node_test_t *)node_test_op->par1))
                {
                    n_nodes +=
                        axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
                }

                axutil_stack_push(stack, context->env, child);

                child = axiom_node_get_last_child(child, context->env);
            }
        }
        parent = axiom_node_get_parent(parent, context->env);
    }

    /* Change the context node back to what it was */
    context->node = context_node;

    return n_nodes;
}

/* attribute axis */
int axiom_xpath_attribute_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_types_t type;
    axiom_node_t *context_node = NULL;
    axiom_element_t *element;
    axutil_hash_t *ht;
    axutil_hash_index_t *hi;

    /*  void *key;
     *  axis2_ssize_t klen;
     */
    void *attr;

    AXIOM_XPATH_ITERATOR_INITIALIZE;

    type = axiom_node_get_node_type(context_node, context->env);

    if (type != AXIOM_ELEMENT)
    {
        return 0;
    }

    element = axiom_node_get_data_element(context_node, context->env);

    context->node = NULL;

    ht = axiom_element_get_all_attributes(element, context->env);

    if (ht)
    {
        for (hi = axutil_hash_first(ht, context->env);
                hi;
                hi = axutil_hash_next(context->env, hi))
        {
            attr = &context->attribute;
            axutil_hash_this(hi, NULL, NULL, attr);

            if (axiom_xpath_node_test_match(
                        context, (axiom_xpath_node_test_t *)node_test_op->par1))
            {
                n_nodes +=
                    axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
            }
        }
    }

    context->node = context_node;
    context->attribute = NULL;

    return n_nodes;
}

/* namespace axis */
int axiom_xpath_namespace_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_types_t type;
    axiom_node_t *context_node = NULL;
    axiom_element_t *element;
    axutil_hash_t *ht;
    axutil_hash_index_t *hi;

    /*  void *key;
     *  axis2_ssize_t klen;
     */

    void *ns;

    AXIOM_XPATH_ITERATOR_INITIALIZE;

    type = axiom_node_get_node_type(context_node, context->env);

    if (type != AXIOM_ELEMENT)
    {
        return 0;
    }

    element = axiom_node_get_data_element(context_node, context->env);

    context->node = NULL;

    ht = axiom_element_get_namespaces(element, context->env);

    if (ht)
    {
        for (hi = axutil_hash_first(ht, context->env);
                hi;
                hi = axutil_hash_next(context->env, hi))
        {
            ns = &context->ns;
            axutil_hash_this(hi, NULL, NULL, ns);

            if (axiom_xpath_node_test_match(
                        context, (axiom_xpath_node_test_t *)node_test_op->par1))
            {
                n_nodes +=
                    axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
            }
        }
    }

    context->node = context_node;
    context->ns = NULL;

    return n_nodes;
}

/* self axis */
int axiom_xpath_self_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_node_t *context_node = NULL;

    AXIOM_XPATH_ITERATOR_INITIALIZE;
    if (axiom_xpath_node_test_match(
                context, (axiom_xpath_node_test_t *)node_test_op->par1))
    {
        n_nodes +=
            axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
    }

    context->node = context_node;

    return n_nodes;
}

/* descendant-or-self axis */
int axiom_xpath_descendant_self_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_node_t *child = NULL;
    axiom_node_t *context_node = NULL;
    axutil_stack_t *stack;

    AXIOM_XPATH_ITERATOR_INITIALIZE;

    if (axiom_xpath_node_test_match(
                context, (axiom_xpath_node_test_t *)node_test_op->par1))
    {
        n_nodes +=
            axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
    }

    /* Setting up the stack */
    stack = axutil_stack_create(context->env);

    child = axiom_node_get_first_child(context->node, context->env);
    while (child)
    {
        axutil_stack_push(stack, context->env, child);
        child = axiom_node_get_first_child(child, context->env);
    }

    /* Processing nodes */
    while (axutil_stack_size(stack, context->env) > 0)
    {
        child = (axiom_node_t *)axutil_stack_pop(stack, context->env);

        context->node = child;
        if (axiom_xpath_node_test_match(
                    context, (axiom_xpath_node_test_t *)node_test_op->par1))
        {
            n_nodes +=
                axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
        }

        child = axiom_node_get_next_sibling(child, context->env);
        while (child)
        {
            axutil_stack_push(stack, context->env, child);
            child = axiom_node_get_first_child(child, context->env);
        }
    }

    /* Change the context node back to what it was */
    context->node = context_node;

    return n_nodes;
}

/* ancestor-or-self axis */
int axiom_xpath_ancestor_self_iterator(
    axiom_xpath_context_t *context,
    int op_node_test,
    int op_next,
    int op_predicate)
{
    int n_nodes = 0;
    axiom_xpath_operation_t * node_test_op;
    axiom_node_t *parent = NULL;
    axiom_node_t *context_node = NULL;

    AXIOM_XPATH_ITERATOR_INITIALIZE;

    if (axiom_xpath_node_test_match(
                context, (axiom_xpath_node_test_t *)node_test_op->par1))
    {
        n_nodes +=
            axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
    }

    parent = axiom_node_get_parent(context->node, context->env);

    while (parent != NULL)
    {
        context->node = parent;
        if (axiom_xpath_node_test_match(
                    context, (axiom_xpath_node_test_t *)node_test_op->par1))
        {
            n_nodes +=
                axiom_xpath_evaluate_predicate(context, op_next, op_predicate);
        }

        parent = axiom_node_get_parent(parent, context->env);
    }

    /* Change the context node back to what it was */
    context->node = context_node;

    return n_nodes;
}
