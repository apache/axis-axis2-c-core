
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
#include "xpath_functions.h"
#include "xpath_internals_engine.h"

int axiom_xpath_function_count(axiom_xpath_context_t *context, int np)
{
    axiom_xpath_result_node_t *node;
    double * v;
    int i;

    node = AXIS2_MALLOC(
                context->env->allocator, sizeof(axiom_xpath_result_node_t));
    v = AXIS2_MALLOC(context->env->allocator, sizeof(double));

    *v = np;
    node->value = v;
    node->type = AXIOM_XPATH_TYPE_NUMBER;

    for (i = 0; i < np; i++)
    {
        axutil_stack_pop(context->stack, context->env);
    }

    axutil_stack_push(context->stack, context->env, node);

    return 1;
}
