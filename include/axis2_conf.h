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
typedef struct axis2_conf_ops axis2_conf_ops_t;
typedef struct axis2_conf axis2_conf_t;
    
struct axis2_svc_grp;
struct axis2_svc;
struct axis2_op;    
    
/** 
 * @brief Engine Configuration ops struct
 * Encapsulator struct for ops of axis2_config
 */
struct axis2_conf_ops
{
	axis2_status_t (AXIS2_CALL *
    free) (axis2_conf_t *conf, 
           axis2_env_t **env);
    
	axis2_status_t (AXIS2_CALL *
    add_svc_grp) (axis2_conf_t *conf, 
                    axis2_env_t **env, 
                    struct axis2_svc_grp *svc_grp);
    
	struct axis2_svc_grp *(AXIS2_CALL *
    get_svc_grp) (  axis2_conf_t *conf, 
                    axis2_env_t **env, 
                    axis2_char_t *svc_grp_name);
 
    axis2_hash_t * (AXIS2_CALL *
    get_svc_grps) (axis2_conf_t *conf, 
                   axis2_env_t **env);
    
	axis2_status_t (AXIS2_CALL *
    add_svc) (axis2_conf_t *conf, 
                axis2_env_t **env, 
                struct axis2_svc* svc);
    
	struct axis2_svc *(AXIS2_CALL *
    get_svc) (axis2_conf_t *conf, 
                axis2_env_t **env, 
                axis2_char_t* svc_name);
    
	axis2_status_t (AXIS2_CALL *
    remove_svc) (axis2_conf_t *conf, 
                    axis2_env_t **env, 
                    const axis2_char_t *name);
    
    axis2_status_t (AXIS2_CALL *
    add_param) (axis2_conf_t *conf, 
                    axis2_env_t **env,
                    axis2_param_t *param);

	axis2_param_t *(AXIS2_CALL *
    get_param) (axis2_conf_t *conf, 
                axis2_env_t **env,
                const axis2_char_t *name);

	axis2_array_list_t *(AXIS2_CALL *
    get_params) (axis2_conf_t *conf, 
                 axis2_env_t **env);
	
	axis2_bool_t (AXIS2_CALL *
    is_param_locked) (axis2_conf_t *conf, 
                        axis2_env_t **env,
                        axis2_char_t *param_name);
                                                
    struct axis2_transport_in_desc * (AXIS2_CALL *
    get_transport_in)(axis2_conf_t *conf,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname);
    
    /**
     * Method addTransport
     *
     * @param transport
     * @throws AxisFault
     */
    axis2_status_t (AXIS2_CALL *
    add_transport_in) (axis2_conf_t *conf,
                        axis2_env_t **env,
                        struct axis2_transport_in_desc *transport);
    
    struct axis2_transport_out_desc * (AXIS2_CALL *
    get_transport_out) (axis2_conf_t *conf,
                        axis2_env_t **env,
                        axis2_qname_t *qname);
    
    /**
     * Method addTransport
     *
     * @param transport
     * @throws AxisFault
     */
    axis2_status_t (AXIS2_CALL *
    add_transport_out) (axis2_conf_t *conf,
                                            axis2_env_t **env,
                                            struct axis2_transport_out_desc *transport);
    
    axis2_hash_t * (AXIS2_CALL *
    get_transports_in) (axis2_conf_t *conf,
                                            axis2_env_t **env);
    
    axis2_hash_t * (AXIS2_CALL *
    get_transports_out) (axis2_conf_t *conf,
                                            axis2_env_t **env);
    /**
     * Method getModule
     *
     * @param name
     * @return ModuleDescription
     */
    struct axis2_module_desc *(AXIS2_CALL *
    get_module) (axis2_conf_t *conf,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname);
    
    /**
     * Method getEngadgedModules
     *
     * @return  Collection
     */
    axis2_array_list_t * (AXIS2_CALL *
    get_engaged_modules) (axis2_conf_t *conf,
                                            axis2_env_t **env);
    
    axis2_array_list_t * (AXIS2_CALL *
    get_in_phases_upto_and_including_post_dispatch) (
                                                axis2_conf_t *conf,
                                                axis2_env_t **env);
    
    axis2_array_list_t * (AXIS2_CALL *
    get_outflow) (axis2_conf_t *conf,
                                            axis2_env_t **env);
    
    
    /**
     * @return ArrayList
     */
    axis2_array_list_t * (AXIS2_CALL *
    get_in_faultflow) (axis2_conf_t *conf,
                                            axis2_env_t **env);
    
    /**
     * @return ArrayList
     */
    axis2_array_list_t * (AXIS2_CALL *
    get_out_faultflow) (axis2_conf_t *conf,
                                            axis2_env_t **env);

        axis2_hash_t *(AXIS2_CALL *
    get_faulty_svcs) (axis2_conf_t *conf,
                                        axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    get_faulty_modules) (axis2_conf_t *conf,
                                        axis2_env_t **env);
        
    /*to get all the services in the system */
    axis2_hash_t *(AXIS2_CALL *
    get_svcs) (axis2_conf_t *conf,
                                    axis2_env_t **env);
    
    axis2_bool_t (AXIS2_CALL *
    is_engaged) (axis2_conf_t *conf,
                                    axis2_env_t **env,
                                    axis2_qname_t *module_name);
    
    struct axis2_phases_info *(AXIS2_CALL *
    get_phases_info) (axis2_conf_t *conf,
                                        axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_phases_info) (axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        struct axis2_phases_info *phases_info);
    axis2_status_t (AXIS2_CALL *
    add_msg_recv) (axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_char_t *key,
                                        struct axis2_msg_recv *msg_recv);
    
    struct axis2_msg_recv *(AXIS2_CALL *
    get_msg_recv) (axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_char_t *key);
    
    axis2_status_t (AXIS2_CALL *
    set_out_phases) (axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_array_list_t *out_phases);
    
        /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_in_faultphases) (axis2_conf_t *conf,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_out_faultphases) (axis2_conf_t *conf,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list);
        
    /**
     * @return HashMap
     */
    axis2_hash_t *(AXIS2_CALL *
    get_modules) (axis2_conf_t *conf,
                                    axis2_env_t **env); 
    
    /**
     * Method addMdoule
     *
     * @param module
     */
    axis2_status_t (AXIS2_CALL *
    add_module) (axis2_conf_t *conf,
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
struct axis2_conf
{
	axis2_conf_ops_t *ops;
    axis2_param_container_t *param_container;
};

/** create Engine configuration struct
 * @return pointer to newly created engine configuration
 */
axis2_conf_t * AXIS2_CALL 
axis2_conf_create(axis2_env_t **env);

/************************* Start of function macros	***************************/
	
#define AXIS2_ENGINE_CONFIG_FREE(conf, env) \
		(conf->ops->free(conf, env))
	
#define AXIS2_ENGINE_CONFIG_ADD_SVC_GRP(conf, env, svc_grp) \
        (conf->ops->add_svc_grp (conf, env, svc_grp))

#define AXIS2_ENGINE_CONFIG_GET_SVC_GRP(conf, env, svc_grp_name) \
        (conf->ops->get_svc_grp (conf, env, svc_grp_name))

#define AXIS2_ENGINE_CONFIG_GET_SVC_GRPS(conf, env) \
        (conf->ops->get_svc_grps (conf, env))

#define AXIS2_ENGINE_CONFIG_ADD_SVC(conf, env, svc) \
		(conf->ops->get_svc (conf, env, svc))
		
#define AXIS2_ENGINE_CONFIG_GET_SVC(conf, env, svc_name) \
		(conf->ops->get_svc (conf, env, svc_name))

#define AXIS2_ENGINE_CONFIG_REMOVE_SVC(conf, env, svc_name) \
		(conf->ops->remove_svc (conf, env, svc_name))
        
#define AXIS2_ENGINE_CONFIG_ADD_PARAM(conf, env, param) \
        (conf->ops->add_param(conf , env, param))
		
#define AXIS2_ENGINE_CONFIG_GET_PARAM(conf, env, name) \
        (conf->ops->get_param(conf , env, name))
		
#define AXIS2_ENGINE_CONFIG_GET_PARAMS(conf, env) \
        (conf->ops->get_params(conf , env))
		
#define AXIS2_ENGINE_CONFIG_IS_PARAM_LOCKED(conf, env, param_name) \
        (conf->ops->is_param_locked(conf, env, param_name))

#define AXIS2_ENGINE_CONFIG_GET_TRANSPORT_IN(conf, env, qname) \
        (conf->ops->get_transport_in(conf , env, qname))

#define AXIS2_ENGINE_CONFIG_ADD_TRANSPORT_IN(conf, env, transport) \
        (conf->ops->add_transport_in(conf , env, transport))

#define AXIS2_ENGINE_CONFIG_GET_TRANSPORT_OUT(conf, env, qname) \
        (conf->ops->get_transport_out(conf , env, qname))

#define AXIS2_ENGINE_CONFIG_ADD_TRANSPORT_OUT(conf, env, transport) \
        (conf->ops->add_transport_out(conf , env, transport))
 
#define AXIS2_ENGINE_CONFIG_GET_TRANSPORTS_IN(conf, env) \
        (conf->ops->get_transports_in(conf , env)) 
 
#define AXIS2_ENGINE_CONFIG_GET_TRANSPORTS_OUT(conf, env) \
        (conf->ops->get_transports_out(conf , env)) 
 
#define AXIS2_ENGINE_CONFIG_GET_MODULE(conf, env, qname) \
        (conf->ops->get_module(conf , env, qname)) 

#define AXIS2_ENGINE_CONFIG_GET_ENGAGED_MODULES(conf, env) \
        (conf->ops->get_engaged_modules(conf , env)) 
        
#define AXIS2_ENGINE_CONFIG_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(conf, env) \
        (conf->ops->get_in_phases_upto_and_including_post_dispatch(conf , env)) 

#define AXIS2_ENGINE_CONFIG_GET_OUTFLOW(conf, env) \
        (conf->ops->get_outflow(conf , env)) 
        
#define AXIS2_ENGINE_CONFIG_GET_IN_FAULT_FLOW(conf, env) \
        (conf->ops->get_in_faultflow(conf , env)) 

#define AXIS2_ENGINE_CONFIG_GET_OUT_FAULT_FLOW(conf, env) \
        (conf->ops->get_out_faultflow(conf , env)) 


#define AXIS2_ENGINE_CONFIG_GET_FAULTY_SVCS(conf, env) \
        (conf->ops->get_faulty_svcs(conf , env)) 

#define AXIS2_ENGINE_CONFIG_GET_FAULTY_MODULES(conf, env) \
        (conf->ops->get_faulty_modules(conf , env)) 

#define AXIS2_ENGINE_CONFIG_GET_SVCS(conf, env) \
        (conf->ops->get_svcs(conf , env)) 

#define AXIS2_ENGINE_CONFIG_IS_ENGAGED(conf, env, module_name) \
        (conf->ops->is_engaged(conf , env, module_name)) 

#define AXIS2_ENGINE_CONFIG_GET_PHASESINFO(conf, env) \
        (conf->ops->get_phases_info(conf , env)) 

#define AXIS2_ENGINE_CONFIG_SET_PHASESINFO(conf, env, phases_info) \
        (conf->ops->set_phases_info(conf , env, phases_info)) 

#define AXIS2_ENGINE_CONFIG_ADD_MSG_RECV(conf, env, key, msg_recv) \
        (conf->ops->add_msg_recv(conf , env, key, msg_recv)) 

#define AXIS2_ENGINE_CONFIG_GET_MSG_RECV(conf, env, key) \
        (conf->ops->get_msg_recv(conf , env, key)) 

#define AXIS2_ENGINE_CONFIG_SET_OUTPHASES(conf, env, out_phases) \
        (conf->ops->set_out_phases(conf , env, out_phases)) 

#define AXIS2_ENGINE_CONFIG_SET_IN_FAULTPHASES(conf, env, list) \
        (conf->ops->set_in_faultphases(conf , env, list)) 

#define AXIS2_ENGINE_CONFIG_SET_OUT_FAULTPHASES(conf, env, list) \
        (conf->ops->set_out_faultphases(conf , env, list)) 

#define AXIS2_ENGINE_CONFIG_GET_MODULES(conf, env) \
        (conf->ops->get_modules(conf , env)) 

#define AXIS2_ENGINE_CONFIG_ADD_MODULE(conf, env, module) \
        (conf->ops->add_module(conf , env, module)) 
        
/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_CONFIG_H */
