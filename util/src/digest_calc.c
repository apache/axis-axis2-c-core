
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

#include <axutil_string.h>
#include <axutil_md5.h>
#include <axutil_digest_calc.h>

void AXIS2_CALL convert_to_hex(
    axutil_digest_hash_t bin,
    axutil_digest_hash_hex_t hex);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_digest_calc_get_h_a1(
    const axutil_env_t *env,
    char *algorithm,                      
    char *user_name,                      
    char *realm,                          
    char *password,                       
    char *nonce,                          
    char *cnonce,                         
    axutil_digest_hash_hex_t session_key)
{
    axutil_md5_ctx_t *ctx;
    axutil_digest_hash_t ha1;

    ctx = axutil_md5_ctx_create(env);
    if (!ctx)
        return AXIS2_FAILURE;
    axutil_md5_update(ctx, env, user_name, strlen(user_name));
    axutil_md5_update(ctx, env, ":", 1);
    axutil_md5_update(ctx, env, realm, strlen(realm));
    axutil_md5_update(ctx, env, ":", 1);
    axutil_md5_update(ctx, env, password, strlen(password));
    axutil_md5_final(ctx, env, ha1);
    axutil_md5_ctx_free(ctx, env);
    if (!axutil_strcasecmp(algorithm, "md5-sess"))
    {
        ctx = axutil_md5_ctx_create(env);
        if (!ctx)
            return AXIS2_FAILURE;
        axutil_md5_update(ctx, env, ha1, AXIS2_DIGEST_HASH_LEN);
        axutil_md5_update(ctx, env, ":", 1);
        axutil_md5_update(ctx, env, nonce, strlen(nonce));
        axutil_md5_update(ctx, env, ":", 1);
        axutil_md5_update(ctx, env, cnonce, strlen(cnonce));
        axutil_md5_final(ctx, env, ha1);
        axutil_md5_ctx_free(ctx, env);
    }
    convert_to_hex(ha1, session_key);
    return AXIS2_SUCCESS;
} 

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_digest_calc_get_response(
    const axutil_env_t *env,
    axutil_digest_hash_hex_t h_a1,                       
    char *nonce,                       
    char *nonce_count,                 
    char *cnonce,                      
    char *qop,                         
    char *method,                      
    char *digest_uri,                  
    axutil_digest_hash_hex_t h_entity,  
    axutil_digest_hash_hex_t response)
{
    axutil_md5_ctx_t *ctx;
    axutil_digest_hash_t ha2;
    axutil_digest_hash_t resp_hash;
    axutil_digest_hash_hex_t ha2_hex;

    ctx = axutil_md5_ctx_create(env);
    if (!ctx)
        return AXIS2_FAILURE;
    axutil_md5_update(ctx, env, method, strlen(method));
    axutil_md5_update(ctx, env, ":", 1);
    axutil_md5_update(ctx, env, digest_uri, strlen(digest_uri));
    if (!axutil_strcasecmp(qop, "auth-int"))
    {
        axutil_md5_update(ctx, env, ":", 1);
        axutil_md5_update(ctx, env, h_entity, AXIS2_DIGEST_HASH_HEX_LEN);
    };
    axutil_md5_final(ctx, env, ha2);
    axutil_md5_ctx_free(ctx, env);
    convert_to_hex(ha2, ha2_hex);

    ctx = axutil_md5_ctx_create(env);
    if (!ctx)
        return AXIS2_FAILURE;
    axutil_md5_update(ctx, env, h_a1, AXIS2_DIGEST_HASH_HEX_LEN);
    axutil_md5_update(ctx, env, ":", 1);
    axutil_md5_update(ctx, env, nonce, strlen(nonce));
    axutil_md5_update(ctx, env, ":", 1);
    if (*qop)
    {
        axutil_md5_update(ctx, env, nonce_count, strlen(nonce_count));
        axutil_md5_update(ctx, env, ":", 1);
        axutil_md5_update(ctx, env, cnonce, strlen(cnonce));
        axutil_md5_update(ctx, env, ":", 1);
        axutil_md5_update(ctx, env, qop, strlen(qop));
        axutil_md5_update(ctx, env, ":", 1);
    };
    axutil_md5_update(ctx, env, ha2_hex, AXIS2_DIGEST_HASH_HEX_LEN);
    axutil_md5_final(ctx, env, resp_hash);
    axutil_md5_ctx_free(ctx, env);
    convert_to_hex(resp_hash, response);
    return AXIS2_SUCCESS;
}

void AXIS2_CALL convert_to_hex(
    axutil_digest_hash_t bin,
    axutil_digest_hash_hex_t hex)
{
    unsigned short i;
    unsigned char j;

    for (i = 0; i < AXIS2_DIGEST_HASH_LEN; i++)
    {
        j = (bin[i] >> 4) & 0xf;
        if (j <= 9)
            hex[i*2] = (j + '0');
         else
            hex[i*2] = (j + 'a' - 10);
        j = bin[i] & 0xf;
        if (j <= 9)
            hex[i*2+1] = (j + '0');
         else
            hex[i*2+1] = (j + 'a' - 10);
    }
    hex[AXIS2_DIGEST_HASH_HEX_LEN] = '\0';
}
