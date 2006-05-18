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

#ifndef AXIS2_WODEN_BINDING_OP_ELEMENT_H
#define AXIS2_WODEN_BINDING_OP_ELEMENT_H

/**
 * @file axis2_woden_binding_op_element.h
 * @brief Axis2 Binding Msg Reference Element Binding
 * This interface represents the &lt;operation&gt; child element of a
 * WSDL &lt;binding&gt; element. 
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

typedef struct axis2_woden_binding_op_element axis2_woden_binding_op_element_t;
typedef struct axis2_woden_binding_op_element_ops axis2_woden_binding_op_element_ops_t;
typedef union axis2_woden_binding_op_element_base axis2_woden_binding_op_element_base_t;

/** @defgroup axis2_woden_binding_op_element Binding Msg Reference Element
  * @ingroup axis2_binding
  * @{
  */

struct axis2_woden_binding_op_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *binding_op_element,
            axis2_env_t **env);
 
    axis2_status_t (AXIS2_CALL *
    to_binding_op_element_free) (
            void *binding_op_element,
            axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *binding_op_element,
            axis2_env_t **env);
 
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *binding_op_element,
            axis2_env_t **env);
    
    /**
     * Set the 'ref' attribute to the specified QName, which identifies the
     * interface operation referenced by this binding operation.
     *
     * @param qname identifies the associated interface operation.
     */
    axis2_status_t (AXIS2_CALL *
    set_ref) (
            void *iface_op_ele,
            axis2_env_t **env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_ref) (
            void *iface_op_ele,
            axis2_env_t **env);
 
    /**
     * Get the Interface Operation Element identified by the QName specified in the
     * <tt>ref</tt> attribute of this binding &lt;operation&gt; element.
     * 
     * @return the Interface Operation Element associated with this binding operation
     */
    void *(AXIS2_CALL *
    get_interface_op_element) (
            void *iface_op_ele,
            axis2_env_t **env);
 
    axis2_status_t (AXIS2_CALL *
    add_binding_msg_ref_element) (
            void *iface_op_ele,
            axis2_env_t **env,
            void *msg_ref);

    axis2_status_t (AXIS2_CALL *
    remove_binding_msg_ref_element) (
            void *iface_op_ele,
            axis2_env_t **env,
            void *msg_ref);
 
    axis2_array_list_t *(AXIS2_CALL *
    get_binding_msg_ref_elements) (
            void *iface_op_ele,
            axis2_env_t **env);
     
    axis2_status_t (AXIS2_CALL *
    add_binding_fault_ref_element) (
            void *iface_op_ele,
            axis2_env_t **env,
            void *fault_ref);

    axis2_status_t (AXIS2_CALL *
    remove_binding_fault_ref_element) (
            void *iface_op_ele,
            axis2_env_t **env,
            void *fault_ref);
 
    axis2_array_list_t *(AXIS2_CALL *
    get_binding_fault_ref_elements) (
            void *iface_op_ele,
            axis2_env_t **env);
    

};

union axis2_woden_binding_op_element_base
{
    axis2_woden_documentable_element_t documentable_element;
    axis2_woden_configurable_element_t configurable_element;
    axis2_woden_nested_element_t nested_element;
};

struct axis2_woden_binding_op_element
{
    axis2_woden_binding_op_element_base_t base;
    axis2_woden_binding_op_element_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_binding_op_element_t *)
axis2_woden_binding_op_element_create(
        axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_status_t)
axis2_woden_binding_op_element_resolve_methods(
        axis2_woden_binding_op_element_t *binding_op_element,
        axis2_env_t **env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_BINDING_OP_ELEMENT_FREE(binding_op_element, env) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         free (binding_op_element, env))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_TO_BINDING_OP_ELEMENT_FREE(binding_op_element, env) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         to_binding_op_element_free (binding_op_element, env))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_SUPER_OBJS(binding_op_element, env) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         super_objs (binding_op_element, env))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_TYPE(binding_op_element, env) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         type (binding_op_element, env))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_SET_REF(binding_op_element, env, qname) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         set_ref (binding_op_element, env, qname))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_GET_REF(binding_op_element, env) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
        get_ref  (binding_op_element, env))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_GET_INTERFACE_OP_ELEMENT(binding_op_element, env) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         get_interface_op_element (binding_op_element, env))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_ADD_BINDING_MSG_REF_ELEMENT(binding_op_element, env, msg_ref) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         add_msg_ref_element (binding_op_element, env, msg_ref))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_REMOVE_MSG_REF_ELEMENT(binding_op_element, env, msg_ref) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         remove_msg_ref_element (binding_op_element, env, msg_ref))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_GET_MSG_REF_ELEMENTS(binding_op_element, env) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         get_msg_ref_elements (binding_op_element, env))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_ADD_BINDING_FAULT_REF_ELEMENT(binding_op_element, env, fault_ref) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         add_fault_ref_element (binding_op_element, env, fault_ref))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_REMOVE_FAULT_REF_ELEMENT(binding_op_element, env, fault_ref) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         remove_fault_ref_element (binding_op_element, env, fault_ref))

#define AXIS2_WODEN_BINDING_OP_ELEMENT_GET_FAULT_REF_ELEMENTS(binding_op_element, env) \
		(((axis2_woden_binding_op_element_t *) binding_op_element)->ops->\
         get_fault_ref_elements (binding_op_element, env))





/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_BINDING_OP_ELEMENT_H */
