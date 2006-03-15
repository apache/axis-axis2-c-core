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

#ifndef AXIS2_WSDL_DESC_H
#define AXIS2_WSDL_DESC_H

/**
 * @file axis2_wsdl_desc.h
 * @brief Axis2 Wsdl interface interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_hash.h>
#include <axis2_array_list.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
struct axis2_wsdl_op;
struct axis2_wsdl_component;
struct axis2_wsdl_include;
struct axis2_wsdl_import;
struct axis2_wsdl_types;
struct axis2_wsdl_msg_ref;
struct axis2_wsdl_fault_ref;
struct axis2_wsdl_feature;
struct axis2_wsdl_property;
struct axis2_wsdl_binding;
struct axis2_wsdl_binding_fault;
struct axis2_wsdl_binding_op;
struct axis2_wsdl_extensible_attribute;
struct axis2_wsdl_interface;

typedef struct axis2_wsdl_desc axis2_wsdl_desc_t;
typedef struct axis2_wsdl_desc_ops axis2_wsdl_desc_ops_t;

/** @defgroup axis2_wsdl_desc Wsdl Description
 * @ingroup axis2_wsdl
 * @{
 */


/** 
 * @brief Wsdl Description ops struct
 * Encapsulator struct for ops of axis2_wsdl_desc
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_desc_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_desc_t *wsdl_desc,
		    axis2_env_t **env);

    /**
     * Returns a Map of <code>wsdl_bindings</code> Objects keyed by the 
     * <code>axis2_qname_t</code> of the Binding.
     *
     * @return
     */
    axis2_hash_t *(AXIS2_CALL *
    get_bindings)(axis2_wsdl_desc_t *wsdl_desc,  
                                    axis2_env_t **env);
    
    /**
     * Sets the whole new set of Bindings to the axis2_wsd_desc.
     *
     * @param bindings
     */
    axis2_status_t (AXIS2_CALL *
    set_bindings)(axis2_wsdl_desc_t *wsdl_desc,
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
    axis2_status_t (AXIS2_CALL *
    add_binding)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                struct axis2_wsdl_binding *binding);
    
    /**
     * Retrives the <code>axis2_wsdl_binding_t</code> by its qname. Wil return NULL
     * if <code>axis2_wsdl_binding_t</code> is not found.
     *
     * @param qname The qname of the Binding.
     * @return
     */
    struct axis2_wsdl_binding *(AXIS2_CALL *
    get_binding)(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env,
                                    axis2_qname_t *qname);
    
    /**
     * The Interface component will be added to the map keyed with its own name.
     * If the Interface is NULL it will not be added.
     * If the interface name is NULL an Error will be set and return.
     *
     * @param interface
     */
    axis2_status_t (AXIS2_CALL *
    add_interface)(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_interface *interface);
    
    /**
     * The Interface will be returned if it exsists,
     * otherwise null will be returned.
     *
     * @param qname qname of the Interface.
     * @return The Interface with the relavent qname
     */
    struct axis2_wsdl_interface *(AXIS2_CALL *
    get_interface)(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env,
                                    axis2_qname_t *qname);
    
    /**
     * Method get_interfaces
     *
     * @return
     */
    axis2_hash_t *(AXIS2_CALL *
    get_interfaces)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env);
    
    /**
     * Method set_interfaces
     *
     * @param interfaces
     */
    axis2_status_t (AXIS2_CALL *
    set_interfaces)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_hash_t *interfaces);
    
    /**
     * Method get_svcs
     *
     * @return
     */
    axis2_hash_t *(AXIS2_CALL *
    get_svcs)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env);
    
    /**
     * Method set_svcs
     *
     * @param svcs
     */
    axis2_status_t (AXIS2_CALL *
    set_svcs)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_hash_t *svcs);
    
    /**
     * Will return the <code>axis2_wsdl_svc </code> if found otherwise return NULL.
     *
     * @param qname <code>qname</code> of the svc
     * @return The svc with the relevant qname
     */
    struct axis2_wsdl_svc *(AXIS2_CALL *
    get_svc)(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env,
                                    axis2_qname_t *qname);
    
    /**
     * Will add the <code>axis2_wsdl_svc_t</code> to the map.
     * If svc is NULL it will not be added.
     * If the <code>axis2_wsdl_svc_t</code> name is NULL an error will be set.
     *
     * @param svc
     */
    axis2_status_t (AXIS2_CALL *
    add_svc)(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_svc *svc);
    
    /**
     * Method get_target_namespace
     *
     * @return
     */
    axis2_char_t *(AXIS2_CALL *
    get_target_namespace)(axis2_wsdl_desc_t *wsdl_desc,
                                            axis2_env_t **env);
    
    /**
     * Method set_target_namespace
     *
     * @param target_namespace
     */
    axis2_status_t (AXIS2_CALL *
    set_target_namespace)(axis2_wsdl_desc_t *wsdl_desc,
                                            axis2_env_t **env,
                                            axis2_char_t *target_namespace);
    
    /**
     * Method get_types
     *
     * @return
     */
    struct axis2_wsdl_types *(AXIS2_CALL *
    get_types)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env);
    
    /**
     * Method axis2_wsdl_desc_set_types
     *
     * @param types
     */
    axis2_status_t (AXIS2_CALL *
    set_types)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                struct axis2_wsdl_types *types);
    
    /**
     * Gets the name attribute of the WSDL 1.1 Definitions Element
     *
     * @return
     */
    axis2_qname_t *(AXIS2_CALL *
    get_wsdl1_def_name)(axis2_wsdl_desc_t *wsdl_desc,
                                        axis2_env_t **env);
    
    /**
     * Sets the name attribute of the WSDL 1.1 Definitions Element
     *
     * @param wsdl1_def_name
     */
    axis2_status_t (AXIS2_CALL *
    set_wsdl1_def_name)(axis2_wsdl_desc_t *wsdl_desc,
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
    axis2_char_t *(AXIS2_CALL *
    get_namespace)(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env,
                                    axis2_char_t *prefix);
    
    /**
     * Will return all the Namespaces associated with the Definition
     * Component and will be keyed by the Napespace Prefix.
     *
     * @return
     */
    axis2_hash_t *(AXIS2_CALL *
    get_namespaces)(axis2_wsdl_desc_t *wsdl_desc,
                                    axis2_env_t **env);
    
    /**
     * Sets the Namespaces associated with the Difinition Component
     * and they should be keyed by its Namespace Prefix.
     *
     * @param namespaces
     */
    axis2_status_t (AXIS2_CALL *
    set_namespaces)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_hash_t *namespaces);
    
    /**
     * Returns the WSDL Imports in an <code>axis2_array_list_t</code>
     *
     * @return
     */
    axis2_array_list_t *(AXIS2_CALL *
    get_imports)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t *env);
    
    /**
     * Sets the imports as an <code>axis2_array_list_t</code>
     *
     * @param imports
     */
    axis2_status_t (AXIS2_CALL *
    set_imports)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_array_list_t *imports);
    
    /**
     * Adds an import to the list.
     *
     * @param wsdl_import
     */
    axis2_status_t (AXIS2_CALL *
    add_import)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                struct axis2_wsdl_import *wsdl_import);
    
    /**
     * Returns the Includes in an <code>axis2_array_list_t</code>
     *
     * @return
     */
    axis2_array_list_t *(AXIS2_CALL *
    get_includes)(axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t *env);
    
    /**
     * Sets the includes as an <code>axis2_array_list_t</code>
     *
     * @param includes
     */
    axis2_status_t (AXIS2_CALL *
    set_includes) (axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                axis2_array_list_t *includes);
    
    /**
     * Adds an wsdl include to the list.
     *
     * @param wsdl_include
     */
    axis2_status_t (AXIS2_CALL *
    add_include) (axis2_wsdl_desc_t *wsdl_desc,
                                axis2_env_t **env,
                                struct axis2_wsdl_include *wsdl_include);
    
    struct axis2_wsdl_binding *(AXIS2_CALL *
    get_first_binding) (axis2_wsdl_desc_t *wsdl_desc,
                      axis2_env_t **env);

};

/** 
 * @brief Wsdl container struct
 *	Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_desc
{
	axis2_wsdl_desc_ops_t *ops;
    struct axis2_wsdl_component *component;
};

/**
 * Creates wsdl property struct
 * @return pointer to newly created wsdl property
 */
AXIS2_DECLARE(axis2_wsdl_desc_t *) 
axis2_wsdl_desc_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_DESC_FREE(wsdl_desc, env) \
		((wsdl_desc)->ops->free (wsdl_desc, env))

#define AXIS2_WSDL_DESC_GET_BINDINGS(wsdl_desc, env) \
		((wsdl_desc)->ops->get_bindings (wsdl_desc, env))

#define AXIS2_WSDL_DESC_SET_BINDINGS(wsdl_desc, env, bindings) \
		((wsdl_desc)->ops->set_bindings (wsdl_desc, env, bindings))

#define AXIS2_WSDL_DESC_ADD_BINDING(wsdl_desc, env, binding) \
		((wsdl_desc)->ops->add_binding (wsdl_desc, env, binding))

#define AXIS2_WSDL_DESC_GET_BINDING(wsdl_desc, env, qname) \
		((wsdl_desc)->ops->get_binding (wsdl_desc, env, qname))

#define AXIS2_WSDL_DESC_ADD_INTERFACE(wsdl_desc, env, interface) \
		((wsdl_desc)->ops->get_interface (wsdl_desc, env, interface))
        
#define AXIS2_WSDL_DESC_GET_INTERFACE(wsdl_desc, env, qname) \
		((wsdl_desc)->ops->get_interface (wsdl_desc, env, qname))

#define AXIS2_WSDL_DESC_GET_INTERFACES(wsdl_desc, env) \
		((wsdl_desc)->ops->get_interfaces (wsdl_desc, env))        

#define AXIS2_WSDL_DESC_SET_INTERFACES(wsdl_desc, env, interfaces) \
		((wsdl_desc)->ops->set_interfaces (wsdl_desc, env, interfaces))
        
#define AXIS2_WSDL_DESC_GET_SVCS(wsdl_desc, env) \
		((wsdl_desc)->ops->get_svcs (wsdl_desc, env))
        
#define AXIS2_WSDL_DESC_SET_SVCS(wsdl_desc, env, svcs) \
		((wsdl_desc)->ops->set_svcs (wsdl_desc, env, svcs))
        
#define AXIS2_WSDL_DESC_GET_SVC(wsdl_desc, env, qname) \
		((wsdl_desc)->ops->get_svc (wsdl_desc, env, qname))

#define AXIS2_WSDL_DESC_ADD_SVC(wsdl_desc, env, svc) \
		((wsdl_desc)->ops->add_svc (wsdl_desc, env, svc))

#define AXIS2_WSDL_DESC_GET_TARGET_NAMESPACE(wsdl_desc, env) \
		((wsdl_desc)->ops->get_target_namespace (wsdl_desc, env))
        
#define AXIS2_WSDL_DESC_SET_TARGET_NAMESPACE(wsdl_desc, env, target_namespace) \
		((wsdl_desc)->ops->set_target_namespace (wsdl_desc, env, target_namespace))
        
#define AXIS2_WSDL_DESC_GET_TYPES(wsdl_desc, env) \
		((wsdl_desc)->ops->get_types (wsdl_desc, env))
        
#define AXIS2_WSDL_DESC_SET_TYPES(wsdl_desc, env, types) \
		((wsdl_desc)->ops->set_types (wsdl_desc, env, types))

#define AXIS2_WSDL_DESC_GET_WSDL1_DEF_NAME(wsdl_desc, env) \
		((wsdl_desc)->ops->get_wsdl1_def_name (wsdl_desc, env))
        
#define AXIS2_WSDL_DESC_SET_WSDL1_DEF_NAME(wsdl_desc, env, wsdl1_def_qname) \
		((wsdl_desc)->ops->set_wsdl1_def_name (wsdl_desc, env, wsdl1_def_qname))

#define AXIS2_WSDL_DESC_GET_NAMESPACE(wsdl_desc, env, prefix) \
		((wsdl_desc)->ops->get_namespace (wsdl_desc, env, prefix))

#define AXIS2_WSDL_DESC_GET_NAMESPACES(wsdl_desc, env) \
		((wsdl_desc)->ops->get_namespaces (wsdl_desc, env))
        
#define AXIS2_WSDL_DESC_SET_NAMESPACES(wsdl_desc, env, namespaces) \
		((wsdl_desc)->ops->set_namespace (wsdl_desc, env, namespaces))
        
#define AXIS2_WSDL_DESC_GET_IMPORTS(wsdl_desc, env) \
		((wsdl_desc)->ops->get_imports (wsdl_desc, env))
        
#define AXIS2_WSDL_DESC_SET_IMPORTS(wsdl_desc, env, imports) \
		((wsdl_desc)->ops->set_imports (wsdl_desc, env, imports))

#define AXIS2_WSDL_DESC_ADD_IMPORT(wsdl_desc, env, wsdl_import) \
		((wsdl_desc)->ops->add_import (wsdl_desc, env, wsdl_import))
        
#define AXIS2_WSDL_DESC_GET_INCLUDES(wsdl_desc, env) \
		((wsdl_desc)->ops->get_includes (wsdl_desc, env))

#define AXIS2_WSDL_DESC_SET_INCLUDES(wsdl_desc, env, includes) \
		((wsdl_desc)->ops->set_includes (wsdl_desc, env, includes))

#define AXIS2_WSDL_DESC_ADD_INCLUDE(wsdl_desc, env, wsdl_include) \
		((wsdl_desc)->ops->add_include (wsdl_desc, env, wsdl_include))
        	
/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_DESC_H */
