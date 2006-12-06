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

#ifndef AXIS2_MODULE_BUILDER_H
#define AXIS2_MODULE_BUILDER_H

/** @defgroup axis2_module_builder Module Builder
 * @ingroup axis2_deployment
 * @{
 */

/**
 * @file axis2_module_builder.h
 * @brief Axis2 Module Builder interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_module_desc.h>
#include <axis2_dep_engine.h>
#include <axis2_desc_builder.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
    /** Type name for struct axis2_module_builder */
    typedef struct axis2_module_builder axis2_module_builder_t;
    /** Type name for struct axis2_module_builder_ops */
    typedef struct axis2_module_builder_ops axis2_module_builder_ops_t;

    /**
     * Module Builder ops struct
     * Encapsulator struct for ops of axis2_module_builder
     */
    struct axis2_module_builder_ops
    {
        /** 
         * De-allocate memory
         * @param module_builder pointer to module builder
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_module_builder_t *module_builder,
                    const axis2_env_t *env);
        /**
         * @param module_builder pointer to module builder
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                populate_module)(
                    axis2_module_builder_t *module_builder,
                    const axis2_env_t *env);

    };

    /**
     * Module Builder struct 
     */
    struct axis2_module_builder
    {
        /** Operations of Module Builder */
        axis2_module_builder_ops_t *ops;
        struct axis2_desc_builder *desc_builder;
    };

    /**
     * Creates module builder struct
     * @param env pointer to environment struct
     * @return pointer to newly created module builder
     */
    AXIS2_EXTERN axis2_module_builder_t *AXIS2_CALL
    axis2_module_builder_create(
        const axis2_env_t *env);

    /**
     * Creates module builder struct
     * @param env pointer to environment struct
     * @param file_name pointer to file name
     * @param dep_engine pointer to deployment engine
     * @return pointer to newly created module builder
     */
    AXIS2_EXTERN axis2_module_builder_t *AXIS2_CALL
    axis2_module_builder_create_with_file_and_dep_engine_and_module(
        const axis2_env_t *env,
        axis2_char_t *file_name,
        struct axis2_dep_engine *dep_engine,
        axis2_module_desc_t *module);

/*************************** Function macros **********************************/

/** Frees the module builder
    @sa axis2_module_builder_ops#free */
#define AXIS2_MODULE_BUILDER_FREE(module_builder, env) \
      ((module_builder)->ops->free (module_builder, env))

/** Populates the module.
    @sa axis2_module_builder_ops#populate_module */
#define AXIS2_MODULE_BUILDER_POPULATE_MODULE(module_builder, env) \
      ((module_builder)->ops->populate_module (module_builder, env))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_MODULE_BUILDER_H */
