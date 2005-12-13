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
 
#ifndef AXIS2_CONFIG_H
#define AXIS2_CONFIG_H

/**
 * @file axis2_config.h
 * @brief axis2 engine configuration interface. Global states of all the Axis2
 *        can be accessed here
 */

#include <axis2_core.h>
#include <axis2_param_container.h>
#include <axis2_svc_grp.h>
#include <axis2_transport_in_desc.h>
#include <axis2_transport_out_desc.h>
#include <axis2_qname.h>
#include <axis2_hash.h>
#include <axis2_phases_info.h>
#include <axis2_msg_recv.h>

#ifdef __cplusplus
extern "C" 
{
#endif
/** @defgroup axis2_config Axis Configuration
  * @ingroup axis2_core_engine
  * @{
  */

struct axis2_msg_recv;
struct axis2_phases_info;    
struct axis2_transport_in_desc *transports_in;
struct axis2_transport_out_desc *transports_out;
typedef struct axis2_engine_config_ops axis2_engine_config_ops_t;
typedef struct axis2_engine_config axis2_engine_config_t;
    
struct axis2_svc_grp;
struct axis2_svc;
struct axis2_op;    
    
/** 
 * @brief Engine Configuration ops struct
 * Encapsulator struct for ops of axis2_config
 */
struct axis2_engine_config_ops
{
	axis2_status_t (AXIS2_CALL *
    free) (axis2_engine_config_t *engine_config, 
           axis2_env_t **env);
    
	axis2_status_t (AXIS2_CALL *
    add_svc_grp) (axis2_engine_config_t *engine_config, 
                    axis2_env_t **env, 
                    struct axis2_svc_grp *svc_grp);
    
	struct axis2_svc_grp *(AXIS2_CALL *
    get_svc_grp) (  axis2_engine_config_t *engine_config, 
                    axis2_env_t **env, 
                    axis2_char_t *svc_grp_name);
 
    axis2_hash_t * (AXIS2_CALL *
    get_svc_grps) (axis2_engine_config_t *engine_config, 
                   axis2_env_t **env);
    
	axis2_status_t (AXIS2_CALL *
    add_svc) (axis2_engine_config_t *engine_config, 
                axis2_env_t **env, 
                struct axis2_svc* svc);
    
	struct axis2_svc *(AXIS2_CALL *
    get_svc) (axis2_engine_config_t *engine_config, 
                axis2_env_t **env, 
                axis2_char_t* svc_name);
    
	axis2_status_t (AXIS2_CALL *
    remove_svc) (axis2_engine_config_t *engine_config, 
                    axis2_env_t **env, 
                    const axis2_char_t *name);
    
    axis2_status_t (AXIS2_CALL *
    add_param) (axis2_engine_config_t *engine_config, 
                    axis2_env_t **env,
                    axis2_param_t *param);

	axis2_param_t *(AXIS2_CALL *
    get_param) (axis2_engine_config_t *engine_config, 
                axis2_env_t **env,
                const axis2_char_t *name);

	axis2_array_list_t *(AXIS2_CALL *
    get_params) (axis2_engine_config_t *engine_config, 
                 axis2_env_t **env);
	
	axis2_bool_t (AXIS2_CALL *
    is_param_locked) (axis2_engine_config_t *engine_config, 
                        axis2_env_t **env,
                        axis2_char_t *param_name);
                                                
    struct axis2_transport_in_desc * (AXIS2_CALL *
    get_transport_in)(axis2_engine_config_t *engine_config,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname);
    
    /**
     * Method addTransport
     *
     * @param transport
     * @throws AxisFault
     */
    axis2_status_t (AXIS2_CALL *
    add_transport_in) (axis2_engine_config_t *engine_config,
                        axis2_env_t **env,
                        struct axis2_transport_in_desc *transport);
    
    struct axis2_transport_out_desc * (AXIS2_CALL *
    get_transport_out) (axis2_engine_config_t *engine_config,
                        axis2_env_t **env,
                        axis2_qname_t *qname);
    
    /**
     * Method addTransport
     *
     * @param transport
     * @throws AxisFault
     */
    axis2_status_t (AXIS2_CALL *
    add_transport_out) (axis2_engine_config_t *engine_config,
                                            axis2_env_t **env,
                                            struct axis2_transport_out_desc *transport);
    
    axis2_hash_t * (AXIS2_CALL *
    get_transports_in) (axis2_engine_config_t *engine_config,
                                            axis2_env_t **env);
    
    axis2_hash_t * (AXIS2_CALL *
    get_transports_out) (axis2_engine_config_t *engine_config,
                                            axis2_env_t **env);
    /**
     * Method getModule
     *
     * @param name
     * @return ModuleDescription
     */
    struct axis2_module_desc *(AXIS2_CALL *
    get_module) (axis2_engine_config_t *engine_config,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname);
    
    /**
     * Method getEngadgedModules
     *
     * @return  Collection
     */
    axis2_array_list_t * (AXIS2_CALL *
    get_engaged_modules) (axis2_engine_config_t *engine_config,
                                            axis2_env_t **env);
    
    axis2_array_list_t * (AXIS2_CALL *
    get_inphases_upto_and_including_post_dispatch) (
                                                axis2_engine_config_t *engine_config,
                                                axis2_env_t **env);
    
    axis2_array_list_t * (AXIS2_CALL *
    get_outflow) (axis2_engine_config_t *engine_config,
                                            axis2_env_t **env);
    
    
    /**
     * @return ArrayList
     */
    axis2_array_list_t * (AXIS2_CALL *
    get_in_faultflow) (axis2_engine_config_t *engine_config,
                                            axis2_env_t **env);
    
    /**
     * @return ArrayList
     */
    axis2_array_list_t * (AXIS2_CALL *
    get_out_faultflow) (axis2_engine_config_t *engine_config,
                                            axis2_env_t **env);

        axis2_hash_t *(AXIS2_CALL *
    get_faulty_svcs) (axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    get_faulty_modules) (axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);
        
    /*to get all the services in the system */
    axis2_hash_t *(AXIS2_CALL *
    get_svcs) (axis2_engine_config_t *engine_config,
                                    axis2_env_t **env);
    
    axis2_bool_t (AXIS2_CALL *
    is_engaged) (axis2_engine_config_t *engine_config,
                                    axis2_env_t **env,
                                    axis2_qname_t *module_name);
    
    struct axis2_phases_info *(AXIS2_CALL *
    get_phases_info) (axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_phases_info) (axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        struct axis2_phases_info *phases_info);
    axis2_status_t (AXIS2_CALL *
    add_msg_recv) (axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_char_t *key,
                                        struct axis2_msg_recv *msg_recv);
    
    struct axis2_msg_recv *(AXIS2_CALL *
    get_msg_recv) (axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_char_t *key);
    
    axis2_status_t (AXIS2_CALL *
    set_outphases) (axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_array_list_t *outphases);
    
        /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_in_faultphases) (axis2_engine_config_t *engine_config,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_out_faultphases) (axis2_engine_config_t *engine_config,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list);
        
    /**
     * @return HashMap
     */
    axis2_hash_t *(AXIS2_CALL *
    get_modules) (axis2_engine_config_t *engine_config,
                                    axis2_env_t **env); 
    
    /**
     * Method addMdoule
     *
     * @param module
     */
    axis2_status_t (AXIS2_CALL *
    add_module) (axis2_engine_config_t *engine_config,
                                    axis2_env_t **env,
                                    struct axis2_module_desc *module);
                                                
};

/**
 * @brief Engine configuration struct
 * Axis2 Engine Configuration
 * The palce where all the global states of Axis is accessed.
 * All the global states kept in the <code>EngineRegistry</code> and all the
 * Service states kept in the <code>MessageContext</code>. Other runtime
 * artifacts does not keep states forward from the execution.
 */
struct axis2_engine_config
{
	axis2_engine_config_ops_t *ops;
    axis2_param_container_t *param_container;
};

/** create Engine configuration struct
 * @return pointer to newly created engine configuration
 */
axis2_engine_config_t * AXIS2_CALL 
axis2_engine_config_create(axis2_env_t **env);

/************************* Start of function macros	***************************/
	
#define AXIS2_ENGINE_CONFIG_FREE(engine_config, env) \
		(engine_config->ops->free(engine_config, env))
	
#define AXIS2_ENGINE_CONFIG_ADD_SVC_GRP(engine_config, env, svc_grp) \
        (engine_config->ops->add_svc_grp (engine_config, env, svc_grp))

#define AXIS2_ENGINE_CONFIG_GET_SVC_GRP(engine_config, env, svc_grp_name) \
        (engine_config->ops->get_svc_grp (engine_config, env, svc_grp_name))

#define AXIS2_ENGINE_CONFIG_GET_SVC_GRPS(engine_config, env) \
        (engine_config->ops->get_svc_grps (engine_config, env))

#define AXIS2_ENGINE_CONFIG_ADD_SVC(engine_config, env, svc) \
		(engine_config->ops->get_svc (engine_config, env, svc))
		
#define AXIS2_ENGINE_CONFIG_GET_SVC(engine_config, env, svc_name) \
		(engine_config->ops->get_svc (engine_config, env, svc_name))

#define AXIS2_ENGINE_CONFIG_REMOVE_SVC(engine_config, env, svc_name) \
		(engine_config->ops->remove_svc (engine_config, env, svc_name))
        
#define AXIS2_ENGINE_CONFIG_ADD_PARAM(engine_config, env, param) \
        (engine_config->ops->add_param(engine_config , env, param))
		
#define AXIS2_ENGINE_CONFIG_GET_PARAM(engine_config, env, name) \
        (engine_config->ops->get_param(engine_config , env, name))
		
#define AXIS2_ENGINE_CONFIG_GET_PARAMS(engine_config, env) \
        (engine_config->ops->get_params(engine_config , env))
		
#define AXIS2_ENGINE_CONFIG_IS_PARAM_LOCKED(engine_config, env, param_name) \
        (engine_config->ops->is_param_locked(engine_config, env, param_name))

#define AXIS2_ENGINE_CONFIG_GET_TRANSPORT_IN(engine_config, env, qname) \
        (engine_config->ops->get_transport_in(engine_config , env, qname))

#define AXIS2_ENGINE_CONFIG_ADD_TRANSPORT_IN(engine_config, env, transport) \
        (engine_config->ops->add_transport_in(engine_config , env, transport))

#define AXIS2_ENGINE_CONFIG_GET_TRANSPORT_OUT(engine_config, env, qname) \
        (engine_config->ops->get_transport_out(engine_config , env, qname))

#define AXIS2_ENGINE_CONFIG_ADD_TRANSPORT_OUT(engine_config, env, transport) \
        (engine_config->ops->add_transport_out(engine_config , env, transport))
 
#define AXIS2_ENGINE_CONFIG_GET_TRANSPORTS_IN(engine_config, env) \
        (engine_config->ops->get_transports_in(engine_config , env)) 
 
#define AXIS2_ENGINE_CONFIG_GET_TRANSPORTS_OUT(engine_config, env) \
        (engine_config->ops->get_transports_out(engine_config , env)) 
 
#define AXIS2_ENGINE_CONFIG_GET_MODULE(engine_config, env, qname) \
        (engine_config->ops->get_module(engine_config , env, qname)) 

#define AXIS2_ENGINE_CONFIG_GET_ENGAGED_MODULES(engine_config, env) \
        (engine_config->ops->get_engaged_modules(engine_config , env)) 
        
#define AXIS2_ENGINE_CONFIG_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(engine_config, env) \
        (engine_config->ops->get_inphases_upto_and_including_post_dispatch(engine_config , env)) 

#define AXIS2_ENGINE_CONFIG_GET_OUTFLOW(engine_config, env) \
        (engine_config->ops->get_outflow(engine_config , env)) 
        
#define AXIS2_ENGINE_CONFIG_GET_IN_FAULT_FLOW(engine_config, env) \
        (engine_config->ops->get_in_faultflow(engine_config , env)) 

#define AXIS2_ENGINE_CONFIG_GET_OUT_FAULT_FLOW(engine_config, env) \
        (engine_config->ops->get_out_faultflow(engine_config , env)) 


#define AXIS2_ENGINE_CONFIG_GET_FAULTY_SVCS(engine_config, env) \
        (engine_config->ops->get_faulty_svcs(engine_config , env)) 

#define AXIS2_ENGINE_CONFIG_GET_FAULTY_MODULES(engine_config, env) \
        (engine_config->ops->get_faulty_modules(engine_config , env)) 

#define AXIS2_ENGINE_CONFIG_GET_SVCS(engine_config, env) \
        (engine_config->ops->get_svcs(engine_config , env)) 

#define AXIS2_ENGINE_CONFIG_IS_ENGAGED(engine_config, env, module_name) \
        (engine_config->ops->is_engaged(engine_config , env, module_name)) 

#define AXIS2_ENGINE_CONFIG_GET_PHASESINFO(engine_config, env) \
        (engine_config->ops->get_phases_info(engine_config , env)) 

#define AXIS2_ENGINE_CONFIG_SET_PHASESINFO(engine_config, env, phases_info) \
        (engine_config->ops->set_phases_info(engine_config , env, phases_info)) 

#define AXIS2_ENGINE_CONFIG_ADD_MSG_RECV(engine_config, env, key, msg_recv) \
        (engine_config->ops->add_msg_recv(engine_config , env, key, msg_recv)) 

#define AXIS2_ENGINE_CONFIG_GET_MSG_RECV(engine_config, env, key) \
        (engine_config->ops->get_msg_recv(engine_config , env, key)) 

#define AXIS2_ENGINE_CONFIG_SET_OUTPHASES(engine_config, env, outphases) \
        (engine_config->ops->set_outphases(engine_config , env, outphases)) 

#define AXIS2_ENGINE_CONFIG_SET_IN_FAULTPHASES(engine_config, env, list) \
        (engine_config->ops->set_in_faultphases(engine_config , env, list)) 

#define AXIS2_ENGINE_CONFIG_SET_OUT_FAULTPHASES(engine_config, env, list) \
        (engine_config->ops->set_out_faultphases(engine_config , env, list)) 

#define AXIS2_ENGINE_CONFIG_GET_MODULES(engine_config, env) \
        (engine_config->ops->get_modules(engine_config , env)) 

#define AXIS2_ENGINE_CONFIG_ADD_MODULE(engine_config, env, module) \
        (engine_config->ops->add_module(engine_config , env, module)) 
        
/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_CONFIG_H */
