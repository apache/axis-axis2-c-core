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

#include <axiom_node.h>
#include <axiom_element.h>
#include <woden_qname_util.h>


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_qname_util_matches(
    const axis2_env_t *env,
    axis2_qname_t *qname,
    axiom_node_t *node)
{
    axis2_qname_t *qname_l = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    qname_l = axis2_qname_util_new_qname(env, node);
    if (node && AXIS2_TRUE == axis2_qname_equals(qname, env, qname_l))
        ret = AXIS2_TRUE;
    axis2_qname_free(qname_l, env);

    return ret;
}

AXIS2_EXTERN axis2_qname_t * AXIS2_CALL
axis2_qname_util_new_qname(
    const axis2_env_t *env,
    axiom_node_t *node)
{
    axiom_element_t *element = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (node)
    {
        axiom_namespace_t *namespc = NULL;
        axis2_char_t *uri_str = NULL;
        axis2_char_t *localname = NULL;

        element = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
        namespc = axiom_element_get_namespace(element, env, node);
        uri_str = axiom_namespace_get_uri(namespc, env);
        localname = axiom_element_get_localname(element, env);
        return axis2_qname_create(env, localname, uri_str, NULL);
    }

    return NULL;
}

