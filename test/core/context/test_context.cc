
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

#include <axis2_conf_ctx.h>
#include <axis2_svc_grp.h>
#include <axis2_const.h>
#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <stdio.h>

class TestContext: public ::testing::Test
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

TEST_F(TestContext, test_conf_ctx_init)
{

    struct axis2_conf *conf = NULL;
    struct axis2_svc_grp_ctx *svc_grp_ctx1 = NULL;
    struct axis2_svc_grp_ctx *svc_grp_ctx2 = NULL;
    struct axis2_svc_grp *svc_grp1 = NULL;
    struct axis2_svc_grp *svc_grp2 = NULL;
    struct axis2_conf_ctx *conf_ctx = NULL;
    struct axis2_svc_ctx *svc_ctx1 = NULL;
    struct axis2_svc_ctx *svc_ctx2 = NULL;
    struct axis2_svc *svc1 = NULL;
    struct axis2_svc *svc2 = NULL;
    struct axutil_qname *qname1 = NULL;
    struct axutil_qname *qname2 = NULL;
    struct axis2_op_ctx *op_ctx1 = NULL;
    struct axis2_op_ctx *op_ctx2 = NULL;
    struct axis2_op *op = NULL;
    struct axutil_hash_t *op_ctx_map = NULL;
    struct axutil_hash_t *svc_ctx_map = NULL;
    struct axutil_hash_t *svc_grp_ctx_map = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    conf = axis2_conf_create(m_env);
    ASSERT_NE(conf, nullptr);

    conf_ctx = axis2_conf_ctx_create(m_env, conf);
    ASSERT_NE(conf_ctx, nullptr);
	
    svc_grp1 = axis2_svc_grp_create(m_env);
    ASSERT_NE(svc_grp1, nullptr);
    svc_grp2 = axis2_svc_grp_create(m_env);
    ASSERT_NE(svc_grp2, nullptr);

    svc_grp_ctx1 = axis2_svc_grp_ctx_create(m_env, svc_grp1, conf_ctx);
    ASSERT_NE(svc_grp_ctx1, nullptr);
    svc_grp_ctx2 = axis2_svc_grp_ctx_create(m_env, svc_grp2, conf_ctx);
    ASSERT_NE(svc_grp_ctx2, nullptr);

    qname1 = axutil_qname_create(m_env, "name1", NULL, NULL);
    ASSERT_NE(qname1, nullptr);
    qname2 = axutil_qname_create(m_env, "name2", NULL, NULL);
    ASSERT_NE(qname2, nullptr);

    svc1 = axis2_svc_create_with_qname(m_env, qname1);
    ASSERT_NE(svc1, nullptr);
    svc2 = axis2_svc_create_with_qname(m_env, qname2);
    ASSERT_NE(svc2, nullptr);

    svc_ctx1 = axis2_svc_ctx_create(m_env, svc1, svc_grp_ctx1);
    ASSERT_NE(svc_ctx1, nullptr);
    svc_ctx2 = axis2_svc_ctx_create(m_env, svc2, svc_grp_ctx2);
    ASSERT_NE(svc_ctx1, nullptr);

    op = axis2_op_create(m_env);
    ASSERT_NE(op, nullptr);

    op_ctx1 = axis2_op_ctx_create(m_env, op, svc_ctx1);
    ASSERT_NE(op_ctx1, nullptr);

    op_ctx2 = axis2_op_ctx_create(m_env, op, svc_ctx2);
    ASSERT_NE(op_ctx2, nullptr);

    op_ctx_map = axis2_conf_ctx_get_op_ctx_map(conf_ctx, m_env);
    ASSERT_NE(op_ctx_map, nullptr);
    if (op_ctx_map)
    {
        axutil_hash_set(op_ctx_map, "op_ctx1", AXIS2_HASH_KEY_STRING, op_ctx1);
        axutil_hash_set(op_ctx_map, "op_ctx2", AXIS2_HASH_KEY_STRING, op_ctx2);
    }

    svc_ctx_map = axis2_conf_ctx_get_svc_ctx_map(conf_ctx, m_env);
    ASSERT_NE(svc_ctx_map, nullptr);
    if (svc_ctx_map)
    {
        axutil_hash_set(svc_ctx_map, "svc_ctx1", AXIS2_HASH_KEY_STRING,
                        svc_ctx1);
        axutil_hash_set(svc_ctx_map, "svc_ctx2", AXIS2_HASH_KEY_STRING,
                        svc_ctx2);
    }

    svc_grp_ctx_map = axis2_conf_ctx_get_svc_grp_ctx_map(conf_ctx, m_env);
    ASSERT_NE(svc_grp_ctx_map, nullptr);

    if (svc_grp_ctx_map)
    {
        axutil_hash_set(svc_ctx_map, "svc_grp_ctx1", AXIS2_HASH_KEY_STRING,
                        svc_grp_ctx1);
        axutil_hash_set(svc_ctx_map, "svc_grp_ctx2", AXIS2_HASH_KEY_STRING,
                        svc_grp_ctx2);
    }

    status = axis2_conf_ctx_init(conf_ctx, m_env, conf);
    ASSERT_EQ(status, AXIS2_SUCCESS);
	printf("Error code : %d\n", m_env->error->error_number);
	/*
    CUT_ASSERT_INT_EQUAL(env->error->status_code, AXIS2_SUCCESS, 0);
    */

    axis2_conf_ctx_free(conf_ctx, m_env);
}

