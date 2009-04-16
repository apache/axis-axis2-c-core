
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
#include "xpath_streaming.h"
#include "xpath_internals.h"
#include "xpath_internals_engine.h"

axiom_xpath_streaming_t axiom_xpath_streaming_check_operation(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr,
    int op_p)
{
    axiom_xpath_operation_t *op;

    if (op_p == AXIOM_XPATH_PARSE_END)
    {
        return AXIOM_XPATH_STREAMING_CONSTANT;
    }

    op = AXIOM_XPATH_OPR_EXPR_GET(op_p);

    switch (op->opr)
    {
        case AXIOM_XPATH_OPERATION_CONTEXT_NODE:
        case AXIOM_XPATH_OPERATION_ROOT_NODE:
            return axiom_xpath_streaming_combine_dependent(
                        AXIOM_XPATH_CHECK(op->op1),
                        AXIOM_XPATH_STREAMING_CONSTANT);

        case AXIOM_XPATH_OPERATION_STEP:
            return axiom_xpath_streaming_combine_dependent(
                        AXIOM_XPATH_CHECK(op->op1),
                        AXIOM_XPATH_CHECK(op->op2));

        case AXIOM_XPATH_OPERATION_RESULT:
            return AXIOM_XPATH_STREAMING_CONSTANT;

        case AXIOM_XPATH_OPERATION_UNION:
            return axiom_xpath_streaming_combine_independent(
                        AXIOM_XPATH_CHECK(op->op1),
                        AXIOM_XPATH_CHECK(op->op2));

        case AXIOM_XPATH_OPERATION_EQUAL_EXPR:
            return axiom_xpath_streaming_combine_independent(
                        AXIOM_XPATH_CHECK(op->op1),
                        AXIOM_XPATH_CHECK(op->op2));

        case AXIOM_XPATH_OPERATION_LITERAL:
            return AXIOM_XPATH_STREAMING_CONSTANT;

        case AXIOM_XPATH_OPERATION_NUMBER:
            return AXIOM_XPATH_STREAMING_CONSTANT;

        case AXIOM_XPATH_OPERATION_PATH_EXPRESSION:
            return axiom_xpath_streaming_combine_dependent(
                        AXIOM_XPATH_CHECK(op->op1),
                        AXIOM_XPATH_CHECK(op->op2));

        case AXIOM_XPATH_OPERATION_NODE_TEST:
            return axiom_xpath_streaming_check_node_test(env, expr, op);

        case AXIOM_XPATH_OPERATION_PREDICATE:
            return axiom_xpath_streaming_check_predicate(env, expr, op_p);

        default:
#ifdef AXIOM_XPATH_DEBUG
            printf("Unidentified operation.\n");
#endif

            return AXIOM_XPATH_STREAMING_NOT_SUPPORTED;
    }
}

axiom_xpath_streaming_t axiom_xpath_streaming_check_predicate(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr,
    int op_p)
{
    axiom_xpath_operation_t *op;

    if (op_p == AXIOM_XPATH_PARSE_END)
    {
        return AXIOM_XPATH_STREAMING_CONSTANT;
    }

    op = AXIOM_XPATH_OPR_EXPR_GET(op_p);

    return axiom_xpath_streaming_combine_dependent(
                AXIOM_XPATH_CHECK(op->op1),
                AXIOM_XPATH_CHECK(op->op2));
}

axiom_xpath_streaming_t axiom_xpath_streaming_check_node_test(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr,
    axiom_xpath_operation_t *op)
{
    axiom_xpath_axis_t axis = AXIOM_XPATH_AXIS_NONE;
    axiom_xpath_streaming_t r;

    if (!op->par2)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf("axis is NULL in the step operator\n");
#endif
        return AXIOM_XPATH_STREAMING_NOT_SUPPORTED;
    }

    axis = *((axiom_xpath_axis_t *)op->par2);

    switch (axis)
    {
        case AXIOM_XPATH_AXIS_ATTRIBUTE:
        case AXIOM_XPATH_AXIS_CHILD:
            break;

        default:
            return AXIOM_XPATH_STREAMING_NOT_SUPPORTED;
    }

    r = axiom_xpath_streaming_check_predicate(env, expr, op->op1);

    if (r != AXIOM_XPATH_STREAMING_ATTRIBUTE
            && r != AXIOM_XPATH_STREAMING_CONSTANT)
    {
        return AXIOM_XPATH_STREAMING_NOT_SUPPORTED;
    }

    if (axis == AXIOM_XPATH_AXIS_ATTRIBUTE)
    {
        return AXIOM_XPATH_STREAMING_ATTRIBUTE;
    }
    else
    {
        return AXIOM_XPATH_STREAMING_SUPPORTED;
    }
}

axiom_xpath_streaming_t axiom_xpath_streaming_combine_dependent(
    axiom_xpath_streaming_t r1,
    axiom_xpath_streaming_t r2)
{
    if (r1 == AXIOM_XPATH_STREAMING_NOT_SUPPORTED
            || r2 == AXIOM_XPATH_STREAMING_NOT_SUPPORTED)
    {
        return AXIOM_XPATH_STREAMING_NOT_SUPPORTED;
    }
    else if (r1 == AXIOM_XPATH_STREAMING_SUPPORTED
            || r2 == AXIOM_XPATH_STREAMING_SUPPORTED)
    {
        return AXIOM_XPATH_STREAMING_SUPPORTED;
    }
    else if (r1 == AXIOM_XPATH_STREAMING_ATTRIBUTE
            || r2 == AXIOM_XPATH_STREAMING_ATTRIBUTE)
    {
        return AXIOM_XPATH_STREAMING_ATTRIBUTE;
    }
    else
    {
        return AXIOM_XPATH_STREAMING_CONSTANT;
    }
}

axiom_xpath_streaming_t axiom_xpath_streaming_combine_independent(
    axiom_xpath_streaming_t r1,
    axiom_xpath_streaming_t r2)
{
    if (r1 == AXIOM_XPATH_STREAMING_NOT_SUPPORTED
            || r2 == AXIOM_XPATH_STREAMING_NOT_SUPPORTED)
    {
        return AXIOM_XPATH_STREAMING_NOT_SUPPORTED;
    }
    else if (r1 == AXIOM_XPATH_STREAMING_CONSTANT
            || r2 == AXIOM_XPATH_STREAMING_CONSTANT)
    {
        if (r1 == AXIOM_XPATH_STREAMING_SUPPORTED
                || r2 == AXIOM_XPATH_STREAMING_SUPPORTED)
        {
            return AXIOM_XPATH_STREAMING_SUPPORTED;
        }
        else if (r1 == AXIOM_XPATH_STREAMING_ATTRIBUTE
                || r2 == AXIOM_XPATH_STREAMING_ATTRIBUTE)
        {
            return AXIOM_XPATH_STREAMING_ATTRIBUTE;
        }
        else
        {
            return AXIOM_XPATH_STREAMING_CONSTANT;
        }
    }
    else if (r1 == AXIOM_XPATH_STREAMING_ATTRIBUTE
            || r2 == AXIOM_XPATH_STREAMING_ATTRIBUTE)
    {
        if (r1 == AXIOM_XPATH_STREAMING_SUPPORTED
                || r2 == AXIOM_XPATH_STREAMING_SUPPORTED)
        {
            return AXIOM_XPATH_STREAMING_SUPPORTED;
        }
        else
        {
            return AXIOM_XPATH_STREAMING_ATTRIBUTE;
        }
    }
    else
    {
        return AXIOM_XPATH_STREAMING_NOT_SUPPORTED;
    }
}

