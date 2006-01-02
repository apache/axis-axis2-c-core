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
 
#include <axis2_dep_engine.h>

/** 
 * @brief DLL Description struct impl
 * Axis2 DLL Description impl  
 */ 
typedef struct axis2_dep_engine_impl
{
	axis2_dep_engine_t dep_engine;
    struct axis2_arch_file_data *current_arch_file;
    
    /**
     * to keep a ref to engine register
     * this ref will pass to engine when it call start()
     * method
     */
    struct axis2_conf *axis_config;
    /**
     * This will store all the web Services to deploy
     */
    axis2_array_list_t *ws_to_deploy;
    /**
     * this will store all the web Services to undeploy
     */
    axis2_array_list_t *ws_to_undeploy;
    
    axis2_phases_info_t *phases_info;
    
} axis2_dep_engine_impl_t;

#define AXIS2_INTF_TO_IMPL(dep_engine) ((axis2_dep_engine_impl_t *) dep_engine)
	
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_dep_engine_free (axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env);

struct axis2_module_desc *AXIS2_CALL
axis2_dep_engine_get_module(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_qname_t *module_name);

struct axis2_arch_file_data *AXIS2_CALL
axis2_dep_engine_get_current_file_item(axis2_dep_engine_t *dep_engine,
                                        axis2_env_t **env);

/**
 * @param file
 */
axis2_status_t AXIS2_CALL
axis2_dep_engine_add_ws_to_deploy(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_arch_file_data_t *file);

/**
 * @param file
 */
axis2_status_t AXIS2_CALL
axis2_dep_engine_add_ws_to_undeploy(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_ws_info_t *file);

axis2_status_t AXIS2_CALL
axis2_dep_engine_do_deploy(axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_dep_engine_undeploy(axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env);

axis2_phases_info_t *AXIS2_CALL
axis2_dep_engine_get_phases_info(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env);

/************************* End of function headers ****************************/	

axis2_dep_engine_t *AXIS2_CALL 
axis2_dep_engine_create_with_repos_name (
                                        axis2_env_t **env, 
                                        axis2_char_t *repos_path)
{
    axis2_dep_engine_impl_t *dep_engine_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
	dep_engine_impl = (axis2_dep_engine_impl_t *) AXIS2_MALLOC ((*env)->allocator
		    , sizeof (axis2_dep_engine_impl_t));
    
	if(NULL == dep_engine_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    dep_engine_impl->axis_config = NULL;
    dep_engine_impl->current_arch_file = NULL;
    dep_engine_impl->ws_to_deploy = NULL;
    dep_engine_impl->ws_to_undeploy = NULL;
    dep_engine_impl->phases_info = NULL;
    
    dep_engine_impl->dep_engine.ops = NULL;
    
    dep_engine_impl->dep_engine.ops = (axis2_dep_engine_ops_t *) AXIS2_MALLOC(
        (*env)->allocator, sizeof(axis2_dep_engine_ops_t));
    
	if(NULL == dep_engine_impl->dep_engine.ops)
	{
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
	dep_engine_impl->dep_engine.ops->free = axis2_dep_engine_free;
    dep_engine_impl->dep_engine.ops->get_module = axis2_dep_engine_get_module;
    dep_engine_impl->dep_engine.ops->get_current_file_item = 
        axis2_dep_engine_get_current_file_item; 
    dep_engine_impl->dep_engine.ops->add_ws_to_deploy = 
        axis2_dep_engine_add_ws_to_deploy;
    dep_engine_impl->dep_engine.ops->add_ws_to_undeploy = 
        axis2_dep_engine_add_ws_to_undeploy;
    dep_engine_impl->dep_engine.ops->do_deploy = axis2_dep_engine_do_deploy;
    dep_engine_impl->dep_engine.ops->undeploy = axis2_dep_engine_undeploy;
    dep_engine_impl->dep_engine.ops->get_phases_info = 
        axis2_dep_engine_get_phases_info;
        
						
	return &(dep_engine_impl->dep_engine);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_dep_engine_free (axis2_dep_engine_t *dep_engine,
                        axis2_env_t **env)
{
    axis2_dep_engine_impl_t *dep_engine_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    
    dep_engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    if(dep_engine_impl->current_arch_file)
    {
        AXIS2_ARCH_FILE_DATA_FREE(dep_engine_impl->current_arch_file, env);
        dep_engine_impl->current_arch_file = NULL;
    }
    if(dep_engine_impl->ws_to_deploy)
    {
        AXIS2_ARRAY_LIST_FREE(dep_engine_impl->ws_to_deploy, env);
        dep_engine_impl->ws_to_deploy = NULL;
    }
    if(dep_engine_impl->ws_to_undeploy)
    {
        AXIS2_ARRAY_LIST_FREE(dep_engine_impl->ws_to_undeploy, env);
        dep_engine_impl->ws_to_undeploy = NULL;
    }
    
	if(NULL != dep_engine->ops)
    {
		AXIS2_FREE((*env)->allocator, dep_engine->ops);
        dep_engine->ops = NULL;
    }
    
    
	return AXIS2_SUCCESS;
}

struct axis2_module_desc *AXIS2_CALL
axis2_dep_engine_get_module(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_qname_t *module_name) 
{
    axis2_dep_engine_impl_t *dep_engine_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, module_name, NULL);
    
    dep_engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    return AXIS2_CONF_GET_MODULE(dep_engine_impl->axis_config, env, module_name);
}

struct axis2_arch_file_data *AXIS2_CALL
axis2_dep_engine_get_current_file_item(axis2_dep_engine_t *dep_engine,
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(dep_engine)->current_arch_file;
}

/**
 * @param file
 */
axis2_status_t AXIS2_CALL
axis2_dep_engine_add_ws_to_deploy(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_arch_file_data_t *file) 
{
    axis2_dep_engine_impl_t *dep_engine_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, file, AXIS2_FAILURE);
    dep_engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
    return AXIS2_ARRAY_LIST_ADD(dep_engine_impl->ws_to_deploy, env, file);
}

/**
 * @param file
 */
axis2_status_t AXIS2_CALL
axis2_dep_engine_add_ws_to_undeploy(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_ws_info_t *file) 
{
    axis2_dep_engine_impl_t *dep_engine_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, file, AXIS2_FAILURE);
    dep_engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
    return AXIS2_ARRAY_LIST_ADD(dep_engine_impl->ws_to_undeploy, env, file);
}

axis2_status_t AXIS2_CALL
axis2_dep_engine_do_deploy(axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env)
{
    axis2_status_t status = AXIS2_FAILURE;
    return status;
}

axis2_status_t AXIS2_CALL
axis2_dep_engine_undeploy(axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env)
{
    axis2_status_t status = AXIS2_FAILURE;
    return status;
}

axis2_phases_info_t *AXIS2_CALL
axis2_dep_engine_get_phases_info(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, NULL);
    return AXIS2_INTF_TO_IMPL(dep_engine)->phases_info;
}
