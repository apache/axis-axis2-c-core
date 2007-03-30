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
#include <axis2_util.h>
#include <axis2_string.h>
#include <w2c_config_property_loader.h>
#include <stdlib.h>
#include "w2c_writer_test.h"
#include <w2c_writer.h>
#include <w2c_class_loader.h>
#include <axiom.h>
#include <w2c_xslt_utils.h>
#include <w2c_cmdline_option_parser.h>
#include <axis2_hash.h>
#include <w2c_engine_configuration.h>

static const axis2_char_t* w2c_config_test_input1[] =
    {"-o", "src/howar",
            "-uri", "a.wsdl"
    };

void test_writer(CuTest *tc)
{
    axis2_env_t *env;
    axutil_allocator_t *allocator;
    axis2_char_t* testing_lang = NULL;
    w2c_writer_t* writer = NULL;
    w2c_config_property_loader_t* loader = NULL;
    axis2_char_t file_path[128];
    axis2_char_t* axis2c_home = NULL;
    axiom_node_t* root = NULL;
    axiom_node_t* inter_node = NULL;
    axis2_char_t* buffer = NULL;
    w2c_cmdline_option_parser_t* parser = NULL;
    axis2_hash_t* hash = NULL;
    w2c_engine_configuration_t* conf = NULL;
    axutil_dll_desc_t* dll_desc = NULL;

    int input_size = 0;

    allocator = axutil_allocator_init(NULL);
    env = axis2_env_create(allocator);

    axis2c_home = getenv("AXIS2C_HOME");
    /* this is to load configuration from properties file */
    loader = w2c_config_property_loader_create(env);

    root = w2c_xslt_utils_add_child_node(env, "interface", NULL);
    w2c_xslt_utils_add_attribute(env, root, "servicename", "myservice");

    inter_node = w2c_xslt_utils_add_child_node(env, "method", root);
    w2c_xslt_utils_add_attribute(env, inter_node, "name", "myfirstmethod");

    inter_node = w2c_xslt_utils_add_child_node(env, "method", root);
    w2c_xslt_utils_add_attribute(env, inter_node, "name", "mysecondmethod");

    inter_node = w2c_xslt_utils_add_child_node(env, "method", root);
    w2c_xslt_utils_add_attribute(env, inter_node, "name", "lastmethod-if more add prior to this");

    buffer = w2c_xslt_utils_serialize(env, root);

    sprintf(file_path, "%s/codegen/util/codegen-config.properties",
            axis2c_home);
    testing_lang = "c";

    input_size = 2;
    parser = w2c_cmdline_option_parser_create(env,
            input_size, (axis2_char_t**)w2c_config_test_input1);
    hash = W2C_CMDLINE_OPTION_PARSER_GET_OPTIONS(parser, env);

    conf = w2c_engine_configuration_create(env);
    CuAssertPtrNotNull(tc, conf);
    /* this is to load configuration from command line arguments*/
    w2c_engine_config_loader_load_config(env, conf, hash);

    writer = (w2c_writer_t*)w2c_class_loader_get_object_from_class_name
            (env, "w2c_cservice_xml_writer", "/codegen/writers/", &dll_desc);
    CuAssertPtrNotNull(tc, writer);
    if (writer)
    {
        W2C_WRITER_INITIALIZE(writer, env, loader, conf);
        W2C_WRITER_CREATE_OUT_FILE(writer, env, NULL, NULL);
        W2C_WRITER_PARSE(writer, env, root);
    }

    w2c_class_loader_free_loaded_class(env, dll_desc);

    /*CuAssertStrEquals(tc, expected, actual);*/
}


CuSuite* w2c_writerGetSuite()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_writer);
    return suite;
}

