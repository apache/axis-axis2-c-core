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
#include <axis2_qname.h>
#include <axis2_conf.h>
#include <axis2_arch_file_data.h>
#include <axis2_ws_info.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_conf;
struct axis2_arch_file_data;    
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
    
    axis2_module_desc_t *(AXIS2_CALL *
    get_module) (axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env,
                                axis2_qname_t *module_qname);

    struct axis2_arch_file_data *(AXIS2_CALL *
    get_current_file_item)(axis2_dep_engine_t *dep_engine,
                                            axis2_env_t **env);
    
    /**
     * @param file
     */
    axis2_status_t (AXIS2_CALL *
    add_ws_to_deploy) (axis2_dep_engine_t *dep_engine,
                                        axis2_env_t **env,
                                        axis2_arch_file_data_t *file);
    
    /**
     * @param file
     */
    axis2_status_t (AXIS2_CALL *
    add_ws_to_undeploy) (axis2_dep_engine_t *dep_engine,
                                        axis2_env_t **env,
                                        axis2_ws_info_t *file);
    
    axis2_status_t (AXIS2_CALL *
    do_deploy) (axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    undeploy) (axis2_dep_engine_t *dep_engine,
                                axis2_env_t **env);
    
    axis2_phases_info_t *(AXIS2_CALL *
    get_phases_info) (axis2_dep_engine_t *dep_engine,
                                    axis2_env_t **env);                          
 

};

/** 
 * @brief Description Builder struct 
 */  
AXIS2_DECLARE_DATA struct axis2_dep_engine
{
	axis2_dep_engine_ops_t *ops;
};

/**
 * Creates description builder struct
 * @param repos_path
 * @return pointer to newly created deployment engine
 */
AXIS2_DECLARE(axis2_dep_engine_t *) 
axis2_dep_engine_create_with_repos_name (
                                        axis2_env_t **env, 
                                        axis2_char_t *repos_path);

/**
 * Creates description builder struct
 * @param repos_path
 * @param svr_xml_file
 * @return pointer to newly created deployment engine
 */
AXIS2_DECLARE(axis2_dep_engine_t *) 
axis2_dep_engine_create_with_repos_name_and_svr_xml (
                                        axis2_env_t **env, 
                                        axis2_char_t *repos_path, 
                                        axis2_char_t *svr_xml_file);

/*************************** Function macros **********************************/

#define AXIS2_DEP_ENGINE_FREE(dep_engine, env) \
		((dep_engine->ops)->free (dep_engine, env))  

#define AXIS2_DEP_ENGINE_ADD_MODULE(dep_engine, env, module_qname) \
		((dep_engine->ops)->add_module (dep_engine, env, module_qname))
        
#define AXIS2_DEP_ENGINE_GET_MODULE(dep_engine, env, module_qname) \
		((dep_engine->ops)->get_module (dep_engine, env, module_qname))
        
#define AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(dep_engine, env) \
		((dep_engine->ops)->get_current_file_item (dep_engine, env))        
 
#define AXIS2_DEP_ENGINE_ADD_WS_TO_DEPLOY(dep_engine, env, file) \
		((dep_engine->ops)->add_ws_to_deploy (dep_engine, env, file))  


#define AXIS2_DEP_ENGINE_ADD_WS_TO_UNDEPLOY(dep_engine, env, file) \
		((dep_engine->ops)->add_ws_to_undeploy (dep_engine, env, file)) 

#define AXIS2_DEP_ENGINE_DO_DEPLOY(dep_engine, env) \
		((dep_engine->ops)->do_deploy (dep_engine, env)) 

#define AXIS2_DEP_ENGINE_UNDEPLOY(dep_engine, env) \
		((dep_engine->ops)->undeploy (dep_engine, env)) 

#define AXIS2_DEP_ENGINE_GET_PHASES_INFO(dep_engine, env) \
		((dep_engine->ops)->get_phases_info (dep_engine, env)) 
        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_DEP_ENGINE_H */
