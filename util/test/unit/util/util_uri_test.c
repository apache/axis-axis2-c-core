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

#include <axis2_uri.h>
#include <axis2_utils.h>
#include <axis2_string.h>
#include "util_uri_test.h"
#include <stdio.h>

void Testaxis2_uri_parse_string(CuTest *tc)
{
    axis2_char_t *actual = NULL;
    axis2_char_t *expected = "temp/checkout.sh";
    axis2_uri_t *uri = NULL;
    axis2_char_t *uri_str = NULL;

    printf("******************************************\n");
    printf("testing axis2_uri_parse_string\n");
    printf("******************************************\n");
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create (allocator);

    /*uri_str = "http://myserver:80/home/damitha/temp/checkout.sh";*/
    /*uri_str = "../temp/checkout.sh";*/
    uri_str = "temp/checkout.sh";
    uri = axis2_uri_parse_string(env, uri_str);
    CuAssertPtrNotNull(tc, uri);

    actual = AXIS2_URI_TO_STRING(uri, env, AXIS2_URI_UNP_OMITUSERINFO);
   
    printf("actual:%s\n", actual);
    
    CuAssertStrEquals(tc, expected, actual);
    AXIS2_URI_FREE(uri, env);
}

void Testaxis2_uri_parse_relative(CuTest *tc)
{
    axis2_char_t *actual = NULL;
    axis2_char_t *expected = "http://myserver/temp/checkout.sh";
    axis2_uri_t *new_uri = NULL;
    axis2_uri_t *base = NULL;
    axis2_char_t *uri_str = NULL;
    axis2_char_t *base_str = NULL;

    printf("******************************************\n");
    printf("testing axis2_uri_parse_relative\n");
    printf("******************************************\n");
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create (allocator);

    /*base_str = "http://myserver:80/home/damitha";*/
    base_str = "file://home/damitha/temp";
    uri_str = "temp/checkout.sh";
    base = axis2_uri_parse_string(env, base_str);
    new_uri = axis2_uri_parse_relative(env, base, uri_str);
    actual = AXIS2_URI_TO_STRING(new_uri, env, AXIS2_URI_UNP_OMITUSERINFO);
   
    printf("actual:%s\n", actual);
    
    CuAssertStrEquals(tc, expected, actual);
    AXIS2_URI_FREE(new_uri, env);
}

void Testaxis2_uri_clone(CuTest *tc)
{
    axis2_char_t *actual = NULL;
    axis2_char_t *expected = "http://myserver/home/damitha/temp";
    axis2_uri_t *uri = NULL;
    axis2_uri_t *new_uri = NULL;
    axis2_char_t *uri_str = NULL;

    printf("******************************************\n");
    printf("testing axis2_uri_clone\n");
    printf("******************************************\n");
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create (allocator);

    uri_str = "http://myserver:80/home/damitha/temp";
    uri = axis2_uri_parse_string(env, uri_str);
    new_uri = AXIS2_URI_CLONE(uri, env);
    CuAssertPtrNotNull(tc, new_uri);

    actual = AXIS2_URI_TO_STRING(new_uri, env, AXIS2_URI_UNP_OMITUSERINFO);
   
    printf("actual:%s\n", actual);
    
    CuAssertStrEquals(tc, expected, actual);
    AXIS2_URI_FREE(uri, env);

}

