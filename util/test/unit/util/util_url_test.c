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

#include "util_url_test.h"
#include <axis2_url.h>
#include <axis2_utils.h>
#include <axis2_string.h>
#include <stdio.h>

void Testaxis2_url_parse_string(CuTest *tc)
{
    axis2_char_t *actual = NULL;
    axis2_char_t *expected = "file://home/damitha/temp/checkout.sh";
    axis2_url_t *url = NULL;
    axis2_char_t *url_str = NULL;

    printf("******************************************\n");
    printf("testing axis2_url_parse_string\n");
    printf("******************************************\n");
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create (allocator);
   
    url_str = AXIS2_STRDUP("file://home/damitha/temp/checkout.sh", env);
    url = axis2_url_parse_string(env, url_str);
   
    actual = AXIS2_URL_TO_EXTERNAL_FORM(url, env);
    printf("actual:%s\n", actual);
    
    CuAssertStrEquals(tc, expected, actual);
}
