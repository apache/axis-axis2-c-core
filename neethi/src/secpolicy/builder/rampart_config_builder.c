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

#include <rp_rampart_config_builder.h>

/*private functions*/

axis2_status_t AXIS2_CALL rp_rampart_config_builder_populate(
    const axutil_env_t *env,
    rp_rampart_config_t *rampart_config,
    axiom_node_t *node,
    axiom_element_t *element,
    axis2_char_t *local_name);

AXIS2_EXTERN neethi_assertion_t *AXIS2_CALL
rp_rampart_config_builder_build(
    const axutil_env_t *env,
    axiom_node_t *config,
    axiom_element_t *config_ele)
{
    rp_rampart_config_t *rampart_config = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axiom_children_iterator_t *children_iter = NULL;
    neethi_assertion_t *assertion = NULL;

    rampart_config = rp_rampart_config_create(env);
    if (!rampart_config)
    {
        return NULL;
    }

    children_iter = axiom_element_get_children(config_ele, env, config);
    if (children_iter)
    {
        while (axiom_children_iterator_has_next(children_iter, env))
        {
            axiom_node_t *node = NULL;
            axiom_element_t *ele = NULL;
            axis2_char_t *local_name = NULL;
            node = axiom_children_iterator_next(children_iter, env);
            if (node)
            {
                if (axiom_node_get_node_type(node, env) == AXIOM_ELEMENT)
                {
                    ele =
                        (axiom_element_t *) axiom_node_get_data_element(node,
                                                                        env);
                    if (ele)
                    {
                        local_name = axiom_element_get_localname(ele, env);
                        if (local_name)
                        {
                            status =
                                rp_rampart_config_builder_populate(env,
                                                                   rampart_config,
                                                                   node, ele,
                                                                   local_name);
                            if (status != AXIS2_SUCCESS)
                            {
                                return NULL;
                            }
                        }
                    }
                }
            }
        }
    }
    assertion =
        neethi_assertion_create_with_args(env, (AXIS2_FREE_VOID_ARG)rp_rampart_config_free,
                                          rampart_config,
                                          ASSERTION_TYPE_RAMPART_CONFIG);
    return assertion;
}

axis2_status_t AXIS2_CALL
rp_rampart_config_builder_populate(
    const axutil_env_t *env,
    rp_rampart_config_t *rampart_config,
    axiom_node_t *node,
    axiom_element_t *element,
    axis2_char_t *local_name)
{
    axis2_char_t *ns = NULL;
    axutil_qname_t *node_qname = NULL;

    node_qname = axiom_element_get_qname(element, env, node);
    if(!node_qname)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Cannot get qname from element %s.", local_name);
        return AXIS2_FAILURE;
    }

    ns = axutil_qname_get_uri(node_qname, env);
    if(!ns)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Cannot get namespace from element %s.", local_name);
        return AXIS2_FAILURE;
    }

    if(!axutil_strcmp(ns, RP_RAMPART_NS))
    {
        if(!axutil_strcmp(local_name, RP_USER))
        {
            axis2_char_t *user = NULL;
            user = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_user(rampart_config, env, user);
        }
        else if(!axutil_strcmp(local_name, RP_ENCRYPTION_USER))
        {
            axis2_char_t *encryption_user = NULL;
            encryption_user = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_encryption_user(rampart_config, env,encryption_user);
        }
        else if(!axutil_strcmp(local_name, RP_PASSWORD_CALLBACK_CLASS))
        {
            axis2_char_t *password_callback_class = NULL;
            password_callback_class = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_password_callback_class(
                rampart_config, env, password_callback_class);
        }
        else if(!axutil_strcmp(local_name, RP_AUTHN_MODULE_NAME))
        {
            axis2_char_t *authenticate_module = NULL;
            authenticate_module = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_authenticate_module(
                rampart_config, env, authenticate_module);
        }
        else if(!axutil_strcmp(local_name, RP_RD_MODULE))
        {
            axis2_char_t *replay_detector = NULL;
            replay_detector = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_replay_detector(rampart_config, env, replay_detector);
        }
        else if(!axutil_strcmp(local_name, RP_SCT_MODULE))
        {
            axis2_char_t *sct_module = NULL;
            sct_module = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_sct_provider(rampart_config, env, sct_module);
        }
        else if(!axutil_strcmp(local_name, RP_PASSWORD_TYPE))
        {
            axis2_char_t *password_type = NULL;
            password_type = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_password_type(rampart_config, env, password_type);
        }
        else if(!axutil_strcmp(local_name, RP_CERTIFICATE))
        {
            axis2_char_t *certificate_file = NULL;
            certificate_file = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_certificate_file(rampart_config, env, certificate_file);
        }
        else if(!axutil_strcmp(local_name, RP_RECEIVER_CERTIFICATE))
        {
            axis2_char_t *receiver_certificate_file = NULL;
            receiver_certificate_file = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_receiver_certificate_file(
                rampart_config, env, receiver_certificate_file);
        }
        else if(!axutil_strcmp(local_name, RP_PRIVATE_KEY))
        {
            axis2_char_t *private_key_file = NULL;
            private_key_file = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_private_key_file(rampart_config, env, private_key_file);
        }
        else if(!axutil_strcmp(local_name, RP_PKCS12_KEY_STORE))
        {
            axis2_char_t *pkcs12_key_store = NULL;
            pkcs12_key_store = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_pkcs12_file(rampart_config, env, pkcs12_key_store);
        }
        else if(!axutil_strcmp(local_name, RP_TIME_TO_LIVE))
        {
            axis2_char_t *time_to_live = NULL;
            time_to_live = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_time_to_live(rampart_config, env, time_to_live);
        }
        else if(!axutil_strcmp(local_name, RP_CLOCK_SKEW_BUFFER))
        {
            axis2_char_t *clock_skew_buffer = NULL;
            clock_skew_buffer = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_clock_skew_buffer(rampart_config, env, clock_skew_buffer);
        }
        else if(!axutil_strcmp(local_name, RP_NEED_MILLISECOND_PRECISION))
        {
            axis2_char_t *need_ms_precision = NULL;
            need_ms_precision = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_need_millisecond_precision(
                rampart_config, env, need_ms_precision);
        }
        else if(!axutil_strcmp(local_name, RP_RD))
        {
            axis2_char_t *rd_val = NULL;
            rd_val = axiom_element_get_text(element, env, node);
            return rp_rampart_config_set_rd_val(rampart_config, env, rd_val);
        }
    }

    /* either the assertion or the namespace is not identified */
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
        "[neethi] Unknown Assertion %s with namespace %s", local_name, ns);
    return AXIS2_FAILURE;


}
