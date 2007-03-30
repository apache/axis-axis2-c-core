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

#include <woden_wsdl10_desc_element.h>

axis2_status_t AXIS2_CALL
woden_wsdl10_desc_element_resolve_methods(
    woden_wsdl10_desc_element_t *desc_element,
    const axutil_env_t *env,
    axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);

    desc_element->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    desc_element->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    desc_element->ops->set_document_base_uri = axis2_hash_get(methods,
            "set_document_base_uri", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_document_base_uri = axis2_hash_get(methods,
            "get_document_base_uri", AXIS2_HASH_KEY_STRING);
    desc_element->ops->set_target_namespace = axis2_hash_get(methods,
            "set_target_namespace", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_target_namespace = axis2_hash_get(methods,
            "get_target_namespace", AXIS2_HASH_KEY_STRING);
    desc_element->ops->add_namespace = axis2_hash_get(methods,
            "add_namespace", AXIS2_HASH_KEY_STRING);
    desc_element->ops->remove_namespace = axis2_hash_get(methods,
            "remove_namespace", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_namespace = axis2_hash_get(methods,
            "get_namespace", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_namespaces = axis2_hash_get(methods,
            "get_namespaces", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_documentation_element = axis2_hash_get(methods,
            "create_documentation_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_import_element = axis2_hash_get(methods,
            "create_import_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_include_element = axis2_hash_get(methods,
            "create_include_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_types_element = axis2_hash_get(methods,
            "create_types_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_msg_element = axis2_hash_get(methods,
            "create_msg_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_part_element = axis2_hash_get(methods,
            "create_part_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_interface_element = axis2_hash_get(methods,
            "create_interface_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_interface_op_element = axis2_hash_get(methods,
            "create_interface_op_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_interface_fault_ref_element = axis2_hash_get(methods,
            "create_interface_fault_ref_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_interface_msg_ref_element = axis2_hash_get(methods,
            "create_interface_msg_ref_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_binding_element = axis2_hash_get(methods,
            "create_binding_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_binding_op_element = axis2_hash_get(methods,
            "create_binding_op_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_binding_fault_ref_element = axis2_hash_get(methods,
            "create_binding_fault_ref_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_binding_msg_ref_element = axis2_hash_get(methods,
            "create_binding_msg_ref_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_feature_element = axis2_hash_get(methods,
            "create_feature_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_svc_element = axis2_hash_get(methods,
            "create_svc_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_endpoint_element = axis2_hash_get(methods,
            "create_endpoint_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->create_property_element = axis2_hash_get(methods,
            "create_property_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->add_import_element = axis2_hash_get(methods,
            "add_import_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_import_elements = axis2_hash_get(methods,
            "get_import_elements", AXIS2_HASH_KEY_STRING);
    desc_element->ops->add_include_element = axis2_hash_get(methods,
            "add_include_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_include_elements = axis2_hash_get(methods,
            "get_include_elements", AXIS2_HASH_KEY_STRING);
    desc_element->ops->set_types_element = axis2_hash_get(methods,
            "set_types_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_types_element = axis2_hash_get(methods,
            "get_types_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->add_msg_element = axis2_hash_get(methods,
            "add_msg_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_msg_elements = axis2_hash_get(methods,
            "get_msg_elements", AXIS2_HASH_KEY_STRING);
    desc_element->ops->add_interface_element = axis2_hash_get(methods,
            "add_interface_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_interface_elements = axis2_hash_get(methods,
            "get_interface_elements", AXIS2_HASH_KEY_STRING);
    desc_element->ops->add_binding_element = axis2_hash_get(methods,
            "add_binding_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_binding_elements = axis2_hash_get(methods,
            "get_binding_elements", AXIS2_HASH_KEY_STRING);
    desc_element->ops->add_svc_element = axis2_hash_get(methods,
            "add_svc_element", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_svc_elements = axis2_hash_get(methods,
            "get_svc_elements", AXIS2_HASH_KEY_STRING);
    desc_element->ops->set_ext_registry = axis2_hash_get(methods,
            "set_ext_registry", AXIS2_HASH_KEY_STRING);
    desc_element->ops->get_ext_registry = axis2_hash_get(methods,
            "get_ext_registry", AXIS2_HASH_KEY_STRING);
    desc_element->ops->to_component = axis2_hash_get(methods,
            "to_component", AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

