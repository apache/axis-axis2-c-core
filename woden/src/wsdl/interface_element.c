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

#include <woden_interface_element.h>

axis2_status_t AXIS2_CALL
woden_interface_element_resolve_methods(
    woden_interface_element_t *interface_element,
    const axis2_env_t *env,
    axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);

    interface_element->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);

    interface_element->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    interface_element->ops->set_qname =
        axis2_hash_get(methods, "set_qname",
                AXIS2_HASH_KEY_STRING);

    interface_element->ops->get_qname =
        axis2_hash_get(methods, "get_qname",
                AXIS2_HASH_KEY_STRING);

    interface_element->ops->add_extends_qname =
        axis2_hash_get(methods, "add_extends_qname",
                AXIS2_HASH_KEY_STRING);

    interface_element->ops->get_extends_qnames =
        axis2_hash_get(methods, "get_extends_qnames",
                AXIS2_HASH_KEY_STRING);

    interface_element->ops->add_style_default_uri =
        axis2_hash_get(methods, "add_style_default_uri",
                AXIS2_HASH_KEY_STRING);

    interface_element->ops->get_style_default =
        axis2_hash_get(methods, "get_style_default",
                AXIS2_HASH_KEY_STRING);

    interface_element->ops->add_interface_fault_element =
        axis2_hash_get(methods, "add_interface_fault_element",
                AXIS2_HASH_KEY_STRING);

    interface_element->ops->get_interface_fault_elements =
        axis2_hash_get(methods, "get_interface_fault_elements",
                AXIS2_HASH_KEY_STRING);

    interface_element->ops->add_interface_op_element =
        axis2_hash_get(methods, "add_interface_op_element",
                AXIS2_HASH_KEY_STRING);

    interface_element->ops->get_interface_op_elements =
        axis2_hash_get(methods, "get_interface_op_elements",
                AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

