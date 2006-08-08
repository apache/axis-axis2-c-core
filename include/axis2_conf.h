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

/** 
 * @defgroup axis2_config configuration
 * @ingroup axis2_engine
 * Axis2 configuration captures all configuration information. Configuration
 * information includes user preferences along with module and 
 * service information that is either statically configured using axis2.xml
 * file, service.xml files and module.xml files or dynamically using the 
 * functions defined in the ops struct related to this conf struct.
 * @{
 */

/**
 * @file axis2_config.h
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
     * configuration ops struct.
     * Encapsulator struct for ops of axis2_conf.
     */
    struct axis2_conf_ops
    {
        /**
         * Frees conf struct.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Adds a service group to the configuration.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param svc_grp pointer to service group, conf takes over the 
         * ownership of the service group
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_svc_grp)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_svc_grp *svc_grp);

        /** 
         * Gets a named service group.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param svc_grp_name name of the service group to be accessed
         * @return pointer to service group with the given name if exists, 
         * else NULL. Returns a reference, not a cloned copy
         */
        struct axis2_svc_grp *(AXIS2_CALL *
                get_svc_grp)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_char_t *svc_grp_name);

        /** 
         * Gets all service group added to conf.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return pointer to hash table containing the service groups, returns 
         * a reference, not a cloned copy
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_svc_grps)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Adds a service to configuration.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param svc pointer to service, conf takes over the ownership of the 
         * service
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_svc)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_svc* svc);

        /**
         * Gets a service with given name.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param svc_name service name string
         * @return pointer to service with the given name if exists, else NULL.
         * Returns a reference, not a cloned copy
         */
        struct axis2_svc *(AXIS2_CALL *
                get_svc)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_char_t* svc_name);

        /**
         * Removes the named service from configuration.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param name name of service to be removed
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                remove_svc)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * Adds a parameter to configuration.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param param pointer to parameter struct to be added
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * Gets a parameter with the given name.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param name name of the parameter to be accessed
         * @return pointer to parameter with the given name if exists, else NULL.
         * Returns a reference, not a cloned copy
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * Gets all the parameters added to the configuration.
         * @param conf pointer to conf struct
         * @param env pointer to environment
         * @return pointer to array list containing parameters if exists, 
         * else NULL. Returns a reference, not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_params)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Checks if the named parameter is locked.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param param_name name of the parameter
         * @return AXIS2_TRUE if parameter is locked, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

        /**
         * Gets in transport corresponding to the given transport QName.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param qname QName of transport 
         * @return pointer to transport in description if exists, 
         * else NULL. Returns a reference, not a cloned copy
         */
        axis2_transport_in_desc_t *(AXIS2_CALL *
                get_transport_in)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_qname_t *qname);

        /**
         * Adds a transport in description.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param transport  pointer to transport in description. conf assumes
         * ownership of the struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_transport_in)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_transport_in_desc_t *transport);

        /**
         * Gets out transport corresponding to the given transport QName.
         * @param conf pointer to conf struct
         * @param env pointer to environment strcut
         * @param qname pointer to transport qname
         * @return pointer to transport out description if exists, 
         * else NULL. Returns a reference, not a cloned copy
         */
        axis2_transport_out_desc_t *(AXIS2_CALL *
                get_transport_out)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_qname_t *qname);

       /**
        * Adds a transport out description.
        * @param conf pointer to conf struct
        * @param env pointer to environment struct
        * @param transport pointer to transport out description. conf assumes
        * ownership of the struct
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
                add_transport_out)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_transport_out_desc_t *transport);

        /**
         * Gets all in transports.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return hash table containing all transport in descriptions. 
         * Returns a reference, not a cloned copy
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_in_transports)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Gets all out transports.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return hash table containing all transport out descriptions. 
         * Returns a reference, not a cloned copy
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_out_transports)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

       /**
        * Gets a module with given QName.
        * @param conf pointer to conf struct
        * @param env pointer to environment struct
        * @param qname pointer to qname
        * @return module description corresponding to the given qname
        */
        struct axis2_module_desc *(AXIS2_CALL *
                get_module)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_qname_t *qname);

       /**
        * Gets the list of engaged modules.
        * @param conf pointer to conf struct
        * @param env pointer to environment struct
        * @return a pointer to the array list of engaged modules. Returns a 
        * reference, not a cloned copy
        */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_engaged_modules)( 
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Gets the in phases up to and including port dispatch phase.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return a pointer to the array list of in phases up to post dispatch
         * inclusive. Returns a reference, not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_in_phases_upto_and_including_post_dispatch)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Gets the out flow. Out flow is a list of phases invoked in the out 
         * path of execution of the engine.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return a pointer to the array list of out flow phases. Returns a 
         * reference, not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_out_flow)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);


       /**
        * Gets the in fault flow. In fault flow is a list of phases invoked in 
        * the in path of execution, if some fault happens.
        * @param conf pointer to conf struct
        * @param env pointer to environment struct
        * @return a pointer to the array list of in fault flow phases. Returns a 
        * reference, not a cloned copy
        */
        axis2_array_list_t *(AXIS2_CALL *
                get_in_fault_flow)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

       /**
        * Gets the out fault flow. Out fault flow is a list of phases invoked in 
        * the out path of execution, if some fault happens.
        * @param conf pointer to conf struct
        * @param env pointer to environment struct
        * @return a pointer to the array list of out fault flow phases. Returns a 
        * reference, not a cloned copy
        */
        axis2_array_list_t *(AXIS2_CALL *
                get_out_fault_flow)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Gets faulty services. A faulty service is a service that does not 
         * meet the service configuration criteria or a service with errors in 
         * the service dynamic link library.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return a pointer to the hash table of faulty services. Returns a 
         * reference, not a cloned copy
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_faulty_svcs)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Gets faulty modules. A faulty module is a module that does not 
         * meet the module configuration criteria or a module with errors in 
         * the service dynamic link library.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return a pointer to the hash table of faulty modules. Returns a 
         * reference, not a cloned copy
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_faulty_modules)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**        
         * Gets all the list of services loaded into configuration.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return a pointer to the hash table of services. Returns a 
         * reference, not a cloned copy
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_svcs)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Checks is the named module is engaged.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_name pointer to QName representing the module name
         * @return AXIS2_TRUE if named module is engaged, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                is_engaged)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_qname_t *module_name);

        /**
         * Gets phases information struct.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return a pointer to the struct containing phases information. 
         * Returns a reference, not a cloned copy
         */
        struct axis2_phases_info *(AXIS2_CALL *
                get_phases_info)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Sets phases information struct.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param phases_info pointer to phases_info struct. conf assumes 
         * ownership of the struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_phases_info)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_phases_info *phases_info);

        /**
         * Adds message receiver with the given key.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param key key string with which the message receive is to be added
         * @param msg_recv pointer to message receiver 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_msg_recv)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_char_t *key,
                    struct axis2_msg_recv *msg_recv);

        /**
         * Gets message receiver with the given key.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param key key string corresponding to the message receiver to 
         * be retrieved
         * @return pointer to the message receiver with the given key if it 
         * exists, else null. Returns a reference, not a cloned copy
         */
        struct axis2_msg_recv *(AXIS2_CALL *
                get_msg_recv)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_char_t *key);

        /**
         * Sets the list of out phases.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param out_phases pointer to array list of the phases. conf assumes 
         * ownership of the struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_out_phases)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_array_list_t *out_phases);

        /**
         * Gets the list of out phases.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return pointer to array list of out phases. Returns a reference, 
         * not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_out_phases)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Sets fault phases for in path.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param list pointer to array list of phases
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_in_fault_phases)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);

        /**
         * Sets fault phases for out path.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param list pointer to array list of phases
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_out_fault_phases)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);

        /**
         * Gets all modules configured,
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return a pointer to hash table containing the list of modules. 
         * Returns a reference, not a cloned copy
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_modules)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Adds a module.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module pointer to module struct to be added
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_module)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_module_desc *module);

        /**
         * Sets the default dispatchers.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_default_dispatchers)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Sets a custom dispatching phase.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param dispatch pointer to phase to be dispatched
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_dispatch_phase)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_phase_t *dispatch);

        /**
         * Gets the repository location.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @return returns repository location as a string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_repo)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env);

        /**
         * Sets the repository location.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param axis2_repo repository location as a string
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_repo)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    axis2_char_t *axis2_repo);

        /**
         * Engages the named module.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_ref pointer to the QName of the module to be engaged
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                engage_module)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_qname_t *module_ref);

        /**
         * Sets the deployment engine.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param dep_engine pointer to dep_engine struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_dep_engine)(
                    axis2_conf_t *conf,
                    const axis2_env_t *env,
                    struct axis2_dep_engine *dep_engine);

        /**
         * Gets the default module version for the named module.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_name module name string
         * @return default module version as a string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_default_module_version)(
                    const axis2_conf_t *conf,
                    const axis2_env_t *env,
                    const axis2_char_t *module_name);

        /**
         * Gets the default module reference for the named module.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_name module name string
         * @return pointer to the module description struct corresponding to
         * the given name
         */
        struct axis2_module_desc *(AXIS2_CALL *
                get_default_module)(
                    const axis2_conf_t *conf, 
                    const axis2_env_t *env,
                    const axis2_char_t *module_name);

        /**
         * Adds a default module version for the named module.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_name name of the module
         * @param module_version default version for the module
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_default_module_version)(
                    axis2_conf_t *conf, 
                    const axis2_env_t *env,
                    const axis2_char_t *module_name,
                    const axis2_char_t *module_version);

        /**
         * Engages the module with the given version.
         * @param conf pointer to conf struct
         * @param env pointer to environment struct
         * @param module_name name of the module to be engaged
         * @param version_id version of the module to be engaged
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                engage_module_with_version)(
                    axis2_conf_t *conf, 
                    const axis2_env_t *env,
                    const axis2_char_t *module_name,
                    const axis2_char_t *version_id);

    };

    /**
     * configuration struct.
     */
    struct axis2_conf
    {
        /** operations of configuration struct */
        axis2_conf_ops_t *ops;
        /** configuration parameter container */
        axis2_param_container_t *param_container;
    };

    /** 
     * Creates configuration struct.
     * @param env pointer to environment struct
     * @return pointer to newly created configuration
     */
    AXIS2_EXTERN axis2_conf_t *AXIS2_CALL
    axis2_conf_create(
        const axis2_env_t *env);


/** Frees the conf.
    @sa axis2_conf_ops#free */
#define AXIS2_CONF_FREE(conf, env) \
      ((conf)->ops->free(conf, env))

/** Adds a service group.
    @sa axis2_conf_ops#add_svc_grp */
#define AXIS2_CONF_ADD_SVC_GRP(conf, env, svc_grp) \
        ((conf)->ops->add_svc_grp (conf, env, svc_grp))

/** Gets the named service group.
    @sa axis2_conf_ops#get_svc_grp */
#define AXIS2_CONF_GET_SVC_GRP(conf, env, svc_grp_name) \
        ((conf)->ops->get_svc_grp (conf, env, svc_grp_name))

/** Gets all the service groups.
    @sa axis2_conf_ops#get_all_svc_grps */
#define AXIS2_CONF_GET_ALL_SVC_GRPS(conf, env) \
        ((conf)->ops->get_all_svc_grps (conf, env))

/** Adds a service.
    @sa axis2_conf_ops#add_svc */
#define AXIS2_CONF_ADD_SVC(conf, env, svc) \
      ((conf)->ops->add_svc (conf, env, svc))

/** Gets the named service.
    @sa axis2_conf_ops#get_svc */
#define AXIS2_CONF_GET_SVC(conf, env, svc_name) \
      ((conf)->ops->get_svc (conf, env, svc_name))

/** Removes the named service.
    @sa axis2_conf_ops#remove_svc */
#define AXIS2_CONF_REMOVE_SVC(conf, env, svc_name) \
      ((conf)->ops->remove_svc (conf, env, svc_name))

/** Adds a parameter.
    @sa axis2_conf_ops#add_param */
#define AXIS2_CONF_ADD_PARAM(conf, env, param) \
        ((conf)->ops->add_param(conf , env, param))

/** Gets the named parameter.
    @sa axis2_conf_ops#get_param */
#define AXIS2_CONF_GET_PARAM(conf, env, name) \
        ((conf)->ops->get_param(conf , env, name))

/** Gets all the parameters.
    @sa axis2_conf_ops#get_all_params */
#define AXIS2_CONF_GET_ALL_PARAMS(conf, env) \
        ((conf)->ops->get_all_params(conf , env))

/** Checks is the named parameter is locked.
    @sa axis2_conf_ops#is_param_locked */
#define AXIS2_CONF_IS_PARAM_LOCKED(conf, env, param_name) \
        ((conf)->ops->is_param_locked(conf, env, param_name))

/** Gets transport in description with given QName.
    @sa axis2_conf_ops#get_transport_in */
#define AXIS2_CONF_GET_TRANSPORT_IN(conf, env, qname) \
        ((conf)->ops->get_transport_in(conf , env, qname))

/** Adds a transport in.
    @sa axis2_conf_ops#add_transport_in */
#define AXIS2_CONF_ADD_TRANSPORT_IN(conf, env, transport) \
        ((conf)->ops->add_transport_in(conf , env, transport))

/** Gets transport out description with given QName.
    @sa axis2_conf_ops#get_transport_out */
#define AXIS2_CONF_GET_TRANSPORT_OUT(conf, env, qname) \
        ((conf)->ops->get_transport_out(conf , env, qname))

/** Adds a transport out.
    @sa axis2_conf_ops#add_transport_out */
#define AXIS2_CONF_ADD_TRANSPORT_OUT(conf, env, transport) \
        ((conf)->ops->add_transport_out(conf , env, transport))

/** Gets all the in transports.
    @sa axis2_conf_ops#get_all_in_transports */
#define AXIS2_CONF_GET_ALL_IN_TRANSPORTS(conf, env) \
        ((conf)->ops->get_all_in_transports(conf , env))

/** Gets all the out transport.
    @sa axis2_conf_ops#get_all_out_transports */
#define AXIS2_CONF_GET_ALL_OUT_TRANSPORTS(conf, env) \
        ((conf)->ops->get_all_out_transports(conf , env))

/** Gets the named module.
    @sa axis2_conf_ops#get_module */
#define AXIS2_CONF_GET_MODULE(conf, env, qname) \
        ((conf)->ops->get_module(conf , env, qname))

/** Gets all the engaged modules.
    @sa axis2_conf_ops#get_all_engaged_modules */
#define AXIS2_CONF_GET_ALL_ENGAGED_MODULES(conf, env) \
        ((conf)->ops->get_all_engaged_modules(conf , env))

/** Get in phases up to and including post dispatch.
    @sa axis2_conf_ops#get_in_phases_upto_and_including_post_dispatch */
#define AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(conf, env) \
        ((conf)->ops->get_in_phases_upto_and_including_post_dispatch(conf , env))

/** Gets out flow.
    @sa axis2_conf_ops#get_out_flow */
#define AXIS2_CONF_GET_OUT_FLOW(conf, env) \
        ((conf)->ops->get_out_flow(conf , env))

/** Gets in fault flow.
    @sa axis2_conf_ops#get_in_fault_flow */
#define AXIS2_CONF_GET_IN_FAULT_FLOW(conf, env) \
        ((conf)->ops->get_in_fault_flow(conf , env))

/** Gets out fault flow.
    @sa axis2_conf_ops#get_out_fault_flow */
#define AXIS2_CONF_GET_OUT_FAULT_FLOW(conf, env) \
        ((conf)->ops->get_out_fault_flow(conf , env))

/** Gets all faulty services.
    @sa axis2_conf_ops#get_all_faulty_svcs */
#define AXIS2_CONF_GET_ALL_FAULTY_SVCS(conf, env) \
        ((conf)->ops->get_all_faulty_svcs(conf , env))

/** Gets all faulty modules.
    @sa axis2_conf_ops#get_all_faulty_modules */
#define AXIS2_CONF_GET_ALL_FAULTY_MODULES(conf, env) \
        ((conf)->ops->get_all_faulty_modules(conf , env))

/** Gets all services.
    @sa axis2_conf_ops#get_all_svcs */
#define AXIS2_CONF_GET_ALL_SVCS(conf, env) \
        ((conf)->ops->get_all_svcs(conf , env))

/** Checks if the named module is engaged.
    @sa axis2_conf_ops#is_engaged */
#define AXIS2_CONF_IS_ENGAGED(conf, env, module_name) \
        ((conf)->ops->is_engaged(conf , env, module_name))

/** Gets phase information.
    @sa axis2_conf_ops#get_phases_info */
#define AXIS2_CONF_GET_PHASES_INFO(conf, env) \
        ((conf)->ops->get_phases_info(conf , env))

/** Sets phase information.
    @sa axis2_conf_ops#set_phases_info */
#define AXIS2_CONF_SET_PHASES_INFO(conf, env, phases_info) \
        ((conf)->ops->set_phases_info(conf , env, phases_info))

/** Adds a message receiver with the given name.
    @sa axis2_conf_ops#add_msg_recv */
#define AXIS2_CONF_ADD_MSG_RECV(conf, env, key, msg_recv) \
        ((conf)->ops->add_msg_recv(conf , env, key, msg_recv))

/** Gets the message receiver with given key.
    @sa axis2_conf_ops#get_msg_recv */
#define AXIS2_CONF_GET_MSG_RECV(conf, env, key) \
        ((conf)->ops->get_msg_recv(conf , env, key))

/** Sets out phases.
    @sa axis2_conf_ops#set_out_phases */
#define AXIS2_CONF_SET_OUT_PHASES(conf, env, out_phases) \
        ((conf)->ops->set_out_phases(conf , env, out_phases))

/** Gets out phases.
    @sa axis2_conf_ops#get_out_phases */
#define AXIS2_CONF_GET_OUT_PHASES(conf, env) \
        ((conf)->ops->get_out_phases(conf , env))

/** Sets in fault phases.
    @sa axis2_conf_ops#set_in_fault_phases */
#define AXIS2_CONF_SET_IN_FAULT_PHASES(conf, env, list) \
        ((conf)->ops->set_in_fault_phases(conf , env, list))

/** Sets out fault phases.
    @sa axis2_conf_ops#set_out_fault_phases */
#define AXIS2_CONF_SET_OUT_FAULT_PHASES(conf, env, list) \
        ((conf)->ops->set_out_fault_phases(conf , env, list))

/** Gets all the modules.
    @sa axis2_conf_ops#get_all_modules */
#define AXIS2_CONF_GET_ALL_MODULES(conf, env) \
        ((conf)->ops->get_all_modules(conf , env))

/** Adds a module.
    @sa axis2_conf_ops#add_module */
#define AXIS2_CONF_ADD_MODULE(conf, env, module) \
        ((conf)->ops->add_module(conf , env, module))

/** Sets the default dispatchers.
    @sa axis2_conf_ops#set_default_dispatchers */
#define AXIS2_CONF_SET_DEFAULT_DISPATCHERS(conf, env) \
        ((conf)->ops->set_default_dispatchers(conf , env))

/** Sets a custom dispatch phase.
    @sa axis2_conf_ops#set_dispatch_phase */
#define AXIS2_CONF_SET_DISPATCH_PHASE(conf, env, dispatch) \
        ((conf)->ops->set_dispatch_phase(conf , env, dispatch))

/** Gets repository location.
    @sa axis2_conf_ops#get_repo */
#define AXIS2_CONF_GET_REPO(conf, env) \
        ((conf)->ops->get_repo(conf , env))

/** Sets repository location.
    @sa axis2_conf_ops#set_repo */
#define AXIS2_CONF_SET_REPO(conf, env, axis2_repos) \
        ((conf)->ops->set_repo(conf , env, axis2_repos))

/** Engages a module.
    @sa axis2_conf_ops#engage_module */
#define AXIS2_CONF_ENGAGE_MODULE(conf, env, module_ref) \
        ((conf)->ops->engage_module(conf , env, module_ref))

/** Sets the deployment engine.
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

/** Adds the default module version for a named module.
    @sa axis2_conf_ops#add_default_module_version */
#define AXIS2_CONF_ADD_DEFAULT_MODULE_VERSION(conf, env, mod_name, mod_ver)\
        ((conf)->ops->add_default_module_version(conf, env, mod_name, mod_ver))

/** Engages a module with a given version.
    @sa axis2_conf_ops#engage_module_with_version */
#define AXIS2_CONF_ENGAGE_MODULE_WITH_VERSION(conf, env, mod_name, mod_ver)\
        ((conf)->ops->engage_module_with_version(conf, env, mod_name, mod_ver))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_CONFIG_H */
