
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

#ifndef RP_SIGNED_ENCRYPTED_PARTS_H
#define RP_SIGNED_ENCRYPTED_PARTS_H

/** @defgroup rp_signed_encrypted_parts
 * @ingroup rp_signed_encrypted_parts
 * @{
 */

#include <rp_includes.h>
#include <rp_header.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct rp_signed_encrypted_parts_t rp_signed_encrypted_parts_t;

    AXIS2_EXTERN rp_signed_encrypted_parts_t *AXIS2_CALL
    rp_signed_encrypted_parts_create(
        const axutil_env_t * env);

    AXIS2_EXTERN void AXIS2_CALL
    rp_signed_encrypted_parts_free(
        rp_signed_encrypted_parts_t * signed_encrypted_parts,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_signed_encrypted_parts_get_body(
        rp_signed_encrypted_parts_t * signed_encrypted_parts,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_signed_encrypted_parts_set_body(
        rp_signed_encrypted_parts_t * signed_encrypted_parts,
        const axutil_env_t * env,
        axis2_bool_t body);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_signed_encrypted_parts_get_signedparts(
        rp_signed_encrypted_parts_t * signed_encrypted_parts,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_signed_encrypted_parts_set_signedparts(
        rp_signed_encrypted_parts_t * signed_encrypted_parts,
        const axutil_env_t * env,
        axis2_bool_t signedparts);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_signed_encrypted_parts_get_attachments(
        rp_signed_encrypted_parts_t * signed_encrypted_parts,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_signed_encrypted_parts_set_attachments(
        rp_signed_encrypted_parts_t * signed_encrypted_parts,
        const axutil_env_t * env,
        axis2_bool_t attachments);

    AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
    rp_signed_encrypted_parts_get_headers(
        rp_signed_encrypted_parts_t * signed_encrypted_parts,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_signed_encrypted_parts_add_header(
        rp_signed_encrypted_parts_t * signed_encrypted_parts,
        const axutil_env_t * env,
        rp_header_t * header);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_signed_encrypted_parts_increment_ref(
        rp_signed_encrypted_parts_t * signed_encrypted_parts,
        const axutil_env_t * env);

#ifdef __cplusplus
}
#endif
#endif
