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

#include <CuTest.h>
#include <axis2_utils.h>
#include <axis2_string.h>
#include <w2c_cmdline_option.h>
#include <w2c_cmdline_option_parser.h>
#include <w2c_config_property_loader.h>
#include <stdlib.h>
#include <stdio.h>
#include <w2c_engine_configuration.h>
#include "w2c_engine_test.h"

static const axis2_char_t* w2c_config_test_input1[] =
    {"-o", "src",
            "-uri", "a.wsdl"
    };
static const axis2_char_t* w2c_config_test_input2[] =
    {"-ns2p",
            "http://somewhere.com/someres=com.somewhere.someres,"
            "http://anotherwhere.com/anotherres=com.anotherwhere.anothereres,"
            "http://nowhere.lk/anotheres=lk.nowhere.anotherres"
    };
static const axis2_char_t* w2c_config_test_input3[] =
    {"-ns2p",
            "ns2p_file"
    };

void test_engine_config_loader(CuTest *tc)
{
    axutil_env_t *env;
    axutil_allocator_t *allocator;
    w2c_cmdline_option_parser_t* parser = NULL;
    axutil_hash_index_t *hi;
    axutil_hash_t* hash = NULL;
    int input_size;
    axis2_char_t actual[1024];
    int actual_len = 0;
    axis2_char_t* expected;
    axis2_char_t* key;
    axis2_char_t* value;
    w2c_engine_configuration_t* conf = NULL;

    allocator = axutil_allocator_init(NULL);
    env = axutil_env_create(allocator);
    input_size = 4;

    parser = w2c_cmdline_option_parser_create(env,
            input_size, (axis2_char_t**)w2c_config_test_input1);
    hash = W2C_CMDLINE_OPTION_PARSER_GET_OPTIONS(parser, env);

    conf = w2c_engine_configuration_create(env);
    w2c_engine_config_loader_load_config(env, conf, hash);
    strcpy(actual,  W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LOCATION(conf, env));
    expected = "src";
    CuAssertStrEquals(tc, expected, actual);

    W2C_ENGINE_CONFIGURATION_FREE(conf, env);
    W2C_CMDLINE_OPTION_PARSER_FREE(parser, env);

    /* ---------------------------------------------------------------------*/
    input_size = 2;
    parser = w2c_cmdline_option_parser_create(env,
            input_size, (axis2_char_t**)w2c_config_test_input2);
    hash = W2C_CMDLINE_OPTION_PARSER_GET_OPTIONS(parser, env);

    conf = w2c_engine_configuration_create(env);
    w2c_engine_config_loader_load_config(env, conf, hash);

    hash = W2C_ENGINE_CONFIGURATION_GET_URI2PACKAGEMAP(conf, env);

    for (hi = axutil_hash_first(hash, env), actual[0] = '\0';
            hi; hi = axutil_hash_next(env, hi))
    {
        axutil_hash_this(hi, (void*)&key, NULL, (void*)&value);
        sprintf(actual, "%s%s=%s,", actual, key, value);
    }
    actual_len = axis2_strlen(actual);
    actual[actual_len -1 ] = '\0'; /* remove the last comma */
    expected = (char*)w2c_config_test_input2[1];

    CuAssertStrEquals(tc, expected, actual);

    /* ---------------------------------------------------------------------*/
    W2C_ENGINE_CONFIGURATION_FREE(conf, env);
    W2C_CMDLINE_OPTION_PARSER_FREE(parser, env);

    parser = w2c_cmdline_option_parser_create(env,
            input_size, (axis2_char_t**)w2c_config_test_input3);
    hash = W2C_CMDLINE_OPTION_PARSER_GET_OPTIONS(parser, env);

    conf = w2c_engine_configuration_create(env);
    w2c_engine_config_loader_load_config(env, conf, hash);

    hash = W2C_ENGINE_CONFIGURATION_GET_URI2PACKAGEMAP(conf, env);

    for (hi = axutil_hash_first(hash, env), actual[0] = '\0';
            hi; hi = axutil_hash_next(env, hi))
    {
        axutil_hash_this(hi, (void*)&key, NULL, (void*)&value);
        sprintf(actual, "%s%s=%s,", actual, key, value);
    }
    actual_len = axis2_strlen(actual);
    actual[actual_len -1 ] = '\0'; /* remove the last comma */
    expected = (char*)w2c_config_test_input2[1];
    CuAssertStrEquals(tc, expected, actual);

    W2C_ENGINE_CONFIGURATION_FREE(conf, env);
    W2C_CMDLINE_OPTION_PARSER_FREE(parser, env);


}


CuSuite* w2c_engineGetSuite()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_engine_config_loader);
    return suite;
}

