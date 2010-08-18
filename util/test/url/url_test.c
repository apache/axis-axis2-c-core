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

#include <axutil_url.h>
#include <cut_defs.h>
#include "../util/create_env.h"

/** @brief test url 
 * create URL and get the values of it's components  
 */

void test_url(axutil_env_t *env)
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

    url = axutil_url_parse_string(env,url_str);
    CUT_ASSERT_PTR_NOT_EQUAL(url, NULL, 1);

    status = axutil_url_set_protocol(url,env,set_protocol);   
    CUT_ASSERT_INT_EQUAL(status, AXIS2_SUCCESS, 0);
    status = axutil_url_set_server(url,env,set_server);    
    CUT_ASSERT_INT_EQUAL(status, AXIS2_SUCCESS, 0);
    status = axutil_url_set_port(url,env,set_port);    
    CUT_ASSERT_INT_EQUAL(status, AXIS2_SUCCESS, 0);
	status = axutil_url_set_path(url,env,set_path);   
    CUT_ASSERT_INT_EQUAL(status, AXIS2_SUCCESS, 0);
  
    get_protocol = axutil_url_get_protocol(url,env);
 	CUT_ASSERT_STR_EQUAL(get_protocol, set_protocol, 0);

    get_server = axutil_url_get_server(url,env);
	CUT_ASSERT_STR_EQUAL(get_server, "www.yahoo.com:80", 0);
    
    get_port = axutil_url_get_port(url,env);
    CUT_ASSERT_INT_EQUAL(get_port,set_port,0);
 
    get_path = axutil_url_get_path(url,env);
	CUT_ASSERT_STR_EQUAL(get_path, set_path, 0);

    axutil_url_free(url,env);
}

int main()
{
    axutil_env_t *env = cut_setup_env("Url");
	CUT_ASSERT(env != NULL);
	if (env) {
        test_url(env);    
        axutil_env_free(env);
     }
    CUT_RETURN_ON_FAILURE(-1);  
    return 0;
}



