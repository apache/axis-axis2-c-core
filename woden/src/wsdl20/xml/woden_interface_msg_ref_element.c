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

#include <woden_interface_msg_ref_element.h>

axis2_status_t AXIS2_CALL
woden_interface_msg_ref_element_resolve_methods(
    woden_interface_msg_ref_element_t *interface_msg_ref_element,
    const axutil_env_t *env,
    axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);

    interface_msg_ref_element->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    interface_msg_ref_element->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    interface_msg_ref_element->ops->set_msg_label =
        axis2_hash_get(methods, "set_msg_label",
                AXIS2_HASH_KEY_STRING);

    interface_msg_ref_element->ops->get_msg_label =
        axis2_hash_get(methods, "get_msg_label",
                AXIS2_HASH_KEY_STRING);

    interface_msg_ref_element->ops->set_msg_content_model =
        axis2_hash_get(methods, "set_msg_content_model",
                AXIS2_HASH_KEY_STRING);

    interface_msg_ref_element->ops->get_msg_content_model =
        axis2_hash_get(methods, "get_msg_cotent_model",
                AXIS2_HASH_KEY_STRING);

    interface_msg_ref_element->ops->set_element_qname =
        axis2_hash_get(methods, "set_element_qname",
                AXIS2_HASH_KEY_STRING);

    interface_msg_ref_element->ops->get_element_qname =
        axis2_hash_get(methods, "get_element_qname",
                AXIS2_HASH_KEY_STRING);


    interface_msg_ref_element->ops->get_element =
        axis2_hash_get(methods, "get_element",
                AXIS2_HASH_KEY_STRING);


    interface_msg_ref_element->ops->set_direction =
        axis2_hash_get(methods, "set_direction",
                AXIS2_HASH_KEY_STRING);

    interface_msg_ref_element->ops->get_direction =
        axis2_hash_get(methods, "get_direction",
                AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

