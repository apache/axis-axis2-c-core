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

#ifndef AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_H
#define AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_H

/**
 * @file axis2_woden_interface_fault_element.h
 * @brief Axis2 Interface Fault Element Interface
 * This interface represents a &lt;fault&gt; child element of the
 * WSDL &lt;interface&gt; element. 
 * It declares the behaviour required to support parsing, 
 * creating and manipulating a &lt;fault&gt; element.
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>
#include <axis2_array_list.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_documentable_element.h>
#include <woden/wsdl20/xml/axis2_woden_configurable_element.h>
#include <woden/wsdl20/xml/axis2_woden_nested_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_interface_fault_element axis2_woden_interface_fault_element_t;
typedef struct axis2_woden_interface_fault_element_ops axis2_woden_interface_fault_element_ops_t;
typedef union axis2_woden_interface_fault_element_base axis2_woden_interface_fault_element_base_t;

/** @defgroup axis2_woden_interface_fault_element Interface Fault Element
  * @ingroup axis2_interface
  * @{
  */

struct axis2_woden_interface_fault_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *interface_fault_element,
            axis2_env_t **env);
 
    axis2_status_t (AXIS2_CALL *
    to_interface_fault_element_free) (
            void *interface_fault_element,
            axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *interface_fault_element,
            axis2_env_t **env);
 
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *interface_fault_element,
            axis2_env_t **env);

    /**
     * Set the QName that represens the 'name' attribute of the interface 
     * &lt;fault&gt; element. This identifies the interface fault.
     * 
     * @param qname the QName that identifies the interface fault
     */
    axis2_status_t (AXIS2_CALL *
    set_qname) (
            void *iface_fault_ele,
            axis2_env_t **env,
            axis2_qname_t *qname);
    
    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *iface_fault_ele,
            axis2_env_t **env);
    
    /**
     * Set the QName that represents the 'element' attribute of the interface 
     * &lt;fault&gt; element. This identifies a Schema element declaration.
     * 
     * @param qname the QName that identifies a Schema element declaration
     */
    axis2_status_t (AXIS2_CALL *
    set_element_qname) (
            void *iface_fault_ele,
            axis2_env_t **env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_element_qname) (
            void *iface_fault_ele,
            axis2_env_t **env);
    
    /**
     * Returns the Schema element declaration identified by the QName in the 'element' 
     * attribute of the interface &lt;fault&gt; element. 
     * If this QName does not resolve to an element declaration in a schema that is visible 
     * to the containing WSDL description, null will be returned by this method. 
     * To be visible, the Schema must have been correctly imported or inlined within 
     * the &lt;types&gt; element.
     * 
     * @return the XmlSchemaElement identified by the 'element' attribute
     */
    void *(AXIS2_CALL *
    get_element) (
            void *iface_fault_ele,
            axis2_env_t **env);

};

union axis2_woden_interface_fault_element_base
{
    axis2_woden_documentable_element_t documentable_element;
    axis2_woden_configurable_element_t configurable_element;
    axis2_woden_nested_element_t nested_element;
};

struct axis2_woden_interface_fault_element
{
    axis2_woden_interface_fault_element_base_t base;
    axis2_woden_interface_fault_element_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_interface_fault_element_t *)
axis2_woden_interface_fault_element_create(
        axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_status_t)
axis2_woden_interface_fault_element_resolve_methods(
        axis2_woden_interface_fault_element_t *interface_fault_element,
        axis2_env_t **env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_FREE(interface_fault_element, env) \
		(((axis2_woden_interface_fault_element_t *) interface_fault_element)->ops->\
         free (interface_fault_element, env))

#define AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_TO_INTERFACE_FAULT_ELEMENT_FREE(interface_fault_element, env) \
		(((axis2_woden_interface_fault_element_t *) interface_fault_element)->ops->\
         to_interface_fault_element_free (interface_fault_element, env))

#define AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_SUPER_OBJS(interface_fault_element, env) \
		(((axis2_woden_interface_fault_element_t *) interface_fault_element)->ops->\
         super_objs (interface_fault_element, env))

#define AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_TYPE(interface_fault_element, env) \
		(((axis2_woden_interface_fault_element_t *) interface_fault_element)->ops->\
         type (interface_fault_element, env))

#define AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_SET_QNAME(interface_fault_element, env, qname) \
		(((axis2_woden_interface_fault_element_t *) interface_fault_element)->ops->\
         set_qname (interface_fault_element, env, qname))

#define AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_GET_QNAME(interface_fault_element, env) \
		(((axis2_woden_interface_fault_element_t *) interface_fault_element)->ops->\
        get_qname  (interface_fault_element, env))

#define AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_SET_ELEMENT_QNAME(interface_fault_element, env, qname) \
		(((axis2_woden_interface_fault_element_t *) interface_fault_element)->ops->\
         set_element_qname (interface_fault_element, env, qname))

#define AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_GET_ELEMENT_QNAME(interface_fault_element, env) \
		(((axis2_woden_interface_fault_element_t *) interface_fault_element)->ops->\
         get_element_qname (interface_fault_element, env))

#define AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_GET_ELEMENT(interface_fault_element, env) \
		(((axis2_woden_interface_fault_element_t *) interface_fault_element)->ops->\
         get_element (interface_fault_element, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_H */
