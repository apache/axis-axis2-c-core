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
 * @defgroup axis2_dep_engine Deployment Engine
 * @ingroup axis2_deployment
 * @{
 */

/**
 * @file axis2_dep_engine.h
 * @brief Axis2 Deployment Engine interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include "axis2_arch_file_data.h"
#include "axis2_ws_info.h"
#include "axis2_conf_builder.h"
#include "axis2_repos_listener.h"


#ifdef __cplusplus
extern "C"
{
#endif
    struct axis2_arch_file_data;
    struct axis2_arch_reader;
    struct axis2_ws_info;
    struct axis2_phases_info;
    struct axis2_svc;

    /** Type name for struct axis2_dep_engine */
    typedef struct axis2_dep_engine axis2_dep_engine_t;
    /** Type name for struct axis2_dep_engine_ops */
    typedef struct axis2_dep_engine_ops axis2_dep_engine_ops_t;

    /**
     * Deployment Engine ops struct
     * Encapsulator struct for ops of axis2_dep_engine
     */
    struct axis2_dep_engine_ops
    {
        /** 
         * De-allocate memory
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env);


        /**
         * while parsing the axis2.xml the module refferences have to be stored some 
         * where , since at that time none of module availble (they load after parsing 
         * the document)
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @param module_qname <code>QName</code>
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_module)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env,
                    axis2_qname_t *module_qname);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @param module_name pointer to module name
         */
        struct axis2_module_desc *(AXIS2_CALL *
                get_module)(
                    const axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env,
                    axis2_qname_t *module_name);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         */
        struct axis2_arch_file_data *(AXIS2_CALL *
                get_current_file_item)(
                    const axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @param file pointer to file
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_ws_to_deploy)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env,
                    struct axis2_arch_file_data *file);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @param file pointer to file
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_ws_to_undeploy)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env,
                    struct axis2_ws_info *file);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         */
        struct axis2_phases_info *(AXIS2_CALL *
                get_phases_info)(
                    const axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env);

        /**
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @return AxisConfiguration <code>AxisConfiguration</code>
         */
        struct axis2_conf *(AXIS2_CALL *
                get_axis_conf)(
                    const axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         */
        struct axis2_conf *(AXIS2_CALL *
                load)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         */
        struct axis2_conf *(AXIS2_CALL *
                load_client)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env,
                    const axis2_char_t *client_home);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @param dll_name pointer to dll_name
         */
        void *(AXIS2_CALL *
                get_handler_dll)(
                    const axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env,
                    axis2_char_t *dll_name);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                do_deploy)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                undeploy)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                is_hot_update)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @param phases_info pointer to phases info
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_phases_info)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env,
                    struct axis2_phases_info *phases_info);

        /**
         * This method is used to fill a axisservice object using services.xml , first it should create
         * an axisservice object using WSDL and then fill that using given servic.xml and load all the requed
         * class and build the chains , finally add the  servicecontext to EngineContext and axisservice into
         * EngineConfiguration
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @param svc pointer to service
         * @param file_name pointer to file name
         */
        struct axis2_svc *(AXIS2_CALL *
                build_svc)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env,
                    struct axis2_svc *svc,
                    axis2_char_t *file_name);

        /**
         * This method can be used to build ModuleDescription for a given module archiev file
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @param module_archive pointer to module archive
         * @param conf pointer to conf
         */
        struct axis2_module_desc *(AXIS2_CALL *
                build_module)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env,
                    axis2_file_t *module_archive,
                    struct axis2_conf *conf);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_repos_path)(
                    const axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @param file_data pointer to file data
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_current_file_item)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env,
                    struct axis2_arch_file_data *file_data);

        /** 
         * @param dep_engine pointer to deployment engine
         * @param env pointer to environment struct
         * @param arch_reader pointer to arch reader
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_arch_reader)(
                    axis2_dep_engine_t *dep_engine,
                    const axis2_env_t *env,
                    struct axis2_arch_reader *arch_reader);
    };

    /**
     * Deployment Engine struct 
     */
    struct axis2_dep_engine
    {
        /** Operations of deployment engine */
        axis2_dep_engine_ops_t *ops;
    };

    /**
     * No param constructor is need to deploye module and service programatically
     * @param env pointer to environment struct
     * @return pointer to newly created deployment engine
     */
    AXIS2_EXTERN axis2_dep_engine_t *AXIS2_CALL
    axis2_dep_engine_create(
        const axis2_env_t *env);

    /**
     * Creates description builder struct
     * This the constructor which is used by Engine in order to start
     * Deployment module,
     * @param env pointer to environment struct
     * @param repos_path is the path to which Repositary Listner should listen.
     * @return pointer to newly created deployment engine
     */
    AXIS2_EXTERN axis2_dep_engine_t *AXIS2_CALL
    axis2_dep_engine_create_with_repos_name(
        const axis2_env_t *env,
        const axis2_char_t *repos_path);

    /**
     * Creates deployment engine struct
     * @param env pointer to environment struct
     * @param repos_path is the path to which Repositary Listner should listen.
     * @param svr_xml_file pointer to service xml file
     * @return pointer to newly created deployment engine
     */
    AXIS2_EXTERN axis2_dep_engine_t *AXIS2_CALL
    axis2_dep_engine_create_with_repos_name_and_svr_xml_file(
        const axis2_env_t *env,
        const axis2_char_t *repos_path,
        const axis2_char_t *svr_xml_file);

/*************************** Function macros **********************************/

/** Frees the deployment engine.
    @sa axis2_dep_engine_ops#free */
#define AXIS2_DEP_ENGINE_FREE(dep_engine, env) \
      ((dep_engine)->ops->free (dep_engine, env))

/** Adds the module.
    @sa axis2_dep_engine_ops#add_module */
#define AXIS2_DEP_ENGINE_ADD_MODULE(dep_engine, env, module_qname) \
      ((dep_engine)->ops->add_module (dep_engine, env, module_qname))

/** Get module.
    @sa axis2_dep_engine_ops#get_module */
#define AXIS2_DEP_ENGINE_GET_MODULE(dep_engine, env, module_qname) \
      ((dep_engine)->ops->get_module (dep_engine, env, module_qname))

/** Gets the current file item.
    @sa axis2_dep_engine_ops#get_current_file_item */
#define AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(dep_engine, env) \
      ((dep_engine)->ops->get_current_file_item (dep_engine, env))

/** Adds web service to deploy.
    @sa axis2_dep_engine_ops#add_ws_to_deploy */
#define AXIS2_DEP_ENGINE_ADD_WS_TO_DEPLOY(dep_engine, env, file) \
      ((dep_engine)->ops->add_ws_to_deploy (dep_engine, env, file))

/** Adds web service to undelploy
    @sa axis2_dep_engine_ops#add_ws_to_undeploy */
#define AXIS2_DEP_ENGINE_ADD_WS_TO_UNDEPLOY(dep_engine, env, file) \
      ((dep_engine)->ops->add_ws_to_undeploy (dep_engine, env, file))

/** Do deploy.
    @sa axis2_dep_engine_ops#do_deploy */
#define AXIS2_DEP_ENGINE_DO_DEPLOY(dep_engine, env) \
      ((dep_engine)->ops->do_deploy (dep_engine, env))

/** Undeploy.
    @sa axis2_dep_engine_ops#undeploy */
#define AXIS2_DEP_ENGINE_UNDEPLOY(dep_engine, env) \
      ((dep_engine)->ops->undeploy (dep_engine, env))

/** Gets the phases infomation..
    @sa axis2_dep_engine_ops#get_phases_info */
#define AXIS2_DEP_ENGINE_GET_PHASES_INFO(dep_engine, env) \
      ((dep_engine)->ops->get_phases_info (dep_engine, env))

/** Gets the axis2 configuration.
    @sa axis2_dep_engine_ops#get_axis2_conf */
#define AXIS2_DEP_ENGINE_GET_AXIS2_CONF(dep_engine, env) \
      ((dep_engine)->ops->get_axis_conf (dep_engine, env))

/** Load.
    @sa axis2_dep_engine_ops#load */
#define AXIS2_DEP_ENGINE_LOAD(dep_engine, env) \
      ((dep_engine)->ops->load (dep_engine, env))

/** Loads the client.
    @sa axis2_dep_engine_ops#load_client */
#define AXIS2_DEP_ENGINE_LOAD_CLIENT(dep_engine, env, client_home) \
      ((dep_engine)->ops->load_client (dep_engine, env, client_home))

/** Gets the handler dll.
    @sa axis2_dep_engine_ops#get_handler_dll */
#define AXIS2_DEP_ENGINE_GET_HANDLER_DLL(dep_engine, env, dll_name) \
      ((dep_engine)->ops->get_handler_dll (dep_engine, env, dll_name))

/** Is hot update.
    @sa axis2_dep_engine_ops#is_hot_update */
#define AXIS2_DEP_ENGINE_IS_HOT_UPDATE(dep_engine, env) \
      ((dep_engine)->ops->is_hot_update (dep_engine, env))

/** Sets phases info.
    @sa axis2_dep_engine_ops#set_phases_info */
#define AXIS2_DEP_ENGINE_SET_PHASES_INFO(dep_engine, env, phases_info) \
      ((dep_engine)->ops->set_phases_info (dep_engine, env, phases_info))

/** Build the service.
    @sa axis2_dep_engine_ops#build_svc */
#define AXIS2_DEP_ENGINE_BUILD_SVC(dep_engine, env, svc, file_name) \
      ((dep_engine)->ops->build_svc (dep_engine, env, svc, file_name))

/** Builds the module.
    @sa axis2_dep_engine_ops#build_module */
#define AXIS2_DEP_ENGINE_BUILD_MODULE(dep_engine, env, module_archive, conf) \
      ((dep_engine)->ops->build_module (dep_engine, env, module_archive, conf))

/** Gets the repos path.
    @sa axis2_dep_engine_ops#get_repos_path */
#define AXIS2_DEP_ENGINE_GET_REPOS_PATH(dep_engine, env) \
      ((dep_engine)->ops->get_repos_path (dep_engine, env))

/** Sets the current file item.
    @sa axis2_dep_engine_ops#set_current_file_item */
#define AXIS2_DEP_ENGINE_SET_CURRENT_FILE_ITEM(dep_engine, env, file_data) \
      ((dep_engine)->ops->set_current_file_item (dep_engine, env, file_data))

/** Sets the arch reader.
    @sa axis2_dep_engine_ops#set_arch_reader */
#define AXIS2_DEP_ENGINE_SET_ARCH_READER(dep_engine, env, arch_reader) \
      ((dep_engine)->ops->set_arch_reader (dep_engine, env, arch_reader))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_DEP_ENGINE_H */
