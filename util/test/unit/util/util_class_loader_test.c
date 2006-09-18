/*
* Copyright 2004,2005 The Apache Software Foundation.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "util_class_loader_test.h"
#include <axis2_dll_desc.h>
#include <axis2_param.h>
#include <axis2_class_loader.h>
#include <stdio.h>

void Testaxis2_class_loader_create_dll(CuTest *tc)
{
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_char_t *dll_name = NULL;
    void *svc = NULL;
    axis2_param_t *impl_info_param = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_status_t expected = AXIS2_SUCCESS;
    axis2_char_t *axis2c_home = NULL;

    printf("******************************************\n");
    printf("testing axis2_class_loader_create_dll\n");
    printf("******************************************\n");
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    const axis2_env_t *env = axis2_env_create(allocator);


    dll_desc = axis2_dll_desc_create(env);

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dll_name = AXIS2_STRACAT(axis2c_home, "/services/echo/libecho.so", env);

    AXIS2_DLL_DESC_SET_NAME(dll_desc, env, dll_name);
    AXIS2_DLL_DESC_SET_TYPE(dll_desc, env, AXIS2_SVC_DLL);
    impl_info_param = axis2_param_create(env, NULL, NULL);
    AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc);
    axis2_class_loader_init(env);
    svc = axis2_class_loader_create_dll(env, impl_info_param);
    AXIS2_FREE(env->allocator, dll_name);
    CuAssertIntEquals(tc, expected, status);
}
