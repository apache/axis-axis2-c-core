
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

#ifndef AXIOM_XPATH_INTERNALS_PARSER_H
#define AXIOM_XPATH_INTERNALS_PARSER_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axiom_xpath_parser parser
     * @ingroup axiom_xpath
     * @{
     */


    /* Macros */

    /** Get the current character in the expression */
#define AXIOM_XPATH_CURRENT (expr->expr_ptr < expr->expr_len ? \
 expr->expr_str[expr->expr_ptr] : -1)

    /** Get a character after the current */
#define AXIOM_XPATH_NEXT(ind) (expr->expr_ptr + ind < expr->expr_len ? \
 expr->expr_str[expr->expr_ptr + ind] : -1)

    /** Increment the pointer to the expression */
#define AXIOM_XPATH_READ(n) (expr->expr_ptr += n)

    /** Check if expression has finished parsing */
#define AXIOM_XPATH_HAS_MORE (expr->expr_ptr < expr->expr_len)

    /** Skip white spaces */
#define AXIOM_XPATH_SKIP_WHITESPACES \
 { while(AXIOM_XPATH_CURRENT == ' ') \
    AXIOM_XPATH_READ(1); \
 }

    /** Wrape an operation in a self_or_descendent step; used to handle '//' */
#define AXIOM_XPATH_WRAP_SELF_DESCENDANT(_op2) \
 axiom_xpath_add_operation(env, expr, AXIOM_XPATH_OPERATION_STEP, \
      axiom_xpath_add_operation(env, expr, AXIOM_XPATH_OPERATION_NODE_TEST, \
           AXIOM_XPATH_PARSE_END, AXIOM_XPATH_PARSE_END, \
           axiom_xpath_create_node_test_node(env), \
           axiom_xpath_create_axis(env, AXIOM_XPATH_AXIS_DESCENDANT_OR_SELF)), \
      _op2, NULL, NULL)

    /** Adds an operation */
#define AXIOM_XPATH_PUSH(_opr, _op1, _op2) axiom_xpath_add_operation( \
 env, expr, _opr, _op1, _op2, NULL, NULL)

    /** Adds an operation with parameters */
#define AXIOM_XPATH_PUSH_PAR(_opr, _par1, _par2, _op1) axiom_xpath_add_operation( \
 env, expr, _opr, _op1, AXIOM_XPATH_PARSE_END, (void *)_par1, (void *)_par2)

    /* Functions */

    /**
      * Add an operation to the operations array
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @param op_type Type of the operation
      * @param op1 First operand
      * @param op2 Second operand
      * @param par1 First parameter
      * @param par2 Second parameter
      * @return Index of the operation in the array
      */
    int axiom_xpath_add_operation(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr,
        axiom_xpath_operation_type_t op_type,
        int op1,
        int op2,
        void *par1,
        void *par2);

    /**
      * Compile a XPath expression
      *
      * [14] Expr ::= OrExpr
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the main operation in the array
      */
    int axiom_xpath_compile(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile an equals expression
      *
      * ***Not completed***
      * [23] EqualityExpr ::= UnionExpr '=' UnionExpr
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_equalexpr(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile an union
      *
      * [18] UnionExpr ::= PathExpr
      *                    | UnionExpr '|' PathExpr
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_union(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile a path expression
      *
      * [19] PathExpr ::= LocationPath
      *                   | FilterExpr
      *                   | FilterExpr '/' RelativeLocationPath
      *                   | FilterExpr '//' RelativeLocationPath
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_path_expression(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile a path expression with a filter
      *
      * [19] PathExpr ::= FilterExpr
      *                   | FilterExpr '/' RelativeLocationPath
      *                   | FilterExpr '//' RelativeLocationPath
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_path_compile_path_expression_filter(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile a filter expression
      *
      * 20] FilterExpr ::= PrimaryExpr
      *                    | FilterExpr Predicate
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_filter(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile a location path
      *
      * [1] LocationPath          ::= RelativeLocationPath
      *                               | AbsoluteLocationPath
      * [2] AbsoluteLocationPath  ::= '/' RelativeLocationPath?
      *                               | AbbreviatedAbsoluteLocationPath
      *
      * [10] AbbreviatedAbsoluteLocationPath ::= '//' RelativeLocationPath
      * [11] AbbreviatedRelativeLocationPath ::= RelativeLocationPath '//' Step
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_location_path(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile a relative location
      *
      * [3] RelativeLocationPath ::= Step
      *                              | RelativeLocationPath '/' Step
      *                              | AbbreviatedRelativeLocationPath
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_relative_location(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile a step
      *
      * [4] Step          ::= AxisSpecifier NodeTest Predicate*
      *                       | AbbreviatedStep
      * [5] AxisSpecifier ::= AxisName '::'
      *                       | AbbreviatedAxisSpecifier
      *
      * [12] AbbreviatedStep             ::= '.' | '..'
      * [13] AbbreviatedAxisSpecifier    ::= '@'?
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_step(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compiles an OrExpr.
      *
      * [21]   	OrExpr	   ::=   	AndExpr
    		   *                          | OrExpr 'or' AndExpr
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_orexpr(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compiles an AndExpr.
      *
      * [22]   	AndExpr	   ::=   	EqualityExpr
    		   *                           | AndExpr 'and' EqualityExpr
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_andexpr(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);


    /**
      * Compiles a FunctionCall
      *
      * [16] FunctionCall    ::=    FunctionName '(' ( Argument ( ',' Argument )* )? ')'
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_function_call(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compiles an Argument.
      *
      * [17] Argument    ::=    Expr
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_argument(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile a node test
      *
      * [7] NodeTest ::= NameTest
      *                  | NodeType '(' ')'
      *                  | 'processing-instruction' '(' Literal ')'
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    axiom_xpath_node_test_t* axiom_xpath_compile_node_test(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile a predicate(s)
      *
      * [8] Predicate     ::= '[' PredicateExpr ']'
      * [9] PredicateExpr ::= Expr
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return Index of the operation in the array
      */
    int axiom_xpath_compile_predicate(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);



    /**
      * Compile a literal
      *
      * [29] Literal ::= '"' [^"]* '"'
      *                  | "'" [^']* "'"
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return The literal parsed
      */
    axis2_char_t* axiom_xpath_compile_literal(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile a number
      *
      * [30] Number ::= Digits ('.' Digits?)?
      *                 | '.' Digits
      * [31]    Digits    ::=    [0-9]+
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return The number parsed
      */
    double* axiom_xpath_compile_number(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);

    /**
      * Compile a ncname
      *
      * @param env Environment must not be null
      * @param expr A pointer to the XPath expression
      * @return The ncname parsed
      */
    axis2_char_t* axiom_xpath_compile_ncname(
        const axutil_env_t *env,
        axiom_xpath_expression_t* expr);



    /**
      * Get the XPath axis by axis name
      *
      * [6] AxisName ::= 'ancestor'
      *                  | 'ancestor-or-self'
      *                  | 'attribute'
      *                  | 'child'
      *                  | 'descendant'
      *                  | 'descendant-or-self'
      *                  | 'following'
      *                  | 'following-sibling'
      *                  | 'namespace'
      *                  | 'parent'
      *                  | 'preceding'
      *                  | 'preceding-sibling'
      *                  | 'self'
      *
      * @param env Environment must not be null
      * @param name Name of the axis
      * @return XPath axis; returns AXIOM_XPATH_AXIS_NONE if invalid name
      */
    axiom_xpath_axis_t axiom_xpath_get_axis(
        const axutil_env_t *env,
        axis2_char_t* name);

    /**
      * Create a node test which matches all nodes (*)
      *
      * @param env Environment must not be null
      * @return Node test
      */
    axiom_xpath_node_test_t* axiom_xpath_create_node_test_all(
        const axutil_env_t *env);

    /**
      * Create a node test which matches all nodes (nodes())
      *
      * @param env Environment must not be null
      * @return Node test
      */
    axiom_xpath_node_test_t* axiom_xpath_create_node_test_node(
        const axutil_env_t *env);

    /**
      * Create a pointer to an axis; allocate memory using the allocator
      *
      * @param env Environment must not be null
      * @param axis XPath axis
      * @return Pointer to the axis created
      */
    axiom_xpath_axis_t *axiom_xpath_create_axis(
        const axutil_env_t *env,
        axiom_xpath_axis_t axis);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif
