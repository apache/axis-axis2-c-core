
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
#include <axis2_module_desc.h>
#include <axis2_phases_info.h>
#include <stdio.h>
#include <axis2_op.h>
#include <axis2_dep_engine.h>
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_class_loader.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axis2_transport_sender.h>
#include <axis2_transport_receiver.h>
#include <axis2_core_utils.h>
#include <cut_defs.h>

void
axis2_test_dep_engine_load(axutil_env_t *env
    )
{
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_conf_t *conf = NULL;
    axutil_hash_t *svc_map = NULL;
    axutil_array_list_t *in_phases = NULL;
    axis2_char_t *axis2c_home = NULL;

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dep_engine = axis2_dep_engine_create_with_repos_name(env, axis2c_home);
    CUT_ASSERT_PTR_NOT_EQUAL(dep_engine, NULL, 1);
    conf = axis2_dep_engine_load(dep_engine, env);
    axis2_conf_set_dep_engine(conf, env, dep_engine);
    CUT_ASSERT_PTR_NOT_EQUAL(conf, NULL, 1);
    svc_map = axis2_conf_get_all_svcs(conf, env);
    CUT_ASSERT_PTR_NOT_EQUAL(svc_map, NULL, 0);
    if (svc_map)
    {
        axutil_hash_index_t *hi = NULL;
        void *service = NULL;
        printf("svc_map count = %d\n", axutil_hash_count(svc_map));
        for (hi = axutil_hash_first(svc_map, env);
             hi; hi = axutil_hash_next(env, hi))
        {
            axutil_hash_t *ops = NULL;
            axis2_svc_t *svc = NULL;
            axutil_param_t *impl_info_param = NULL;

            axutil_hash_this(hi, NULL, NULL, &service);
            svc = (axis2_svc_t *) service;
            impl_info_param =
                axis2_svc_get_param(svc, env, AXIS2_SERVICE_CLASS);
            if (!impl_info_param)
            {
                printf("imple_info_param is null\n");
            }
            ops = axis2_svc_get_all_ops(svc, env);
            if (ops)
            {
                axutil_hash_index_t *hi2 = NULL;
                void *op = NULL;
                axis2_char_t *oname = NULL;
                printf("ops count = %d\n", axutil_hash_count(ops));
 
                for (hi2 = axutil_hash_first(ops, env); hi2;
                     hi2 = axutil_hash_next(env, hi2))
                {
                    if (!hi2)
                        break;
                    axutil_hash_this(hi2, NULL, NULL, &op);
                    if (op)
                    {
                        const axutil_qname_t *qname = NULL;
                        qname = axis2_op_get_qname((axis2_op_t *) op, env);
                        oname = axutil_qname_get_localpart(qname, env);
                        printf("op name = %s\n", oname);
                    }
                }
            }
            else
                printf("ops count = zero\n");
        }
    }

    in_phases =
        axis2_conf_get_in_phases_upto_and_including_post_dispatch(conf, env);
    CUT_ASSERT_PTR_NOT_EQUAL(in_phases, NULL, 0);
    CUT_ASSERT_INT_EQUAL(env->error->status_code, AXIS2_SUCCESS, 0);
    
    /* To avoid warning of not using cut_str_equal */
    CUT_ASSERT_STR_EQUAL("", "", 0);

    axis2_conf_free(conf, env);

}

int
axis2_test_transport_receiver_load(axutil_env_t *env
    )
{
    axutil_dll_desc_t *dll_desc = NULL;
    axis2_char_t *dll_name = NULL;
    axis2_transport_receiver_t *transport_recv = NULL;
    axutil_param_t *impl_info_param = NULL;
    axis2_bool_t is_running = AXIS2_FALSE;
    axis2_char_t *axis2c_home = NULL;

    printf("******************************************\n");
    printf("testing axis2_transport_recv load\n");
    printf("******************************************\n");

    dll_desc = axutil_dll_desc_create(env);

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dll_name =
        axutil_stracat(env, axis2c_home, "/lib/libaxis2_http_receiver.so");
    printf("transport receiver name:%s\n", dll_name);
    axutil_dll_desc_set_name(dll_desc, env, dll_name);
    axutil_dll_desc_set_type(dll_desc, env, AXIS2_TRANSPORT_RECV_DLL);
    impl_info_param = axutil_param_create(env, NULL, NULL);
    axutil_param_set_value(impl_info_param, env, dll_desc);
    axutil_class_loader_init(env);
    transport_recv =
        (axis2_transport_receiver_t *) axutil_class_loader_create_dll(env,
                                                                      impl_info_param);
    is_running = axis2_transport_receiver_is_running(transport_recv, env);
    printf("is_running:%d\n", is_running);
    AXIS2_FREE(env->allocator, dll_name);
    printf("transport receiver load test successful\n");
    return 0;
}

int
axis2_test_transport_sender_load(axutil_env_t *env
    )
{
    axutil_dll_desc_t *dll_desc = NULL;
    axis2_char_t *dll_name = NULL;
    axis2_transport_sender_t *transport_sender = NULL;
    axutil_param_t *impl_info_param = NULL;
    axis2_char_t *axis2c_home = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;

    printf("******************************************\n");
    printf("testing axis2_transport_sender load\n");
    printf("******************************************\n");

    msg_ctx = (axis2_msg_ctx_t *) AXIS2_MALLOC(env->allocator, 5);
    dll_desc = axutil_dll_desc_create(env);

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dll_name = axutil_stracat(env, axis2c_home, "/lib/libaxis2_http_sender.so");
    printf("transport sender name:%s\n", dll_name);
    axutil_dll_desc_set_name(dll_desc, env, dll_name);
    axutil_dll_desc_set_type(dll_desc, env, AXIS2_TRANSPORT_SENDER_DLL);
    impl_info_param = axutil_param_create(env, NULL, NULL);
    axutil_param_set_value(impl_info_param, env, dll_desc);
    axutil_class_loader_init(env);
    transport_sender =
        (axis2_transport_sender_t *) axutil_class_loader_create_dll(env,
                                                                    impl_info_param);

    AXIS2_FREE(env->allocator, dll_name);
    printf("transport sender load test successful\n");
    return 0;
}

void
axis2_test_default_module_version(axutil_env_t *env
    )
{

    axis2_conf_t *axis_conf = NULL;
    axutil_qname_t *mod_qname1 = NULL;
    axutil_qname_t *mod_qname2 = NULL;
    axutil_qname_t *mod_qname3 = NULL;
    axutil_qname_t *mod_qname4 = NULL;
    axutil_qname_t *mod_qname5 = NULL;
    axis2_module_desc_t *module1 = NULL;
    axis2_module_desc_t *module2 = NULL;
    axis2_module_desc_t *module3 = NULL;
    axis2_module_desc_t *module4 = NULL;
    axis2_module_desc_t *module5 = NULL;
    axis2_module_desc_t *def_mod = NULL;
    axutil_array_list_t *engaged_modules = NULL;
    axutil_qname_t *engage_qname = NULL;
    axis2_bool_t found1 = AXIS2_FALSE;
    axis2_bool_t found2 = AXIS2_FALSE;
    axis2_bool_t found3 = AXIS2_FALSE;


    axis_conf = axis2_conf_create(env);
    mod_qname1 = axutil_qname_create(env, "module1", NULL, NULL);
    module1 = axis2_module_desc_create_with_qname(env, mod_qname1);
    axis2_conf_add_module(axis_conf, env, module1);

    mod_qname2 = axutil_qname_create(env, "module2-0.90", NULL, NULL);
    module2 = axis2_module_desc_create_with_qname(env, mod_qname2);
    axis2_conf_add_module(axis_conf, env, module2);

    mod_qname3 = axutil_qname_create(env, "module2-0.92", NULL, NULL);
    module3 = axis2_module_desc_create_with_qname(env, mod_qname3);
    axis2_conf_add_module(axis_conf, env, module3);

    mod_qname4 = axutil_qname_create(env, "module2-0.91", NULL, NULL);
    module4 = axis2_module_desc_create_with_qname(env, mod_qname4);
    axis2_conf_add_module(axis_conf, env, module4);

    mod_qname5 = axutil_qname_create(env, "test_module-1.92", NULL, NULL);
    module5 = axis2_module_desc_create_with_qname(env, mod_qname5);
    axis2_conf_add_module(axis_conf, env, module5);

    axis2_core_utils_calculate_default_module_version(env,
                                                      axis2_conf_get_all_modules
                                                      (axis_conf, env),
                                                      axis_conf);
    def_mod = axis2_conf_get_default_module(axis_conf, env, "module1");
    CUT_ASSERT_PTR_EQUAL(def_mod, module1, 0);
    def_mod = axis2_conf_get_default_module(axis_conf, env, "module2-0.92");
    CUT_ASSERT_PTR_EQUAL(def_mod, module3, 0);
    def_mod = axis2_conf_get_default_module(axis_conf, env, "test_module-1.92");
    CUT_ASSERT_PTR_EQUAL(def_mod, module5, 0);
	
    engage_qname = axutil_qname_create(env, "module2-0.92", NULL, NULL);
    axis2_conf_engage_module(axis_conf, env, engage_qname);
    axutil_qname_free(engage_qname, env);
    engage_qname = NULL;

    engage_qname = axutil_qname_create(env, "module1", NULL, NULL);
    axis2_conf_engage_module(axis_conf, env, engage_qname);
    axutil_qname_free(engage_qname, env);
    engage_qname = NULL;

    axis2_conf_engage_module_with_version(axis_conf, env, "test_module",
                                          "1.92");

    engaged_modules = axis2_conf_get_all_engaged_modules(axis_conf, env);
    CUT_ASSERT_PTR_NOT_EQUAL(engaged_modules, NULL, 0);
    if (engaged_modules)
    {
        int list_size = 0;
        int i = 0;
        list_size = axutil_array_list_size(engaged_modules, env);
		CUT_ASSERT_INT_EQUAL(list_size, 3, 0);
        for (i = 0; i < list_size; i++)
        {
            axutil_qname_t *engaged_mod_qname = NULL;
            engaged_mod_qname = axutil_array_list_get(engaged_modules, env, i);
            if (0 == axutil_strcmp("module2-0.92",
                                   axutil_qname_get_localpart(engaged_mod_qname,
                                                              env)))
            {
                found1 = AXIS2_TRUE;
            }
            if (0 == axutil_strcmp("module1",
                                   axutil_qname_get_localpart(engaged_mod_qname,
                                                              env)))
            {
                found2 = AXIS2_TRUE;
            }
            if (0 == axutil_strcmp("test_module-1.92",
                                   axutil_qname_get_localpart(engaged_mod_qname,
                                                              env)))
            {
                found3 = AXIS2_TRUE;
            }

        }
    }
    CUT_ASSERT_INT_NOT_EQUAL(found1, AXIS2_FALSE, 0);
    CUT_ASSERT_INT_NOT_EQUAL(found2, AXIS2_FALSE, 0);
    CUT_ASSERT_INT_NOT_EQUAL(found3, AXIS2_FALSE, 0);
    axis2_conf_free(axis_conf, env);
    CUT_ASSERT_INT_EQUAL(env->error->status_code, AXIS2_SUCCESS, 0);
}

int
main(
    )
{
    axutil_env_t *env = cut_setup_env("Context");
	CUT_ASSERT(env != NULL);
	if (env) {
        axis2_test_dep_engine_load(env);
        axis2_test_default_module_version(env);
        axutil_env_free(env);
    }
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}
