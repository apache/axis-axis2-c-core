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

#include "test_dep_engine.h"
#include <axis2_env.h>
#include <stdio.h>
#include <string.h>
#include <axis2_dep_engine.h>

void Testaxis2_dep_engine_free(CuTest *tc)
{
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_status_t actual = AXIS2_FAILURE;
    axis2_status_t expected = AXIS2_SUCCESS;
    axis2_char_t *axis2c_home = NULL;

    printf("**************************************\n");
    printf("testing axis2_dep_engine_free  method \n");
    printf("**************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_env_t *env = axis2_env_create(allocator);
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");

    dep_engine = axis2_dep_engine_create_with_repos_name(env, axis2c_home
                                                        );
    if (!dep_engine)
    {
        printf("dep engine is not created \n");
        return;
    }
    actual = AXIS2_DEP_ENGINE_FREE(dep_engine, env);
    CuAssertIntEquals(tc, expected, actual);
    axis2_env_free(env);
}


void Testaxis2_dep_engine_create(CuTest *tc)
{
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_dep_engine_t *dep_engine = NULL;

    printf("****************************************\n");
    printf("testing axis2_dep_engine_create  method \n");
    printf("****************************************\n");

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    dep_engine = axis2_dep_engine_create(env);

    CuAssertPtrNotNull(tc, dep_engine);

    if (dep_engine)
        AXIS2_DEP_ENGINE_FREE(dep_engine, env);
    axis2_env_free(env);
}

void Testaxis2_dep_engine_create_with_repos_name(CuTest *tc)
{
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_char_t *axis2c_home = NULL;

    printf("********************************************************\n");
    printf("testing axis2_dep_engine_create_with_repos_name  method \n");
    printf("********************************************************\n");

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");

    dep_engine = axis2_dep_engine_create_with_repos_name(env, axis2c_home);

    CuAssertPtrNotNull(tc, dep_engine);

    if (dep_engine)
        AXIS2_DEP_ENGINE_FREE(dep_engine, env);

    axis2_env_free(env);
}


void Testaxis2_dep_engine_load(CuTest *tc)
{

    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_conf_t *conf_actual = NULL;
    axis2_char_t *axis2c_home = NULL;
    axis2_dep_engine_t *dep_engine = NULL;

    printf("**************************************\n");
    printf("testing axis2_dep_engine_load  method \n");
    printf("**************************************\n");

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");

    dep_engine = axis2_dep_engine_create_with_repos_name(env, axis2c_home);
    if (dep_engine)
        conf_actual = AXIS2_DEP_ENGINE_LOAD(dep_engine, env);

    CuAssertPtrNotNull(tc, conf_actual);

    if (conf_actual)
        AXIS2_DEP_ENGINE_FREE(dep_engine, env);

    axis2_env_free(env);
}

