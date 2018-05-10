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

#include <axutil_uri.h>
#include "../util/create_env.h"

class TestURI: public ::testing::Test
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

/** @brief test uri
 *  * create URI and get the values of it's components
 *   */
TEST_F(TestURI, test_uri)
{
    axis2_char_t * uri_str = "http://user:pass@example.com:80/foo?bar#item5";
    axis2_char_t * host = "home.netscape.com:443";
    axis2_char_t * uri_str_base = "http://user:pass@example.com:80/foo?bar";
    axis2_char_t * scheme_str = "http";
    axutil_uri_t * base = NULL;
    axutil_uri_t * hostinfo = NULL;
    axutil_uri_t * uri = NULL;
    axutil_uri_t * clone = NULL;
    axutil_uri_t * rel = NULL;
    axis2_port_t scheme_port;
    axis2_port_t port;
    axis2_char_t * str;

    hostinfo = axutil_uri_parse_hostinfo(m_env,host);
    ASSERT_NE(hostinfo, nullptr);

    scheme_port = axutil_uri_port_of_scheme(scheme_str);
    ASSERT_NE(scheme_port, 0);

    uri = axutil_uri_parse_string(m_env,uri_str);
    ASSERT_NE(uri, nullptr);
    str = axutil_uri_get_protocol(uri,m_env);
    ASSERT_STREQ(str, "http");
    port = axutil_uri_get_port(uri,m_env);
    ASSERT_EQ(port, 80);
    str = axutil_uri_get_path(uri,m_env);
    ASSERT_STREQ(str, "/foo");
    str = axutil_uri_get_host(uri,m_env);
    ASSERT_STREQ(str, "example.com");

    base = axutil_uri_parse_string(m_env,uri_str_base);
    ASSERT_NE(base, nullptr);
    if (base)
    {
        str = axutil_uri_to_string(base,m_env,0);
        ASSERT_STREQ(str, "http://user:XXXXXXXX@example.com/foo?bar");
        AXIS2_FREE(m_allocator, str);
    }

    clone = axutil_uri_clone(uri,m_env);
    ASSERT_NE(clone, nullptr);
    if (clone)
    {
        str = axutil_uri_to_string(clone,m_env,0);
        ASSERT_STREQ(str, "http://user:XXXXXXXX@example.com/foo?bar#item5");
        axutil_uri_free(clone,m_env);
        AXIS2_FREE(m_allocator, str);
    }

    rel = axutil_uri_resolve_relative(m_env,base,uri);
    ASSERT_NE(rel, nullptr);
    if (rel)
    {
        str = axutil_uri_to_string(rel,m_env,0);
        ASSERT_STREQ(str, "http://user:XXXXXXXX@example.com/foo?bar#item5");
        AXIS2_FREE(m_allocator, str);
    }

    axutil_uri_free(uri,m_env);
    axutil_uri_free(base, m_env);
    axutil_uri_free(hostinfo, m_env);
}
