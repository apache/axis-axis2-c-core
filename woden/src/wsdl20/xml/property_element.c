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

#include <woden/wsdl20/xml/axis2_woden_property_element.h>

axis2_status_t AXIS2_CALL
axis2_woden_property_element_resolve_methods(
        axis2_woden_property_element_t *property_element,
        axis2_env_t **env,
        axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    property_element->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    property_element->ops->to_property_element_free = axis2_hash_get(methods, 
            "to_property_element_free", AXIS2_HASH_KEY_STRING);
    property_element->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    property_element->ops->set_ref = axis2_hash_get(methods,
            "set_ref", AXIS2_HASH_KEY_STRING);
    property_element->ops->get_ref = axis2_hash_get(methods,
            "get_ref", AXIS2_HASH_KEY_STRING);
    property_element->ops->set_value = axis2_hash_get(methods,
            "set_value", AXIS2_HASH_KEY_STRING);
    property_element->ops->get_value = axis2_hash_get(methods,
            "get_value", AXIS2_HASH_KEY_STRING);
    property_element->ops->set_constraint_qname = axis2_hash_get(methods,
            "set_constraint_qname", AXIS2_HASH_KEY_STRING);
    property_element->ops->get_constraint_qname = axis2_hash_get(methods,
            "get_constraint_qname", AXIS2_HASH_KEY_STRING);
    property_element->ops->get_constraint = axis2_hash_get(methods,
            "get_constraint", AXIS2_HASH_KEY_STRING);
    property_element->ops->set_has_value_token = axis2_hash_get(methods,
            "set_has_value_token", AXIS2_HASH_KEY_STRING);
    property_element->ops->has_value_token = axis2_hash_get(methods,
            "has_value_token", AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;    
}

