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
 
#include <axis2_svc.h>
#include <axis2_addr.h>
#include <axis2_property.h>

typedef struct axis2_svc_impl axis2_svc_impl_t;
	
/** 
 * @brief Service struct impl
 *	Axis2 Service impl  
 */ 
struct axis2_svc_impl
{
	axis2_svc_t svc;
	axis2_svc_grp_t *parent;
	axis2_hash_t *wasaction_op_map;  
    axis2_char_t *axis_svc_name; 
    /** to keep the time that last update time of the service */
    long last_update;
    axis2_char_t *filename;
    /* axis2_wsdl_svc_t *service_impl; */
    /** to store module ref at deploy time parsing */
    axis2_array_list_t *module_list;    

};

#define AXIS2_INTF_TO_IMPL(svc) ((axis2_svc_impl_t *)svc)

/*************************** Function headers ********************************/

axis2_status_t AXIS2_CALL 
axis2_svc_free (axis2_svc_t *svc, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_add_op (axis2_svc_t *svc, axis2_env_t **env
		,                   axis2_op_t *op);

axis2_op_t * AXIS2_CALL
axis2_svc_get_op_with_qname (axis2_svc_t *svc, axis2_env_t **env,
		                            axis2_qname_t *op_name);
		
axis2_op_t * AXIS2_CALL
axis2_svc_get_op_with_name (axis2_svc_t *svc, axis2_env_t **env,
		                            const axis2_char_t* op_name);

axis2_hash_t * AXIS2_CALL
axis2_svc_get_ops (axis2_svc_t *svc, axis2_env_t **env);
		
axis2_status_t AXIS2_CALL
axis2_svc_set_parent (axis2_svc_t *svc, axis2_env_t **env, 
                        axis2_svc_grp_t *svc_grp);

axis2_svc_grp_t * AXIS2_CALL
axis2_svc_get_parent (axis2_svc_t *svc, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_qname (axis2_svc_t *svc, 
                    axis2_env_t **env,
                    axis2_qname_t *qname);

axis2_qname_t * AXIS2_CALL
axis2_svc_get_qname (const axis2_svc_t *svc, 
                    axis2_env_t **env);	

axis2_status_t AXIS2_CALL
axis2_svc_add_param (axis2_svc_t *svc, 
                        axis2_env_t **env, 
                        axis2_param_t *param);

axis2_param_t * AXIS2_CALL
axis2_svc_get_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                const axis2_char_t *name);

axis2_array_list_t * AXIS2_CALL
axis2_svc_get_params (axis2_svc_t *svc, 
                        axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked (axis2_svc_t *svc, 
                            axis2_env_t **env,
		                    axis2_char_t *param_name);

axis2_status_t AXIS2_CALL
axis2_svc_set_svc_interface(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_wsdl_interface_t *svc_interface);

axis2_wsdl_interface_t * AXIS2_CALL
axis2_svc_get_svc_interface(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_engage_module(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_module_desc_t * moduleref,
                            axis2_conf_t * conf);

axis2_status_t AXIS2_CALL
axis2_svc_add_module_ops(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_module_desc_t * module,
                            axis2_conf_t * conf);
                                
axis2_status_t AXIS2_CALL
axis2_svc_add_to_engaged_module_list(axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        axis2_module_desc_t *module_name);
                                    
axis2_array_list_t * AXIS2_CALL
axis2_svc_get_engaged_modules(axis2_svc_t *svc,
                                axis2_env_t **env);


void *AXIS2_CALL
axis2_svc_get_wsdl_op(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_qname_t *op_name);

axis2_status_t AXIS2_CALL
axis2_svc_set_context_path(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *context_path);

axis2_char_t * AXIS2_CALL
axis2_svc_get_context_path(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_style(axis2_svc_t *svc,
                        axis2_env_t **env,
                        axis2_char_t * style);

axis2_char_t * AXIS2_CALL
axis2_svc_get_style(axis2_svc_t *svc,
                    axis2_env_t **env);

axis2_flow_t * AXIS2_CALL
axis2_svc_get_inflow(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_inflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_flow_t *inflow);

axis2_flow_t * AXIS2_CALL
axis2_svc_get_outflow(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_outflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_flow_t *outflow);

axis2_flow_t *AXIS2_CALL
axis2_svc_get_fault_inflow(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_fault_inflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_flow_t *fault_flow);

axis2_flow_t * AXIS2_CALL
axis2_svc_get_fault_outflow(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_fault_outflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_flow_t *fault_flow);

axis2_op_t * AXIS2_CALL
axis2_svc_get_op_by_soap_action(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *soap_action);


axis2_op_t * AXIS2_CALL
axis2_svc_get_op_by_soap_action_and_endpoint(axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        axis2_char_t *soap_action,
                                        axis2_qname_t * endpoint);       

axis2_char_t * AXIS2_CALL
axis2_svc_get_axis2_svc_name(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_axis2_svc_name(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *axis_svc_name);

axis2_status_t AXIS2_CALL
axis2_svc_set_last_update(axis2_svc_t *svc,
                            axis2_env_t **env);

long AXIS2_CALL
axis2_svc_get_last_update(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_char_t * AXIS2_CALL
axis2_svc_get_filename(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_filename(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *filename);

axis2_hash_t * AXIS2_CALL
axis2_svc_get_endpoints(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_endpoints(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_hash_t * endpoints);

axis2_status_t AXIS2_CALL
axis2_svc_set_endpoint(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_wsdl_endpoint_t * endpoint);

axis2_wsdl_endpoint_t * AXIS2_CALL
axis2_svc_get_endpoint(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_qname_t * qname);

axis2_char_t * AXIS2_CALL
axis2_svc_get_namespace(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_add_mapping(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t * mapping_key , 
                            axis2_op_t * op_desc);
 
axis2_status_t AXIS2_CALL
axis2_svc_add_module_ref(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_qname_t *moduleref);

axis2_array_list_t *AXIS2_CALL
axis2_svc_get_modules(axis2_svc_t *svc,
                        axis2_env_t **env);

/************************* End of function headers ***************************/

axis2_svc_t * AXIS2_CALL
axis2_svc_create (axis2_env_t **env)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_array_list_t *array_list_l = NULL;
    axis2_param_container_t *param_container_l = NULL;
    axis2_wsdl_interface_t *wsdl_interface_l = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_property_t *property = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
	svc_impl = (axis2_svc_impl_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_svc_impl_t));
    if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    svc_impl->parent = NULL;
    svc_impl->axis_svc_name = NULL;
    svc_impl->filename = NULL;
    svc_impl->last_update = 0;
    svc_impl->svc.param_container = NULL;
    svc_impl->svc.flow_container = NULL;
    svc_impl->svc.wsdl_svc = NULL;
    svc_impl->wasaction_op_map = NULL;
    svc_impl->module_list = NULL;
    
    svc_impl->svc.param_container = axis2_param_container_create(env);		
	if(NULL == svc_impl->svc.param_container)
	{
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;		
	}
    
    svc_impl->svc.flow_container = axis2_flow_container_create(env);		
	if(NULL == svc_impl->svc.flow_container)
	{
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);		
        return NULL;
	}
    
    svc_impl->svc.wsdl_svc = axis2_wsdl_svc_create(env);		
	if(NULL == svc_impl->svc.wsdl_svc)
	{
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);		
        return NULL;
	}
    
    svc_impl->wasaction_op_map = axis2_hash_make (env);				
	if(NULL == svc_impl->wasaction_op_map)
	{
        axis2_svc_free(&(svc_impl->svc), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);		
        return NULL;
	}
    
    /** Create modle list of default size */
    svc_impl->module_list = axis2_array_list_create(env, 20);
    if(NULL == svc_impl->module_list)
    {
        axis2_svc_free(&(svc_impl->svc), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    svc_impl->svc.ops = AXIS2_MALLOC((*env)->allocator, sizeof(axis2_svc_ops_t));
	if(NULL == svc_impl->svc.ops)
	{
        axis2_svc_free(&(svc_impl->svc), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}

    wsdl_interface_l = axis2_wsdl_interface_create(env);
    if(NULL == wsdl_interface_l)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    status = axis2_svc_set_svc_interface(&(svc_impl->svc), env, wsdl_interface_l); 
    if(AXIS2_FAILURE == status)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        return NULL;
    }
    
    param_container_l = axis2_param_container_create(env);
    if(NULL == param_container_l)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, 
        axis2_param_container_free_void_arg);
    AXIS2_PROPERTY_SET_VALUE(property, env, param_container_l);
    status = AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc_impl->svc.wsdl_svc->
        wsdl_component, env, AXIS2_PARAMETER_KEY, property);
    if(AXIS2_SUCCESS != status)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        return NULL;
    }
    array_list_l = axis2_array_list_create(env, 0);
    if(NULL == array_list_l)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, 
        axis2_array_list_free_void_arg);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_SESSION);
    AXIS2_PROPERTY_SET_VALUE(property, env, array_list_l);

    status = AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc_impl->svc.wsdl_svc->
        wsdl_component, env, AXIS2_MODULEREF_KEY, property);
    if(AXIS2_SUCCESS != status)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        return NULL;
    }
    
	svc_impl->svc.ops->free = axis2_svc_free;
	svc_impl->svc.ops->add_op = axis2_svc_add_op;
	svc_impl->svc.ops->get_op_with_qname = axis2_svc_get_op_with_qname;
	svc_impl->svc.ops->get_op_with_name = axis2_svc_get_op_with_name;
	svc_impl->svc.ops->get_ops = axis2_svc_get_ops;
	svc_impl->svc.ops->set_parent = axis2_svc_set_parent;
	svc_impl->svc.ops->get_parent = axis2_svc_get_parent;
    svc_impl->svc.ops->set_qname = axis2_svc_set_qname;
	svc_impl->svc.ops->get_qname = axis2_svc_get_qname;
	svc_impl->svc.ops->add_param = axis2_svc_add_param;
	svc_impl->svc.ops->get_param = axis2_svc_get_param;
	svc_impl->svc.ops->get_params = axis2_svc_get_params;
    svc_impl->svc.ops->is_param_locked = axis2_svc_is_param_locked;
    svc_impl->svc.ops->set_svc_interface = axis2_svc_set_svc_interface;
    svc_impl->svc.ops->get_svc_interface = axis2_svc_get_svc_interface;
    svc_impl->svc.ops->engage_module = axis2_svc_engage_module;
    svc_impl->svc.ops->add_module_ops = axis2_svc_add_module_ops;
    svc_impl->svc.ops->add_to_engaged_module_list = 
            axis2_svc_add_to_engaged_module_list;
    svc_impl->svc.ops->get_engaged_modules = axis2_svc_get_engaged_modules;
    svc_impl->svc.ops->get_wsdl_op = axis2_svc_get_wsdl_op;
    svc_impl->svc.ops->set_context_path = axis2_svc_set_context_path;
    svc_impl->svc.ops->get_context_path = axis2_svc_get_context_path;
    svc_impl->svc.ops->set_style = axis2_svc_set_style;
    svc_impl->svc.ops->get_style = axis2_svc_get_style;
    svc_impl->svc.ops->get_inflow = axis2_svc_get_inflow;
    svc_impl->svc.ops->set_inflow = axis2_svc_set_inflow;
    svc_impl->svc.ops->get_outflow = axis2_svc_get_outflow;
    svc_impl->svc.ops->set_outflow = axis2_svc_set_outflow;
    svc_impl->svc.ops->get_fault_inflow = axis2_svc_get_fault_inflow;
    svc_impl->svc.ops->set_fault_inflow = axis2_svc_set_fault_inflow;
    svc_impl->svc.ops->get_fault_outflow = axis2_svc_get_fault_outflow;
    svc_impl->svc.ops->set_fault_outflow = axis2_svc_set_fault_outflow;
    svc_impl->svc.ops->get_op_by_soap_action = axis2_svc_get_op_by_soap_action;
    svc_impl->svc.ops->get_op_by_soap_action_and_endpoint = 
            axis2_svc_get_op_by_soap_action_and_endpoint;
    svc_impl->svc.ops->get_axis2_svc_name = axis2_svc_get_axis2_svc_name;
    svc_impl->svc.ops->set_axis2_svc_name = axis2_svc_set_axis2_svc_name;
    svc_impl->svc.ops->set_last_update = axis2_svc_set_last_update;
    svc_impl->svc.ops->get_last_update = axis2_svc_get_last_update;
    svc_impl->svc.ops->get_filename = axis2_svc_get_filename;
    svc_impl->svc.ops->set_filename = axis2_svc_set_filename;
    svc_impl->svc.ops->get_endpoints = axis2_svc_get_endpoints;
    svc_impl->svc.ops->set_endpoints = axis2_svc_set_endpoints;
    svc_impl->svc.ops->set_endpoint = axis2_svc_set_endpoint;
    svc_impl->svc.ops->get_endpoint = axis2_svc_get_endpoint;
    svc_impl->svc.ops->get_namespace = axis2_svc_get_namespace;
    svc_impl->svc.ops->add_mapping = axis2_svc_add_mapping;
    svc_impl->svc.ops->add_module_ref = axis2_svc_add_module_ref;
    svc_impl->svc.ops->get_modules = axis2_svc_get_modules;
    
	return &(svc_impl->svc);	
}

axis2_svc_t * AXIS2_CALL
axis2_svc_create_with_qname (axis2_env_t **env, 
                                axis2_qname_t *qname)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    
	svc_impl = AXIS2_INTF_TO_IMPL(axis2_svc_create(env));
	if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
	status = axis2_svc_set_qname(&(svc_impl->svc), env, qname);
    if(AXIS2_FAILURE == status)
    {
        axis2_svc_free(&(svc_impl->svc), env);
        return NULL;
    }
	
	return &(svc_impl->svc);
}

AXIS2_DECLARE(axis2_svc_t *)
axis2_svc_create_with_wsdl_svc (axis2_env_t **env, 
                                axis2_wsdl_svc_t *wsdl_svc)
{
    axis2_svc_impl_t *svc_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_svc, NULL);
    
	 svc_impl = AXIS2_INTF_TO_IMPL(axis2_svc_create(env));
    
	if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    if(NULL != svc_impl->svc.wsdl_svc)
    {
        AXIS2_WSDL_SVC_FREE(svc_impl->svc.wsdl_svc, env);
    }
    
    svc_impl->svc.wsdl_svc = wsdl_svc;
       
	return &(svc_impl->svc);	
}

/********************** Start of function implementations ********************/

axis2_status_t AXIS2_CALL
axis2_svc_free (axis2_svc_t *svc, 
                axis2_env_t **env)
{
    axis2_svc_impl_t *svc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    
    if(NULL != svc->param_container)
    {
	    AXIS2_PARAM_CONTAINER_FREE(svc->param_container, env);
        svc->param_container = NULL;
    }
    
    if(NULL != svc->flow_container)
    {
	    AXIS2_FLOW_CONTAINER_FREE(svc->flow_container, env);
        svc->flow_container = NULL;
    }
    
    if(NULL != svc->wsdl_svc)
    {
	    AXIS2_WSDL_SVC_FREE(svc->wsdl_svc, env);
        svc->wsdl_svc = NULL;
    }
    
    svc_impl->parent = NULL;
    
    if(NULL != svc_impl->wasaction_op_map)
    {
        /* By this time operations are already freed
        */
		axis2_hash_free(svc_impl->wasaction_op_map, env);
        svc_impl->wasaction_op_map = NULL;
    }
    
    if(NULL != svc_impl->module_list)
    {
        int i = 0;
        int size = 0;
        
        size = AXIS2_ARRAY_LIST_SIZE(svc_impl->module_list, env);
        for (i = 0; i < size; i++)
        {
            axis2_qname_t *qname = NULL;
            qname = AXIS2_ARRAY_LIST_GET(svc_impl->module_list, env, i);
            if (qname)
            {
                AXIS2_QNAME_FREE (qname, env);
                qname = NULL;
            }  
        }
	    AXIS2_ARRAY_LIST_FREE(svc_impl->module_list, env);
        svc_impl->module_list = NULL;
    }

    if(NULL != svc_impl->axis_svc_name)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->axis_svc_name);
        svc_impl->axis_svc_name = NULL;        
    }        
    
	if(NULL != svc->ops)
    {
		AXIS2_FREE((*env)->allocator, svc->ops);
        svc->ops = NULL;
    }

    if(svc_impl)
    {
	    AXIS2_FREE((*env)->allocator, svc_impl);
        svc_impl = NULL;
    }
    
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_svc_add_op (axis2_svc_t *svc,
                    axis2_env_t **env,
                    axis2_op_t *op)
{
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op, AXIS2_FAILURE);
    
    status = AXIS2_OP_SET_PARENT(op, env, svc);
    if(AXIS2_SUCCESS == status)
    {
        axis2_wsdl_interface_t *wsdl_interface = NULL;
        
        wsdl_interface = axis2_svc_get_svc_interface(svc, env);
        axis2_qname_t *svc_qname = AXIS2_SVC_GET_QNAME(svc, env);
        axis2_char_t *svc_name = AXIS2_QNAME_GET_LOCALPART(svc_qname, env);
        axis2_qname_t *op_qname = AXIS2_OP_GET_QNAME(op, env);
        axis2_char_t *op_name = AXIS2_QNAME_GET_LOCALPART(op_qname, env);
        status = AXIS2_WSDL_INTERFACE_SET_OP(wsdl_interface, env, op);
    }
    return status;
}

axis2_op_t * AXIS2_CALL
axis2_svc_get_op_with_qname (axis2_svc_t *svc,
                                        axis2_env_t **env,
		                                axis2_qname_t *op_name)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_op_t *op_l = NULL;
    axis2_char_t *op_str = NULL;
    /*axis2_hash_t *all_ops = NULL; */
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, op_name, NULL);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    
    op_str = AXIS2_QNAME_GET_LOCALPART(op_name, env);
    /*TODO commented until AXIS2_WSDL_INTERFACE_GET_ALL_OPS is implemented
    all_ops = AXIS2_WSDL_INTERFACE_GET_ALL_OPS(
        axis2_svc_get_svc_interface(svc, env), env);
    op_l = (axis2_op_t) (axis2_hash_get (all_ops, 
        op_str, AXIS2_HASH_KEY_STRING));
    */
    if(NULL == op_l )
    {
        op_l = (axis2_op_t *) (axis2_hash_get (
            svc_impl->wasaction_op_map, op_str, AXIS2_HASH_KEY_STRING));
    }
    
    return op_l;	
}	

axis2_op_t * AXIS2_CALL
axis2_svc_get_op_with_name (axis2_svc_t *svc, 
                                    axis2_env_t **env,
		                            const axis2_char_t* nc_name)
{
    axis2_wsdl_interface_t *wsdl_interface = NULL;
    axis2_hash_t *ops = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, nc_name, NULL);
    
    wsdl_interface = axis2_svc_get_svc_interface(svc, env);
    ops = AXIS2_WSDL_INTERFACE_GET_OPS(wsdl_interface, env);
    return (axis2_op_t *) axis2_hash_get(ops, nc_name, AXIS2_HASH_KEY_STRING);
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_ops (axis2_svc_t *svc, 
                            axis2_env_t **env)
{
    axis2_wsdl_interface_t *wsdl_interface = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    wsdl_interface = axis2_svc_get_svc_interface(svc, env);
    return AXIS2_WSDL_INTERFACE_GET_OPS(wsdl_interface, env);
}
	
axis2_status_t AXIS2_CALL
axis2_svc_set_parent (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                axis2_svc_grp_t *svc_grp)
{
    axis2_svc_impl_t *svc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_grp, AXIS2_FAILURE);
    
    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    
	svc_impl->parent = svc_grp;
    
	return AXIS2_SUCCESS;
}

axis2_svc_grp_t * AXIS2_CALL
axis2_svc_get_parent (axis2_svc_t *svc, 
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
	
	return AXIS2_INTF_TO_IMPL(svc)->parent;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_qname (axis2_svc_t *svc, 
                    axis2_env_t **env,
                    axis2_qname_t *qname)
{
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    
    return AXIS2_WSDL_SVC_SET_QNAME(svc->wsdl_svc, env, qname);
}

axis2_qname_t * AXIS2_CALL
axis2_svc_get_qname (const axis2_svc_t *svc, 
                    axis2_env_t **env)
{
	AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_WSDL_SVC_GET_QNAME(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_add_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                axis2_param_t *param)
{
    axis2_param_container_t *param_container_l = NULL;
    axis2_property_t *property = NULL;
        
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
    
   
    if(axis2_svc_is_param_locked(svc, env, AXIS2_PARAM_GET_NAME(param, env)))
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    } 
    else
    {
        property = (axis2_property_t *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->
                wsdl_component, env, AXIS2_PARAMETER_KEY);
        if(property)
            param_container_l = (axis2_param_container_t *)
                AXIS2_PROPERTY_GET_VALUE(property, env);
        if(param_container_l)
            return AXIS2_PARAM_CONTAINER_ADD_PARAM(param_container_l, env, param);
    }
    return AXIS2_FAILURE;
}

axis2_param_t * AXIS2_CALL
axis2_svc_get_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                const axis2_char_t *name)
{
    axis2_param_container_t *param_container_l = NULL;
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    property = (axis2_property_t *)
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->
            wsdl_component, env, AXIS2_PARAMETER_KEY);
    if(property)
        param_container_l = (axis2_param_container_t *)
            AXIS2_PROPERTY_GET_VALUE(property, env);
    if(param_container_l)
        return AXIS2_PARAM_CONTAINER_GET_PARAM(param_container_l, env, name);
    return NULL;
}

axis2_array_list_t * AXIS2_CALL
axis2_svc_get_params (axis2_svc_t *svc, 
                        axis2_env_t **env)
{
    axis2_param_container_t *param_container_l = NULL;
    axis2_property_t *property = NULL;

	AXIS2_ENV_CHECK(env, NULL);
	
    property = (axis2_property_t *)
    AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->
        wsdl_component, env, AXIS2_PARAMETER_KEY);
    if(property)
        param_container_l = (axis2_param_container_t *)
            AXIS2_PROPERTY_GET_VALUE(property, env);
    if(param_container_l)
        return AXIS2_PARAM_CONTAINER_GET_PARAMS(param_container_l, env);
    return NULL;
}

axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked (axis2_svc_t *svc, 
                            axis2_env_t **env,
		                    axis2_char_t *param_name)
{
    axis2_bool_t locked = AXIS2_FALSE;
    axis2_param_t *param_l = NULL;
    axis2_conf_t *conf_l = NULL;
    axis2_svc_grp_t *parent = NULL;
    axis2_bool_t ret = AXIS2_FALSE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);
    
   /*checking the locked value of parent */

    parent = axis2_svc_get_parent(svc, env);
    if(NULL == parent)
    {
        return AXIS2_FALSE;
    }
    
    conf_l = AXIS2_SVC_GRP_GET_AXIS_DESC(parent, env);
    if(NULL == conf_l)
    {
        return AXIS2_FALSE;
    }
    locked =  AXIS2_CONF_IS_PARAM_LOCKED(conf_l, env, param_name);
    
    if(AXIS2_TRUE == locked)
    {
        ret = AXIS2_TRUE;
    } 
    else 
    {
        param_l = axis2_svc_get_param(svc, env, param_name);
        if(NULL == param_l)
        {
            return AXIS2_FALSE;
        }
        ret = AXIS2_PARAM_IS_LOCKED(param_l, env);
    }
    
    return ret;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_svc_interface(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_wsdl_interface_t *svc_interface) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_interface, AXIS2_FAILURE);
    
    return AXIS2_WSDL_SVC_SET_SVC_INTERFACE(svc->wsdl_svc, env, svc_interface);
}

axis2_wsdl_interface_t *AXIS2_CALL
axis2_svc_get_svc_interface(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_WSDL_SVC_GET_SVC_INTERFACE(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_engage_module(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_module_desc_t * moduleref,
                            axis2_conf_t * conf)
{
    axis2_module_desc_t * modu = NULL;
    axis2_array_list_t *collection_module = NULL;
    axis2_phase_resolver_t *phase_resolver = NULL;
    int i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    int size = 0;
    axis2_property_t *property = NULL;
        
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, moduleref, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, conf, AXIS2_FAILURE);
    
    property = (axis2_property_t *)
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->wsdl_component,
            env, AXIS2_MODULEREF_KEY);
    if(property)
        collection_module = (axis2_array_list_t *) 
            AXIS2_PROPERTY_GET_VALUE(property, env);
    if(!collection_module)
    {
        return AXIS2_FAILURE;
    }
    size = AXIS2_ARRAY_LIST_SIZE(collection_module, env);
    if(AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
    {
        return AXIS2_ERROR_GET_STATUS_CODE((*env)->error);
    }
    for(i = 0; i < size; i++)
    {
        modu = (axis2_module_desc_t *) AXIS2_ARRAY_LIST_GET(collection_module,
            env, i);
        if(AXIS2_QNAME_EQUALS(AXIS2_MODULE_DESC_GET_NAME(modu, env), env,  
            AXIS2_MODULE_DESC_GET_NAME(moduleref, env)))
        {
            /* module has already been engaged on the service. Operation 
             *terminated !!! 
             */
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC, AXIS2_FAILURE);
            return AXIS2_FAILURE;            
        }
        
    }
   
    phase_resolver = axis2_phase_resolver_create_with_config(env, conf);
    if(!phase_resolver)
    {
        return AXIS2_FAILURE;
    }
    status = AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_SVC(phase_resolver, env, svc, 
        moduleref);
    if(status)
    {
        status = AXIS2_ARRAY_LIST_ADD(collection_module, env, moduleref);
    }
    if(phase_resolver)
    {
        AXIS2_PHASE_RESOLVER_FREE(phase_resolver, env);
        phase_resolver = NULL;
    }
    
    return status;
}

axis2_status_t AXIS2_CALL
axis2_svc_add_module_ops(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_module_desc_t *module_desc,
                            axis2_conf_t *conf) 
{
    axis2_hash_t *map = NULL;
    axis2_hash_index_t *index = NULL;
    axis2_phase_resolver_t *pr = NULL;
    axis2_op_t *op_desc = NULL;
    axis2_array_list_t *params = NULL;
    axis2_param_t *param = NULL;
    axis2_status_t status = AXIS2_FAILURE;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_desc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, conf, AXIS2_FAILURE);
    
    map = AXIS2_MODULE_DESC_GET_OPS(module_desc, env);
    pr = axis2_phase_resolver_create_with_config_and_svc(env, conf, svc);
    
    if(!pr)
    {
        return AXIS2_FAILURE;
    }
    
    for (index = axis2_hash_first (map, env); index; index = 
        axis2_hash_next (env, index))
    {
        int size = 0;
        int j = 0;
        void *v = NULL;
        axis2_hash_this (index, NULL, NULL, &v);
        op_desc = (axis2_op_t *) v;
        params = AXIS2_OP_GET_PARAMS(op_desc, env);
        /* Adding wsa-maping into service */
        size = AXIS2_ARRAY_LIST_SIZE(params, env);
        
        if(AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
        {
            if(pr)
            {
                AXIS2_PHASE_RESOLVER_FREE(pr, env);
                pr = NULL;
            }
            return AXIS2_FAILURE;
        }
        for(j = 0; j < size; j++)
        {
            param = (axis2_param_t *) AXIS2_ARRAY_LIST_GET(params, env, j);
            if(0 == AXIS2_STRCMP( AXIS2_PARAM_GET_NAME(param, env), 
                    AXIS2_WSA_ACTION))
            {
                status = axis2_svc_add_mapping(svc, env,
                    (axis2_char_t *) AXIS2_PARAM_GET_VALUE(param, env), op_desc);
                if(AXIS2_SUCCESS != status)
                {
                    if(pr)
                    {
                        AXIS2_PHASE_RESOLVER_FREE(pr, env);
                        pr = NULL;                        
                    }
                    return status;
                }
            }
                
        }
        
        status = AXIS2_PHASE_RESOLVER_BUILD_MODULE_OP(pr, env, op_desc);
        
        if(AXIS2_SUCCESS != status)
        {
            if(pr)
            {
                AXIS2_PHASE_RESOLVER_FREE(pr, env);
                pr = NULL;                
            }
            return status;
        }
        
        status = axis2_svc_add_op(svc, env, op_desc);
        if(AXIS2_SUCCESS != status)
        {
            if(pr)
            {
                AXIS2_PHASE_RESOLVER_FREE(pr, env);
                pr = NULL;                
            }
            return status;
        }
 
    }
    
    if(pr)
    {
        AXIS2_PHASE_RESOLVER_FREE(pr, env); 
        pr = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_add_to_engaged_module_list(axis2_svc_t *svc,
                                axis2_env_t **env,
                                axis2_module_desc_t *module_name) 
{
    axis2_array_list_t *collection_module = NULL;
    axis2_module_desc_t * module_desc = NULL;
    int i = 0;
    int size = 0;
    axis2_property_t *property = NULL;
 
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_name, AXIS2_FAILURE);
    
    property = (axis2_property_t *) 
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->
        wsdl_component, env, AXIS2_MODULEREF_KEY);
    if(property)
        collection_module = (axis2_array_list_t *) AXIS2_PROPERTY_GET_VALUE(
            property, env);
    if(!collection_module)
    {
        return AXIS2_FAILURE;
    }
    
    size = AXIS2_ARRAY_LIST_SIZE(collection_module, env);
    
    if(AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
    {
        return AXIS2_ERROR_GET_STATUS_CODE((*env)->error);
    }
    
    for(i = 0; i < size; i++)
    {
        axis2_qname_t *module_d_name = NULL;
        axis2_qname_t *module_d_name_l = NULL;
        
        module_desc = (axis2_module_desc_t *) AXIS2_ARRAY_LIST_GET(
            collection_module, env, i);
        module_d_name = AXIS2_MODULE_DESC_GET_NAME(module_desc, env);
        module_d_name_l = AXIS2_MODULE_DESC_GET_NAME(module_name, env);
            
        if(AXIS2_QNAME_EQUALS(module_d_name, env, module_d_name_l))
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC, AXIS2_FAILURE);
            return AXIS2_SUCCESS;
        }
    }
    return AXIS2_ARRAY_LIST_ADD(collection_module, env, module_name);
}

axis2_array_list_t * AXIS2_CALL
axis2_svc_get_engaged_modules(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    
    property = (axis2_property_t *)    
     AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, AXIS2_MODULEREF_KEY);
    if(property)
        return (axis2_array_list_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

void * AXIS2_CALL
axis2_svc_get_wsdl_op(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_qname_t *op_name)
{
    axis2_wsdl_interface_t *svc_interface = NULL;
    axis2_char_t * op_str = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, op_name, NULL);
    
    op_str = AXIS2_QNAME_GET_LOCALPART(op_name, env);
    svc_interface = axis2_svc_get_svc_interface(svc, env);
    if(svc_interface)
    {
        return AXIS2_WSDL_INTERFACE_GET_OP(svc_interface, env, op_str) ;
    }
    else
        return NULL;
    
}

axis2_status_t AXIS2_CALL
axis2_svc_set_context_path(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *context_path) 
{
    axis2_char_t *context_path_l = NULL;
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, context_path, AXIS2_FAILURE);
    
    context_path_l = AXIS2_STRDUP(context_path, env);
    if(context_path_l)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;        
    }
    property = axis2_property_create(env);
    property = AXIS2_PROPERTY_SET_VALUE(property, env, context_path_l);
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
            wsdl_component, env, AXIS2_CONTEXTPATH_KEY, property);
}

axis2_char_t * AXIS2_CALL
axis2_svc_get_context_path(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    
    property = (axis2_property_t *)
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
            svc->wsdl_svc->wsdl_component, env, AXIS2_CONTEXTPATH_KEY);
    if(property)
        return (axis2_char_t *) AXIS2_PROPERTY_GET_VALUE(property, env); 
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_style(axis2_svc_t *svc,
                        axis2_env_t **env,
                        axis2_char_t * style) 
{
    axis2_char_t *style_l = NULL;
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, style, AXIS2_FAILURE);
    
    style_l = AXIS2_STRDUP(style, env);
    if(!style_l)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    property = axis2_property_create(env);
    property = AXIS2_PROPERTY_SET_VALUE(property, env, style_l);
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
        wsdl_component, env, AXIS2_STYLE_KEY, property);
}

axis2_char_t * AXIS2_CALL
axis2_svc_get_style(axis2_svc_t *svc,
                    axis2_env_t **env) 
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    property = (axis2_property_t *)
     AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, AXIS2_STYLE_KEY);
    if(property)
        return (axis2_char_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

axis2_flow_t * AXIS2_CALL
axis2_svc_get_inflow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    property = (axis2_property_t *)
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
            svc->wsdl_svc->wsdl_component, env, AXIS2_INFLOW_KEY);
    if(property)
        return (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_inflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_flow_t *inflow) 
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, inflow, AXIS2_FAILURE);
    
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_VALUE(property, env, inflow);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_flow_free_void_arg);
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
        wsdl_component, env, AXIS2_INFLOW_KEY, property);
}

axis2_flow_t * AXIS2_CALL
axis2_svc_get_outflow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    property = (axis2_property_t *)
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, AXIS2_OUTFLOW_KEY);
    if(property)
        return (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_outflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_flow_t *outflow) 
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, outflow, AXIS2_FAILURE);
    
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_VALUE(property, env, outflow);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_flow_free_void_arg);
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
        wsdl_component, env, AXIS2_OUTFLOW_KEY, property);
}

axis2_flow_t *AXIS2_CALL
axis2_svc_get_fault_inflow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    
    property = (axis2_property_t *)
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, AXIS2_IN_FAULTFLOW_KEY);
    if(property)
        return (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_fault_inflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_flow_t *fault_flow) 
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, fault_flow, AXIS2_FAILURE);
    
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_VALUE(property, env, fault_flow);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_flow_free_void_arg);
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
        wsdl_component, env, AXIS2_IN_FAULTFLOW_KEY, property);
}

axis2_flow_t * AXIS2_CALL
axis2_svc_get_fault_outflow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    axis2_property_t *property = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    property = (axis2_property_t *)
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, AXIS2_OUT_FAULTFLOW_KEY);
    if(property)
        return (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_fault_outflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_flow_t *fault_flow) 
{
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, fault_flow, AXIS2_FAILURE);
    
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_VALUE(property, env, fault_flow);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_flow_free_void_arg);
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
        wsdl_component, env, AXIS2_OUT_FAULTFLOW_KEY, property);
}

axis2_op_t * AXIS2_CALL
axis2_svc_get_op_by_soap_action(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *soap_action) 
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *endpoints = NULL;
    axis2_hash_index_t *index = NULL;
    void *value = NULL;
    axis2_wsdl_endpoint_t * endpoint = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, soap_action, NULL);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    
    if(0 == AXIS2_STRCMP(soap_action, ""))
        return NULL;
    
    endpoints = axis2_svc_get_endpoints(svc, env);
    if(!endpoints)
    {
        return NULL;
    }
    index = axis2_hash_first (endpoints, env);
    if(NULL != index)
    {
        axis2_qname_t *endpoint_qname = NULL;
        
        axis2_hash_this(index, NULL, NULL, &value);
        endpoint = (axis2_wsdl_endpoint_t *) value;
        endpoint_qname = AXIS2_WSDL_ENDPOINT_GET_NAME(endpoint, env);
        return axis2_svc_get_op_by_soap_action_and_endpoint(svc, env, 
            soap_action, endpoint_qname);
    }

    return NULL;
}

axis2_op_t * AXIS2_CALL
axis2_svc_get_op_by_soap_action_and_endpoint(axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        axis2_char_t *soap_action,
                                        axis2_qname_t * endpoint) 
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *binding_ops = NULL;
    int count = 0;
    axis2_hash_index_t *index_i = NULL;
    axis2_wsdl_binding_op_t *binding_op = NULL;
    axis2_wsdl_soap_op_t *element = NULL;
    axis2_op_t *op = NULL;
    axis2_linked_list_t *extensiblity_elements = NULL;
    axis2_qname_t *type_1 = NULL;
    axis2_wsdl_binding_t *wsdl_binding = NULL;
    axis2_wsdl_endpoint_t *wsdl_endpoint = NULL;
        
    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    
    type_1 = axis2_qname_create(env, "op", AXIS2_SOAP_11_OP, NULL);
    wsdl_endpoint = axis2_svc_get_endpoint(svc, env, endpoint);
    wsdl_binding = AXIS2_WSDL_ENDPOINT_GET_BINDING(wsdl_endpoint, env);
    binding_ops = AXIS2_WSDL_BINDING_GET_BINDING_OPS(wsdl_binding, env);
    if(NULL == binding_ops)
    {
        return NULL;
    }
    index_i = axis2_hash_first (binding_ops, env);
    while(NULL != index_i)
    {
        int j = 0;
        int size = 0;
        const void *k = NULL;
        axis2_status_t status = AXIS2_FAILURE;
        
        axis2_hash_this (index_i, &k, NULL, NULL);
        binding_op = (axis2_wsdl_binding_op_t *) k;
        extensiblity_elements = AXIS2_WSDL_COMPONENT_GET_EXTENSIBILITY_ELEMENTS(
            binding_op->extensible_component->wsdl_component, env);
        
        size = AXIS2_LINKED_LIST_SIZE(extensiblity_elements, env);
        status = AXIS2_ERROR_GET_STATUS_CODE((*env)->error);
        if(AXIS2_SUCCESS != status)
        {
            return NULL;
        }
        while(j < size)
        {
            axis2_qname_t *type_2 = NULL;
            
            element = (axis2_wsdl_soap_op_t *) AXIS2_LINKED_LIST_GET(
                extensiblity_elements, env, j); 
                        
            type_2 = AXIS2_WSDL_EXTENSIBLE_ELEMENT_GET_TYPE(element->
                extensible_element, env);
            if(AXIS2_QNAME_EQUALS(type_2, env, type_1))
            {
                if(0 == AXIS2_STRCMP(AXIS2_WSDL_SOAP_OP_GET_SOAP_ACTION(
                    element, env), soap_action))
                {
                    op = (axis2_op_t *) 
                        AXIS2_WSDL_BINDING_OP_GET_OP(binding_op,
                            env);
                    count++;    
                }
                    
            }
            AXIS2_QNAME_FREE(type_2, env);
            type_2 = NULL;
            j++;
        }
        index_i = axis2_hash_next (env, index_i);
    } 
    if (1 == count) 
    {
        return op;
    }
    AXIS2_QNAME_FREE(type_1, env);
    type_1 = NULL;
    return NULL;
}
       
axis2_char_t * AXIS2_CALL
axis2_svc_get_axis2_svc_name(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(svc)->axis_svc_name;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_axis2_svc_name(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *axis_svc_name) 
{
    axis2_svc_impl_t *svc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, axis_svc_name, AXIS2_FAILURE);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    
    if(svc_impl->axis_svc_name)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->axis_svc_name);
        svc_impl->axis_svc_name = NULL;
    }
    svc_impl->axis_svc_name = AXIS2_STRDUP(axis_svc_name, env);
    if(!svc_impl->axis_svc_name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_svc_set_last_update(axis2_svc_t *svc,
                            axis2_env_t **env)
{
    /*last_update = new Date().getTime(); */
    return AXIS2_SUCCESS;
}

long AXIS2_CALL
axis2_svc_get_last_update(axis2_svc_t *svc,
                            axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(svc)->last_update;
}

axis2_char_t * AXIS2_CALL
axis2_svc_get_filename(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(svc)->filename;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_filename(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *filename) 
{
    axis2_svc_impl_t *svc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, filename, AXIS2_FAILURE);
    
    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if(svc_impl->filename)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->filename);
        svc_impl->filename = NULL;
    }
    svc_impl->filename = (axis2_char_t *) AXIS2_STRDUP(filename, env);
    if(!svc_impl->filename)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_endpoints(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return AXIS2_WSDL_SVC_GET_ENDPOINTS(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_endpoints(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_hash_t * endpoints) 
{
    return AXIS2_WSDL_SVC_SET_ENDPOINTS(svc->wsdl_svc, env, endpoints);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_endpoint(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_wsdl_endpoint_t * endpoint)
{
    return AXIS2_WSDL_SVC_SET_ENDPOINT(svc->wsdl_svc, env, endpoint);
}

axis2_wsdl_endpoint_t * AXIS2_CALL
axis2_svc_get_endpoint(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_qname_t * qname) 
{
    return AXIS2_WSDL_SVC_GET_ENDPOINT(svc->wsdl_svc, env, qname);
}

axis2_char_t * AXIS2_CALL
axis2_svc_get_namespace(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return AXIS2_WSDL_SVC_GET_NAMESPACE(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_add_mapping(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t * mapping_key , 
                            axis2_op_t * op_desc)
{
    axis2_svc_impl_t *svc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, mapping_key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op_desc, AXIS2_FAILURE);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    
    axis2_hash_set(svc_impl->wasaction_op_map, mapping_key, 
        AXIS2_HASH_KEY_STRING, op_desc);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_add_module_ref(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_qname_t *moduleref)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_qname_t *qmoduleref_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, moduleref, AXIS2_FAILURE);
    svc_impl = AXIS2_INTF_TO_IMPL(svc);

    qmoduleref_l = AXIS2_QNAME_CLONE(moduleref, env);
    return AXIS2_ARRAY_LIST_ADD(AXIS2_INTF_TO_IMPL(svc)->module_list, env, 
        qmoduleref_l);
}

axis2_array_list_t *AXIS2_CALL
axis2_svc_get_modules(axis2_svc_t *svc,
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(svc)->module_list;
}
