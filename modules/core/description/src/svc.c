#include <axis2_svc.h>

typedef struct axis2_svc_impl_s axis2_svc_impl_t;
	
/** 
 * @brief Service struct impl
 *	Axis2 Service impl  
 */ 
struct axis2_svc_impl_s
{
	axis2_svc_t svc;
	axis2_param_container_t *param_container;
    axis2_wsdl_svc_t *wsdl_svc;
	axis2_svc_grp_t *parent;
	axis2_hash_t *wasaction_opeartionmap;  
    axis2_char_t *axis2_svc_name;
    axis2_array_list_t *module_list; 
    long last_update;    

};

#define AXIS2_INTF_TO_IMPL(svc) ((axis2_svc_impl_t *)svc)

/*************************** Function headers ********************************/

axis2_status_t AXIS2_CALL 
axis2_svc_free (axis2_svc_t *svc, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_add_operation (axis2_svc_t *svc, axis2_env_t **env
		,                   struct axis2_operation *operation);

struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_with_qname (axis2_svc_t *svc, axis2_env_t **env,
		                            axis2_qname_t *operation_name);
		
struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_with_name (axis2_svc_t *svc, axis2_env_t **env,
		                            const axis2_char_t* operation_name);

axis2_hash_t * AXIS2_CALL
axis2_svc_get_operations (axis2_svc_t *svc, axis2_env_t **env);
		
axis2_status_t AXIS2_CALL
axis2_svc_set_parent (axis2_svc_t *svc, axis2_env_t **env, 
                        axis2_svc_grp_t *svc_grp);

axis2_svc_grp_t * AXIS2_CALL
axis2_svc_get_parent (axis2_svc_t *svc, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_name (axis2_svc_t *svc, 
                    axis2_env_t **env,
                    axis2_qname_t *qname);

axis2_qname_t * AXIS2_CALL
axis2_svc_get_name (const axis2_svc_t *svc, axis2_env_t **env);	

axis2_status_t AXIS2_CALL
axis2_svc_add_param (axis2_svc_t *svc, axis2_env_t **env, axis2_param_t *param);

axis2_param_t * AXIS2_CALL
axis2_svc_get_param (axis2_svc_t *svc, axis2_env_t **env,
		                const axis2_char_t *name);

axis2_hash_t * AXIS2_CALL
axis2_svc_get_params (axis2_svc_t *svc, axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked (axis2_svc_t *svc, axis2_env_t **env,
		                    const axis2_char_t *param_name);

axis2_hash_t * AXIS2_CALL
axis2_svc_get_component_properties(axis2_svc_t *svc,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_component_properties(axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        axis2_hash_t *properties);

axis2_wsdl_component_t * AXIS2_CALL
axis2_svc_get_component_property(axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        const axis2_char_t *key);

axis2_status_t AXIS2_CALL
axis2_svc_set_component_property (axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        const void *key,
                                        void *value);

/************************* End of function headers ***************************/

axis2_svc_t * AXIS2_CALL
axis2_svc_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
	axis2_svc_impl_t *svc_impl = (axis2_svc_impl_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_svc_impl_t));
    if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    svc_impl->svc.ops = AXIS2_MALLOC((*env)->allocator, sizeof(axis2_svc_ops_t));
	if(NULL == svc_impl->svc.ops)
	{
        AXIS2_FREE((*env)->allocator, svc_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
	svc_impl->svc.ops->free = axis2_svc_free;
	svc_impl->svc.ops->add_operation = axis2_svc_add_operation;
	svc_impl->svc.ops->get_operation_with_qname = 
		axis2_svc_get_operation_with_qname;
	svc_impl->svc.ops->get_operation_with_name = 
		axis2_svc_get_operation_with_name;
	svc_impl->svc.ops->get_operations = axis2_svc_get_operations;
	svc_impl->svc.ops->set_parent = axis2_svc_set_parent;
	svc_impl->svc.ops->get_parent = axis2_svc_get_parent;
    svc_impl->svc.ops->set_name = axis2_svc_set_name;
	svc_impl->svc.ops->get_name = axis2_svc_get_name;
	svc_impl->svc.ops->add_param = axis2_svc_add_param;
	svc_impl->svc.ops->get_param = axis2_svc_get_param;
	svc_impl->svc.ops->get_params = axis2_svc_get_params;
    svc_impl->svc.ops->is_param_locked = axis2_svc_is_param_locked;
    svc_impl->svc.ops->get_component_properties = axis2_svc_get_component_properties;
    svc_impl->svc.ops->set_component_properties = axis2_svc_set_component_properties;
	svc_impl->svc.ops->get_component_property = axis2_svc_get_component_property;
    svc_impl->svc.ops->set_component_property = axis2_svc_set_component_property;
    
	axis2_param_container_t *param_container = (axis2_param_container_t *)
		axis2_param_container_create(env);		
	if(NULL == param_container)
	{
        AXIS2_FREE((*env)->allocator, svc_impl->svc.ops);
        AXIS2_FREE((*env)->allocator, svc_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}
    
    svc_impl->param_container = param_container;
    
    axis2_wsdl_svc_t *wsdl_svc = (axis2_wsdl_svc_t *)
		axis2_wsdl_svc_create(env);		
	if(NULL == wsdl_svc)
	{
        AXIS2_FREE((*env)->allocator, svc_impl->svc.ops);
        AXIS2_PARAM_CONTAINER_FREE(svc_impl->param_container, env);
        AXIS2_FREE((*env)->allocator, svc_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}

	svc_impl->wsdl_svc = wsdl_svc;
	
	svc_impl->parent = NULL;
	
	svc_impl->wasaction_opeartionmap = axis2_hash_make (env);				
	if(NULL == svc_impl->wasaction_opeartionmap)
	{
        AXIS2_FREE((*env)->allocator, svc_impl->svc.ops);
        AXIS2_PARAM_CONTAINER_FREE(svc_impl->param_container, env);
        AXIS2_WSDL_SVC_FREE(svc_impl->wsdl_svc, env);
        AXIS2_FREE((*env)->allocator, svc_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}
    
    /** Create modle list of default size */
    svc_impl->module_list = axis2_array_list_create(env, 0);
    if(NULL == svc_impl->module_list)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.ops);
        AXIS2_PARAM_CONTAINER_FREE(svc_impl->param_container, env);
        AXIS2_WSDL_SVC_FREE(svc_impl->wsdl_svc, env);
		axis2_hash_free(svc_impl->wasaction_opeartionmap, env);
        AXIS2_FREE((*env)->allocator, svc_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    svc_impl->axis2_svc_name = NULL;
    svc_impl->last_update = 0;

	return &(svc_impl->svc);	
}

axis2_svc_t * AXIS2_CALL
axis2_svc_create_with_qname (axis2_env_t **env, 
                                axis2_qname_t *qname)
{
	axis2_svc_impl_t *svc_impl = AXIS2_INTF_TO_IMPL(axis2_svc_create(env));
	if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    
	axis2_svc_set_name(&(svc_impl->svc), env, qname);
	
	return &(svc_impl->svc);
}

axis2_svc_t * AXIS2_CALL
axis2_svc_create_with_wsdl_svc (axis2_env_t **env, 
                                axis2_wsdl_svc_t *wsdl_svc)
{
	axis2_svc_impl_t *svc_impl = 
        AXIS2_INTF_TO_IMPL(axis2_svc_create(env));
    
	if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    AXIS2_PARAM_CHECK((*env)->error, wsdl_svc, NULL);	
	
    svc_impl->wsdl_svc = wsdl_svc;
       
	return &(svc_impl->svc);	
}

/********************** Start of function implementations ********************/

axis2_status_t AXIS2_CALL
axis2_svc_free (axis2_svc_t *svc, 
                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
	if(NULL != svc->ops)
		AXIS2_FREE((*env)->allocator, svc->ops);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->param_container)
	    AXIS2_PARAM_CONTAINER_FREE(AXIS2_INTF_TO_IMPL(svc)->param_container
        , env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->wsdl_svc)
	    AXIS2_PARAM_CONTAINER_FREE(AXIS2_INTF_TO_IMPL(svc)->wsdl_svc
        , env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->parent)
	    AXIS2_SVC_GRP_FREE(AXIS2_INTF_TO_IMPL(svc)->parent, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap)
		axis2_hash_free(AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->module_list)
	    AXIS2_ARRAY_LIST_FREE(AXIS2_INTF_TO_IMPL(svc)->module_list, env);

    if(NULL != AXIS2_INTF_TO_IMPL(svc)->axis2_svc_name)
        AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(svc)->axis2_svc_name);        
    
	AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(svc));
    
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_svc_add_operation (axis2_svc_t *svc,
                            axis2_env_t **env,
		                    struct axis2_operation *operation)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, operation, AXIS2_FALSE);
	
	if (NULL == (AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap))
	{                    
		AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap = axis2_hash_make (env);
	}
    
    axis2_qname_t *tempqname = AXIS2_OPERATION_GET_NAME(operation, env);
	
	if(!tempqname) return AXIS2_ERROR_INVALID_NULL_PARAM;
		
	axis2_char_t *tempname = AXIS2_QNAME_GET_LOCALPART(tempqname, env);
	
    if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
	
	axis2_hash_set (AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap		
		, tempname
		, AXIS2_HASH_KEY_STRING, operation);
	
	return AXIS2_SUCCESS;
}

struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_with_qname (axis2_svc_t *svc,
                                        axis2_env_t **env,
		                                axis2_qname_t *operation_name)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, operation_name, NULL);
	if(NULL == AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM, NULL);	
	}
    
	return (struct axis2_operation *) (axis2_hash_get (AXIS2_INTF_TO_IMPL(svc)->
        wasaction_opeartionmap, AXIS2_QNAME_GET_LOCALPART(operation_name, env),
        AXIS2_HASH_KEY_STRING));
	
}	

struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_with_name (axis2_svc_t *svc, 
                                    axis2_env_t **env,
		                            const axis2_char_t* name)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, name, NULL);
	axis2_char_t *temp_name = AXIS2_STRDUP(name, env);
    AXIS2_PARAM_CHECK((*env)->error, temp_name, NULL); 
    
	return (struct axis2_operation *) (axis2_hash_get 
		(AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap
		, temp_name, AXIS2_HASH_KEY_STRING));
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_operations (axis2_svc_t *svc, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
	
	return AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap;
}
	
axis2_status_t AXIS2_CALL
axis2_svc_set_parent (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                axis2_svc_grp_t *svc_grp)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_grp, AXIS2_FAILURE);
	
	AXIS2_INTF_TO_IMPL(svc)->parent = svc_grp;
    
	return AXIS2_SUCCESS;
}

axis2_svc_grp_t * AXIS2_CALL
axis2_svc_get_parent (axis2_svc_t *svc, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    if(NULL == AXIS2_INTF_TO_IMPL(svc)->parent)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_PARENT_NULL, NULL);
	
	return AXIS2_INTF_TO_IMPL(svc)->parent;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_name (axis2_svc_t *svc, 
                    axis2_env_t **env,
                    axis2_qname_t *qname)
{
	AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    
    return AXIS2_WSDL_SVC_SET_NAME(AXIS2_INTF_TO_IMPL(svc)->wsdl_svc, env, qname);
}

axis2_qname_t * AXIS2_CALL
axis2_svc_get_name (const axis2_svc_t *svc, 
                    axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    return AXIS2_WSDL_SVC_GET_NAME(AXIS2_INTF_TO_IMPL(svc)->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_add_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                axis2_param_t *param)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
    
	if(NULL == AXIS2_INTF_TO_IMPL(svc)->param_container)
	{
		AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FAILURE);
	}
	axis2_hash_set (AXIS2_PARAM_CONTAINER_GET_PARAMS(AXIS2_INTF_TO_IMPL(svc)->
        param_container, env), AXIS2_PARAM_GET_NAME(param, env), 
        AXIS2_HASH_KEY_STRING, param);	
	return AXIS2_SUCCESS;
}

axis2_param_t * AXIS2_CALL
axis2_svc_get_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                const axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    
	if(NULL == AXIS2_INTF_TO_IMPL(svc)->param_container)
	{
		AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FAILURE);
	}
    
	axis2_char_t *tempname = AXIS2_STRDUP(name, env);
	if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM,
        AXIS2_FAILURE);
		
	return (axis2_param_t *)(axis2_hash_get 
		(AXIS2_PARAM_CONTAINER_GET_PARAMS(AXIS2_INTF_TO_IMPL(svc)->
        param_container, env), tempname, AXIS2_HASH_KEY_STRING));
	
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_params (axis2_svc_t *svc, 
                        axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
	return AXIS2_PARAM_CONTAINER_GET_PARAMS(AXIS2_INTF_TO_IMPL(svc)->
        param_container, env);
	
}

axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked (axis2_svc_t *svc, 
                            axis2_env_t **env,
		                    const axis2_char_t *param_name)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FALSE);
    if(NULL == AXIS2_INTF_TO_IMPL(svc)->param_container)
	{
		AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FALSE);
	}
	
	axis2_char_t *tempname = AXIS2_STRDUP(param_name, env);
	if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM,
        AXIS2_FAILURE);
		
	return AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED
		(AXIS2_INTF_TO_IMPL(svc)->param_container, env, param_name); 
	
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_component_properties(axis2_svc_t *svc,
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    
    return AXIS2_WSDL_SVC_GET_COMPONENT_PROPERTIES(AXIS2_INTF_TO_IMPL(svc)->
        wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_component_properties(axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        axis2_hash_t *properties)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    
    return AXIS2_WSDL_SVC_SET_COMPONENT_PROPERTIES(AXIS2_INTF_TO_IMPL(svc)->
        wsdl_svc, env, properties);
}

axis2_wsdl_component_t * AXIS2_CALL
axis2_svc_get_component_property(axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        const axis2_char_t *key)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, key, NULL);
    
    return AXIS2_WSDL_SVC_GET_COMPONENT_PROPERTY(AXIS2_INTF_TO_IMPL(svc)->
        wsdl_svc, env, key); 
}

axis2_status_t AXIS2_CALL
axis2_svc_set_component_property (axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        const void *key,
                                        void *value)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
    
    return AXIS2_WSDL_SVC_SET_COMPONENT_PROPERTY(AXIS2_INTF_TO_IMPL(svc)->
        wsdl_svc, env, (axis2_char_t *) key, value);
}
