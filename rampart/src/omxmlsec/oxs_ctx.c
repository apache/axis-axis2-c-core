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
#include <oxs_constants.h>
#include <oxs_ctx.h>


AXIS2_EXTERN enc_ctx_t* AXIS2_CALL
oxs_ctx_create_ctx(const axis2_env_t *env)
{
    enc_ctx_t* ctx = NULL;  
    ctx = (enc_ctx_t*) AXIS2_MALLOC(env->allocator,sizeof(enc_ctx_t));
    
    return ctx;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_ctx_free_ctx(enc_ctx_t *ctx)
{
    /*TODO Free memory*/
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_ctx_reset_ctx(enc_ctx_t *ctx)
{
    /*TODO Free memory before Reset*/
    /*mode*/
    ctx->mode = enc_ctx_mode_encrypted_data;

    /*transformation type */
    ctx->operation = enc_ctx_transform_operation_none;

    /* attributes from EncryptedData or EncryptedKey */
    ctx->encdata_id = NULL;
    ctx->encdata_type = NULL;
    ctx->encdata_mime_type = NULL;
    ctx->encdata_encoding = NULL;

    /*attributes from EncryptionMethod*/
    ctx->encmtd_algorithm = NULL;

    /*Key name*/
    ctx->keyname_val = NULL;

    ctx->enc_data_node = NULL;
    ctx->enc_method_node = NULL;
    ctx->key_info_node = NULL;
    ctx->cipher_value_node = NULL;


}

