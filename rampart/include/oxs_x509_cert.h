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

#ifndef OXS_X509_CERT
#define OXS_X509_CERT


/**
  * @file oxs_certificate.h
  * @brief 
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <openssl_pkey.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup oxs_x509_cert 
 * @ingroup 
 * @{
 */

/**
 * issuer
 * subject
 * dates
 * hash
 * fingerprint
 */

typedef struct oxs_x509_cert_t oxs_x509_cert_t;
/*Create*/
AXIS2_EXTERN oxs_x509_cert_t *AXIS2_CALL
oxs_x509_cert_create( const axis2_env_t *env);

/*Free*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_x509_cert_free(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env);

/*Getters*/
AXIS2_EXTERN int AXIS2_CALL
oxs_x509_cert_get_serial_number(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
oxs_x509_cert_get_subject(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
oxs_x509_cert_get_issuer(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
oxs_x509_cert_get_key_identifier(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
oxs_x509_cert_get_fingerprint(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
oxs_x509_cert_get_date(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
oxs_x509_cert_get_hash(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
oxs_x509_cert_get_data(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env);

AXIS2_EXTERN openssl_pkey_t *AXIS2_CALL
oxs_x509_cert_get_public_key(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env);

/*Setters*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_x509_cert_set_serial_number(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env,
    int value);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_x509_cert_set_issuer(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env,
    axis2_char_t *value);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_x509_cert_set_key_identifier(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env,
    axis2_char_t *value);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_x509_cert_set_subject(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env,
    axis2_char_t *value);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_x509_cert_set_fingerprint(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env,
    axis2_char_t *value);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_x509_cert_set_date(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env,
    axis2_char_t *value);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_x509_cert_set_hash(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env,
    axis2_char_t *value);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_x509_cert_set_data(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env,
    axis2_char_t *value);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_x509_cert_set_public_key(oxs_x509_cert_t *x509_cert,
    const axis2_env_t *env,
    openssl_pkey_t *public_key);

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_X509_CERT */
