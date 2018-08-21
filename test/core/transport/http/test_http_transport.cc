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

#include <gtest/gtest.h>

#include <stdio.h>
#include <axis2_http_request_line.h>
#include <axis2_http_status_line.h>
#include <axis2_http_header.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_url.h>
#include <axis2_http_client.h>
#ifdef AXIS2_JSON_ENABLED
#include <axiom.h>
#include <axutil_utils.h>
#include <json.h>
#include <axis2_json_writer.h>
#include <axis2_json_reader.h>
#endif

#include "../../../cutest/include/cut_http_server.h"

class TestHTTPTransport: public ::testing::Test
{

    protected:
        void SetUp()
        {

            m_allocator = axutil_allocator_init(NULL);
            m_axis_log = axutil_log_create(m_allocator, NULL, NULL);
            m_error = axutil_error_create(m_allocator);

            m_env = axutil_env_create_with_error_log(m_allocator, m_error, m_axis_log);

        }

        void TearDown()
        {
            axutil_env_free(m_env);
        }


        axutil_allocator_t *m_allocator = NULL;
        axutil_env_t *m_env = NULL;
        axutil_error_t *m_error = NULL;
        axutil_log_t *m_axis_log = NULL;

};


TEST_F(TestHTTPTransport, test_http_request_line)
{
    char *request_line_str = (char *) axutil_strdup(m_env,
        "POST http://ws.apache.org/axis2/c/services/test_svc/test_op?x=1 HTTP/1.1\r\n");
    axis2_http_request_line_t *request_line;

    printf("Starting http_request_line tests\n");
    request_line = axis2_http_request_line_parse_line(m_env, request_line_str);
    ASSERT_STREQ(axis2_http_request_line_get_method(request_line, m_env), "POST");
    ASSERT_STREQ(axis2_http_request_line_get_uri(request_line, m_env),
            "http://ws.apache.org/axis2/c/services/test_svc/test_op?x=1");
    ASSERT_STREQ(axis2_http_request_line_get_http_version(request_line, m_env), "HTTP/1.1");
    printf("Method: %s|URI: %s|HTTP Version: %s|\n",
           axis2_http_request_line_get_method(request_line, m_env),
           axis2_http_request_line_get_uri(request_line, m_env),
           axis2_http_request_line_get_http_version(request_line, m_env));

    axis2_http_request_line_free(request_line, m_env);
    AXIS2_FREE(m_env->allocator, request_line_str);
    printf("Finished http_request_line tests ..........\n\n");
}

TEST_F(TestHTTPTransport, test_http_status_line)
{
    const char *status_line_str = "HTTP/1.1 202 Accepted\r\n";
    axis2_http_status_line_t *status_line;

    printf("Starting http_status_line tests\n");
    status_line = axis2_http_status_line_create(m_env, status_line_str);
    ASSERT_TRUE(axis2_http_status_line_starts_with_http);
    printf("Staus Line starts with HTTP ? :%d\n",
           axis2_http_status_line_starts_with_http(status_line, m_env));
    ASSERT_STREQ(axis2_http_status_line_get_http_version(status_line, m_env), "HTTP/1.1");
    ASSERT_EQ(axis2_http_status_line_get_status_code(status_line, m_env), 202);
    ASSERT_STREQ(axis2_http_status_line_get_reason_phrase(status_line, m_env), "Accepted");
    printf("HTTP Version :%s|Status Code:%d|Reason Phrase:%s|\n",
           axis2_http_status_line_get_http_version(status_line, m_env),
           axis2_http_status_line_get_status_code(status_line, m_env),
           axis2_http_status_line_get_reason_phrase(status_line, m_env));
    printf("to_string :%s\n",
           axis2_http_status_line_to_string(status_line, m_env));
    ASSERT_STREQ(axis2_http_status_line_to_string(status_line, m_env), status_line_str);
    axis2_http_status_line_free(status_line, m_env);
    printf("Finished http_status_line tests ..........\n\n");
}

TEST_F(TestHTTPTransport, test_http_header)
{
    const char *header_name = "Content-Type";
    const char *header_value = "text/xml";
    char *str_header = (char *)axutil_strdup(m_env,"Content-Type: text/xml; charset=UTF-8\r\n");
    axis2_http_header_t *http_header;
    axis2_char_t *external_form = NULL;

    printf("Starting http_header tests\n");
    http_header = axis2_http_header_create(m_env, header_name, header_value);
    external_form = axis2_http_header_to_external_form(http_header, m_env);
    ASSERT_STREQ(header_name, axis2_http_header_get_name(http_header, m_env));
    ASSERT_STREQ(header_value, axis2_http_header_get_value(http_header, m_env));
    ASSERT_STREQ(external_form, "Content-Type: text/xml\r\n");
    printf("Header Name :%s|Header Value:%s|External Form:%s\n",
           axis2_http_header_get_name(http_header, m_env),
           axis2_http_header_get_value(http_header, m_env), external_form);
    AXIS2_FREE(m_env->allocator, external_form);
    axis2_http_header_free(http_header, m_env);

    http_header = axis2_http_header_create_by_str(m_env, str_header);
    axis2_http_header_free(http_header, m_env);
    AXIS2_FREE(m_env->allocator, str_header);
    printf("Finished http_header tests ..........\n\n");
}

TEST_F(TestHTTPTransport, test_url)
{
    const axis2_char_t *str_url = "http://ws.apache.org/axis2/";
    axutil_url_t *url = axutil_url_parse_string(m_env, str_url);
    ASSERT_NE(url, nullptr);
    printf("Starting URL Test ....\n");
    printf
        ("Parsed URL : \n Protocol :%s|\n Host :%s|\n Port :%d|\n Path : %s|\n",
         axutil_url_get_protocol(url, m_env), axutil_url_get_host(url, m_env),
         axutil_url_get_port(url, m_env), axutil_url_get_path(url, m_env));
    ASSERT_STREQ(axutil_url_get_protocol(url, m_env), "http");
    ASSERT_STREQ(axutil_url_get_host(url, m_env), "ws.apache.org");
    ASSERT_EQ(axutil_url_get_port(url, m_env), 80);
    ASSERT_STREQ(axutil_url_get_path(url, m_env), "/axis2/");
    printf("End of URL Test ... \n");
    axutil_url_free(url, m_env);
}

/* Note: This test fails unless you have a deployed axis2c instance with the
 * echo service on the appropriate port, and AXIS2C_HOME defined in your
 * environment.
 */
TEST_F(TestHTTPTransport, test_http_client)
{
    axis2_http_client_t *client = NULL;
    axis2_http_simple_request_t *request = NULL;
    axis2_http_request_line_t *request_line = NULL;
    axutil_url_t *url = NULL;
    axis2_http_header_t *header = NULL;
    axis2_http_simple_response_t *response = NULL;
    axis2_status_t status;
    char *body_bytes = NULL;
    int body_bytes_len = 0;
    char * content ="<soapenv:Envelope xmlns:soapenv=\"http://www.w3.org/2003/05/soap-envelope\"><soapenv:Body><echoString><text>echo5</text></echoString></soapenv:Body></soapenv:Envelope>";
    char tmpbuf[100];
    int server_status;
    printf("Starting http_client tests\n");
    server_status = ut_start_http_server(m_env);
    ASSERT_EQ(server_status, 0);
    request_line = axis2_http_request_line_create(m_env, "POST",
                                                  "/axis2/services/echo/echo",
                                                  "HTTP/1.1");
    request = axis2_http_simple_request_create(m_env, request_line,
                                               NULL, 0, NULL);
    axis2_http_simple_request_set_body_string(request, m_env, content, strlen(content));
    url = axutil_url_create(m_env, "http", "localhost", 9090, NULL);
	sprintf(tmpbuf,"%s:%d", axutil_url_get_host(url, m_env), axutil_url_get_port(url, m_env));
    header =
        axis2_http_header_create(m_env, "Host", tmpbuf);
    axis2_http_simple_request_add_header(request, m_env, header);
    header =
        axis2_http_header_create(m_env, "Content-Type", "application/soap+xml");
    axis2_http_simple_request_add_header(request, m_env, header);
	sprintf(tmpbuf,"%d", (int) strlen(content));
    header =
        axis2_http_header_create(m_env, "Content-Length", tmpbuf);
    axis2_http_simple_request_add_header(request, m_env, header);
    client = axis2_http_client_create(m_env, url);

    AXIS2_LOG_DEBUG(m_env->log, AXIS2_LOG_SI, "Trying to call axis2_http_client_send");
    status = axis2_http_client_send(client, m_env, request, NULL);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    if (status != AXIS2_SUCCESS)
    {
        printf("Test FAILED .........Can't send the request. Status : %d\n",
               status);
        return;
    }
    status = axis2_http_client_receive_header(client, m_env);
    ASSERT_EQ(status, 200);
    if (status != 200)
    {
        printf("Test FAILED ......... Can't receive. Status: %d\n", status);
        return;
    }
    response = axis2_http_client_get_response(client, m_env);
	ASSERT_NE(response, nullptr);
    if (!response)
    {
        printf("Test Failed : NULL response");
        return;
    }
    printf("Content Type :%s\n",
           axis2_http_simple_response_get_content_type(response, m_env));
    printf("Content Length :%d\n",
           axis2_http_simple_response_get_content_length(response, m_env));
    printf("Status code :%d\n", status);
    body_bytes_len =
        axis2_http_simple_response_get_body_bytes(response, m_env, &body_bytes);
    printf("body :%s\n%d", body_bytes , body_bytes_len);

    axis2_http_client_free(client, m_env);
    axis2_http_simple_request_free(request, m_env);
    ut_stop_http_server(m_env);
    AXIS2_FREE(m_env->allocator, body_bytes);
    printf("Finished http_client tests ..........\n\n");
}

TEST_F(TestHTTPTransport, test_https_client)
{
#ifndef AXIS2_SSL_ENABLED
    return;
#else
    axis2_http_client_t *client = NULL;
    axis2_http_simple_request_t *request = NULL;
    axis2_http_request_line_t *request_line = NULL;
    axutil_url_t *url = NULL;
    axis2_http_header_t *header = NULL;
    axutil_stream_t *request_body = NULL;
    axis2_http_simple_response_t *response = NULL;
    int status = 0;
    char *body_bytes = NULL;
    int body_bytes_len = 0;

    printf("Starting https_client tests\n");
    request_line = axis2_http_request_line_create(m_env, "GET", "/", "HTTP/1.0");
    request_body = axutil_stream_create_basic(m_env);
    request = axis2_http_simple_request_create(m_env, request_line,
                                               NULL, 0, NULL);
    url = axutil_url_create(m_env, "https", "localhost", 9099, NULL);

    header =
        axis2_http_header_create(m_env, "Host", axutil_url_get_host(url, m_env));
    axis2_http_simple_request_add_header(request, m_env, header);
    client = axis2_http_client_create(m_env, url);

    /* if you weant to test the proxy uncomment following */
    /*axis2_http_client_set_proxy(client, env, "127.0.0.1", 8080); */

    /* Add CA/Server certificate */
    status = axis2_http_client_set_server_cert(client, m_env,
                                               "/home/dummy/dummyCA/demoCA/cacert.pem");

    status = axis2_http_client_send(client, m_env, request, NULL);
    ASSERT_GE(status, 0);
    if (status < 0)
    {
        printf("Test FAILED .........Can't send the request. Status :%d\n",
               status);
        return;
    }
    status = axis2_http_client_recieve_header(client, m_env);
    ASSERT_GE(status, 0);
    if (status < 0)
    {
        printf("Test FAILED ......... Can't recieve. Status: %d\n", status);
        return;
    }
    response = axis2_http_client_get_response(client, env);
    ASSERT_NE(response, nullptr);
    if (!response)
    {
        printf("Test Failed : NULL response");
        return;
    }
    printf("Content Type :%s\n",
           axis2_http_simple_response_get_content_type(response, m_env));
    printf("Content Length :%d\n",
           axis2_http_simple_response_get_content_length(response, m_env));
    printf("Status code :%d\n", status);
    body_bytes_len =
        axis2_http_simple_response_get_body_bytes(response, m_env, &body_bytes);

    axis2_http_client_free(client, m_env);
    axis2_http_simple_request_free(request, m_env);
    axutil_stream_free(request_body, m_env);
    AXIS2_FREE(m_env->allocator, body_bytes);
    printf("Finished https_client tests ..........\n\n");
#endif
}

#ifdef AXIS2_JSON_ENABLED
TEST_F(TestHTTPTransport, test_json)
{
    axiom_node_t* root_node = NULL;

    /* JSON_C */
    axis2_json_writer_t* json_writer;
    axis2_json_reader_t* json_reader;
    unsigned i;
    axis2_char_t* xml_str;
    const axis2_char_t* result_str;
    int passed = 0;
    int failed = 0;


    const char* xml_data[] =
    {
        "<root><child1>value 1</child1><child2>value 2</child2></root>",
        "<root> \t\r\n<child1>value 1</child1> <child2>value 2</child2>\n</root>",
        "<root><child1><sub>value 1</sub></child1><child2>value 2</child2></root>",
        "<root><child></child><ch>value 1</ch><ch>value 2</ch><ch>value 3</ch></root>",
        "<root><child></child><ch><sub>11</sub><sub>12</sub></ch><ch><sub>11</sub><sub>12</sub></ch></root>",
        "<root><ch xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:nil=\"true\"></ch></root>"
    };

    const char* json_data_mapped[] =
    {
        "{\"root\":{\"child1\":\"value 1\",\"child2\":\"value 2\"}}",
        "{\"root\":{\"child1\":{\"sub\":\"value 1\"},\"child2\":\"value 2\"}}",
        "{\"root\":{\"child\":\"\",\"ch\":[\"value 1\",\"value 2\",\"value 3\"]}}",
        "{\"root\":{\"child\":\"\",\"ch\":[{\"sub\":[\"11\",\"12\"]},{\"sub\":[\"11\",\"12\"]}]}}",
        "{\"root\":{\"ch\":null}}"
    };

    int xml2mapped[sizeof(xml_data) / sizeof(xml_data[0])] =
    {
        0, 0, 1, 2, 3, 4
    };

    int mapped2xml[sizeof(json_data_mapped) / sizeof(json_data_mapped[0])] =
    {
        0, 2, 3, 4, 5
    };

    printf(" ######################## testing xml -> json ########################## \n");

    for (i = 0; i < sizeof(xml_data) / sizeof(xml_data[0]); ++i)
    {
        const char* xml = xml_data[i];

        root_node = axiom_node_create_from_buffer(m_env, (axis2_char_t*)xml);

        xml_str = axiom_node_to_string(root_node, m_env);

        printf(" =============== source XML ================\n%s\n"
               " ===========================================\n",
               xml_str);
        AXIS2_FREE(env->allocator, xml_str);

        json_writer = axis2_json_writer_create(m_env);
        axis2_json_writer_write(json_writer, root_node, m_env);

        result_str = axis2_json_writer_get_json_string(json_writer, m_env, 0);
        printf(" ============= resulting JSON ==============\n%s\n"
               " ===========================================\n",
               result_str);

        if (strcmp(result_str, json_data_mapped[xml2mapped[i]]))
        {
            ++failed;
            printf("TEST FAILED\nexpected result: %s\n", json_data_mapped[xml2mapped[i]]);
        }
        else
        {
            ++passed;
            printf("test passed\n\n");
        }

        axis2_json_writer_free(json_writer, m_env);

        axiom_node_free_tree(root_node, m_env);
    }


    printf(" ######################## testing json -> xml ########################## \n");

    for (i = 0; i < sizeof(json_data_mapped) / sizeof(json_data_mapped[0]); ++i)
    {
        const char* json = json_data_mapped[i];
        int length = strlen(json);

        json_reader = axis2_json_reader_create_for_memory(m_env, json, length);
        if (!json_reader)
        {
            printf("Failed to create json_reader");
            return;
        }

        if (axis2_json_reader_read(json_reader, m_env) != AXIS2_SUCCESS)
        {
            printf("Failed to axis2_json_reader_read");
            return;
        }

        root_node = axis2_json_reader_get_root_node(json_reader, m_env);
        if (!root_node)
        {
            printf("Failed to get root_node");
            return;
        }


        printf(" =============== source JSON ================\n%s\n"
               " ===========================================\n",
               json);


        xml_str = axiom_node_to_string(root_node, m_env);

        printf(" =============== resulting XML ================\n%s\n"
               " ===========================================\n",
               xml_str);

        if (strcmp(xml_str, xml_data[mapped2xml[i]]))
        {
            ++failed;
            printf("TEST FAILED\nExpected result: %s\n", xml_data[mapped2xml[i]]);
        }
        else
        {
            ++passed;
            printf("test passed\n\n");
        }
        AXIS2_FREE(m_env->allocator, xml_str);

        axis2_json_reader_free(json_reader, m_env);
        axiom_node_free_tree(root_node, m_env);
    }

    printf("JSON tests passed: %d, failed: %d\n", passed, failed);
}
#endif

