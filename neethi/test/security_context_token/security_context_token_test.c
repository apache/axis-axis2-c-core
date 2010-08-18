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

#include <rp_security_context_token.h>
#include "../../../util/test/util/create_env.h"

axutil_env_t *env = NULL;
rp_security_context_token_t * security_context_token = NULL;
axis2_char_t * inclusion = NULL ;
axis2_status_t status = AXIS2_FAILURE ;
axis2_bool_t derivedkeys ;
axis2_bool_t require_external_uri_ref;
axis2_bool_t sc10_security_context_token;

/** @brief test options 
 * build a om from a client options
 */

axis2_status_t test_security_context_token(axutil_env_t *env)
{  
   security_context_token = rp_security_context_token_create(env); 
   inclusion = rp_security_context_token_get_inclusion(security_context_token,env);
   printf("%s\n",inclusion);
   status = rp_security_context_token_set_inclusion(security_context_token,env,inclusion);
   if (status == AXIS2_SUCCESS)
        printf("rp security context token set is success\n");
   derivedkeys = rp_security_context_token_get_derivedkeys(security_context_token,env);
   status = rp_security_context_token_set_derivedkeys(security_context_token,env,derivedkeys);
   if (status == AXIS2_SUCCESS)
        printf("rp security token derivedkeys set is success\n");
   require_external_uri_ref = rp_security_context_token_get_require_external_uri_ref(security_context_token,env);
   status = rp_security_context_token_set_require_external_uri_ref(security_context_token,env,require_external_uri_ref);
   if (status == AXIS2_SUCCESS)
        printf("rp security require external uri set is success\n");
   sc10_security_context_token = rp_security_context_token_get_sc10_security_context_token(security_context_token,env);
   status = rp_security_context_token_set_sc10_security_context_token(security_context_token,env,sc10_security_context_token);
   if (status == AXIS2_SUCCESS)
        printf("rp security sc10 security token  set is success\n");
   rp_security_context_token_increment_ref(security_context_token,env);
   rp_security_context_token_free(security_context_token,env);
   return AXIS2_SUCCESS;
}
int main()
{
    int status = AXIS2_SUCCESS;
    env = create_environment();
    status = test_security_context_token(env);
    if(status == AXIS2_FAILURE)
    {
        printf(" test  failed");
    }
    axutil_env_free(env);
    return 0;
}



