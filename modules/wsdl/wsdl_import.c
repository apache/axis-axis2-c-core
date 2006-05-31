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
 
#include <axis2_wsdl_import.h>

/** 
 * @brief Wsdl import struct impl
 *	Wsdl imports  
 */ 
typedef struct axis2_wsdl_import_impl
{
    axis2_wsdl_import_t wsdl_import;    
    axis2_char_t *location;
    axis2_char_t *namespace;

} axis2_wsdl_import_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_import) \
		((axis2_wsdl_import_impl_t *)wsdl_import)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_import_free (axis2_wsdl_import_t *wsdl_import,
									const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_wsdl_import_get_namespace(axis2_wsdl_import_t *wsdl_import,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_import_set_namespace(axis2_wsdl_import_t *wsdl_import,
                                const axis2_env_t *env,
                                axis2_char_t *namespace);

axis2_char_t *AXIS2_CALL
axis2_wsdl_import_get_location(axis2_wsdl_import_t *wsdl_import,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_import_set_location(axis2_wsdl_import_t *wsdl_import,
                                const axis2_env_t *env,
                                axis2_char_t *location);
                                
/************************** End of function prototypes ************************/

axis2_wsdl_import_t * AXIS2_CALL 
axis2_wsdl_import_create (const axis2_env_t *env)
{
    axis2_wsdl_import_impl_t *wsdl_import_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    wsdl_import_impl = (axis2_wsdl_import_impl_t *) AXIS2_MALLOC(env->allocator,
        sizeof(axis2_wsdl_import_impl_t));
    if(NULL == wsdl_import_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    wsdl_import_impl->namespace = NULL;
    wsdl_import_impl->location = NULL;
    wsdl_import_impl->wsdl_import.ops = NULL;
    wsdl_import_impl->wsdl_import.wsdl_component = NULL;
	
    wsdl_import_impl->wsdl_import.wsdl_component = axis2_wsdl_component_create(env);
    if(NULL == wsdl_import_impl->wsdl_import.wsdl_component)
    {
        axis2_wsdl_import_free(&(wsdl_import_impl->wsdl_import), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }    
    
	wsdl_import_impl->wsdl_import.ops = 
		AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_import_ops_t));
	if(NULL == wsdl_import_impl->wsdl_import.ops)
    {
        axis2_wsdl_import_free(&(wsdl_import_impl->wsdl_import), env);
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	wsdl_import_impl->wsdl_import.ops->free =  axis2_wsdl_import_free;
	wsdl_import_impl->wsdl_import.ops->get_namespace = 
        axis2_wsdl_import_get_namespace;
    wsdl_import_impl->wsdl_import.ops->set_namespace = 
        axis2_wsdl_import_set_namespace;
	wsdl_import_impl->wsdl_import.ops->get_location = 
        axis2_wsdl_import_get_location;
    wsdl_import_impl->wsdl_import.ops->set_location = 
        axis2_wsdl_import_set_location;
	
	return &(wsdl_import_impl->wsdl_import);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_import_free (axis2_wsdl_import_t *wsdl_import, 
                            const axis2_env_t *env)
{
    axis2_wsdl_import_impl_t *import_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    import_impl = AXIS2_INTF_TO_IMPL(wsdl_import);
    
	if(NULL != wsdl_import->ops)
        AXIS2_FREE(env->allocator, wsdl_import->ops);
    
    if(NULL != import_impl->namespace)
    {
        AXIS2_FREE(env->allocator, import_impl->namespace);
        import_impl->namespace = NULL;
    }

    if(NULL != import_impl->location)
    {
        AXIS2_FREE(env->allocator, import_impl->location);
        import_impl->location = NULL;
    }
    
    if(NULL != wsdl_import->wsdl_component)
    {
        AXIS2_WSDL_COMPONENT_FREE(wsdl_import->wsdl_component, env);
        wsdl_import->wsdl_component = NULL;
    }
    
    AXIS2_FREE(env->allocator, import_impl);
    import_impl = NULL;
    
	return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_import_get_namespace(axis2_wsdl_import_t *wsdl_import,
                                const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_import)->namespace;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_import_set_namespace(axis2_wsdl_import_t *wsdl_import,
                                const axis2_env_t *env,
                                axis2_char_t *namespace) 
{
    axis2_wsdl_import_impl_t *import_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, namespace, AXIS2_FAILURE);
    
    import_impl = AXIS2_INTF_TO_IMPL(wsdl_import);
    
    if(import_impl->namespace)
    {
        AXIS2_FREE(env->allocator, import_impl->namespace);
        import_impl->namespace = NULL;
    }
    import_impl->namespace = AXIS2_STRDUP(namespace, env);
    if(!import_impl->namespace)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_import_get_location(axis2_wsdl_import_t *wsdl_import,
                                const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_import)->location;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_import_set_location(axis2_wsdl_import_t *wsdl_import,
                                const axis2_env_t *env,
                                axis2_char_t *location) 
{
    axis2_wsdl_import_impl_t *import_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, location, AXIS2_FAILURE);
    
    import_impl = AXIS2_INTF_TO_IMPL(wsdl_import);
    
    if(import_impl->location)
    {
        AXIS2_FREE(env->allocator, import_impl->location);
        import_impl->location = NULL;
    }
    import_impl->location = AXIS2_STRDUP(location, env);
    if(!import_impl->location)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}
