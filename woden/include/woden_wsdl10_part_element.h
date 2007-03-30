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

#ifndef WODEN_WSDL10_PART_ELEMENT_H
#define WODEN_WSDL10_PART_ELEMENT_H

/**
 * @file woden_wsdl10_part_element.h
 * @brief Axis2 Interface Msg Reference Element Interface
 * This interface represents the &lt;input&gt; and &lt;output&gt; 
 * child elements of interface operation. 
 */

#include <axutil_allocator.h>
#include <axis2_env.h>
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

typedef struct woden_wsdl10_part_element woden_wsdl10_part_element_t;
typedef struct woden_wsdl10_part_element_ops woden_wsdl10_part_element_ops_t;
typedef union woden_wsdl10_part_element_base woden_wsdl10_part_element_base_t;

/** @defgroup woden_wsdl10_part_element Interface Msg Reference Element
  * @ingroup axis2_interface
  * @{
  */

struct woden_wsdl10_part_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *part_element,
            const axis2_env_t *env);
 
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *part_element,
            const axis2_env_t *env);
    
    /**
     * Set the QName that represents the 'element' attribute of the interface 
     * operation &lt;input&gt; or &lt;output&gt; element. This identifies a Schema 
     * element declaration.
     * 
     * @param qname the QName that identifies a Schema element declaration
     */
    axis2_status_t (AXIS2_CALL *
    set_element_qname) (
            void *iface_fault_ele,
            const axis2_env_t *env,
            axis2_qname_t *element);

    axis2_qname_t *(AXIS2_CALL *
    get_element_qname) (
            void *iface_fault_ele,
            const axis2_env_t *env);
    
    /**
     * Returns the Schema element declaration identified by the QName in the 'element' 
     * attribute of the interface operation &lt;input&gt; or &lt;output&gt;  element. 
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
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_qname) (
            void *iface_fault_ele,
            const axis2_env_t *env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *iface_fault_ele,
            const axis2_env_t *env);
    

};

union woden_wsdl10_part_element_base
{
    woden_documentable_element_t documentable_element;
    woden_configurable_element_t configurable_element;
    woden_nested_element_t nested_element;
};

struct woden_wsdl10_part_element
{
    woden_wsdl10_part_element_base_t base;
    woden_wsdl10_part_element_ops_t *ops;
};

AXIS2_EXTERN woden_wsdl10_part_element_t * AXIS2_CALL
woden_wsdl10_part_element_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_wsdl10_part_element_resolve_methods(
        woden_wsdl10_part_element_t *part_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_WSDL10_PART_ELEMENT_FREE(part_element, env) \
      (((woden_wsdl10_part_element_t *) part_element)->ops->\
         free (part_element, env))

#define WODEN_WSDL10_PART_ELEMENT_TYPE(part_element, env) \
      (((woden_wsdl10_part_element_t *) part_element)->ops->\
         type (part_element, env))

#define WODEN_WSDL10_PART_ELEMENT_SET_ELEMENT_QNAME(part_element, env, element) \
      (((woden_wsdl10_part_element_t *) part_element)->ops->\
        set_element_qname  (part_element, env, element))

#define WODEN_WSDL10_PART_ELEMENT_GET_ELEMENT_QNAME(part_element, env) \
      (((woden_wsdl10_part_element_t *) part_element)->ops->\
        get_element_qname  (part_element, env))

#define WODEN_WSDL10_PART_ELEMENT_GET_ELEMENT(part_element, env) \
      (((woden_wsdl10_part_element_t *) part_element)->ops->\
        get_element  (part_element, env))

#define WODEN_WSDL10_PART_ELEMENT_SET_QNAME(part_element, env, qname) \
      (((woden_wsdl10_part_element_t *) part_element)->ops->\
         set_qname (part_element, env, qname))

#define WODEN_WSDL10_PART_ELEMENT_GET_QNAME(part_element, env) \
      (((woden_wsdl10_part_element_t *) part_element)->ops->\
         get_qname (part_element, env))




/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL10_PART_ELEMENT_H */
