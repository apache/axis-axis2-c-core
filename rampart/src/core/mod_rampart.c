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
#include <axis2_module.h>
#include <rampart_mod.h>
#include <axis2_conf_ctx.h>

axis2_status_t AXIS2_CALL
mod_rampart_shutdown(axis2_module_t *module,
                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
mod_rampart_init(axis2_module_t *module,
                        const axis2_env_t *env,
                        axis2_conf_ctx_t *conf_ctx,
                        axis2_module_desc_t *module_desc);

axis2_status_t AXIS2_CALL
mod_rampart_fill_handler_create_func_map(axis2_module_t *module,
                                            const axis2_env_t *env);

axis2_module_t *
mod_rampart_create(const axis2_env_t *env)
{
    axis2_module_t *module = NULL;
    module = AXIS2_MALLOC(env->allocator, 
        sizeof(axis2_module_t));

    
    module->ops = AXIS2_MALLOC(
        env->allocator, sizeof(axis2_module_ops_t));

    module->ops->shutdown = mod_rampart_shutdown;
    module->ops->init = mod_rampart_init;
    module->ops->fill_handler_create_func_map = 
        mod_rampart_fill_handler_create_func_map;

    return module;
}

axis2_status_t AXIS2_CALL
mod_rampart_init(axis2_module_t *module,
                       const axis2_env_t *env,
                        axis2_conf_ctx_t *conf_ctx, 
                        axis2_module_desc_t *module_desc)
{
    /* Any initialization stuff of mod_rampart goes here */
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
mod_rampart_shutdown(axis2_module_t *module,
                        const axis2_env_t *env)
{
    if(module->ops)
    {
        AXIS2_FREE(env->allocator, module->ops);
        module->ops = NULL;
    }

    if(module->handler_create_func_map)
    {
        /* TODO
         *  do the neccessary clean in hash map
         */
        axis2_hash_free(module->handler_create_func_map, env);
        module->handler_create_func_map = NULL;
    }
    
    if(module)
    {
        AXIS2_FREE(env->allocator, module);
        module = NULL;
    }
    return AXIS2_SUCCESS; 
}

axis2_status_t AXIS2_CALL
mod_rampart_fill_handler_create_func_map(axis2_module_t *module,
                                            const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    module->handler_create_func_map = axis2_hash_make(env);
    if(!module->handler_create_func_map)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    axis2_hash_set(module->handler_create_func_map, "RampartInHandler", 
        AXIS2_HASH_KEY_STRING, rampart_in_handler_create);

    axis2_hash_set(module->handler_create_func_map, "RampartOutHandler", 
        AXIS2_HASH_KEY_STRING, rampart_out_handler_create);
    
    return AXIS2_SUCCESS;
}

/**
 * Following block distinguish the exposed part of the dll.
 */

AXIS2_EXPORT int 
axis2_get_instance(axis2_module_t **inst,
                   const axis2_env_t *env)
{
   *inst = mod_rampart_create(env);
    if(!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int 
axis2_remove_instance(axis2_module_t *inst,
                      const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
   if (inst)
   {
        status = mod_rampart_shutdown(inst, env);
    }
    return status;
}
