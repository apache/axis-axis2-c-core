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

#ifndef AXIS2_SVC_GRP_H
#define AXIS2_SVC_GRP_H

/** 
 * @defgroup axis2_svc_grp service group
 * @ingroup axis2_desc
 * service group represents the static structure of a service group in 
 * the Axis2 configuration. 
 * In Axis2 description hierarchy, a service group lives inside the 
 * configuration. service groups are configured in services.xml files located 
 * in the respective service group folders of the services folder in the repository. 
 * In services.xml file, services groups are declared at top level.
 * A service group can have one or more services associated with it.
 * Sometimes services.xml would not have a service group defined, but only a 
 * service. In such cases a service group with the same name as that of the 
 * service mentioned in services.xml would be created by the deployment 
 * engine and the service would be associated with that newly created service 
 * group. The deployment engine would create service group instances to represent 
 * those configured service groups in services.xml files and would store 
 * them in the configuration.
 * service group encapsulates data on engaged module information and the 
 * service associated with service group.
 * @{
 */

/**
 * @file axis2_svc_grp.h
 */

#include <axis2_param_container.h>
#include <axis2_svc.h>
#include <axis2_svc_grp_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_svc_grp_ops */
    typedef struct axis2_svc_grp_ops axis2_svc_grp_ops_t;
    /** Type name for struct axis2_svc_grp */
    typedef struct axis2_svc_grp axis2_svc_grp_t;

    struct axis2_svc;
    struct axis2_svc_grp_ctx;

    /**
     * service group ops struct.
     * Encapsulator struct for ops of axis2_svc_grp.
     */
    struct axis2_svc_grp_ops
    {
        /**
          * Frees service group.
          * @param svc_grp pointer to service group struct
          * @param env pointer to environment struct
          * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
          */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env);

        /**
         * Sets service group name.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param svc_grp_name service group name string 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_name)(
                    axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    const axis2_char_t *svc_grp_name);

        /**
         * Gets service group name.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @return service group name string 
         */
        const axis2_char_t *(AXIS2_CALL *
                get_name)(
                    const axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env) ;

        /**
         * Adds given service to service group.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param service service to be added, service group assumes ownership
         * of service
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_svc)(
                    axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    struct axis2_svc *svc) ;

        /**
         * Gets named service from service group.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param svc_qname pointer to QName of the service
         * @return pointer to service corresponding to given QName, returns a 
         * reference, not a cloned copy
         */
        struct axis2_svc *(AXIS2_CALL *
                get_svc)(
                    const axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    const axis2_qname_t *svc_qname);

        /**
         * Gets all services associated with service group. 
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @return pointer to hash table containing all services, returns a 
         * reference, not a cloned copy
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_svcs)(
                    const axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env);

        /**
         * Removes named service from service group.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param svc_name pointer to service QName
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                remove_svc)(
                    axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    const axis2_qname_t *svc_qname);

        /**
         * Adds parameter.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param param pointer to parameter, service group assumes ownership 
         * of parameter
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * Gets named parameter.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param name parameter name
         * @return pointer to named parameter if exists, else NULL. Returns a 
         * reference, not a cloned copy
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * Gets all parameters set on service group.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @return pointer to array list containing parameter, returns a 
         * reference, not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_params)(
                    const axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env);

        /**
         * Checks if the named parameter is locked.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param param_name pointer to param name
         * @return AXIS2_TRUE if the named parameter is locked, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

        /**
         * Adds given module QName to list of module QNames.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param module_name pointer to module QName
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_module_qname)(
                    axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    const axis2_qname_t *module_qname);

        /**
         * Gets parent which is of type configuration.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @return pointer to parent configuration, returns a 
         * reference, not a cloned copy
         */
        struct axis2_conf *(AXIS2_CALL *
                get_parent)(
                    const axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env);

        /**
         * Sets parent which is of type configuration.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param parent parent configuration, service group does not assume
         * the ownership of configuration
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_parent)(
                    axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    struct axis2_conf *parent);

        /**
         * Engages named module to service group. Engaging a module to service 
         * group would ensure that the same module would be engaged to all 
         * services within the group.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param module_name pointer to module QName
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                engage_module)(
                    axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    const axis2_qname_t *module_qname);

        /**
         * Gets all module QNames associated with service group.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @return pointer to array list containing all QNames, returns a 
         * reference, not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_module_qnames)(
                    const axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env);

        /**
         * Adds module reference.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param moduleref pointer to module QName
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_module_ref)(
                    axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    const axis2_qname_t *moduleref);

        /**
         * Gets all module references.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @return pointer to array list containing module reference, returns 
         * a reference, not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_module_refs)(
                    const axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env);

        /**
         * Gets service group context related to this service group.
         * @param svc_grp pointer to service group struct
         * @param env pointer to environment struct
         * @param parent pointer to configuration context which is the parent of 
         * the context hierarchy
         * @return pointer to service group context related to this service 
         * group, returns a reference, not a cloned copy
         */
        struct axis2_svc_grp_ctx *(AXIS2_CALL *
                get_svc_grp_ctx)(
                    const axis2_svc_grp_t *svc_grp,
                    const axis2_env_t *env,
                    struct axis2_conf_ctx *parent);

    };

    /**
     * service group struct.
     */
    struct axis2_svc_grp
    {
        /** operations of service group */
        axis2_svc_grp_ops_t *ops;
        /** parameter container to hold service related parameters */
        axis2_param_container_t *param_container;

    };

    /**
     * Creates a service group struct instance.
     * @param env pointer to environment struct
     * @return pointer to newly created service group
     */
    AXIS2_EXTERN axis2_svc_grp_t *AXIS2_CALL
    axis2_svc_grp_create(
        const axis2_env_t *env);

    /**
     * Creates service group with given configuration as the parent.
     * @param env pointer to environment struct
     * @param conf pointer to configuration, service group created does not 
     * assume ownership of configuration
     * @return pointer to newly created service group
     */
    AXIS2_EXTERN axis2_svc_grp_t *AXIS2_CALL
    axis2_svc_grp_create_with_conf(
        const axis2_env_t *env,
        struct axis2_conf *conf);

/** Frees service group.
    @sa axis2_svc_grp_ops#free */
#define AXIS2_SVC_GRP_FREE(svc_grp, env) ((svc_grp)->ops->free (svc_grp , env))

/** Sets name.
    @sa axis2_svc_grp_ops#set_name */
#define AXIS2_SVC_GRP_SET_NAME(svc_grp, env , svc_grp_name) \
        ((svc_grp)->ops->set_name (svc_grp, env, svc_grp_name))

/** Gets name.
    @sa axis2_svc_grp_ops#get_name */
#define AXIS2_SVC_GRP_GET_NAME(svc_grp, env) \
        ((svc_grp)->ops->get_name(svc_grp, env))

/** Adds service.
    @sa axis2_svc_grp_ops#add_svc */
#define AXIS2_SVC_GRP_ADD_SVC(svc_grp, env , svc) \
        ((svc_grp)->ops->add_svc (svc_grp, env, svc))

/** Gets named service.
    @sa axis2_svc_grp_ops#get_svc */
#define AXIS2_SVC_GRP_GET_SVC(svc_grp, env , svc_name) \
        ((svc_grp)->ops->get_svc(svc_grp, env, svc_names))

/** Gets all services.
    @sa axis2_svc_grp_ops#get_all_svcs */
#define AXIS2_SVC_GRP_GET_ALL_SVCS(svc_grp, env) \
        ((svc_grp)->ops->get_all_svcs(svc_grp, env))

/** Removes named service.
    @sa axis2_svc_grp_ops#remove_svc */
#define AXIS2_SVC_GRP_REMOVE_SVC(svc_grp, env, svc_name) \
      ((svc_grp)->ops->remove_svc(svc_grp, env, svc_name))

/** Adds parameter.
    @sa axis2_svc_grp_ops#add_param */
#define AXIS2_SVC_GRP_ADD_PARAM(svc_grp, env, param) \
      ((svc_grp)->ops->add_param(svc_grp, env, param))

/** Gets named parameter.
    @sa axis2_svc_grp_ops#get_param */
#define AXIS2_SVC_GRP_GET_PARAM(svc_grp, env, name) \
      ((svc_grp)->ops->get_param(svc_grp, env, name))

/** Gets all parameters.
    @sa axis2_svc_grp_ops#get_all_params */
#define AXIS2_SVC_GRP_GET_ALL_PARAMS(svc_grp, env) \
        ((svc_grp)->ops->get_all_params(svc_grp, env))

/** Checks if the named parameter is locked.
    @sa axis2_svc_grp_ops#is_param_locked */
#define AXIS2_SVC_GRP_IS_PARAM_LOCKED(svc_grp, env , param_name) \
        ((svc_grp)->ops->is_param_locked(svc_grp, env, param_name))

/** Adds module QName.
    @sa axis2_svc_grp_ops#add_module_qname */
#define AXIS2_SVC_GRP_ADD_MODULE_QNAME(svc_grp, env , module_qname) \
        ((svc_grp)->ops->add_module_qname(svc_grp, env, module_qname))

/** Gets parent configuration.
    @sa axis2_svc_grp_ops#get_parent */
#define AXIS2_SVC_GRP_GET_PARENT(svc_grp, env) \
        ((svc_grp)->ops->get_parent(svc_grp, env))

/** Sets parent configuration.
    @sa axis2_svc_grp_ops#set_parent */
#define AXIS2_SVC_GRP_SET_PARENT(svc_grp, env , parent) \
        ((svc_grp)->ops->set_parent(svc_grp, env, parent))

/** Engages module to service group.
    @sa axis2_svc_grp_ops#engage_module */
#define AXIS2_SVC_GRP_ENGAGE_MODULE(svc_grp, env, module_name) \
        ((svc_grp)engage_module_modulep, env, module_name))

/** Gets all module QNames.
    @sa axis2_svc_grp_ops#get_all_module_qnames */
#define AXIS2_SVC_GRP_GET_ALL_MODULE_QNAMES(svc_grp, env) \
        ((svc_grp)->ops->get_all_module_qnames(svc_grp, env))

/** Adds module reference.
    @sa axis2_svc_grp_ops#add_module_ref */
#define AXIS2_SVC_GRP_ADD_MODULE_REF(svc_grp, env, moduleref) \
        ((svc_grp)->ops->add_module_ref(svc_grp, env, moduleref))

/** Get all module references.
    @sa axis2_svc_grp_ops#get_all_module_refs */
#define AXIS2_SVC_GRP_GET_ALL_MODULE_REFS(svc_grp, env) \
        ((svc_grp)->ops->get_all_module_refs(svc_grp, env))

/** Gets service group context related to service.
    @sa axis2_svc_grp_ops#get_svc_grp_ctx */
#define AXIS2_SVC_GRP_GET_SVC_GRP_CTX(svc_grp, env, parent) \
        ((svc_grp)->ops->get_svc_grp_ctx(svc_grp, env, parent))

/** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_SVC_GRP_H  */
