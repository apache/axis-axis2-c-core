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
#include <woden_wsdl10_reader.h>
#include <woden_schema_constants.h>
#include <woden_qname_util.h>

#include <woden_binding_fault.h>
#include <woden_binding_fault_ref.h>
#include <woden_wsdl10_binding_msg_ref.h>
#include <woden_wsdl10_binding_op.h>
#include <woden_binding.h>
#include <woden_configurable_component.h>
#include <woden_configurable.h>
#include <woden_wsdl10_desc.h>
#include <woden_documentable.h>
#include <woden_documentation.h>
#include <woden_element_decl.h>
#include <woden_wsdl10_endpoint.h>
#include <woden_feature.h>
#include <woden_import.h>
#include <woden_include.h>
#include <woden_wsdl10_interface_fault_ref.h>
#include <woden_wsdl10_msg_ref.h>
#include <woden_wsdl10_msg_ref_element.h>
#include <woden_wsdl10_part.h>
#include <woden_wsdl10_part_element.h>
#include <woden_interface.h>
#include <woden_wsdl10_interface_msg_ref.h>
#include <woden_interface_op.h>
#include <woden_nested_component.h>
#include <woden_nested_configurable.h>
#include <woden_property.h>
#include <woden_wsdl10_svc.h>
#include <woden_type_def.h>
#include <woden_types.h>
#include <woden_wsdl_component.h>
#include <woden_wsdl_ref.h>

#include <woden_attr_extensible.h>
#include <woden_component_exts.h>
#include <woden_element_extensible.h>
#include <woden_ext_element.h>
#include <woden_wsdl10_ext_registry.h>

#include <woden_direction.h>
#include <woden_msg_label.h>
#include <woden_soap_fault_code.h>
#include <woden_soap_fault_subcodes.h>

#include <woden_binding_element.h>
#include <woden_binding_fault_element.h>
#include <woden_binding_fault_ref_element.h>
#include <woden_wsdl10_binding_msg_ref_element.h>
#include <woden_wsdl10_binding_op_element.h>
#include <woden_configurable_element.h>
#include <woden_wsdl10_desc_element.h>
#include <woden_documentable_element.h>
#include <woden_documentation_element.h>
#include <woden_wsdl10_endpoint_element.h>
#include <woden_fault_ref_element.h>
#include <woden_feature_element.h>
#include <woden_import_element.h>
#include <woden_include_element.h>
#include <woden_interface_element.h>
#include <woden_wsdl10_interface_fault_ref_element.h>
#include <woden_wsdl10_interface_msg_ref_element.h>
#include <woden_interface_op_element.h>
#include <woden_nested_element.h>
#include <woden_property_element.h>
#include <woden_wsdl10_svc_element.h>
#include <woden_types_element.h>
#include <woden_wsdl_element.h>

#include <woden_bool_attr.h>
#include <woden_qname_attr.h>
#include <woden_qname_list_attr.h>
#include <woden_qname_list_or_token_any_attr.h>
#include <woden_qname_or_token_any_attr.h>
#include <woden_string_attr.h>
#include <woden_uri_attr.h>
#include <woden_xml_attr.h>

#include <woden_schema.h>
#include <woden_imported_schema.h>
#include <woden_inlined_schema.h>

#include <woden_wsdl10_ext_deserializer.h>

#include "../../wsdl10/woden_wsdl10_constants.h"
#include "../../util/woden_om_util.h"

#include <xml_schema.h>
#include <xml_schema_collection.h>

#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_attribute.h>
#include <axiom_child_element_iterator.h>
#include <axiom_namespace.h>
#include <axiom_text.h>
#include <axiom_document.h>
#include <axiom_stax_builder.h>
#include <axiom_xml_reader.h>
#include <axiom_util.h>
#include <axis2_utils.h>
#include <axis2_generic_obj.h>

#include <axis2_string_util.h>

typedef struct woden_wsdl10_reader_impl woden_wsdl10_reader_impl_t;

/**
 * @brief Woden Wsdl Reader Struct Impl
 *   Woden Wsdl Reader
 */
struct woden_wsdl10_reader_impl
{
    woden_wsdl10_reader_t reader;

    axiom_document_t *om_doc;
    axiom_node_t *root_node;
    /* A map of imported schema definitions keyed by schema location URI */
    axis2_hash_t *f_imported_schemas;
    woden_wsdl10_ext_registry_t *f_ext_reg;

};

#define INTF_TO_IMPL(reader) ((woden_wsdl10_reader_impl_t *) reader)

axis2_status_t AXIS2_CALL
woden_wsdl10_reader_free(
    void *reader,
    const axis2_env_t *env);

void *AXIS2_CALL
woden_wsdl10_reader_read_wsdl(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *root_node,
    const axis2_char_t *uri);

axis2_status_t AXIS2_CALL
woden_wsdl10_reader_set_ext_registry(
    void *reader,
    const axis2_env_t *env,
    woden_wsdl10_ext_registry_t *ext_reg);

woden_wsdl10_ext_registry_t *AXIS2_CALL
woden_wsdl10_reader_get_ext_registry(
    void *reader,
    const axis2_env_t *env);

/* Parse the attributes and child elements of the <description> element.
 * As per the WSDL 2.0 spec, the child elements must be in the
 * following order if present:
 * <documentation>
 * <import> <include> or WSDL extension elements in any order
 * <types>
 * <interface> <binding> <service> or WSDL extension elements in any order.
 * TODO validate that the elements are in correct order
 */
static void *
parse_desc(
    void *reader,
    const axis2_env_t *env,
    const axis2_char_t *document_base_uri,
    axiom_node_t *desc_el_node,
    axis2_hash_t *wsdl_modules);

static void *
parse_documentation(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *doc_el_node,
    void *desc);

static void *
parse_import(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *import_el_node,
    void *desc,
    axis2_hash_t *wsdl_modules);

static void *
parse_include(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *include_el_node,
    void *desc,
    axis2_hash_t *wsdl_modules);

/*
 * TODO Initial schema parsing is specific to XML Schema.
 * Need generic support for other type systems.
 * Consider extension architecture with serializer/deserializer.
 */

static void *
parse_types(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *types_el_node,
    void *desc);

static void *
parse_schema_inline(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *schema_el_node,
    void *desc);

/*
 * Parse the &lt;xs:import&gt; element and retrieve the imported
 * schema document if schemaLocation specified. Failure to retrieve
 * the schema will only matter if any WSDL components contain elements or
 * constraints that refer to the schema, and typically this will be
 * determined later by WSDL validation. So just report any such errors
 * and return the Schema Import object (i.e. with a null schema property).
 *
 * WSDL 2.0 spec validation:
 * - namespace attribute is REQUIRED
 * - imported schema MUST have a targetNamespace
 * - namespace and targetNamespace MUST be the same
 */
static void *
parse_schema_import(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *import_el_node,
    void *desc);

static void *
parse_part(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *part_el_node,
    void *desc,
    void *parent);

static void *
parse_msg_ref(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *msg_el_node,
    void *desc);

static void *
parse_interface(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *interface_el_node,
    void *desc);

static void *
parse_interface_op(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *op_el_node,
    void *desc,
    void *parent);

static void *
parse_interface_fault_ref(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *fault_ref_el_node,
    void *desc,
    void *parent);

static void *
parse_interface_msg_ref(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *msg_ref_el_node,
    void *desc,
    void *parent);

static void *
parse_binding(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *binding_el_node,
    void *desc);

static void *
parse_binding_op(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *op_el_node,
    void *desc,
    void *parent);

static void *
parse_binding_fault_ref(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *fault_ref_el_node,
    void *desc,
    void *parent);

static void *
parse_binding_msg_ref(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *msg_ref_el_node,
    void *desc,
    void *parent);

static void *
parse_svc(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *svc_el_node,
    void *desc);

static void *
parse_endpoint(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *endpoint_el_node,
    void *desc,
    void *parent);

static void *
parse_feature(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *feature_el_node,
    void *desc,
    void *parent);

/* ******************************************************************
 * Parse the attributes and child elements of the <property> element.
 *
 * As per WSDL 2.0 spec, they must be in the following order if present:
 * <documentation>
 * <value> or <constraint>
 * extension elements.
 *
 * TODO validate that the elements are in correct order
 *
 * Child elements may include either a <value> or a <constraint>,
 * but not both. If a <value> element is present, a <constraint>
 * may only be present if it contains the NMToken "#value", which
 * indicates that this <property> specifies a value, not a constraint.
 *
 * This method will parse all child elements of <property> without
 * checking for any erroneous use of  <value> and <constraint>.
 * This will be done later by validation, if it's enabled.
 * If the NMToken "#value" is present in a <constraint> element,
 * this will be flagged with a boolean field in PropertyElement
 * and the constraint field will be set to null.
 *
 */

static void *
parse_property(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *property_el_node,
    void *desc,
    void *parent);

/**
 * @param om_el_node element node
 * @param wsdl_class
 * @param wsdl_obj wsdl_obj_t type object
 * @param desc desc object
 */
static axis2_status_t
parse_ext_attributes(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *om_el_node,
    axis2_char_t *wsdl_class,
    void *wsdl_obj,
    void *desc);

static void *
parse_ext_element(
    void *reader,
    const axis2_env_t *env,
    axis2_char_t *parent_type,
    void *parent,
    axiom_node_t *el_node,
    void *desc);

/* ************************************************************
 *  Utility/helper methods
 * ************************************************************/

/**
 * Check the actual element encountered against the expected qname
 *
 * @param el actual element encountered
 * @param qname expected element's qname
 */
static axis2_status_t
check_element_qname(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *el_node,
    axis2_qname_t *qname);

/*
 * Retrieve a WSDL document by resolving the location URI specified
 * on a WSDL &lt;import&gt; or &lt;include&gt; element.
 *
 * TODO add support for a URL Catalog Resolver
 */
static void *
get_wsdl_from_location(
    void *reader,
    const axis2_env_t *env,
    axis2_char_t *location_uri_str,
    void *desc,
    axis2_hash_t *wsdl_modules);

/*
 * Convert a string of type xs:anyURI to a axis2_uri.
 * An empty string argument will return an empty string URI.
 * A null argument will return a null.
 */
static axis2_uri_t *
get_uri(
    const axis2_env_t *env,
    const axis2_char_t *uri_str);

static woden_wsdl10_reader_t *
create(
    const axis2_env_t *env)
{
    woden_wsdl10_reader_impl_t *reader_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    reader_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl10_reader_impl_t));

    reader_impl->f_imported_schemas = NULL;
    reader_impl->f_ext_reg = NULL;

    reader_impl->reader.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl10_reader_ops_t));

    reader_impl->f_imported_schemas = axis2_hash_make(env);
    if (!reader_impl->f_imported_schemas)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    reader_impl->reader.ops->free = woden_wsdl10_reader_free;
    reader_impl->reader.ops->read_wsdl = woden_wsdl10_reader_read_wsdl;
    reader_impl->reader.ops->set_ext_registry = woden_wsdl10_reader_set_ext_registry;
    reader_impl->reader.ops->get_ext_registry = woden_wsdl10_reader_get_ext_registry;

    return &(reader_impl->reader);
}

AXIS2_EXTERN woden_wsdl10_reader_t * AXIS2_CALL
woden_wsdl10_reader_create(
    const axis2_env_t *env)
{
    woden_wsdl10_reader_impl_t *reader_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    reader_impl = (woden_wsdl10_reader_impl_t *) create(env);

    return &(reader_impl->reader);
}

axis2_status_t AXIS2_CALL
woden_wsdl10_reader_free(
    void *reader,
    const axis2_env_t *env)
{
    woden_wsdl10_reader_impl_t *reader_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    reader_impl = INTF_TO_IMPL(reader);

    if (reader_impl->f_imported_schemas)
    {
        axis2_hash_free(reader_impl->f_imported_schemas, env);
        reader_impl->f_imported_schemas = NULL;
    }

    if (reader_impl->f_ext_reg)
    {
        WODEN_WSDL10_EXT_REGISTRY_FREE(reader_impl->f_ext_reg, env);
        reader_impl->f_ext_reg = NULL;
    }

    if ((&(reader_impl->reader))->ops)
    {
        AXIS2_FREE(env->allocator, (&(reader_impl->reader))->ops);
        (&(reader_impl->reader))->ops = NULL;
    }

    if (reader_impl)
    {
        AXIS2_FREE(env->allocator, reader_impl);
        reader_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  API public methods
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_wsdl10_reader_set_ext_registry(
    void *reader,
    const axis2_env_t *env,
    woden_wsdl10_ext_registry_t *ext_reg)
{
    woden_wsdl10_reader_impl_t *reader_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ext_reg, AXIS2_FAILURE);
    reader_impl = INTF_TO_IMPL(reader);

    if (reader_impl->f_ext_reg)
    {
        WODEN_WSDL10_EXT_REGISTRY_FREE(reader_impl->f_ext_reg, env);
        reader_impl->f_ext_reg = NULL;
    }
    reader_impl->f_ext_reg = ext_reg;
    return AXIS2_SUCCESS;
}

woden_wsdl10_ext_registry_t *AXIS2_CALL
woden_wsdl10_reader_get_ext_registry(
    void *reader,
    const axis2_env_t *env)
{
    woden_wsdl10_reader_impl_t *reader_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    reader_impl = INTF_TO_IMPL(reader);

    return reader_impl->f_ext_reg;
}

void *AXIS2_CALL
woden_wsdl10_reader_read_wsdl(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *root_node,
    const axis2_char_t *uri)
{
    woden_wsdl10_reader_impl_t *reader_impl = NULL;
    void *desc = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, root_node, NULL);
    AXIS2_PARAM_CHECK(env->error, uri, NULL);
    reader_impl = INTF_TO_IMPL(reader);

    /* TODO add WSDL locator for resolving URIs */
    reader_impl->root_node = root_node;
    if (!reader_impl->root_node)
        return NULL;
    desc = parse_desc(reader, env, uri, reader_impl->root_node, NULL);

    /* TODO Validate the model if validation is enabled. */

    return desc;
}

/* ************************************************************
 *  Parsing methods - e.g. parse_xxxx()
 * ************************************************************/

/* Parse the attributes and child elements of the <description> element.
 * As per the WSDL 2.0 spec, the child elements must be in the
 * following order if present:
 * <documentation>
 * <import> <include> or WSDL extension elements in any order
 * <types>
 * <interface> <binding> <service> or WSDL extension elements in any order.
 * TODO validate that the elements are in correct order
 */
static void *
parse_desc(
    void *reader,
    const axis2_env_t *env,
    const axis2_char_t *document_base_uri,
    axiom_node_t *desc_el_node,
    axis2_hash_t *wsdl_modules)
{
    woden_wsdl10_reader_impl_t *reader_impl = NULL;
    void *desc = NULL;
    void *ext_reg = NULL;
    axis2_uri_t *uri = NULL;
    axis2_char_t *target_namespc = NULL;
    axiom_element_t *desc_el = NULL;
    axis2_hash_t *attrs = NULL;
    axis2_hash_index_t *index = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    void *types = NULL;
    axis2_qname_t *qname = NULL;
    void *schema = NULL;
    axiom_attribute_t *attr_ns = NULL;
    axiom_attribute_t *attr_loc = NULL;
    axiom_element_t *schema_elem = NULL;
    axiom_node_t *schema_elem_node = NULL;
    axiom_namespace_t *schema_ns = NULL;
    void *documentable = NULL;
    void *wsdl_obj = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, document_base_uri, NULL);
    AXIS2_PARAM_CHECK(env->error, desc_el_node, NULL);
    reader_impl = INTF_TO_IMPL(reader);

    qname = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_DEFINITIONS);
    check_element_qname(reader, env, desc_el_node, qname);

    desc = woden_wsdl10_desc_create(env);
    ext_reg = woden_wsdl10_ext_registry_create(env);
    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    WODEN_WSDL10_DESC_ELEMENT_SET_EXT_REGISTRY(desc, env, ext_reg);

    if (!wsdl_modules)
    {
        /* This is the initial WSDL document. No imports or includes yet.
         * TODO this might be the place to flag the initial Desc if necessary.
         */
        wsdl_modules = axis2_hash_make(env);
    }
    ext_reg = woden_wsdl10_reader_get_ext_registry(reader, env);
    if (ext_reg)
    {
        WODEN_WSDL10_DESC_ELEMENT_SET_EXT_REGISTRY(desc, env, ext_reg);
    }

    uri = get_uri(env, document_base_uri);
    WODEN_WSDL10_DESC_ELEMENT_SET_DOCUMENT_BASE_URI(desc, env, uri);
    AXIS2_URI_FREE(uri, env);
    uri = NULL;

    desc_el = AXIOM_NODE_GET_DATA_ELEMENT(desc_el_node, env);
    target_namespc = axiom_element_get_attribute_value_by_name(desc_el,
            env, WODEN_WSDL10_ATTR_TARGET_NAMESPACE);

    if (target_namespc)
    {
        axis2_uri_t *target_namespc_uri = NULL;

        target_namespc_uri = get_uri(env, target_namespc);
        WODEN_WSDL10_DESC_ELEMENT_SET_TARGET_NAMESPACE(desc, env, target_namespc_uri);
        AXIS2_URI_FREE(target_namespc_uri, env);
    }

    /* parse the namespace declarations */
    attrs = axiom_element_get_all_attributes(desc_el, env);
    for (index = axis2_hash_first(attrs, env); index; index = axis2_hash_next(
                env, index))
    {
        void *v = NULL;
        axiom_attribute_t *attr = NULL;
        axiom_namespace_t *namespc = NULL;
        axis2_char_t *namespc_uri = NULL;
        axis2_char_t *local_part = NULL;
        axis2_char_t *value = NULL;

        axis2_hash_this(index, NULL, NULL, &v);
        attr = (axiom_attribute_t *) v;
        namespc = axiom_attribute_get_namespace(attr, env);
        if (!namespc)
            continue;
        namespc_uri = axiom_namespace_get_uri(namespc, env);
        local_part = axiom_attribute_get_localname(attr, env);
        value = axiom_attribute_get_value(attr, env);
        uri = get_uri(env, value);
        if (0 == AXIS2_STRCMP(WODEN_WSDL10_NS_URI_XMLNS, namespc_uri))
        {
            if (0 == AXIS2_STRCMP(WODEN_WSDL10_ATTR_XMLNS, local_part))
            {
                WODEN_WSDL10_DESC_ELEMENT_ADD_NAMESPACE(desc, env, local_part, uri);
                /* a prefixed namespace */
            }
            else
            {
                WODEN_WSDL10_DESC_ELEMENT_ADD_NAMESPACE(desc, env, NULL, uri);
            }
        }
        AXIS2_URI_FREE(uri, env);
        uri = NULL;

    }
    desc = woden_wsdl10_desc_to_attr_extensible(desc, env);
    documentable = WODEN_WSDL10_DESC_GET_BASE_IMPL(desc, env);
    wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
    status = parse_ext_attributes(reader, env, desc_el_node,
            "description_element", wsdl_obj, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    /* Parse the child elements */
    temp_el = axiom_util_get_first_child_element(desc_el, env, desc_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_import = NULL;
        axis2_qname_t *q_elem_include = NULL;
        axis2_qname_t *q_elem_types = NULL;
        axis2_qname_t *q_elem_msg_ref = NULL;
        axis2_qname_t *q_elem_interface = NULL;
        axis2_qname_t *q_elem_binding = NULL;
        axis2_qname_t *q_elem_svc = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_DOCUMENTATION);
        q_elem_import = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_IMPORT);
        q_elem_include = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_INCLUDE);
        q_elem_types = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_TYPES);
        q_elem_msg_ref = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_MSG_REF);
        q_elem_interface = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_INTERFACE);
        q_elem_binding = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_BINDING);
        q_elem_svc = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_SERVICE);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);

            desc = woden_wsdl10_desc_to_documentable(desc, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(desc, env, documentation);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_import, temp_el_node))
        {
            void *import_element = NULL;

            if (document_base_uri && AXIS2_TRUE !=
                    axis2_hash_contains_key(wsdl_modules, env,
                            document_base_uri))
            {
                axis2_hash_set(wsdl_modules, document_base_uri,
                        AXIS2_HASH_KEY_STRING, desc);
            }
            import_element = parse_import(reader, env, temp_el_node, desc,
                    wsdl_modules);
            desc = woden_wsdl10_desc_to_desc_element(desc, env);
            WODEN_WSDL10_DESC_ELEMENT_ADD_IMPORT_ELEMENT(desc, env, import_element);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_include, temp_el_node))
        {
            void *include_element = NULL;

            if (document_base_uri && AXIS2_TRUE !=
                    axis2_hash_contains_key(wsdl_modules, env,
                            document_base_uri))
            {
                axis2_hash_set(wsdl_modules, document_base_uri,
                        AXIS2_HASH_KEY_STRING, desc);
            }
            include_element = parse_include(reader, env, temp_el_node, desc,
                    wsdl_modules);
            WODEN_WSDL10_DESC_ELEMENT_ADD_INCLUDE_ELEMENT(desc, env, include_element);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_types, temp_el_node))
        {
            void *types = NULL;

            types = parse_types(reader, env, temp_el_node, desc);
            WODEN_WSDL10_DESC_ELEMENT_SET_TYPES_ELEMENT(desc, env, types);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_msg_ref, temp_el_node))
        {
            void *msg_ref = NULL;

            msg_ref = parse_msg_ref(reader, env, temp_el_node, desc);
            WODEN_WSDL10_DESC_ELEMENT_ADD_MSG_ELEMENT(desc, env, msg_ref);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_interface, temp_el_node))
        {
            void *interface = NULL;

            interface = parse_interface(reader, env, temp_el_node, desc);
            WODEN_WSDL10_DESC_ELEMENT_ADD_INTERFACE_ELEMENT(desc, env, interface);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_binding, temp_el_node))
        {
            void *binding = NULL;

            binding = parse_binding(reader, env, temp_el_node, desc);
            WODEN_WSDL10_DESC_ELEMENT_ADD_BINDING_ELEMENT(desc, env, binding);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_svc, temp_el_node))
        {
            void *svc = NULL;

            svc = parse_svc(reader, env, temp_el_node, desc);
            WODEN_WSDL10_DESC_ELEMENT_ADD_SVC_ELEMENT(desc, env, svc);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "desc_element", desc, temp_el_node, desc);
            if (ext_element)
            {
                void *documentable = NULL;
                void *wsdl_obj = NULL;
                void *wsdl_el = NULL;

                documentable = WODEN_WSDL10_DESC_GET_BASE_IMPL(desc, env);
                wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
                wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
                wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
                WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
            }
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    /*
     * Parse the schema for schema to include the built in schema types in the Woden model.
     * TODO: As there are a finite number of built in schema types it may be better to create
     * constants rather than reading the schema for schema on the creation of every model. 
     * Also, this method currently requires that the schema elements exist in the types element.
     * This may not be the best idea as it may imply that this schema contains an actual import
     * statement in a WSDL 2.0 document. This method also does not work for when building the
     * model programmatically.
     * This method should be reevaluated at a later point.
     */
    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    types = WODEN_WSDL10_DESC_ELEMENT_GET_TYPES_ELEMENT(desc, env);
    if (!types)
    {

        types = WODEN_WSDL10_DESC_ELEMENT_CREATE_TYPES_ELEMENT(desc, env);
        types = woden_types_to_types_element(types, env);
        WODEN_TYPES_ELEMENT_SET_TYPE_SYSTEM(types, env, WODEN_WSDL10_TYPE_XSD_2001);
        WODEN_WSDL10_DESC_ELEMENT_SET_TYPES_ELEMENT(desc, env, types);
    }
    schema_ns = axiom_namespace_create(env,
            "http://www.w3.org/2001/XMLSchema", "import");
    schema_elem = axiom_element_create(env, desc_el_node, "import",
            schema_ns, &schema_elem_node);
    attr_ns = axiom_attribute_create(env, "namespace",
            "http://www.w3.org/2001/XMLSchema", NULL);
    attr_loc = axiom_attribute_create(env, "schemaLocation",
            "http://www.w3.org/2001/XMLSchema.xsd", NULL);
    axiom_element_add_attribute(schema_elem, env, attr_ns, schema_elem_node);
    axiom_element_add_attribute(schema_elem, env, attr_loc, schema_elem_node);

    schema = parse_schema_import(reader, env, schema_elem_node, desc);
    if (schema)
    {
        types = woden_types_to_types_element(types, env);
        schema = woden_imported_schema_to_schema(schema, env);
        WODEN_TYPES_ELEMENT_ADD_SCHEMA(types, env, schema);
    }
    return desc;
}

static void *
parse_documentation(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *doc_el_node,
    void *desc)
{
    void *documentation = NULL;
    axiom_element_t *doc_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    documentation = WODEN_WSDL10_DESC_ELEMENT_CREATE_DOCUMENTATION_ELEMENT(desc,
            env);

    /* TODO store doc_el as below, or just extract any text? */
    documentation = woden_documentation_to_documentation_element(
                documentation, env);
    WODEN_DOCUMENTATION_ELEMENT_SET_CONTENT(documentation, env, doc_el_node);

    /* Now parse any extensibility attributes or elements */
    documentation = woden_documentation_to_attr_extensible(documentation,
            env);
    status = parse_ext_attributes(reader, env, doc_el_node, "documentation_element",
            documentation, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    doc_el = AXIOM_NODE_GET_DATA_ELEMENT(doc_el_node, env);
    temp_el = axiom_util_get_first_child_element(doc_el, env, doc_el_node,
            &temp_el_node);

    while (temp_el)
    {
        void *ext_el = NULL;
        void *wsdl_el = NULL;

        ext_el = parse_ext_element(reader, env, "documentation_element",
                documentation, temp_el_node, desc);

        wsdl_el = WODEN_DOCUMENTATION_GET_BASE_IMPL(documentation, env);
        wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
        WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env,
                ext_el);
        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return documentation;
}

static void *
parse_import(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *import_el_node,
    void *desc,
    axis2_hash_t *wsdl_modules)
{
    void *imp = NULL;
    axiom_element_t *import_el = NULL;
    axis2_char_t *namespc_uri = NULL;
    axis2_char_t *location_uri = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    imp = WODEN_WSDL10_DESC_ELEMENT_CREATE_IMPORT_ELEMENT(desc, env);
    import_el = AXIOM_NODE_GET_DATA_ELEMENT(import_el_node, env);

    namespc_uri = axiom_element_get_attribute_value_by_name(import_el, env,
            WODEN_WSDL10_ATTR_NAMESPACE);
    location_uri = axiom_element_get_attribute_value_by_name(import_el, env,
            WODEN_WSDL10_ATTR_LOCATION);

    imp = woden_import_to_attr_extensible(imp, env);
    status = parse_ext_attributes(reader, env, import_el_node, "import_element", imp, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    if (namespc_uri)
    {
        axis2_uri_t *uri = NULL;

        /* TODO handle missing namespace attribute (REQUIRED attr) */
        uri = get_uri(env, namespc_uri);
        WODEN_IMPORT_SET_NAMESPACE(imp, env, uri);
        AXIS2_URI_FREE(uri, env);
    }

    if (location_uri)
    {
        axis2_uri_t *uri = NULL;
        void *imported_desc = NULL;

        /* TODO handle missing locationURI (OPTIONAL attr) */
        imp = woden_import_to_import_element(imp, env);
        uri = get_uri(env, location_uri);
        WODEN_IMPORT_ELEMENT_SET_LOCATION(imp, env, uri);
        AXIS2_URI_FREE(uri, env);

        imported_desc = get_wsdl_from_location(reader, env, location_uri, desc,
                wsdl_modules);
        WODEN_IMPORT_ELEMENT_SET_DESC_ELEMENT(imp, env, imported_desc);
    }

    return imp;
}

static void *
parse_include(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *include_el_node,
    void *desc,
    axis2_hash_t *wsdl_modules)
{
    void *include = NULL;
    axiom_element_t *include_el = NULL;
    axis2_char_t *location_uri = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    include = WODEN_WSDL10_DESC_ELEMENT_CREATE_INCLUDE_ELEMENT(desc, env);
    include_el = AXIOM_NODE_GET_DATA_ELEMENT(include_el_node, env);

    location_uri = axiom_element_get_attribute_value_by_name(include_el, env,
            WODEN_WSDL10_ATTR_LOCATION);

    include = woden_include_to_attr_extensible(include, env);
    status = parse_ext_attributes(reader, env, include_el_node, "include_element",
            include, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    if (location_uri)
    {
        axis2_uri_t *uri = NULL;
        void *included_desc = NULL;

        uri = get_uri(env, location_uri);
        include = woden_include_to_include_element(include, env);
        WODEN_INCLUDE_ELEMENT_SET_LOCATION(include, env, uri);
        AXIS2_URI_FREE(uri, env);

        included_desc = get_wsdl_from_location(reader, env, location_uri, desc,
                wsdl_modules);
        WODEN_INCLUDE_ELEMENT_SET_DESC_ELEMENT(include, env, included_desc);
    }

    return include;
}

/*
 * TODO Initial schema parsing is specific to XML Schema.
 * Need generic support for other type systems.
 * Consider extension architecture with serializer/deserializer.
 */

static void *
parse_types(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *types_el_node,
    void *desc)
{
    void *types = NULL;
    axiom_element_t *types_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    types = WODEN_WSDL10_DESC_ELEMENT_CREATE_TYPES_ELEMENT(desc, env);
    types = woden_types_to_types_element(types, env);
    WODEN_TYPES_ELEMENT_SET_TYPE_SYSTEM(types, env, WODEN_WSDL10_TYPE_XSD_2001);

    types = woden_types_to_attr_extensible(types, env);
    status = parse_ext_attributes(reader, env, types_el_node, "types_element",
            types, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    types_el = AXIOM_NODE_GET_DATA_ELEMENT(types_el_node, env);
    temp_el = axiom_util_get_first_child_element(types_el, env, types_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_temp_el_type = NULL;

        q_temp_el_type = axis2_qname_util_new_qname(env, temp_el_node);
        q_elem_documentation = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_DOCUMENTATION);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            types = woden_types_to_documentable(types, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(types, env, documentation);
        }
        if (woden_schema_constants_compare_import(q_temp_el_type, env))
        {
            void *schema = NULL;

            schema = parse_schema_import(reader, env, temp_el_node, desc);
            if (schema)
            {
                types = woden_types_to_types_element(types, env);
                schema = woden_imported_schema_to_schema(schema, env);
                WODEN_TYPES_ELEMENT_ADD_SCHEMA(types, env, schema);
            }
        }
        if (woden_schema_constants_compare_schema(q_temp_el_type, env))
        {
            void *schema = NULL;

            schema = parse_schema_inline(reader, env, temp_el_node, desc);
            if (schema)
            {
                types = woden_types_to_types_element(types, env);
                schema = woden_inlined_schema_to_schema(schema, env);
                WODEN_TYPES_ELEMENT_ADD_SCHEMA(types, env, schema);
            }
        }
        else
        {
            void *ext_element = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "types_element",
                    types, temp_el_node, desc);

            documentable = WODEN_TYPES_GET_BASE_IMPL(types, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return types;
}

static void *
parse_schema_inline(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *schema_el_node,
    void *desc)
{
    void *schema = NULL;
    axis2_char_t *attr_id = NULL;
    axis2_char_t *tns = NULL;
    axis2_uri_t *base_uri = NULL;
    axis2_char_t *base_uri_str = NULL;
    xml_schema_t *schema_def = NULL;
    xml_schema_collection_t *xsc = NULL;
    axiom_element_t *schema_el = NULL;

    schema = woden_inlined_schema_create(env);
    schema_el = AXIOM_NODE_GET_DATA_ELEMENT(schema_el_node, env);
    attr_id = axiom_element_get_attribute_value_by_name(schema_el, env,
            WODEN_WSDL10_ATTR_ID);
    WODEN_INLINED_SCHEMA_SET_ID(schema, env, attr_id);
    tns = axiom_element_get_attribute_value_by_name(schema_el, env,
            WODEN_WSDL10_ATTR_TARGET_NAMESPACE);

    if (tns)
    {
        void *base = WODEN_INLINED_SCHEMA_GET_BASE_IMPL(schema, env);
        axis2_uri_t *uri = get_uri(env, tns);
        WODEN_SCHEMA_SET_NAMESPACE(base, env, uri);
        AXIS2_URI_FREE(uri, env);
    }
    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    base_uri = WODEN_WSDL10_DESC_ELEMENT_GET_DOCUMENT_BASE_URI(desc, env);
    if (base_uri)
        base_uri_str = AXIS2_URI_TO_STRING(base_uri, env,
                AXIS2_URI_UNP_OMITUSERINFO);

    xsc = xml_schema_collection_create(env);
    /* TODO Temporarily assume that imported schemas are stored as files
     * with the url as file name
     */
    schema_def = XML_SCHEMA_COLLECTION_READ_ELEMENT_WITH_URI(xsc,
            env, schema_el_node, base_uri_str);
    if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
    {
        return NULL;
    }
    if (schema_def)
    {
        void *base_schema = NULL;

        base_schema = WODEN_INLINED_SCHEMA_GET_BASE_IMPL(schema, env);
        WODEN_SCHEMA_SET_REFERENCEABLE(base_schema, env, AXIS2_TRUE);
        WODEN_SCHEMA_SET_SCHEMA_DEF(base_schema, env, schema_def);
    }
    else
    {
        void *base_schema = NULL;

        base_schema = WODEN_INLINED_SCHEMA_GET_BASE_IMPL(schema, env);
        WODEN_SCHEMA_SET_REFERENCEABLE(base_schema, env, AXIS2_FALSE);
    }

    return schema;
}

/*
 * Parse the &lt;xs:import&gt; element and retrieve the imported
 * schema document if schemaLocation specified. Failure to retrieve
 * the schema will only matter if any WSDL components contain elements or
 * constraints that refer to the schema, and typically this will be
 * determined later by WSDL validation. So just report any such errors
 * and return the Schema Import object (i.e. with a null schema property).
 *
 * WSDL 2.0 spec validation:
 * - namespace attribute is REQUIRED
 * - imported schema MUST have a targetNamespace
 * - namespace and targetNamespace MUST be the same
 */
static void *
parse_schema_import(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *import_el_node,
    void *desc)
{
    woden_wsdl10_reader_impl_t *reader_impl = NULL;
    void *schema = NULL;
    axis2_char_t *ns = NULL;
    axis2_char_t *sloc = NULL;
    axis2_char_t *schema_uri = NULL;
    axis2_uri_t *uri = NULL;
    void *schema_def = NULL;
    axis2_uri_t *context_uri = NULL;
    axis2_char_t *schema_loc = NULL;
    axiom_element_t *import_el = NULL;
    void *base_schema = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, desc, NULL);
    reader_impl = INTF_TO_IMPL(reader);

    schema = woden_imported_schema_create(env);
    base_schema = WODEN_IMPORTED_SCHEMA_GET_BASE_IMPL(schema, env);
    if (!schema)
        return NULL;
    import_el = AXIOM_NODE_GET_DATA_ELEMENT(import_el_node, env);
    ns = axiom_element_get_attribute_value_by_name(import_el, env,
            WODEN_WSDL10_ATTR_NAMESPACE);
    if (ns)
    {
        uri = get_uri(env, ns);
        WODEN_SCHEMA_SET_NAMESPACE(base_schema, env, uri);
        AXIS2_URI_FREE(uri, env);
        uri = NULL;
    }
    sloc = axiom_element_get_attribute_value_by_name(import_el, env,
            WODEN_ATTR_SCHEMA_LOCATION);
    if (sloc)
    {
        uri = get_uri(env, sloc);
        WODEN_IMPORTED_SCHEMA_SET_LOCATION(schema, env, uri);
        AXIS2_URI_FREE(uri, env);
        uri = NULL;
    }

    if (!WODEN_SCHEMA_GET_NAMESPACE(base_schema, env))
    {

        /* The namespace attribute is REQUIRED on xs:import, so don't continue. */
        WODEN_SCHEMA_SET_REFERENCEABLE(base_schema, env, AXIS2_FALSE);
        return schema;
    }

    if (!WODEN_IMPORTED_SCHEMA_GET_LOCATION(schema, env))
    {
        /* This is a namespace-only import, no schema document to be retrieved so don't continue.*/

        /* TODO investigate whether/how to try to resolve the imported namespace to known schema
         * components from that namespace (e.g. via a URI catalog resolver). Currently, any attempt
         * to resolve a QName against schema components from this namespace will search ALL
         * schemas imported from this namespace (see methods in TypesImpl).
         */

        return schema;
    }

    /* Now try to retrieve the schema import using schemaLocation */
    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    context_uri = WODEN_WSDL10_DESC_ELEMENT_GET_DOCUMENT_BASE_URI(desc, env);
    uri = WODEN_IMPORTED_SCHEMA_GET_LOCATION(schema, env);
    schema_loc = AXIS2_URI_TO_STRING(uri, env, AXIS2_URI_UNP_OMITUSERINFO);
    uri = axis2_uri_parse_relative(env, context_uri, schema_loc);
    if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
    {
        void *base_schema = NULL;

        base_schema = WODEN_IMPORTED_SCHEMA_GET_BASE_IMPL(schema, env);
        /* can't continue schema retrieval with a bad URL.*/
        WODEN_SCHEMA_SET_REFERENCEABLE(base_schema, env, AXIS2_FALSE);
        return schema;
    }

    schema_uri = AXIS2_URI_TO_STRING(uri, env, AXIS2_URI_UNP_OMITUSERINFO);

    /* If the schema has already been imported, reuse it. */
    schema_def = axis2_hash_get(reader_impl->f_imported_schemas, schema_uri,
            AXIS2_HASH_KEY_STRING);
    if (schema_def)
    {
        /* Not previously imported, so retrieve it now. */
        xml_schema_collection_t *schema_col = NULL;
        axiom_xml_reader_t *xml_reader = NULL;
        axiom_document_t *imported_schema_doc = NULL;
        axiom_stax_builder_t *xml_builder = NULL;

        schema_col = xml_schema_collection_create(env);
        /* TODO Temporarily assume that imported schemas are stored as files
         * with the url as file name
         */
        xml_reader = axiom_xml_reader_create_for_file(env, schema_uri, NULL);
        xml_builder = axiom_stax_builder_create(env, reader);
        imported_schema_doc = axiom_document_create(env, NULL, xml_builder);
        schema_def = XML_SCHEMA_COLLECTION_READ_DOCUMENT(schema_col,
                env, imported_schema_doc);
        if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
        {
            void *base_schema = NULL;

            base_schema = WODEN_IMPORTED_SCHEMA_GET_BASE_IMPL(schema, env);
            WODEN_SCHEMA_SET_REFERENCEABLE(base_schema, env, AXIS2_FALSE);
            return schema;
        }
        axis2_hash_set(reader_impl->f_imported_schemas, schema_uri,
                AXIS2_HASH_KEY_STRING, schema_def);

    }

    if (schema_def)
    {
        void *base_schema = NULL;

        base_schema = WODEN_IMPORTED_SCHEMA_GET_BASE_IMPL(schema, env);
        WODEN_SCHEMA_SET_SCHEMA_DEF(base_schema, env, schema_def);
    }
    else
    {
        void *base_schema = NULL;

        base_schema = WODEN_IMPORTED_SCHEMA_GET_BASE_IMPL(schema, env);
        WODEN_SCHEMA_SET_REFERENCEABLE(base_schema, env, AXIS2_FALSE);
    }

    return schema;
}

static void *
parse_msg_ref(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *msg_el_node,
    void *desc)
{
    void *msg = NULL;
    axis2_char_t *name = NULL;
    axiom_element_t *msg_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    msg = WODEN_WSDL10_DESC_ELEMENT_CREATE_MSG_ELEMENT(desc, env);

    msg_el = AXIOM_NODE_GET_DATA_ELEMENT(msg_el_node, env);
    name = axiom_element_get_attribute_value_by_name(msg_el, env, WODEN_WSDL10_ATTR_NAME);

    if (name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        msg = woden_wsdl10_msg_ref_to_msg_ref_element(msg, env);
        WODEN_WSDL10_MSG_REF_ELEMENT_SET_QNAME(msg, env, qname);
    }

    /* TODO extends attribute */

    msg = woden_wsdl10_msg_ref_to_attr_extensible(msg, env);
    status = parse_ext_attributes(reader, env, msg_el_node, "msg_element",
            msg, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    /* Parse the child elements of <msg>.
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <part> or extension elements in any order
     * TODO validate that the elements are in correct order
     */
    temp_el = axiom_util_get_first_child_element(msg_el, env, msg_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_part = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_DOCUMENTATION);
        q_elem_part = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_PART);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            msg = woden_wsdl10_msg_ref_to_documentable(msg, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(msg, env, documentation);
        }
        if (axis2_qname_util_matches(env,
                q_elem_part, temp_el_node))
        {
            void *part = NULL;

            part = parse_part(reader, env, temp_el_node, desc,
                    msg);
            msg = woden_wsdl10_msg_ref_to_msg_ref_element(msg, env);
            WODEN_WSDL10_MSG_REF_ELEMENT_SET_PART(msg, env, part);
        }
        else
        {
            void *ext_element = NULL;
            void *nested_confble = NULL;
            void *configurable = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "msg_element",
                    msg, temp_el_node, desc);

            nested_confble = WODEN_WSDL10_MSG_REF_GET_BASE_IMPL(msg, env);
            configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(nested_confble, env);
            documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return msg;
}

static void *
parse_part(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *part_el_node,
    void *desc,
    void *parent)
{
    void *part = NULL;
    axis2_char_t *element = NULL;
    axis2_char_t *name = NULL;
    axiom_element_t *part_el;
    axiom_element_t *temp_el;
    axiom_node_t *temp_el_node;
    void *types = NULL;
    void *nested_configurable = NULL;
    axis2_char_t *localname = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    part = WODEN_WSDL10_DESC_ELEMENT_CREATE_PART_ELEMENT(desc,
            env);
    nested_configurable = WODEN_WSDL10_PART_GET_BASE_IMPL(part, env);
    nested_configurable = woden_nested_configurable_to_nested_element(
                nested_configurable, env);
    WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(nested_configurable, env, parent);
    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    types = WODEN_WSDL10_DESC_ELEMENT_GET_TYPES_ELEMENT(desc, env);
    WODEN_WSDL10_PART_SET_TYPES(part, env, types);

    part_el = AXIOM_NODE_GET_DATA_ELEMENT(part_el_node, env);
    localname = axiom_element_get_localname(part_el, env);
    if (0 != AXIS2_STRCMP(WODEN_WSDL10_ELEM_PART, localname))
    {
        /* TODO Set No element error */
        return NULL;
    }
    name = axiom_element_get_attribute_value_by_name(part_el, env, WODEN_WSDL10_ATTR_NAME);

    if (name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        part = woden_wsdl10_part_to_part_element(part, env);
        WODEN_WSDL10_PART_ELEMENT_SET_QNAME(part, env, qname);
    }
    element = axiom_element_get_attribute_value_by_name(part_el, env, WODEN_WSDL10_ATTR_ELEMENT);

    if (element)
    {
        axis2_qname_t *qname = NULL;
        axis2_hash_t *namespcs = NULL;

        desc = woden_wsdl10_desc_to_desc_element(desc, env);
        namespcs = WODEN_WSDL10_DESC_ELEMENT_GET_NAMESPACES(desc, env);

        qname = woden_om_util_get_qname(env, part_el_node, element, namespcs);
        part = woden_wsdl10_part_to_part_element(part, env);
        WODEN_WSDL10_PART_ELEMENT_SET_ELEMENT_QNAME(part,
                env, qname);
    }

    part = woden_wsdl10_part_to_attr_extensible(part, env);
    status = parse_ext_attributes(reader, env, part_el_node, "interface_part_element",
            part, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    temp_el = axiom_util_get_first_child_element(part_el, env, part_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_DOCUMENTATION);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            part = woden_wsdl10_part_to_documentable(part, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(part, env, documentation);
        }
        else
        {
            void *ext_element = NULL;
            void *nested_confble = NULL;
            void *configurable = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "interface_part_element",
                    part, temp_el_node, desc);

            nested_confble = WODEN_WSDL10_PART_GET_BASE_IMPL(part, env);
            configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(nested_confble, env);
            documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return part;
}

static void *
parse_interface(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *interface_el_node,
    void *desc)
{
    void *intface = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *style_default = NULL;
    axiom_element_t *interface_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    intface = WODEN_WSDL10_DESC_ELEMENT_CREATE_INTERFACE_ELEMENT(desc, env);

    interface_el = AXIOM_NODE_GET_DATA_ELEMENT(interface_el_node, env);
    name = axiom_element_get_attribute_value_by_name(interface_el, env, WODEN_WSDL10_ATTR_NAME);

    if (name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        intface = woden_interface_to_interface_element(intface, env);
        WODEN_INTERFACE_ELEMENT_SET_QNAME(intface, env, qname);
    }

    style_default = axiom_element_get_attribute_value_by_name(interface_el,
            env, WODEN_WSDL10_ATTR_STYLE_DEFAULT);

    if (style_default)
    {
        axis2_array_list_t *string_list = NULL;
        axis2_char_t *uri_str = NULL;
        int i = 0, size = 0;

        string_list = axis2_tokenize(env, style_default, ' ');
        if (string_list)
            size = axis2_array_list_size(string_list, env);
        for (i = 0; i < size; i++)
        {
            axis2_uri_t *uri = NULL;
            uri_str = axis2_array_list_get(string_list, env, i);
            intface = woden_interface_to_interface_element(intface, env);
            uri = get_uri(env, uri_str);
            WODEN_INTERFACE_ELEMENT_ADD_STYLE_DEFAULT_URI(intface, env, uri);
            AXIS2_URI_FREE(uri, env);
        }
    }

    /* TODO extends attribute */

    intface = woden_interface_to_attr_extensible(intface, env);
    status = parse_ext_attributes(reader, env, interface_el_node, "interface_element",
            intface, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    /* Parse the child elements of <interface>.
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <fault> <operation> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */
    temp_el = axiom_util_get_first_child_element(interface_el, env, interface_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_op = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_DOCUMENTATION);
        q_elem_op = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_OPERATION);
        q_elem_feature = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_PROPERTY);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            intface = woden_interface_to_documentable(intface, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(intface, env, documentation);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_op, temp_el_node))
        {
            void *intface_op = NULL;

            intface_op = parse_interface_op(reader, env, temp_el_node, desc,
                    intface);
            intface = woden_interface_to_interface_element(intface, env);
            WODEN_INTERFACE_ELEMENT_ADD_INTERFACE_OP_ELEMENT(intface, env, intface_op);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_feature, temp_el_node))
        {
            void *feature = NULL;
            void *configurable = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, intface);
            configurable = WODEN_INTERFACE_GET_BASE_IMPL(intface, env);
            configurable = woden_configurable_to_configurable_element(configurable, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(configurable, env, feature);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_property, temp_el_node))
        {
            void *property = NULL;
            void *configurable = NULL;

            property = parse_property(reader, env, temp_el_node, desc, intface);
            configurable = WODEN_INTERFACE_GET_BASE_IMPL(intface, env);
            configurable = woden_configurable_to_configurable_element(configurable, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(configurable, env, property);
        }
        else
        {
            void *ext_element = NULL;
            void *configurable = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "interface_element",
                    intface, temp_el_node, desc);

            configurable = WODEN_INTERFACE_GET_BASE_IMPL(intface, env);
            documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return intface;
}

static void *
parse_interface_op(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *op_el_node,
    void *desc,
    void *parent)
{
    void *op = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *style = NULL;
    axis2_char_t *pat = NULL;
    axiom_element_t *op_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    void *nested_configurable = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    op = WODEN_WSDL10_DESC_ELEMENT_CREATE_INTERFACE_OP_ELEMENT(desc, env);
    nested_configurable = WODEN_INTERFACE_OP_GET_BASE_IMPL(op, env);
    nested_configurable = woden_nested_configurable_to_nested_element(
                nested_configurable, env);
    WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(nested_configurable, env, parent);

    op_el = AXIOM_NODE_GET_DATA_ELEMENT(op_el_node, env);
    name = axiom_element_get_attribute_value_by_name(op_el, env, WODEN_WSDL10_ATTR_NAME);

    if (name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *ns = NULL;
        axis2_char_t *ns_str = NULL;

        ns = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        ns_str = AXIS2_URI_TO_STRING(ns, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, ns_str, NULL);
        op = woden_interface_op_to_interface_op_element(op, env);
        WODEN_INTERFACE_OP_ELEMENT_SET_QNAME(op, env, qname);
    }

    style = axiom_element_get_attribute_value_by_name(op_el, env, WODEN_WSDL10_ATTR_STYLE);

    if (style)
    {
        axis2_array_list_t *str_list = NULL;
        axis2_char_t *uri_str = NULL;
        axis2_uri_t *uri = NULL;
        int i = 0, size = 0;

        size = axis2_array_list_size(str_list, env);
        axis2_tokenize(env, style, ' ');
        for (i = 0; i < size; i++)
        {
            uri_str = axis2_array_list_get(str_list, env, i);
            uri = get_uri(env, uri_str);
            op = woden_interface_op_to_interface_op_element(op, env);
            WODEN_INTERFACE_OP_ELEMENT_ADD_STYLE_URI(op, env, uri);
            AXIS2_URI_FREE(uri, env);
        }
    }

    pat = axiom_element_get_attribute_value_by_name(op_el, env, WODEN_WSDL10_ATTR_PATTERN);

    if (pat)
    {
        axis2_uri_t *uri = NULL;

        uri = get_uri(env, pat);
        op = woden_interface_op_to_interface_op_element(op, env);
        WODEN_INTERFACE_OP_ELEMENT_SET_PATTERN(op, env, uri);
        AXIS2_URI_FREE(uri, env);
    }
    op = woden_interface_op_to_attr_extensible(op, env);
    status = parse_ext_attributes(reader, env, op_el_node, "interface_op_element",
            op, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    /* Parse the child elements of interface <operation>.
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <input> <output> <infault> <outfault> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */
    temp_el = axiom_util_get_first_child_element(op_el, env, op_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;
        axis2_qname_t *q_elem_input = NULL;
        axis2_qname_t *q_elem_output = NULL;
        axis2_qname_t *q_elem_infault = NULL;
        axis2_qname_t *q_elem_outfault = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_PROPERTY);
        q_elem_input = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_INPUT);
        q_elem_output = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_OUTPUT);
        q_elem_infault = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_INFAULT);
        q_elem_outfault = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_OUTFAULT);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            op = woden_interface_op_to_documentable(op, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(op, env, documentation);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, op);
            op = woden_interface_op_to_configurable_element(op, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(op, env, feature);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, op);
            op = woden_interface_op_to_configurable_element(op, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(op, env, property);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_input, temp_el_node))
        {
            void *input = NULL;

            input = parse_interface_msg_ref(reader, env, temp_el_node, desc, op);
            op = woden_interface_op_to_interface_op_element(op, env);
            WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_MSG_REF_ELEMENT(op, env, input);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_output, temp_el_node))
        {
            void *output = NULL;

            output = parse_interface_msg_ref(reader, env, temp_el_node, desc, op);
            op = woden_interface_op_to_interface_op_element(op, env);
            WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_MSG_REF_ELEMENT(op, env, output);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_infault, temp_el_node))
        {
            void *infault = NULL;

            infault = parse_interface_fault_ref(reader, env, temp_el_node, desc, op);
            op = woden_interface_op_to_interface_op_element(op, env);
            WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_FAULT_REF_ELEMENT(op, env, infault);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_outfault, temp_el_node))
        {
            void *outfault = NULL;

            outfault = parse_interface_fault_ref(reader, env, temp_el_node, desc, op);
            op = woden_interface_op_to_interface_op_element(op, env);
            WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_FAULT_REF_ELEMENT(op, env, outfault);
        }
        else
        {
            void *ext_element = NULL;
            void *nested_confble = NULL;
            void *configurable = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "interface_op_element",
                    op, temp_el_node, desc);

            nested_confble = WODEN_INTERFACE_OP_GET_BASE_IMPL(op, env);
            configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(nested_confble, env);
            documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return op;
}

static void *
parse_interface_fault_ref(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *fault_ref_el_node,
    void *desc,
    void *parent)
{
    void *fault_ref = NULL;
    axis2_char_t *localname = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *msg = NULL;
    axiom_element_t *fault_ref_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    void *nested_configurable = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    fault_ref = WODEN_WSDL10_DESC_ELEMENT_CREATE_INTERFACE_FAULT_REF_ELEMENT(desc,
            env);
    nested_configurable = WODEN_WSDL10_INTERFACE_FAULT_REF_GET_BASE_IMPL(fault_ref, env);
    nested_configurable = woden_nested_configurable_to_nested_element(
                nested_configurable, env);
    WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(nested_configurable, env, parent);

    fault_ref_el = AXIOM_NODE_GET_DATA_ELEMENT(fault_ref_el_node, env);
    localname = axiom_element_get_localname(fault_ref_el, env);
    if (0 == AXIS2_STRCMP(WODEN_WSDL10_ELEM_INFAULT, localname))
    {
        woden_direction_t *direction_in = NULL;

        direction_in = woden_direction_get_direction_in(env);
        WODEN_WSDL10_INTERFACE_FAULT_REF_ELEMENT_SET_DIRECTION(fault_ref, env, direction_in);
    }
    if (0 == AXIS2_STRCMP(WODEN_WSDL10_ELEM_OUTFAULT, localname))
    {
        woden_direction_t *direction_out = NULL;

        direction_out = woden_direction_get_direction_out(env);
        fault_ref = woden_wsdl10_interface_fault_ref_to_interface_fault_ref_element(
                    fault_ref, env);
        WODEN_WSDL10_INTERFACE_FAULT_REF_ELEMENT_SET_DIRECTION(fault_ref, env, direction_out);
    }

    name = axiom_element_get_attribute_value_by_name(fault_ref_el, env, WODEN_WSDL10_ATTR_NAME);
    if (name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        fault_ref =
            woden_wsdl10_interface_fault_ref_to_interface_fault_ref_element(
                fault_ref, env);
        WODEN_WSDL10_INTERFACE_FAULT_REF_ELEMENT_SET_QNAME(fault_ref, env, qname);
    }

    msg = axiom_element_get_attribute_value_by_name(fault_ref_el, env, WODEN_WSDL10_ATTR_MSG);
    if (msg)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, msg, namespc_str, NULL);
        fault_ref =
            woden_wsdl10_interface_fault_ref_to_interface_fault_ref_element(
                fault_ref, env);
        WODEN_WSDL10_INTERFACE_FAULT_REF_ELEMENT_SET_MSG_QNAME(fault_ref, env, qname);
    }

    fault_ref = woden_wsdl10_interface_fault_ref_to_attr_extensible(fault_ref, env);
    status = parse_ext_attributes(reader, env, fault_ref_el_node, "interface_fault_ref_element",
            fault_ref, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    temp_el = axiom_util_get_first_child_element(fault_ref_el, env, fault_ref_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_DOCUMENTATION);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            fault_ref = woden_wsdl10_interface_fault_ref_to_documentable(fault_ref, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(fault_ref, env, documentation);
        }
        else
        {
            void *ext_element = NULL;
            void *nested_confble = NULL;
            void *configurable = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "interface_fault_ref_element",
                    fault_ref, temp_el_node, desc);

            nested_confble = WODEN_WSDL10_INTERFACE_FAULT_REF_GET_BASE_IMPL(fault_ref, env);
            configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(nested_confble, env);
            documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return fault_ref;
}

static void *
parse_interface_msg_ref(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *msg_ref_el_node,
    void *desc,
    void *parent)
{
    void *msg_ref = NULL;
    axis2_char_t *localname = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *msg = NULL;
    axiom_element_t *msg_ref_el;
    axiom_element_t *temp_el;
    axiom_node_t *temp_el_node;
    void *nested_configurable = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    msg_ref = WODEN_WSDL10_DESC_ELEMENT_CREATE_INTERFACE_MSG_REF_ELEMENT(desc,
            env);
    nested_configurable = WODEN_WSDL10_INTERFACE_MSG_REF_GET_BASE_IMPL(msg_ref, env);
    nested_configurable = woden_nested_configurable_to_nested_element(
                nested_configurable, env);
    WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(nested_configurable, env, parent);

    msg_ref_el = AXIOM_NODE_GET_DATA_ELEMENT(msg_ref_el_node, env);
    localname = axiom_element_get_localname(msg_ref_el, env);
    if (0 == AXIS2_STRCMP(WODEN_WSDL10_ELEM_INPUT, localname))
    {
        woden_direction_t *direction_in = NULL;

        direction_in = woden_direction_get_direction_in(env);
        msg_ref = woden_wsdl10_interface_msg_ref_to_interface_msg_ref_element(
                    msg_ref, env);
        WODEN_WSDL10_INTERFACE_MSG_REF_ELEMENT_SET_DIRECTION(msg_ref, env, direction_in);
    }
    if (0 == AXIS2_STRCMP(WODEN_WSDL10_ELEM_OUTPUT, localname))
    {
        woden_direction_t *direction_out = NULL;

        direction_out = woden_direction_get_direction_out(env);
        msg_ref = woden_wsdl10_interface_msg_ref_to_interface_msg_ref_element(
                    msg_ref, env);
        WODEN_WSDL10_INTERFACE_MSG_REF_ELEMENT_SET_DIRECTION(msg_ref, env, direction_out);
    }

    name = axiom_element_get_attribute_value_by_name(msg_ref_el, env, WODEN_WSDL10_ATTR_NAME);
    if (name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        msg_ref = woden_wsdl10_interface_msg_ref_to_interface_msg_ref_element(
                    msg_ref, env);
        WODEN_WSDL10_INTERFACE_MSG_REF_ELEMENT_SET_QNAME(msg_ref, env, qname);
    }

    msg = axiom_element_get_attribute_value_by_name(msg_ref_el, env, WODEN_WSDL10_ATTR_MSG);
    if (msg)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;
        axis2_array_list_t *msgs = NULL;
        int i = 0, size = 0;
        axis2_char_t *tmp_msg = NULL;

       /**
        * NOTE:
        * msg can contain names like "impl:msg_name"
        * this time msg_name is in impl prefixed namespace rather than in tns
        * so the following logic is wrong.
        */

        namespc = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);

        /** currently I m removing impl from the msg */
        for ( tmp_msg = msg; *msg != ':' && *msg != '\0' && *msg != '|'; msg ++ );
        if(  *msg == '\0' || *msg == '|')
        {
            msg = tmp_msg;
        }
        else
        {
            msg++;
        }

        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, msg, namespc_str, NULL);
        msg_ref = woden_wsdl10_interface_msg_ref_to_interface_msg_ref_element(
                    msg_ref, env);
        desc = woden_wsdl10_desc_to_desc_element(desc, env);
        msgs = WODEN_WSDL10_DESC_ELEMENT_GET_MSG_ELEMENTS(desc, env);
        if (msgs)
            size = axis2_array_list_size(msgs, env);
        for (i = 0; i < size; i++)
        {
            void *msg = NULL;
            axis2_qname_t *msg_qname = NULL;

            msg = axis2_array_list_get(msgs, env, i);
            msg_qname = WODEN_WSDL10_MSG_REF_GET_QNAME(msg, env);
            if (AXIS2_QNAME_EQUALS(msg_qname, env, qname))
            {
                WODEN_WSDL10_INTERFACE_MSG_REF_ELEMENT_SET_MSG(msg_ref, env, msg);
            }
        }
        WODEN_WSDL10_INTERFACE_MSG_REF_ELEMENT_SET_MSG_QNAME(msg_ref, env, qname);
    }
    msg_ref = woden_wsdl10_interface_msg_ref_to_attr_extensible(msg_ref, env);
    status = parse_ext_attributes(reader, env, msg_ref_el_node,
            "interface_msg_ref_element", msg_ref, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    temp_el = axiom_util_get_first_child_element(msg_ref_el, env, msg_ref_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_DOCUMENTATION);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            msg_ref = woden_wsdl10_interface_msg_ref_to_documentable(msg_ref, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(msg_ref, env, documentation);
        }
        else
        {
            void *ext_element = NULL;
            void *nested_confble = NULL;
            void *configurable = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "interface_msg_ref_element",
                    msg_ref, temp_el_node, desc);

            nested_confble = WODEN_WSDL10_INTERFACE_MSG_REF_GET_BASE_IMPL(msg_ref, env);
            configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(nested_confble, env);
            documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return msg_ref;
}

static void *
parse_binding(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *binding_el_node,
    void *desc)
{
    void *binding = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *type = NULL;
    axis2_char_t *intface = NULL;
    axiom_element_t *binding_el;
    axiom_element_t *temp_el;
    axiom_node_t *temp_el_node;
    axis2_qname_t *intface_qn = NULL;
    void *configurable = NULL;
    void *documentable = NULL;
    void *wsdl_obj = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    binding = WODEN_WSDL10_DESC_ELEMENT_CREATE_BINDING_ELEMENT(desc, env);

    binding_el = AXIOM_NODE_GET_DATA_ELEMENT(binding_el_node, env);
    name = axiom_element_get_attribute_value_by_name(binding_el, env, WODEN_WSDL10_ATTR_NAME);

    if (name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        binding = woden_binding_to_binding_element(binding, env);
        WODEN_BINDING_ELEMENT_SET_QNAME(binding, env, qname);
    }

    intface = axiom_element_get_attribute_value_by_name(binding_el, env, WODEN_WSDL10_ATTR_INTERFACE);
    if (intface)
    {
        axis2_hash_t *namespcs = NULL;

        desc = woden_wsdl10_desc_to_desc_element(desc, env);
        namespcs = WODEN_WSDL10_DESC_ELEMENT_GET_NAMESPACES(desc, env);
        intface_qn = woden_om_util_get_qname(env, binding_el_node, intface, namespcs);
        binding = woden_binding_to_binding_element(binding, env);
        WODEN_BINDING_ELEMENT_SET_INTERFACE_QNAME(binding, env, intface_qn);
    }
    /* Dereference the interface qname */
    if (intface_qn)
    {
        axis2_array_list_t *interfaces = NULL;
        int i = 0, size = 0;

        desc = woden_wsdl10_desc_to_desc_element(desc, env);
        interfaces = WODEN_WSDL10_DESC_ELEMENT_GET_INTERFACE_ELEMENTS(desc, env);
        size = axis2_array_list_size(interfaces, env);
        for (i = 0; i < size; i++)
        {
            void *intface = NULL;
            axis2_qname_t *qname = NULL;

            intface = axis2_array_list_get(interfaces, env, i);
            intface = woden_interface_to_interface_element(intface, env);
            qname = WODEN_INTERFACE_ELEMENT_GET_QNAME(intface, env);
            if (AXIS2_QNAME_EQUALS(intface_qn, env, qname))
            {
                WODEN_BINDING_SET_INTERFACE_ELEMENT(binding, env, intface);
            }
        }
    }

    type = axiom_element_get_attribute_value_by_name(binding_el, env, WODEN_WSDL10_ATTR_TYPE);

    if (type)
    {
        axis2_uri_t *uri = NULL;

        uri = get_uri(env, type);
        binding = woden_binding_to_binding_element(binding, env);
        WODEN_BINDING_ELEMENT_SET_TYPE(binding, env, uri);
        AXIS2_URI_FREE(uri, env);
    }

    /* TODO extends attribute */

    configurable = WODEN_BINDING_GET_BASE_IMPL(binding, env);
    documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
    wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
    status = parse_ext_attributes(reader, env, binding_el_node, "binding_element",
            wsdl_obj, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    /* Parse the child elements of <binding>.
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <fault> <operation> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */
    temp_el = axiom_util_get_first_child_element(binding_el, env, binding_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_op = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_DOCUMENTATION);
        q_elem_op = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_OPERATION);
        q_elem_feature = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_PROPERTY);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            binding = woden_binding_to_documentable(binding, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(binding, env, documentation);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_op, temp_el_node))
        {
            void *binding_op = NULL;

            binding_op = parse_binding_op(reader, env, temp_el_node, desc,
                    binding);
            binding = woden_binding_to_binding_element(binding, env);
            WODEN_BINDING_ELEMENT_ADD_BINDING_OP_ELEMENT(binding, env, binding_op);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, binding);
            binding = woden_binding_to_configurable_element(binding, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(binding, env, feature);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, binding);
            binding = woden_binding_to_configurable_element(binding, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(binding, env, property);
        }
        else
        {
            void *ext_element = NULL;
            void *configurable = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "binding_element",
                    binding, temp_el_node, desc);

            configurable = WODEN_BINDING_GET_BASE_IMPL(binding, env);
            documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return binding;
}

static void *
parse_binding_op(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *op_el_node,
    void *desc,
    void *parent)
{
    void *op = NULL;
    axis2_char_t *name = NULL;
    axiom_element_t *op_el;
    axiom_element_t *temp_el;
    axiom_node_t *temp_el_node;
    axis2_qname_t *ref_qn = NULL;
    void *nested_configurable = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    op = WODEN_WSDL10_DESC_ELEMENT_CREATE_BINDING_OP_ELEMENT(desc, env);
    nested_configurable = WODEN_WSDL10_BINDING_OP_GET_BASE_IMPL(op, env);
    nested_configurable = woden_nested_configurable_to_nested_element(
                nested_configurable, env);
    WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(nested_configurable, env, parent);

    op_el = AXIOM_NODE_GET_DATA_ELEMENT(op_el_node, env);
    name = axiom_element_get_attribute_value_by_name(op_el, env, WODEN_WSDL10_ATTR_NAME);

    if (name)
    {
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        ref_qn = axis2_qname_create(env, name, namespc_str, NULL);
        op = woden_wsdl10_binding_op_to_binding_op_element(op, env);
        WODEN_WSDL10_BINDING_OP_ELEMENT_SET_QNAME(op, env, ref_qn);
    }
    /* Dereference the 'ref' qname to an interface_op_element */
    if (ref_qn)
    {
        void *intface = NULL;
        axis2_array_list_t *int_ops = NULL;
        int i = 0, size = 0;

        /* Here I know that parent is an instance of binding */
        parent = woden_binding_to_binding_element(parent, env);
        intface = WODEN_BINDING_ELEMENT_GET_INTERFACE_ELEMENT(parent, env);
        intface = woden_interface_to_interface_element(intface, env);
        int_ops = WODEN_INTERFACE_ELEMENT_GET_INTERFACE_OP_ELEMENTS(intface, env);
        size = axis2_array_list_size(int_ops, env);
        for (i = 0; i < size; i++)
        {
            void *intface_op = NULL;
            axis2_qname_t *qname = NULL;

            intface_op = axis2_array_list_get(int_ops, env, i);
            if (intface_op)
                qname = WODEN_INTERFACE_OP_ELEMENT_GET_QNAME(
                            intface_op, env);
            if (AXIS2_QNAME_EQUALS(ref_qn, env, qname))
            {
                WODEN_WSDL10_BINDING_OP_SET_INTERFACE_OP_ELEMENT(op,
                        env, intface_op);
            }
        }
    }

    op = woden_wsdl10_binding_op_to_attr_extensible(op, env);
    status = parse_ext_attributes(reader, env, op_el_node, "binding_op_element",
            op, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    /* Parse the child elements of binding <operation>.
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <input> <output> <infault> <outfault> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */
    temp_el = axiom_util_get_first_child_element(op_el, env, op_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;
        axis2_qname_t *q_elem_input = NULL;
        axis2_qname_t *q_elem_output = NULL;
        axis2_qname_t *q_elem_infault = NULL;
        axis2_qname_t *q_elem_outfault = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_PROPERTY);
        q_elem_input = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_INPUT);
        q_elem_output = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_OUTPUT);
        q_elem_infault = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_INFAULT);
        q_elem_outfault = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_OUTFAULT);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            op = woden_wsdl10_binding_op_to_documentable(op, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(op, env, documentation);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, op);
            op = woden_wsdl10_binding_op_to_configurable_element(op, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(op, env, feature);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, op);
            op = woden_wsdl10_binding_op_to_configurable_element(op, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(op, env, property);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_input, temp_el_node))
        {
            void *input = NULL;

            input = parse_binding_msg_ref(reader, env, temp_el_node, desc, op);
            op = woden_wsdl10_binding_op_to_binding_op_element(op, env);
            WODEN_WSDL10_BINDING_OP_ELEMENT_ADD_BINDING_MSG_REF_ELEMENT(op, env, input);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_output, temp_el_node))
        {
            void *output = NULL;

            output = parse_binding_msg_ref(reader, env, temp_el_node, desc, op);
            op = woden_wsdl10_binding_op_to_binding_op_element(op, env);
            WODEN_WSDL10_BINDING_OP_ELEMENT_ADD_BINDING_MSG_REF_ELEMENT(op, env, output);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_infault, temp_el_node))
        {
            void *infault = NULL;

            infault = parse_binding_fault_ref(reader, env, temp_el_node, desc, op);
            op = woden_wsdl10_binding_op_to_binding_op_element(op, env);
            WODEN_WSDL10_BINDING_OP_ELEMENT_ADD_BINDING_FAULT_REF_ELEMENT(op, env, infault);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_outfault, temp_el_node))
        {
            void *outfault = NULL;

            outfault = parse_binding_fault_ref(reader, env, temp_el_node, desc, op);
            op = woden_wsdl10_binding_op_to_binding_op_element(op, env);
            WODEN_WSDL10_BINDING_OP_ELEMENT_ADD_BINDING_FAULT_REF_ELEMENT(op, env, outfault);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "binding_op_element",
                    op, temp_el_node, desc);

            op = woden_wsdl10_binding_op_to_element_extensible(op, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(op, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return op;
}

static void *
parse_binding_fault_ref(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *fault_ref_el_node,
    void *desc,
    void *parent)
{
    void *fault_ref = NULL;
    axis2_char_t *ref = NULL;
    axis2_char_t *msg_label_str = NULL;
    axiom_element_t *fault_ref_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_qname_t *attr_ref = NULL;
    axis2_qname_t *ref_qn = NULL;
    axis2_qname_t *qref = NULL;
    woden_msg_label_t *msg_label = NULL;
    woden_msg_label_t *msg_label_in = NULL;
    woden_msg_label_t *msg_label_out = NULL;
    void *nested_configurable = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    fault_ref = WODEN_WSDL10_DESC_ELEMENT_CREATE_BINDING_FAULT_REF_ELEMENT(desc, env);
    nested_configurable = WODEN_BINDING_FAULT_REF_GET_BASE_IMPL(fault_ref, env);
    nested_configurable = woden_nested_configurable_to_nested_element(
                nested_configurable, env);
    WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(nested_configurable, env, parent);

    fault_ref_el = AXIOM_NODE_GET_DATA_ELEMENT(fault_ref_el_node, env);
    attr_ref = axis2_qname_create_from_string(env, WODEN_WSDL10_ATTR_REF);
    ref = axiom_element_get_attribute_value_by_name(fault_ref_el, env, WODEN_WSDL10_ATTR_REF);

    if (ref)
    {
        axis2_hash_t *namespcs = NULL;

        desc = woden_wsdl10_desc_to_desc_element(desc, env);
        namespcs = WODEN_WSDL10_DESC_ELEMENT_GET_NAMESPACES(desc, env);
        ref_qn = woden_om_util_get_qname(env, fault_ref_el_node, ref, namespcs);
        fault_ref = woden_binding_fault_ref_to_binding_fault_ref_element(
                    fault_ref, env);
        WODEN_BINDING_FAULT_REF_ELEMENT_SET_REF(fault_ref, env, ref_qn);
    }

    msg_label_str = axiom_element_get_attribute_value_by_name(fault_ref_el, env,
            WODEN_WSDL10_ATTR_MESSAGE_LABEL);

    if (msg_label_str)
    {
        woden_msg_label_t *msg_label_in = NULL;
        woden_msg_label_t *msg_label_out = NULL;
        axis2_char_t *label_in_str = NULL;
        axis2_char_t *label_out_str = NULL;

        msg_label_in = woden_msg_label_get_msg_label_in(env);
        label_in_str = WODEN_MSG_LABEL_TO_STRING(msg_label_in, env);
        msg_label_out = woden_msg_label_get_msg_label_out(env);
        label_out_str = WODEN_MSG_LABEL_TO_STRING(msg_label_out, env);
        if (0 == AXIS2_STRCMP(msg_label_str, label_in_str))
        {
            fault_ref =
                woden_binding_fault_ref_to_binding_fault_ref_element(
                    fault_ref, env);
            WODEN_BINDING_FAULT_REF_ELEMENT_SET_MSG_LABEL(fault_ref,
                    env, msg_label_in);
        }
        else if (0 == AXIS2_STRCMP(msg_label_str, label_out_str))
        {
            fault_ref =
                woden_binding_fault_ref_to_binding_fault_ref_element(
                    fault_ref, env);
            WODEN_BINDING_FAULT_REF_ELEMENT_SET_MSG_LABEL(fault_ref,
                    env, msg_label_out);
        }
        else
        {
            woden_msg_label_t *msg_label_invalid = NULL;

            /* Invalid value, but capture it anyway. */
            msg_label_invalid = woden_msg_label_get_invalid_value(env, msg_label_str);
            fault_ref =
                woden_binding_fault_ref_to_binding_fault_ref_element(
                    fault_ref, env);
            WODEN_BINDING_FAULT_REF_ELEMENT_SET_MSG_LABEL(fault_ref,
                    env, msg_label_invalid);
        }
    }
    /* Dereference the 'ref' and 'messageLabel' attributes to an interface fault
     * reference.
     */
    fault_ref = woden_binding_fault_ref_to_binding_fault_ref_element(
                fault_ref, env);
    qref = WODEN_BINDING_FAULT_REF_ELEMENT_GET_REF(fault_ref, env);
    msg_label = WODEN_BINDING_FAULT_REF_ELEMENT_GET_MSG_LABEL(fault_ref, env);
    msg_label_in = woden_msg_label_get_msg_label_in(env);
    msg_label_out = woden_msg_label_get_msg_label_out(env);

    if (qref &&
            (msg_label == msg_label_in ||
                    msg_label == msg_label_out))
    {
        void *bind_op = parent;
        void *int_op = NULL;

        bind_op = woden_wsdl10_binding_op_to_binding_op_element(bind_op, env);
        int_op = WODEN_WSDL10_BINDING_OP_ELEMENT_GET_INTERFACE_OP_ELEMENT(
                    bind_op, env);
        if (int_op)
        {
            axis2_array_list_t *int_flt_refs = NULL;
            int i = 0, size = 0;

            int_op = woden_interface_op_to_interface_op_element(int_op, env);
            int_flt_refs =
                WODEN_INTERFACE_OP_ELEMENT_GET_INTERFACE_FAULT_REF_ELEMENTS(
                    int_op, env);
            size = axis2_array_list_size(int_flt_refs, env);
            for (i = 0; i < size; i++)
            {
                void *int_flt_ref = NULL;
                axis2_qname_t *qname = NULL;

                int_flt_ref = axis2_array_list_get(int_flt_refs, env, i);
                int_flt_ref =
                    woden_wsdl10_interface_fault_ref_to_interface_fault_ref_element(
                        int_flt_ref, env);
                qname = WODEN_WSDL10_INTERFACE_FAULT_REF_ELEMENT_GET_MSG_QNAME(
                            int_flt_ref, env);
                /*label = WODEN_WSDL10_INTERFACE_FAULT_REF_ELEMENT_GET_MSG_LABEL(
                        int_flt_ref, env);
                if(AXIS2_QNAME_EQUALS(qref, env, qname) &&
                        WODEN_MSG_LABEL_EQUALS(msg_label, env, label))
                 */
                if (AXIS2_QNAME_EQUALS(qref, env, qname))
                {
                    WODEN_BINDING_FAULT_REF_SET_INTERFACE_FAULT_REF_ELEMENT(
                        fault_ref, env, int_flt_ref);
                }
            }
        }
    }

    /* Parse the child elements of binding operation <infault> or <outfault>.
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */

    fault_ref = woden_binding_fault_ref_to_attr_extensible(fault_ref, env);
    status = parse_ext_attributes(reader, env, fault_ref_el_node, "binding_fault_ref_element",
            fault_ref, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    temp_el = axiom_util_get_first_child_element(fault_ref_el, env, fault_ref_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_PROPERTY);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            fault_ref = woden_binding_fault_ref_to_documentable(fault_ref, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(fault_ref, env, documentation);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, fault_ref);
            fault_ref = woden_binding_fault_ref_to_configurable_element(fault_ref, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(fault_ref, env, feature);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, fault_ref);
            fault_ref = woden_binding_fault_ref_to_configurable_element(fault_ref, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(fault_ref, env, property);
        }
        else
        {
            void *ext_element = NULL;
            void *nested_confble = NULL;
            void *configurable = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "binding_fault_ref_element",
                    fault_ref, temp_el_node, desc);

            nested_confble = WODEN_BINDING_FAULT_REF_GET_BASE_IMPL(fault_ref, env);
            configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(nested_confble, env);
            documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return fault_ref;
}

static void *
parse_binding_msg_ref(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *msg_ref_el_node,
    void *desc,
    void *parent)
{
    void *msg_ref = NULL;
    axis2_char_t *name = NULL;
    axiom_element_t *msg_ref_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_char_t *localname = NULL;
    void *nested_configurable = NULL;
    void *bind_op = NULL;
    void *int_op = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    msg_ref = WODEN_WSDL10_DESC_ELEMENT_CREATE_BINDING_MSG_REF_ELEMENT(desc, env);
    nested_configurable = WODEN_WSDL10_BINDING_MSG_REF_GET_BASE_IMPL(msg_ref, env);
    nested_configurable = woden_nested_configurable_to_nested_element(
                nested_configurable, env);
    WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(nested_configurable, env, parent);

    msg_ref_el = AXIOM_NODE_GET_DATA_ELEMENT(msg_ref_el_node, env);
    localname = axiom_element_get_localname(msg_ref_el, env);
    if (0 == AXIS2_STRCMP(WODEN_WSDL10_ELEM_INPUT, localname))
    {
        woden_direction_t *direction_in = NULL;

        direction_in = woden_direction_get_direction_in(env);
        msg_ref = woden_wsdl10_binding_msg_ref_to_binding_msg_ref_element(msg_ref, env);
        WODEN_WSDL10_BINDING_MSG_REF_ELEMENT_SET_DIRECTION(msg_ref, env, direction_in);
    }
    if (0 == AXIS2_STRCMP(WODEN_WSDL10_ELEM_OUTPUT, localname))
    {
        woden_direction_t *direction_out = NULL;

        direction_out = woden_direction_get_direction_out(env);
        msg_ref = woden_wsdl10_binding_msg_ref_to_binding_msg_ref_element(msg_ref, env);
        WODEN_WSDL10_BINDING_MSG_REF_ELEMENT_SET_DIRECTION(msg_ref, env, direction_out);
    }

    name = axiom_element_get_attribute_value_by_name(msg_ref_el, env,
            WODEN_WSDL10_ATTR_NAME);

    if (name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        msg_ref = woden_wsdl10_binding_msg_ref_to_binding_msg_ref_element(
                    msg_ref, env);
        WODEN_WSDL10_BINDING_MSG_REF_ELEMENT_SET_QNAME(msg_ref, env, qname);
    }

    /* Dereference the msgLabel to an interface_msg_ref_element */
    msg_ref = woden_wsdl10_binding_msg_ref_to_binding_msg_ref_element(msg_ref,
            env);

    bind_op = parent;

    bind_op = woden_wsdl10_binding_op_to_binding_op_element(bind_op, env);
    int_op = WODEN_WSDL10_BINDING_OP_ELEMENT_GET_INTERFACE_OP_ELEMENT(
                bind_op, env);
    if (int_op)
    {
        axis2_array_list_t *int_msg_refs = NULL;
        int i = 0, size = 0;

        int_op = woden_interface_op_to_interface_op_element(int_op, env);
        int_msg_refs =
            WODEN_INTERFACE_OP_ELEMENT_GET_INTERFACE_MSG_REF_ELEMENTS(
                int_op, env);
        size = axis2_array_list_size(int_msg_refs, env);
        for (i = 0; i < size; i++)
        {
            void *int_msg_ref = NULL;
            axis2_qname_t *intf_msg_qname = NULL;
            axis2_qname_t *binding_msg_qname = NULL;

            int_msg_ref = axis2_array_list_get(int_msg_refs, env, i);
            int_msg_ref =
                woden_wsdl10_interface_msg_ref_to_interface_msg_ref_element(
                    int_msg_ref, env);
            intf_msg_qname = WODEN_WSDL10_INTERFACE_MSG_REF_ELEMENT_GET_QNAME(
                        int_msg_ref, env);
            msg_ref = woden_wsdl10_binding_msg_ref_to_binding_msg_ref_element(msg_ref, env);
            binding_msg_qname = WODEN_WSDL10_BINDING_MSG_REF_ELEMENT_GET_QNAME(msg_ref, env);
            if(intf_msg_qname != NULL &&
                    binding_msg_qname != NULL &&
                    AXIS2_QNAME_EQUALS(intf_msg_qname, env, binding_msg_qname))
            {
                WODEN_WSDL10_BINDING_MSG_REF_SET_INTERFACE_MSG_REF_ELEMENT(
                    msg_ref, env, int_msg_ref);
            }
        }
    }

    msg_ref = woden_wsdl10_binding_msg_ref_to_attr_extensible(msg_ref, env);
    status = parse_ext_attributes(reader, env, msg_ref_el_node, "binding_msg_ref_element",
            msg_ref, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    /* Parse the child elements of binding operation <input> or <output>.
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */
    temp_el = axiom_util_get_first_child_element(msg_ref_el, env, msg_ref_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_PROPERTY);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            msg_ref = woden_wsdl10_binding_msg_ref_to_documentable(msg_ref, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(msg_ref, env, documentation);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, msg_ref);
            msg_ref = woden_wsdl10_binding_msg_ref_to_configurable_element(msg_ref, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(msg_ref, env, feature);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, msg_ref);
            msg_ref = woden_wsdl10_binding_msg_ref_to_configurable_element(msg_ref, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(msg_ref, env, property);
        }
        else
        {
            void *ext_element = NULL;
            void *nested_confble = NULL;
            void *configurable = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "binding_msg_ref_element",
                    msg_ref, temp_el_node, desc);

            nested_confble = WODEN_WSDL10_BINDING_MSG_REF_GET_BASE_IMPL(msg_ref, env);
            configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(nested_confble, env);
            documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return msg_ref;
}

static void *
parse_svc(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *svc_el_node,
    void *desc)

{
    void *svc = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *intface = NULL;
    axis2_qname_t *intface_qn = NULL;
    axiom_element_t *svc_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    svc_el = AXIOM_NODE_GET_DATA_ELEMENT(svc_el_node, env);
    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    svc = WODEN_WSDL10_DESC_ELEMENT_CREATE_SVC_ELEMENT(desc, env);

    svc_el = AXIOM_NODE_GET_DATA_ELEMENT(svc_el_node, env);
    name = axiom_element_get_attribute_value_by_name(svc_el, env, WODEN_WSDL10_ATTR_NAME);

    if (name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        svc = woden_wsdl10_svc_to_svc_element(svc, env);
        WODEN_WSDL10_SVC_ELEMENT_SET_QNAME(svc, env, qname);
    }

    intface = axiom_element_get_attribute_value_by_name(svc_el, env, WODEN_WSDL10_ATTR_INTERFACE);
    if (intface)
    {
        axis2_hash_t *namespcs = NULL;

        desc = woden_wsdl10_desc_to_desc_element(desc, env);
        namespcs = WODEN_WSDL10_DESC_ELEMENT_GET_NAMESPACES(desc, env);
        intface_qn = woden_om_util_get_qname(env, svc_el_node, intface, namespcs);
        svc = woden_wsdl10_svc_to_svc_element(svc, env);
        WODEN_WSDL10_SVC_ELEMENT_SET_INTERFACE_QNAME(svc, env, intface_qn);
    }
    /* Dereference the interface qname */
    if (intface_qn)
    {
        axis2_array_list_t *interfaces = NULL;
        int i = 0, size = 0;

        desc = woden_wsdl10_desc_to_desc_element(desc, env);
        interfaces = WODEN_WSDL10_DESC_ELEMENT_GET_INTERFACE_ELEMENTS(desc, env);
        if (interfaces)
            size = axis2_array_list_size(interfaces, env);
        for (i = 0; i < size; i++)
        {
            void *intface = NULL;
            axis2_qname_t *qname = NULL;

            intface = axis2_array_list_get(interfaces, env, i);
            intface = woden_interface_to_interface_element(intface, env);
            qname = WODEN_INTERFACE_ELEMENT_GET_QNAME(intface, env);
            if (AXIS2_QNAME_EQUALS(intface_qn, env, qname))
            {
                WODEN_WSDL10_SVC_SET_INTERFACE_ELEMENT(svc, env, intface);
            }
        }
    }

    /* TODO extends attribute */
    svc = woden_wsdl10_svc_to_attr_extensible(svc, env);
    status = parse_ext_attributes(reader, env, svc_el_node, "svc_element",
            svc, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    /* Parse the child elements of <service>.
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <endpoint>
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */
    temp_el = axiom_util_get_first_child_element(svc_el, env, svc_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_endpoint = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_DOCUMENTATION);
        q_elem_endpoint = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_ENDPOINT);
        q_elem_feature = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_PROPERTY);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            svc = woden_wsdl10_svc_to_documentable(svc, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(svc, env, documentation);
        }
        if (axis2_qname_util_matches(env,
                q_elem_endpoint, temp_el_node))
        {
            void *endpoint = NULL;

            endpoint = parse_endpoint(reader, env, temp_el_node, desc, svc);
            svc = woden_wsdl10_svc_to_svc_element(svc, env);
            WODEN_WSDL10_SVC_ELEMENT_ADD_ENDPOINT_ELEMENT(svc, env, endpoint);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, svc);
            svc = woden_wsdl10_svc_to_configurable_element(svc, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(svc, env, feature);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, svc);
            svc = woden_wsdl10_svc_to_configurable_element(svc, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(svc, env, property);
        }
        else
        {
            void *ext_element = NULL;
            void *configurable = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "svc_element",
                    svc, temp_el_node, desc);

            configurable = WODEN_WSDL10_SVC_GET_BASE_IMPL(svc, env);
            documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return svc;
}

static void *
parse_endpoint(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *endpoint_el_node,
    void *desc,
    void *parent)

{
    void *endpoint = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *address = NULL;
    axis2_char_t *binding = NULL;
    axis2_qname_t *binding_qn = NULL;
    axis2_qname_t *attr_binding = NULL;
    axis2_qname_t *attr_address = NULL;
    axiom_element_t *endpoint_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    void *nested_configurable = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    endpoint = WODEN_WSDL10_DESC_ELEMENT_CREATE_ENDPOINT_ELEMENT(desc, env);
    nested_configurable = WODEN_WSDL10_ENDPOINT_GET_BASE_IMPL(endpoint, env);
    nested_configurable = woden_nested_configurable_to_nested_element(
                nested_configurable, env);
    WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(nested_configurable, env, parent);


    endpoint_el = AXIOM_NODE_GET_DATA_ELEMENT(endpoint_el_node, env);
    name = axiom_element_get_attribute_value_by_name(endpoint_el, env, WODEN_WSDL10_ATTR_NAME);

    if (name)
    {
        woden_nc_name_t *nc_name = woden_nc_name_create(env, name);
        endpoint = woden_wsdl10_endpoint_to_endpoint_element(endpoint, env);
        WODEN_WSDL10_ENDPOINT_ELEMENT_SET_NAME(endpoint, env, nc_name);
    }

    attr_binding = axis2_qname_create_from_string(env, WODEN_WSDL10_ATTR_BINDING);
    binding = axiom_element_get_attribute_value_by_name(endpoint_el, env, WODEN_WSDL10_ATTR_BINDING);
    if (binding)
    {
        axis2_hash_t *namespcs = NULL;

        desc = woden_wsdl10_desc_to_desc_element(desc, env);
        namespcs = WODEN_WSDL10_DESC_ELEMENT_GET_NAMESPACES(desc, env);
        binding_qn = woden_om_util_get_qname(env, endpoint_el_node, binding, namespcs);
        endpoint = woden_wsdl10_endpoint_to_endpoint_element(endpoint, env);
        WODEN_WSDL10_ENDPOINT_ELEMENT_SET_BINDING_QNAME(endpoint, env, binding_qn);
    }
    /* Dereference the binding qname */
    if (binding_qn)
    {
        axis2_array_list_t *bindings = NULL;
        int i = 0, size = 0;

        desc = woden_wsdl10_desc_to_desc_element(desc, env);
        bindings = WODEN_WSDL10_DESC_ELEMENT_GET_BINDING_ELEMENTS(desc, env);
        if (bindings)
            size = axis2_array_list_size(bindings, env);
        for (i = 0; i < size; i++)
        {
            void *binding = NULL;
            axis2_qname_t *qname = NULL;

            binding = axis2_array_list_get(bindings, env, i);
            binding = woden_binding_to_binding_element(binding, env);
            qname = WODEN_BINDING_ELEMENT_GET_QNAME(binding, env);
            if (AXIS2_QNAME_EQUALS(binding_qn, env, qname))
            {
                WODEN_WSDL10_ENDPOINT_SET_BINDING_ELEMENT(endpoint, env, binding);
            }
        }
    }

    attr_address = axis2_qname_create_from_string(env, WODEN_WSDL10_ATTR_ADDRESS);
    endpoint_el = AXIOM_NODE_GET_DATA_ELEMENT(endpoint_el_node, env);
    address = axiom_element_get_attribute_value_by_name(endpoint_el, env, WODEN_WSDL10_ATTR_ADDRESS);

    if (address)
    {
        axis2_uri_t *address_uri = NULL;

        address_uri = get_uri(env, address);
        endpoint = woden_wsdl10_endpoint_to_endpoint_element(endpoint, env);
        WODEN_WSDL10_ENDPOINT_ELEMENT_SET_ADDRESS(endpoint, env, address_uri);
        AXIS2_URI_FREE(address_uri, env);
    }

    endpoint = woden_wsdl10_endpoint_to_attr_extensible(endpoint, env);
    status = parse_ext_attributes(reader, env, endpoint_el_node, "endpoint_element",
            endpoint, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    /* Parse the child elements of <endpoint>.
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */
    temp_el = axiom_util_get_first_child_element(endpoint_el, env, endpoint_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_PROPERTY);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            endpoint = woden_wsdl10_endpoint_to_documentable(endpoint, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(endpoint, env, documentation);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, endpoint);
            endpoint = woden_wsdl10_endpoint_to_configurable_element(endpoint, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(endpoint, env, feature);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, endpoint);
            endpoint = woden_wsdl10_endpoint_to_configurable_element(endpoint, env);
            WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(endpoint, env, property);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "endpoint_element",
                    endpoint, temp_el_node, desc);

            endpoint = woden_wsdl10_endpoint_to_element_extensible(endpoint, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(endpoint, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return endpoint;
}

static void *
parse_feature(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *feature_el_node,
    void *desc,
    void *parent)

{
    void *feature = NULL;
    axis2_char_t *ref = NULL;
    axis2_char_t *req = NULL;
    axis2_qname_t *attr_ref = NULL;
    axis2_qname_t *attr_req = NULL;
    axiom_element_t *feature_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_bool_t required = AXIS2_FALSE;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    feature = WODEN_WSDL10_DESC_ELEMENT_CREATE_FEATURE_ELEMENT(desc, env);
    feature = woden_feature_to_nested_element(feature, env);
    WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(feature, env, parent);


    attr_ref = axis2_qname_create_from_string(env, WODEN_WSDL10_ATTR_REF);
    feature_el = AXIOM_NODE_GET_DATA_ELEMENT(feature_el_node, env);
    ref = axiom_element_get_attribute_value_by_name(feature_el, env, WODEN_WSDL10_ATTR_REF);

    if (ref)
    {
        axis2_uri_t *ref_uri = get_uri(env, ref);
        feature = woden_feature_to_feature_element(feature, env);
        WODEN_FEATURE_ELEMENT_SET_REF(feature, env, ref_uri);
        AXIS2_URI_FREE(ref_uri, env);
    }

    attr_req = axis2_qname_create_from_string(env, WODEN_WSDL10_ATTR_REQUIRED);

    req = axiom_element_get_attribute_value_by_name(feature_el, env, WODEN_WSDL10_ATTR_REQUIRED);
    if (req && 0 == AXIS2_STRCMP(WODEN_WSDL10_VALUE_TRUE, req))
        required = AXIS2_TRUE;
    else
        required = AXIS2_FALSE;
    feature = woden_feature_to_feature_element(feature, env);
    WODEN_FEATURE_ELEMENT_SET_REQUIRED(feature, env, required);
    /*TODO t.b.c. what if attr value is not 'true' or 'false'? (eg, missing,
     * mispelt or not lower case.
     */

    feature = woden_feature_to_attr_extensible(feature, env);
    status = parse_ext_attributes(reader, env, feature_el_node, "feature_element",
            feature, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    /* Parse the child elements of <feature>.
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * extension elements.
     * TODO validate that the elements are in correct order
     */
    temp_el = axiom_util_get_first_child_element(feature_el, env, feature_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_DOCUMENTATION);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            feature = woden_feature_to_documentable(feature, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(feature, env, documentation);
        }
        else
        {
            void *ext_element = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "feature_element",
                    feature, temp_el_node, desc);

            documentable = WODEN_FEATURE_GET_BASE_IMPL(feature, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return feature;
}

/* ******************************************************************
 * Parse the attributes and child elements of the <property> element.
 *
 * As per WSDL 2.0 spec, they must be in the following order if present:
 * <documentation>
 * <value> or <constraint>
 * extension elements.
 *
 * TODO validate that the elements are in correct order
 *
 * Child elements may include either a <value> or a <constraint>,
 * but not both. If a <value> element is present, a <constraint>
 * may only be present if it contains the NMToken "#value", which
 * indicates that this <property> specifies a value, not a constraint.
 *
 * This method will parse all child elements of <property> without
 * checking for any erroneous use of  <value> and <constraint>.
 * This will be done later by validation, if it's enabled.
 * If the NMToken "#value" is present in a <constraint> element,
 * this will be flagged with a boolean field in PropertyElement
 * and the constraint field will be set to null.
 *
 */

static void *
parse_property(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *property_el_node,
    void *desc,
    void *parent)

{
    void *property = NULL;
    axis2_char_t *ref = NULL;
    axis2_qname_t *attr_ref = NULL;
    void *types = NULL;
    axiom_element_t *property_el = NULL;
    axiom_element_t *temp_el = NULL;
    axiom_node_t *temp_el_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    property = WODEN_WSDL10_DESC_ELEMENT_CREATE_PROPERTY_ELEMENT(desc, env);
    property = woden_property_to_nested_element(property, env);
    WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(property, env, parent);
    types = WODEN_WSDL10_DESC_ELEMENT_GET_TYPES_ELEMENT(desc, env);
    WODEN_PROPERTY_SET_TYPES(property, env, types);

    attr_ref = axis2_qname_create_from_string(env, WODEN_WSDL10_ATTR_REF);
    property_el = AXIOM_NODE_GET_DATA_ELEMENT(property_el_node, env);
    ref = axiom_element_get_attribute_value_by_name(property_el, env, WODEN_WSDL10_ATTR_REF);

    if (ref)
    {
        axis2_uri_t *ref_uri = get_uri(env, ref);
        property = woden_property_to_property_element(property, env);
        WODEN_PROPERTY_ELEMENT_SET_REF(property, env, ref_uri);
        AXIS2_URI_FREE(ref_uri, env);
    }

    property = woden_property_to_attr_extensible(property, env);
    status = parse_ext_attributes(reader, env, property_el_node, "property_element",
            property, desc);
    if (AXIS2_SUCCESS != status)
        return NULL;

    temp_el = axiom_util_get_first_child_element(property_el, env, property_el_node,
            &temp_el_node);

    while (temp_el && temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_value = NULL;
        axis2_qname_t *q_elem_constraint = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_DOCUMENTATION);
        q_elem_value = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_VALUE);
        q_elem_constraint = axis2_qname_create_from_string(env,
                WODEN_WSDL10_Q_ELEM_CONSTRAINT);

        if (axis2_qname_util_matches(env,
                q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            property = woden_property_to_documentable(property, env);
            WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(property, env, documentation);
        }
        if (axis2_qname_util_matches(env,
                q_elem_value, temp_el_node))
        {
            axiom_child_element_iterator_t *node_list = NULL;
            axis2_generic_obj_t *node_list_obj = NULL;

            /* The property value consists of the child info items of <value> */
            node_list = axiom_element_get_child_elements(temp_el, env, temp_el_node);
            node_list_obj = axis2_generic_obj_create(env);
             axis2_generic_obj_set_value(node_list_obj, env, node_list);
            axis2_generic_obj_set_free_func(node_list_obj, env, node_list->ops->free_fn);
            property = woden_property_to_property_element(property, env);
            WODEN_PROPERTY_ELEMENT_SET_VALUE(property, env, node_list_obj);
        }
        else if (axis2_qname_util_matches(env,
                q_elem_constraint, temp_el_node))
        {
            axiom_node_t *node = NULL;
            axis2_char_t *text_value = NULL;

            /* TODO t.b.c. Assume <constraint> has just '#value' or a qname
             * and don't check for extraneous text or child elements?
             */
            node = AXIOM_NODE_GET_FIRST_CHILD(temp_el_node, env);
            if (node && AXIOM_TEXT == AXIOM_NODE_GET_NODE_TYPE(
                        node, env))
            {
                axis2_char_t *temp_str = NULL;
                int len = 0;
                axiom_text_t *text_node = NULL;

                text_node = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
                temp_str = axiom_text_get_value(text_node, env);
                text_value = AXIS2_STRTRIM(env, temp_str, NULL);
                len = AXIS2_STRLEN(text_value);
                if (len > 0)
                {
                    if (0 == AXIS2_STRCMP(text_value, WODEN_WSDL10_NMTOKEN_VALUE))
                    {
                        property = woden_property_to_property_element(property, env);
                        WODEN_PROPERTY_ELEMENT_SET_HAS_VALUE_TOKEN(property, env, AXIS2_TRUE);
                    }
                    else
                    {
                        axis2_qname_t *qname = NULL;
                        axis2_hash_t *namespcs = NULL;

                        desc = woden_wsdl10_desc_to_desc_element(desc, env);
                        namespcs = WODEN_WSDL10_DESC_ELEMENT_GET_NAMESPACES(desc, env);

                        qname = woden_om_util_get_qname(env, temp_el_node,
                                text_value, namespcs);
                        property = woden_property_to_property_element(property, env);
                        WODEN_PROPERTY_ELEMENT_SET_CONSTRAINT_QNAME(property, env, qname);
                    }
                }
            }
        }
        else
        {
            void *ext_element = NULL;
            void *documentable = NULL;
            void *wsdl_obj = NULL;
            void *wsdl_el = NULL;

            ext_element = parse_ext_element(reader, env, "property_element",
                    property, temp_el_node, desc);

            documentable = WODEN_PROPERTY_GET_BASE_IMPL(property, env);
            wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
            wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
            wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
            WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_el, env, ext_element);
        }

        temp_el = axiom_util_get_next_sibling_element(temp_el, env,
                temp_el_node, &temp_el_node);
    }

    return property;
}

static axis2_status_t
parse_ext_attributes(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *om_el_node,
    axis2_char_t *wsdl_class,
    void *wsdl_obj,
    void *desc)
{
    axis2_hash_t *node_map = NULL;
    axiom_element_t *om_el = NULL;
    axis2_hash_index_t *index = NULL;

    om_el = AXIOM_NODE_GET_DATA_ELEMENT(om_el_node, env);
    node_map = axiom_element_get_all_attributes(om_el, env);
    if (!node_map)
    {
        /* If no error condition occured then this will return
         */
        if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
            return AXIS2_FAILURE;
        else
            return AXIS2_SUCCESS;
    }
    for (index = axis2_hash_first(node_map, env); index; index =
                axis2_hash_next(env, index))
    {
        void *om_attr = NULL;
        axis2_char_t *localname = NULL;
        axiom_namespace_t *namespc = NULL;
        axis2_char_t *namespc_uri_str = NULL;
        axis2_char_t *prefix = NULL;
        axis2_qname_t *attr_type = NULL;

        axis2_hash_this(index, NULL, NULL, &om_attr);
        localname = axiom_attribute_get_localname((axiom_attribute_t *) om_attr, env);
        namespc = axiom_attribute_get_namespace((axiom_attribute_t *) om_attr, env);
        if (namespc)
        {
            namespc_uri_str = axiom_namespace_get_uri(namespc, env);
            prefix = axiom_namespace_get_prefix(namespc, env);
        }
        if (!prefix) prefix = "";
        attr_type = axis2_qname_create(env, localname, namespc_uri_str, prefix);
        if (namespc_uri_str && 0 != AXIS2_STRCMP(namespc_uri_str,
                WODEN_WSDL10_NS_URI_WSDL20))
        {
            if (0 != AXIS2_STRCMP(namespc_uri_str, WODEN_WSDL10_NS_URI_XMLNS) &&
                    0 != AXIS2_STRCMP(namespc_uri_str, WODEN_WSDL10_NS_URI_XSI))
            {
                void *xml_attr = NULL;
                woden_wsdl10_ext_registry_t *ext_reg = NULL;

                desc = woden_wsdl10_desc_to_desc_element(desc, env);
                ext_reg = WODEN_WSDL10_DESC_ELEMENT_GET_EXT_REGISTRY(desc, env);
                if (ext_reg)
                {
                    xml_attr = WODEN_WSDL10_EXT_REGISTRY_QUERY_EXT_ATTR_TYPE(
                                ext_reg, env, wsdl_class, attr_type);
                    if (xml_attr)
                    {
                        axis2_char_t *attr_value = NULL;
                        void *wsdl_el = NULL;

                        attr_value = axiom_attribute_get_value((axiom_attribute_t *) om_attr, env);
                        WODEN_XML_ATTR_INIT(xml_attr, env, om_el,
                                om_el_node, attr_type, attr_value);
                        wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
                        WODEN_ATTR_EXTENSIBLE_SET_EXT_ATTR(wsdl_el, env,
                                attr_type, xml_attr);
                    }
                }
                else
                {
                    /* This reader cannot handle extensions, so stop. */
                    break;
                }
            }
            else
            {
                /* TODO Parse xmlns namespace declarations - here or elsewhere? */
            }
        }
        else
        {
            /* TODO confirm non-native attrs in WSDL 2.0 namespace will be detected
             * by schema validation, so no need to handle error here.
             */
        }
    }
    return AXIS2_SUCCESS;
}

static void *
parse_ext_element(
    void *reader,
    const axis2_env_t *env,
    axis2_char_t *parent_type,
    void *parent,
    axiom_node_t *el_node,
    void *desc)
{
    axis2_qname_t *element_type = NULL;
    axis2_char_t *namespc_uri_str = NULL;
    axiom_namespace_t *namespc = NULL;
    axiom_element_t *el = NULL;
    woden_wsdl10_ext_registry_t *ext_reg = NULL;
    void *ext_ds = NULL;
    void *ext_el = NULL;

    element_type = axis2_qname_util_new_qname(env, el_node);

    el = AXIOM_NODE_GET_DATA_ELEMENT(el_node, env);
    namespc = axiom_element_get_namespace(el, env, el_node);
    namespc_uri_str = axiom_namespace_get_uri(namespc, env);
    if (!namespc_uri_str || (0 == AXIS2_STRCMP(namespc_uri_str,
            WODEN_WSDL10_NS_URI_WSDL20)))
    {
        /* TODO Set error message */
        return NULL;
    }

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    ext_reg = WODEN_WSDL10_DESC_ELEMENT_GET_EXT_REGISTRY(desc, env);
    if (!ext_reg)
    {
        /* TODO Set error message */
        return NULL;
    }

    ext_ds = WODEN_WSDL10_EXT_REGISTRY_QUERY_DESERIALIZER(ext_reg, env,
            parent_type, element_type);

    if (ext_ds)
    {
        ext_el = (void *) WODEN_WSDL10_EXT_DESERIALIZER_UNMARSHALL(ext_ds, env,
                parent_type, parent, element_type, el_node, desc, ext_reg);
    }

    return ext_el;
}


/* ************************************************************
 *  Utility/helper methods
 * ************************************************************/

/**
 * Check the actual element encountered against the expected qname
 *
 * @param el actual element encountered
 * @param qname expected element's qname
 */
static axis2_status_t
check_element_qname(
    void *reader,
    const axis2_env_t *env,
    axiom_node_t *el_node,
    axis2_qname_t *qname)
{
    if (AXIS2_TRUE != axis2_qname_util_matches(env, qname, el_node))
    {
        /* TODO woden_wsdl_exc_set_location(wsdl_exc, env, xpath);*/
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/*
 * Retrieve a WSDL document by resolving the location URI specified
 * on a WSDL &lt;import&gt; or &lt;include&gt; element.
 *
 * TODO add support for a URL Catalog Resolver
 */
static void *
get_wsdl_from_location(
    void *reader,
    const axis2_env_t *env,
    axis2_char_t *location_uri_str,
    void *desc,
    axis2_hash_t *wsdl_modules)
{
    axis2_uri_t *context_uri = NULL;
    axis2_uri_t *location_uri = NULL;
    axis2_char_t *location_str = NULL;
    void *referenced_desc = NULL;
    axiom_node_t *doc_el_node = NULL;

    desc = woden_wsdl10_desc_to_desc_element(desc, env);
    context_uri = WODEN_WSDL10_DESC_ELEMENT_GET_DOCUMENT_BASE_URI(desc, env);
    location_uri = axis2_uri_parse_relative(env, context_uri, location_uri_str);
    if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
    {
        /* Can't continue import with a bad URL.*/
        return NULL;
    }
    location_str = AXIS2_URI_TO_STRING(location_uri, env, AXIS2_URI_UNP_OMITUSERINFO);

    /* Check if WSDL imported or included previously from this location.*/
    referenced_desc = axis2_hash_get(wsdl_modules, location_str, AXIS2_HASH_KEY_STRING);

    if (!referenced_desc)
    {
        /* not previously imported or included, so retrieve the WSDL.*/
        void *schema_col = NULL;
        void *xml_reader = NULL;
        axiom_stax_builder_t *builder = NULL;
        axiom_document_t *doc = NULL;
        axis2_qname_t *qname = NULL;

        schema_col = xml_schema_collection_create(env);
        xml_reader = axiom_xml_reader_create_for_file(env, location_str, NULL);
        builder = axiom_stax_builder_create(env, xml_reader);
        doc = axiom_stax_builder_get_document(builder, env);
        doc_el_node = axiom_document_get_root_element(doc, env);
        if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
        {
            /* Cannot contine without the referenced document */
            return NULL;
        }
        /* The referenced document should contain a WSDL <description> */
        qname = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_DEFINITIONS);
        if (AXIS2_TRUE != axis2_qname_util_matches(env, qname, doc_el_node))
        {
            /* Cannot continue without a <description> element */
            AXIS2_QNAME_FREE(qname, env);
            return NULL;
        }
        AXIS2_QNAME_FREE(qname, env);

        referenced_desc = parse_desc(reader, env, location_str,
                doc_el_node, wsdl_modules);
        if (AXIS2_TRUE != axis2_hash_contains_key(wsdl_modules, env,
                location_str))
        {
            axis2_hash_set(wsdl_modules, location_str, AXIS2_HASH_KEY_STRING,
                    referenced_desc);
        }
    }

    return referenced_desc;
}

/*
 * Convert a string of type xs:anyURI to a axis2_uri.
 * An empty string argument will return an empty string URI.
 * A null argument will return a null.
 */
static axis2_uri_t *
get_uri(
    const axis2_env_t *env,
    const axis2_char_t *uri_str)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, uri_str, NULL);

    return axis2_uri_parse_string(env, uri_str);
}

