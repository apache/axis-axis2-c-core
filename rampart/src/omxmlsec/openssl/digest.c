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
#include <openssl/sha.h>
#include <openssl_digest.h>
#include <axis2_base64.h>

#define SIZE_HASH 32

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
openssl_sha1(const axis2_env_t *env,
            axis2_char_t *input,
            int length)
{
    SHA_CTX c ;
    unsigned char md[SHA_DIGEST_LENGTH];
    axis2_char_t* encoded_str = NULL;

    SHA1_Init(&c);
    SHA1_Update(&c,(unsigned char*)input,length);
    SHA1_Final(md,&c);

    encoded_str = AXIS2_MALLOC(env->allocator, axis2_base64_encode_len(SIZE_HASH));
    axis2_base64_encode(encoded_str, (char*)md, SHA_DIGEST_LENGTH);

    return encoded_str;
}




