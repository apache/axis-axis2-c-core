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

#include <woden_wsdl_obj.h>
#include <woden_desc.h>
#include <woden_element_decl.h>
#include <woden_type_def.h>
#include <woden_documentation.h>
#include <woden_import.h>
#include <woden_include.h>
#include <woden_types.h>
#include <woden_interface.h>
#include <woden_interface_fault.h>
#include <woden_interface_op.h>
#include <woden_interface_fault_ref.h>
#include <woden_interface_msg_ref.h>
#include <woden_binding.h>
#include <woden_binding_fault.h>
#include <woden_binding_op.h>
#include <woden_binding_fault_ref.h>
#include <woden_binding_msg_ref.h>
#include <woden_svc.h>
#include <woden_endpoint.h>
#include <woden_feature.h>
#include <woden_property.h>

#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include "woden_component_model_builder.h"

typedef struct woden_desc_impl woden_desc_impl_t;

/**
 * @brief Description Struct Impl
 *   Axis2 Description
 */
struct woden_desc_impl
{
    woden_desc_t desc;
    woden_obj_types_t obj_type;
    woden_documentable_t *documentable;
    axis2_hash_t *super;
    axis2_hash_t *methods;

    axutil_array_list_t *f_all_interfaces;
    axutil_array_list_t *f_all_bindings;
    axutil_array_list_t *f_all_svcs;
    axutil_array_list_t *f_all_element_decls;
    axutil_array_list_t *f_all_type_defs;
    /* WSDL Element model data */
    axis2_uri_t *f_doc_base_uri;
    /* <description> attributes */
    axis2_uri_t *f_target_namespc;
    axis2_hash_t *f_namespcs;
    /* <description> child elements */
    axutil_array_list_t *f_import_elements;
    axutil_array_list_t *f_include_elements;
    axutil_array_list_t *f_interface_elements;
    axutil_array_list_t *f_binding_elements;
    axutil_array_list_t *f_svc_elements;
    void *f_types_element;
    /* Woden specific vars */
    axis2_bool_t f_component_initialized;
    void *f_ext_reg;

};

#define INTF_TO_IMPL(desc) ((woden_desc_impl_t *) desc)

axis2_status_t AXIS2_CALL
woden_desc_free(
    void *desc,
    const axutil_env_t *env);

static axis2_status_t AXIS2_CALL
woden_desc_free_ops(
    void *desc,
    const axutil_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_desc_super_objs(
    void *desc,
    const axutil_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_desc_type(
    void *desc,
    const axutil_env_t *env);

woden_documentable_t *AXIS2_CALL
woden_desc_get_base_impl(
    void *desc,
    const axutil_env_t *env);

/* ************************************************************
 *  Description interface methods (the WSDL Component model)
 * ************************************************************/
axutil_array_list_t *AXIS2_CALL
woden_desc_get_interfaces(
    void *desc,
    const axutil_env_t *env);

axutil_array_list_t *AXIS2_CALL
woden_desc_get_bindings(
    void *desc,
    const axutil_env_t *env);

axutil_array_list_t *AXIS2_CALL
woden_desc_get_svcs(
    void *desc,
    const axutil_env_t *env);

axutil_array_list_t *AXIS2_CALL
woden_desc_get_element_decls(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_get_element_decl(
    void *desc,
    const axutil_env_t *env,
    axis2_qname_t *qname);

axutil_array_list_t *AXIS2_CALL
woden_desc_get_type_defs(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_get_type_def(
    void *desc,
    const axutil_env_t *env,
    axis2_qname_t *qname);

void *AXIS2_CALL
woden_desc_to_element(
    void *desc,
    const axutil_env_t *env);

/* ************************************************************
 *  Description Element interface methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_desc_set_document_base_uri(
    void *desc,
    const axutil_env_t *env,
    axis2_uri_t *doc_base_uri);

axis2_uri_t *AXIS2_CALL
woden_desc_get_document_base_uri(
    void *desc,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_desc_set_target_namespace(
    void *desc,
    const axutil_env_t *env,
    axis2_uri_t *namespc);

axis2_uri_t *AXIS2_CALL
woden_desc_get_target_namespace(
    void *desc,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_desc_add_namespace(
    void *desc,
    const axutil_env_t *env,
    axis2_char_t *prefix,
    axis2_uri_t *namespc);

axis2_status_t AXIS2_CALL
woden_desc_remove_namespace(
    void *desc,
    const axutil_env_t *env,
    axis2_char_t *prefix);

axis2_uri_t *AXIS2_CALL
woden_desc_get_namespace(
    void *desc,
    const axutil_env_t *env,
    axis2_char_t *prefix);

axis2_hash_t *AXIS2_CALL
woden_desc_get_namespaces(
    void *desc,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_desc_add_import_element(
    void *desc,
    const axutil_env_t *env,
    void *import_el);

axutil_array_list_t *AXIS2_CALL
woden_desc_get_import_elements(
    void *desc,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_desc_add_include_element(
    void *desc,
    const axutil_env_t *env,
    void *include_el);

axutil_array_list_t *AXIS2_CALL
woden_desc_get_include_elements(
    void *desc,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_desc_set_types_element(
    void *desc,
    const axutil_env_t *env,
    void *types_el);

axis2_uri_t *AXIS2_CALL
woden_desc_get_types_element(
    void *desc,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_desc_add_interface_element(
    void *desc,
    const axutil_env_t *env,
    void *interface_el);

axutil_array_list_t *AXIS2_CALL
woden_desc_get_interface_elements(
    void *desc,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_desc_add_binding_element(
    void *desc,
    const axutil_env_t *env,
    void *binding_el);

axutil_array_list_t *AXIS2_CALL
woden_desc_get_binding_elements(
    void *desc,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_desc_add_svc_element(
    void *desc,
    const axutil_env_t *env,
    void *svc_el);

axutil_array_list_t *AXIS2_CALL
woden_desc_get_svc_elements(
    void *desc,
    const axutil_env_t *env);

/* Creator methods */

void *AXIS2_CALL
woden_desc_create_documentation_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_import_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_include_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_types_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_interface_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_interface_fault_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_interface_op_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_interface_fault_ref_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_interface_msg_ref_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_binding_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_binding_fault_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_binding_op_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_binding_fault_ref_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_binding_msg_ref_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_svc_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_endpoint_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_feature_element(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_create_property_element(
    void *desc,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_desc_set_ext_registry(
    void *desc,
    const axutil_env_t *env,
    void *ext_reg);

void *AXIS2_CALL
woden_desc_get_ext_registry(
    void *desc,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_desc_to_component(
    void *desc,
    const axutil_env_t *env);
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

axis2_status_t AXIS2_CALL
woden_desc_add_to_all_interfaces(
    void *desc,
    const axutil_env_t *env,
    void *interface);

axis2_status_t AXIS2_CALL
woden_desc_add_to_all_bindings(
    void *desc,
    const axutil_env_t *env,
    void *binding);

axis2_status_t AXIS2_CALL
woden_desc_add_to_all_svcs(
    void *desc,
    const axutil_env_t *env,
    void *svc);

axis2_status_t AXIS2_CALL
woden_desc_add_to_all_element_decls(
    void *desc,
    const axutil_env_t *env,
    void *element_decl);

axis2_status_t AXIS2_CALL
woden_desc_add_to_all_type_defs(
    void *desc,
    const axutil_env_t *env,
    void *type_def);

static axis2_status_t AXIS2_CALL
woden_desc_init_components(
    void *desc,
    const axutil_env_t *env);

static woden_desc_t *
create(const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_desc_element(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!desc)
    {
	  desc = (void *) create (env);
	  desc_impl = (woden_desc_impl_t *) desc;
    }
    else
        desc_impl = (woden_desc_impl_t *) desc;

    if (desc)
	  woden_desc_free_ops(desc, env);

    desc_impl->desc.base.desc_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_desc_element_ops_t));
    woden_desc_element_resolve_methods(&(desc_impl->desc.base.
            desc_element), env, desc_impl->methods);
    return desc;
}

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_documentable_element(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!desc)
    {
	  desc = (void *) create (env);
	  desc_impl = (woden_desc_impl_t *) desc;
    }
    else
        desc_impl = (woden_desc_impl_t *) desc;

    if (desc)
	  woden_desc_free_ops(desc, env);

    desc_impl->desc.base.desc_element.documentable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(desc_impl->desc.base.
            desc_element.documentable_element), env,
            desc_impl->methods);
    return desc;
}

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_documentable(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!desc)
    {
	  desc = (void *) create (env);
	  desc_impl = (woden_desc_impl_t *) desc;
    }
    else
        desc_impl = (woden_desc_impl_t *) desc;

    if (desc)
	  woden_desc_free_ops(desc, env);

    desc_impl->desc.base.documentable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_ops_t));
    woden_documentable_resolve_methods(&(desc_impl->desc.base.
            documentable), env, desc_impl->documentable,
            desc_impl->methods);
    return desc;
}

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_wsdl_obj(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    void *wsdl_obj = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!desc)
    {
	  desc = (void *) create (env);
	  desc_impl = (woden_desc_impl_t *) desc;
    }
    else
        desc_impl = (woden_desc_impl_t *) desc;

    if (desc)
	  woden_desc_free_ops(desc, env);

    desc_impl->desc.base.documentable.base.wsdl_obj.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_wsdl_obj_ops_t));
    wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(
                desc_impl->documentable, env);
    woden_wsdl_obj_resolve_methods(&(desc_impl->desc.base.
            documentable.base.wsdl_obj), env, wsdl_obj, desc_impl->methods);
    return desc;
}

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_wsdl_component(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!desc)
    {
	  desc = (void *) create (env);
	  desc_impl = (woden_desc_impl_t *) desc;
    }
    else
        desc_impl = (woden_desc_impl_t *) desc;
    if (desc)
	  woden_desc_free_ops(desc, env);

    desc_impl->desc.base.wsdl_component.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_wsdl_component_ops_t));
    woden_wsdl_component_resolve_methods(&(desc_impl->desc.base.
            wsdl_component), env, desc_impl->methods);
    return desc;
}

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_attr_extensible(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!desc)
    {
	  desc = (void *) create (env);
	  desc_impl = (woden_desc_impl_t *) desc;
    }
    else
        desc_impl = (woden_desc_impl_t *) desc;

    if (desc)
	  woden_desc_free_ops(desc, env);


    desc_impl->desc.base.desc_element.documentable_element.
    wsdl_element.base.attr_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(desc_impl->desc.base.
            desc_element.documentable_element.wsdl_element.base.
            attr_extensible), env, NULL, desc_impl->methods);
    return desc;
}

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_to_element_extensible(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!desc)
    {
	  desc = (void *) create (env);
	  desc_impl = (woden_desc_impl_t *) desc;
    }
    else
        desc_impl = (woden_desc_impl_t *) desc;

    if (desc)
	  woden_desc_free_ops(desc, env);

    desc_impl->desc.base.desc_element.documentable_element.
    wsdl_element.base.element_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(desc_impl->desc.base.
            desc_element.documentable_element.wsdl_element.base.
            element_extensible), env, NULL, desc_impl->methods);
    return desc;
}


/************************End of Woden C Internal Methods***********************/
static woden_desc_t *
create(const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    desc_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_desc_impl_t));

    desc_impl->obj_type = WODEN_DESC;
    desc_impl->super = NULL;
    desc_impl->methods = NULL;

    desc_impl->f_all_interfaces = NULL;
    desc_impl->f_all_bindings = NULL;
    desc_impl->f_all_svcs = NULL;
    desc_impl->f_all_element_decls = NULL;
    desc_impl->f_all_type_defs = NULL;
    desc_impl->f_doc_base_uri = NULL;
    desc_impl->f_target_namespc = NULL;
    desc_impl->f_namespcs = NULL;
    desc_impl->f_import_elements = NULL;
    desc_impl->f_include_elements = NULL;
    desc_impl->f_interface_elements = NULL;
    desc_impl->f_binding_elements = NULL;
    desc_impl->f_svc_elements = NULL;
    desc_impl->f_types_element = NULL;
    desc_impl->f_component_initialized = AXIS2_FALSE;
    desc_impl->f_ext_reg = NULL;

    desc_impl->desc.base.desc_element.ops = NULL;
    desc_impl->desc.base.desc_element.documentable_element.ops =
        NULL;
    desc_impl->desc.base.documentable.ops = NULL;
    desc_impl->desc.base.wsdl_component.ops = NULL;
    desc_impl->desc.base.documentable.base.wsdl_obj.ops = NULL;
    desc_impl->desc.base.documentable.base.wsdl_obj.base.wsdl_element.
    base.attr_extensible.ops = NULL;
    desc_impl->desc.base.documentable.base.wsdl_obj.base.wsdl_element.
    base.element_extensible.ops = NULL;

    desc_impl->desc.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_desc_ops_t));

    desc_impl->desc.ops->free = woden_desc_free;
    desc_impl->desc.ops->super_objs = woden_desc_super_objs;
    desc_impl->desc.ops->type = woden_desc_type;
    desc_impl->desc.ops->get_base_impl = woden_desc_get_base_impl;

    desc_impl->desc.ops->get_interfaces = woden_desc_get_interfaces;
    desc_impl->desc.ops->get_bindings = woden_desc_get_bindings;
    desc_impl->desc.ops->get_svcs = woden_desc_get_svcs;
    desc_impl->desc.ops->get_element_decls = woden_desc_get_element_decls;
    desc_impl->desc.ops->get_element_decl = woden_desc_get_element_decl;
    desc_impl->desc.ops->get_type_defs = woden_desc_get_type_defs;
    desc_impl->desc.ops->get_type_def = woden_desc_get_type_def;
    desc_impl->desc.ops->to_element = woden_desc_to_element;
    desc_impl->desc.ops->add_to_all_interfaces =
        woden_desc_add_to_all_interfaces;
    desc_impl->desc.ops->add_to_all_bindings =
        woden_desc_add_to_all_bindings;
    desc_impl->desc.ops->add_to_all_svcs =
        woden_desc_add_to_all_svcs;
    desc_impl->desc.ops->add_to_all_element_decls =
        woden_desc_add_to_all_element_decls;
    desc_impl->desc.ops->add_to_all_type_defs =
        woden_desc_add_to_all_type_defs;


    desc_impl->f_namespcs = axis2_hash_make(env);
    if (!desc_impl->f_namespcs)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    desc_impl->f_binding_elements = axutil_array_list_create(env, 0);
    desc_impl->f_interface_elements = axutil_array_list_create(env, 0);
    desc_impl->f_svc_elements = axutil_array_list_create(env, 0);
    if (!desc_impl->f_interface_elements ||
            !desc_impl->f_svc_elements ||
            !desc_impl->f_binding_elements)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    desc_impl->methods = axis2_hash_make(env);
    if (!desc_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(desc_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_desc_free);
    axis2_hash_set(desc_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_desc_super_objs);
    axis2_hash_set(desc_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_desc_type);

    axis2_hash_set(desc_impl->methods, "get_interfaces",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_interfaces);
    axis2_hash_set(desc_impl->methods, "get_bindings",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_bindings);
    axis2_hash_set(desc_impl->methods, "get_svcs",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_svcs);
    axis2_hash_set(desc_impl->methods, "get_element_decls",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_element_decls);
    axis2_hash_set(desc_impl->methods, "get_element_decl",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_element_decl);
    axis2_hash_set(desc_impl->methods, "get_type_defs",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_type_defs);
    axis2_hash_set(desc_impl->methods, "get_type_def",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_type_def);
    axis2_hash_set(desc_impl->methods, "to_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_to_element);
    axis2_hash_set(desc_impl->methods, "set_document_base_uri",
            AXIS2_HASH_KEY_STRING,
            woden_desc_set_document_base_uri);
    axis2_hash_set(desc_impl->methods, "get_document_base_uri",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_document_base_uri);
    axis2_hash_set(desc_impl->methods, "set_target_namespace",
            AXIS2_HASH_KEY_STRING,
            woden_desc_set_target_namespace);
    axis2_hash_set(desc_impl->methods, "get_target_namespace",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_target_namespace);
    axis2_hash_set(desc_impl->methods, "add_namespace",
            AXIS2_HASH_KEY_STRING,
            woden_desc_add_namespace);
    axis2_hash_set(desc_impl->methods, "remove_namespace",
            AXIS2_HASH_KEY_STRING,
            woden_desc_remove_namespace);
    axis2_hash_set(desc_impl->methods, "get_namespace",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_namespace);
    axis2_hash_set(desc_impl->methods, "get_namespaces",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_namespaces);
    axis2_hash_set(desc_impl->methods, "add_import_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_add_import_element);
    axis2_hash_set(desc_impl->methods, "get_import_elements",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_import_elements);
    axis2_hash_set(desc_impl->methods, "add_include_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_add_include_element);
    axis2_hash_set(desc_impl->methods, "get_include_elements",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_include_elements);
    axis2_hash_set(desc_impl->methods, "set_types_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_set_types_element);
    axis2_hash_set(desc_impl->methods, "get_types_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_types_element);
    axis2_hash_set(desc_impl->methods, "add_interface_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_add_interface_element);
    axis2_hash_set(desc_impl->methods, "get_interface_elements",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_interface_elements);
    axis2_hash_set(desc_impl->methods, "add_binding_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_add_binding_element);
    axis2_hash_set(desc_impl->methods, "get_binding_elements",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_binding_elements);
    axis2_hash_set(desc_impl->methods, "add_svc_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_add_svc_element);
    axis2_hash_set(desc_impl->methods, "get_svc_elements",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_svc_elements);
    axis2_hash_set(desc_impl->methods, "create_documentation_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_documentation_element);
    axis2_hash_set(desc_impl->methods, "create_import_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_import_element);
    axis2_hash_set(desc_impl->methods, "create_include_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_include_element);
    axis2_hash_set(desc_impl->methods, "create_types_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_types_element);
    axis2_hash_set(desc_impl->methods, "create_interface_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_interface_element);
    axis2_hash_set(desc_impl->methods, "create_interface_fault_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_interface_fault_element);
    axis2_hash_set(desc_impl->methods, "create_interface_op_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_interface_op_element);
    axis2_hash_set(desc_impl->methods, "create_interface_fault_ref_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_interface_fault_ref_element);
    axis2_hash_set(desc_impl->methods, "create_interface_msg_ref_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_interface_msg_ref_element);
    axis2_hash_set(desc_impl->methods, "create_binding_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_binding_element);
    axis2_hash_set(desc_impl->methods, "create_binding_fault_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_binding_fault_element);
    axis2_hash_set(desc_impl->methods, "create_binding_op_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_binding_op_element);
    axis2_hash_set(desc_impl->methods, "create_binding_fault_ref_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_binding_fault_ref_element);
    axis2_hash_set(desc_impl->methods, "create_binding_msg_ref_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_binding_msg_ref_element);
    axis2_hash_set(desc_impl->methods, "create_svc_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_svc_element);
    axis2_hash_set(desc_impl->methods, "create_endpoint_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_endpoint_element);
    axis2_hash_set(desc_impl->methods, "create_feature_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_feature_element);
    axis2_hash_set(desc_impl->methods, "create_property_element",
            AXIS2_HASH_KEY_STRING,
            woden_desc_create_property_element);
    axis2_hash_set(desc_impl->methods, "set_ext_registry",
            AXIS2_HASH_KEY_STRING,
            woden_desc_set_ext_registry);
    axis2_hash_set(desc_impl->methods, "get_ext_registry",
            AXIS2_HASH_KEY_STRING,
            woden_desc_get_ext_registry);
    axis2_hash_set(desc_impl->methods, "to_component",
            AXIS2_HASH_KEY_STRING,
            woden_desc_to_component);
    axis2_hash_set(desc_impl->methods, "add_to_all_interfaces",
            AXIS2_HASH_KEY_STRING,
            woden_desc_add_to_all_interfaces);
    axis2_hash_set(desc_impl->methods, "add_to_all_bindings",
            AXIS2_HASH_KEY_STRING,
            woden_desc_add_to_all_bindings);
    axis2_hash_set(desc_impl->methods, "add_to_all_svcs",
            AXIS2_HASH_KEY_STRING,
            woden_desc_add_to_all_svcs);
    axis2_hash_set(desc_impl->methods, "add_to_all_element_decls",
            AXIS2_HASH_KEY_STRING,
            woden_desc_add_to_all_element_decls);
    axis2_hash_set(desc_impl->methods, "add_to_all_type_defs",
            AXIS2_HASH_KEY_STRING,
            woden_desc_add_to_all_type_defs);

    return &(desc_impl->desc);
}

AXIS2_EXTERN woden_desc_t * AXIS2_CALL
woden_desc_create(const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    desc_impl = (woden_desc_impl_t *) create(env);

    desc_impl->documentable = woden_documentable_create(env);

    desc_impl->super = axis2_hash_make(env);
    if (!desc_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(desc_impl->super, "WODEN_DESC", AXIS2_HASH_KEY_STRING,
            &(desc_impl->desc));
    axis2_hash_set(desc_impl->super, "WODEN_DOCUMENTABLE", AXIS2_HASH_KEY_STRING,
            desc_impl->documentable);

    return &(desc_impl->desc);
}

static axis2_status_t AXIS2_CALL
woden_desc_free_ops(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    desc_impl = INTF_TO_IMPL(desc);

    if (desc_impl->desc.base.desc_element.ops)
    {
        AXIS2_FREE(env->allocator, desc_impl->desc.base.
                desc_element.ops);
        desc_impl->desc.base.desc_element.ops = NULL;
    }

    if (desc_impl->desc.base.desc_element.documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, desc_impl->desc.base.
                desc_element.documentable_element.ops);
        desc_impl->desc.base.desc_element.documentable_element.ops =
            NULL;
    }

    if (desc_impl->desc.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, desc_impl->desc.base.
                documentable.ops);
        desc_impl->desc.base.documentable.ops =
            NULL;
    }

    if (desc_impl->desc.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE(env->allocator, desc_impl->desc.base.
                documentable.base.wsdl_obj.ops);
        desc_impl->desc.base.documentable.base.wsdl_obj.ops =
            NULL;
    }

    if (desc_impl->desc.base.wsdl_component.ops)
    {
        AXIS2_FREE(env->allocator, desc_impl->desc.base.
                wsdl_component.ops);
        desc_impl->desc.base.wsdl_component.ops = NULL;
    }

    if (desc_impl->desc.base.desc_element.
            documentable_element.wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, desc_impl->desc.base.
                desc_element.documentable_element.wsdl_element.base.
                element_extensible.ops);
        desc_impl->desc.base.desc_element.
        documentable_element.wsdl_element.base.element_extensible.ops = NULL;
    }

    if (desc_impl->desc.base.desc_element.
            documentable_element.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, desc_impl->desc.base.
                desc_element.documentable_element.wsdl_element.base.
                attr_extensible.ops);
        desc_impl->desc.base.desc_element.
        documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_desc_free(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    desc_impl = INTF_TO_IMPL(desc);

    /* TODO free f_parent */
    if (desc_impl->f_target_namespc)
    {
        axis2_uri_free(desc_impl->f_target_namespc, env);
        desc_impl->f_target_namespc = NULL;
    }
    if (desc_impl->f_doc_base_uri)
    {
        axis2_uri_free(desc_impl->f_doc_base_uri, env);
        desc_impl->f_doc_base_uri = NULL;
    }
    /* TODO free f_namespcs */
    if (desc_impl->super)
    {
        axis2_hash_free(desc_impl->super, env);
        desc_impl->super = NULL;
    }

    if (desc_impl->methods)
    {
        axis2_hash_free(desc_impl->methods, env);
        desc_impl->methods = NULL;
    }

    if (desc_impl->documentable)
    {
        WODEN_DOCUMENTABLE_FREE(desc_impl->documentable, env);
        desc_impl->documentable = NULL;
    }

    woden_desc_free_ops(desc, env);

    if ((&(desc_impl->desc))->ops)
    {
        AXIS2_FREE(env->allocator, (&(desc_impl->desc))->ops);
        (&(desc_impl->desc))->ops = NULL;
    }

    if (desc_impl)
    {
        AXIS2_FREE(env->allocator, desc_impl);
        desc_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_desc_super_objs(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    desc_impl = INTF_TO_IMPL(desc);

    return desc_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_desc_type(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    desc_impl = INTF_TO_IMPL(desc);

    return desc_impl->obj_type;
}

woden_documentable_t *AXIS2_CALL
woden_desc_get_base_impl(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    desc_impl = INTF_TO_IMPL(desc);

    return desc_impl->documentable;
}

axis2_status_t AXIS2_CALL
woden_desc_resolve_methods(
    woden_desc_t *desc,
    const axutil_env_t *env,
    woden_desc_t *desc_impl,
    axis2_hash_t *methods)
{
    woden_desc_impl_t *desc_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    desc_impl_l = INTF_TO_IMPL(desc_impl);

    desc->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    desc->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    desc->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    desc->ops->get_interfaces = axis2_hash_get(methods,
            "get_interfaces", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->get_interfaces && desc_impl_l)
        desc->ops->get_interfaces =
            desc_impl_l->desc.ops->get_interfaces;

    desc->ops->get_bindings = axis2_hash_get(methods,
            "get_bindings", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->get_bindings && desc_impl_l)
        desc->ops->get_bindings =
            desc_impl_l->desc.ops->get_bindings;

    desc->ops->get_svcs = axis2_hash_get(methods,
            "get_svcs", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->get_svcs && desc_impl_l)
        desc->ops->get_svcs =
            desc_impl_l->desc.ops->get_svcs;

    desc->ops->get_element_decls = axis2_hash_get(methods,
            "get_element_decls", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->get_element_decls && desc_impl_l)
        desc->ops->get_element_decls =
            desc_impl_l->desc.ops->get_element_decls;

    desc->ops->get_element_decl = axis2_hash_get(methods,
            "get_element_decl", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->get_element_decl && desc_impl_l)
        desc->ops->get_element_decl =
            desc_impl_l->desc.ops->get_element_decl;

    desc->ops->get_type_defs = axis2_hash_get(methods,
            "get_type_defs", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->get_type_defs && desc_impl_l)
        desc->ops->get_type_defs =
            desc_impl_l->desc.ops->get_type_defs;

    desc->ops->get_type_def = axis2_hash_get(methods,
            "get_type_def", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->get_type_def && desc_impl_l)
        desc->ops->get_type_def =
            desc_impl_l->desc.ops->get_type_def;

    desc->ops->to_element = axis2_hash_get(methods,
            "to_element", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->to_element && desc_impl_l)
        desc->ops->to_element =
            desc_impl_l->desc.ops->to_element;

    desc->ops->add_to_all_interfaces = axis2_hash_get(methods,
            "add_to_all_interfaces", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->add_to_all_interfaces && desc_impl_l)
        desc->ops->add_to_all_interfaces =
            desc_impl_l->desc.ops->add_to_all_interfaces;

    desc->ops->add_to_all_bindings = axis2_hash_get(methods,
            "add_to_all_bindings", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->add_to_all_bindings && desc_impl_l)
        desc->ops->add_to_all_bindings =
            desc_impl_l->desc.ops->add_to_all_bindings;

    desc->ops->add_to_all_svcs = axis2_hash_get(methods,
            "add_to_all_svcs", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->add_to_all_svcs && desc_impl_l)
        desc->ops->add_to_all_svcs =
            desc_impl_l->desc.ops->add_to_all_svcs;

    desc->ops->add_to_all_element_decls = axis2_hash_get(methods,
            "add_to_all_element_decls", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->add_to_all_element_decls && desc_impl_l)
        desc->ops->add_to_all_element_decls =
            desc_impl_l->desc.ops->add_to_all_element_decls;

    desc->ops->add_to_all_type_defs = axis2_hash_get(methods,
            "add_to_all_type_defs", AXIS2_HASH_KEY_STRING);
    if (!desc->ops->add_to_all_type_defs && desc_impl_l)
        desc->ops->add_to_all_type_defs =
            desc_impl_l->desc.ops->add_to_all_type_defs;

    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Description interface methods (the WSDL Component model)
 * ************************************************************/
axutil_array_list_t *AXIS2_CALL
woden_desc_get_interfaces(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    if (AXIS2_TRUE != desc_impl->f_component_initialized)
        woden_desc_init_components(desc, env);
    return desc_impl->f_interface_elements;
}

axutil_array_list_t *AXIS2_CALL
woden_desc_get_bindings(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    if (AXIS2_TRUE != desc_impl->f_component_initialized)
        woden_desc_init_components(desc, env);
    return desc_impl->f_binding_elements;
}

axutil_array_list_t *AXIS2_CALL
woden_desc_get_svcs(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    if (AXIS2_TRUE != desc_impl->f_component_initialized)
        woden_desc_init_components(desc, env);
    return desc_impl->f_svc_elements;
}

axutil_array_list_t *AXIS2_CALL
woden_desc_get_element_decls(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    if (AXIS2_TRUE != desc_impl->f_component_initialized)
        woden_desc_init_components(desc, env);
    return desc_impl->f_all_element_decls;
}

void *AXIS2_CALL
woden_desc_get_element_decl(
    void *desc,
    const axutil_env_t *env,
    axis2_qname_t *qname)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;
    void *el_decl = NULL;
    int i = 0, size = 0;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    if (AXIS2_TRUE != desc_impl->f_component_initialized)
        woden_desc_init_components(desc, env);
    size = axutil_array_list_size(desc_impl->f_all_element_decls, env);
    for (i = 0; i < size; i++)
    {
        axis2_qname_t *qname_l = NULL;
        void *ed = axutil_array_list_get(desc_impl->f_all_element_decls, env, i);
        qname_l = WODEN_ELEMENT_DECL_GET_QNAME(ed, env);
        if (0 == axis2_qname_equals(qname, env, qname_l))
        {
            el_decl = ed;
            break;
        }
    }
    return el_decl;
}

axutil_array_list_t *AXIS2_CALL
woden_desc_get_type_defs(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    if (AXIS2_TRUE != desc_impl->f_component_initialized)
        woden_desc_init_components(desc, env);
    return desc_impl->f_all_type_defs;
}

void *AXIS2_CALL
woden_desc_get_type_def(
    void *desc,
    const axutil_env_t *env,
    axis2_qname_t *qname)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;
    void *type_def = NULL;
    int i = 0, size = 0;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    if (AXIS2_TRUE != desc_impl->f_component_initialized)
        woden_desc_init_components(desc, env);
    size = axutil_array_list_size(desc_impl->f_all_element_decls, env);
    for (i = 0; i < size; i++)
    {
        axis2_qname_t *qname_l = NULL;
        void *ed = axutil_array_list_get(desc_impl->f_all_element_decls, env, i);
        qname_l = WODEN_TYPE_DEF_GET_QNAME(ed, env);
        if (0 == axis2_qname_equals(qname, env, qname_l))
        {
            type_def = ed;
            break;
        }
    }
    return type_def;
}

void *AXIS2_CALL
woden_desc_to_element(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return &(desc_impl->desc);
}

/* ************************************************************
 *  Description Element interface methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_desc_set_document_base_uri(
    void *desc,
    const axutil_env_t *env,
    axis2_uri_t *doc_base_uri)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, doc_base_uri, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));
    if (desc_impl->f_doc_base_uri)
    {
        axis2_uri_free(desc_impl->f_doc_base_uri, env);
        desc_impl->f_doc_base_uri = NULL;
    }

    desc_impl->f_doc_base_uri = axis2_uri_clone(doc_base_uri, env);
    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL
woden_desc_get_document_base_uri(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return desc_impl->f_doc_base_uri;
}


axis2_status_t AXIS2_CALL
woden_desc_set_target_namespace(
    void *desc,
    const axutil_env_t *env,
    axis2_uri_t *namespc)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, namespc, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));
    if (desc_impl->f_target_namespc)
    {
        axis2_uri_free(desc_impl->f_target_namespc, env);
        desc_impl->f_target_namespc = NULL;
    }

    desc_impl->f_target_namespc = axis2_uri_clone(namespc, env);
    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL
woden_desc_get_target_namespace(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return desc_impl->f_target_namespc;
}

axis2_status_t AXIS2_CALL
woden_desc_add_namespace(
    void *desc,
    const axutil_env_t *env,
    axis2_char_t *prefix,
    axis2_uri_t *namespc)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;
    axis2_char_t *pfx = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    pfx = (prefix) ? prefix : "";
    if (namespc)
        axis2_hash_set(desc_impl->f_namespcs, pfx, AXIS2_HASH_KEY_STRING,
                axis2_uri_clone(namespc, env));
    else
        axis2_hash_set(desc_impl->f_namespcs, pfx, AXIS2_HASH_KEY_STRING, NULL);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_desc_remove_namespace(
    void *desc,
    const axutil_env_t *env,
    axis2_char_t *prefix)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;
    axis2_char_t *pfx = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    pfx = (prefix) ? prefix : "";
    axis2_hash_set(desc_impl->f_namespcs, pfx, AXIS2_HASH_KEY_STRING, NULL);
    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL
woden_desc_get_namespace(
    void *desc,
    const axutil_env_t *env,
    axis2_char_t *prefix)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;
    axis2_char_t *pfx = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    pfx = (prefix) ? prefix : "";
    return axis2_hash_get(desc_impl->f_namespcs, pfx, AXIS2_HASH_KEY_STRING);
}

axis2_hash_t *AXIS2_CALL
woden_desc_get_namespaces(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return desc_impl->f_namespcs;
}

axis2_status_t AXIS2_CALL
woden_desc_add_import_element(
    void *desc,
    const axutil_env_t *env,
    void *import_el)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, import_el, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return axutil_array_list_add(desc_impl->f_import_elements, env, import_el);
}

axutil_array_list_t *AXIS2_CALL
woden_desc_get_import_elements(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return desc_impl->f_import_elements;
}

axis2_status_t AXIS2_CALL
woden_desc_add_include_element(
    void *desc,
    const axutil_env_t *env,
    void *include_el)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, include_el, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return axutil_array_list_add(desc_impl->f_include_elements, env, include_el);
}

axutil_array_list_t *AXIS2_CALL
woden_desc_get_include_elements(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return desc_impl->f_include_elements;
}

axis2_status_t AXIS2_CALL
woden_desc_set_types_element(
    void *desc,
    const axutil_env_t *env,
    void *types_el)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, types_el, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));
    if (desc_impl->f_types_element)
    {
        /* TODO */
    }

    desc_impl->f_types_element = types_el;
    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL
woden_desc_get_types_element(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return desc_impl->f_types_element;
}

axis2_status_t AXIS2_CALL
woden_desc_add_interface_element(
    void *desc,
    const axutil_env_t *env,
    void *intface)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, intface, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return axutil_array_list_add(desc_impl->f_interface_elements, env, intface);
}

axutil_array_list_t *AXIS2_CALL
woden_desc_get_interface_elements(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return desc_impl->f_interface_elements;
}

axis2_status_t AXIS2_CALL
woden_desc_add_binding_element(
    void *desc,
    const axutil_env_t *env,
    void *binding)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, binding, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return axutil_array_list_add(desc_impl->f_binding_elements, env, binding);
}

axutil_array_list_t *AXIS2_CALL
woden_desc_get_binding_elements(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return desc_impl->f_binding_elements;
}

axis2_status_t AXIS2_CALL
woden_desc_add_svc_element(
    void *desc,
    const axutil_env_t *env,
    void *svc_el)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_el, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return axutil_array_list_add(desc_impl->f_svc_elements, env, svc_el);
}

axutil_array_list_t *AXIS2_CALL
woden_desc_get_svc_elements(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return desc_impl->f_svc_elements;
}

/* Creator methods */

void *AXIS2_CALL
woden_desc_create_documentation_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_documentation_create(env);
}

void *AXIS2_CALL
woden_desc_create_import_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_import_create(env);
}

void *AXIS2_CALL
woden_desc_create_include_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_include_create(env);
}

void *AXIS2_CALL
woden_desc_create_types_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_types_create(env);
}

void *AXIS2_CALL
woden_desc_create_interface_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_interface_create(env);
}

void *AXIS2_CALL
woden_desc_create_interface_fault_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_interface_fault_create(env);
}

void *AXIS2_CALL
woden_desc_create_interface_op_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_interface_op_create(env);
}

void *AXIS2_CALL
woden_desc_create_interface_fault_ref_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_interface_fault_ref_create(env);
}

void *AXIS2_CALL
woden_desc_create_interface_msg_ref_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_interface_msg_ref_create(env);
}

void *AXIS2_CALL
woden_desc_create_binding_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_binding_create(env);
}

void *AXIS2_CALL
woden_desc_create_binding_fault_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_binding_fault_create(env);
}

void *AXIS2_CALL
woden_desc_create_binding_op_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_binding_op_create(env);
}

void *AXIS2_CALL
woden_desc_create_binding_fault_ref_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_binding_fault_ref_create(env);
}

void *AXIS2_CALL
woden_desc_create_binding_msg_ref_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_binding_msg_ref_create(env);
}

void *AXIS2_CALL
woden_desc_create_svc_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_svc_create(env);
}

void *AXIS2_CALL
woden_desc_create_endpoint_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_endpoint_create(env);
}

void *AXIS2_CALL
woden_desc_create_feature_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_feature_create(env);
}

void *AXIS2_CALL
woden_desc_create_property_element(
    void *desc,
    const axutil_env_t *env)
{
    return woden_property_create(env);
}

axis2_status_t AXIS2_CALL
woden_desc_set_ext_registry(
    void *desc,
    const axutil_env_t *env,
    void *ext_reg)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ext_reg, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));
    if (desc_impl->f_ext_reg)
    {
        /* TODO */
    }

    desc_impl->f_ext_reg = ext_reg;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_desc_get_ext_registry(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return desc_impl->f_ext_reg;
}

void *AXIS2_CALL
woden_desc_to_component(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    if (AXIS2_TRUE != desc_impl->f_component_initialized)
        woden_desc_init_components(desc, env);
    return &(desc_impl->desc);
}
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

axis2_status_t AXIS2_CALL
woden_desc_add_to_all_interfaces(
    void *desc,
    const axutil_env_t *env,
    void *interface)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, interface, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return axutil_array_list_add(desc_impl->f_all_interfaces, env, interface);
}

axis2_status_t AXIS2_CALL
woden_desc_add_to_all_bindings(
    void *desc,
    const axutil_env_t *env,
    void *binding)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, binding, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return axutil_array_list_add(desc_impl->f_all_bindings, env, binding);
}

axis2_status_t AXIS2_CALL
woden_desc_add_to_all_svcs(
    void *desc,
    const axutil_env_t *env,
    void *svc)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    return axutil_array_list_add(desc_impl->f_all_svcs, env, svc);
}

axis2_status_t AXIS2_CALL
woden_desc_add_to_all_element_decls(
    void *desc,
    const axutil_env_t *env,
    void *element_decl)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element_decl, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    if (!desc_impl->f_all_element_decls)
    {
        desc_impl->f_all_element_decls = axutil_array_list_create(env, 0);
        if (!desc_impl->f_all_element_decls)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return axutil_array_list_add(desc_impl->f_all_element_decls, env, element_decl);
}

axis2_status_t AXIS2_CALL
woden_desc_add_to_all_type_defs(
    void *desc,
    const axutil_env_t *env,
    void *type_def)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type_def, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    if (!desc_impl->f_all_type_defs)
    {
        desc_impl->f_all_type_defs = axutil_array_list_create(env, 0);
        if (!desc_impl->f_all_type_defs)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }

    return axutil_array_list_add(desc_impl->f_all_type_defs, env, type_def);
}

static axis2_status_t AXIS2_CALL
woden_desc_init_components(
    void *desc,
    const axutil_env_t *env)
{
    woden_desc_impl_t *desc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_DESC_SUPER_OBJS(desc, env);
    desc_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_DESC", AXIS2_HASH_KEY_STRING));

    desc_impl->f_component_initialized = AXIS2_TRUE;
    woden_component_model_builder_create(env, &(desc_impl->desc));

    return AXIS2_SUCCESS;
}


