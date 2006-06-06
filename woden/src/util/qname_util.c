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
 
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <woden/util/axis2_qname_util.h>


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_qname_util_matches(
        const axis2_env_t *env,
        axis2_qname_t *qname,
        axis2_om_node_t *node)
{
    axis2_qname_t *qname_l = NULL;
    axis2_bool_t *ret = NULL;

    qname_l = axis2_qname_util_new_qname(env, node);
    ret = (NULL != node && AXIS2_TRUE == AXIS2_QNAME_EQUALS(qname, env, qname_l));
    AXIS2_QNAME_FREE(qname_l, env);

    return ret;
}        
        
AXIS2_EXTERN axis2_qname_t * AXIS2_CALL
axis2_qname_util_new_qname(
        const axis2_env_t *env,
        axis2_om_node_t *node)
{
    axis2_om_element_t *element = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if(NULL != node)
    {
        axis2_om_namespace_t *namespc = NULL;
        axis2_char_t *uri_str = NULL;
        axis2_char_t *localname = NULL;

        element = AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
        namespc = AXIS2_OM_ELEMENT_GET_NAMESPACE(element, env, node);
        uri_str = AXIS2_OM_NAMESPACE_GET_URI(namespc, env);
        localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(element, env);
        return axis2_qname_create(env, localname, uri_str, NULL);
    }
        
    return NULL;    
}                                     

