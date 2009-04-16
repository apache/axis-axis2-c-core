
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
#include "xpath_internals_engine.h"
#include "xpath_functions.h"
#include "xpath_streaming.h"

/* Create XPath context */
AXIS2_EXTERN axiom_xpath_context_t * AXIS2_CALL axiom_xpath_context_create(
    const axutil_env_t *env,
    axiom_node_t * root_node)
{
    axiom_xpath_context_t* context;

    /*HACK: xpath impl requires a dummy root node in order to process properly.*/
    axiom_node_t * dummy_root;
    dummy_root = axiom_node_create(env);
    axiom_node_add_child(dummy_root, env, root_node);


    context = AXIS2_MALLOC(env->allocator,
            sizeof(axiom_xpath_context_t));

    context->env = env;
    context->root_node = dummy_root;
    context->node = dummy_root;
    context->expr = NULL;
    context->attribute = NULL;
    context->namespaces = NULL;
    context->functions = NULL;

    axiom_xpath_register_default_functions_set(context);

    return context;
}

/* Compile XPath expression */
AXIS2_EXTERN axiom_xpath_expression_t * AXIS2_CALL axiom_xpath_compile_expression(
    const axutil_env_t *env,
    const axis2_char_t* xpath_expr)
{
    axiom_xpath_expression_t* expr;

    expr = AXIS2_MALLOC(env->allocator,
            sizeof(axiom_xpath_expression_t));

    expr->expr_str = axutil_strdup(env, xpath_expr);

    if (axiom_xpath_compile(env, expr) == AXIOM_XPATH_PARSE_ERROR)
    {
        AXIS2_FREE(env->allocator, expr->expr_str);
        AXIS2_FREE(env->allocator, expr);

        return NULL;
    }
    else
        return expr;
}

/* Evaluate compiled XPath expression */
AXIS2_EXTERN axiom_xpath_result_t * AXIS2_CALL axiom_xpath_evaluate(
    axiom_xpath_context_t *context,
    axiom_xpath_expression_t *xpath_expr)
{
    axiom_xpath_expression_copy(context, xpath_expr);

    context->streaming = AXIS2_FALSE;
    
    return axiom_xpath_run(context);
}

AXIS2_EXTERN axiom_xpath_result_t * AXIS2_CALL axiom_xpath_evaluate_streaming(
    axiom_xpath_context_t *context,
    axiom_xpath_expression_t *xpath_expr)
{
    axiom_xpath_result_t *res;

    axiom_xpath_expression_copy(context, xpath_expr);

    if (axiom_xpath_streaming_check(context->env, xpath_expr))
    {
        context->streaming = AXIS2_TRUE;
        return axiom_xpath_run(context);
    }
    else
    {
        res = AXIS2_MALLOC(
                    context->env->allocator, sizeof(axiom_xpath_result_t));
        res->nodes = NULL;
        res->flag = AXIOM_XPATH_ERROR_STREAMING_NOT_SUPPORTED;

        return res;
    }
}

AXIS2_EXTERN void AXIS2_CALL axiom_xpath_register_default_functions_set(
    axiom_xpath_context_t *context)
{
    axiom_xpath_register_function(
        context, "count", axiom_xpath_function_count);
}


AXIS2_EXTERN void AXIS2_CALL axiom_xpath_register_function(
    axiom_xpath_context_t *context,
    axis2_char_t *name,
    axiom_xpath_function_t func)
{
    if (name && func)
    {
        if (!context->functions)
        {
            context->functions = axutil_hash_make(context->env);
        }

        axutil_hash_set(context->functions, name, AXIS2_HASH_KEY_STRING, func);
    }
}

AXIS2_EXTERN axiom_xpath_function_t AXIS2_CALL axiom_xpath_get_function(
    axiom_xpath_context_t *context,
    axis2_char_t *name)
{
    axiom_xpath_function_t func = NULL;

    if (context->functions)
    {
        func = axutil_hash_get(context->functions, name, AXIS2_HASH_KEY_STRING);
    }

    return func;
}

AXIS2_EXTERN void AXIS2_CALL axiom_xpath_register_namespace(
    axiom_xpath_context_t *context,
    axiom_namespace_t *ns)
{
    axis2_char_t *prefix = NULL;

    if (!context->namespaces)
    {
        context->namespaces = axutil_hash_make(context->env);
    }

    prefix = axiom_namespace_get_prefix(ns, context->env);

    if (prefix)
    {
        axutil_hash_set(
            context->namespaces, prefix, AXIS2_HASH_KEY_STRING, ns);
    }
}

AXIS2_EXTERN axiom_namespace_t * AXIS2_CALL axiom_xpath_get_namespace(
    axiom_xpath_context_t *context,
    axis2_char_t *prefix)
{
    axiom_namespace_t *ns = NULL;

    if (context->namespaces)
    {
        ns = axutil_hash_get(context->namespaces, prefix, AXIS2_HASH_KEY_STRING);
    }

    return ns;
}

AXIS2_EXTERN void AXIS2_CALL axiom_xpath_clear_namespaces(axiom_xpath_context_t *context)
{
    axutil_hash_index_t *hi;
    void *val;

    if (context->namespaces)
    {
        for (hi = axutil_hash_first(context->namespaces, context->env);
                hi;
                hi = axutil_hash_next(context->env, hi))
        {
            axutil_hash_this(hi, NULL, NULL, &val);
            axiom_namespace_free((axiom_namespace_t *)val, context->env);
        }

        axutil_hash_free(context->namespaces, context->env);
    }

    context->namespaces = NULL;
}

/* Cast to boolean */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL axiom_xpath_cast_node_to_boolean(
    const axutil_env_t *env,
    axiom_xpath_result_node_t * node)
{
    if (node->type == AXIOM_XPATH_TYPE_BOOLEAN)
    {
        return *(axis2_bool_t *)node->value;
    }
    else if (node->type == AXIOM_XPATH_TYPE_NUMBER)
    {
        /* Cannot evaluate as *(double *)(node->value) == 1e-12
           since there might be an precision error */
        if (*(double *)(node->value) > 1e-12
                || *(double *)(node->value) < -1e-12)
        {
            return AXIS2_TRUE;
        }
        else
        {
            return AXIS2_FALSE;
        }
    }
    else if (node->value)
    {
        return AXIS2_TRUE;
    }
    else
    {
        return AXIS2_FALSE;
    }
}

/* Cast to double */
AXIS2_EXTERN double AXIS2_CALL axiom_xpath_cast_node_to_number(
    const axutil_env_t *env,
    axiom_xpath_result_node_t * node)
{
    if (node->type == AXIOM_XPATH_TYPE_BOOLEAN)
    {
        if (*(axis2_bool_t *)(node->value) == AXIS2_TRUE)
        {
            return 1.0;
        }
        else
        {
            return 0.0;
        }
    }
    else if (node->type == AXIOM_XPATH_TYPE_NUMBER)
    {
        return *(double *)node->value;
    }
    else if (node->value)
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}

/* Cast to text */
AXIS2_EXTERN axis2_char_t * AXIS2_CALL axiom_xpath_cast_node_to_string(
    const axutil_env_t *env,
    axiom_xpath_result_node_t * node)
{
    axiom_element_t *ele;
    axis2_char_t *res;

    if (!node->value)
    {
        return NULL;
    }

    if (node->type == AXIOM_XPATH_TYPE_BOOLEAN)
    {
        if (*(axis2_bool_t *)(node->value) == AXIS2_TRUE)
        {
            return axutil_strdup(env, "true");
        }
        else
        {
            return axutil_strdup(env, "false");
        }
    }
    else if (node->type == AXIOM_XPATH_TYPE_NUMBER)
    {
        /* Allocate 50 bytes */
        res = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * 50);

        sprintf(res, "%lf", *(double *)(node->value));

        return res;
    }
    else if (node->type == AXIOM_XPATH_TYPE_TEXT)
    {
        return (axis2_char_t *)node->value;
    }
    else if (node->type == AXIOM_XPATH_TYPE_NODE)
    {
        ele = (axiom_element_t *)axiom_node_get_data_element(
                    (axiom_node_t *)(node->value), env);

        if (ele)
        {
            return axiom_element_get_text(
                        ele, env, (axiom_node_t *)(node->value));
        }
        else
        {
            return NULL;
        }
    }
    else if (node->type == AXIOM_XPATH_TYPE_ATTRIBUTE)
    {
        return axiom_attribute_get_value(
                    (axiom_attribute_t *)(node->value), env);
    }
    else if (node->type == AXIOM_XPATH_TYPE_NAMESPACE)
    {
        return axiom_namespace_get_prefix(
                    (axiom_namespace_t *)(node->value), env);
    }

    return NULL;
}

/* Cast to axiom node */
AXIS2_EXTERN axiom_node_t * AXIS2_CALL axiom_xpath_cast_node2axiom_node(
    const axutil_env_t *env,
    axiom_xpath_result_node_t * node)
{
    if (node->type == AXIOM_XPATH_TYPE_NODE && node->value)
    {
        return (axiom_node_t *)node->value;
    }
    else
    {
        return NULL;
    }
}

/* Free context */
AXIS2_EXTERN void AXIS2_CALL axiom_xpath_free_context(
    const axutil_env_t *env,
    axiom_xpath_context_t *context)
{
    if (context)
    {
        /* Free the expression if not freed */
        if (context->expr)
        {
            /* axiom_xpath_free_expression(env, context->expr); */

            context->expr = NULL;
        }

        AXIS2_FREE(env->allocator, context);
    }
}

/* Free expression */
AXIS2_EXTERN void AXIS2_CALL axiom_xpath_free_expression(
    const axutil_env_t *env,
    axiom_xpath_expression_t * xpath_expr)
{
    if (xpath_expr)
    {
        if (xpath_expr->expr_str)
        {
            AXIS2_FREE(env->allocator, xpath_expr->expr_str);

            xpath_expr->expr_str = NULL;
        }

        AXIS2_FREE(env->allocator, xpath_expr);
    }
}

/* Free result set */
AXIS2_EXTERN void AXIS2_CALL axiom_xpath_free_result(
    const axutil_env_t *env,
    axiom_xpath_result_t* result)
{
    if (result)
    {
        if (result->nodes)
        {
            axutil_array_list_free(result->nodes, env);
        }

        AXIS2_FREE(env->allocator, result);
    }
}

/* Check if the expression can be evaluated on streaming XML */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL axiom_xpath_streaming_check(
    const axutil_env_t *env,
    axiom_xpath_expression_t* expr)
{
    axiom_xpath_streaming_t r = AXIOM_XPATH_CHECK(expr->start);

    if (r == AXIOM_XPATH_STREAMING_NOT_SUPPORTED)
    {
        return AXIS2_FALSE;
    }
    else
    {
        return AXIS2_TRUE;
    }
}

