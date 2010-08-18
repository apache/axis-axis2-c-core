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

#include <axutil_uri.h>
#include <cut_defs.h>
#include "../util/create_env.h"
/** @brief test uri 
 *  * create URI and get the values of it's components  
 *   */
void test_uri(axutil_env_t *env)
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

    hostinfo = axutil_uri_parse_hostinfo(env,host);
    CUT_ASSERT_PTR_NOT_EQUAL(hostinfo, NULL, 0);
    
    scheme_port = axutil_uri_port_of_scheme(scheme_str); 
    CUT_ASSERT_INT_NOT_EQUAL(scheme_port, 0, 0);
    
    uri = axutil_uri_parse_string(env,uri_str);    
    CUT_ASSERT_PTR_NOT_EQUAL(uri, NULL, 0);
    str = axutil_uri_get_protocol(uri,env);
	CUT_ASSERT_STR_EQUAL(str, "http", 0);
    port = axutil_uri_get_port(uri,env);
    CUT_ASSERT_INT_EQUAL(port, 80, 0);
    str = axutil_uri_get_path(uri,env);
	CUT_ASSERT_STR_EQUAL(str, "/foo", 0);
    str = axutil_uri_get_host(uri,env);
	CUT_ASSERT_STR_EQUAL(str, "example.com", 0);

    base = axutil_uri_parse_string(env,uri_str_base);
    CUT_ASSERT_PTR_NOT_EQUAL(base, NULL, 0);
	if (base)
    {
        str = axutil_uri_to_string(base,env,0);
		CUT_ASSERT_STR_EQUAL(str, "http://user:XXXXXXXX@example.com/foo?bar", 0);
    }

    clone = axutil_uri_clone(uri,env);
    CUT_ASSERT_PTR_NOT_EQUAL(clone, NULL, 0);
	if (clone)
    {
        str = axutil_uri_to_string(clone,env,0);
		CUT_ASSERT_STR_EQUAL(str, "http://user:XXXXXXXX@example.com/foo?bar#item5", 0);
        axutil_uri_free(clone,env);
   }
    
    rel = axutil_uri_resolve_relative(env,base,uri);
    CUT_ASSERT_PTR_NOT_EQUAL(rel, NULL, 0);
	if (rel)
    {
        str = axutil_uri_to_string(rel,env,0);
		CUT_ASSERT_STR_EQUAL(str, "http://user:XXXXXXXX@example.com/foo?bar#item5", 0);
    }
    
    axutil_uri_free(uri,env);
}

int main()
{   
    axutil_env_t *env = cut_setup_env("Uri");
	CUT_ASSERT(env != NULL);
	if (env) {
        test_uri(env);
        axutil_env_free(env);
    }
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}




