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

#ifndef AXIS2_ARCH_READER_H
#define AXIS2_ARCH_READER_H

/** @defgroup axis2_arch_reader Arch Reader
 * @ingroup axis2_deployment
 * @{
 */

/**
 * @file axis2_arch_reader.h
 * @brief Axis2 Arch Reader interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_conf.h>
#include <axis2_deployment.h>
#include <axis2_arch_file_data.h>
#include <axis2_dep_engine.h>
#include <axis2_file_handler.h>
#include <axiom_element.h>
#include <axis2_desc_builder.h>
#include <axis2_svc_grp_builder.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_conf;
    struct axis2_arch_file_data;
    struct axis2_dep_engine;
    /** Type name for struct axis2_arch_reader */
    typedef struct axis2_arch_reader axis2_arch_reader_t;
    /** Type name for struct axis2_arch_reader_ops */
    typedef struct axis2_arch_reader_ops axis2_arch_reader_ops_t;

    /**
     * Arch Reader ops struct
     * Encapsulator struct for ops of axis2_arch_reader
     */
    struct axis2_arch_reader_ops
    {
        /** 
         * De-allocate memory
         * @param arch_reader pointer to arch reader
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_arch_reader_t *arch_reader,
                    const axis2_env_t *env);


        /**
         * To create a ServiceDescrption <code>AxisService</code>   using given wsdl.
         * If the service.wsdl is there in the arcive file AxisService will be creted 
         * using that, else default AxisService will be created
         * @param arch_reader pointer to arch reader
         * @param env pointer to environment struct
         * @param file pointer to file
         */
        struct axis2_svc *(AXIS2_CALL *
                create_svc)(
                    axis2_arch_reader_t *arch_reader,
                    const axis2_env_t *env,
                    struct axis2_arch_file_data *file);

        /**
         * it take two arguments filename and refereance to DeployEngine
         * @param arch_reader pointer to arch reader
         * @param env pointer to environment struct
         * @param file_name pointer to file name
         * @param dep_engine pointer to deployment engine
         * @param svc_grp pointer to service group
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                process_svc_grp)(
                    axis2_arch_reader_t *arch_reader,
                    const axis2_env_t *env,
                    axis2_char_t *file_name,
                    struct axis2_dep_engine *dep_engine,
                    axis2_svc_grp_t *svc_grp);

        /** 
         * @param arch_reader pointer to arch reader
         * @param env pointer to environment struct
         * @param file_path pointer to file path
         * @param dep_engine pointer to deployment engine
         * @param svc_grp pointer to service group
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                build_svc_grp)(
                    axis2_arch_reader_t *arch_reader,
                    const axis2_env_t *env,
                    axis2_char_t *file_path,
                    struct axis2_dep_engine *dep_engine,
                    struct axis2_svc_grp *svc_grp);

        /** 
         * @param arch_reader pointer to arch reader
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                read_module_arch)(
                    axis2_arch_reader_t *arch_reader,
                    const axis2_env_t *env,
                    axis2_char_t *file_path,
                    struct axis2_dep_engine *dep_engine,
                    axis2_module_desc_t *module);

        /** 
         * @param arch_reader pointer to arch reader
         * @param env pointer to environment struct
         * @param module_name pointer to module name
         */
        axis2_file_t *(AXIS2_CALL *
                create_module_arch)(
                    axis2_arch_reader_t *arch_reader,
                    const axis2_env_t *env,
                    axis2_char_t *module_name);
    };

    /**
     *Arch Reader struct 
     */
    struct axis2_arch_reader
    {
        /** Operations of arch reader */
        axis2_arch_reader_ops_t *ops;
    };

    /**
     * Creates arch reader struct
     * @param env pointer to environment struct
     * @return pointer to newly created arch reader
     */
    AXIS2_EXTERN axis2_arch_reader_t *AXIS2_CALL
    axis2_arch_reader_create(
        const axis2_env_t *env);


/*************************** Function macros **********************************/

/** Frees the arch reader.
    @sa axis2_arch_reader_ops#free */
#define AXIS2_ARCH_READER_FREE(arch_reader, env) \
      ((arch_reader)->ops->free (arch_reader, env))

/** Creates the service.
    @sa axis2_arch_reader_ops#create_svc */
#define AXIS2_ARCH_READER_CREATE_SVC(arch_reader, env, file) \
      ((arch_reader)->ops->create_svc (arch_reader, env, file))

/** Process service group.
    @sa axis2_arch_reader_ops#process_svc_grp */
#define AXIS2_ARCH_READER_PROCESS_SVC_GRP(arch_reader, env, file_path, dep_engine, svc_grp) \
      ((arch_reader)->ops->process_svc_grp (arch_reader, env, file_path, dep_engine, svc_grp))

/** Builds the service group.
    @sa axis2_arch_reader_ops#build_svc_grp */
#define AXIS2_ARCH_READER_BUILD_SVC_GRP(arch_reader, env, file_name, dep_engine, svc_grp) \
      ((arch_reader)->ops->build_svc_grp (arch_reader, env, file_name, dep_engine, svc_grp))

/** Reads the module archive.
    @sa axis2_arch_reader_ops#read_module_arch */
#define AXIS2_ARCH_READER_READ_MODULE_ARCH(arch_reader, env, file_path, dep_engine, module) \
      ((arch_reader)->ops->read_module_arch (arch_reader, env, file_path, dep_engine, module))

/** Creates the module archive.
    @sa axis2_arch_reader_ops#create_module_arch */
#define AXIS2_ARCH_READER_CREATE_MODULE_ARCH(arch_reader, env, module_name) \
      ((arch_reader)->ops->create_module_arch (arch_reader, env, module_name))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_ARCH_READER_H */
