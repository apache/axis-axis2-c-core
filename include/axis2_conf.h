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
 * @defgroup axis2_engine engine
 * @ingroup axis2
 * @{
 * @}
 */

/** @defgroup axis2_config configuration
 * @ingroup axis2_engine
 * Axis2 Engine Configuration
 * The palce where all the global states of Axis is accessed.
 * All the global states kept in the <code>EngineRegistry</code> and all the
 * Service states kept in the <code>MessageContext</code>. Other runtime
 * artifacts does not keep states forward from the execution.
 * @{
 */

/**
 * @file axis2_config.h
 * @brief axis2 engine configuration interface. Global states of all the Axis2
 * can be accessed here
 */

#include <axis2_param_container.h>
#include <axis2_svc_grp.h>
#include <axis2_transport_in_desc.h>
#include <axis2_transport_out_desc.h>
#include <axis2_qname.h>
#include <axis2_hash.h>
#include <axis2_phases_info.h>
#include <axis2_msg_recv.h>
#include <axis2_disp.h>
#include <axis2_disp_checker.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_conf_ops */
    typedef struct axis2_conf_ops axis2_conf_ops_t;
    /** Type name for struct axis2_conf */
    typedef struct axis2_conf axis2_conf_t;

    struct axis2_msg_recv;
    struct axis2_phases_info;
    struct axis2_svc_grp;
    struct axis2_svc;
    struct axis2_op;
    struct axis2_dep_engine;

    /**
     * Engine Configuration ops struct
     * Encapsulator struct for ops of axis2_config
     */
    struct axis2_conf_ops
    {
        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param svc_grp pointer to service group
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_svc_grp)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_svc_grp *svc_grp);

        /** 
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param svc_grp_name pointer to service group name
         */
        struct axis2_svc_grp *(AXIS2_CALL *
                get_svc_grp)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_char_t *svc_grp_name);

        /** 
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_hash_t * (AXIS2_CALL *
                get_svc_grps)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param svc pointer to service
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_svc)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_svc* svc);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param svc_name pointer to service name
         */
        struct axis2_svc *(AXIS2_CALL *
                get_svc)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_char_t* svc_name);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param name pointer to name
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                remove_svc)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param param pointer to param
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param name pointer to name
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_params)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param param_name pointer to param name
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_char_t *param_name);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param qname pointer to qname
         */
        struct axis2_transport_in_desc * (AXIS2_CALL *
                get_transport_in)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_qname_t *qname);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param transport  pointer to transport
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_transport_in)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_transport_in_desc *transport);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment strcut
         * @param qname pointer to qname
         */
        struct axis2_transport_out_desc * (AXIS2_CALL *
                get_transport_out)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_qname_t *qname);

        /**
        * @param conf pointer to conf struct
        * @param env pointer to environment struct
        * @param transport pointer to transport
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
                add_transport_out)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_transport_out_desc *transport);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_hash_t * (AXIS2_CALL *
                get_transports_in)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_hash_t * (AXIS2_CALL *
                get_transports_out)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);
        /**
        * @param conf pointer to conf struct
        * @param env pointer to environment struct
        * @param qname pointer to qname
        * @return module description
        */
        struct axis2_module_desc *(AXIS2_CALL *
                get_module)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_qname_t *qname);

        /**
        * @param conf pointer to conf struct
        * @param env pointer to environment struct
        * @param qname pointer to qname
        * @return  collection
        */
        axis2_array_list_t * (AXIS2_CALL *
                get_engaged_modules)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_array_list_t * (AXIS2_CALL *
                get_in_phases_upto_and_including_post_dispatch)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_array_list_t * (AXIS2_CALL *
                get_outflow)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);


        /**
        * @param conf pointer to conf struct
        * @param env pointer to environment struct
        * @return array list
        */
        axis2_array_list_t * (AXIS2_CALL *
                get_in_faultflow)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
        * @param conf pointer to conf struct
        * @param env pointer to environment struct
        * @return array list
        */
        axis2_array_list_t * (AXIS2_CALL *
                get_out_faultflow)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_hash_t *(AXIS2_CALL *
                get_faulty_svcs)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_hash_t *(AXIS2_CALL *
                get_faulty_modules)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /*
         * Gets all the services in the system 
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_hash_t *(AXIS2_CALL *
                get_svcs)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_name pointer to module name struct
         */
        axis2_bool_t (AXIS2_CALL *
                is_engaged)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_qname_t *module_name);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        struct axis2_phases_info *(AXIS2_CALL *
                get_phases_info)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param phases_info pointer to phases_info struct
         */
        axis2_status_t (AXIS2_CALL *
                set_phases_info)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_phases_info *phases_info);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param key pointer to key
         * @param msg_recv pointer to message receiver 
         */
        axis2_status_t (AXIS2_CALL *
                add_msg_recv)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_char_t *key,
                    struct axis2_msg_recv *msg_recv);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param key pointer to key
         */
        struct axis2_msg_recv *(AXIS2_CALL *
                get_msg_recv)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_char_t *key);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param out_phases ponter to out phases
         */
        axis2_status_t (AXIS2_CALL *
                set_out_phases)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_array_list_t *out_phases);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_array_list_t* (AXIS2_CALL *
                get_out_phases)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param list pointer to list 
         */
        axis2_status_t (AXIS2_CALL *
                set_in_faultphases)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param list pointer to list
         */
        axis2_status_t (AXIS2_CALL *
                set_out_faultphases)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_hash_t *(AXIS2_CALL *
                get_modules)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module pointer to module
         */
        axis2_status_t (AXIS2_CALL *
                add_module)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_module_desc *module);

        /**
         * Sets the default dispatching order
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                set_default_dispatchers)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Sets the custom dispatching order
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param dispatch pointer to dispatch
         */
        axis2_status_t (AXIS2_CALL *
                set_dispatch_phase)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_phase_t *dispatch);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_repos)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param axis2_repos pointer to axis2_repos struct
         */
        axis2_status_t (AXIS2_CALL *
                set_repos)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_char_t *axis2_repos);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_ref pointer to module reference
         */
        axis2_status_t (AXIS2_CALL *
                engage_module)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_qname_t *module_ref);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param dep_engine pointer to dep_engine 
         */
        axis2_status_t (AXIS2_CALL *
                set_dep_engine)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_dep_engine *dep_engine);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_name pointer to module name 
         */
        axis2_char_t * (AXIS2_CALL *
                get_default_module_version)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_char_t* module_name);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_name pointer to module name 
         */
        struct axis2_module_desc * (AXIS2_CALL *
                get_default_module)(
                    axis2_conf_t *conf, 
                    const axis2_env_t *env,
                    const axis2_char_t *module_name);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_name pointer to module name 
         * @param module_version pointer to module version
         */
        axis2_status_t (AXIS2_CALL *
                add_default_module_version)(
                    axis2_conf_t *conf, 
                    const axis2_env_t *env,
                    axis2_char_t *module_name,
                    axis2_char_t *module_version);

        /**
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_name pointer to module name
         * @param version_id pointer to version
         */
        axis2_status_t (AXIS2_CALL *
                engage_module_with_version)(
                    axis2_conf_t *conf, 
                    const axis2_env_t *env,
                    const axis2_char_t *module_name,
                    const axis2_char_t *version_id);

    };

    /**
     * engine configuration struct
     */
    struct axis2_conf
    {
        /** operations of engine configuration struct */
        axis2_conf_ops_t *ops;
        axis2_param_container_t *param_container;
    };

    /** create Engine configuration struct
     * @param env pointer to environment struct
     * @return pointer to newly created engine configuration
     */
    AXIS2_EXTERN axis2_conf_t * AXIS2_CALL
    axis2_conf_create(
        const axis2_env_t *env);


/************************* Start of function macros   ***************************/

/** Frees the conf.
    @sa axis2_conf_ops#free */
#define AXIS2_CONF_FREE(conf, env) \
      ((conf)->ops->free(conf, env))

/** Adds teh service group.
    @sa axis2_conf_ops#add_svc_grp */
#define AXIS2_CONF_ADD_SVC_GRP(conf, env, svc_grp) \
        ((conf)->ops->add_svc_grp (conf, env, svc_grp))

/** Gets the service group.
    @sa axis2_conf_ops#get_svc_grp */
#define AXIS2_CONF_GET_SVC_GRP(conf, env, svc_grp_name) \
        ((conf)->ops->get_svc_grp (conf, env, svc_grp_name))

/** Gets the service groups.
    @sa axis2_conf_ops#get_svc_grps */
#define AXIS2_CONF_GET_SVC_GRPS(conf, env) \
        ((conf)->ops->get_svc_grps (conf, env))

/** Adds the service.
    @sa axis2_conf_ops#add_svc */
#define AXIS2_CONF_ADD_SVC(conf, env, svc) \
      ((conf)->ops->add_svc (conf, env, svc))

/** Gets the service.
    @sa axis2_conf_ops#get_svc */
#define AXIS2_CONF_GET_SVC(conf, env, svc_name) \
      ((conf)->ops->get_svc (conf, env, svc_name))

/** Removes the service.
    @sa axis2_conf_ops#remove_svc */
#define AXIS2_CONF_REMOVE_SVC(conf, env, svc_name) \
      ((conf)->ops->remove_svc (conf, env, svc_name))

/** Adds the param.
    @sa axis2_conf_ops#add_param */
#define AXIS2_CONF_ADD_PARAM(conf, env, param) \
        ((conf)->ops->add_param(conf , env, param))

/** Gets the param.
    @sa axis2_conf_ops#get_param */
#define AXIS2_CONF_GET_PARAM(conf, env, name) \
        ((conf)->ops->get_param(conf , env, name))

/** Gets the params.
    @sa axis2_conf_ops#_get_params */
#define AXIS2_CONF_GET_PARAMS(conf, env) \
        ((conf)->ops->get_params(conf , env))

/** Is param locked.
    @sa axis2_conf_ops#is_param_locked */
#define AXIS2_CONF_IS_PARAM_LOCKED(conf, env, param_name) \
        ((conf)->ops->is_param_locked(conf, env, param_name))

/** Gets the transport in.
    @sa axis2_conf_ops#get_transport_in */
#define AXIS2_CONF_GET_TRANSPORT_IN(conf, env, qname) \
        ((conf)->ops->get_transport_in(conf , env, qname))

/** Adds the transport in.
    @sa axis2_conf_ops#add_transport_in */
#define AXIS2_CONF_ADD_TRANSPORT_IN(conf, env, transport) \
        ((conf)->ops->add_transport_in(conf , env, transport))

/** Gets the transport out.
    @sa axis2_conf_ops#get_transport_out */
#define AXIS2_CONF_GET_TRANSPORT_OUT(conf, env, qname) \
        ((conf)->ops->get_transport_out(conf , env, qname))

/** Adds the transport out
    @sa axis2_conf_ops#add_transport_out */
#define AXIS2_CONF_ADD_TRANSPORT_OUT(conf, env, transport) \
        ((conf)->ops->add_transport_out(conf , env, transport))

/** Gets the transports in.
    @sa axis2_conf_ops#get_transport_in */
#define AXIS2_CONF_GET_TRANSPORTS_IN(conf, env) \
        ((conf)->ops->get_transports_in(conf , env))

/** Gets the transport out.
    @sa axis2_conf_ops#get_transport_out */
#define AXIS2_CONF_GET_TRANSPORTS_OUT(conf, env) \
        ((conf)->ops->get_transports_out(conf , env))

/** Gets the module.
    @sa axis2_conf_ops#get_module */
#define AXIS2_CONF_GET_MODULE(conf, env, qname) \
        ((conf)->ops->get_module(conf , env, qname))

/** Gets the engaged modules.
    @sa axis2_conf_ops#get_engaged_modules */
#define AXIS2_CONF_GET_ENGAGED_MODULES(conf, env) \
        ((conf)->ops->get_engaged_modules(conf , env))

/** Get in phases upot and including post dispatch.
    @sa axis2_conf_ops#get_in_phases_upto_and_including_post_dispatch */
#define AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(conf, env) \
        ((conf)->ops->get_in_phases_upto_and_including_post_dispatch(conf , env))

/** Gets the outflow.
    @sa axis2_conf_ops#get_outflow */
#define AXIS2_CONF_GET_OUTFLOW(conf, env) \
        ((conf)->ops->get_outflow(conf , env))

/** Gets the in fault flow.
    @sa axis2_conf_ops#get_in_fault_flow */
#define AXIS2_CONF_GET_IN_FAULT_FLOW(conf, env) \
        ((conf)->ops->get_in_faultflow(conf , env))

/** Gets the out fault flow.
    @sa axis2_conf_ops#get_out_fault_flow */
#define AXIS2_CONF_GET_OUT_FAULT_FLOW(conf, env) \
        ((conf)->ops->get_out_faultflow(conf , env))

/** Gets the faulty services.
    @sa axis2_conf_ops#get_faulty_svcs */
#define AXIS2_CONF_GET_FAULTY_SVCS(conf, env) \
        ((conf)->ops->get_faulty_svcs(conf , env))

/** Gets the faulty modules.
    @sa axis2_conf_ops#get_faulty_modules */
#define AXIS2_CONF_GET_FAULTY_MODULES(conf, env) \
        ((conf)->ops->get_faulty_modules(conf , env))

/** Gets the services.
    @sa axis2_conf_ops#get_svcs */
#define AXIS2_CONF_GET_SVCS(conf, env) \
        ((conf)->ops->get_svcs(conf , env))

/** Is engaged.
    @sa axis2_conf_ops#is_engaged */
#define AXIS2_CONF_IS_ENGAGED(conf, env, module_name) \
        ((conf)->ops->is_engaged(conf , env, module_name))

/** Gets the phase info.
    @sa axis2_conf_ops#get_phasesinfo */
#define AXIS2_CONF_GET_PHASESINFO(conf, env) \
        ((conf)->ops->get_phases_info(conf , env))

/** Sets the phase info.
    @sa axis2_conf_ops#set_phasesinfo */
#define AXIS2_CONF_SET_PHASESINFO(conf, env, phases_info) \
        ((conf)->ops->set_phases_info(conf , env, phases_info))

/** Adds the message receiver.
    @sa axis2_conf_ops#add_msg_recv */
#define AXIS2_CONF_ADD_MSG_RECV(conf, env, key, msg_recv) \
        ((conf)->ops->add_msg_recv(conf , env, key, msg_recv))

/** Gets the message receiver.
    @sa axis2_conf_ops#get_msg_recv */
#define AXIS2_CONF_GET_MSG_RECV(conf, env, key) \
        ((conf)->ops->get_msg_recv(conf , env, key))

/** Sets the out phases.
    @sa axis2_conf_ops#set_out_phases */
#define AXIS2_CONF_SET_OUT_PHASES(conf, env, out_phases) \
        ((conf)->ops->set_out_phases(conf , env, out_phases))

/** Gets the out phases.
    @sa axis2_conf_ops#get_out_phases */
#define AXIS2_CONF_GET_OUT_PHASES(conf, env) \
        ((conf)->ops->get_out_phases(conf , env))

/** Sets in fault phases.
    @sa axis2_conf_ops#set_in_faultphases */
#define AXIS2_CONF_SET_IN_FAULTPHASES(conf, env, list) \
        ((conf)->ops->set_in_faultphases(conf , env, list))

/** Sets out fault phases.
    @sa axis2_conf_ops#set_out_faultphases */
#define AXIS2_CONF_SET_OUT_FAULTPHASES(conf, env, list) \
        ((conf)->ops->set_out_faultphases(conf , env, list))

/** Gets the modules.
    @sa axis2_conf_ops#get_modules */
#define AXIS2_CONF_GET_MODULES(conf, env) \
        ((conf)->ops->get_modules(conf , env))

/** Adds the modules.
    @sa axis2_conf_ops#add_module */
#define AXIS2_CONF_ADD_MODULE(conf, env, module) \
        ((conf)->ops->add_module(conf , env, module))

/** Sets the default dispatchers.
    @sa axis2_conf_ops#set_default_dispatchers */
#define AXIS2_CONF_SET_DEFAULT_DISPATCHERS(conf, env) \
        ((conf)->ops->set_default_dispatchers(conf , env))

/** Sets the dispatch phase.
    @sa axis2_conf_ops#set_dispatch_phase */
#define AXIS2_CONF_SET_DISPATCH_PHASE(conf, env, dispatch) \
        ((conf)->ops->set_dispatch_phase(conf , env, dispatch))

/** Gets the repos.
    @sa axis2_conf_ops#get_repos */
#define AXIS2_CONF_GET_REPOS(conf, env) \
        ((conf)->ops->get_repos(conf , env))

/** Sets the repos.
    @sa axis2_conf_ops#set_repos */
#define AXIS2_CONF_SET_REPOS(conf, env, axis2_repos) \
        ((conf)->ops->set_repos(conf , env, axis2_repos))

/** Engages the module.
    @sa axis2_conf_ops#engage_module */
#define AXIS2_CONF_ENGAGE_MODULE(conf, env, module_ref) \
        ((conf)->ops->engage_module(conf , env, module_ref))

/** Sets the dip engine.
    @sa axis2_conf_ops#set_dep_engine */
#define AXIS2_CONF_SET_DEP_ENGINE(conf, env, dep_engine) \
        ((conf)->ops->set_dep_engine(conf, env, dep_engine))

/** Gets the default module version.
    @sa axis2_conf_ops#get_default_module_version */
#define AXIS2_CONF_GET_DEFAULT_MODULE_VERSION(conf, env, module_name)\
        ((conf)->ops->get_default_module_version(conf, env, module_name))

/** Gets the default module.
    @sa axis2_conf_ops#get_default_module */
#define AXIS2_CONF_GET_DEFAULT_MODULE(conf, env, module_name)\
        ((conf)->ops->get_default_module(conf, env, module_name))

/** Adds the default module version.
    @sa axis2_conf_ops#add_default_module_version */
#define AXIS2_CONF_ADD_DEFAULT_MODULE_VERSION(conf, env, mod_name, mod_ver)\
        ((conf)->ops->add_default_module_version(conf, env, mod_name, mod_ver))

/** Engages the module with version.
    @sa axis2_conf_ops#engage_module_with_version */
#define AXIS2_CONF_ENGAGE_MODULE_WITH_VERSION(conf, env, mod_name, mod_ver)\
        ((conf)->ops->engage_module_with_version(conf, env, mod_name, mod_ver))

/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_CONFIG_H */
