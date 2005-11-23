#include <axis2_svc.h>

typedef struct axis2_svc_impl axis2_svc_impl_t;
	
/** 
 * @brief Service struct impl
 *	Axis2 Service impl  
 */ 
struct axis2_svc_impl
{
	axis2_svc_t svc;
	struct axis2_svc_grp *parent;
	axis2_hash_t *wasaction_opeartionmap;  
    axis2_char_t *axis2_svc_name; 
    /** to keep the time that last update time of the service */
    long last_update;
    axis2_char_t * filename;
    struct axis2_wsdl_svc *service_impl;
    /** to store module ref at deploy time parsing */
    axis2_array_list_t *module_list;    

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

axis2_status_t AXIS2_CALL
axis2_svc_set_svc_interface(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_wsdl_interface *svc_interface);

/************************* End of function headers ***************************/

axis2_svc_t * AXIS2_CALL
axis2_svc_create (axis2_env_t **env)
{
    struct axis2_param_container *param_container = NULL;
    struct axis2_flow_container *flow_container = NULL;
    axis2_svc_impl_t *svc_impl = NULL;
    struct axis2_wsdl_svc *wsdl_svc = NULL;
    axis2_array_list_t *array_list_l = NULL;
    struct axis2_param_container *param_container_l = NULL;
    struct axis2_wsdl_interface *wsdl_interface_l = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
	svc_impl = (axis2_svc_impl_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_svc_impl_t));
    if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    param_container = (struct axis2_param_container *)
		axis2_param_container_create(env);		
	if(NULL == param_container)
	{
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}
    
    flow_container = (struct axis2_flow_container *)
		axis2_flow_container_create(env);		
	if(NULL == flow_container)
	{
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}
    
    svc_impl->svc.param_container = param_container;
    
    wsdl_svc = (axis2_wsdl_svc_t *)axis2_wsdl_svc_create(env);		
	if(NULL == wsdl_svc)
	{
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}

	svc_impl->svc.wsdl_svc = wsdl_svc;
    
    	svc_impl->wasaction_opeartionmap = axis2_hash_make (env);				
	if(NULL == svc_impl->wasaction_opeartionmap)
	{
        axis2_svc_free(&(svc_impl->svc), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}
    
    /** Create modle list of default size */
    svc_impl->module_list = axis2_array_list_create(env, 0);
    if(NULL == svc_impl->module_list)
    {
        axis2_svc_free(&(svc_impl->svc), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    svc_impl->svc.ops = AXIS2_MALLOC((*env)->allocator, sizeof(axis2_svc_ops_t));
	if(NULL == svc_impl->svc.ops)
	{
        axis2_svc_free(&(svc_impl->svc), env);
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
    svc_impl->svc.ops->set_svc_interface = axis2_svc_set_svc_interface;
	
	svc_impl->parent = NULL;
    svc_impl->axis2_svc_name = NULL;
    svc_impl->last_update = 0;
    
    wsdl_interface_l = axis2_wsdl_interface_create(env);
    if(NULL == wsdl_interface_l)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    axis2_svc_set_svc_interface(&(svc_impl->svc), env, wsdl_interface_l); 
    
    param_container_l = axis2_param_container_create(env);
    if(NULL == param_container_l)
       AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc_impl->svc.wsdl_svc->
        wsdl_component, env, PARAMETER_KEY, param_container_l);
    
    array_list_l = axis2_array_list_create(env, 0);
    if(NULL == array_list_l)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc_impl->svc.wsdl_svc->
        wsdl_component, env, MODULEREF_KEY, array_list_l);
	return &(svc_impl->svc);	
}

axis2_svc_t * AXIS2_CALL
axis2_svc_create_with_qname (axis2_env_t **env, 
                                axis2_qname_t *qname)
{
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
	axis2_svc_impl_t *svc_impl = AXIS2_INTF_TO_IMPL(axis2_svc_create(env));
	if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
	axis2_svc_set_name(&(svc_impl->svc), env, qname);
	
	return &(svc_impl->svc);
}

axis2_svc_t * AXIS2_CALL
axis2_svc_create_with_wsdl_svc (axis2_env_t **env, 
                                axis2_wsdl_svc_t *wsdl_svc)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_svc, NULL);
    
	axis2_svc_impl_t *svc_impl = 
        AXIS2_INTF_TO_IMPL(axis2_svc_create(env));
    
	if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
	if(NULL != svc_impl->svc.wsdl_svc)
        AXIS2_WSDL_SVC_FREE(svc_impl->svc.wsdl_svc, env);
    svc_impl->svc.wsdl_svc = wsdl_svc;
       
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
    
    if(NULL != svc->param_container)
	    AXIS2_PARAM_CONTAINER_FREE(svc->param_container, env);
    
    if(NULL != svc->flow_container)
	    AXIS2_FLOW_CONTAINER_FREE(svc->flow_container, env);
    
    if(NULL != svc->wsdl_svc)
	    AXIS2_PARAM_CONTAINER_FREE(svc->wsdl_svc, env);
    
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
		                    struct axis2_operation *axis_operation)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, operation, AXIS2_FALSE);
    
    AXIS2_OPERATION_SET_PARENT(axis_operation, env, svc);
    return AXIS2_WSDL_INTERFACE_SET_OPERATION(axis2_svc_get_svc_interface(svc, env),
        env, axis2_operation);
}

struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_with_qname (axis2_svc_t *svc,
                                        axis2_env_t **env,
		                                axis2_qname_t *operation_name)
{
    struct axis2_operation *opeartion_l = NULL;
    axis2_char_t *op_str = NULL;
    /*axis2_hash_t *all_operations = NULL; */
    
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, operation_name, NULL);
    
    op_str = AXIS2_QNAME_GET_LOCALPART(operation_name, env);
    /* commented until AXIS2_WSDL_INTERFACE_GET_ALL_OPERATIONS is implemented
    all_operations = AXIS2_WSDL_INTERFACE_GET_ALL_OPERATIONS(
        axis2_svc_get_svc_interface(svc, env), env);
    opeartion_l = (struct axis2_operation) (axis2_hash_get (all_operations, 
        op_str, AXIS2_HASH_KEY_STRING));
    */
    if(NULL == operation_l )
    {
        opeartion_l = (struct axis2_operation *) (axis2_hash_get (
                AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap, op_str,
                AXIS2_HASH_KEY_STRING));
    }
    
    return opeartion_l;	
}	

struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_with_name (axis2_svc_t *svc, 
                                    axis2_env_t **env,
		                            const axis2_char_t* nc_name)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, nc_name, NULL);
    
    return (axis2_operation_t *) axis2_hash_get(AXIS2_WSDL_INTERFACE_GET_OPERATIONS(
        axis2_wsdl_get_svc_interface(svc, env), env), nc_name, AXIS2_HASH_KEY_STRING);
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_operations (axis2_svc_t *svc, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    
    return AXIS2_WSDL_INTERFACE_GET_OPERATIONS(axis2_wsdl_get_svc_interface(svc,
        env), env);
}
	
axis2_status_t AXIS2_CALL
axis2_svc_set_parent (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                axis2_svc_grp_t *svc_grp)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_grp, AXIS2_FAILURE);
    axis2_svc_impl_t *svc_impl = AXIS2_INTF_TO_IMPL(svc);
	if(svc_impl->parent)
        AXIS2_SVC_GRP_FREE(svc_impl->parent, env);
	svc_impl->parent = svc_grp;
    
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
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    
    return AXIS2_WSDL_SVC_SET_NAME(svc->wsdl_svc, env, qname);
}

axis2_qname_t * AXIS2_CALL
axis2_svc_get_name (const axis2_svc_t *svc, 
                    axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    return AXIS2_WSDL_SVC_GET_NAME(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_add_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                axis2_param_t *param)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
    struct axis2_param_container *param_container_l = NULL;
   
    if(axis2_svc_is_param_locked(svc, env, AXIS2_PARAM_GET_NAME(param, env)))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE,
            AXIS2_FAILURE);
    } else
    {
        param_container_l = (struct axis2_param_container *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->
                wsdl_component, env, PARAMETER_KEY);
        return AXIS2_PARAM_CONTAINER_ADD_PARAM(param_container_l, env, param);
    }
    return AXIS2_FAILURE;
}

axis2_param_t * AXIS2_CALL
axis2_svc_get_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                const axis2_char_t *name)
{
    struct axis2_param_container *param_container_l = NULL;
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    
    param_container_l = (struct axis2_param_container *)
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->
            wsdl_component, env, PARAMETER_KEY);
    return AXIS2_PARAM_CONTAINER_GET_PARAM(param_container_l, env, name);
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_params (axis2_svc_t *svc, 
                        axis2_env_t **env)
{
    struct axis2_param_container *param_container_l = NULL;
	AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
	
    param_container_l = (struct axis2_param_container *)
    AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->
        wsdl_component, env, PARAMETER_KEY);
    
    return AXIS2_PARAM_CONTAINER_GET_PARAMS(param_container_l, env);
}

axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked (axis2_svc_t *svc, 
                            axis2_env_t **env,
		                    const axis2_char_t *param_name)
{
    axis2_bool_t locked = AXIS2_FALSE;
    axis2_param_t *param_l = NULL;
    axis2_engine_config *engine_config_l = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);
    
   /*checking the locked value of parent */

    if (NULL != axis2_svc_get_parent(svc, env)) 
    {
        /*commented until AXIS2_SVC_GRP_GET_ENGINE_CONFIG is implemented
        engine_config_l = AXIS2_SVC_GRP_GET_ENGINE_CONFIG(
            AXIS2_SVC_GET_PARENT(svc, env), env);
        locked =  AXIS2_ENGINE_CONFIG_IS_PARAM_LOCKED(engine_config_l, env, param_name);
        */
    }
    if(AXIS2_TRUE == locked)
    {
        return AXIS_TRUE;
    } else 
    {
        param_l = axis2_svc_get_param(svc, env, param_name);
        return (NULL != param && AXIS2_PARAM_IS_LOCKED(param_l, env));
    }
}

axis2_status_t AXIS2_CALL
axis2_svc_set_svc_interface(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_wsdl_interface *svc_interface) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_interface, AXIS2_FAILURE);
    return AXIS2_WSDL_SVC_SET_SVC_INTERFACE(svc->wsdl_svc, env, svc_interface);
}

/**
 * To ebgage a module it is reuired to use this method
 *
 * @param moduleref
 * @throws AxisFault
 */
axis2_status_t AXIS2_CALL
axis2_svc_engage_module(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_module_desc * moduleref,
                            struct axis2_engine_config * axis2_config)
{
    if (moduleref == null) {
        return;
    }
    Collection collectionModule = (Collection) this.getComponentProperty(
            MODULEREF_KEY);
    for (Iterator iterator = collectionModule.iterator();
         iterator.hasNext();) {
        struct axis2_module_desc * modu = (struct axis2_module_desc *) iterator.next();
        if (modu.getname().equals(moduleref.getname())) {
            throw new AxisFault(moduleref.getname().getLocalPart() +
                    " module has alredy been engaged on the service. " +
                    " Operation terminated !!!");
        }

    }
    new PhaseResolver(axis2_config).engageModuleTosvc(this, moduleref);
    collectionModule.add(moduleref);
}

/**
 * To add a opeartion to a service if a module requird to do so
 *
 * @param module
 */

axis2_status_t AXIS2_CALL
axis2_add_module_operations(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_module_desc * module,
                            struct axis2_engine_config * axis2_config) 
{
    axis2_hash_t * map = module.getOperations();
    Collection col = map.values();
    PhaseResolver pr = new PhaseResolver(axis2_config, this);

    for (Iterator iterator = col.iterator(); iterator.hasNext();) {
        struct axis2_operation * axis2_operation = (struct axis2_operation *) iterator.next();
        ArrayList paramters = axis2_operation.getParameters();
        // Adding wsa-maping into service
        for (int j = 0; j < paramters.size(); j++) {
            Parameter parameter = (Parameter) paramters.get(j);
            if(parameter.getname().equals(Constants.WSA_ACTION)){
                this.addMapping((axis2_char_t *)parameter.getValue(),axis2_operation);
            }
        }
        pr.buildModuleOperation(axis2_operation);
        this.addOperation(axis2_operation);
    }
}

axis2_status_t AXIS2_CALL
axis2_add_to_engage_module_list(axis2_svc_t *svc,
                                axis2_env_t **env,
                                struct axis2_module_desc *module_name) 
{
    Collection collectionModule = (Collection) this.getComponentProperty(
            MODULEREF_KEY);
    for (Iterator iterator = collectionModule.iterator();
         iterator.hasNext();) {
        struct axis2_module_desc * moduleDescription = (struct axis2_module_desc *) iterator.next();
        if (module_name.getname().equals(moduleDescription.getname())) {
            return;
        }
    }
    collectionModule.add(module_name);
}

/**
 * Method getEngadgedModules
 *
 * @return Collection
 */
Collection AXIS2_CALL
axis2_svc_get_engaged_modules(axis2_svc_t *svc,
                            axis2_env_t **envs) 
{
    return (Collection) this.getComponentProperty(MODULEREF_KEY);
}

/**
 * To get the WSDL opeartion element in servic einterface
 * @param operationName  <code>QName</cde>
 * @return  WSDLOperation <code>WSDLOperation</code>
 */
WSDLOperation AXIS2_CALL
axis2_svc_get_wsdl_operation(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_qname_t *operation_name)
{
    axis2_char_t * opStr = operationname.getLocalPart();
    return this.getsvcInterface().getOperation(opStr) ;
}

   /**
 * Method setContextPath
 *
 * @param contextPath
 */
axis2_status_t AXIS2_CALL
axis2_svc_set_context_path(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *context_path) 
{
    if (context_path != null) {
        this.setComponentProperty(CONTEXTPATH_KEY, contextPath);
    }
}

/**
 * Method getContextPath
 *
 * @return  context path
 */
axis2_char_t * AXIS2_CALL
axis2_svc_get_context_path() 
{
    return (axis2_char_t *) this.getComponentProperty(CONTEXTPATH_KEY);
}

/**
 * Method setStyle
 *
 * @param style
 */
axis2_status_t AXIS2_CALL
axis2_svc_set_style(axis2_svc_t *svc,
                        axis2_env_t **env,
                        axis2_char_t * style) 
{
    if (style != null) {
        this.setComponentProperty(STYLE_KEY, style);
    }
}

/**
 * Method getStyle
 *
 * @return axis2_char_t *
 */
axis2_char_t * AXIS2_CALL
axis2_svc_get_style() 
{
    return (axis2_char_t *) this.getComponentProperty(STYLE_KEY);
}

/**
 * Method getInFlow
 *
 * @return struct axis2_flow *
 */
struct axis2_flow * AXIS2_CALL
axis2_svc_get_in_flow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return (struct axis2_flow *) this.getComponentProperty(INFLOW_KEY);
}

/**
 * Method setInFlow
 *
 * @param inFlow
 */
axis2_status_t AXIS2_CALL
axis2_svc_set_in_flow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *inflow) 
{
    if (inflow != null) {
        this.setComponentProperty(INFLOW_KEY, inflow);
    }
}

/**
 * Method getOutFlow
 *
 * @return struct axis2_flow *
 */
struct axis2_flow * AXIS2_CALL
axis2_svc_get_out_flow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return (struct axis2_flow *) this.getComponentProperty(OUTFLOW_KEY);
}

/**
 * Method setOutFlow
 *
 * @param outFlow
 */
axis2_status_t AXIS2_CALL
axis2_svc_set_outflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *outflow) 
{
    if (outflow != null) {
        this.setComponentProperty(OUTFLOW_KEY, outflow);
    }
}

/**
 * Method getFaultInFlow
 *
 * @return struct axis2_flow *
 */
struct axis2_flow *AXIS2_CALL
axis2_svc_get_fault_inflow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return (struct axis2_flow *) this.getComponentProperty(IN_FAULTFLOW_KEY);
}

/**
 * Method setFaultInFlow
 *
 * @param fault_flow
 */
axis2_status_t AXIS2_CALL
axis2_svc_set_fault_inflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *fault_flow) 
{
    if (fault_flow != null) {
        this.setComponentProperty(IN_FAULTFLOW_KEY, fault_flow);
    }
}

struct axis2_flow * AXIS2_CALL
axis2_svc_get_faultout_flow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return (struct axis2_flow *) this.getComponentProperty(OUT_FAULTFLOW_KEY);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_faultOut_flow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *fault_flow) 
{
    if (fault_flow != null) {
        this.setComponentProperty(OUT_FAULTFLOW_KEY, fault_flow);
    }
}

/**
 * This method will return the operation given particular SOAP Action.
 * This method should only be called if there is only one Endpoint is defined
 * for this Service. If more than one Endpoint exists one of them will be picked.
 * If more than one Operation is found with the given
 * SOAP Action; null will be ruturned. If no particular Operation is found with
 * the given SOAP Action; null will be returned.
 *
 * @param soapAction SOAP Action defined for the particular Operation
 * @return A struct axis2_operation * if a unque Operation can be found with the given SOAP Action
 *         otherwise will return null.
 */
struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_by_soap_action(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t * soap_action) 
{
    if(soapAction == null || soapAction.equals("")){
        return null;
    }
    Iterator iterator = this.getendpoints().keySet().iterator();
    if (iterator.hasNext()) {
        struct axis2_wsdl_endpoint * endpoint = (struct axis2_wsdl_endpoint *) this.getendpoints().get(
                iterator.next());
        return this.getOperationBySOAPAction(soapAction,
                endpoint.getname());
    }

    return null;


}


/**
 * This method will return the operation given the particular endpoing and the
 * particular SOAP Action. If more than one Operation is found with the given
 * SOAP Action; null will be ruturned. If no particular Operation is found with
 * the given SOAP Action; null will be returned
 *
 * @param endpoint   Particular Enpoint in which the bining is defined with the particular SOAP
 *                   Action.
 * @param soapAction SOAP Action defined for the particular Operation
 * @return A struct axis2_operation * if a unque Operation can be found with the given SOAP Action
 *         otherwise will return null.
 */
struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_by_soap_action(axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        axis2_char_t *soap_action,
                                        axis2_qname_t * endpoint) 
{
    axis2_hash_t * binding_operations = this.getendpoint(endpoint).getBinding()
            .getBindingOperations();
    Iterator operationKeySetIterator = bindingOperations.keySet().iterator();
    struct axis2_operation * axis2_operation = null;
    int count = 0;
    while (operationKeySetIterator.hasNext()) {
        wsdlBindingOperation bindingOperation = (wsdlBindingOperation) bindingOperations.get(
                operationKeySetIterator.next());
        Iterator extIterator = bindingOperation.getExtensibilityElements()
                .iterator();
        while (extIterator.hasNext()) {
            wsdlExtensibilityElement element = (wsdlExtensibilityElement) extIterator.next();
            if (ExtensionConstants.SOAP_11_OPERATION.equals(element.getType())||
                    ExtensionConstants.SOAP_12_OPERATION.equals(element.getType())) {
                if (((SOAPOperation) element).getSoapAction().equals(
                        soapAction)) {
                    struct axis2_wsdl_operation *op = bindingOperation.getOperation();
                    if (op instanceof AxisOperation) {
                        axis2_operation = (struct axis2_operation *) op;
                        count++;
                    }
                }
            }
        }
    }
    if (1 == count) {
        return axis2_operation;
    }
    return NULL;
}

 /**
 * To get the description about the service
 *                                                                  
 * @return axis2_char_t *
 */
axis2_char_t * AXIS2_CALL
axis2_svc_get_axis2_svc_name(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(svc)->axis2_svc_name;
}

/**
 * Set the description about the service
 *
 * @param axissvcname
 */
axis2_status_t AXIS2_CALL
axis2_svc_set_axis2_svc_name(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *axis2_svc_name) 
{
    axis2_svc_impl_t *svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if(svc_impl->axis2_svc_name)
        AXIS2_FREE((*env)->allocator, svc_impl->axis2_svc_name);
    axis2_svc_name->axis2_svc_name = axis2_svc_name;
    return AXIS2_SUCCESS;
}

    /**
 * This method will set the current time as last update time of the service
 */
axis2_status_t AXIS2_CALL
axis2_svc_set_last_update(axis2_svc_t *svc,
                            axis2_env_t **env)
{
    /*last_update = new Date().getTime(); */
    return AXIS2_SUCCESS;
}

long axis2_svc_get_last_update(axis2_svc_t *svc,
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
    axis2_svc_impl_t *svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if(svc_impl->filename)
        AXIS2_FREE((*env)->allocator, svc_impl->filename);
    AXIS2_INTF_TO_IMPL(svc)->filename = filename;
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
                            struct axis2_wsdl_endpoint * endpoint)
{
    return AXIS2_WSDL_SVC_SET_ENDPOINT(svc->wsdl_svc, env, endpoint);
}

struct axis2_wsdl_endpoint * AXIS2_CALL
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

/**
 * To add the was action paramater into has map so that was action based dispatch can support
 */
axis2_status_t AXIS2_CALL
axis2_svc_add_mapping(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t * mapping_key , 
                            struct axis2_operation * axis2_operation)
{
    axis2_svc_impl_t *svc_impl = AXIS2_INTF_TO_IMPL(svc);
    
    axis2_hash_set(svc_impl->wasaction_opeartionmap, mapping_key, 
        AXIS2_HASH_KEY_STRING, axis2_operation);
    return AXIS2_SUCCESS;
}
