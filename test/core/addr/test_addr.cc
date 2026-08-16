
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

#include <axis2_msg_info_headers.h>
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axis2_endpoint_ref.h>
#include <axis2_core_utils.h>
#include <axis2_conf.h>
#include <axis2_conf_ctx.h>
#include <axutil_param.h>
#include <axis2_addr.h>
#include <stdio.h>


class TestAddr: public ::testing::Test
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

TEST_F(TestAddr, test_msg_info_headers)
{

    axis2_endpoint_ref_t *to = NULL;
    axis2_endpoint_ref_t *from = NULL;
    axis2_endpoint_ref_t *reply_to = NULL;
    axis2_endpoint_ref_t *fault_to = NULL;
    axis2_endpoint_ref_t *axis2_endpoint_ref = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_msg_info_headers_t *axis2_msg_info_headers = NULL;
    axis2_char_t *action = "test action";
    const axis2_char_t *get_action = NULL;
    to = axis2_endpoint_ref_create(m_env, "to");
    from = axis2_endpoint_ref_create(m_env, "from");
    reply_to = axis2_endpoint_ref_create(m_env, "reply to");
    fault_to = axis2_endpoint_ref_create(m_env, "fault to");

    axis2_msg_info_headers = axis2_msg_info_headers_create(m_env, NULL, action);
    ASSERT_NE(axis2_msg_info_headers, nullptr);
    status = axis2_msg_info_headers_set_to(axis2_msg_info_headers, m_env, to);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    axis2_endpoint_ref =
        axis2_msg_info_headers_get_to(axis2_msg_info_headers, m_env);
    ASSERT_NE(axis2_endpoint_ref, nullptr);

    status = axis2_msg_info_headers_set_from(axis2_msg_info_headers, m_env, from);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    axis2_endpoint_ref =
        axis2_msg_info_headers_get_from(axis2_msg_info_headers, m_env);
    ASSERT_NE(axis2_endpoint_ref, nullptr);

    axis2_endpoint_ref =
        axis2_msg_info_headers_get_reply_to(axis2_msg_info_headers, m_env);
    ASSERT_EQ(axis2_endpoint_ref, nullptr);

    status =
        axis2_msg_info_headers_set_reply_to(axis2_msg_info_headers, m_env, reply_to);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    axis2_endpoint_ref =
        axis2_msg_info_headers_get_reply_to(axis2_msg_info_headers, m_env);
    ASSERT_NE(axis2_endpoint_ref, nullptr);
    status =
        axis2_msg_info_headers_set_fault_to(axis2_msg_info_headers, m_env, fault_to);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    axis2_endpoint_ref =
        axis2_msg_info_headers_get_fault_to(axis2_msg_info_headers, m_env);
    ASSERT_NE(axis2_endpoint_ref, nullptr);

    get_action = axis2_msg_info_headers_get_action(axis2_msg_info_headers, m_env);
    ASSERT_STREQ(get_action, action);

    status =
        axis2_msg_info_headers_set_action(axis2_msg_info_headers, m_env, action);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    axis2_msg_info_headers_free(axis2_msg_info_headers, m_env);
}


/* ------------------------------------------------------------------------
 * Response endpoint policy
 *
 * A non-anonymous wsa:ReplyTo/FaultTo names the destination of a
 * server-initiated send, so it is declined unless a deployment opts in.
 * These cases pin the decisions that policy makes; a NULL conf_ctx exercises
 * the shipped defaults, which is what an unconfigured deployment gets.
 * ---------------------------------------------------------------------- */

TEST_F(TestAddr, test_response_endpoint_anonymous_always_allowed)
{
    /* Anonymous and none mean "reply on the inbound connection" and drive no
     * outbound send, so refusing them would break ordinary in-out messaging. */
    ASSERT_TRUE(axis2_core_utils_is_response_endpoint_allowed(
        m_env, NULL, AXIS2_WSA_ANONYMOUS_URL));
    ASSERT_TRUE(axis2_core_utils_is_response_endpoint_allowed(
        m_env, NULL, AXIS2_WSA_ANONYMOUS_URL_SUBMISSION));
    ASSERT_TRUE(axis2_core_utils_is_response_endpoint_allowed(
        m_env, NULL, AXIS2_WSA_NONE_URL));
    ASSERT_TRUE(axis2_core_utils_is_response_endpoint_allowed(m_env, NULL, NULL));
    ASSERT_TRUE(axis2_core_utils_is_response_endpoint_allowed(m_env, NULL, ""));
}

TEST_F(TestAddr, test_response_endpoint_declined_by_default)
{
    /* Nothing non-anonymous is honoured until allowNonAnonymousResponseEndpoints
     * is turned on, whatever the address happens to be. */
    ASSERT_FALSE(axis2_core_utils_is_response_endpoint_allowed(
        m_env, NULL, "https://198.51.100.7/callback"));
    ASSERT_FALSE(axis2_core_utils_is_response_endpoint_allowed(
        m_env, NULL, "http://169.254.169.254/latest/meta-data/"));
    ASSERT_FALSE(axis2_core_utils_is_response_endpoint_allowed(
        m_env, NULL, "file:///etc/passwd"));
}

/* ----------------------------------------------------------------------
 * IPv6 response endpoints.
 *
 * The address checks were written against dotted quads. An IPv6 literal is
 * bracketed and full of colons, so the host scan stopped at the first colon
 * and classified a host of "[" -- every restricted v6 address was allowed
 * through. These fix the classification at the point it is decided; the tests
 * in test_http_transport.cc cover the call site that consults it.
 *
 * The gate has to be opened first: with the defaults nothing non-anonymous is
 * allowed, so every case below would pass for the wrong reason.
 * ---------------------------------------------------------------------- */

class TestResponseEndpointIPv6 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_allocator = axutil_allocator_init(NULL);
        m_error = axutil_error_create(m_allocator);
        m_log = axutil_log_create(m_allocator, NULL, NULL);
        m_env = axutil_env_create_with_error_log(m_allocator, m_error, m_log);
        m_conf = axis2_conf_create(m_env);
        m_conf_ctx = axis2_conf_ctx_create(m_env, m_conf);
        set_param(AXIS2_ALLOW_NON_ANONYMOUS_RESPONSE_ENDPOINTS, "true");
    }

    void TearDown() override
    {
        if (m_conf_ctx) axis2_conf_ctx_free(m_conf_ctx, m_env);
        axutil_env_free(m_env);
    }

    void set_param(const axis2_char_t *name, const axis2_char_t *value)
    {
        /* Asymmetric on purpose: axutil_param_create strdups the name but
         * shallow-copies the value, and axutil_param_free releases both. So
         * the name must NOT be copied here (that copy would leak) while the
         * value must be. */
        axis2_conf_add_param(m_conf, m_env, axutil_param_create(m_env,
            (axis2_char_t *)name, axutil_strdup(m_env, value)));
    }

    axis2_bool_t allowed(const axis2_char_t *address)
    {
        return axis2_core_utils_is_response_endpoint_allowed(m_env, m_conf_ctx, address);
    }

    axutil_allocator_t *m_allocator = NULL;
    axutil_error_t *m_error = NULL;
    axutil_log_t *m_log = NULL;
    axutil_env_t *m_env = NULL;
    axis2_conf_t *m_conf = NULL;
    axis2_conf_ctx_t *m_conf_ctx = NULL;
};

TEST_F(TestResponseEndpointIPv6, control_gate_is_actually_open)
{
    /* Without this the rest prove nothing: every ASSERT_FALSE below would hold
     * simply because the feature is off. */
    ASSERT_TRUE(allowed("https://198.51.100.7/callback"));
    ASSERT_TRUE(allowed("https://[2001:db8::1]/callback"));
}

TEST_F(TestResponseEndpointIPv6, refuses_link_local_whatever_the_configuration)
{
    ASSERT_FALSE(allowed("https://[fe80::1]/callback"));
    ASSERT_FALSE(allowed("https://[fe80::1]:8080/callback"));
    ASSERT_FALSE(allowed("https://[febf::dead:beef]/callback"));
}

TEST_F(TestResponseEndpointIPv6, refuses_unspecified_and_multicast)
{
    ASSERT_FALSE(allowed("https://[::]/callback"));
    ASSERT_FALSE(allowed("https://[ff02::1]/callback"));
    ASSERT_FALSE(allowed("https://[ff00::]/callback"));
}

TEST_F(TestResponseEndpointIPv6, refuses_v4_mapped_metadata_address)
{
    /* The headline bypass: ::ffff:169.254.169.254 reaches the same instance
     * metadata service the dotted quad does. */
    ASSERT_FALSE(allowed("https://[::ffff:169.254.169.254]/latest/meta-data/"));
    ASSERT_FALSE(allowed("https://[::169.254.169.254]/latest/meta-data/"));
    ASSERT_FALSE(allowed("https://[::ffff:0.0.0.0]/callback"));
}

TEST_F(TestResponseEndpointIPv6, spelling_does_not_change_the_verdict)
{
    /* The same address written four ways. A textual prefix check would pass
     * some of these; only parsing rejects all four. */
    ASSERT_FALSE(allowed("https://[fe80::1]/callback"));
    ASSERT_FALSE(allowed("https://[FE80::1]/callback"));
    ASSERT_FALSE(allowed("https://[fe80:0:0:0:0:0:0:1]/callback"));
    ASSERT_FALSE(allowed("https://[fe80:0000:0000:0000:0000:0000:0000:0001]/callback"));
}

TEST_F(TestResponseEndpointIPv6, loopback_and_unique_local_follow_the_private_switch)
{
    /* Off by default, because a callback inside the same network is how most
     * decoupled deployments are wired. */
    ASSERT_TRUE(allowed("https://[::1]/callback"));
    ASSERT_TRUE(allowed("https://[fd00::1]/callback"));

    set_param(AXIS2_BLOCK_PRIVATE_NETWORK_RESPONSE_ENDPOINTS, "true");
    ASSERT_FALSE(allowed("https://[::1]/callback"));
    ASSERT_FALSE(allowed("https://[fd00::1]/callback"));
    ASSERT_FALSE(allowed("https://[fc00::1]/callback"));
    ASSERT_FALSE(allowed("https://[::ffff:127.0.0.1]/callback"));
}

TEST_F(TestResponseEndpointIPv6, refuses_malformed_bracketed_hosts)
{
    /* Nothing legitimate produces these, so they must not fall through to the
     * name path unclassified. */
    ASSERT_FALSE(allowed("https://[notanaddress]/callback"));
    ASSERT_FALSE(allowed("https://[fe80::1/callback"));
    ASSERT_FALSE(allowed("https://[fe80::1::2]/callback"));
    ASSERT_FALSE(allowed("https://[12345::1]/callback"));
    ASSERT_FALSE(allowed("https://[fe80::1%eth0]/callback"));
}

TEST_F(TestResponseEndpointIPv6, embedded_quad_must_be_the_whole_host)
{
    /* sscanf would stop at the fourth number and drop the rest, classifying
     * this as 127.0.0.1 while the text says something else entirely. Inside
     * brackets there is no name to fall back on, so it is refused. */
    ASSERT_FALSE(allowed("https://[::ffff:127.0.0.1.evil.com]/callback"));
    ASSERT_FALSE(allowed("https://[::ffff:1.2.3.4.5]/callback"));
    ASSERT_FALSE(allowed("https://[::ffff:1.2.3]/callback"));
    ASSERT_FALSE(allowed("https://[::ffff:1.2.3.999]/callback"));
    ASSERT_FALSE(allowed("https://[::ffff:1.2.3.4junk]/callback"));

    /* Leading zeros are ambiguous -- ten here, eight to an octal reader -- so
     * they are refused rather than guessed at. */
    ASSERT_FALSE(allowed("https://[::ffff:010.0.0.1]/callback"));

    /* The control: strictness must not have broken well-formed mapped
     * addresses in either direction. */
    ASSERT_TRUE(allowed("https://[::ffff:198.51.100.7]/callback"));
    ASSERT_FALSE(allowed("https://[::ffff:169.254.169.254]/callback"));
}

TEST_F(TestResponseEndpointIPv6, ipv4_classification_still_holds)
{
    ASSERT_FALSE(allowed("https://169.254.169.254/latest/meta-data/"));
    ASSERT_FALSE(allowed("https://0.0.0.0/callback"));
    ASSERT_FALSE(allowed("https://239.255.255.250/callback"));
    ASSERT_TRUE(allowed("https://198.51.100.7/callback"));
}

/* ----------------------------------------------------------------------
 * Non-canonical IPv4 literal forms.
 *
 * The classifier used to recognise an address only through
 * sscanf("%u.%u.%u.%u"), i.e. canonical dotted decimal. The socket, though, is
 * opened with inet_addr() (util/src/network_handler.c), which also honours
 * octal, hex, abbreviated and bare-integer notations. Anything the classifier
 * did not recognise as a quad fell through as "nothing to classify" -- allowed
 * -- while inet_addr() still dialled the restricted address. Each address below
 * is a second spelling of a target the canonical tests already refuse; the fix
 * classifies with inet_addr() so the two can no longer disagree.
 *
 * inet_addr()'s octet mapping for these is verified: 0251.0376.0251.0376,
 * 2852039166 and 0xa9fea9fe all resolve to 169.254.169.254, and 0177.0.0.1,
 * 0x7f.0.0.1, 127.1 and 2130706433 all resolve to 127.0.0.1.
 * ---------------------------------------------------------------------- */

TEST_F(TestResponseEndpointIPv6, refuses_metadata_in_alternate_literal_forms)
{
    /* 169.254.169.254 is refused unconditionally, so the private-network switch
     * stays off here -- these must fail on the metadata rule alone. */
    ASSERT_FALSE(allowed("http://0251.0376.0251.0376/latest/meta-data/")); /* octal */
    ASSERT_FALSE(allowed("http://2852039166/latest/meta-data/"));          /* decimal int */
    ASSERT_FALSE(allowed("http://0xa9fea9fe/latest/meta-data/"));          /* hex int */
}

TEST_F(TestResponseEndpointIPv6, refuses_loopback_in_alternate_literal_forms)
{
    /* Loopback follows the private-network switch, so open it first; without it
     * these would pass for the wrong reason. */
    set_param(AXIS2_BLOCK_PRIVATE_NETWORK_RESPONSE_ENDPOINTS, "true");
    ASSERT_FALSE(allowed("http://0177.0.0.1/callback")); /* octal first octet */
    ASSERT_FALSE(allowed("http://0x7f.0.0.1/callback")); /* hex first octet */
    ASSERT_FALSE(allowed("http://127.1/callback"));      /* abbreviated */
    ASSERT_FALSE(allowed("http://2130706433/callback")); /* decimal int */
}

TEST_F(TestResponseEndpointIPv6, alternate_forms_do_not_over_block)
{
    /* The fix must reject the bypasses without turning every unusual host into a
     * refusal. A public dotted quad and a name both remain allowed: the name is
     * not a literal, so it is left for the network layer to police. */
    ASSERT_TRUE(allowed("https://198.51.100.7/callback"));
    ASSERT_TRUE(allowed("https://service.example.com/callback"));
}
