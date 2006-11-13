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
#include <oxs_asym_ctx.h>
#include <oxs_certificate.h>
#include <oxs_error.h>

typedef struct oxs_asym_ctx_impl
{
    oxs_asym_ctx_t asym_ctx;
    
    axis2_char_t *file_name;
    axis2_char_t *algorithm;
    oxs_asym_ctx_operation_t operation;   
    oxs_certificate_t *certificate;
}
oxs_asym_ctx_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(oxs_asym_ctx) ((oxs_asym_ctx_impl_t *)oxs_asym_ctx)

/******** function headers ***************/

/*private functions*/
static void
oxs_asym_ctx_init_ops(
    oxs_asym_ctx_t *asym_ctx);

/*Public functions*/
axis2_status_t AXIS2_CALL
oxs_asym_ctx_free_impl(
    oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_asym_ctx_get_file_name_impl(
    const oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
oxs_asym_ctx_get_algorithm_impl(
    const oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env);

oxs_asym_ctx_operation_t AXIS2_CALL
oxs_asym_ctx_get_operation_impl(
    const oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env);

oxs_certificate_t *AXIS2_CALL
oxs_asym_ctx_get_certificate_impl(
    const oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_file_name_impl(
    oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env,
    axis2_char_t *file_name);

axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_algorithm_impl(
    oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env,
    axis2_char_t *algorithm);

axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_operation_impl(
    oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env,
    oxs_asym_ctx_operation_t operation);

axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_certificate_impl(
    oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env,
    oxs_certificate_t *certificate);

/******************** end of function headers *****************/


axis2_char_t *AXIS2_CALL
oxs_asym_ctx_get_file_name_impl(
    const oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env)
{
    oxs_asym_ctx_impl_t *asym_ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    asym_ctx_impl = AXIS2_INTF_TO_IMPL(asym_ctx);

    return asym_ctx_impl->file_name;
}

axis2_char_t *AXIS2_CALL
oxs_asym_ctx_get_algorithm_impl(
    const oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env)
{
    oxs_asym_ctx_impl_t *asym_ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    asym_ctx_impl = AXIS2_INTF_TO_IMPL(asym_ctx);

    return asym_ctx_impl->algorithm;
}

oxs_asym_ctx_operation_t AXIS2_CALL
oxs_asym_ctx_get_operation_impl(
    const oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env)
{
    oxs_asym_ctx_impl_t *asym_ctx_impl = NULL;
    /*AXIS2_ENV_CHECK(env, NULL);*/
    asym_ctx_impl = AXIS2_INTF_TO_IMPL(asym_ctx);

    return asym_ctx_impl->operation;
}

oxs_certificate_t *AXIS2_CALL
oxs_asym_ctx_get_certificate_impl(
    const oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env)
{
    oxs_asym_ctx_impl_t *asym_ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    asym_ctx_impl = AXIS2_INTF_TO_IMPL(asym_ctx);

    return asym_ctx_impl->certificate;
}

axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_file_name_impl(
    oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env,
    axis2_char_t *file_name)
{
    oxs_asym_ctx_impl_t *oxs_asym_ctx_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, file_name, AXIS2_FAILURE);
    oxs_asym_ctx_impl = AXIS2_INTF_TO_IMPL(asym_ctx);

    if (oxs_asym_ctx_impl->file_name)
    {
        AXIS2_FREE(env->allocator, oxs_asym_ctx_impl->file_name);
        oxs_asym_ctx_impl->file_name = NULL;
    }
    oxs_asym_ctx_impl->file_name = AXIS2_STRDUP(file_name, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_algorithm_impl(
    oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env,
    axis2_char_t *algorithm)
{
    oxs_asym_ctx_impl_t *oxs_asym_ctx_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, algorithm, AXIS2_FAILURE);
    oxs_asym_ctx_impl = AXIS2_INTF_TO_IMPL(asym_ctx);

    if (oxs_asym_ctx_impl->algorithm)
    {
        AXIS2_FREE(env->allocator, oxs_asym_ctx_impl->algorithm);
        oxs_asym_ctx_impl->algorithm = NULL;
    }
    oxs_asym_ctx_impl->algorithm = AXIS2_STRDUP(algorithm, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_operation_impl(
    oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env,
    oxs_asym_ctx_operation_t operation)
{
    oxs_asym_ctx_impl_t *oxs_asym_ctx_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    oxs_asym_ctx_impl = AXIS2_INTF_TO_IMPL(asym_ctx);

    oxs_asym_ctx_impl->operation = operation;
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_certificate_impl(
    oxs_asym_ctx_t *asym_ctx,
    const axis2_env_t *env,
    oxs_certificate_t *certificate)
{
    oxs_asym_ctx_impl_t *oxs_asym_ctx_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, certificate, AXIS2_FAILURE);
    oxs_asym_ctx_impl = AXIS2_INTF_TO_IMPL(asym_ctx);

    if (oxs_asym_ctx_impl->certificate)
    {
        oxs_certificate_free(oxs_asym_ctx_impl->certificate, env);
        oxs_asym_ctx_impl->certificate = NULL;
    }
    oxs_asym_ctx_impl->certificate = certificate;
    return AXIS2_SUCCESS;
}


static void
oxs_asym_ctx_init_ops(
    oxs_asym_ctx_t *asym_ctx)
{
    asym_ctx->ops->get_file_name  = oxs_asym_ctx_get_file_name_impl;
    asym_ctx->ops->get_algorithm  = oxs_asym_ctx_get_algorithm_impl;
    asym_ctx->ops->get_operation  = oxs_asym_ctx_get_operation_impl;
    asym_ctx->ops->get_certificate  = oxs_asym_ctx_get_certificate_impl;
    asym_ctx->ops->set_file_name   = oxs_asym_ctx_set_file_name_impl;
    asym_ctx->ops->set_algorithm  = oxs_asym_ctx_set_algorithm_impl;
    asym_ctx->ops->set_operation  = oxs_asym_ctx_set_operation_impl;
    asym_ctx->ops->set_certificate  = oxs_asym_ctx_set_certificate_impl;
    asym_ctx->ops->free      = oxs_asym_ctx_free_impl;
}


AXIS2_EXTERN oxs_asym_ctx_t *AXIS2_CALL
oxs_asym_ctx_create_asym_ctx(const axis2_env_t *env)
{
    oxs_asym_ctx_impl_t *asym_ctx_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    asym_ctx_impl = AXIS2_MALLOC(env->allocator, sizeof(oxs_asym_ctx_impl_t));
    if (!asym_ctx_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    asym_ctx_impl->file_name= NULL;
    asym_ctx_impl->algorithm = NULL;
    asym_ctx_impl->operation = -1;
    asym_ctx_impl->certificate = NULL;

    asym_ctx_impl->asym_ctx.ops =  AXIS2_MALLOC(env->allocator, sizeof(oxs_asym_ctx_ops_t));
    if (!asym_ctx_impl->asym_ctx.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        oxs_asym_ctx_free(&(asym_ctx_impl->asym_ctx), env);
        return NULL;
    }

    oxs_asym_ctx_init_ops(&(asym_ctx_impl->asym_ctx));

    return &(asym_ctx_impl->asym_ctx);

}


axis2_status_t AXIS2_CALL
oxs_asym_ctx_free_impl(oxs_asym_ctx_t *asym_ctx,
        const axis2_env_t *env)
{
    oxs_asym_ctx_impl_t *asym_ctx_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    asym_ctx_impl = AXIS2_INTF_TO_IMPL(asym_ctx);

    if (asym_ctx_impl->file_name)
    {
        AXIS2_FREE(env->allocator, asym_ctx_impl->file_name);
        asym_ctx_impl->file_name = NULL;
    }

    if (asym_ctx_impl->algorithm)
    {
        AXIS2_FREE(env->allocator, asym_ctx_impl->algorithm);
        asym_ctx_impl->algorithm = NULL;
    }

    if (asym_ctx_impl->certificate)
    {
        oxs_certificate_free(asym_ctx_impl->certificate, env);
        asym_ctx_impl->certificate = NULL;
    }

    AXIS2_FREE(env->allocator,  asym_ctx_impl);
    asym_ctx_impl = NULL;

    return AXIS2_SUCCESS;
}

/**********************Wrappers******************************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_free(oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env)
{
     return  ctx->ops->free(ctx, env);
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_asym_ctx_get_file_name(const oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env)
{
     return  ctx->ops->get_file_name(ctx, env);
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_asym_ctx_get_algorithm(const oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env)
{
     return  ctx->ops->get_algorithm(ctx, env);
}

AXIS2_EXTERN oxs_asym_ctx_operation_t AXIS2_CALL
oxs_asym_ctx_get_operation(const oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env)
{
     return  ctx->ops->get_operation(ctx, env);
}

AXIS2_EXTERN oxs_certificate_t* AXIS2_CALL
oxs_asym_ctx_get_certificate(const oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env)
{
     return  ctx->ops->get_certificate(ctx, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_file_name(oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *file_name)
{
     return  ctx->ops->set_file_name(ctx, env,file_name );
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_algorithm(oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *algorithm)
{
     return  ctx->ops->set_algorithm(ctx, env, algorithm);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_operation(oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env,
                    oxs_asym_ctx_operation_t operation)
{
     return  ctx->ops->set_operation(ctx, env,operation );
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_asym_ctx_set_certificate(oxs_asym_ctx_t *ctx,
                    const axis2_env_t *env,
                    oxs_certificate_t *certificate)
{
     return  ctx->ops->set_certificate(ctx, env, certificate);
}

