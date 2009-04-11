
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

#ifndef AXIOM_XPATH_INTERNALS_ENGINE_H
#define AXIOM_XPATH_INTERNALS_ENGINE_H

#include "xpath_internals.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axiom_xpath_engine engine
     * @ingroup axiom_xpath
     * @{
     */


    /* Macros */

    /** Set node->value to _value */
#define AXIOM_XPATH_CAST_SET_VALUE(_type, _value) { \
    _type * _var; \
    if(node->value \
      && node->type != AXIOM_XPATH_TYPE_NODE \
      && node->type != AXIOM_XPATH_TYPE_ATTRIBUTE \
      && node->type != AXIOM_XPATH_TYPE_NAMESPACE) \
      { AXIS2_FREE(context->env->allocator, node->value); } \
    _var = AXIS2_MALLOC(context->env->allocator, sizeof(_type)); \
    *_var = _value; \
    node->value = (void *) _var; \
}

    /* Functions */

    /**
      * Evaluate an XPath context, which containes a parsed expression
      *
      * @param context XPath context
      * @return Result set
      */
    axiom_xpath_result_t* axiom_xpath_run(axiom_xpath_context_t *context);

    /**
      * Finds the operator function for a given operation
      *
      * @param op Operation
      * @return Pointer to the function which process the given operation
      */
    axiom_xpath_operator_t axiom_xpath_get_operator(axiom_xpath_operation_t * op);

    /**
      * Get the iterotor for give axis
      *
      * @param axis XPath axis
      * @return Pointer to the function which iterater through the given axis
      */
    axiom_xpath_iterator_t axiom_xpath_get_iterator(axiom_xpath_axis_t axis);

    /* Operators */

    /**
      * Select the start node depending on whether it is an absolute or relative location path
      *
      * @param context XPath context
      * @param op XPath operation
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_start_node_operator(axiom_xpath_context_t *context, axiom_xpath_operation_t * op);

    /**
      * Add the context node or attribute to the results stack
      *
      * @param context XPath context
      * @param op XPath operation
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_collect_operator(axiom_xpath_context_t *context, axiom_xpath_operation_t * op);

    /**
      * Process union operator
      *
      * @param context XPath context
      * @param op XPath operation
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_union_operator(axiom_xpath_context_t *context, axiom_xpath_operation_t * op);

    /**
      * Perform or operation
      *
      * @param context XPath context
      * @param op XPath operation
      * @return Number of nodes added to the stack; which is one
      */
    int axiom_xpath_orexpr_operator(axiom_xpath_context_t *context, axiom_xpath_operation_t * op);

    /**
      * Perform and operation
      *
      * @param context XPath context
      * @param op XPath operation
      * @return Number of nodes added to the stack; which is one
      */
    int axiom_xpath_andexpr_operator(axiom_xpath_context_t *context, axiom_xpath_operation_t * op);

    /**
      * Perform equality test operation
      *
      * @param context XPath context
      * @param op XPath operation
      * @return Number of nodes added to the stack; which is one
      */
    int axiom_xpath_equalexpr_operator(axiom_xpath_context_t *context, axiom_xpath_operation_t * op);

    /**
      * Evaluate function call operation
      *
      * @param context XPath context
      * @param op XPath operation
      * @return Number of nodes added to the stack; which is one
      */
    int axiom_xpath_function_call_operator(axiom_xpath_context_t *context, axiom_xpath_operation_t * op);

    /**
      * Evaluate argument operation
      *
      * @param context XPath context
      * @param op XPath operation
      * @return Number of nodes added to the stack; which is one
      */
    int axiom_xpath_argument_operator(axiom_xpath_context_t *context, axiom_xpath_operation_t * op);

    /**
      * Process a path expression
      *
      * @param context XPath context
      * @param op XPath operation
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_path_expression_operator(axiom_xpath_context_t *context, axiom_xpath_operation_t * op);

    /**
      * Process a number; push the value to the stack
      *
      * @param context XPath context
      * @param op XPath operation
      * @return Number of nodes added to the stack; which is one
      */
    int axiom_xpath_number_operator(axiom_xpath_context_t *context, axiom_xpath_operation_t * op);

    /**
      * Process a literal; push the string to the stack
      *
      * @param context XPath context
      * @param op XPath operation
      * @return Number of nodes added to the stack; which is one
      */
    int axiom_xpath_literal_operator(axiom_xpath_context_t *context, axiom_xpath_operation_t * op);


    /**
      * Converts an XPath result to a boolean result
      *
      * @param node Result
      * @param context XPath context
      */
    void axiom_xpath_cast_boolean(axiom_xpath_result_node_t *node, axiom_xpath_context_t *context);

    /**
      * Converts an XPath result to a number
      *
      * @param node Result
      * @param context XPath context
      */
    void axiom_xpath_cast_number(axiom_xpath_result_node_t *node, axiom_xpath_context_t *context);

    /**
      * Converts an XPath result to a string
      *
      * @param node Result
      * @param context XPath context
      */
    void axiom_xpath_cast_string(axiom_xpath_result_node_t *node, axiom_xpath_context_t *context);

    /* Convert a node set to boolean */
    /**
      * Convert a XPath node set to boolean
      *
      * @param node_set Node set
      * @return AXIS2_TRUE if true; AXIS2_FALSE otherwise
      */
    axis2_bool_t axiom_xpath_convert_to_boolean(axutil_array_list_t *node_set, axiom_xpath_context_t *context);

    /**
      * Test if two result nodes are equal
      *
      * @param node1 First node
      * @param node2 Second node
      * @param context XPath context
      * @return AXIS2_TRUE if equal; AXIS2_FALSE otherwise
      */
    axis2_bool_t axiom_xpath_compare_equal(axiom_xpath_result_node_t *node1, axiom_xpath_result_node_t *node2, axiom_xpath_context_t *context);

    /**
      * Test if the axiom node matches node test
      *
      * @param context XPath context
      * @param node_test Node test
      * @return AXIS2_TRUE if matches; AXIS2_FALSE otherwise
      */
    axis2_bool_t axiom_xpath_node_test_match(axiom_xpath_context_t *context, axiom_xpath_node_test_t *node_test);

    /**
      * Evaluate if the predicate is true
      *
      * @param context XPath context
      * @param n_nodes Number of results after evaluating the expression in the predicate
      * @return AXIS2_TRUE if matches; AXIS2_FALSE otherwise
      */
    axis2_bool_t axiom_xpath_evaluate_predicate_condition(axiom_xpath_context_t *context, int n_nodes);

    /**
      * Evaluate if the predicate(s) is true using axiom_xpath_evaluate_predicate_condition and executes the operation op_next
      *
      * @param context XPath context
      * @param op_next The operation to be executed if all predicates are true
      * @param op_predicate Reference to predicate
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_evaluate_predicate(axiom_xpath_context_t *context, int op_next, int op_predicate);

    /**
      * Evaluate operation pointed by op
      *
      * @param context XPath context
      * @param op The index of the operation in the operations list
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_evaluate_operation(axiom_xpath_context_t *context, int op);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif
