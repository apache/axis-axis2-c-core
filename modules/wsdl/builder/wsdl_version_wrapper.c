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
 
#include <axis2_wom_builder_version_wrapper.h>

/** 
 * @brief Wsdl1 To Wom Builder struct impl
 *	Axis2 Wsdl1 To Wom Builder impl  
 */
typedef struct axis2_version_wrapper_impl
{
	axis2_version_wrapper_t version_wrapper;
    private Definition definition;
    axis2_wsdl_desc_t *desc;

  
} axis2_version_wrapper_impl_t;

#define AXIS2_INTF_TO_IMPL(version_wrapper) ((axis2_version_wrapper_impl_t *)(version_wrapper))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_version_wrapper_free (axis2_version_wrapper_t *version_wrapper, axis2_env_t **env);



/************************** End of Function headers ************************/

AXIS2_DECLARE(axis2_version_wrapper_t *)
axis2_version_wrapper_create (axis2_env_t **env)
{
    axis2_version_wrapper_impl_t *version_wrapper_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	version_wrapper_impl = (axis2_version_wrapper_impl_t *) AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_version_wrapper_impl_t));
		
	if(NULL == version_wrapper_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
	
    version_wrapper_impl->simple_typetoxsd = NULL;
    version_wrapper_impl->complex_type_map = NULL;
    version_wrapper_impl->version_wrapper.ops = NULL;
    
	version_wrapper_impl->version_wrapper.ops = AXIS2_MALLOC ((*env)->allocator, 
        sizeof(axis2_version_wrapper_ops_t));
	if(NULL == version_wrapper_impl->version_wrapper.ops)
    {
        axis2_version_wrapper_free(&(version_wrapper_impl->version_wrapper), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	version_wrapper.ops->free =  axis2_version_wrapper_free;
    version_wrapper.ops->add_handler =  axis2_version_wrapper_add_handler;
    version_wrapper.ops->get_handler =  axis2_version_wrapper_get_handler;
    version_wrapper.ops->get_handler_count =  axis2_version_wrapper_get_handler_count;
    	
	return &(version_wrapper_impl->version_wrapper);
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL
axis2_version_wrapper_free (axis2_version_wrapper_t *version_wrapper, axis2_env_t **env)
{
    axis2_version_wrapper_impl_t *version_wrapper_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    version_wrapper_impl = AXIS2_INTF_TO_IMPL(version_wrapper);
    
    if(NULL != version_wrapper->ops)
    {
        AXIS2_FREE((*env)->allocator, version_wrapper->ops);
        version_wrapper->ops = NULL;
    }
    
    if(version_wrapper_impl)
    {
        AXIS2_FREE((*env)->allocator, version_wrapper_impl);
        version_wrapper_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

public class WSDLVersionWrapper {

    private Definition definition;

    private WSDLDescription description;


    /**
     * @param description WSDL 2.0 WOM description
     * @param definition  WSDL 1.1 WSDL4J based <code>Definition</code>
     */
    public WSDLVersionWrapper(WSDLDescription description,
                              Definition definition) {
        this.definition = definition;
        this.description = description;
    }

    /**
     * Returns the WSDL 1.1 Definition.
     *
     * @return Returns <code>Definition</code>
     */
    public Definition getDefinition() {
        return definition;
    }

    /**
     * Returns a WOM description.
     *
     * @return Returns <code>WSDLDescription</code>
     */
    public WSDLDescription getDescription() {
        return description;
    }
}
