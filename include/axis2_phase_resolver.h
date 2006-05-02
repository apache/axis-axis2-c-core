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
typedef struct axis2_phase_resolver axis2_phase_resolver_t;
typedef struct axis2_phase_resolver_ops axis2_phase_resolver_ops_t;

/** @defgroup axis2_phase_resolver Phase Resolver
 * @ingroup axis2_phase
 * @{
 */

/** 
 * @brief Wsdl Phase Resolver ops struct
 * Encapsulator struct for ops of axis2_phase_resolver
 */
AXIS2_DECLARE_DATA struct axis2_phase_resolver_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_phase_resolver_t *phase_resolver,
			        axis2_env_t **env);

    /**
     * Method buildchains
     *
     */
    axis2_status_t (AXIS2_CALL *
    build_chains) (axis2_phase_resolver_t *phase_resolver,
                   axis2_env_t **env);
    
    /**
     * To build the opration for the opeartion which the module going to be added
     *
     * @param op <code>AxisOperation</code>
     */
    axis2_status_t (AXIS2_CALL *
    build_module_op) (axis2_phase_resolver_t *phase_resolver,
                                axis2_env_t **env,
                                struct axis2_op *op);
    
    
    
    
    /**
     * Method buildTranspotsChains
     *
     */
    axis2_status_t (AXIS2_CALL *
    build_transport_chains) (axis2_phase_resolver_t *phase_resolver,
                             axis2_env_t **env);
    
    
    axis2_status_t (AXIS2_CALL *
    engage_module_globally) (axis2_phase_resolver_t *phase_resolver,
                                axis2_env_t **env,
                                struct axis2_module_desc *module);
    
    /**
     * To engage modules come form global
     *
     * @param service
     * @param module
     */
    axis2_status_t (AXIS2_CALL *
    engage_module_to_svc_from_global) (axis2_phase_resolver_t *phase_resolver,
                                        axis2_env_t **env,
                                        struct axis2_svc *svc,
                                        struct axis2_module_desc *module_desc);
    
    axis2_status_t (AXIS2_CALL *
    engage_module_to_svc) (axis2_phase_resolver_t *phase_resolver,
                            axis2_env_t **env,
                            struct axis2_svc *svc,
                            struct axis2_module_desc *module_desc);
                                                        
    axis2_status_t (AXIS2_CALL *
    engage_module_to_op) (axis2_phase_resolver_t *phase_resolver,
                                    axis2_env_t **env,
                                    struct axis2_op *axis_op,
                                    struct axis2_module_desc *module_desc);    


};

/** 
 * @brief Phase Resolver struct  
 */  
AXIS2_DECLARE_DATA struct axis2_phase_resolver
{
	axis2_phase_resolver_ops_t *ops;
};

/**
 * Creates phase resolver struct
 * @return pointer to newly created phase resolver
 */
AXIS2_DECLARE(axis2_phase_resolver_t *) 
axis2_phase_resolver_create (axis2_env_t **env);

/**
 * default constructor , to obuild chains for GlobalDescription
 *
 * @param engineConfig
 */
AXIS2_DECLARE(axis2_phase_resolver_t *) 
axis2_phase_resolver_create_with_config (axis2_env_t **env, 
                                         struct axis2_conf *axis2_config);

/**
 * Constructor PhaseResolver
 *
 * @param axisConfig
 * @param serviceContext
 */
AXIS2_DECLARE(axis2_phase_resolver_t *)
axis2_phase_resolver_create_with_config_and_svc (axis2_env_t **env, 
                                                struct axis2_conf *axis2_config,
                                                struct axis2_svc *svc);

/*************************** Function macros **********************************/

#define AXIS2_PHASE_RESOLVER_FREE(phase_resolver, env) \
		((phase_resolver)->ops->free (phase_resolver, env))

#define AXIS2_PHASE_RESOLVER_BUILD_CHAINS(phase_resolver, env) \
		((phase_resolver)->ops->build_chains (phase_resolver, env))

#define AXIS2_PHASE_RESOLVER_BUILD_MODULE_OP(phase_resolver, env, op) \
		((phase_resolver)->ops->build_module_op (phase_resolver, env, op))

#define AXIS2_PHASE_RESOLVER_BUILD_TRANSPORT_CHAINS(phase_resolver, env) \
		((phase_resolver)->ops->build_transport_chains (phase_resolver, env))

#define AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_GLOBALLY(phase_resolver, env, module) \
		((phase_resolver)->ops->engage_module_globally (phase_resolver, env, module))       

#define AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_SVC_FROM_GLOBAL(phase_resolver, env, svc, module_desc) \
		((phase_resolver)->ops->engage_module_to_svc_from_global (phase_resolver, env, svc, module_desc))

#define AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_SVC(phase_resolver, env, svc, module_desc) \
		((phase_resolver)->ops->engage_module_to_svc (phase_resolver, env, svc, module_desc))

#define AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_OP(phase_resolver, env, axis_op, module_desc) \
		((phase_resolver)->ops->engage_module_to_op (phase_resolver, env, axis_op, module_desc))

        
/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_PHASE_RESOLVER_H */
