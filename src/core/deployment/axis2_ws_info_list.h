
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

#ifndef AXIS2_WS_INFO_LIST_H
#define AXIS2_WS_INFO_LIST_H

/** @defgroup axis2_ws_info_list Ws Info List
 * @ingroup axis2_deployment
 * @{
 */

/**
 * @file axis2_ws_info_list.h
 * @brief Axis2 Ws Info List interface. Ws Info List is the list of ws_info structs.
 *  Each ws_info construct contain file information for module or service configuration file.
 */

#include <axis2_const.h>
#include <axutil_error.h>
#include <axis2_defines.h>
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_qname.h>
#include <axis2_conf.h>
#include "axis2_deployment.h"
#include <axutil_file_handler.h>
#include <axutil_dir_handler.h>
#include "axis2_ws_info.h"
#include "axis2_arch_file_data.h"
#include "axis2_dep_engine.h"

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_ws_info;
    struct axis2_dep_engine;

    /** Type name for struct axis2_ws_info_list */
    typedef struct axis2_ws_info_list axis2_ws_info_list_t;

    /**
     * De-allocate memory
     * @param ws_info_list pointer to ws info list
     * @param env pointer to environment struct
     * @return void
     */
    AXIS2_EXTERN void AXIS2_CALL
    axis2_ws_info_list_free(
        axis2_ws_info_list_t * ws_info_list,
        const axutil_env_t * env);

    /**
     * This method is used to initialize the ws info list.
     * @param info_list pointer to ws info list
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_ws_info_list_init(
        axis2_ws_info_list_t * info_list,
        const axutil_env_t * env);

    /**
     * This will add the deployment info struct(ws_info) into the deployment info 
     * list and deployment info file to the deployment engine as new 
     * service or module deployment info struct/file.
     * In doing this, it first create an deployment info struct called ws_info 
     * to keep the file info that will be added to deployment info file 
     * list and then create deployment info file called arch_file_data that will 
     * be added to deployment engine for actual deployment of the service/module.
     *
     * This will add two entries to deployment engine one for new Deployment and 
     * other for undeployment.
     * @param info_list pointer to ws info list
     * @param env pointer to environment struct
     * @param file actual axis2 configuration file for module/service.
     * @param type indicate either service or module
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL

    axis2_ws_info_list_add_ws_info_item(
        axis2_ws_info_list_t * info_list,
        const axutil_env_t * env,
        axutil_file_t * file,
        int type);

    /**
     * This method is used to check whether the service/module configuration 
     * file exist and if so it will return related ws_info object to the file, 
     * else return NULL;
     * @param info_list pointer to ws info list
     * @param env pointer to environment struct
     * @param file_name pointer to file name
     */
    AXIS2_EXTERN axis2_ws_info_t *AXIS2_CALL

    axis2_ws_info_list_get_file_item(
        axis2_ws_info_list_t * info_list,
        const axutil_env_t * env,
        axis2_char_t * file_name);

    /**
     * Compare the last update dates of both files and if those differ
     * it will be assumed as the file has been modified.
     * @param info_list pointer to ws info list
     * @param env pointer to environment struct
     * @param file pointer to servie/module configuration file.
     * @param ws_info pointer to ws info struct.
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_ws_info_list_is_modified(
        axis2_ws_info_list_t * info_list,
        const axutil_env_t * env,
        axutil_file_t * file,
        struct axis2_ws_info *ws_info);

    /**
     * To check whether the deployment info file is alredy in the list.
     * @param info_list pointer to ws info list
     * @param env pointer to environment struct
     * @param file_name pointer to file name
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_ws_info_list_is_file_exist(
        axis2_ws_info_list_t * info_list,
        const axutil_env_t * env,
        axis2_char_t * file_name);

    /**
     * This is to check whether to undeploy ws info struct.
     * What this realy does is, it check ws_info list and current deployment 
     * info name list. If ws_info exists in the ws_info_list but it's 
     * corresponding file name does not exist in the current deploymet file 
     * name list then, struct is deemed non existant. ie. that is hot 
     * undeployment.
     * @param info_list pointer to ws info list
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL

    axis2_ws_info_list_check_for_undeploy(
        axis2_ws_info_list_t * info_list,
        const axutil_env_t * env);

    /**
     * @param info_list pointer to ws info list
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_ws_info_list_update(
        axis2_ws_info_list_t * info_list,
        const axutil_env_t * env);

    /**
     * Creates ws info list struct
     * @param env pointer to environment struct
     * @param dep_engine pointer to deployment engine
     * @return pointer to newly created ws info list
     */
    AXIS2_EXTERN axis2_ws_info_list_t *AXIS2_CALL

    axis2_ws_info_list_create_with_dep_engine(
        const axutil_env_t * env,
        struct axis2_dep_engine *dep_engine);


    /** @} */

#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_WS_INFO_LIST_H */
