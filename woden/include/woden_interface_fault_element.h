/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WODEN_INTERFACE_FAULT_ELEMENT_H
#define WODEN_INTERFACE_FAULT_ELEMENT_H

/**
 * @file woden_interface_fault_element.h
 * @brief Axis2 Interface Fault Element Interface
 * This interface represents a &lt;fault&gt; child element of the
 * WSDL &lt;interface&gt; element. 
 * It declares the behaviour required to support parsing, 
 * creating and manipulating a &lt;fault&gt; element.
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>
#include <axutil_array_list.h>
#include <woden.h>
#include <woden_documentable_element.h>
#include <woden_configurable_element.h>
#include <woden_nested_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_interface_fault_element woden_interface_fault_element_t;
typedef struct woden_interface_fault_element_ops woden_interface_fault_element_ops_t;
typedef union woden_interface_fault_element_base woden_interface_fault_element_base_t;

/** @defgroup woden_interface_fault_element Interface Fault Element
  * @ingroup axis2_interface
  * @{
  */

struct woden_interface_fault_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *interface_fault_element,
            const axutil_env_t *env);
 
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *interface_fault_element,
            const axutil_env_t *env);

    /**
     * Set the QName that represens the 'name' attribute of the interface 
     * &lt;fault&gt; element. This identifies the interface fault.
     * 
     * @param qname the QName that identifies the interface fault
     */
    axis2_status_t (AXIS2_CALL *
    set_qname) (
            void *iface_fault_ele,
            const axutil_env_t *env,
            axis2_qname_t *qname);
    
    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *iface_fault_ele,
            const axutil_env_t *env);
    
    /**
     * Set the QName that represents the 'element' attribute of the interface 
     * &lt;fault&gt; element. This identifies a Schema element declaration.
     * 
     * @param qname the QName that identifies a Schema element declaration
     */
    axis2_status_t (AXIS2_CALL *
    set_element_qname) (
            void *iface_fault_ele,
            const axutil_env_t *env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_element_qname) (
            void *iface_fault_ele,
            const axutil_env_t *env);
    
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
            const axutil_env_t *env);

};

union woden_interface_fault_element_base
{
    woden_documentable_element_t documentable_element;
    woden_configurable_element_t configurable_element;
    woden_nested_element_t nested_element;
};

struct woden_interface_fault_element
{
    woden_interface_fault_element_base_t base;
    woden_interface_fault_element_ops_t *ops;
};

AXIS2_EXTERN woden_interface_fault_element_t * AXIS2_CALL
woden_interface_fault_element_create(
        const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_interface_fault_element_resolve_methods(
        woden_interface_fault_element_t *interface_fault_element,
        const axutil_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_INTERFACE_FAULT_ELEMENT_FREE(interface_fault_element, env) \
      (((woden_interface_fault_element_t *) interface_fault_element)->ops->\
         free (interface_fault_element, env))

#define WODEN_INTERFACE_FAULT_ELEMENT_TYPE(interface_fault_element, env) \
      (((woden_interface_fault_element_t *) interface_fault_element)->ops->\
         type (interface_fault_element, env))

#define WODEN_INTERFACE_FAULT_ELEMENT_SET_QNAME(interface_fault_element, env, qname) \
      (((woden_interface_fault_element_t *) interface_fault_element)->ops->\
         set_qname (interface_fault_element, env, qname))

#define WODEN_INTERFACE_FAULT_ELEMENT_GET_QNAME(interface_fault_element, env) \
      (((woden_interface_fault_element_t *) interface_fault_element)->ops->\
        get_qname  (interface_fault_element, env))

#define WODEN_INTERFACE_FAULT_ELEMENT_SET_ELEMENT_QNAME(interface_fault_element, env, qname) \
      (((woden_interface_fault_element_t *) interface_fault_element)->ops->\
         set_element_qname (interface_fault_element, env, qname))

#define WODEN_INTERFACE_FAULT_ELEMENT_GET_ELEMENT_QNAME(interface_fault_element, env) \
      (((woden_interface_fault_element_t *) interface_fault_element)->ops->\
         get_element_qname (interface_fault_element, env))

#define WODEN_INTERFACE_FAULT_ELEMENT_GET_ELEMENT(interface_fault_element, env) \
      (((woden_interface_fault_element_t *) interface_fault_element)->ops->\
         get_element (interface_fault_element, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_INTERFACE_FAULT_ELEMENT_H */
