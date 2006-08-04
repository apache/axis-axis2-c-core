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

#ifndef AXIS2_PHASE_RESOLVER_H
#define AXIS2_PHASE_RESOLVER_H

/**
 * @defgroup axis2_phase_resolver phase resolver
 * @ingroup axis2
 * @{
 * @}
 */

/** @defgroup axis2_phase_res phsae resolver
 * @ingroup axis2_phase_resolver
 * Description.
 * @{
 */

/**
 * @file axis2_phase_resolver.h
 * @brief Axis2 Wsdl soap op interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <axis2_handler_desc.h>
#include <axis2_phase.h>
#include <axis2_phase_rule.h>
#include <axis2_handler.h>
#include <axis2_handler_desc.h>
#include <axis2_flow.h>
#include <axis2_module_desc.h>
#include <axis2_phase_holder.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_phase;
    struct axis2_handler_desc;
    struct axis2_module_desc;
    struct axis2_handler;
    struct axis2_phase_rule;
    struct axis2_svc;
    struct axis2_conf;
    struct axis2_op;
    struct axis2_phase_holder;
    /** Type name for axis2_phase_resolver */
    typedef struct axis2_phase_resolver axis2_phase_resolver_t;
    /** Type name for axis2_phase_resolver_ops */
    typedef struct axis2_phase_resolver_ops axis2_phase_resolver_ops_t;

    /**
     * Wsdl Phase Resolver ops struct
     * Encapsulator struct for ops of axis2_phase_resolver
     */
    AXIS2_DECLARE_DATA struct axis2_phase_resolver_ops
    {
        /** De-allocate memory
    * @param phase_recolver pointer to phase resolver
    * @param env pointer to environment struct
         * @return status code
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_phase_resolver_t *phase_resolver,
                    const axis2_env_t *env);

        /**
    * @param phase_resolver pointer to phase resolver
    * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                build_chains)(
                    axis2_phase_resolver_t *phase_resolver,
                    const axis2_env_t *env);

        /**
         * To build the opration for the opeartion which the module going to be added
    * @param phase_resolver pointer to phase resolver
         * @param env pointer to environment struct
    * @param op pointer to operation
         */
        axis2_status_t (AXIS2_CALL *
                build_module_op)(
                    axis2_phase_resolver_t *phase_resolver,
                    const axis2_env_t *env,
                    struct axis2_op *op);

        /**
         * Method buildTranspotsChains
    * @param phase_resolver pointer to phase resolver
    * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                build_transport_chains)(
                    axis2_phase_resolver_t *phase_resolver,
                    const axis2_env_t *env);


   /**
    * @param phase_resolver pointer to phase resolver
    * @param env pointer to environment struct
    * @param module pointer to module
    */
        axis2_status_t (AXIS2_CALL *
                engage_module_globally)(
                    axis2_phase_resolver_t *phase_resolver,
                    const axis2_env_t *env,
                    struct axis2_module_desc *module);

        /**
         * To engage modules come form global
    * @param phase_resolver pointer to phase resolver
    * @param env pointer to environment struct
    * @param svc pointer to service
    * @param module_desc pointer to module descriptoin
         */
        axis2_status_t (AXIS2_CALL *
                engage_module_to_svc_from_global)(
                    axis2_phase_resolver_t *phase_resolver,
                    const axis2_env_t *env,
                    struct axis2_svc *svc,
                    struct axis2_module_desc *module_desc);

   /**
    * @param phase_resolver pointer to phase resolver
    * @param env pointer to environmnet struct
    * @param svc pointer to service
    * @param module_desc pointedr to module description
    */
        axis2_status_t (AXIS2_CALL *
                engage_module_to_svc)(
                    axis2_phase_resolver_t *phase_resolver,
                    const axis2_env_t *env,
                    struct axis2_svc *svc,
                    struct axis2_module_desc *module_desc);

   /**
    * @param phase_resolver pointer to phase resolver
    * @param env pointer to environment struct
    * @param axis_op pointer to axis operation
    * @param pointer to module description
    */
        axis2_status_t (AXIS2_CALL *
                engage_module_to_op)(
                    axis2_phase_resolver_t *phase_resolver,
                    const axis2_env_t *env,
                    struct axis2_op *axis_op,
                    struct axis2_module_desc *module_desc);


    };

    /**
     * Phase Resolver struct  
     */
    AXIS2_DECLARE_DATA struct axis2_phase_resolver
    {
   /** Operations of phase resolver */
        axis2_phase_resolver_ops_t *ops;
    };

    /**
     * Creates phase resolver struct
     * @param env pointer to environment struct
     * @return pointer to newly created phase resolver
     */
    AXIS2_EXTERN axis2_phase_resolver_t * AXIS2_CALL
    axis2_phase_resolver_create (
        const axis2_env_t *env);

    /**
     * default constructor , to obuild chains for GlobalDescription
     * @param env pointer to environment struct
     * @param axis2_config pointer to aixs2 configuratoin
     */
    AXIS2_EXTERN axis2_phase_resolver_t * AXIS2_CALL
    axis2_phase_resolver_create_with_config (
        const axis2_env_t *env,
        struct axis2_conf *axis2_config);

    /**
     * @param env pointer to environment struct
     * @param aixs2_config pointer to aixs2_config
     * @param svc pointer to service
     */
    AXIS2_EXTERN axis2_phase_resolver_t * AXIS2_CALL
    axis2_phase_resolver_create_with_config_and_svc (
        const axis2_env_t *env,
        struct axis2_conf *axis2_config,
        struct axis2_svc *svc);

/*************************** Function macros **********************************/

/** Frees the phase resolver.
*   @sa axis2_phase_resolver_ops#free */
#define AXIS2_PHASE_RESOLVER_FREE(phase_resolver, env) \
      ((phase_resolver)->ops->free (phase_resolver, env))

/** Build chains.
*   @sa axis2_phase_resolver_ops#build_chains */
#define AXIS2_PHASE_RESOLVER_BUILD_CHAINS(phase_resolver, env) \
      ((phase_resolver)->ops->build_chains (phase_resolver, env))

/** Build module operations.
*   @sa axis2_phase_resolver_ops#build_module_op */
#define AXIS2_PHASE_RESOLVER_BUILD_MODULE_OP(phase_resolver, env, op) \
      ((phase_resolver)->ops->build_module_op (phase_resolver, env, op))

/** Build transport chains.
*   @sa axis2_phase_resolver_ops#build_transport_chains */
#define AXIS2_PHASE_RESOLVER_BUILD_TRANSPORT_CHAINS(phase_resolver, env) \
      ((phase_resolver)->ops->build_transport_chains (phase_resolver, env))

/** Engages module globally.
*   @sa axis2_phase_resolver_ops#engage_module_globally */
#define AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_GLOBALLY(phase_resolver, env, module) \
      ((phase_resolver)->ops->engage_module_globally (phase_resolver, env, module))

/** Engages module to service from global.
*   @sa axis2_phase_resolver_ops#engage_module_to_svc_from_global */
#define AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_SVC_FROM_GLOBAL(phase_resolver, env, svc, module_desc) \
      ((phase_resolver)->ops->engage_module_to_svc_from_global (phase_resolver, env, svc, module_desc))

/** Engage module to service.
*   @sa axis2_phase_resolver_ops#engage_module_to_svc */
#define AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_SVC(phase_resolver, env, svc, module_desc) \
      ((phase_resolver)->ops->engage_module_to_svc (phase_resolver, env, svc, module_desc))

/** Engage module to operation.
*   @sa axis2_phase_resolver_ops#engage_module_to_op */
#define AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_OP(phase_resolver, env, axis_op, module_desc) \
      ((phase_resolver)->ops->engage_module_to_op (phase_resolver, env, axis_op, module_desc))


/*************************** End of function macros ***************************/
/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_PHASE_RESOLVER_H */
