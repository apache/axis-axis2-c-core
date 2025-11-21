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

/* Operation structure */
typedef struct wsdl2c_operation {
    axis2_char_t *name;
    axis2_char_t *input_message;
    axis2_char_t *output_message;
    axis2_char_t *soap_action;
} wsdl2c_operation_t;

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

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Successfully parsed WSDL: %s",
                   context->options->wsdl_uri);

cleanup:
    if (xpath_ctx) {
        xmlXPathFreeContext(xpath_ctx);
    }
    return status;
}