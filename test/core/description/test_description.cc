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
#include <axis2_op.h>
#include <axis2_conf.h>
#include <axis2_module_desc.h>
#include <axis2_phases_info.h>
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>

class TestDescription: public ::testing::Test
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

TEST_F(TestDescription, test_op_engage_module)
{

    struct axis2_module_desc *moduleref = NULL;
    axis2_conf_t *conf = NULL;
    struct axutil_qname *qname = NULL;
    axis2_op_t *op = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    qname = axutil_qname_create(m_env, "op1", NULL, NULL);
    ASSERT_NE(qname, nullptr);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);
    op = axis2_op_create_with_qname(m_env, qname);
    ASSERT_NE(op, nullptr);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);

    moduleref = axis2_module_desc_create(m_env);
    ASSERT_NE(moduleref, nullptr);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);
    conf = axis2_conf_create(m_env);
    ASSERT_NE(conf, nullptr);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);

    status = axis2_op_engage_module(op, m_env, moduleref, conf);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    /* TODO the following test fails due to an uninitialized array_list.
     * need to determine best way/location to fix.  See AXIS2C-1697  */
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);

    axis2_op_free(op, m_env);
}

TEST_F(TestDescription, test_svc_add_module_ops)
{
    struct axis2_svc *svc = NULL;
    struct axutil_qname *qname = NULL;
    struct axutil_qname *qname1 = NULL;
    struct axis2_module_desc *module_desc = NULL;
    struct axis2_conf *axis2_config = NULL;

    axis2_status_t status = AXIS2_FAILURE;

    qname = axutil_qname_create(m_env, "svc1", NULL, NULL);
    svc = axis2_svc_create_with_qname(m_env, qname);
    ASSERT_NE(svc, nullptr);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);
    qname1 = axutil_qname_create(m_env, "mod1", NULL, NULL);
    module_desc = axis2_module_desc_create_with_qname(m_env, qname1);
    ASSERT_NE(module_desc, nullptr);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);
    axis2_config = axis2_conf_create(m_env);
    ASSERT_NE(axis2_config, nullptr);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);
    status = axis2_svc_add_module_ops(svc, m_env, module_desc, axis2_config);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);

    axis2_svc_free(svc, m_env);
    axis2_module_desc_free(module_desc, m_env);
    axis2_conf_free(axis2_config, m_env);
}

TEST_F(TestDescription, test_svc_engage_module)
{
    axis2_svc_t *svc = NULL;
    axutil_qname_t *qname = NULL;
    axutil_qname_t *qname1 = NULL;
    axis2_module_desc_t *moduleref = NULL;
    axis2_conf_t *axis2_config = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    qname = axutil_qname_create(m_env, "svc1", NULL, NULL);
    svc = axis2_svc_create_with_qname(m_env, qname);
    qname1 = axutil_qname_create(m_env, "mod1", NULL, NULL);
    moduleref = axis2_module_desc_create_with_qname(m_env, qname1);
    axis2_config = axis2_conf_create(m_env);

    status = axis2_svc_engage_module(svc, m_env, moduleref, axis2_config);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);

    axis2_svc_free(svc, m_env);
    axis2_conf_free(axis2_config, m_env);
}

TEST_F(TestDescription, test_svc_get_op)
{
    struct axis2_svc *svc = NULL;
    struct axutil_qname *qname = NULL;
    struct axutil_hash_t *ops = NULL;
    struct axis2_op *op = NULL;

    qname = axutil_qname_create(m_env, "op1", NULL, NULL);
    op = axis2_op_create_with_qname(m_env, qname);
    qname = axutil_qname_create(m_env, "svc1", NULL, NULL);
    svc = axis2_svc_create_with_qname(m_env, qname);

    axis2_svc_add_op(svc, m_env, op);

    qname = axutil_qname_create(m_env, "op2", NULL, NULL);
    op = axis2_op_create_with_qname(m_env, qname);
    axis2_svc_add_op(svc, m_env, op);

    ops = axis2_svc_get_all_ops(svc, m_env);
    ASSERT_NE(ops, nullptr);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);

    if (ops)
    {
        axutil_hash_index_t *hi2 = NULL;
        void *op2 = NULL;
        axis2_char_t *oname = NULL;
        int count = 0;
        printf("ops count = %d\n", axutil_hash_count(ops));

        for (hi2 = axutil_hash_first(ops, m_env); hi2;
             hi2 = axutil_hash_next(m_env, hi2))
        {
            printf("count = %d \n", count++);
            axis2_svc_get_all_ops(svc, m_env);
            if (!(hi2))
                break;
            axutil_hash_this(hi2, NULL, NULL, &op2);
            if (op2)
            {
                const axutil_qname_t *qname = NULL;
                qname = axis2_op_get_qname((axis2_op_t *) op2, m_env);
                oname = axutil_qname_get_localpart(qname, m_env);
                printf("op name = %s\n", oname);
            }
        }
    }
    else
        printf("ops count = zero\n");
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);

}
