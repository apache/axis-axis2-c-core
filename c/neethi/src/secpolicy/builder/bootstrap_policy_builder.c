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

#include <rp_bootstrap_policy_builder.h>
#include <neethi_operator.h>
#include <neethi_policy.h>
#include <neethi_exactlyone.h>
#include <neethi_all.h>
#include <neethi_engine.h>

AXIS2_EXTERN neethi_assertion_t *AXIS2_CALL
rp_bootstrap_policy_builder_build(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element)
{
    neethi_policy_t *policy = NULL;
    axiom_node_t *child_node = NULL;
    axiom_element_t *child_element = NULL;
    neethi_assertion_t *assertion = NULL;

    child_node = axiom_node_get_first_element(node, env);
    if (!child_node)
    {
        return NULL;
    }

    if (axiom_node_get_node_type(child_node, env) == AXIOM_ELEMENT)
    {
        child_element =
            (axiom_element_t *) axiom_node_get_data_element(child_node, env);
        if (child_element)
        {
            policy = neethi_engine_get_policy(env, child_node, child_element);
            if (!policy)
            {
                return NULL;
            }
            assertion = neethi_assertion_create_with_args(env,
                                                  NULL, /*this policy should not be deleted*/
                                                  policy,
                                                  ASSERTION_TYPE_BOOTSTRAP_POLICY);
            return assertion;
        }
        else
            return NULL;
    }
    else
        return NULL;
}
