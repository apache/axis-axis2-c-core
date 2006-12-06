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

#ifndef WODEN_BINDING_FAULT_ELEMENT_H
#define WODEN_BINDING_FAULT_ELEMENT_H

/**
 * @file woden_binding_fault_element.h
 * @brief Axis2 Binding Fault Element Binding
 * This binding represents the &lt;fault&gt; child element of a &lt;binding&gt; element. 
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>
#include <axis2_array_list.h>
#include <woden.h>
#include <woden_documentable_element.h>
#include <woden_configurable_element.h>
#include <woden_nested_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_binding_fault_element woden_binding_fault_element_t;
typedef struct woden_binding_fault_element_ops woden_binding_fault_element_ops_t;
typedef union woden_binding_fault_element_base woden_binding_fault_element_base_t;

/** @defgroup woden_binding_fault_element Binding Fault Element
  * @ingroup axis2_binding
  * @{
  */

struct woden_binding_fault_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *binding_fault_element,
            const axis2_env_t *env);
 
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *binding_fault_element,
            const axis2_env_t *env);

    /**
     * Set the 'ref' attribute to the specified QName, which identifies the
     * binding fault referenced by this binding fault.
     *
     * @param qname identifies the associated binding fault.
     */
    axis2_status_t (AXIS2_CALL *
    set_ref) (
            void *iface_fault_ele,
            const axis2_env_t *env,
            axis2_qname_t *qname);
    
    axis2_qname_t *(AXIS2_CALL *
    get_ref) (
            void *iface_fault_ele,
            const axis2_env_t *env);
    
    /**
     * Get the Interface Fault Element identified by the QName specified in the
     * <tt>ref</tt> attribute of this binding &lt;fault&gt; element.
     * 
     * @return the Interface Fault Element associated with this binding fault
     */
    void *(AXIS2_CALL *
    get_interface_fault_element) (
            void *iface_fault_ele,
            const axis2_env_t *env);

};

union woden_binding_fault_element_base
{
    woden_documentable_element_t documentable_element;
    woden_configurable_element_t configurable_element;
    woden_nested_element_t nested_element;
};

struct woden_binding_fault_element
{
    woden_binding_fault_element_base_t base;
    woden_binding_fault_element_ops_t *ops;
};

AXIS2_EXTERN woden_binding_fault_element_t * AXIS2_CALL
woden_binding_fault_element_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_binding_fault_element_resolve_methods(
        woden_binding_fault_element_t *binding_fault_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_BINDING_FAULT_ELEMENT_FREE(binding_fault_element, env) \
      (((woden_binding_fault_element_t *) binding_fault_element)->ops->\
         free (binding_fault_element, env))

#define WODEN_BINDING_FAULT_ELEMENT_TO_BINDING_FAULT_ELEMENT_FREE(binding_fault_element, env) \
      (((woden_binding_fault_element_t *) binding_fault_element)->ops->\
         to_binding_fault_element_free (binding_fault_element, env))

#define WODEN_BINDING_FAULT_ELEMENT_SUPER_OBJS(binding_fault_element, env) \
      (((woden_binding_fault_element_t *) binding_fault_element)->ops->\
         super_objs (binding_fault_element, env))

#define WODEN_BINDING_FAULT_ELEMENT_TYPE(binding_fault_element, env) \
      (((woden_binding_fault_element_t *) binding_fault_element)->ops->\
         type (binding_fault_element, env))

#define WODEN_BINDING_FAULT_ELEMENT_SET_REF(binding_fault_element, env, qname) \
      (((woden_binding_fault_element_t *) binding_fault_element)->ops->\
         set_ref (binding_fault_element, env, qname))

#define WODEN_BINDING_FAULT_ELEMENT_GET_REF(binding_fault_element, env) \
      (((woden_binding_fault_element_t *) binding_fault_element)->ops->\
        get_ref  (binding_fault_element, env))

#define WODEN_BINDING_FAULT_ELEMENT_GET_INTERFACE_FAULT_ELEMENT(binding_fault_element, env) \
      (((woden_binding_fault_element_t *) binding_fault_element)->ops->\
         get_interface_fault_element (binding_fault_element, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_BINDING_FAULT_ELEMENT_H */
