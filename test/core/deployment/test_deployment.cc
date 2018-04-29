
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

class TestDeployment: public ::testing::Test
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


/* Note: AXIS2C_HOME must be set to a valid axis2c deployment in order for
 * this test to pass */
TEST_F(TestDeployment, test_dep_engine_load)
{
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_conf_t *conf = NULL;
    axutil_hash_t *svc_map = NULL;
    axutil_array_list_t *in_phases = NULL;
    axis2_char_t *axis2c_home = NULL;

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dep_engine = axis2_dep_engine_create_with_repos_name(m_env, axis2c_home);
    ASSERT_NE(dep_engine, nullptr);
    conf = axis2_dep_engine_load(dep_engine, m_env);
    axis2_conf_set_dep_engine(conf, m_env, dep_engine);
    ASSERT_NE(conf, nullptr);
    svc_map = axis2_conf_get_all_svcs(conf, m_env);
    ASSERT_NE(svc_map, nullptr);
    if (svc_map)
    {
        axutil_hash_index_t *hi = NULL;
        void *service = NULL;
        printf("svc_map count = %d\n", axutil_hash_count(svc_map));
        for (hi = axutil_hash_first(svc_map, m_env);
             hi; hi = axutil_hash_next(m_env, hi))
        {
            axutil_hash_t *ops = NULL;
            axis2_svc_t *svc = NULL;
            axutil_param_t *impl_info_param = NULL;

            axutil_hash_this(hi, NULL, NULL, &service);
            svc = (axis2_svc_t *) service;
            impl_info_param =
                axis2_svc_get_param(svc, m_env, AXIS2_SERVICE_CLASS);
            if (!impl_info_param)
            {
                printf("imple_info_param is null\n");
            }
            ops = axis2_svc_get_all_ops(svc, m_env);
            if (ops)
            {
                axutil_hash_index_t *hi2 = NULL;
                void *op = NULL;
                axis2_char_t *oname = NULL;
                printf("ops count = %d\n", axutil_hash_count(ops));
 
                for (hi2 = axutil_hash_first(ops, m_env); hi2;
                     hi2 = axutil_hash_next(m_env, hi2))
                {
                    if (!hi2)
                        break;
                    axutil_hash_this(hi2, NULL, NULL, &op);
                    if (op)
                    {
                        const axutil_qname_t *qname = NULL;
                        qname = axis2_op_get_qname((axis2_op_t *) op, m_env);
                        oname = axutil_qname_get_localpart(qname, m_env);
                        printf("op name = %s\n", oname);
                    }
                }
            }
            else
                printf("ops count = zero\n");
        }
    }

    in_phases =
        axis2_conf_get_in_phases_upto_and_including_post_dispatch(conf, m_env);
    ASSERT_NE(in_phases, nullptr);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);

    axis2_conf_free(conf, m_env);

}

/* Note: AXIS2C_HOME must be set to a valid axis2c deployment in order for
 * this test to pass */
TEST_F(TestDeployment, test_transport_receiver_load)
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

    dll_desc = axutil_dll_desc_create(m_env);

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dll_name =
        axutil_stracat(m_env, axis2c_home, "/lib/libaxis2_http_receiver.so");
    printf("transport receiver name:%s\n", dll_name);
    axutil_dll_desc_set_name(dll_desc, m_env, dll_name);
    axutil_dll_desc_set_type(dll_desc, m_env, AXIS2_TRANSPORT_RECV_DLL);
    impl_info_param = axutil_param_create(m_env, NULL, NULL);
    axutil_param_set_value(impl_info_param, m_env, dll_desc);
    axutil_class_loader_init(m_env);
    transport_recv =
        (axis2_transport_receiver_t *) axutil_class_loader_create_dll(m_env,
                                                                      impl_info_param);
    ASSERT_NE(transport_recv, nullptr);
    is_running = axis2_transport_receiver_is_running(transport_recv, m_env);
    printf("is_running:%d\n", is_running);
    AXIS2_FREE(m_env->allocator, dll_name);
    printf("transport receiver load test successful\n");
}


TEST_F(TestDeployment, test_transport_sender_load)
{
    axutil_dll_desc_t *dll_desc = NULL;
    axis2_char_t *dll_name = NULL;
    /*axis2_transport_sender_t *transport_sender = NULL;*/
    axutil_param_t *impl_info_param = NULL;
    axis2_char_t *axis2c_home = NULL;

    printf("******************************************\n");
    printf("testing axis2_transport_sender load\n");
    printf("******************************************\n");

    dll_desc = axutil_dll_desc_create(m_env);

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dll_name = axutil_stracat(m_env, axis2c_home, "/lib/libaxis2_http_sender.so");
    printf("transport sender name:%s\n", dll_name);
    axutil_dll_desc_set_name(dll_desc, m_env, dll_name);
    axutil_dll_desc_set_type(dll_desc, m_env, AXIS2_TRANSPORT_SENDER_DLL);
    impl_info_param = axutil_param_create(m_env, NULL, NULL);
    axutil_param_set_value(impl_info_param, m_env, dll_desc);
    axutil_class_loader_init(m_env);
    /*    transport_sender =
        (axis2_transport_sender_t *) */axutil_class_loader_create_dll(m_env,
                                                                    impl_info_param);

    AXIS2_FREE(m_env->allocator, dll_name);
    printf("transport sender load test successful\n");
}

TEST_F(TestDeployment, test_default_module_version)
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


    axis_conf = axis2_conf_create(m_env);
    mod_qname1 = axutil_qname_create(m_env, "module1", NULL, NULL);
    module1 = axis2_module_desc_create_with_qname(m_env, mod_qname1);
    axis2_conf_add_module(axis_conf, m_env, module1);

    mod_qname2 = axutil_qname_create(m_env, "module2-0.90", NULL, NULL);
    module2 = axis2_module_desc_create_with_qname(m_env, mod_qname2);
    axis2_conf_add_module(axis_conf, m_env, module2);

    mod_qname3 = axutil_qname_create(m_env, "module2-0.92", NULL, NULL);
    module3 = axis2_module_desc_create_with_qname(m_env, mod_qname3);
    axis2_conf_add_module(axis_conf, m_env, module3);

    mod_qname4 = axutil_qname_create(m_env, "module2-0.91", NULL, NULL);
    module4 = axis2_module_desc_create_with_qname(m_env, mod_qname4);
    axis2_conf_add_module(axis_conf, m_env, module4);

    mod_qname5 = axutil_qname_create(m_env, "test_module-1.92", NULL, NULL);
    module5 = axis2_module_desc_create_with_qname(m_env, mod_qname5);
    axis2_conf_add_module(axis_conf, m_env, module5);

    axis2_core_utils_calculate_default_module_version(m_env,
                                                      axis2_conf_get_all_modules
                                                      (axis_conf, m_env),
                                                      axis_conf);
    def_mod = axis2_conf_get_default_module(axis_conf, m_env, "module1");
    ASSERT_EQ(def_mod, module1);
    def_mod = axis2_conf_get_default_module(axis_conf, m_env, "module2-0.92");
    ASSERT_EQ(def_mod, module3);
    def_mod = axis2_conf_get_default_module(axis_conf, m_env, "test_module-1.92");
    ASSERT_EQ(def_mod, module5);

    engage_qname = axutil_qname_create(m_env, "module2-0.92", NULL, NULL);
    axis2_conf_engage_module(axis_conf, m_env, engage_qname);
    axutil_qname_free(engage_qname, m_env);
    engage_qname = NULL;

    engage_qname = axutil_qname_create(m_env, "module1", NULL, NULL);
    axis2_conf_engage_module(axis_conf, m_env, engage_qname);
    axutil_qname_free(engage_qname, m_env);
    engage_qname = NULL;

    axis2_conf_engage_module_with_version(axis_conf, m_env, "test_module",
                                          "1.92");

    engaged_modules = axis2_conf_get_all_engaged_modules(axis_conf, m_env);
    ASSERT_NE(engaged_modules, nullptr);
    if (engaged_modules)
    {
        int list_size = 0;
        int i = 0;
        list_size = axutil_array_list_size(engaged_modules, m_env);
		ASSERT_EQ(list_size, 3);
        for (i = 0; i < list_size; i++)
        {
            axutil_qname_t *engaged_mod_qname = NULL;
            engaged_mod_qname = (axutil_qname_t*) axutil_array_list_get(engaged_modules, m_env, i);
            if (0 == axutil_strcmp("module2-0.92",
                                   axutil_qname_get_localpart(engaged_mod_qname,
                                                              m_env)))
            {
                found1 = AXIS2_TRUE;
            }
            if (0 == axutil_strcmp("module1",
                                   axutil_qname_get_localpart(engaged_mod_qname,
                                                              m_env)))
            {
                found2 = AXIS2_TRUE;
            }
            if (0 == axutil_strcmp("test_module-1.92",
                                   axutil_qname_get_localpart(engaged_mod_qname,
                                                              m_env)))
            {
                found3 = AXIS2_TRUE;
            }

        }
    }
    ASSERT_NE(found1, AXIS2_FALSE);
    ASSERT_NE(found2, AXIS2_FALSE);
    ASSERT_NE(found3, AXIS2_FALSE);
    axis2_conf_free(axis_conf, m_env);
    ASSERT_EQ(m_env->error->status_code, AXIS2_SUCCESS);
}
