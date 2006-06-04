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
 
#include <axis2_wsdl_types.h>
#include <axis2_wsdl_extensible_element.h>
#include <axis2_wsdl_extensible_component.h>

/** 
 * @brief Wsdl Types struct impl
 *   Axis2 Wsdl Types Implementation 
 */ 
typedef struct axis2_wsdl_types_impl
{
   axis2_wsdl_types_t wsdl_types;
    
} axis2_wsdl_types_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_types) ((axis2_wsdl_types_impl_t *)wsdl_types)
   
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_types_free (axis2_wsdl_types_t *wsdl_types, 
                        const axis2_env_t *env);   

/**
 * Adds the <code>ExtensionElement</code> to the map keyed with the 
 * <code>axis2_qname_t</code>
 *
 * @param element
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_types_add_element(axis2_wsdl_types_t *wsdl_types,
                                const axis2_env_t *env,
                                axis2_wsdl_extensible_element_t *element);


/**
 * Will return the first Element with the given <code>QName</code>
 * Returns null if not found.
 *
 * @param qname
 * @return
 */
axis2_wsdl_extensible_element_t *AXIS2_CALL
axis2_wsdl_types_get_first_element(axis2_wsdl_types_t *wsdl_types,
                                    const axis2_env_t *env,
                                    axis2_qname_t *qname);

/************************* End of function headers ****************************/   

axis2_wsdl_types_t * AXIS2_CALL
axis2_wsdl_types_create (const axis2_env_t *env)
{
    axis2_wsdl_types_impl_t *wsdl_types_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
   wsdl_types_impl = (axis2_wsdl_types_impl_t *) AXIS2_MALLOC (env->
        allocator, sizeof (axis2_wsdl_types_impl_t));
     
   if(NULL == wsdl_types_impl)
   {
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
   }
    
    wsdl_types_impl->wsdl_types.ops = NULL;
    
    wsdl_types_impl->wsdl_types.ext_component = 
        axis2_wsdl_extensible_component_create(env);
    
   if(NULL == wsdl_types_impl->wsdl_types.ext_component)
   {
        axis2_wsdl_types_free(&(wsdl_types_impl->wsdl_types), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
   }    
    
    wsdl_types_impl->wsdl_types.ops = AXIS2_MALLOC(env->allocator, 
        sizeof(axis2_wsdl_types_ops_t));
   if(NULL == wsdl_types_impl->wsdl_types.ops)
   {
        axis2_wsdl_types_free(&(wsdl_types_impl->wsdl_types), env);
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
   }
   
   wsdl_types_impl->wsdl_types.ops->free = axis2_wsdl_types_free;
    wsdl_types_impl->wsdl_types.ops->add_element = axis2_wsdl_types_add_element;
    wsdl_types_impl->wsdl_types.ops->get_first_element = 
            axis2_wsdl_types_get_first_element;
   return &(wsdl_types_impl->wsdl_types);
}

/*************************Function implementations*****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_types_free (
                        axis2_wsdl_types_t *wsdl_types, 
                        const axis2_env_t *env)
{
    axis2_wsdl_types_impl_t *wsdl_types_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE); 
    wsdl_types_impl = AXIS2_INTF_TO_IMPL(wsdl_types);
    
   if(wsdl_types->ops)
    {
        AXIS2_FREE(env->allocator, wsdl_types->ops);
        wsdl_types->ops = NULL;
    }
    
    if(wsdl_types->ext_component)
    {
        AXIS2_WSDL_COMPONENT_FREE(wsdl_types->ext_component, env);
        wsdl_types->ext_component = NULL;
    }
    
    if(wsdl_types_impl)
        AXIS2_FREE(env->allocator, wsdl_types_impl);
    wsdl_types_impl = NULL;
    
   return AXIS2_SUCCESS;
}


/**
 * Adds the <code>ExtensionElement</code> to the map keyed with the 
 * <code>axis2_qname_t</code>
 *
 * @param element
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_types_add_element(axis2_wsdl_types_t *wsdl_types,
                                const axis2_env_t *env,
                                axis2_wsdl_extensible_element_t *element)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element, AXIS2_FAILURE); 

    return AXIS2_WSDL_COMPONENT_ADD_EXTENSIBILITY_ELEMENT(wsdl_types->
        ext_component->wsdl_component, env, element);
}

/**
 * Will return the first Element with the given <code>QName</code>
 * Returns null if not found.
 *
 * @param qname
 * @return
 */
axis2_wsdl_extensible_element_t *AXIS2_CALL
axis2_wsdl_types_get_first_element(axis2_wsdl_types_t *wsdl_types,
                                    const axis2_env_t *env,
                                    axis2_qname_t *qname)
{
    axis2_linked_list_t *ext_elements = NULL;
    int i = 0;
    int size = 0;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);
    
    ext_elements = AXIS2_WSDL_COMPONENT_GET_EXTENSIBILITY_ELEMENTS(wsdl_types->
        ext_component->wsdl_component, env);
    size = AXIS2_LINKED_LIST_SIZE(ext_elements, env);
    for(i = 0; i < size; i++)
    {
        axis2_wsdl_extensible_element_t *temp = NULL;
        axis2_qname_t *qname_l = NULL;
        temp = AXIS2_LINKED_LIST_GET(ext_elements, env, i);
        qname_l = AXIS2_WSDL_EXTENSIBLE_ELEMENT_GET_TYPE(temp, env);
        if(AXIS2_QNAME_EQUALS(qname, env, qname_l))
        {
            return temp;
        }
    }
    return NULL;;
}
