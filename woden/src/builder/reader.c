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

#include <woden/builder/woden_reader.h>
#include <woden/axis2_woden_schema_constants.h>
#include <woden/util/axis2_qname_util.h>

#include <woden/wsdl20/axis2_woden_binding_fault.h>
#include <woden/wsdl20/axis2_woden_binding_fault_ref.h>
#include <woden/wsdl20/axis2_woden_binding.h>
#include <woden/wsdl20/axis2_woden_binding_msg_ref.h>
#include <woden/wsdl20/axis2_woden_binding_op.h>
#include <woden/wsdl20/axis2_woden_configurable_component.h>
#include <woden/wsdl20/axis2_woden_configurable.h>
#include <woden/wsdl20/axis2_woden_desc.h>
#include <woden/wsdl20/axis2_woden_documentable.h>
#include <woden/wsdl20/axis2_woden_documentation.h>
#include <woden/wsdl20/axis2_woden_element_decl.h>
#include <woden/wsdl20/axis2_woden_endpoint.h>
#include <woden/wsdl20/axis2_woden_feature.h>
#include <woden/wsdl20/axis2_woden_import.h>
#include <woden/wsdl20/axis2_woden_include.h>
#include <woden/wsdl20/axis2_woden_interface_fault.h>
#include <woden/wsdl20/axis2_woden_interface_fault_ref.h>
#include <woden/wsdl20/axis2_woden_interface.h>
#include <woden/wsdl20/axis2_woden_interface_msg_ref.h>
#include <woden/wsdl20/axis2_woden_interface_op.h>
#include <woden/wsdl20/axis2_woden_nested_component.h>
#include <woden/wsdl20/axis2_woden_nested_configurable.h>
#include <woden/wsdl20/axis2_woden_property.h>
#include <woden/wsdl20/axis2_woden_svc.h>
#include <woden/wsdl20/axis2_woden_type_def.h>
#include <woden/wsdl20/axis2_woden_types.h>
#include <woden/wsdl20/axis2_woden_wsdl_component.h>
#include <woden/wsdl20/axis2_woden_wsdl_ref.h>

#include <woden/wsdl20/extensions/axis2_woden_attr_extensible.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/wsdl20/extensions/axis2_woden_element_extensible.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_registry.h>

#include <woden/wsdl20/enumeration/axis2_woden_direction.h>
#include <woden/wsdl20/enumeration/axis2_woden_msg_label.h>
#include <woden/wsdl20/enumeration/axis2_woden_soap_fault_code.h>
#include <woden/wsdl20/enumeration/axis2_woden_soap_fault_subcodes.h>

#include <woden/wsdl20/xml/axis2_woden_binding_element.h>
#include <woden/wsdl20/xml/axis2_woden_binding_fault_element.h>
#include <woden/wsdl20/xml/axis2_woden_binding_fault_ref_element.h>
#include <woden/wsdl20/xml/axis2_woden_binding_msg_ref_element.h>
#include <woden/wsdl20/xml/axis2_woden_binding_op_element.h>
#include <woden/wsdl20/xml/axis2_woden_configurable_element.h>
#include <woden/wsdl20/xml/axis2_woden_desc_element.h>
#include <woden/wsdl20/xml/axis2_woden_documentable_element.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <woden/wsdl20/xml/axis2_woden_endpoint_element.h>
#include <woden/wsdl20/xml/axis2_woden_fault_ref_element.h>
#include <woden/wsdl20/xml/axis2_woden_feature_element.h>
#include <woden/wsdl20/xml/axis2_woden_import_element.h>
#include <woden/wsdl20/xml/axis2_woden_include_element.h>
#include <woden/wsdl20/xml/axis2_woden_interface_element.h>
#include <woden/wsdl20/xml/axis2_woden_interface_fault_element.h>
#include <woden/wsdl20/xml/axis2_woden_interface_fault_ref_element.h>
#include <woden/wsdl20/xml/axis2_woden_interface_msg_ref_element.h>
#include <woden/wsdl20/xml/axis2_woden_interface_op_element.h>
#include <woden/wsdl20/xml/axis2_woden_nested_element.h>
#include <woden/wsdl20/xml/axis2_woden_property_element.h>
#include <woden/wsdl20/xml/axis2_woden_svc_element.h>
#include <woden/wsdl20/xml/axis2_woden_types_element.h>
#include <woden/wsdl20/xml/axis2_woden_wsdl_element.h>

#include <woden/xml/axis2_woden_bool_attr.h>
#include <woden/xml/axis2_woden_qname_attr.h>
#include <woden/xml/axis2_woden_qname_list_attr.h>
#include <woden/xml/axis2_woden_qname_list_or_token_any_attr.h>
#include <woden/xml/axis2_woden_qname_or_token_any_attr.h>
#include <woden/xml/axis2_woden_string_attr.h>
#include <woden/xml/axis2_woden_uri_attr.h>
#include <woden/xml/axis2_woden_xml_attr.h>

#include <woden/schema/axis2_woden_schema.h>
#include <woden/schema/axis2_woden_imported_schema.h>
#include <woden/schema/axis2_woden_inlined_schema.h>

#include "../wsdl20/woden_constants.h"
#include "../util/woden_om_util.h"

#include <xml_schema/axis2_xml_schema_collection.h>

#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_attribute.h>
#include <axis2_om_child_element_iterator.h>
#include <axis2_om_namespace.h>
#include <axis2_om_text.h>
#include <axis2_om_document.h>
#include <axis2_om_stax_builder.h>
#include <axis2_xml_reader.h>
#include <axis2_om_util.h>

#include <axis2_string_util.h>

typedef struct woden_reader_impl woden_reader_impl_t;

/** 
 * @brief Woden Wsdl Reader Struct Impl
 *   Woden Wsdl Reader  
 */ 
struct woden_reader_impl
{
    woden_reader_t reader;

    axis2_om_document_t *om_doc;
    axis2_om_node_t *root_node;
    /* A map of imported schema definitions keyed by schema location URI */
    axis2_hash_t *f_imported_schemas;
    axis2_woden_ext_registry_t *f_ext_reg;
    axis2_uri_t *f_uri;

};

#define INTF_TO_IMPL(reader) ((woden_reader_impl_t *) reader)

axis2_status_t AXIS2_CALL 
woden_reader_free(
        void *reader,
        const axis2_env_t *env);

void *AXIS2_CALL
woden_reader_read_wsdl(
        void *reader,
        const axis2_env_t *env,
        axis2_om_document_t *om_doc,
        axis2_char_t *uri);

axis2_status_t AXIS2_CALL
woden_reader_set_ext_registry(
        void *reader,
        const axis2_env_t *env,
        axis2_woden_ext_registry_t *ext_reg);

axis2_woden_ext_registry_t *AXIS2_CALL
woden_reader_get_ext_registry(
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
        axis2_char_t *document_base_uri, 
        axis2_om_node_t *desc_el_node,
        axis2_hash_t *wsdl_modules);

static void *
parse_documentation(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *doc_el_node,
        void *desc);

static void *
parse_import(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *import_el_node,
        void *desc,
        axis2_hash_t *wsdl_modules);
    
static void *
parse_include(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *include_el_node,
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
        axis2_om_node_t *types_el_node,
        void *desc);

static void *
parse_schema_inline(
        void *reader, 
        const axis2_env_t *env,
        axis2_om_node_t *schema_el_node,
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
        axis2_om_node_t *import_el_node,
        void *desc);

static void *
parse_interface(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *interface_el_node,
        void *desc);

/* Parse the attributes and child elements of interface <fault>. 
 * As per WSDL 2.0 spec, the child elements must be in the following order if present:
 * <documentation>
 * <feature> <property> or extension elements in any order
 * 
 * TODO validate that the elements are in correct order
 */ 
static void *
parse_interface_fault(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *fault_el_node,
        void *desc,
        void *parent);

static void *
parse_interface_op(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *op_el_node,
        void *desc,
        void *parent);

static void *
parse_interface_fault_ref(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *fault_ref_el_node,
        void *desc,
        void *parent);

static void *
parse_interface_msg_ref(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *msg_ref_el_node,
        void *desc,
        void *parent);

static void *
parse_binding(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *binding_el_node,
        void *desc);

static void *
parse_binding_fault(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *fault_el_node,
        void *desc,
        void *parent);

static void *
parse_binding_op(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *op_el_node,
        void *desc,
        void *parent);

static void *
parse_binding_fault_ref(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *fault_ref_el_node,
        void *desc,
        void *parent);

static void *
parse_binding_msg_ref(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *msg_ref_el_node,
        void *desc,
        void *parent);

static void *
parse_svc(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *svc_el_node,
        void *desc);

static void *
parse_endpoint(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *endpoint_el_node,
        void *desc,
        void *parent);

static void *
parse_feature(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *feature_el_node,
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
        axis2_om_node_t *property_el_node,
        void *desc,
        void *parent);

axis2_status_t AXIS2_CALL
parse_ext_attributes(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *om_el_node,
        axis2_char_t *wsdl_class,
        void *wsdl_obj,
        void *desc);

static void *
parse_ext_element(
        void *reader,
        const axis2_env_t *env,
        axis2_char_t *parent_type,
        void *parent,
        axis2_om_node_t *el_node,
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
        axis2_om_node_t *el_node,
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
        void *reader,
        const axis2_env_t *env,
        axis2_char_t *uri_str);

static woden_reader_t *
create(
        const axis2_env_t *env)
{
    woden_reader_impl_t *reader_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    reader_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_reader_impl_t));

    reader_impl->f_imported_schemas = NULL;
    reader_impl->f_ext_reg = NULL;
    reader_impl->f_uri = NULL;
    
    reader_impl->reader.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_reader_ops_t));
    
    reader_impl->f_imported_schemas = axis2_hash_make(env);
    if(reader_impl->f_imported_schemas)
    {
        AXIS2_SET_ERROR(env->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
    }
    
    reader_impl->reader.ops->free = woden_reader_free;

    return &(reader_impl->reader);
}

AXIS2_EXTERN woden_reader_t * AXIS2_CALL
woden_reader_create(
        const axis2_env_t *env)
{
    woden_reader_impl_t *reader_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    reader_impl = (woden_reader_impl_t *) create(env);

    return &(reader_impl->reader);
}

axis2_status_t AXIS2_CALL
woden_reader_free(
        void *reader,
        const axis2_env_t *env)
{
    woden_reader_impl_t *reader_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    reader_impl = INTF_TO_IMPL(reader);

    if(reader_impl->f_imported_schemas)
    {
        axis2_hash_free(reader_impl->f_imported_schemas, env);
        reader_impl->f_imported_schemas = NULL;
    }

    if(reader_impl->f_ext_reg)
    {
        AXIS2_WODEN_EXT_REGISTRY_FREE(reader_impl->f_ext_reg, env);
        reader_impl->f_ext_reg = NULL;
    }

    if(reader_impl->f_uri)
    {
        AXIS2_URI_FREE(reader_impl->f_uri, env);
        reader_impl->f_uri = NULL;
    }

    if((&(reader_impl->reader))->ops)
    {
        AXIS2_FREE(env->allocator, (&(reader_impl->reader))->ops);
        (&(reader_impl->reader))->ops = NULL;
    }

    if(reader_impl)
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
woden_reader_set_ext_registry(
        void *reader,
        const axis2_env_t *env,
        axis2_woden_ext_registry_t *ext_reg)
{
    woden_reader_impl_t *reader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ext_reg, AXIS2_FAILURE);
    reader_impl = INTF_TO_IMPL(reader);

    if(reader_impl->f_ext_reg)
    {
        AXIS2_WODEN_EXT_REGISTRY_FREE(reader_impl->f_ext_reg, env);
        reader_impl->f_ext_reg = NULL;
    }
    reader_impl->f_ext_reg = ext_reg;
    return AXIS2_SUCCESS;
}

axis2_woden_ext_registry_t *AXIS2_CALL
woden_reader_get_ext_registry(
        void *reader,
        const axis2_env_t *env)
{
    woden_reader_impl_t *reader_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    reader_impl = INTF_TO_IMPL(reader);

    return reader_impl->f_ext_reg;
}

void *AXIS2_CALL
woden_reader_read_wsdl(
        void *reader,
        const axis2_env_t *env,
        axis2_om_document_t *om_doc,
        axis2_char_t *uri) 
{
    woden_reader_impl_t *reader_impl = NULL;
    void *desc = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_doc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, uri, AXIS2_FAILURE);
    reader_impl = INTF_TO_IMPL(reader);
        
    /* TODO add WSDL locator for resolving URIs */
    reader_impl->om_doc = om_doc;
    reader_impl->root_node = AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(om_doc, env);            
    if(!reader_impl->root_node)
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
        axis2_char_t *document_base_uri, 
        axis2_om_node_t *desc_el_node,
        axis2_hash_t *wsdl_modules)
{
    woden_reader_impl_t *reader_impl = NULL;
    void *desc = NULL;
    void *ext_reg = NULL;
    axis2_uri_t *uri = NULL;
    axis2_char_t *target_namespc = NULL;
    axis2_uri_t *target_namespc_uri = NULL;
    axis2_om_element_t *desc_el = NULL;
    axis2_hash_t *attrs = NULL;
    axis2_hash_index_t *index = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;
    void *types = NULL;
    axis2_qname_t *qname = NULL;
    void *schema = NULL;
    axis2_om_attribute_t *attr_ns = NULL;
    axis2_om_attribute_t *attr_loc = NULL;
    axis2_om_element_t *schema_elem = NULL;
    axis2_om_node_t *schema_elem_node = NULL;
    axis2_om_namespace_t *schema_ns = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, document_base_uri, NULL);
    AXIS2_PARAM_CHECK(env->error, desc_el_node, NULL);
    reader_impl = INTF_TO_IMPL(reader);

    qname = axis2_qname_create_from_string(env, WODEN_Q_ELEM_DESCRIPTION);
    check_element_qname(reader, env, desc_el_node, qname);
    
    desc = axis2_woden_desc_create(env);
    ext_reg = axis2_woden_ext_registry_create(env);
    desc = axis2_woden_desc_to_desc_element(desc, env);
    AXIS2_WODEN_DESC_ELEMENT_SET_EXT_REGISTRY(desc, env, ext_reg);
    
    if(NULL == wsdl_modules) 
    {
        /* This is the initial WSDL document. No imports or includes yet.
         * TODO this might be the place to flag the initial Desc if necessary.
         */
        wsdl_modules = axis2_hash_make(env);
    }
    ext_reg = woden_reader_get_ext_registry(reader, env);
    if(NULL != ext_reg) 
    {
        AXIS2_WODEN_DESC_ELEMENT_SET_EXT_REGISTRY(desc, env, ext_reg);
    }
  
    uri = get_uri(reader, env, document_base_uri);
    AXIS2_WODEN_DESC_ELEMENT_SET_DOCUMENT_BASE_URI(desc, env, uri);

    desc_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(desc_el_node, env);
    target_namespc = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(desc_el, 
            env, WODEN_ATTR_TARGET_NAMESPACE); 
    
    if(NULL != target_namespc)
    {
        target_namespc_uri = get_uri(reader, env, target_namespc);
        AXIS2_WODEN_DESC_ELEMENT_SET_TARGET_NAMESPACE(desc, env, target_namespc_uri);
    }
    
    /* parse the namespace declarations */
    attrs = AXIS2_OM_ELEMENT_GET_ALL_ATTRIBUTES(desc_el, env);
    for (index = axis2_hash_first (attrs, env); index; index = axis2_hash_next (
                env, index))
    {
        void *v = NULL;
        axis2_om_attribute_t *attr = NULL;
        axis2_om_namespace_t *namespc = NULL;
        axis2_char_t *namespc_uri = NULL;
        axis2_char_t *local_part = NULL;
        axis2_char_t *value = NULL;
        
        axis2_hash_this (index, NULL, NULL, &v);
        attr = (axis2_om_attribute_t *) v;
        namespc = AXIS2_OM_ATTRIBUTE_GET_NAMESPACE(attr, env);
        namespc_uri = AXIS2_OM_NAMESPACE_GET_URI(namespc, env);
        local_part = AXIS2_OM_ATTRIBUTE_GET_LOCALNAME(attr, env);
        value = AXIS2_OM_ATTRIBUTE_GET_VALUE(attr, env);
        uri = get_uri(reader, env, value);
        if(0 == AXIS2_STRCMP(WODEN_NS_URI_XMLNS, namespc_uri))
        {
            if(0 == AXIS2_STRCMP(WODEN_ATTR_XMLNS, local_part))
            {
                AXIS2_WODEN_DESC_ELEMENT_ADD_NAMESPACE(desc, env, local_part, uri); 
                    /* a prefixed namespace */
            }
            else
            {
                AXIS2_WODEN_DESC_ELEMENT_ADD_NAMESPACE(desc, env, NULL, uri); 
            }
        }

    }
    desc = axis2_woden_desc_to_attr_extensible(desc, env); 
    parse_ext_attributes(reader, env, desc_el_node, "description_element", desc, desc);
    
    /* Parse the child elements */
    temp_el = axis2_om_util_get_first_child_element(desc_el, env, desc_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_import = NULL;
        axis2_qname_t *q_elem_include = NULL;
        axis2_qname_t *q_elem_types = NULL;
        axis2_qname_t *q_elem_interface = NULL;
        axis2_qname_t *q_elem_binding = NULL;
        axis2_qname_t *q_elem_svc = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_import = axis2_qname_create_from_string(env, WODEN_Q_ELEM_IMPORT);
        q_elem_include = axis2_qname_create_from_string(env, WODEN_Q_ELEM_INCLUDE);
        q_elem_types = axis2_qname_create_from_string(env, WODEN_Q_ELEM_TYPES);
        q_elem_interface = axis2_qname_create_from_string(env, WODEN_Q_ELEM_INTERFACE);
        q_elem_binding = axis2_qname_create_from_string(env, WODEN_Q_ELEM_BINDING);
        q_elem_svc = axis2_qname_create_from_string(env, WODEN_Q_ELEM_SERVICE);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            desc = axis2_woden_desc_to_documentable(desc, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(desc, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_import, temp_el_node))
        {
            void *import_element = NULL;

            if(NULL != document_base_uri && AXIS2_TRUE != 
                    axis2_hash_contains_key(wsdl_modules, env, 
                        document_base_uri))
            {
                axis2_hash_set(wsdl_modules, document_base_uri, 
                        AXIS2_HASH_KEY_STRING, desc);
            }
            import_element = parse_import(reader, env, temp_el_node, desc, 
                    wsdl_modules);
            desc = axis2_woden_desc_to_desc_element(desc, env);
            AXIS2_WODEN_DESC_ELEMENT_ADD_IMPORT_ELEMENT(desc, env, import_element);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_include, temp_el_node))
        {
            void *include_element = NULL;

            if(NULL != document_base_uri && AXIS2_TRUE != 
                    axis2_hash_contains_key(wsdl_modules, env, 
                        document_base_uri))
            {
                axis2_hash_set(wsdl_modules, document_base_uri, 
                        AXIS2_HASH_KEY_STRING, desc);
            }
            include_element = parse_include(reader, env, temp_el_node, desc, 
                    wsdl_modules);
            AXIS2_WODEN_DESC_ELEMENT_ADD_INCLUDE_ELEMENT(desc, env, include_element);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_types, temp_el_node))
        {
            void *types = NULL;

            types = parse_types(reader, env, temp_el_node, desc);
            AXIS2_WODEN_DESC_ELEMENT_SET_TYPES_ELEMENT(desc, env, types);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_interface, temp_el_node))
        {
            void *interface = NULL;

            interface = parse_interface(reader, env, temp_el_node, desc);
            AXIS2_WODEN_DESC_ELEMENT_ADD_INTERFACE_ELEMENT(desc, env, interface);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_binding, temp_el_node))
        {
            void *binding = NULL;

            binding = parse_binding(reader, env, temp_el_node, desc);
            AXIS2_WODEN_DESC_ELEMENT_ADD_BINDING_ELEMENT(desc, env, binding);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_svc, temp_el_node))
        {
            void *svc = NULL;

            svc = parse_svc(reader, env, temp_el_node, desc);
            AXIS2_WODEN_DESC_ELEMENT_ADD_SVC_ELEMENT(desc, env, svc);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "desc_element", desc, temp_el_node, desc);
            desc = axis2_woden_desc_to_element_extensible(desc, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(desc, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
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
    desc = axis2_woden_desc_to_desc_element(desc, env);
    types = AXIS2_WODEN_DESC_ELEMENT_GET_TYPES_ELEMENT(desc, env);
    if(NULL == types)
    {
       
        types = AXIS2_WODEN_DESC_ELEMENT_CREATE_TYPES_ELEMENT(desc, env);
        types = axis2_woden_types_to_types_element(types, env);
        AXIS2_WODEN_TYPES_ELEMENT_SET_TYPE_SYSTEM(types, env, WODEN_TYPE_XSD_2001);
        AXIS2_WODEN_DESC_ELEMENT_SET_TYPES_ELEMENT(desc, env, types);
    }
    schema_ns = axis2_om_namespace_create(env, 
            "http://www.w3.org/2001/XMLSchema", "import");
    schema_elem = axis2_om_element_create(env, desc_el_node, "import", 
            schema_ns, &schema_elem_node);
    attr_ns = axis2_om_attribute_create(env, "namespace", 
            "http://www.w3.org/2001/XMLSchema", NULL);
    attr_loc = axis2_om_attribute_create(env, "schemaLocation", 
            "http://www.w3.org/2001/XMLSchema.xsd", NULL);
    AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(schema_elem, env, attr_ns, schema_elem_node);
    AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(schema_elem, env, attr_loc, schema_elem_node);
    
    schema = parse_schema_import(reader, env, schema_elem_node, desc);
    types = axis2_woden_types_to_types_element(types, env);
    AXIS2_WODEN_TYPES_ELEMENT_ADD_SCHEMA(types, env, (axis2_xml_schema_t *) schema);
    
    return desc;
}

static void *
parse_documentation(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *doc_el_node,
        void *desc)
{
    void *documentation = NULL;
    axis2_om_element_t *doc_el = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;
    
    desc = axis2_woden_desc_to_desc_element(desc, env);
    documentation = AXIS2_WODEN_DESC_ELEMENT_CREATE_DOCUMENTATION_ELEMENT(desc, 
            env);
    
    /* TODO store doc_el as below, or just extract any text? */
    documentation = axis2_woden_documentation_to_documentation_element(
            documentation, env);
    AXIS2_WODEN_DOCUMENTATION_ELEMENT_SET_CONTENT(documentation, env, doc_el_node);
    
    /* Now parse any extensibility attributes or elements */
    documentation = axis2_woden_documentation_to_attr_extensible(documentation, 
            env);
    parse_ext_attributes(reader, env, doc_el_node, "documentation_element", 
            documentation, desc); 

    doc_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(doc_el_node, env);
    temp_el = axis2_om_util_get_first_child_element(doc_el, env, doc_el_node, 
            &temp_el_node); 

    while (NULL != temp_el)
    {
        void *ext_el = NULL;

        ext_el = parse_ext_element(reader, env, "documentation_element", 
                documentation, temp_el_node, desc);
        documentation = axis2_woden_documentation_to_element_extensible(
                documentation, env);
        AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(documentation, env, 
                ext_el);
        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return documentation;
}

static void *
parse_import(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *import_el_node,
        void *desc,
        axis2_hash_t *wsdl_modules)
{
    void *imp = NULL;
    axis2_om_element_t *import_el = NULL;
    axis2_char_t *namespc_uri = NULL;
    axis2_char_t *location_uri = NULL;

    imp = AXIS2_WODEN_DESC_ELEMENT_CREATE_IMPORT_ELEMENT(desc, env);
    import_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(import_el_node, env);
    
    namespc_uri = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(import_el, env, 
            WODEN_ATTR_NAMESPACE); 
    location_uri = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(import_el, env, 
            WODEN_ATTR_LOCATION); 
  
    imp = axis2_woden_import_to_attr_extensible(imp, env);
    parse_ext_attributes(reader, env, import_el_node, "import_element", imp, desc);

    if(NULL != namespc_uri) 
    {
        axis2_uri_t *uri = NULL;

        /* TODO handle missing namespace attribute (REQUIRED attr) */
        uri = get_uri(reader, env, namespc_uri);
        AXIS2_WODEN_IMPORT_SET_NAMESPACE(imp, env, uri);
    }
    
    if(NULL != location_uri)
    {
        axis2_uri_t *uri = NULL;
        void *imported_desc = NULL;
        
        /* TODO handle missing locationURI (OPTIONAL attr) */
        imp = axis2_woden_import_to_import_element(imp, env);
        uri = get_uri(reader, env, location_uri);
        AXIS2_WODEN_IMPORT_ELEMENT_SET_LOCATION(imp, env, uri);
        
        imported_desc = get_wsdl_from_location(reader, env, location_uri, desc, 
                wsdl_modules);
        AXIS2_WODEN_IMPORT_ELEMENT_SET_DESC_ELEMENT(imp, env, imported_desc);
    }
    
    return imp;
}
    
static void *
parse_include(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *include_el_node,
        void *desc,
        axis2_hash_t *wsdl_modules)
{
    void *include = NULL;
    axis2_om_element_t *include_el = NULL;
    axis2_char_t *location_uri = NULL;

    include = AXIS2_WODEN_DESC_ELEMENT_CREATE_INCLUDE_ELEMENT(desc, env);
    include_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(include_el_node, env);
    
    location_uri = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(include_el, env, 
            WODEN_ATTR_LOCATION); 
  
    include = axis2_woden_include_to_attr_extensible(include, env);
    parse_ext_attributes(reader, env, include_el_node, "include_element", 
            include, desc);

    if(NULL != location_uri)
    {
        axis2_uri_t *uri = NULL;
        void *included_desc = NULL;
       
        uri = get_uri(reader, env, location_uri);
        include = axis2_woden_include_to_include_element(include, env);
        AXIS2_WODEN_INCLUDE_ELEMENT_SET_LOCATION(include, env, uri);
        
        included_desc = get_wsdl_from_location(reader, env, location_uri, desc, 
                wsdl_modules);
        AXIS2_WODEN_INCLUDE_ELEMENT_SET_DESC_ELEMENT(include, env, included_desc);
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
        axis2_om_node_t *types_el_node,
        void *desc)
{
    void *types = NULL;
    axis2_om_element_t *types_el = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;
    
    desc = axis2_woden_desc_to_desc_element(desc, env);
    types = AXIS2_WODEN_DESC_ELEMENT_CREATE_TYPES_ELEMENT(desc, env);
    types = axis2_woden_types_to_types_element(types, env);
    AXIS2_WODEN_TYPES_ELEMENT_SET_TYPE_SYSTEM(types, env, WODEN_TYPE_XSD_2001);
 
    types = axis2_woden_types_to_attr_extensible(types, env); 
    parse_ext_attributes(reader, env, types_el_node, "types_element", 
            types, desc);
    
    types_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(types_el_node, env);
    temp_el = axis2_om_util_get_first_child_element(types_el, env, types_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_temp_el_type = NULL;

        q_temp_el_type = axis2_qname_util_new_qname(env, temp_el_node);
        q_elem_documentation = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_DOCUMENTATION);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            types = axis2_woden_types_to_documentable(types, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(types, env, documentation);
        }
        if(AXIS2_TRUE == woden_schema_constants_compare_import(q_temp_el_type, env))
        {
            axis2_woden_schema_t *schema = NULL;

            schema = parse_schema_import(reader, env, temp_el_node, desc);
            types = axis2_woden_types_to_types_element(types, env);
            AXIS2_WODEN_TYPES_ELEMENT_ADD_SCHEMA(types, env, (axis2_xml_schema_t *) schema);
        }
        if(AXIS2_TRUE == woden_schema_constants_compare_schema(q_temp_el_type, env))
        {
            axis2_woden_schema_t *schema = NULL;

            schema = parse_schema_inline(reader, env, temp_el_node, desc);
            types = axis2_woden_types_to_types_element(types, env);
            AXIS2_WODEN_TYPES_ELEMENT_ADD_SCHEMA(types, env, 
                    (axis2_xml_schema_t *) schema);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "types_element", 
                    types, temp_el_node, desc);
            types = axis2_woden_types_to_element_extensible(types, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(types, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return types;
}

static void *
parse_schema_inline(
        void *reader, 
        const axis2_env_t *env,
        axis2_om_node_t *schema_el_node,
        void *desc)
{
    void *schema = axis2_woden_inlined_schema_create(env);
    axis2_char_t *attr_id = NULL;
    axis2_char_t *tns = NULL;
    axis2_uri_t *base_uri = NULL;
    axis2_char_t *base_uri_str = NULL;
    axis2_xml_schema_t *schema_def = NULL;
    axis2_xml_schema_collection_t *xsc = NULL;
    axis2_om_element_t *schema_el = NULL;
  
    schema_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(schema_el_node, env);
    attr_id = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(schema_el, env, 
            WODEN_ATTR_ID); 
    AXIS2_WODEN_INLINED_SCHEMA_SET_ID(schema, env, attr_id);
    tns = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(schema_el, env, 
            WODEN_ATTR_TARGET_NAMESPACE); 
   
    if(NULL != tns)
    {
        void *base = AXIS2_WODEN_INLINED_SCHEMA_GET_BASE_IMPL(schema, env);
        axis2_uri_t *uri = get_uri(reader, env, tns);
        AXIS2_WODEN_SCHEMA_SET_NAMESPACE(base, env, uri);
    }
    desc = axis2_woden_desc_to_desc_element(desc, env);
    base_uri = AXIS2_WODEN_DESC_ELEMENT_GET_DOCUMENT_BASE_URI(desc, env);
    if(base_uri)
        base_uri_str = AXIS2_URI_TO_STRING(base_uri, env, 
                AXIS2_URI_UNP_OMITUSERINFO);
   
    xsc = axis2_xml_schema_collection_create(env);
    /* TODO Temporarily assume that imported schemas are stored as files
     * with the url as file name
     */
    schema_def = AXIS2_XML_SCHEMA_COLLECTION_READ_ELEMENT_WITH_URI(xsc, 
            env, schema_el_node, base_uri_str);
    if(AXIS2_ERROR_NONE != AXIS2_ERROR_GET_STATUS_CODE(env->error))
    {
        return NULL;
    }
    if(schema_def)
    {
        AXIS2_WODEN_SCHEMA_SET_SCHEMA_DEF(schema, env, schema_def);
    }
    else
        AXIS2_WODEN_SCHEMA_SET_REFERENCEABLE(schema, env, AXIS2_FALSE);
    
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
        axis2_om_node_t *import_el_node,
        void *desc)
{
    woden_reader_impl_t *reader_impl = NULL;
    void *schema = NULL;
    axis2_char_t *ns = NULL;
    axis2_char_t *sloc = NULL;
    axis2_char_t *schema_uri = NULL;
    axis2_uri_t *uri = NULL;
    void *schema_def = NULL; 
    axis2_uri_t *context_uri = NULL;
    axis2_char_t *schema_loc = NULL;
    axis2_om_element_t *import_el = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, desc, AXIS2_FAILURE);
    reader_impl = INTF_TO_IMPL(reader);

    import_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(import_el_node, env);
    ns = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(import_el, env, 
            WODEN_ATTR_NAMESPACE);
    if(NULL != ns)
    {
        uri = get_uri(reader, env, ns);
        schema = axis2_woden_imported_schema_to_schema(schema, env);
        AXIS2_WODEN_SCHEMA_SET_NAMESPACE(schema, env, uri);
    }
    sloc = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(import_el, env, 
            WODEN_ATTR_SCHEMA_LOCATION);
    if(NULL != sloc)
    {
    uri = get_uri(reader, env, sloc);
    AXIS2_WODEN_IMPORTED_SCHEMA_SET_LOCATION(schema, env, uri);
    }
        
    if(NULL == AXIS2_WODEN_SCHEMA_GET_NAMESPACE(schema, env))
    {
        /* The namespace attribute is REQUIRED on xs:import, so don't continue. */
        AXIS2_WODEN_SCHEMA_SET_REFERENCEABLE(schema, env, AXIS2_FALSE);
        return schema;
    }
    
    if(NULL == AXIS2_WODEN_IMPORTED_SCHEMA_GET_LOCATION(schema, env))
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
    desc = axis2_woden_desc_to_desc_element(desc, env);
    context_uri = AXIS2_WODEN_DESC_ELEMENT_GET_DOCUMENT_BASE_URI(desc, env);
    uri = AXIS2_WODEN_IMPORTED_SCHEMA_GET_LOCATION(schema, env);
    schema_loc = AXIS2_URI_TO_STRING(uri, env, AXIS2_URI_UNP_OMITUSERINFO);
    uri = axis2_uri_parse_relative(env, context_uri, schema_loc);
    if(AXIS2_ERROR_NONE != AXIS2_ERROR_GET_STATUS_CODE(env->error))
    {
        /* can't continue schema retrieval with a bad URL.*/
        AXIS2_WODEN_SCHEMA_SET_REFERENCEABLE(schema, env, AXIS2_FALSE);
        return schema;
    }

    schema_uri = AXIS2_URI_TO_STRING(uri, env, AXIS2_URI_UNP_OMITUSERINFO);
    
    /* If the schema has already been imported, reuse it. */
    schema_def = axis2_hash_get(reader_impl->f_imported_schemas, schema_uri, 
            AXIS2_HASH_KEY_STRING);
    if(NULL != schema_def)
    {
        /* Not previously imported, so retrieve it now. */
        axis2_xml_schema_collection_t *schema_col = NULL;
        axis2_xml_reader_t *xml_reader = NULL;
        axis2_om_document_t *imported_schema_doc = NULL;
        axis2_om_stax_builder_t *xml_builder = NULL;

        schema_col = axis2_xml_schema_collection_create(env);
        /* TODO Temporarily assume that imported schemas are stored as files
         * with the url as file name
         */
        xml_reader = axis2_xml_reader_create_for_file(env, schema_uri, NULL);
        xml_builder = axis2_om_stax_builder_create(env, reader);
        imported_schema_doc = axis2_om_document_create(env, NULL, xml_builder);
        schema_def = AXIS2_XML_SCHEMA_COLLECTION_READ_DOCUMENT(schema_col, 
                env, imported_schema_doc);
        if(AXIS2_ERROR_NONE != AXIS2_ERROR_GET_STATUS_CODE(env->error))
        {
            AXIS2_WODEN_SCHEMA_SET_REFERENCEABLE(schema, env, AXIS2_FALSE);
            return schema;
        }
        axis2_hash_set(reader_impl->f_imported_schemas, schema_uri, 
                AXIS2_HASH_KEY_STRING, schema_def);

    }
    
    if(NULL != schema_def) 
    {
        AXIS2_XML_SCHEMA_SET_SCHEMA_DEF(schema, env, schema_def);
    } else 
    {
        AXIS2_WODEN_SCHEMA_SET_REFERENCEABLE(schema, env, AXIS2_FALSE);
    }
    
    return schema;
}

static void *
parse_interface(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *interface_el_node,
        void *desc)
{
    void *intface = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *style_default = NULL;
    axis2_om_element_t *interface_el = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    intface = AXIS2_WODEN_DESC_ELEMENT_CREATE_INTERFACE_ELEMENT(desc, env);

    interface_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(interface_el_node, env);
    name = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(interface_el, env, WODEN_ATTR_NAME);
   
/*
 *
 *  |----------|   ----       ----   ------------|
 *  |---    ---|   |  |       |  |   |  |--------| 
 *      |  |       |  |       |  |   |  |
 *      |  |       |  |       |  |   |  |
 *      |  |       |  ---------  |   |  -----|
 *      |  |       |  ---------  |   |  -----| 
 *      |  |       |  |       |  |   |  |
 *      |  |       |  |       |  |   |  |
 *      |  |       |  |       |  |   |  |--------|
 *      ----       ----       ----   ------------|
 *
 *
 */
    
    if(NULL != name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = AXIS2_WODEN_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        intface = axis2_woden_interface_to_interface_element(intface, env);
        AXIS2_WODEN_INTERFACE_ELEMENT_SET_QNAME(intface, env, qname);
    }
    
    style_default = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(interface_el, 
            env, WODEN_ATTR_STYLE_DEFAULT);
    
    if(NULL != style_default)
    {
        axis2_array_list_t *string_list = NULL;
        axis2_char_t *uri_str = NULL;
        int i = 0, size = 0;

        string_list = axis2_tokenize(env, style_default, ' '); 
        if(string_list)
            size = AXIS2_ARRAY_LIST_SIZE(string_list, env);
        for(i = 0; i < size; i++)
        {
            uri_str = AXIS2_ARRAY_LIST_GET(string_list, env, i);
            axis2_uri_t *uri = NULL;
            intface = axis2_woden_interface_to_interface_element(intface, env);
            uri = get_uri(reader, env, uri_str);
            AXIS2_WODEN_INTERFACE_ELMENT_ADD_STYLE_DEFAULT_URI(intface, env, uri);
        }
    }
    
    /* TODO extends attribute */
  
    intface = axis2_woden_interface_to_attr_extensible(intface, env);
    parse_ext_attributes(reader, env, interface_el_node, "interface_element", 
            intface, desc);
    
    /* Parse the child elements of <interface>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <fault> <operation> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 
    temp_el = axis2_om_util_get_first_child_element(interface_el, env, interface_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_fault = NULL;
        axis2_qname_t *q_elem_op = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_fault = axis2_qname_create_from_string(env, WODEN_Q_ELEM_FAULT);
        q_elem_op = axis2_qname_create_from_string(env, WODEN_Q_ELEM_OPERATION);
        q_elem_feature = axis2_qname_create_from_string(env, WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, WODEN_Q_ELEM_PROPERTY);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            intface = axis2_woden_interface_to_documentable(intface, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(intface, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_fault, temp_el_node))
        {
            void *intface_fault = NULL;

            intface_fault = parse_interface_fault(reader, env, temp_el_node, desc, 
                    intface);
            intface = axis2_woden_interface_to_interface_element(intface, env);
            AXIS2_WODEN_INTERFACE_ELEMENT_ADD_INTERFACE_FAULT_ELEMENT(intface, env, intface_fault);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_op, temp_el_node))
        {
            void *intface_op = NULL;

            intface_op = parse_interface_op(reader, env, temp_el_node, desc, 
                    intface);
            intface = axis2_woden_interface_to_interface_element(intface, env);
            AXIS2_WODEN_INTERFACE_ELEMENT_ADD_INTERFACE_OP_ELEMENT(intface, env, intface_op);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, intface);
            intface = axis2_woden_interface_to_configurable_element(intface, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(intface, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, intface);
            intface = axis2_woden_interface_to_configurable_element(intface, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(intface, env, property);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "interface_element", 
                    intface, temp_el_node, desc);
            intface = axis2_woden_interface_to_element_extensible(intface, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(intface, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return intface;
}

/* Parse the attributes and child elements of interface <fault>. 
 * As per WSDL 2.0 spec, the child elements must be in the following order if present:
 * <documentation>
 * <feature> <property> or extension elements in any order
 * 
 * TODO validate that the elements are in correct order
 */ 
static void *
parse_interface_fault(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *fault_el_node,
        void *desc,
        void *parent)
{
    void *fault = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *element = NULL;
    axis2_om_element_t *fault_el = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;
    void *types = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    fault = AXIS2_WODEN_DESC_ELEMENT_CREATE_INTERFACE_FAULT_ELEMENT(desc, env);
    fault = axis2_woden_interface_fault_to_nested_element(fault, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(fault, env, parent); 
    types = AXIS2_WODEN_DESC_ELEMENT_GET_TYPES_ELEMENT(desc, env);
    AXIS2_WODEN_INTERFACE_FAULT_SET_TYPES(fault, env, types);

    fault_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(fault_el_node, env);
    name = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(fault_el, env, WODEN_ATTR_NAME);
    
    if(NULL != name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *ns = NULL;
        axis2_char_t *ns_str = NULL;

        ns = AXIS2_WODEN_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        if(!ns)
            ns_str = WODEN_VALUE_EMPTY_STRING;
        ns_str = AXIS2_URI_TO_STRING(ns, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, ns_str, NULL);
        fault = axis2_woden_interface_fault_to_interface_fault_element(fault, env);
        AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_SET_QNAME(fault, env, qname);
    }
    
    element = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(fault_el, env, WODEN_ATTR_ELEMENT);
    
    if(NULL != element)
    {
        axis2_qname_t *qname = NULL;
        qname = woden_om_util_get_qname(env, fault_el_node, element, desc);
        fault = axis2_woden_interface_fault_to_interface_fault_element(fault, env);
        AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_SET_QNAME(fault, env, qname);
    }
    
    /* TODO extends attribute */
   
    fault = axis2_woden_interface_fault_to_attr_extensible(fault, env);
    parse_ext_attributes(reader, env, fault_el_node, "interface_fault_element", 
            fault, desc);
    
    temp_el = axis2_om_util_get_first_child_element(fault_el, env, fault_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_PROPERTY);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            fault = axis2_woden_interface_fault_to_documentable(fault, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(fault, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, fault);
            fault = axis2_woden_interface_fault_to_configurable_element(fault, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(fault, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, fault);
            fault = axis2_woden_interface_fault_to_configurable_element(fault, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(fault, env, property);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "interface_fault_element", 
                    fault, temp_el_node, desc);
            fault = axis2_woden_interface_fault_to_element_extensible(fault, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(fault, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return fault;
}

static void *
parse_interface_op(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *op_el_node,
        void *desc,
        void *parent)
{
    void *op = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *style = NULL;
    axis2_char_t *pat = NULL;
    axis2_om_element_t *op_el = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    op = AXIS2_WODEN_DESC_ELEMENT_CREATE_INTERFACE_OP_ELEMENT(desc, env);
    op = axis2_woden_interface_op_to_nested_element(op, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(op, env, parent); 

    op_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(op_el_node, env);
    name = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(op_el, env, WODEN_ATTR_NAME);
    
    if(NULL != name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *ns = NULL;
        axis2_char_t *ns_str = NULL;

        ns = AXIS2_WODEN_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        ns_str = AXIS2_URI_TO_STRING(ns, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, ns_str, NULL);
        op = axis2_woden_interface_op_to_interface_op_element(op, env);
        AXIS2_WODEN_INTERFACE_OP_ELEMENT_SET_QNAME(op, env, qname);
    }
    
    style = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(op_el, env, WODEN_ATTR_STYLE);
    
    if(NULL != style)
    {
        axis2_array_list_t *str_list = NULL;
        axis2_char_t *uri_str = NULL;
        axis2_uri_t *uri = NULL;
        int i = 0, size = 0;
        
        size = AXIS2_ARRAY_LIST_SIZE(str_list, env);
        axis2_tokenize(env, style, ' ');
        for(i = 0; i < size; i++)
        {
            uri_str = AXIS2_ARRAY_LIST_GET(str_list, env, i);
            uri = get_uri(reader, env, uri_str);
            op = axis2_woden_interface_op_to_interface_op_element(op, env);
            AXIS2_WODEN_INTERFACE_OP_ELEMENT_ADD_STYLE_URI(op, env, uri);
        }
    }
     
    pat = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(op_el, env, WODEN_ATTR_PATTERN);
    
    if(NULL != pat)
    {
        axis2_uri_t *uri = NULL;
        
        uri = get_uri(reader, env, pat);
        op = axis2_woden_interface_op_to_interface_op_element(op, env);
        AXIS2_WODEN_INTERFACE_OP_ELEMENT_SET_PATTERN(op, env, uri);
    }
    op = axis2_woden_interface_op_to_attr_extensible(op, env);
    parse_ext_attributes(reader, env, op_el_node, "interface_op_element", 
            op, desc);
    
    /* Parse the child elements of interface <operation>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <input> <output> <infault> <outfault> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 
    temp_el = axis2_om_util_get_first_child_element(op_el, env, op_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;
        axis2_qname_t *q_elem_input = NULL;
        axis2_qname_t *q_elem_output = NULL;
        axis2_qname_t *q_elem_infault = NULL;
        axis2_qname_t *q_elem_outfault = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_PROPERTY);
        q_elem_input = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_INPUT);
        q_elem_output = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_OUTPUT);
        q_elem_infault = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_INFAULT);
        q_elem_outfault = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_OUTFAULT);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            op = axis2_woden_interface_op_to_documentable(op, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(op, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, op);
            op = axis2_woden_interface_op_to_configurable_element(op, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(op, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, op);
            op = axis2_woden_interface_op_to_configurable_element(op, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(op, env, property);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_input, temp_el_node))
        {
            void *input = NULL;

            input = parse_interface_msg_ref(reader, env, temp_el_node, desc, op);
            op = axis2_woden_interface_op_to_interface_op_element(op, env);
            AXIS2_WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_MSG_REF_ELEMENT(op, env, input);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_output, temp_el_node))
        {
            void *output = NULL;

            output = parse_interface_msg_ref(reader, env, temp_el_node, desc, op);
            op = axis2_woden_interface_op_to_interface_op_element(op, env);
            AXIS2_WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_MSG_REF_ELEMENT(op, env, output);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_infault, temp_el_node))
        {
            void *infault = NULL;

            infault = parse_interface_fault_ref(reader, env, temp_el_node, desc, op);
            op = axis2_woden_interface_op_to_interface_op_element(op, env);
            AXIS2_WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_FAULT_REF_ELEMENT(op, env, infault);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_outfault, temp_el_node))
        {
            void *outfault = NULL;

            outfault = parse_interface_fault_ref(reader, env, temp_el_node, desc, op);
            op = axis2_woden_interface_op_to_interface_op_element(op, env);
            AXIS2_WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_FAULT_REF_ELEMENT(op, env, outfault);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "interface_op_element", 
                    op, temp_el_node, desc);
            op = axis2_woden_interface_op_to_element_extensible(op, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(op, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return op;
}

static void *
parse_interface_fault_ref(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *fault_ref_el_node,
        void *desc,
        void *parent)
{
    void *fault_ref = NULL;
    axis2_char_t *ref = NULL;
    axis2_char_t *localname = NULL;
    axis2_char_t *msg_label_str = NULL;
    axis2_om_element_t *fault_ref_el = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;
    axis2_qname_t *attr_ref = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    fault_ref = AXIS2_WODEN_DESC_ELEMENT_CREATE_INTERFACE_FAULT_REF_ELEMENT(desc, 
            env);
    fault_ref = axis2_woden_interface_op_to_nested_element(fault_ref, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(fault_ref, env, parent); 

    fault_ref_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(fault_ref_el_node, env);
    localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(fault_ref_el, env);
    if(0 == AXIS2_STRCMP(WODEN_ELEM_INFAULT, localname))
    {
        axis2_woden_direction_t *direction_in = NULL;
        
        direction_in = axis2_woden_direction_get_direction_in(env);
        AXIS2_WODEN_INTERFACE_FAULT_REF_SET_DIRECTION(fault_ref, env, direction_in);
    }
    if(0 == AXIS2_STRCMP(WODEN_ELEM_OUTFAULT, localname))
    {
        axis2_woden_direction_t *direction_out = NULL;
        
        direction_out = axis2_woden_direction_get_direction_out(env);
        AXIS2_WODEN_INTERFACE_FAULT_REF_SET_DIRECTION(fault_ref, env, direction_out);
    }
    
    attr_ref = axis2_qname_create_from_string(env, WODEN_ATTR_REF);
    ref = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(fault_ref_el, env, WODEN_ATTR_REF);
    
    if(NULL != ref)
    {
        axis2_qname_t *qname = NULL;

        qname = woden_om_util_get_qname(env, fault_ref_el_node, ref, desc);
        fault_ref = 
            axis2_woden_interface_fault_ref_to_interface_fault_ref_element(
                    fault_ref, env);
        AXIS2_WODEN_INTERFACE_FAULT_REF_ELEMENT_SET_REF(fault_ref, env, qname);
    }
    
    msg_label_str = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(fault_ref_el, env, 
            WODEN_ATTR_MESSAGE_LABEL);
    
    if(NULL != msg_label_str)
    {
        axis2_woden_msg_label_t *msg_label_in = NULL;
        axis2_woden_msg_label_t *msg_label_out = NULL;
        axis2_char_t *label_in_str = NULL;
        axis2_char_t *label_out_str = NULL;

        msg_label_in = axis2_woden_msg_label_get_msg_label_in(env);
        label_in_str = AXIS2_WODEN_MSG_LABEL_TO_STRING(msg_label_in, env);
        msg_label_out = axis2_woden_msg_label_get_msg_label_out(env);
        label_out_str = AXIS2_WODEN_MSG_LABEL_TO_STRING(msg_label_out, env);
        if(0 == AXIS2_STRCMP(msg_label_str, label_in_str))
        {
            fault_ref = 
                axis2_woden_interface_fault_ref_to_interface_fault_ref_element(
                    fault_ref, env);
            AXIS2_WODEN_INTERFACE_FAULT_REF_ELEMENT_SET_MSG_LABEL(fault_ref, 
                    env, msg_label_in);
        }
        else if (0 == AXIS2_STRCMP(msg_label_str, label_out_str))
        {
            fault_ref = 
                axis2_woden_interface_fault_ref_to_interface_fault_ref_element(
                    fault_ref, env);
            AXIS2_WODEN_INTERFACE_FAULT_REF_ELEMENT_SET_MSG_LABEL(fault_ref, 
                    env, msg_label_out);
        }
        else
        {
            axis2_woden_msg_label_t *msg_label_invalid = NULL;
            
            /* Invalid value, but capture it anyway. */
            msg_label_invalid = axis2_woden_msg_label_get_invalid_value(env, msg_label_str);
            fault_ref = 
                axis2_woden_interface_fault_ref_to_interface_fault_ref_element(
                    fault_ref, env);
            AXIS2_WODEN_INTERFACE_FAULT_REF_ELEMENT_SET_MSG_LABEL(fault_ref, 
                    env, msg_label_invalid);
        }
    }
    
    fault_ref = axis2_woden_interface_fault_ref_to_attr_extensible(fault_ref, env);
    parse_ext_attributes(reader, env, fault_ref_el_node, "interface_fault_ref_element", 
            fault_ref, desc);
    
    temp_el = axis2_om_util_get_first_child_element(fault_ref_el, env, fault_ref_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_PROPERTY);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            fault_ref = axis2_woden_interface_fault_ref_to_documentable(fault_ref, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(fault_ref, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, fault_ref);
            fault_ref = axis2_woden_interface_fault_ref_to_configurable_element(fault_ref, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(fault_ref, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, fault_ref);
            fault_ref = axis2_woden_interface_fault_ref_to_configurable_element(fault_ref, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(fault_ref, env, property);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "interface_fault_ref_element", 
                    fault_ref, temp_el_node, desc);
            fault_ref = axis2_woden_interface_fault_ref_to_element_extensible(fault_ref, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(fault_ref, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return fault_ref;
}

static void *
parse_interface_msg_ref(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *msg_ref_el_node,
        void *desc,
        void *parent)
{
    void *msg_ref = NULL;
    axis2_char_t *ref = NULL;
    axis2_char_t *localname = NULL;
    axis2_char_t *msg_label_str = NULL;
    axis2_om_element_t *msg_ref_el;
    axis2_om_element_t *temp_el;
    axis2_om_node_t *temp_el_node;
    axis2_qname_t *attr_ref = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    msg_ref = AXIS2_WODEN_DESC_ELEMENT_CREATE_INTERFACE_MSG_REF_ELEMENT(desc, 
            env);
    msg_ref = axis2_woden_interface_op_to_nested_element(msg_ref, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(msg_ref, env, parent); 

    msg_ref_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(msg_ref_el_node, env);
    localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(msg_ref_el, env);
    if(0 == AXIS2_STRCMP(WODEN_ELEM_INFAULT, localname))
    {
        axis2_woden_direction_t *direction_in = NULL;
        
        direction_in = axis2_woden_direction_get_direction_in(env);
        AXIS2_WODEN_INTERFACE_MSG_REF_SET_DIRECTION(msg_ref, env, direction_in);
    }
    if(0 == AXIS2_STRCMP(WODEN_ELEM_OUTFAULT, localname))
    {
        axis2_woden_direction_t *direction_out = NULL;
        
        direction_out = axis2_woden_direction_get_direction_out(env);
        AXIS2_WODEN_INTERFACE_MSG_REF_SET_DIRECTION(msg_ref, env, direction_out);
    }
    
    attr_ref = axis2_qname_create_from_string(env, WODEN_ATTR_REF);
    ref = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(msg_ref_el, env, WODEN_ATTR_REF);
    
    if(NULL != ref)
    {
        axis2_qname_t *qname = NULL;

        qname = woden_om_util_get_qname(env, msg_ref_el_node, ref, desc);
        msg_ref = 
            axis2_woden_interface_msg_ref_to_interface_msg_ref_element(
                    msg_ref, env);
        AXIS2_WODEN_INTERFACE_MSG_REF_ELEMENT_SET_REF(msg_ref, env, qname);
    }
    
    msg_label_str = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(msg_ref_el, env, 
            WODEN_ATTR_MESSAGE_LABEL);
    
    if(NULL != msg_label_str)
    {
        axis2_woden_msg_label_t *msg_label_in = NULL;
        axis2_woden_msg_label_t *msg_label_out = NULL;
        axis2_char_t *label_in_str = NULL;
        axis2_char_t *label_out_str = NULL;

        msg_label_in = axis2_woden_msg_label_get_msg_label_in(env);
        label_in_str = AXIS2_WODEN_MSG_LABEL_TO_STRING(msg_label_in, env);
        msg_label_out = axis2_woden_msg_label_get_msg_label_out(env);
        label_out_str = AXIS2_WODEN_MSG_LABEL_TO_STRING(msg_label_out, env);
        if(0 == AXIS2_STRCMP(msg_label_str, label_in_str))
        {
            msg_ref = 
                axis2_woden_interface_msg_ref_to_interface_msg_ref_element(
                    msg_ref, env);
            AXIS2_WODEN_INTERFACE_MSG_REF_ELEMENT_SET_MSG_LABEL(msg_ref, 
                    env, msg_label_in);
        }
        else if (0 == AXIS2_STRCMP(msg_label_str, label_out_str))
        {
            msg_ref = 
                axis2_woden_interface_msg_ref_to_interface_msg_ref_element(
                    msg_ref, env);
            AXIS2_WODEN_INTERFACE_MSG_REF_ELEMENT_SET_MSG_LABEL(msg_ref, 
                    env, msg_label_out);
        }
        else
        {
            axis2_woden_msg_label_t *msg_label_invalid = NULL;
            
            /* Invalid value, but capture it anyway. */
            msg_label_invalid = axis2_woden_msg_label_get_invalid_value(env, msg_label_str);
            msg_ref = 
                axis2_woden_interface_msg_ref_to_interface_msg_ref_element(
                    msg_ref, env);
            AXIS2_WODEN_INTERFACE_MSG_REF_ELEMENT_SET_MSG_LABEL(msg_ref, 
                    env, msg_label_invalid);
        }
    }
    msg_ref = axis2_woden_interface_msg_ref_to_attr_extensible(msg_ref, env); 
    parse_ext_attributes(reader, env, msg_ref_el_node, "interface_msg_ref_element", 
            msg_ref, desc);
    
    temp_el = axis2_om_util_get_first_child_element(msg_ref_el, env, msg_ref_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_PROPERTY);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            msg_ref = axis2_woden_interface_msg_ref_to_documentable(msg_ref, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(msg_ref, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, msg_ref);
            msg_ref = axis2_woden_interface_msg_ref_to_configurable_element(msg_ref, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(msg_ref, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, msg_ref);
            msg_ref = axis2_woden_interface_msg_ref_to_configurable_element(msg_ref, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(msg_ref, env, property);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "interface_msg_ref_element", 
                    msg_ref, temp_el_node, desc);
            msg_ref = axis2_woden_interface_msg_ref_to_element_extensible(msg_ref, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(msg_ref, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return msg_ref;
}

static void *
parse_binding(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *binding_el_node,
        void *desc)
{
    void *binding = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *type = NULL;
    axis2_char_t *intface = NULL;
    axis2_om_element_t *binding_el;
    axis2_om_element_t *temp_el;
    axis2_om_node_t *temp_el_node;
    axis2_qname_t *intface_qn = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    binding = AXIS2_WODEN_DESC_ELEMENT_CREATE_BINDING_ELEMENT(desc, env);

    binding_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(binding_el_node, env);
    name = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(binding_el, env, WODEN_ATTR_NAME);
   
    if(NULL != name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = AXIS2_WODEN_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        binding = axis2_woden_binding_to_binding_element(binding, env);
        AXIS2_WODEN_BINDING_ELEMENT_SET_QNAME(binding, env, qname);
    }
 
    intface = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(binding_el, env, WODEN_ATTR_INTERFACE);
    if(NULL != intface)
    {
        intface_qn = woden_om_util_get_qname(env, binding_el_node, intface, desc);
        binding = axis2_woden_binding_to_binding_element(binding, env);
        AXIS2_WODEN_BINDING_ELEMENT_SET_INTERFACE_QNAME(binding, env, intface_qn);
    }
    /* Dereference the interface qname */
    if(NULL != intface_qn)
    {
        axis2_array_list_t *interfaces = NULL;
        int i = 0, size = 0;
        
        desc = axis2_woden_desc_to_desc_element(desc, env);
        interfaces = AXIS2_WODEN_DESC_ELEMENT_GET_INTERFACE_ELEMENTS(desc, env);
        size = AXIS2_ARRAY_LIST_SIZE(interfaces, env);
        for(i = 0; i < size; i++)
        {
            void *intface = NULL;
            axis2_qname_t *qname = NULL;

            intface = AXIS2_ARRAY_LIST_GET(interfaces, env, i);
            intface = axis2_woden_interface_to_interface_element(intface, env);
            qname = AXIS2_WODEN_INTERFACE_ELEMENT_GET_QNAME(intface, env);
            if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(intface_qn, env, qname))
            {
                AXIS2_WODEN_BINDING_SET_INTERFACE_ELEMENT(binding, env, intface);
            }
        }
    }
    
    type = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(binding_el, env, WODEN_ATTR_TYPE);
    
    if(NULL != type)
    {
        axis2_uri_t *uri = NULL;

        uri = get_uri(reader, env, type);
        binding = axis2_woden_binding_to_binding_element(binding, env);
        AXIS2_WODEN_BINDING_ELEMENT_SET_TYPE(binding, env, uri);
    }
    
    /* TODO extends attribute */
  
    binding = axis2_woden_binding_to_attr_extensible(binding, env);
    parse_ext_attributes(reader, env, binding_el_node, "binding_element", 
            binding, desc);
    
    /* Parse the child elements of <binding>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <fault> <operation> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 
    temp_el = axis2_om_util_get_first_child_element(binding_el, env, binding_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_fault = NULL;
        axis2_qname_t *q_elem_op = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_fault = axis2_qname_create_from_string(env, WODEN_Q_ELEM_FAULT);
        q_elem_op = axis2_qname_create_from_string(env, WODEN_Q_ELEM_OPERATION);
        q_elem_feature = axis2_qname_create_from_string(env, WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, WODEN_Q_ELEM_PROPERTY);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            binding = axis2_woden_binding_to_documentable(binding, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(binding, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_fault, temp_el_node))
        {
            void *binding_fault = NULL;
            binding_fault = parse_binding_fault(reader, env, temp_el_node, desc, 
                    binding);
            binding = axis2_woden_binding_to_binding_element(binding, env);
            AXIS2_WODEN_BINDING_ELEMENT_ADD_BINDING_FAULT_ELEMENT(binding, env, binding_fault);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_op, temp_el_node))
        {
            void *binding_op = NULL;

            binding_op = parse_binding_op(reader, env, temp_el_node, desc, 
                    binding);
            binding = axis2_woden_binding_to_binding_element(binding, env);
            AXIS2_WODEN_BINDING_ELEMENT_ADD_BINDING_OP_ELEMENT(binding, env, binding_op);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, binding);
            binding = axis2_woden_binding_to_configurable_element(binding, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(binding, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, binding);
            binding = axis2_woden_binding_to_configurable_element(binding, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(binding, env, property);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "binding_element", 
                    binding, temp_el_node, desc);
            binding = axis2_woden_binding_to_element_extensible(binding, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(binding, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return binding;
}

static void *
parse_binding_fault(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *fault_el_node,
        void *desc,
        void *parent)
{
    void *fault = NULL;
    axis2_char_t *ref = NULL;
    axis2_om_element_t *fault_el;
    axis2_om_element_t *temp_el;
    axis2_om_node_t *temp_el_node;
    axis2_qname_t *attr_ref = NULL;
    axis2_qname_t *int_flt_qn = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    fault = AXIS2_WODEN_DESC_ELEMENT_CREATE_BINDING_FAULT_ELEMENT(desc, env);
    fault = axis2_woden_binding_fault_to_nested_element(fault, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(fault, env, parent); 

    attr_ref = axis2_qname_create_from_string(env, WODEN_ATTR_NAME);
    fault_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(fault_el_node, env);
    ref = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(fault_el, env, WODEN_ATTR_REF);
    
    if(NULL != ref)
    {
        int_flt_qn = woden_om_util_get_qname(env, fault_el_node, ref, desc);
        fault = axis2_woden_binding_fault_to_binding_fault_element(fault, env);
        AXIS2_WODEN_BINDING_FAULT_ELEMENT_SET_REF(fault, env, int_flt_qn);
    }
    /* Dereference the interface fault qname */
    if(NULL != int_flt_qn)
    {
        void *intface = NULL;

        /* Here I know that parent is an instance of binding */
        parent = axis2_woden_binding_to_binding_element(parent, env);
        intface = AXIS2_WODEN_BINDING_ELEMENT_GET_INTERFACE_ELEMENT(parent, env);
        if(NULL != intface)
        {
            axis2_array_list_t *int_faults = NULL;
            int i = 0, size = 0;

            int_faults = 
                AXIS2_WODEN_INTERFACE_ELEMENT_GET_INTERFACE_FAULT_ELEMENTS(
                        intface, env);
            size = AXIS2_ARRAY_LIST_SIZE(int_faults, env);
            for(i = 0; i < size; i++)
            {
                void *intface_flt = NULL;
                axis2_qname_t *qname = NULL;
               
                intface_flt = AXIS2_ARRAY_LIST_GET(int_faults, env, i);
                if(intface_flt)
                    qname = AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_GET_QNAME(
                        intface_flt, env);
                if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(int_flt_qn, env, qname))
                {
                    AXIS2_WODEN_BINDING_FAULT_SET_INTERFACE_FAULT_ELEMENT(fault, 
                            env, intface_flt);
                }

            }
        }
    }
    fault = axis2_woden_binding_fault_to_attr_extensible(fault, env);
    parse_ext_attributes(reader, env, fault_el_node, "binding_fault_element", 
            fault, desc);
    
    temp_el = axis2_om_util_get_first_child_element(fault_el, env, fault_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_PROPERTY);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            fault = axis2_woden_binding_fault_to_documentable(fault, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(fault, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, fault);
            fault = axis2_woden_binding_fault_to_configurable_element(fault, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(fault, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, fault);
            fault = axis2_woden_binding_fault_to_configurable_element(fault, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(fault, env, property);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "binding_fault_element", 
                    fault, temp_el_node, desc);
            fault = axis2_woden_binding_fault_to_element_extensible(fault, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(fault, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return fault;
}

static void *
parse_binding_op(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *op_el_node,
        void *desc,
        void *parent)
{
    void *op = NULL;
    axis2_char_t *ref = NULL;
    axis2_om_element_t *op_el;
    axis2_om_element_t *temp_el;
    axis2_om_node_t *temp_el_node;
    axis2_qname_t *attr_ref = NULL;
    axis2_qname_t *ref_qn = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    op = AXIS2_WODEN_DESC_ELEMENT_CREATE_BINDING_OP_ELEMENT(desc, env);
    op = axis2_woden_binding_op_to_nested_element(op, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(op, env, parent); 

    attr_ref = axis2_qname_create_from_string(env, WODEN_ATTR_REF);
    op_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(op_el_node, env);
    ref = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(op_el, env, WODEN_ATTR_REF);
    
    if(NULL != ref)
    {
        ref_qn = woden_om_util_get_qname(env, op_el_node, ref, desc);
        op = axis2_woden_binding_op_to_binding_op_element(op, env);
        AXIS2_WODEN_BINDING_OP_ELEMENT_SET_REF(op, env, ref_qn);
    }
    /* Dereference the 'ref' qname to an interface_op_element */
    if(NULL != ref_qn)
    {
        void *intface = NULL;
        axis2_array_list_t *int_ops = NULL;
        int i = 0, size = 0;

        /* Here I know that parent is an instance of binding */
        parent = axis2_woden_binding_to_binding_element(parent, env);
        intface = AXIS2_WODEN_BINDING_ELEMENT_GET_INTERFACE_ELEMENT(parent, env);
        intface = axis2_woden_interface_to_interface_element(intface, env);
        int_ops = AXIS2_WODEN_INTERFACE_ELEMENT_GET_INTERFACE_OP_ELEMENTS(intface, env);
        size = AXIS2_ARRAY_LIST_SIZE(int_ops, env);
        for(i = 0; i < size; i++)
        {
            void *intface_op = NULL;
            axis2_qname_t *qname = NULL;
           
            intface_op = AXIS2_ARRAY_LIST_GET(int_ops, env, i);
            if(intface_op)
                qname = AXIS2_WODEN_INTERFACE_OP_ELEMENT_GET_QNAME(
                    intface_op, env);
            if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(ref_qn, env, qname))
            {
                AXIS2_WODEN_BINDING_OP_SET_INTERFACE_OP_ELEMENT(op, 
                        env, intface_op);
            }
        }
    }

    op = axis2_woden_binding_op_to_attr_extensible(op, env);
    parse_ext_attributes(reader, env, op_el_node, "binding_op_element", 
            op, desc);
    
    /* Parse the child elements of binding <operation>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <input> <output> <infault> <outfault> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 
    temp_el = axis2_om_util_get_first_child_element(op_el, env, op_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;
        axis2_qname_t *q_elem_input = NULL;
        axis2_qname_t *q_elem_output = NULL;
        axis2_qname_t *q_elem_infault = NULL;
        axis2_qname_t *q_elem_outfault = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_PROPERTY);
        q_elem_input = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_INPUT);
        q_elem_output = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_OUTPUT);
        q_elem_infault = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_INFAULT);
        q_elem_outfault = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_OUTFAULT);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            op = axis2_woden_binding_op_to_documentable(op, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(op, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, op);
            op = axis2_woden_binding_op_to_configurable_element(op, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(op, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, op);
            op = axis2_woden_binding_op_to_configurable_element(op, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(op, env, property);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_input, temp_el_node))
        {
            void *input = NULL;

            input = parse_binding_msg_ref(reader, env, temp_el_node, desc, op);
            op = axis2_woden_binding_op_to_binding_op_element(op, env);
            AXIS2_WODEN_BINDING_OP_ELEMENT_ADD_BINDING_MSG_REF_ELEMENT(op, env, input);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_output, temp_el_node))
        {
            void *output = NULL;

            output = parse_binding_msg_ref(reader, env, temp_el_node, desc, op);
            op = axis2_woden_binding_op_to_binding_op_element(op, env);
            AXIS2_WODEN_BINDING_OP_ELEMENT_ADD_BINDING_MSG_REF_ELEMENT(op, env, output);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_infault, temp_el_node))
        {
            void *infault = NULL;

            infault = parse_binding_fault_ref(reader, env, temp_el_node, desc, op);
            op = axis2_woden_binding_op_to_binding_op_element(op, env);
            AXIS2_WODEN_BINDING_OP_ELEMENT_ADD_BINDING_FAULT_REF_ELEMENT(op, env, infault);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_outfault, temp_el_node))
        {
            void *outfault = NULL;

            outfault = parse_binding_fault_ref(reader, env, temp_el_node, desc, op);
            op = axis2_woden_binding_op_to_binding_op_element(op, env);
            AXIS2_WODEN_BINDING_OP_ELEMENT_ADD_BINDING_FAULT_REF_ELEMENT(op, env, outfault);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "binding_op_element", 
                    op, temp_el_node, desc);
            op = axis2_woden_binding_op_to_element_extensible(op, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(op, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return op;
}

static void *
parse_binding_fault_ref(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *fault_ref_el_node,
        void *desc,
        void *parent)
{
    void *fault_ref = NULL;
    axis2_char_t *ref = NULL;
    axis2_char_t *msg_label_str = NULL;
    axis2_om_element_t *fault_ref_el;
    axis2_om_element_t *temp_el;
    axis2_om_node_t *temp_el_node;
    axis2_qname_t *attr_ref = NULL;
    axis2_qname_t *ref_qn = NULL;
    axis2_qname_t *qref = NULL;
    axis2_woden_msg_label_t *msg_label = NULL;
    axis2_woden_msg_label_t *msg_label_in = NULL;
    axis2_woden_msg_label_t *msg_label_out = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    fault_ref = AXIS2_WODEN_DESC_ELEMENT_CREATE_BINDING_FAULT_REF_ELEMENT(desc, env);
    fault_ref = axis2_woden_binding_op_to_nested_element(fault_ref, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(fault_ref, env, parent); 

    attr_ref = axis2_qname_create_from_string(env, WODEN_ATTR_REF);
    ref = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(fault_ref_el, env, WODEN_ATTR_REF);
    
    if(NULL != ref)
    {
        ref_qn = woden_om_util_get_qname(env, fault_ref_el_node, ref, desc);
        fault_ref = axis2_woden_binding_fault_ref_to_binding_fault_ref_element(
                fault_ref, env);
        AXIS2_WODEN_BINDING_FAULT_REF_ELEMENT_SET_REF(fault_ref, env, ref_qn);
    }
    
    msg_label_str = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(fault_ref_el, env, 
            WODEN_ATTR_MESSAGE_LABEL);
    
    if(NULL != msg_label_str)
    {
        axis2_woden_msg_label_t *msg_label_in = NULL;
        axis2_woden_msg_label_t *msg_label_out = NULL;
        axis2_char_t *label_in_str = NULL;
        axis2_char_t *label_out_str = NULL;

        msg_label_in = axis2_woden_msg_label_get_msg_label_in(env);
        label_in_str = AXIS2_WODEN_MSG_LABEL_TO_STRING(msg_label_in, env);
        msg_label_out = axis2_woden_msg_label_get_msg_label_out(env);
        label_out_str = AXIS2_WODEN_MSG_LABEL_TO_STRING(msg_label_out, env);
        if(0 == AXIS2_STRCMP(msg_label_str, label_in_str))
        {
            fault_ref = 
                axis2_woden_binding_fault_ref_to_binding_fault_ref_element(
                    fault_ref, env);
            AXIS2_WODEN_BINDING_FAULT_REF_ELEMENT_SET_MSG_LABEL(fault_ref, 
                    env, msg_label_in);
        }
        else if (0 == AXIS2_STRCMP(msg_label_str, label_out_str))
        {
            fault_ref = 
                axis2_woden_binding_fault_ref_to_binding_fault_ref_element(
                    fault_ref, env);
            AXIS2_WODEN_BINDING_FAULT_REF_ELEMENT_SET_MSG_LABEL(fault_ref, 
                    env, msg_label_out);
        }
        else
        {
            axis2_woden_msg_label_t *msg_label_invalid = NULL;
            
            /* Invalid value, but capture it anyway. */
            msg_label_invalid = axis2_woden_msg_label_get_invalid_value(env, msg_label_str);
            fault_ref = 
                axis2_woden_binding_fault_ref_to_binding_fault_ref_element(
                    fault_ref, env);
            AXIS2_WODEN_BINDING_FAULT_REF_ELEMENT_SET_MSG_LABEL(fault_ref, 
                    env, msg_label_invalid);
        }
    }
    /* Dereference the 'ref' and 'messageLabel' attributes to an interface fault 
     * reference.
     */
    fault_ref = axis2_woden_binding_fault_ref_to_binding_fault_ref_element(
            fault_ref, env);
    qref = AXIS2_WODEN_BINDING_FAULT_REF_ELEMENT_GET_REF(fault_ref, env);
    msg_label = AXIS2_WODEN_BINDING_FAULT_REF_ELEMENT_GET_MSG_LABEL(fault_ref, env);
    msg_label_in = axis2_woden_msg_label_get_msg_label_in(env);
    msg_label_out = axis2_woden_msg_label_get_msg_label_out(env);

    if(NULL !=qref &&
            ( msg_label == msg_label_in ||
              msg_label == msg_label_out ))
    {
        void *bind_op = parent;
        void *int_op = NULL;
        
        bind_op = axis2_woden_binding_op_to_binding_op_element(bind_op, env);
        int_op = AXIS2_WODEN_BINDING_OP_ELEMENT_GET_INTERFACE_OP_ELEMENT(
                bind_op, env);
        if(NULL != int_op)
        {
            axis2_array_list_t *int_flt_refs = NULL;
            int i = 0, size = 0;
            
            int_op = axis2_woden_interface_op_to_interface_op_element(int_op, env);
            int_flt_refs = 
                AXIS2_WODEN_INTERFACE_OP_ELEMENT_GET_INTERFACE_FAULT_REF_ELEMENTS(
                        int_op, env);
            size = AXIS2_ARRAY_LIST_SIZE(int_flt_refs, env);
            for( i = 0; i < size; i++)
            {
                void *int_flt_ref = NULL;
                axis2_qname_t *qname = NULL;
                axis2_woden_msg_label_t *label = NULL;

                int_flt_ref = AXIS2_ARRAY_LIST_GET(int_flt_refs, env, i);
                int_flt_ref = 
                    axis2_woden_interface_fault_ref_to_interface_fault_ref_element(
                            int_flt_ref, env);
                qname = AXIS2_WODEN_INTERFACE_FAULT_REF_ELEMENT_GET_REF(
                        int_flt_ref, env);
                label = AXIS2_WODEN_INTERFACE_FAULT_REF_ELEMENT_GET_MSG_LABEL(
                        int_flt_ref, env);
                if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(qref, env, qname) &&
                        AXIS2_TRUE == AXIS2_WODEN_MSG_LABEL_EQUALS(msg_label, env, label))
                {
                    AXIS2_WODEN_BINDING_FAULT_REF_SET_INTERFACE_FAULT_REF_ELEMENT(
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
 
    fault_ref = axis2_woden_binding_fault_ref_to_attr_extensible(fault_ref, env);
    parse_ext_attributes(reader, env, fault_ref_el_node, "binding_fault_ref_element", 
            fault_ref, desc);
    
    temp_el = axis2_om_util_get_first_child_element(fault_ref_el, env, fault_ref_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_PROPERTY);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            fault_ref = axis2_woden_binding_fault_ref_to_documentable(fault_ref, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(fault_ref, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, fault_ref);
            fault_ref = axis2_woden_binding_fault_ref_to_configurable_element(fault_ref, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(fault_ref, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, fault_ref);
            fault_ref = axis2_woden_binding_fault_ref_to_configurable_element(fault_ref, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(fault_ref, env, property);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "binding_fault_ref_element", 
                    fault_ref, temp_el_node, desc);
            fault_ref = axis2_woden_binding_fault_ref_to_element_extensible(fault_ref, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(fault_ref, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return fault_ref;
}

static void *
parse_binding_msg_ref(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *msg_ref_el_node,
        void *desc,
        void *parent)
{
    void *msg_ref = NULL;
    axis2_char_t *msg_label_str = NULL;
    axis2_om_element_t *msg_ref_el = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;
    axis2_char_t *localname = NULL;
    axis2_woden_msg_label_t *msg_label = NULL;
    axis2_woden_msg_label_t *msg_label_in = NULL;
    axis2_woden_msg_label_t *msg_label_out = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    msg_ref = AXIS2_WODEN_DESC_ELEMENT_CREATE_BINDING_MSG_REF_ELEMENT(desc, env);
    msg_ref = axis2_woden_binding_op_to_nested_element(msg_ref, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(msg_ref, env, parent); 

    msg_ref_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(msg_ref_el_node, env);
    localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(msg_ref_el, env);
    if(0 == AXIS2_STRCMP(WODEN_ELEM_INPUT, localname))
    {
        axis2_woden_direction_t *direction_in = NULL;
        
        direction_in = axis2_woden_direction_get_direction_in(env);
        AXIS2_WODEN_BINDING_MSG_REF_SET_DIRECTION(msg_ref, env, direction_in);
    }
    if(0 == AXIS2_STRCMP(WODEN_ELEM_OUTPUT, localname))
    {
        axis2_woden_direction_t *direction_out = NULL;
        
        direction_out = axis2_woden_direction_get_direction_out(env);
        AXIS2_WODEN_BINDING_MSG_REF_SET_DIRECTION(msg_ref, env, direction_out);
    }
    
    msg_label_str = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(msg_ref_el, env, 
            WODEN_ATTR_MESSAGE_LABEL);
    
    if(NULL != msg_label_str)
    {
        axis2_woden_msg_label_t *msg_label_in = NULL;
        axis2_woden_msg_label_t *msg_label_out = NULL;
        axis2_char_t *label_in_str = NULL;
        axis2_char_t *label_out_str = NULL;

        msg_label_in = axis2_woden_msg_label_get_msg_label_in(env);
        label_in_str = AXIS2_WODEN_MSG_LABEL_TO_STRING(msg_label_in, env);
        msg_label_out = axis2_woden_msg_label_get_msg_label_out(env);
        label_out_str = AXIS2_WODEN_MSG_LABEL_TO_STRING(msg_label_out, env);
        if(0 == AXIS2_STRCMP(msg_label_str, label_in_str))
        {
            msg_ref = 
                axis2_woden_binding_msg_ref_to_binding_msg_ref_element(
                    msg_ref, env);
            AXIS2_WODEN_BINDING_MSG_REF_ELEMENT_SET_MSG_LABEL(msg_ref, 
                    env, msg_label_in);
        }
        else if (0 == AXIS2_STRCMP(msg_label_str, label_out_str))
        {
            msg_ref = 
                axis2_woden_binding_msg_ref_to_binding_msg_ref_element(
                    msg_ref, env);
            AXIS2_WODEN_BINDING_MSG_REF_ELEMENT_SET_MSG_LABEL(msg_ref, 
                    env, msg_label_out);
        }
        else
        {
            axis2_woden_msg_label_t *msg_label_invalid = NULL;
            
            /* Invalid value, but capture it anyway. */
            msg_label_invalid = axis2_woden_msg_label_get_invalid_value(env, msg_label_str);
            msg_ref = 
                axis2_woden_binding_msg_ref_to_binding_msg_ref_element(
                    msg_ref, env);
            AXIS2_WODEN_BINDING_MSG_REF_ELEMENT_SET_MSG_LABEL(msg_ref, 
                    env, msg_label_invalid);
        }
    }
    else
    {
        axis2_woden_msg_label_t *msg_label_in = NULL;
        axis2_woden_msg_label_t *msg_label_out = NULL;
        axis2_char_t *label_in_str = NULL;
        axis2_char_t *label_out_str = NULL;
        axis2_woden_direction_t *direction = NULL;
        axis2_woden_direction_t *direction_in = NULL;
        axis2_woden_direction_t *direction_out = NULL;
        
        /* TODO This is a temp fix, correct action is to use MEP to determine 
         * default
         */

        msg_label_in = axis2_woden_msg_label_get_msg_label_in(env);
        label_in_str = AXIS2_WODEN_MSG_LABEL_TO_STRING(msg_label_in, env);
        msg_label_out = axis2_woden_msg_label_get_msg_label_out(env);
        label_out_str = AXIS2_WODEN_MSG_LABEL_TO_STRING(msg_label_out, env);
        direction_in = axis2_woden_direction_get_direction_in(env); 
        direction_out = axis2_woden_direction_get_direction_out(env); 
        if(AXIS2_TRUE == AXIS2_WODEN_DIRECTION_EQUALS(direction, direction_in))
        {
            msg_ref = 
                axis2_woden_binding_msg_ref_to_binding_msg_ref_element(
                    msg_ref, env);
            AXIS2_WODEN_BINDING_MSG_REF_ELEMENT_SET_MSG_LABEL(msg_ref, 
                    env, msg_label_in);
        }
        else
        {
            msg_ref = 
                axis2_woden_binding_msg_ref_to_binding_msg_ref_element(
                    msg_ref, env);
            AXIS2_WODEN_BINDING_MSG_REF_ELEMENT_SET_MSG_LABEL(msg_ref, 
                    env, msg_label_out);
        }
    }

    /* Dereference the msgLabel to an interface_msg_ref_element */
    msg_ref = axis2_woden_binding_msg_ref_to_binding_msg_ref_element(msg_ref, 
            env);
    msg_label = AXIS2_WODEN_BINDING_MSG_REF_ELEMENT_GET_MSG_LABEL(msg_ref, env);
    msg_label_in = axis2_woden_msg_label_get_msg_label_in(env);
    msg_label_out = axis2_woden_msg_label_get_msg_label_out(env);

    if ( msg_label == msg_label_in ||
         msg_label == msg_label_out )
    {
        void *bind_op = parent;
        void *int_op = NULL;
        
        bind_op = axis2_woden_binding_op_to_binding_op_element(bind_op, env);
        int_op = AXIS2_WODEN_BINDING_OP_ELEMENT_GET_INTERFACE_OP_ELEMENT(
                bind_op, env);
        if(NULL != int_op)
        {
            axis2_array_list_t *int_msg_refs = NULL;
            int i = 0, size = 0;
            
            int_op = axis2_woden_interface_op_to_interface_op_element(int_op, env);
            int_msg_refs = 
                AXIS2_WODEN_INTERFACE_OP_ELEMENT_GET_INTERFACE_MSG_REF_ELEMENTS(
                        int_op, env);
            size = AXIS2_ARRAY_LIST_SIZE(int_msg_refs, env);
            for( i = 0; i < size; i++)
            {
                void *int_msg_ref = NULL;
                axis2_woden_msg_label_t *msg_label = NULL;
                axis2_woden_msg_label_t *label = NULL;

                int_msg_ref = AXIS2_ARRAY_LIST_GET(int_msg_refs, env, i);
                int_msg_ref = 
                    axis2_woden_interface_msg_ref_to_interface_msg_ref_element(
                            int_msg_ref, env);
                label = AXIS2_WODEN_INTERFACE_MSG_REF_ELEMENT_GET_MSG_LABEL(
                        int_msg_ref, env);
                msg_ref = axis2_woden_binding_msg_ref_to_binding_msg_ref_element(msg_ref, env);
                msg_label = AXIS2_WODEN_BINDING_MSG_REF_ELEMENT_GET_MSG_LABEL(msg_ref, env);
                if(AXIS2_TRUE == AXIS2_WODEN_MSG_LABEL_EQUALS(msg_label, env, label))
                {
                    AXIS2_WODEN_BINDING_MSG_REF_SET_INTERFACE_MSG_REF_ELEMENT(
                            msg_ref, env, int_msg_ref);
                }
            }
        }
    }
    
    msg_ref = axis2_woden_binding_msg_ref_to_attr_extensible(msg_ref, env);
    parse_ext_attributes(reader, env, msg_ref_el_node, "binding_msg_ref_element", 
            msg_ref, desc);

    /* Parse the child elements of binding operation <input> or <output>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 
    temp_el = axis2_om_util_get_first_child_element(msg_ref_el, env, msg_ref_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_PROPERTY);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            msg_ref = axis2_woden_binding_msg_ref_to_documentable(msg_ref, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(msg_ref, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, msg_ref);
            msg_ref = axis2_woden_binding_msg_ref_to_configurable_element(msg_ref, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(msg_ref, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, msg_ref);
            msg_ref = axis2_woden_binding_msg_ref_to_configurable_element(msg_ref, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(msg_ref, env, property);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "binding_msg_ref_element", 
                    msg_ref, temp_el_node, desc);
            msg_ref = axis2_woden_binding_msg_ref_to_element_extensible(msg_ref, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(msg_ref, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return msg_ref;
}

static void *
parse_svc(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *svc_el_node,
        void *desc)

{
    void *svc = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *intface = NULL;
    axis2_qname_t *intface_qn = NULL;
    axis2_om_element_t *svc_el = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;
    
    svc_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(svc_el_node, env);
    desc = axis2_woden_desc_to_desc_element(desc, env);
    svc = AXIS2_WODEN_DESC_ELEMENT_CREATE_SVC_ELEMENT(desc, env);

    svc_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(svc_el_node, env);
    name = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(svc_el, env, WODEN_ATTR_NAME);
   
    if(NULL != name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = AXIS2_WODEN_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        svc = axis2_woden_svc_to_svc_element(svc, env);
        AXIS2_WODEN_SVC_ELEMENT_SET_QNAME(svc, env, qname);
    }
 
    intface = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(svc_el, env, WODEN_ATTR_INTERFACE);
    if(NULL != intface)
    {
        intface_qn = woden_om_util_get_qname(env, svc_el_node, intface, desc);
        svc = axis2_woden_svc_to_svc_element(svc, env);
        AXIS2_WODEN_SVC_ELEMENT_SET_INTERFACE_QNAME(svc, env, intface_qn);
    }
    /* Dereference the interface qname */
    if(NULL != intface_qn)
    {
        axis2_array_list_t *interfaces = NULL;
        int i = 0, size = 0;
        
        desc = axis2_woden_desc_to_desc_element(desc, env);
        interfaces = AXIS2_WODEN_DESC_ELEMENT_GET_INTERFACE_ELEMENTS(desc, env);
        size = AXIS2_ARRAY_LIST_SIZE(interfaces, env);
        for(i = 0; i < size; i++)
        {
            void *intface = NULL;
            axis2_qname_t *qname = NULL;

            intface = AXIS2_ARRAY_LIST_GET(interfaces, env, i);
            intface = axis2_woden_interface_to_interface_element(intface, env);
            qname = AXIS2_WODEN_INTERFACE_ELEMENT_GET_QNAME(intface, env);
            if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(intface_qn, env, qname))
            {
                AXIS2_WODEN_SVC_SET_INTERFACE_ELEMENT(svc, env, intface);
            }
        }
    }
    
    /* TODO extends attribute */
    svc = axis2_woden_svc_to_attr_extensible(svc, env); 
    parse_ext_attributes(reader, env, svc_el_node, "svc_element", 
            svc, desc);
    
    /* Parse the child elements of <service>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <endpoint>
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 
    temp_el = axis2_om_util_get_first_child_element(svc_el, env, svc_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_endpoint = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_endpoint = axis2_qname_create_from_string(env, WODEN_Q_ELEM_ENDPOINT);
        q_elem_feature = axis2_qname_create_from_string(env, WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, WODEN_Q_ELEM_PROPERTY);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            svc = axis2_woden_svc_to_documentable(svc, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(svc, env, documentation);
        }
        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_endpoint, temp_el_node))
        {
            void *endpoint = NULL;

            endpoint = parse_endpoint(reader, env, temp_el_node, desc, svc);
            svc = axis2_woden_svc_to_svc_element(svc, env);
            AXIS2_WODEN_SVC_ELEMENT_ADD_ENDPOINT_ELEMENT(svc, env, endpoint);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, svc);
            svc = axis2_woden_svc_to_configurable_element(svc, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(svc, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, svc);
            svc = axis2_woden_svc_to_configurable_element(svc, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(svc, env, property);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "svc_element", 
                    svc, temp_el_node, desc);
            svc = axis2_woden_svc_to_element_extensible(svc, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(svc, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return svc;
}

static void *
parse_endpoint(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *endpoint_el_node,
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
    axis2_uri_t *address_uri = NULL;
    axis2_om_element_t *endpoint_el = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;
    
    desc = axis2_woden_desc_to_desc_element(desc, env);
    endpoint = AXIS2_WODEN_DESC_ELEMENT_CREATE_ENDPOINT_ELEMENT(desc, env);
    endpoint = axis2_woden_endpoint_to_nested_element(endpoint, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(endpoint, env, parent); 


    endpoint_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(endpoint_el_node, env);
    name = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(endpoint_el, env, WODEN_ATTR_NAME);
   
    if(NULL != name)
    {
        woden_nc_name_t *nc_name = woden_nc_name_create(env, name);
        endpoint = axis2_woden_endpoint_to_endpoint_element(endpoint, env);
        AXIS2_WODEN_ENDPOINT_ELEMENT_SET_NAME(endpoint, env, nc_name);
    }
 
    attr_binding = axis2_qname_create_from_string(env, WODEN_ATTR_BINDING);
    binding = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(endpoint_el, env, WODEN_ATTR_BINDING);
    if(NULL != binding)
    {
        binding_qn = woden_om_util_get_qname(env, endpoint_el_node, binding, desc);
        endpoint = axis2_woden_endpoint_to_endpoint_element(endpoint, env);
        AXIS2_WODEN_ENDPOINT_ELEMENT_SET_BINDING_QNAME(endpoint, env, binding_qn);
    }
    /* Dereference the binding qname */
    if(NULL != binding_qn)
    {
        axis2_array_list_t *bindings = NULL;
        int i = 0, size = 0;
        
        desc = axis2_woden_desc_to_desc_element(desc, env);
        bindings = AXIS2_WODEN_DESC_ELEMENT_GET_BINDING_ELEMENTS(desc, env);
        size = AXIS2_ARRAY_LIST_SIZE(bindings, env);
        for(i = 0; i < size; i++)
        {
            void *binding = NULL;
            axis2_qname_t *qname = NULL;

            binding = AXIS2_ARRAY_LIST_GET(bindings, env, i);
            binding = axis2_woden_binding_to_binding_element(binding, env);
            qname = AXIS2_WODEN_BINDING_ELEMENT_GET_QNAME(binding, env);
            if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(binding_qn, env, qname))
            {
                AXIS2_WODEN_ENDPOINT_SET_BINDING_ELEMENT(endpoint, env, binding);
            }
        }
    }
    
    attr_address = axis2_qname_create_from_string(env, WODEN_ATTR_ADDRESS);
    endpoint_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(endpoint_el_node, env);
    address = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(endpoint_el, env, WODEN_ATTR_ADDRESS);
   
    if(NULL != address)
    {
        address_uri = get_uri(reader, env, address);
        endpoint = axis2_woden_endpoint_to_endpoint_element(endpoint, env);
        AXIS2_WODEN_ENDPOINT_ELEMENT_SET_ADDRESS(endpoint, env, address_uri);
    }
    
    endpoint = axis2_woden_endpoint_to_attr_extensible(endpoint, env); 
    parse_ext_attributes(reader, env, endpoint_el_node, "endpoint_element", 
            endpoint, desc);
    
    /* Parse the child elements of <endpoint>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 
    temp_el = axis2_om_util_get_first_child_element(endpoint_el, env, endpoint_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_feature = NULL;
        axis2_qname_t *q_elem_property = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_feature = axis2_qname_create_from_string(env, WODEN_Q_ELEM_FEATURE);
        q_elem_property = axis2_qname_create_from_string(env, WODEN_Q_ELEM_PROPERTY);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            endpoint = axis2_woden_endpoint_to_documentable(endpoint, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(endpoint, env, documentation);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, endpoint);
            endpoint = axis2_woden_endpoint_to_configurable_element(endpoint, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(endpoint, env, feature);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, endpoint);
            endpoint = axis2_woden_endpoint_to_configurable_element(endpoint, env);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(endpoint, env, property);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "endpoint_element", 
                    endpoint, temp_el_node, desc);
            endpoint = axis2_woden_endpoint_to_element_extensible(endpoint, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(endpoint, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return endpoint;
}

static void *
parse_feature(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *feature_el_node,
        void *desc,
        void *parent)

{
    void *feature = NULL;
    axis2_char_t *ref = NULL;
    axis2_char_t *req = NULL;
    axis2_qname_t *attr_ref = NULL;
    axis2_qname_t *attr_req = NULL;
    axis2_om_element_t *feature_el = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;
    axis2_bool_t required = AXIS2_FALSE;
    
    desc = axis2_woden_desc_to_desc_element(desc, env);
    feature = AXIS2_WODEN_DESC_ELEMENT_CREATE_FEATURE_ELEMENT(desc, env);
    feature = axis2_woden_feature_to_nested_element(feature, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(feature, env, parent); 


    attr_ref = axis2_qname_create_from_string(env, WODEN_ATTR_REF);
    feature_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(feature_el_node, env);
    ref = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(feature_el, env, WODEN_ATTR_REF);
   
    if(NULL != ref)
    {
        axis2_uri_t *ref_uri = get_uri(reader, env, ref);
        feature = axis2_woden_feature_to_feature_element(feature, env);
        AXIS2_WODEN_FEATURE_ELEMENT_SET_REF(feature, env, ref_uri);
    }
 
    attr_req = axis2_qname_create_from_string(env, WODEN_ATTR_REQUIRED);
    
    req = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(feature_el, env, WODEN_ATTR_REQUIRED);
    if(NULL != req && 0 == AXIS2_STRCMP(WODEN_VALUE_TRUE, req))
            required = AXIS2_TRUE;
    else
            required = AXIS2_FALSE;
    feature = axis2_woden_feature_to_feature_element(feature, env);
    AXIS2_WODEN_ENDPOINT_ELEMENT_SET_REQUIRED(feature, env, required);
    /*TODO t.b.c. what if attr value is not 'true' or 'false'? (eg, missing, 
     * mispelt or not lower case.
     */
    
    feature = axis2_woden_feature_to_attr_extensible(feature, env); 
    parse_ext_attributes(reader, env, feature_el_node, "feature_element", 
            feature, desc);
    
    /* Parse the child elements of <feature>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * extension elements.
     * TODO validate that the elements are in correct order
     */ 
    temp_el = axis2_om_util_get_first_child_element(feature_el, env, feature_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, WODEN_Q_ELEM_DOCUMENTATION);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            feature = axis2_woden_feature_to_documentable(feature, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(feature, env, documentation);
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "feature_element", 
                    feature, temp_el_node, desc);
            feature = axis2_woden_feature_to_element_extensible(feature, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(feature, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
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
        axis2_om_node_t *property_el_node,
        void *desc,
        void *parent)

{
    void *property = NULL;
    axis2_char_t *ref = NULL;
    axis2_qname_t *attr_ref = NULL;
    void *types = NULL;
    axis2_om_element_t *property_el = NULL;
    axis2_om_element_t *temp_el = NULL;
    axis2_om_node_t *temp_el_node = NULL;
    
    desc = axis2_woden_desc_to_desc_element(desc, env);
    property = AXIS2_WODEN_DESC_ELEMENT_CREATE_PROPERTY_ELEMENT(desc, env);
    property = axis2_woden_property_to_nested_element(property, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(property, env, parent);
    types = AXIS2_WODEN_DESC_ELEMENT_GET_TYPES_ELEMENT(desc, env);
    AXIS2_WODEN_PROPERTY_SET_TYPES(property, env, types);

    attr_ref = axis2_qname_create_from_string(env, WODEN_ATTR_REF);
    property_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(property_el_node, env);
    ref = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(property_el, env, WODEN_ATTR_REF);
   
    if(NULL != ref)
    {
        axis2_uri_t *ref_uri = get_uri(reader, env, ref);
        property = axis2_woden_property_to_property_element(property, env);
        AXIS2_WODEN_PROPERTY_ELEMENT_SET_REF(property, env, ref_uri);
    }
 
    property = axis2_woden_property_to_attr_extensible(property, env); 
    parse_ext_attributes(reader, env, property_el_node, "property_element", 
            property, desc);
    
    temp_el = axis2_om_util_get_first_child_element(property_el, env, property_el_node, 
            &temp_el_node);

    while (NULL != temp_el && NULL != temp_el_node)
    {
        axis2_qname_t *q_elem_documentation = NULL;
        axis2_qname_t *q_elem_value = NULL;
        axis2_qname_t *q_elem_constraint = NULL;

        q_elem_documentation = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_DOCUMENTATION);
        q_elem_value = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_VALUE);
        q_elem_constraint = axis2_qname_create_from_string(env, 
                WODEN_Q_ELEM_CONSTRAINT);

        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            property = axis2_woden_property_to_documentable(property, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(property, env, documentation);
        }
        if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_value, temp_el_node))
        {
            axis2_om_child_element_iterator_t *node_list = NULL;
            axis2_generic_obj_t *node_list_obj = NULL;
            
            /* The property value consists of the child info items of <value> */
            node_list = AXIS2_OM_ELEMENT_GET_CHILD_ELEMENTS(temp_el, env, temp_el_node);
            AXIS2_GENERIC_OBJ_SET_VALUE(node_list_obj, env, node_list);
            AXIS2_GENERIC_OBJ_SET_FREE_FUNC(node_list_obj, env, node_list->ops->free_fn);
            property = axis2_woden_property_to_property_element(property, env);
            AXIS2_PROPERTY_ELEMENT_SET_VALUE(property, env);
        }
        else if(AXIS2_TRUE == axis2_qname_util_matches(env, 
                    q_elem_constraint, temp_el_node))
        {
            axis2_om_node_t *node = NULL;
            axis2_char_t *text_value = NULL;
            
            /* TODO t.b.c. Assume <constraint> has just '#value' or a qname 
             * and don't check for extraneous text or child elements?
             */
            node = AXIS2_OM_NODE_GET_FIRST_CHILD(temp_el_node, env);
            if(NULL != node && AXIS2_OM_TEXT == AXIS2_OM_NODE_GET_NODE_TYPE(
                        node, env))
            {
                axis2_char_t *temp_str = NULL;
                int len = 0;
                axis2_om_text_t *text_node = NULL;

                text_node = AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
                temp_str = AXIS2_OM_TEXT_GET_VALUE(text_node, env);
                text_value = AXIS2_STRTRIM(env, temp_str, NULL);
                len = AXIS2_STRLEN(text_value);
                if(len > 0)
                {
                    if(0 == AXIS2_STRCMP(text_value, WODEN_NMTOKEN_VALUE))
                    {
                        property = axis2_woden_property_to_property_element(property, env);
                        AXIS2_WODEN_PROPERTY_ELEMENT_SET_HAS_VALUE_TOKEN(property, env, AXIS2_TRUE);
                    }
                    else
                    {
                        axis2_qname_t *qname = NULL;

                        qname = woden_om_util_get_qname(env, temp_el_node, 
                                text_value, desc);
                        property = axis2_woden_property_to_property_element(property, env);
                        AXIS2_WODEN_PROPERTY_ELEMENT_SET_CONSTRAINT_QNAME(property, env, qname);
                    }
                }
            }
        }
        else
        {
            void *ext_element = NULL;

            ext_element = parse_ext_element(reader, env, "property_element", 
                    property, temp_el_node, desc);
            property = axis2_woden_property_to_element_extensible(property, env);
            AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(property, env, ext_element);
        }

        temp_el = axis2_om_util_get_next_sibling_element(temp_el, env, 
                temp_el_node, &temp_el_node); 
    }
    
    return property;
}

axis2_status_t AXIS2_CALL
parse_ext_attributes(
        void *reader,
        const axis2_env_t *env,
        axis2_om_node_t *om_el_node,
        axis2_char_t *wsdl_class,
        void *wsdl_obj,
        void *desc)
{
    axis2_hash_t *node_map = NULL;
    axis2_om_element_t *om_el = NULL;
    axis2_hash_index_t *index = NULL;

    om_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(om_el_node, env);
    node_map = AXIS2_OM_ELEMENT_GET_ALL_ATTRIBUTES(om_el, env);
    for (index = axis2_hash_first (node_map, env); index; index = 
            axis2_hash_next(env, index))
    {
        void *om_attr = NULL;
        axis2_char_t *localname = NULL;
        axis2_om_namespace_t *namespc = NULL;
        axis2_char_t *namespc_uri_str = NULL;
        axis2_char_t *prefix = NULL;
        axis2_qname_t *attr_type = NULL;
        
        axis2_hash_this (index, NULL, NULL, &om_attr);
        localname = AXIS2_OM_ATTRIBUTE_GET_LOCALNAME((axis2_om_attribute_t *) om_attr, env);
        namespc = AXIS2_OM_ATTRIBUTE_GET_NAMESPACE((axis2_om_attribute_t *) om_attr, env);
        namespc_uri_str = AXIS2_OM_NAMESPACE_GET_URI(namespc, env);
        prefix = AXIS2_OM_NAMESPACE_GET_PREFIX(namespc, env);
        if(!prefix) prefix = "";
        attr_type = axis2_qname_create(env, localname, namespc_uri_str, prefix);
        if(NULL != namespc_uri_str && 0 != AXIS2_STRCMP(namespc_uri_str, 
                    WODEN_NS_URI_WSDL20))
        {
            if(0 != AXIS2_STRCMP(namespc_uri_str, WODEN_NS_URI_XMLNS) &&
               0 != AXIS2_STRCMP(namespc_uri_str, WODEN_NS_URI_XSI))
            {
                void *xml_attr = NULL;
                axis2_woden_ext_registry_t *ext_reg = NULL;
                
                desc = axis2_woden_desc_to_desc_element(desc, env);
                ext_reg = AXIS2_WODEN_DESC_ELEMENT_GET_EXT_REGISTRY(desc, env);
                if(NULL != ext_reg)
                {
                    xml_attr = AXIS2_WODEN_EXT_REGISTRY_QUERY_EXT_ATTR_TYPE(
                            ext_reg, env, wsdl_class, attr_type); 
                    if(NULL != xml_attr)
                    {
                        axis2_char_t *attr_value = NULL;

                        attr_value = AXIS2_OM_ATTRIBUTE_GET_VALUE((axis2_om_attribute_t *) om_attr, env);
                        AXIS2_WODEN_XML_ATTR_INIT(xml_attr, env, om_el, 
                                om_el_node, attr_type, attr_value);
                        AXIS2_WODEN_ATTR_EXTENSIBLE_SET_EXT_ATTR(wsdl_obj, env, 
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
        axis2_om_node_t *el_node,
        void *desc)
{
    axis2_qname_t *element_type = NULL;
    axis2_char_t *namespc_uri_str = NULL;
    axis2_om_namespace_t *namespc = NULL;
    axis2_om_element_t *el = NULL;
    axis2_woden_ext_registry_t *ext_reg = NULL;
    void *ext_ds = NULL;
    void *ext_el = NULL;

    element_type = axis2_qname_util_new_qname(env, el_node);
    
    el = AXIS2_OM_NODE_GET_DATA_ELEMENT(el_node, env);
    namespc = AXIS2_OM_ELEMENT_GET_NAMESPACE(el, env, el_node);
    namespc_uri_str = AXIS2_OM_NAMESPACE_GET_URI(namespc, env);
    if(NULL == namespc_uri_str || (0 == AXIS2_STRCMP(namespc_uri_str, WODEN_NS_URI_WSDL20)))
    {
        /* TODO Set error message */
        return NULL;
    }
   
    desc = axis2_woden_desc_to_desc_element(desc, env);
    ext_reg = AXIS2_WODEN_DESC_ELEMENT_GET_EXT_REGISTRY(desc, env);
    if(NULL == ext_reg)
    {
        /* TODO Set error message */
        return NULL;
    }
    
    ext_ds = AXIS2_WODEN_EXT_REGISTRY_QUERY_DESERIALIZER(ext_reg, env, 
            parent_type, element_type);
    
    ext_el = (void *) AXIS2_WODEN_EXT_DESERIALIZER_UNMARSHALL(ext_ds, env, parent_type, 
            parent, element_type, el_node, desc, ext_reg);

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
        axis2_om_node_t *el_node,
        axis2_qname_t *qname)
{
    if(AXIS2_TRUE != axis2_qname_util_matches(env, qname, el_node))
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
    axis2_om_node_t *doc_el_node = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    context_uri = AXIS2_WODEN_DESC_ELEMENT_GET_DOCUMENT_BASE_URI(desc, env);
    location_uri = axis2_uri_parse_relative(env, context_uri, location_uri_str);
    if(AXIS2_ERROR_NONE != AXIS2_ERROR_GET_STATUS_CODE(env->error))
    {
        /* Can't continue import with a bad URL.*/
        return NULL;
    }
    location_str = AXIS2_URI_TO_STRING(location_uri, env, AXIS2_URI_UNP_OMITUSERINFO);

    /* Check if WSDL imported or included previously from this location.*/
    referenced_desc = axis2_hash_get(wsdl_modules, location_str, AXIS2_HASH_KEY_STRING);
    
    if(NULL == referenced_desc)
    {
        /* not previously imported or included, so retrieve the WSDL.*/
        void *schema_col = NULL;
        void *xml_reader = NULL;
        axis2_om_stax_builder_t *builder = NULL;
        axis2_om_document_t *doc = NULL;
        axis2_qname_t *qname = NULL;

        schema_col = axis2_xml_schema_collection_create(env);
        xml_reader = axis2_xml_reader_create_for_file(env, location_str, NULL);
        builder = axis2_om_stax_builder_create(env, xml_reader);
        doc = AXIS2_OM_STAX_BUILDER_GET_DOCUMENT(builder, env);
        doc_el_node = AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(doc, env); 
        if(AXIS2_ERROR_NONE != AXIS2_ERROR_GET_STATUS_CODE(env->error))
        {
            /* Cannot contine without the referenced document */
            return NULL;
        }
        /* The referenced document should contain a WSDL <description> */
        qname = axis2_qname_create_from_string(env, WODEN_Q_ELEM_DESCRIPTION);
        if(AXIS2_TRUE != axis2_qname_util_matches(env, qname, doc_el_node))
        {
            /* Cannot continue without a <description> element */
            AXIS2_QNAME_FREE(qname, env);
            return NULL;
        }
        AXIS2_QNAME_FREE(qname, env);
        
        referenced_desc = parse_desc(reader, env, location_str, 
                doc_el_node, wsdl_modules);
        if(AXIS2_TRUE != axis2_hash_contains_key(wsdl_modules, env, 
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
        void *reader,
        const axis2_env_t *env,
        axis2_char_t *uri_str)
{
    woden_reader_impl_t *reader_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, uri_str, NULL);
    reader_impl = INTF_TO_IMPL(reader);

    if(reader_impl->f_uri)
    {
        AXIS2_URI_FREE(reader_impl->f_uri, env);
        reader_impl->f_uri = NULL;
    }
    reader_impl->f_uri = axis2_uri_parse_string(env, uri_str);
    
    return reader_impl->f_uri;
}

