
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

#ifndef AXIOM_XPATH_INTERNALS_ITERATORS_H
#define AXIOM_XPATH_INTERNALS_ITERATORS_H

#include <axiom_xpath.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axiom_xpath_iterators iterators
     * @ingroup axiom_xpath
     * @{
     */


#ifdef AXIOM_XPATH_DEBUG
#define AXIOM_XPATH_ITERATOR_INITIALIZE { \
 if(!context->node) { \
  printf("Context node NULL; cannot evaluate self or descendent axis.\n"); \
  return AXIOM_XPATH_EVALUATION_ERROR; } \
 node_test_op = AXIOM_XPATH_OPR_GET(op_node_test); \
 if(!node_test_op) { \
  printf("Node test not present.\n"); \
  return AXIOM_XPATH_EVALUATION_ERROR; } \
 context_node = context->node; \
}
#else
#define AXIOM_XPATH_ITERATOR_INITIALIZE { \
 if(!context->node) { \
  return AXIOM_XPATH_EVALUATION_ERROR; } \
 node_test_op = AXIOM_XPATH_OPR_GET(op_node_test); \
 if(!node_test_op) { \
  return AXIOM_XPATH_EVALUATION_ERROR; } \
 context_node = context->node; \
}
#endif

    /**
      * Iterate through children
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_child_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through descendents
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_descendant_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through parent node. (Only one node)
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_parent_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through ancestors
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_ancestor_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through siblings following the context node
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_following_sibling_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through sibling preceding the context node
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_preceding_sibling_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through all nodes following the context node
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_following_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through all nodes preceding the context node
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_preceding_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through attributes
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_attribute_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through namespaces defined in the context node
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_namespace_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through self node (Only one node)
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_self_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through descendents and context node
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_descendant_self_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /**
      * Iterate through ancestors and context node
      *
      * @param context XPath context
      * @param op_node_test Reference to the node test operation
      *        in the parsed expression
      * @param op_next Reference to the next step in the location path,
      *        in the parsed expression
      * @param op_predicate Reference to the first predicate in the
      *        parsed expression
      * @return Number of nodes added to the stack
      */
    int axiom_xpath_ancestor_self_iterator(
        axiom_xpath_context_t *context,
        int op_node_test,
        int op_next,
        int op_predicate);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif
