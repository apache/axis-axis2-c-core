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

#include <stdio.h>
#include <axis2_http_request_line.h>
#include <axis2_http_status_line.h>
#include <axis2_http_header.h>
#include <axis2_error_default.h>
#include <axis2_url.h>
#include <axis2_http_client.h>

typedef struct a
{
    axis2_char_t *value;
}
a;

axis2_env_t *test_init()
{
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_error_t *error = axis2_error_create(allocator);
    axis2_env_t *env = axis2_env_create_with_error(allocator, error);
    return env;
}

void test_http_request_line(const axis2_env_t *env)
{
    const char *request_line_str = "POST http://ws.apache.org/axis2/c/services/test_svc/test_op?x=1 HTTP/1.1\r\n";
    axis2_http_request_line_t *request_line;

    printf("Starting http_request_line tests\n");
    request_line = axis2_http_request_line_parse_line(env, request_line_str);
    printf("Method: %s|URI: %s|HTTP Version: %s|\n", AXIS2_HTTP_REQUEST_LINE_GET_METHOD
            (request_line, env), AXIS2_HTTP_REQUEST_LINE_GET_URI(request_line, env),
            AXIS2_HTTP_REQUEST_LINE_GET_HTTP_VERSION(request_line, env));
    AXIS2_HTTP_REQUEST_LINE_FREE(request_line, env);
    printf("Finished http_request_line tests ..........\n\n");
}


void test_http_status_line(const axis2_env_t *env)
{
    const char *status_line_str = "HTTP/1.1 202 Accepted\r\n";
    axis2_http_status_line_t *status_line;

    printf("Starting http_status_line tests\n");
    status_line = axis2_http_status_line_create(env, status_line_str);
    printf("Staus Line starts with HTTP ? :%d\n",
            AXIS2_HTTP_STATUS_LINE_STARTS_WITH_HTTP(status_line, env));
    printf("HTTP Version :%s|Status Code:%d|Reason Phrase:%s|\n",
            AXIS2_HTTP_STATUS_LINE_GET_HTTP_VERSION(status_line, env),
            AXIS2_HTTP_STATUS_LINE_GET_STATUS_CODE(status_line, env),
            AXIS2_HTTP_STATUS_LINE_GET_REASON_PHRASE(status_line, env));
    printf("to_string :%s\n", AXIS2_HTTP_STATUS_LINE_TO_STRING(status_line, env));
    AXIS2_HTTP_STATUS_LINE_FREE(status_line, env);
    printf("Finished http_status_line tests ..........\n\n");
}

void test_http_header(const axis2_env_t *env)
{
    const char *header_name = "Content-Type";
    const char *header_value = "text/xml";
    const char *str_header = "Content-Type: text/xml; charset=UTF-8\r\n";
    axis2_http_header_t *http_header;
    axis2_char_t *external_form = NULL;

    printf("Starting http_header tests\n");
    http_header = axis2_http_header_create(env, header_name, header_value);
    external_form = AXIS2_HTTP_HEADER_TO_EXTERNAL_FORM(http_header, env);
    printf("Heder Name :%s|Header Value:%s|External Form:%s\n",
            AXIS2_HTTP_HEADER_GET_NAME(http_header, env),
            AXIS2_HTTP_HEADER_GET_VALUE(http_header, env),
            external_form);
    AXIS2_FREE(env->allocator, external_form);
    AXIS2_HTTP_HEADER_FREE(http_header, env);

    http_header = axis2_http_header_create_by_str(env, str_header);
    printf("Finished http_header tests ..........\n\n");
}

void test_url(const axis2_env_t *env)
{
    const axis2_char_t *str_url = "http://ws.apache.org/axis2/";
    axis2_url_t *url = axis2_url_parse_string(env, str_url);
    if (NULL == url)
    {
        printf("URL is NULL\n");
        return;
    }
    printf("Starting URL Test ....\n");
    printf("Parsed URL : \n Protocol :%s|\n Server :%s|\n Port :%d|\n Path : %s|\n",
            AXIS2_URL_GET_PROTOCOL(url, env), AXIS2_URL_GET_SERVER(url, env),
            AXIS2_URL_GET_PORT(url, env), AXIS2_URL_GET_PATH(url, env));
    printf("End of URL Test ... \n");
    AXIS2_URL_FREE(url, env);
}

void test_http_client(const axis2_env_t *env)
{
    axis2_http_client_t *client = NULL;
    axis2_http_simple_request_t *request = NULL;
    axis2_http_request_line_t *request_line = NULL;
    axis2_url_t *url = NULL;
    axis2_http_header_t *header = NULL;
    axis2_stream_t *request_body = NULL;
    axis2_http_simple_response_t *response = NULL;
    int status = 0;
    char *body_bytes = NULL;
    int body_bytes_len = 0;

    printf("Starting http_client tests\n");
    request_line = axis2_http_request_line_create(env, "GET",
            "/axis2/services", "HTTP/1.0");
    request_body = axis2_stream_create_basic(env);
    request = axis2_http_simple_request_create(env, request_line,
            NULL, 0, NULL);
    url = axis2_url_create(env, "http", "localhost", 80,
            NULL);
    header = axis2_http_header_create(env, "Host", AXIS2_URL_GET_SERVER(url, env));
    AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(request, env, header);
    client = axis2_http_client_create(env, url);

    status = AXIS2_HTTP_CLIENT_SEND(client, env, request);
    if (status < 0)
    {
        printf("Test FAILED .........Can't send the request. Status :%d\n", status);
        return;
    }
    status = AXIS2_HTTP_CLIENT_RECIEVE_HEADER(client, env);
    if (status < 0)
    {
        printf("Test FAILED ......... Can't recieve. Status: %d\n", status);
        return;
    }
    response = AXIS2_HTTP_CLIENT_GET_RESPONSE(client, env);
    if (NULL == response)
    {
        printf("Test Failed : NULL response");
        return;
    }
    printf("Content Type :%s\n", AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_TYPE(
                response, env));
    printf("Content Length :%d\n", AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_LENGTH(
                response, env));
    printf("Status code :%d\n", status);
    body_bytes_len = AXIS2_HTTP_SIMPLE_RESPONSE_GET_BODY_BYTES(response, env, &body_bytes);
    printf("body :%s\n", body_bytes);

    AXIS2_HTTP_CLIENT_FREE(client, env);
    AXIS2_HTTP_SIMPLE_REQUEST_FREE(request, env);
    AXIS2_STREAM_FREE(request_body, env);
    AXIS2_FREE(env->allocator, body_bytes);
    printf("Finished http_client tests ..........\n\n");
}

void test_https_client(const axis2_env_t *env)
{
#ifndef AXIS2_SSL_ENABLED
    return;
#else
    axis2_http_client_t *client = NULL;
    axis2_http_simple_request_t *request = NULL;
    axis2_http_request_line_t *request_line = NULL;
    axis2_url_t *url = NULL;
    axis2_http_header_t *header = NULL;
    axis2_stream_t *request_body = NULL;
    axis2_http_simple_response_t *response = NULL;
    int status = 0;
    char *body_bytes = NULL;
    int body_bytes_len = 0;

    printf("Starting https_client tests\n");
    request_line = axis2_http_request_line_create(env, "GET",
            "/", "HTTP/1.0");
    request_body = axis2_stream_create_basic(env);
    request = axis2_http_simple_request_create(env, request_line,
            NULL, 0, NULL);
    url = axis2_url_create(env, "https", "localhost", 443,
            NULL);
    /* Add an ssl certificate variable */
    setenv("AXIS2_SSL_CA_FILE", "cert.pem", 1);
    header = axis2_http_header_create(env, "Host", AXIS2_URL_GET_SERVER(url, env));
    AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(request, env, header);
    client = axis2_http_client_create(env, url);

    /* if you weant to test the proxy uncomment following */
    /*AXIS2_HTTP_CLIENT_SET_PROXY(client, env, "127.0.0.1", 8080);*/

    status = AXIS2_HTTP_CLIENT_SEND(client, env, request);
    if (status < 0)
    {
        printf("Test FAILED .........Can't send the request. Status :%d\n", status);
        return;
    }
    status = AXIS2_HTTP_CLIENT_RECIEVE_HEADER(client, env);
    if (status < 0)
    {
        printf("Test FAILED ......... Can't recieve. Status: %d\n", status);
        return;
    }
    response = AXIS2_HTTP_CLIENT_GET_RESPONSE(client, env);
    if (NULL == response)
    {
        printf("Test Failed : NULL response");
        return;
    }
    printf("Content Type :%s\n", AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_TYPE(
                response, env));
    printf("Content Length :%d\n", AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_LENGTH(
                response, env));
    printf("Status code :%d\n", status);
    body_bytes_len = AXIS2_HTTP_SIMPLE_RESPONSE_GET_BODY_BYTES(response, env, &body_bytes);

    AXIS2_HTTP_CLIENT_FREE(client, env);
    AXIS2_HTTP_SIMPLE_REQUEST_FREE(request, env);
    AXIS2_STREAM_FREE(request_body, env);
    AXIS2_FREE(env->allocator, body_bytes);
    printf("Finished https_client tests ..........\n\n");
#endif
}

int main(void)
{
    axis2_env_t *env = test_init();
    test_http_request_line(env);
    test_http_status_line(env);
    test_http_header(env);
    test_http_client(env);
    test_https_client(env);
    test_url(env);

    axis2_env_free(env);
    return 0;
}
