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

#ifndef WODEN_DESC_H
#define WODEN_DESC_H

/**
 * @file woden_desc.h
 * @brief Axis2 Description Interface
 * This class provides the implementation for a Description component from 
 * the WSDL Component Model, as described in the WSDL 2.0 specification.  
 * <p>
 * Note: this class is different to the other WSDL implementation classes, 
 * which all implement two interfaces; a component model interface and the 
 * interface for the corresponding WSDL element. Different implementations
 * are used for the Description component and for the &lt;wsdl:description&gt; 
 * element because the latter exposes the composite structure of imported and 
 * included WSDL documents, while the Description component 'flattens' this
 * structure into an abstract view of the WSDL. A separate implementation
 * class, <code>Descriptioni Element</code>, exists to represent
 * the &lt;wsdl:description&gt; element.
 * 
 */

#include <woden.h>
#include <woden_desc_element.h>
#include <woden_wsdl_obj.h>
#include <woden_documentable.h>
#include <woden_wsdl_component.h>

/**
 * @defgroup woden Woden/C
 * @{
 * @} 
 */


/** @defgroup woden_desc Description
  * @ingroup woden
  * @{
  */

typedef union woden_desc_base woden_desc_base_t;
typedef struct woden_desc woden_desc_t;
typedef struct woden_desc_ops woden_desc_ops_t;
struct woden_documentation_element;
struct woden_wsdl_component;
struct woden_documentable;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_desc_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *desc,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    to_desc_free) (void *desc,
            const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *desc,
            const axis2_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *desc,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_documentable *(AXIS2_CALL *
    get_base_impl) (
            void *desc,
            const axis2_env_t *env);
    /* ************************************************************
     *  Description interface methods (the WSDL Component model)
     * ************************************************************/
    axutil_array_list_t *(AXIS2_CALL *
    get_interfaces) (
            void *desc,
            const axis2_env_t *env);

    axutil_array_list_t *(AXIS2_CALL *
    get_bindings) (
            void *desc,
            const axis2_env_t *env);

    axutil_array_list_t *(AXIS2_CALL *
    get_svcs) (
            void *desc,
            const axis2_env_t *env);

    axutil_array_list_t *(AXIS2_CALL *
    get_element_decls) (
            void *desc,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_element_decl) (
            void *desc,
            const axis2_env_t *env,
            axis2_qname_t *qname);

    axutil_array_list_t *(AXIS2_CALL *
    get_type_defs) (
            void *desc,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_type_def) (
            void *desc,
            const axis2_env_t *env,
            axis2_qname_t *qname);

    void *(AXIS2_CALL *
    to_element) (
            void *desc,
            const axis2_env_t *env);
    /* ************************************************************
     *  Non-API implementation methods
     * ************************************************************/

    /*
     * The 'addAllXXX' methods are used to initialize the Description Component 
     * with its flattened properties - {Interfaces}, {Bindings}, {Services},
     * {Element Declarations}, {Type Definitions}. Currently these are non-API
     * public helper methods used by the implementation to convert the Element
     * model into the Component model (i.e. the Component model is read-only).
     * If we need to support programmatic creation/updating of the Component model,
     * we will need to consider exposing these methods on the API.
     */

    axis2_status_t (AXIS2_CALL *
    add_to_all_interfaces) (
            void *desc,
            const axis2_env_t *env,
            void *interface_);

    axis2_status_t (AXIS2_CALL *
    add_to_all_bindings) (
            void *desc,
            const axis2_env_t *env,
            void *binding);

    axis2_status_t (AXIS2_CALL *
    add_to_all_svcs) (
            void *desc,
            const axis2_env_t *env,
            void *svc);

    axis2_status_t (AXIS2_CALL *
    add_to_all_element_decls) (
            void *desc,
            const axis2_env_t *env,
            void *element_decl);

    axis2_status_t (AXIS2_CALL *
    add_to_all_type_defs) (
            void *desc,
            const axis2_env_t *env,
            void *type_def);
};

union woden_desc_base
{
    woden_documentable_t documentable;
    woden_desc_element_t desc_element;
    woden_wsdl_component_t wsdl_component;
};

struct woden_desc
{
    woden_desc_base_t base;
    woden_desc_ops_t *ops;
};

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_desc_element(
        void *desc,
        const axis2_env_t *env);

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_documentable_element(
        void *desc,
        const axis2_env_t *env);

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_documentable(
        void *desc,
        const axis2_env_t *env);

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_wsdl_obj(
        void *desc,
        const axis2_env_t *env);

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_wsdl_component(
        void *desc,
        const axis2_env_t *env);

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_attr_extensible(
        void *desc,
        const axis2_env_t *env);

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_element_extensible(
        void *desc,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_desc_resolve_methods(
        woden_desc_t *desc,
        const axis2_env_t *env,
        woden_desc_t *desc_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_DESC_FREE(desc, env) \
      (((woden_desc_t *) desc)->ops->free(desc, env))

#define WODEN_DESC_TO_DESC_FREE(desc, env) \
      (((woden_desc_t *) desc)->ops->to_desc_free(desc, env))

#define WODEN_DESC_SUPER_OBJS(desc, env) \
      (((woden_desc_t *) desc)->ops->super_objs(desc, env))

#define WODEN_DESC_TYPE(desc, env) \
      (((woden_desc_t *) desc)->ops->type(desc, env))

#define WODEN_DESC_GET_BASE_IMPL(desc, env) \
      (((woden_desc_t *) desc)->ops->get_base_impl(desc, env))

#define WODEN_DESC_GET_INTERFACES(desc, env) \
      (((woden_desc_t *) desc)->ops->\
        get_interfaces (desc, env))

#define WODEN_DESC_GET_BINDINGS(desc, env) \
      (((woden_desc_t *) desc)->ops->\
         get_bindings(desc, env))

#define WODEN_DESC_GET_SVCS(desc, env) \
      (((woden_desc_t *) desc)->ops->\
         get_svcs(desc, env))

#define WODEN_DESC_GET_ELEMENT_DECLS(desc, env) \
      (((woden_desc_t *) desc)->ops->\
         get_element_decls(desc, env))

#define WODEN_DESC_GET_ELEMENT_DECL(desc, env, qname) \
      (((woden_desc_t *) desc)->ops->\
        get_element_decl (desc, env, qname))

#define WODEN_DESC_GET_TYPE_DEFS(desc, env) \
      (((woden_desc_t *) desc)->ops->\
        get_type_defs (desc, env))

#define WODEN_DESC_GET_TYPE_DEF(desc, env, qname) \
      (((woden_desc_t *) desc)->ops->\
        get_type_def (desc, env, qname))

#define WODEN_DESC_TO_ELEMENT(desc, env) \
      (((woden_desc_t *) desc)->ops->\
        to_element (desc, env))

#define WODEN_DESC_ADD_TO_ALL_INTERFACES(desc, env, interface) \
      (((woden_desc_t *) desc)->ops->\
        add_to_all_interfaces (desc, env, interface))

#define WODEN_DESC_ADD_TO_ALL_BINDINGS(desc, env, binding) \
      (((woden_desc_t *) desc)->ops->\
        add_to_all_bindings (desc, env, binding))

#define WODEN_DESC_ADD_TO_ALL_SVCS(desc, env, svc) \
      (((woden_desc_t *) desc)->ops->\
        add_to_all_svcs (desc, env, svc))

#define WODEN_DESC_ADD_TO_ALL_ELEMENT_DECLS(desc, env, element_decl) \
      (((woden_desc_t *) desc)->ops->\
        add_to_all_element_decls (desc, env, element_decl))

#define WODEN_DESC_ADD_TO_ALL_TYPE_DEFS(desc, env, type_def) \
      (((woden_desc_t *) desc)->ops->\
        add_to_all_type_defs (desc, env, type_def))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_DESC_H */
