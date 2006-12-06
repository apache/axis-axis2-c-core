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

#ifndef AXIS2_ARCH_FILE_DATA_H
#define AXIS2_ARCH_FILE_DATA_H

/** @defgroup axis2_arch_file_data Arch File Data
 * @ingroup axis2_deployment
 * @{
 */


/**
 * @file axis2_arch_file_data.h
 * @brief Axis2 Arch File Data interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_conf.h>
#include <axis2_array_list.h>
#include <axis2_dir_handler.h>
#include <axis2_file.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_conf;
    
    /** Type name for struct axis2_arch_fila_data */
    typedef struct axis2_arch_file_data axis2_arch_file_data_t;
    /** Type name for struct axis2_arch_fila_data_ops */
    typedef struct axis2_arch_file_data_ops axis2_arch_file_data_ops_t;

    /**
     * Arch File Data ops struct
     * Encapsulator struct for ops of axis2_arch_file_data
     */
    struct axis2_arch_file_data_ops
    {
        /** 
         * De-allocate memory
         * @param arch_file_data pointer to arch_file_data
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_arch_file_data_t *arch_file_data,
                    const axis2_env_t *env);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_msg_recv)(
                    const axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         * @param msg_recv pointer to message receiver
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_msg_recv)(
                    axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env,
                    axis2_char_t *msg_recv);

        /**
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         * @return the name of the contained file.
         */
        axis2_char_t *(AXIS2_CALL *
                get_name)(
                    const axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env);
        /**
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         * @return the service name. if contained file is not null this is the
         * file name. else this is the name property
         */
        axis2_char_t *(AXIS2_CALL *
                get_svc_name)(
                    const axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         */
        int (AXIS2_CALL *
                get_type)(
                    const axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         */
        axis2_file_t *(AXIS2_CALL *
                get_file)(
                    const axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_module_name)(
                    const axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         * @param module_name pointer to module_name
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_module_name)(
                    axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env,
                    axis2_char_t *module_name);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_module_dll_name)(
                    const axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_module_dll_name)(
                    axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env,
                    axis2_char_t *module_dll_name);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_svc)(
                    axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env,
                    struct axis2_svc *svc_desc);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         */
        struct axis2_svc *(AXIS2_CALL *
                get_svc)(
                    const axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env,
                    axis2_char_t *svc_name);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         */
        axis2_hash_t *(AXIS2_CALL *
                get_svc_map)(
                    const axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_deployable_svcs)(
                    const axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env);

        /** 
         * @param file_data pointer to arch_file_data
         * @param env pointer to environment struct
         * @param deployable_svcs pointer to deployable services
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_deployable_svcs)(
                    axis2_arch_file_data_t *file_data,
                    const axis2_env_t *env,
                    axis2_array_list_t *deployable_svcs);


    };

    /**
     * Arch File Data struct 
     */
    struct axis2_arch_file_data
    {
        /** Ops of Arch File Data */
        axis2_arch_file_data_ops_t *ops;
    };

    /**
     * Creates arch file data struct
     * @param env pointer to environment struct
     * @return pointer to newly created arch file data
     */
    AXIS2_EXTERN axis2_arch_file_data_t *AXIS2_CALL
    axis2_arch_file_data_create(
        const axis2_env_t *env);

    /**
     * Creates arch file data struct
     * @param env pointer to environment struct
     * @param type type
     * @param file folder name of service or module
     * @return pointer to newly created arch file data
     */
    AXIS2_EXTERN axis2_arch_file_data_t *AXIS2_CALL
    axis2_arch_file_data_create_with_type_and_file(
        const axis2_env_t *env,
        int type,
        axis2_file_t *file);

    /**
     * Creates arch file data struct
     * @param env pointer to environment struct
     * @param type type
     * @param name pointer to name
     * @return pointer to newly created arch file data
     */
    AXIS2_EXTERN axis2_arch_file_data_t *AXIS2_CALL
    axis2_arch_file_data_create_with_type_and_name(
        const axis2_env_t *env,
        int type,
        const axis2_char_t *name);

/*************************** Function macros **********************************/

/** Frees the arch file data struct.
    @sa axis2_arch_file_data_ops#free */
#define AXIS2_ARCH_FILE_DATA_FREE(arch_file_data, env) \
      ((arch_file_data)->ops->free (arch_file_data, env))

/** Gets the message receiver.
    @sa axis2_arch_file_data_ops#get_msg_recv */
#define AXIS2_ARCH_FILE_DATA_GET_MSG_RECV(arch_file_data, env) \
      ((arch_file_data)->ops->get_msg_recv (arch_file_data, env))

/** Sets the message receiver.
    @sa axis2_arch_file_data_ops#set_msg_recv */
#define AXIS2_ARCH_FILE_DATA_SET_MSG_RECV(arch_file_data, env, msg_recv) \
      ((arch_file_data)->ops->set_msg_recv (arch_file_data, env, msg_recv))

/** Gets the name.
    @sa axis2_arch_file_data_ops#get_name */
#define AXIS2_ARCH_FILE_DATA_GET_NAME(arch_file_data, env) \
      ((arch_file_data)->ops->get_name (arch_file_data, env))

/** Gets the service name.
    @sa axis2_arch_file_data_ops#get_svc_name */
#define AXIS2_ARCH_FILE_DATA_GET_SVC_NAME(arch_file_data, env) \
      ((arch_file_data)->ops->get_svc_name (arch_file_data, env))

/** Gets the type.
    @sa axis2_arch_file_data_ops#get_type */
#define AXIS2_ARCH_FILE_DATA_GET_TYPE(arch_file_data, env) \
      ((arch_file_data)->ops->get_type (arch_file_data, env))

/** Gets the file.
    @sa axis2_arch_file_data_ops#get_file */
#define AXIS2_ARCH_FILE_DATA_GET_FILE(arch_file_data, env) \
      ((arch_file_data)->ops->get_file (arch_file_data, env))

/** Gets the module name.
    @sa axis2_arch_file_data_ops#get_module */
#define AXIS2_ARCH_FILE_DATA_GET_MODULE_NAME(arch_file_data, env) \
      ((arch_file_data)->ops->get_module_name (arch_file_data, env))

/** Sets the module name.
    @sa axis2_arch_file_data_ops#set_module_name */
#define AXIS2_ARCH_FILE_DATA_SET_MODULE_NAME(arch_file_data, env, module_name) \
      ((arch_file_data)->ops->set_module_name (arch_file_data, env, module_name))

/** Gets the module dll name.
    @sa axis2_arch_file_data_ops#get_module_dll_name */
#define AXIS2_ARCH_FILE_DATA_GET_MODULE_DLL_NAME(arch_file_data, env) \
      ((arch_file_data)->ops->get_module_dll_name (arch_file_data, env))

/** Sets the module dll name.
    @sa axis2_arch_file_data_ops#set_module_dll_name */
#define AXIS2_ARCH_FILE_DATA_SET_MODULE_DLL_NAME(arch_file_data, env, class_name) \
      ((arch_file_data)->ops->set_module_dll_name (arch_file_data, env, class_name))

/** Adds the service.
    @sa axis2_arch_file_data_ops#add_svc */
#define AXIS2_ARCH_FILE_DATA_ADD_SVC(arch_file_data, env, svc_desc) \
      ((arch_file_data)->ops->add_svc (arch_file_data, env, svc_desc))

/** Gets the service.
    @sa axis2_arch_file_data_ops#get_svc */
#define AXIS2_ARCH_FILE_DATA_GET_SVC(arch_file_data, env, svc_name) \
      ((arch_file_data)->ops->get_svc (arch_file_data, env, svc_name))

/** Gets the service map.
    @sa axis2_arch_file_data_ops#get_svc_map */
#define AXIS2_ARCH_FILE_DATA_GET_SVC_MAP(arch_file_data, env) \
      ((arch_file_data)->ops->get_svc_map (arch_file_data, env))

/** Gets the deployable services.
    @sa axis2_arch_file_data_ops#get_deployable_svcs */
#define AXIS2_ARCH_FILE_DATA_GET_DEPLOYABLE_SVCS(arch_file_data, env) \
      ((arch_file_data)->ops->get_deployable_svcs (arch_file_data, env))

/** Sets the deployable services.
    @sa axis2_arch_file_data_ops#set_deployable_svcs */
#define AXIS2_ARCH_FILE_DATA_SET_DEPLOYABLE_SVCS(arch_file_data, env, deployable_svcs) \
      ((arch_file_data)->ops->set_deployable_svcs (arch_file_data, env, deployable_svcs))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_ARCH_FILE_DATA_H */
