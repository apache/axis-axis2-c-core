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
#include <axis2_error_default.h>
#include <axis2_log.h>
#include <axis2_log_default.h>
#include <axis2_allocator.h>
#include <test_log.h>
#include <string.h>
const axis2_env_t *create_env_with_error_log()
{
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    if (!allocator)
    {
        printf("allocator is NULL\n");
        return NULL;
    }
    axis2_error_t *error = axis2_error_create(allocator);
    if (!error)
    {
        printf("cannot create error\n");
        return NULL;
    }

    axis2_log_t *log22  = axis2_log_create(allocator, NULL, NULL);
    if (!log22)
    {
        printf("cannot create log\n");
        return NULL;
    }
    /*
     * allow all types of logs
      */
    log22->level = AXIS2_LOG_LEVEL_DEBUG;
    /*   log22->enabled = 0;*/
    const axis2_env_t *env = axis2_env_create_with_error_log(allocator, error, log22);
    if (!env)
    {
        printf("cannot create env with error and log\n");
        return NULL;
    }
    return env;
}

void test_axis2_log_write(const axis2_env_t *env)
{
    char msg[10];
    printf("\n####start of test_axis2_log_write\n\n");
    strcpy(msg, "abcd test123");
    AXIS2_LOG_WRITE(env->log, msg, AXIS2_LOG_LEVEL_ERROR);
    printf("\n####end of test_axis2_log_write\n\n");
}

void test_axis2_log_debug(const axis2_env_t *env)
{
    printf("\n####start of test_axis2_log_degug\n\n");
    env->log->level = AXIS2_LOG_LEVEL_DEBUG;
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "log_debug test %s %d", "foo", 1);
    printf("\n####end of test_axis2_log_debug\n\n");
}

void test_axis2_log_debug_off(const axis2_env_t *env)
{
    printf("\n####start of test_axis2_log_degug_off\n\n");
    env->log->level = AXIS2_LOG_LEVEL_ERROR;/*log only ERROR's and CRITICAL's*/
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "this should not be logged log_debug test %s %d", "foo", 1);
    printf("\n####end of test_axis2_log_debug_off\n\n");
}

void test_axis2_log_info(const axis2_env_t *env)
{
    printf("\n####start of test_axis2_log_info\n\n");
    env->log->level = AXIS2_LOG_LEVEL_DEBUG;
    AXIS2_LOG_INFO(env->log, "log_info test %s %d", "foo", 1);
    printf("\n####end of test_axis2_log_info\n\n");
}


void test_axis2_log_info_off(const axis2_env_t *env)
{
    printf("\n####start of test_axis2_log_info_off\n\n");
    env->log->level = AXIS2_LOG_LEVEL_ERROR;/*log only ERROR's and CRITICAL's*/
    AXIS2_LOG_INFO(env->log, "this should not be logged log_info test %s %d", "foo", 1);
    printf("\n####end of test_axis2_log_info_off\n\n");
}


void test_axis2_log_warning(const axis2_env_t *env)
{
    printf("\n####start of test_axis2_log_warning\n\n");
    env->log->level = AXIS2_LOG_LEVEL_DEBUG;
    AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "log_warning test %s %d", "foo", 1);
    printf("\n####end of test_axis2_log_warning\n\n");
}


void test_axis2_log_warning_off(const axis2_env_t *env)
{
    printf("\n####start of test_axis2_log_warning_off\n\n");
    env->log->level = AXIS2_LOG_LEVEL_ERROR;/*log only ERROR's and CRITICAL's*/
    AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "this should not be logged log_warning test %s %d", "foo", 1);
    printf("\n####end of test_axis2_log_warning_off\n\n");
}

/*no need to sent log level, should always log*/
void test_axis2_log_error(const axis2_env_t *env)
{
    printf("\n####start of test_axis2_log_error\n\n");
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "log_error test %s %d", "foo", 1);
    printf("\n####end of test_axis2_log_error\n\n");
}

/*no need to sent log level, should always log*/
void test_axis2_log_critical(const axis2_env_t *env)
{
    printf("\n####start of test_axis2_log_critical\n\n");
    AXIS2_LOG_CRITICAL(env->log, AXIS2_LOG_SI, "log_critical test %s %d", "foo", 1);
    printf("\n####end of test_axis2_log_critical\n\n");
}

void run_test_log()
{
    printf("\n####start of run_test_log test suite\n\n");
    const axis2_env_t *env = create_env_with_error_log();
    if (!env)
        return;
    test_axis2_log_write(env);
    test_axis2_log_debug(env);
    test_axis2_log_debug_off(env);

    test_axis2_log_info(env);
    test_axis2_log_info_off(env);

    test_axis2_log_warning(env);
    test_axis2_log_warning_off(env);

    test_axis2_log_error(env);

    test_axis2_log_critical(env);
    printf("\n####end of run_test_log test suite \n\n");
}

