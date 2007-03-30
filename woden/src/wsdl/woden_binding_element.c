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

#include <woden_binding_element.h>

axis2_status_t AXIS2_CALL
woden_binding_element_resolve_methods(
    woden_binding_element_t *binding_element,
    const axutil_env_t *env,
    axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);

    binding_element->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    binding_element->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    binding_element->ops->set_qname =
        axis2_hash_get(methods, "set_qname",
                AXIS2_HASH_KEY_STRING);

    binding_element->ops->get_qname =
        axis2_hash_get(methods, "get_qname",
                AXIS2_HASH_KEY_STRING);

    binding_element->ops->set_interface_qname =
        axis2_hash_get(methods, "set_interface_qname",
                AXIS2_HASH_KEY_STRING);

    binding_element->ops->get_interface_qname =
        axis2_hash_get(methods, "get_interface_qname",
                AXIS2_HASH_KEY_STRING);

    binding_element->ops->get_interface_element =
        axis2_hash_get(methods, "get_interface_element",
                AXIS2_HASH_KEY_STRING);

    binding_element->ops->set_type =
        axis2_hash_get(methods, "set_type",
                AXIS2_HASH_KEY_STRING);

    binding_element->ops->get_type =
        axis2_hash_get(methods, "get_type",
                AXIS2_HASH_KEY_STRING);

    binding_element->ops->add_binding_fault_element =
        axis2_hash_get(methods, "add_binding_fault_element",
                AXIS2_HASH_KEY_STRING);

    binding_element->ops->get_binding_fault_elements =
        axis2_hash_get(methods, "get_binding_fault_elements",
                AXIS2_HASH_KEY_STRING);

    binding_element->ops->add_binding_op_element =
        axis2_hash_get(methods, "add_binding_op_element",
                AXIS2_HASH_KEY_STRING);

    binding_element->ops->get_binding_op_elements =
        axis2_hash_get(methods, "get_binding_op_elements",
                AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

