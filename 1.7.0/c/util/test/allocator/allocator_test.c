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

#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axiom_node.h>
#include <stdio.h>
#include <cut_defs.h>
#include "../util/create_env.h"

unsigned char buffer[1024];

void test_base64(axutil_env_t *env)
{  
    axis2_status_t status = AXIS2_FAILURE;
    axutil_base64_binary_t *base64_binary;
    axutil_base64_binary_t *plain_base64_binary;
    const char *encoded_binary;
    unsigned char * get_binary = NULL;
    int plain_binary_len, b_len;
    unsigned char * plain_binary = NULL;
	FILE *in = fopen("test","rb");
	CUT_ASSERT(in != NULL);
    if (!in) return;
	
    plain_binary_len = fread(buffer,1,sizeof(buffer),in);
    fclose(in);
    plain_binary = buffer;
    CUT_ASSERT(plain_binary_len != 0);
    if (plain_binary_len == 0 ) return;
	
    base64_binary = axutil_base64_binary_create(env);
    CUT_ASSERT(base64_binary != NULL);

    plain_base64_binary = axutil_base64_binary_create_with_plain_binary(env,
                                                                        plain_binary,
                                                                        plain_binary_len); 
    CUT_ASSERT(plain_base64_binary != NULL);

    encoded_binary = axutil_base64_binary_get_encoded_binary(plain_base64_binary,env);
    CUT_ASSERT(encoded_binary != NULL);

    status = axutil_base64_binary_set_plain_binary(base64_binary,env,plain_binary,
                                                   plain_binary_len);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    plain_binary = axutil_base64_binary_get_plain_binary(base64_binary,env,&b_len);
    CUT_ASSERT(plain_binary != NULL);
    status = axutil_base64_binary_set_encoded_binary(base64_binary,env,encoded_binary);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    get_binary = (unsigned char *) axutil_base64_binary_get_encoded_binary(base64_binary,env);
    CUT_ASSERT(get_binary != NULL);
    b_len = axutil_base64_binary_get_encoded_binary_len(base64_binary,env);
    CUT_ASSERT(b_len != 0);
		
    axutil_base64_binary_free(base64_binary, env);
	base64_binary = axutil_base64_binary_create_with_encoded_binary(env, encoded_binary);
    CUT_ASSERT(base64_binary != NULL);
    if (base64_binary != NULL)
	{
		int binary_len;
		get_binary = axutil_base64_binary_get_plain_binary(base64_binary,env, &binary_len);
		CUT_ASSERT(binary_len == plain_binary_len);
        CUT_ASSERT(memcmp(get_binary, plain_binary, plain_binary_len) == 0);
        axutil_base64_binary_free(base64_binary, env);
	}
		
    /* To avoid warning of not using cut_ptr_equal */
    CUT_ASSERT_PTR_EQUAL(NULL, NULL, 0);
    /* To avoid warning of not using cut_int_equal */
    CUT_ASSERT_INT_EQUAL(0, 0, 0);
    /* To avoid warning of not using cut_str_equal */
    CUT_ASSERT_STR_EQUAL("", "", 0);

	axutil_base64_binary_free(plain_base64_binary, env);
}

int main()
{
    axutil_env_t *env = cut_setup_env("Base64");
    CUT_ASSERT(env != NULL);
    test_base64(env);
    axutil_env_free(env);
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}








