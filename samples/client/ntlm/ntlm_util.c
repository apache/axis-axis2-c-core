
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

#include "ntlm_util.h"

/* build SOAP request message content using OM */
axiom_node_t *
build_om_payload_for_ntlm_auth_service(
    const axutil_env_t * env)
{
    axiom_node_t *ntlm_om_node = NULL;
    axiom_element_t *ntlm_om_ele = NULL;
    axiom_namespace_t *ns1 = NULL;
    axis2_char_t *om_str = NULL;

    ns1 = axiom_namespace_create(env, "http://tempuri.org", "ns1");
    ntlm_om_ele = axiom_element_create(env, NULL, "HelloWorld", ns1, &ntlm_om_node);
    axiom_element_set_text(ntlm_om_ele, env, "Hello World!", ntlm_om_node);
	axiom_namespace_free(ns1, env);

    om_str = axiom_node_to_string(ntlm_om_node, env);
    if (om_str)
	{
        printf("\nSending OM : %s\n", om_str);
    	AXIS2_FREE(env->allocator, om_str);
	}

    return ntlm_om_node;
}

