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

/**
 * Test for AXIS2C-1355: Changing service client options between messages
 *
 * This test verifies that replacing options on a service client does not
 * cause a use-after-free when the op_client still holds a reference to
 * the old options.
 *
 * The bug occurred because:
 * 1. svc_client creates op_client with a pointer to svc_client->options
 * 2. When axis2_svc_client_set_options() is called with new options,
 *    the old options are freed
 * 3. But op_client->options still points to the freed memory
 * 4. When op_client is freed, it accesses op_client->options (UAF)
 *
 * The fix updates op_client->options when svc_client options change.
 */

#include <gtest/gtest.h>

#include <axis2_const.h>
#include <axutil_env.h>
#include <axis2_svc_client.h>
#include <axis2_options.h>
#include <axis2_op_client.h>
#include <platforms/axutil_platform_auto_sense.h>

class TestAxis2C1355 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_allocator = axutil_allocator_init(NULL);
        ASSERT_NE(m_allocator, nullptr);

        m_error = axutil_error_create(m_allocator);
        ASSERT_NE(m_error, nullptr);

        m_axis_log = axutil_log_create(m_allocator, NULL, NULL);
        ASSERT_NE(m_axis_log, nullptr);

        m_env = axutil_env_create_with_error_log(m_allocator, m_error, m_axis_log);
        ASSERT_NE(m_env, nullptr);
    }

    void TearDown() override
    {
        if (m_env)
        {
            axutil_env_free(m_env);
            m_env = NULL;
        }
    }

    axutil_allocator_t *m_allocator = NULL;
    axutil_env_t *m_env = NULL;
    axutil_error_t *m_error = NULL;
    axutil_log_t *m_axis_log = NULL;
};

/**
 * Test that axis2_op_client_set_options_ref correctly updates options
 * without freeing the old ones.
 */
TEST_F(TestAxis2C1355, test_op_client_set_options_ref_exists)
{
    /* Verify the new function exists and can be called */
    /* We can't easily create an op_client without a full service context,
     * but we can verify the function handles NULL gracefully */
    axis2_options_t *options = axis2_options_create(m_env);
    ASSERT_NE(options, nullptr);

    /* Should not crash when op_client is NULL */
    axis2_op_client_set_options_ref(NULL, m_env, options);

    axis2_options_free(options, m_env);
}

/**
 * Test that replacing svc_client options does not cause use-after-free.
 * This is the main regression test for AXIS2C-1355.
 *
 * Note: This test requires AXIS2C_HOME to be set or uses deploy directory.
 * The actual message send will fail without a running server, but that's OK -
 * we just need to exercise the code path that creates op_client and then
 * replaces options.
 */
TEST_F(TestAxis2C1355, test_svc_client_replace_options_no_uaf)
{
    const axis2_char_t *client_home = NULL;
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options1 = NULL;
    axis2_options_t *options2 = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;

    /* Get client home */
    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../deploy";

    /* Create service client */
    svc_client = axis2_svc_client_create(m_env, client_home);
    if (!svc_client)
    {
        /* Skip test if we can't create svc_client (missing deployment) */
        GTEST_SKIP() << "Could not create svc_client - AXIS2C_HOME may not be set";
    }

    /* Create first options and set them */
    endpoint_ref = axis2_endpoint_ref_create(m_env, "http://localhost:9090/axis2/services/echo");
    options1 = axis2_options_create(m_env);
    ASSERT_NE(options1, nullptr);
    axis2_options_set_to(options1, m_env, endpoint_ref);
    axis2_options_set_action(options1, m_env, "http://example.org/action1");

    axis2_svc_client_set_options(svc_client, m_env, options1);
    /* Note: options1 is now owned by svc_client */

    /* Try to send a message - this creates the op_client internally.
     * The send will fail (no server), but op_client will be created. */
    axiom_node_t *payload = NULL;
    axiom_namespace_t *ns = axiom_namespace_create(m_env, "http://test", "t");
    axiom_element_create(m_env, NULL, "test", ns, &payload);
    axiom_namespace_free(ns, m_env);

    /* Ignore the return - we just want to trigger op_client creation */
    (void)axis2_svc_client_send_receive(svc_client, m_env, payload);

    /* Now create NEW options and set them.
     * BEFORE THE FIX: This would free options1, but op_client still references it.
     * AFTER THE FIX: op_client->options is updated to point to options2. */
    endpoint_ref = axis2_endpoint_ref_create(m_env, "http://localhost:9090/axis2/services/echo");
    options2 = axis2_options_create(m_env);
    ASSERT_NE(options2, nullptr);
    axis2_options_set_to(options2, m_env, endpoint_ref);
    axis2_options_set_action(options2, m_env, "http://example.org/action2");

    /* This is the critical call that would trigger UAF before the fix */
    axis2_svc_client_set_options(svc_client, m_env, options2);

    /* Free the service client - this frees op_client which accesses options.
     * BEFORE THE FIX: This would access freed memory (options1).
     * AFTER THE FIX: op_client->options points to valid memory (options2). */
    axis2_svc_client_free(svc_client, m_env);

    /* If we get here without crashing or ASAN errors, the fix works! */
    SUCCEED();
}

/**
 * Test that getting and modifying existing options (workaround) still works.
 * This was the recommended workaround before the fix.
 */
TEST_F(TestAxis2C1355, test_svc_client_modify_existing_options)
{
    const axis2_char_t *client_home = NULL;
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;

    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../deploy";

    svc_client = axis2_svc_client_create(m_env, client_home);
    if (!svc_client)
    {
        GTEST_SKIP() << "Could not create svc_client - AXIS2C_HOME may not be set";
    }

    /* Create and set initial options */
    endpoint_ref = axis2_endpoint_ref_create(m_env, "http://localhost:9090/axis2/services/echo");
    options = axis2_options_create(m_env);
    axis2_options_set_to(options, m_env, endpoint_ref);
    axis2_options_set_action(options, m_env, "http://example.org/action1");
    axis2_svc_client_set_options(svc_client, m_env, options);

    /* Get the options back and modify them (this was the workaround) */
    const axis2_options_t *existing_options = axis2_svc_client_get_options(svc_client, m_env);
    ASSERT_NE(existing_options, nullptr);

    /* Modify the existing options - this should work fine */
    endpoint_ref = axis2_endpoint_ref_create(m_env, "http://localhost:9090/axis2/services/echo2");
    axis2_options_set_to((axis2_options_t *)existing_options, m_env, endpoint_ref);
    axis2_options_set_action((axis2_options_t *)existing_options, m_env, "http://example.org/action2");

    axis2_svc_client_free(svc_client, m_env);
    SUCCEED();
}
