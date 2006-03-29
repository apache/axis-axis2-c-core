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
 
#include <axis2_wsdl_desc.h>
#include <axis2_wsdl_types.h>
#include <axis2_wsdl_component.h>
#include <axis2_wsdl_binding.h>
#include <axis2_wsdl_import.h>
#include <axis2_wsdl_include.h>
#include <axis2_wsdl_extensible_attribute.h>
#include <axis2_wsdl_binding_fault.h>


/** 
 * @brief Wsdl Description struct impl
 *	Axis2 Wsdl Description Implementation 
 */ 
typedef struct axis2_wsdl_desc_impl
{
	axis2_wsdl_desc_t wsdl_desc;
    /**
     * The name token of WSDL 1.1 Definition.
     */
    axis2_qname_t *wsdl1_def_name;
    /* TODO local name and the naspace name to be made static or through a 
     * Constant class.*/
    /* TODO required; thus check it up */
    /**
     * Field targetNameSpace
     */
    axis2_char_t *target_namespace;
    /* TODO The object structure of some external xml data binding is going to 
     *be pluged here eventually.*/  
    /**
     * Field types
     */
    axis2_wsdl_types_t *types;
    /**
     * This List will be a list of <code>WSDLInterface</code> objects.
     */
    axis2_hash_t *interfaces;
    /**
     * This <code>HashMap </code> is a Map of <code>WSDLBinding </code> objects.
     */
    axis2_hash_t *bindings;
    /**
     * This <code>HashMap </code> is a list of <code>WSDLService </code> objects.
     * Support of multiple is backed by the requirements in the specification.
     */
    axis2_hash_t *svcs;
    /**
     * WSDL imports
     */
    axis2_array_list_t *imports;
    /**
     * WSDL Includes.
     */
    axis2_array_list_t *includes;
    /**
     * Will keep a map of all the Namespaces associated with the
     * Definition Component and will be keyed by the Namespace prefix.
     */
    axis2_hash_t *namespaces;
    
} axis2_wsdl_desc_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_desc) ((axis2_wsdl_desc_impl_t *)wsdl_desc)
	
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_desc_free (axis2_wsdl_desc_t *wsdl_desc, 
                        axis2_env_t **env);	

/**
 * Returns a Map of <code>wsdl_bindings</code> Objects keyed by the 
 * <code>axis2_qname_t</code> of the Binding.
 *
 * @return
 */
axis2_hash_t *AXIS2_CALL
axis2_wsdl_desc_get_bindings(axis2_wsdl_desc_t *wsdl_desc,  
                                axis2_env_t **env);

/**
 * Sets the whole new set of Bindings to the axis2_wsd_desc.
 *
 * @param bindings
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_bindings(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_hash_t *bindings);

/**
 * The axis2_wsdl_binding Will be added to the map keyed  with its own qname.
 * If the  axis2_wsdl_binding is null it will not be added.
 * If the axis2_wsdl_binding is not null and  Binding name is null then
 * error is set.
 *
 * @param binding <code>axis2_wsdl_binding</code> Object
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_add_binding(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_wsdl_binding_t *binding);

/**
 * Retrives the <code>axis2_wsdl_binding_t</code> by its qname. Wil return NULL
 * if <code>axis2_wsdl_binding_t</code> is not found.
 *
 * @param qname The qname of the Binding.
 * @return
 */
axis2_wsdl_binding_t *AXIS2_CALL
axis2_wsdl_desc_get_binding(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_qname_t *qname);

/**
 * The Interface component will be added to the map keyed with its own name.
 * If the Interface is NULL it will not be added.
 * If the interface name is NULL an Error will be set and return.
 *
 * @param interface
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_add_interface(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_wsdl_interface_t *interface);

/**
 * The Interface will be returned if it exsists,
 * otherwise null will be returned.
 *
 * @param qname qname of the Interface.
 * @return The Interface with the relavent qname
 */
axis2_wsdl_interface_t *AXIS2_CALL
axis2_wsdl_desc_get_interface(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_qname_t *qname);

/**
 * Method get_interfaces
 *
 * @return
 */
axis2_hash_t *AXIS2_CALL
axis2_wsdl_desc_get_interfaces(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env);

/**
 * Method axis2_wsdl_desc_set_interfaces
 *
 * @param interfaces
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_interfaces(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_hash_t *interfaces);

/**
 * Method get_svcs
 *
 * @return
 */
axis2_hash_t *AXIS2_CALL
axis2_wsdl_desc_get_svcs(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env);

/**
 * Method axis2_wsdl_desc_set_svcs
 *
 * @param svcs
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_svcs(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_hash_t *svcs);

/**
 * Will return the <code>axis2_wsdl_svc </code> if found otherwise return NULL.
 *
 * @param qname <code>qname</code> of the svc
 * @return The svc with the relevant qname
 */
axis2_wsdl_svc_t *AXIS2_CALL
axis2_wsdl_desc_get_svc(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_qname_t *qname);

/**
 * Will add the <code>axis2_wsdl_svc_t</code> to the map.
 * If svc is NULL it will not be added.
 * If the <code>axis2_wsdl_svc_t</code> name is NULL an error will be set.
 *
 * @param svc
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_add_svc(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_wsdl_svc_t *svc);

/**
 * Method get_target_namespace
 *
 * @return
 */
axis2_char_t *AXIS2_CALL
axis2_wsdl_desc_get_target_namespace(axis2_wsdl_desc_t *wsdl_desc,
                                        axis2_env_t **env);

/**
 * Method set_target_namespace
 *
 * @param target_namespace
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_target_namespace(axis2_wsdl_desc_t *wsdl_desc,
                                        axis2_env_t **env,
                                        axis2_char_t *target_namespace);

/**
 * Method get_types
 *
 * @return
 */
axis2_wsdl_types_t *AXIS2_CALL
axis2_wsdl_desc_get_types(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env);

/**
 * Method axis2_wsdl_desc_set_types
 *
 * @param types
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_types(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_wsdl_types_t *types);

/**
 * Gets the name attribute of the WSDL 1.1 Definitions Element
 *
 * @return
 */
axis2_qname_t *AXIS2_CALL
axis2_wsdl_desc_get_wsdl1_def_name(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env);

/**
 * Sets the name attribute of the WSDL 1.1 Definitions Element
 *
 * @param wsdl1_def_name
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_wsdl1_def_name(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env,
                                    axis2_qname_t *wsdl1_def_name);

/**
 * Will return the Namespace URI as a String if there exists an
 * Namespace URI associated with the given prefix, in the Definition
 * Component, Will return NULL if not found.
 *
 * @param prefix Prefix defined in the Definitions elemet in the WSDL file
 * @return The Namespace URI for the prefix.
 */
axis2_char_t *AXIS2_CALL
axis2_wsdl_desc_get_namespace(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_char_t *prefix);

/**
 * Will return all the Namespaces associated with the Definition
 * Component and will be keyed by the Napespace Prefix.
 *
 * @return
 */
axis2_hash_t *AXIS2_CALL
axis2_wsdl_desc_get_namespaces(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env);

/**
 * Sets the Namespaces associated with the Difinition Component
 * and they should be keyed by its Namespace Prefix.
 *
 * @param namespaces
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_namespaces(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_hash_t *namespaces);

/**
 * Returns the WSDL Imports in an <code>axis2_array_list_t</code>
 *
 * @return
 */
axis2_array_list_t *AXIS2_CALL
axis2_wsdl_desc_get_imports(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t *env);

/**
 * Sets the imports as an <code>axis2_array_list_t</code>
 *
 * @param imports
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_imports(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_array_list_t *imports);

/**
 * Adds an import to the list.
 *
 * @param wsdl_import
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_add_import(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_wsdl_import_t *wsdl_import);

/**
 * Returns the Includes in an <code>axis2_array_list_t</code>
 *
 * @return
 */
axis2_array_list_t *AXIS2_CALL
axis2_wsdl_desc_get_includes(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t *env);

/**
 * Sets the includes as an <code>axis2_array_list_t</code>
 *
 * @param includes
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_includes(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_array_list_t *includes);

/**
 * Adds an wsdl include to the list.
 *
 * @param wsdl_include
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_add_include(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_wsdl_include_t *wsdl_include);


/************************* End of function headers ****************************/	

axis2_wsdl_desc_t * AXIS2_CALL
axis2_wsdl_desc_create (axis2_env_t **env)
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
	wsdl_desc_impl = (axis2_wsdl_desc_impl_t *) AXIS2_MALLOC ((*env)->
        allocator, sizeof (axis2_wsdl_desc_impl_t));
     
	if(NULL == wsdl_desc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    wsdl_desc_impl->wsdl1_def_name = NULL;
    wsdl_desc_impl->target_namespace = NULL;
    wsdl_desc_impl->types = NULL;
    wsdl_desc_impl->interfaces = NULL;
    wsdl_desc_impl->bindings = NULL;
    wsdl_desc_impl->svcs = NULL;
    wsdl_desc_impl->imports = NULL;
    wsdl_desc_impl->includes = NULL;
    wsdl_desc_impl->namespaces = NULL;
    wsdl_desc_impl->wsdl_desc.ops = NULL;
    
    wsdl_desc_impl->wsdl_desc.component = axis2_wsdl_component_create(env);
    
	if(NULL == wsdl_desc_impl->wsdl_desc.component)
	{
        axis2_wsdl_desc_free(&(wsdl_desc_impl->wsdl_desc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
	}    
    
    wsdl_desc_impl->wsdl_desc.ops = AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_wsdl_desc_ops_t));
	if(NULL == wsdl_desc_impl->wsdl_desc.ops)
	{
        axis2_wsdl_desc_free(&(wsdl_desc_impl->wsdl_desc), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
	
	wsdl_desc_impl->wsdl_desc.ops->free = axis2_wsdl_desc_free;
    wsdl_desc_impl->wsdl_desc.ops->get_bindings = axis2_wsdl_desc_get_bindings;
    wsdl_desc_impl->wsdl_desc.ops->set_bindings = axis2_wsdl_desc_set_bindings;
    wsdl_desc_impl->wsdl_desc.ops->add_binding = axis2_wsdl_desc_add_binding;
    wsdl_desc_impl->wsdl_desc.ops->get_binding = axis2_wsdl_desc_get_binding;
    wsdl_desc_impl->wsdl_desc.ops->add_interface = axis2_wsdl_desc_add_interface;
    wsdl_desc_impl->wsdl_desc.ops->get_interface = axis2_wsdl_desc_get_interface;
    wsdl_desc_impl->wsdl_desc.ops->get_interfaces = axis2_wsdl_desc_get_interfaces;
    wsdl_desc_impl->wsdl_desc.ops->set_interfaces = axis2_wsdl_desc_set_interfaces;
    wsdl_desc_impl->wsdl_desc.ops->get_svcs = axis2_wsdl_desc_get_svcs;
    wsdl_desc_impl->wsdl_desc.ops->set_svcs = axis2_wsdl_desc_set_svcs;
    wsdl_desc_impl->wsdl_desc.ops->get_svc = axis2_wsdl_desc_get_svc;
    wsdl_desc_impl->wsdl_desc.ops->add_svc = axis2_wsdl_desc_add_svc;
    wsdl_desc_impl->wsdl_desc.ops->get_target_namespace = 
        axis2_wsdl_desc_get_target_namespace;
    wsdl_desc_impl->wsdl_desc.ops->set_target_namespace = 
            axis2_wsdl_desc_set_target_namespace;
    wsdl_desc_impl->wsdl_desc.ops->get_types = axis2_wsdl_desc_get_types;
    wsdl_desc_impl->wsdl_desc.ops->set_types = axis2_wsdl_desc_set_types;
    wsdl_desc_impl->wsdl_desc.ops->get_wsdl1_def_name = 
            axis2_wsdl_desc_get_wsdl1_def_name;
    wsdl_desc_impl->wsdl_desc.ops->set_wsdl1_def_name = 
            axis2_wsdl_desc_set_wsdl1_def_name;
    wsdl_desc_impl->wsdl_desc.ops->get_namespace = axis2_wsdl_desc_get_namespace;
    wsdl_desc_impl->wsdl_desc.ops->get_namespaces = axis2_wsdl_desc_get_namespaces;
    wsdl_desc_impl->wsdl_desc.ops->set_namespaces = axis2_wsdl_desc_set_namespaces;
    wsdl_desc_impl->wsdl_desc.ops->get_imports = axis2_wsdl_desc_get_imports;
    wsdl_desc_impl->wsdl_desc.ops->set_imports = axis2_wsdl_desc_set_imports;
    wsdl_desc_impl->wsdl_desc.ops->add_import = axis2_wsdl_desc_add_import;
    wsdl_desc_impl->wsdl_desc.ops->get_includes = axis2_wsdl_desc_get_includes;
    wsdl_desc_impl->wsdl_desc.ops->set_includes = axis2_wsdl_desc_set_includes;
    wsdl_desc_impl->wsdl_desc.ops->add_include = axis2_wsdl_desc_add_include; 
    
	return &(wsdl_desc_impl->wsdl_desc);
}

/*************************Function implementations*****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_desc_free (
                        axis2_wsdl_desc_t *wsdl_desc, 
                        axis2_env_t **env)
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE); 
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
	if(wsdl_desc->ops)
    {
        AXIS2_FREE((*env)->allocator, wsdl_desc->ops);
        wsdl_desc->ops = NULL;
    }
    
    if(wsdl_desc->component)
    {
        AXIS2_WSDL_COMPONENT_FREE(wsdl_desc->component, env);
        wsdl_desc->component = NULL;
    }
    
    if(wsdl_desc_impl)
        AXIS2_FREE((*env)->allocator, wsdl_desc_impl);
    wsdl_desc_impl = NULL;
    
	return AXIS2_SUCCESS;
}

/**
 * Returns a Map of <code>wsdl_bindings</code> Objects keyed by the 
 * <code>axis2_qname_t</code> of the Binding.
 *
 * @return
 */
axis2_hash_t *AXIS2_CALL
axis2_wsdl_desc_get_bindings(axis2_wsdl_desc_t *wsdl_desc,  
                                axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(wsdl_desc)->bindings;
}

/**
 * Sets the whole new set of Bindings to the axis2_wsd_desc.
 *
 * @param bindings
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_bindings(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_hash_t *bindings) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
    /* TODO free previous bindings before assign new bindings */
    wsdl_desc_impl->bindings = bindings;
  
    return AXIS2_SUCCESS;
}

/**
 * The axis2_wsdl_binding Will be added to the map keyed  with its own qname.
 * If the  axis2_wsdl_binding is null it will not be added.
 * If the axis2_wsdl_binding is not null and  Binding name is null then
 * error is set.
 *
 * @param binding <code>axis2_wsdl_binding</code> Object
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_add_binding(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_wsdl_binding_t *binding) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;
    axis2_qname_t *binding_qname = NULL;
    axis2_char_t *binding_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, binding, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);

    binding_qname = AXIS2_WSDL_BINDING_GET_NAME(binding, env);
    if(NULL == binding_qname)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_WSDL_BINDING_NAME_IS_REQUIRED,
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    binding_name = AXIS2_QNAME_TO_STRING(binding_qname, env);
    if(!wsdl_desc_impl->bindings)
    {
        wsdl_desc_impl->bindings = axis2_hash_make(env);
        if(!wsdl_desc_impl->bindings)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axis2_hash_set(wsdl_desc_impl->bindings, binding_name, AXIS2_HASH_KEY_STRING,
        binding);
    return AXIS2_SUCCESS;
}

/**
 * Retrives the <code>axis2_wsdl_binding_t</code> by its qname. Wil return NULL
 * if <code>axis2_wsdl_binding_t</code> is not found.
 *
 * @param qname The qname of the Binding.
 * @return
 */
axis2_wsdl_binding_t *AXIS2_CALL
axis2_wsdl_desc_get_binding(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_qname_t *qname) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;
    axis2_char_t *name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
    name = AXIS2_QNAME_TO_STRING(qname, env);
    if(wsdl_desc_impl->bindings)
    {
        return (axis2_wsdl_binding_t *) axis2_hash_get(wsdl_desc_impl->bindings, 
            name, AXIS2_HASH_KEY_STRING);
    }
    return NULL;
}

/**
 * The Interface component will be added to the map keyed with its own name.
 * If the Interface is NULL it will not be added.
 * If the interface name is NULL an Error will be set and return.
 *
 * @param interface
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_add_interface(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_wsdl_interface_t *interface) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;
    axis2_qname_t *interface_qname = NULL;
    axis2_char_t *interface_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, interface, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);

    interface_qname = AXIS2_WSDL_INTERFACE_GET_NAME(interface, env);
    if(NULL == interface_qname)
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_WSDL_INTERFACE_NAME_IS_REQUIRED, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    interface_name = AXIS2_QNAME_TO_STRING(interface_qname, env);
    if(!wsdl_desc_impl->interfaces)
    {
        wsdl_desc_impl->interfaces = axis2_hash_make(env);
        if(!wsdl_desc_impl->interfaces)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axis2_hash_set(wsdl_desc_impl->interfaces, interface_name, 
        AXIS2_HASH_KEY_STRING, interface);
    return AXIS2_SUCCESS;
}

/**
 * The Interface will be returned if it exsists,
 * otherwise null will be returned.
 *
 * @param qname qname of the Interface.
 * @return The Interface with the relavent qname
 */
axis2_wsdl_interface_t *AXIS2_CALL
axis2_wsdl_desc_get_interface(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_qname_t *qname) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;
    axis2_char_t *name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
    name = AXIS2_QNAME_TO_STRING(qname, env);
    if(wsdl_desc_impl->interfaces)
    {
        return (axis2_wsdl_interface_t *) axis2_hash_get(wsdl_desc_impl->interfaces, 
            name, AXIS2_HASH_KEY_STRING);
    }
    return NULL;
}

/**
 * Method get_interfaces
 *
 * @return
 */
axis2_hash_t *AXIS2_CALL
axis2_wsdl_desc_get_interfaces(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_desc)->interfaces;
}

/**
 * Method axis2_wsdl_desc_set_interfaces
 *
 * @param interfaces
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_interfaces(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_hash_t *interfaces) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
    /*TODO free existing interfaces before setting new map */
    wsdl_desc_impl->interfaces = interfaces;
  
    return AXIS2_SUCCESS;
}

/**
 * Method get_svcs
 *
 * @return
 */
axis2_hash_t *AXIS2_CALL
axis2_wsdl_desc_get_svcs(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_desc)->svcs;
}

/**
 * Method axis2_wsdl_desc_set_svcs
 *
 * @param svcs
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_svcs(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_hash_t *svcs) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
    /*TODO free existing svcs before setting new map */
    wsdl_desc_impl->svcs = svcs;
  
    return AXIS2_SUCCESS;
}

/**
 * Will return the <code>axis2_wsdl_svc </code> if found otherwise return NULL.
 *
 * @param qname <code>qname</code> of the svc
 * @return The svc with the relevant qname
 */
axis2_wsdl_svc_t *AXIS2_CALL
axis2_wsdl_desc_get_svc(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_qname_t *qname) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;
    axis2_char_t *name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
    name = AXIS2_QNAME_TO_STRING(qname, env);
    if(wsdl_desc_impl->svcs)
    {
        return (axis2_wsdl_svc_t *) axis2_hash_get(wsdl_desc_impl->svcs, 
            name, AXIS2_HASH_KEY_STRING);
    }
    return NULL;
}

/**
 * Will add the <code>axis2_wsdl_svc_t</code> to the map.
 * If svc is NULL it will not be added.
 * If the <code>axis2_wsdl_svc_t</code> name is NULL an error will be set.
 *
 * @param svc
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_add_svc(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_wsdl_svc_t *svc) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;
    axis2_qname_t *svc_qname = NULL;
    axis2_char_t *svc_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);

    svc_qname = AXIS2_WSDL_SVC_GET_QNAME(svc, env);
    if(NULL == svc_qname)
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_WSDL_SVC_NAME_IS_REQUIRED, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    svc_name = AXIS2_QNAME_TO_STRING(svc_qname, env);
    if(!wsdl_desc_impl->svcs)
    {
        wsdl_desc_impl->svcs = axis2_hash_make(env);
        if(!wsdl_desc_impl->svcs)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axis2_hash_set(wsdl_desc_impl->svcs, svc_name, 
        AXIS2_HASH_KEY_STRING, svc);
    return AXIS2_SUCCESS;
}

/**
 * Method get_target_namespace
 *
 * @return
 */
axis2_char_t *AXIS2_CALL
axis2_wsdl_desc_get_target_namespace(axis2_wsdl_desc_t *wsdl_desc,
                                        axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_desc)->target_namespace;
}

/**
 * Method set_target_namespace
 *
 * @param target_namespace
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_target_namespace(axis2_wsdl_desc_t *wsdl_desc,
                                        axis2_env_t **env,
                                        axis2_char_t *target_namespace) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, target_namespace, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);

    if(wsdl_desc_impl->target_namespace)
    {
        AXIS2_FREE((*env)->allocator, wsdl_desc_impl->target_namespace);
        wsdl_desc_impl->target_namespace = NULL;
    }
    wsdl_desc_impl->target_namespace = AXIS2_STRDUP(target_namespace, env);
    if(wsdl_desc_impl->target_namespace)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/**
 * Method get_types
 *
 * @return
 */
axis2_wsdl_types_t *AXIS2_CALL
axis2_wsdl_desc_get_types(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_desc)->types;
}

/**
 * Method axis2_wsdl_desc_set_types
 *
 * @param types
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_types(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_wsdl_types_t *types) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    AXIS2_PARAM_CHECK((*env)->error, types, AXIS2_FAILURE);
    
    if(wsdl_desc_impl->types)
    {
        AXIS2_WSDL_TYPES_FREE(wsdl_desc_impl->types, env);
        wsdl_desc_impl->types = NULL;
    }
    wsdl_desc_impl->types = types;
  
    return AXIS2_SUCCESS;
}


/**
 * Gets the name attribute of the WSDL 1.1 Definitions Element
 *
 * @return
 */
axis2_qname_t *AXIS2_CALL
axis2_wsdl_desc_get_wsdl1_def_name(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_desc)->wsdl1_def_name;
}

/**
 * Sets the name attribute of the WSDL 1.1 Definitions Element
 *
 * @param wsdl1_def_name
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_wsdl1_def_name(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env,
                                    axis2_qname_t *wsdl1_def_name) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
    if(wsdl_desc_impl->wsdl1_def_name)
    {
        AXIS2_QNAME_FREE(wsdl_desc_impl->wsdl1_def_name, env);
        wsdl_desc_impl->wsdl1_def_name = NULL;
    }
    wsdl_desc_impl->wsdl1_def_name = AXIS2_QNAME_CLONE(wsdl1_def_name, env);
  
    return AXIS2_SUCCESS;
}

/**
 * Will return the Namespace URI as a String if there exists an
 * Namespace URI associated with the given prefix, in the Definition
 * Component, Will return NULL if not found.
 *
 * @param prefix Prefix defined in the Definitions elemet in the WSDL file
 * @return The Namespace URI for the prefix.
 */
axis2_char_t *AXIS2_CALL
axis2_wsdl_desc_get_namespace(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_char_t *prefix) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, prefix, NULL);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);

    return (axis2_char_t *) axis2_hash_get(wsdl_desc_impl->namespaces, prefix, 
        AXIS2_HASH_KEY_STRING);
}

/**
 * Will return all the Namespaces associated with the Definition
 * Component and will be keyed by the Napespace Prefix.
 *
 * @return
 */
axis2_hash_t *AXIS2_CALL
axis2_wsdl_desc_get_namespaces(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(wsdl_desc)->namespaces;
}

/**
 * Sets the Namespaces associated with the Difinition Component
 * and they should be keyed by its Namespace Prefix.
 *
 * @param namespaces
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_namespaces(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_hash_t *namespaces) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
    /*TODO free existing namespaces before setting new map */
    wsdl_desc_impl->namespaces = namespaces;
  
    return AXIS2_SUCCESS;
}

/**
 * Returns the WSDL Imports in an <code>axis2_array_list_t</code>
 *
 * @return
 */
axis2_array_list_t *AXIS2_CALL
axis2_wsdl_desc_get_imports(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t *env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_desc)->imports;
}

/**
 * Sets the imports as an <code>axis2_array_list_t</code>
 *
 * @param imports
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_imports(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_array_list_t *imports) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    AXIS2_PARAM_CHECK((*env)->error, imports, AXIS2_FAILURE);
    
    if(wsdl_desc_impl->imports)
    {
        /* TODO Free elements of the array */
        AXIS2_ARRAY_LIST_FREE(wsdl_desc_impl->imports, env);
        wsdl_desc_impl->imports = NULL;
    }
    wsdl_desc_impl->imports = imports;
  
    return AXIS2_SUCCESS;
}

/**
 * Adds an import to the list.
 *
 * @param wsdl_import
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_add_import(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_wsdl_import_t *wsdl_import) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_import, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
    return AXIS2_ARRAY_LIST_ADD(wsdl_desc_impl->imports, env, wsdl_import);
}

/**
 * Returns the Includes in an <code>axis2_array_list_t</code>
 *
 * @return
 */
axis2_array_list_t *AXIS2_CALL
axis2_wsdl_desc_get_includes(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t *env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_desc)->includes;
}

/**
 * Sets the includes as an <code>axis2_array_list_t</code>
 *
 * @param includes
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_set_includes(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_array_list_t *includes) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    AXIS2_PARAM_CHECK((*env)->error, includes, AXIS2_FAILURE);
    
    if(wsdl_desc_impl->includes)
    {
        /* TODO Free elements of the array */
        AXIS2_ARRAY_LIST_FREE(wsdl_desc_impl->includes, env);
        wsdl_desc_impl->includes = NULL;
    }
    wsdl_desc_impl->includes = includes;
  
    return AXIS2_SUCCESS;
}

/**
 * Adds an wsdl include to the list.
 *
 * @param wsdl_include
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_desc_add_include(axis2_wsdl_desc_t *wsdl_desc,
                            axis2_env_t **env,
                            axis2_wsdl_include_t *wsdl_include) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_include, AXIS2_FAILURE);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
    return AXIS2_ARRAY_LIST_ADD(wsdl_desc_impl->includes, env, wsdl_include);
}

axis2_wsdl_binding_t *AXIS2_CALL
axis2_wsdl_desc_get_first_binding(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env) 
{
    axis2_wsdl_desc_impl_t *wsdl_desc_impl = NULL;
    axis2_hash_index_t *index = NULL;
    void *v = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    wsdl_desc_impl = AXIS2_INTF_TO_IMPL(wsdl_desc);
    
    index = axis2_hash_first(wsdl_desc_impl->bindings, env);
    axis2_hash_this (index, NULL, NULL, &v);
    if(v)
        return (axis2_wsdl_binding_t *)v;
    return NULL;
}
