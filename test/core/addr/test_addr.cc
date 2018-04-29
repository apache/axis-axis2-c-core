
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

#include <axis2_msg_info_headers.h>
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axis2_endpoint_ref.h>
#include <stdio.h>


class TestAddr: public ::testing::Test
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

TEST_F(TestAddr, test_msg_info_headers)
{

    axis2_endpoint_ref_t *to = NULL;
    axis2_endpoint_ref_t *from = NULL;
    axis2_endpoint_ref_t *reply_to = NULL;
    axis2_endpoint_ref_t *fault_to = NULL;
    axis2_endpoint_ref_t *axis2_endpoint_ref = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_msg_info_headers_t *axis2_msg_info_headers = NULL;
    axis2_char_t *action = "test action";
    const axis2_char_t *get_action = NULL;
    to = axis2_endpoint_ref_create(m_env, "to");
    from = axis2_endpoint_ref_create(m_env, "from");
    reply_to = axis2_endpoint_ref_create(m_env, "reply to");
    fault_to = axis2_endpoint_ref_create(m_env, "fault to");

    axis2_msg_info_headers = axis2_msg_info_headers_create(m_env, NULL, action);
    ASSERT_NE(axis2_msg_info_headers, nullptr);
    status = axis2_msg_info_headers_set_to(axis2_msg_info_headers, m_env, to);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    axis2_endpoint_ref =
        axis2_msg_info_headers_get_to(axis2_msg_info_headers, m_env);
    ASSERT_NE(axis2_endpoint_ref, nullptr);

    status = axis2_msg_info_headers_set_from(axis2_msg_info_headers, m_env, from);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    axis2_endpoint_ref =
        axis2_msg_info_headers_get_from(axis2_msg_info_headers, m_env);
    ASSERT_NE(axis2_endpoint_ref, nullptr);

    axis2_endpoint_ref =
        axis2_msg_info_headers_get_reply_to(axis2_msg_info_headers, m_env);
    ASSERT_EQ(axis2_endpoint_ref, nullptr);

    status =
        axis2_msg_info_headers_set_reply_to(axis2_msg_info_headers, m_env, reply_to);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    axis2_endpoint_ref =
        axis2_msg_info_headers_get_reply_to(axis2_msg_info_headers, m_env);
    ASSERT_NE(axis2_endpoint_ref, nullptr);
    status =
        axis2_msg_info_headers_set_fault_to(axis2_msg_info_headers, m_env, fault_to);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    axis2_endpoint_ref =
        axis2_msg_info_headers_get_fault_to(axis2_msg_info_headers, m_env);
    ASSERT_NE(axis2_endpoint_ref, nullptr);

    get_action = axis2_msg_info_headers_get_action(axis2_msg_info_headers, m_env);
    ASSERT_STREQ(get_action, action);

    status =
        axis2_msg_info_headers_set_action(axis2_msg_info_headers, m_env, action);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    axis2_msg_info_headers_free(axis2_msg_info_headers, m_env);
}
