/*
* Copyright 2004,2005 The Apache Software Foundation.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <axis2_op_client.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_mep_client.h>
#include <axis2_uuid_gen.h>
#include <axis2_listener_manager.h>
#include <axis2_engine.h>
#include "callback_recv.h"
#include <axiom_xml_reader.h>

typedef struct axis2_op_client_impl
{
    /** op_client base struct */
    axis2_op_client_t op_client;

    axis2_mep_client_t *base;

    axis2_svc_ctx_t *svc_ctx;

    axis2_options_t *options;

    axis2_op_ctx_t *op_ctx;

    axis2_callback_t *callback;

    axis2_bool_t completed;
    /* to hold the locally created async result */
    axis2_async_result_t *async_result;
    axis2_callback_recv_t *callback_recv;
}
axis2_op_client_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(op_client) ((axis2_op_client_impl_t *)op_client)

typedef struct axis2_op_client_worker_func_args
{
    const axis2_env_t *env;
    axis2_op_client_impl_t *op_client_impl;
    axis2_callback_t *callback;
    axis2_op_t *op;
    axis2_msg_ctx_t *msg_ctx;
} axis2_op_client_worker_func_args_t;

void *AXIS2_THREAD_FUNC
axis2_op_client_worker_func(
    axis2_thread_t *thd, 
    void *data);


/** private function prototypes */
static void 
axis2_op_client_init_ops(
    axis2_op_client_t *op_client);

/** public function prototypes */
axis2_status_t AXIS2_CALL
axis2_op_client_set_options(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    const axis2_options_t *options);

const axis2_options_t *AXIS2_CALL
axis2_op_client_get_options(
    const axis2_op_client_t *op_client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_op_client_add_msg_ctx(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_msg_ctx_t *mc);


const axis2_msg_ctx_t *AXIS2_CALL
axis2_op_client_get_msg_ctx(
    const axis2_op_client_t *op_client,
    const axis2_env_t *env,
    const axis2_char_t *message_label);

axis2_status_t AXIS2_CALL
axis2_op_client_set_callback(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_callback_t *callback);

axis2_status_t AXIS2_CALL
axis2_op_client_execute(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    const axis2_bool_t block);

axis2_status_t AXIS2_CALL
axis2_op_client_reset(
    axis2_op_client_t *op_client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_op_client_complete(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_msg_ctx_t *mc);

axis2_op_ctx_t *AXIS2_CALL
axis2_op_client_get_operation_context(
    const axis2_op_client_t *op_client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_op_client_set_callback_recv(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_callback_recv_t *callback_recv);

axis2_status_t AXIS2_CALL
axis2_op_client_free(
    axis2_op_client_t *op_client,
    const axis2_env_t *env);

axis2_op_client_t *AXIS2_CALL
axis2_op_client_create(
    const axis2_env_t *env, axis2_op_t *op,
    axis2_svc_ctx_t *svc_ctx,
    axis2_options_t *options)
{
    axis2_op_client_impl_t *op_client_impl = NULL;
    axis2_char_t *mep_uri = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, op, NULL);
    AXIS2_PARAM_CHECK(env->error, svc_ctx, NULL);
    AXIS2_PARAM_CHECK(env->error, options, NULL);

    op_client_impl = AXIS2_MALLOC( env->allocator, sizeof(axis2_op_client_impl_t) );
    if (!op_client_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /** initialize data */
    op_client_impl->base = NULL;
    op_client_impl->svc_ctx = NULL;
    op_client_impl->options = NULL;
    op_client_impl->op_ctx = NULL;
    op_client_impl->callback = NULL;
    op_client_impl->completed = AXIS2_FALSE;
    op_client_impl->async_result = NULL;
    op_client_impl->callback_recv = NULL;

    op_client_impl->options = options;
    op_client_impl->svc_ctx = svc_ctx;

    op_client_impl->op_ctx = axis2_op_ctx_create(env, op,
            op_client_impl->svc_ctx);
    if (!(op_client_impl->op_ctx))
    {
        axis2_op_client_free(&(op_client_impl->op_client), env);
        return NULL;
    }

    mep_uri = AXIS2_OP_GET_MSG_EXCHANGE_PATTERN(op, env);

    if (!mep_uri)
    {
        axis2_op_client_free(&(op_client_impl->op_client), env);
        return NULL;
    }

    op_client_impl->base = axis2_mep_client_create(env, svc_ctx, mep_uri);
    if (!(op_client_impl->base))
    {
        axis2_op_client_free(&(op_client_impl->op_client), env);
        return NULL;
    }

    /* initialize ops */
    op_client_impl->op_client.ops =
        AXIS2_MALLOC( env->allocator, sizeof(axis2_op_client_ops_t) );

    if (!op_client_impl->op_client.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_CALLBACK_FREE(&(op_client_impl->op_client), env);
        return NULL;
    }

    axis2_op_client_init_ops(&(op_client_impl->op_client));
    /** initialize parser for thread safety */
    axiom_xml_reader_init();
    return &(op_client_impl->op_client);
}


axis2_status_t AXIS2_CALL
axis2_op_client_set_options(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    const axis2_options_t *options)
{
    axis2_op_client_impl_t *op_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

    if (op_client_impl->options)
    {
        AXIS2_OPTIONS_FREE(op_client_impl->options, env);
    }
    op_client_impl->options = (axis2_options_t *)options;

    return AXIS2_SUCCESS;
}

const axis2_options_t *AXIS2_CALL
axis2_op_client_get_options(
    const axis2_op_client_t *op_client,
    const axis2_env_t *env)
{
    axis2_op_client_impl_t *op_client_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

    return op_client_impl->options;
}

axis2_status_t AXIS2_CALL
axis2_op_client_add_msg_ctx(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_msg_ctx_t *mc)
{
    axis2_op_client_impl_t *op_client_impl = NULL;
    axis2_msg_ctx_t *out_msg_ctx = NULL, *in_msg_ctx = NULL;
    axis2_hash_t *msg_ctx_map = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

    msg_ctx_map = AXIS2_OP_CTX_GET_MSG_CTX_MAP(op_client_impl->op_ctx, env);

    out_msg_ctx = axis2_hash_get(msg_ctx_map, AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE,
            AXIS2_HASH_KEY_STRING);
    in_msg_ctx = axis2_hash_get(msg_ctx_map, AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE,
            AXIS2_HASH_KEY_STRING);

    if (out_msg_ctx && in_msg_ctx)
    {
        /*TODO:error - completed*/
        return AXIS2_FAILURE;
    }

    if (!out_msg_ctx)
    {
        axis2_hash_set(msg_ctx_map, AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE, AXIS2_HASH_KEY_STRING, mc);
    }
    else
    {
        axis2_hash_set(msg_ctx_map, AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE, AXIS2_HASH_KEY_STRING, mc);
        AXIS2_OP_CTX_SET_IS_COMPLETE(op_client_impl->op_ctx, env, AXIS2_TRUE);
    }
    return AXIS2_SUCCESS;
}


const axis2_msg_ctx_t *AXIS2_CALL
axis2_op_client_get_msg_ctx(
    const axis2_op_client_t *op_client,
    const axis2_env_t *env,
    const axis2_char_t *message_label)
{

    axis2_op_client_impl_t *op_client_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

    if (!message_label)
    {
        return NULL;
    }
    return AXIS2_OP_CTX_GET_MSG_CTX(op_client_impl->op_ctx, env, message_label);
}

axis2_status_t AXIS2_CALL
axis2_op_client_set_callback(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_callback_t *callback)
{
    axis2_op_client_impl_t *op_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

    if (op_client_impl->callback)
    {
        AXIS2_CALLBACK_FREE(op_client_impl->callback, env);
    }

    op_client_impl->callback = callback;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_op_client_execute(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    const axis2_bool_t block)
{
    axis2_op_client_impl_t *op_client_impl = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;

    axis2_transport_out_desc_t *transport_out = NULL;
    axis2_transport_in_desc_t *transport_in = NULL;

    axis2_status_t status = AXIS2_FAILURE;
    axis2_op_t *op = NULL;
    axis2_char_t *msg_id = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

    if (op_client_impl->completed)
    {
        /**TODO:error completed*/
        return AXIS2_FAILURE;
    }

    conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(op_client_impl->svc_ctx, env);
    msg_ctx = (axis2_msg_ctx_t *)axis2_op_client_get_msg_ctx(op_client, env,
            AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE);

    if (!msg_ctx)
    {
        /*TODO:error msg ctx is null*/
        return AXIS2_FAILURE;
    }

    AXIS2_MSG_CTX_SET_OPTIONS(msg_ctx, env, op_client_impl->options);

    /**
     if the transport to use for sending is not specified, try to find it
     from the URL
    */
    transport_out = AXIS2_OPTIONS_GET_TRANSPORT_OUT(op_client_impl->options,
            env);
    if (!transport_out)
    {
        axis2_endpoint_ref_t *to_epr = NULL;
        to_epr = AXIS2_OPTIONS_GET_TO(op_client_impl->options, env);
        if (!to_epr)
            to_epr = AXIS2_MSG_CTX_GET_TO(msg_ctx, env);
        transport_out = AXIS2_MEP_CLIENT_INFER_TRANSPORT(op_client_impl->base, env, to_epr);
    }

    if (!transport_out)
    {
        return AXIS2_FAILURE;
    }

    if (!(AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(msg_ctx, env)))
        AXIS2_MSG_CTX_SET_TRANSPORT_OUT_DESC(msg_ctx, env, transport_out);


    transport_in = AXIS2_OPTIONS_GET_TRANSPORT_IN(op_client_impl->options, env);
    if (!transport_in)
    {
        axis2_conf_ctx_t *conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(op_client_impl->svc_ctx, env);
        if (conf_ctx)
        {
            axis2_conf_t *conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
            if (conf)
            {
                transport_in = AXIS2_CONF_GET_TRANSPORT_IN(conf, env,
                        AXIS2_TRANSPORT_OUT_DESC_GET_QNAME(transport_out, env));
            }
        }
    }

    if (!(AXIS2_MSG_CTX_GET_TRANSPORT_IN_DESC(msg_ctx, env)))
    {
        AXIS2_MSG_CTX_SET_TRANSPORT_IN_DESC(msg_ctx, env, transport_in);
    }

    op = AXIS2_OP_CTX_GET_OP(op_client_impl->op_ctx, env);
    if (!op)
        return AXIS2_FAILURE;

    status = AXIS2_MEP_CLIENT_PREPARE_INVOCATION(op_client_impl->base, env, op, msg_ctx);
    if (status != AXIS2_SUCCESS)
        return AXIS2_FAILURE;

    msg_id = (axis2_char_t*)axis2_uuid_gen(env);
    AXIS2_MSG_CTX_SET_MESSAGE_ID(msg_ctx, env, msg_id);
    if (NULL != msg_id)
    {
        AXIS2_FREE(env->allocator, msg_id);
        msg_id = NULL;
    }

    if (AXIS2_OPTIONS_GET_USE_SEPERATE_LISTENER(op_client_impl->options, env))
    {
        axis2_engine_t *engine = NULL;

        AXIS2_CALLBACK_RECV_ADD_CALLBACK(op_client_impl->callback_recv, env,
                AXIS2_MSG_CTX_GET_MSG_ID(msg_ctx, env),
                op_client_impl->callback);
        /* TODO: set up reply to */
        AXIS2_MSG_CTX_SET_OP_CTX(msg_ctx, env, AXIS2_OP_FIND_OP_CTX(op, env,
                msg_ctx, op_client_impl->svc_ctx));
        AXIS2_MSG_CTX_SET_SVC_CTX(msg_ctx, env, op_client_impl->svc_ctx);

        /* send the message */
        engine = axis2_engine_create(env, conf_ctx);
        if (!engine)
            return AXIS2_FAILURE;
        AXIS2_ENGINE_SEND(engine, env, msg_ctx);
    }
    else
    {
        if (block)
        {
            axis2_msg_ctx_t *response_mc = NULL;
            axis2_char_t *address = NULL;
            axis2_char_t *epr_address = NULL;
            axis2_property_t *property = NULL;

            /* Usual Request-Response Sync implementation */
            property = axis2_property_create(env);
            AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
            epr_address = AXIS2_ENDPOINT_REF_GET_ADDRESS(
                        AXIS2_OPTIONS_GET_TO(op_client_impl->options, env), env);
            address = AXIS2_STRDUP(epr_address, env);
            AXIS2_PROPERTY_SET_VALUE(property, env, address);
            AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env,
                    AXIS2_TRANSPORT_URL, property, AXIS2_FALSE);
            AXIS2_MSG_CTX_SET_SVC_CTX(msg_ctx, env, op_client_impl->svc_ctx);
            AXIS2_MSG_CTX_SET_CONF_CTX(msg_ctx, env,
                    AXIS2_SVC_CTX_GET_CONF_CTX(op_client_impl->svc_ctx, env));
            AXIS2_MSG_CTX_SET_OP_CTX(msg_ctx, env, op_client_impl->op_ctx);

            /*Send the SOAP Message and receive a response */
            response_mc = axis2_mep_client_two_way_send(env, msg_ctx);
            if (!response_mc)
            {
                axis2_char_t *mep = AXIS2_OP_GET_MSG_EXCHANGE_PATTERN(op, env);
                if (AXIS2_STRCMP(mep, AXIS2_MEP_URI_OUT_ONLY) == 0 ||
                        AXIS2_STRCMP(mep, AXIS2_MEP_URI_ROBUST_OUT_ONLY) == 0)
                {
                    return AXIS2_SUCCESS;
                }
                else
                    return AXIS2_FAILURE;
            }
            axis2_op_client_add_msg_ctx(&(op_client_impl->op_client), env,
                    response_mc);
        }
        else
        {
            axis2_thread_t *worker_thread = NULL;
            axis2_op_client_worker_func_args_t *arg_list = NULL;
            arg_list = AXIS2_MALLOC(env->allocator,
                    sizeof(axis2_op_client_worker_func_args_t));
            if (NULL == arg_list)
            {
                return AXIS2_FAILURE;
            }
            arg_list->env = env;
            arg_list->op_client_impl = op_client_impl;
            arg_list->callback = op_client_impl->callback;
            arg_list->op = op;
            arg_list->msg_ctx = msg_ctx;
#ifdef AXIS2_SVR_MULTI_THREADED
            if (env->thread_pool)
            {
                worker_thread = AXIS2_THREAD_POOL_GET_THREAD(env->thread_pool,
                        axis2_op_client_worker_func, (void*)arg_list);
                if (NULL == worker_thread)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Thread creation failed"
                            "call invoke non blocking");
                }
                AXIS2_THREAD_POOL_THREAD_DETACH(env->thread_pool, worker_thread);
            }
            else
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Thread pool not set in environment."
                        " Cannot invoke call non blocking");
            }
#else
            axis2_op_client_worker_func(NULL, (void*)arg_list);
#endif

        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_op_client_reset(
    axis2_op_client_t *op_client,
    const axis2_env_t *env)
{
    axis2_op_client_impl_t *op_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

    if (!op_client_impl->completed)
        return AXIS2_FAILURE;

    op_client_impl->completed = AXIS2_FALSE;

    /*TODO:check
    if (op_client_impl->op_ctx)
       AXIS2_OP_CTX_FREE(op_client_impl->op_ctx, env);
    */
    op_client_impl->op_ctx = NULL;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_op_client_complete(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_msg_ctx_t *mc)
{
    axis2_op_client_impl_t *op_client_impl = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_listener_manager_t *listener_manager = NULL;
    axis2_char_t *transport = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(mc, env);

    if (!conf_ctx)
        return AXIS2_FAILURE;

    /*TODO:uncomment when implemented
    listener_manager = AXIS2_CONF_CTX_GET_LISTENER_MANAGER(conf_ctx, env);
    */
    if (!listener_manager)
        return AXIS2_FAILURE;

    return AXIS2_LISTENER_MANAGER_STOP(listener_manager, env, transport);
}

axis2_op_ctx_t *AXIS2_CALL
axis2_op_client_get_operation_context(
    const axis2_op_client_t *op_client,
    const axis2_env_t *env)
{
    axis2_op_client_impl_t *op_client_impl = NULL;

    op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

    return op_client_impl->op_ctx;
}

axis2_status_t AXIS2_CALL
axis2_op_client_free(
    axis2_op_client_t *op_client,
    const axis2_env_t *env)
{
    axis2_op_client_impl_t *op_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

    if (op_client_impl->base)
    {
        AXIS2_MEP_CLIENT_FREE(op_client_impl->base, env);
        op_client_impl->base = NULL;
    }

    if (op_client_impl->callback)
    {
        AXIS2_CALLBACK_FREE(op_client_impl->callback, env);
        op_client_impl->callback = NULL;
    }

    if (op_client_impl->op_client.ops)
    {
        AXIS2_FREE(env->allocator, op_client_impl->op_client.ops);
        op_client_impl->op_client.ops = NULL;
    }

    AXIS2_FREE(env->allocator, op_client_impl);
    op_client_impl = NULL;
    axiom_xml_reader_cleanup();
    return AXIS2_SUCCESS;
}


static void 
axis2_op_client_init_ops(
    axis2_op_client_t *op_client)
{
    op_client->ops->set_options = axis2_op_client_set_options;
    op_client->ops->get_options = axis2_op_client_get_options;
    op_client->ops->add_msg_ctx = axis2_op_client_add_msg_ctx;
    op_client->ops->get_msg_ctx = axis2_op_client_get_msg_ctx;
    op_client->ops->set_callback = axis2_op_client_set_callback;
    op_client->ops->execute = axis2_op_client_execute;
    op_client->ops->reset = axis2_op_client_reset;
    op_client->ops->complete = axis2_op_client_complete;
    op_client->ops->get_operation_context = axis2_op_client_get_operation_context;
    op_client->ops->set_callback_recv = axis2_op_client_set_callback_recv;
    op_client->ops->free = axis2_op_client_free;
}

void *AXIS2_THREAD_FUNC
axis2_op_client_worker_func(
    axis2_thread_t *thd, 
    void *data)
{
    axis2_op_client_worker_func_args_t *args_list = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_msg_ctx_t *response = NULL;
    const axis2_env_t *th_env = NULL;

    args_list = (axis2_op_client_worker_func_args_t *) data;
    if (!args_list)
        return NULL;

    AXIS2_ENV_CHECK(args_list->env, AXIS2_FAILURE);
    th_env = axis2_init_thread_env(args_list->env);

    op_ctx = axis2_op_ctx_create(th_env, args_list->op, args_list->op_client_impl->svc_ctx);
    if (!op_ctx)
        return NULL;
    AXIS2_MSG_CTX_SET_OP_CTX(args_list->msg_ctx, th_env, op_ctx);
    AXIS2_MSG_CTX_SET_SVC_CTX(args_list->msg_ctx, th_env, args_list->op_client_impl->svc_ctx);

    /* send the request and wait for response */
    response = axis2_mep_client_two_way_send(th_env, args_list->msg_ctx);
    args_list->op_client_impl->async_result = axis2_async_result_create(th_env, response);
    AXIS2_CALLBACK_INVOKE_ON_COMPLETE(args_list->callback, th_env, args_list->op_client_impl->async_result);
    AXIS2_CALLBACK_SET_COMPLETE(args_list->callback, th_env, AXIS2_TRUE);

    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_op_client_set_callback_recv(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_callback_recv_t *callback_recv)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(op_client)->callback_recv = callback_recv;
    return AXIS2_SUCCESS;
}
