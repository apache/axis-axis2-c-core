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
#include <axis2_param_container.h>
#include <axis2_url.h>
#include <axis2_conf_init.h>

/**
 * @brief HTTP Client struct impl
 *   Axis2 HTTP Client impl
 */

typedef struct axis2_http_server_impl
{
    axis2_transport_receiver_t http_server;
    axis2_http_svr_thread_t *svr_thread;
    int port;
    axis2_conf_ctx_t* conf_ctx;
    axis2_conf_ctx_t* conf_ctx_private;
}
axis2_http_server_impl_t;

#define AXIS2_INTF_TO_IMPL(http_server) \
                ((axis2_http_server_impl_t *)(http_server))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_http_server_init(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env,
    axis2_conf_ctx_t *conf_ctx,
    axis2_transport_in_desc_t *in_desc);

axis2_status_t AXIS2_CALL
axis2_http_server_start(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_http_server_stop(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env);

axis2_conf_ctx_t *AXIS2_CALL
axis2_http_server_get_conf_ctx(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env);

axis2_endpoint_ref_t *AXIS2_CALL
axis2_http_server_get_reply_to_epr(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env,
    const axis2_char_t *svc_name);

axis2_bool_t AXIS2_CALL
axis2_http_server_is_running(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_http_server_free(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env);

/***************************** End of function headers ************************/

AXIS2_EXTERN axis2_transport_receiver_t *AXIS2_CALL
axis2_http_server_create(
    const axis2_env_t *env,
    const axis2_char_t *repo,
    const int port)
{
    axis2_http_server_impl_t *server_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    server_impl = (axis2_http_server_impl_t *)AXIS2_MALLOC
            (env->allocator, sizeof(axis2_http_server_impl_t));

    if (NULL == server_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    server_impl->svr_thread = NULL;
    server_impl->conf_ctx = NULL;
    server_impl->conf_ctx_private = NULL;
    server_impl->port = port;

    server_impl->http_server.ops = AXIS2_MALLOC(env->allocator,
            sizeof(axis2_transport_receiver_ops_t));
    if (NULL == server_impl->http_server.ops)
    {
        axis2_http_server_free((axis2_transport_receiver_t *) server_impl, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    if (repo)
    {
        /**
         * We first create a private conf ctx which is owned by this server
         * we only free this private conf context. We should never free the
         * server_impl->conf_ctx because it may own to any other object which
         * may lead to double free
         */
        server_impl->conf_ctx_private = build_conf_ctx(env, repo);
        if (NULL == server_impl->conf_ctx_private)
        {
            axis2_http_server_free((axis2_transport_receiver_t *) server_impl, env);
            return NULL;
        }
        server_impl->conf_ctx = server_impl->conf_ctx_private;
    }
    server_impl->http_server.ops->init = axis2_http_server_init;
    server_impl->http_server.ops->start = axis2_http_server_start;
    server_impl->http_server.ops->stop = axis2_http_server_stop;
    server_impl->http_server.ops->get_reply_to_epr =
        axis2_http_server_get_reply_to_epr;
    server_impl->http_server.ops->get_conf_ctx = axis2_http_server_get_conf_ctx;
    server_impl->http_server.ops->is_running = axis2_http_server_is_running;
    server_impl->http_server.ops->free = axis2_http_server_free;

    return &(server_impl->http_server);
}

axis2_status_t AXIS2_CALL
axis2_http_server_free(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env)
{
    axis2_http_server_impl_t *server_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    server_impl = AXIS2_INTF_TO_IMPL(server);
    if (server_impl->svr_thread)
    {
        AXIS2_HTTP_SVR_THREAD_DESTROY(server_impl->svr_thread, env);
        AXIS2_HTTP_SVR_THREAD_FREE(server_impl->svr_thread, env);
        server_impl->svr_thread = NULL;
    }

    if (server_impl->conf_ctx_private)
    {
        AXIS2_CONF_CTX_FREE(server_impl->conf_ctx_private, env);
        server_impl->conf_ctx_private = NULL;
    }
    /**
     * Do not free this. It may own to some other object
     */
    server_impl->conf_ctx = NULL;

    if (server->ops)
    {
        AXIS2_FREE(env->allocator, server->ops);
    }
    AXIS2_FREE(env->allocator, server_impl);
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_http_server_init(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env,
    axis2_conf_ctx_t *conf_ctx,
    axis2_transport_in_desc_t *in_desc)
{
    axis2_http_server_impl_t *server_impl = NULL;
    axis2_char_t *port_str = NULL;
    axis2_param_t *param = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    server_impl = AXIS2_INTF_TO_IMPL(server);

    server_impl->conf_ctx = conf_ctx;
    param = (axis2_param_t *)AXIS2_PARAM_CONTAINER_GET_PARAM(
                in_desc->param_container, env, "port");
    if (param)
    {
        port_str = AXIS2_PARAM_GET_VALUE(param, env);
    }
    if (port_str)
    {
        server_impl->port = atoi(port_str);
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_http_server_start(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env)
{

    axis2_http_server_impl_t *server_impl = NULL;
    axis2_http_worker_t *worker = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    server_impl = AXIS2_INTF_TO_IMPL(server);
    server_impl->svr_thread = axis2_http_svr_thread_create(env,
            server_impl->port);
    if (NULL == server_impl->svr_thread)
    {
        return AXIS2_FAILURE;
    }
    worker = axis2_http_worker_create(env, server_impl->conf_ctx);
    AXIS2_HTTP_WORKER_SET_SVR_PORT(worker, env, server_impl->port);
    if (NULL == worker)
    {
        AXIS2_HTTP_SVR_THREAD_FREE(server_impl->svr_thread, env);
        return AXIS2_FAILURE;
    }
    AXIS2_LOG_INFO(env->log, "Starting HTTP server thread");
    AXIS2_HTTP_SVR_THREAD_SET_WORKER(server_impl->svr_thread, env, worker);
    AXIS2_HTTP_SVR_THREAD_RUN(server_impl->svr_thread, env);
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_http_server_stop(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    AXIS2_LOG_INFO(env->log, "Terminating HTTP server thread");
    if (AXIS2_INTF_TO_IMPL(server)->svr_thread)
    {
        AXIS2_HTTP_SVR_THREAD_DESTROY(AXIS2_INTF_TO_IMPL(server)->svr_thread,
                env);
    }
    AXIS2_LOG_INFO(env->log, "Successfully terminated  HTTP server"
            " thread");
    return AXIS2_SUCCESS;
}

axis2_conf_ctx_t *AXIS2_CALL
axis2_http_server_get_conf_ctx(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(server)->conf_ctx;
}

axis2_endpoint_ref_t *AXIS2_CALL
axis2_http_server_get_reply_to_epr(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env,
    const axis2_char_t *svc_name)
{
    axis2_endpoint_ref_t *epr = NULL;
    const axis2_char_t *host_address = NULL;
    axis2_char_t *svc_path = NULL;
    axis2_url_t *url = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, svc_name, NULL);

    host_address = "127.0.0.1"; /* TODO : get from axis2.xml */
    svc_path = AXIS2_STRACAT("/axis2/services/", svc_name, env);
    url = axis2_url_create(env, "http", host_address,
            AXIS2_INTF_TO_IMPL(server)->port, svc_path);
    AXIS2_FREE(env->allocator, svc_path);
    if (NULL == url)
    {
        return NULL;
    }
    epr = axis2_endpoint_ref_create(env, AXIS2_URL_TO_EXTERNAL_FORM(url, env));
    AXIS2_URL_FREE(url, env);
    return epr;
}

axis2_bool_t AXIS2_CALL
axis2_http_server_is_running(
    axis2_transport_receiver_t *server,
    const axis2_env_t *env)
{
    axis2_http_server_impl_t *server_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    server_impl = AXIS2_INTF_TO_IMPL(server);
    if (NULL == server_impl->svr_thread)
    {
        return AXIS2_FALSE;
    }
    else
    {
        return AXIS2_HTTP_SVR_THREAD_IS_RUNNING(server_impl->svr_thread, env);
    }
    return AXIS2_FALSE;
}

/**
 * Following block distinguish the exposed part of the dll.
 */
AXIS2_EXPORT int axis2_get_instance(
    struct axis2_transport_receiver **inst,
    const axis2_env_t *env)
{
    *inst = axis2_http_server_create(env, NULL, -1);
    if (!(*inst))
    {
        printf("transport receiver load not success\n");
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int axis2_remove_instance(
    axis2_transport_receiver_t *inst,
    const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
    if (inst)
    {
        status = AXIS2_TRANSPORT_RECEIVER_FREE(inst, env);
    }
    return status;
}
