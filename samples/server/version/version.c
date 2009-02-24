
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
#include "version.h"
#include <stdio.h>

axiom_node_t *
axis2_version_get_version(
    const axutil_env_t * env,
    axiom_node_t * node)
{
    if (!node)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INPUT_OM_NODE_NULL,
                        AXIS2_FAILURE);
        printf("Math client request ERROR: input parameter NULL\n");
        return NULL;
    }
    else
    {
        axis2_char_t result_str[255];

        axiom_element_t *ele1 = NULL;
        axiom_node_t *node1 = NULL,
            *node2 = NULL;
        axiom_namespace_t *ns1 = NULL;
        axiom_text_t *text1 = NULL;

        sprintf(result_str, "%s", "Version 1.6");

        ns1 = axiom_namespace_create(env,
                                     "urn:aewebservices71", "ns1");
        ele1 = axiom_element_create(env, NULL, "result", ns1, &node1);
        text1 = axiom_text_create(env, node1, result_str, &node2);

        return node1;
    }

    AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_SVC_SKEL_INVALID_OPERATION_PARAMETERS_IN_SOAP_REQUEST,
                    AXIS2_FAILURE);
    printf("Math service ERROR: invalid parameters\n");
    return NULL;
}

