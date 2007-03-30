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
#include <w2c_properties.h>
#include <w2c_config_property_loader.h>
#include <w2c_messages.h>
#include "w2c_conf_test.h"
#include <stdlib.h>
#include <stdio.h>

#define HASH_CONFIG_TEST_EXPECTED "{c.skeleton.impl.template}: " \
        "w2c_cskel_source_writer "  \
        "c/SkelSourceTemplate.xsl " \
        "{c.interface.header.template}: " \
        "w2c_cstub_header_writer " \
        "c/StubHeaderTemplate.xsl " \
        "{c.skeleton.header.template}: " \
        "w2c_cskel_header_writer " \
        "c/SkelHeaderTemplate.xsl " \
        "{c.svc_skeleton.template}: " \
        "w2c_csvc_skeleton_writer " \
        "c/ServiceSkeleton.xsl " \
        "{c.interface.impl.template}: " \
        "w2c_cstub_source_writer " \
        "c/StubSourceTemplate.xsl " \
        "{c.service.template}: " \
        "w2c_cservice_xml_writer " \
        "c/ServiceXMLTemplate.xsl " \
        "{c.filename.extension}: c "

#define ARRAYLIST_CONFIG_TEST_EXPECTED \
        "org.apache.axis2.wsdl.codegen.extension.SchemaUnwrapperExtension,"\
        "org.apache.axis2.wsdl.codegen.extension.JaxMeExtension,"\
        "org.apache.axis2.wsdl.codegen.extension.XMLBeansExtension,"\
        "org.apache.axis2.wsdl.codegen.extension.SimpleDBExtension,"\
        "org.apache.axis2.wsdl.codegen.extension.JiBXExtension,"\
        "org.apache.axis2.wsdl.codegen.extension.JAXBRIExtension," \
        "org.apache.axis2.wsdl.codegen.extension.TypeMapperExtension,"\
        "org.apache.axis2.wsdl.codegen.extension.DefaultDatabindingExtension,"\
        "org.apache.axis2.wsdl.codegen.extension.PolicyEvaluator,"

static const axis2_char_t* w2c_util_test_input[] =
    { "-uri", "a.wsdl" , "b.wsdl" , "-ss",
            "-sd" , "-o" , "src" , "-pp" , "zs"
    };

void test_cmdline_option(CuTest *tc)
{
    axutil_env_t *env;
    axutil_allocator_t *allocator;
    w2c_cmdline_option_parser_t *parser = NULL;
    w2c_cmdline_option_t *option = NULL;
    axis2_hash_index_t *hi;
    axis2_hash_t *hash = NULL;
    axutil_array_list_t *arr_list = NULL;
    int i = 0;
    int input_size;
    axis2_char_t actual[128];
    axis2_char_t *expected;

    allocator = axutil_allocator_init(NULL);
    env = axutil_env_create(allocator);

    input_size = 9;
    parser = w2c_cmdline_option_parser_create
            (env, input_size, (axis2_char_t**)w2c_util_test_input);
    hash = W2C_CMDLINE_OPTION_PARSER_GET_OPTIONS(parser, env);

    option = (w2c_cmdline_option_t*)
            axis2_hash_get(hash, "uri", AXIS2_HASH_KEY_STRING);
    arr_list = W2C_CMDLINE_OPTION_GET_VALUES(option, env);

    for (i = 0 , strcpy(actual, W2C_CMDLINE_OPTION_GET_TYPE(option, env));
            i < axutil_array_list_size(arr_list, env); i ++)
    {
        sprintf(actual , "%s %s", actual ,
                (char*)axutil_array_list_get(arr_list, env, i));
    }
    expected = "uri a.wsdl b.wsdl";
    CuAssertStrEquals(tc, expected, actual);

    /* ----------------------------------------------------------------------*/
    for (hi = axis2_hash_first(hash, env), actual[0] = '\0';
            hi; hi = axis2_hash_next(env, hi))
    {
        axis2_hash_this(hi, NULL, NULL, (void*)&option);
        arr_list = W2C_CMDLINE_OPTION_GET_VALUES(option, env);
        sprintf(actual, "%s{%s}: ", actual, W2C_CMDLINE_OPTION_GET_TYPE(option, env));

        for (i = 0 ; i < axutil_array_list_size(arr_list, env); i ++)
        {
            sprintf(actual , "%s%s ", actual ,
                    (char*)axutil_array_list_get(arr_list, env, i));
        }
    }

    expected = "{uri}: a.wsdl b.wsdl {pp}: zs {ss}: {sd}: {o}: src ";
    CuAssertStrEquals(tc, expected, actual);

    /* ----------------------------------------------------------------------*/
    arr_list = W2C_CMDLINE_OPTION_PARSER_GET_INVALID_OPTIONS(parser, env);
    for (i = 0, actual[0]  = '\0';
            i < axutil_array_list_size(arr_list, env); i ++)
    {
        option = (w2c_cmdline_option_t*)
                axutil_array_list_get(arr_list, env, i);
        sprintf(actual , "%s%s ", actual ,
                W2C_CMDLINE_OPTION_GET_TYPE(option, env));
    }
    expected = "pp ";
    CuAssertStrEquals(tc, expected, actual);
}


void test_config_property_loader(CuTest *tc)
{
    axutil_env_t *env;
    axutil_allocator_t *allocator;
    w2c_config_property_loader_t *loader = NULL;
    axis2_char_t *actual = NULL;
    axis2_char_t *expected = NULL;
    axis2_hash_index_t *hi = NULL;
    axis2_hash_t *hash = NULL;
    axutil_array_list_t *arr_list = NULL;
    int i = 0;
    axis2_char_t *axis2c_home = NULL;
    axis2_char_t file_path[512];
    axis2_char_t *key = NULL;

    allocator = axutil_allocator_init(NULL);
    env = axutil_env_create(allocator);


    loader = w2c_config_property_loader_create(env);

    axis2c_home = getenv("AXIS2C_HOME");
    sprintf(file_path, "%s/codegen/util/codegen-config.properties",
            axis2c_home);

    /* file_path = axis2_strdup
                      ("../../resources/codegen-config.properties", env); */
    /*W2C_CONFIG_PROPERTY_LOADER_W2C_CONFIG_PROPERTY_LOADER_SET_FILENAME( loader,
              env, file_path );*/
    actual = W2C_CONFIG_PROPERTY_LOADER_GET_DEFAULT_LANGUAGE(loader, env);

    expected = "c";
    CuAssertStrEquals(tc, expected, actual);

    hash =
        W2C_CONFIG_PROPERTY_LOADER_GET_LANGUAGE_SPECIFIC_PROPERTIES_MAP(loader, env);
    hash = (axis2_hash_t*)axis2_hash_get(hash, "c", AXIS2_HASH_KEY_STRING);

    if (hash)
    {
        actual = (axis2_char_t*) malloc(sizeof(axis2_char_t) * 4096);
        for (hi = axis2_hash_first(hash, env), actual[0] = '\0';
                hi; hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, (void*)&key, NULL, (void*)&arr_list);
            sprintf(actual, "%s{%s}: ", actual, key);

            for (i = 0 ; i < axutil_array_list_size(arr_list, env); i ++)
            {
                sprintf(actual , "%s%s ", actual ,
                        (char*)axutil_array_list_get(arr_list, env, i));
            }
        }
    }
    expected = HASH_CONFIG_TEST_EXPECTED;
    CuAssertStrEquals(tc, expected, actual);

    W2C_CONFIG_PROPERTY_LOADER_FREE(loader, env);
    /** this part is removed from the config file - check later after it is readd */
    /*
    arr_list = W2C_CONFIG_PROPERTY_LOADER_GET_EXTENSION_CLASS_NAMES ( loader, env );
    for ( i = 0, actual[0] ='\0'; i < axutil_array_list_size ( arr_list, env ); i ++ )
    {
        sprintf ( actual, "%s%s,", actual,
                         (char*) axutil_array_list_get ( arr_list, env , i ) );
    }
    expected = ARRAYLIST_CONFIG_TEST_EXPECTED;
    CuAssertStrEquals(tc, expected, actual);
    */
}

void test_messages(CuTest *tc)
{
    axutil_env_t *env;
    axutil_allocator_t *allocator;
    axis2_char_t *key;
    axis2_char_t *actual = NULL;
    axis2_char_t *expected = NULL;
    allocator = axutil_allocator_init(NULL);
    env = axutil_env_create(allocator);

    key = axis2_strdup(env, "wsdl2code.arg1");
    expected = "Usage WSDL2Code -uri <Location of WSDL> : WSDL file location";
    actual = w2c_messages_get_message(env, key);
    CuAssertStrEquals(tc, expected, actual);
}

CuSuite* w2c_confGetSuite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_cmdline_option);
    SUITE_ADD_TEST(suite, test_config_property_loader);
    SUITE_ADD_TEST(suite, test_messages);
    return suite;
}

