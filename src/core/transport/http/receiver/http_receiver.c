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

#include <axis2_http_server.h>
#include <axis2_http_transport.h>
#include <axis2_http_svr_thread.h>
#include <axis2_transport_in_desc.h>
#include <axutil_param_container.h>
#include <axutil_url.h>
#include <axutil_network_handler.h>
#include <axis2_conf_init.h>
#include <stdlib.h>


/**
 * @brief HTTP Client struct impl
 * Axis2 HTTP Client impl
 */

typedef struct axis2_http_server_impl
{
    axis2_transport_receiver_t http_server;
    axis2_http_svr_thread_t *svr_thread;
    int port;
	axis2_char_t *svr_ip;
    axis2_conf_ctx_t *conf_ctx;
    axis2_conf_ctx_t *conf_ctx_private;
    axis2_bool_t is_application_client_side;
} axis2_http_server_impl_t;

#define AXIS2_INTF_TO_IMPL(http_server) \
    ((axis2_http_server_impl_t *)(http_server))

/***************************** Function headers *******************************/

static axis2_status_t AXIS2_CALL
axis2_http_server_init(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx,
    axis2_transport_in_desc_t * in_desc);

static axis2_status_t AXIS2_CALL
axis2_http_server_start(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env);

axis2_status_t AXIS2_CALL
axis2_http_server_stop(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env);

static axis2_conf_ctx_t *AXIS2_CALL
axis2_http_server_get_conf_ctx(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env);

static axis2_endpoint_ref_t *AXIS2_CALL
axis2_http_server_get_reply_to_epr(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env,
    const axis2_char_t * svc_name);

static axis2_endpoint_ref_t *AXIS2_CALL
axis2_http_server_get_epr_for_service(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env,
    const axis2_char_t * svc_name);

static axis2_bool_t AXIS2_CALL
axis2_http_server_is_running(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env);

static void AXIS2_CALL
axis2_http_server_set_is_application_client_side(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env,
    axis2_bool_t is_application_client_side);

static void AXIS2_CALL
axis2_http_server_free(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env);

static  axis2_char_t* AXIS2_CALL
axis2_http_server_get_server_ip(
axis2_transport_receiver_t *transport_receiver,
const axutil_env_t *env);

static void AXIS2_CALL
axis2_http_server_set_server_ip(
axis2_transport_receiver_t *transport_receiver,
const axutil_env_t *env,
 axis2_char_t *serverip);

static const axis2_transport_receiver_ops_t http_transport_receiver_ops_var = {
    axis2_http_server_init, 
	axis2_http_server_start, 
	axis2_http_server_get_reply_to_epr,
	axis2_http_server_get_epr_for_service, 
	axis2_http_server_get_server_ip, 
	axis2_http_server_set_server_ip,
	axis2_http_server_get_conf_ctx, 
	axis2_http_server_is_running, 
    axis2_http_server_set_is_application_client_side, 
	axis2_http_server_stop,
    axis2_http_server_free };

AXIS2_EXTERN axis2_transport_receiver_t *AXIS2_CALL
axis2_http_server_create(
    const axutil_env_t * env,
    const axis2_char_t * repo,
    const int port)
{
    axis2_http_server_impl_t *server_impl = NULL;

    server_impl = (axis2_http_server_impl_t *)AXIS2_MALLOC(env->allocator,
        sizeof(axis2_http_server_impl_t));

    if(!server_impl)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    server_impl->svr_thread = NULL;
    server_impl->conf_ctx = NULL;
    server_impl->conf_ctx_private = NULL;
    server_impl->port = port;
	server_impl->svr_ip = NULL;
    server_impl->is_application_client_side = AXIS2_FALSE;

    server_impl->http_server.ops = &http_transport_receiver_ops_var;

    if(repo)
    {
		axis2_transport_in_desc_t *transport_in = NULL;
		axis2_conf_t *conf = NULL;
		axis2_transport_receiver_t *receiver = NULL;
        /**
         * We first create a private conf ctx which is owned by this server
         * we only free this private conf context. We should never free the
         * server_impl->conf_ctx because it may own to any other object which
         * may lead to double free
         */
        server_impl->conf_ctx_private = axis2_build_conf_ctx(env, repo);
		
        if(!server_impl->conf_ctx_private)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "unable to create private configuration context for repo path %s", repo);
            axis2_http_server_free((axis2_transport_receiver_t *)server_impl, env);
            return NULL;
        }
		conf = axis2_conf_ctx_get_conf(server_impl->conf_ctx_private, env);
		transport_in = axis2_conf_get_transport_in(conf, env, AXIS2_TRANSPORT_ENUM_HTTP);
		receiver = axis2_transport_in_desc_get_recv(transport_in, env);
		AXIS2_INTF_TO_IMPL(receiver)->port = port;
		
		server_impl->conf_ctx = server_impl->conf_ctx_private;
    }

    return &(server_impl->http_server);
}

AXIS2_EXTERN axis2_transport_receiver_t *AXIS2_CALL
axis2_http_server_create_with_file(
    const axutil_env_t * env,
    const axis2_char_t * file,
    const int port)
{
    axis2_http_server_impl_t *server_impl = NULL;

    server_impl = (axis2_http_server_impl_t *)AXIS2_MALLOC(env->allocator,
        sizeof(axis2_http_server_impl_t));

    if(!server_impl)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    server_impl->svr_thread = NULL;
    server_impl->conf_ctx = NULL;
    server_impl->conf_ctx_private = NULL;
    server_impl->port = port;
	server_impl->svr_ip = NULL;
    server_impl->http_server.ops = &http_transport_receiver_ops_var;

    if(file)
    {
		axis2_transport_in_desc_t *transport_in = NULL;
		axis2_conf_t *conf = NULL;
		axis2_transport_receiver_t *receiver = NULL;
        /**
         * We first create a private conf ctx which is owned by this server
         * we only free this private conf context. We should never free the
         * server_impl->conf_ctx because it may own to any other object which
         * may lead to double free
         */
        server_impl->conf_ctx_private = axis2_build_conf_ctx_with_file(env, file);

        if(!server_impl->conf_ctx_private)
        {
            axis2_http_server_free((axis2_transport_receiver_t *)server_impl, env);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "unable to create configuration context for file %s", file);
            return NULL;
        }
		conf = axis2_conf_ctx_get_conf(server_impl->conf_ctx_private, env);
		transport_in = axis2_conf_get_transport_in(conf, env, AXIS2_TRANSPORT_ENUM_HTTP);
		receiver = axis2_transport_in_desc_get_recv(transport_in, env);
		AXIS2_INTF_TO_IMPL(receiver)->port = port;
		
		
        server_impl->conf_ctx = server_impl->conf_ctx_private;
    }

    return &(server_impl->http_server);
}

static void AXIS2_CALL
axis2_http_server_free(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env)
{
    axis2_http_server_impl_t *server_impl = NULL;

    if(!server)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failure, server value is null , nothing to free");
        return;
    }

    server_impl = AXIS2_INTF_TO_IMPL(server);
    if(server_impl->svr_thread)
    {
        axis2_http_svr_thread_destroy(server_impl->svr_thread, env);
        axis2_http_svr_thread_free(server_impl->svr_thread, env);
        server_impl->svr_thread = NULL;
    }

    if(server_impl->conf_ctx_private)
    {
        axis2_conf_ctx_free(server_impl->conf_ctx_private, env);
        server_impl->conf_ctx_private = NULL;
    }

    /**
     * Do not free this. It may own to some other object
     */
    server_impl->conf_ctx = NULL;

    AXIS2_FREE(env->allocator, server_impl);
}

static axis2_status_t AXIS2_CALL
axis2_http_server_init(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx,
    axis2_transport_in_desc_t * in_desc)
{
    axis2_http_server_impl_t *server_impl = NULL;
    axis2_char_t *port_str = NULL;
    axutil_param_t *param = NULL;

    AXIS2_PARAM_CHECK(env->error, server, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, in_desc, AXIS2_FAILURE);

    server_impl = AXIS2_INTF_TO_IMPL(server);

    server_impl->conf_ctx = conf_ctx;
    param = (axutil_param_t *)axutil_param_container_get_param(
        axis2_transport_in_desc_param_container(in_desc, env), env, AXIS2_PORT_STRING);
    if(param)
    {
        port_str = axutil_param_get_value(param, env);
    }

    if(port_str)
    {
        server_impl->port = atoi(port_str);
    }
    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
axis2_http_server_start(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env)
{
    axis2_http_server_impl_t *server_impl = NULL;
    axis2_http_worker_t *worker = NULL;
	
    server_impl = AXIS2_INTF_TO_IMPL(server);
    server_impl->svr_thread = axis2_http_svr_thread_create(env, server_impl->port);
    if(!server_impl->svr_thread)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "unable to create server thread for port %d",
            server_impl->port);
        return AXIS2_FAILURE;
    }

    worker = axis2_http_worker_create(env, server_impl->conf_ctx);
    if(!worker)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "axis2 http worker creation failed");
        axis2_http_svr_thread_free(server_impl->svr_thread, env);
        return AXIS2_FAILURE;
    }
    axis2_http_worker_set_is_application_client_side(worker, env, 
            server_impl->is_application_client_side);
    axis2_http_worker_set_svr_port(worker, env, server_impl->port);
    {/** Obtain server IP and set it 
		axis2_transport_in_desc_t *transport_in = NULL;
		axis2_conf_t *conf = NULL;
		axis2_transport_receiver_t *receiver = NULL;
		conf = axis2_conf_ctx_get_conf(server_impl->conf_ctx_private, env);
		transport_in = axis2_conf_get_transport_in(conf, env, AXIS2_TRANSPORT_ENUM_HTTP);
		receiver = axis2_transport_in_desc_get_recv(transport_in, env);
		if(receiver)
		{
			int listen_socket = axis2_http_svr_thread_get_listen_socket(server_impl->svr_thread, env);
			server_impl->svr_ip = axutil_network_handler_get_svr_ip(env, listen_socket);
			AXIS2_INTF_TO_IMPL(receiver)->svr_ip = server_impl->svr_ip;
		}
		*/
	}
	AXIS2_LOG_INFO(env->log, "Starting HTTP server thread");
    axis2_http_svr_thread_set_worker(server_impl->svr_thread, env, worker);
    axis2_http_svr_thread_run(server_impl->svr_thread, env);
	
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_server_stop(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env)
{
    AXIS2_LOG_INFO(env->log, "Terminating HTTP server thread");
    if(AXIS2_INTF_TO_IMPL(server)->svr_thread)
    {
        axis2_http_svr_thread_destroy(AXIS2_INTF_TO_IMPL(server)->svr_thread, env);
    }
    AXIS2_LOG_INFO(env->log, "Successfully terminated  HTTP server thread");
    return AXIS2_SUCCESS;
}

static axis2_conf_ctx_t *AXIS2_CALL
axis2_http_server_get_conf_ctx(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env)
{
    return AXIS2_INTF_TO_IMPL(server)->conf_ctx;
}

static axis2_endpoint_ref_t *AXIS2_CALL
axis2_http_server_get_reply_to_epr(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env,
    const axis2_char_t * svc_name)
{
    axis2_endpoint_ref_t *epr = NULL;
    const axis2_char_t *host_address = NULL;
    axis2_char_t *svc_path = NULL;
    axutil_url_t *url = NULL;

    host_address = AXIS2_DEFAULT_HOST_ADDRESS; /* TODO : get from axis2.xml */
    svc_path = axutil_stracat(env, AXIS2_DEFAULT_SVC_PATH, svc_name);
    url = axutil_url_create(env, AXIS2_HTTP_PROTOCOL, host_address,
        AXIS2_INTF_TO_IMPL(server)->port, svc_path);
		
    AXIS2_FREE(env->allocator, svc_path);
    if(!url)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create reply to epr for service %s.",
            svc_name);
        return NULL;
    }
    epr = axis2_endpoint_ref_create(env, axutil_url_to_external_form(url, env));
    axutil_url_free(url, env);
    return epr;
}

/** 
 * If service name is set then this will return epr path for service. Otherwise it will return 
 * server name
 */
static axis2_endpoint_ref_t *AXIS2_CALL
axis2_http_server_get_epr_for_service(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env,
    const axis2_char_t * svc_name)
{
    axis2_endpoint_ref_t *epr = NULL;
    const axis2_char_t *host_address = NULL;
    axis2_char_t *svc_path = NULL;
    axutil_url_t *url = NULL;

	if(AXIS2_INTF_TO_IMPL(server)->svr_ip)
	{
		host_address = AXIS2_INTF_TO_IMPL(server)->svr_ip;
	}else
	{
	    host_address = AXIS2_DEFAULT_HOST_ADDRESS; /* TODO : get from axis2.xml */
	}
    if(svc_name)
    {
	    svc_path = axutil_stracat(env, AXIS2_DEFAULT_SVC_PATH, svc_name);
    }
    
	url = axutil_url_create(env, AXIS2_HTTP_PROTOCOL, host_address,
        AXIS2_INTF_TO_IMPL(server)->port, svc_path);

    if(svc_path)
    {
        AXIS2_FREE(env->allocator, svc_path);
    }
    if(!url)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create reply to epr for service %s.",
            svc_name);
        return NULL;
    }
    epr = axis2_endpoint_ref_create(env, axutil_url_to_external_form(url, env));
    axutil_url_free(url, env);
    return epr;
}

static axis2_bool_t AXIS2_CALL
axis2_http_server_is_running(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env)
{
    axis2_http_server_impl_t *server_impl = NULL;
    server_impl = AXIS2_INTF_TO_IMPL(server);
    if(server_impl->svr_thread)
    {
        return axis2_http_svr_thread_is_running(server_impl->svr_thread, env);
    }
    return AXIS2_FALSE;
}

static void AXIS2_CALL
axis2_http_server_set_is_application_client_side(
    axis2_transport_receiver_t * server,
    const axutil_env_t * env,
    axis2_bool_t is_application_client_side)
{
    axis2_http_server_impl_t *server_impl = NULL;
    server_impl = AXIS2_INTF_TO_IMPL(server);
    server_impl->is_application_client_side = is_application_client_side;
}

static  axis2_char_t* AXIS2_CALL
axis2_http_server_get_server_ip(
axis2_transport_receiver_t *server,
const axutil_env_t *env)
{
	return AXIS2_INTF_TO_IMPL(server)->svr_ip;
}

static void AXIS2_CALL
axis2_http_server_set_server_ip(
axis2_transport_receiver_t *server,
const axutil_env_t *env,
 axis2_char_t *serverip)
{
	AXIS2_INTF_TO_IMPL(server)->svr_ip = serverip;
}

/**
 * Following block distinguish the exposed part of the dll.
 */
AXIS2_EXPORT int
axis2_get_instance(
    struct axis2_transport_receiver **inst,
    const axutil_env_t * env)
{
    *inst = axis2_http_server_create(env, NULL, -1);
    if(!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int
axis2_remove_instance(
    axis2_transport_receiver_t * inst,
    const axutil_env_t * env)
{
    if(inst)
    {
        axis2_transport_receiver_free(inst, env);
    }
    return AXIS2_SUCCESS;
}

