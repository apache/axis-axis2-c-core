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
 * @file test_axis2c_1566.cc
 * @brief Unit test for AXIS2C-1566: Cookie persistence when Set-Cookie absent
 *
 * This test verifies that HTTP cookies are NOT deleted when the server
 * response does not include a Set-Cookie header. Per RFC 6265, cookies
 * should persist until they expire or the server explicitly deletes them.
 *
 * Prior to the fix, the client would delete all cookies if the server
 * response didn't include a Set-Cookie header, breaking session management.
 */

#include <gtest/gtest.h>

extern "C" {
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_property.h>
#include <axutil_hash.h>
#include <axis2_msg_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_conf.h>
#include <axis2_endpoint_ref.h>
#include <axis2_http_sender.h>
#include <axis2_http_simple_response.h>
#include <axis2_http_transport.h>
#include <axis2_http_transport_utils.h>
#include <axis2_http_header.h>
}

#define TEST_ENDPOINT "http://example.com/service"
#define TEST_COOKIE "SESSIONID=abc123; Path=/"

class CookiePersistenceTest : public ::testing::Test {
protected:
    axutil_env_t *env;
    axutil_allocator_t *allocator;
    axutil_error_t *error;
    axutil_log_t *log;
    axis2_conf_ctx_t *conf_ctx;
    axis2_conf_t *conf;

    void SetUp() override {
        allocator = axutil_allocator_init(NULL);
        ASSERT_NE(allocator, nullptr) << "Failed to create allocator";

        error = axutil_error_create(allocator);
        log = axutil_log_create(allocator, NULL, NULL);
        env = axutil_env_create_with_error_log(allocator, error, log);
        ASSERT_NE(env, nullptr) << "Failed to create environment";

        conf = axis2_conf_create(env);
        ASSERT_NE(conf, nullptr) << "Failed to create configuration";

        conf_ctx = axis2_conf_ctx_create(env, conf);
        ASSERT_NE(conf_ctx, nullptr) << "Failed to create configuration context";
    }

    void TearDown() override {
        if (conf_ctx) {
            axis2_conf_ctx_free(conf_ctx, env);
        }
        if (env) {
            axutil_env_free(env);
        }
    }
};

/**
 * Test that cookies persist when server response has no Set-Cookie header.
 *
 * This is the main test for AXIS2C-1566. The bug was that the HTTP client
 * would delete stored cookies if the server response didn't include a
 * Set-Cookie header, which is incorrect per RFC 6265.
 */
TEST_F(CookiePersistenceTest, CookiePersistsWithoutSetCookieHeader) {
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    axis2_http_sender_t *sender = NULL;
    axis2_http_simple_response_t *response = NULL;
    axis2_char_t *cookie_before = NULL;
    axis2_char_t *cookie_after = NULL;

    /* Create message context */
    msg_ctx = axis2_msg_ctx_create(env, conf_ctx, NULL, NULL);
    ASSERT_NE(msg_ctx, nullptr) << "Failed to create message context";

    /* Set endpoint reference - needed for cookie storage keying */
    endpoint_ref = axis2_endpoint_ref_create(env, TEST_ENDPOINT);
    ASSERT_NE(endpoint_ref, nullptr) << "Failed to create endpoint reference";
    axis2_msg_ctx_set_to(msg_ctx, env, endpoint_ref);

    /* Store a cookie for this endpoint */
    axis2_http_transport_utils_store_cookie(env, msg_ctx, (axis2_char_t*)TEST_COOKIE);

    /* Verify cookie was stored */
    cookie_before = axis2_http_transport_utils_read_from_cookie_store(env, msg_ctx);
    ASSERT_NE(cookie_before, nullptr) << "Cookie was not stored initially";
    EXPECT_STREQ(cookie_before, TEST_COOKIE) << "Stored cookie doesn't match";

    /* Create HTTP response WITHOUT Set-Cookie header */
    response = axis2_http_simple_response_create_default(env);
    ASSERT_NE(response, nullptr) << "Failed to create HTTP response";

    /* Set a basic response status (200 OK) */
    axis2_http_simple_response_set_status_line(response, env,
        (axis2_char_t*)"HTTP/1.1", 200, (axis2_char_t*)"OK");

    /* Add some headers but NOT Set-Cookie */
    axis2_http_simple_response_set_header(response, env,
        axis2_http_header_create(env, (axis2_char_t*)"Content-Type",
                                (axis2_char_t*)"text/xml"));
    axis2_http_simple_response_set_header(response, env,
        axis2_http_header_create(env, (axis2_char_t*)"Content-Length",
                                (axis2_char_t*)"100"));

    /* Create HTTP sender */
    sender = axis2_http_sender_create(env);
    ASSERT_NE(sender, nullptr) << "Failed to create HTTP sender";

    /* Process the response headers - this is where the bug manifested */
    axis2_http_sender_get_header_info(sender, env, msg_ctx, response);

    /* Read cookie after processing - should still be there! */
    cookie_after = axis2_http_transport_utils_read_from_cookie_store(env, msg_ctx);

    /* THE CRITICAL ASSERTION: Cookie must persist */
    ASSERT_NE(cookie_after, nullptr)
        << "AXIS2C-1566: Cookie was incorrectly deleted when Set-Cookie header was absent. "
        << "Per RFC 6265, cookies should persist until they expire or are explicitly deleted.";

    EXPECT_STREQ(cookie_after, TEST_COOKIE)
        << "Cookie value changed unexpectedly";

    /* Cleanup */
    if (sender) axis2_http_sender_free(sender, env);
    if (response) axis2_http_simple_response_free(response, env);
    if (msg_ctx) axis2_msg_ctx_free(msg_ctx, env);
}

/**
 * Test that new cookies from Set-Cookie header are still properly stored.
 *
 * This verifies we didn't break the normal cookie setting behavior while
 * fixing the deletion bug.
 */
TEST_F(CookiePersistenceTest, NewCookieIsStoredFromSetCookieHeader) {
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    axis2_http_sender_t *sender = NULL;
    axis2_http_simple_response_t *response = NULL;
    axis2_char_t *cookie_after = NULL;
    const char *NEW_COOKIE = "NEWSESSION=xyz789; Path=/";

    /* Create message context */
    msg_ctx = axis2_msg_ctx_create(env, conf_ctx, NULL, NULL);
    ASSERT_NE(msg_ctx, nullptr);

    /* Set endpoint reference */
    endpoint_ref = axis2_endpoint_ref_create(env, TEST_ENDPOINT);
    ASSERT_NE(endpoint_ref, nullptr);
    axis2_msg_ctx_set_to(msg_ctx, env, endpoint_ref);

    /* Create HTTP response WITH Set-Cookie header */
    response = axis2_http_simple_response_create_default(env);
    ASSERT_NE(response, nullptr);

    axis2_http_simple_response_set_status_line(response, env,
        (axis2_char_t*)"HTTP/1.1", 200, (axis2_char_t*)"OK");

    /* Add Set-Cookie header */
    axis2_http_simple_response_set_header(response, env,
        axis2_http_header_create(env, (axis2_char_t*)"Set-Cookie",
                                (axis2_char_t*)NEW_COOKIE));

    /* Create HTTP sender and process headers */
    sender = axis2_http_sender_create(env);
    ASSERT_NE(sender, nullptr);

    axis2_http_sender_get_header_info(sender, env, msg_ctx, response);

    /* Verify new cookie was stored */
    cookie_after = axis2_http_transport_utils_read_from_cookie_store(env, msg_ctx);
    ASSERT_NE(cookie_after, nullptr)
        << "New cookie from Set-Cookie header was not stored";

    EXPECT_STREQ(cookie_after, NEW_COOKIE)
        << "Stored cookie doesn't match Set-Cookie header value";

    /* Cleanup */
    if (sender) axis2_http_sender_free(sender, env);
    if (response) axis2_http_simple_response_free(response, env);
    if (msg_ctx) axis2_msg_ctx_free(msg_ctx, env);
}
