
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

/* Evaluates the expath expression */
axiom_xpath_result_t * axiom_xpath_run(axiom_xpath_context_t *context)
{
    axiom_xpath_result_t* res;

    /* Initialize result ret */
    res = AXIS2_MALLOC(context->env->allocator,
            sizeof(axiom_xpath_result_t));

    res->flag = 0;

    res->nodes = axutil_array_list_create(context->env, 0);

    context->stack = axutil_stack_create(context->env);

    /* Expression is empty */
    if (context->expr->start == AXIOM_XPATH_PARSE_END)
    {
        return res;
    }

    axiom_xpath_evaluate_operation(context, context->expr->start);

    /* Add nodes to the result set from the stack */
    while (axutil_stack_size(context->stack, context->env) > 0)
    {
        axutil_array_list_add(
            res->nodes, context->env,
            axutil_stack_pop(context->stack, context->env));
    }

    return res;
}

/* Casting functions; these make use of the casting functions defined in xpath.h */
void axiom_xpath_cast_boolean(
    axiom_xpath_result_node_t *node,
    axiom_xpath_context_t *context)
{
    axis2_bool_t v = axiom_xpath_cast_node_to_boolean(context->env, node);

    AXIOM_XPATH_CAST_SET_VALUE(axis2_bool_t, v);

    node->type = AXIOM_XPATH_TYPE_BOOLEAN;
}

void axiom_xpath_cast_number(
    axiom_xpath_result_node_t *node,
    axiom_xpath_context_t *context)
{
    double v = axiom_xpath_cast_node_to_number(context->env, node);

    AXIOM_XPATH_CAST_SET_VALUE(double, v);

    node->type = AXIOM_XPATH_TYPE_NUMBER;
}

void axiom_xpath_cast_string(
    axiom_xpath_result_node_t *node,
    axiom_xpath_context_t *context)
{
    node->value = axiom_xpath_cast_node_to_string(context->env, node);

    node->type = AXIOM_XPATH_TYPE_TEXT;
}

/* Evaluate whether two results are equal

   If either node is a boolean other is casted to a boolean;
   Otherwise, if either node is a number other is casted to a number;
   Otherwise, both nodes are casted to strings.*/
axis2_bool_t axiom_xpath_compare_equal(
    axiom_xpath_result_node_t *node1,
    axiom_xpath_result_node_t *node2,
    axiom_xpath_context_t *context)
{
    if (node1->type == AXIOM_XPATH_TYPE_BOOLEAN
            || node2->type == AXIOM_XPATH_TYPE_BOOLEAN)
    {
        axiom_xpath_cast_boolean(node1, context);
        axiom_xpath_cast_boolean(node2, context);

        if (*(axis2_bool_t*)(node1->value) == *(axis2_bool_t*)(node2->value))
        {
            return AXIS2_TRUE;
        }
        else
        {
            return AXIS2_FALSE;
        }
    }

    if (node1->type == AXIOM_XPATH_TYPE_NUMBER
            || node2->type == AXIOM_XPATH_TYPE_NUMBER)
    {
        axiom_xpath_cast_number(node1, context);
        axiom_xpath_cast_number(node2, context);

        if (*(double *)(node1->value) == *(double *)(node2->value))
        {
            return AXIS2_TRUE;
        }
        else
        {
            return AXIS2_FALSE;
        }
    }

    axiom_xpath_cast_string(node1, context);
    axiom_xpath_cast_string(node2, context);

    if (axutil_strcmp(
                (axis2_char_t *)(node1->value),
                (axis2_char_t *)(node2->value)) == 0)
    {
        return AXIS2_TRUE;
    }
    else
    {
        return AXIS2_FALSE;
    }
}

/* Convert a node set to boolean */
axis2_bool_t axiom_xpath_convert_to_boolean(
    axutil_array_list_t *node_set,
    axiom_xpath_context_t *context)
{
    if (axutil_array_list_size(node_set, context->env) == 0)
    {
        return AXIS2_FALSE;
    }
    else if (axutil_array_list_size(node_set, context->env) >= 2)
    {
        return AXIS2_TRUE;
    }
    else
    {
        axiom_xpath_result_node_t *node = axutil_array_list_get(node_set, context->env, 0);
        axiom_xpath_cast_boolean(node, context);

        return *(axis2_bool_t *)node->value;
    }
}

/* Operators */

/* Literal */
int axiom_xpath_literal_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    axiom_xpath_result_node_t *node;

    node = AXIS2_MALLOC(context->env->allocator,
            sizeof(axiom_xpath_result_node_t));

    /* Set the context node to NULL */
    /* This is not required; it gives some problems */
    /* context->node = NULL;*/

    node->value = op->par1;
    node->type = AXIOM_XPATH_TYPE_TEXT;

    axutil_stack_push(context->stack, context->env, node);

    return 1;
}

/* Number */
int axiom_xpath_number_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    axiom_xpath_result_node_t *node;

    node = AXIS2_MALLOC(context->env->allocator,
            sizeof(axiom_xpath_result_node_t));

    /* Set the context node to NULL */
    /* This is not required; it gives some problems */
    /* context->node = NULL;*/

    node->value = op->par1;
    node->type = AXIOM_XPATH_TYPE_NUMBER;

    axutil_stack_push(context->stack, context->env, node);

    return 1;
}

/* Path Expression */
int axiom_xpath_path_expression_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    int filter_res_n, n_nodes = 0;
    axiom_xpath_operation_t *rel_loc_op;
    axiom_xpath_operator_t rel_loc_func;
    axutil_array_list_t *arr;
    axiom_xpath_result_node_t *res_node;
    int i;

    /* Filter operation */
    if (op->op1 == AXIOM_XPATH_PARSE_END)
    {
        return 0;
    }

    filter_res_n = axiom_xpath_evaluate_operation(context, op->op1);

    /* Relative location path */
    if (op->op2 == AXIOM_XPATH_PARSE_END)
    {
        return filter_res_n;
    }

    rel_loc_op = AXIOM_XPATH_OPR_GET(op->op2);
    rel_loc_func = axiom_xpath_get_operator(rel_loc_op);

    /* Array list to add all results from the filter expression */
    arr = axutil_array_list_create(context->env, 0);

    for (i = 0; i < filter_res_n; i++)
    {
        axutil_array_list_add(
            arr,
            context->env,
            axutil_stack_pop(context->stack, context->env));
    }

    /* Evaluate relative location path for all results from the
       filter expression */
    for (i = 0; i < axutil_array_list_size(arr, context->env); i++)
    {
        res_node =
            (axiom_xpath_result_node_t *)axutil_array_list_get(
                arr, context->env, i);

        if (res_node->type != AXIOM_XPATH_TYPE_NODE)
        {
            continue;
        }

        context->node = (axiom_node_t *)res_node->value;
        context->position = i + 1;
        context->size = filter_res_n;

        n_nodes += rel_loc_func(context, rel_loc_op);
    }

    return n_nodes;
}

/* Or Expression */
int axiom_xpath_orexpr_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    axiom_xpath_result_node_t *node;
    int n_nodes[2];
    int i, j;
    int op12[2];
    axutil_array_list_t *arr[2];

    op12[0] = op->op1;
    op12[1] = op->op2;

    /* Evaluate both operands and get number of results */
    for (i = 0; i < 2; i++)
    {
        if (op12[i] == AXIOM_XPATH_PARSE_END)
        {
            continue;
        }

        n_nodes[i] = axiom_xpath_evaluate_operation(context, op12[i]);
    }


    for (i = 1; i >= 0; i--)
    {
        arr[i] = axutil_array_list_create(context->env, 0);

        for (j = 0; j < n_nodes[i]; j++)
        {
            axutil_array_list_add(
                arr[i],
                context->env,
                axutil_stack_pop(context->stack, context->env));
        }
    }

    node = AXIS2_MALLOC(context->env->allocator,
            sizeof(axiom_xpath_result_node_t));

    node->type = AXIOM_XPATH_TYPE_BOOLEAN;
    node->value = NULL;

    /* Checking equality
       - If any node from the first set is equal to any node from the second set
         the result is true */
    if (axiom_xpath_convert_to_boolean(arr[0], context) || axiom_xpath_convert_to_boolean(arr[1], context))
    {
        AXIOM_XPATH_CAST_SET_VALUE(axis2_bool_t, AXIS2_TRUE);

        axutil_stack_push(context->stack, context->env, node);
    }
    else
    {
        AXIOM_XPATH_CAST_SET_VALUE(axis2_bool_t, AXIS2_FALSE);

        axutil_stack_push(context->stack, context->env, node);
    }

    axutil_array_list_free(arr[0], context->env);
    axutil_array_list_free(arr[1], context->env);

    return 1;
}

/* And Expression */
int axiom_xpath_andexpr_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    axiom_xpath_result_node_t *node;
    int n_nodes[2];
    int i, j;
    int op12[2];
    axutil_array_list_t *arr[2];

    op12[0] = op->op1;
    op12[1] = op->op2;

    /* Evaluate both operands and get number of results */
    for (i = 0; i < 2; i++)
    {
        if (op12[i] == AXIOM_XPATH_PARSE_END)
        {
            continue;
        }

        n_nodes[i] = axiom_xpath_evaluate_operation(context, op12[i]);
    }


    for (i = 1; i >= 0; i--)
    {
        arr[i] = axutil_array_list_create(context->env, 0);

        for (j = 0; j < n_nodes[i]; j++)
        {
            axutil_array_list_add(
                arr[i],
                context->env,
                axutil_stack_pop(context->stack, context->env));
        }
    }

    node = AXIS2_MALLOC(context->env->allocator,
            sizeof(axiom_xpath_result_node_t));

    node->type = AXIOM_XPATH_TYPE_BOOLEAN;
    node->value = NULL;

    /* Checking equality
       - If any node from the first set is equal to any node from the second set
         the result is true */
    if (axiom_xpath_convert_to_boolean(arr[0], context) && axiom_xpath_convert_to_boolean(arr[1], context))
    {
        AXIOM_XPATH_CAST_SET_VALUE(axis2_bool_t, AXIS2_TRUE);

        axutil_stack_push(context->stack, context->env, node);
    }
    else
    {
        AXIOM_XPATH_CAST_SET_VALUE(axis2_bool_t, AXIS2_FALSE);

        axutil_stack_push(context->stack, context->env, node);
    }

    axutil_array_list_free(arr[0], context->env);
    axutil_array_list_free(arr[1], context->env);

    return 1;
}

/* Equal Expression */
int axiom_xpath_equalexpr_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    axiom_xpath_result_node_t *node;
    int n_nodes[2];
    int i, j;
    int op12[2];
    axutil_array_list_t *arr[2];

    op12[0] = op->op1;
    op12[1] = op->op2;

    /* Evaluate both operands and get number of results */
    for (i = 0; i < 2; i++)
    {
        if (op12[i] == AXIOM_XPATH_PARSE_END)
        {
            continue;
        }

        n_nodes[i] = axiom_xpath_evaluate_operation(context, op12[i]);
    }


    for (i = 1; i >= 0; i--)
    {
        arr[i] = axutil_array_list_create(context->env, 0);

        for (j = 0; j < n_nodes[i]; j++)
        {
            axutil_array_list_add(
                arr[i],
                context->env,
                axutil_stack_pop(context->stack, context->env));
        }
    }

    node = AXIS2_MALLOC(context->env->allocator,
            sizeof(axiom_xpath_result_node_t));

    node->type = AXIOM_XPATH_TYPE_BOOLEAN;
    node->value = NULL;

    /* Checking equality
       - If any node from the first set is equal to any node from the second set
         the result is true */
    for (i = 0; i < n_nodes[0]; i++)
    {
        for (j = 0; j < n_nodes[1]; j++)
        {
            if (axiom_xpath_compare_equal(
                        axutil_array_list_get(arr[0], context->env, i),
                        axutil_array_list_get(arr[1], context->env, j),
                        context))
            {
                AXIOM_XPATH_CAST_SET_VALUE(axis2_bool_t, AXIS2_TRUE);

                axutil_stack_push(context->stack, context->env, node);

                break;
            }
        }

        if (j < n_nodes[1])
        {
            break;
        }
    }

    if (!node->value)
    {
        AXIOM_XPATH_CAST_SET_VALUE(axis2_bool_t, AXIS2_FALSE);

        axutil_stack_push(context->stack, context->env, node);
    }

    axutil_array_list_free(arr[0], context->env);
    axutil_array_list_free(arr[1], context->env);

    return 1;
}

/* Union */
int axiom_xpath_union_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    int n_nodes = 0;
    int i;
    int op12[2];

    op12[0] = op->op1;
    op12[1] = op->op2;

    for (i = 0; i < 2; i++)
    {
        if (op12[i] == AXIOM_XPATH_PARSE_END)
        {
            continue;
        }

        n_nodes += axiom_xpath_evaluate_operation(context, op12[i]);
    }

    return n_nodes;
}

/* Set context node depending on whether relative or absolute location path */
int axiom_xpath_start_node_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    int n_nodes = 0;

    if (op->op1 == AXIOM_XPATH_PARSE_END)
    {
        return 0;
    }

    if (op->opr == AXIOM_XPATH_OPERATION_ROOT_NODE)
    {
        context->node = context->root_node;
        n_nodes += axiom_xpath_evaluate_operation(context, op->op1);
    }
    else if (op->opr == AXIOM_XPATH_OPERATION_CONTEXT_NODE)
    {
        n_nodes += axiom_xpath_evaluate_operation(context, op->op1);
    }

    return n_nodes;
}

/* Step */
int axiom_xpath_step_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    axiom_xpath_operation_t * node_test_op;
    axiom_xpath_iterator_t iter;
    axiom_xpath_axis_t axis;

    if (op->op1 == AXIOM_XPATH_PARSE_END)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf("Node test operator empty\n");
#endif

        return AXIOM_XPATH_EVALUATION_ERROR;
    }

    if (op->op2 == AXIOM_XPATH_PARSE_END)
    {
        return 0;
    }

    /* Get the name */
    node_test_op = AXIOM_XPATH_OPR_GET(op->op1);

    /* Get the axis */
    if (!node_test_op->par2)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf("axis is NULL in the step operator\n");
#endif
        return AXIOM_XPATH_EVALUATION_ERROR;
    }
    axis = *((axiom_xpath_axis_t *)node_test_op->par2);


    /* Get the iteration for the axis */
    iter = axiom_xpath_get_iterator(axis);

    return iter(context,
            op->op1 /* node test */,
            op->op2 /* next step */,
            node_test_op->op1 /* predicate */);
}

/* Predicates */
axis2_bool_t axiom_xpath_evaluate_predicate_condition(
    axiom_xpath_context_t *context,
    int n_nodes)
{
    axiom_xpath_result_node_t *res;
    int i;

    if (n_nodes <= 0)
    {
        return AXIS2_FALSE;
    }
    else if (n_nodes > 1)
    {
        for (i = 0; i < n_nodes; i++)
        {
            axutil_stack_pop(context->stack, context->env);
        }

        return AXIS2_TRUE;
    }
    else
    {
        res = (axiom_xpath_result_node_t *)axutil_stack_pop(
                    context->stack, context->env);

        if (res->type == AXIOM_XPATH_TYPE_NUMBER)
        {
            if (*(double *)(res->value) == context->position)
            {
                return AXIS2_TRUE;
            }
            else
            {
                return AXIS2_FAILURE;
            }
        }
        else if (res->type == AXIOM_XPATH_TYPE_BOOLEAN)
        {
            return *(axis2_bool_t *)(res->value);
        }
        else
        {
            return AXIS2_TRUE;
        }
    }
}

/* Predicate */
int axiom_xpath_evaluate_predicate(
    axiom_xpath_context_t *context,
    int op_next,
    int op_predicate)
{
    int n_res;
    axis2_bool_t res;

    axiom_xpath_operation_t * pred_op;
    axiom_node_t *context_node = context->node;

    if (op_predicate == AXIOM_XPATH_PARSE_END)
    {
        return axiom_xpath_evaluate_operation(context, op_next);
    }
    else
    {
        pred_op = AXIOM_XPATH_OPR_GET(op_predicate);

        pred_op->pos++;

        /* Evaluate the predicate */
        if (pred_op->op1 != AXIOM_XPATH_PARSE_END)
        {
            n_res = axiom_xpath_evaluate_operation(context, pred_op->op1);
            context->position = pred_op->pos;

            res = axiom_xpath_evaluate_predicate_condition(context, n_res);
        }
        else
        {
            res = AXIS2_TRUE;
        }
        /* A PredicateExpr is evaluated by evaluating the Expr and
           converting the result to a boolean. If the result is a number,
           the result will be converted to true if the number is equal to the
           context position and will be converted to false otherwise; if the
           result is not a number, then the result will be converted as if
           by a call to the boolean function. */

        /* Transform the result to number or boolean ? */

        if (res)
        {
            context->node = context_node;

            return axiom_xpath_evaluate_predicate(
                        context, op_next, pred_op->op2);
        }
    }

    return 0;
}

/* Node test match */
axis2_bool_t axiom_xpath_node_test_match(
    axiom_xpath_context_t *context,
    axiom_xpath_node_test_t *node_test)
{
    axiom_types_t type;
    axiom_element_t *element = NULL;
    axis2_char_t *name = NULL;
    axiom_namespace_t *ns = NULL, *xpath_ns = NULL;

    if (!context->node && !context->attribute && !context->ns)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf("Both context node and attribute are NULL.");
        printf(" May be a literal or a number.\n");
#endif
        return AXIS2_FALSE;
    }
    else if (context->node)
    {
        /* Test if the node matches */
        type = axiom_node_get_node_type(context->node, context->env);

        if (type == AXIOM_ELEMENT)
        {
            element = axiom_node_get_data_element(
                        context->node, context->env);

            name = axiom_element_get_localname(
                        element, context->env);

            ns = axiom_element_get_namespace(
                        element, context->env, context->node);
        }

        if (node_test->type == AXIOM_XPATH_NODE_TEST_NONE)
        {
            return AXIS2_FALSE;
        }
        else
        {
            if (node_test->type == AXIOM_XPATH_NODE_TEST_ALL
                    ||  node_test->type == AXIOM_XPATH_NODE_TEST_STANDARD)
            {
                if (type != AXIOM_ELEMENT)
                {
                    return AXIS2_FALSE;
                }

                /* Check namespace */
                if (node_test->type == AXIOM_XPATH_NODE_TEST_ALL)
                {
                    if (!ns && node_test->prefix)
                    {
                        return AXIS2_FALSE;
                    }
                }
                else
                {
                    if ((ns && !node_test->prefix)
                            || (!ns && node_test->prefix))
                    {
                        return AXIS2_FALSE;
                    }
                }

                if (ns && node_test->prefix)
                {
                    xpath_ns =
                        axiom_xpath_get_namespace(context, node_test->prefix);

                    if (!xpath_ns)
                    {
                        return AXIS2_FALSE;
                    }

                    if (axutil_strcmp(
                                axiom_namespace_get_uri(ns, context->env),
                                axiom_namespace_get_uri(xpath_ns, context->env)))
                    {
                        return AXIS2_FALSE;
                    }
                }

                /* Check local name */
                if (node_test->type == AXIOM_XPATH_NODE_TEST_ALL)
                {
                    return AXIS2_TRUE;
                }
                else if (node_test->type == AXIOM_XPATH_NODE_TEST_STANDARD)
                {
                    if (name && axutil_strcmp(node_test->name, name) == 0)
                    {
                        return AXIS2_TRUE;
                    }
                    else
                    {
                        return AXIS2_FALSE;
                    }
                }
            }
            else if (node_test->type == AXIOM_XPATH_NODE_TYPE_COMMENT)
            {
                if (type == AXIOM_COMMENT)
                {
                    return AXIS2_TRUE;
                }
                else
                {
                    return AXIS2_FALSE;
                }
            }
            else if (node_test->type == AXIOM_XPATH_NODE_TYPE_PI)
            {
                if (type == AXIOM_PROCESSING_INSTRUCTION)
                {
                    return AXIS2_TRUE;
                }
                else
                {
                    return AXIS2_FALSE;
                }
            }
            else if (node_test->type == AXIOM_XPATH_NODE_TYPE_NODE)
            {
                if (type == AXIOM_ELEMENT)
                {
                    return AXIS2_TRUE;
                }
                else
                {
                    return AXIS2_FALSE;
                }
            }
            else if (node_test->type == AXIOM_XPATH_NODE_TYPE_TEXT)
            {
                if (type == AXIOM_TEXT)
                {
                    return AXIS2_TRUE;
                }
                else
                {
                    return AXIS2_FALSE;
                }
            }
        }
    }
    /* Attributes */
    else if (context->attribute)
    {
        name = axiom_attribute_get_localname(context->attribute, context->env);
        ns = axiom_attribute_get_namespace(context->attribute, context->env);

        if (node_test->type == AXIOM_XPATH_NODE_TEST_NONE)
        {
            return AXIS2_FALSE;
        }
        else
        {
            /* Check namespace */
            if (node_test->type == AXIOM_XPATH_NODE_TEST_ALL)
            {
                if (!ns && node_test->prefix)
                {
                    return AXIS2_FALSE;
                }
            }
            else
            {
                if ((ns && !node_test->prefix) || (!ns && node_test->prefix))
                {
                    return AXIS2_FALSE;
                }
            }

            if (ns && node_test->prefix)
            {
                xpath_ns =
                    axiom_xpath_get_namespace(context, node_test->prefix);

                if (!xpath_ns)
                {
                    return AXIS2_FALSE;
                }

                if (axutil_strcmp(axiom_namespace_get_uri(ns, context->env),
                        axiom_namespace_get_uri(xpath_ns, context->env)))
                {
                    return AXIS2_FALSE;
                }
            }

            /* Check local name */
            if (node_test->type == AXIOM_XPATH_NODE_TEST_ALL)
            {
                return AXIS2_TRUE;
            }
            else if (node_test->type == AXIOM_XPATH_NODE_TEST_STANDARD)
            {
                if (name && axutil_strcmp(node_test->name, name) == 0)
                {
                    return AXIS2_TRUE;
                }
                else
                {
                    return AXIS2_FALSE;
                }
            }
        }
    }

    /* Namespace */
    else if (context->ns)
    {
        /* Prefix is checked ??? If changed to uri the cast
           method in xpath.c needs to be changed as well */
        name = axiom_namespace_get_prefix(context->ns, context->env);
        ns = NULL;

        if (node_test->type == AXIOM_XPATH_NODE_TEST_NONE)
        {
            return AXIS2_FALSE;
        }
        else
        {
            /* Check namespace */
            if (node_test->type == AXIOM_XPATH_NODE_TEST_ALL)
            {
                if (!ns && node_test->prefix)
                {
                    return AXIS2_FALSE;
                }
            }
            else
            {
                if ((ns && !node_test->prefix) || (!ns && node_test->prefix))
                {
                    return AXIS2_FALSE;
                }
            }

            if (ns && node_test->prefix)
            {
                xpath_ns =
                    axiom_xpath_get_namespace(context, node_test->prefix);

                if (!xpath_ns)
                {
                    return AXIS2_FALSE;
                }

                if (axutil_strcmp(axiom_namespace_get_uri(ns, context->env),
                        axiom_namespace_get_uri(xpath_ns, context->env)))
                {
                    return AXIS2_FALSE;
                }
            }

            /* Check local name */
            if (node_test->type == AXIOM_XPATH_NODE_TEST_ALL)
            {
                return AXIS2_TRUE;
            }
            else if (node_test->type == AXIOM_XPATH_NODE_TEST_STANDARD)
            {
                if (name && axutil_strcmp(node_test->name, name) == 0)
                {
                    return AXIS2_TRUE;
                }
                else
                {
                    return AXIS2_FALSE;
                }
            }
        }
    }

    return AXIS2_FALSE;
}

int axiom_xpath_function_call_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    axiom_xpath_function_t func = axiom_xpath_get_function(context, op->par1);
    int n_args = 0;

    if (!func)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf("Function %s not found\n", (char *)op->par1);
#endif

        return AXIOM_XPATH_EVALUATION_ERROR;
    }

    if (op->op1 != AXIOM_XPATH_PARSE_END)
    {
        n_args = axiom_xpath_evaluate_operation(context, op->op1);
    }

    return func(context, n_args);
}

int axiom_xpath_argument_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    int n_args = 0;

    if (op->op1 != AXIOM_XPATH_PARSE_END)
    {
        n_args += axiom_xpath_evaluate_operation(context, op->op1);
    }

    if (op->op2 != AXIOM_XPATH_PARSE_END)
    {
        n_args += axiom_xpath_evaluate_operation(context, op->op1);
    }

    return n_args;
}

/* Collect the current node to the results list */
int axiom_xpath_collect_operator(
    axiom_xpath_context_t *context,
    axiom_xpath_operation_t * op)
{
    axiom_xpath_result_node_t *node;

    node = AXIS2_MALLOC(context->env->allocator,
            sizeof(axiom_xpath_result_node_t));

    if (context->node)
    {
        node->value = context->node;
        node->type = AXIOM_XPATH_TYPE_NODE;
    }
    else if (context->attribute)
    {
        node->value = context->attribute;
        node->type = AXIOM_XPATH_TYPE_ATTRIBUTE;
    }
    else if (context->ns)
    {
        node->value = context->ns;
        node->type = AXIOM_XPATH_TYPE_NAMESPACE;
    }

    axutil_stack_push(context->stack, context->env, node);

    return 1;
}

/* Returns a pointer to the respective processing function */
axiom_xpath_operator_t axiom_xpath_get_operator(axiom_xpath_operation_t * op)
{
    switch (op->opr)
    {
        case AXIOM_XPATH_OPERATION_STEP:
            return axiom_xpath_step_operator;

        case AXIOM_XPATH_OPERATION_ROOT_NODE:
        case AXIOM_XPATH_OPERATION_CONTEXT_NODE:
            return axiom_xpath_start_node_operator;

        case AXIOM_XPATH_OPERATION_RESULT:
            return axiom_xpath_collect_operator;

        case AXIOM_XPATH_OPERATION_UNION:
            return axiom_xpath_union_operator;

        case AXIOM_XPATH_OPERATION_OR_EXPR:
            return axiom_xpath_orexpr_operator;

        case AXIOM_XPATH_OPERATION_AND_EXPR:
            return axiom_xpath_andexpr_operator;

        case AXIOM_XPATH_OPERATION_EQUAL_EXPR:
            return axiom_xpath_equalexpr_operator;

        case AXIOM_XPATH_OPERATION_LITERAL:
            return axiom_xpath_literal_operator;

        case AXIOM_XPATH_OPERATION_NUMBER:
            return axiom_xpath_number_operator;

        case AXIOM_XPATH_OPERATION_PATH_EXPRESSION:
            return axiom_xpath_path_expression_operator;

        case AXIOM_XPATH_OPERATION_FUNCTION_CALL:
            return axiom_xpath_function_call_operator;

        case AXIOM_XPATH_OPERATION_ARGUMENT:
            return axiom_xpath_argument_operator;

        default:
#ifdef AXIOM_XPATH_DEBUG
            printf("Unidentified operation.\n");
#endif

            return NULL;
    }
}

/* Returns a pointer to the respective processing function */
axiom_xpath_iterator_t axiom_xpath_get_iterator(axiom_xpath_axis_t axis)
{
    switch (axis)
    {
        case AXIOM_XPATH_AXIS_CHILD:
            return axiom_xpath_child_iterator;

        case AXIOM_XPATH_AXIS_DESCENDANT:
            return axiom_xpath_descendant_iterator;

        case AXIOM_XPATH_AXIS_PARENT:
            return axiom_xpath_parent_iterator;

        case AXIOM_XPATH_AXIS_ANCESTOR:
            return axiom_xpath_ancestor_iterator;

        case AXIOM_XPATH_AXIS_FOLLOWING_SIBLING:
            return axiom_xpath_following_sibling_iterator;

        case AXIOM_XPATH_AXIS_PRECEDING_SIBLING:
            return axiom_xpath_preceding_sibling_iterator;

        case AXIOM_XPATH_AXIS_FOLLOWING:
            return axiom_xpath_following_iterator;

        case AXIOM_XPATH_AXIS_PRECEDING:
            return axiom_xpath_preceding_iterator;

        case AXIOM_XPATH_AXIS_ATTRIBUTE:
            return axiom_xpath_attribute_iterator;

        case AXIOM_XPATH_AXIS_NAMESPACE:
            return axiom_xpath_namespace_iterator;

        case AXIOM_XPATH_AXIS_SELF:
            return axiom_xpath_self_iterator;

        case AXIOM_XPATH_AXIS_DESCENDANT_OR_SELF:
            return axiom_xpath_descendant_self_iterator;

        case AXIOM_XPATH_AXIS_ANCESTOR_OR_SELF:
            return axiom_xpath_ancestor_self_iterator;

        default:
#ifdef AXIOM_XPATH_DEBUG
            printf("Unidentified axis.\n");
#endif

            return NULL;
    }
}

int axiom_xpath_evaluate_operation(axiom_xpath_context_t *context, int op)
{
    axiom_xpath_operation_t * operation;
    axiom_xpath_operator_t function;

    /* Get a pointer to the operation */
    operation = AXIOM_XPATH_OPR_GET(op);

    /* Get the funciton which processes the operations */
    function = axiom_xpath_get_operator(operation);

    return function(context, operation);
}
