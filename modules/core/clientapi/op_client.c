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

#include <axis2_op_client.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_mep_client.h>
#include <axis2_uuid_gen.h>
#include <axis2_listener_manager.h>
#include <axis2_engine.h>
#include "axis2_callback_recv.h"
#include <axiom_xml_reader.h>
#include <axis2_core_utils.h>

struct axis2_op_client
{
    axis2_mep_client_t *base;

    axis2_svc_ctx_t *svc_ctx;

    axis2_options_t *options;

    axis2_op_ctx_t *op_ctx;

    axis2_callback_t *callback;

    axis2_bool_t completed;
    /* to hold the locally created async result */
    axis2_async_result_t *async_result;
    axis2_callback_recv_t *callback_recv;
};


typedef struct axis2_op_client_worker_func_args
{
    const axis2_env_t *env;
    axis2_op_client_t *op_client;
    axis2_callback_t *callback;
    axis2_op_t *op;
    axis2_msg_ctx_t *msg_ctx;
}
axis2_op_client_worker_func_args_t;

void *AXIS2_THREAD_FUNC
axis2_op_client_worker_func(
    axis2_thread_t *thd,
    void *data);

AXIS2_EXTERN axis2_op_client_t *AXIS2_CALL
axis2_op_client_create(
    const axis2_env_t *env, axis2_op_t *op,
    axis2_svc_ctx_t *svc_ctx,
    axis2_options_t *options)
{
    axis2_op_client_t *op_client = NULL;
    const axis2_char_t *mep_uri = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, op, NULL);
    AXIS2_PARAM_CHECK(env->error, svc_ctx, NULL);
    AXIS2_PARAM_CHECK(env->error, options, NULL);

    op_client = AXIS2_MALLOC(env->allocator, sizeof(axis2_op_client_t));
    if (!op_client)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /** initialize data */
    op_client->base = NULL;
    op_client->svc_ctx = NULL;
    op_client->options = NULL;
    op_client->op_ctx = NULL;
    op_client->callback = NULL;
    op_client->completed = AXIS2_FALSE;
    op_client->async_result = NULL;
    op_client->callback_recv = NULL;

    op_client->options = options;
    op_client->svc_ctx = svc_ctx;

    op_client->op_ctx = axis2_op_ctx_create(env, op,
            op_client->svc_ctx);
    if (!(op_client->op_ctx))
    {
        axis2_op_client_free(op_client, env);
        return NULL;
    }

    mep_uri = axis2_op_get_msg_exchange_pattern(op, env);

    if (!mep_uri)
    {
        axis2_op_client_free(op_client, env);
        return NULL;
    }

    op_client->base = axis2_mep_client_create(env, svc_ctx, mep_uri);
    if (!(op_client->base))
    {
        axis2_op_client_free(op_client, env);
        return NULL;
    }

    /** initialize parser for thread safety */
    axiom_xml_reader_init();
    return op_client;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_client_set_options(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    const axis2_options_t *options)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (op_client->options)
    {
        AXIS2_OPTIONS_FREE(op_client->options, env);
    }
    op_client->options = (axis2_options_t *)options;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_options_t *AXIS2_CALL
axis2_op_client_get_options(
    const axis2_op_client_t *op_client,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return op_client->options;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_client_add_msg_ctx(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_msg_ctx_t *mc)
{
    axis2_msg_ctx_t *out_msg_ctx = NULL, *in_msg_ctx = NULL;
    axis2_msg_ctx_t **msg_ctx_map = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    msg_ctx_map =  axis2_op_ctx_get_msg_ctx_map(op_client->op_ctx, env);

    out_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT];
    in_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_IN];

    if (out_msg_ctx && in_msg_ctx)
    {
        /* may be this is the second invocation using the same service clinet,
           so reset */
        axis2_msg_ctx_free(out_msg_ctx, env);
        out_msg_ctx = NULL;
        msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT] = NULL;
        axis2_msg_ctx_free(in_msg_ctx, env);
        in_msg_ctx = NULL;
        msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_IN] = NULL;
        axis2_op_ctx_set_complete(op_client->op_ctx, env, AXIS2_FALSE);
    }

    if (!out_msg_ctx)
    {
        msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT] = mc;
    }
    else
    {
        msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_IN] = mc;
        axis2_op_ctx_set_complete(op_client->op_ctx, env, AXIS2_TRUE);
    }

    if (out_msg_ctx && !mc)
    {
		axis2_property_t *dump_property;
		axis2_char_t *dump_value = NULL;
		if (! axis2_msg_ctx_get_doing_rest(out_msg_ctx, env))
		{
			dump_property =  axis2_msg_ctx_get_property(out_msg_ctx, env,
													   AXIS2_DUMP_INPUT_MSG_TRUE,
													   AXIS2_FALSE);
			if (dump_property)
				dump_value = (axis2_char_t *) axis2_property_get_value (
					dump_property, env);
		}
		
		if(axis2_strcmp(dump_value, AXIS2_VALUE_TRUE))
        {
			 axis2_msg_ctx_free(out_msg_ctx, env);
			out_msg_ctx = NULL;
			msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT] = NULL;
        }
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_client_add_out_msg_ctx(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_msg_ctx_t *mc)
{
    axis2_msg_ctx_t **msg_ctx_map = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    msg_ctx_map =  axis2_op_ctx_get_msg_ctx_map(op_client->op_ctx, env);

	msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT] = mc;

    return AXIS2_SUCCESS;
}



AXIS2_EXTERN const axis2_msg_ctx_t *AXIS2_CALL
axis2_op_client_get_msg_ctx(
    const axis2_op_client_t *op_client,
    const axis2_env_t *env,
    const axis2_wsdl_msg_labels_t message_label)
{
    return  axis2_op_ctx_get_msg_ctx(op_client->op_ctx, env, message_label);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_client_set_callback(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_callback_t *callback)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (op_client->callback)
    {
        axis2_callback_free(op_client->callback, env);
    }

    op_client->callback = callback;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_callback_t *AXIS2_CALL
axis2_op_client_get_callback(
    axis2_op_client_t *op_client,
    const axis2_env_t *env)
{
    return op_client->callback;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_client_execute(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    const axis2_bool_t block)
{
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;

    axis2_transport_out_desc_t *transport_out = NULL;
    axis2_transport_in_desc_t *transport_in = NULL;

    axis2_status_t status = AXIS2_FAILURE;
    axis2_op_t *op = NULL;
    axis2_char_t *msg_id = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (op_client->completed)
    {
        /**TODO:error completed*/
        return AXIS2_FAILURE;
    }

    conf_ctx =  axis2_svc_ctx_get_conf_ctx(op_client->svc_ctx, env);

    msg_ctx = (axis2_msg_ctx_t *)axis2_op_client_get_msg_ctx(op_client, env,
            AXIS2_WSDL_MESSAGE_LABEL_OUT);

    if (!msg_ctx)
    {
        /*TODO:error msg ctx is null*/
        return AXIS2_FAILURE;
    }

     axis2_msg_ctx_set_options(msg_ctx, env, op_client->options);

    /**
     if the transport to use for sending is not specified, try to find it
     from the URL
    */
    transport_out = AXIS2_OPTIONS_GET_TRANSPORT_OUT(op_client->options,
            env);
    if (!transport_out)
    {
        axis2_endpoint_ref_t *to_epr = NULL;
        axis2_property_t *property = NULL;
         property = AXIS2_OPTIONS_GET_PROPERTY(op_client->options, env, 
                 AXIS2_TARGET_EPR);
         if(property)
            to_epr = axis2_property_get_value(property, env);
         if(!to_epr)
            to_epr = AXIS2_OPTIONS_GET_TO(op_client->options, env);
        if (!to_epr)
            to_epr =  axis2_msg_ctx_get_to(msg_ctx, env);
        transport_out = AXIS2_MEP_CLIENT_INFER_TRANSPORT(op_client->base, env, to_epr);
    }

    if (!transport_out)
    {
        return AXIS2_FAILURE;
    }

    if (!( axis2_msg_ctx_get_transport_out_desc(msg_ctx, env)))
         axis2_msg_ctx_set_transport_out_desc(msg_ctx, env, transport_out);


    transport_in = AXIS2_OPTIONS_GET_TRANSPORT_IN(op_client->options, env);
    if (!transport_in)
    {
        axis2_conf_ctx_t *conf_ctx =  axis2_svc_ctx_get_conf_ctx(op_client->svc_ctx, env);

        if (conf_ctx)
        {
            axis2_conf_t *conf =  axis2_conf_ctx_get_conf(conf_ctx, env);
            if (conf)
            {
                transport_in =  axis2_conf_get_transport_in(conf, env,
                        axis2_transport_out_desc_get_enum(transport_out, env));
            }
        }
    }

    if (!( axis2_msg_ctx_get_transport_in_desc(msg_ctx, env)))
    {
         axis2_msg_ctx_set_transport_in_desc(msg_ctx, env, transport_in);
    }

    op =  axis2_op_ctx_get_op(op_client->op_ctx, env);

    if (!op)
        return AXIS2_FAILURE;

    status = AXIS2_MEP_CLIENT_PREPARE_INVOCATION(op_client->base, env, op, msg_ctx);
    if (status != AXIS2_SUCCESS)
        return AXIS2_FAILURE;

    msg_id = (axis2_char_t*)axis2_uuid_gen(env);
     axis2_msg_ctx_set_message_id(msg_ctx, env, msg_id);
    if (msg_id)
    {
        AXIS2_FREE(env->allocator, msg_id);
        msg_id = NULL;
    }

    if (AXIS2_OPTIONS_GET_USE_SEPERATE_LISTENER(op_client->options, env))
    {
        axis2_engine_t *engine = NULL;

        AXIS2_CALLBACK_RECV_ADD_CALLBACK(op_client->callback_recv, env,
                 axis2_msg_ctx_get_msg_id(msg_ctx, env),
                op_client->callback);
        /* TODO: set up reply to */
         axis2_msg_ctx_set_op_ctx(msg_ctx, env, axis2_op_find_op_ctx(op, env,
                msg_ctx, op_client->svc_ctx));
         axis2_msg_ctx_set_svc_ctx(msg_ctx, env, op_client->svc_ctx);

        /* send the message */
        engine = axis2_engine_create(env, conf_ctx);
        if (!engine)
            return AXIS2_FAILURE;
        axis2_engine_send(engine, env, msg_ctx);
         axis2_engine_free(engine, env);
    }
    else
    {
        if (block)
        {
            axis2_msg_ctx_t *response_mc = NULL;
            
            axis2_msg_ctx_set_svc_ctx(msg_ctx, env, op_client->svc_ctx);
            axis2_msg_ctx_set_conf_ctx(msg_ctx, env,
                     axis2_svc_ctx_get_conf_ctx(op_client->svc_ctx, env));
            axis2_msg_ctx_set_op_ctx(msg_ctx, env, op_client->op_ctx);


            /*Send the SOAP Message and receive a response */
            response_mc = axis2_mep_client_two_way_send(env, msg_ctx);
            if (!response_mc)
            {
                const axis2_char_t *mep = axis2_op_get_msg_exchange_pattern(op, env);
                if (axis2_strcmp(mep, AXIS2_MEP_URI_OUT_ONLY) == 0 ||
                        axis2_strcmp(mep, AXIS2_MEP_URI_ROBUST_OUT_ONLY) == 0)
                {
                    if (env->error)
                        return env->error->status_code;
                    else
                        return AXIS2_FAILURE;
                }
                else
                    return AXIS2_FAILURE;
            }
            axis2_op_client_add_msg_ctx(op_client, env,
                    response_mc);
        }
        else
        {
            axis2_thread_t *worker_thread = NULL;
            axis2_op_client_worker_func_args_t *arg_list = NULL;
            arg_list = AXIS2_MALLOC(env->allocator,
                    sizeof(axis2_op_client_worker_func_args_t));
            if (! arg_list)
            {
                return AXIS2_FAILURE;
            }
            arg_list->env = env;
            arg_list->op_client = op_client;
            arg_list->callback = op_client->callback;
            arg_list->op = op;
            arg_list->msg_ctx = msg_ctx;
#ifdef AXIS2_SVR_MULTI_THREADED
            if (env->thread_pool)
            {
                worker_thread = AXIS2_THREAD_POOL_GET_THREAD(env->thread_pool,
                        axis2_op_client_worker_func, (void*)arg_list);
                if (! worker_thread)
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

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_client_reset(
    axis2_op_client_t *op_client,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (!op_client->completed)
        return AXIS2_FAILURE;

    op_client->completed = AXIS2_FALSE;

    op_client->op_ctx = NULL;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_client_complete(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_msg_ctx_t *mc)
{
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_listener_manager_t *listener_manager = NULL;
    AXIS2_TRANSPORT_ENUMS transport = AXIS2_TRANSPORT_ENUM_HTTP;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    conf_ctx =  axis2_msg_ctx_get_conf_ctx(mc, env);

    if (!conf_ctx)
        return AXIS2_FAILURE;

    /*TODO:uncomment when implemented
    listener_manager = AXIS2_CONF_CTX_GET_LISTENER_MANAGER(conf_ctx, env);
    */
    if (!listener_manager)
        return AXIS2_FAILURE;

    return axis2_listener_manager_stop(listener_manager, env, transport);
}

AXIS2_EXTERN axis2_op_ctx_t *AXIS2_CALL
axis2_op_client_get_operation_context(
    const axis2_op_client_t *op_client,
    const axis2_env_t *env)
{
    return op_client->op_ctx;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_client_free(
    axis2_op_client_t *op_client,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (op_client->base)
    {
        AXIS2_MEP_CLIENT_FREE(op_client->base, env);
    }

    if (op_client->callback)
    {
        axis2_callback_free(op_client->callback, env);
    }

    if (op_client->op_ctx)
    {
         axis2_op_ctx_free(op_client->op_ctx, env);
         op_client->op_ctx = NULL;
    }

	if (AXIS2_OPTIONS_GET_XML_PARSER_RESET(op_client->options, env))
	{
		axiom_xml_reader_cleanup(); 
	}

    AXIS2_FREE(env->allocator, op_client);

    return AXIS2_SUCCESS;
}


void *AXIS2_THREAD_FUNC
axis2_op_client_worker_func(
    axis2_thread_t *thd,
    void *data)
{
    axis2_op_client_worker_func_args_t *args_list = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_msg_ctx_t *response = NULL;
    axis2_env_t *th_env = NULL;
    axis2_thread_pool_t *th_pool = NULL;

    args_list = (axis2_op_client_worker_func_args_t *) data;
    if (!args_list)
        return NULL;

    AXIS2_ENV_CHECK(args_list->env, AXIS2_FAILURE);
    th_env = axis2_init_thread_env(args_list->env);

    op_ctx = axis2_op_ctx_create(th_env, args_list->op, args_list->op_client->svc_ctx);
    if (!op_ctx)
        return NULL;
     axis2_msg_ctx_set_op_ctx(args_list->msg_ctx, th_env, op_ctx);
     axis2_msg_ctx_set_svc_ctx(args_list->msg_ctx, th_env, args_list->op_client->svc_ctx);

    /* send the request and wait for response */
    response = axis2_mep_client_two_way_send(th_env, args_list->msg_ctx);

    axis2_op_client_add_msg_ctx(args_list->op_client, th_env,
        response);
    args_list->op_client->async_result = axis2_async_result_create(th_env, response);
    axis2_callback_invoke_on_complete(args_list->callback, th_env, args_list->op_client->async_result);
    axis2_callback_set_complete(args_list->callback, th_env, AXIS2_TRUE);

    /* clean up memory */
    axis2_async_result_free(args_list->op_client->async_result, th_env);
    
     axis2_op_ctx_free(op_ctx, th_env);
        
    th_pool = th_env->thread_pool;
    
    AXIS2_FREE(th_env->allocator, args_list);
    
    if (th_env)
    {
        axis2_free_thread_env(th_env);
        th_env = NULL;
    }
    AXIS2_THREAD_POOL_EXIT_THREAD(th_pool, thd);
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_op_client_set_callback_recv(
    axis2_op_client_t *op_client,
    const axis2_env_t *env,
    axis2_callback_recv_t *callback_recv)
{
    op_client->callback_recv = callback_recv;
    return AXIS2_SUCCESS;
}

