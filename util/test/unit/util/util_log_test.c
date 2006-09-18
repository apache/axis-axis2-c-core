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

#include <axis2_env.h>
#include <axis2_log_default.h>
#include <axis2_error_default.h>
#include <util_log_test.h>
#include <axis2_stream.h>
#include <stdio.h>

void Testaxis2_log_write(CuTest *tc)
{

    printf("testing axis2_log_write\n");
    char actual[10];

    printf("************************\n");
    printf("testing axis2_log_write \n");
    printf("************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_error_t *error = axis2_error_create(allocator);
    axis2_log_t *log  = axis2_log_create(allocator, NULL, NULL);
    const axis2_env_t *env = axis2_env_create_with_error_log(allocator, error, log);
    axis2_stream_t *stream = axis2_stream_create_basic(env);

    char *expected = (char*)strdup("aaaaaaaaa");
    AXIS2_STREAM_WRITE(stream, env, "aaaaaaaaa", 10);
    AXIS2_STREAM_READ(stream, env,  actual, 10);
    AXIS2_LOG_WRITE(env->log, actual, 10);
    CuAssertStrEquals(tc, expected, actual);
}
