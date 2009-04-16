
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
#include "xpath_internals_parser.h"
#include <ctype.h>

/* Compile an XPath expression */
int axiom_xpath_compile(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    if (!expr || !expr->expr_str)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf("Expression in NULL.\n");
#endif

        return AXIS2_FAILURE;
    }

    expr->expr_len = (int)axutil_strlen(expr->expr_str);

    expr->operations = axutil_array_list_create(env, 0);

    expr->expr_ptr = 0;

    expr->start = axiom_xpath_compile_orexpr(env, expr);

    if (expr->start == AXIOM_XPATH_PARSE_ERROR)
    {
        axutil_array_list_free(expr->operations, env);

        return AXIOM_XPATH_PARSE_ERROR;
    }
    else
    {
#ifdef AXIOM_XPATH_DEBUG
        printf("Expression successfully parsed\n");
#endif

        return AXIOM_XPATH_PARSE_SUCCESS;
    }
}

/* Parse Or Expression */
int axiom_xpath_compile_orexpr(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int op1, op2;

    if (!AXIOM_XPATH_HAS_MORE)
    {
        return AXIOM_XPATH_PARSE_END;
    }

    op1 = axiom_xpath_compile_andexpr(env, expr);

    if (op1 == AXIOM_XPATH_PARSE_ERROR)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: AndEpxr expected - %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    AXIOM_XPATH_SKIP_WHITESPACES;

    while (AXIOM_XPATH_CURRENT == 'o' && AXIOM_XPATH_NEXT(1) == 'r')
    {
        AXIOM_XPATH_READ(2);
        AXIOM_XPATH_SKIP_WHITESPACES;

        op2 = axiom_xpath_compile_andexpr(env, expr);

        if (op2 == AXIOM_XPATH_PARSE_ERROR)
        {
#ifdef AXIOM_XPATH_DEBUG
            printf(
                "Parse error: AndEpxr expected - %s\n",
                expr->expr_str + expr->expr_ptr);
#endif

            return AXIOM_XPATH_PARSE_ERROR;
        }

        op1 = AXIOM_XPATH_PUSH(AXIOM_XPATH_OPERATION_OR_EXPR, op1, op2);

        AXIOM_XPATH_SKIP_WHITESPACES;
    }

    return op1;
}

/* Parse And Expression */
int axiom_xpath_compile_andexpr(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int op1, op2;

    if (!AXIOM_XPATH_HAS_MORE)
    {
        return AXIOM_XPATH_PARSE_END;
    }

    op1 = axiom_xpath_compile_equalexpr(env, expr);

    if (op1 == AXIOM_XPATH_PARSE_ERROR)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: EqualityExpr expected - %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    AXIOM_XPATH_SKIP_WHITESPACES;

    while (AXIOM_XPATH_CURRENT == 'a' && AXIOM_XPATH_NEXT(1) == 'n' && AXIOM_XPATH_NEXT(1) == 'd')
    {
        AXIOM_XPATH_READ(2);
        AXIOM_XPATH_SKIP_WHITESPACES;

        op2 = axiom_xpath_compile_equalexpr(env, expr);

        if (op2 == AXIOM_XPATH_PARSE_ERROR)
        {
#ifdef AXIOM_XPATH_DEBUG
            printf(
                "Parse error: EqualityExpr expected - %s\n",
                expr->expr_str + expr->expr_ptr);
#endif

            return AXIOM_XPATH_PARSE_ERROR;
        }

        op1 = AXIOM_XPATH_PUSH(AXIOM_XPATH_OPERATION_AND_EXPR, op1, op2);

        AXIOM_XPATH_SKIP_WHITESPACES;
    }

    return op1;
}

/* Parse Equality Expression */
int axiom_xpath_compile_equalexpr(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int op1, op2;

    if (!AXIOM_XPATH_HAS_MORE)
    {
        return AXIOM_XPATH_PARSE_END;
    }

    op1 = axiom_xpath_compile_union(env, expr);

    if (op1 == AXIOM_XPATH_PARSE_ERROR)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: UnionExpr expected - %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    while (AXIOM_XPATH_CURRENT == '=')
    {
        AXIOM_XPATH_SKIP_WHITESPACES;
        AXIOM_XPATH_READ(1);
        AXIOM_XPATH_SKIP_WHITESPACES;

        op2 = axiom_xpath_compile_union(env, expr);

        if (op2 == AXIOM_XPATH_PARSE_ERROR)
        {
#ifdef AXIOM_XPATH_DEBUG
            printf(
                "Parse error: UnionExpr expected - %s\n",
                expr->expr_str + expr->expr_ptr);
#endif

            return AXIOM_XPATH_PARSE_ERROR;
        }

        op1 = AXIOM_XPATH_PUSH(AXIOM_XPATH_OPERATION_EQUAL_EXPR, op1, op2);

        AXIOM_XPATH_SKIP_WHITESPACES;
    }

    return op1;
}

/* Parse Union */
int axiom_xpath_compile_union(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int op1, op2;

    if (!AXIOM_XPATH_HAS_MORE)
    {
        return AXIOM_XPATH_PARSE_END;
    }

    op1 = axiom_xpath_compile_path_expression(env, expr);

    if (op1 == AXIOM_XPATH_PARSE_ERROR)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: PathExpr expected - %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    AXIOM_XPATH_SKIP_WHITESPACES;

    if (AXIOM_XPATH_CURRENT == '|')
    {
        AXIOM_XPATH_READ(1);
        AXIOM_XPATH_SKIP_WHITESPACES;
        op2 = axiom_xpath_compile_union(env, expr);

        if (op2 == AXIOM_XPATH_PARSE_ERROR)
        {
#ifdef AXIOM_XPATH_DEBUG
            printf(
                "Parse error: UnionExpr expected - %s\n",
                expr->expr_str + expr->expr_ptr);
#endif

            return AXIOM_XPATH_PARSE_ERROR;
        }

        return AXIOM_XPATH_PUSH(AXIOM_XPATH_OPERATION_UNION, op1, op2);
    }

    /* Just a location path */
    return op1;
}

/* Compile Filter expression */
int axiom_xpath_compile_filter(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int op = AXIOM_XPATH_PARSE_END;

    if (AXIOM_XPATH_CURRENT == '(')
    {
        AXIOM_XPATH_READ(1);
        op = axiom_xpath_compile_orexpr(env, expr);
        AXIOM_XPATH_SKIP_WHITESPACES;
        if (AXIOM_XPATH_CURRENT == ')')
        {
            AXIOM_XPATH_READ(1);
            return op;
        }
        else
        {
            return AXIOM_XPATH_PARSE_ERROR;
        }
    }
    else if (AXIOM_XPATH_CURRENT == '\'' || AXIOM_XPATH_CURRENT == '\"')
    {
        return AXIOM_XPATH_PUSH_PAR(
                    AXIOM_XPATH_OPERATION_LITERAL,
                    axiom_xpath_compile_literal(env, expr),
                    NULL, AXIOM_XPATH_PARSE_END);
    }
    else if (isdigit(AXIOM_XPATH_CURRENT)
            || (AXIOM_XPATH_CURRENT == '.' && isdigit(AXIOM_XPATH_NEXT(1))))
    {
        return AXIOM_XPATH_PUSH_PAR(
                    AXIOM_XPATH_OPERATION_NUMBER,
                    axiom_xpath_compile_number(env, expr),
                    NULL, AXIOM_XPATH_PARSE_END);
    }
    else if (AXIOM_XPATH_CURRENT == '$')
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: Variables are not supported, yet -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif
    }
    else
    {
        return axiom_xpath_compile_function_call(env, expr);
    }

#ifdef AXIOM_XPATH_DEBUG
    printf(
        "Parse error: Invalid Filter expression -  %s\n",
        expr->expr_str + expr->expr_ptr);
#endif

    return AXIOM_XPATH_PARSE_ERROR;
    /* TODO: functions and variables */
}

/* Parse Path expression (not a location path) */
int axiom_xpath_path_compile_path_expression_filter(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int op_filter, op_next;

    op_filter = axiom_xpath_compile_filter(env, expr);

    if (op_filter == AXIOM_XPATH_PARSE_ERROR)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: FilterExpr expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    AXIOM_XPATH_SKIP_WHITESPACES;

    if (AXIOM_XPATH_NEXT(0) == '/' && AXIOM_XPATH_NEXT(1) == '/')
    {
        AXIOM_XPATH_READ(2);

        op_next = axiom_xpath_compile_relative_location(env, expr);

        if (op_next == AXIOM_XPATH_PARSE_ERROR)
        {
#ifdef AXIOM_XPATH_DEBUG
            printf(
                "Parse error: RelativeLocation expected -  %s\n",
                expr->expr_str + expr->expr_ptr);
#endif

            return AXIOM_XPATH_PARSE_ERROR;
        }

        op_next = AXIOM_XPATH_WRAP_SELF_DESCENDANT(op_next);
    }
    else if (AXIOM_XPATH_NEXT(0) == '/')
    {
        AXIOM_XPATH_READ(1);

        op_next = axiom_xpath_compile_relative_location(env, expr);

        if (op_next == AXIOM_XPATH_PARSE_ERROR)
        {
#ifdef AXIOM_XPATH_DEBUG
            printf(
                "Parse error: RelativeLocation expected -  %s\n",
                expr->expr_str + expr->expr_ptr);
#endif

            return AXIOM_XPATH_PARSE_ERROR;
        }
    }
    else
    {
        op_next = AXIOM_XPATH_PARSE_END;
    }

    return AXIOM_XPATH_PUSH(
                AXIOM_XPATH_OPERATION_PATH_EXPRESSION,
                op_filter, op_next);
}

/* Parse Path expression */
int axiom_xpath_compile_path_expression(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int temp_ptr = expr->expr_ptr;
    axis2_char_t *name;
    axis2_char_t *node_types[] =
        {"comment", "node", "processing-instruction", "text"};
    axis2_char_t filter_start[] = {'$', '\'', '\"', '('};
    int i;

    /* if  | FilterExpr
           | FilterExpr '/' RelativeLocationPath
           | FilterExpr '//' RelativeLocationPath */
    for (i = 0; i < 4; i++)
    {
        if (AXIOM_XPATH_CURRENT == filter_start[i])
        {
            return axiom_xpath_path_compile_path_expression_filter(env, expr);
        }
    }

    if (isdigit(AXIOM_XPATH_CURRENT)
            || (AXIOM_XPATH_CURRENT == '.' && isdigit(AXIOM_XPATH_NEXT(1))))
    {
        return axiom_xpath_path_compile_path_expression_filter(env, expr);
    }

    /* Funciton calls */
    name = axiom_xpath_compile_ncname(env, expr);
    AXIOM_XPATH_SKIP_WHITESPACES;

    if (name != NULL && AXIOM_XPATH_CURRENT == '(')
    {
        expr->expr_ptr = temp_ptr;
        for (i = 0; i < 4; i++)
        {
            /* If node type */
            if (axutil_strcmp(name, node_types[i]) == 0)
            {
                return axiom_xpath_compile_location_path(env, expr);
            }
        }

        return axiom_xpath_path_compile_path_expression_filter(env, expr);
    }

    expr->expr_ptr = temp_ptr;

    return axiom_xpath_compile_location_path(env, expr);
}

/* Parses Location Path */
int axiom_xpath_compile_location_path(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int op1;
    axiom_xpath_operation_type_t opr;

    if (!AXIOM_XPATH_HAS_MORE)
    {
        return AXIOM_XPATH_PARSE_END;
    }

    if (AXIOM_XPATH_CURRENT == '/')
    {
        /* Descendent */
        if (AXIOM_XPATH_NEXT(1) == '/')
        {
            opr = AXIOM_XPATH_OPERATION_CONTEXT_NODE;
            AXIOM_XPATH_READ(2);
            AXIOM_XPATH_SKIP_WHITESPACES;

            op1 = axiom_xpath_compile_relative_location(env, expr);

            if (op1 == AXIOM_XPATH_PARSE_ERROR)
            {
#ifdef AXIOM_XPATH_DEBUG
                printf(
                    "Parse error: RelativeLocation expected -  %s\n",
                    expr->expr_str + expr->expr_ptr);
#endif

                return AXIOM_XPATH_PARSE_ERROR;
            }

            op1 = AXIOM_XPATH_WRAP_SELF_DESCENDANT(op1);
        }
        else
        {
            opr = AXIOM_XPATH_OPERATION_ROOT_NODE;
            AXIOM_XPATH_READ(1);

            op1 = axiom_xpath_compile_relative_location(env, expr);

            if (op1 == AXIOM_XPATH_PARSE_ERROR)
            {
#ifdef AXIOM_XPATH_DEBUG
                printf(
                    "Parse error: RelativeLocation expected -  %s\n",
                    expr->expr_str + expr->expr_ptr);
#endif

                return AXIOM_XPATH_PARSE_ERROR;
            }
        }
    }
    else
    {
        opr = AXIOM_XPATH_OPERATION_CONTEXT_NODE;

        op1 = axiom_xpath_compile_relative_location(env, expr);

        if (op1 == AXIOM_XPATH_PARSE_ERROR)
        {
#ifdef AXIOM_XPATH_DEBUG
            printf(
                "Parse error: RelativeLocation expected -  %s\n",
                expr->expr_str + expr->expr_ptr);
#endif

            return AXIOM_XPATH_PARSE_ERROR;
        }
    }

    if (op1 == AXIOM_XPATH_PARSE_ERROR)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: RelativeLocation expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    return AXIOM_XPATH_PUSH(opr, op1, AXIOM_XPATH_PARSE_END);
}

/* Parses Relative Location Path */
int axiom_xpath_compile_relative_location(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int op_step, op_next;

    if (!AXIOM_XPATH_HAS_MORE)
    {
        return AXIOM_XPATH_PARSE_END;
    }

    op_step = axiom_xpath_compile_step(env, expr);
    op_next = AXIOM_XPATH_PARSE_END; /* Will change if there are more steps */

    if (op_step == AXIOM_XPATH_PARSE_ERROR)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: Step expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    AXIOM_XPATH_SKIP_WHITESPACES;

    if (AXIOM_XPATH_NEXT(0) == '/' && AXIOM_XPATH_NEXT(1) == '/')
    {
        AXIOM_XPATH_READ(2);

        op_next = axiom_xpath_compile_relative_location(env, expr);

        if (op_next == AXIOM_XPATH_PARSE_ERROR)
        {
#ifdef AXIOM_XPATH_DEBUG
            printf(
                "Parse error: RelativeLocation expected -  %s\n",
                expr->expr_str + expr->expr_ptr);
#endif

            return AXIOM_XPATH_PARSE_ERROR;
        }

        op_next = AXIOM_XPATH_WRAP_SELF_DESCENDANT(op_next);
    }
    else if (AXIOM_XPATH_NEXT(0) == '/')
    {
        AXIOM_XPATH_READ(1);

        op_next = axiom_xpath_compile_relative_location(env, expr);

        if (op_next == AXIOM_XPATH_PARSE_ERROR)
        {
#ifdef AXIOM_XPATH_DEBUG
            printf(
                "Parse error: RelativeLocation expected -  %s\n",
                expr->expr_str + expr->expr_ptr);
#endif

            return AXIOM_XPATH_PARSE_ERROR;
        }
    }

    /* End of the location path */
    if (op_next == AXIOM_XPATH_PARSE_END)
    {
        op_next = AXIOM_XPATH_PUSH(
                    AXIOM_XPATH_OPERATION_RESULT,
                    AXIOM_XPATH_PARSE_END,
                    AXIOM_XPATH_PARSE_END);
    }

    return AXIOM_XPATH_PUSH(AXIOM_XPATH_OPERATION_STEP, op_step, op_next);
}

/* Parses Step */
int axiom_xpath_compile_step(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int op_predicate = AXIOM_XPATH_PARSE_END;
    axiom_xpath_node_test_t *node_test;
    int temp_ptr;
    axis2_char_t *name = NULL;
    axiom_xpath_axis_t axis = AXIOM_XPATH_AXIS_NONE;

    AXIOM_XPATH_SKIP_WHITESPACES;

    /* . and .. */
    if (AXIOM_XPATH_CURRENT == '.')
    {
        if (AXIOM_XPATH_NEXT(1) == '.')
        {
            AXIOM_XPATH_READ(2);
            axis = AXIOM_XPATH_AXIS_PARENT;
        }
        else
        {
            AXIOM_XPATH_READ(1);
            axis = AXIOM_XPATH_AXIS_SELF;
        }

        return AXIOM_XPATH_PUSH_PAR(
                    AXIOM_XPATH_OPERATION_NODE_TEST,
                    axiom_xpath_create_node_test_node(env),
                    axiom_xpath_create_axis(env, axis), op_predicate);
    }
    else if (AXIOM_XPATH_CURRENT == '@')
    {
        axis = AXIOM_XPATH_AXIS_ATTRIBUTE;

        AXIOM_XPATH_READ(1);
        AXIOM_XPATH_SKIP_WHITESPACES;
    }
    else
    {
        temp_ptr = expr->expr_ptr;

        name = axiom_xpath_compile_ncname(env, expr);

        if (name)
        {
            AXIOM_XPATH_SKIP_WHITESPACES;

            /* An axis */
            if (AXIOM_XPATH_CURRENT == ':' && AXIOM_XPATH_NEXT(1) == ':')
            {
                axis = axiom_xpath_get_axis(env, name);

                if (axis == AXIOM_XPATH_AXIS_NONE)
                {
#ifdef AXIOM_XPATH_DEBUG
                    printf("Parse error: Invalid axis -  %s\n", name);
#endif

                    return AXIOM_XPATH_PARSE_ERROR;
                }

                AXIOM_XPATH_READ(2);
                AXIOM_XPATH_SKIP_WHITESPACES;
            }
            else
            {
                axis = AXIOM_XPATH_AXIS_CHILD;

                expr->expr_ptr = temp_ptr;
            }
        }
        else
        {
            axis = AXIOM_XPATH_AXIS_CHILD;

            expr->expr_ptr = temp_ptr;
        }
    }

    node_test = axiom_xpath_compile_node_test(env, expr);

    if (!node_test)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: NodeTest expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    op_predicate = axiom_xpath_compile_predicate(env, expr);

    if (op_predicate == AXIOM_XPATH_PARSE_ERROR)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: Predicate expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    return AXIOM_XPATH_PUSH_PAR(
                AXIOM_XPATH_OPERATION_NODE_TEST,
                node_test, axiom_xpath_create_axis(env, axis),
                op_predicate);
}

axiom_xpath_node_test_t* axiom_xpath_compile_node_test(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    axis2_char_t* name;
    axiom_xpath_node_test_t *node_test;

    node_test = AXIS2_MALLOC(env->allocator, sizeof(axiom_xpath_node_test_t));
    node_test->type = AXIOM_XPATH_NODE_TEST_NONE;
    node_test->prefix = NULL;
    node_test->name = NULL;
    node_test->lit = NULL;

    if (AXIOM_XPATH_CURRENT == '*')
    {
        AXIOM_XPATH_READ(1);

        node_test->type = AXIOM_XPATH_NODE_TEST_ALL;

        return node_test;
    }

    name = axiom_xpath_compile_ncname(env, expr);

    if (!name)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: NCName expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        AXIS2_FREE(env->allocator, node_test);

        return NULL;
    }

    /* Node type */

    if (AXIOM_XPATH_CURRENT == '(')
    {
        AXIOM_XPATH_READ(1);

        if (axutil_strcmp(name, "comment") == 0)
        {
            node_test->type = AXIOM_XPATH_NODE_TYPE_COMMENT;
        }
        if (axutil_strcmp(name, "node") == 0)
        {
            node_test->type = AXIOM_XPATH_NODE_TYPE_NODE;
        }
        if (axutil_strcmp(name, "processing-instruction") == 0)
        {
            node_test->type = AXIOM_XPATH_NODE_TYPE_PI;

            node_test->lit = axiom_xpath_compile_literal(env, expr);
        }
        if (axutil_strcmp(name, "text") == 0)
        {
            node_test->type = AXIOM_XPATH_NODE_TYPE_TEXT;
        }

        AXIOM_XPATH_SKIP_WHITESPACES;

        if (node_test->type == AXIOM_XPATH_NODE_TEST_NONE
                || AXIOM_XPATH_CURRENT != ')')
        {
#ifdef AXIOM_XPATH_DEBUG
            printf("Parse error: Invalid node type -  %s\n", name);
#endif

            AXIS2_FREE(env->allocator, node_test);

            return NULL;
        }

        AXIOM_XPATH_READ(1);
    }
    else
    {
        node_test->type = AXIOM_XPATH_NODE_TEST_STANDARD;

        if (AXIOM_XPATH_CURRENT == ':')
        {
            AXIOM_XPATH_READ(1);

            node_test->prefix = name;

            if (AXIOM_XPATH_CURRENT == '*')
            {
                AXIOM_XPATH_READ(1);

                node_test->type = AXIOM_XPATH_NODE_TEST_ALL;

                return node_test;
            }

            node_test->name = axiom_xpath_compile_ncname(env, expr);

            if (!node_test->name)
            {
#ifdef AXIOM_XPATH_DEBUG
                printf(
                    "Parse error: NCName expected -  %s\n",
                    expr->expr_str + expr->expr_ptr);
#endif

                AXIS2_FREE(env->allocator, node_test);

                return NULL;
            }
        }
        else
        {
            node_test->name = name;
        }
    }

    return node_test;
}

int axiom_xpath_compile_function_call(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    axis2_char_t *name;
    int op1 = AXIOM_XPATH_PARSE_END;

    name = axiom_xpath_compile_ncname(env, expr);

    if (!name)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: NCName expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    if (AXIOM_XPATH_CURRENT != '(')
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: '(' expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    AXIOM_XPATH_READ(1);

    AXIOM_XPATH_SKIP_WHITESPACES;

    if (AXIOM_XPATH_CURRENT != ')')
    {
        op1 = axiom_xpath_compile_argument(env, expr);
    }

    if (AXIOM_XPATH_CURRENT != ')')
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: ')' expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    AXIOM_XPATH_READ(1);

    return AXIOM_XPATH_PUSH_PAR(
                AXIOM_XPATH_OPERATION_FUNCTION_CALL,
                name, NULL, op1);
}

int axiom_xpath_compile_argument(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int op1 = AXIOM_XPATH_PARSE_END;
    int op2 = AXIOM_XPATH_PARSE_END;

    op1 = axiom_xpath_compile_orexpr(env, expr);

    AXIOM_XPATH_SKIP_WHITESPACES;

    if (AXIOM_XPATH_CURRENT == ',')
    {
        op2 = axiom_xpath_compile_argument(env, expr);
    }

    return AXIOM_XPATH_PUSH(AXIOM_XPATH_OPERATION_ARGUMENT, op1, op2);
}

axiom_xpath_node_test_t* axiom_xpath_create_node_test_all(
    const axutil_env_t *env)
{
    axiom_xpath_node_test_t *node_test;

    node_test = AXIS2_MALLOC(env->allocator, sizeof(axiom_xpath_node_test_t));
    node_test->type = AXIOM_XPATH_NODE_TEST_ALL;
    node_test->prefix = NULL;
    node_test->name = NULL;
    node_test->lit = NULL;

    return node_test;
}

axiom_xpath_node_test_t* axiom_xpath_create_node_test_node(
    const axutil_env_t *env)
{
    axiom_xpath_node_test_t *node_test;

    node_test = AXIS2_MALLOC(env->allocator, sizeof(axiom_xpath_node_test_t));
    node_test->type = AXIOM_XPATH_NODE_TYPE_NODE;
    node_test->prefix = NULL;
    node_test->name = NULL;
    node_test->lit = NULL;

    return node_test;
}

double* axiom_xpath_compile_number(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    double *ret = AXIS2_MALLOC(env->allocator, sizeof(double));
    double res = 0, dec = 0.1;
    axis2_bool_t dot = AXIS2_FALSE;

    *ret = 0;

    while (1)
    {
        if (isdigit(AXIOM_XPATH_CURRENT))
        {
            if (!dot)
            {
                res = res * 10 + (AXIOM_XPATH_CURRENT - '0');
            }
            else
            {
                res += dec * (AXIOM_XPATH_CURRENT - '0');
                dec /= 10;
            }
        }
        else if (AXIOM_XPATH_CURRENT == '.')
        {
            if (dot)
            {
                return ret;
            }
            else
            {
                dot = AXIS2_TRUE;
                dec = 0.1;
            }
        }
        else
        {
            break;
        }

        AXIOM_XPATH_READ(1);
    }

    *ret = res;
    return ret;
}

axis2_char_t* axiom_xpath_compile_literal(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    axis2_char_t lit[255];
    int i = 0;
    axis2_char_t del;

    if (AXIOM_XPATH_CURRENT == '\"')
    {
        del = '\"';
    }
    else if (AXIOM_XPATH_CURRENT == '\'')
    {
        del = '\'';
    }
    else
        return NULL;

    AXIOM_XPATH_READ(1);

    while (AXIOM_XPATH_HAS_MORE && AXIOM_XPATH_CURRENT != del)
    {
        lit[i] = AXIOM_XPATH_CURRENT;
        AXIOM_XPATH_READ(1);
        ++i;
    }

    if (AXIOM_XPATH_HAS_MORE)
    {
        AXIOM_XPATH_READ(1);
    }

    lit[i] = '\0';

    return axutil_strdup(env, lit);

}

/* Get axis for name */
axiom_xpath_axis_t axiom_xpath_get_axis(
    const axutil_env_t *env,
    axis2_char_t* name)
{
    if (axutil_strcmp(name, "child") == 0)
    {
        return AXIOM_XPATH_AXIS_CHILD;
    }
    else if (axutil_strcmp(name, "descendant") == 0)
    {
        return AXIOM_XPATH_AXIS_DESCENDANT;
    }
    else if (axutil_strcmp(name, "parent") == 0)
    {
        return AXIOM_XPATH_AXIS_PARENT;
    }
    else if (axutil_strcmp(name, "ancestor") == 0)
    {
        return AXIOM_XPATH_AXIS_ANCESTOR;
    }
    else if (axutil_strcmp(name, "following-sibling") == 0)
    {
        return AXIOM_XPATH_AXIS_FOLLOWING_SIBLING;
    }
    else if (axutil_strcmp(name, "preceding-sibling") == 0)
    {
        return AXIOM_XPATH_AXIS_PRECEDING_SIBLING;
    }
    else if (axutil_strcmp(name, "following") == 0)
    {
        return AXIOM_XPATH_AXIS_FOLLOWING;
    }
    else if (axutil_strcmp(name, "preceding") == 0)
    {
        return AXIOM_XPATH_AXIS_PRECEDING;
    }
    else if (axutil_strcmp(name, "attribute") == 0)
    {
        return AXIOM_XPATH_AXIS_ATTRIBUTE;
    }
    else if (axutil_strcmp(name, "namespace") == 0)
    {
        return AXIOM_XPATH_AXIS_NAMESPACE;
    }
    else if (axutil_strcmp(name, "self") == 0)
    {
        return AXIOM_XPATH_AXIS_SELF;
    }
    else if (axutil_strcmp(name, "descendant-or-self") == 0)
    {
        return AXIOM_XPATH_AXIS_DESCENDANT_OR_SELF;
    }
    else if (axutil_strcmp(name, "ancestor-or-self") == 0)
    {
        return AXIOM_XPATH_AXIS_ANCESTOR_OR_SELF;
    }
    else
    {
#ifdef AXIOM_XPATH_DEBUG
        printf("Unidentified axis name.\n");
#endif

        return AXIOM_XPATH_AXIS_NONE;
    }
}

/* Parse Predicate */
int axiom_xpath_compile_predicate(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    int op1, op_next_predicate;

    AXIOM_XPATH_SKIP_WHITESPACES;

    if (!AXIOM_XPATH_HAS_MORE || AXIOM_XPATH_CURRENT != '[')
    {
        return AXIOM_XPATH_PARSE_END;
    }

    AXIOM_XPATH_READ(1);
    AXIOM_XPATH_SKIP_WHITESPACES;

    /* A PredicateExpr is evaluated by evaluating the Expr and converting the result to a boolean.
       If the result is a number, the result will be converted to true if the number is equal to the
       context position and will be converted to false otherwise; if the result is not a number,
       then the result will be converted as if by a call to the boolean function. */

    op1 = axiom_xpath_compile_orexpr(env, expr);

    if (op1 == AXIOM_XPATH_PARSE_ERROR)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: EqualExpr expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    AXIOM_XPATH_SKIP_WHITESPACES;

    if (AXIOM_XPATH_CURRENT != ']')
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: ] expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    AXIOM_XPATH_READ(1);

    op_next_predicate = axiom_xpath_compile_predicate(env, expr);

    if (op_next_predicate == AXIOM_XPATH_PARSE_ERROR)
    {
#ifdef AXIOM_XPATH_DEBUG
        printf(
            "Parse error: Predicate expected -  %s\n",
            expr->expr_str + expr->expr_ptr);
#endif

        return AXIOM_XPATH_PARSE_ERROR;
    }

    return AXIOM_XPATH_PUSH(
                AXIOM_XPATH_OPERATION_PREDICATE,
                op1, op_next_predicate);
}

/* Parse Node Test */
axis2_char_t * axiom_xpath_compile_ncname(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    axis2_char_t name[255];
    int i = 0;

    if (!isalpha(AXIOM_XPATH_CURRENT) && AXIOM_XPATH_CURRENT != '_')
    {
        return NULL;
    }

    /* TODO: Add CombiningChar and Extender
     * Link http://www.w3.org/TR/REC-xml/#NT-NameChar */
    while (AXIOM_XPATH_HAS_MORE
            && (isalnum(AXIOM_XPATH_CURRENT)
                    || AXIOM_XPATH_CURRENT == '_'
                    || AXIOM_XPATH_CURRENT == '.'
                    || AXIOM_XPATH_CURRENT == '-'))
    {
        name[i] = AXIOM_XPATH_CURRENT;
        AXIOM_XPATH_READ(1);
        ++i;
    }

    name[i] = '\0';

    return axutil_strdup(env, name);
}

/* Supporting functions */
int axiom_xpath_add_operation(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr,
    axiom_xpath_operation_type_t op_type,
    int op1, int op2, void *par1, void *par2)
{
    axiom_xpath_operation_t *op;

    op = AXIS2_MALLOC(env->allocator, sizeof(axiom_xpath_operation_t));
    op->opr = op_type;
    op->op1 = op1;
    op->op2 = op2;

    op->pos = 0;

    op->par1 = par1;
    op->par2 = par2;

    axutil_array_list_add(expr->operations, env, op);

    return axutil_array_list_size(expr->operations, env) - 1;
}

axiom_xpath_axis_t *axiom_xpath_create_axis(
    const axutil_env_t *env,
    axiom_xpath_axis_t axis)
{
    axiom_xpath_axis_t *axis_p =
        AXIS2_MALLOC(env->allocator, sizeof(axiom_xpath_axis_t));

    *axis_p = axis;
    return axis_p;
}
