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
#include <oxs_constants.h>
#include <oxs_ctx.h>


typedef struct oxs_ctx_impl
{
    oxs_ctx_t ctx;

    /*Encryption mode*/
    oxs_ctx_mode_t mode;

    /*transformation type */
    oxs_ctx_operation_t operation;

    /*key*/
    oxs_key_t* key;

    /* attributes from EncryptedData or EncryptedKey */
    axis2_char_t*           id;
    axis2_char_t*           type;
    axis2_char_t*           mime_type;
    axis2_char_t*           encoding;
    axis2_char_t*           recipient;
    axis2_char_t*           carried_key_name;

    /*attributes from EncryptionMethod*/
    axis2_char_t*           enc_mtd_algorithm;

    /*Used  in decryption process to keep the data to be decrypted*/
    axis2_char_t*           input_data;

    /*TODO: Not in use. Remove later*/
    axiom_node_t*           enc_data_node;
    axiom_node_t*           enc_method_node;
    axiom_node_t*           key_info_node;
    axiom_node_t*           cipher_value_node;
}
oxs_ctx_impl_t;


/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(oxs_ctx) ((oxs_ctx_impl_t *)oxs_ctx)

/******************* function headers ******************************/
/* private functions */
static void
oxs_ctx_init_ops(
    oxs_ctx_t *ctx);

/*public functions*/
axis2_status_t AXIS2_CALL
oxs_ctx_free(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

oxs_ctx_mode_t AXIS2_CALL
oxs_ctx_get_mode(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

oxs_ctx_operation_t AXIS2_CALL
oxs_ctx_get_operation(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

oxs_key_t *AXIS2_CALL
oxs_ctx_get_key(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_ctx_get_id(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_ctx_get_type(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_ctx_get_mime_type(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_ctx_get_encoding(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_ctx_get_recipient(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_ctx_get_carried_key_name(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_ctx_get_enc_mtd_algorithm(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_ctx_get_input_data(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axiom_node_t *AXIS2_CALL
oxs_ctx_get_enc_data_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axiom_node_t *AXIS2_CALL
oxs_ctx_get_enc_method_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axiom_node_t *AXIS2_CALL
oxs_ctx_get_key_info_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axiom_node_t *AXIS2_CALL
oxs_ctx_get_cipher_value_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_mode(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    oxs_ctx_mode_t mode
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_operation(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    oxs_ctx_operation_t operation
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_key(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    oxs_key_t *key
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_id(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *id
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_type(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *type
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_mime_type(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *mime_type
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_encoding(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *encoding
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_recipient(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *recipient
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_carried_key_name(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *carried_key_name
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_enc_mtd_algorithm(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *enc_mtd_algorithm
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_input_data(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *input_data
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_enc_data_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axiom_node_t *enc_data_node
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_enc_method_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axiom_node_t *enc_method_node
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_key_info_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axiom_node_t *key_info_node
);

axis2_status_t AXIS2_CALL
oxs_ctx_set_cipher_value_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axiom_node_t *cipher_value_node
);


/******************* end of function headers ******************************/
AXIS2_EXTERN oxs_ctx_t *AXIS2_CALL
oxs_ctx_create(const axis2_env_t *env)
{
    oxs_ctx_impl_t *ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    ctx_impl = AXIS2_MALLOC(env->allocator, sizeof(oxs_ctx_impl_t));
    if (!ctx_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ctx_impl->mode  = -1;
    ctx_impl->operation  = -1;
    ctx_impl->key  = NULL;
    ctx_impl->id  = NULL;
    ctx_impl->type  = NULL;
    ctx_impl->mime_type  = NULL;
    ctx_impl->encoding  = NULL;
    ctx_impl->recipient  = NULL;
    ctx_impl->carried_key_name  = NULL;
    ctx_impl->enc_mtd_algorithm  = NULL;
    ctx_impl->input_data  = NULL;
    ctx_impl->enc_data_node  = NULL;
    ctx_impl->enc_method_node  = NULL;
    ctx_impl->key_info_node  = NULL;
    ctx_impl->cipher_value_node  = NULL;


    ctx_impl->ctx.ops =  AXIS2_MALLOC(env->allocator, sizeof(oxs_ctx_ops_t));
    if (!ctx_impl->ctx.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        oxs_ctx_free(&(ctx_impl->ctx), env);
        return NULL;
    }

    oxs_ctx_init_ops(&(ctx_impl->ctx));

    return &(ctx_impl->ctx);
}

/* private functions */
static void
oxs_ctx_init_ops(
    oxs_ctx_t *ctx)
{
    ctx->ops->free = oxs_ctx_free ;

    ctx->ops->get_mode = oxs_ctx_get_mode ;
    ctx->ops->get_operation = oxs_ctx_get_operation ;
    ctx->ops->get_key = oxs_ctx_get_key;
    ctx->ops->get_id = oxs_ctx_get_id ;
    ctx->ops->get_type = oxs_ctx_get_type ;
    ctx->ops->get_mime_type = oxs_ctx_get_mime_type ;
    ctx->ops->get_encoding = oxs_ctx_get_encoding ;
    ctx->ops->get_recipient = oxs_ctx_get_recipient ;
    ctx->ops->get_carried_key_name = oxs_ctx_get_carried_key_name ;
    ctx->ops->get_enc_mtd_algorithm = oxs_ctx_get_enc_mtd_algorithm ;
    ctx->ops->get_input_data = oxs_ctx_get_input_data ;
    ctx->ops->get_enc_data_node = oxs_ctx_get_enc_data_node ;
    ctx->ops->get_enc_method_node = oxs_ctx_get_enc_method_node ;
    ctx->ops->get_key_info_node = oxs_ctx_get_key_info_node ;
    ctx->ops->get_cipher_value_node = oxs_ctx_get_cipher_value_node ;

    ctx->ops->set_mode = oxs_ctx_set_mode ;
    ctx->ops->set_operation = oxs_ctx_set_operation ;
    ctx->ops->set_key = oxs_ctx_set_key;
    ctx->ops->set_id = oxs_ctx_set_id ;
    ctx->ops->set_type = oxs_ctx_set_type ;
    ctx->ops->set_mime_type = oxs_ctx_set_mime_type ;
    ctx->ops->set_encoding = oxs_ctx_set_encoding ;
    ctx->ops->set_recipient = oxs_ctx_set_recipient ;
    ctx->ops->set_carried_key_name = oxs_ctx_set_carried_key_name ;
    ctx->ops->set_enc_mtd_algorithm = oxs_ctx_set_enc_mtd_algorithm ;
    ctx->ops->set_input_data = oxs_ctx_set_input_data ;
    ctx->ops->set_enc_data_node = oxs_ctx_set_enc_data_node ;
    ctx->ops->set_enc_method_node = oxs_ctx_set_enc_method_node ;
    ctx->ops->set_key_info_node = oxs_ctx_set_key_info_node ;
    ctx->ops->set_cipher_value_node = oxs_ctx_set_cipher_value_node ;
}

/*public functions*/
axis2_status_t AXIS2_CALL
oxs_ctx_free(oxs_ctx_t *ctx,
        const axis2_env_t *env
            )
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);


    if (ctx_impl->id)
    {
        AXIS2_FREE(env->allocator, ctx_impl->id);
        ctx_impl->id = NULL;
    }

    if (ctx_impl->type)
    {
        AXIS2_FREE(env->allocator, ctx_impl->type);
        ctx_impl->type = NULL;
    }

    if (ctx_impl->mime_type)
    {
        AXIS2_FREE(env->allocator, ctx_impl->mime_type);
        ctx_impl->mime_type = NULL;
    }

    if (ctx_impl->encoding)
    {
        AXIS2_FREE(env->allocator, ctx_impl->encoding);
        ctx_impl->encoding = NULL;
    }

    if (ctx_impl->recipient)
    {
        AXIS2_FREE(env->allocator, ctx_impl->recipient);
        ctx_impl->recipient = NULL;
    }

    if (ctx_impl->carried_key_name)
    {
        AXIS2_FREE(env->allocator, ctx_impl->carried_key_name);
        ctx_impl->carried_key_name = NULL;
    }

    if (ctx_impl->enc_mtd_algorithm)
    {
        AXIS2_FREE(env->allocator, ctx_impl->enc_mtd_algorithm);
        ctx_impl->enc_mtd_algorithm = NULL;
    }

    if (ctx_impl->input_data)
    {
        AXIS2_FREE(env->allocator, ctx_impl->input_data);
        ctx_impl->input_data = NULL;
    }

    /*TODO free nodes and key*/

    AXIS2_FREE(env->allocator,  ctx_impl);
    ctx_impl = NULL;

    return AXIS2_SUCCESS;
}

oxs_ctx_mode_t AXIS2_CALL
oxs_ctx_get_mode(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->mode ;
}

oxs_ctx_operation_t AXIS2_CALL
oxs_ctx_get_operation(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->operation ;
}


oxs_key_t *AXIS2_CALL
oxs_ctx_get_key(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->key ;
}


axis2_char_t *AXIS2_CALL
oxs_ctx_get_id(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->id ;
}


axis2_char_t *AXIS2_CALL
oxs_ctx_get_type(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->type ;
}


axis2_char_t *AXIS2_CALL
oxs_ctx_get_mime_type(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->mime_type ;
}


axis2_char_t *AXIS2_CALL
oxs_ctx_get_encoding(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->encoding ;
}


axis2_char_t *AXIS2_CALL
oxs_ctx_get_recipient(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->recipient ;
}


axis2_char_t *AXIS2_CALL
oxs_ctx_get_carried_key_name(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->carried_key_name ;
}


axis2_char_t *AXIS2_CALL
oxs_ctx_get_enc_mtd_algorithm(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->enc_mtd_algorithm ;
}


axis2_char_t *AXIS2_CALL
oxs_ctx_get_input_data(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->input_data ;
}


axiom_node_t *AXIS2_CALL
oxs_ctx_get_enc_data_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->enc_data_node ;
}


axiom_node_t *AXIS2_CALL
oxs_ctx_get_enc_method_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->enc_method_node ;
}


axiom_node_t *AXIS2_CALL
oxs_ctx_get_key_info_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->key_info_node ;
}


axiom_node_t *AXIS2_CALL
oxs_ctx_get_cipher_value_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->cipher_value_node ;
}


axis2_status_t AXIS2_CALL
oxs_ctx_set_mode(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    oxs_ctx_mode_t mode
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    ctx_impl->mode = mode ;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_operation(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    oxs_ctx_operation_t operation
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    ctx_impl->operation = operation ;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_key(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    oxs_key_t *key
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->key)
    {
        AXIS2_FREE(env->allocator, ctx_impl->key);
        ctx_impl->key = NULL;
    }
    ctx_impl->key = key;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_id(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *id
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, id, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->id)
    {
        AXIS2_FREE(env->allocator, ctx_impl->id);
        ctx_impl->id = NULL;
    }
    ctx_impl->id = axis2_strdup(id, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_type(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *type
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->type)
    {
        AXIS2_FREE(env->allocator, ctx_impl->type);
        ctx_impl->type = NULL;
    }
    ctx_impl->type = axis2_strdup(type, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_mime_type(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *mime_type
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, mime_type, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->mime_type)
    {
        AXIS2_FREE(env->allocator, ctx_impl->mime_type);
        ctx_impl->mime_type = NULL;
    }
    ctx_impl->mime_type = axis2_strdup(mime_type, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_encoding(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *encoding
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, encoding, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->encoding)
    {
        AXIS2_FREE(env->allocator, ctx_impl->encoding);
        ctx_impl->encoding = NULL;
    }
    ctx_impl->encoding = axis2_strdup(encoding, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_recipient(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *recipient
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, recipient, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->recipient)
    {
        AXIS2_FREE(env->allocator, ctx_impl->recipient);
        ctx_impl->recipient = NULL;
    }
    ctx_impl->recipient = axis2_strdup(recipient, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_carried_key_name(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *carried_key_name
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, carried_key_name, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->carried_key_name)
    {
        AXIS2_FREE(env->allocator, ctx_impl->carried_key_name);
        ctx_impl->carried_key_name = NULL;
    }
    ctx_impl->carried_key_name = axis2_strdup(carried_key_name, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_enc_mtd_algorithm(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *enc_mtd_algorithm
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, enc_mtd_algorithm, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->enc_mtd_algorithm)
    {
        AXIS2_FREE(env->allocator, ctx_impl->enc_mtd_algorithm);
        ctx_impl->enc_mtd_algorithm = NULL;
    }
    ctx_impl->enc_mtd_algorithm = axis2_strdup(enc_mtd_algorithm, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_input_data(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axis2_char_t *input_data
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, input_data, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->input_data)
    {
        AXIS2_FREE(env->allocator, ctx_impl->input_data);
        ctx_impl->input_data = NULL;
    }
    ctx_impl->input_data = axis2_strdup(input_data, env) ;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_enc_data_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axiom_node_t *enc_data_node
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->enc_data_node)
    {
        AXIS2_FREE(env->allocator, ctx_impl->enc_data_node);
        ctx_impl->enc_data_node = NULL;
    }
    ctx_impl->enc_data_node = enc_data_node ;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_enc_method_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axiom_node_t *enc_method_node
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->enc_method_node)
    {
        AXIS2_FREE(env->allocator, ctx_impl->enc_method_node);
        ctx_impl->enc_method_node = NULL;
    }
    ctx_impl->enc_method_node = enc_method_node ;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_key_info_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axiom_node_t *key_info_node
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->key_info_node)
    {
        AXIS2_FREE(env->allocator, ctx_impl->key_info_node);
        ctx_impl->key_info_node = NULL;
    }
    ctx_impl->key_info_node = key_info_node ;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_ctx_set_cipher_value_node(
    oxs_ctx_t *ctx,
    const axis2_env_t *env,
    axiom_node_t *cipher_value_node
)
{
    oxs_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->cipher_value_node)
    {
        AXIS2_FREE(env->allocator, ctx_impl->cipher_value_node);
        ctx_impl->cipher_value_node = NULL;
    }
    ctx_impl->cipher_value_node = cipher_value_node ;

    return AXIS2_SUCCESS;
}





