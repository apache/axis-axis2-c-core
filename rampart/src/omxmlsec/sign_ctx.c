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
#include <oxs_sign_ctx.h>
#include <oxs_error.h>

struct oxs_sign_ctx_t
{
    axis2_char_t *sign_mtd_algo;
    axis2_char_t *c14n_mtd ; 
    axis2_array_list_t *sign_parts; 
    /*TODO:  key*/
};

/*Public functions*/
axis2_char_t *AXIS2_CALL
oxs_sign_ctx_get_sign_mtd_algo(
    const oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env)
{
    return sign_ctx->sign_mtd_algo;
}

axis2_char_t *AXIS2_CALL
oxs_sign_ctx_get_c14n_mtd(
    const oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env)
{
    return sign_ctx->c14n_mtd;
}


axis2_array_list_t *AXIS2_CALL
oxs_sign_ctx_get_sign_parts(
    const oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env)
{
    return sign_ctx->sign_parts;
}

axis2_status_t AXIS2_CALL
oxs_sign_ctx_set_sign_mtd_algo(
    oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env,
    axis2_char_t *sign_mtd_algo)
{

    if (sign_ctx->sign_mtd_algo)
    {
        AXIS2_FREE(env->allocator, sign_ctx->sign_mtd_algo);
        sign_ctx->sign_mtd_algo = NULL;
    }
    sign_ctx->sign_mtd_algo = AXIS2_STRDUP(sign_mtd_algo, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_sign_ctx_set_c14n_mtd(
    oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env,
    axis2_char_t *c14n_mtd)
{

    if (sign_ctx->c14n_mtd)
    {
        AXIS2_FREE(env->allocator, sign_ctx->c14n_mtd);
        sign_ctx->c14n_mtd = NULL;
    }
    sign_ctx->c14n_mtd = AXIS2_STRDUP(c14n_mtd, env);
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
oxs_sign_ctx_set_sign_parts(
    oxs_sign_ctx_t *sign_ctx,
    const axis2_env_t *env,
    axis2_array_list_t *sign_parts)
{
    sign_ctx->sign_parts = sign_parts;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN oxs_sign_ctx_t *AXIS2_CALL
oxs_sign_ctx_create(const axis2_env_t *env)
{
    oxs_sign_ctx_t *sign_ctx = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    sign_ctx = AXIS2_MALLOC(env->allocator, sizeof(oxs_sign_ctx_t));
    if (!sign_ctx)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    sign_ctx->sign_mtd_algo= NULL;
    sign_ctx->c14n_mtd = NULL;
    sign_ctx->sign_parts = NULL;

    return sign_ctx;
}


axis2_status_t AXIS2_CALL
oxs_sign_ctx_free(oxs_sign_ctx_t *sign_ctx,
        const axis2_env_t *env)
{

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (sign_ctx->sign_mtd_algo)
    {
        AXIS2_FREE(env->allocator, sign_ctx->sign_mtd_algo);
        sign_ctx->sign_mtd_algo = NULL;
    }

    if (sign_ctx->c14n_mtd)
    {
        AXIS2_FREE(env->allocator, sign_ctx->c14n_mtd);
        sign_ctx->c14n_mtd = NULL;
    }

    sign_ctx->sign_parts = NULL;

    AXIS2_FREE(env->allocator,  sign_ctx);
    sign_ctx = NULL;

    return AXIS2_SUCCESS;
}


