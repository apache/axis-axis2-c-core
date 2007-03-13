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

#include <axis2_svc_client.h>
#include <axis2_phases_info.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_uri.h>
#include "axis2_callback_recv.h"
#include <axiom_soap_const.h>
#include <axiom_soap_body.h>
#include <axiom_soap_header.h>
#include <axis2_listener_manager.h>
#include <axis2_module_desc.h>
#include <axis2_array_list.h>
#include <axis2_options.h>
#include <axis2_conf_init.h>
#include <axis2_mep_client.h>
#include "../description/axis2_client_utils.h"
#include <platforms/axis2_platform_auto_sense.h>
#include <stdio.h>

typedef struct axis2_svc_client_impl
{
    /** svc_client base struct */
    axis2_svc_client_t svc_client;

    axis2_svc_t *svc;

    axis2_conf_t *conf;

    axis2_conf_ctx_t *conf_ctx;

    axis2_svc_ctx_t *svc_ctx;

    axis2_options_t *options;

    axis2_options_t *override_options;

    axis2_array_list_t *headers;

    /** for receiving the async messages */
    axis2_callback_recv_t *callback_recv;

    axis2_listener_manager_t *listener_manager;

    axis2_op_client_t *op_client;

}
axis2_svc_client_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(svc_client) ((axis2_svc_client_impl_t *)svc_client)

/** private functions */
static axis2_bool_t
axis2_svc_client_init_transports_from_conf_ctx(
    const axis2_env_t *env,
    axis2_svc_client_impl_t *svc_client_impl,
    axis2_conf_ctx_t *conf_ctx,
    const axis2_char_t *client_home);

static axis2_bool_t
axis2_svc_client_init_data(
    const axis2_env_t *env,
    axis2_svc_client_impl_t *svc_client_impl);

static void
axis2_svc_client_init_ops(
    axis2_svc_client_t *svc_client);

static axis2_svc_t *
axis2_svc_client_create_annonymous_svc(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env);

static axis2_bool_t
axis2_svc_client_fill_soap_envelope(
    const axis2_env_t *env,
    axis2_svc_client_impl_t *svc_client_impl,
    axis2_msg_ctx_t *msg_ctx,
    const axiom_node_t *payload);

/** public functions */
axis2_svc_t *AXIS2_CALL
axis2_svc_client_get_svc(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_client_set_options(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_options_t *options);

const axis2_options_t *AXIS2_CALL
axis2_svc_client_get_options(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_client_set_override_options(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_options_t *override_options);

const axis2_options_t *AXIS2_CALL
axis2_svc_client_get_override_options(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_client_engage_module(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_char_t *module_name);

axis2_status_t AXIS2_CALL
axis2_svc_client_disengage_module(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_char_t *module_name);

axis2_status_t AXIS2_CALL
axis2_svc_client_add_header(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    axiom_node_t *header);

axis2_status_t AXIS2_CALL
axis2_svc_client_remove_all_headers(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_client_send_robust(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname,
    const axiom_node_t *payload);

void AXIS2_CALL
axis2_svc_client_fire_and_forget(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname,
    const axiom_node_t *payload);

axiom_node_t *AXIS2_CALL
axis2_svc_client_send_receive(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname,
    const axiom_node_t *payload);

void AXIS2_CALL
axis2_svc_client_send_receive_non_blocking(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname,
    const axiom_node_t *payload,
    axis2_callback_t *callback);

axis2_op_client_t *AXIS2_CALL
axis2_svc_client_create_op_client(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname);

axis2_status_t AXIS2_CALL
axis2_svc_client_finalize_invoke(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env);

const axis2_endpoint_ref_t *AXIS2_CALL
axis2_svc_client_get_own_endpoint_ref(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_char_t *transport);

const axis2_endpoint_ref_t *AXIS2_CALL
axis2_svc_client_get_target_endpoint_ref(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_client_set_target_endpoint_ref(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *target_endpoint_ref);

axis2_svc_ctx_t *AXIS2_CALL
axis2_svc_client_get_svc_ctx(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_svc_client_free(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env);

axis2_op_client_t *AXIS2_CALL
axis2_svc_client_get_op_client(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env);


axis2_svc_client_t *AXIS2_CALL
axis2_svc_client_create(
    const axis2_env_t *env,
    const axis2_char_t *client_home)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    svc_client_impl = AXIS2_INTF_TO_IMPL(axis2_svc_client_create_with_conf_ctx_and_svc
            (env, client_home, NULL, NULL));

    if (!svc_client_impl)
        return NULL;

    return &(svc_client_impl->svc_client);
}

AXIS2_EXTERN axis2_svc_client_t *AXIS2_CALL
axis2_svc_client_create_for_dynamic_invocation(
    const axis2_env_t *env,
    axis2_conf_ctx_t *conf_ctx,
    const axis2_uri_t *wsdl_uri,
    const axis2_qname_t *wsdl_svc_qname,
    const axis2_char_t *endpoint_name,
    const axis2_char_t *client_home)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    axis2_svc_grp_t *svc_grp = NULL;
    axis2_svc_grp_ctx_t *svc_grp_ctx = NULL;
    const axis2_char_t *svc_grp_name = NULL;
    axis2_hash_t *ops = NULL;
    const axis2_char_t *repos_path = NULL;
    axis2_char_t *wsdl_path = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc_client_impl = AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_client_impl_t));
    if (!svc_client_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc_client_impl->svc = NULL;
    svc_client_impl->conf = NULL;
    svc_client_impl->conf_ctx = NULL;
    svc_client_impl->svc_ctx = NULL;
    svc_client_impl->options = NULL;
    svc_client_impl->override_options = NULL;
    svc_client_impl->headers = NULL;
    svc_client_impl->callback_recv = NULL;
    svc_client_impl->listener_manager = NULL;
    svc_client_impl->op_client = NULL;

    /** initialize private data to NULL, create options */
    if (!axis2_svc_client_init_data(env, svc_client_impl))
    {
        axis2_svc_client_free(&(svc_client_impl->svc_client), env);
        return NULL;
    }

    /* the following method call will create the default conf_ctx if it is NULL */
    if (!axis2_svc_client_init_transports_from_conf_ctx(env, svc_client_impl,
            conf_ctx, client_home))
    {
        axis2_svc_client_free(&(svc_client_impl->svc_client), env);
        return NULL;
    }

    svc_client_impl->conf =  axis2_conf_ctx_get_conf(svc_client_impl->conf_ctx, env);
    repos_path = AXIS2_CONF_GET_REPO(svc_client_impl->conf, env);
    wsdl_path = axis2_strcat(env, repos_path, AXIS2_PATH_SEP_STR, "woden", NULL);

    svc_client_impl->options = axis2_options_create(env);
    /* TODO: this method should be moved out of core implementation
    svc_client_impl->svc = axis2_client_utils_create_axis2_svc(env, wsdl_uri,
            wsdl_svc_qname, endpoint_name, wsdl_path, svc_client_impl->options); */
    if (svc_client_impl->svc)
    {
        axis2_hash_index_t *i = NULL;
        void *v = NULL;
        axis2_op_t *op = NULL;

        ops = AXIS2_SVC_GET_ALL_OPS(svc_client_impl->svc, env);
        for (i = axis2_hash_first(ops, env); i; i = axis2_hash_next(env, i))
        {
            axis2_phases_info_t * info = NULL;
            axis2_hash_this(i, NULL, NULL, &v);
            op = (axis2_op_t *) v;

            /* Setting operation phase */
            info = AXIS2_CONF_GET_PHASES_INFO(svc_client_impl->conf, env);
            AXIS2_PHASES_INFO_SET_OP_PHASES(info, env, op);
        }
    }
	 else
		  return AXIS2_FAILURE;

    /** add the service to the config context if it isn't in there already */
    if (NULL == AXIS2_CONF_GET_SVC(svc_client_impl->conf, env,
            AXIS2_SVC_GET_NAME(svc_client_impl->svc, env)))
    {
        AXIS2_CONF_ADD_SVC(svc_client_impl->conf, env, svc_client_impl->svc);
    }

    /** create a service context for myself: create a new service group
     context and then get the service context for myself as I'll need that
     later for stuff that I gotta do */
    svc_grp = AXIS2_SVC_GET_PARENT(svc_client_impl->svc, env);
    if (!svc_grp)
        return NULL;

    svc_grp_ctx =  axis2_svc_grp_get_svc_grp_ctx(svc_grp, env,
            svc_client_impl->conf_ctx);
    if (!svc_grp_ctx)
        return NULL;

    svc_grp_name =  axis2_svc_grp_get_name(svc_grp, env);
    if (!svc_grp_name)
        return NULL; /* service group name is mandatory */

    axis2_conf_ctx_register_svc_grp_ctx(svc_client_impl->conf_ctx, env,
            svc_grp_name, svc_grp_ctx);

    svc_client_impl->svc_ctx =  axis2_svc_grp_ctx_get_svc_ctx(svc_grp_ctx, env,
            AXIS2_SVC_GET_NAME(svc_client_impl->svc, env));

    /* create ops */
    svc_client_impl->svc_client.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_client_ops_t));

    if (!svc_client_impl->svc_client.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_svc_client_free(&(svc_client_impl->svc_client), env);
        return NULL;
    }

    /** initialize ops */
    axis2_svc_client_init_ops(&(svc_client_impl->svc_client));

    return &(svc_client_impl->svc_client);
}


axis2_svc_client_t* AXIS2_CALL
axis2_svc_client_create_with_conf_ctx_and_svc(
    const axis2_env_t *env,
    const axis2_char_t *client_home,
    axis2_conf_ctx_t *conf_ctx,
    axis2_svc_t *svc)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    axis2_svc_grp_t *svc_grp = NULL;
    axis2_svc_grp_ctx_t *svc_grp_ctx = NULL;
    const axis2_char_t *svc_grp_name = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc_client_impl = AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_client_impl_t));
    if (!svc_client_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc_client_impl->svc = NULL;
    svc_client_impl->conf = NULL;
    svc_client_impl->conf_ctx = NULL;
    svc_client_impl->svc_ctx = NULL;
    svc_client_impl->options = NULL;
    svc_client_impl->override_options = NULL;
    svc_client_impl->headers = NULL;
    svc_client_impl->callback_recv = NULL;
    svc_client_impl->listener_manager = NULL;
    svc_client_impl->op_client = NULL;

    /** initialize private data to NULL, create options */
    if (!axis2_svc_client_init_data(env, svc_client_impl))
    {
        axis2_svc_client_free(&(svc_client_impl->svc_client), env);
        return NULL;
    }

    /* the following method call will create the default conf_ctx if it is NULL */
    if (!axis2_svc_client_init_transports_from_conf_ctx(env, svc_client_impl,
            conf_ctx, client_home))
    {
        axis2_svc_client_free(&(svc_client_impl->svc_client), env);
        return NULL;
    }

    svc_client_impl->conf =  axis2_conf_ctx_get_conf(svc_client_impl->conf_ctx, env);

    if (svc)
    {
        svc_client_impl->svc = svc;
    }
    else
    {
        if (NULL == (svc_client_impl->svc =
                    axis2_svc_client_create_annonymous_svc(
                        &(svc_client_impl->svc_client), env)))
        {
            axis2_svc_client_free(&(svc_client_impl->svc_client), env);
            return NULL;
        }
    }

    /** add the service to the config context if it isn't in there already */
    if (NULL == AXIS2_CONF_GET_SVC(svc_client_impl->conf, env,
            AXIS2_SVC_GET_NAME(svc_client_impl->svc, env)))
    {
        AXIS2_CONF_ADD_SVC(svc_client_impl->conf, env, svc_client_impl->svc);
    }

    /** create a service context for myself: create a new service group
     context and then get the service context for myself as I'll need that
     later for stuff that I gotta do */
    svc_grp = AXIS2_SVC_GET_PARENT(svc_client_impl->svc, env);
    if (!svc_grp)
        return NULL;

    svc_grp_ctx =  axis2_svc_grp_get_svc_grp_ctx(svc_grp, env,
            svc_client_impl->conf_ctx);
    if (!svc_grp_ctx)
        return NULL;

    svc_grp_name =  axis2_svc_grp_get_name(svc_grp, env);
    if (!svc_grp_name)
        return NULL; /* service group name is mandatory */

    axis2_conf_ctx_register_svc_grp_ctx(svc_client_impl->conf_ctx, env,
            svc_grp_name, svc_grp_ctx);

    svc_client_impl->svc_ctx =  axis2_svc_grp_ctx_get_svc_ctx(svc_grp_ctx, env,
            AXIS2_SVC_GET_NAME(svc_client_impl->svc, env));

    /* create ops */
    svc_client_impl->svc_client.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_client_ops_t));

    if (!svc_client_impl->svc_client.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_svc_client_free(&(svc_client_impl->svc_client), env);
        return NULL;
    }

    /** initialize ops */
    axis2_svc_client_init_ops(&(svc_client_impl->svc_client));

    return &(svc_client_impl->svc_client);
}

axis2_svc_t *AXIS2_CALL
axis2_svc_client_get_svc(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
    return svc_client_impl->svc;
}

axis2_status_t AXIS2_CALL
axis2_svc_client_set_options(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_options_t *options)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    if (svc_client_impl->options)
        AXIS2_OPTIONS_FREE(svc_client_impl->options, env);
    svc_client_impl->options = (axis2_options_t *)options;
    return AXIS2_SUCCESS;
}

const axis2_options_t* AXIS2_CALL
axis2_svc_client_get_options(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    return svc_client_impl->options;
}

axis2_status_t AXIS2_CALL
axis2_svc_client_set_override_options(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_options_t *override_options)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    if (svc_client_impl->override_options)
        AXIS2_OPTIONS_FREE(svc_client_impl->override_options, env);

    svc_client_impl->override_options = (axis2_options_t *)override_options;

    return AXIS2_SUCCESS;
}

const axis2_options_t* AXIS2_CALL
axis2_svc_client_get_override_options(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
    return svc_client_impl->override_options;
}

axis2_status_t AXIS2_CALL
axis2_svc_client_engage_module(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_char_t *module_name)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    axis2_module_desc_t *module = NULL;
    axis2_qname_t *mod_qname = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_name, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
    mod_qname = axis2_qname_create(env, module_name, NULL, NULL);

    if (mod_qname)
    {
        module = AXIS2_CONF_GET_MODULE(svc_client_impl->conf, env, mod_qname);

        AXIS2_QNAME_FREE(mod_qname, env);
        mod_qname = NULL;
    }
    else
        return AXIS2_FAILURE;

    if (module)
    {
        return AXIS2_SVC_ENGAGE_MODULE(svc_client_impl->svc, env, module,
                svc_client_impl->conf);
    }
    return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL
axis2_svc_client_disengage_module(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_char_t *module_name)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    axis2_module_desc_t *module = NULL;
    axis2_qname_t *mod_qname = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_name, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
    mod_qname = axis2_qname_create(env, module_name, NULL, NULL);

    module = AXIS2_CONF_GET_MODULE(svc_client_impl->conf, env, mod_qname);

    /**TODO:uncomment once axis2_svc_disengage_module is implemented
    if (module)
    {
       return AXIS2_SVC_DISENGAGE_MODULE(svc_client_impl->svc, env, module);

    }
    */

    return AXIS2_FAILURE;

}

axis2_status_t AXIS2_CALL
axis2_svc_client_add_header(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    axiom_node_t *header)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    if (!svc_client_impl->headers)
    {
        svc_client_impl->headers = axis2_array_list_create(env, 0);
        if (!svc_client_impl->headers)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axis2_array_list_add(svc_client_impl->headers, env, header);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_client_remove_all_headers(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    int i = 0, size = 0;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    if (!svc_client_impl->headers)
        return AXIS2_FAILURE;

    size = axis2_array_list_size(svc_client_impl->headers, env);
    for (i = 0; i < size; i++)
    {
        axiom_node_t *node = NULL;
        node = axis2_array_list_get(svc_client_impl->headers, env, i);
        if (node)
        {
            AXIOM_NODE_FREE_TREE(node, env);
            node = NULL;
        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_client_send_robust(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname,
    const axiom_node_t *payload)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_bool_t qname_free_flag = AXIS2_FALSE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    if (!op_qname)
    {
        op_qname = axis2_qname_create(env, AXIS2_ANON_ROBUST_OUT_ONLY_OP, NULL, NULL);
        qname_free_flag = AXIS2_TRUE;
    }

    msg_ctx = axis2_msg_ctx_create(env,
             axis2_svc_ctx_get_conf_ctx(svc_client_impl->svc_ctx, env), NULL, NULL);
    if (!axis2_svc_client_fill_soap_envelope(env, svc_client_impl, msg_ctx,
            payload))
    {
        return AXIS2_FAILURE;
    }

    if(!axis2_svc_client_create_op_client(&(svc_client_impl->svc_client),
        env, op_qname))
    {
        return AXIS2_FAILURE;
    }

    AXIS2_OP_CLIENT_ADD_OUT_MSG_CTX(svc_client_impl->op_client, env, msg_ctx);
    status = AXIS2_OP_CLIENT_EXECUTE(svc_client_impl->op_client, env, AXIS2_TRUE);
    
    if (qname_free_flag)
    {
        AXIS2_QNAME_FREE((axis2_qname_t *) op_qname, env);
        op_qname = NULL;
    }

    return status;
}

void AXIS2_CALL
axis2_svc_client_fire_and_forget(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname,
    const axiom_node_t *payload)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    axis2_op_client_t *op_client = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_bool_t qname_free_flag = AXIS2_FALSE;

    if (!env)
        return;

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    if (!op_qname)
    {
        op_qname = axis2_qname_create(env, AXIS2_ANON_OUT_ONLY_OP, NULL, NULL);
        qname_free_flag = AXIS2_TRUE;
    }

    msg_ctx = axis2_msg_ctx_create(env,
             axis2_svc_ctx_get_conf_ctx(svc_client_impl->svc_ctx, env), NULL, NULL);
    if (!axis2_svc_client_fill_soap_envelope(env, svc_client_impl, msg_ctx,
            payload))
    {
        return;
    }

    if(!axis2_svc_client_create_op_client(&(svc_client_impl->svc_client),
        env, op_qname))
    {
        return;
    }

    AXIS2_OP_CLIENT_ADD_OUT_MSG_CTX(svc_client_impl->op_client, env, msg_ctx);
    AXIS2_OP_CLIENT_EXECUTE(op_client, env, AXIS2_FALSE);
    
    if (qname_free_flag)
    {
        AXIS2_QNAME_FREE((axis2_qname_t *) op_qname, env);
        op_qname = NULL;
    }

    return;
}

axiom_node_t* AXIS2_CALL
axis2_svc_client_send_receive(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname,
    const axiom_node_t *payload)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_node_t *soap_node = NULL;
    axis2_op_t *op = NULL;
    axis2_param_t *param = NULL;
    axis2_uri_t *action_uri = NULL;
    axis2_char_t *action_str = NULL;
    axis2_bool_t qname_free_flag = AXIS2_FALSE;

    AXIS2_ENV_CHECK(env, NULL);
    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    op = AXIS2_SVC_GET_OP_WITH_QNAME(svc_client_impl->svc, env, op_qname);
    if (op)
    {
        param = axis2_op_get_param(op, env, AXIS2_SOAP_ACTION);
        if (param)
        {
            action_uri = (axis2_uri_t *) AXIS2_PARAM_GET_VALUE(param, env);
            action_str = AXIS2_URI_TO_STRING(action_uri, env, AXIS2_URI_UNP_OMITUSERINFO);
            AXIS2_OPTIONS_SET_ACTION(svc_client_impl->options, env, action_str);
        }
    }

    if (!op_qname)
    {
        qname_free_flag = AXIS2_TRUE;
        op_qname = axis2_qname_create(env, AXIS2_ANON_OUT_IN_OP, NULL, NULL);
    }

    if (AXIS2_OPTIONS_GET_USE_SEPERATE_LISTENER(svc_client_impl->options, env))
    {
        axis2_callback_t *callback = NULL;
        axis2_msg_ctx_t *msg_ctx = NULL;
        long index = 0;

        /* This means doing a Request-Response invocation using two channels.
        If the transport is a two way transport (e.g. http), only one channel is used
        (e.g. in http cases 202 OK is sent to say no response available).
        Axis2 gets blocked and return when the response is available. */

        callback = axis2_callback_create(env);
        if (!callback)
            return NULL;

        /* call two channel non blocking invoke to do the work and wait on the callback */
        axis2_svc_client_send_receive_non_blocking(
            svc_client, env, op_qname, payload, callback);

        index = AXIS2_OPTIONS_GET_TIMEOUT_IN_MILLI_SECONDS(svc_client_impl->options, env) / 10;

        while (!(AXIS2_CALLBACK_GET_COMPLETE(callback, env)))
        {
            /*wait till the response arrives*/
            if (index-- >= 0)
            {
                AXIS2_USLEEP(10000);
                msg_ctx = (axis2_msg_ctx_t *)AXIS2_OP_CLIENT_GET_MSG_CTX(
                            svc_client_impl->op_client, env,
                            AXIS2_WSDL_MESSAGE_LABEL_OUT);
                if (msg_ctx)
                {
                    axis2_msg_ctx_t *res_msg_ctx =
                        axis2_mep_client_receive(env, msg_ctx);
                    if (res_msg_ctx)
                    {
                        soap_envelope =  axis2_msg_ctx_get_soap_envelope(res_msg_ctx, env);
                        if (soap_envelope)
                        {
                            soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);

                            if (soap_body)
                            {
                                soap_node = AXIOM_SOAP_BODY_GET_BASE_NODE(soap_body, env);
                                if (soap_node)
                                {
                                    return AXIOM_NODE_GET_FIRST_ELEMENT(soap_node, env);
                                }
                            }
                        }
                    }
                }

            }
            else
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_RESPONSE_TIMED_OUT, AXIS2_FAILURE);
                return NULL;
            }
        }

        soap_envelope = AXIS2_CALLBACK_GET_ENVELOPE(callback, env);

        /* start of hack to get rid of memory leak */
        msg_ctx = axis2_msg_ctx_create(env,
                         axis2_svc_ctx_get_conf_ctx(svc_client_impl->svc_ctx, env), NULL, NULL);
        AXIS2_OP_CLIENT_ADD_MSG_CTX(svc_client_impl->op_client, env, msg_ctx);
         axis2_msg_ctx_set_soap_envelope(msg_ctx, env, soap_envelope);
        /* end of hack to get rid of memory leak */

        /* process the result of the invocation */
        if (!soap_envelope)
        {
            if (AXIS2_CALLBACK_GET_ERROR(callback, env) != AXIS2_ERROR_NONE)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_CALLBACK_GET_ERROR(callback, env), AXIS2_FAILURE);
                return NULL;
            }
        }
    }
    else
    {
        axis2_msg_ctx_t *res_msg_ctx = NULL;
        axis2_msg_ctx_t *msg_ctx = NULL;

        msg_ctx = axis2_msg_ctx_create(env,
                 axis2_svc_ctx_get_conf_ctx(svc_client_impl->svc_ctx, env), NULL, NULL);
        if (!axis2_svc_client_fill_soap_envelope(env, svc_client_impl, msg_ctx,
                payload))
        {
            return NULL;
        }

        if(!axis2_svc_client_create_op_client(&(svc_client_impl->svc_client), 
            env, op_qname))
        {
            return NULL;
        }

        AXIS2_OP_CLIENT_ADD_MSG_CTX(svc_client_impl->op_client, env, msg_ctx);
        AXIS2_OP_CLIENT_EXECUTE(svc_client_impl->op_client, env, AXIS2_TRUE);
        res_msg_ctx = (axis2_msg_ctx_t *)AXIS2_OP_CLIENT_GET_MSG_CTX(
            svc_client_impl->op_client, env, AXIS2_WSDL_MESSAGE_LABEL_IN);
        if (res_msg_ctx)
            soap_envelope =  axis2_msg_ctx_get_soap_envelope(res_msg_ctx, env);
        else
            AXIS2_OP_CLIENT_ADD_MSG_CTX(svc_client_impl->op_client, env, 
                res_msg_ctx); /* set in msg_ctx to be NULL to reset */
    }

    if (qname_free_flag)
    {
        AXIS2_QNAME_FREE((axis2_qname_t *) op_qname, env);
        op_qname = NULL;
    }

    if (!soap_envelope)
    {
        return NULL;
    }
    soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);

    if (!soap_body)
    {
        return NULL;
    }

    soap_node = AXIOM_SOAP_BODY_GET_BASE_NODE(soap_body, env);
    if (!soap_node)
    {
        return NULL;
    }
    return AXIOM_NODE_GET_FIRST_ELEMENT(soap_node, env);
}


void AXIS2_CALL
axis2_svc_client_send_receive_non_blocking(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname,
    const axiom_node_t *payload,
    axis2_callback_t *callback)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    AXIS2_TRANSPORT_ENUMS transport_in_protocol;
    axis2_bool_t qname_free_flag = AXIS2_FALSE;

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    if (!op_qname)
    {
        op_qname = axis2_qname_create(env, AXIS2_ANON_OUT_IN_OP, NULL, NULL);
        qname_free_flag = AXIS2_TRUE;
    }

    msg_ctx = axis2_msg_ctx_create(env,
             axis2_svc_ctx_get_conf_ctx(svc_client_impl->svc_ctx, env), NULL, NULL);
    if (!axis2_svc_client_fill_soap_envelope(env, svc_client_impl, msg_ctx, payload))
        return;

    if(!axis2_svc_client_create_op_client(&(svc_client_impl->svc_client),
        env, op_qname))
    {
        return;
    }

    AXIS2_OP_CLIENT_SET_CALLBACK(svc_client_impl->op_client, env, callback);
    AXIS2_OP_CLIENT_ADD_OUT_MSG_CTX(svc_client_impl->op_client, env, msg_ctx);

    if (AXIS2_OPTIONS_GET_USE_SEPERATE_LISTENER(svc_client_impl->options, env))
    {
        axis2_op_t *op = NULL;

        transport_in_protocol = AXIS2_OPTIONS_GET_TRANSPORT_IN_PROTOCOL(
                    svc_client_impl->options, env);
        AXIS2_LISTNER_MANAGER_MAKE_SURE_STARTED(svc_client_impl->listener_manager, env,
                transport_in_protocol, svc_client_impl->conf_ctx);
        /* Following sleep is required to ensure the listner is ready to receive response.
           If it is missing, the response gets lost. - Samisa */
        AXIS2_USLEEP(1);

        op = AXIS2_SVC_GET_OP_WITH_QNAME(svc_client_impl->svc, env,
            op_qname);
        axis2_op_set_msg_recv(op, env,
            AXIS2_CALLBACK_RECV_GET_BASE(svc_client_impl->callback_recv, env));
        AXIS2_OP_CLIENT_SET_CALLBACK_RECV(svc_client_impl->op_client, env,
            svc_client_impl->callback_recv);
    }

    AXIS2_OP_CLIENT_EXECUTE(svc_client_impl->op_client, env, AXIS2_FALSE);
    
    if (qname_free_flag)
    {
        AXIS2_QNAME_FREE((axis2_qname_t *) op_qname, env);
        op_qname = NULL;
    }

    return;
}

axis2_op_client_t* AXIS2_CALL
axis2_svc_client_create_op_client(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_qname_t *op_qname)
{
    axis2_op_t *op = NULL;
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    op = AXIS2_SVC_GET_OP_WITH_QNAME(svc_client_impl->svc, env, op_qname);

    if (!op)
    {
        /*TODO:error - svc does not have the operation*/
        return NULL;
    }

    if (!(svc_client_impl->op_client))
    {
        svc_client_impl->op_client = axis2_op_client_create(env, op, svc_client_impl->svc_ctx,
                svc_client_impl->options);
    }

    /**
      If override options have been set, that means we need to make sure
      those options override the options of even the operation client. So,
      what we do is switch the parents around to make that work.
    */
    if (svc_client_impl->override_options)
    {
        AXIS2_OPTIONS_SET_PARENT(svc_client_impl->override_options, env,
                AXIS2_OP_CLIENT_GET_OPTIONS(svc_client_impl->op_client, env));
        AXIS2_OP_CLIENT_SET_OPTIONS(svc_client_impl->op_client, env,
                svc_client_impl->override_options);
    }

    return svc_client_impl->op_client;
}

axis2_status_t AXIS2_CALL
axis2_svc_client_finalize_invoke(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    AXIS2_TRANSPORT_ENUMS transport_in_protocol;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    transport_in_protocol = AXIS2_OPTIONS_GET_TRANSPORT_IN_PROTOCOL(
                svc_client_impl->options, env);

    if (svc_client_impl->listener_manager)
    {
        return AXIS2_LISTENER_MANAGER_STOP(svc_client_impl->listener_manager,
                env, transport_in_protocol);
    }

    return AXIS2_SUCCESS;
}

const axis2_endpoint_ref_t *AXIS2_CALL
axis2_svc_client_get_own_endpoint_ref(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    const axis2_char_t *transport)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
    /*TODO:implement-issue - there's not get_own_endpoint_ref in svc_ctx*/

    return NULL;
}

const axis2_endpoint_ref_t *AXIS2_CALL
axis2_svc_client_get_target_endpoint_ref(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
    /*TODO:implement-issue - there's not get_target_endpoint_ref in svc_ctx*/

    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_svc_client_set_target_endpoint_ref(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *target_endpoint_ref)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
    /*TODO:implement-issue - there's not set_my_epr in svc_ctx*/

    return AXIS2_FAILURE;
}

axis2_svc_ctx_t* AXIS2_CALL
axis2_svc_client_get_svc_ctx(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    return svc_client_impl->svc_ctx;
}

/** private function implementation */

static axis2_bool_t
axis2_svc_client_init_transports_from_conf_ctx(
    const axis2_env_t *env,
    axis2_svc_client_impl_t *svc_client_impl,
    axis2_conf_ctx_t *conf_ctx,
    const axis2_char_t *client_home)
{

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl->conf_ctx = conf_ctx;
    if (!(svc_client_impl->conf_ctx))
    {
        svc_client_impl->conf_ctx = build_client_conf_ctx(env, client_home);
        if (!(svc_client_impl->conf_ctx))
        {
            return AXIS2_FALSE;
        }
    }

    if (!(svc_client_impl->listener_manager))
    {
        svc_client_impl->listener_manager = axis2_listener_manager_create(env);
    }

    if (!(svc_client_impl->listener_manager))
    {
        return AXIS2_FALSE;
    }


    return AXIS2_TRUE;
}

static axis2_bool_t
axis2_svc_client_init_data(
    const axis2_env_t *env,
    axis2_svc_client_impl_t *svc_client_impl)
{
    svc_client_impl->svc = NULL;

    svc_client_impl->conf_ctx = NULL;

    svc_client_impl->svc_ctx = NULL;

    svc_client_impl->options = axis2_options_create(env);
    if (!svc_client_impl->options)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FALSE;
    }

    svc_client_impl->override_options = NULL;

    svc_client_impl->headers = NULL;

    if (svc_client_impl->callback_recv)
    {
        AXIS2_CALLBACK_RECV_FREE(svc_client_impl->callback_recv, env);
        svc_client_impl->callback_recv = NULL;
    }

    svc_client_impl->callback_recv = axis2_callback_recv_create(env);
    if (!(svc_client_impl->callback_recv))
    {
        axis2_svc_client_free(&(svc_client_impl->svc_client), env);
        return AXIS2_FALSE;
    }

    return AXIS2_TRUE;
}

static void
axis2_svc_client_init_ops(
    axis2_svc_client_t *svc_client)
{

    svc_client->ops->get_svc = axis2_svc_client_get_svc;
    svc_client->ops->set_options = axis2_svc_client_set_options;
    svc_client->ops->get_options = axis2_svc_client_get_options;
    svc_client->ops->set_override_options = axis2_svc_client_set_override_options;
    svc_client->ops->get_override_options = axis2_svc_client_get_override_options;
    svc_client->ops->engage_module = axis2_svc_client_engage_module;
    svc_client->ops->disengage_module = axis2_svc_client_disengage_module;
    svc_client->ops->add_header = axis2_svc_client_add_header;
    svc_client->ops->remove_all_headers = axis2_svc_client_remove_all_headers;
    svc_client->ops->send_robust = axis2_svc_client_send_robust;
    svc_client->ops->fire_and_forget = axis2_svc_client_fire_and_forget;
    svc_client->ops->send_receive = axis2_svc_client_send_receive;
    svc_client->ops->send_receive_non_blocking = axis2_svc_client_send_receive_non_blocking;
    svc_client->ops->create_op_client = axis2_svc_client_create_op_client;
    svc_client->ops->finalize_invoke = axis2_svc_client_finalize_invoke;
    svc_client->ops->get_own_endpoint_ref = axis2_svc_client_get_own_endpoint_ref;
    svc_client->ops->get_target_endpoint_ref = axis2_svc_client_get_target_endpoint_ref;
    svc_client->ops->set_target_endpoint_ref = axis2_svc_client_set_target_endpoint_ref;
    svc_client->ops->get_svc_ctx = axis2_svc_client_get_svc_ctx;
    svc_client->ops->free_fn = axis2_svc_client_free;
    svc_client->ops->get_op_client = axis2_svc_client_get_op_client;
}

static axis2_svc_t *
axis2_svc_client_create_annonymous_svc(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env)
{
    /**
     now add anonymous operations to the axis2 service for use with the
     shortcut client API. NOTE: We only add the ones we know we'll use
     later in the convenience API; if you use
     this constructor then you can't expect any magic!
    */
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    axis2_qname_t *tmp_qname;
    axis2_svc_t *svc;
    axis2_op_t *op_out_in, *op_out_only, *op_robust_out_only;
    axis2_phases_info_t *info = NULL;

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    tmp_qname = axis2_qname_create(env, AXIS2_ANON_SERVICE, NULL, NULL);

    if (!tmp_qname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc = axis2_svc_create_with_qname(env, tmp_qname);

    if (!svc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    AXIS2_QNAME_FREE(tmp_qname, env);

    tmp_qname = axis2_qname_create(env, AXIS2_ANON_OUT_IN_OP, NULL, NULL);

    if (!tmp_qname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    op_out_in = axis2_op_create_with_qname(env, tmp_qname);
    AXIS2_QNAME_FREE(tmp_qname, env);


    tmp_qname = axis2_qname_create(env, AXIS2_ANON_OUT_ONLY_OP, NULL, NULL);

    if (!tmp_qname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    op_out_only = axis2_op_create_with_qname(env, tmp_qname);
    AXIS2_QNAME_FREE(tmp_qname, env);

    tmp_qname = axis2_qname_create(env, AXIS2_ANON_ROBUST_OUT_ONLY_OP, NULL, NULL);

    if (!tmp_qname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    op_robust_out_only = axis2_op_create_with_qname(env, tmp_qname);
    AXIS2_QNAME_FREE(tmp_qname, env);

    if (!op_out_in || !op_out_only || !op_robust_out_only)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        if (op_out_in)
        {
            axis2_op_free(op_out_in, env);
        }
        if (op_out_only)
        {
            axis2_op_free(op_out_only, env);
        }
        if (op_robust_out_only)
        {
            axis2_op_free(op_robust_out_only, env);
        }

        return NULL;
    }

    axis2_op_set_msg_exchange_pattern(op_out_in, env, AXIS2_MEP_URI_OUT_IN);
    axis2_op_set_msg_exchange_pattern(op_out_only, env, AXIS2_MEP_URI_OUT_ONLY);
    axis2_op_set_msg_exchange_pattern(op_robust_out_only, env, AXIS2_MEP_URI_ROBUST_OUT_ONLY);

    /* Setting operation phase */
    info = AXIS2_CONF_GET_PHASES_INFO(svc_client_impl->conf, env);
    AXIS2_PHASES_INFO_SET_OP_PHASES(info, env, op_out_in);
    AXIS2_PHASES_INFO_SET_OP_PHASES(info, env, op_out_only);
    AXIS2_PHASES_INFO_SET_OP_PHASES(info, env, op_robust_out_only);
    AXIS2_SVC_ADD_OP(svc, env, op_out_in);
    AXIS2_SVC_ADD_OP(svc, env, op_out_only);
    AXIS2_SVC_ADD_OP(svc, env, op_robust_out_only);
    return svc;
}

axis2_status_t AXIS2_CALL
axis2_svc_client_free(
    axis2_svc_client_t *svc_client,
    const axis2_env_t *env)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

    if (svc_client_impl->svc)
    {
        AXIS2_SVC_FREE(svc_client_impl->svc, env);
        svc_client_impl->svc = NULL;
    }

    if (svc_client_impl->callback_recv)
    {
        AXIS2_CALLBACK_RECV_FREE(svc_client_impl->callback_recv, env);
        svc_client_impl->callback_recv = NULL;
    }

    if (svc_client_impl->op_client)
    {
        AXIS2_OP_CLIENT_FREE(svc_client_impl->op_client, env);
        svc_client_impl->op_client = NULL;
    }

    if (svc_client_impl->options)
    {
        AXIS2_OPTIONS_FREE(svc_client_impl->options, env);
        svc_client_impl->options = NULL;
    }

    
    if (svc_client_impl->listener_manager)
    {
        AXIS2_LISTNER_MANAGER_FREE(svc_client_impl->listener_manager, env);
        svc_client_impl->listener_manager = NULL;
    }


    if (svc_client_impl->conf_ctx)
    {
         axis2_conf_ctx_free(svc_client_impl->conf_ctx, env);
        svc_client_impl->conf_ctx = NULL;
    }

    if (svc_client_impl->svc_client.ops)
    {
        AXIS2_FREE(env->allocator, svc_client_impl->svc_client.ops);
        svc_client_impl->svc_client.ops = NULL;
    }

    AXIS2_FREE(env->allocator, svc_client_impl);
    svc_client_impl = NULL;

    return AXIS2_SUCCESS;
}

static axis2_bool_t
axis2_svc_client_fill_soap_envelope(
    const axis2_env_t *env,
    axis2_svc_client_impl_t *svc_client_impl,
    axis2_msg_ctx_t *msg_ctx,
    const axiom_node_t *payload)
{
    const axis2_char_t *soap_version_uri;
    int soap_version;
    axiom_soap_envelope_t *envelope = NULL;

    soap_version_uri = AXIS2_OPTIONS_GET_SOAP_VERSION_URI(svc_client_impl->options, env);

    if (!soap_version_uri)
    {
        return AXIS2_FALSE;
    }

    if (AXIS2_STRCMP(soap_version_uri,
            AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI) == 0)
        soap_version = AXIOM_SOAP11;
    else
        soap_version = AXIOM_SOAP12;


    envelope = axiom_soap_envelope_create_default_soap_envelope(env, soap_version);
    if (!envelope)
    {
        return AXIS2_FALSE;
    }

    if (svc_client_impl->headers)
    {
        axiom_soap_header_t *soap_header = NULL;
        soap_header = AXIOM_SOAP_ENVELOPE_GET_HEADER(envelope, env);

        if (soap_header)
        {
            axiom_node_t *header_node = NULL;
            header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(soap_header, env);

            if (header_node)
            {
                int size = 0;
                int i = 0;
                size = axis2_array_list_size(svc_client_impl->headers, env);
                for (i = 0; i < size; i++)
                {
                    axiom_node_t *node = NULL;
                    node = axis2_array_list_get(svc_client_impl->headers, env, i);
                    if (node)
                    {
                        AXIOM_NODE_ADD_CHILD(header_node, env, node);
                    }
                }
            }
        }
    }

    if (payload)
    {
        axiom_soap_body_t *soap_body = NULL;
        soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(envelope, env);
        if (soap_body)
        {
            axiom_node_t *node = NULL;
            node = AXIOM_SOAP_BODY_GET_BASE_NODE(soap_body, env);
            if (node)
            {
                AXIOM_NODE_ADD_CHILD(node, env, (axiom_node_t *)payload);
            }
        }
    }

     axis2_msg_ctx_set_soap_envelope(msg_ctx, env, envelope);

    return AXIS2_TRUE;
}


axis2_op_client_t *AXIS2_CALL
axis2_svc_client_get_op_client(
    const axis2_svc_client_t *svc_client,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(svc_client)->op_client;
}
