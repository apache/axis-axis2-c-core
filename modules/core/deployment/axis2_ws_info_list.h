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

#ifndef AXIS2_WS_INFO_LIST_H
#define AXIS2_WS_INFO_LIST_H

/** @defgroup axis2_ws_info_list Ws Info List
 * @ingroup axis2_deployment
 * @{
 */

/**
 * @file axis2_ws_info_list.h
 * @brief Axis2 Ws Info List interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_conf.h>
#include "axis2_deployment.h"
#include <axis2_file_handler.h>
#include <axis2_dir_handler.h>
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
    /** Type name for struct axis2_ws_info_list_ops  */
    typedef struct axis2_ws_info_list_ops axis2_ws_info_list_ops_t;

    /**
     * Ws Info List ops struct
     * Encapsulator struct for ops of axis2_ws_info_list
     */
    struct axis2_ws_info_list_ops
    {
        /** 
         * De-allocate memory
         * @param ws_info_list pointer to ws info list
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_ws_info_list_t *ws_info_list,
                    const axis2_env_t *env);

        /**
         * This method is used to initialize the vector
         * @param info_list pointer to ws info list
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                init)(
                    axis2_ws_info_list_t *info_list,
                    const axis2_env_t *env);

        /**
         * First it check whether the file is already available in the
         * system call isFileExist , if it is not deployed yet then it will add
         * that to jarlist and to the deployment engine as new service or module
         * in adding new item to jarlist it first create optimice and requird object to
         * keep those infor call WSInfo and that will be added to jarist and actual
         * jar file will be added to DeploymentEngine
         * <p/>
         * If it is alredy exsit then it check whether it has been updated
         * then change the last update date of the wsInfo and added two entries to 
         * DeploymentEngine one for New Deployment and other for undeployment
         * @param info_list pointer to ws info list
         * @param env pointer to environment struct
         * @param file actual jar files for either Module or service
         * @param type indicate either Service or Module
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_ws_info_item)(
                    axis2_ws_info_list_t *info_list,
                    const axis2_env_t *env,
                    axis2_file_t *file,
                    int type);

        /**
         * This method is used to check whether the file exist and if so
         * it will return related wsinfo object to the file, else return null;
         * @param info_list pointer to ws info list
         * @param env pointer to environment struct
         * @param file_name pointer to file name
         */
        axis2_ws_info_t *(AXIS2_CALL *
                get_file_item)(
                    axis2_ws_info_list_t *info_list,
                    const axis2_env_t *env,
                    axis2_char_t *file_name);

        /**
         * compare the last update dates of both files and if those differ
         * it will assumed as the file has been modified
         * @param info_list pointer to ws info list
         * @param env pointer to environment struct
         * @param file pointer to file
         * @param ws_info pointer to ws info
         */
        axis2_bool_t (AXIS2_CALL *
                is_modified)(
                    axis2_ws_info_list_t *info_list,
                    const axis2_env_t *env,
                    axis2_file_t *file,
                    struct axis2_ws_info *ws_info);

        /**
         * to check whether the file is alredy in the list
         * @param info_list pointer to ws info list
         * @param env pointer to environment struct
         * @param file_name pointer to file name
         */
        axis2_bool_t (AXIS2_CALL *
                is_file_exist)(
                    axis2_ws_info_list_t *info_list,
                    const axis2_env_t *env,
                    axis2_char_t *file_name);

        /**
         * this is to check , undeploye WS
         * What this relly does is it check older jars files and
         * current jars. If name of the old jar file does not exit in the currentjar
         * list then it is assumed that the jar file has been removed
         * that is hot undeployment
         * @param env pointer to environment struct
         * @param file_name pointer to file name
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                check_for_undeploy)(
                    axis2_ws_info_list_t *info_list,
                    const axis2_env_t *env);


        /**
         * @param env pointer to environment struct
         * @param file_name pointer to file name
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                update)(
                    axis2_ws_info_list_t *info_list,
                    const axis2_env_t *env);

    };

    /**
     * Ws Info List struct 
     */
    struct axis2_ws_info_list
    {
        /** Operations of ws info list */
        axis2_ws_info_list_ops_t *ops;
    };

    /**
     * Creates ws info list struct
     * @param env pointer to environment struct
     * @param dep_engine pointer to deployment engine
     * @return pointer to newly created ws info list
     */
    AXIS2_EXTERN axis2_ws_info_list_t *AXIS2_CALL
    axis2_ws_info_list_create_with_dep_engine(
        const axis2_env_t *env,
        struct axis2_dep_engine *dep_engine);


/*************************** Function macros **********************************/

/** Frees the ws info list.
    @sa axis2_ws_info_list_ops#free */
#define AXIS2_WS_INFO_LIST_FREE(ws_info_list, env) \
      ((ws_info_list)->ops->free (ws_info_list, env))

/** Initialize the ws info list.
    @sa axis2_ws_info_list_ops#init */
#define AXIS2_WS_INFO_LIST_INIT(ws_info_list, env) \
      ((ws_info_list)->ops->init (ws_info_list, env))

/** Add ws info item.
    @sa axis2_ws_info_list_ops#add_ws_info_item */
#define AXIS2_WS_INFO_LIST_ADD_WS_INFO_ITEM(ws_info_list, env, file, type) \
      ((ws_info_list)->ops->add_ws_info_item (ws_info_list, env, file, type))

/** Get file item.
    @sa axis2_ws_info_list_ops#get_file_item */
#define AXIS2_WS_INFO_LIST_GET_FILE_ITEM(ws_info_list, env, file_name) \
      ((ws_info_list)->ops->get_file_item (ws_info_list, env, file_name))

/** Is modified.
    @sa axis2_ws_info_list_ops#is_modified */
#define AXIS2_WS_INFO_LIST_IS_MODIFIED(ws_info_list, env, file, ws_info) \
      ((ws_info_list)->ops->is_modified (ws_info_list, env, file, ws_info))

/** Is file exist.
    @sa axis2_ws_info_list_ops#is_file_exist */
#define AXIS2_WS_INFO_LIST_IS_FILE_EXIST(ws_info_list, env, file_name) \
      ((ws_info_list)->ops->is_file_exist (ws_info_list, env, file_name))

/** Check for undeploy.
    @sa axis2_ws_info_list_ops#check_for_undeploy */
#define AXIS2_WS_INFO_LIST_CHECK_FOR_UNDEPLOY(ws_info_list, env) \
      ((ws_info_list)->ops->check_for_undeploy (ws_info_list, env))

/** Update.
    @sa axis2_ws_info_list_ops#update */
#define AXIS2_WS_INFO_LIST_UPDATE(ws_info_list, env) \
      ((ws_info_list)->ops->update (ws_info_list, env))


/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WS_INFO_LIST_H */
