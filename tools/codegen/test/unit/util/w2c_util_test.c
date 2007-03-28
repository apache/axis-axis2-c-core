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
#include <w2c_url_processor.h>
#include <w2c_xslt_template_processor.h>
#include "w2c_util_test.h"
#include <stdlib.h>
#include <w2c_typemapper.h>

static axis2_char_t* read_file(axis2_char_t* filename);

void test_typemapper(CuTest *tc)
{
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_char_t *actual;
    axis2_char_t *expected;
    w2c_typemapper_t *typemapper;
    axis2_qname_t *qname;

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    expected = "axiom_node_t*";
    typemapper = w2c_typemapper_create_from_file(env, "c.default.typemap.xml");
    actual = W2C_TYPEMAPPER_GET_DEFAULT_MAPPING_NAME(typemapper, env);
    CuAssertStrEquals(tc, expected, actual);

    qname = W2C_TYPEMAPPER_GET_DEFAULT_QNAME(typemapper, env);
    actual = W2C_TYPEMAPPER_GET_TYPE_NAME(typemapper, env, qname);
    CuAssertStrEquals(tc, expected, actual);

    W2C_TYPEMAPPER_FREE(typemapper, env);
}

void test_namespace_to_package_name(CuTest *tc)
{
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_char_t *input;
    axis2_char_t *actual;
    axis2_char_t *expected;

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    input = (char*)axis2_strdup(env, "http://ws.apache.org/axis2/c/w2c");
    actual =
        (char*)w2c_url_processor_make_package_name(env, input);
    expected = "org.apache.ws.axis2.c.w2c";
    CuAssertStrEquals(tc, expected, actual);
    free(input);
    free(actual);

    input = (char*)axis2_strdup(env, "http://while.234who.for/switch/case/2sixa");
    actual =
        (char*)w2c_url_processor_make_package_name(env, input);
    expected = "_for._234who._while._switch._case._2sixa";
    CuAssertStrEquals(tc, expected, actual);
    free(input);
    free(actual);
}

void test_template_parse(CuTest *tc)
{
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_char_t *xml_stream;
    axis2_char_t *xslt_filename;
    axis2_char_t *out_filename;
    axis2_char_t *out_content = NULL;
    axis2_char_t *cmp_out_content = NULL;
    int len = 0;

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    xml_stream = read_file("../../resources/xml/atoms.xml");
    if (! xml_stream)
        return;
    xslt_filename = "../../resources/xml/style1.xsl";
    out_filename = "../../resources/xml/result1.xml";
    w2c_xslt_template_processor_parse(
        env, xml_stream, xslt_filename, out_filename);
    out_content = read_file("../../resources/xml/result1.xml");
    cmp_out_content = read_file("../../resources/xml/style1_result.xml");
    /* just ignore the last new line char for avoid warning */
    len = axis2_strlen(out_content);
    out_content[len -1] = '\0';

    CuAssertStrEquals(tc, out_content, cmp_out_content);

    if (out_content)free(out_content);
    if (cmp_out_content)free(cmp_out_content);

    xslt_filename = "../../resources/xml/style2.xsl";
    out_filename = "../../resources/xml/result2.xml";
    w2c_xslt_template_processor_parse(
        env, xml_stream, xslt_filename, out_filename);
    out_content = read_file("../../resources/xml/result2.xml");
    cmp_out_content = read_file("../../resources/xml/style2_result.xml");
    /* just ignore the last new line char for avoid warning */
    len = axis2_strlen(out_content);
    out_content[len -1] = '\0';

    CuAssertStrEquals(tc, out_content, cmp_out_content);

    if (out_content)free(out_content);
    if (cmp_out_content)free(cmp_out_content);
}

CuSuite* w2c_utilGetSuite()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_namespace_to_package_name);
    SUITE_ADD_TEST(suite, test_template_parse);
    SUITE_ADD_TEST(suite, test_typemapper);
    return suite;
}


static axis2_char_t* read_file(axis2_char_t *filename)
{
    const int MAX_SIZE = 100;
    int nread = 0;
    FILE *f = 0;
    axis2_char_t *out_stream = NULL;
    int ncount = 0;

    out_stream = (axis2_char_t*) malloc(sizeof(axis2_char_t) * MAX_SIZE);
    if (out_stream == NULL)
    {
        return NULL;
    }

    f = fopen(filename, "r+");
    if (f == NULL)
    {
        free(out_stream);
        return NULL;
    }
    do
    {
        nread = fread(out_stream + ncount, sizeof(axis2_char_t), MAX_SIZE, f);
        ncount += nread;
        out_stream = (axis2_char_t*) realloc(out_stream,
                sizeof(axis2_char_t) * (MAX_SIZE + ncount));
        if (out_stream == NULL)
        {
            return NULL;
        }
    }
    while (nread == MAX_SIZE);

    out_stream[ncount] = '\0';
    fclose(f);

    return out_stream;
}
