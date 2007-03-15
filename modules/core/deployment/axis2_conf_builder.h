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

#ifndef AXIS2_CONF_BUILDER_H
#define AXIS2_CONF_BUILDER_H

/** 
 * @defgroup axis2_conf_builder Conf Builder
 * @ingroup axis2_deployment
 * @{
 */

/**
 * @file axis2_conf_builder.h
 * @brief Axis2 Conf Builder interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include "axis2_desc_builder.h"
#include <axis2_conf.h>
#include "axis2_dep_engine.h"
#include <axiom_children_qname_iterator.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_desc_builder;
    struct axis2_dep_engine;
    /** Type name for struct axis2_conf_builder */
    typedef struct axis2_conf_builder axis2_conf_builder_t;

    /** 
     * De-allocate memory
     * @param conf_builder pointer to configuration builder
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_conf_builder_free(axis2_conf_builder_t *conf_builder,
        const axis2_env_t *env);

    /** 
     * @param conf_builder pointer to configuration builder
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_conf_builder_populate_conf(axis2_conf_builder_t *conf_builder,
        const axis2_env_t *env);

    /**
     * To get the list og modules that is requird to be engage globally
     * @param conf_builder pointer to configuration builder
     * @param env pointer to environment struct
     * @param module_refs pointer to module refs
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_conf_builder_process_module_refs(axis2_conf_builder_t *conf_builder,
        const axis2_env_t *env,
        axiom_children_qname_iterator_t *module_refs);

    /**
     * Creates conf builder struct
     * @param env pointer to environment struct
     * @return pointer to newly created conf builder
     */
    AXIS2_EXTERN axis2_conf_builder_t *AXIS2_CALL
    axis2_conf_builder_create (const axis2_env_t *env);

    /**
     * Creates conf builder struct
     * @param env pointer to environment struct
     * @param file This is the full path to the server xml file
     * @param dep_engine pointer to dep engine
     * @param conf pointer to conf
     * @return pointer to newly created conf builder
     */

    AXIS2_EXTERN axis2_conf_builder_t *AXIS2_CALL
    axis2_conf_builder_create_with_file_and_dep_engine_and_conf(const axis2_env_t *env,
        axis2_char_t *file,
        struct axis2_dep_engine *dep_engine,
        axis2_conf_t *conf);

/** Frees the conf builder */
#define AXIS2_CONF_BUILDER_FREE(conf_builder, env) \
      axis2_conf_builder_free (conf_builder, env)

/** Populate conf. */
#define AXIS2_CONF_BUILDER_POPULATE_CONF(conf_builder, env) \
      axis2_conf_builder_populate_conf (conf_builder, env)

/** Process module refs. */
#define AXIS2_CONF_BUILDER_PROCESS_MODULE_REFS(conf_builder, env, module_refs) \
      axis2_conf_builder_process_module_refs (conf_builder, env, module_refs)


/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_CONF_BUILDER_H */
