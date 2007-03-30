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

#ifndef WODEN_MSG_REG_ELEMENT_H
#define WODEN_MSG_REG_ELEMENT_H

/**
 * @file woden_msg_ref_element.h
 * @brief Axis2 Interface Msg Reference Element Interface
 * This interface represents the &lt;input&gt; and &lt;output&gt; 
 * child elements of interface operation. 
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axutil_hash.h>
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

typedef struct woden_msg_ref_element woden_msg_ref_element_t;
typedef struct woden_msg_ref_element_ops woden_msg_ref_element_ops_t;
typedef union woden_msg_ref_element_base woden_msg_ref_element_base_t;

/** @defgroup woden_msg_ref_element Interface Msg Reference Element
  * @ingroup axis2_interface
  * @{
  */

struct woden_msg_ref_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *msg_ref_element,
            const axutil_env_t *env);
 
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *msg_ref_element,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_msg_label) (
            void *iface_fault_ele,
            const axutil_env_t *env,
            void *msg_label);

    void *(AXIS2_CALL *
    get_msg_label) (
            void *iface_fault_ele,
            const axutil_env_t *env);
 
    axis2_status_t (AXIS2_CALL *
    set_msg_content_model) (
            void *iface_fault_ele,
            const axutil_env_t *env,
            axis2_char_t *nm_token);
    
    axis2_char_t *(AXIS2_CALL *
    get_msg_content_model) (
            void *iface_fault_ele,
            const axutil_env_t *env);
 
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
            const axutil_env_t *env,
            axis2_qname_t *element);

    axis2_qname_t *(AXIS2_CALL *
    get_element_qname) (
            void *iface_fault_ele,
            const axutil_env_t *env);
    
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
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_direction) (
            void *iface_fault_ele,
            const axutil_env_t *env,
            void *dir);

    void *(AXIS2_CALL *
    get_direction) (
            void *iface_fault_ele,
            const axutil_env_t *env);
    

};

union woden_msg_ref_element_base
{
    woden_documentable_element_t documentable_element;
    woden_configurable_element_t configurable_element;
    woden_nested_element_t nested_element;
};

struct woden_msg_ref_element
{
    woden_msg_ref_element_base_t base;
    woden_msg_ref_element_ops_t *ops;
};

AXIS2_EXTERN woden_msg_ref_element_t * AXIS2_CALL
woden_msg_ref_element_create(
        const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_msg_ref_element_resolve_methods(
        woden_msg_ref_element_t *msg_ref_element,
        const axutil_env_t *env,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_MSG_REG_ELEMENT_FREE(msg_ref_element, env) \
      (((woden_msg_ref_element_t *) msg_ref_element)->ops->\
         free (msg_ref_element, env))

#define WODEN_MSG_REG_ELEMENT_TYPE(msg_ref_element, env) \
      (((woden_msg_ref_element_t *) msg_ref_element)->ops->\
         type (msg_ref_element, env))

#define WODEN_MSG_REG_ELEMENT_SET_MSG_LABEL(msg_ref_element, env, msg_label) \
      (((woden_msg_ref_element_t *) msg_ref_element)->ops->\
         set_msg_label (msg_ref_element, env, msg_label))

#define WODEN_MSG_REG_ELEMENT_GET_MSG_LABEL(msg_ref_element, env) \
      (((woden_msg_ref_element_t *) msg_ref_element)->ops->\
        get_msg_label  (msg_ref_element, env))

#define WODEN_MSG_REG_ELEMENT_SET_MSG_CONTENT_MODEL(msg_ref_element, env, nm_token) \
      (((woden_msg_ref_element_t *) msg_ref_element)->ops->\
         set_msg_content_model (msg_ref_element, env, nm_token))

#define WODEN_MSG_REG_ELEMENT_GET_MSG_CONTENT_MODEL(msg_ref_element, env) \
      (((woden_msg_ref_element_t *) msg_ref_element)->ops->\
         get_msg_content_model (msg_ref_element, env))

#define WODEN_MSG_REG_ELEMENT_SET_ELEMENT_QNAME(msg_ref_element, env, element) \
      (((woden_msg_ref_element_t *) msg_ref_element)->ops->\
        set_element_qname  (msg_ref_element, env, element))

#define WODEN_MSG_REG_ELEMENT_GET_ELEMENT_QNAME(msg_ref_element, env) \
      (((woden_msg_ref_element_t *) msg_ref_element)->ops->\
        get_element_qname  (msg_ref_element, env))

#define WODEN_MSG_REG_ELEMENT_GET_ELEMENT(msg_ref_element, env) \
      (((woden_msg_ref_element_t *) msg_ref_element)->ops->\
        get_element  (msg_ref_element, env))

#define WODEN_MSG_REG_ELEMENT_SET_DIRECTION(msg_ref_element, env, dir) \
      (((woden_msg_ref_element_t *) msg_ref_element)->ops->\
         set_direction (msg_ref_element, env, dir))

#define WODEN_MSG_REG_ELEMENT_GET_DIRECTION(msg_ref_element, env) \
      (((woden_msg_ref_element_t *) msg_ref_element)->ops->\
         get_direction (msg_ref_element, env))




/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_MSG_REG_ELEMENT_H */
