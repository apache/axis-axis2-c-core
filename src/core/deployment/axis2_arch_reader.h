
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

#ifndef AXIS2_ARCH_READER_H
#define AXIS2_ARCH_READER_H

/** @defgroup axis2_arch_reader Arch Reader
 * @ingroup axis2_deployment
 * @{
 */

#include <axis2_const.h>
#include <axutil_error.h>
#include <axis2_defines.h>
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_qname.h>
#include <axis2_conf.h>
#include <axis2_deployment.h>
#include <axis2_arch_file_data.h>
#include <axis2_dep_engine.h>
#include <axutil_file_handler.h>
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

    /**
     * De-allocate memory
     * @param arch_reader pointer to arch reader
     * @param env pointer to environment struct
     * @return void
     */
    AXIS2_EXTERN void AXIS2_CALL
    axis2_arch_reader_free(
        axis2_arch_reader_t * arch_reader,
        const axutil_env_t * env);

    /**
     * To create a service descrption <code>axis2_svc_t</code> using given 
     * deployment info file.
     * @param env pointer to environment struct
     * @param file pointer to file
     */
    AXIS2_EXTERN struct axis2_svc *AXIS2_CALL
                axis2_arch_reader_create_svc(
                    const axutil_env_t * env,
                    struct axis2_arch_file_data *file);

    /**
     * Construct the path to the service group configuration file(services.xml)
     * using the passed file name and populate the passed service group 
     * description.
     * @param arch_reader pointer to arch reader
     * @param env pointer to environment struct
     * @param file_name pointer to service group configuration file.
     * @param dep_engine pointer to deployment engine
     * @param svc_grp pointer to service group to be populated.
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_arch_reader_process_svc_grp(
        axis2_arch_reader_t * arch_reader,
        const axutil_env_t * env,
        axis2_char_t * file_name,
        struct axis2_dep_engine *dep_engine,
        axis2_svc_grp_t * svc_grp);

    /**
     * Populate the passed service group description using the service group 
     * configuration file(services.xml).
     * @param arch_reader pointer to arch reader
     * @param env pointer to environment struct
     * @param file_path path to the service group configuration file(services.xml)
     * @param dep_engine pointer to deployment engine
     * @param svc_grp pointer to service group
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_arch_reader_build_svc_grp(
        axis2_arch_reader_t * arch_reader,
        const axutil_env_t * env,
        axis2_char_t * file_path,
        struct axis2_dep_engine *dep_engine,
        struct axis2_svc_grp *svc_grp);

    /**
     * Construct the path to the module configuration file(module.xml)
     * using the passed file name and populate the passed module description.
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL

    axis2_arch_reader_read_module_arch(
        const axutil_env_t * env,
        axis2_char_t * file_path,
        struct axis2_dep_engine *dep_engine,
        axis2_module_desc_t * module);

    /**
     * Create an axis2 file using the passed module name.
     * @param env pointer to environment struct
     * @param module_name pointer to module name
     */
    AXIS2_EXTERN axutil_file_t *AXIS2_CALL

    axis2_arch_reader_create_module_arch(
        const axutil_env_t * env,
        axis2_char_t * module_name);

    /**
     * Creates arch reader struct
     * @param env pointer to environment struct
     * @return pointer to newly created arch reader
     */
    AXIS2_EXTERN axis2_arch_reader_t *AXIS2_CALL
    axis2_arch_reader_create(
        const axutil_env_t * env);

    /** @} */

#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_ARCH_READER_H */
