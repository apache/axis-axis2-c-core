/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "echo_util.h"

/* build SOAP request message content using OM */
axis2_om_node_t *
build_om_payload_for_echo_svc(const axis2_env_t *env)
{
    axis2_om_node_t *echo_om_node = NULL;
    axis2_om_element_t* echo_om_ele = NULL;
    axis2_om_node_t* text_om_node = NULL;
    axis2_om_element_t * text_om_ele = NULL;
    axis2_om_namespace_t *ns1 = NULL;
    axis2_char_t *om_str = NULL;
    
    ns1 = axis2_om_namespace_create (env, "http://ws.apache.org/axis2/c/samples", "ns1");
    echo_om_ele = axis2_om_element_create(env, NULL, "echoString", ns1, &echo_om_node);
    text_om_ele = axis2_om_element_create(env, echo_om_node, "text", NULL, &text_om_node);
    AXIS2_OM_ELEMENT_SET_TEXT(text_om_ele, env, "echo5", text_om_node);
    
    om_str = AXIS2_OM_NODE_TO_STRING(echo_om_node, env);
    if (om_str)
        printf("\nSending OM : %s\n", om_str);

    return echo_om_node;
}
