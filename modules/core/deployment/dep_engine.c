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
#include <axis2_array_list.h>
#include <axis2_deployment.h>
#include <axis2_file_handler.h>
#include <axis2_dll_desc.h>
#include <axis2_flow.h>
#include <axis2_arch_reader.h>
#include <axis2_svc_builder.h>
#include <axis2_om_node.h>
#include <axis2_class_loader.h>

/** 
 * @brief DLL Description struct impl
 * Axis2 DLL Description impl  
 */ 
typedef struct axis2_dep_engine_impl
{
	axis2_dep_engine_t dep_engine;
    struct axis2_arch_file_data *curr_file;
    
    /**
     * to keep a ref to engine register
     * this ref will pass to engine when it call start()
     * method
     */
    struct axis2_conf *conf;
    axis2_char_t *axis2_repos;
    axis2_bool_t hot_dep;   /* to do hot deployment or not */
    axis2_bool_t hot_update;  /* to do hot update or not */
    /**
     * This will store all the web Services to deploy
     */
    axis2_array_list_t *ws_to_deploy;
    /**
     * this will store all the web Services to undeploy
     */
    axis2_array_list_t *ws_to_undeploy;
    
    struct axis2_phases_info *phases_info; /* to store phases list in axis2.xml */
    /**
     * this constructor for the testing
     */
    axis2_char_t *folder_name;

    axis2_char_t *conf_name;

    /**
     * To store the module specified in the server.xml at the document parsing 
     * time
     */
    axis2_array_list_t *module_list;
 
    
} axis2_dep_engine_impl_t;

#define AXIS2_INTF_TO_IMPL(dep_engine) ((axis2_dep_engine_impl_t *) dep_engine)
	
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_dep_engine_free (axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_dep_engine_add_module(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_qname_t *module_qname);
    
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

axis2_phases_info_t *AXIS2_CALL
axis2_dep_engine_get_phases_info(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env);

/**
 * tio get ER
 *
 * @return AxisConfiguration <code>AxisConfiguration</code>
 */
axis2_conf_t *AXIS2_CALL
axis2_dep_engine_get_axis_conf(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env);

/**
 * To set hotDeployment and hot update
 */
axis2_status_t
axis2_dep_engine_set_dep_features(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env);

axis2_conf_t *AXIS2_CALL
axis2_dep_engine_load(axis2_dep_engine_t *dep_engine,
                        axis2_env_t **env);

axis2_conf_t *AXIS2_CALL
axis2_dep_engine_load_client(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_char_t *client_home);

axis2_status_t
axis2_dep_engine_check_client_home(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_char_t *client_home);

/**
 * This methode used to check the modules referd by server.xml
 * are exist , or they have deployed
 */
axis2_status_t
axis2_dep_engine_engage_modules(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env);

/**
 * This method is to check wether some one has change the system pre defined phases for all the
 * flows if some one has done so will throw a DeploymentException
 *
 */
axis2_status_t
axis2_dep_engine_validate_system_predefined_phases(axis2_dep_engine_t *dep_engine,
                                                    axis2_env_t **env);

axis2_conf_t *
axis2_dep_engine_create_conf(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env);

axis2_status_t
axis2_dep_engine_add_new_svc(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_svc_grp_t *svc_metadata);

/**
 * This method is used to fill the axis service , it dose loading service class and also the provider class
 * and it will also load the service handlers
 *
 * @param axisService
 * @throws org.apache.axis2.AxisFault
 */
axis2_status_t
axis2_dep_engine_load_svc_props(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_svc_t *svc);

axis2_status_t
axis2_dep_engine_load_module_dll(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_module_desc_t *module_desc);

axis2_status_t
axis2_dep_engine_add_flow_handlers(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_flow_t *flow);


void * AXIS2_CALL
axis2_dep_engine_get_handler_dll(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_char_t *dll_name);


axis2_status_t
axis2_dep_engine_add_new_module(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_module_desc_t *module_metadata);

axis2_status_t AXIS2_CALL
axis2_dep_engine_do_deploy(axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_dep_engine_undeploy(axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_dep_engine_is_hot_update(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env);

/**
 * This method is used to retrive service name form the arechive file name
 * if the archive file name is service1.aar , then axis service name would be service1
 *
 * @param fileName
 * @return String
 */
axis2_char_t *
axis2_dep_engine_get_axis_svc_name(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_char_t *file_name);

axis2_status_t AXIS2_CALL
axis2_dep_engine_set_phases_info(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_phases_info_t *phases_info);

/**
 * This method is used to fill a axisservice object using services.xml , first it should create
 * an axisservice object using WSDL and then fill that using given servic.xml and load all the requed
 * class and build the chains , finally add the  servicecontext to EngineContext and axisservice into
 * EngineConfiguration
 *
 * @param axisService
 * @param serviceInputStream
 * @param classLoader
 * @return
 * @throws DeploymentException
 */
axis2_svc_t *AXIS2_CALL
axis2_dep_engine_build_svc(axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env,
                            axis2_svc_t *svc,
                            axis2_char_t *file_name);

/**
 * This method can be used to build ModuleDescription for a given module archiev file
 *
 * @param modulearchive
 * @return
 * @throws DeploymentException
 */

axis2_module_desc_t *AXIS2_CALL
axis2_dep_engine_build_module(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_file_t *module_archive,
                                axis2_conf_t *conf);  
  
/************************* End of function headers ****************************/	

axis2_dep_engine_t *AXIS2_CALL
axis2_dep_engine_create(axis2_env_t **env)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    engine_impl = (axis2_dep_engine_impl_t *) AXIS2_MALLOC ((*env)->allocator
		    , sizeof (axis2_dep_engine_impl_t));
    
	if(NULL == engine_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 
    engine_impl->conf = NULL;
    engine_impl->axis2_repos = NULL;
    engine_impl->curr_file = NULL;
    engine_impl->ws_to_deploy = NULL;
    engine_impl->ws_to_undeploy = NULL;
    engine_impl->phases_info = NULL;
    engine_impl->module_list = NULL;
    engine_impl->folder_name = NULL;
    engine_impl->conf_name = NULL; 
    engine_impl->dep_engine.ops = NULL;
    
    engine_impl->dep_engine.ops = (axis2_dep_engine_ops_t *) AXIS2_MALLOC(
        (*env)->allocator, sizeof(axis2_dep_engine_ops_t));
    
	if(NULL == engine_impl->dep_engine.ops)
	{
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    engine_impl->dep_engine.ops->free = axis2_dep_engine_free;
    engine_impl->dep_engine.ops->add_module = axis2_dep_engine_add_module;
    engine_impl->dep_engine.ops->get_module = axis2_dep_engine_get_module;
    engine_impl->dep_engine.ops->get_current_file_item = 
        axis2_dep_engine_get_current_file_item; 
    engine_impl->dep_engine.ops->add_ws_to_deploy = 
        axis2_dep_engine_add_ws_to_deploy;
    engine_impl->dep_engine.ops->add_ws_to_undeploy = 
        axis2_dep_engine_add_ws_to_undeploy;
    engine_impl->dep_engine.ops->get_phases_info = 
        axis2_dep_engine_get_phases_info;
    
    engine_impl->dep_engine.ops->get_axis_conf = axis2_dep_engine_get_axis_conf;
    engine_impl->dep_engine.ops->load = axis2_dep_engine_load;
    engine_impl->dep_engine.ops->load_client = axis2_dep_engine_load_client; 
    engine_impl->dep_engine.ops->get_handler_dll = 
        axis2_dep_engine_get_handler_dll;
    engine_impl->dep_engine.ops->do_deploy = axis2_dep_engine_do_deploy;
    engine_impl->dep_engine.ops->undeploy = axis2_dep_engine_undeploy;
    engine_impl->dep_engine.ops->is_hot_update = axis2_dep_engine_is_hot_update;
    engine_impl->dep_engine.ops->set_phases_info = 
        axis2_dep_engine_set_phases_info;
    engine_impl->dep_engine.ops->build_svc = axis2_dep_engine_build_svc;
    engine_impl->dep_engine.ops->build_module = axis2_dep_engine_build_module;
    
    return &(engine_impl->dep_engine);
}
axis2_dep_engine_t *AXIS2_CALL 
axis2_dep_engine_create_with_repos_name (
                                        axis2_env_t **env, 
                                        axis2_char_t *repos_path)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
	engine_impl = (axis2_dep_engine_impl_t *)
        axis2_dep_engine_create_with_repos_name_and_svr_xml_file(env, 
            repos_path, AXIS2_SERVER_XML_FILE);
    if(!engine_impl)
    {
        return NULL;
    }
					
	return &(engine_impl->dep_engine);
}

axis2_dep_engine_t *AXIS2_CALL
axis2_dep_engine_create_with_repos_name_and_svr_xml_file(axis2_env_t **env,
                                                    axis2_char_t *repos_path,
                                                    axis2_char_t *svr_xml_file)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    axis2_char_t *conf_file_l = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, repos_path, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svr_xml_file, NULL);
    if(0 == AXIS2_STRCMP("", repos_path))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_REPO_CAN_NOT_BE_NULL,
            AXIS2_FAILURE);
        return NULL;
    }
    
    engine_impl = (axis2_dep_engine_impl_t *) axis2_dep_engine_create(env);
    
	if(NULL == engine_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    status = axis2_file_handler_access(repos_path, AXIS2_F_OK);
    if(AXIS2_SUCCESS != status)
    {
        axis2_dep_engine_free(&(engine_impl->dep_engine), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_REPOSITORY_NOT_EXIST, 
            AXIS2_FAILURE);
        return NULL;
    }
    
    engine_impl->folder_name = AXIS2_STRDUP(repos_path, env);
    if(NULL == engine_impl->folder_name)
	{
        axis2_dep_engine_free(&(engine_impl->dep_engine), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    engine_impl->axis2_repos = AXIS2_STRDUP(repos_path, env);
    if(NULL == engine_impl->axis2_repos)
	{
        axis2_dep_engine_free(&(engine_impl->dep_engine), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
   
    conf_file_l = AXIS2_STRACAT(repos_path, AXIS2_PATH_SEP_STR, env);
    engine_impl->conf_name = AXIS2_STRACAT(conf_file_l, svr_xml_file, env);
    AXIS2_FREE((*env)->allocator, conf_file_l);
    if(!engine_impl->conf_name)
    {
        engine_impl->conf_name = 
            AXIS2_STRDUP(AXIS2_CONFIGURATION_RESOURCE, env);
        if(!engine_impl->conf_name)
        {
            axis2_dep_engine_free(&(engine_impl->dep_engine), env);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }
    status = axis2_file_handler_access(engine_impl->conf_name, AXIS2_F_OK);
    if(AXIS2_SUCCESS != status)
    {
        axis2_dep_engine_free(&(engine_impl->dep_engine), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CONFIG_NOT_FOUND, 
            AXIS2_FAILURE);
        return NULL;
    }
    
    return &(engine_impl->dep_engine); 
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_dep_engine_free (axis2_dep_engine_t *dep_engine,
                        axis2_env_t **env)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    if(engine_impl->curr_file)
    {
        AXIS2_ARCH_FILE_DATA_FREE(engine_impl->curr_file, env);
        engine_impl->curr_file = NULL;
    }
    if(engine_impl->ws_to_deploy)
    {
        AXIS2_ARRAY_LIST_FREE(engine_impl->ws_to_deploy, env);
        engine_impl->ws_to_deploy = NULL;
    }
    if(engine_impl->ws_to_undeploy)
    {
        AXIS2_ARRAY_LIST_FREE(engine_impl->ws_to_undeploy, env);
        engine_impl->ws_to_undeploy = NULL;
    }
    if(engine_impl->module_list)
    {
        AXIS2_ARRAY_LIST_FREE(engine_impl->module_list, env);
        engine_impl->module_list = NULL;
    }
    if(engine_impl->folder_name)
    {
        AXIS2_FREE((*env)->allocator, engine_impl->folder_name);
        engine_impl->folder_name = NULL;
    }
    if(engine_impl->conf_name)
    {
        AXIS2_FREE((*env)->allocator, engine_impl->conf_name);
        engine_impl->conf_name = NULL;
    }
    if(engine_impl->axis2_repos)
    {
        AXIS2_FREE((*env)->allocator, engine_impl->axis2_repos);
        engine_impl->axis2_repos = NULL;
    }
    
	if(NULL != dep_engine->ops)
    {
		AXIS2_FREE((*env)->allocator, dep_engine->ops);
        dep_engine->ops = NULL;
    }
    
    
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_dep_engine_add_module(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_qname_t *module_qname) 
{
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_qname, AXIS2_FAILURE);
    
    return AXIS2_ARRAY_LIST_ADD(AXIS2_INTF_TO_IMPL(dep_engine)->module_list,
        env, module_qname);
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
    return AXIS2_CONF_GET_MODULE(dep_engine_impl->conf, env, module_name);
}

struct axis2_arch_file_data *AXIS2_CALL
axis2_dep_engine_get_current_file_item(axis2_dep_engine_t *dep_engine,
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(dep_engine)->curr_file;
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

axis2_phases_info_t *AXIS2_CALL
axis2_dep_engine_get_phases_info(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, NULL);
    return AXIS2_INTF_TO_IMPL(dep_engine)->phases_info;
}

/**
 * tio get ER
 *
 * @return AxisConfiguration <code>AxisConfiguration</code>
 */
axis2_conf_t *AXIS2_CALL
axis2_dep_engine_get_axis_conf(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(dep_engine)->conf;
}

/**
 * To set hotDeployment and hot update
 */
axis2_status_t
axis2_dep_engine_set_dep_features(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env) 
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    axis2_char_t *value = NULL;
    axis2_param_t *para_hot_dep = NULL;
    axis2_param_t *para_hot_update = NULL;
    
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
    para_hot_dep = AXIS2_CONF_GET_PARAM(engine_impl->conf, env, 
        AXIS2_HOTDEPLOYMENT);
    para_hot_update = AXIS2_CONF_GET_PARAM(engine_impl->conf, env, 
        AXIS2_HOTUPDATE); 
    
    if(NULL != para_hot_dep)
    {
        value = (axis2_char_t *) AXIS2_PARAM_GET_VALUE(para_hot_dep, env);
        if(0 == AXIS2_STRCASECMP("false", value))
        {
            engine_impl->hot_dep = AXIS2_FALSE;
        }
    }
    if(NULL != para_hot_update)
    {
        value = (axis2_char_t *) AXIS2_PARAM_GET_VALUE(para_hot_update, env);
        if(0 == AXIS2_STRCASECMP("false", value))
        {
            engine_impl->hot_update = AXIS2_FALSE;
        }
    }
    return AXIS2_SUCCESS;
}

axis2_conf_t *AXIS2_CALL
axis2_dep_engine_load(axis2_dep_engine_t *dep_engine,
                        axis2_env_t **env) 
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    axis2_conf_builder_t *builder = NULL;
    axis2_repos_listener_t *repos_listener = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, NULL);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
    if(NULL == engine_impl->conf_name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_PATH_TO_CONFIG_CAN_NOT_BE_NULL,
            AXIS2_FAILURE);
        return NULL;        
    }
    
    engine_impl->conf = axis2_conf_create(env);
    if(!engine_impl->conf)
    {
        return NULL;
    }
    builder = axis2_conf_builder_create_with_file_and_dep_engine_and_conf(env,
        engine_impl->conf_name, dep_engine, engine_impl->conf);
    if(!builder)
    {
        AXIS2_CONF_FREE(engine_impl->conf, env);
        engine_impl->conf = NULL;
    }
    status = AXIS2_CONF_BUILDER_POPULATE_CONF(builder, env);
    if(AXIS2_SUCCESS != status)
    {
        AXIS2_CONF_FREE(engine_impl->conf, env);
        AXIS2_CONF_BUILDER_FREE(builder, env);
        engine_impl->conf = NULL;
        return NULL;
    }
    axis2_dep_engine_set_dep_features(dep_engine, env);
    
    /*
    if (hotDeployment) {
        startSearch(this);
    } else {
    */
    repos_listener = 
        axis2_repos_listener_create_with_folder_name_and_dep_engine(env, 
            engine_impl->folder_name, dep_engine);
    if(!repos_listener)
    {
        AXIS2_CONF_FREE(engine_impl->conf, env);
        AXIS2_CONF_BUILDER_FREE(builder, env);
        return NULL;
    }
    AXIS2_CONF_SET_REPOS(engine_impl->conf, env, engine_impl->axis2_repos);
    status = axis2_dep_engine_validate_system_predefined_phases(dep_engine, env);
    if(AXIS2_FAILURE == status)
    {
        AXIS2_REPOS_LISTENER_FREE(repos_listener, env);
        AXIS2_CONF_FREE(engine_impl->conf, env);
        AXIS2_CONF_BUILDER_FREE(builder, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MODULE_VAL_FAILED, 
            AXIS2_FAILURE);
        return NULL;
    }
    AXIS2_CONF_SET_PHASESINFO(engine_impl->conf, env, engine_impl->phases_info);
    status = axis2_dep_engine_engage_modules(dep_engine, env);
    if(AXIS2_FAILURE == status)
    {
        AXIS2_REPOS_LISTENER_FREE(repos_listener, env);
        AXIS2_CONF_FREE(engine_impl->conf, env);
        AXIS2_CONF_BUILDER_FREE(builder, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MODULE_VAL_FAILED, 
            AXIS2_FAILURE);
        return NULL;
    }
    /*}*/
    
    return engine_impl->conf;
}


axis2_conf_t *AXIS2_CALL
axis2_dep_engine_load_client(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_char_t *client_home) 
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    axis2_conf_builder_t *builder = NULL;
    axis2_repos_listener_t *repos_listener = NULL;
    axis2_bool_t is_repos_exist = AXIS2_FALSE;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, client_home, NULL);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
    engine_impl->axis2_repos = AXIS2_STRDUP(client_home, env);
    if(!engine_impl->axis2_repos)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    if(NULL != client_home && 0 != AXIS2_STRCMP("", client_home))
    {
        axis2_dep_engine_check_client_home(dep_engine, env, client_home);
        is_repos_exist = AXIS2_TRUE;
    }
    else
    {
        engine_impl->conf_name = AXIS2_STRDUP(AXIS2_CONFIGURATION_RESOURCE,
                env);
        if(!engine_impl->conf_name)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }
    engine_impl->conf = axis2_conf_create(env);
    if(!engine_impl->conf)
    {
        return NULL;
    }
    builder = axis2_conf_builder_create_with_file_and_dep_engine_and_conf(env,
        engine_impl->conf_name, dep_engine, engine_impl->conf);
    if(!builder)
    {
        AXIS2_CONF_FREE(engine_impl->conf, env);
        engine_impl->conf = NULL;
    }
    status = AXIS2_CONF_BUILDER_POPULATE_CONF(builder, env);
    if(AXIS2_SUCCESS != status)
    {
        AXIS2_CONF_FREE(engine_impl->conf, env);
        AXIS2_CONF_BUILDER_FREE(builder, env);
        engine_impl->conf = NULL;
        return NULL;
    }
 
    if (AXIS2_TRUE == is_repos_exist) 
    {
        engine_impl->hot_dep = AXIS2_FALSE;
        engine_impl->hot_update = AXIS2_FALSE;
        repos_listener = 
            axis2_repos_listener_create_with_folder_name_and_dep_engine(env, 
                engine_impl->folder_name, dep_engine);
    }
    
    AXIS2_CONF_SET_REPOS(engine_impl->conf, env, engine_impl->axis2_repos);
    AXIS2_CONF_SET_PHASESINFO(engine_impl->conf, env, engine_impl->phases_info);
    status = axis2_dep_engine_engage_modules(dep_engine, env);
    if(AXIS2_FAILURE == status)
    {
        AXIS2_REPOS_LISTENER_FREE(repos_listener, env);
        AXIS2_CONF_FREE(engine_impl->conf, env);
        AXIS2_CONF_BUILDER_FREE(builder, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MODULE_VAL_FAILED, 
            AXIS2_FAILURE);
        return NULL;
    }
    
    return engine_impl->conf;
}


axis2_status_t
axis2_dep_engine_check_client_home(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_char_t *client_home)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    axis2_char_t *path_l = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);

    engine_impl->folder_name = AXIS2_STRDUP(client_home, env);
    if(engine_impl->folder_name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    path_l = AXIS2_STRACAT(client_home, AXIS2_PATH_SEP_STR, env);
    engine_impl->conf_name = AXIS2_STRACAT(path_l,AXIS2_SERVER_XML_FILE, env);
    AXIS2_FREE((*env)->allocator, path_l);
    if(!engine_impl->conf_name)
    {
        engine_impl->conf_name = AXIS2_STRDUP(AXIS2_CONFIGURATION_RESOURCE, env);
        if(!engine_impl->conf_name)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CONFIG_NOT_FOUND,
                AXIS2_FAILURE)
            return AXIS2_FAILURE;
        }  
    }
    status = axis2_file_handler_access(engine_impl->conf_name, AXIS2_F_OK);
    if(AXIS2_SUCCESS != status)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CONFIG_NOT_FOUND, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}

/**
 * This methode used to check the modules referd by server.xml
 * are exist , or they have deployed
 */
axis2_status_t
axis2_dep_engine_engage_modules(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    int size = 0;
    int i = 0;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    /* ArrayList modules = DeploymentData.getInstance().getModules(); */
    /* PhaseResolver resolver = new PhaseResolver(axisConfig); */
    size = AXIS2_ARRAY_LIST_SIZE(engine_impl->module_list, env);
    for(i = 0; i < size; i++)
    {
        axis2_qname_t *qname = (axis2_qname_t *) AXIS2_ARRAY_LIST_GET(
            engine_impl->module_list, env, i);
        AXIS2_CONF_ENGAGE_MODULE(engine_impl->conf, env, qname);
    }
    return AXIS2_SUCCESS;
}

/**
 * This method is to check wether some one has change the system pre defined phases for all the
 * flows if some one has done so will throw a DeploymentException
 *
 */
axis2_status_t
axis2_dep_engine_validate_system_predefined_phases(axis2_dep_engine_t *dep_engine,
                                                    axis2_env_t **env) 
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    axis2_array_list_t *in_phases = NULL;
    axis2_char_t *phase0 = NULL;
    axis2_char_t *phase1 = NULL;
    axis2_char_t *phase2 = NULL;
    axis2_char_t *phase3 = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
    in_phases = AXIS2_PHASES_INFO_GET_IN_PHASES(engine_impl->phases_info, env);
    
    /* TODO condition checking should be otherway since null value can occur */
    phase0 = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(in_phases, env, 0);
    phase1 = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(in_phases, env, 1);
    phase2 = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(in_phases, env, 2);
    phase3 = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(in_phases, env, 3);
    if(0 == AXIS2_STRCMP(phase0, AXIS2_PHASE_TRANSPORTIN) && 
        0 == AXIS2_STRCMP(phase1, AXIS2_PHASE_PRE_DISPATCH) &&
        0 == AXIS2_STRCMP(phase2, AXIS2_PHASE_DISPATCH) && 
        0 == AXIS2_STRCMP(phase3, AXIS2_PHASE_POST_DISPATCH))
    {
        AXIS2_ERROR_SET((*env)->error, AXI2_ERROR_IN_VALID_PHASE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    /*  ArrayList outPhaes = tempdata.getOutphases(); */
    /* TODO do the validation code here */
    /* ArrayList systemDefaultPhases =((AxisConfigurationImpl)axisConfig).
        getInPhasesUptoAndIncludingPostDispatch();
     */
    return AXIS2_SUCCESS;
}

axis2_conf_t *
axis2_dep_engine_create_conf(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return axis2_conf_create(env);
}


axis2_status_t
axis2_dep_engine_add_new_svc(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_svc_grp_t *svc_metadata)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    axis2_array_list_t *svcs = NULL;
    int size = 0;
    int i = 0;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_metadata, AXIS2_FAILURE);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
/*  Iterator services = currentArchiveFile.getService().values().iterator(); */
    
    svcs = AXIS2_ARCH_FILE_DATA_GET_DEPLOYABLE_SVCS(engine_impl->curr_file, env);
    size = AXIS2_ARRAY_LIST_SIZE(svcs, env);
    for(i = 0; i < size; i++)
    {
        axis2_svc_t *svc = NULL;
        axis2_file_t *file = NULL;
        axis2_array_list_t *grp_modules = NULL;
        axis2_array_list_t *list = NULL;
        int size = 0;
        int i = 0;
        axis2_hash_t *ops = NULL;
        axis2_hash_index_t *index_i = NULL;
        
        svc = (axis2_svc_t *) AXIS2_ARRAY_LIST_GET(svcs, env, i);
        axis2_dep_engine_load_svc_props(dep_engine, env, svc);
        file = AXIS2_ARCH_FILE_DATA_GET_FILE(engine_impl->curr_file, env);
        AXIS2_SVC_SET_FILENAME(svc, env, file->name);

        /* module form serviceGroup */
        grp_modules = AXIS2_SVC_GRP_GET_MODULES(svc_metadata, env);
        size = AXIS2_ARRAY_LIST_SIZE(grp_modules, env);
        for(i = 0; i< size; i++)
        {
            axis2_module_desc_t *module_desc = NULL;
            axis2_qname_t *qmodulename = NULL;
            
            qmodulename = (axis2_qname_t *) AXIS2_ARRAY_LIST_GET(grp_modules, 
                env, i);
            module_desc = AXIS2_CONF_GET_MODULE(engine_impl->conf, env,
                qmodulename);
            if(NULL != module_desc)
            {
                AXIS2_SVC_ENGAGE_MODULE(svc, env, module_desc, engine_impl->conf);
                
            } 
            else 
            {
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_MODUELE_REF,
                    AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
        }

        /* modules from <service> */
        list = AXIS2_SVC_GET_MODULES(svc, env);
        size = AXIS2_ARRAY_LIST_SIZE(list, env);
        for (i = 0; i < size; i++) 
        {
            axis2_module_desc_t *module_desc = NULL;
            axis2_qname_t *qmodulename = NULL;
            
            qmodulename = (axis2_qname_t *) AXIS2_ARRAY_LIST_GET(grp_modules, 
                env, i);
            module_desc = AXIS2_CONF_GET_MODULE(engine_impl->conf, env,
                qmodulename);
            if(NULL != module_desc)
            {
                AXIS2_SVC_ENGAGE_MODULE(svc, env, module_desc, engine_impl->conf);
                
            } 
            else 
            {
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_MODUELE_REF,
                    AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
        }

        ops = AXIS2_SVC_GET_OPS(svc, env);
        for (index_i = axis2_hash_first (ops, env); index_i; index_i = 
            axis2_hash_next (env, index_i))
        {
            void * v = NULL;
            axis2_op_t *op_desc = NULL;
            axis2_array_list_t *modules = NULL;
            int size = 0;
            int i = 0;
            
            axis2_hash_this (index_i, NULL, NULL, &v);
            op_desc = (axis2_op_t *)v;
        
            modules = AXIS2_OP_GET_MODULE_REFS(op_desc, env);
            size = AXIS2_ARRAY_LIST_SIZE(modules, env);
            for (i = 0; i < size; i++) 
            {
                axis2_qname_t *module_qname = NULL;
                axis2_module_desc_t *module = NULL;
                
                module_qname = (axis2_qname_t *) AXIS2_ARRAY_LIST_GET(modules, 
                    env, i);
                module = AXIS2_CONF_GET_MODULE(engine_impl->conf, env, 
                    module_qname);
                
                if (NULL != module) 
                {
                    AXIS2_OP_ENGAGE_MODULE(op_desc, env, module);
                } 
                else 
                {
                    AXIS2_ERROR_SET((*env)->error,
                        AXIS2_ERROR_INVALID_MODUELE_REF_BY_OP, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
            }
        }
        AXIS2_SVC_GRP_ADD_SVC(svc_metadata, env, svc);
    }
    return AXIS2_CONF_ADD_SVC_GRP(engine_impl->conf, env, svc_metadata);
}

/**
 * This method is used to fill the axis service , it dose loading service class and also the provider class
 * and it will also load the service handlers
 *
 * @param axisService
 * @throws org.apache.axis2.AxisFault
 */
axis2_status_t
axis2_dep_engine_load_svc_props(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_svc_t *svc)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    axis2_flow_t *in_flow = NULL;
    axis2_flow_t *out_flow = NULL;
    axis2_flow_t *in_fault_flow = NULL;
    axis2_flow_t *out_fault_flow = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
    in_flow = AXIS2_SVC_GET_INFLOW(svc, env);
    if(NULL != in_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, in_flow);
    }
    
    out_flow = AXIS2_SVC_GET_OUTFLOW(svc, env);
    if(NULL != out_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, out_flow);
    }
    
    in_fault_flow = AXIS2_SVC_GET_FAULT_INFLOW(svc, env);
    if(NULL != in_fault_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, in_fault_flow);
    }
    
    out_fault_flow = AXIS2_SVC_GET_FAULT_OUTFLOW(svc, env);
    if(NULL != out_fault_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, out_fault_flow);
    }
    /* axisService.setClassLoader(currentArchiveFile.getClassLoader()); */
    return AXIS2_SUCCESS;
}


axis2_status_t
axis2_dep_engine_load_module_dll(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_module_desc_t *module_desc)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    axis2_char_t *read_in_dll = NULL;
    axis2_module_t *module = NULL;
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_param_t *impl_info_param = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_desc, AXIS2_FAILURE);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
    read_in_dll = AXIS2_ARCH_FILE_DATA_GET_MODULE_DLL_NAME(engine_impl->
        curr_file, env);
    dll_desc = axis2_dll_desc_create(env);
    AXIS2_DLL_DESC_SET_NAME(dll_desc, env, read_in_dll);
    AXIS2_DLL_DESC_SET_TYPE(dll_desc, env, AXIS2_MODULE_DLL);
    axis2_class_loader_init(env);
    impl_info_param = axis2_param_create(env, NULL, NULL);
    AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc);
    module = (axis2_module_t *) axis2_class_loader_create_dll(env, 
        impl_info_param);
    
    return AXIS2_MODULE_DESC_SET_MODULE(module_desc, env, module);
}


axis2_status_t
axis2_dep_engine_add_flow_handlers(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_flow_t *flow)
{
    int count = 0;
    int j = 0;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, flow, AXIS2_FAILURE);
    
    count = AXIS2_FLOW_GET_HANDLER_COUNT(flow, env);
    
    for (j = 0; j < count; j++) 
    {
        axis2_handler_desc_t *handlermd = NULL;
        axis2_handler_t *handler = NULL;
        axis2_char_t *handler_dll_name = NULL;
        axis2_dll_desc_t *dll_desc = NULL;
        axis2_param_t *impl_info_param = NULL;
        
        handlermd = AXIS2_FLOW_GET_HANDLER(flow, env, j);
        handler_dll_name = AXIS2_HANDLER_DESC_GET_CLASS_NAME(handlermd, env);
        dll_desc = axis2_dll_desc_create(env);
        AXIS2_DLL_DESC_SET_NAME(dll_desc, env, handler_dll_name);
        AXIS2_DLL_DESC_SET_TYPE(dll_desc, env, AXIS2_HANDLER_DLL);
        axis2_class_loader_init(env);
        impl_info_param = axis2_param_create(env, NULL, NULL);
        AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc);
        handler = (axis2_handler_t *) axis2_class_loader_create_dll(env, 
            impl_info_param);
        AXIS2_HANDLER_INIT(handler, env, handlermd);
        AXIS2_HANDLER_DESC_SET_HANDLER(handlermd, env, handler);
    }
    return AXIS2_SUCCESS;
}


void * AXIS2_CALL
axis2_dep_engine_get_handler_dll(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_char_t *dll_name)
{
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_param_t *impl_info_param = NULL;
    axis2_handler_t *handler = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, dll_name, NULL);
    
    dll_desc = axis2_dll_desc_create(env);
    AXIS2_DLL_DESC_SET_NAME(dll_desc, env, dll_name);
    AXIS2_DLL_DESC_SET_TYPE(dll_desc, env, AXIS2_HANDLER_DLL);
    axis2_class_loader_init(env);
    impl_info_param = axis2_param_create(env, NULL, NULL);
    AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc);
    handler = (axis2_handler_t *) axis2_class_loader_create_dll(env, 
        impl_info_param);
    
    return handler;
}


axis2_status_t
axis2_dep_engine_add_new_module(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_module_desc_t *module_metadata)
{
    axis2_flow_t *in_flow = NULL;
    axis2_flow_t *out_flow = NULL;
    axis2_flow_t *in_fault_flow = NULL;
    axis2_flow_t *out_fault_flow = NULL;
    /* currentArchiveFile.setClassLoader(); */
    
    in_flow = AXIS2_MODULE_DESC_GET_INFLOW(module_metadata, env);
    if(NULL != in_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, in_flow);
    }
    
    out_flow = AXIS2_MODULE_DESC_GET_OUTFLOW(module_metadata, env);
    if(NULL != out_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, out_flow);
    }
    
    in_fault_flow = AXIS2_MODULE_DESC_GET_FAULT_INFLOW(module_metadata, env);
    if(NULL != in_fault_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, in_fault_flow);
    }
    
    out_fault_flow = AXIS2_MODULE_DESC_GET_FAULT_OUTFLOW(module_metadata, env);
    if(NULL != out_fault_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, out_fault_flow);
    }
    
    axis2_dep_engine_load_module_dll(dep_engine, env, module_metadata);
    /* module_metadata.setModuleClassLoader(currentArchiveFile.getClassLoader()); */
    AXIS2_CONF_ADD_MODULE(AXIS2_INTF_TO_IMPL(dep_engine)->conf, env, 
        module_metadata);
    /* log.info(Messages.getMessage(DeploymentErrorMsgs.ADDING_NEW_MODULE)); */
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_dep_engine_do_deploy(axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    int size = 0;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
    size = AXIS2_ARRAY_LIST_SIZE(engine_impl->ws_to_deploy, env);
    
    if (size > 0) 
    {
        int i = 0;
        for (i = 0; i < size; i++) 
        {
            int type = 0;
            axis2_arch_reader_t *arch_reader = NULL;
            axis2_svc_grp_t *svc_grp = NULL;
            axis2_char_t *file_name = NULL;
            axis2_module_desc_t *meta_data = NULL;
            
            engine_impl->curr_file = (axis2_arch_file_data_t *) 
                AXIS2_ARRAY_LIST_GET(engine_impl->ws_to_deploy, env, i);
            
            type = AXIS2_ARCH_FILE_DATA_GET_TYPE(engine_impl->curr_file, env);
            
            switch (type) 
            {
                case AXIS2_SVC:
                    arch_reader = axis2_arch_reader_create(env);
                    
                    /*archiveReader.processWSDLs(currentArchiveFile,this); */
                    /* AxisService service = archiveReader.createService(currentArchiveFile.getAbsolutePath()); */
                    svc_grp = axis2_svc_grp_create_with_conf(env, 
                        engine_impl->conf);
                    file_name = AXIS2_ARCH_FILE_DATA_GET_NAME(engine_impl->
                        curr_file, env);
                    status = AXIS2_ARCH_READER_PROCESS_SVC_GRP(arch_reader, env,
                        file_name, dep_engine, svc_grp);
                    if(AXIS2_SUCCESS != status)
                    {
                        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_SVC, 
                            AXIS2_FAILURE);
                        return AXIS2_FAILURE;
                    }
                    status = axis2_dep_engine_add_new_svc(dep_engine, env, 
                        svc_grp);
                    if(AXIS2_SUCCESS != status)
                    {
                        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_SVC, 
                            AXIS2_FAILURE);
                        return AXIS2_FAILURE;
                    }
                    /* log.info(Messages.getMessage(
                            DeploymentErrorMsgs.DEPLOYING_WS, currentArchiveFile.getName())); */
                    engine_impl->curr_file = NULL;
                    break;
                case AXIS2_MODULE:
                    arch_reader = axis2_arch_reader_create(env);
                    meta_data = axis2_module_desc_create(env);
                    file_name = AXIS2_ARCH_FILE_DATA_GET_NAME(engine_impl->
                        curr_file, env);
                    status = AXIS2_ARCH_READER_READ_MODULE_ARCH(arch_reader, 
                        env, file_name, dep_engine, meta_data);
                    if(AXIS2_SUCCESS != status)
                    {
                        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_MODULE, 
                            AXIS2_FAILURE);
                        return AXIS2_FAILURE;
                    }
                    status = axis2_dep_engine_add_new_module(dep_engine, env, 
                        meta_data);
                    if(AXIS2_SUCCESS != status)
                    {
                        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_MODULE, 
                            AXIS2_FAILURE);
                        return AXIS2_FAILURE;
                    }
                        
                    /*log.info(Messages.getMessage(DeploymentErrorMsgs.DEPLOYING_MODULE,
                            metaData.getName().getLocalPart())); */
                    engine_impl->curr_file = NULL;
                    
                    break;

            }
        }
    }
    AXIS2_ARRAY_LIST_FREE(engine_impl->ws_to_deploy, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_dep_engine_undeploy(axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    int size = 0;
    axis2_char_t *svc_name = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    size = AXIS2_ARRAY_LIST_SIZE (engine_impl->ws_to_undeploy, env);
    
    if (size > 0) 
    {
        int i = 0;
        for (i = 0; i < size; i++) 
        {
            int type = 0;         
            axis2_ws_info_t *ws_info = NULL;
            axis2_hash_t *faulty_svcs = NULL;
            
            ws_info = (axis2_ws_info_t *) AXIS2_ARRAY_LIST_GET(engine_impl->
                ws_to_undeploy, env, i);
            type = AXIS2_WS_INFO_GET_TYPE(ws_info, env);
            if (type == AXIS2_SVC) 
            {
                axis2_char_t *file_name = NULL;
                
                file_name = AXIS2_WS_INFO_GET_FILE_NAME(ws_info, env);
                svc_name = axis2_dep_engine_get_axis_svc_name(dep_engine, env,
                    file_name);
                
                /* todo  */
                /*   axisConfig.removeService(new QName(serviceName)); */
                /*log.info(Messages.getMessage(DeploymentErrorMsgs.SERVICE_REMOVED,
                        wsInfo.getFilename()));*/
            }
            faulty_svcs = AXIS2_CONF_GET_FAULTY_SVCS(engine_impl->conf, env);
            axis2_hash_set(faulty_svcs, svc_name, AXIS2_HASH_KEY_STRING, NULL);
        }

    }
    AXIS2_ARRAY_LIST_FREE(engine_impl->ws_to_undeploy, env);
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_dep_engine_is_hot_update(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(dep_engine)->hot_update;
}

/**
 * This method is used to retrive service name form the arechive file name
 * if the archive file name is service1.aar , then axis service name would be service1
 *
 * @param fileName
 * @return String
 */
axis2_char_t *
axis2_dep_engine_get_axis_svc_name(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_char_t *file_name) 
{
    axis2_char_t name_sep = '.';
    axis2_char_t *temp_name = NULL;
    axis2_char_t *ptr = NULL;
    axis2_char_t* file_name_l = NULL;
    axis2_char_t *svc_name = NULL;
    int len = 0;

    file_name_l = AXIS2_STRDUP(file_name, env);
    ptr = AXIS2_STRRCHR(file_name_l, AXIS2_PATH_SEP_CHAR);
    temp_name = ptr + 1;
    ptr = AXIS2_STRRCHR(temp_name, name_sep);
    ptr[0] = '\0';
    len = strlen(temp_name);
    svc_name = AXIS2_MALLOC((*env)->allocator, len + 1);
    sscanf(temp_name, "%s", svc_name);
    AXIS2_FREE((*env)->allocator, file_name_l);
    return svc_name;
}

axis2_status_t AXIS2_CALL
axis2_dep_engine_set_phases_info(axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_phases_info_t *phases_info) 
{
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, phases_info, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(dep_engine)->phases_info = phases_info;
    return AXIS2_SUCCESS;
}

/**
 * This method is used to fill a axisservice object using services.xml , first it should create
 * an axisservice object using WSDL and then fill that using given servic.xml and load all the requed
 * class and build the chains , finally add the  servicecontext to EngineContext and axisservice into
 * EngineConfiguration
 *
 * @param axisService
 * @param serviceInputStream
 * @param classLoader
 * @return
 * @throws DeploymentException
 */
axis2_svc_t *AXIS2_CALL
axis2_dep_engine_build_svc(axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env,
                            axis2_svc_t *svc,
                            axis2_char_t *file_name)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    axis2_svc_builder_t *builder = NULL;
    axis2_om_node_t *node = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, file_name, NULL);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
    engine_impl->curr_file = axis2_arch_file_data_create_with_type_and_name(
        env, AXIS2_SVC, "");

    builder = axis2_svc_builder_create_with_file_and_dep_engine_and_svc(env, 
        file_name, dep_engine, svc);
    
    node = AXIS2_DESC_BUILDER_BUILD_OM(builder->desc_builder, env);
    AXIS2_SVC_BUILDER_POPULATE_SVC(builder, env, node);
    axis2_dep_engine_load_svc_props(dep_engine, env, svc);
    
    return svc;
}

/**
 * This method can be used to build ModuleDescription for a given module archiev file
 *
 * @param modulearchive
 * @return
 * @throws DeploymentException
 */

axis2_module_desc_t *AXIS2_CALL
axis2_dep_engine_build_module(axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_file_t *module_archive,
                                axis2_conf_t *conf)
{
    axis2_dep_engine_impl_t *engine_impl = NULL;
    axis2_module_desc_t *module = NULL;
    axis2_phases_info_t *phases_info = NULL;
    axis2_arch_reader_t *arch_reader = NULL;
    axis2_flow_t *in_flow = NULL;
    axis2_flow_t *out_flow = NULL;
    axis2_flow_t *in_fault_flow = NULL;
    axis2_flow_t *out_fault_flow = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(dep_engine, env, NULL);
    engine_impl = AXIS2_INTF_TO_IMPL(dep_engine);
    
    phases_info = AXIS2_CONF_GET_PHASESINFO(engine_impl->conf, env);
    axis2_dep_engine_set_phases_info(dep_engine, env, phases_info);
    engine_impl->curr_file = axis2_arch_file_data_create_with_type_and_file(
        env, AXIS2_MODULE, module_archive);
    module = axis2_module_desc_create(env);
    arch_reader = axis2_arch_reader_create(env);
    AXIS2_ARCH_READER_READ_MODULE_ARCH(arch_reader, env, module_archive->name,
        dep_engine, module);    

    in_flow = AXIS2_MODULE_DESC_GET_INFLOW(module, env);
    if(NULL != in_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, in_flow);
    }
    
    out_flow = AXIS2_MODULE_DESC_GET_OUTFLOW(module, env);
    if(NULL != out_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, out_flow);
    }
    
    in_fault_flow = AXIS2_MODULE_DESC_GET_FAULT_INFLOW(module, env);
    if(NULL != in_fault_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, in_fault_flow);
    }
    
    out_fault_flow = AXIS2_MODULE_DESC_GET_FAULT_OUTFLOW(module, env);
    if(NULL != out_fault_flow)
    {
        axis2_dep_engine_add_flow_handlers(dep_engine, env, out_fault_flow);
    }
    
    axis2_dep_engine_load_module_dll(dep_engine, env, module);
    engine_impl->curr_file = NULL;
   
    /*axismodule.setModuleClassLoader(currentArchiveFile.getClassLoader()); */
    return module;
}

/* public AxisService deployService(ClassLoader classLoder, InputStream serviceStream, String servieName) throws DeploymentException {
AxisService service = null;
try {
currentArchiveFileile = new ArchiveFileData(SERVICE, servieName);
currentArchiveFileile.setClassLoader(classLoder);
service = new AxisService();
DeploymentParser schme = new DeploymentParser(serviceStream, this);
schme.parseServiceXML(service);
service = loadServiceProperties(service);
} catch (XMLStreamException e) {
throw  new DeploymentException(e.getMessage());
} catch (PhaseException e) {
throw  new DeploymentException(e.getMessage());
} catch (AxisFault axisFault) {
throw  new DeploymentException(axisFault.getMessage());
}
return service;
}
*/

/**
 * this method use to start the Deployment engine
 * inorder to perform Hot deployment and so on..
 */
/*
private void startSearch(DeploymentEngine engine) {
    Scheduler scheduler = new Scheduler();
    scheduler.schedule(new SchedulerTask(engine, folderName),
            new DeploymentIterator());
}
*/
