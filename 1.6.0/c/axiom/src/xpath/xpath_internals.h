
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

#ifndef AXIOM_XPATH_INTERNALS_H
#define AXIOM_XPATH_INTERNALS_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axiom_xpath_internals internals
     * @ingroup axiom_xpath
     * @{
     */


    /** Get operation at index ind */
#define AXIOM_XPATH_OPR_GET(ind) (axiom_xpath_operation_t *) \
 axutil_array_list_get(context->expr->operations, context->env, ind)

    /**
      * An error has occured while parsing
      */
#define AXIOM_XPATH_PARSE_ERROR -2

    /**
      * XPath expression was successfully compiled
      */
#define AXIOM_XPATH_PARSE_SUCCESS 0

    /**
      * End of expression reached
      */
#define AXIOM_XPATH_PARSE_END -1

    /* Types */

    /**
      * XPath operation
      * Contains the operands and parameters
      */
    typedef struct axiom_xpath_operation axiom_xpath_operation_t;

    /**
      * XPath node test
      * Stores the components of a node test
      */
    typedef struct axiom_xpath_node_test axiom_xpath_node_test_t;

    /**
      * Functions to process a XPath operator
      */
    typedef int (*axiom_xpath_operator_t)(axiom_xpath_context_t *context,
            axiom_xpath_operation_t * op);

    /**
      * Functions to iterate through different XPath axes
      */
    typedef int (*axiom_xpath_iterator_t)(axiom_xpath_context_t *context,
            int op_node_test,
            int op_next,
            int op_predicate);

    /**
      * XPath node test types
      */
    typedef enum axiom_xpath_node_test_type_t
    {
        AXIOM_XPATH_NODE_TEST_NONE = 0,
        AXIOM_XPATH_NODE_TEST_ALL,
        AXIOM_XPATH_NODE_TYPE_COMMENT,
        AXIOM_XPATH_NODE_TYPE_NODE,
        AXIOM_XPATH_NODE_TYPE_PI,
        AXIOM_XPATH_NODE_TYPE_TEXT,
        AXIOM_XPATH_NODE_TEST_STANDARD
    } axiom_xpath_node_test_type_t;

    /**
      * XPath operations
      */
    typedef enum axiom_xpath_operation_type_t
    {
        AXIOM_XPATH_OPERATION_ROOT_NODE = 0,
        AXIOM_XPATH_OPERATION_CONTEXT_NODE,
        AXIOM_XPATH_OPERATION_NODE_TEST,
        AXIOM_XPATH_OPERATION_STEP,
        AXIOM_XPATH_OPERATION_RESULT,
        AXIOM_XPATH_OPERATION_UNION,
        AXIOM_XPATH_OPERATION_EQUAL_EXPR,
        AXIOM_XPATH_OPERATION_AND_EXPR,
        AXIOM_XPATH_OPERATION_OR_EXPR,
        AXIOM_XPATH_OPERATION_PREDICATE,
        AXIOM_XPATH_OPERATION_LITERAL,
        AXIOM_XPATH_OPERATION_NUMBER,
        AXIOM_XPATH_OPERATION_PATH_EXPRESSION,
        AXIOM_XPATH_OPERATION_FUNCTION_CALL,
        AXIOM_XPATH_OPERATION_ARGUMENT
    } axiom_xpath_operation_type_t;

    /**
      * XPath axes
      */
    typedef enum axiom_xpath_axis_t
    {
        AXIOM_XPATH_AXIS_NONE = -1,
        AXIOM_XPATH_AXIS_CHILD,
        AXIOM_XPATH_AXIS_DESCENDANT,
        AXIOM_XPATH_AXIS_PARENT,
        AXIOM_XPATH_AXIS_ANCESTOR,
        AXIOM_XPATH_AXIS_FOLLOWING_SIBLING,
        AXIOM_XPATH_AXIS_PRECEDING_SIBLING,
        AXIOM_XPATH_AXIS_FOLLOWING,
        AXIOM_XPATH_AXIS_PRECEDING,
        AXIOM_XPATH_AXIS_ATTRIBUTE,
        AXIOM_XPATH_AXIS_NAMESPACE,
        AXIOM_XPATH_AXIS_SELF,
        AXIOM_XPATH_AXIS_DESCENDANT_OR_SELF,
        AXIOM_XPATH_AXIS_ANCESTOR_OR_SELF
    } axiom_xpath_axis_t;

    /**
      * XPath node test structure
      */
    struct axiom_xpath_node_test
    {
        /** Node test type */
        axiom_xpath_node_test_type_t type;

        /**  Prefix
          * NULL if no prefix
          */
        axis2_char_t *prefix;

        /** Name */
        axis2_char_t *name;

        /** Literal for processing instruction (PI)  */
        axis2_char_t *lit;
    };

    /**
      * XPath operation structure
      */
    struct axiom_xpath_operation
    {
        /** Type of operator */
        axiom_xpath_operation_type_t opr;

        /** Parameters of the operation */
        void *par1;
        void *par2;

        /**  Position: Used for predicate evaluation
          */
        int pos;

        /** Operands pointing to other operations */
        int op1;
        int op2;
    };

    /**
      * Copies an XPath expression to a context
      *
      * No data is duplicated just the reference is assigned.
      * Some parameters in the expression are reset.
      * - pos in every operation
      *
      * @param context XPath context must not be NULL
      * @param expr Expression to be copied
      */
    void axiom_xpath_expression_copy(
        axiom_xpath_context_t *context,
        axiom_xpath_expression_t* expr);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif
