#include <axis2_wsdl_component.h>
#include <string.h>

typedef struct  axis2_wsdl_component_impl_s axis2_wsdl_component_impl_t;

/** 
 * @brief Wsdl Component struct impl
 *	Axis2 Wsdl Component impl  
 */
struct axis2_wsdl_component_impl_s
{
	axis2_wsdl_component_t wsdl_component;
	axis2_hash_t *component_properties;
};

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_component_free (axis2_wsdl_component_t *wsdl_component,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_properties(
                                        axis2_wsdl_component_t *wsdl_component,
                                        axis2_env_t **env,
		                                axis2_hash_t *properties);

axis2_hash_t * AXIS2_CALL
axis2_wsdl_component_get_component_properties(
                                        axis2_wsdl_component_t *wsdl_component,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_property(
	                                    axis2_wsdl_component_t *wsdl_component, 
                                        axis2_env_t **env,
                                        const void *key, 
                                        void *value);
	
void * AXIS2_CALL
axis2_wsdl_component_get_component_property(
		                                axis2_wsdl_component_t *wsdl_component,
                                        axis2_env_t **env,
                                        const void *key);
		
/***************************** End of function headers ************************/

axis2_wsdl_component_t * AXIS2_CALL 
axis2_wsdl_component_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
	axis2_wsdl_component_impl_t *wsdl_impl_component = 
        (axis2_wsdl_component_impl_t *) AXIS2_MALLOC (env->allocator, 
        sizeof(axis2_wsdl_component_impl_t));
    
	if(NULL == wsdl_impl_component)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
	
	wsdl_impl_component->wsdl_component.ops = (axis2_wsdl_component_ops_t *)
		AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_component_ops_t));
	if(NULL == wsdl_impl_component->wsdl_component.ops)
	{
		AXIS2_FREE ((*env)->allocator, wsdl_impl_component);
		AXIS2_ERROR_SET ((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);	
	}
    
	wsdl_impl_component->wsdl_component.ops->free = axis2_wsdl_component_free;
	
	wsdl_impl_component->component_properties = axis2_hash_make (env);
	
	return &(wsdl_impl_component->wsdl_component);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_component_free (axis2_wsdl_component_t *wsdl_component, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_component, env, AXIS2_FAILURE);
	if(NULL != wsdl_component->ops)
        AXIS2_FREE((*env)->allocator, wsdl_component->ops);
    
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_component)->component_properties)
        axis2_hash_free(AXIS2_INTF_TO_IMPL(wsdl_component)->
            component_properties, env);
    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(wsdl_component));
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_properties(
		                                axis2_wsdl_component_t *wsdl_component, 
                                        axis2_env_t **env,
                                        axis2_hash_t *properties) 
{
	if(NULL == wsdl_component || NULL == properties) 
		return AXIS2_ERROR_INVALID_NULL_PARAM;
	
    AXIS2_FUNC_PARAM_CHECK(wsdl_component, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, properties, AXIS2_FAILURE);
    
	AXIS2_INTF_TO_IMPL(wsdl_component)->component_properties =
        AXIS2_MALLOC((*env)->allocator, sizeof(axis2_wsdl_component_t));
    
	memcpy(wsdl_impl_component->component_properties, properties
		, sizeof(properties));
	return AXIS2_SUCCESS;
}

axis2_hash_t * AXIS2_CALL
axis2_wsdl_component_get_component_properties(
                                        axis2_wsdl_component_t *wsdl_component,
                                        axis2_env_t **env)
{
	if(NULL == wsdl_component) return AXIS2_ERROR_INVALID_NULL_PARAM;
	axis2_wsdl_component_impl_t *wsdl_impl_component 
		= (axis2_wsdl_component_impl_t*)(wsdl_component);
	*properties = wsdl_impl_component->component_properties;
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_property(
                                        axis2_wsdl_component_t *wsdl_component, 
                                        axis2_env_t **env,
                                        const void *key, 
                                        void *value) 
{
	if(NULL ==wsdl_component || NULL == key || NULL == value) 
		return AXIS2_ERROR_INVALID_NULL_PARAM;
    axis2_wsdl_component_impl_t *wsdl_impl_component
		= (axis2_wsdl_component_impl_t *) wsdl_component;
	axis2_hash_set (wsdl_impl_component->component_properties		
		, key
		, sizeof(key), value);
	return AXIS2_SUCCESS;
}

void * AXIS2_CALL
axis2_wsdl_component_get_component_property(
		                                axis2_wsdl_component_t *wsdl_component,
                                        axis2_env_t **env, 
                                        const void *key) 
{
	if(NULL == wsdl_component || NULL == key) 
		return AXIS2_ERROR_INVALID_NULL_PARAM;
		
	axis2_wsdl_component_impl_t *wsdl_impl_component
		= (axis2_wsdl_component_impl_t *) wsdl_component;
	
	*value = (void *)(axis2_hash_get (wsdl_impl_component->component_properties
		, key, sizeof(key)));
	
	return AXIS2_SUCCESS;
}
