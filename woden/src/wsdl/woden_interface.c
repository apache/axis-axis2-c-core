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
#include <woden_interface.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <woden_interface_fault.h>

typedef struct woden_interface_impl woden_interface_impl_t;

/**
 * @brief Interface Struct Impl
 *   Axis2 Interface
 */
struct woden_interface_impl
{
    woden_interface_t interface;
    woden_configurable_t *configurable;
    woden_obj_types_t obj_type;
    axutil_hash_t *super;
    axutil_hash_t *methods;
    axutil_array_list_t *f_extended_interfaces;
    axutil_qname_t *f_qname;
    axutil_array_list_t *f_extends_qnames;
    axutil_array_list_t *f_style_default;
    axutil_array_list_t *f_interface_fault_elements;
    axutil_array_list_t *f_interface_op_elements;
};

#define INTF_TO_IMPL(interface) ((woden_interface_impl_t *) interface)

axis2_status_t AXIS2_CALL
woden_interface_free(
    void *interface,
    const axutil_env_t *env);

axutil_hash_t *AXIS2_CALL
woden_interface_super_objs(
    void *interface,
    const axutil_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_interface_type(
    void *interface,
    const axutil_env_t *env);

woden_configurable_t *AXIS2_CALL
woden_interface_get_base_impl(
    void *interface,
    const axutil_env_t *env);

/* ************************************************************
 *  Interface interface methods (the WSDL Component model)
 * ************************************************************/
axutil_qname_t *AXIS2_CALL
woden_interface_get_qname(
    void *interface,
    const axutil_env_t *env);

axutil_array_list_t *AXIS2_CALL
woden_interface_get_extended_interfaces(
    void *interface,
    const axutil_env_t *env);

axutil_array_list_t *AXIS2_CALL
woden_interface_get_interface_faults(
    void *interface,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_interface_get_interface_fault(
    void *interface,
    const axutil_env_t *env,
    axutil_qname_t *qname);

axutil_array_list_t *AXIS2_CALL
woden_interface_get_interface_ops(
    void *interface,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_interface_to_element(
    void *interface,
    const axutil_env_t *env);

/* ************************************************************
 *  InterfaceElement interface methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_interface_set_qname(
    void *interface,
    const axutil_env_t *env,
    axutil_qname_t *qname);

axis2_status_t AXIS2_CALL
woden_interface_add_style_default_uri(
    void *interface,
    const axutil_env_t *env,
    axutil_uri_t *uri);

void *AXIS2_CALL
woden_interface_get_style_default(
    void *interface,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_interface_add_extended_qname(
    void *interface,
    const axutil_env_t *env,
    woden_wsdl_element_t *qname);

axutil_array_list_t *AXIS2_CALL
woden_interface_get_extends_qnames(
    void *interface,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_interface_add_interface_fault_element(
    void *interface,
    const axutil_env_t *env,
    void *fault);

axutil_array_list_t *AXIS2_CALL
woden_interface_get_interface_fault_elements(
    void *interface,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_interface_add_interface_op_element(
    void *interface,
    const axutil_env_t *env,
    void *op);

axutil_array_list_t *AXIS2_CALL
woden_interface_get_interface_op_elements(
    void *interface,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_interface_add_extended_interface(
    void *interface,
    const axutil_env_t *env,
    void *extended_interface);


static woden_interface_t *
create(const axutil_env_t *env);

static axis2_status_t
woden_interface_free_ops(
    void *interface,
    const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_to_interface_element(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface)
    {
        interface_impl = (woden_interface_impl_t *) create(env);
    }
    else
        interface_impl = (woden_interface_impl_t *) interface;
	/*
    woden_interface_free_ops(interface, env);
	*/
    interface_impl->interface.base.interface_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_interface_element_ops_t));
    woden_interface_element_resolve_methods(&(interface_impl->interface.base.
            interface_element), env, interface_impl->methods);
    return interface;
}

AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_to_documentable(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface)
    {
        interface_impl = (woden_interface_impl_t *) create(env);
    }
    else
        interface_impl = (woden_interface_impl_t *) interface;

    woden_interface_free_ops(interface, env);

    interface_impl->interface.base.configurable.base.documentable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_ops_t));
    woden_documentable_resolve_methods(&(interface_impl->interface.base.
            configurable.base.documentable), env, NULL,
            interface_impl->methods);
    return interface;
}

AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_to_configurable(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface)
    {
        interface_impl = (woden_interface_impl_t *) create(env);
    }
    else
        interface_impl = (woden_interface_impl_t *) interface;

    woden_interface_free_ops(interface, env);

    interface_impl->interface.base.configurable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_configurable_ops_t));
    woden_configurable_resolve_methods(&(interface_impl->interface.base.
            configurable), env, interface_impl->configurable,
            interface_impl->methods);
    return interface;
}

AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_to_attr_extensible(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface)
    {
        interface_impl = (woden_interface_impl_t *) create(env);
    }
    else
        interface_impl = (woden_interface_impl_t *) interface;

    woden_interface_free_ops(interface, env);

    interface_impl->interface.base.interface_element.base.documentable_element.
    wsdl_element.base.attr_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(interface_impl->interface.base.
            interface_element.base.documentable_element.wsdl_element.base.
            attr_extensible), env, NULL, interface_impl->methods);
    return interface;
}


AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_to_element_extensible(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface)
    {
        interface_impl = (woden_interface_impl_t *) create(env);
    }
    else
        interface_impl = (woden_interface_impl_t *) interface;

    woden_interface_free_ops(interface, env);

    interface_impl->interface.base.interface_element.base.documentable_element.
    wsdl_element.base.element_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(interface_impl->interface.base.
            interface_element.base.documentable_element.wsdl_element.base.
            element_extensible), env, NULL, interface_impl->methods);
    return interface;
}

/************************End of Woden C Internal Methods***********************/
static woden_interface_t *
create(const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_interface_impl_t));

    interface_impl->obj_type = WODEN_INTERFACE;
    interface_impl->super = NULL;
    interface_impl->methods = NULL;
    interface_impl->f_qname = NULL;
    interface_impl->f_extended_interfaces = NULL;
    interface_impl->f_extends_qnames = NULL;
    interface_impl->f_style_default = NULL;
    interface_impl->f_interface_fault_elements = NULL;
    interface_impl->f_interface_op_elements = NULL;

    interface_impl->interface.base.interface_element.ops = NULL;
    interface_impl->interface.base.configurable.base.documentable.ops = NULL;
    interface_impl->interface.base.configurable.ops = NULL;
    interface_impl->interface.base.interface_element.base.documentable_element.
    wsdl_element.base.attr_extensible.ops = NULL;
    interface_impl->interface.base.interface_element.base.documentable_element.
    wsdl_element.base.element_extensible.ops = NULL;

    interface_impl->interface.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_interface_ops_t));

    interface_impl->interface.ops->free = woden_interface_free;
    interface_impl->interface.ops->super_objs = woden_interface_super_objs;
    interface_impl->interface.ops->type = woden_interface_type;
    interface_impl->interface.ops->get_base_impl = woden_interface_get_base_impl;

    interface_impl->interface.ops->get_qname = woden_interface_get_qname;
    interface_impl->interface.ops->get_extended_interfaces =
        woden_interface_get_extended_interfaces;
    interface_impl->interface.ops->get_interface_faults =
        woden_interface_get_interface_faults;
    interface_impl->interface.ops->get_interface_fault =
        woden_interface_get_interface_fault;
    interface_impl->interface.ops->get_interface_ops =
        woden_interface_get_interface_ops;
    interface_impl->interface.ops->to_element =
        woden_interface_to_element;

    interface_impl->methods = axutil_hash_make(env);
    if (!interface_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(interface_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_interface_free);
    axutil_hash_set(interface_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_interface_super_objs);
    axutil_hash_set(interface_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_interface_type);

    axutil_hash_set(interface_impl->methods, "get_qname",
            AXIS2_HASH_KEY_STRING,
            woden_interface_get_qname);
    axutil_hash_set(interface_impl->methods, "get_extended_interfaces",
            AXIS2_HASH_KEY_STRING,
            woden_interface_get_extended_interfaces);
    axutil_hash_set(interface_impl->methods, "get_interface_faults",
            AXIS2_HASH_KEY_STRING,
            woden_interface_get_interface_faults);
    axutil_hash_set(interface_impl->methods, "get_interface_fault",
            AXIS2_HASH_KEY_STRING,
            woden_interface_get_interface_fault);
    axutil_hash_set(interface_impl->methods, "get_interface_ops",
            AXIS2_HASH_KEY_STRING,
            woden_interface_get_interface_ops);
    axutil_hash_set(interface_impl->methods, "to_element",
            AXIS2_HASH_KEY_STRING,
            woden_interface_to_element);
    axutil_hash_set(interface_impl->methods, "set_qname",
            AXIS2_HASH_KEY_STRING,
            woden_interface_set_qname);
    axutil_hash_set(interface_impl->methods, "add_style_default_uri",
            AXIS2_HASH_KEY_STRING,
            woden_interface_add_style_default_uri);
    axutil_hash_set(interface_impl->methods, "get_style_default",
            AXIS2_HASH_KEY_STRING,
            woden_interface_get_style_default);
    axutil_hash_set(interface_impl->methods, "add_extends_qnames",
            AXIS2_HASH_KEY_STRING,
            woden_interface_get_extends_qnames);
    axutil_hash_set(interface_impl->methods, "get_extends_qnames",
            AXIS2_HASH_KEY_STRING,
            woden_interface_get_extends_qnames);
    axutil_hash_set(interface_impl->methods, "add_interface_fault_element",
            AXIS2_HASH_KEY_STRING,
            woden_interface_add_interface_fault_element);
    axutil_hash_set(interface_impl->methods, "get_interface_fault_elements",
            AXIS2_HASH_KEY_STRING,
            woden_interface_get_interface_fault_elements);
    axutil_hash_set(interface_impl->methods, "add_interface_op_element",
            AXIS2_HASH_KEY_STRING,
            woden_interface_add_interface_op_element);
    axutil_hash_set(interface_impl->methods, "get_interface_op_elements",
            AXIS2_HASH_KEY_STRING,
            woden_interface_get_interface_op_elements);
    axutil_hash_set(interface_impl->methods, "add_extended_interface",
            AXIS2_HASH_KEY_STRING,
            woden_interface_add_extended_interface);

    return &(interface_impl->interface);
}

AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_create(const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_impl = (woden_interface_impl_t *) create(env);

    interface_impl->configurable = woden_configurable_create(env);

    interface_impl->super = axutil_hash_make(env);
    if (!interface_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(interface_impl->super, "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING,
            &(interface_impl->interface));
    axutil_hash_set(interface_impl->super, "WODEN_CONFIGURABLE", AXIS2_HASH_KEY_STRING,
            interface_impl->configurable);

    return &(interface_impl->interface);
}

static axis2_status_t
woden_interface_free_ops(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_impl = INTF_TO_IMPL(interface);

    if (interface_impl->interface.base.interface_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_impl->interface.base.
                interface_element.ops);
        interface_impl->interface.base.interface_element.ops = NULL;
    }

    if (interface_impl->interface.base.interface_element.base.documentable_element.ops)
    {
		/*
        AXIS2_FREE(env->allocator, interface_impl->interface.base.
                interface_element.base.documentable_element.ops);
        */
		interface_impl->interface.base.interface_element.base.documentable_element.ops =
            NULL;
    }

    if (interface_impl->interface.base.configurable.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, interface_impl->interface.base.
                configurable.base.documentable.ops);
        interface_impl->interface.base.configurable.base.documentable.ops =
            NULL;
    }

    if (interface_impl->interface.base.configurable.ops)
    {
        AXIS2_FREE(env->allocator, interface_impl->interface.base.
                configurable.ops);
        interface_impl->interface.base.configurable.ops =
            NULL;
    }

    if (interface_impl->interface.base.interface_element.base.
            documentable_element.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, interface_impl->interface.base.
                interface_element.base.documentable_element.wsdl_element.base.
                attr_extensible.ops);
        interface_impl->interface.base.interface_element.base.
        documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    }

    if (interface_impl->interface.base.interface_element.base.
            documentable_element.wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, interface_impl->interface.base.
                interface_element.base.documentable_element.wsdl_element.base.
                element_extensible.ops);
        interface_impl->interface.base.interface_element.base.
        documentable_element.wsdl_element.base.element_extensible.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_interface_free(void *interface,
        const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_impl = INTF_TO_IMPL(interface);

    if (interface_impl->f_extended_interfaces)
    {
        axutil_array_list_free(interface_impl->f_extended_interfaces, env);
        interface_impl->f_extended_interfaces = NULL;
    }

    if (interface_impl->f_qname)
    {
        axutil_qname_free(interface_impl->f_qname, env);
        interface_impl->f_qname = NULL;
    }

    if (interface_impl->f_extends_qnames)
    {
        axutil_array_list_free(interface_impl->f_extends_qnames, env);
        interface_impl->f_extends_qnames = NULL;
    }

    if (interface_impl->f_style_default)
    {
        axutil_array_list_free(interface_impl->f_style_default, env);
        interface_impl->f_style_default = NULL;
    }

    if (interface_impl->f_interface_fault_elements)
    {
        axutil_array_list_free(interface_impl->f_interface_fault_elements, env);
        interface_impl->f_interface_fault_elements = NULL;
    }

    if (interface_impl->f_interface_op_elements)
    {
        axutil_array_list_free(interface_impl->f_interface_op_elements, env);
        interface_impl->f_interface_op_elements = NULL;
    }

    if (interface_impl->super)
    {
        axutil_hash_free(interface_impl->super, env);
        interface_impl->super = NULL;
    }

    if (interface_impl->methods)
    {
        axutil_hash_free(interface_impl->methods, env);
        interface_impl->methods = NULL;
    }

    if (interface_impl->configurable)
    {
        WODEN_CONFIGURABLE_FREE(interface_impl->configurable, env);
        interface_impl->configurable = NULL;
    }

    woden_interface_free_ops(interface, env);

    if ((&(interface_impl->interface))->ops)
    {
        AXIS2_FREE(env->allocator, (&(interface_impl->interface))->ops);
        (&(interface_impl->interface))->ops = NULL;
    }

    if (interface_impl)
    {
        AXIS2_FREE(env->allocator, interface_impl);
        interface_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axutil_hash_t *AXIS2_CALL
woden_interface_super_objs(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_impl = INTF_TO_IMPL(interface);

    return interface_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_interface_type(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_impl = INTF_TO_IMPL(interface);

    return interface_impl->obj_type;
}

woden_configurable_t *AXIS2_CALL
woden_interface_get_base_impl(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_impl = INTF_TO_IMPL(interface);

    return interface_impl->configurable;
}

axis2_status_t AXIS2_CALL
woden_interface_resolve_methods(
    woden_interface_t *interface,
    const axutil_env_t *env,
    woden_interface_t *interface_impl,
    axutil_hash_t *methods)
{
    woden_interface_impl_t *interface_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    interface_impl_l = INTF_TO_IMPL(interface_impl);

    interface->ops->free = axutil_hash_get(methods, "free",
                        AXIS2_HASH_KEY_STRING);
    interface->ops->super_objs = axutil_hash_get(methods, "super_objs",
                        AXIS2_HASH_KEY_STRING);
    interface->ops->type = axutil_hash_get(methods, "type",
                        AXIS2_HASH_KEY_STRING);

    interface->ops->get_qname = axutil_hash_get(methods,
                        "get_qname", AXIS2_HASH_KEY_STRING);
    if (!interface->ops->get_qname && interface_impl_l)
        interface->ops->get_qname =
                        interface_impl_l->interface.ops->get_qname;

    interface->ops->get_extended_interfaces = axutil_hash_get(methods,
                        "get_extended_interfaces", AXIS2_HASH_KEY_STRING);
    if (!interface->ops->get_extended_interfaces && interface_impl_l)
        interface->ops->get_extended_interfaces =
                        interface_impl_l->interface.ops->get_extended_interfaces;

    interface->ops->get_interface_faults = axutil_hash_get(methods,
                        "get_interface_faults", AXIS2_HASH_KEY_STRING);
    if (!interface->ops->get_interface_faults && interface_impl_l)
        interface->ops->get_interface_faults =
                        interface_impl_l->interface.ops->get_interface_faults;

    interface->ops->get_interface_fault = axutil_hash_get(methods,
                        "get_interface_fault", AXIS2_HASH_KEY_STRING);
    if (!interface->ops->get_interface_fault && interface_impl_l)
        interface->ops->get_interface_fault =
                        interface_impl_l->interface.ops->get_interface_fault;

    interface->ops->get_interface_ops = axutil_hash_get(methods,
                        "get_interface_ops", AXIS2_HASH_KEY_STRING);
    if (!interface->ops->get_interface_ops && interface_impl_l)
        interface->ops->get_interface_ops =
                        interface_impl_l->interface.ops->get_interface_ops;

    interface->ops->to_element = axutil_hash_get(methods,
                        "to_element", AXIS2_HASH_KEY_STRING);
    if (!interface->ops->to_element && interface_impl_l)
        interface->ops->to_element =
                        interface_impl_l->interface.ops->to_element;

    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Interface interface methods (the WSDL Component model)
 * ************************************************************/
axutil_qname_t *AXIS2_CALL
woden_interface_get_qname(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    return interface_impl->f_qname;
}

axutil_array_list_t *AXIS2_CALL
woden_interface_get_extended_interfaces(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    return interface_impl->f_extended_interfaces;
}

axutil_array_list_t *AXIS2_CALL
woden_interface_get_interface_faults(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    return interface_impl->f_interface_fault_elements;
}

void *AXIS2_CALL
woden_interface_get_interface_fault(
    void *interface,
    const axutil_env_t *env,
    axutil_qname_t *qname)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;
    int i = 0, size = 0;
    void *fault = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    size = axutil_array_list_size(interface_impl->f_interface_fault_elements, env);
    for (i = 0; i < size; i++)
    {
        void *flt = NULL;
        axutil_qname_t *qname_l = NULL;

        flt = axutil_array_list_get(interface_impl->f_interface_fault_elements,
                env, i);
        qname_l = (axutil_qname_t *) WODEN_INTERFACE_FAULT_GET_QNAME(flt, env);
        if (AXIS2_TRUE == axutil_qname_equals(qname, env, qname_l))
        {
            fault = flt;
            break;
        }
    }

    return fault;
}

axutil_array_list_t *AXIS2_CALL
woden_interface_get_interface_ops(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    return interface_impl->f_interface_op_elements;
}

void *AXIS2_CALL
woden_interface_to_element(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    return &(interface_impl->interface);
}

/* ************************************************************
 *  InterfaceElement interface methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_interface_set_qname(
    void *interface,
    const axutil_env_t *env,
    axutil_qname_t *qname)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    if (interface_impl->f_qname)
    {
        axutil_qname_free(interface_impl->f_qname, env);
    }
    interface_impl->f_qname = axutil_qname_clone(qname, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_interface_add_style_default_uri(
    void *interface,
    const axutil_env_t *env,
    axutil_uri_t *uri)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, uri, AXIS2_FAILURE);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    if (!interface_impl->f_style_default)
    {
        interface_impl->f_style_default = axutil_array_list_create(env, 0);
        if (!interface_impl->f_style_default)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axutil_array_list_add(interface_impl->f_style_default, env, axutil_uri_clone(
                uri, env));

    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_interface_get_style_default(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    return interface_impl->f_style_default;
}

axis2_status_t AXIS2_CALL
woden_interface_add_extended_qname(
    void *interface,
    const axutil_env_t *env,
    woden_wsdl_element_t *qname)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    if (!interface_impl->f_extends_qnames)
    {
        interface_impl->f_extends_qnames = axutil_array_list_create(env, 0);
        if (!interface_impl->f_extends_qnames)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axutil_array_list_add(interface_impl->f_extends_qnames, env, qname);

    return AXIS2_SUCCESS;
}

axutil_array_list_t *AXIS2_CALL
woden_interface_get_extends_qnames(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    return interface_impl->f_extends_qnames;
}

axis2_status_t AXIS2_CALL
woden_interface_add_interface_fault_element(
    void *interface,
    const axutil_env_t *env,
    void *fault)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault, AXIS2_FAILURE);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    if (!interface_impl->f_interface_fault_elements)
    {
        interface_impl->f_interface_fault_elements = axutil_array_list_create(env, 0);
        if (!interface_impl->f_interface_fault_elements)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axutil_array_list_add(interface_impl->f_interface_fault_elements, env, fault);

    return AXIS2_SUCCESS;
}

axutil_array_list_t *AXIS2_CALL
woden_interface_get_interface_fault_elements(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    return interface_impl->f_interface_fault_elements;
}

axis2_status_t AXIS2_CALL
woden_interface_add_interface_op_element(
    void *interface,
    const axutil_env_t *env,
    void *op)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    if (!interface_impl->f_interface_op_elements)
    {
        interface_impl->f_interface_op_elements = axutil_array_list_create(env, 0);
        if (!interface_impl->f_interface_op_elements)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axutil_array_list_add(interface_impl->f_interface_op_elements, env, op);

    return AXIS2_SUCCESS;
}

axutil_array_list_t *AXIS2_CALL
woden_interface_get_interface_op_elements(
    void *interface,
    const axutil_env_t *env)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    return interface_impl->f_interface_op_elements;
}

axis2_status_t AXIS2_CALL
woden_interface_add_extended_interface(
    void *interface,
    const axutil_env_t *env,
    void *extended_interface)
{
    woden_interface_impl_t *interface_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, interface, AXIS2_FAILURE);
    super = WODEN_INTERFACE_SUPER_OBJS(interface, env);
    interface_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_INTERFACE", AXIS2_HASH_KEY_STRING));

    if (!interface_impl->f_extended_interfaces)
    {
        interface_impl->f_extended_interfaces = axutil_array_list_create(env, 0);
        if (!interface_impl->f_extended_interfaces)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axutil_array_list_add(interface_impl->f_extended_interfaces, env, extended_interface);
    return AXIS2_SUCCESS;
}





