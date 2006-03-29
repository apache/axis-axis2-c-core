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
 
 /** <wsa:RelatesTo RelationshipType="..."?>xs:anyURI</wsa:RelatesTo> */

#include <axis2_endpoint_ref.h>
#include <axis2_string.h>
#include <axis2_array_list.h>

typedef struct axis2_endpoint_ref_impl
{
    axis2_endpoint_ref_t endpoint_ref;    
    /** endpoint address */
    axis2_char_t *address;
    /** interface qname */
    axis2_qname_t *interface_qname;
    /** reference properties */
    /*axis2_any_content_type_t *ref_properties;*/
    /** reference parameters */
    /*axis2_any_content_type_t *ref_params;*/
    /** reference parameters */
    axis2_array_list_t *ref_param_list;
    /** meta data */
    axis2_array_list_t *meta_data_list;
    /** reference parameter attribute list */
    axis2_array_list_t *ref_attribute_list;
    /** meta data attribute list */
    axis2_array_list_t *meta_attribute_list;
    /** extensible element list */
    axis2_array_list_t *extension_list;
    /** service name */
    axis2_svc_name_t *svc_name;
    /** policies represented as an OM node */
    /*axis2_om_node_t *policies;*/
    /** meta data */
    /*axis2_any_content_type_t *metadata;*/
} axis2_endpoint_ref_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(endpoint_ref) ((axis2_endpoint_ref_impl_t *)endpoint_ref)



axis2_char_t* AXIS2_CALL 
axis2_endpoint_ref_get_address(struct axis2_endpoint_ref *endpoint_ref, 
                               axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_address(struct axis2_endpoint_ref *endpoint_ref, 
                               axis2_env_t **env, 
                               axis2_char_t *address);
                               
axis2_qname_t* AXIS2_CALL 
axis2_endpoint_ref_get_interface_qname(struct axis2_endpoint_ref *endpoint_ref, 
                                       axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_interface_qname(struct axis2_endpoint_ref *endpoint_ref, 
                                       axis2_env_t **env, 
                                       axis2_qname_t *interface_qname);
                                       
/*axis2_any_content_type_t* AXIS2_CALL 
axis2_endpoint_ref_get_ref_properties(struct axis2_endpoint_ref *endpoint_ref, 
                                      axis2_env_t **env);
                                      
axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_ref_properties(struct axis2_endpoint_ref *endpoint_ref, 
                                      axis2_env_t **env, 
                                      axis2_any_content_type_t *ref_properties);
*/

axis2_array_list_t* AXIS2_CALL
axis2_endpoint_ref_get_ref_param_list(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env);

axis2_array_list_t* AXIS2_CALL
axis2_endpoint_ref_get_meta_data_list(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env);

                                      
axis2_array_list_t* AXIS2_CALL
axis2_endpoint_ref_get_ref_attribute_list(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env);

axis2_array_list_t* AXIS2_CALL
axis2_endpoint_ref_get_meta_attribute_list(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env);

axis2_array_list_t* AXIS2_CALL
axis2_endpoint_ref_get_extension_list(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env);

/*axis2_any_content_type_t* AXIS2_CALL 
axis2_endpoint_ref_get_ref_params(struct axis2_endpoint_ref *endpoint_ref, 
                                  axis2_env_t **env);
                                  
axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_ref_params(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_any_content_type_t* any_content_type);
*/                                  
axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_add_ref_param(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_om_node_t* ref_param_node);

axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_add_meta_data(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_om_node_t* meta_data_node);

axis2_status_t AXIS2_CALL
axis2_endpoint_ref_add_ref_attribute(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_om_attribute_t* attr);

axis2_status_t AXIS2_CALL
axis2_endpoint_ref_add_meta_attribute(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_om_attribute_t* attr);

axis2_status_t AXIS2_CALL
axis2_endpoint_ref_add_extension(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env, 
                                  axis2_om_node_t* extension_node);

axis2_svc_name_t* AXIS2_CALL 
axis2_endpoint_ref_get_svc_name(struct axis2_endpoint_ref *endpoint_ref, 
                                axis2_env_t **env);
                                
axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_svc_name(struct axis2_endpoint_ref *endpoint_ref, 
                                axis2_env_t **env, 
                                axis2_svc_name_t *svc_name);
                                
/*axis2_om_node_t* AXIS2_CALL 
axis2_endpoint_ref_get_policies(struct axis2_endpoint_ref *endpoint_ref, 
                                axis2_env_t **env);
                                
axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_policies(struct axis2_endpoint_ref *endpoint_ref, 
                                axis2_env_t **env, 
                                axis2_om_node_t *policies);
*/                                
/*axis2_any_content_type_t* AXIS2_CALL 
axis2_endpoint_ref_get_metadata(struct axis2_endpoint_ref *endpoint_ref, 
                                axis2_env_t **env);
                                
axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_metadata(struct axis2_endpoint_ref *endpoint_ref, 
                                axis2_env_t **env, 
                                axis2_any_content_type_t *metadata);
*/                                
/*axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_add_ref_param(struct axis2_endpoint_ref *endpoint_ref, 
                                 axis2_env_t **env, 
                                 axis2_qname_t *qname, 
                                 axis2_char_t *value);
*/

axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_free (struct axis2_endpoint_ref *endpoint_ref, 
                         axis2_env_t **env);



axis2_endpoint_ref_t* AXIS2_CALL 
axis2_endpoint_ref_create(axis2_env_t **env, 
                          axis2_char_t *address) 
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    endpoint_ref_impl = AXIS2_MALLOC( (*env)->allocator, 
                            sizeof(axis2_endpoint_ref_impl_t) );
    if (!endpoint_ref_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    endpoint_ref_impl->endpoint_ref.ops = NULL;
    endpoint_ref_impl->address = NULL;
    endpoint_ref_impl->interface_qname = NULL;
    /*endpoint_ref_impl->ref_properties = NULL;*/
    /*endpoint_ref_impl->ref_params = NULL;*/
    endpoint_ref_impl->ref_param_list = NULL;
    endpoint_ref_impl->meta_data_list = NULL;
    endpoint_ref_impl->ref_attribute_list = NULL;
    endpoint_ref_impl->meta_attribute_list = NULL;
    endpoint_ref_impl->extension_list = NULL;
    endpoint_ref_impl->svc_name = NULL;
    /*endpoint_ref_impl->policies = NULL;*/
    /*endpoint_ref_impl->metadata = NULL;*/
    
    if (address)
    {
        endpoint_ref_impl->address = AXIS2_STRDUP(address, env);
        if (!(endpoint_ref_impl->address))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            axis2_endpoint_ref_free(&(endpoint_ref_impl->endpoint_ref), env);
            return NULL;        
        }
    }
    
    endpoint_ref_impl->ref_param_list = axis2_array_list_create(env, 0);
    if (!(endpoint_ref_impl->ref_param_list))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_endpoint_ref_free(&(endpoint_ref_impl->endpoint_ref), env);
        return NULL;        
    }
    
    endpoint_ref_impl->meta_data_list = axis2_array_list_create(env, 0);
    if (!(endpoint_ref_impl->meta_data_list))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_endpoint_ref_free(&(endpoint_ref_impl->endpoint_ref), env);
        return NULL;        
    }

    endpoint_ref_impl->ref_attribute_list = axis2_array_list_create(env, 0);
    if (!(endpoint_ref_impl->ref_attribute_list))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_endpoint_ref_free(&(endpoint_ref_impl->endpoint_ref), env);
        return NULL;        
    }

    endpoint_ref_impl->meta_attribute_list = axis2_array_list_create(env, 0);
    if (!(endpoint_ref_impl->meta_attribute_list))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_endpoint_ref_free(&(endpoint_ref_impl->endpoint_ref), env);
        return NULL;        
    }
    
    endpoint_ref_impl->extension_list = axis2_array_list_create(env, 0);
    if (!(endpoint_ref_impl->extension_list))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_endpoint_ref_free(&(endpoint_ref_impl->endpoint_ref), env);
        return NULL;        
    }

    /* initialize ops */
    endpoint_ref_impl->endpoint_ref.ops  = 
            AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_endpoint_ref_ops_t) );
    if (!endpoint_ref_impl->endpoint_ref.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_endpoint_ref_free(&(endpoint_ref_impl->endpoint_ref), env);
        return NULL;        
    }

    endpoint_ref_impl->endpoint_ref.ops->get_address = 
        axis2_endpoint_ref_get_address;
        
    endpoint_ref_impl->endpoint_ref.ops->set_address = 
        axis2_endpoint_ref_set_address;
        
    endpoint_ref_impl->endpoint_ref.ops->get_interface_qname = 
        axis2_endpoint_ref_get_interface_qname;
        
    endpoint_ref_impl->endpoint_ref.ops->set_interface_qname = 
        axis2_endpoint_ref_set_interface_qname;
    
    endpoint_ref_impl->endpoint_ref.ops->get_ref_attribute_list =
        axis2_endpoint_ref_get_ref_attribute_list;

    endpoint_ref_impl->endpoint_ref.ops->get_meta_attribute_list =
        axis2_endpoint_ref_get_meta_attribute_list;

    endpoint_ref_impl->endpoint_ref.ops->get_extension_list = 
        axis2_endpoint_ref_get_extension_list;

    endpoint_ref_impl->endpoint_ref.ops->add_ref_attribute = 
        axis2_endpoint_ref_add_ref_attribute;

    endpoint_ref_impl->endpoint_ref.ops->add_meta_attribute = 
        axis2_endpoint_ref_add_meta_attribute;

    endpoint_ref_impl->endpoint_ref.ops->add_extension = 
        axis2_endpoint_ref_add_extension;

/*    endpoint_ref_impl->endpoint_ref.ops->get_ref_properties = 
        axis2_endpoint_ref_get_ref_properties;
        
    endpoint_ref_impl->endpoint_ref.ops->set_ref_properties = 
        axis2_endpoint_ref_set_ref_properties;
*/        
/*    endpoint_ref_impl->endpoint_ref.ops->get_ref_params = 
        axis2_endpoint_ref_get_ref_params;
        
    endpoint_ref_impl->endpoint_ref.ops->set_ref_params = 
        axis2_endpoint_ref_set_ref_params;
*/        
    endpoint_ref_impl->endpoint_ref.ops->get_svc_name = 
        axis2_endpoint_ref_get_svc_name;
        
    endpoint_ref_impl->endpoint_ref.ops->set_svc_name = 
        axis2_endpoint_ref_set_svc_name;
        
/*    endpoint_ref_impl->endpoint_ref.ops->get_policies = 
        axis2_endpoint_ref_get_policies;
        
    endpoint_ref_impl->endpoint_ref.ops->set_policies = 
        axis2_endpoint_ref_set_policies;
*/        
/*    endpoint_ref_impl->endpoint_ref.ops->get_metadata = 
        axis2_endpoint_ref_get_metadata;
        
    endpoint_ref_impl->endpoint_ref.ops->set_metadata = 
        axis2_endpoint_ref_set_metadata;*/
    
    endpoint_ref_impl->endpoint_ref.ops->get_ref_param_list =
    axis2_endpoint_ref_get_ref_param_list;
    
    endpoint_ref_impl->endpoint_ref.ops->get_meta_data_list =
    axis2_endpoint_ref_get_meta_data_list;
    
    endpoint_ref_impl->endpoint_ref.ops->add_ref_param = 
        axis2_endpoint_ref_add_ref_param;
        
    endpoint_ref_impl->endpoint_ref.ops->add_meta_data = 
        axis2_endpoint_ref_add_meta_data;
        
    endpoint_ref_impl->endpoint_ref.ops->free = 
        axis2_endpoint_ref_free;
        

    return &(endpoint_ref_impl->endpoint_ref);
}


/**
 * Method get_address
 */
axis2_char_t* AXIS2_CALL 
axis2_endpoint_ref_get_address(struct axis2_endpoint_ref *endpoint_ref, 
                               axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(endpoint_ref)->address;
}

/**
 * Method set_address
 *
 * @param address
 */
axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_address(struct axis2_endpoint_ref *endpoint_ref, 
                               axis2_env_t **env, 
                               axis2_char_t *address) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(NULL != AXIS2_INTF_TO_IMPL(endpoint_ref)->address)
    {
        AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(endpoint_ref)->address);
        AXIS2_INTF_TO_IMPL(endpoint_ref)->address = NULL;
    }
    
    AXIS2_INTF_TO_IMPL(endpoint_ref)->address = AXIS2_STRDUP(address, env);
    return AXIS2_SUCCESS;
}

/**
 * Method get_interface_qname
 */
axis2_qname_t* AXIS2_CALL 
axis2_endpoint_ref_get_interface_qname(struct axis2_endpoint_ref *endpoint_ref, 
                                       axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(endpoint_ref)->interface_qname;
}

/**
 * Method set_interface_qname
 *
 * @param interface_qname
 */
axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_interface_qname(struct axis2_endpoint_ref *endpoint_ref, 
                                       axis2_env_t **env, 
                                       axis2_qname_t *interface_qname) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(endpoint_ref)->interface_qname = interface_qname;
    return AXIS2_SUCCESS;
}

/**
 * Method get_ref_properties
 */
/*axis2_any_content_type_t* AXIS2_CALL 
axis2_endpoint_ref_get_ref_properties(struct axis2_endpoint_ref *endpoint_ref, 
                                      axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(endpoint_ref)->ref_properties;
}*/

/**
 * Method set_ref_properties
 *
 * @param ref_properties
 */
/*axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_ref_properties(struct axis2_endpoint_ref *endpoint_ref, 
                                      axis2_env_t **env, 
                                      axis2_any_content_type_t *ref_properties) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(endpoint_ref)->ref_properties = ref_properties;
    return AXIS2_SUCCESS;    
}*/

/**
 * Method get_ref_params
 */
/*axis2_any_content_type_t* AXIS2_CALL 
axis2_endpoint_ref_get_ref_params(struct axis2_endpoint_ref *endpoint_ref, 
                                  axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(endpoint_ref)->ref_params;
}

axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_ref_params(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_any_content_type_t* any_content_type)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(endpoint_ref)->ref_params = any_content_type;
    return AXIS2_SUCCESS;
}*/

/**
 * Method get_svc_name
 */
axis2_svc_name_t* AXIS2_CALL 
axis2_endpoint_ref_get_svc_name(struct axis2_endpoint_ref *endpoint_ref, 
                                axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(endpoint_ref)->svc_name;
}

/**
 * Method set_svc_name
 *
 * @param svc_name
 */
axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_svc_name(struct axis2_endpoint_ref *endpoint_ref, 
                                axis2_env_t **env, 
                                axis2_svc_name_t *svc_name) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(endpoint_ref)->svc_name = svc_name;
    return AXIS2_SUCCESS;
}

/*axis2_om_node_t* AXIS2_CALL 
axis2_endpoint_ref_get_policies(struct axis2_endpoint_ref *endpoint_ref, 
                                axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(endpoint_ref)->policies;
}

axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_policies(struct axis2_endpoint_ref *endpoint_ref, 
                                axis2_env_t **env, 
                                axis2_om_node_t *policies) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(endpoint_ref)->policies = policies;
    return AXIS2_SUCCESS;
}*/

/*axis2_any_content_type_t* AXIS2_CALL 
axis2_endpoint_ref_get_metadata(struct axis2_endpoint_ref *endpoint_ref, 
                                axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(endpoint_ref)->metadata;
}

axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_set_metadata(struct axis2_endpoint_ref *endpoint_ref, 
                               axis2_env_t **env, 
                               axis2_any_content_type_t *metadata) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(endpoint_ref)->metadata = metadata;
    return AXIS2_SUCCESS;
}*/

/*axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_add_ref_param(struct axis2_endpoint_ref *endpoint_ref, 
                                 axis2_env_t **env, 
                                 axis2_qname_t *qname, 
                                 axis2_char_t *value) 
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);
    
    if (!(endpoint_ref_impl->ref_params))
    {
        endpoint_ref_impl->ref_params = axis2_any_content_type_create(env);
        if (!(endpoint_ref_impl->ref_params ))
            return AXIS2_FAILURE;
    }
    
    return AXIS2_ANY_CONTENT_TYPE_ADD_VALUE(endpoint_ref_impl->ref_params, env, qname, value);
}*/

axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_free (struct axis2_endpoint_ref *endpoint_ref, 
                         axis2_env_t **env)
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);
    
    if (endpoint_ref_impl->endpoint_ref.ops)
    {
        AXIS2_FREE((*env)->allocator, endpoint_ref_impl->endpoint_ref.ops);
        endpoint_ref_impl->endpoint_ref.ops = NULL;
    }
    
    if (endpoint_ref_impl->address)
    {
        AXIS2_FREE((*env)->allocator, endpoint_ref_impl->address);
        endpoint_ref_impl->address = NULL;
    }    
    
    /*if (endpoint_ref_impl->ref_params)
    {
        AXIS2_ANY_CONTENT_TYPE_FREE(endpoint_ref_impl->ref_params, env);
        endpoint_ref_impl->ref_params = NULL;
    }*/

    if (endpoint_ref_impl->ref_param_list)
    {
        AXIS2_ARRAY_LIST_FREE(endpoint_ref_impl->ref_param_list, env);
        endpoint_ref_impl->ref_param_list = NULL;
    }

    if (endpoint_ref_impl->meta_data_list)
    {
        AXIS2_ARRAY_LIST_FREE(endpoint_ref_impl->meta_data_list, env);
        endpoint_ref_impl->meta_data_list = NULL;
    }

    if (endpoint_ref_impl->ref_attribute_list)
    {
        AXIS2_ARRAY_LIST_FREE(endpoint_ref_impl->ref_attribute_list, env);
        endpoint_ref_impl->ref_attribute_list = NULL;
    }

    if (endpoint_ref_impl->meta_attribute_list)
    {
        AXIS2_ARRAY_LIST_FREE(endpoint_ref_impl->meta_attribute_list, env);
        endpoint_ref_impl->meta_attribute_list = NULL;
    }

    if (endpoint_ref_impl->extension_list)
    {
        AXIS2_ARRAY_LIST_FREE(endpoint_ref_impl->extension_list, env);
        endpoint_ref_impl->extension_list = NULL;
    }
            

    AXIS2_FREE((*env)->allocator, endpoint_ref_impl);
    endpoint_ref_impl = NULL;
    
    return AXIS2_SUCCESS;
}

axis2_array_list_t* AXIS2_CALL
axis2_endpoint_ref_get_ref_param_list(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env)
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);

    return endpoint_ref_impl->ref_param_list;
}

axis2_array_list_t* AXIS2_CALL
axis2_endpoint_ref_get_meta_data_list(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env)
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);

    return endpoint_ref_impl->meta_data_list;
}


axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_add_ref_param(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_om_node_t* ref_param_node)
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);

    if (endpoint_ref_impl->ref_param_list && ref_param_node)
    {
        return AXIS2_ARRAY_LIST_ADD(endpoint_ref_impl->ref_param_list, env, ref_param_node);
    }

    return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL 
axis2_endpoint_ref_add_meta_data(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_om_node_t* meta_data_node)
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);

    if (endpoint_ref_impl->meta_data_list && meta_data_node)
    {
        return AXIS2_ARRAY_LIST_ADD(endpoint_ref_impl->meta_data_list, env, meta_data_node);
    }

    return AXIS2_FAILURE;
}


axis2_array_list_t* AXIS2_CALL
axis2_endpoint_ref_get_ref_attribute_list(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env)
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);

    return endpoint_ref_impl->ref_attribute_list;
}

axis2_array_list_t* AXIS2_CALL
axis2_endpoint_ref_get_meta_attribute_list(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env)
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);

    return endpoint_ref_impl->meta_attribute_list;
}

axis2_array_list_t* AXIS2_CALL
axis2_endpoint_ref_get_extension_list(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env)
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);

    return endpoint_ref_impl->extension_list;
}

axis2_status_t AXIS2_CALL
axis2_endpoint_ref_add_ref_attribute(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_om_attribute_t* attr)
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);

    if (endpoint_ref_impl->ref_attribute_list && attr)
    {
        return AXIS2_ARRAY_LIST_ADD(endpoint_ref_impl->ref_attribute_list, env, attr);
    }

    return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL
axis2_endpoint_ref_add_meta_attribute(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env,
                                  axis2_om_attribute_t* attr)
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);

    if (endpoint_ref_impl->meta_attribute_list && attr)
    {
        return AXIS2_ARRAY_LIST_ADD(endpoint_ref_impl->meta_attribute_list, env, attr);
    }

    return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL
axis2_endpoint_ref_add_extension(struct axis2_endpoint_ref *endpoint_ref,
                                  axis2_env_t **env, 
                                  axis2_om_node_t* extension_node)
{
    axis2_endpoint_ref_impl_t *endpoint_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    endpoint_ref_impl = AXIS2_INTF_TO_IMPL(endpoint_ref);

    if (endpoint_ref_impl->extension_list && extension_node)
    {
        return AXIS2_ARRAY_LIST_ADD(endpoint_ref_impl->extension_list, env, extension_node);
    }

    return AXIS2_FAILURE;
}
