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

#include <neethi_operator.h>
#include <neethi_exactlyone.h>
#include <neethi_all.h>
#include <neethi_engine.h>
#include <neethi_assertion.h>
#include <neethi_mtom_assertion_checker.h>


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
neethi_is_mtom_required(
    const axutil_env_t *env,
    neethi_policy_t *policy)
{
    axutil_array_list_t *alternatives = NULL;
    neethi_operator_t *component = NULL;
    neethi_all_t *all = NULL;
    axutil_array_list_t *arraylist = NULL;
    neethi_policy_t *normalized_policy = NULL;
    neethi_operator_t *operator = NULL;
    neethi_assertion_t *assertion = NULL;
    neethi_assertion_type_t type;
    void *value = NULL;
    int i = 0;

    normalized_policy = neethi_engine_get_normalize(env, AXIS2_FALSE, policy);

    if(normalized_policy)
    {
        alternatives = neethi_policy_get_alternatives(normalized_policy, env);
    }

    component =
        (neethi_operator_t *) axutil_array_list_get(alternatives, env, 0);
    all = (neethi_all_t *) neethi_operator_get_value(component, env);

    arraylist = neethi_all_get_policy_components(all, env);

    for (i = 0; i < axutil_array_list_size(arraylist, env); i++)
    {
        operator =(neethi_operator_t *) axutil_array_list_get(arraylist, env,
                                                              i);
        assertion =
            (neethi_assertion_t *) neethi_operator_get_value(operator, env);
        value = neethi_assertion_get_value(assertion, env);
        type = neethi_assertion_get_type(assertion, env);

        /*if (value)
        {*/
            if (type == ASSERTION_TYPE_OPTIMIZED_MIME_SERIALIZATION)
            {
                neethi_policy_free(normalized_policy, env);
                normalized_policy = NULL;
                return AXIS2_TRUE;
            }
        /*}*/
    }
    neethi_policy_free(normalized_policy, env);
    normalized_policy = NULL;

    return AXIS2_FALSE;
}
