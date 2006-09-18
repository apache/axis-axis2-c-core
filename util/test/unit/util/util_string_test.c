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

#include "util_string_test.h"
#include <axis2_env.h>
#include <axis2_error_default.h>
#include <axis2_log_default.h>
#include <stdlib.h>

void Testaxis2_strndup(CuTest *tc)
{
    axis2_char_t *actual = NULL;
    int n = 3;
    axis2_char_t *expected = "dam";

    printf("******************************************\n");
    printf("testing axis2_strndup\n");
    printf("******************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_error_t *error = axis2_error_create(allocator);
    const axis2_env_t *env = axis2_env_create_with_error(allocator, error);

    actual = axis2_strndup("damitha", n, env);
    printf("actual:%s\n", actual);

    CuAssertStrEquals(tc, expected, actual);
}

void Testaxis2_strcat(CuTest *tc)
{
    axis2_char_t *actual = NULL;
    axis2_char_t *expected = "damitha_kumarage";

    printf("******************************************\n");
    printf("testing axis2_strcat\n");
    printf("******************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_error_t *error = axis2_error_create(allocator);
    const axis2_env_t *env = axis2_env_create_with_error(allocator, error);

    actual = axis2_strcat(env, "damitha", "_kumarage", NULL);
    printf("actual:%s\n", actual);

    CuAssertStrEquals(tc, expected, actual);
}

