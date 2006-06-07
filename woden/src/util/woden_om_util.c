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
 
#include <woden_om_node.h>
#include <woden_om_element.h>
#include <axis2_qname.h>


AXIS2_EXTERN axis2_qname_t * AXIS2_CALL
woden_om_util_get_qname(
        const axis2_env_t *env,
        woden_om_node_t *context_el_node,
        axis2_char_t *prefixed_value,
        void *desc)
{
    int index = 0;
    axis2_char_t *prefix = "";
    axis2_char_t *localpart = NULL;
    axis2_char_t *namespc_uri_str = NULL;

    index = axis2_rindex(prefixed_value, ':');
    localpart = index + 1;
    index[0] = '\0';
    prefix = prefixed_value;

    namespc_uri_str = woden_om_util_get_namespace_uri_from_prefix(
            env, context_el_node, prefix);
    if(NULL != namespc_uri_str)
    {
        woden_om_util_register_unique_prefix(env, prefix, namespc_uri_str, desc);
        return axis2_qname_create(env, localpart, namespc_uri_str, prefix);
    }
    return NULL; 
}        
 
AXIS2_EXTERN axis2_status_t  AXIS2_CALL
woden_om_util_get_qname(
        const axis2_env_t *env,
        axis2_char_t *prefix,
        axis2_char_t *namespc_uri_str,
        void *desc)
{
    axis2_uri_t *ns_uri = NULL;
    axis2_uri_t *uri = NULL;
    axis2_char_t *ns_uri_str = NULL;
    axis2_char_t *tmp_prefix = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    ns_uri = AXIS2_WODEN_DESC_ELEMENT_GET_NAMESPACE(desc, env, prefix);
    ns_uri_str = AXIS2_URI_TO_STRING(ns_uri, env);
    if(NULL != ns_uri_str && 0 == AXIS2_STRCMP(ns_uri_str, namespc_uri_str))
    {
        return; 
    }
    tmp_prefix = AXIS2_STRDUP(prefix, env);
    while(NULL != ns_uri_str && 0 != AXIS2_STRCMP(ns_uri_str, namespc_uri_str))
    {
        axis2_char_t *temp = NULL;

        temp = AXIS2_STRACAT(tmp_prefix, "_", env); 
        ns_uri = AXIS2_WODEN_DESC_ELEMENT_GET_NAMESPACE(desc, env, temp);
        ns_uri_str = AXIS2_URI_TO_STRING(ns_uri, env);
        AXIS2_FREE(env->allocator, tmp_prefix);
        tmp_prefix = AXIS2_STRDUP(temp, env);
        AXIS2_FREE(env->allocator, temp);
    }
    uri = axis2_uri_parse_string(env, namespc_uri_str);

    return AXIS2_WODEN_DESC_ELEMENT_ADD_NAMESPACE(desc, env, namespc_uri_str);
}
 
/**
* Given a prefix and a node, return the namespace URI that the prefix
* has been associated with. This method is useful in resolving the
* namespace URI of attribute values which are being interpreted as
* QNames. If prefix is null, this method will return the default
* namespace.
*
* @param context the starting node (looks up recursively from here)
* @param prefix the prefix to find an xmlns:prefix=uri for
*
* @return the namespace URI or null if not found
*/
AXIS2_EXTERN axis2_qname_t * AXIS2_CALL
woden_om_util_get_namespace_uri_from_prefix(
        const axis2_env_t *env,
        axis2_om_node_t *context_el_node,
        axis2_char_t *prefix)
{
    axis2_om_types_t node_type = AXIS2_OM_INVALID;
    axis2_om_node_t *temp_node = NULL;
    axis2_om_element_t *

    node_type = AXIS2_OM_NODE_GET_NODE_TYPE(context_el_node, env);
    switch(node_type)
    {
        case AXIS2_OM_ATTRIBUTE:
        {
            temp_node = context_el_node;
        }
        case AXIS2_OM_ELEMENT:
        {
            temp_node = AXIS2_OM_NODE_GET_PARENT(context_el_node, env);
        }
        default:
        {
        }
    }
}

