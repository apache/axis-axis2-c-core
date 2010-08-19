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

#include <string.h>   
#include "../util/create_env.h"
#include <axutil_string_util.h>
#include <axutil_array_list.h>
#include <cut_defs.h>

/** @brief test string 
 *  tokenize a string  
 */

void test_string(axutil_env_t *env)
{   
    int delim = ' ';
    void *token = NULL;
    void *last_token_string = NULL;
    void *first_token_string = NULL;
    axutil_array_list_t * first_token, * last_token;
    axis2_char_t * in =  "this is a test string";
    
    axutil_array_list_t * tokenize = axutil_tokenize(env, in, delim);
	CUT_ASSERT(tokenize != NULL);
    if(!tokenize) return;
    token  = axutil_array_list_get(tokenize,env,4);
	CUT_ASSERT(token != NULL);
 	CUT_ASSERT(strcmp(token, "string") == 0);

    first_token = axutil_first_token(env,in,delim);
	CUT_ASSERT(first_token != NULL);
    if(first_token)
    {
        first_token_string = axutil_array_list_get(first_token,env,1);
		CUT_ASSERT(first_token_string != NULL);
 	    CUT_ASSERT(strcmp(first_token_string, "is a test string") == 0);
    }
    
    last_token = axutil_last_token(env,in,delim);
 	CUT_ASSERT(last_token != NULL);
    if(last_token)
    {
        last_token_string = axutil_array_list_get(last_token,env,1);
 		CUT_ASSERT(last_token_string != NULL);
 	    CUT_ASSERT(strcmp(last_token_string, "string") == 0);
    }
    
    /* To avoid warning of not using cut_ptr_equal */
    CUT_ASSERT_PTR_EQUAL(NULL, NULL, 0);
    /* To avoid warning of not using cut_int_equal */
    CUT_ASSERT_INT_EQUAL(0, 0, 0);
    /* To avoid warning of not using cut_str_equal */
    CUT_ASSERT_STR_EQUAL("", "", 0);

}
int main()
{
    axutil_env_t *env = cut_setup_env("String util");
	CUT_ASSERT(env != NULL);
	if (env) {
        test_string(env);
        axutil_env_free(env);
    }
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}



