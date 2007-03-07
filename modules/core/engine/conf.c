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

#include <string.h>
#include <axis2_disp.h>
#include "axis2_disp_checker.h"
#include <axis2_conf.h>
#include <axis2_dir_handler.h>
#include <axis2_dep_engine.h>
#include <axis2_arch_reader.h>
#include <axis2_core_utils.h>


typedef struct axis2_conf_impl axis2_conf_impl_t;

/**
 * @brief Engine configuration struct impl
 * Axis2 Engine Configuration impl
 */
struct axis2_conf_impl
{
    axis2_conf_t conf;
    axis2_hash_t *svc_grps;
    axis2_transport_in_desc_t *transports_in[AXIS2_TRANSPORT_ENUM_MAX];
    axis2_transport_out_desc_t *transports_out[AXIS2_TRANSPORT_ENUM_MAX];
    /**
     * Field modules
     */
    axis2_array_list_t *engaged_modules;
    /**
     * Field modules
     */
    /*axis2_hash_t *modules;*/
    /*to store all the available modules (including version)*/
    axis2_hash_t *all_modules;
    /*to store mapping between default version to module name*/
    axis2_hash_t *name_to_version_map;
    axis2_array_list_t *out_phases;
    axis2_array_list_t *in_fault_phases;
    axis2_array_list_t *out_fault_phases;

    axis2_array_list_t *in_phases_upto_and_including_post_dispatch;
    axis2_phases_info_t *phases_info;
    axis2_hash_t *all_svcs;
    axis2_hash_t *msg_recvs;
    axis2_hash_t *faulty_svcs;
    axis2_hash_t *faulty_modules;
    axis2_char_t *axis2_repo;
    axis2_dep_engine_t *dep_engine;
    axis2_array_list_t *handlers;
    axis2_bool_t enable_mtom;

    /*This is used in rampart*/    
    axis2_bool_t enable_security;
};

#define AXIS2_INTF_TO_IMPL(conf) ((axis2_conf_impl_t *)conf)

/***************************** Function prototypes ****************************/

axis2_status_t AXIS2_CALL
axis2_conf_free(
    axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_conf_add_svc_grp(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_svc_grp_t *svc_grp);

axis2_svc_grp_t *AXIS2_CALL
axis2_conf_get_svc_grp(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *svc_grp_name);

axis2_hash_t *AXIS2_CALL
axis2_conf_get_all_svc_grps(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_conf_add_svc(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_svc_t *svc);

axis2_svc_t *AXIS2_CALL
axis2_conf_get_svc(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *svc_name);

axis2_status_t AXIS2_CALL
axis2_conf_remove_svc(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *name);

axis2_status_t AXIS2_CALL
axis2_conf_add_param(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_param_t *param);

axis2_param_t *AXIS2_CALL
axis2_conf_get_param(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *name);

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_all_params(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_conf_is_param_locked(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *param_name);

/**
 * To split a given svc name into it's svc grp name and svc name.
 * if the svc name is foo:bar then svc grp name is "foo" and
 * svc name is "bar" but if the svc name is only the "foo" we assume
 * svc grp name is "foo" and svc name is "foo"
 * meaning foo := foo:foo
 * @param svc_name
 * @return svc name and grp name
 */
axis2_status_t
split_svc_name(
    const axis2_env_t *env,
    axis2_char_t *svc_name,
    axis2_char_t **svc_name_st);

axis2_transport_in_desc_t *AXIS2_CALL
axis2_conf_get_transport_in(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const AXIS2_TRANSPORT_ENUMS qname);

axis2_status_t AXIS2_CALL
axis2_conf_add_transport_in(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_transport_in_desc_t *transport,
    const AXIS2_TRANSPORT_ENUMS trans_enum);

axis2_transport_out_desc_t *AXIS2_CALL
axis2_conf_get_transport_out(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const AXIS2_TRANSPORT_ENUMS trans_enum);

axis2_status_t AXIS2_CALL
axis2_conf_add_transport_out(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_transport_out_desc_t *transport,
    const AXIS2_TRANSPORT_ENUMS trans_enum);

axis2_transport_in_desc_t **AXIS2_CALL
axis2_conf_get_all_in_transports(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_transport_out_desc_t **AXIS2_CALL
axis2_conf_get_all_out_transports(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_module_desc_t *AXIS2_CALL
axis2_conf_get_module(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_qname_t *qname);

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_all_engaged_modules(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_in_phases_upto_and_including_post_dispatch(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_out_flow(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_in_fault_flow(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_out_fault_flow(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
axis2_conf_get_all_faulty_svcs(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
axis2_conf_get_all_faulty_modules(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

/*to get all the services in the system */
axis2_hash_t *AXIS2_CALL
axis2_conf_get_all_svcs(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_conf_is_engaged(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_qname_t *module_name);

axis2_phases_info_t *AXIS2_CALL
axis2_conf_get_phases_info(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_conf_set_phases_info(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_phases_info_t *phases_info);
axis2_status_t AXIS2_CALL

axis2_conf_add_msg_recv(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *key,
    axis2_msg_recv_t *msg_recv);

axis2_msg_recv_t *AXIS2_CALL
axis2_conf_get_msg_recv(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_char_t *key);

axis2_status_t AXIS2_CALL
axis2_conf_set_out_phases(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_array_list_t *out_phases);

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_out_phases(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_conf_set_in_fault_phases(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_array_list_t *list);

axis2_status_t AXIS2_CALL
axis2_conf_set_out_fault_phases(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_array_list_t *list);

axis2_hash_t *AXIS2_CALL
axis2_conf_get_all_modules(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_conf_add_module(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_module_desc_t *module);

axis2_status_t AXIS2_CALL
axis2_conf_set_default_dispatchers(
    axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_conf_set_dispatch_phase(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_phase_t *dispatch);

axis2_status_t AXIS2_CALL
axis2_conf_set_dep_engine(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_dep_engine_t *dep_engine);


axis2_status_t AXIS2_CALL
axis2_conf_engage_module(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_qname_t *module_ref);

const axis2_char_t *AXIS2_CALL
axis2_conf_get_repo(
    const axis2_conf_t *conf,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_conf_set_repo(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_char_t *repo_path);

const axis2_char_t *AXIS2_CALL
axis2_conf_get_default_module_version(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *module_name);

axis2_module_desc_t *AXIS2_CALL
axis2_conf_get_default_module(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *module_name);

axis2_status_t AXIS2_CALL
axis2_conf_add_default_module_version(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *module_name,
    const axis2_char_t *module_version);

axis2_status_t AXIS2_CALL
axis2_conf_engage_module_with_version(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *module_name,
    const axis2_char_t *version_id);

axis2_conf_t *AXIS2_CALL
axis2_conf_create(
    const axis2_env_t *env)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_phase_t *phase = NULL;
    int i = 0;

    AXIS2_ENV_CHECK(env, NULL);

    config_impl = (axis2_conf_impl_t *) AXIS2_MALLOC(env->allocator
            , sizeof(axis2_conf_impl_t));

    if (NULL == config_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    config_impl->conf.param_container = NULL;
    config_impl->svc_grps = NULL;
    config_impl->engaged_modules = NULL;
    config_impl->in_phases_upto_and_including_post_dispatch = NULL;
    config_impl->out_phases = NULL;
    config_impl->in_fault_phases = NULL;
    config_impl->out_fault_phases = NULL;
    config_impl->phases_info = NULL;
    config_impl->all_svcs = NULL;
    config_impl->msg_recvs = NULL;
    config_impl->faulty_svcs = NULL;
    config_impl->faulty_modules = NULL;
    config_impl->axis2_repo = NULL;
    config_impl->dep_engine = NULL;
    config_impl->all_modules = NULL;
    config_impl->name_to_version_map = NULL;
    config_impl->handlers = NULL;
    config_impl->conf.ops = NULL;
    config_impl->enable_mtom = AXIS2_FALSE;
    config_impl->enable_security = AXIS2_FALSE;

    config_impl->conf.param_container = (axis2_param_container_t *)
            axis2_param_container_create(env);
    if (NULL == config_impl->conf.param_container)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    config_impl->svc_grps = axis2_hash_make(env);
    if (NULL == config_impl->svc_grps)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    for (i = 0; i < AXIS2_TRANSPORT_ENUM_MAX; i++)
    {
        config_impl->transports_in[i] = NULL;
    }

    for (i = 0; i < AXIS2_TRANSPORT_ENUM_MAX; i++)
    {
        config_impl->transports_out[i] = NULL;
    }

    config_impl->engaged_modules = axis2_array_list_create(env, 0);
    if (NULL == config_impl->engaged_modules)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    config_impl->handlers = axis2_array_list_create(env, 0);
    if (NULL == config_impl->handlers)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    config_impl->in_phases_upto_and_including_post_dispatch =
        axis2_array_list_create(env, 0);
    if (NULL == config_impl->in_phases_upto_and_including_post_dispatch)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    else
    {
        axis2_disp_t *uri_dispatch = NULL;

        phase = axis2_phase_create(env, AXIS2_PHASE_TRANSPORT_IN);
        if (NULL == phase)
        {
            axis2_conf_free(&(config_impl->conf), env);
            return NULL;
        }
        /*Incase of using security we need to find the service/operation parameters before the dispatch phase.
         *This is required to give parameters to the security inflow.*/
        uri_dispatch = axis2_req_uri_disp_create(env);
        if (uri_dispatch)
        {
                axis2_handler_t *handler = NULL;
                handler = AXIS2_DISP_GET_BASE(uri_dispatch, env);
                AXIS2_DISP_FREE(uri_dispatch, env);
                AXIS2_PHASE_ADD_HANDLER_AT(phase, env, 0, handler);
                axis2_array_list_add(config_impl->handlers, env, AXIS2_HANDLER_GET_HANDLER_DESC(handler, env));
                handler = NULL;
        }

        status = axis2_array_list_add(config_impl->
                in_phases_upto_and_including_post_dispatch, env, phase);
        if (AXIS2_FAILURE == status)
        {
            axis2_conf_free(&(config_impl->conf), env);
            AXIS2_PHASE_FREE(phase, env);
            return NULL;

        }
        phase = axis2_phase_create(env, AXIS2_PHASE_PRE_DISPATCH);
        if (NULL == phase)
        {
            axis2_conf_free(&(config_impl->conf), env);
            return NULL;
        }
        status = axis2_array_list_add(config_impl->
                in_phases_upto_and_including_post_dispatch, env, phase);
        if (AXIS2_FAILURE == status)
        {
            axis2_conf_free(&(config_impl->conf), env);
            AXIS2_PHASE_FREE(phase, env);
            return NULL;
        }
    }


    config_impl->all_svcs = axis2_hash_make(env);
    if (NULL == config_impl->all_svcs)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    config_impl->msg_recvs = axis2_hash_make(env);
    if (NULL == config_impl->msg_recvs)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    config_impl->faulty_svcs = axis2_hash_make(env);
    if (NULL == config_impl->faulty_svcs)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    config_impl->faulty_modules = axis2_hash_make(env);
    if (NULL == config_impl->faulty_modules)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    config_impl->all_modules = axis2_hash_make(env);
    if (NULL == config_impl->all_modules)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    config_impl->name_to_version_map = axis2_hash_make(env);
    if (NULL == config_impl->name_to_version_map)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    config_impl->conf.ops = AXIS2_MALLOC(env->allocator,
            sizeof(axis2_conf_ops_t));
    if (NULL == config_impl->conf.ops)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    config_impl->conf.ops->free = axis2_conf_free;
    config_impl->conf.ops->add_svc_grp =
        axis2_conf_add_svc_grp;
    config_impl->conf.ops->get_svc_grp =
        axis2_conf_get_svc_grp;
    config_impl->conf.ops->get_all_svc_grps =
        axis2_conf_get_all_svc_grps;
    config_impl->conf.ops->add_svc = axis2_conf_add_svc;
    config_impl->conf.ops->get_svc = axis2_conf_get_svc;
    config_impl->conf.ops->remove_svc =
        axis2_conf_remove_svc;
    config_impl->conf.ops->add_param =
        axis2_conf_add_param;
    config_impl->conf.ops->get_param =
        axis2_conf_get_param;
    config_impl->conf.ops->get_all_params =
        axis2_conf_get_all_params;
    config_impl->conf.ops->is_param_locked =
        axis2_conf_is_param_locked;
    config_impl->conf.ops->get_transport_in =
        axis2_conf_get_transport_in;
    config_impl->conf.ops->add_transport_in =
        axis2_conf_add_transport_in;
    config_impl->conf.ops->get_transport_out =
        axis2_conf_get_transport_out;
    config_impl->conf.ops->add_transport_out =
        axis2_conf_add_transport_out;
    config_impl->conf.ops->get_all_in_transports =
        axis2_conf_get_all_in_transports;
    config_impl->conf.ops->get_all_out_transports =
        axis2_conf_get_all_out_transports;
    config_impl->conf.ops->get_module =
        axis2_conf_get_module;
    config_impl->conf.ops->get_all_engaged_modules =
        axis2_conf_get_all_engaged_modules;
    config_impl->conf.ops->get_in_phases_upto_and_including_post_dispatch =
        axis2_conf_get_in_phases_upto_and_including_post_dispatch;
    config_impl->conf.ops->get_out_flow =
        axis2_conf_get_out_flow;
    config_impl->conf.ops->get_in_fault_flow =
        axis2_conf_get_in_fault_flow;
    config_impl->conf.ops->get_out_fault_flow =
        axis2_conf_get_out_fault_flow;
    config_impl->conf.ops->get_all_faulty_svcs =
        axis2_conf_get_all_faulty_svcs;
    config_impl->conf.ops->get_all_faulty_modules =
        axis2_conf_get_all_faulty_modules;
    config_impl->conf.ops->get_all_svcs =
        axis2_conf_get_all_svcs;
    config_impl->conf.ops->is_engaged =
        axis2_conf_is_engaged;
    config_impl->conf.ops->get_phases_info =
        axis2_conf_get_phases_info;
    config_impl->conf.ops->set_phases_info =
        axis2_conf_set_phases_info;
    config_impl->conf.ops->add_msg_recv =
        axis2_conf_add_msg_recv;
    config_impl->conf.ops->get_msg_recv =
        axis2_conf_get_msg_recv;
    config_impl->conf.ops->set_out_phases =
        axis2_conf_set_out_phases;
    config_impl->conf.ops->get_out_phases =
        axis2_conf_get_out_phases;
    config_impl->conf.ops->set_in_fault_phases =
        axis2_conf_set_in_fault_phases;
    config_impl->conf.ops->set_out_fault_phases =
        axis2_conf_set_out_fault_phases;
    config_impl->conf.ops->get_all_modules =
        axis2_conf_get_all_modules;
    config_impl->conf.ops->add_module =
        axis2_conf_add_module;
    config_impl->conf.ops->set_default_dispatchers =
        axis2_conf_set_default_dispatchers;
    config_impl->conf.ops->set_dispatch_phase = axis2_conf_set_dispatch_phase;
    config_impl->conf.ops->get_repo = axis2_conf_get_repo;
    config_impl->conf.ops->set_repo = axis2_conf_set_repo;
    config_impl->conf.ops->engage_module = axis2_conf_engage_module;
    config_impl->conf.ops->set_dep_engine = axis2_conf_set_dep_engine;

    config_impl->conf.ops->get_default_module_version =
        axis2_conf_get_default_module_version;
    config_impl->conf.ops->get_default_module = axis2_conf_get_default_module;
    config_impl->conf.ops->add_default_module_version =
        axis2_conf_add_default_module_version;
    config_impl->conf.ops->engage_module_with_version =
        axis2_conf_engage_module_with_version;

    return &(config_impl->conf);
}

axis2_status_t AXIS2_CALL
axis2_conf_free(
    axis2_conf_t *conf,
    const axis2_env_t *env)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    int i = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(conf);

    if (conf->param_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(conf->param_container, env);
        conf->param_container = NULL;
    }

    if (config_impl->svc_grps)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first(config_impl->svc_grps, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_svc_grp_t * svc_grp = NULL;
            axis2_hash_this(hi, NULL, NULL, &val);
            svc_grp = (axis2_svc_grp_t *) val;
            if (svc_grp)
                AXIS2_SVC_GRP_FREE(svc_grp, env);

            val = NULL;
            svc_grp = NULL;

        }
        axis2_hash_free(config_impl->svc_grps, env);
        config_impl->svc_grps = NULL;
    }

    for (i = 0; i < AXIS2_TRANSPORT_ENUM_MAX; i++)
    {
        if (config_impl->transports_in[i])
        {
            AXIS2_TRANSPORT_IN_DESC_FREE(config_impl->transports_in[i], env);
            config_impl->transports_in[i] = NULL;
        }
    }
    
    for (i = 0; i < AXIS2_TRANSPORT_ENUM_MAX; i++)
    {
        if (config_impl->transports_out[i])
        {
            AXIS2_TRANSPORT_OUT_DESC_FREE(config_impl->transports_out[i], env);
            config_impl->transports_out[i] = NULL;
        }
    }

    if (config_impl->dep_engine)
    {
        AXIS2_DEP_ENGINE_FREE(config_impl->dep_engine, env);
        config_impl->dep_engine = NULL;
    }


    if (config_impl->all_modules)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first(config_impl->all_modules, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_module_desc_t * module_desc = NULL;

            axis2_hash_this(hi, NULL, NULL, &val);
            module_desc = (axis2_module_desc_t *) val;
            if (module_desc)
            {
                AXIS2_MODULE_DESC_FREE(module_desc, env);
                module_desc = NULL;
            }

            val = NULL;
        }
        axis2_hash_free(config_impl->all_modules, env);
        config_impl->all_modules = NULL;
    }

    if (config_impl->name_to_version_map)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first(config_impl->name_to_version_map, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_char_t * module_ver = NULL;

            axis2_hash_this(hi, NULL, NULL, &val);
            module_ver = (axis2_char_t *) val;
            if (module_ver)
            {
                AXIS2_FREE(env->allocator, module_ver);
                module_ver = NULL;
            }

            val = NULL;
        }
        axis2_hash_free(config_impl->name_to_version_map, env);
        config_impl->name_to_version_map = NULL;
    }

    if (config_impl->engaged_modules)
    {
        int i = 0;
        for (i = 0; i < axis2_array_list_size(config_impl->engaged_modules,
                env); i++)
        {
            axis2_qname_t *module_desc_qname = NULL;
            module_desc_qname = (axis2_qname_t *)
                    axis2_array_list_get(config_impl->engaged_modules, env, i);
            if (module_desc_qname)
                AXIS2_QNAME_FREE(module_desc_qname, env);

            module_desc_qname = NULL;
        }
        axis2_array_list_free(config_impl->engaged_modules, env);
        config_impl->engaged_modules = NULL;
    }

    if (config_impl->out_phases)
    {
        int i = 0;
        for (i = 0; i < axis2_array_list_size(config_impl->out_phases, env); i++)
        {
            axis2_phase_t *phase = NULL;
            phase = (axis2_phase_t *)
                    axis2_array_list_get(config_impl->out_phases, env, i);
            if (phase)
                AXIS2_PHASE_FREE(phase, env);

            phase = NULL;

        }
        axis2_array_list_free(config_impl->out_phases, env);
        config_impl->out_phases = NULL;
    }

    if (config_impl->in_fault_phases)
    {
        int i = 0;
        for (i = 0; i < axis2_array_list_size(config_impl->in_fault_phases, env); i++)
        {
            axis2_phase_t *phase = NULL;
            phase = (axis2_phase_t *)
                    axis2_array_list_get(config_impl->in_fault_phases, env, i);
            if (phase)
                AXIS2_PHASE_FREE(phase, env);

            phase = NULL;

        }
        axis2_array_list_free(config_impl->in_fault_phases, env);
        config_impl->in_fault_phases = NULL;
    }

    if (config_impl->out_fault_phases)
    {
        /* No need of the following commented lines. Phases are freed in 
         * phase_info 
         */
        /*int i = 0;
        for (i = 0; i < axis2_array_list_size(config_impl->out_fault_phases, env); i++)
        {
            axis2_phase_t *phase = NULL;
            phase = (axis2_phase_t *)
                    axis2_array_list_get(config_impl->out_fault_phases, env, i);

            if (phase)
                AXIS2_PHASE_FREE(phase, env);

            phase = NULL;

        }*/
        axis2_array_list_free(config_impl->out_fault_phases, env);
        config_impl->out_fault_phases = NULL;
    }

    if (config_impl->in_phases_upto_and_including_post_dispatch)
    {
        int i = 0;
        for (i = 0; i < axis2_array_list_size(config_impl->
                in_phases_upto_and_including_post_dispatch, env); i++)
        {
            axis2_phase_t *phase = NULL;
            phase = (axis2_phase_t *) axis2_array_list_get(config_impl->
                    in_phases_upto_and_including_post_dispatch, env, i);

            if (phase)
                AXIS2_PHASE_FREE(phase, env);
            phase = NULL;

        }
        axis2_array_list_free(config_impl->
                in_phases_upto_and_including_post_dispatch, env);
        config_impl->in_phases_upto_and_including_post_dispatch = NULL;
    }

    if (config_impl->all_svcs)
    {
        axis2_hash_free(config_impl->all_svcs, env);
        config_impl->all_svcs = NULL;
    }

    if (config_impl->msg_recvs)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first(config_impl->msg_recvs, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_msg_recv_t * msg_recv = NULL;
            axis2_hash_this(hi, NULL, NULL, &val);
            msg_recv = (axis2_msg_recv_t *) val;
            if (msg_recv)
            {
                AXIS2_MSG_RECV_FREE(msg_recv, env);
                msg_recv = NULL;
            }

            val = NULL;
        }
        axis2_hash_free(config_impl->msg_recvs, env);
        config_impl->msg_recvs = NULL;
    }

    if (config_impl->faulty_svcs)
    {
        axis2_hash_free(config_impl->faulty_svcs, env);
        config_impl->faulty_svcs = NULL;
    }

    if (config_impl->faulty_modules)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first(config_impl->faulty_modules, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_module_desc_t * module_desc = NULL;
            axis2_hash_this(hi, NULL, NULL, &val);
            module_desc = (axis2_module_desc_t *) val;
            if (module_desc)
                AXIS2_MODULE_DESC_FREE(module_desc, env);

            val = NULL;
            module_desc = NULL;

        }
        axis2_hash_free(config_impl->faulty_modules, env);
        config_impl->faulty_modules = NULL;
    }

    if (config_impl->handlers)
    {
        int i = 0;
        for (i = 0; i < axis2_array_list_size(config_impl->handlers, env); i++)
        {
            axis2_handler_desc_t *handler_desc = NULL;
            handler_desc = (axis2_handler_desc_t *)
                    axis2_array_list_get(config_impl->handlers, env, i);
            if (handler_desc)
                axis2_handler_desc_free(handler_desc, env);

            handler_desc = NULL;

        }
        axis2_array_list_free(config_impl->handlers, env);
        config_impl->handlers = NULL;
    }

    if (config_impl->axis2_repo)
    {
        AXIS2_FREE(env->allocator, config_impl->axis2_repo);
        config_impl->axis2_repo = NULL;
    }

    if (conf->ops)
    {
        AXIS2_FREE(env->allocator, conf->ops);
        conf->ops = NULL;
    }

    if (config_impl)
    {
        AXIS2_FREE(env->allocator, config_impl);
        config_impl = NULL;
    }

    return status;
}

axis2_status_t AXIS2_CALL
axis2_conf_add_svc_grp(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_svc_grp_t *svc_grp)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_hash_t *svcs = NULL;
    axis2_hash_index_t *index_i = NULL;
    axis2_char_t *svc_name = NULL;
    const axis2_char_t *svc_grp_name = NULL;
    int k = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_grp, AXIS2_FAILURE);

    config_impl = AXIS2_INTF_TO_IMPL(conf);
    svcs = AXIS2_SVC_GRP_GET_ALL_SVCS(svc_grp, env);
    if (!config_impl->all_svcs)
    {
        config_impl->all_svcs = axis2_hash_make(env);
        if (!config_impl->all_svcs)
            return AXIS2_FAILURE;
    }
    k = axis2_hash_count(svcs);
    index_i = axis2_hash_first(svcs, env);
    while (index_i)
    {
        void *value = NULL;
        axis2_svc_t *desc = NULL;
        axis2_char_t *svc_name2 = NULL;
        const axis2_qname_t *svc_qname = NULL;

        axis2_hash_this(index_i, NULL, NULL, &value);
        desc = (axis2_svc_t *) value;
        svc_qname = AXIS2_SVC_GET_QNAME(desc, env);
        svc_name = AXIS2_QNAME_GET_LOCALPART(svc_qname, env);

        svc_name2 = axis2_hash_get(config_impl->all_svcs, svc_name,
                AXIS2_HASH_KEY_STRING);
        /* no two service names deployed in the engine can be same */
        if (svc_name2)
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_TWO_SVCS_CANNOT_HAVE_SAME_NAME, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        index_i = axis2_hash_next(env, index_i);
    }

    svcs = AXIS2_SVC_GRP_GET_ALL_SVCS(svc_grp, env);
    index_i = axis2_hash_first(svcs, env);
    while (index_i)
    {
        void *value = NULL;
        axis2_svc_t *desc = NULL;

        axis2_hash_this(index_i, NULL, NULL, &value);
        desc = (axis2_svc_t *) value;
        svc_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_SVC_GET_QNAME(desc, env), env);
        axis2_hash_set(config_impl->all_svcs, svc_name, AXIS2_HASH_KEY_STRING,
                desc);
        index_i = axis2_hash_next(env, index_i);
    }

    svc_grp_name = AXIS2_SVC_GRP_GET_NAME(svc_grp, env);
    if (!config_impl->svc_grps)
    {
        config_impl->svc_grps = axis2_hash_make(env);
        if (!config_impl->svc_grps)
            return AXIS2_FAILURE;
    }
    axis2_hash_set(config_impl->svc_grps, svc_grp_name,
            AXIS2_HASH_KEY_STRING, svc_grp);

    return AXIS2_SUCCESS;
}

axis2_svc_grp_t *AXIS2_CALL
axis2_conf_get_svc_grp(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *svc_grp_name)
{
    axis2_conf_impl_t *config_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, svc_grp_name, NULL);

    config_impl = AXIS2_INTF_TO_IMPL(conf);
    if (!config_impl->svc_grps)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE_CONF,
                AXIS2_FAILURE);
        return NULL;
    }
    return (axis2_svc_grp_t *)(axis2_hash_get(config_impl->svc_grps,
            svc_grp_name, AXIS2_HASH_KEY_STRING));
}

axis2_hash_t *AXIS2_CALL
axis2_conf_get_all_svc_grps(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->svc_grps;
}

axis2_status_t AXIS2_CALL
axis2_conf_add_svc(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_svc_t *svc)
{
    axis2_svc_grp_t *svc_grp = NULL;
    const axis2_qname_t *svc_grp_qname = NULL;
    axis2_char_t *svc_grp_name = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc, AXIS2_FAILURE);

    svc_grp = axis2_svc_grp_create(env);

    if (NULL == svc_grp)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE)
        return AXIS2_FAILURE;
    }

    svc_grp_qname = AXIS2_SVC_GET_QNAME(svc, env);
    if (NULL == svc_grp_qname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE_SVC,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    svc_grp_name = AXIS2_QNAME_GET_LOCALPART(svc_grp_qname, env);
    if (!svc_grp_name)
    {
        return AXIS2_FAILURE;
    }
    status = AXIS2_SVC_GRP_SET_NAME(svc_grp, env, svc_grp_name);
    if (AXIS2_FAILURE == status)
    {
        return status;
    }
    status = AXIS2_SVC_GRP_SET_PARENT(svc_grp, env, conf);
    if (AXIS2_FAILURE == status)
    {
        return status;
    }
    status = AXIS2_SVC_GRP_ADD_SVC(svc_grp, env, svc);
    if (AXIS2_FAILURE == status)
    {
        return status;
    }
    status = axis2_conf_add_svc_grp(conf, env, svc_grp);

    return status;
}

axis2_svc_t *AXIS2_CALL
axis2_conf_get_svc(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t* svc_name)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, svc_name, NULL);

    return axis2_hash_get(AXIS2_INTF_TO_IMPL(conf)->all_svcs, svc_name,
            AXIS2_HASH_KEY_STRING);
}

axis2_status_t AXIS2_CALL
axis2_conf_remove_svc(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *svc_name)
{
    axis2_conf_impl_t *config_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_name, AXIS2_FAILURE);

    axis2_hash_set(config_impl->all_svcs, svc_name, AXIS2_HASH_KEY_STRING,
            NULL);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_add_param(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_param_t *param)
{
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, param, AXIS2_FAILURE);

    if (AXIS2_TRUE == axis2_conf_is_param_locked(conf, env,
            AXIS2_PARAM_GET_NAME(param, env)))
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        status = AXIS2_PARAM_CONTAINER_ADD_PARAM(conf->param_container, env,
                param);
    }
    return status;
}

axis2_param_t *AXIS2_CALL
axis2_conf_get_param(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *name)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, name, NULL);

    if (NULL == conf->param_container)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FAILURE);
        return NULL;
    }

    return AXIS2_PARAM_CONTAINER_GET_PARAM(conf->param_container, env,
            name);

}

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_all_params(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_PARAM_CONTAINER_GET_PARAMS(conf->param_container, env);

}

axis2_bool_t AXIS2_CALL
axis2_conf_is_param_locked(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *param_name)
{
    axis2_param_t *param = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param_name, AXIS2_FALSE);

    param = axis2_conf_get_param(conf, env, param_name);
    return (param && AXIS2_TRUE == AXIS2_PARAM_IS_LOCKED(param, env));
}

axis2_transport_in_desc_t *AXIS2_CALL
axis2_conf_get_transport_in(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const AXIS2_TRANSPORT_ENUMS trans_enum)
{
    axis2_conf_impl_t *config_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    config_impl = AXIS2_INTF_TO_IMPL(conf);

    return (axis2_transport_in_desc_t *) config_impl->transports_in[trans_enum];
}

axis2_status_t AXIS2_CALL
axis2_conf_add_transport_in(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_transport_in_desc_t *transport,
    const AXIS2_TRANSPORT_ENUMS trans_enum)
{
    axis2_conf_impl_t *config_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, transport, AXIS2_FAILURE);

    config_impl = AXIS2_INTF_TO_IMPL(conf);

    config_impl->transports_in[trans_enum] = transport;

    return AXIS2_SUCCESS;

}

axis2_transport_out_desc_t *AXIS2_CALL
axis2_conf_get_transport_out(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const AXIS2_TRANSPORT_ENUMS trans_enum)
{
    axis2_conf_impl_t *config_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    config_impl = AXIS2_INTF_TO_IMPL(conf);

    return config_impl->transports_out[trans_enum];
}

axis2_status_t AXIS2_CALL
axis2_conf_add_transport_out(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_transport_out_desc_t *transport,
    const AXIS2_TRANSPORT_ENUMS trans_enum)
{
    axis2_conf_impl_t *config_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, transport, AXIS2_FAILURE);

    config_impl = AXIS2_INTF_TO_IMPL(conf);

    config_impl->transports_out[trans_enum] = transport;

    return AXIS2_SUCCESS;
}

axis2_transport_in_desc_t ** AXIS2_CALL
axis2_conf_get_all_in_transports(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->transports_in;
}

axis2_module_desc_t *AXIS2_CALL
axis2_conf_get_module(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    axis2_conf_impl_t *conf_impl = NULL;
    axis2_char_t *name = NULL;
    axis2_module_desc_t *ret = NULL;
    axis2_char_t *module_name = NULL;
    axis2_qname_t *mod_qname = NULL;
    const axis2_char_t *def_mod_ver = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);
    conf_impl = AXIS2_INTF_TO_IMPL(conf);

    name = AXIS2_QNAME_TO_STRING((axis2_qname_t *)qname, env);
    ret = (axis2_module_desc_t *) axis2_hash_get(conf_impl->all_modules,
            name, AXIS2_HASH_KEY_STRING);
    if (ret)
    {
        return ret;
    }
    module_name = AXIS2_QNAME_GET_LOCALPART(qname, env);
    if (NULL == module_name)
    {
        return NULL;
    }
    def_mod_ver = AXIS2_CONF_GET_DEFAULT_MODULE_VERSION(conf, env, module_name);
    mod_qname = axis2_core_utils_get_module_qname(env, name, def_mod_ver);
    if (NULL == mod_qname)
    {
        return NULL;
    }
    name = AXIS2_QNAME_TO_STRING(mod_qname, env);
    ret = (axis2_module_desc_t *) axis2_hash_get(conf_impl->all_modules,
            name, AXIS2_HASH_KEY_STRING);
    AXIS2_QNAME_FREE(mod_qname, env);
    mod_qname = NULL;
    return ret;
}

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_all_engaged_modules(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    axis2_conf_impl_t *conf_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    conf_impl = AXIS2_INTF_TO_IMPL(conf);

    return conf_impl->engaged_modules;
}

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_in_phases_upto_and_including_post_dispatch(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->
            in_phases_upto_and_including_post_dispatch;
}

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_out_flow(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->out_phases;
}

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_in_fault_flow(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->in_fault_phases;
}

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_out_fault_flow(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->out_fault_phases;
}

axis2_transport_out_desc_t **AXIS2_CALL
axis2_conf_get_all_out_transports(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->transports_out;
}

axis2_hash_t *AXIS2_CALL
axis2_conf_get_all_faulty_svcs(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_INTF_TO_IMPL(conf)->faulty_svcs;
}

axis2_hash_t *AXIS2_CALL
axis2_conf_get_all_faulty_modules(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->faulty_modules;
}

axis2_hash_t *AXIS2_CALL
axis2_conf_get_all_svcs(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_hash_t *sgs = NULL;
    axis2_hash_index_t *index_i = NULL;
    axis2_hash_index_t *index_j = NULL;
    void *value = NULL;
    void *value2 = NULL;
    axis2_svc_grp_t *axis_svc_grp = NULL;
    axis2_hash_t *svcs = NULL;
    axis2_svc_t *svc = NULL;
    axis2_char_t *svc_name = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    config_impl = AXIS2_INTF_TO_IMPL(conf);
    sgs = axis2_conf_get_all_svc_grps(conf, env);
    index_i = axis2_hash_first(sgs, env);
    while (index_i)
    {
        axis2_hash_this(index_i, NULL, NULL, &value);
        axis_svc_grp = (axis2_svc_grp_t *) value;
        svcs = AXIS2_SVC_GRP_GET_ALL_SVCS(axis_svc_grp, env);
        index_j = axis2_hash_first(svcs, env);
        while (index_j)
        {
            axis2_hash_this(index_j, NULL, NULL, &value2);
            svc = (axis2_svc_t *) value2;
            svc_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_SVC_GET_QNAME(svc, env), env);
            axis2_hash_set(config_impl->all_svcs, svc_name,
                    AXIS2_HASH_KEY_STRING, svc);

            index_j = axis2_hash_next(env, index_j);
        }

        index_i = axis2_hash_next(env, index_i);
    }
    return config_impl->all_svcs;
}

axis2_bool_t AXIS2_CALL
axis2_conf_is_engaged(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_qname_t *module_name)
{
    axis2_conf_impl_t *config_impl = NULL;
    const axis2_qname_t *def_mod_qname = NULL;
    axis2_module_desc_t *def_mod = NULL;
    int i = 0;
    int size = 0;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, module_name, AXIS2_FALSE);
    config_impl = AXIS2_INTF_TO_IMPL(conf);

    def_mod = AXIS2_CONF_GET_DEFAULT_MODULE(conf, env,
            AXIS2_QNAME_GET_LOCALPART(module_name, env));
    if (def_mod)
    {
        def_mod_qname = AXIS2_MODULE_DESC_GET_QNAME(def_mod, env);
    }
    size = axis2_array_list_size(config_impl->engaged_modules, env);
    for (i = 0; i < size; i++)
    {
        axis2_qname_t *qname = NULL;


        qname = (axis2_qname_t *) axis2_array_list_get(config_impl->
                engaged_modules, env, i);

        if (AXIS2_TRUE == AXIS2_QNAME_EQUALS(module_name, env, qname) ||
                (def_mod_qname && AXIS2_TRUE == AXIS2_QNAME_EQUALS(
                            def_mod_qname, env, qname)))
        {
            return AXIS2_TRUE;
        }
    }
    return AXIS2_FALSE;
}

axis2_phases_info_t *AXIS2_CALL
axis2_conf_get_phases_info(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return (AXIS2_INTF_TO_IMPL(conf))->phases_info;
}

axis2_status_t AXIS2_CALL
axis2_conf_set_phases_info(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_phases_info_t *phases_info)
{
    axis2_conf_impl_t *config_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, phases_info, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(conf);

    if (config_impl->phases_info)
    {
        AXIS2_PHASES_INFO_FREE(phases_info, env);
        config_impl->phases_info = NULL;
    }
    config_impl->phases_info = phases_info;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_add_msg_recv(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *key,
    axis2_msg_recv_t *msg_recv)
{
    axis2_conf_impl_t *config_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_recv, AXIS2_FAILURE);

    config_impl = AXIS2_INTF_TO_IMPL(conf);

    if (!config_impl->msg_recvs)
    {
        config_impl->msg_recvs = axis2_hash_make(env);
        if (!config_impl->msg_recvs)
            return AXIS2_FAILURE;
    }
    axis2_hash_set(config_impl->msg_recvs, key, AXIS2_HASH_KEY_STRING, msg_recv);
    return AXIS2_SUCCESS;
}

axis2_msg_recv_t *AXIS2_CALL
axis2_conf_get_msg_recv(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_char_t *key)
{
    AXIS2_ENV_CHECK(env, NULL);

    return (axis2_msg_recv_t *) axis2_hash_get(AXIS2_INTF_TO_IMPL(
                conf)->msg_recvs, key, AXIS2_HASH_KEY_STRING);
}

axis2_status_t AXIS2_CALL
axis2_conf_set_out_phases(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_array_list_t *out_phases)
{
    axis2_conf_impl_t *config_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, out_phases, AXIS2_FAILURE);

    config_impl = AXIS2_INTF_TO_IMPL(conf);
    if (config_impl->out_phases)
    {
        axis2_array_list_free(config_impl->out_phases, env);
        config_impl->out_phases = NULL;
    }
    config_impl->out_phases = out_phases;
    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL
axis2_conf_get_out_phases(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->out_phases;
}

axis2_status_t AXIS2_CALL
axis2_conf_set_in_fault_phases(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_array_list_t *list)
{
    axis2_conf_impl_t *config_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, list, AXIS2_FAILURE);

    config_impl = AXIS2_INTF_TO_IMPL(conf);
    if (config_impl->in_fault_phases)
    {
        axis2_array_list_free(config_impl->in_fault_phases, env);
        config_impl->in_fault_phases = NULL;
    }
    config_impl->in_fault_phases = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_set_out_fault_phases(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_array_list_t *list)
{
    axis2_conf_impl_t *config_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, list, AXIS2_FAILURE);

    config_impl = AXIS2_INTF_TO_IMPL(conf);
    if (config_impl->out_fault_phases)
    {
        axis2_array_list_free(config_impl->out_fault_phases, env);
        config_impl->out_fault_phases = NULL;
    }
    config_impl->out_fault_phases = list;
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_conf_get_all_modules(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_INTF_TO_IMPL(conf)->all_modules;
}

axis2_status_t AXIS2_CALL
axis2_conf_add_module(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_module_desc_t *module)
{
    axis2_conf_impl_t *config_impl = NULL;
    const axis2_qname_t *module_qname = NULL;

    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module, AXIS2_FAILURE);

    config_impl = AXIS2_INTF_TO_IMPL(conf);

    status = AXIS2_MODULE_DESC_SET_PARENT(module, env, conf);
    if (AXIS2_FAILURE == status)
    {
        return status;
    }

    if (!config_impl->all_modules)
    {
        return AXIS2_FAILURE;
    }
    module_qname = AXIS2_MODULE_DESC_GET_QNAME(module, env);
    if (module_qname)
    {
        axis2_char_t *module_name = NULL;

        module_name = AXIS2_QNAME_TO_STRING((axis2_qname_t *)module_qname, env);
        axis2_hash_set(config_impl->all_modules, module_name,
                AXIS2_HASH_KEY_STRING, module);
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_set_default_dispatchers(
    axis2_conf_t *conf,
    const axis2_env_t *env)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_phase_t *dispatch = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_disp_t *add_dispatch = NULL;
    /*axis2_disp_t *uri_dispatch = NULL;*/
    axis2_disp_t *soap_action_based_dispatch = NULL;
    axis2_disp_t *soap_msg_body_based_dispatch = NULL;
    axis2_handler_t *handler = NULL;
    axis2_phase_t *post_dispatch = NULL;
    axis2_disp_checker_t *disp_checker = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(conf);

    dispatch = axis2_phase_create(env, AXIS2_PHASE_DISPATCH);
    if (!dispatch)
    {
        return AXIS2_FAILURE;
    }

    soap_msg_body_based_dispatch = axiom_soap_body_disp_create(env);
    if (!soap_msg_body_based_dispatch)
    {
        return AXIS2_FAILURE;
    }

    handler = AXIS2_DISP_GET_BASE(soap_msg_body_based_dispatch, env);
    AXIS2_DISP_FREE(soap_msg_body_based_dispatch, env);
    AXIS2_PHASE_ADD_HANDLER_AT(dispatch, env, 0, handler);
    axis2_array_list_add(config_impl->handlers, env, AXIS2_HANDLER_GET_HANDLER_DESC(handler, env));
    handler = NULL;


    add_dispatch = axis2_addr_disp_create(env);
    if (!add_dispatch)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        return AXIS2_FAILURE;
    }

    handler = AXIS2_DISP_GET_BASE(add_dispatch, env);
    AXIS2_DISP_FREE(add_dispatch, env);
    AXIS2_PHASE_ADD_HANDLER_AT(dispatch, env, 1, handler);
    axis2_array_list_add(config_impl->handlers, env, AXIS2_HANDLER_GET_HANDLER_DESC(handler, env));
    handler = NULL;

    /*uri_dispatch = axis2_req_uri_disp_create(env);
    if (!uri_dispatch)
    {
        return AXIS2_FAILURE;
    }

    handler = AXIS2_DISP_GET_BASE(uri_dispatch, env);
    AXIS2_DISP_FREE(uri_dispatch, env);
    AXIS2_PHASE_ADD_HANDLER_AT(dispatch, env, 1, handler);
    axis2_array_list_add(config_impl->handlers, env, AXIS2_HANDLER_GET_HANDLER_DESC(handler, env));
    handler = NULL;*/

    soap_action_based_dispatch = axiom_soap_action_disp_create(env);
    if (!soap_action_based_dispatch)
    {
        return AXIS2_FAILURE;
    }

    handler = AXIS2_DISP_GET_BASE(soap_action_based_dispatch, env);
    AXIS2_DISP_FREE(soap_action_based_dispatch, env);
    AXIS2_PHASE_ADD_HANDLER_AT(dispatch, env, 2, handler);
    axis2_array_list_add(config_impl->handlers, env, AXIS2_HANDLER_GET_HANDLER_DESC(handler, env));
    handler = NULL;

    status = axis2_array_list_add(config_impl->
            in_phases_upto_and_including_post_dispatch, env, dispatch);
    if (AXIS2_SUCCESS != status)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        return status;
    }

    post_dispatch = axis2_phase_create(env, AXIS2_PHASE_POST_DISPATCH);
    if (NULL == post_dispatch)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        return AXIS2_FAILURE;
    }

    disp_checker = axis2_disp_checker_create(env);
    handler = AXIS2_DISP_CHECKER_GET_BASE(disp_checker, env);
    AXIS2_DISP_CHECKER_FREE(disp_checker, env);
    AXIS2_PHASE_ADD_HANDLER_AT(post_dispatch, env, 0, handler);
    axis2_array_list_add(config_impl->handlers, env, AXIS2_HANDLER_GET_HANDLER_DESC(handler, env));
    handler = NULL;

    handler = axis2_ctx_handler_create(env, NULL);
    AXIS2_PHASE_ADD_HANDLER_AT(post_dispatch, env, 1, handler);
    axis2_array_list_add(config_impl->handlers, env, AXIS2_HANDLER_GET_HANDLER_DESC(handler, env));

    status = axis2_array_list_add(config_impl->
            in_phases_upto_and_including_post_dispatch, env, post_dispatch);
    if (AXIS2_SUCCESS != status)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        AXIS2_PHASE_FREE(post_dispatch, env);
        return status;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_set_dispatch_phase(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_phase_t *dispatch)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_handler_t *handler = NULL;
    axis2_phase_t *post_dispatch = NULL;
    axis2_disp_checker_t *disp_checker = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dispatch, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(conf);

    status = axis2_array_list_add(config_impl->
            in_phases_upto_and_including_post_dispatch, env, dispatch);
    if (AXIS2_FAILURE == status)
    {
        return AXIS2_FAILURE;
    }

    post_dispatch = axis2_phase_create(env, AXIS2_PHASE_POST_DISPATCH);
    if (NULL == post_dispatch)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        return AXIS2_FAILURE;
    }

    disp_checker = axis2_disp_checker_create(env);

    handler = AXIS2_DISP_CHECKER_GET_BASE(disp_checker, env);
    AXIS2_PHASE_ADD_HANDLER_AT(post_dispatch, env, 0, handler);

    status = axis2_array_list_add(config_impl->
            in_phases_upto_and_including_post_dispatch, env, post_dispatch);
    if (AXIS2_FAILURE == status)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        AXIS2_PHASE_FREE(post_dispatch, env);
        AXIS2_DISP_CHECKER_FREE(disp_checker, env);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/**
 * For each module reference qname stored in dep_engine this function is called.
 * All module_desc instances are stored in axis2_conf. So each module_desc
 * is retrieved from their by giving module_qname and engaged globally by
 * calling phase_resolvers engage_module_globally function. Modules is added
 * to axis2_conf's engaged module list.
 */
axis2_status_t AXIS2_CALL
axis2_conf_engage_module(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_qname_t *module_ref)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_module_desc_t *module_desc = NULL;
    axis2_bool_t is_new_module = AXIS2_FALSE;
    axis2_bool_t to_be_engaged = AXIS2_TRUE;
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_ref, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(conf);

    module_desc = axis2_conf_get_module(conf, env, module_ref);
    if (NULL == module_desc)
    {
        axis2_file_t *file = NULL;
        axis2_char_t *file_name = NULL;
        axis2_arch_reader_t *arch_reader = NULL;
        const axis2_char_t *repos_path = NULL;
        axis2_arch_file_data_t *file_data = NULL;
        axis2_char_t *temp_path1 = NULL;
        axis2_char_t *temp_path2 = NULL;
        axis2_char_t *temp_path3 = NULL;
        axis2_char_t *path = NULL;

        arch_reader = axis2_arch_reader_create(env);
        if (!arch_reader)
        {
            return AXIS2_FAILURE;
        }
        file_name = AXIS2_QNAME_GET_LOCALPART(module_ref, env);
        file = (axis2_file_t *) AXIS2_ARCH_READER_CREATE_MODULE_ARCH(
                    arch_reader, env, file_name) ;
        repos_path = AXIS2_CONF_GET_REPO(conf, env);
        temp_path1 = AXIS2_STRACAT(repos_path, AXIS2_PATH_SEP_STR, env);
        temp_path2 = AXIS2_STRACAT(temp_path1, AXIS2_MODULE_FOLDER, env);
        temp_path3 = AXIS2_STRACAT(temp_path2, AXIS2_PATH_SEP_STR, env);
        path = AXIS2_STRACAT(temp_path3, file_name, env);
        AXIS2_FREE(env->allocator, temp_path1);
        AXIS2_FREE(env->allocator, temp_path2);
        AXIS2_FREE(env->allocator, temp_path3);
        AXIS2_FILE_SET_PATH(file, env, path);
        file_data = axis2_arch_file_data_create_with_type_and_file(env,
                AXIS2_MODULE, file);

        dep_engine = axis2_dep_engine_create_with_repos_name(env, repos_path);
        AXIS2_DEP_ENGINE_SET_CURRENT_FILE_ITEM(dep_engine, env, file_data);
        module_desc = AXIS2_DEP_ENGINE_BUILD_MODULE(dep_engine, env, file, conf);
        is_new_module = AXIS2_TRUE;
    }
    if (module_desc)
    {
        int size = 0;
        int i = 0;
        const axis2_qname_t *module_qname = NULL;

        size = axis2_array_list_size(config_impl->engaged_modules, env);
        module_qname = AXIS2_MODULE_DESC_GET_QNAME(module_desc, env);
        for (i = 0; i < size; i++)
        {
            axis2_qname_t *qname = NULL;

            qname = (axis2_qname_t *) axis2_array_list_get(config_impl->
                    engaged_modules, env, i);
            if (AXIS2_TRUE == AXIS2_QNAME_EQUALS(module_qname, env, qname))
            {
                to_be_engaged = AXIS2_FALSE;
            }
        }
    }
    else
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_MODULE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    if (to_be_engaged)
    {
        axis2_phase_resolver_t *phase_resolver = NULL;
        axis2_qname_t *module_qref_l = NULL;
        const axis2_qname_t *module_qname = NULL;

        phase_resolver = axis2_phase_resolver_create_with_config(env, conf);
        if (!phase_resolver)
        {
            return AXIS2_FAILURE;
        }

        status = AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_GLOBALLY(phase_resolver, env,
                module_desc);
        AXIS2_PHASE_RESOLVER_FREE(phase_resolver, env);
        if (!status)
        {
            return status;
        }
        module_qname = AXIS2_MODULE_DESC_GET_QNAME(module_desc, env);
        module_qref_l = AXIS2_QNAME_CLONE((axis2_qname_t *)module_qname, env);
        status = axis2_array_list_add(config_impl->engaged_modules, env,
                module_qref_l);
    }
    if (is_new_module)
    {
        status = axis2_conf_add_module(conf, env, module_desc);
    }
    return status;
}

const axis2_char_t *AXIS2_CALL
axis2_conf_get_repo(
    const axis2_conf_t *conf,
    const axis2_env_t *env)
{
    axis2_conf_impl_t *conf_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    conf_impl = AXIS2_INTF_TO_IMPL(conf);

    return conf_impl->axis2_repo;
}

axis2_status_t AXIS2_CALL
axis2_conf_set_repo(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_char_t *repos_path)
{
    axis2_conf_impl_t *conf_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    conf_impl = AXIS2_INTF_TO_IMPL(conf);

    if (conf_impl->axis2_repo)
    {
        AXIS2_FREE(env->allocator, conf_impl->axis2_repo);
        conf_impl->axis2_repo = NULL;
    }
    conf_impl->axis2_repo = AXIS2_STRDUP(repos_path, env);
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_conf_set_dep_engine(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_dep_engine_t *dep_engine)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(conf)->dep_engine = dep_engine;
    return AXIS2_SUCCESS;
}

const axis2_char_t *AXIS2_CALL
axis2_conf_get_default_module_version(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *module_name)
{
    axis2_hash_t *def_ver_map = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, module_name, NULL);

    def_ver_map = AXIS2_INTF_TO_IMPL(conf)->name_to_version_map;
    if (NULL == def_ver_map)
    {
        return NULL;
    }
    return axis2_hash_get(def_ver_map, module_name, AXIS2_HASH_KEY_STRING);
}

axis2_module_desc_t *AXIS2_CALL
axis2_conf_get_default_module(
    const axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *module_name)
{
    axis2_module_desc_t *ret_mod = NULL;
    axis2_char_t *mod_name = NULL;
    const axis2_char_t *mod_ver = NULL;
    axis2_hash_t *all_modules = NULL;
    axis2_qname_t *mod_qname = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, module_name, NULL);

    all_modules = AXIS2_INTF_TO_IMPL(conf)->all_modules;
    mod_ver = AXIS2_CONF_GET_DEFAULT_MODULE_VERSION(conf, env, module_name);

    if (NULL == mod_ver)
    {
        mod_name = AXIS2_STRDUP(module_name, env);
    }
    else
    {
        axis2_char_t *tmp_name = NULL;
        tmp_name = AXIS2_STRACAT(module_name, "-", env);
        mod_name = AXIS2_STRACAT(tmp_name, mod_ver, env);
        AXIS2_FREE(env->allocator, tmp_name);
    }
    mod_qname = axis2_qname_create(env, mod_name, NULL, NULL);
    AXIS2_FREE(env->allocator, mod_name);
    mod_name = NULL;

    if (NULL == mod_qname)
    {
        return NULL;
    }
    ret_mod = (axis2_module_desc_t *)axis2_hash_get(all_modules,
            AXIS2_QNAME_TO_STRING(mod_qname, env),
            AXIS2_HASH_KEY_STRING);

    return ret_mod;
}


axis2_status_t AXIS2_CALL
axis2_conf_add_default_module_version(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *module_name,
    const axis2_char_t *module_version)
{
    axis2_hash_t *name_to_ver_map = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_name, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_version, AXIS2_FAILURE);
    /*
     * If we already have a default module version we don't put
     * it again
     */
    name_to_ver_map = AXIS2_INTF_TO_IMPL(conf)->name_to_version_map;

    if (NULL == axis2_hash_get(name_to_ver_map, module_name,
            AXIS2_HASH_KEY_STRING))
    {
        axis2_char_t *new_entry = AXIS2_STRDUP(module_version, env);
        if (NULL == new_entry)
        {
            return AXIS2_FAILURE;
        }
        axis2_hash_set(name_to_ver_map, module_name, AXIS2_HASH_KEY_STRING,
                new_entry);
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL
axis2_conf_engage_module_with_version(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    const axis2_char_t *module_name,
    const axis2_char_t *version_id)
{
    axis2_qname_t *module_qname = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_name, AXIS2_FAILURE);

    module_qname = axis2_core_utils_get_module_qname(env, module_name,
            version_id);
    if (NULL == module_qname)
    {
        return AXIS2_FAILURE;
    }
    status = AXIS2_CONF_ENGAGE_MODULE(conf, env, module_qname);
    AXIS2_QNAME_FREE(module_qname, env);
    return status;
}

axis2_bool_t AXIS2_CALL
axis2_conf_get_enable_mtom(
    axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);

    return AXIS2_INTF_TO_IMPL(conf)->enable_mtom;
}

axis2_status_t AXIS2_CALL
axis2_conf_set_enable_mtom(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_bool_t enable_mtom)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);

    AXIS2_INTF_TO_IMPL(conf)->enable_mtom = enable_mtom;
    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL
axis2_conf_get_enable_security(
    axis2_conf_t *conf,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);

    return AXIS2_INTF_TO_IMPL(conf)->enable_security;
}

axis2_status_t AXIS2_CALL
axis2_conf_set_enable_security(
    axis2_conf_t *conf,
    const axis2_env_t *env,
    axis2_bool_t enable_security)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf, AXIS2_FAILURE);

    AXIS2_INTF_TO_IMPL(conf)->enable_security = enable_security;
    return AXIS2_SUCCESS;
}

