
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

#include <axis2_conf_ctx.h>
#include <axis2_svc_grp.h>
#include <axis2_const.h>
#include <axutil_allocator.h>
#include <axutil_env.h>
#include <stdio.h>
#include <cut_defs.h>

void
axis2_test_conf_ctx_init(axutil_env_t *env
    )
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

    conf = axis2_conf_create(env);
    CUT_ASSERT_PTR_NOT_EQUAL(conf, NULL, 1);

    conf_ctx = axis2_conf_ctx_create(env, conf);
    CUT_ASSERT_PTR_NOT_EQUAL(conf_ctx, NULL, 1);
	
    svc_grp1 = axis2_svc_grp_create(env);
    CUT_ASSERT_PTR_NOT_EQUAL(svc_grp1, NULL, 0);
    svc_grp2 = axis2_svc_grp_create(env);
    CUT_ASSERT_PTR_NOT_EQUAL(svc_grp2, NULL, 0);

    svc_grp_ctx1 = axis2_svc_grp_ctx_create(env, svc_grp1, conf_ctx);
    CUT_ASSERT_PTR_NOT_EQUAL(svc_grp_ctx1, NULL, 0);
    svc_grp_ctx2 = axis2_svc_grp_ctx_create(env, svc_grp2, conf_ctx);
    CUT_ASSERT_PTR_NOT_EQUAL(svc_grp_ctx2, NULL, 0);

    qname1 = axutil_qname_create(env, "name1", NULL, NULL);
    CUT_ASSERT_PTR_NOT_EQUAL(qname1, NULL, 0);
    qname2 = axutil_qname_create(env, "name2", NULL, NULL);
    CUT_ASSERT_PTR_NOT_EQUAL(qname2, NULL, 0);

    svc1 = axis2_svc_create_with_qname(env, qname1);
    CUT_ASSERT_PTR_NOT_EQUAL(svc1, NULL, 0);
    svc2 = axis2_svc_create_with_qname(env, qname2);
    CUT_ASSERT_PTR_NOT_EQUAL(svc2, NULL, 0);

    svc_ctx1 = axis2_svc_ctx_create(env, svc1, svc_grp_ctx1);
    CUT_ASSERT_PTR_NOT_EQUAL(svc_ctx1, NULL, 0);
    svc_ctx2 = axis2_svc_ctx_create(env, svc2, svc_grp_ctx2);
    CUT_ASSERT_PTR_NOT_EQUAL(svc_ctx1, NULL, 0);

    op = axis2_op_create(env);
    CUT_ASSERT_PTR_NOT_EQUAL(op, NULL, 0);

    op_ctx1 = axis2_op_ctx_create(env, op, svc_ctx1);
    CUT_ASSERT_PTR_NOT_EQUAL(op_ctx1, NULL, 0);

    op_ctx2 = axis2_op_ctx_create(env, op, svc_ctx2);
    CUT_ASSERT_PTR_NOT_EQUAL(op_ctx2, NULL, 0);

    op_ctx_map = axis2_conf_ctx_get_op_ctx_map(conf_ctx, env);
    CUT_ASSERT_PTR_NOT_EQUAL(op_ctx_map, NULL, 0);
    if (op_ctx_map)
    {
        axutil_hash_set(op_ctx_map, "op_ctx1", AXIS2_HASH_KEY_STRING, op_ctx1);
        axutil_hash_set(op_ctx_map, "op_ctx2", AXIS2_HASH_KEY_STRING, op_ctx2);
    }

    svc_ctx_map = axis2_conf_ctx_get_svc_ctx_map(conf_ctx, env);
    CUT_ASSERT_PTR_NOT_EQUAL(svc_ctx_map, NULL, 0);
    if (svc_ctx_map)
    {
        axutil_hash_set(svc_ctx_map, "svc_ctx1", AXIS2_HASH_KEY_STRING,
                        svc_ctx1);
        axutil_hash_set(svc_ctx_map, "svc_ctx2", AXIS2_HASH_KEY_STRING,
                        svc_ctx2);
    }

    svc_grp_ctx_map = axis2_conf_ctx_get_svc_grp_ctx_map(conf_ctx, env);
    CUT_ASSERT_PTR_NOT_EQUAL(svc_grp_ctx_map, NULL, 0);

    if (svc_grp_ctx_map)
    {
        axutil_hash_set(svc_ctx_map, "svc_grp_ctx1", AXIS2_HASH_KEY_STRING,
                        svc_grp_ctx1);
        axutil_hash_set(svc_ctx_map, "svc_grp_ctx2", AXIS2_HASH_KEY_STRING,
                        svc_grp_ctx2);
    }

    status = axis2_conf_ctx_init(conf_ctx, env, conf);
    CUT_ASSERT_INT_EQUAL(status, AXIS2_SUCCESS, 0);
	printf("Error code : %d\n", env->error->error_number);
	/*
    CUT_ASSERT_INT_EQUAL(env->error->status_code, AXIS2_SUCCESS, 0);
    */
    
    /* To avoid warning of not using cut_str_equal */
    CUT_ASSERT_STR_EQUAL("", "", 0);

    axis2_conf_ctx_free(conf_ctx, env);
}

int
main(
    )
{
    axutil_env_t *env = cut_setup_env("Context");
	CUT_ASSERT(env != NULL);
	if (env) {
       axis2_test_conf_ctx_init(env);
       axutil_env_free(env);
    }
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}
