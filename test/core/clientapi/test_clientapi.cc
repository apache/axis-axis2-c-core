
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

#include <gtest/gtest.h>

#include <axis2_const.h>
#include <axutil_env.h>
#include <axis2_engine.h>
#include <axutil_allocator.h>
#include <axis2_svc_client.h>
#include <axis2_options.h>
#include <platforms/axutil_platform_auto_sense.h>

axiom_node_t *build_om_payload_for_echo_svc(
    const axutil_env_t * env,
    const axis2_char_t * echo_str);

class TestClientAPI: public ::testing::Test
{

    protected:
        void SetUp()
        {

            m_allocator = axutil_allocator_init(NULL);
            m_axis_log = axutil_log_create(m_allocator, NULL, NULL);
            m_error = axutil_error_create(m_allocator);

            m_env = axutil_env_create_with_error_log(m_allocator, m_error, m_axis_log);

        }

        void TearDown()
        {
            axutil_env_free(m_env);
        }


        axutil_allocator_t *m_allocator = NULL;
        axutil_env_t *m_env = NULL;
        axutil_error_t *m_error = NULL;
        axutil_log_t *m_axis_log = NULL;

};

/* Note: This test fails unless you have a deployed axis2c instance running the
 * echo service on the appropriate port, and AXIS2C_HOME defined in your
 * environment */
TEST_F(TestClientAPI, test_svc_client_blocking)
{
    axis2_options_t *options = NULL;
    const axis2_char_t *client_home = NULL;
    axis2_svc_client_t *svc_client = NULL;
    axiom_node_t *payload = NULL;
    axiom_node_t *ret_node = NULL;
    const axis2_char_t *address = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;

    axiom_element_t *result_ele = NULL;
    const axis2_char_t *echo_text = "echo_text";
    axis2_char_t *result = NULL;

    address = "http://localhost:9090/axis2/services/echo/echo";
    endpoint_ref = axis2_endpoint_ref_create(m_env, address);
    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../deploy";

    svc_client = axis2_svc_client_create(m_env, client_home);
    ASSERT_NE(svc_client, nullptr);
    if (!svc_client)
    {
        printf("axis2_test axis2_svc_client_create FAILURE\n");
        printf
            ("Client repository path not properly set. Please check AXIS2C_HOME setting\n");
        return;
    }

    options = axis2_options_create(m_env);
    axis2_options_set_to(options, m_env, endpoint_ref);
    axis2_svc_client_set_options(svc_client, m_env, options);

    payload = build_om_payload_for_echo_svc(m_env, echo_text);
    ret_node = axis2_svc_client_send_receive(svc_client, m_env, payload);
    if (ret_node)
    {
        if (axiom_node_get_node_type(ret_node, m_env) == AXIOM_ELEMENT)
        {
            ret_node = axiom_node_get_first_child(ret_node, m_env);
            result_ele =
                (axiom_element_t *) axiom_node_get_data_element(ret_node, m_env);
            result = axiom_element_get_text(result_ele, m_env, ret_node);
            if (!strcmp(result, echo_text))
                printf("axis2_test SVC_CLIENT_SEND_RECEIVE SUCCESS\n");
            else
                printf("axis2_test SVC_CLIENT_SEND_RECEIVE FAILURE\n");
        }
    }

    axis2_svc_client_free(svc_client, m_env);
}

/* build SOAP request message content using OM */
axiom_node_t *
build_om_payload_for_echo_svc(
    const axutil_env_t * env,
    const axis2_char_t * echo_text)
{
    axiom_node_t *echo_om_node = NULL;
    /*axiom_element_t *echo_om_ele = NULL; */
    axiom_node_t *text_om_node = NULL;
    axiom_element_t *text_om_ele = NULL;
    axiom_namespace_t *ns1 = NULL;

    ns1 =
        axiom_namespace_create(env, "http://ws.apache.org/axis2/c/samples",
                               "ns1");
    /*echo_om_ele =*/
        axiom_element_create(env, NULL, "echoString", ns1, &echo_om_node);
    text_om_ele =
        axiom_element_create(env, echo_om_node, "text", NULL, &text_om_node);
    axiom_element_set_text(text_om_ele, env, echo_text, text_om_node);

    return echo_om_node;
}

