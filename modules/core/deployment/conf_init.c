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
 
#include <axis2_conf_init.h>
#include <axis2_transport_sender.h>
#include <axis2_transport_receiver.h>
#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_allocator.h>
#include <axis2_dep_engine.h>
#include <axis2_module.h>

axis2_status_t AXIS2_CALL 
axis2_init_modules(
    const axis2_env_t *env, 
    axis2_conf_ctx_t *conf_ctx);

axis2_status_t AXIS2_CALL 
axis2_init_transports(
    const axis2_env_t *env, 
    axis2_conf_ctx_t *conf_ctx);
    
AXIS2_EXTERN axis2_conf_ctx_t *AXIS2_CALL 
build_conf_ctx(
    const axis2_env_t *env,
    const axis2_char_t *repo_name)
{    
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_conf_t *conf = NULL;
    axis2_phase_resolver_t *phase_resolver = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    dep_engine = axis2_dep_engine_create_with_repos_name(env, repo_name);
    if (!dep_engine)
    {
        return NULL;
    }
    
    conf = AXIS2_DEP_ENGINE_LOAD(dep_engine, env);
    if (!conf)
    {
        return NULL;
    }
    AXIS2_CONF_SET_DEP_ENGINE(conf, env, dep_engine);
    
    phase_resolver = axis2_phase_resolver_create_with_config(env, conf);
    if (!phase_resolver)
    {
        return NULL;
    }
    
    conf_ctx = axis2_conf_ctx_create(env, conf);
    
    AXIS2_PHASE_RESOLVER_BUILD_CHAINS(phase_resolver, env);
    
    axis2_init_modules(env, conf_ctx);
    axis2_init_transports(env, conf_ctx);

    AXIS2_PHASE_RESOLVER_FREE(phase_resolver, env);
    
    return conf_ctx;
}

axis2_conf_ctx_t *AXIS2_CALL 
build_client_conf_ctx(
    const axis2_env_t *env,
    const axis2_char_t *axis2_home)
{
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_conf_t *conf = NULL;
    axis2_phase_resolver_t *phase_resolver = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    dep_engine = axis2_dep_engine_create(env);
    if (!dep_engine)
    {
        return NULL;
    }
    
    conf = AXIS2_DEP_ENGINE_LOAD_CLIENT(dep_engine, env, axis2_home);
    if (!conf)
    {
        return NULL;
    }
    AXIS2_CONF_SET_DEP_ENGINE(conf, env, dep_engine);

    phase_resolver = axis2_phase_resolver_create_with_config(env, conf);
    if (!phase_resolver)
    {
        return NULL;
    }

    conf_ctx = axis2_conf_ctx_create(env, conf);
    if (!conf_ctx)
    {
        return NULL;
    }

    AXIS2_PHASE_RESOLVER_BUILD_CHAINS(phase_resolver, env);

    axis2_init_modules(env, conf_ctx);
    axis2_init_transports(env, conf_ctx);

    AXIS2_PHASE_RESOLVER_FREE(phase_resolver, env);
   
    return conf_ctx;
}

axis2_status_t AXIS2_CALL 
axis2_init_modules(
    const axis2_env_t *env, 
    axis2_conf_ctx_t *conf_ctx)
{
    axis2_conf_t *conf = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf_ctx, AXIS2_FAILURE);
    
    conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
    if (conf)
    {
        axis2_hash_t *module_map = AXIS2_CONF_GET_ALL_MODULES(conf, env);
        if (module_map)
        {
            axis2_hash_index_t *hi = NULL;
            void *module = NULL;
            for (hi = axis2_hash_first (module_map, env);
                    hi; hi = axis2_hash_next (env, hi))
            {
                axis2_hash_this (hi, NULL, NULL, &module);
                if (module)
                {
                    axis2_module_desc_t *mod_desc = (axis2_module_desc_t*)module;
                    if (mod_desc)
                    {
                        axis2_module_t *mod = AXIS2_MODULE_DESC_GET_MODULE(mod_desc, env);
                        if (mod)
                            AXIS2_MODULE_INIT(mod, env, conf_ctx, mod_desc);
                    }
                }
            }
        }
        status = AXIS2_SUCCESS;
    }
    
    return status;
}

axis2_status_t AXIS2_CALL 
axis2_init_transports(
    const axis2_env_t *env, 
    axis2_conf_ctx_t *conf_ctx)
{
    axis2_conf_t *conf = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf_ctx, AXIS2_FAILURE);
    
    conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
    if (conf)
    {
        axis2_hash_t *transport_map = NULL;
       
        transport_map = AXIS2_CONF_GET_ALL_IN_TRANSPORTS(conf, env);
        if (transport_map)
        {
            axis2_hash_index_t *hi = NULL;
            void *transport = NULL;
            for (hi = axis2_hash_first (transport_map, env);
                    hi; hi = axis2_hash_next (env, hi))
            {
                axis2_hash_this (hi, NULL, NULL, &transport);
                if (transport)
                {
                    axis2_transport_in_desc_t *transport_in = (axis2_transport_in_desc_t*)transport;
                    axis2_transport_receiver_t *listener = AXIS2_TRANSPORT_IN_DESC_GET_RECV(transport_in, env);
                    if (listener)
                    {
                        status = AXIS2_TRANSPORT_RECEIVER_INIT(listener, env, conf_ctx, transport_in);
                    }
                    
                }
            }
        }
        
        transport_map = AXIS2_CONF_GET_ALL_OUT_TRANSPORTS(conf, env);
        if (transport_map)
        {
            axis2_hash_index_t *hi = NULL;
            void *transport = NULL;
            for (hi = axis2_hash_first (transport_map, env);
                    hi; hi = axis2_hash_next (env, hi))
            {
                axis2_hash_this (hi, NULL, NULL, &transport);
                if (transport)
                {
                    axis2_transport_out_desc_t *transport_out = (axis2_transport_out_desc_t*)transport;
                    axis2_transport_sender_t *sender = AXIS2_TRANSPORT_OUT_DESC_GET_SENDER(transport_out, env);
                    if (sender)
                    {
                        status = AXIS2_TRANSPORT_SENDER_INIT(sender, env, conf_ctx, transport_out);
                    }
                    
                }
            }
        }
        status = AXIS2_SUCCESS;
    }
    
    return status;
}
