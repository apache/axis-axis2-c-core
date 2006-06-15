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

/* Rampart_crypto_util will contain crypto functionalities of the rampart model
 * 
 */
#include <stdio.h>
#include <rampart_crypto_util.h>
#include <axis2_util.h>
#include <axis2_base64.h>

/*Calculate the hash of concatenated string of 
 * nonce, created and the password.
 * 
 */
#define SIZE 256
#define SIZE_HASH 32

AXIS2_EXTERN axis2_char_t* AXIS2_CALL rampart_crypto_sha1(const axis2_env_t *env, 
					const axis2_char_t *nonce,
					const axis2_char_t *created,
					const axis2_char_t *password)
					
{
	axis2_char_t *result = NULL;
	char* input = NULL;
	axis2_char_t* encoded_str=NULL;

	input= AXIS2_MALLOC(env->allocator, AXIS2_STRLEN(nonce) + 
		    AXIS2_STRLEN(created) + AXIS2_STRLEN(password) + 1);
	sprintf(input, "%s%s%s", nonce, created, password);
	result = AXIS2_MALLOC(env->allocator, SHA_DIGEST_LENGTH + 1);
    
	SHA1(input,SHA_DIGEST_LENGTH, result);
	result[SHA_DIGEST_LENGTH + 1] = '\0';
	  
	encoded_str = AXIS2_MALLOC(env->allocator, axis2_base64_encode_len(SIZE_HASH));
	axis2_base64_encode(encoded_str, result, AXIS2_STRLEN(result));
	
    AXIS2_FREE(env->allocator, input);
    AXIS2_FREE(env->allocator, result);

	return encoded_str;
}
