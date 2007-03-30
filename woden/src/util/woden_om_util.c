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

#include "woden_om_util.h"
#include <axutil_qname.h>
#include <axiom_element.h>
#include <axutil_uri.h>


AXIS2_EXTERN axutil_qname_t * AXIS2_CALL
woden_om_util_get_qname(
    const axutil_env_t *env,
    axiom_node_t *context_el_node,
    axis2_char_t *prefixed_value,
    axutil_hash_t *namespcs)
{
    axis2_char_t *index = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *localpart = NULL;
    axis2_char_t *namespc_uri_str = NULL;
    axiom_element_t *context_el = NULL;
    axiom_namespace_t *namespc_uri = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, context_el_node, NULL);
    AXIS2_PARAM_CHECK(env->error, prefixed_value, NULL);
    AXIS2_PARAM_CHECK(env->error, namespcs, NULL);

    index = axis2_rindex(prefixed_value, ':');
    localpart = index + 1;
    index[0] = '\0';
    prefix = prefixed_value;

    context_el = AXIOM_NODE_GET_DATA_ELEMENT(context_el_node, env);

    namespc_uri = axiom_element_find_namespace_uri(context_el,
            env, prefix, context_el_node);
    namespc_uri_str = axiom_namespace_get_uri(namespc_uri, env);
    if (namespc_uri_str)
    {
        woden_om_util_register_unique_prefix(env, prefix, namespc_uri_str, namespcs);
        return axutil_qname_create(env, localpart, namespc_uri_str, prefix);
    }
    return NULL;
}

AXIS2_EXTERN axis2_status_t  AXIS2_CALL
woden_om_util_register_unique_prefix(
    const axutil_env_t *env,
    axis2_char_t *prefix,
    axis2_char_t *namespc_uri_str,
    axutil_hash_t *namespcs)
{
    axutil_uri_t *ns_uri = NULL;
    axutil_uri_t *uri = NULL;
    axis2_char_t *ns_uri_str = NULL;
    axis2_char_t *tmp_prefix = NULL;

    ns_uri = axutil_hash_get(namespcs, prefix, AXIS2_HASH_KEY_STRING);
    if (ns_uri)
        ns_uri_str = axutil_uri_to_string(ns_uri, env, AXIS2_URI_UNP_OMITUSERINFO);
    if (ns_uri_str && 0 == axis2_strcmp(ns_uri_str, namespc_uri_str))
    {
        /* Namespace already registerd */
        return AXIS2_SUCCESS;
    }
    tmp_prefix = axis2_strdup(env, prefix);
    while (ns_uri_str && 0 != axis2_strcmp(ns_uri_str, namespc_uri_str))
    {
        axis2_char_t *temp = NULL;

        temp = axis2_stracat(env, tmp_prefix, "_");
        ns_uri = axutil_hash_get(namespcs, temp, AXIS2_HASH_KEY_STRING);
        ns_uri_str = axutil_uri_to_string(ns_uri, env, AXIS2_URI_UNP_OMITUSERINFO);
        AXIS2_FREE(env->allocator, tmp_prefix);
        tmp_prefix = axis2_strdup(env, temp);
        AXIS2_FREE(env->allocator, temp);
    }
    uri = axutil_uri_parse_string(env, namespc_uri_str);
    axutil_hash_set(namespcs, prefix, AXIS2_HASH_KEY_STRING, uri);

    return AXIS2_SUCCESS;
}

