#include <axis2_wsdl_component.h>
#include <string.h>

/** 
 * @brief Wsdl Component struct impl
 *	Axis2 Wsdl Component impl  
 */
typedef struct axis2_wsdl_component_impl
{
	axis2_wsdl_component_t wsdl_component;
	axis2_hash_t *component_properties;
    /**
     * Field Namespace Qualified elements that can be sticked in the component.
     */
    axis2_linked_list_t *elements;
    
    /**
     * Field Namespace Qualified attrebutes that can be sticked int eh
     * component.
     */
    axis2_linked_list_t *attributes;
    
} axis2_wsdl_component_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_component) \
        ((axis2_wsdl_component_impl_t *)(wsdl_component))

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
                                        void *key, 
                                        void *value);
	
void * AXIS2_CALL
axis2_wsdl_component_get_component_property(
		                                axis2_wsdl_component_t *wsdl_component,
                                        axis2_env_t **env,
                                        void *key);


                                        
axis2_status_t AXIS2_CALL
axis2_wsdl_component_add_extensibility_element(axis2_wsdl_component_t *wsdl_component,
                                                axis2_env_t **env,
                                                void *element);

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_component_get_extensibility_elements(axis2_wsdl_component_t *wsdl_component,
                                                axis2_env_t **env);
                                                
/***************************** End of function headers ************************/

axis2_wsdl_component_t * AXIS2_CALL 
axis2_wsdl_component_create (axis2_env_t **env)
{
    axis2_wsdl_component_impl_t *wsdl_component_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
	wsdl_component_impl = (axis2_wsdl_component_impl_t *) AXIS2_MALLOC ((*env)->
        allocator, sizeof(axis2_wsdl_component_impl_t));
    
	if(NULL == wsdl_component_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    wsdl_component_impl->component_properties = NULL;
    wsdl_component_impl->elements = NULL;
    wsdl_component_impl->attributes = NULL;
	
    wsdl_component_impl->component_properties = axis2_hash_make (env);
    if(NULL == wsdl_component_impl->component_properties)
    {
        axis2_wsdl_component_free(&(wsdl_component_impl->wsdl_component), env);
		AXIS2_ERROR_SET ((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
	wsdl_component_impl->wsdl_component.ops = (axis2_wsdl_component_ops_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_component_ops_t));
    
	if(NULL == wsdl_component_impl->wsdl_component.ops)
	{
		axis2_wsdl_component_free(&(wsdl_component_impl->wsdl_component), env);
		AXIS2_ERROR_SET ((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);	
        return NULL;
	}
    
	wsdl_component_impl->wsdl_component.ops->free = axis2_wsdl_component_free;
    
	wsdl_component_impl->wsdl_component.ops->set_component_properties =
        axis2_wsdl_component_set_component_properties;
    
    wsdl_component_impl->wsdl_component.ops->get_component_properties =
        axis2_wsdl_component_get_component_properties;
    
    wsdl_component_impl->wsdl_component.ops->set_component_property =
        axis2_wsdl_component_set_component_property;
    
    wsdl_component_impl->wsdl_component.ops->get_component_property =
        axis2_wsdl_component_get_component_property;
    
    wsdl_component_impl->wsdl_component.ops->add_extensibility_element = 
            axis2_wsdl_component_add_extensibility_element;
    
    wsdl_component_impl->wsdl_component.ops->get_extensibility_elements = 
            axis2_wsdl_component_get_extensibility_elements;
	
	return &(wsdl_component_impl->wsdl_component);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_component_free (axis2_wsdl_component_t *wsdl_component, 
                            axis2_env_t **env)
{
    axis2_wsdl_component_impl_t *component_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_component, env, AXIS2_FAILURE);
    
    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);
    
	if(NULL != wsdl_component->ops)
    {
        AXIS2_FREE((*env)->allocator, wsdl_component->ops);
        wsdl_component->ops = NULL;
    }
    
    if(NULL != component_impl->component_properties)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        void *key = NULL;
        for (hi = axis2_hash_first (component_impl->component_properties, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            axis2_param_container_t *param_container = NULL;
            axis2_array_list_t *module_desc_list = NULL;
             
            axis2_hash_this (hi, &key, NULL, &val);
            
            if(0 == AXIS2_STRCMP(PARAMETER_KEY, (axis2_char_t *)key))
            {
                param_container = (axis2_param_container_t *) val;
                AXIS2_PARAM_CONTAINER_FREE(param_container, env);
                param_container = NULL;
            }
            
            if(0 == AXIS2_STRCMP(MODULEREF_KEY, (axis2_char_t *)key))
            {
                void *val = NULL;
                int i = 0;
                int size = 0;
                module_desc_list = (axis2_array_list_t *) val;
                if(module_desc_list)
                {
                    size = AXIS2_ARRAY_LIST_SIZE(module_desc_list, env);
                    for (i = 0; i < size; i++)
                    {
                        struct axis2_module_desc *module_desc = NULL;
                        module_desc = AXIS2_ARRAY_LIST_GET(module_desc_list, env, i);
                        
                        module_desc = (struct axis2_module_desc *) val;
                        if (module_desc)
                           AXIS2_MODULE_DESC_FREE (module_desc, env);
                        
                        val = NULL;
                        module_desc = NULL;
                           
                    }
                    AXIS2_ARRAY_LIST_FREE(module_desc_list, env);
                }
                
                module_desc_list = NULL;
            }
            
            val = NULL;
            key = NULL;
               
        }
        axis2_hash_free(component_impl->component_properties, env);
        component_impl->component_properties = NULL;
    }
    
    if(NULL != component_impl->attributes)
    {
        AXIS2_LINKED_LIST_FREE(component_impl->attributes, env);
        component_impl->attributes = NULL;
    }
    
    if(NULL != component_impl->elements)
    {
        AXIS2_LINKED_LIST_FREE(component_impl->elements, env);
        component_impl->elements = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, component_impl);
    component_impl = NULL;
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_properties(
		                                axis2_wsdl_component_t *wsdl_component, 
                                        axis2_env_t **env,
                                        axis2_hash_t *properties) 
{
    axis2_wsdl_component_impl_t *component_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_component, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, properties, AXIS2_FAILURE);
    
    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);
    
    if(component_impl->component_properties)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        void *key = NULL;
        for (hi = axis2_hash_first (component_impl->component_properties, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            axis2_param_container_t *param_container = NULL;
            axis2_array_list_t *module_desc_list = NULL;
             
            axis2_hash_this (hi, &key, NULL, &val);
            
            if(0 == AXIS2_STRCMP(PARAMETER_KEY, (axis2_char_t *)key))
            {
                param_container = (axis2_param_container_t *) val;
                AXIS2_PARAM_CONTAINER_FREE(param_container, env);
                param_container = NULL;
            }
            
            if(0 == AXIS2_STRCMP(MODULEREF_KEY, (axis2_char_t *)key))
            {
                void *val = NULL;
                int i = 0;
                int size = 0;
                module_desc_list = (axis2_array_list_t *) val;
                if(module_desc_list)
                {
                    size = AXIS2_ARRAY_LIST_SIZE(module_desc_list, env);
                    for (i = 0; i < size; i++)
                    {
                        struct axis2_module_desc *module_desc = NULL;
                        module_desc = AXIS2_ARRAY_LIST_GET(module_desc_list, env, i);
                        
                        module_desc = (struct axis2_module_desc *) val;
                        if (module_desc)
                           AXIS2_MODULE_DESC_FREE (module_desc, env);
                        
                        val = NULL;
                        module_desc = NULL;
                           
                    }
                    AXIS2_ARRAY_LIST_FREE(module_desc_list, env);
                }
                
                module_desc_list = NULL;
            }
            
            val = NULL;
            key = NULL;
               
        }
        axis2_hash_free(component_impl->component_properties, env);
        component_impl->component_properties = NULL;
    }
	component_impl->component_properties = properties;
	return AXIS2_SUCCESS;
}

axis2_hash_t * AXIS2_CALL
axis2_wsdl_component_get_component_properties(
                                        axis2_wsdl_component_t *wsdl_component,
                                        axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(wsdl_component, env, NULL);
	return AXIS2_INTF_TO_IMPL(wsdl_component)->component_properties;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_component_set_component_property(
                                        axis2_wsdl_component_t *wsdl_component, 
                                        axis2_env_t **env,
                                        void *key, 
                                        void *value) 
{
    axis2_wsdl_component_impl_t *component_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_component, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    
    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);
    
	axis2_hash_set (component_impl->component_properties, key, sizeof(key), value);
    
	return AXIS2_SUCCESS;
}

void * AXIS2_CALL
axis2_wsdl_component_get_component_property(
		                                axis2_wsdl_component_t *wsdl_component,
                                        axis2_env_t **env, 
                                        void *key) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_component, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
	
	return (void *)(axis2_hash_get (AXIS2_INTF_TO_IMPL(wsdl_component)->
        component_properties, key, sizeof(key)));
}

axis2_status_t AXIS2_CALL
axis2_wsdl_component_add_extensibility_element(axis2_wsdl_component_t *wsdl_component,
                                                axis2_env_t **env,
                                                void *element) 
{
    axis2_wsdl_component_impl_t *component_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_component, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, element, AXIS2_FAILURE);
    
    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);
    
    if (NULL == component_impl->elements) 
    {
        component_impl->elements = axis2_linked_list_create(env);
    }
    
    return AXIS2_LINKED_LIST_ADD(component_impl->elements, env, element);
}

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_component_get_extensibility_elements(axis2_wsdl_component_t *wsdl_component,
                                                axis2_env_t **env) 
{
    axis2_wsdl_component_impl_t *component_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_component, env, NULL);
    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);
    
    return component_impl->elements;
}

/**
 * Adds the <code>ExtensibilityAttribute</code> as a attrebute of this
 * Component.
 *
 * @param attribute <code>ExtensibilityAttribute</code>
 */
/*axis2_status_t AXIS2_CALL
axis2_wsdl_component_add_extensible_attributes(WSDLExtensibilityAttribute attribute) {
    if (NULL == this.attributes) {
        this.attributes = new LinkedList();
    }
    if (null == attribute)
        return;
    this.attributes.add(attribute);
}
*/

/**
 * Returns a <code>List</code> of ExtensibleAttributes of this component.
 *
 * @return <code>List</code>
 */
/*axis2_array_list_t *AXIS2_CALL
axis2_wsdl_component_get_extensibility_attributes(axis2_wsdl_component_t *wsdl_component,
                                                    axis2_env_t **env) 
{
    axis2_wsdl_component_impl_t *component_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_component, env, NULL);
    component_impl = AXIS2_INTF_TO_IMPL(wsdl_component);
    
    if (NULL == component_impl->attributes) 
    {
        component_impl->attributes = axis2_linked_list_create(env);
    }
    return component_impl->attributes;
}
*/
