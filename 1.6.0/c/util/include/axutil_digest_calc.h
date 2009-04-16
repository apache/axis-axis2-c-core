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

#ifndef AXUTIL_DIGEST_CALC_H
#define AXUTIL_DIGEST_CALC_H

/**
 * @file axutil_digest_calc.h
 * @brief implements the calculations of H(A1), H(A2),
 * request-digest and response-digest for Axis2 based on rfc2617.
 */

#include <axutil_utils_defines.h>
#include <axutil_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** 
     * @defgroup axis2_util utilities
     * @ingroup axis2
     * @{
     * @}
     */

    /**
     * @defgroup axutil_digest_calc digest_calc
     * @ingroup axis2_util
     * @{
     */

    #define AXIS2_DIGEST_HASH_LEN 16
    #define AXIS2_DIGEST_HASH_HEX_LEN 32

    typedef unsigned char axutil_digest_hash_t[AXIS2_DIGEST_HASH_LEN];
    typedef unsigned char axutil_digest_hash_hex_t[AXIS2_DIGEST_HASH_HEX_LEN + 1];

    /**
      * calculate H(A1) as per HTTP Digest spec
      * @param env, pointer to env struct
      * @param algorithm, algorithm
      * @param user_name, user name
      * @param realm, reaalm
      * @param password, password
      * @param nonce, nonce from server
      * @param cnonce, client nonce
      * @param session_key, H(A1)
      * @return AXIS2_SUCCESS on success or AXIS2_FAILURE
      */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_digest_calc_get_h_a1(
        const axutil_env_t * env,
        char * algorithm,               
        char * user_name,              
        char * realm,                 
        char * password,             
        char * nonce,               
        char * cnonce,             
        axutil_digest_hash_hex_t session_key); 

    /** 
      * calculate request-digest/response-digest as per HTTP Digest spec 
      * @param env, pointer to env struct
      * @param h_a1, H(A1)
      * @param nonce, nonce from server
      * @param cnonce, client nonce
      * @param qop, qop-value: "", "auth", "auth-int"
      * @param method, method from the request
      * @param digest_uri, requested URL 
      * @param h_entry, H(entity body) if qop="auth-int"
      * @param response, request-digest or response-digest
      * @return AXIS2_SUCCESS on success or AXIS2_FAILURE
      */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axutil_digest_calc_get_response(
        const axutil_env_t * env,
        axutil_digest_hash_hex_t h_a1,                       
        char * nonce,                      
        char * nonce_count,               
        char * cnonce,                   
        char * qop,                     
        char * method,                 
        char * digest_uri,            
        axutil_digest_hash_hex_t h_entity,  
        axutil_digest_hash_hex_t response); 

    /** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_DIGEST_CALC_H */
