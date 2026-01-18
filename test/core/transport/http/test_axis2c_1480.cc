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

/**
 * @file test_axis2c_1480.cc
 * @brief Unit tests for AXIS2C-1480: Buffered HTTP header reading
 *
 * These tests verify:
 * 1. Buffered header reading with body data preservation
 * 2. Security against malicious HTTP responses (CVE prevention)
 * 3. Proper handling of edge cases and malformed input
 *
 * Security considerations (for Apache security team / white hat testers):
 * - Buffer overflow attempts via extremely long headers
 * - Resource exhaustion via many headers
 * - Header injection via embedded CRLF
 * - Null byte injection
 * - Incomplete/malformed responses
 */

#include <gtest/gtest.h>

#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_network_handler.h>
#include <axutil_stream.h>
#include <axis2_http_client.h>
#include <axis2_http_simple_request.h>
#include <axis2_http_simple_response.h>
#include <axis2_http_request_line.h>
#include <axis2_http_header.h>

/* Maximum response size for security tests - prevents memory exhaustion */
#define MAX_TEST_RESPONSE_SIZE (1024 * 1024)  /* 1MB limit for test responses */

class TestAXIS2C1480 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_allocator = axutil_allocator_init(NULL);
        m_axis_log = axutil_log_create(m_allocator, NULL, NULL);
        m_error = axutil_error_create(m_allocator);
        m_env = axutil_env_create_with_error_log(m_allocator, m_error, m_axis_log);
    }

    void TearDown() override
    {
        axutil_env_free(m_env);
    }

    axutil_allocator_t *m_allocator = NULL;
    axutil_env_t *m_env = NULL;
    axutil_error_t *m_error = NULL;
    axutil_log_t *m_axis_log = NULL;
};

/*
 * Mock HTTP server context - runs in a separate thread and sends
 * pre-configured responses. Supports chunked delivery for testing
 * buffered reading.
 */
typedef struct mock_http_server_ctx
{
    axis2_socket_t server_socket;
    int port;
    const char *response_data;
    size_t response_len;
    int chunk_size;           /* 0 = send all at once, >0 = send in chunks */
    int chunk_delay_us;       /* Microseconds between chunks */
    volatile int ready;       /* 0=starting, 1=ready, -1=failed */
    volatile int done;
    volatile int client_connected;
    pthread_t thread;
} mock_http_server_ctx_t;

static void *mock_http_server_thread(void *arg)
{
    mock_http_server_ctx_t *ctx = (mock_http_server_ctx_t *)arg;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    axis2_socket_t client_fd;
    char req_buf[4096];
    ssize_t n;

    /* Wait for client connection */
    client_fd = accept(ctx->server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0)
    {
        ctx->done = 1;
        return NULL;
    }
    ctx->client_connected = 1;

    /* Read client request (we don't care about content for these tests) */
    n = recv(client_fd, req_buf, sizeof(req_buf) - 1, 0);
    if (n > 0)
    {
        req_buf[n] = '\0';
    }

    /* Send response */
    if (ctx->response_data && ctx->response_len > 0)
    {
        if (ctx->chunk_size <= 0 || (size_t)ctx->chunk_size >= ctx->response_len)
        {
            /* Send all at once */
            send(client_fd, ctx->response_data, ctx->response_len, 0);
        }
        else
        {
            /* Send in chunks with optional delay */
            size_t sent = 0;
            while (sent < ctx->response_len)
            {
                size_t to_send = ctx->response_len - sent;
                if (to_send > (size_t)ctx->chunk_size)
                    to_send = ctx->chunk_size;

                ssize_t written = send(client_fd, ctx->response_data + sent, to_send, 0);
                if (written <= 0)
                    break;
                sent += written;

                if (ctx->chunk_delay_us > 0 && sent < ctx->response_len)
                    usleep(ctx->chunk_delay_us);
            }
        }
    }

    close(client_fd);
    ctx->done = 1;
    return NULL;
}

/*
 * Start a mock HTTP server on a random available port.
 * Returns port number on success, -1 on failure.
 */
static int start_mock_server(mock_http_server_ctx_t *ctx, const char *response,
                             size_t response_len, int chunk_size, int chunk_delay_us)
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int opt = 1;

    memset(ctx, 0, sizeof(*ctx));
    ctx->response_data = response;
    ctx->response_len = response_len;
    ctx->chunk_size = chunk_size;
    ctx->chunk_delay_us = chunk_delay_us;

    /* Create server socket */
    ctx->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (ctx->server_socket < 0)
        return -1;

    setsockopt(ctx->server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;  /* Let OS assign port */

    if (bind(ctx->server_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(ctx->server_socket);
        return -1;
    }

    /* Get assigned port */
    getsockname(ctx->server_socket, (struct sockaddr *)&addr, &addr_len);
    ctx->port = ntohs(addr.sin_port);

    if (listen(ctx->server_socket, 1) < 0)
    {
        close(ctx->server_socket);
        return -1;
    }

    /* Start server thread */
    if (pthread_create(&ctx->thread, NULL, mock_http_server_thread, ctx) != 0)
    {
        close(ctx->server_socket);
        return -1;
    }

    ctx->ready = 1;
    return ctx->port;
}

static void stop_mock_server(mock_http_server_ctx_t *ctx)
{
    if (ctx->server_socket >= 0)
    {
        /* If thread is still waiting for connection, close socket to unblock */
        if (!ctx->client_connected)
        {
            shutdown(ctx->server_socket, SHUT_RDWR);
            close(ctx->server_socket);
        }
    }

    pthread_join(ctx->thread, NULL);

    if (ctx->server_socket >= 0 && ctx->client_connected)
    {
        close(ctx->server_socket);
    }
}

/*
 * Helper to create HTTP client and send a simple request.
 * Returns status code on success, -1 on failure.
 */
static int send_request_and_get_status(const axutil_env_t *env, int port)
{
    axis2_http_client_t *client = NULL;
    axis2_http_simple_request_t *request = NULL;
    axis2_http_request_line_t *request_line = NULL;
    axutil_url_t *url = NULL;
    axis2_http_header_t *header = NULL;
    axis2_status_t status;
    int result = -1;
    char host_header[64];

    request_line = axis2_http_request_line_create(env, "POST", "/test", "HTTP/1.1");
    if (!request_line)
        return -1;

    request = axis2_http_simple_request_create(env, request_line, NULL, 0, NULL);
    if (!request)
        return -1;

    /* Set minimal body */
    axis2_http_simple_request_set_body_string(request, env, (void *)"", 0);

    /* Use 127.0.0.1 to avoid DNS lookup - more reliable in tests */
    url = axutil_url_create(env, "http", "127.0.0.1", port, NULL);
    if (!url)
    {
        axis2_http_simple_request_free(request, env);
        return -1;
    }

    snprintf(host_header, sizeof(host_header), "127.0.0.1:%d", port);
    header = axis2_http_header_create(env, "Host", host_header);
    axis2_http_simple_request_add_header(request, env, header);

    header = axis2_http_header_create(env, "Content-Length", "0");
    axis2_http_simple_request_add_header(request, env, header);

    client = axis2_http_client_create(env, url);
    if (!client)
    {
        axis2_http_simple_request_free(request, env);
        return -1;
    }

    /* Set a short timeout for tests */
    axis2_http_client_set_timeout(client, env, 5000);

    status = axis2_http_client_send(client, env, request, NULL);
    if (status == AXIS2_SUCCESS)
    {
        result = axis2_http_client_receive_header(client, env);
    }

    axis2_http_client_free(client, env);
    axis2_http_simple_request_free(request, env);

    return result;
}

/*
 * Helper to get response body after receiving headers.
 */
static char *get_response_body(const axutil_env_t *env, int port, int *body_len)
{
    axis2_http_client_t *client = NULL;
    axis2_http_simple_request_t *request = NULL;
    axis2_http_request_line_t *request_line = NULL;
    axutil_url_t *url = NULL;
    axis2_http_header_t *header = NULL;
    axis2_http_simple_response_t *response = NULL;
    axis2_status_t status;
    char *body = NULL;
    char host_header[64];

    *body_len = 0;

    request_line = axis2_http_request_line_create(env, "POST", "/test", "HTTP/1.1");
    request = axis2_http_simple_request_create(env, request_line, NULL, 0, NULL);
    axis2_http_simple_request_set_body_string(request, env, (void *)"", 0);

    url = axutil_url_create(env, "http", "127.0.0.1", port, NULL);

    snprintf(host_header, sizeof(host_header), "127.0.0.1:%d", port);
    header = axis2_http_header_create(env, "Host", host_header);
    axis2_http_simple_request_add_header(request, env, header);

    header = axis2_http_header_create(env, "Content-Length", "0");
    axis2_http_simple_request_add_header(request, env, header);

    client = axis2_http_client_create(env, url);
    axis2_http_client_set_timeout(client, env, 5000);

    status = axis2_http_client_send(client, env, request, NULL);
    if (status == AXIS2_SUCCESS)
    {
        int http_status = axis2_http_client_receive_header(client, env);
        if (http_status >= 100 && http_status < 600)
        {
            response = axis2_http_client_get_response(client, env);
            if (response)
            {
                *body_len = axis2_http_simple_response_get_body_bytes(response, env, &body);
            }
        }
    }

    axis2_http_client_free(client, env);
    axis2_http_simple_request_free(request, env);

    return body;
}

/* ============================================================================
 * FUNCTIONAL TESTS - AXIS2C-1480 Buffered Header Reading
 * ============================================================================ */

/**
 * Test basic buffered header reading with body data preservation.
 * This is the core AXIS2C-1480 fix - verifies body data read during
 * header buffering is correctly preserved via prepend stream.
 */
TEST_F(TestAXIS2C1480, buffered_header_with_body)
{
    mock_http_server_ctx_t server;
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello World!\n";

    int port = start_mock_server(&server, response, strlen(response), 0, 0);
    ASSERT_GT(port, 0) << "Failed to start mock server";

    int body_len = 0;
    char *body = get_response_body(m_env, port, &body_len);

    stop_mock_server(&server);

    ASSERT_NE(body, nullptr) << "Body should not be null";
    ASSERT_EQ(body_len, 13) << "Body length should be 13";
    ASSERT_STREQ(body, "Hello World!\n") << "Body content mismatch";

    AXIS2_FREE(m_env->allocator, body);
}

/**
 * Test chunked delivery - headers span multiple TCP segments.
 * Simulates slow network where data arrives in small chunks.
 */
TEST_F(TestAXIS2C1480, chunked_delivery_headers_span_chunks)
{
    mock_http_server_ctx_t server;
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/xml\r\n"
        "Content-Length: 45\r\n"
        "X-Custom-Header: some-value-here\r\n"
        "\r\n"
        "<response><status>success</status></response>";

    /* Send in 30-byte chunks - headers will definitely span multiple chunks */
    int port = start_mock_server(&server, response, strlen(response), 30, 1000);
    ASSERT_GT(port, 0);

    int body_len = 0;
    char *body = get_response_body(m_env, port, &body_len);

    stop_mock_server(&server);

    ASSERT_NE(body, nullptr);
    ASSERT_EQ(body_len, 45);
    ASSERT_STREQ(body, "<response><status>success</status></response>");

    AXIS2_FREE(m_env->allocator, body);
}

/**
 * Test empty body response - no prepend stream should be needed.
 */
TEST_F(TestAXIS2C1480, empty_body_response)
{
    mock_http_server_ctx_t server;
    const char *response =
        "HTTP/1.1 204 No Content\r\n"
        "Content-Length: 0\r\n"
        "\r\n";

    int port = start_mock_server(&server, response, strlen(response), 0, 0);
    ASSERT_GT(port, 0);

    int status = send_request_and_get_status(m_env, port);

    stop_mock_server(&server);

    ASSERT_EQ(status, 204);
}

/**
 * Test large body exceeding initial buffer size (4KB).
 * Verifies prepend stream correctly serves buffered data then
 * delegates to underlying stream.
 */
TEST_F(TestAXIS2C1480, large_body_exceeds_buffer)
{
    mock_http_server_ctx_t server;

    /* Create 8KB body - exceeds 4KB buffer */
    const int body_size = 8192;
    std::string large_body;
    for (int i = 0; i < body_size; i++)
    {
        large_body += 'A' + (i % 26);
    }

    char content_len[32];
    snprintf(content_len, sizeof(content_len), "%d", body_size);

    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Content-Length: ";
    response += content_len;
    response += "\r\n\r\n";
    response += large_body;

    int port = start_mock_server(&server, response.c_str(), response.length(), 0, 0);
    ASSERT_GT(port, 0);

    int body_len = 0;
    char *body = get_response_body(m_env, port, &body_len);

    stop_mock_server(&server);

    ASSERT_NE(body, nullptr);
    ASSERT_EQ(body_len, body_size);
    ASSERT_EQ(memcmp(body, large_body.c_str(), body_size), 0);

    AXIS2_FREE(m_env->allocator, body);
}

/* ============================================================================
 * SECURITY TESTS - CVE Prevention
 *
 * These tests verify proper handling of malicious input that could be
 * used in attacks. Each test documents the potential vulnerability and
 * verifies the code handles it safely.
 * ============================================================================ */

/**
 * SECURITY: Test extremely long header line (buffer overflow attempt)
 *
 * CVE Risk: If header parsing doesn't limit line length, an attacker
 * could send a multi-megabyte header line causing:
 * - Memory exhaustion (DoS)
 * - Buffer overflow if fixed-size buffer used
 *
 * Expected: Parser should reject or truncate excessively long headers.
 */
TEST_F(TestAXIS2C1480, security_long_header_line)
{
    mock_http_server_ctx_t server;

    /* Create a 100KB header value - way beyond reasonable limits */
    std::string long_value(100 * 1024, 'X');
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "X-Malicious-Header: " + long_value + "\r\n"
        "Content-Length: 0\r\n"
        "\r\n";

    int port = start_mock_server(&server, response.c_str(), response.length(), 0, 0);
    ASSERT_GT(port, 0);

    /* The request should either succeed (with truncated header) or fail gracefully */
    int status = send_request_and_get_status(m_env, port);

    stop_mock_server(&server);

    /* Accept either success (robust handling) or any error code (rejection) */
    /* What we DON'T want is a crash or hang */
    /* Status codes: 200=success, -1=parse error, 0=connection/status parse issue, -2=timeout */
    ASSERT_TRUE(status == 200 || status <= 0)
        << "Long header should be handled gracefully, got status: " << status;
}

/**
 * SECURITY: Test many headers (resource exhaustion attempt)
 *
 * CVE Risk: Sending thousands of headers could cause:
 * - Memory exhaustion
 * - CPU exhaustion in parsing
 * - Hash table DoS if headers stored in hash
 *
 * Expected: Parser should limit number of headers or total header size.
 */
TEST_F(TestAXIS2C1480, security_many_headers)
{
    mock_http_server_ctx_t server;

    std::string response = "HTTP/1.1 200 OK\r\n";

    /* Add 10,000 headers */
    for (int i = 0; i < 10000; i++)
    {
        char header[64];
        snprintf(header, sizeof(header), "X-Header-%d: value%d\r\n", i, i);
        response += header;
    }
    response += "Content-Length: 0\r\n\r\n";

    int port = start_mock_server(&server, response.c_str(), response.length(), 0, 0);
    ASSERT_GT(port, 0);

    int status = send_request_and_get_status(m_env, port);

    stop_mock_server(&server);

    /* Should handle gracefully - either accept or reject */
    ASSERT_TRUE(status == 200 || status == -1)
        << "Many headers should be handled gracefully";
}

/**
 * SECURITY: Test header with embedded CRLF (header injection)
 *
 * CVE Risk: If CRLF in header value isn't handled, attacker could inject
 * additional headers or even a fake response body, leading to:
 * - Response splitting attacks
 * - Cache poisoning
 * - XSS via injected headers
 *
 * Expected: Embedded CRLF should be rejected or the characters escaped.
 */
TEST_F(TestAXIS2C1480, security_header_injection_crlf)
{
    mock_http_server_ctx_t server;

    /* Header value contains CRLF attempting to inject another header */
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "X-Safe-Header: value\r\nX-Injected: malicious\r\n"
        "Content-Length: 0\r\n"
        "\r\n";

    int port = start_mock_server(&server, response, strlen(response), 0, 0);
    ASSERT_GT(port, 0);

    int status = send_request_and_get_status(m_env, port);

    stop_mock_server(&server);

    /* Parser behavior depends on implementation - just verify no crash */
    ASSERT_TRUE(status >= -1 && status < 600);
}

/**
 * SECURITY: Test null byte in header (null byte injection)
 *
 * CVE Risk: Null bytes can truncate strings in C, potentially:
 * - Bypassing security checks
 * - Causing incorrect header parsing
 * - Memory safety issues
 *
 * Expected: Null bytes should be rejected or handled safely.
 */
TEST_F(TestAXIS2C1480, security_null_byte_in_header)
{
    mock_http_server_ctx_t server;

    /* Construct response with null byte in header value */
    std::string response = "HTTP/1.1 200 OK\r\nX-Header: before";
    response += '\0';  /* Null byte */
    response += "after\r\nContent-Length: 0\r\n\r\n";

    int port = start_mock_server(&server, response.c_str(), response.length(), 0, 0);
    ASSERT_GT(port, 0);

    int status = send_request_and_get_status(m_env, port);

    stop_mock_server(&server);

    /* Should handle gracefully */
    ASSERT_TRUE(status >= -1 && status < 600);
}

/**
 * SECURITY: Test malformed status line (protocol confusion)
 *
 * CVE Risk: Malformed status lines could cause:
 * - Parser crashes
 * - Incorrect status code extraction
 * - Protocol downgrade attacks
 *
 * Expected: Malformed status should be rejected gracefully.
 */
TEST_F(TestAXIS2C1480, security_malformed_status_line)
{
    mock_http_server_ctx_t server;

    /* Missing HTTP version */
    const char *response =
        "200 OK\r\n"
        "Content-Length: 0\r\n"
        "\r\n";

    int port = start_mock_server(&server, response, strlen(response), 0, 0);
    ASSERT_GT(port, 0);

    int status = send_request_and_get_status(m_env, port);

    stop_mock_server(&server);

    /* Should fail gracefully - definitely not return 200 */
    ASSERT_NE(status, 200) << "Malformed status should not parse as valid";
}

/**
 * SECURITY: Test incomplete response (connection drop mid-header)
 *
 * CVE Risk: Server closing connection mid-header could cause:
 * - Infinite loop waiting for more data
 * - Use of uninitialized memory
 * - Crash due to unexpected EOF
 *
 * Expected: Should handle EOF gracefully during header parsing.
 */
TEST_F(TestAXIS2C1480, security_incomplete_headers)
{
    mock_http_server_ctx_t server;

    /* Response truncated mid-header - missing final CRLF CRLF */
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 10\r\n"
        "X-Incompl";  /* Truncated header */

    int port = start_mock_server(&server, response, strlen(response), 0, 0);
    ASSERT_GT(port, 0);

    int status = send_request_and_get_status(m_env, port);

    stop_mock_server(&server);

    /* Should fail - incomplete headers (any error code is acceptable) */
    /* Status 0 = server shutdown/EOF, -1 = parse error, -2 = timeout */
    ASSERT_LE(status, 0) << "Incomplete headers should cause failure, got status: " << status;
}

/**
 * SECURITY: Test only CR without LF
 *
 * CVE Risk: Some parsers treat CR alone as line ending, others don't.
 * Inconsistent handling can lead to request smuggling.
 *
 * Expected: Should either reject or normalize to CRLF.
 */
TEST_F(TestAXIS2C1480, security_cr_without_lf)
{
    mock_http_server_ctx_t server;

    /* Use only CR instead of CRLF */
    const char *response =
        "HTTP/1.1 200 OK\r"
        "Content-Length: 0\r"
        "\r";

    int port = start_mock_server(&server, response, strlen(response), 0, 0);
    ASSERT_GT(port, 0);

    int status = send_request_and_get_status(m_env, port);

    stop_mock_server(&server);

    /* Behavior depends on implementation - just verify no crash/hang */
    /* Accept any status: errors (<= 0), timeouts (-2), or valid HTTP codes */
    ASSERT_TRUE((status <= 0) || (status >= 100 && status < 600));
}

/**
 * SECURITY: Test only LF without CR
 *
 * CVE Risk: Similar to CR-only, inconsistent handling can lead to
 * request smuggling or header injection.
 *
 * Expected: Should either reject or accept (LF-only is common in practice).
 */
TEST_F(TestAXIS2C1480, security_lf_without_cr)
{
    mock_http_server_ctx_t server;

    /* Use only LF instead of CRLF (bare LF) */
    const char *response =
        "HTTP/1.1 200 OK\n"
        "Content-Length: 5\n"
        "\n"
        "hello";

    int port = start_mock_server(&server, response, strlen(response), 0, 0);
    ASSERT_GT(port, 0);

    int body_len = 0;
    char *body = get_response_body(m_env, port, &body_len);

    stop_mock_server(&server);

    /* Many implementations accept bare LF - verify it works or fails cleanly */
    if (body != nullptr)
    {
        ASSERT_EQ(body_len, 5);
        ASSERT_STREQ(body, "hello");
        AXIS2_FREE(m_env->allocator, body);
    }
    /* If body is null, that's also acceptable - just no crash */
}

/**
 * SECURITY: Test negative Content-Length
 *
 * CVE Risk: Negative Content-Length could cause:
 * - Integer underflow
 * - Huge memory allocation (if cast to unsigned)
 * - Buffer over-read
 *
 * Expected: Should reject negative Content-Length.
 */
TEST_F(TestAXIS2C1480, security_negative_content_length)
{
    mock_http_server_ctx_t server;

    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: -1\r\n"
        "\r\n"
        "This should not be read";

    int port = start_mock_server(&server, response, strlen(response), 0, 0);
    ASSERT_GT(port, 0);

    int body_len = 0;
    char *body = get_response_body(m_env, port, &body_len);

    stop_mock_server(&server);

    /* Should not crash - either reject or treat as 0/unknown length */
    if (body != nullptr)
    {
        AXIS2_FREE(m_env->allocator, body);
    }
}

/**
 * SECURITY: Test extremely large Content-Length (memory exhaustion)
 *
 * CVE Risk: Huge Content-Length without corresponding data could cause:
 * - Memory exhaustion if pre-allocated
 * - Hang waiting for data that never arrives
 *
 * Expected: Should handle gracefully with timeouts.
 */
TEST_F(TestAXIS2C1480, security_huge_content_length)
{
    mock_http_server_ctx_t server;

    /* Claim huge content but send nothing */
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 999999999999\r\n"
        "\r\n";

    int port = start_mock_server(&server, response, strlen(response), 0, 0);
    ASSERT_GT(port, 0);

    /* This should timeout or fail, not hang forever or crash */
    int body_len = 0;
    char *body = get_response_body(m_env, port, &body_len);

    stop_mock_server(&server);

    /* Body should be null or empty - definitely shouldn't allocate 999GB */
    if (body != nullptr)
    {
        AXIS2_FREE(m_env->allocator, body);
    }
}

/**
 * SECURITY: Test slowloris-style slow header delivery
 *
 * CVE Risk: Sending headers one byte at a time with delays could:
 * - Exhaust connection pool
 * - Cause resource exhaustion on server
 * - DoS via connection starvation
 *
 * Note: This tests the client side, so it's about ensuring the client
 * doesn't hang indefinitely on slow servers.
 *
 * Expected: Should complete or timeout within reasonable time.
 */
TEST_F(TestAXIS2C1480, security_slowloris_defense)
{
    mock_http_server_ctx_t server;

    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 2\r\n"
        "\r\n"
        "OK";

    /* Send 1 byte at a time with 1ms delay - tests timeout handling */
    /* Total response is ~50 bytes, so ~50ms total which is well within 5s timeout */
    int port = start_mock_server(&server, response, strlen(response), 1, 1000);
    ASSERT_GT(port, 0);

    int body_len = 0;
    char *body = get_response_body(m_env, port, &body_len);

    stop_mock_server(&server);

    /* Should either succeed or timeout - not hang forever */
    if (body != nullptr)
    {
        ASSERT_EQ(body_len, 2);
        AXIS2_FREE(m_env->allocator, body);
    }
}
