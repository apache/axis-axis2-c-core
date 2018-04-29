
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

#include <stdio.h>
#include <axutil_env.h>
#include <axis2_engine.h>
#include <axis2_conf_ctx.h>
#include <axis2_msg_ctx.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
/* #include <axis2_conf_builder.h> */

class TestEngine: public ::testing::Test
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


TEST_F(TestEngine, test_engine_send)
{

    axis2_status_t status = AXIS2_FAILURE;
    struct axis2_conf *conf = NULL;

    struct axis2_conf_ctx *conf_ctx;
    struct axis2_msg_ctx *msg_ctx;
    struct axis2_op_ctx *op_ctx;
    struct axis2_op *op;
    struct axis2_svc *svc;
    struct axis2_svc_ctx *svc_ctx;
    struct axis2_svc_grp *svc_grp;
    struct axis2_svc_grp_ctx *svc_grp_ctx;
    struct axutil_qname *qname;
    axis2_engine_t *engine = NULL;

    conf = axis2_conf_create(m_env);
    conf_ctx = axis2_conf_ctx_create(m_env, conf);

    svc_grp = axis2_svc_grp_create(m_env);
    svc_grp_ctx = axis2_svc_grp_ctx_create(m_env, svc_grp, conf_ctx);

    qname = axutil_qname_create(m_env, "name1", NULL, NULL);
    svc = axis2_svc_create_with_qname(m_env, qname);
    svc_ctx = axis2_svc_ctx_create(m_env, svc, svc_grp_ctx);

    op = axis2_op_create(m_env);
    op_ctx = axis2_op_ctx_create(m_env, op, svc_ctx);

    msg_ctx = axis2_msg_ctx_create(m_env, conf_ctx, NULL, NULL);

    axis2_msg_ctx_set_conf_ctx(msg_ctx, m_env, conf_ctx);
    axis2_msg_ctx_set_op_ctx(msg_ctx, m_env, op_ctx);
    axis2_msg_ctx_set_svc_ctx(msg_ctx, m_env, svc_ctx);

    engine = axis2_engine_create(m_env, conf_ctx);
    status = axis2_engine_send(engine, m_env, msg_ctx);
    ASSERT_NE(status, AXIS2_SUCCESS);
    printf("Error code : %d\n", m_env->error->error_number);

    axis2_conf_ctx_free(conf_ctx, m_env);
    axis2_msg_ctx_free(msg_ctx, m_env);
    axutil_qname_free(qname, m_env);
    axis2_svc_grp_ctx_free(svc_grp_ctx, m_env);
    axis2_svc_ctx_free(svc_ctx, m_env);
    axis2_svc_free(svc, m_env);
    axis2_op_ctx_free(op_ctx, m_env);
    axis2_op_free(op, m_env);
    axis2_engine_free(engine, m_env);
}

TEST_F(TestEngine, test_engine_receive)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_conf_t *conf = NULL;
    struct axis2_conf_ctx *conf_ctx;
    struct axis2_msg_ctx *msg_ctx;
    axis2_engine_t *engine = NULL;

    conf = axis2_conf_create(m_env);
    conf_ctx = axis2_conf_ctx_create(m_env, conf);

    msg_ctx = axis2_msg_ctx_create(m_env, conf_ctx, NULL, NULL);

    engine = axis2_engine_create(m_env, conf_ctx);

    status = axis2_engine_receive(engine, m_env, msg_ctx);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    axis2_conf_ctx_free(conf_ctx, m_env);
    axis2_msg_ctx_free(msg_ctx, m_env);
    axis2_engine_free(engine, m_env);
}
