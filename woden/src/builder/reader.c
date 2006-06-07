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
#include <woden/wsdl20/axis2_woden_desc.h>
#include <woden/wsdl20/extension/axis2_woden_ext_registry.h>

typedef struct woden_reader_impl woden_reader_impl_t;

/** 
 * @brief Woden Wsdl Reader Struct Impl
 *	Woden Wsdl Reader  
 */ 
struct woden_reader_impl
{
    woden_reader_t reader;

    axis2_om_document_t *om_doc;
    axis2_om_node_t *root_node;
    /* A map of imported schema definitions keyed by schema location URI */
    axis2_hash_t *f_imported_schemas;

};

#define INTF_TO_IMPL(reader) ((woden_reader_impl_t *) reader)

axis2_status_t AXIS2_CALL 
woden_reader_free(
        void *reader,
        const axis2_env_t *env);

static woden_reader_t *
create(
        const axis2_env_t *env)
{
    woden_reader_impl_t *reader_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    reader_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_reader_impl_t));

    reader_impl->f_imported_schemas = NULL;
    
    reader_impl->reader.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_reader_ops_t));
    
    reader_impl->f_imported_schemas = axis2_hash_make(env);
    if(reader_impl->f_imported_schemas)
    {
        AXIS2_SET_ERROR((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
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

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, document_base_uri, NULL);
    AXIS2_PARAM_CHECK(env->error, desc_el_node, NULL);
    reader_impl = INTF_TO_IMPL(reader);

    check_elemnt_qname(reader, env, desc_el_node, WODEN_Q_ELEM_DESCRIPTION);
    
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
    ext_reg = get_ext_registry(reader, env);
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
    for (index = axis2_hash_first (ht, env); index; index = axis2_hash_next (
                env, index))
    {
        void *v = NULL;
        axis2_om_attribute_t *attr = NULL;
        axis2_om_namespace_t *namespc = NULL;
        axis2_char_t *namespc_uri = NULL;
        axis2_char_t *local_part = NULL;
        axis2_char_t *value = NULL;
        
        axis2_hash_this (i, NULL, NULL, &v);
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
   
    parse_ext_attributes(reader, env, desc_el, "description_element", desc, desc);
    
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

        if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            desc = axis2_woden_desc_to_documentable(desc, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(desc, env, documentation);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_import, temp_el_node))
        {
            void *import_elment = NULL;

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
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_include, temp_el_node))
        {
            void *include_elment = NULL;

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
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_types, temp_el_node))
        {
            void *types = NULL;

            types = parse_types(reader, env, temp_el_node, desc);
            AXIS2_WODEN_DESC_ELEMENT_SET_TYPES_ELEMENT(desc, env, types);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_interface, temp_el_node))
        {
            void *interface = NULL;

            interface = parse_interface(reader, env, temp_el_node, desc);
            AXIS2_WODEN_DESC_ELEMENT_ADD_INTERFACE_ELEMENT(desc, env, interface);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_binding, temp_el_node))
        {
            void *binding = NULL;

            binding = parse_binding(reader, env, temp_el_node, desc);
            AXIS2_WODEN_DESC_ELEMENT_ADD_BINDING_ELEMENT(desc, env, binding);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
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
    schema = parse_schema_import(reader, env, desc);
    types = axis2_woden_types_to_types_element(types, env);
    AXIS2_WODEN_TYPES_ELEMENT_ADD_SCHEMA(types, env, schema);
    /*
      Document schemaDoc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
      Element schemaElem = schemaDoc.createElementNS("http://www.w3.org/2001/XMLSchema", "import");
      schemaElem.setAttribute("namespace", "http://www.w3.org/2001/XMLSchema");
      schemaElem.setAttribute("schemaLocation", "http://www.w3.org/2001/XMLSchema.xsd");
      desc.getTypesElement().addSchema(parseSchemaImport(schemaElem, desc));
      */
    
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
    
    desc = axis2_woden_desc_to_desc_element(desc, env);
    documentation = AXIS2_WODEN_DESC_ELEMENT_CREATE_DOCUMENTATION_ELEMENT(desc, 
            env);
    
    /* TODO store doc_el as below, or just extract any text? */
    documentation = axis2_woden_documentation_to_documentation_element(
            documentation, env);
    AXIS2_WODEN_DOCUMENTATION_ELEMENT_SET_CONTENT(documenation, env, doc_el_node);
    
    /* Now parse any extensibility attributes or elements */
    parse_ext_attributes(reader, env, doc_el_node, "documentation_element", 
            documentation, desc); 

    doc_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(doc_el_node, env);
    temp_el = axis2_om_util_get_first_child_element(doc_el, env, doc_el_node, 
            &temp_el_node); 

    while (NULL != temp_el)
    {
        void *ext_el = NULL;

        ext_el = parse_ext_element(reader, env, "documentation_element", 
                documentation, temp_el, desc);
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

    imp = axis2_woden_desc_element_create_import_element(env);
    import_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(import_el_node, env);
    
    namespc_uri = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(import_el, env, 
            WODEN_ATTR_NAMESPACE); 
    location_uri = AXIS2_OM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(import_el, env, 
            WODEN_ATTR_LOCATION); 
   
    parse_ext_attributes(reader, env, "import_element", imp, desc);

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
    
private IncludeElement parseInclude(Element includeEl,
                                    DescriptionElement desc,
                                    Map wsdlModules) 
                                    throws WSDLException
{
    IncludeElement include = desc.createIncludeElement();
    
    String locationURI = DOMUtils.getAttribute(includeEl, Constants.ATTR_LOCATION);
    
    parseExtensionAttributes(includeEl, IncludeElement.class, include, desc);

    if(locationURI != null)
    {
        include.setLocation(getURI(locationURI));
        DescriptionElement includedDesc = 
            getWSDLFromLocation(locationURI, desc, wsdlModules);
        include.setDescriptionElement(includedDesc);
    }
    
    return include;
}

/*
 * TODO Initial schema parsing is specific to XML Schema. 
 * Need generic support for other type systems.
 * Consider extension architecture with serializer/deserializer.
 */
private TypesElement parseTypes(Element typesEl,
                                DescriptionElement desc) 
                                throws WSDLException
{
    TypesElement types = desc.createTypesElement();
    
    //TODO for now set to W3 XML Schema. Later, add support for non-XML Schema type systems
    types.setTypeSystem(Constants.TYPE_XSD_2001);
    
    parseExtensionAttributes(typesEl, TypesElement.class, types, desc);

    Element tempEl = DOMUtils.getFirstChildElement(typesEl);

    while (tempEl != null)
    {
        QName tempElType = QNameUtils.newQName(tempEl);
        
        //TODO validate element order? <documentation> must be first.
        
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            types.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (SchemaConstants.XSD_IMPORT_QNAME_LIST.contains(tempElType))
        {
            types.addSchema(parseSchemaImport(tempEl, desc));
        }
        else if (SchemaConstants.XSD_SCHEMA_QNAME_LIST.contains(tempElType))
        {
            types.addSchema(parseSchemaInline(tempEl, desc));
        }
        else
        {
            types.addExtensionElement(
                parseExtensionElement(TypesElement.class, types, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }
    
    return types;
}

private Schema parseSchemaInline(Element schemaEl,
                                 DescriptionElement desc) 
                                 throws WSDLException
{
    InlinedSchemaImpl schema = new InlinedSchemaImpl();
    
    schema.setId(DOMUtils.getAttribute(schemaEl, Constants.ATTR_ID));
    
    String tns = DOMUtils.getAttribute(schemaEl, Constants.ATTR_TARGET_NAMESPACE);
    if(tns != null) {
        schema.setNamespace(getURI(tns));
    }
    
    String baseURI = desc.getDocumentBaseURI() != null ?
                     desc.getDocumentBaseURI().toString() : null;
    XmlSchema schemaDef = null;
    
    try {
        XmlSchemaCollection xsc = new XmlSchemaCollection();
        schemaDef = xsc.read(schemaEl, baseURI);
    } 
    catch (XmlSchemaException e) 
    {
        getErrorReporter().reportError(
                new ErrorLocatorImpl(),  //TODO line&col nos.
                "WSDL521", 
                new Object[] {baseURI}, 
                ErrorReporter.SEVERITY_WARNING,
                e);
    }
    
    if(schemaDef != null) {
        schema.setSchemaDefinition(schemaDef);
    } else {
        schema.setReferenceable(false);
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
        void *desc)
{
    woden_reader_impl_t *reader_impl = NULL;
    void *schema = NULL;
    axis2_uri_t *uri = NULL;
    void *schema_def = NULL; 
    axis2_uri_t *context_uri = NULL;
    axis2_char_t *shcema_loc = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, desc, AXIS2_FAILURE);
    reader_impl = INTF_TO_IMPL(reader);

    uri = get_uri(reader, env, "http://www.w3.org/2001/XMLSchema");
    schema = axis2_woden_imported_schema_create(env);
    schema = axis2_woden_imported_schema_to_schema(schema, env);
    AXIS2_WODEN_SCHEMA_SET_NAMESPACE(schema, env, uri);
    
    uri = get_uri(reader, env, "http://www.w3.org/2001/XMLSchema.xsd");
    AXIS2_WODEN_IMPORTED_SCHEMA_SET_LOCATION(schema, env, uri);
    
    if(NULL == AXIS2_WODEN_SCHEMA_GET_NAMESPACE(schema, env))
    {
        /* The namespace attribute is REQUIRED on xs:import, so don't continue. */
        AXIS2_WODEN_SCHEMA_SET_REFERENCEABLE(schema, env AXIS2_FALSE);
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
    schema_loc = AXIS2_URI_TO_STRING(uri, env);
    uri = axis2_string_util_get_url(env, context_uri, schema_loc);
    if(AXIS2_ERROR_NONE != AXIS2_ERROR_GET_STATUS_CODE(env->error))
    {
        /* can't continue schema retrieval with a bad URL.*/
        AXIS2_WODEN_SCHEMA_SET_REFERENCEABLE(schema, env AXIS2_FALSE);
        return schema;
    }

    schema_url = AXIS2_UTL_TO_EXTERNAL_FORM(uri, env);
    
    /* If the schema has already been imported, reuse it. */
    schema_def = axis2_hash_get(reader_impl->f_imported_schemas, schema_url, 
            AXIS2_HASH_KEY_STRING);
    if(NULL != schema_def)
    {
        /* Not previously imported, so retrieve it now. */
        void *schema_col = NULL;
        void *xml_reader = NULL;

        schema_col = axis2_xml_schema_collection_create(env);
        xml_reader = axis2_xml_reader_create_for_file(env, schema_url, NULL);
        schema_def = AXIS2_XML_SCHEMA_COLLECTION_READ_WITH_READER(schema_col, 
                env, xml_reader);
        if(AXIS2_ERROR_NONE != AXIS2_ERROR_GET_STATUS_CODE(env->error))
        {
            AXIS2_WODEN_SCHEMA_SET_REFERENCEABLE(schema, env AXIS2_FALSE);
            return schema;
        }
        axis2_hash_set(reader_impl->f_imported_schemas, schema_url, 
                AXIS2_HASH_KEY_STRING, schema_def);

    }
    
    if(NULL != schema_def) 
    {
        AXIS2_XML_SCHEMA_SET_SCHEMA_DEF(schema, env, schema_def);
    } else 
    {
        AXIS2_WODEN_SCHEMA_SET_REFERENCEABLE(schema, env AXIS2_FALSE);
    }
    
    return schema;
}

void *parse_interface(
        void reader,
        const axis2_env_t *env,
        axis2_om_node_t *interface_el_node,
        void *desc)
{
    void *intface = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *style_default = NULL;
    axis2_om_element_t *interface_el;
    axis2_qname_t *attr_qname = NULL;
    axis2_qname_t *attr_style_default = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    intface = AXIS2_WODEN_DESC_ELEMENT_CREATE_INTERFACE_ELEMENT(desc, env);

    attr_qname = axis2_qname_create_from_string(env, WODEN_ATTR_NAME);
    interface_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(interface_el_node, env);
    name = axis2_om_util_get_attribute_value(interface_el, env, attr_qname);
    
    if(NULL != name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        namespc = AXIS2_WODEN_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        namespc_str = AXIS2_URI_TO_STRING(namespc, env);
        qname = axis2_qname_create(env, name, namespc_str, NULL);
        intface = axis2_woden_interface_to_interface_element(intface, env);
        AXIS2_WODEN_INTERFACE_ELEMENT_SET_QNAME(intface, env, qname);
    }
    
    attr_style_default = axis2_qname_create_from_string(env, WODEN_ATTR_STYLE_DEFAULT);
    style_default = axis2_om_util_get_attribute_value(interface_el, env, attr_style_default);
    
    if(NULL != style_default)
    {
        axis2_array_list_t *string_list = NULL;
        axis2_char_t *uri_str = NULL;
        int i = 0, size = 0;

        string_list = axis2_tokenize(env, style_default, ''); 
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
   
    parser_ext_attributes(reader, env, interface_el_node, "interface_element", 
            intface, desc);
    
    /* Parse the child elements of <interface>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <fault> <operation> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 
    temp_el = axis2_om_util_get_first_child_element(desc_el, env, desc_el_node, 
            &interface_el_node);

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

        if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            intface = axis2_woden_interface_to_documentable(intface, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(intface, env, documentation);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_fault, temp_el_node))
        {
            void *intface_fault = NULL;

            intface_falut = parse_interface_fault(reader, env, temp_el_node, desc, 
                    intface);
            intface = axis2_woden_desc_to_interface_element(intface, env);
            AXIS2_WODEN_INTERFACE_ELEMENT_ADD_INTERFACE_FAULT_ELEMENT(intface, env, intface_fault);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_op, temp_el_node))
        {
            void *intface_op = NULL;

            intface_falut = parse_interface_op(reader, env, temp_el_node, desc, 
                    intface);
            intface = axis2_woden_desc_to_interface_element(intface, env);
            AXIS2_WODEN_INTERFACE_ELEMENT_ADD_INTERFACE_OP_ELEMENT(intface, env, intface_op);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, intface);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(desc, env, feature);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, intface);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(desc, env, property);
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

void *parse_interface_fault(
        void reader,
        const axis2_env_t *env,
        axis2_om_node_t *fault_el_node,
        void *desc,
        void *parent)
{
    void *fault = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *element = NULL;
    axis2_om_element_t *fault_el;
    axis2_qname_t *attr_qname = NULL;
    axis2_qname_t *attr_element = NULL;
    void *types = NULL;

    desc = axis2_woden_desc_to_desc_element(desc, env);
    fault = AXIS2_WODEN_DESC_ELEMENT_CREATE_INTERFACE_FAULT_ELEMENT(desc, env);
    axis2_woden_interface_fault_to_nested_element(fault, env);
    AXIS2_WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(fault, env, parent); 
    types = AXIS2_WODEN_DESC_ELEMENT_GET_TYPES_ELEMENT(desc, env);
    AXIS2_WODEN_INTERFACE_FAULT_SET_TYPES(fault, env, types);

    attr_qname = axis2_qname_create_from_string(env, WODEN_ATTR_NAME);
    fault_el = AXIS2_OM_NODE_GET_DATA_ELEMENT(fault_el_node, env);
    name = axis2_om_util_get_attribute_value(fault_el, env, attr_qname);
    
    if(NULL != name)
    {
        axis2_qname_t *qname = NULL;
        axis2_uri_t *namespc = NULL;
        axis2_char_t *namespc_str = NULL;

        ns = AXIS2_WODEN_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc, env);
        if(!ns)
            ns_str = WODEN_VALUE_EMPTY_STRING;
        ns_str = AXIS2_URI_TO_STRING(ns, env);
        qname = axis2_qname_create(env, name, ns_str, NULL);
        fault = axis2_woden_interface_to_interface_fault_element(fault, env);
        AXIS2_WODEN_INTERFACE_FAULT_ELEMENT_SET_QNAME(fault, env, qname);
    }
    
    attr_element = axis2_qname_create_from_string(env, WODEN_ATTR_ELEMENT);
    element = axis2_om_util_get_attribute_value(fault_el, env, attr_element);
    
    if(NULL != element)
    {
        axis2_qname_t *qname = NULL;
        qname = woden_om_util_get_qname(element, fault_el, desc);
    }
    
    /* TODO extends attribute */
   
    parser_ext_attributes(reader, env, interface_el_node, "interface_element", 
            intface, desc);
    
    /* Parse the child elements of <interface>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <fault> <operation> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 
    temp_el = axis2_om_util_get_first_child_element(desc_el, env, desc_el_node, 
            &interface_el_node);

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

        if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_documentation, temp_el_node))
        {
            void *documentation = NULL;

            documentation = parse_documentation(reader, env, temp_el_node, desc);
            intface = axis2_woden_interface_to_documentable(intface, env);
            AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(intface, env, documentation);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_fault, temp_el_node))
        {
            void *intface_fault = NULL;

            intface_falut = parse_interface_fault(reader, env, temp_el_node, desc, 
                    intface);
            intface = axis2_woden_desc_to_interface_element(intface, env);
            AXIS2_WODEN_INTERFACE_ELEMENT_ADD_INTERFACE_FAULT_ELEMENT(intface, env, intface_fault);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_op, temp_el_node))
        {
            void *intface_op = NULL;

            intface_falut = parse_interface_op(reader, env, temp_el_node, desc, 
                    intface);
            intface = axis2_woden_desc_to_interface_element(intface, env);
            AXIS2_WODEN_INTERFACE_ELEMENT_ADD_INTERFACE_OP_ELEMENT(intface, env, intface_op);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_feature, temp_el_node))
        {
            void *feature = NULL;

            feature = parse_feature(reader, env, temp_el_node, desc, intface);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(desc, env, feature);
        }
        else if(AXIS2_TURE == axis2_qname_util_matches(env, 
                    q_elem_property, temp_el_node))
        {
            void *property = NULL;

            property = parse_property(reader, env, temp_el_node, desc, intface);
            AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(desc, env, property);
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
private InterfaceFaultElement parseInterfaceFault(
                                         Element faultEl,
                                         DescriptionElement desc,
                                         WSDLElement parent) 
                                         throws WSDLException
{
    InterfaceFaultElement fault = desc.createInterfaceFaultElement();
    fault.setParentElement(parent);
    ((InterfaceFaultImpl)fault).setTypes(desc.getTypesElement());
    
    String name = DOMUtils.getAttribute(faultEl, Constants.ATTR_NAME);
    if(name != null)
    {
        String ns = desc.getTargetNamespace() != null ? 
                    desc.getTargetNamespace().toString() : 
                    Constants.VALUE_EMPTY_STRING;
        QName qname = new QName(ns, name);
        fault.setName(qname);
    }
    
    String element = DOMUtils.getAttribute(faultEl, Constants.ATTR_ELEMENT);
    if(element != null)
    {
        try {
            QName qname = DOMUtils.getQName(element, faultEl, desc);
            fault.setElementName(qname);
        } catch (WSDLException e) {
            getErrorReporter().reportError( 
                    new ErrorLocatorImpl(),  //TODO line&col nos.
                    "WSDL505",
                    new Object[] {element, QNameUtils.newQName(faultEl)}, 
                    ErrorReporter.SEVERITY_ERROR);
        }
    }
    
    parseExtensionAttributes(faultEl, InterfaceFaultElement.class, fault, desc);

    Element tempEl = DOMUtils.getFirstChildElement(faultEl);

    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            fault.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FEATURE, tempEl))
        {
            fault.addFeatureElement(parseFeature(tempEl, desc, fault));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_PROPERTY, tempEl))
        {
            fault.addPropertyElement(parseProperty(tempEl, desc, fault));
        }
        else
        {
            fault.addExtensionElement(
                parseExtensionElement(InterfaceFaultElement.class, fault, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }

    return fault;
}

private InterfaceOperationElement parseInterfaceOperation(
                                             Element operEl, 
                                             DescriptionElement desc,
                                             WSDLElement parent) 
                                             throws WSDLException
{
    InterfaceOperationElement oper = desc.createInterfaceOperationElement();
    oper.setParentElement(parent);
    
    String name = DOMUtils.getAttribute(operEl, Constants.ATTR_NAME);
    if(name != null)
    {
        QName qname = new QName(desc.getTargetNamespace().toString(), name);
        oper.setName(qname);
    }
    
    String style = DOMUtils.getAttribute(operEl, Constants.ATTR_STYLE);
    if(style != null)
    {
        List stringList = StringUtils.parseNMTokens(style);
        String uriString = null;
        Iterator it = stringList.iterator();
        while(it.hasNext())
        {
            uriString = (String)it.next();
            oper.addStyleURI(getURI(uriString));
        }
    }
    
    String pat = DOMUtils.getAttribute(operEl, Constants.ATTR_PATTERN);
    if(pat != null)
    {
        oper.setPattern(getURI(pat));
    }
   
    parseExtensionAttributes(operEl, InterfaceOperationElement.class, oper, desc);
    
    /* Parse the child elements of interface <operation>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <input> <output> <infault> <outfault> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 

    Element tempEl = DOMUtils.getFirstChildElement(operEl);

    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            oper.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FEATURE, tempEl))
        {
            oper.addFeatureElement(parseFeature(tempEl, desc, oper));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_PROPERTY, tempEl))
        {
            oper.addPropertyElement(parseProperty(tempEl, desc, oper));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_INPUT, tempEl))
        {
            oper.addInterfaceMessageReferenceElement(parseInterfaceMessageReference(tempEl, desc, oper));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_OUTPUT, tempEl))
        {
            oper.addInterfaceMessageReferenceElement(parseInterfaceMessageReference(tempEl, desc, oper));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_INFAULT, tempEl))
        {
            oper.addInterfaceFaultReferenceElement(parseInterfaceFaultReference(tempEl, desc, oper));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_OUTFAULT, tempEl))
        {
            oper.addInterfaceFaultReferenceElement(parseInterfaceFaultReference(tempEl, desc, oper));
        }
        else
        {
            oper.addExtensionElement(
                parseExtensionElement(InterfaceOperationElement.class, oper, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }

    return oper;
}

private InterfaceFaultReferenceElement parseInterfaceFaultReference(
                                             Element faultRefEl,
                                             DescriptionElement desc,
                                             WSDLElement parent)
                                             throws WSDLException
{
    InterfaceFaultReferenceElement faultRef = desc.createInterfaceFaultReferenceElement();
    faultRef.setParentElement(parent);
    
    if(Constants.ELEM_INFAULT.equals(faultRefEl.getLocalName())) {
        faultRef.setDirection(Direction.IN);
    } 
    else if(Constants.ELEM_OUTFAULT.equals(faultRefEl.getLocalName())){
        faultRef.setDirection(Direction.OUT);
    }
    
    String ref = DOMUtils.getAttribute(faultRefEl, Constants.ATTR_REF);
    if(ref != null)
    {
        try {
            QName qname = DOMUtils.getQName(ref, faultRefEl, desc);
            faultRef.setRef(qname);
        } catch (WSDLException e) {
            getErrorReporter().reportError( 
                    new ErrorLocatorImpl(),  //TODO line&col nos.
                    "WSDL505",
                    new Object[] {ref, QNameUtils.newQName(faultRefEl)}, 
                    ErrorReporter.SEVERITY_ERROR);
        }
    }
    
    String msgLabel = DOMUtils.getAttribute(faultRefEl, Constants.ATTR_MESSAGE_LABEL);
    if(msgLabel != null)
    {
        if(msgLabel.equals(MessageLabel.IN.toString())) {
            faultRef.setMessageLabel(MessageLabel.IN);
        } else if(msgLabel.equals(MessageLabel.OUT.toString())) {
            faultRef.setMessageLabel(MessageLabel.OUT);
        } else {
            //invalid value, but capture it anyway.
            faultRef.setMessageLabel(MessageLabel.invalidValue(msgLabel));
        }
    }
    
    parseExtensionAttributes(faultRefEl, InterfaceFaultReferenceElement.class, faultRef, desc);
    
    Element tempEl = DOMUtils.getFirstChildElement(faultRefEl);
    
    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            faultRef.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FEATURE, tempEl))
        {
            faultRef.addFeatureElement(parseFeature(tempEl, desc, faultRef));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_PROPERTY, tempEl))
        {
            faultRef.addPropertyElement(parseProperty(tempEl, desc, faultRef));
        }
        else
        {
            faultRef.addExtensionElement(
                parseExtensionElement(InterfaceFaultReferenceElement.class, faultRef, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }
    
    return faultRef;
}

private InterfaceMessageReferenceElement parseInterfaceMessageReference(
                                             Element msgRefEl,
                                             DescriptionElement desc,
                                             WSDLElement parent)
                                             throws WSDLException
{
    InterfaceMessageReferenceElement message = desc.createInterfaceMessageReferenceElement();
    message.setParentElement(parent);
    ((InterfaceMessageReferenceImpl)message).setTypes(desc.getTypesElement());
    
    if(Constants.ELEM_INPUT.equals(msgRefEl.getLocalName())) {
        message.setDirection(Direction.IN);
    } 
    else if(Constants.ELEM_OUTPUT.equals(msgRefEl.getLocalName())) {
        message.setDirection(Direction.OUT);
    }
    
    String msgLabel = DOMUtils.getAttribute(msgRefEl, Constants.ATTR_MESSAGE_LABEL);
    if(msgLabel != null) 
    {
        if(msgLabel.equals(MessageLabel.IN.toString())) {
            message.setMessageLabel(MessageLabel.IN);
        } else if(msgLabel.equals(MessageLabel.OUT.toString())) {
            message.setMessageLabel(MessageLabel.OUT);
        } else {
            //invalid value, but capture it anyway.
            message.setMessageLabel(MessageLabel.invalidValue(msgLabel));
        }
    }
    else
    {
        //TODO this is a temp fix, correct action to use MEP to determine default
        if(message.getDirection().equals(Direction.IN))
        {
            message.setMessageLabel(MessageLabel.IN);
        }
        else
        {
            message.setMessageLabel(MessageLabel.OUT);
        }
    }
    
    String element = DOMUtils.getAttribute(msgRefEl, Constants.ATTR_ELEMENT);
    if(element != null)
    {
        if(element.equals(Constants.NMTOKEN_ANY) ||
           element.equals(Constants.NMTOKEN_NONE) ||
           element.equals(Constants.NMTOKEN_OTHER))
        {
            message.setMessageContentModel(element);
        }
        else
        {
            //element is not #any, #none or #other, so it must be an element qname
            message.setMessageContentModel(Constants.NMTOKEN_ELEMENT);
            try {
                QName qname = DOMUtils.getQName(element, msgRefEl, desc);
                message.setElementName(qname);
            } catch (WSDLException e) {
                getErrorReporter().reportError( 
                        new ErrorLocatorImpl(),  //TODO line&col nos.
                        "WSDL505",
                        new Object[] {element, QNameUtils.newQName(msgRefEl)}, 
                        ErrorReporter.SEVERITY_ERROR);
            }
        }
    }
    else
    {
        //Per mapping defined in WSDL 2.0 Part 2 spec section 2.5.3,
        //if element attribute not present, message content model is #other
        message.setMessageContentModel(Constants.NMTOKEN_OTHER);
    }

    parseExtensionAttributes(msgRefEl, InterfaceMessageReferenceElement.class, message, desc);

    Element tempEl = DOMUtils.getFirstChildElement(msgRefEl);

    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            message.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FEATURE, tempEl))
        {
            message.addFeatureElement(parseFeature(tempEl, desc, message));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_PROPERTY, tempEl))
        {
            message.addPropertyElement(parseProperty(tempEl, desc, message));
        }
        else
        {
            message.addExtensionElement(
                parseExtensionElement(InterfaceMessageReferenceElement.class, message, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }
    
    return message;
}

private BindingElement parseBinding(Element bindEl,
                                    DescriptionElement desc)
                                    throws WSDLException
{
    BindingElement binding = desc.createBindingElement();

    String name = DOMUtils.getAttribute(bindEl, Constants.ATTR_NAME);
    if(name != null)
    {
        QName qname = new QName(desc.getTargetNamespace().toString(), name);
        binding.setName(qname);
    }
    
    QName intfaceQN = null;
    String intface = DOMUtils.getAttribute(bindEl, Constants.ATTR_INTERFACE);
    if(intface != null)
    {
        try {
            intfaceQN = DOMUtils.getQName(intface, bindEl, desc);
            binding.setInterfaceName(intfaceQN);
        } catch (WSDLException e) {
            getErrorReporter().reportError( 
                    new ErrorLocatorImpl(),  //TODO line&col nos.
                    "WSDL505",
                    new Object[] {intface, QNameUtils.newQName(bindEl)}, 
                    ErrorReporter.SEVERITY_ERROR);
        }
    }
    
    //Dereference the interface qname
    if(intfaceQN != null)
    {
        InterfaceElement[] interfaces = desc.getInterfaceElements();
        for(int i=0; i<interfaces.length; i++)
        {
            if( intfaceQN.equals(interfaces[i].getName()) )
            {
                ((BindingImpl)binding).setInterfaceElement(interfaces[i]);
            }
        }
    }
    
    String type = DOMUtils.getAttribute(bindEl, Constants.ATTR_TYPE);
    if(type != null) {
        binding.setType(getURI(type));
    }
    
    parseExtensionAttributes(bindEl, BindingElement.class, binding, desc);
    
    /* Parse the child elements of <binding>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * <fault> <operation> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 

    Element tempEl = DOMUtils.getFirstChildElement(bindEl);

    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            binding.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FAULT, tempEl))
        {
            binding.addBindingFaultElement(parseBindingFault(tempEl, desc, binding));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_OPERATION, tempEl))
        {
            binding.addBindingOperationElement(parseBindingOperation(tempEl, desc, binding));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FEATURE, tempEl))
        {
            binding.addFeatureElement(parseFeature(tempEl, desc, binding));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_PROPERTY, tempEl))
        {
            binding.addPropertyElement(parseProperty(tempEl, desc, binding));
        }
        else
        {
            binding.addExtensionElement(
                parseExtensionElement(BindingElement.class, binding, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }
    
    return binding;
}

private BindingFaultElement parseBindingFault(Element bindFaultEl,
                                              DescriptionElement desc,
                                              WSDLElement parent)
                                              throws WSDLException
{
    BindingFaultElement fault = desc.createBindingFaultElement();
    fault.setParentElement(parent);
    
    QName intFltQN = null;
    String ref = DOMUtils.getAttribute(bindFaultEl, Constants.ATTR_REF);
    if(ref != null)
    {
        try {
            intFltQN = DOMUtils.getQName(ref, bindFaultEl, desc);
            fault.setRef(intFltQN);
        } catch (WSDLException e) {
            getErrorReporter().reportError( 
                    new ErrorLocatorImpl(),  //TODO line&col nos.
                    "WSDL505",
                    new Object[] {ref, QNameUtils.newQName(bindFaultEl)}, 
                    ErrorReporter.SEVERITY_ERROR);
        }
    }
    
    //Dereference the interface fault qname
    if(intFltQN != null)
    {
        InterfaceElement intface = ((BindingElement)parent).getInterfaceElement();
        if(intface != null)
        {
            InterfaceFaultElement[] intFaults = intface.getInterfaceFaultElements();
            for(int i=0; i<intFaults.length; i++)
            {
                if( intFltQN.equals(intFaults[i].getName()) )
                {
                    ((BindingFaultImpl)fault).setInterfaceFaultElement(intFaults[i]);
                }
            }
        }
    }
    
    parseExtensionAttributes(bindFaultEl, BindingFaultElement.class, fault, desc);
    
    /* Parse the child elements of binding <fault>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 

    Element tempEl = DOMUtils.getFirstChildElement(bindFaultEl);

    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            fault.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FEATURE, tempEl))
        {
            fault.addFeatureElement(parseFeature(tempEl, desc, fault));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_PROPERTY, tempEl))
        {
            fault.addPropertyElement(parseProperty(tempEl, desc, fault));
        }
        else
        {
            fault.addExtensionElement(
                parseExtensionElement(BindingFaultElement.class, fault, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }
    
    return fault;
}

private BindingOperationElement parseBindingOperation(
                                             Element bindOpEl,
                                             DescriptionElement desc,
                                             WSDLElement parent)
                                             throws WSDLException
{
    BindingOperationElement oper = desc.createBindingOperationElement();
    oper.setParentElement(parent);
    
    QName refQN = null;
    String ref = DOMUtils.getAttribute(bindOpEl, Constants.ATTR_REF);
    if(ref != null)
    {
        try {
            refQN = DOMUtils.getQName(ref, bindOpEl, desc);
            oper.setRef(refQN);
        } catch (WSDLException e) {
            getErrorReporter().reportError( 
                    new ErrorLocatorImpl(),  //TODO line&col nos.
                    "WSDL505",
                    new Object[] {ref, QNameUtils.newQName(bindOpEl)}, 
                    ErrorReporter.SEVERITY_ERROR);
        }
    }
    
    //Dereference the 'ref' qname to an InterfaceOperationElement
    if(refQN != null)
    {
        InterfaceElement intface = ((BindingElement)parent).getInterfaceElement();
        InterfaceOperationElement[] intOps = intface.getInterfaceOperationElements();
        for(int i=0; i<intOps.length; i++)
        {
            if( refQN.equals(intOps[i].getName()) )
            {
                ((BindingOperationImpl)oper).setInterfaceOperationElement(intOps[i]);
            }
        }
    }
    
    parseExtensionAttributes(bindOpEl, BindingOperationElement.class, oper, desc);
    
    /* Parse the child elements of binding <operation>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <input> <output> <infault> <outfault> <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 

    Element tempEl = DOMUtils.getFirstChildElement(bindOpEl);

    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            oper.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FEATURE, tempEl))
        {
            oper.addFeatureElement(parseFeature(tempEl, desc, oper));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_PROPERTY, tempEl))
        {
            oper.addPropertyElement(parseProperty(tempEl, desc, oper));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_INPUT, tempEl))
        {
            oper.addBindingMessageReferenceElement(parseBindingMessageReference(tempEl, desc, oper));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_OUTPUT, tempEl))
        {
            oper.addBindingMessageReferenceElement(parseBindingMessageReference(tempEl, desc, oper));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_INFAULT, tempEl))
        {
            oper.addBindingFaultReferenceElement(parseBindingFaultReference(tempEl, desc, oper));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_OUTFAULT, tempEl))
        {
            oper.addBindingFaultReferenceElement(parseBindingFaultReference(tempEl, desc, oper));
        }
        else
        {
            oper.addExtensionElement(
                parseExtensionElement(BindingOperationElement.class, oper, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }
    
    return oper;
}

private BindingFaultReferenceElement parseBindingFaultReference(
                                              Element faultRefEl,
                                              DescriptionElement desc,
                                              WSDLElement parent)
                                              throws WSDLException
{
    BindingFaultReferenceElement faultRef = desc.createBindingFaultReferenceElement();
    faultRef.setParentElement(parent);
    
    QName refQN = null;
    String ref = DOMUtils.getAttribute(faultRefEl, Constants.ATTR_REF);
    if(ref != null)
    {
        try {
            refQN = DOMUtils.getQName(ref, faultRefEl, desc);
            faultRef.setRef(refQN);
        } catch (WSDLException e) {
            getErrorReporter().reportError( 
                    new ErrorLocatorImpl(),  //TODO line&col nos.
                    "WSDL505",
                    new Object[] {ref, QNameUtils.newQName(faultRefEl)}, 
                    ErrorReporter.SEVERITY_ERROR);
        }
    }
    
    String msgLabel = DOMUtils.getAttribute(faultRefEl, Constants.ATTR_MESSAGE_LABEL);
    if(msgLabel != null)
    {
        if(msgLabel.equals(MessageLabel.IN.toString())) {
            faultRef.setMessageLabel(MessageLabel.IN);
        } else if(msgLabel.equals(MessageLabel.OUT.toString())) {
            faultRef.setMessageLabel(MessageLabel.OUT);
        } else {
            //invalid value, but capture it anyway.
            faultRef.setMessageLabel(MessageLabel.invalidValue(msgLabel));
        }
    }
    
    //Dereference the 'ref' and 'messageLabel' attributes to an interface fault reference
    if(faultRef.getRef() != null && 
       (faultRef.getMessageLabel() == MessageLabel.IN || 
        faultRef.getMessageLabel() == MessageLabel.OUT)  )
    {
        BindingOperationElement bindOp = (BindingOperationElement)parent;
        InterfaceOperationElement intOp = bindOp.getInterfaceOperationElement();
        if(intOp != null)
        {
            InterfaceFaultReferenceElement[] intFltRefs = intOp.getInterfaceFaultReferenceElements();
            for(int i=0; i<intFltRefs.length; i++)
            {
                if((faultRef.getRef()).equals(intFltRefs[i].getRef()) &&
                   (faultRef.getMessageLabel()).equals(intFltRefs[i].getMessageLabel()) )
                {
                    ((BindingFaultReferenceImpl)faultRef).setInterfaceFaultReferenceElement(intFltRefs[i]);
                }
            }
        }
    }
    
    parseExtensionAttributes(faultRefEl, BindingFaultReferenceElement.class, faultRef, desc);
    
    /* Parse the child elements of binding operation <infault> or <outfault>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 

    Element tempEl = DOMUtils.getFirstChildElement(faultRefEl);
    
    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            faultRef.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FEATURE, tempEl))
        {
            faultRef.addFeatureElement(parseFeature(tempEl, desc, faultRef));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_PROPERTY, tempEl))
        {
            faultRef.addPropertyElement(parseProperty(tempEl, desc, faultRef));
        }
        else
        {
            faultRef.addExtensionElement(
                parseExtensionElement(BindingFaultReferenceElement.class, faultRef, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }
    
    return faultRef;
}

private BindingMessageReferenceElement parseBindingMessageReference(
                                             Element msgRefEl,
                                             DescriptionElement desc,
                                             WSDLElement parent)
                                             throws WSDLException
{
    BindingMessageReferenceElement message = desc.createBindingMessageReferenceElement();
    message.setParentElement(parent);
    
    if(Constants.ELEM_INPUT.equals(msgRefEl.getLocalName())) {
        message.setDirection(Direction.IN);
    } 
    else if(Constants.ELEM_OUTPUT.equals(msgRefEl.getLocalName())) {
        message.setDirection(Direction.OUT);
    }
    
    String msgLabel = DOMUtils.getAttribute(msgRefEl, Constants.ATTR_MESSAGE_LABEL);
    if(msgLabel != null) 
    {
        if(msgLabel.equals(MessageLabel.IN.toString())) {
            message.setMessageLabel(MessageLabel.IN);
        } else if(msgLabel.equals(MessageLabel.OUT.toString())) {
            message.setMessageLabel(MessageLabel.OUT);
        } else {
            //invalid value, but capture it anyway.
            message.setMessageLabel(MessageLabel.invalidValue(msgLabel));
        }
    }
    else
    {
        //TODO this is a temp fix, correct action to use MEP to determine default
        if(message.getDirection().equals(Direction.IN))
        {
            message.setMessageLabel(MessageLabel.IN);
        }
        else
        {
            message.setMessageLabel(MessageLabel.OUT);
        }
    }
    
    //Dereference the msgLabel to an InterfaceMessageReferenceElement
    if(message.getMessageLabel() == MessageLabel.IN || 
       message.getMessageLabel() == MessageLabel.OUT)
    {
        BindingOperationElement bindOp = (BindingOperationElement)parent;
        InterfaceOperationElement intOp = bindOp.getInterfaceOperationElement();
        if(intOp != null)
        {
            InterfaceMessageReferenceElement[] intMsgRefs = intOp.getInterfaceMessageReferenceElements();
            for(int i=0; i<intMsgRefs.length; i++)
            {
                if( (message.getMessageLabel()).equals(intMsgRefs[i].getMessageLabel()) )
                {
                    ((BindingMessageReferenceImpl)message).setInterfaceMessageReferenceElement(intMsgRefs[i]);
                }
            }
        }
    }
    
    parseExtensionAttributes(msgRefEl, BindingMessageReferenceElement.class, message, desc);

    /* Parse the child elements of binding operation <input> or <output>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 

    Element tempEl = DOMUtils.getFirstChildElement(msgRefEl);

    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            message.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FEATURE, tempEl))
        {
            message.addFeatureElement(parseFeature(tempEl, desc, message));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_PROPERTY, tempEl))
        {
            message.addPropertyElement(parseProperty(tempEl, desc, message));
        }
        else
        {
            message.addExtensionElement(
                parseExtensionElement(BindingMessageReferenceElement.class, message, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }
    
    return message;
}

private ServiceElement parseService(Element serviceEl,
                                    DescriptionElement desc)
                                    throws WSDLException
{
    ServiceElement service = desc.createServiceElement();

    String name = DOMUtils.getAttribute(serviceEl, Constants.ATTR_NAME);
    if(name != null)
    {
        QName qname = new QName(desc.getTargetNamespace().toString(), name);
        service.setName(qname);
    }
    
    QName intfaceQN = null;
    String intface = DOMUtils.getAttribute(serviceEl, Constants.ATTR_INTERFACE);
    if(intface != null)
    {
        try {
            intfaceQN = DOMUtils.getQName(intface, serviceEl, desc);
            service.setInterfaceName(intfaceQN);
        } catch (WSDLException e) {
            getErrorReporter().reportError( 
                    new ErrorLocatorImpl(),  //TODO line&col nos.
                    "WSDL505",
                    new Object[] {intface, QNameUtils.newQName(serviceEl)}, 
                    ErrorReporter.SEVERITY_ERROR);
        }
    }
    
    //Dereference the interface qname
    if(intfaceQN != null)
    {
        InterfaceElement[] interfaces = desc.getInterfaceElements();
        for(int i=0; i<interfaces.length; i++)
        {
            if( intfaceQN.equals(interfaces[i].getName()) )
            {
                ((ServiceImpl)service).setInterfaceElement(interfaces[i]);
            }
        }
    }
    
    parseExtensionAttributes(serviceEl, ServiceElement.class, service, desc);

    /* Parse the child elements of <service>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <endpoint>
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 

    Element tempEl = DOMUtils.getFirstChildElement(serviceEl);

    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            service.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_ENDPOINT, tempEl))
        {
            service.addEndpointElement(parseEndpoint(tempEl, desc, service));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FEATURE, tempEl))
        {
            service.addFeatureElement(parseFeature(tempEl, desc, service));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_PROPERTY, tempEl))
        {
            service.addPropertyElement(parseProperty(tempEl, desc, service));
        }
        else
        {
            service.addExtensionElement(
                parseExtensionElement(ServiceElement.class, service, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }
    
    return service;
}
                                    
private EndpointElement parseEndpoint(Element endpointEl,
                                      DescriptionElement desc,
                                      WSDLElement parent)
                                      throws WSDLException
{
    EndpointElement endpoint = desc.createEndpointElement();
    endpoint.setParentElement(parent);

    String name = DOMUtils.getAttribute(endpointEl, Constants.ATTR_NAME);
    if(name != null)
    {
        NCName ncname = new NCName(name);
        endpoint.setName(ncname);
    }
    
    QName bindingQN = null;
    String binding = DOMUtils.getAttribute(endpointEl, Constants.ATTR_BINDING);
    if(binding != null)
    {
        try {
            bindingQN = DOMUtils.getQName(binding, endpointEl, desc);
            endpoint.setBindingName(bindingQN);
        } catch (WSDLException e) {
            getErrorReporter().reportError( 
                    new ErrorLocatorImpl(),  //TODO line&col nos.
                    "WSDL505",
                    new Object[] {binding, QNameUtils.newQName(endpointEl)}, 
                    ErrorReporter.SEVERITY_ERROR);
        }
    }
    
    //Dereference the binding qname
    if(bindingQN != null)
    {
        BindingElement[] bindings = desc.getBindingElements();
        for(int i=0; i<bindings.length; i++)
        {
            if( bindingQN.equals(bindings[i].getName()) )
            {
                ((EndpointImpl)endpoint).setBindingElement(bindings[i]);
            }
        }
    }
    
    String address = 
        DOMUtils.getAttribute(endpointEl, Constants.ATTR_ADDRESS);
    
    if(address != null)
    {
        endpoint.setAddress(getURI(address));
    }

    parseExtensionAttributes(endpointEl, EndpointElement.class, endpoint, desc);
    
    /* Parse the child elements of <endpoint>. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation> 
     * <feature> <property> or extension elements in any order
     * TODO validate that the elements are in correct order
     */ 

    Element tempEl = DOMUtils.getFirstChildElement(endpointEl);

    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            endpoint.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_FEATURE, tempEl))
        {
            endpoint.addFeatureElement(parseFeature(tempEl, desc, endpoint));
        }
        else if (QNameUtils.matches(Constants.Q_ELEM_PROPERTY, tempEl))
        {
            endpoint.addPropertyElement(parseProperty(tempEl, desc, endpoint));
        }
        else
        {
            endpoint.addExtensionElement(
                parseExtensionElement(ServiceElement.class, endpoint, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }
    
    return endpoint;
}

private FeatureElement parseFeature(Element featEl, 
                                    DescriptionElement desc,
                                    WSDLElement parent) 
                                    throws WSDLException
{
    FeatureElement feature = desc.createFeatureElement();
    feature.setParentElement(parent);
    
    String ref = DOMUtils.getAttribute(featEl, Constants.ATTR_REF);
    if(ref != null)
    {
        feature.setRef(getURI(ref));
    }
    
    String req = DOMUtils.getAttribute(featEl, Constants.ATTR_REQUIRED);
    feature.setRequired(Constants.VALUE_TRUE.equals(req) ? true : false);
    //TODO t.b.c. what if attr value is not 'true' or 'false'? (eg, missing, mispelt or not lower case.
    
    parseExtensionAttributes(featEl, FeatureElement.class, feature, desc);
    
    /* Parse the child elements of the <feature> element. 
     * As per WSDL 2.0 spec, they must be in the following order if present:
     * <documentation>
     * extension elements.
     * 
     * TODO validate that the elements are in correct order
     */ 
    
    Element tempEl = DOMUtils.getFirstChildElement(featEl);
    
    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            feature.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else
        {
            feature.addExtensionElement(
                parseExtensionElement(FeatureElement.class, feature, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
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
private PropertyElement parseProperty(Element propEl, 
                                      DescriptionElement desc,
                                      WSDLElement parent)
                                      throws WSDLException
{
    PropertyElement property = desc.createPropertyElement();
    property.setParentElement(parent);
    ((PropertyImpl)property).setTypes(desc.getTypesElement());
    
    String ref = DOMUtils.getAttribute(propEl, Constants.ATTR_REF);
    if(ref != null)
    {
        property.setRef(getURI(ref));
    }
    
    parseExtensionAttributes(propEl, PropertyElement.class, property, desc);
    
    Element tempEl = DOMUtils.getFirstChildElement(propEl);

    while (tempEl != null)
    {
        if (QNameUtils.matches(Constants.Q_ELEM_DOCUMENTATION, tempEl))
        {
            property.addDocumentationElement(parseDocumentation(tempEl, desc));
        }
        else if(QNameUtils.matches(Constants.Q_ELEM_VALUE, tempEl))
        {
            //the property value consists of the child info items of <value>
            NodeList nodeList = tempEl.getChildNodes();
            property.setValue(nodeList);
        }
        else if(QNameUtils.matches(Constants.Q_ELEM_CONSTRAINT, tempEl))
        {
            //TODO t.b.c. assume <constraint> has just '#value' or a qname 
            //and don't check for extraneous text or child elements?
            
            Node node = tempEl.getFirstChild();
            if(node != null && node instanceof Text)
            {
                Text textNode = (Text)node;
                String textValue = textNode.getData().trim();
                if(textValue.length() > 0)
                {
                    if(textValue.equals(Constants.NMTOKEN_VALUE))
                    {
                        property.setHasValueToken(true);
                    }
                    else 
                    {
                        try {
                            QName qname = DOMUtils.getQName(textValue, tempEl, desc);
                            property.setConstraintName(qname);
                        } catch (WSDLException e) {
                            getErrorReporter().reportError(
                                    new ErrorLocatorImpl(),  //TODO line&col nos.
                                    "WSDL505",
                                    new Object[] {textValue, QNameUtils.newQName(tempEl)},
                                    ErrorReporter.SEVERITY_ERROR,
                                    e);
                        }
                    }
                }
            }
        }
        else
        {
            property.addExtensionElement(
                parseExtensionElement(PropertyElement.class, property, tempEl, desc) );
        }
        
        tempEl = DOMUtils.getNextSiblingElement(tempEl);
    }
    
    return property;
}

private void parseExtensionAttributes(Element domEl, 
                                      Class wsdlClass, 
                                      WSDLElement wsdlObj,
                                      DescriptionElement desc)
                                      throws WSDLException
{
    NamedNodeMap nodeMap = domEl.getAttributes();
    int length = nodeMap.getLength();
    
    for (int i = 0; i < length; i++)
    {
        Attr domAttr = (Attr)nodeMap.item(i);
        String localName = domAttr.getLocalName();
        String namespaceURI = domAttr.getNamespaceURI();
        String prefix = domAttr.getPrefix();
        QName attrType = new QName(namespaceURI, localName, (prefix != null ? prefix : ""));
        
        if (namespaceURI != null && !namespaceURI.equals(Constants.NS_URI_WSDL20))
        {
            if (!namespaceURI.equals(Constants.NS_URI_XMLNS) && 
                !namespaceURI.equals(Constants.NS_URI_XSI))  //TODO handle xsi attrs elsewhere, without need to register
            {
                //TODO reg namespaces at appropriate element scope, not just at desc.
                //DOMUtils.registerUniquePrefix(prefix, namespaceURI, desc);
                
                XMLAttr xmlAttr = null;
                ExtensionRegistry extReg = desc.getExtensionRegistry();
                if (extReg != null)
                {
                    xmlAttr = extReg.createExtAttribute(wsdlClass, attrType);
                    if(xmlAttr != null) //TODO use an 'UnknownAttr' class in place of null
                    {
                        String attrValue = domAttr.getValue();
                        xmlAttr.init(domEl, attrType, attrValue);
                        wsdlObj.setExtensionAttribute(attrType, xmlAttr);
                    }
                }
                else
                {
                    //This reader cannot handle extensions, so stop.
                    break;
                }
            }
            else
            {
                //TODO parse xmlns namespace declarations - here or elsewhere?
            }
        }
        else
        {
            //TODO confirm non-native attrs in WSDL 2.0 namespace will be detected by schema validation,
            //so no need to handle error here.
        }
    }
    
}

private ExtensionElement parseExtensionElement(Class parentType,
                                               WSDLElement parent,
                                               Element el,
                                               DescriptionElement desc)
                                               throws WSDLException
{
    QName elementType = QNameUtils.newQName(el);
    String namespaceURI = el.getNamespaceURI();
    
    try
    {
        if (namespaceURI == null || namespaceURI.equals(Constants.NS_URI_WSDL20))
        {
            getErrorReporter().reportError(
                    new ErrorLocatorImpl(),  //TODO line&col nos.
                    "WSDL520",
                    new Object[] {elementType, parentType.getName()},
                    ErrorReporter.SEVERITY_ERROR);
            return null;
        }
        
        ExtensionRegistry extReg = desc.getExtensionRegistry();
        
        if (extReg == null)
        {
            getErrorReporter().reportError(
                    new ErrorLocatorImpl(),  //TODO line&col nos.
                    "WSDL514",
                    new Object[] {elementType, parentType.getName()},
                    ErrorReporter.SEVERITY_ERROR);
            return null;
        }
        
        ExtensionDeserializer extDS = extReg.queryDeserializer(parentType,
                elementType);
        
        return extDS.unmarshall(parentType, parent, elementType, el, desc, extReg);
    }
    catch (WSDLException e)
    {
        if (e.getLocation() == null)
        {
            e.setLocation(XPathUtils.getXPathExprFromNode(el));
        }
        
        throw e;
    }
}


/* ************************************************************
 *  Utility/helper methods
 * ************************************************************/

/**
 * Check the actual element encountered against the expected qname
 * 
 * @param el actual element encountered
 * @param qname expected element's qname
 * @throws WSDLException
 */
private void checkElementName(Element el, QName qname)
throws WSDLException
{
    if (!QNameUtils.matches(qname, el))
    {
        getErrorReporter().reportError(
            new ErrorLocatorImpl(),  //TODO line&col nos.
            "WSDL501", 
            new Object[] {qname, QNameUtils.newQName(el)}, 
            ErrorReporter.SEVERITY_FATAL_ERROR);
            
        //TODO wsdlExc.setLocation(XPathUtils.getXPathExprFromNode(el));
            
    }
}
    
private Document getDocument(InputSource inputSource, String desc)
                             throws WSDLException, IOException
{
    //TODO use 'desc' URL in any error message(s) for problem resolution.
    
//        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
//            
//        factory.setNamespaceAware(true);
    
    DOMParser parser = new DOMParser();
    try
    {
        parser.setFeature(org.apache.xerces.impl.Constants.SAX_FEATURE_PREFIX + org.apache.xerces.impl.Constants.NAMESPACES_FEATURE, true);
        parser.setFeature(org.apache.xerces.impl.Constants.SAX_FEATURE_PREFIX + org.apache.xerces.impl.Constants.NAMESPACE_PREFIXES_FEATURE, true);
    }
    catch(SAXNotRecognizedException e)
    {
        
    }
    catch(SAXNotSupportedException e)
    {
        
    }
    
    // Enable validation on the XML parser if it has been enabled 
    // for the Woden parser.
    if(features.getValue(ReaderFeatures.VALIDATION_FEATURE_ID))
    {
        //factory.setValidating(true);
        try
        {
            parser.setFeature(org.apache.xerces.impl.Constants.SAX_FEATURE_PREFIX + org.apache.xerces.impl.Constants.VALIDATION_FEATURE, true);
            parser.setFeature(org.apache.xerces.impl.Constants.XERCES_FEATURE_PREFIX + org.apache.xerces.impl.Constants.SCHEMA_VALIDATION_FEATURE, true);
            // TODO: This external schema location should be removed once an URI resolution framework
            // with a catalog is added to Woden.
            parser.setProperty(org.apache.xerces.impl.Constants.XERCES_PROPERTY_PREFIX + org.apache.xerces.impl.Constants.SCHEMA_LOCATION, "http://www.w3.org/2006/01/wsdl http://www.w3.org/2006/01/wsdl/wsdl20.xsd http://www.w3.org/2006/01/wsdl-extensions http://www.w3.org/2006/01/wsdl-extensions.xsd http://www.w3.org/2001/XMLSchema http://www.w3.org/2001/XMLSchema.xsd");
        }
        catch(SAXNotRecognizedException e)
        {
            System.out.println("validation not supported by parser.");
        }
        catch(SAXNotSupportedException e)
        {
            
        }
    }
    else
    {
        //factory.setValidating(false);
    }
        
    Document doc = null;
        
    try {
          
        //DocumentBuilder builder = factory.newDocumentBuilder();
        //builder.getDOMImplementation().hasFeature();
        //builder.setErrorHandler(new ErrorHandlerWrapper(getErrorReporter()));
        //builder.setEntityResolver(new DefaultHandler());
        //doc = builder.parse(inputSource);
        parser.parse(inputSource);
        doc = parser.getDocument();
            
    } 
//        catch (ParserConfigurationException e) 
//        {
//            String msg = getErrorReporter().getFormattedMessage("WSDL002", new Object[] {"XML"});
//            throw new WSDLException(WSDLException.CONFIGURATION_ERROR, msg, e);
//        } 
    catch (SAXException e) 
    {
        getErrorReporter().reportError(
            new ErrorLocatorImpl(),  //TODO line&col nos.
            "WSDL500", 
            new Object[] {"SAX", desc}, 
            ErrorReporter.SEVERITY_FATAL_ERROR, 
            e);
    } 
        
    //TODO - potentially returns null. correct after deciding how 
    //to handle exceptions (e.g. return inside try block).
    return doc;
}

/*
 * Convert a string of type xs:anyURI to a java.net.URI.
 * An empty string argument will return an empty string URI.
 * A null argument will return a null.
 */
private URI getURI(String anyURI) throws WSDLException
{
    URI uri = null;
    if(anyURI != null)
    {
        try {
            uri = new URI(anyURI);
        } catch (URISyntaxException e) {
            getErrorReporter().reportError(
                    new ErrorLocatorImpl(),  //TODO line&col nos.
                    "WSDL506", 
                    new Object[] {anyURI}, 
                    ErrorReporter.SEVERITY_ERROR, 
                    e);
        }
    }
    return uri;
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

    Element docEl;
 
    desc = axis2_woden_desc_to_desc_element(desc, env);
    context_uri = AXIS2_WODEN_DESC_ELEMENT_GET_DOCUMENT_BASE_URI(desc, env);
    location_uri = axis2_string_utils_get_url(env, context_uri, location_uri_str);
    if(AXIS2_ERROR_NONE != AXIS2_ERROR_GET_STATUS_CODE(error))
    {
        /* Can't continue import with a bad URL.*/
        return NULL;
    }
    location_str = AXIS2_URI_TO_STRING(location_uri, env);

    /* Check if WSDL imported or included previously from this location.*/
    referenced_desc = axis2_hash_get(wsdl_modules, location_str, AXIS2_HASH_KEY_STRING);
    
    if(NULL == referenced_desc)
    {
        /* not previously imported or included, so retrieve the WSDL.*/
        void *schema_col = NULL;
        void *xml_reader = NULL;
        axis2_om_stax_builder_t *builder = NULL;
        axis2_om_document_t *doc = NULL;
        axis2_om_node_t *doc_el_node = NULL;
        axis2_om_element_t *doc_el = NULL;
        axis2_qname_t *qname = NULL;

        schema_col = axis2_xml_schema_collection_create(env);
        xml_reader = axis2_xml_reader_create_for_file(env, location_str, NULL);
        builder = axis2_om_stax_builder_create(env, parser);
        doc = AXIS2_OM_STAX_BUILDER_GET_DOCUMENT(builder, env);
        doc_el_node = AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(doc, env); 
        if(AXIS2_ERROR_NONE != AXIS2_ERROR_GET_STATUS_CODE(env->error))
        {
            /* Cannot contine without the referenced document */
            return NULL;
        }
        /* The referenced document should contain a WSDL <description> */
        qname = axis2_qname_create_from_string(env, WODEN_Q_ELEM_DESCRIPTION);
        if(AXIS2_TRUE != axis2_qname_util_matches(qname, doc_el_node))
        {
            /* Cannot continue without a <description> element */
            AXIS2_QNAME_FREE(qname, env);
            return NULL;
        }
        AXIS2_QNAME_FREE(qname, env);
        
        referenced_desc = parse_description(reader, env, location_str, 
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

/**
 * A wrapper that plugs Woden's error reporter mechanism into the
 * XML parser used to parse the WSDL document.
 */
class ErrorHandlerWrapper implements org.xml.sax.ErrorHandler
{
    /**
     * The error reporter used to report errors in Woden.
     */
    private ErrorReporter errorReporter;
    
    /**
     * Constructor.
     * 
     * @param errorReporter The error reporter to be wrapped.
     */
    public ErrorHandlerWrapper(ErrorReporter errorReporter)
    {
        this.errorReporter = errorReporter;
    }

    /* (non-Javadoc)
     * @see org.xml.sax.ErrorHandler#error(org.xml.sax.SAXParseException)
     */
    public void error(SAXParseException error) throws SAXException 
    {
        ErrorLocatorImpl locator = new ErrorLocatorImpl();
        locator.setLineNumber(error.getLineNumber());
        locator.setColumnNumber(error.getColumnNumber());
        try
        {
            errorReporter.reportError(locator, null, error.getMessage(), ErrorReporter.SEVERITY_ERROR, error.getException());
        }
        catch(WSDLException e)
        {
            throw new SAXException("A problem occurred setting the error in the Woden error reporter wrapper.", e);
        }
        
    }

    /* (non-Javadoc)
     * @see org.xml.sax.ErrorHandler#fatalError(org.xml.sax.SAXParseException)
     */
    public void fatalError(SAXParseException error) throws SAXException 
    {
        ErrorLocatorImpl locator = new ErrorLocatorImpl();
        locator.setLineNumber(error.getLineNumber());
        locator.setColumnNumber(error.getColumnNumber());
        try
        {
            errorReporter.reportError(locator, null, error.getMessage(), ErrorReporter.SEVERITY_FATAL_ERROR, error.getException());
        }
        catch(WSDLException e)
        {
            throw new SAXException("A problem occurred setting the error in the Woden error reporter wrapper.", e);
        }
        
    }

    /* (non-Javadoc)
     * @see org.xml.sax.ErrorHandler#warning(org.xml.sax.SAXParseException)
     */
    public void warning(SAXParseException warning) throws SAXException 
    {
        ErrorLocatorImpl locator = new ErrorLocatorImpl();
        locator.setLineNumber(warning.getLineNumber());
        locator.setColumnNumber(warning.getColumnNumber());
        try
        {
            errorReporter.reportError(locator, null, warning.getMessage(), ErrorReporter.SEVERITY_WARNING, warning.getException());
        }
        catch(WSDLException e)
        {
            throw new SAXException("A problem occurred setting the error in the Woden error reporter wrapper.", e);
        }
        
    }
    
}

class WSDLEntityResolver implements org.xml.sax.EntityResolver
{

    public InputSource resolveEntity(String publicId, String systemId) throws SAXException, IOException {
        // TODO Auto-generated method stub
        
        return null;
    }
    
}

static axis2_uri_t *
get_uri(
        void *reader,
        static axis2_env_t *env,
        axis2_char_t *uri_str)
{
    woden_reader_impl_t *reader_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, uri_str, NULL);
    reader_impl = INTF_TO_IMPL(reader);

    if(reader_impl->uri)
    {
        AXIS2_URI_FREE(reader_impl->uri, env);
        reader_impl->uri = NULL;
    }
    reader_impl->uri = axis2_uri_parse_string(env, uri_str);
    
    return reader_impl->uri;
}
