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

#include "wsdl2c_native.h"
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

/* WSDL namespace constants */
#define WSDL_NS_URI "http://schemas.xmlsoap.org/wsdl/"
#define SOAP_NS_URI "http://schemas.xmlsoap.org/wsdl/soap/"
#define XSD_NS_URI "http://www.w3.org/2001/XMLSchema"

/* Note: wsdl2c_operation_t is now defined in wsdl2c_native.h */

/* Message structure */
typedef struct wsdl2c_message {
    axis2_char_t *name;
    axutil_array_list_t *parts;
} wsdl2c_message_t;

/* Message part structure */
typedef struct wsdl2c_message_part {
    axis2_char_t *name;
    axis2_char_t *type;
    axis2_char_t *element;
} wsdl2c_message_part_t;

/* Helper function to register namespaces for XPath */
static void
register_namespaces(xmlXPathContextPtr xpath_ctx)
{
    xmlXPathRegisterNs(xpath_ctx, BAD_CAST "wsdl", BAD_CAST WSDL_NS_URI);
    xmlXPathRegisterNs(xpath_ctx, BAD_CAST "soap", BAD_CAST SOAP_NS_URI);
    xmlXPathRegisterNs(xpath_ctx, BAD_CAST "xsd", BAD_CAST XSD_NS_URI);
    /* Also register xs: prefix which is commonly used for XML Schema */
    xmlXPathRegisterNs(xpath_ctx, BAD_CAST "xs", BAD_CAST XSD_NS_URI);
}

/* Parse WSDL messages */
static axis2_status_t
parse_wsdl_messages(wsdl2c_context_t *context, xmlXPathContextPtr xpath_ctx, const axutil_env_t *env)
{
    xmlXPathObjectPtr messages_result = NULL;
    xmlNodeSetPtr messages_nodes = NULL;
    int i;

    AXIS2_PARAM_CHECK(env->error, context, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, xpath_ctx, AXIS2_FAILURE);

    /* Find all message elements */
    messages_result = xmlXPathEvalExpression(BAD_CAST "//wsdl:message", xpath_ctx);
    if (!messages_result || !messages_result->nodesetval) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No messages found in WSDL");
        return AXIS2_FAILURE;
    }

    messages_nodes = messages_result->nodesetval;
    context->wsdl->messages = axutil_array_list_create(env, messages_nodes->nodeNr);

    for (i = 0; i < messages_nodes->nodeNr; i++) {
        xmlNodePtr message_node = messages_nodes->nodeTab[i];
        wsdl2c_message_t *message = NULL;
        xmlChar *name = NULL;
        xmlNodePtr part_node = NULL;

        /* Get message name */
        name = xmlGetProp(message_node, BAD_CAST "name");
        if (!name) {
            continue;
        }

        message = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_message_t));
        message->name = axutil_strdup(env, (const axis2_char_t*)name);
        message->parts = axutil_array_list_create(env, 0);

        /* Parse message parts */
        for (part_node = message_node->children; part_node; part_node = part_node->next) {
            if (part_node->type == XML_ELEMENT_NODE &&
                xmlStrcmp(part_node->name, BAD_CAST "part") == 0) {

                wsdl2c_message_part_t *part = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_message_part_t));
                xmlChar *part_name = xmlGetProp(part_node, BAD_CAST "name");
                xmlChar *part_type = xmlGetProp(part_node, BAD_CAST "type");
                xmlChar *part_element = xmlGetProp(part_node, BAD_CAST "element");

                part->name = part_name ? axutil_strdup(env, (const axis2_char_t*)part_name) : NULL;
                part->type = part_type ? axutil_strdup(env, (const axis2_char_t*)part_type) : NULL;
                part->element = part_element ? axutil_strdup(env, (const axis2_char_t*)part_element) : NULL;

                axutil_array_list_add(message->parts, env, part);

                if (part_name) xmlFree(part_name);
                if (part_type) xmlFree(part_type);
                if (part_element) xmlFree(part_element);
            }
        }

        axutil_array_list_add(context->wsdl->messages, env, message);
        xmlFree(name);
    }

    xmlXPathFreeObject(messages_result);
    return AXIS2_SUCCESS;
}

/* Parse WSDL operations */
static axis2_status_t
parse_wsdl_operations(wsdl2c_context_t *context, xmlXPathContextPtr xpath_ctx, const axutil_env_t *env)
{
    xmlXPathObjectPtr operations_result = NULL;
    xmlNodeSetPtr operations_nodes = NULL;
    int i;

    AXIS2_PARAM_CHECK(env->error, context, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, xpath_ctx, AXIS2_FAILURE);

    /* Find all operation elements in portType */
    operations_result = xmlXPathEvalExpression(BAD_CAST "//wsdl:portType/wsdl:operation", xpath_ctx);
    if (!operations_result || !operations_result->nodesetval) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No operations found in WSDL");
        return AXIS2_FAILURE;
    }

    operations_nodes = operations_result->nodesetval;
    context->wsdl->operations = axutil_array_list_create(env, operations_nodes->nodeNr);

    for (i = 0; i < operations_nodes->nodeNr; i++) {
        xmlNodePtr operation_node = operations_nodes->nodeTab[i];
        wsdl2c_operation_t *operation = NULL;
        xmlChar *name = NULL;
        xmlNodePtr child_node = NULL;

        /* Get operation name */
        name = xmlGetProp(operation_node, BAD_CAST "name");
        if (!name) {
            continue;
        }

        operation = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_operation_t));
        operation->name = axutil_strdup(env, (const axis2_char_t*)name);
        operation->input_message = NULL;
        operation->output_message = NULL;
        operation->soap_action = NULL;

        /* Parse input and output messages */
        for (child_node = operation_node->children; child_node; child_node = child_node->next) {
            if (child_node->type == XML_ELEMENT_NODE) {
                if (xmlStrcmp(child_node->name, BAD_CAST "input") == 0) {
                    xmlChar *message = xmlGetProp(child_node, BAD_CAST "message");
                    if (message) {
                        operation->input_message = axutil_strdup(env, (const axis2_char_t*)message);
                        xmlFree(message);
                    }
                } else if (xmlStrcmp(child_node->name, BAD_CAST "output") == 0) {
                    xmlChar *message = xmlGetProp(child_node, BAD_CAST "message");
                    if (message) {
                        operation->output_message = axutil_strdup(env, (const axis2_char_t*)message);
                        xmlFree(message);
                    }
                }
            }
        }

        axutil_array_list_add(context->wsdl->operations, env, operation);
        xmlFree(name);
    }

    xmlXPathFreeObject(operations_result);
    return AXIS2_SUCCESS;
}

/* Parse WSDL bindings to extract soapAction for operations (AXIS2C-1581 fix) */
static axis2_status_t
parse_wsdl_bindings(wsdl2c_context_t *context, xmlXPathContextPtr xpath_ctx, const axutil_env_t *env)
{
    xmlXPathObjectPtr binding_ops_result = NULL;
    xmlNodeSetPtr binding_ops_nodes = NULL;
    int i, j;

    AXIS2_PARAM_CHECK(env->error, context, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, xpath_ctx, AXIS2_FAILURE);

    /* Find all binding operation elements */
    binding_ops_result = xmlXPathEvalExpression(
        BAD_CAST "//wsdl:binding/wsdl:operation", xpath_ctx);

    if (!binding_ops_result || !binding_ops_result->nodesetval) {
        /* No bindings found - this is OK, soapAction will remain NULL */
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "No binding operations found in WSDL");
        return AXIS2_SUCCESS;
    }

    binding_ops_nodes = binding_ops_result->nodesetval;

    /* For each binding operation, find the soap:operation and extract soapAction */
    for (i = 0; i < binding_ops_nodes->nodeNr; i++) {
        xmlNodePtr binding_op_node = binding_ops_nodes->nodeTab[i];
        xmlChar *op_name = NULL;
        xmlChar *soap_action = NULL;
        xmlNodePtr child_node = NULL;

        /* Get operation name from binding */
        op_name = xmlGetProp(binding_op_node, BAD_CAST "name");
        if (!op_name) {
            continue;
        }

        /* Find soap:operation child element */
        for (child_node = binding_op_node->children; child_node; child_node = child_node->next) {
            if (child_node->type == XML_ELEMENT_NODE &&
                xmlStrcmp(child_node->name, BAD_CAST "operation") == 0) {
                /* Check if this is a soap:operation element (check namespace) */
                if (child_node->ns && child_node->ns->href &&
                    (xmlStrcmp(child_node->ns->href, BAD_CAST SOAP_NS_URI) == 0 ||
                     xmlStrstr(child_node->ns->href, BAD_CAST "soap") != NULL)) {
                    soap_action = xmlGetProp(child_node, BAD_CAST "soapAction");
                    break;
                }
            }
        }

        /* Find the matching operation and set soapAction */
        if (context->wsdl && context->wsdl->operations) {
            int op_count = axutil_array_list_size(context->wsdl->operations, env);
            for (j = 0; j < op_count; j++) {
                wsdl2c_operation_t *operation = axutil_array_list_get(
                    context->wsdl->operations, env, j);
                if (operation && operation->name &&
                    xmlStrcmp(BAD_CAST operation->name, op_name) == 0) {

                    /* AXIS2C-1581 FIX: Only set soapAction if it's non-empty and not just quotes */
                    if (soap_action) {
                        const char *action_str = (const char*)soap_action;
                        /* Skip empty strings, quoted empty strings */
                        if (action_str[0] != '\0' &&
                            strcmp(action_str, "\"\"") != 0) {
                            operation->soap_action = axutil_strdup(env, action_str);
                            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                "Operation '%s' soapAction: '%s'",
                                operation->name, operation->soap_action);
                        } else {
                            /* Empty or quoted empty - leave as NULL (AXIS2C-1581 fix) */
                            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                "Operation '%s' has empty soapAction - omitting (AXIS2C-1581)",
                                operation->name);
                        }
                    }
                    break;
                }
            }
        }

        if (soap_action) {
            xmlFree(soap_action);
        }
        xmlFree(op_name);
    }

    xmlXPathFreeObject(binding_ops_result);
    return AXIS2_SUCCESS;
}

/* AXIS2C-1224: Parse complexTypes from schema, including empty sequences */
static axis2_status_t
parse_wsdl_complex_types(wsdl2c_context_t *context, xmlXPathContextPtr xpath_ctx, const axutil_env_t *env)
{
    xmlXPathObjectPtr complex_types_result = NULL;
    xmlNodeSetPtr complex_type_nodes = NULL;
    int i;

    AXIS2_PARAM_CHECK(env->error, context, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, xpath_ctx, AXIS2_FAILURE);

    /* Initialize complex types list */
    context->wsdl->complex_types = axutil_array_list_create(env, 0);
    context->wsdl->has_empty_sequences = AXIS2_FALSE;
    context->wsdl->has_typeless_elements = AXIS2_FALSE;

    /* Find all complexType elements in the schema */
    complex_types_result = xmlXPathEvalExpression(
        BAD_CAST "//xsd:complexType[@name] | //xs:complexType[@name]", xpath_ctx);

    if (!complex_types_result || !complex_types_result->nodesetval ||
        complex_types_result->nodesetval->nodeNr == 0) {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "No named complexTypes found in schema");
        if (complex_types_result) {
            xmlXPathFreeObject(complex_types_result);
        }
        return AXIS2_SUCCESS;
    }

    complex_type_nodes = complex_types_result->nodesetval;
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "AXIS2C-1224: Found %d complexTypes in schema", complex_type_nodes->nodeNr);

    for (i = 0; i < complex_type_nodes->nodeNr; i++) {
        xmlNodePtr complex_type_node = complex_type_nodes->nodeTab[i];
        wsdl2c_complex_type_t *complex_type = NULL;
        xmlChar *type_name = NULL;
        xmlNodePtr child_node = NULL;
        xmlNodePtr sequence_node = NULL;
        int element_count = 0;

        /* Get complexType name */
        type_name = xmlGetProp(complex_type_node, BAD_CAST "name");
        if (!type_name) {
            continue;
        }

        /* Allocate and initialize complex type structure */
        complex_type = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_complex_type_t));
        memset(complex_type, 0, sizeof(wsdl2c_complex_type_t));

        complex_type->name = axutil_strdup(env, (const axis2_char_t*)type_name);
        complex_type->c_name = wsdl2c_sanitize_c_identifier(env, (const axis2_char_t*)type_name);
        complex_type->elements = axutil_array_list_create(env, 0);
        complex_type->is_empty_sequence = AXIS2_FALSE;
        complex_type->has_sequence = AXIS2_FALSE;

        /* Look for sequence element within the complexType */
        for (child_node = complex_type_node->children; child_node; child_node = child_node->next) {
            if (child_node->type != XML_ELEMENT_NODE) {
                continue;
            }

            /* Check for direct sequence child */
            if (xmlStrcmp(child_node->name, BAD_CAST "sequence") == 0) {
                sequence_node = child_node;
                complex_type->has_sequence = AXIS2_TRUE;
                break;
            }

            /* Check for sequence inside complexContent/restriction or complexContent/extension */
            if (xmlStrcmp(child_node->name, BAD_CAST "complexContent") == 0 ||
                xmlStrcmp(child_node->name, BAD_CAST "simpleContent") == 0) {
                xmlNodePtr content_child = NULL;
                for (content_child = child_node->children; content_child; content_child = content_child->next) {
                    if (content_child->type != XML_ELEMENT_NODE) {
                        continue;
                    }
                    if (xmlStrcmp(content_child->name, BAD_CAST "restriction") == 0 ||
                        xmlStrcmp(content_child->name, BAD_CAST "extension") == 0) {
                        /* Get base type */
                        xmlChar *base = xmlGetProp(content_child, BAD_CAST "base");
                        if (base) {
                            complex_type->base_type = axutil_strdup(env, (const axis2_char_t*)base);
                            xmlFree(base);
                        }
                        /* Look for sequence inside */
                        xmlNodePtr inner_child = NULL;
                        for (inner_child = content_child->children; inner_child; inner_child = inner_child->next) {
                            if (inner_child->type == XML_ELEMENT_NODE &&
                                xmlStrcmp(inner_child->name, BAD_CAST "sequence") == 0) {
                                sequence_node = inner_child;
                                complex_type->has_sequence = AXIS2_TRUE;
                                break;
                            }
                        }
                    }
                    if (sequence_node) break;
                }
            }
            if (sequence_node) break;
        }

        /* Parse elements within the sequence */
        if (sequence_node) {
            xmlNodePtr elem_node = NULL;
            for (elem_node = sequence_node->children; elem_node; elem_node = elem_node->next) {
                if (elem_node->type == XML_ELEMENT_NODE &&
                    xmlStrcmp(elem_node->name, BAD_CAST "element") == 0) {

                    wsdl2c_schema_element_t *element = NULL;
                    xmlChar *elem_name = NULL;
                    xmlChar *elem_type = NULL;
                    xmlChar *nillable = NULL;
                    xmlChar *min_occurs = NULL;
                    xmlChar *max_occurs = NULL;

                    elem_name = xmlGetProp(elem_node, BAD_CAST "name");
                    if (!elem_name) {
                        continue;
                    }

                    element = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_schema_element_t));
                    memset(element, 0, sizeof(wsdl2c_schema_element_t));

                    element->name = axutil_strdup(env, (const axis2_char_t*)elem_name);
                    element->c_name = wsdl2c_sanitize_c_identifier(env, (const axis2_char_t*)elem_name);
                    element->is_any_type = AXIS2_FALSE;
                    element->is_typeless = AXIS2_FALSE;

                    elem_type = xmlGetProp(elem_node, BAD_CAST "type");
                    if (elem_type) {
                        element->type = axutil_strdup(env, (const axis2_char_t*)elem_type);
                        element->c_type = wsdl2c_sanitize_c_identifier(env, (const axis2_char_t*)elem_type);
                        xmlFree(elem_type);
                    } else {
                        /* AXIS2C-1421: Element has no type attribute - treat as anyType */
                        element->is_typeless = AXIS2_TRUE;
                        element->type = axutil_strdup(env, "anyType");
                        element->c_type = axutil_strdup(env, "axiom_node_t");
                        context->wsdl->has_typeless_elements = AXIS2_TRUE;
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "AXIS2C-1421: Element '%s' has no type - treating as anyType",
                            element->name);
                    }

                    nillable = xmlGetProp(elem_node, BAD_CAST "nillable");
                    element->is_nillable = (nillable && xmlStrcmp(nillable, BAD_CAST "true") == 0);
                    if (nillable) xmlFree(nillable);

                    min_occurs = xmlGetProp(elem_node, BAD_CAST "minOccurs");
                    element->min_occurs = min_occurs ? atoi((const char*)min_occurs) : 1;
                    if (min_occurs) xmlFree(min_occurs);

                    max_occurs = xmlGetProp(elem_node, BAD_CAST "maxOccurs");
                    if (max_occurs) {
                        if (xmlStrcmp(max_occurs, BAD_CAST "unbounded") == 0) {
                            element->max_occurs = -1;
                        } else {
                            element->max_occurs = atoi((const char*)max_occurs);
                        }
                        xmlFree(max_occurs);
                    } else {
                        element->max_occurs = 1;
                    }

                    axutil_array_list_add(complex_type->elements, env, element);
                    element_count++;

                    xmlFree(elem_name);
                }
            }

            /* AXIS2C-1224: Check for empty sequence */
            if (element_count == 0) {
                complex_type->is_empty_sequence = AXIS2_TRUE;
                context->wsdl->has_empty_sequences = AXIS2_TRUE;
                AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                    "AXIS2C-1224: ComplexType '%s' has empty sequence", complex_type->name);
            }
        }

        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
            "Parsed complexType '%s': %d elements, empty_sequence=%d, has_sequence=%d",
            complex_type->name, element_count,
            complex_type->is_empty_sequence, complex_type->has_sequence);

        axutil_array_list_add(context->wsdl->complex_types, env, complex_type);
        xmlFree(type_name);
    }

    xmlXPathFreeObject(complex_types_result);

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
        "AXIS2C-1224: Parsed %d complexTypes, %d have empty sequences",
        axutil_array_list_size(context->wsdl->complex_types, env),
        context->wsdl->has_empty_sequences ? 1 : 0);

    return AXIS2_SUCCESS;
}

/* Parse schema elements to detect xsd:any types (AXIS2C-1580 fix) */
static axis2_status_t
parse_wsdl_schema(wsdl2c_context_t *context, xmlXPathContextPtr xpath_ctx, const axutil_env_t *env)
{
    xmlXPathObjectPtr schema_result = NULL;
    xmlNodeSetPtr element_nodes = NULL;
    int i;

    AXIS2_PARAM_CHECK(env->error, context, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, xpath_ctx, AXIS2_FAILURE);

    /* Initialize schema elements list */
    context->wsdl->schema_elements = axutil_array_list_create(env, 0);
    context->wsdl->has_any_type = AXIS2_FALSE;

    /* Find all xsd:any elements - these have no qname and can cause crashes if not handled */
    schema_result = xmlXPathEvalExpression(
        BAD_CAST "//xsd:any | //xs:any", xpath_ctx);

    if (schema_result && schema_result->nodesetval && schema_result->nodesetval->nodeNr > 0) {
        element_nodes = schema_result->nodesetval;
        context->wsdl->has_any_type = AXIS2_TRUE;

        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
            "AXIS2C-1580: Found %d xsd:any elements - generating safe deserialization code",
            element_nodes->nodeNr);

        for (i = 0; i < element_nodes->nodeNr; i++) {
            xmlNodePtr any_node = element_nodes->nodeTab[i];
            wsdl2c_schema_element_t *element = NULL;
            xmlChar *min_occurs = NULL;
            xmlChar *max_occurs = NULL;
            xmlChar *namespace_attr = NULL;
            xmlChar *process_contents = NULL;

            element = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_schema_element_t));
            memset(element, 0, sizeof(wsdl2c_schema_element_t));

            element->is_any_type = AXIS2_TRUE;
            element->name = axutil_strdup(env, "any"); /* xsd:any has no name */

            /* Get minOccurs (default is 1) */
            min_occurs = xmlGetProp(any_node, BAD_CAST "minOccurs");
            element->min_occurs = min_occurs ? atoi((const char*)min_occurs) : 1;
            if (min_occurs) xmlFree(min_occurs);

            /* Get maxOccurs (default is 1, "unbounded" = -1) */
            max_occurs = xmlGetProp(any_node, BAD_CAST "maxOccurs");
            if (max_occurs) {
                if (xmlStrcmp(max_occurs, BAD_CAST "unbounded") == 0) {
                    element->max_occurs = -1;
                } else {
                    element->max_occurs = atoi((const char*)max_occurs);
                }
                xmlFree(max_occurs);
            } else {
                element->max_occurs = 1;
            }

            /* Get namespace attribute */
            namespace_attr = xmlGetProp(any_node, BAD_CAST "namespace");
            if (namespace_attr) {
                element->namespace_uri = axutil_strdup(env, (const char*)namespace_attr);
                xmlFree(namespace_attr);
            }

            /* Get processContents attribute for logging */
            process_contents = xmlGetProp(any_node, BAD_CAST "processContents");
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                "  xsd:any element: namespace='%s', minOccurs=%d, maxOccurs=%d, processContents='%s'",
                element->namespace_uri ? element->namespace_uri : "##any",
                element->min_occurs, element->max_occurs,
                process_contents ? (const char*)process_contents : "strict");
            if (process_contents) xmlFree(process_contents);

            axutil_array_list_add(context->wsdl->schema_elements, env, element);
        }
    }

    if (schema_result) {
        xmlXPathFreeObject(schema_result);
    }

    /* Also look for regular elements to understand the schema structure */
    schema_result = xmlXPathEvalExpression(
        BAD_CAST "//xsd:element[@name] | //xs:element[@name]", xpath_ctx);

    if (schema_result && schema_result->nodesetval) {
        element_nodes = schema_result->nodesetval;
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
            "Found %d named schema elements", element_nodes->nodeNr);

        for (i = 0; i < element_nodes->nodeNr; i++) {
            xmlNodePtr elem_node = element_nodes->nodeTab[i];
            wsdl2c_schema_element_t *element = NULL;
            xmlChar *name = NULL;
            xmlChar *type = NULL;
            xmlChar *nillable = NULL;

            name = xmlGetProp(elem_node, BAD_CAST "name");
            if (!name) continue;

            element = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_schema_element_t));
            memset(element, 0, sizeof(wsdl2c_schema_element_t));

            element->name = axutil_strdup(env, (const char*)name);
            element->is_any_type = AXIS2_FALSE;

            type = xmlGetProp(elem_node, BAD_CAST "type");
            if (type) {
                element->type = axutil_strdup(env, (const char*)type);
                xmlFree(type);
            }

            nillable = xmlGetProp(elem_node, BAD_CAST "nillable");
            element->is_nillable = (nillable && xmlStrcmp(nillable, BAD_CAST "true") == 0);
            if (nillable) xmlFree(nillable);

            xmlFree(name);
            axutil_array_list_add(context->wsdl->schema_elements, env, element);
        }
    }

    if (schema_result) {
        xmlXPathFreeObject(schema_result);
    }

    return AXIS2_SUCCESS;
}

/* Main WSDL parsing function */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_parse_wsdl(wsdl2c_context_t *context, const axutil_env_t *env)
{
    xmlXPathContextPtr xpath_ctx = NULL;
    xmlXPathObjectPtr result = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_PARAM_CHECK(env->error, context, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, context->options, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, context->options->wsdl_uri, AXIS2_FAILURE);

    /* Parse the WSDL document */
    context->doc = xmlParseFile(context->options->wsdl_uri);
    if (!context->doc) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse WSDL file: %s",
                       context->options->wsdl_uri);
        return AXIS2_FAILURE;
    }

    /* Create XPath context */
    xpath_ctx = xmlXPathNewContext(context->doc);
    if (!xpath_ctx) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to create XPath context");
        goto cleanup;
    }

    /* Register namespaces */
    register_namespaces(xpath_ctx);

    /* Initialize WSDL structure */
    context->wsdl = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_wsdl_t));
    memset(context->wsdl, 0, sizeof(wsdl2c_wsdl_t));

    /* Get target namespace */
    result = xmlXPathEvalExpression(BAD_CAST "/wsdl:definitions/@targetNamespace", xpath_ctx);
    if (result && result->nodesetval && result->nodesetval->nodeNr > 0) {
        xmlNodePtr attr_node = result->nodesetval->nodeTab[0];
        if (attr_node && attr_node->children && attr_node->children->content) {
            context->wsdl->target_namespace = axutil_strdup(env,
                (const axis2_char_t*)attr_node->children->content);
        }
    }
    if (result) {
        xmlXPathFreeObject(result);
        result = NULL;
    }

    /* Parse messages */
    status = parse_wsdl_messages(context, xpath_ctx, env);
    if (status != AXIS2_SUCCESS) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse WSDL messages");
        goto cleanup;
    }

    /* Parse operations */
    status = parse_wsdl_operations(context, xpath_ctx, env);
    if (status != AXIS2_SUCCESS) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse WSDL operations");
        goto cleanup;
    }

    /* Parse bindings to extract soapAction (AXIS2C-1581) */
    status = parse_wsdl_bindings(context, xpath_ctx, env);
    if (status != AXIS2_SUCCESS) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse WSDL bindings");
        goto cleanup;
    }

    /* Parse schema to detect xsd:any types (AXIS2C-1580) */
    status = parse_wsdl_schema(context, xpath_ctx, env);
    if (status != AXIS2_SUCCESS) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse WSDL schema");
        goto cleanup;
    }

    /* Parse complexTypes including empty sequences (AXIS2C-1224) */
    status = parse_wsdl_complex_types(context, xpath_ctx, env);
    if (status != AXIS2_SUCCESS) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse WSDL complexTypes");
        goto cleanup;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Successfully parsed WSDL: %s",
                   context->options->wsdl_uri);

cleanup:
    if (xpath_ctx) {
        xmlXPathFreeContext(xpath_ctx);
    }
    return status;
}