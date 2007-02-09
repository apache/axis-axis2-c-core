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

#include <stdio.h>
#include <axis2_util.h>
#include <oxs_constants.h>
#include <oxs_error.h>
#include <oxs_buffer.h>
#include <oxs_cipher.h>
#include <oxs_sign_ctx.h>
#include <oxs_signature.h>
#include <oxs_buffer.h>
#include <openssl_rsa.h>
#include <openssl_digest.h>
/*Private functions*/



/*Public functions*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL 
oxs_sig_sign(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    oxs_buffer_t *input,
    oxs_buffer_t *output)
{
    /*Get the content*/
    
    /*Make the digest*/

    /*Sign*/

    return AXIS2_SUCCESS;
}

