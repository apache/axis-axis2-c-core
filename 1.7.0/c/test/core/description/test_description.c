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

#include <stdio.h>
#include <axis2_op.h>
#include <axis2_conf.h>
#include <axis2_module_desc.h>
#include <axis2_phases_info.h>
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <cut_defs.h>

void
axis2_test_op_engage_module(axutil_env_t *env
    )
{
    struct axis2_module_desc *moduleref = NULL;
    axis2_conf_t *conf = NULL;
    struct axutil_qname *qname = NULL;
    axis2_op_t *op = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    qname = axutil_qname_create(env, "op1", NULL, NULL);
    CUT_ASSERT_PTR_NOT_EQUAL(qname, NULL, 0);
    CUT_ASSERT_INT_EQUAL(env->error->status_code, AXIS2_SUCCESS, 0);
    op = axis2_op_create_with_qname(env, qname);
    CUT_ASSERT_PTR_NOT_EQUAL(op, NULL, 0);
    CUT_ASSERT(env->error->status_code == AXIS2_SUCCESS);

    moduleref = axis2_module_desc_create(env);
    CUT_ASSERT(moduleref != NULL);
    CUT_ASSERT(env->error->status_code == AXIS2_SUCCESS);
    conf = axis2_conf_create(env);
    CUT_ASSERT(conf != NULL);
    CUT_ASSERT(env->error->status_code == AXIS2_SUCCESS);

    status = axis2_op_engage_module(op, env, moduleref, conf);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    CUT_ASSERT(env->error->status_code == AXIS2_SUCCESS);

    /* To avoid warning of not using cut_str_equal */
    CUT_ASSERT_STR_EQUAL("", "", 0);

    axis2_op_free(op, env);
}

void
axis2_test_svc_add_module_ops(axutil_env_t *env
    )
{
    struct axis2_svc *svc = NULL;
    struct axutil_qname *qname = NULL;
    struct axutil_qname *qname1 = NULL;
    struct axis2_module_desc *module_desc = NULL;
    struct axis2_conf *axis2_config = NULL;

    axis2_status_t status = AXIS2_FAILURE;

    qname = axutil_qname_create(env, "svc1", NULL, NULL);
    svc = axis2_svc_create_with_qname(env, qname);
    CUT_ASSERT(svc != NULL);
    CUT_ASSERT(env->error->status_code == AXIS2_SUCCESS);
    qname1 = axutil_qname_create(env, "mod1", NULL, NULL);
    module_desc = axis2_module_desc_create_with_qname(env, qname1);
    CUT_ASSERT(module_desc != NULL);
    CUT_ASSERT(env->error->status_code == AXIS2_SUCCESS);
    axis2_config = axis2_conf_create(env);
    CUT_ASSERT(axis2_config != NULL);
    CUT_ASSERT(env->error->status_code == AXIS2_SUCCESS);
    status = axis2_svc_add_module_ops(svc, env, module_desc, axis2_config);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    CUT_ASSERT(env->error->status_code == AXIS2_SUCCESS);

    axis2_svc_free(svc, env);
    axis2_module_desc_free(module_desc, env);
    axis2_conf_free(axis2_config, env);
}

void axis2_test_svc_engage_module(axutil_env_t *env
    )
{
    axis2_svc_t *svc = NULL;
    axutil_qname_t *qname = NULL;
    axutil_qname_t *qname1 = NULL;
    axis2_module_desc_t *moduleref = NULL;
    axis2_conf_t *axis2_config = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    qname = axutil_qname_create(env, "svc1", NULL, NULL);
    svc = axis2_svc_create_with_qname(env, qname);
    qname1 = axutil_qname_create(env, "mod1", NULL, NULL);
    moduleref = axis2_module_desc_create_with_qname(env, qname1);
    axis2_config = axis2_conf_create(env);

    status = axis2_svc_engage_module(svc, env, moduleref, axis2_config);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    CUT_ASSERT(env->error->status_code == AXIS2_SUCCESS);

    axis2_svc_free(svc, env);
    axis2_conf_free(axis2_config, env);
}

void
axis2_test_svc_get_op(axutil_env_t *env
    )
{
    struct axis2_svc *svc = NULL;
    struct axutil_qname *qname = NULL;
    struct axutil_hash_t *ops = NULL;
    struct axis2_op *op = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    qname = axutil_qname_create(env, "op1", NULL, NULL);
    op = axis2_op_create_with_qname(env, qname);
    qname = axutil_qname_create(env, "svc1", NULL, NULL);
    svc = axis2_svc_create_with_qname(env, qname);

    status = axis2_svc_add_op(svc, env, op);

    qname = axutil_qname_create(env, "op2", NULL, NULL);
    op = axis2_op_create_with_qname(env, qname);
    status = axis2_svc_add_op(svc, env, op);

    ops = axis2_svc_get_all_ops(svc, env);
    CUT_ASSERT(ops != NULL);
    CUT_ASSERT(env->error->status_code == AXIS2_SUCCESS);

    if (ops)
    {
        axutil_hash_index_t *hi2 = NULL;
        void *op2 = NULL;
        axis2_char_t *oname = NULL;
        int count = 0;
        printf("ops count = %d\n", axutil_hash_count(ops));

        for (hi2 = axutil_hash_first(ops, env); hi2;
             hi2 = axutil_hash_next(env, hi2))
        {
            printf("count = %d \n", count++);
            axis2_svc_get_all_ops(svc, env);
            if (!(hi2))
                break;
            axutil_hash_this(hi2, NULL, NULL, &op2);
            if (op2)
            {
                const axutil_qname_t *qname = NULL;
                qname = axis2_op_get_qname((axis2_op_t *) op2, env);
                oname = axutil_qname_get_localpart(qname, env);
                printf("op name = %s\n", oname);
            }
        }
    }
    else
        printf("ops count = zero\n");
    CUT_ASSERT(env->error->status_code == AXIS2_SUCCESS);

}

int
main(
    )
{
    axutil_env_t *env = cut_setup_env("Core description");
 	CUT_ASSERT(env != NULL);
	if (env) {
       axis2_test_op_engage_module(env);
        axis2_test_svc_add_module_ops(env);
        axis2_test_svc_engage_module(env);
        axis2_test_svc_get_op(env);
        axutil_env_free(env);
	}
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}
