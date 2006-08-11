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

#ifndef AXIS2_SVC_BUILDER_H
#define AXIS2_SVC_BUILDER_H

/** 
 * @defgroup axis2_svc_builder Service Builder
 * @ingroup axis2_deployment
 * @{
 */

/**
 * @file axis2_svc_builder.h
 * @brief Axis2 Service Builder interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_desc_builder.h>
#include <axis2_addr.h>
#include <axis2_dep_engine.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_dep_engine;
    /** Type name for struct axis2_svc_builder */
    typedef struct axis2_svc_builder axis2_svc_builder_t;
    /** Type name for struct axis2_svc_builder_ops */
    typedef struct axis2_svc_builder_ops axis2_svc_builder_ops_t;

    /**
     * Service Builder ops struct
     * Encapsulator struct for ops of axis2_svc_builder
     */
    struct axis2_svc_builder_ops
    {
        /** 
         * De-allocate memory
         * @param svc_builder pointer to service builder
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_svc_builder_t *svc_builder,
                    const axis2_env_t *env);

        /**
         * top most method that is used to populate service from corresponding OM
         * @param svc_builder pointer to service builder
         * @param env pointer to environment struct
         * @param svc_node pointer to service node
         */
        axis2_status_t (AXIS2_CALL *
                populate_svc)(
                    axis2_svc_builder_t *svc_builder,
                    const axis2_env_t *env,
                    axiom_node_t *svc_node);

        /** 
         * @param svc_builder pointer to service builder
         * @param env pointer to environment struct
         * @param module_confs pointer to module configurations
         * @param parent pointer to parent
         * @param svc pointer to service
         */
        axis2_status_t (AXIS2_CALL *
                process_svc_module_conf)(
                    axis2_svc_builder_t *svc_builder,
                    const axis2_env_t *env,
                    axiom_children_qname_iterator_t *module_confs,
                    axis2_param_container_t *parent,
                    axis2_svc_t *svc);

        /**
         * To get the list og modules that is requird to be engage globally
         * @param svc_builder pointer to service builder
         * @param env pointer to environment struct
         * @param module_refs pointer to module refs
         */
        axis2_status_t (AXIS2_CALL *
                process_module_refs)(
                    axis2_svc_builder_t *svc_builder,
                    const axis2_env_t *env,
                    axiom_children_qname_iterator_t *module_refs);

    };

    /**
     * Service Builder struct 
     */
    struct axis2_svc_builder
    {
        /** Operations of service builder */
        axis2_svc_builder_ops_t *ops;
        struct axis2_desc_builder *desc_builder;
    };

    /**
     * Creates svc builder struct
     * @param env pointer to environment struct
     * @return pointer to newly created service builder
     */
    AXIS2_EXTERN axis2_svc_builder_t *AXIS2_CALL
    axis2_svc_builder_create(
        const axis2_env_t *env);

    /**
     * Creates svc builder struct
     * @param env pointer to environment struct
     * @param file_name pointer to file name
     * @param dep_engine pointer to deployment engine
     * @param svc pointer to service
     * @return pointer to newly created service builder
     */
    AXIS2_EXTERN axis2_svc_builder_t *AXIS2_CALL
    axis2_svc_builder_create_with_file_and_dep_engine_and_svc(
        const axis2_env_t *env,
        axis2_char_t *file_name,
        struct axis2_dep_engine *dep_engine,
        axis2_svc_t *svc);

    /**
     * Creates svc builder struct
     * @param env pointer to environment struct
     * @param dep_engine pointer to deployment engine
     * @param svc pointer to service
     * @return pointer to newly created service builder
     */
    AXIS2_EXTERN axis2_svc_builder_t *AXIS2_CALL
    axis2_svc_builder_create_with_dep_engine_and_svc(
        const axis2_env_t *env,
        struct axis2_dep_engine *dep_engine,
        axis2_svc_t *svc);

/*************************** Function macros **********************************/

/** Frees the service builder.
    @sa axis2_svc_builder_ops#free */
#define AXIS2_SVC_BUILDER_FREE(svc_builder, env) \
      ((svc_builder)->ops->free (svc_builder, env))

/** Populates the service.
    @sa axis2_svc_builder_ops#populate_svc */
#define AXIS2_SVC_BUILDER_POPULATE_SVC(svc_builder, env, svc_node) \
      ((svc_builder)->ops->populate_svc (svc_builder, env, svc_node))

/** Process module refs.
    @sa axis2_svc_builder_ops#process_module_refs */
#define AXIS2_SVC_BUILDER_PROCESS_MODULE_REFS(svc_builder, env, module_refs) \
      ((svc_builder)->ops->process_module_refs (svc_builder, env, module_refs))


/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_SVC_BUILDER_H */
