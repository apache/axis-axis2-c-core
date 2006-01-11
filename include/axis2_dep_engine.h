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

#ifndef AXIS2_DEP_ENGINE_H
#define AXIS2_DEP_ENGINE_H

/**
 * @file axis2_dep_engine.h
 * @brief Axis2 Deployment Engine interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_arch_file_data.h>
#include <axis2_ws_info.h>
#include <axis2_conf_builder.h>
#include <axis2_repos_listener.h>


#ifdef __cplusplus
extern "C"
{
#endif
struct axis2_arch_file_data;
struct axis2_ws_info;
struct axis2_phases_info;
    
typedef struct axis2_dep_engine axis2_dep_engine_t;
typedef struct axis2_dep_engine_ops axis2_dep_engine_ops_t;

/** @defgroup axis2_dep_engine Deployment Engine
 * @ingroup axis2_deployment
 * @{
 */

/** 
 * @brief Deployment Engine ops struct
 * Encapsulator struct for ops of axis2_dep_engine
 */
AXIS2_DECLARE_DATA struct axis2_dep_engine_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_dep_engine_t *dep_engine,
	        axis2_env_t **env);
    
    
    /**
     * while parsing the axis2.xml the module refferences have to be stored some 
     * where , since at that time none of module availble (they load after parsing 
     * the document)
     * @param module_qname <code>QName</code>
     */
    axis2_status_t (AXIS2_CALL *
    add_module) (axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_qname_t *module_qname);
    
    struct axis2_module_desc *(AXIS2_CALL *
    get_module) (axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_qname_t *module_name);
    
    struct axis2_arch_file_data *(AXIS2_CALL *
    get_current_file_item) (axis2_dep_engine_t *dep_engine,
                                            axis2_env_t **env);
    
    /**
     * @param file
     */
    axis2_status_t (AXIS2_CALL *
    add_ws_to_deploy) (axis2_dep_engine_t *dep_engine,
                                        axis2_env_t **env,
                                        struct axis2_arch_file_data *file);
    
    /**
     * @param file
     */
    axis2_status_t (AXIS2_CALL *
    add_ws_to_undeploy) (axis2_dep_engine_t *dep_engine,
                                        axis2_env_t **env,
                                        struct axis2_ws_info *file);
    
    struct axis2_phases_info *(AXIS2_CALL *
    get_phases_info) (axis2_dep_engine_t *dep_engine,
                                        axis2_env_t **env);
    
    /**
     * tio get ER
     *
     * @return AxisConfiguration <code>AxisConfiguration</code>
     */
    axis2_conf_t *(AXIS2_CALL *
    get_axis_conf) (axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env);
    
    
    axis2_conf_t *(AXIS2_CALL *
    load) (axis2_dep_engine_t *dep_engine,
                            axis2_env_t **env);
    
    axis2_conf_t *(AXIS2_CALL *
    load_client) (axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_char_t *client_home);
    
    
    void * (AXIS2_CALL *
    get_handler_dll) (axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_char_t *dll_name);
    
    axis2_status_t (AXIS2_CALL *
    do_deploy) (axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    undeploy) (axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env);
    
    axis2_bool_t (AXIS2_CALL *
    is_hot_update) (axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env);
    
    
    axis2_status_t (AXIS2_CALL *
    set_phases_info) (axis2_dep_engine_t *dep_engine,
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
    axis2_svc_t *(AXIS2_CALL *
    build_svc) (axis2_dep_engine_t *dep_engine,
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
    
    axis2_module_desc_t *(AXIS2_CALL *
    build_module) (axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env,
                                    axis2_file_t *module_archive,
                                    axis2_conf_t *conf);  
                                       
};
    
    /** 
     * @brief Deployment Engine struct 
     */  
    AXIS2_DECLARE_DATA struct axis2_dep_engine
    {
        axis2_dep_engine_ops_t *ops;
        
    };
    
    /**
     * No param constructor is need to deploye module and service programatically
     * @return pointer to newly created deployment engine
     */
    AXIS2_DECLARE(axis2_dep_engine_t *)
    axis2_dep_engine_create (axis2_env_t **env);
    
    /**
     * Creates description builder struct
     * This the constructor which is used by Engine in order to start
     * Deployment module,
     *
     * @param repos_path is the path to which Repositary Listner should listen.
     * @return pointer to newly created deployment engine
     */
    AXIS2_DECLARE(axis2_dep_engine_t *)
    axis2_dep_engine_create_with_repos_name (
                                            axis2_env_t **env, 
                                            axis2_char_t *repos_path);
    /**
     * Creates deployment engine struct
     * @param repos_path
     * @param svr_xml_file
     * @return pointer to newly created deployment engine
     */
    AXIS2_DECLARE(axis2_dep_engine_t *) 
    axis2_dep_engine_create_with_repos_name_and_svr_xml_file (
                                            axis2_env_t **env, 
                                            axis2_char_t *repos_path, 
                                            axis2_char_t *svr_xml_file);



/*************************** Function macros **********************************/


#define AXIS2_DEP_ENGINE_FREE(dep_engine, env) \
		((dep_engine)->ops->free (dep_engine, env))  

#define AXIS2_DEP_ENGINE_ADD_MODULE(dep_engine, env, module_qname) \
		((dep_engine)->ops->add_module (dep_engine, env, module_qname))
        
#define AXIS2_DEP_ENGINE_GET_MODULE(dep_engine, env, module_qname) \
		((dep_engine)->ops->get_module (dep_engine, env, module_qname))
        
#define AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(dep_engine, env) \
		((dep_engine)->ops->get_current_file_item (dep_engine, env))        
 
#define AXIS2_DEP_ENGINE_ADD_WS_TO_DEPLOY(dep_engine, env, file) \
		((dep_engine)->ops->add_ws_to_deploy (dep_engine, env, file))  


#define AXIS2_DEP_ENGINE_ADD_WS_TO_UNDEPLOY(dep_engine, env, file) \
		((dep_engine)->ops->add_ws_to_undeploy (dep_engine, env, file)) 

#define AXIS2_DEP_ENGINE_DO_DEPLOY(dep_engine, env) \
		((dep_engine)->ops->do_deploy (dep_engine, env)) 

#define AXIS2_DEP_ENGINE_UNDEPLOY(dep_engine, env) \
		((dep_engine)->ops->undeploy (dep_engine, env)) 

#define AXIS2_DEP_ENGINE_GET_PHASES_INFO(dep_engine, env) \
		((dep_engine)->ops->get_phases_info (dep_engine, env)) 

#define AXIS2_DEP_ENGINE_GET_AXIS2_CONF(dep_engine, env) \
		((dep_engine)->ops->get_axis_conf (dep_engine, env)) 

#define AXIS2_DEP_ENGINE_LOAD(dep_engine, env) \
		((dep_engine)->ops->load (dep_engine, env))
        
#define AXIS2_DEP_ENGINE_LOAD_CLIENT(dep_engine, env, client_home) \
		((dep_engine)->ops->load_client (dep_engine, env, client_home))

#define AXIS2_DEP_ENGINE_GET_HANDLER_DLL(dep_engine, env, dll_name) \
		((dep_engine)->ops->get_handler_dll (dep_engine, env, dll_name))
        
#define AXIS2_DEP_ENGINE_DO_DEPLOY(dep_engine, env) \
		((dep_engine)->ops->do_deploy (dep_engine, env))

#define AXIS2_DEP_ENGINE_UNDEPLOY(dep_engine, env) \
		((dep_engine)->ops->undeploy (dep_engine, env))
        
#define AXIS2_DEP_ENGINE_IS_HOT_UPDATE(dep_engine, env) \
		((dep_engine)->ops->is_hot_update (dep_engine, env))

#define AXIS2_DEP_ENGINE_SET_PHASES_INFO(dep_engine, env, phases_info) \
		((dep_engine)->ops->set_phases_info (dep_engine, env, phases_info))
        
#define AXIS2_DEP_ENGINE_BUILD_SVC(dep_engine, env, svc, file_name) \
		((dep_engine)->ops->build_svc (dep_engine, env, svc, file_name))

#define AXIS2_DEP_ENGINE_BUILD_MODULE(dep_engine, env, module_archive, conf) \
		((dep_engine)->ops->build_module (dep_engine, env, module_archive, conf))
        
        
/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_DEP_ENGINE_H */
