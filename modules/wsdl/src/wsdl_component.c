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
	
	axis2_wsdl_component_t *wsdl_component_local 
		= (axis2_wsdl_component_t *)
		axis2_malloc (env->allocator, sizeof(axis2_wsdl_component_t));
	if(!wsdl_component_local)
	{
		axis2_free(env->allocator, wsdl_impl_component);
		return AXIS2_ERROR_NO_MEMORY;
	}
	wsdl_component_local->ops 
		= (axis2_wsdl_component_ops_t *)
		axis2_malloc (env->allocator, sizeof(axis2_wsdl_component_ops_t));
	if(!wsdl_component_local->ops)
	{
		axis2_free(env->allocator, wsdl_impl_component);
		axis2_free(env->allocator, wsdl_component_local);
		return AXIS2_ERROR_NO_MEMORY;	
	}
	(wsdl_component_local->ops)->free = axis2_wsdl_component_free;
	
	wsdl_impl_component->wsdl_component = *wsdl_component_local;
	
	wsdl_impl_component->component_properties = axis2_hash_make (env);
	
	*wsdl_component = &(wsdl_impl_component->wsdl_component);

	return AXIS2_SUCCESS;	
}

/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_component_free (axis2_env_t *env
		, axis2_wsdl_component_t *wsdl_component)
{
	if(NULL != wsdl_component)
		axis2_free(env->allocator, wsdl_component);
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_component_get_component_properties(axis2_env_t *env
		,axis2_wsdl_component_t *wsdl_component, axis2_hash_t **properties)
{
	if(NULL == wsdl_component) return AXIS2_ERROR_INVALID_NULL_PARAM;
	axis2_wsdl_component_impl_t *wsdl_impl_component 
		= (axis2_wsdl_component_impl_t*)(wsdl_component);
	*properties = wsdl_impl_component->component_properties;
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_properties(axis2_env_t *env
		, axis2_wsdl_component_t *wsdl_component, axis2_hash_t *properties) 
{
	if(NULL == wsdl_component || NULL == properties) 
		return AXIS2_ERROR_INVALID_NULL_PARAM;
	
	axis2_wsdl_component_impl_t *wsdl_impl_component
		= (axis2_wsdl_component_impl_t *) wsdl_component;
	wsdl_impl_component->component_properties = axis2_malloc(env->allocator
		, sizeof(axis2_wsdl_component_t));
	memcpy(wsdl_impl_component->component_properties, properties
		, sizeof(properties));
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_property(axis2_env_t *env
	, axis2_wsdl_component_t *wsdl_component, const void *key, void *value) 
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
axis2_wsdl_component_get_component_property(axis2_env_t *env
		, axis2_wsdl_component_t *wsdl_component,const void *key) 
{
	if(NULL == wsdl_component || NULL == key) 
		return AXIS2_ERROR_INVALID_NULL_PARAM;
		
	axis2_wsdl_component_impl_t *wsdl_impl_component
		= (axis2_wsdl_component_impl_t *) wsdl_component;
	
	*value = (void *)(axis2_hash_get (wsdl_impl_component->component_properties
		, key, sizeof(key)));
	
	return AXIS2_SUCCESS;
}
