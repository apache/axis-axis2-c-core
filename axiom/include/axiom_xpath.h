
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

#ifndef AXIOM_XPATH_H
#define AXIOM_XPATH_H

#include <axiom.h>
#include <axutil_env.h>
#include <axutil_stack.h>
#include <axiom_soap.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axiom_xpath_api api
     * @ingroup axiom_xpath
     * @{
     */


    /**
      * Enable tracing
      */

#define AXIOM_XPATH_DEBUG

    /**
      * An error occured while evaluating the xpath expression
      */
#define AXIOM_XPATH_EVALUATION_ERROR 0

#define AXIOM_XPATH_ERROR_STREAMING_NOT_SUPPORTED 10

    /* Typedefs */

    /**
      * XPath expression
      * It includes the expression as a string and parsed data.
      */
    typedef struct axiom_xpath_expression axiom_xpath_expression_t;

    /**
      * The XPath context
      * Keeps a reference to the context node or attribute,
      * XPath expression, environment and result set.
      */
    typedef struct axiom_xpath_context axiom_xpath_context_t;

    /**
      * XPath result set
      * Contains the result set and other information such as
      * whether the expression was evaluated successfully.
      */
    typedef struct axiom_xpath_result axiom_xpath_result_t;

    /**
      * XPath result
      * Stores type and value of the result.
      */
    typedef struct axiom_xpath_result_node axiom_xpath_result_node_t;

    /**
      * XPath result types
      */
    typedef enum axiom_xpath_result_type_t
    {
        AXIOM_XPATH_TYPE_NODE = 0,
        AXIOM_XPATH_TYPE_ATTRIBUTE,
        AXIOM_XPATH_TYPE_NAMESPACE,
        AXIOM_XPATH_TYPE_TEXT,
        AXIOM_XPATH_TYPE_NUMBER,
        AXIOM_XPATH_TYPE_BOOLEAN
    } axiom_xpath_result_type_t;

    typedef int (*axiom_xpath_function_t)(axiom_xpath_context_t *context,
            int np);

    /**
      * XPath expression
      */
    struct axiom_xpath_expression
    {
        /** XPath expression as a string */
        axis2_char_t* expr_str;

        /** Length of the expression */
        int expr_len;

        /** A cursor pointing to the position currently being parsed */
        int expr_ptr;

        /** Parsed expression in an array list*/
        axutil_array_list_t *operations;

        /** A pointer to the start operation in operations */
        int start;
    };

    /**
      * XPath context
      */
    struct axiom_xpath_context
    {
        /** Environment */
        const axutil_env_t *env;

        /** List of namespaces */
        axutil_hash_t *namespaces;

        /** Set of functions */
        axutil_hash_t *functions;

        /** Root node */
        axiom_node_t *root_node;

        /** Context node */
        axiom_node_t *node;

        /** Context attribute */
        axiom_attribute_t *attribute;

        /** Context attribute */
        axiom_namespace_t *ns;

        /** Context position */
        int position;

        /** Context size
          * *Does not work location paths due to optimizations */
        int size;

        /** XPath expression */
        axiom_xpath_expression_t *expr;

        /** Streaming */
        axis2_bool_t streaming;

        /** Stack of processed items */
        axutil_stack_t *stack;

        /* TODO:
           functions
           variables
           etc */
    };

    /**
      * XPath result set
      */
    struct axiom_xpath_result
    {
        /** A flag indicating whether errors occured while evaluting XPath
          * expression */
        int flag;

        /** An array list containing the set of results */
        axutil_array_list_t * nodes;
    };

    /**
      * XPath result
      */
    struct axiom_xpath_result_node
    {
        /** Type of result */
        axiom_xpath_result_type_t type;

        /** Value */
        void * value;
    };

    /**
      * Compile an XPath expression
      *
      * @param env Environment must not be null
      * @param xpath_expr A pointer to the XPath expression
      * @return The parsed XPath expression. Returns NULL if an error occured
      *         while parsing.
      */
    AXIS2_EXTERN axiom_xpath_expression_t * AXIS2_CALL axiom_xpath_compile_expression(
        const axutil_env_t *env,
        const axis2_char_t* xpath_expr);

    /**
      * Create an empty XPath context
      *
      * @param env Environment must not be null
      * @param root_node A pointer to the root of the tree
      * @return The initialized XPath context.
      */
    AXIS2_EXTERN axiom_xpath_context_t * AXIS2_CALL axiom_xpath_context_create(
        const axutil_env_t *env,
        axiom_node_t * root_node);

    /**
      * Evaluate an parsed XPath expression. Different expressions could
      * be evaluated on the same context, and same expression could be
      * evaluated on multiple trees without recompiling.
      *
      * @param context XPath context must not be null
      * @param xpath_expr XPath expression to be evaluated
      * @return The set of results.
      */
    AXIS2_EXTERN axiom_xpath_result_t * AXIS2_CALL axiom_xpath_evaluate(
        axiom_xpath_context_t *context,
        axiom_xpath_expression_t *xpath_expr);


    /**
      * Convert an XPath result to a boolean.
      * If the result is a boolean the value of it is returned.
      * If the result is a number, AXIS2_TRUE is
      * returned if the value is not equal to 0 and AXIS2_FALSE otherwise.
      * Otherwise AXIS2_TRUE is returned if the result is not NULL and AXIS2_FALSE otherwise.
      *
      * @param env Environment must not be null
      * @param node A pointer to the XPath result
      * @return The boolean value.
      */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL axiom_xpath_cast_node_to_boolean(
        const axutil_env_t *env,
        axiom_xpath_result_node_t * node);

    /**
      * Convert an XPath result to a number.
      * If the result is a boolean, 1 is returned if it's true and 0 otherwise.
      * If the result is a number the value of it is returned.
      * Otherwise AXIS2_TRUE is returned if the result is not NULL and AXIS2_FALSE otherwise.
      *
      * @param env Environment must not be null
      * @param node A pointer to the XPath result
      * @return The numerical value.
      */
    AXIS2_EXTERN double AXIS2_CALL axiom_xpath_cast_node_to_number(
        const axutil_env_t *env,
        axiom_xpath_result_node_t * node);

    /**
      * Convert an XPath result to text.
      * If the result is a boolean, "true" is returned if it's true and "false" otherwise.
      * If the result is a number the text representation of it is returned.
      * If the result is a text the value of it is returned.
      * If the result is an axiom node, the text value of it is returned
      * If the result is an axiom attribue, the text value of it is returned
      *
      * @param env Environment must not be null
      * @param node A pointer to the XPath result
      * @return The text value.
      */
    AXIS2_EXTERN axis2_char_t * AXIS2_CALL axiom_xpath_cast_node_to_string(
        const axutil_env_t *env,
        axiom_xpath_result_node_t * node);

    /**
      * Convert an XPath result to an axiom node.
      * If the result is an axiom node it is returned and NULL otherwise.
      *
      * @param env Environment must not be null
      * @param node A pointer to the XPath result
      * @return The axiom node.
      */
    AXIS2_EXTERN axiom_node_t * AXIS2_CALL axiom_xpath_cast_node_to_axiom_node(
        const axutil_env_t *env,
        axiom_xpath_result_node_t * node);


    /**
      * Free XPath context
      *
      * @param env Environment must not be null
      * @param context XPath context must not be null
      */
    AXIS2_EXTERN void AXIS2_CALL axiom_xpath_free_context(
        const axutil_env_t *env,
        axiom_xpath_context_t *context);

    /**
      * Free XPath expression
      *
      * @param env Environment must not be null
      * @param xpath_expr XPath expression must not be null
      */
    AXIS2_EXTERN void AXIS2_CALL axiom_xpath_free_expression(
        const axutil_env_t *env,
        axiom_xpath_expression_t * xpath_expr);

    /**
      * Free XPath result set
      *
      * @param env Environment must not be null
      * @param result XPath result set must not be null
      */
    AXIS2_EXTERN void AXIS2_CALL axiom_xpath_free_result(
        const axutil_env_t *env,
        axiom_xpath_result_t* result);

    /**
      * Registers a XPath namespace
      *
      * @param context XPath Context, must not be null
      * @param ns AXIOM namespace, must not be null
      */
    AXIS2_EXTERN void AXIS2_CALL axiom_xpath_register_namespace(
        axiom_xpath_context_t *context,
        axiom_namespace_t *ns);

    /**
      * Get a registered namespace by the prefix.
      * If there is no namespace registered by the given prefix NULL will be returned
      *
      * @param context XPath Context, must not be null
      * @param prefix Prefix of the namespace, must not be null
      * @return The namespace corresponding to the prefix.
      */
    AXIS2_EXTERN axiom_namespace_t * AXIS2_CALL axiom_xpath_get_namespace(
        axiom_xpath_context_t *context,
        axis2_char_t *prefix);

    /**
      * Clears all registered XPath namespaces
      *
      * @param context XPath Context, must not be null
      */
    AXIS2_EXTERN void AXIS2_CALL axiom_xpath_clear_namespaces(
        axiom_xpath_context_t *context);

    /**
      * Evaluates an XPath expression on streaming XML.
      * Not all expressions can be evaluated on streaming XML.
      * If the expression cannot be evaluated on streaming XML NULL will be returned.
      *
      * @param context XPath Context, must not be null
      * @param xpath_expr XPath expression to be evaluated
      */
    AXIS2_EXTERN axiom_xpath_result_t * AXIS2_CALL axiom_xpath_evaluate_streaming(
        axiom_xpath_context_t *context,
        axiom_xpath_expression_t *xpath_expr);

    /**
      * Checks whether the given expression can be evaluated on streaming XML.
      * If it is possible AXIS2_TRUE will be retuned; AXIS2_FALSE otherwise.
      *
      * @param env Axis2 environment, must not be null
      * @param expr Complied XPath expression, must not be null
      * @return A boolean indicating whether the expression can be evaluated on streaming XML.
      */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL axiom_xpath_streaming_check(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Setup the XPath core function library
      *
      * @param context XPath Context, must not be null
      */
    AXIS2_EXTERN void AXIS2_CALL axiom_xpath_register_default_functions_set(
        axiom_xpath_context_t *context);

    /**
      * Registers a custom XPath function http://www.w3.org/TR/xpath#corelib
      *
      * @param context XPath Context, must not be null
      * @param name Name of the function, must not be null
      * @param func Pointer to the function, must not be null
      */
    AXIS2_EXTERN void AXIS2_CALL axiom_xpath_register_function(
        axiom_xpath_context_t *context,
        axis2_char_t *name,
        axiom_xpath_function_t func);

    /**
      * Retrive a pointer to a registered funciton by the function name.
      * If there is no function registered by the given name, NULL will be returned.
      *
      * @param context XPath Context, must not be null
      * @param name Name of the function, must not be null
      * @return The corresponding function.
      */
    AXIS2_EXTERN axiom_xpath_function_t AXIS2_CALL axiom_xpath_get_function(
        axiom_xpath_context_t *context,
        axis2_char_t *name);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif
