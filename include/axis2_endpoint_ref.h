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

#ifndef AXIS2_ENDPOINT_REF_H
#define AXIS2_ENDPOINT_REF_H

/**
 * @defgroup axis2_endpoint_ref  endpoint reference
 * @ingroup axis2_addr
 * @{
 */

/**
 * @file axis2_endpoint_ref.h
 * @brief Axis2 core addressing interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_const.h>
#include <axis2_array_list.h>
#include <axis2_any_content_type.h>
#include <axis2_svc_name.h>
#include <axiom_node.h>
#include <axiom_attribute.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_endpoint_ref */
    typedef struct axis2_endpoint_ref axis2_endpoint_ref_t;
    /** Type name for struct axis2_endpoint_ref_ops */
    typedef struct axis2_endpoint_ref_ops axis2_endpoint_ref_ops_t;


    /**
     * Dispatcher ops struct
     */
    struct axis2_endpoint_ref_ops
    {
        /**
         * Method get_address
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_address)(
                    const axis2_endpoint_ref_t *endpoint_ref,
                    const axis2_env_t *env);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         * @param address pointer to address
         */
        axis2_status_t (AXIS2_CALL *
                set_address)(
                    struct axis2_endpoint_ref *endpoint_ref,
                    const axis2_env_t *env,
                    axis2_char_t *address);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         */
        axis2_qname_t *(AXIS2_CALL *
                get_interface_qname)(
                    const axis2_endpoint_ref_t *endpoint_ref,
                    const axis2_env_t *env);
        /**
         * Method set_interface_qname
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         * @param interface_name pointer to interface name
         */
        axis2_status_t (AXIS2_CALL *
                set_interface_qname)(
                    struct axis2_endpoint_ref *endpoint_ref,
                    const axis2_env_t *env,
                    axis2_qname_t *interface_qname);
        /**
         * Method get_ref_properties
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         */
        axis2_any_content_type_t *(AXIS2_CALL *
                get_ref_properties)(
                    const axis2_endpoint_ref_t *endpoint_ref,
                    const axis2_env_t *env);
        /**
         * Method set_ref_properties
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         * @param ref_properties ref properties
         */
        axis2_status_t (AXIS2_CALL *
                set_ref_properties)(
                    struct axis2_endpoint_ref *endpoint_ref,
                    const axis2_env_t *env,
                    axis2_any_content_type_t *ref_properties);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_ref_param_list)(
                    const axis2_endpoint_ref_t *endpoint_ref,
                    const axis2_env_t *env);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_meta_data_list)(
                    const axis2_endpoint_ref_t *endpoint_ref,
                    const axis2_env_t *env);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_ref_attribute_list)(
                    const axis2_endpoint_ref_t *endpoint_ref,
                    const axis2_env_t *env);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_meta_attribute_list)(
                    const axis2_endpoint_ref_t *endpoint_ref,
                    const axis2_env_t *env);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_extension_list)(
                    const axis2_endpoint_ref_t *endpoint_ref,
                    const axis2_env_t *env);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         * @param ref_param_node ref param node
         */
        axis2_status_t (AXIS2_CALL *
                add_ref_param)(
                    struct axis2_endpoint_ref *endpoint_ref,
                    const axis2_env_t *env,
                    axiom_node_t *ref_param_node);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         * @param meta_deta_node meta data node
         */
        axis2_status_t (AXIS2_CALL *
                add_meta_data)(
                    struct axis2_endpoint_ref *endpoint_ref,
                    const axis2_env_t *env,
                    axiom_node_t *meta_data_node);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         * @param attr attribute
         */
        axis2_status_t (AXIS2_CALL *
                add_ref_attribute)(
                    struct axis2_endpoint_ref *endpoint_ref,
                    const axis2_env_t *env,
                    axiom_attribute_t *attr);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         * @param attr attribute
         */
        axis2_status_t (AXIS2_CALL *
                add_meta_attribute)(
                    struct axis2_endpoint_ref *endpoint_ref,
                    const axis2_env_t *env,
                    axiom_attribute_t *attr);

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         * @param extension_node pointer to extension node
         */
        axis2_status_t (AXIS2_CALL *
                add_extension)(
                    struct axis2_endpoint_ref *endpoint_ref,
                    const axis2_env_t *env,
                    axiom_node_t *extension_node);

        /**
         * Method get_ref_params
         */
        /*       axis2_any_content_type_t *(AXIS2_CALL *
             get_ref_params)(struct axis2_endpoint_ref *endpoint_ref, 
                               const axis2_env_t *env);*/
        /**
         * Method set_ref_params
         */
        /*        axis2_status_t (AXIS2_CALL *
              set_ref_params)(struct axis2_endpoint_ref *endpoint_ref,
                                const axis2_env_t *env,
                                axis2_any_content_type_t *any_content_type);*/
        /**
         * Method get_svc_name
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         */
        axis2_svc_name_t *(AXIS2_CALL *
                get_svc_name)(
                    const axis2_endpoint_ref_t *endpoint_ref,
                    const axis2_env_t *env);
        /**
         * Method set_svc_name
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         * @param svc_name pointer to service name
         */
        axis2_status_t (AXIS2_CALL *
                set_svc_name)(
                    struct axis2_endpoint_ref *endpoint_ref,
                    const axis2_env_t *env,
                    axis2_svc_name_t *svc_name);


        /*        axiom_node_t *(AXIS2_CALL *
              get_policies)(struct axis2_endpoint_ref *endpoint_ref, 
                              const axis2_env_t *env);
              
                axis2_status_t (AXIS2_CALL *
              set_policies)(struct axis2_endpoint_ref *endpoint_ref, 
                              const axis2_env_t *env, 
                         axiom_node_t *policies);
        */
        /*        axis2_any_content_type_t *(AXIS2_CALL *
              get_metadata)(struct axis2_endpoint_ref *endpoint_ref, 
                              const axis2_env_t *env);
              
                axis2_status_t (AXIS2_CALL *
              set_metadata)(struct axis2_endpoint_ref *endpoint_ref, 
                              const axis2_env_t *env, 
                         axis2_any_content_type_t *metadata);
        **/
        /*
             axis2_status_t (AXIS2_CALL *
           add_ref_param)(struct axis2_endpoint_ref *endpoint_ref, 
                            const axis2_env_t *env, 
                          axis2_qname_t *qname, 
                       axis2_char_t *value);
        */

        /**
         * @param endpoint_ref pointer to endpoint ref
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_endpoint_ref *endpoint_ref,
                    const axis2_env_t *env);

    };

    /**
     * Dispatcher struct
     */
    struct axis2_endpoint_ref
    {
        /** Dispatcher related ops */
        axis2_endpoint_ref_ops_t *ops;
    };


    /**
     * creates endpoint_ref struct
     * @param env pointer to environment struct
     * @param address pointer to address
     */
    AXIS2_EXTERN axis2_endpoint_ref_t *AXIS2_CALL
    axis2_endpoint_ref_create(
        const axis2_env_t *env,
        const axis2_char_t *address);

/** Get address.
    @sa axis2_endpoint_ref_ops#get_address */
#define AXIS2_ENDPOINT_REF_GET_ADDRESS(endpoint_ref, env) \
      ((endpoint_ref)->ops->get_address(endpoint_ref, env))

/** Set address.
    @sa axis2_endpoint_ref_ops#set_address */
#define AXIS2_ENDPOINT_REF_SET_ADDRESS(endpoint_ref, env, address) \
      ((endpoint_ref)->ops->set_address(endpoint_ref, env, address))

/** Get interface name.
    @sa axis2_endpoint_ref_ops#get_interface_name */
#define AXIS2_ENDPOINT_REF_GET_INTERFACE_NAME(endpoint_ref, env) \
      ((endpoint_ref)->ops->get_interface_qname(endpoint_ref, env))

/** Set interface name.
    @sa axis2_endpoint_ref_ops#set_interface_name */
#define AXIS2_ENDPOINT_REF_SET_INTERFACE_NAME(endpoint_ref, env, interface_qname)\
      ((endpoint_ref)->ops->set_interface_qname(endpoint_ref, env, interface_qname))

/** Get reference properties.
    @sa axis2_endpoint_ref_ops#get_ref_properties */
#define AXIS2_ENDPOINT_REF_GET_REF_PROPERTIES(endpoint_ref, env) \
      ((endpoint_ref)->ops->get_ref_properties(endpoint_ref, env))

/** Set reference properties.
    @sa axis2_endpoint_ref_ops#set_ref_properties */
#define AXIS2_ENDPOINT_REF_SET_REF_PROPERTIES(endpoint_ref, env, ref_properties)\
      ((endpoint_ref)->ops->set_ref_properties(endpoint_ref, env, ref_properties))

/*#define AXIS2_ENDPOINT_REF_GET_REF_PARAMS(endpoint_ref, env) \
      ((endpoint_ref)->ops->get_ref_params(endpoint_ref, env))
          
#define AXIS2_ENDPOINT_REF_SET_REF_PARAMS(endpoint_ref, env, any_content_type) \
      ((endpoint_ref)->ops->set_ref_params(endpoint_ref, env, any_content_type))*/

/** Gets the service name.
    @sa axis2_endpoint_ref_ops#get_svc_name */
#define AXIS2_ENDPOINT_REF_GET_SVC_NAME(endpoint_ref, env) \
      ((endpoint_ref)->ops->get_svc_name(endpoint_ref, env))

/** Sets the service name.
    @sa axis2_endpoint_ref_ops#set_svc_name */
#define AXIS2_ENDPOINT_REF_SET_SVC_NAME(endpoint_ref, env, svc_name) \
      ((endpoint_ref)->ops->set_svc_name(endpoint_ref, env, svc_name))

/*#define AXIS2_ENDPOINT_REF_GET_POLICIES(endpoint_ref, env) \
      ((endpoint_ref)->ops->get_policies(endpoint_ref, env))
          
#define AXIS2_ENDPOINT_REF_SET_POLICIES(endpoint_ref, env, policies) \
      ((endpoint_ref)->ops->set_policies(endpoint_ref, env, policies))*/

/*#define AXIS2_ENDPOINT_REF_GET_METADATA(endpoint_ref, env) \
      ((endpoint_ref)->ops->get_metadata(endpoint_ref, env))
          
#define AXIS2_ENDPOINT_REF_SET_METADATA(endpoint_ref, env, metadata) \
      ((endpoint_ref)->ops->set_metadata(endpoint_ref, env, metadata))*/

/** Gets the reference parameter list.
    @sa axis2_endpoint_ref_ops#get_ref_param_list */
#define AXIS2_ENDPOINT_REF_GET_REF_PARAM_LIST(endpoint_ref, env) \
      ((endpoint_ref)->ops->get_ref_param_list(endpoint_ref, env))

/** Get the meta data list.
    @sa axis2_endpoint_ref_ops#get_meta_data_list */
#define AXIS2_ENDPOINT_REF_GET_META_DATA_LIST(endpoint_ref, env) \
      ((endpoint_ref)->ops->get_meta_data_list(endpoint_ref, env))

/** Add reference param.
    @sa axis2_endpoint_ref_ops#add_ref_param */
#define AXIS2_ENDPOINT_REF_ADD_REF_PARAM(endpoint_ref, env, node) \
      ((endpoint_ref)->ops->add_ref_param(endpoint_ref, env, node))

/** Add meta data.
    @sa axis2_endpoint_ref_ops#add_meta_data */
#define AXIS2_ENDPOINT_REF_ADD_META_DATA(endpoint_ref, env, node) \
      ((endpoint_ref)->ops->add_meta_data(endpoint_ref, env, node))

/** Frees the endpoint reference.
    @sa axis2_endpoint_ref_ops#free */
#define AXIS2_ENDPOINT_REF_FREE(endpoint_ref, env) \
      ((endpoint_ref)->ops->free(endpoint_ref, env))

/** Gets the reference attribute list.
    @sa axis2_endpoint_ref_ops#get_ref_attribute_list */
#define AXIS2_ENDPOINT_REF_GET_REF_ATTRIBUTE_LIST(endpoint_ref, env) \
        ((endpoint_ref)->ops->get_ref_attribute_list(endpoint_ref, env))

/** Gets the meta attribute list.
    @sa axis2_endpoint_ref_ops#get_meta_attribute_list */
#define AXIS2_ENDPOINT_REF_GET_META_ATTRIBUTE_LIST(endpoint_ref, env) \
        ((endpoint_ref)->ops->get_meta_attribute_list(endpoint_ref, env))

/** Get reference extension list.
    @sa axis2_endpoint_ref_ops#get_ref_extension_list */
#define AXIS2_ENDPOINT_REF_GET_REF_EXTENSION_LIST(endpoint_ref, env) \
        ((endpoint_ref)->ops->get_extension_list(endpoint_ref, env))

/** Add reference attribute.
    @sa axis2_endpoint_ref_ops#add_ref_attribute */
#define AXIS2_ENDPOINT_REF_ADD_REF_ATTRIBUTE(endpoint_ref, env, attr) \
        ((endpoint_ref)->ops->add_ref_attribute(endpoint_ref, env, attr))

/** Add meta attribute.
    @sa axis2_endpoint_ref_ops#add_meta_attribute */
#define AXIS2_ENDPOINT_REF_ADD_META_ATTRIBUTE(endpoint_ref, env, attr) \
        ((endpoint_ref)->ops->add_meta_attribute(endpoint_ref, env, attr))

/** Add extension.
    @sa axis2_endpoint_ref_ops#add_extension */
#define AXIS2_ENDPOINT_REF_ADD_EXTENSION(endpoint_ref, env, node) \
        ((endpoint_ref)->ops->add_extension(endpoint_ref, env, node))


/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_ENDPOINT_REF_H */
