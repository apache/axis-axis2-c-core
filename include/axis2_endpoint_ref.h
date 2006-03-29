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
 * @file axis2_endpoint_ref.h
 * @brief Axis2 core addressing interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2.h>
#include <axis2_array_list.h>
#include <axis2_any_content_type.h>
#include <axis2_svc_name.h>
#include <axis2_om_node.h>
#include <axis2_om_attribute.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_endpoint_ref;
    struct axis2_endpoint_ref_ops;
    
/**
 * @defgroup axis2_endpoint_ref Addressing relates to header
 * @ingroup axis2_addr
 * @{
 */

/**
 *   \brief Dispatcher ops struct
 */
 AXIS2_DECLARE_DATA typedef struct axis2_endpoint_ref_ops
    { 
        /**
         * Method get_address
         */
        axis2_char_t* (AXIS2_CALL *
		get_address)(struct axis2_endpoint_ref *endpoint_ref, 
                     axis2_env_t **env);
        /**
         * Method set_address
         *
         * @param address
         */
        axis2_status_t (AXIS2_CALL *
		set_address)(struct axis2_endpoint_ref *endpoint_ref, 
                     axis2_env_t **env,
			 		 axis2_char_t *address);
        /**
         * Method get_interface_qname
         */
        axis2_qname_t* (AXIS2_CALL *
		get_interface_qname)(struct axis2_endpoint_ref *endpoint_ref, 
                             axis2_env_t **env);
        /**
         * Method set_interface_qname
         *
         * @param interface_qname
         */
        axis2_status_t (AXIS2_CALL *
		set_interface_qname)(struct axis2_endpoint_ref *endpoint_ref, 
                             axis2_env_t **env, 
							 axis2_qname_t *interface_qname);
        /**
         * Method get_ref_properties
         */
        axis2_any_content_type_t* (AXIS2_CALL *
		get_ref_properties)(struct axis2_endpoint_ref *endpoint_ref, 
                            axis2_env_t **env);
        /**
         * Method set_ref_properties
         *
         * @param ref_properties
         */
        axis2_status_t (AXIS2_CALL *
		set_ref_properties)(struct axis2_endpoint_ref *endpoint_ref, 
                            axis2_env_t **env, 
							axis2_any_content_type_t *ref_properties);

        axis2_array_list_t* (AXIS2_CALL *
        get_ref_param_list)(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env);

        axis2_array_list_t* (AXIS2_CALL *
        get_meta_data_list)(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env);

        axis2_array_list_t* (AXIS2_CALL *
        get_ref_attribute_list)(struct axis2_endpoint_ref *endpoint_ref,
                                          axis2_env_t **env);

        axis2_array_list_t* (AXIS2_CALL *
        get_meta_attribute_list)(struct axis2_endpoint_ref *endpoint_ref,
                                          axis2_env_t **env);

        axis2_array_list_t* (AXIS2_CALL *
        get_extension_list)(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env);

        axis2_status_t (AXIS2_CALL *
        add_ref_param)(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_om_node_t* ref_param_node);

        axis2_status_t (AXIS2_CALL *
        add_meta_data)(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_om_node_t* meta_data_node);

        axis2_status_t (AXIS2_CALL *
        add_ref_attribute)(struct axis2_endpoint_ref *endpoint_ref,
                                          axis2_env_t **env,
                                          axis2_om_attribute_t* attr);

        axis2_status_t (AXIS2_CALL *
        add_meta_attribute)(struct axis2_endpoint_ref *endpoint_ref,
                                          axis2_env_t **env,
                                          axis2_om_attribute_t* attr);

        axis2_status_t (AXIS2_CALL *
        add_extension)(struct axis2_endpoint_ref *endpoint_ref,
                                          axis2_env_t **env, 
                                          axis2_om_node_t* extension_node);

        /**
         * Method get_ref_params
         */
 /*       axis2_any_content_type_t* (AXIS2_CALL *
		get_ref_params)(struct axis2_endpoint_ref *endpoint_ref, 
                        axis2_env_t **env);*/
        /**
         * Method set_ref_params
         */
/*        axis2_status_t (AXIS2_CALL *
		set_ref_params)(struct axis2_endpoint_ref *endpoint_ref,
                        axis2_env_t **env,
                        axis2_any_content_type_t* any_content_type);*/
        /**
         * Method get_svc_name
         */
        axis2_svc_name_t* (AXIS2_CALL *
		get_svc_name)(struct axis2_endpoint_ref *endpoint_ref, 
                      axis2_env_t **env);
        /**
         * Method set_svc_name
         *
         * @param svc_name
         */
        axis2_status_t (AXIS2_CALL *
		set_svc_name)(struct axis2_endpoint_ref *endpoint_ref, 
                      axis2_env_t **env, 
					  axis2_svc_name_t *svc_name);
		
		
/*        axis2_om_node_t* (AXIS2_CALL *
		get_policies)(struct axis2_endpoint_ref *endpoint_ref, 
                      axis2_env_t **env);
		
        axis2_status_t (AXIS2_CALL *
		set_policies)(struct axis2_endpoint_ref *endpoint_ref, 
                      axis2_env_t **env, 
					  axis2_om_node_t *policies);
*/		
/*        axis2_any_content_type_t* (AXIS2_CALL *
		get_metadata)(struct axis2_endpoint_ref *endpoint_ref, 
                      axis2_env_t **env);
		
        axis2_status_t (AXIS2_CALL *
		set_metadata)(struct axis2_endpoint_ref *endpoint_ref, 
                      axis2_env_t **env, 
					  axis2_any_content_type_t *metadata);
**/		
	/*	
        axis2_status_t (AXIS2_CALL *
		add_ref_param)(struct axis2_endpoint_ref *endpoint_ref, 
                       axis2_env_t **env, 
		               axis2_qname_t *qname, 
					   axis2_char_t *value);
	*/	
		
        axis2_status_t (AXIS2_CALL *
		free)(struct axis2_endpoint_ref *endpoint_ref, 
              axis2_env_t **env);
		
    } axis2_endpoint_ref_ops_t;
	
   /** 
    * \brief Dispatcher struct
    */
    typedef struct axis2_endpoint_ref
    {
        /** Dispatcher related ops */
        axis2_endpoint_ref_ops_t *ops;
    } axis2_endpoint_ref_t;


/**
 * creates endpoint_ref struct
 *
 * @param value
 * @param relationship_type
 */
AXIS2_DECLARE(axis2_endpoint_ref_t*) 
axis2_endpoint_ref_create(axis2_env_t **env, 
						  axis2_char_t *address);
    
#define AXIS2_ENDPOINT_REF_GET_ADDRESS(endpoint_ref, env) \
		((endpoint_ref)->ops->get_address(endpoint_ref, env))
	
#define AXIS2_ENDPOINT_REF_SET_ADDRESS(endpoint_ref, env, address) \
		((endpoint_ref)->ops->set_address(endpoint_ref, env, address))
	
#define AXIS2_ENDPOINT_REF_GET_INTERFACE_NAME(endpoint_ref, env) \
		((endpoint_ref)->ops->get_interface_qname(endpoint_ref, env))
	
#define AXIS2_ENDPOINT_REF_SET_INTERFACE_NAME(endpoint_ref, env, interface_qname)\
		((endpoint_ref)->ops->set_interface_qname(endpoint_ref, env, interface_qname))
		
#define AXIS2_ENDPOINT_REF_GET_REF_PROPERTIES(endpoint_ref, env) \
		((endpoint_ref)->ops->get_ref_properties(endpoint_ref, env))
		
#define AXIS2_ENDPOINT_REF_SET_REF_PROPERTIES(endpoint_ref, env, ref_properties)\
		((endpoint_ref)->ops->set_ref_properties(endpoint_ref, env, ref_properties))
		
/*#define AXIS2_ENDPOINT_REF_GET_REF_PARAMS(endpoint_ref, env) \
		((endpoint_ref)->ops->get_ref_params(endpoint_ref, env))
		
#define AXIS2_ENDPOINT_REF_SET_REF_PARAMS(endpoint_ref, env, any_content_type) \
		((endpoint_ref)->ops->set_ref_params(endpoint_ref, env, any_content_type))*/
		
#define AXIS2_ENDPOINT_REF_GET_SVC_NAME(endpoint_ref, env) \
		((endpoint_ref)->ops->get_svc_name(endpoint_ref, env))
		
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
		
#define AXIS2_ENDPOINT_REF_GET_REF_PARAM_LIST(endpoint_ref, env) \
		((endpoint_ref)->ops->get_ref_param_list(endpoint_ref, env))
		
#define AXIS2_ENDPOINT_REF_GET_META_DATA_LIST(endpoint_ref, env) \
		((endpoint_ref)->ops->get_meta_data_list(endpoint_ref, env))
		
#define AXIS2_ENDPOINT_REF_ADD_REF_PARAM(endpoint_ref, env, node) \
		((endpoint_ref)->ops->add_ref_param(endpoint_ref, env, node))
		
#define AXIS2_ENDPOINT_REF_ADD_META_DATA(endpoint_ref, env, node) \
		((endpoint_ref)->ops->add_meta_data(endpoint_ref, env, node))
		
#define AXIS2_ENDPOINT_REF_FREE(endpoint_ref, env) \
		((endpoint_ref)->ops->free(endpoint_ref, env))

#define AXIS2_ENDPOINT_REF_GET_REF_ATTRIBUTE_LIST(endpoint_ref, env) \
        ((endpoint_ref)->ops->get_ref_attribute_list(endpoint_ref, env))

#define AXIS2_ENDPOINT_REF_GET_META_ATTRIBUTE_LIST(endpoint_ref, env) \
        ((endpoint_ref)->ops->get_meta_attribute_list(endpoint_ref, env))

#define AXIS2_ENDPOINT_REF_GET_REF_EXTENSION_LIST(endpoint_ref, env) \
        ((endpoint_ref)->ops->get_extension_list(endpoint_ref, env))

#define AXIS2_ENDPOINT_REF_ADD_REF_ATTRIBUTE(endpoint_ref, env, attr) \
        ((endpoint_ref)->ops->add_ref_attribute(endpoint_ref, env, attr))

#define AXIS2_ENDPOINT_REF_ADD_META_ATTRIBUTE(endpoint_ref, env, attr) \
        ((endpoint_ref)->ops->add_meta_attribute(endpoint_ref, env, attr))

#define AXIS2_ENDPOINT_REF_ADD_EXTENSION(endpoint_ref, env, node) \
        ((endpoint_ref)->ops->add_extension(endpoint_ref, env, node))


/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_ENDPOINT_REF_H */
