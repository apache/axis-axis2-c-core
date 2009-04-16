
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

#ifndef AXIOM_XPATH_STREAMING_H
#define AXIOM_XPATH_STREAMING_H

#include "xpath_internals.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axiom_xpath_streaming streaming
     * @ingroup axiom_xpath
     * @{
     */


    /**
     * XPath streaming support
     */
    typedef enum axiom_xpath_streaming_t
    {
        AXIOM_XPATH_STREAMING_NOT_SUPPORTED = 0,
        AXIOM_XPATH_STREAMING_SUPPORTED,
        AXIOM_XPATH_STREAMING_CONSTANT,
        AXIOM_XPATH_STREAMING_ATTRIBUTE
    } axiom_xpath_streaming_t;

    /** Check whether the given expression is supported on streaming XML */
#define AXIOM_XPATH_CHECK(op) axiom_xpath_streaming_check_operation(env, expr, op)

    /** Get an operation from the list of operations */
#define AXIOM_XPATH_OPR_EXPR_GET(ind) (axiom_xpath_operation_t *) \
 axutil_array_list_get(expr->operations, env, ind)

    /**
      * Checks whether the given expression is supported on streaming XML
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @param op_p Index of the operation in the list of operations
      * @return Whether the given operation can be evaluated on streaming XML
      */
    axiom_xpath_streaming_t axiom_xpath_streaming_check_operation(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr,
        int op_p);

    /**
      * Checks whether the predicate is supported on streaming XML.
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @param op_p Index of the operation in the list of operations
      * @return Whether the given operation can be evaluated on streaming XML
      */
    axiom_xpath_streaming_t axiom_xpath_streaming_check_predicate(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr,
        int op_p);

    /**
      * Checks whether the predicate is supported on streaming XML.
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @param op Index of the operation in the list of operations
      * @return Whether the given operation can be evaluated on streaming XML
      */
    axiom_xpath_streaming_t axiom_xpath_streaming_check_node_test(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr,
        axiom_xpath_operation_t* op);


    /**
      * Checks whether the two operations can be evaluated on streaming XML
      * sequencially (one after the other), where the result of the first
      * operation is the context of the next
      *
      * @param r1 Type of first operation
      * @param r2 Type of second operation
      * @return Whether the given operations can be evaluated on streaming XML
      */
    axiom_xpath_streaming_t axiom_xpath_streaming_combine_dependent(
        axiom_xpath_streaming_t r1,
        axiom_xpath_streaming_t r2);


    /**
      * Checks whether the two operations can be evaluated on streaming XML
      * simultaneousy
      *
      * @param r1 Type of first operation
      * @param r2 Type of second operation
      * @return Whether the given operations can be evaluated on streaming XML
      */
    axiom_xpath_streaming_t axiom_xpath_streaming_combine_independent(
        axiom_xpath_streaming_t r1,
        axiom_xpath_streaming_t r2);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif
