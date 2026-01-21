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
 * @file test_phase.cc
 * @brief Unit tests for axis2_phase, including AXIS2C-1294 duplicate handler fix
 */

#include <gtest/gtest.h>

#include <stdio.h>
#include <axutil_env.h>
#include <axis2_phase.h>
#include <axis2_handler.h>
#include <axis2_handler_desc.h>
#include <axis2_phase_rule.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>

class TestPhase : public ::testing::Test
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

/* Dummy handler invoke function for testing */
static axis2_status_t AXIS2_CALL
dummy_handler_invoke(
    axis2_handler_t *handler,
    const axutil_env_t *env,
    struct axis2_msg_ctx *msg_ctx)
{
    return AXIS2_SUCCESS;
}

/* Helper to create a test handler with given name */
static axis2_handler_t *
create_test_handler(
    const axutil_env_t *env,
    const axis2_char_t *name)
{
    axis2_handler_t *handler = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    axutil_string_t *handler_name = NULL;

    handler_name = axutil_string_create(env, name);
    handler_desc = axis2_handler_desc_create(env, handler_name);
    axutil_string_free(handler_name, env);

    handler = axis2_handler_create(env);
    axis2_handler_init(handler, env, handler_desc);
    axis2_handler_set_invoke(handler, env, dummy_handler_invoke);
    axis2_handler_desc_set_handler(handler_desc, env, handler);

    return handler;
}

/* Helper to create handler with phase rule */
static axis2_handler_t *
create_handler_with_rule(
    const axutil_env_t *env,
    const axis2_char_t *name,
    const axis2_char_t *phase_name,
    const axis2_char_t *before,
    const axis2_char_t *after)
{
    axis2_handler_t *handler = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    axis2_phase_rule_t *phase_rule = NULL;
    axutil_string_t *handler_name = NULL;

    handler_name = axutil_string_create(env, name);
    handler_desc = axis2_handler_desc_create(env, handler_name);
    axutil_string_free(handler_name, env);

    phase_rule = axis2_handler_desc_get_rules(handler_desc, env);
    axis2_phase_rule_set_name(phase_rule, env, phase_name);

    if (before)
    {
        axis2_phase_rule_set_before(phase_rule, env, before);
    }
    if (after)
    {
        axis2_phase_rule_set_after(phase_rule, env, after);
    }

    handler = axis2_handler_create(env);
    axis2_handler_init(handler, env, handler_desc);
    axis2_handler_set_invoke(handler, env, dummy_handler_invoke);
    axis2_handler_desc_set_handler(handler_desc, env, handler);

    return handler;
}

/**
 * Test basic phase creation
 */
TEST_F(TestPhase, test_phase_create)
{
    axis2_phase_t *phase = NULL;
    const axis2_char_t *name = NULL;

    phase = axis2_phase_create(m_env, "TestPhase");
    ASSERT_NE(phase, nullptr);

    name = axis2_phase_get_name(phase, m_env);
    ASSERT_STREQ(name, "TestPhase");

    axis2_phase_free(phase, m_env);
}

/**
 * Test adding handler to phase
 */
TEST_F(TestPhase, test_phase_add_handler)
{
    axis2_phase_t *phase = NULL;
    axis2_handler_t *handler = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    phase = axis2_phase_create(m_env, "TestPhase");
    ASSERT_NE(phase, nullptr);

    handler = create_test_handler(m_env, "TestHandler");
    ASSERT_NE(handler, nullptr);

    status = axis2_phase_add_handler(phase, m_env, handler);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    ASSERT_EQ(axis2_phase_get_handler_count(phase, m_env), 1);

    axis2_phase_free(phase, m_env);
}

/**
 * AXIS2C-1294: Test that duplicate handlers are NOT inserted when using
 * axis2_phase_add_handler (basic add_unique test)
 */
TEST_F(TestPhase, test_axis2c_1294_no_duplicate_basic)
{
    axis2_phase_t *phase = NULL;
    axis2_handler_t *handler = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    int count = 0;

    phase = axis2_phase_create(m_env, "Transport");
    ASSERT_NE(phase, nullptr);

    handler = create_test_handler(m_env, "DiscoveryHandler");
    ASSERT_NE(handler, nullptr);

    /* Add handler first time */
    status = axis2_phase_add_handler(phase, m_env, handler);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    count = axis2_phase_get_handler_count(phase, m_env);
    ASSERT_EQ(count, 1);

    /* Try to add the same handler again - should NOT increase count */
    status = axis2_phase_add_handler(phase, m_env, handler);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    count = axis2_phase_get_handler_count(phase, m_env);
    ASSERT_EQ(count, 1) << "AXIS2C-1294: Duplicate handler was inserted!";

    axis2_phase_free(phase, m_env);
}

/**
 * AXIS2C-1294: Test that duplicate handlers are NOT inserted when using
 * the 'after' positioning rule - this was the original bug scenario
 */
TEST_F(TestPhase, test_axis2c_1294_no_duplicate_with_after_rule)
{
    axis2_phase_t *phase = NULL;
    axis2_handler_t *base_handler = NULL;
    axis2_handler_t *after_handler = NULL;
    axis2_handler_desc_t *after_desc = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    int count = 0;

    phase = axis2_phase_create(m_env, "Transport");
    ASSERT_NE(phase, nullptr);

    /* Create base handler that other handlers reference with "after" */
    base_handler = create_test_handler(m_env, "addressing_based_dispatcher");
    ASSERT_NE(base_handler, nullptr);

    status = axis2_phase_add_handler(phase, m_env, base_handler);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    ASSERT_EQ(axis2_phase_get_handler_count(phase, m_env), 1);

    /* Create handler with "after" rule - simulates AXIS2C-1294 scenario */
    after_handler = create_handler_with_rule(
        m_env,
        "DiscoveryDuplicateInHandler",
        "Transport",
        NULL,                           /* before */
        "addressing_based_dispatcher"   /* after */
    );
    ASSERT_NE(after_handler, nullptr);

    after_desc = axis2_handler_get_handler_desc(after_handler, m_env);
    ASSERT_NE(after_desc, nullptr);

    /* Add handler with "after" rule first time */
    status = axis2_phase_add_handler_desc(phase, m_env, after_desc);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    count = axis2_phase_get_handler_count(phase, m_env);
    ASSERT_EQ(count, 2);

    /* Try to add the same handler again (simulates multiple engagement paths) */
    status = axis2_phase_add_handler_desc(phase, m_env, after_desc);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    count = axis2_phase_get_handler_count(phase, m_env);
    ASSERT_EQ(count, 2) << "AXIS2C-1294: Handler with 'after' rule was duplicated!";

    axis2_phase_free(phase, m_env);
}

/**
 * AXIS2C-1294: Test that duplicate handlers are NOT inserted when using
 * the 'before' positioning rule
 */
TEST_F(TestPhase, test_axis2c_1294_no_duplicate_with_before_rule)
{
    axis2_phase_t *phase = NULL;
    axis2_handler_t *base_handler = NULL;
    axis2_handler_t *before_handler = NULL;
    axis2_handler_desc_t *before_desc = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    int count = 0;

    phase = axis2_phase_create(m_env, "Transport");
    ASSERT_NE(phase, nullptr);

    /* Create base handler that other handlers reference with "before" */
    base_handler = create_test_handler(m_env, "final_handler");
    ASSERT_NE(base_handler, nullptr);

    status = axis2_phase_add_handler(phase, m_env, base_handler);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    ASSERT_EQ(axis2_phase_get_handler_count(phase, m_env), 1);

    /* Create handler with "before" rule */
    before_handler = create_handler_with_rule(
        m_env,
        "PreFinalHandler",
        "Transport",
        "final_handler",  /* before */
        NULL              /* after */
    );
    ASSERT_NE(before_handler, nullptr);

    before_desc = axis2_handler_get_handler_desc(before_handler, m_env);
    ASSERT_NE(before_desc, nullptr);

    /* Add handler with "before" rule first time */
    status = axis2_phase_add_handler_desc(phase, m_env, before_desc);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    count = axis2_phase_get_handler_count(phase, m_env);
    ASSERT_EQ(count, 2);

    /* Try to add the same handler again */
    status = axis2_phase_add_handler_desc(phase, m_env, before_desc);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    count = axis2_phase_get_handler_count(phase, m_env);
    ASSERT_EQ(count, 2) << "AXIS2C-1294: Handler with 'before' rule was duplicated!";

    axis2_phase_free(phase, m_env);
}

/**
 * AXIS2C-1294: Test that duplicate handlers are NOT inserted when using
 * both 'before' and 'after' positioning rules
 */
TEST_F(TestPhase, test_axis2c_1294_no_duplicate_with_before_and_after_rule)
{
    axis2_phase_t *phase = NULL;
    axis2_handler_t *first_handler = NULL;
    axis2_handler_t *last_handler = NULL;
    axis2_handler_t *middle_handler = NULL;
    axis2_handler_desc_t *middle_desc = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    int count = 0;

    phase = axis2_phase_create(m_env, "Transport");
    ASSERT_NE(phase, nullptr);

    /* Create first handler */
    first_handler = create_test_handler(m_env, "FirstHandler");
    ASSERT_NE(first_handler, nullptr);
    status = axis2_phase_add_handler(phase, m_env, first_handler);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    /* Create last handler */
    last_handler = create_test_handler(m_env, "LastHandler");
    ASSERT_NE(last_handler, nullptr);
    status = axis2_phase_add_handler(phase, m_env, last_handler);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    ASSERT_EQ(axis2_phase_get_handler_count(phase, m_env), 2);

    /* Create handler with both "before" and "after" rules */
    middle_handler = create_handler_with_rule(
        m_env,
        "MiddleHandler",
        "Transport",
        "LastHandler",   /* before */
        "FirstHandler"   /* after */
    );
    ASSERT_NE(middle_handler, nullptr);

    middle_desc = axis2_handler_get_handler_desc(middle_handler, m_env);
    ASSERT_NE(middle_desc, nullptr);

    /* Add handler with both rules first time */
    status = axis2_phase_add_handler_desc(phase, m_env, middle_desc);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    count = axis2_phase_get_handler_count(phase, m_env);
    ASSERT_EQ(count, 3);

    /* Try to add the same handler again */
    status = axis2_phase_add_handler_desc(phase, m_env, middle_desc);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    count = axis2_phase_get_handler_count(phase, m_env);
    ASSERT_EQ(count, 3) << "AXIS2C-1294: Handler with 'before' and 'after' rules was duplicated!";

    axis2_phase_free(phase, m_env);
}

/**
 * AXIS2C-1294: Test that handlers with the same name (but different instances)
 * are also prevented from duplicate insertion
 */
TEST_F(TestPhase, test_axis2c_1294_no_duplicate_by_name)
{
    axis2_phase_t *phase = NULL;
    axis2_handler_t *handler1 = NULL;
    axis2_handler_t *handler2 = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    int count = 0;

    phase = axis2_phase_create(m_env, "Transport");
    ASSERT_NE(phase, nullptr);

    /* Create first handler instance with name */
    handler1 = create_test_handler(m_env, "SameNameHandler");
    ASSERT_NE(handler1, nullptr);

    /* Create second handler instance with the SAME name */
    handler2 = create_test_handler(m_env, "SameNameHandler");
    ASSERT_NE(handler2, nullptr);

    /* Verify they are different instances */
    ASSERT_NE(handler1, handler2);

    /* Add first handler */
    status = axis2_phase_add_handler(phase, m_env, handler1);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    count = axis2_phase_get_handler_count(phase, m_env);
    ASSERT_EQ(count, 1);

    /* Try to add second handler with same name - should NOT increase count */
    status = axis2_phase_add_handler(phase, m_env, handler2);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    count = axis2_phase_get_handler_count(phase, m_env);
    ASSERT_EQ(count, 1) << "AXIS2C-1294: Handler with duplicate name was inserted!";

    axis2_phase_free(phase, m_env);
}
