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
 * @brief Axis2 Wsdl soap operation interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_array_list.h>
#include <axis2_handler_desc.h>
#include <axis2_phase.h>
#include <axis2_phase_rule.h>
#include <axis2_handler.h>
#include <wsdl.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_phase;
struct axis2_handler_desc;
struct axis2_handler;
struct axis2_phase_rule;
typedef struct axis2_phase_resolver axis2_phase_resolver_t;
typedef struct axis2_phase_resolver_ops axis2_phase_resolver_ops_t;

/** @defgroup axis2_phase_resolver Phase Resolver
 * @ingroup axis2_phase
 * @{
 */

/** 
 * @brief Wsdl Phase Resolver operations struct
 * Encapsulator struct for operations of axis2_phase_resolver
 */
AXIS2_DECLARE_DATA struct axis2_phase_resolver_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_phase_resolver_t *phase_resolver,
			        axis2_env_t **env);
    


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
 * Creates phase resolver struct
 * @param phases
 * @return pointer to newly created phase resolver
 */
AXIS2_DECLARE(axis2_phase_resolver_t *) 
axis2_phase_resolver_create_with_phases (axis2_env_t **env, axis2_array_list_t *phases);

/*************************** Function macros **********************************/

#define AXIS2_PHASE_RESOLVER_FREE(phase_resolver, env) \
		((phase_resolver->ops)->free (phase_resolver, env))

#define AXIS2_PHASE_RESOLVER_IS_PHASE_EXIST(phase_resolver, env, phase_name) \
		((phase_resolver->ops)->is_phase_exist (phase_resolver, env, phase_name))

#define AXIS2_PHASE_RESOLVER_ADD_HANDLER(phase_resolver, env, handler) \
		((phase_resolver->ops)->add_handler (phase_resolver, env, handler))

#define AXIS2_PHASE_RESOLVER_GET_PHASE(phase_resolver, env, phase_name) \
		((phase_resolver->ops)->get_phase (phase_resolver, env, phase_name))

#define AXIS2_PHASE_RESOLVER_BUILD_TRANSPORT_HANDLER_CHAIN(phase_resolver, env, phase, handlers) \
		((phase_resolver->ops)->build_transport_handler_chain (phase_resolver, env, phase, handlers))       

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_PHASE_RESOLVER_H */
