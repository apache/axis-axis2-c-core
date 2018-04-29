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

#include <axutil_url.h>
#include "../util/create_env.h"

class TestURL: public ::testing::Test
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



/** @brief test url 
 * create URL and get the values of it's components  
 */
TEST_F(TestURL, test_url)
{
    axutil_url_t * url;
    axis2_char_t *url_str = "https://issues.apache.org/jira/secure/ManageAttachments.jspa?id=12386356";
    axis2_char_t *set_server = "www.yahoo.com";
    axis2_char_t *set_protocol = "file";
    axis2_char_t *set_path = "/bar/";
    axis2_port_t set_port = 80;
    axis2_char_t *get_protocol;
    axis2_char_t *get_server;
    axis2_port_t get_port;
    axis2_char_t *get_path;
    axis2_status_t status;

    url = axutil_url_parse_string(m_env,url_str);
    ASSERT_NE(url, nullptr);

    status = axutil_url_set_protocol(url,m_env,set_protocol);   
    ASSERT_EQ(status, AXIS2_SUCCESS);
    status = axutil_url_set_server(url,m_env,set_server);    
    ASSERT_EQ(status, AXIS2_SUCCESS);
    status = axutil_url_set_port(url,m_env,set_port);    
    ASSERT_EQ(status, AXIS2_SUCCESS);
	status = axutil_url_set_path(url,m_env,set_path);   
    ASSERT_EQ(status, AXIS2_SUCCESS);

    get_protocol = axutil_url_get_protocol(url,m_env);
    ASSERT_STREQ(get_protocol, set_protocol);

    get_server = axutil_url_get_server(url,m_env);
    ASSERT_STREQ(get_server, "www.yahoo.com:80");

    get_port = axutil_url_get_port(url,m_env);
    ASSERT_EQ(get_port,set_port);

    get_path = axutil_url_get_path(url,m_env);
	ASSERT_STREQ(get_path, set_path);

    axutil_url_free(url,m_env);
}

