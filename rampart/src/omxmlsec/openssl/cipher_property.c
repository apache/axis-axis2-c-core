/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <stdio.h>
#include <axis2_util.h>
#include <openssl_cipher_property.h>
#include <openssl_constants.h>
#include <oxs_error.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

/**
* Create a fresh block cipher property
*@return cipher_prop_ptr
*/
AXIS2_EXTERN cipher_prop_ptr AXIS2_CALL
openssl_cipher_property_create(const axis2_env_t *env)
{
    cipher_prop_ptr cprop = NULL;
    cprop = (cipher_prop_ptr) AXIS2_MALLOC(env->allocator,sizeof(cipher_prop));
    openssl_cipher_property_reset(env, cprop);
    return cprop;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
openssl_cipher_property_reset(const axis2_env_t *env, cipher_prop_ptr cprop)
{
    cprop->cipher = NULL;
    cprop->name = NULL;
    cprop->key_size = -1;
    cprop->block_size = -1;
    cprop->iv_size = -1;

    return AXIS2_SUCCESS;
}

/*TODO write the free function*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
openssl_cipher_property_free(const axis2_env_t *env )
{
    return AXIS2_SUCCESS;
}


