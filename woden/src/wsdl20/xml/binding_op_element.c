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

#include <woden/wsdl20/xml/axis2_woden_binding_op_element.h>

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_element_resolve_methods(
        axis2_woden_binding_op_element_t *binding_op_element,
        const axis2_env_t *env,
        axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    binding_op_element->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    binding_op_element->ops->to_binding_op_element_free = 
            axis2_hash_get(methods, "to_binding_op_element_free", 
            AXIS2_HASH_KEY_STRING);
    binding_op_element->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    binding_op_element->ops->set_ref = 
            axis2_hash_get(methods, "set_ref", 
            AXIS2_HASH_KEY_STRING);

    binding_op_element->ops->get_ref = 
            axis2_hash_get(methods, "get_ref", 
            AXIS2_HASH_KEY_STRING);
 
    binding_op_element->ops->get_interface_op_element = 
            axis2_hash_get(methods, "get_interface_op_element", 
            AXIS2_HASH_KEY_STRING);
   
    binding_op_element->ops->add_binding_msg_ref_element = 
            axis2_hash_get(methods, "add_binding_msg_ref_element", 
            AXIS2_HASH_KEY_STRING);

    binding_op_element->ops->remove_binding_msg_ref_element = 
            axis2_hash_get(methods, "remove_binding_msg_ref_element", 
            AXIS2_HASH_KEY_STRING);

    binding_op_element->ops->get_binding_msg_ref_elements = 
            axis2_hash_get(methods, "get_binding_msg_ref_elements", 
            AXIS2_HASH_KEY_STRING);

    binding_op_element->ops->add_binding_fault_ref_element = 
            axis2_hash_get(methods, "add_binding_fault_ref_element", 
            AXIS2_HASH_KEY_STRING);

    binding_op_element->ops->remove_binding_fault_ref_element = 
            axis2_hash_get(methods, "remove_binding_fault_ref_element", 
            AXIS2_HASH_KEY_STRING);

    binding_op_element->ops->get_binding_fault_ref_elements = 
            axis2_hash_get(methods, "get_binding_fault_ref_elements", 
            AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;    
}

