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
#include <axis2_http_request_line.h>
#include <axis2_http_status_line.h>
#include <axis2_http_header.h>
#include <axutil_error_default.h>
#include <axutil_url.h>
#include <axis2_http_client.h>
#include <cut_defs.h>
#include <cut_http_server.h>
#ifdef AXIS2_JSON_ENABLED
#include <axiom.h>
#include <axutil_utils.h>
#include <json.h>
#include <axis2_json_writer.h>
#include <axis2_json_reader.h>
#endif

void
test_http_request_line(
    const axutil_env_t * env)
{
    char *request_line_str = axutil_strdup(env,
        "POST http://ws.apache.org/axis2/c/services/test_svc/test_op?x=1 HTTP/1.1\r\n");
    axis2_http_request_line_t *request_line;

    printf("Starting http_request_line tests\n");
    request_line = axis2_http_request_line_parse_line(env, request_line_str);
    printf("Method: %s|URI: %s|HTTP Version: %s|\n",
           axis2_http_request_line_get_method(request_line, env),
           axis2_http_request_line_get_uri(request_line, env),
           axis2_http_request_line_get_http_version(request_line, env));
    
    /* To avoid warning of not using cut_ptr_equal */
    CUT_ASSERT_PTR_EQUAL(NULL, NULL, 0);
    /* To avoid warning of not using cut_int_equal */
    CUT_ASSERT_INT_EQUAL(0, 0, 0);
    /* To avoid warning of not using cut_str_equal */
    CUT_ASSERT_STR_EQUAL("", "", 0);

    axis2_http_request_line_free(request_line, env);
    printf("Finished http_request_line tests ..........\n\n");
}

void
test_http_status_line(
    const axutil_env_t * env)
{
    const char *status_line_str = "HTTP/1.1 202 Accepted\r\n";
    axis2_http_status_line_t *status_line;

    printf("Starting http_status_line tests\n");
    status_line = axis2_http_status_line_create(env, status_line_str);
    printf("Staus Line starts with HTTP ? :%d\n",
           axis2_http_status_line_starts_with_http(status_line, env));
    printf("HTTP Version :%s|Status Code:%d|Reason Phrase:%s|\n",
           axis2_http_status_line_get_http_version(status_line, env),
           axis2_http_status_line_get_status_code(status_line, env),
           axis2_http_status_line_get_reason_phrase(status_line, env));
    printf("to_string :%s\n",
           axis2_http_status_line_to_string(status_line, env));
    axis2_http_status_line_free(status_line, env);
    printf("Finished http_status_line tests ..........\n\n");
}

void
test_http_header(
    const axutil_env_t * env)
{
    const char *header_name = "Content-Type";
    const char *header_value = "text/xml";
    const char *str_header =  axutil_strdup(env,"Content-Type: text/xml; charset=UTF-8\r\n");
    axis2_http_header_t *http_header;
    axis2_char_t *external_form = NULL;

    printf("Starting http_header tests\n");
    http_header = axis2_http_header_create(env, header_name, header_value);
    external_form = axis2_http_header_to_external_form(http_header, env);
    printf("Header Name :%s|Header Value:%s|External Form:%s\n",
           axis2_http_header_get_name(http_header, env),
           axis2_http_header_get_value(http_header, env), external_form);
    AXIS2_FREE(env->allocator, external_form);
    axis2_http_header_free(http_header, env);

    http_header = axis2_http_header_create_by_str(env, str_header);
    printf("Finished http_header tests ..........\n\n");
}

void
test_url(
    const axutil_env_t * env)
{
    const axis2_char_t *str_url = "http://ws.apache.org/axis2/";
    axutil_url_t *url = axutil_url_parse_string(env, str_url);
    if (!url)
    {
        printf("URL is NULL\n");
        return;
    }
    printf("Starting URL Test ....\n");
    printf
        ("Parsed URL : \n Protocol :%s|\n Host :%s|\n Port :%d|\n Path : %s|\n",
         axutil_url_get_protocol(url, env), axutil_url_get_host(url, env),
         axutil_url_get_port(url, env), axutil_url_get_path(url, env));
    printf("End of URL Test ... \n");
    axutil_url_free(url, env);
}

void
test_http_client(
    axutil_env_t * env)
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
    printf("Starting http_client tests\n");
	if ( ut_start_http_server(env) != 0 ) return;
    request_line = axis2_http_request_line_create(env, "POST",
                                                  "/axis2/services/echo/echo",
                                                  "HTTP/1.1");
    request = axis2_http_simple_request_create(env, request_line,
                                               NULL, 0, NULL);
    axis2_http_simple_request_set_body_string(request, env, content, strlen(content));
    url = axutil_url_create(env, "http", "localhost", 9090, NULL);
	sprintf(tmpbuf,"%s:%d", axutil_url_get_host(url, env), axutil_url_get_port(url, env));
    header =
        axis2_http_header_create(env, "Host", tmpbuf);
    axis2_http_simple_request_add_header(request, env, header);
    header =
        axis2_http_header_create(env, "Content-Type", "application/soap+xml");
    axis2_http_simple_request_add_header(request, env, header);
	sprintf(tmpbuf,"%d", (int) strlen(content));
    header =
        axis2_http_header_create(env, "Content-Length", tmpbuf);
    axis2_http_simple_request_add_header(request, env, header);
    client = axis2_http_client_create(env, url);

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Trying to call axis2_http_client_send");
    status = axis2_http_client_send(client, env, request, NULL);
 	CUT_ASSERT(status == AXIS2_SUCCESS);
    if (status != AXIS2_SUCCESS)
    {
        printf("Test FAILED .........Can't send the request. Status : %d\n",
               status);
        return;
    }
    status = axis2_http_client_recieve_header(client, env);
    CUT_ASSERT(status == 200);
    if (status != 200)
    {
        printf("Test FAILED ......... Can't recieve. Status: %d\n", status);
        return;
    }
    response = axis2_http_client_get_response(client, env);
	CUT_ASSERT(response != NULL);
    if (!response)
    {
        printf("Test Failed : NULL response");
        return;
    }
    printf("Content Type :%s\n",
           axis2_http_simple_response_get_content_type(response, env));
    printf("Content Length :%d\n",
           axis2_http_simple_response_get_content_length(response, env));
    printf("Status code :%d\n", status);
    body_bytes_len =
        axis2_http_simple_response_get_body_bytes(response, env, &body_bytes);
    printf("body :%s\n%d", body_bytes , body_bytes_len);

    axis2_http_client_free(client, env);
    axis2_http_simple_request_free(request, env);
	ut_stop_http_server(env);
    AXIS2_FREE(env->allocator, body_bytes);
    printf("Finished http_client tests ..........\n\n");
}

void
test_https_client(
    const axutil_env_t * env)
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
    request_line = axis2_http_request_line_create(env, "GET", "/", "HTTP/1.0");
    request_body = axutil_stream_create_basic(env);
    request = axis2_http_simple_request_create(env, request_line,
                                               NULL, 0, NULL);
    url = axutil_url_create(env, "https", "localhost", 9099, NULL);

    header =
        axis2_http_header_create(env, "Host", axutil_url_get_host(url, env));
    axis2_http_simple_request_add_header(request, env, header);
    client = axis2_http_client_create(env, url);

    /* if you weant to test the proxy uncomment following */
    /*axis2_http_client_set_proxy(client, env, "127.0.0.1", 8080); */

    /* Add CA/Server certificate */
    status = axis2_http_client_set_server_cert(client, env,
                                               "/home/dummy/dummyCA/demoCA/cacert.pem");

    status = axis2_http_client_send(client, env, request, NULL);
    if (status < 0)
    {
        printf("Test FAILED .........Can't send the request. Status :%d\n",
               status);
        return;
    }
    status = axis2_http_client_recieve_header(client, env);
    if (status < 0)
    {
        printf("Test FAILED ......... Can't recieve. Status: %d\n", status);
        return;
    }
    response = axis2_http_client_get_response(client, env);
    if (!response)
    {
        printf("Test Failed : NULL response");
        return;
    }
    printf("Content Type :%s\n",
           axis2_http_simple_response_get_content_type(response, env));
    printf("Content Length :%d\n",
           axis2_http_simple_response_get_content_length(response, env));
    printf("Status code :%d\n", status);
    body_bytes_len =
        axis2_http_simple_response_get_body_bytes(response, env, &body_bytes);

    axis2_http_client_free(client, env);
    axis2_http_simple_request_free(request, env);
    axutil_stream_free(request_body, env);
    AXIS2_FREE(env->allocator, body_bytes);
    printf("Finished https_client tests ..........\n\n");
#endif
}

#ifdef AXIS2_JSON_ENABLED
void
test_json(
    const axutil_env_t * env)
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

        root_node = axiom_node_create_from_buffer(env, (axis2_char_t*)xml);

        xml_str = axiom_node_to_string(root_node, env);

        printf(" =============== source XML ================\n%s\n"
               " ===========================================\n",
               xml_str);
        AXIS2_FREE(env->allocator, xml_str);

        json_writer = axis2_json_writer_create(env);
        axis2_json_writer_write(json_writer, root_node, env);

        result_str = axis2_json_writer_get_json_string(json_writer, env, 0);
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

        axis2_json_writer_free(json_writer, env);

        axiom_node_free_tree(root_node, env);
    }


    printf(" ######################## testing json -> xml ########################## \n");

    for (i = 0; i < sizeof(json_data_mapped) / sizeof(json_data_mapped[0]); ++i)
    {
        const char* json = json_data_mapped[i];
        int length = strlen(json);

        json_reader = axis2_json_reader_create_for_memory(env, json, length);
        if (!json_reader)
        {
            printf("Failed to create json_reader");
            return;
        }

        if (axis2_json_reader_read(json_reader, env) != AXIS2_SUCCESS)
        {
            printf("Failed to axis2_json_reader_read");
            return;
        }

        root_node = axis2_json_reader_get_root_node(json_reader, env);
        if (!root_node)
        {
            printf("Failed to get root_node");
            return;
        }


        printf(" =============== source JSON ================\n%s\n"
               " ===========================================\n",
               json);


        xml_str = axiom_node_to_string(root_node, env);

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
        AXIS2_FREE(env->allocator, xml_str);

        axis2_json_reader_free(json_reader, env);
        axiom_node_free_tree(root_node, env);
    }

    printf("JSON tests passed: %d, failed: %d\n", passed, failed);
}
#endif


int
main(
    void)
{
    axutil_env_t *env = cut_setup_env("test HTTP server");
    CUT_ASSERT(env != NULL);
	if ( env != NULL ) {
       test_http_request_line(env);
       test_http_status_line(env);
       test_http_header(env);
       test_http_client(env);
       test_https_client(env);
       test_url(env);
#ifdef AXIS2_JSON_ENABLED
       test_json(env);
#endif
       axutil_env_free(env);
	}
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}

