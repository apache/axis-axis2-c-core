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

/* Attach the expression to the context.
 *
 * The name says copy; nothing is copied, and nothing should be freed here
 * either. The context borrows the expression -- the caller compiled it, the
 * caller owns it, and axiom_xpath_free_expression is public API for exactly
 * that. Freeing the previous expression at this point had two ways to go
 * wrong: evaluating the same expression twice on one context freed it and then
 * walked the operations list below, and evaluating a second expression freed
 * the first one out from under a caller still holding it. The matching half of
 * this is axiom_xpath_free_context, which no longer frees the expression
 * either.
 */
void
axiom_xpath_expression_copy(
    axiom_xpath_context_t *context,
    axiom_xpath_expression_t* expr)
{
    int i;
    axiom_xpath_operation_t *op;

    if (!expr)
        return;

    context->expr = expr;

    /* Set value of pos in every operation to 0 */
    for(i = 0; i < axutil_array_list_size(expr->operations, context->env); i++)
    {
        op = AXIOM_XPATH_OPR_GET(i);
        op->pos = 0;
    }
}
