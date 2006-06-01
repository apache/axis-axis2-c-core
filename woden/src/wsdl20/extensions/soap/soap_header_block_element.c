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

#include <woden/wsdl20/extensions/soap/axis2_woden_soap_header_block_element.h>

axis2_status_t AXIS2_CALL
axis2_woden_soap_header_block_element_resolve_methods(
        axis2_woden_soap_header_block_element_t *soap_header_block_element,
        const axis2_env_t *env,
        axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    soap_header_block_element->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    soap_header_block_element->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    soap_header_block_element->ops->set_element_qname = 
            axis2_hash_get(methods, "set_element_qname", 
            AXIS2_HASH_KEY_STRING);

    soap_header_block_element->ops->get_element_qname = 
            axis2_hash_get(methods, "get_element_qname", 
            AXIS2_HASH_KEY_STRING);

    soap_header_block_element->ops->get_element = 
            axis2_hash_get(methods, "get_element", 
            AXIS2_HASH_KEY_STRING);

    soap_header_block_element->ops->set_must_understand = 
            axis2_hash_get(methods, "set_must understand", 
            AXIS2_HASH_KEY_STRING);

    soap_header_block_element->ops->must_understand = 
            axis2_hash_get(methods, "must_understand", 
            AXIS2_HASH_KEY_STRING);

    soap_header_block_element->ops->set_parent_element = 
            axis2_hash_get(methods, "set_parent_element", 
            AXIS2_HASH_KEY_STRING);

    soap_header_block_element->ops->get_parent_element = 
            axis2_hash_get(methods, "get_parent_element", 
            AXIS2_HASH_KEY_STRING);

    soap_header_block_element->ops->add_documentation_element = 
            axis2_hash_get(methods, "add_documentation_element", 
            AXIS2_HASH_KEY_STRING);

    soap_header_block_element->ops->get_documentation_elements = 
            axis2_hash_get(methods, "get_documentation_elements", 
            AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;    
}

