/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 */

/**
 * @file bigdata_h2_bridge.c
 * @brief Minimal SOAP-to-JSON bridge for BigDataH2Service
 *
 * This bridge allows Axis2/C framework to invoke our pure JSON handlers
 * while maintaining all HTTP/2 JSON performance benefits.
 *
 * Architecture: SOAP Envelope → JSON Extraction → Pure JSON Processing → Wrapped Response
 */

#include <axis2_svc_skeleton.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axiom_text.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_children_iterator.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_body.h>
#include "bigdata_h2_service_handler.h"

typedef struct bigdata_h2_svc_skeleton
{
    axis2_svc_skeleton_t svc_skeleton;
} bigdata_h2_svc_skeleton_t;

#define AXIS2_INTF_TO_IMPL(svc_skeleton) ((bigdata_h2_svc_skeleton_t *)(svc_skeleton))

/* Forward declarations */
axiom_node_t* AXIS2_CALL bigdata_h2_svc_skeleton_invoke(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env,
    axiom_node_t *node,
    axis2_msg_ctx_t *msg_ctx);

int AXIS2_CALL bigdata_h2_svc_skeleton_init(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env);

axiom_node_t* AXIS2_CALL bigdata_h2_svc_skeleton_on_fault(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env,
    axiom_node_t *node);

void AXIS2_CALL bigdata_h2_svc_skeleton_free(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env);

/**
 * Extract JSON content from AXIOM node (minimal processing)
 */
static axis2_char_t* extract_json_from_axiom(const axutil_env_t *env, axiom_node_t *node)
{
    axiom_element_t *element = NULL;
    axiom_text_t *text = NULL;
    axiom_node_t *first_child = NULL;
    axis2_char_t *text_value = NULL;

    if (!node || !env)
    {
        return axutil_strdup(env, "{}");
    }

    /* Try to get text content directly */
    if (axiom_node_get_node_type(node, env) == AXIOM_TEXT)
    {
        text = (axiom_text_t *)axiom_node_get_data_element(node, env);
        if (text)
        {
            text_value = axiom_text_get_value(text, env);
            if (text_value)
            {
                return axutil_strdup(env, text_value);
            }
        }
    }

    /* Try to get text from element */
    if (axiom_node_get_node_type(node, env) == AXIOM_ELEMENT)
    {
        element = (axiom_element_t *)axiom_node_get_data_element(node, env);
        if (element)
        {
            text_value = axiom_element_get_text(element, env, node);
            if (text_value && strlen(text_value) > 0)
            {
                return axutil_strdup(env, text_value);
            }

            /* Try first child if no direct text */
            first_child = axiom_node_get_first_child(node, env);
            if (first_child)
            {
                return extract_json_from_axiom(env, first_child);
            }
        }
    }

    /* Default empty JSON object */
    return axutil_strdup(env, "{}");
}

/**
 * Create AXIOM response node with JSON content (minimal wrapping)
 */
static axiom_node_t* create_json_response_node(const axutil_env_t *env, const axis2_char_t *json_response)
{
    axiom_element_t *response_element = NULL;
    axiom_node_t *response_node = NULL;
    axiom_text_t *text_node = NULL;
    axiom_node_t *text_node_axiom = NULL;
    axiom_namespace_t *ns = NULL;

    /* Create response element */
    ns = axiom_namespace_create(env, "http://bigdata.h2.axis2c.apache.org", "ns");
    response_element = axiom_element_create(env, NULL, "jsonResponse", ns, &response_node);

    if (response_element && json_response)
    {
        text_node = axiom_text_create(env, response_node, json_response, &text_node_axiom);
    }

    return response_node;
}

/**
 * Main service invocation - Bridge SOAP to JSON
 */
axiom_node_t* AXIS2_CALL bigdata_h2_svc_skeleton_invoke(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env,
    axiom_node_t *node,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_char_t *json_request = NULL;
    axis2_char_t *json_response = NULL;
    axiom_node_t *response_node = NULL;
    axis2_char_t *operation_name = NULL;

    AXIS2_LOG_INFO(env->log, "[BigDataH2Bridge] Service invoked");

    /* Get operation name from message context */
    if (msg_ctx)
    {
        axis2_op_ctx_t *op_ctx = axis2_msg_ctx_get_op_ctx(msg_ctx, env);
        if (op_ctx)
        {
            axis2_op_t *op = axis2_op_ctx_get_op(op_ctx, env);
            if (op)
            {
                axutil_qname_t *qname = axis2_op_get_qname(op, env);
                if (qname)
                {
                    operation_name = axutil_qname_get_localpart(qname, env);
                }
            }
        }
    }

    /* Extract JSON from AXIOM (minimal processing) */
    json_request = extract_json_from_axiom(env, node);

    AXIS2_LOG_INFO(env->log, "[BigDataH2Bridge] Operation: %s, JSON: %s",
                    operation_name ? operation_name : "unknown",
                    json_request ? json_request : "null");

    /* Route to appropriate JSON handler based on operation */
    if (operation_name && axutil_strcmp(operation_name, "processBigDataSet") == 0)
    {
        json_response = bigdata_h2_process_big_data_set_json(env, json_request);
    }
    else if (operation_name && axutil_strcmp(operation_name, "getServiceMetadata") == 0)
    {
        json_response = bigdata_h2_get_service_metadata_json(env, json_request);
    }
    else
    {
        /* Default response for unknown operations */
        json_response = axutil_strdup(env, "{\"error\": \"Unknown operation\", \"availableOperations\": [\"processBigDataSet\", \"getServiceMetadata\"]}");
    }

    /* Wrap JSON response in AXIOM (minimal wrapping) */
    if (json_response)
    {
        AXIS2_LOG_INFO(env->log, "[BigDataH2Bridge] Response JSON: %s", json_response);
        response_node = create_json_response_node(env, json_response);
        AXIS2_FREE(env->allocator, json_response);
    }
    else
    {
        AXIS2_LOG_ERROR(env->log, "[BigDataH2Bridge] No response from JSON handler");
        response_node = create_json_response_node(env, "{\"error\": \"Service processing failed\"}");
    }

    /* Cleanup */
    if (json_request)
        AXIS2_FREE(env->allocator, json_request);

    return response_node;
}

/**
 * Initialize service
 */
int AXIS2_CALL bigdata_h2_svc_skeleton_init(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env)
{
    AXIS2_LOG_INFO(env->log, "[BigDataH2Bridge] Service initialized");
    return AXIS2_SUCCESS;
}

/**
 * Handle faults
 */
axiom_node_t* AXIS2_CALL bigdata_h2_svc_skeleton_on_fault(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env,
    axiom_node_t *node)
{
    AXIS2_LOG_ERROR(env->log, "[BigDataH2Bridge] Fault occurred");
    return create_json_response_node(env, "{\"error\": \"Service fault occurred\", \"type\": \"SOAP_FAULT\"}");
}

/**
 * Free service skeleton
 */
void AXIS2_CALL bigdata_h2_svc_skeleton_free(
    axis2_svc_skeleton_t *svc_skeleton,
    const axutil_env_t *env)
{
    if (svc_skeleton)
    {
        if (svc_skeleton->ops)
        {
            AXIS2_FREE(env->allocator, svc_skeleton->ops);
        }
        AXIS2_FREE(env->allocator, svc_skeleton);
    }
}

/**
 * Service factory function - Entry point for Axis2/C
 */
AXIS2_EXTERN axis2_svc_skeleton_t* AXIS2_CALL
axis2_bigdata_h2_create(const axutil_env_t *env)
{
    bigdata_h2_svc_skeleton_t *svc_skeleton_impl = NULL;
    axis2_svc_skeleton_t *svc_skeleton = NULL;

    AXIS2_LOG_INFO(env->log, "[BigDataH2Bridge] Creating service skeleton");

    svc_skeleton_impl = AXIS2_MALLOC(env->allocator, sizeof(bigdata_h2_svc_skeleton_t));
    if (!svc_skeleton_impl)
    {
        AXIS2_LOG_ERROR(env->log, "[BigDataH2Bridge] Memory allocation failed");
        return NULL;
    }
    memset(svc_skeleton_impl, 0, sizeof(bigdata_h2_svc_skeleton_t));

    svc_skeleton = &(svc_skeleton_impl->svc_skeleton);
    svc_skeleton->ops = AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_skeleton_ops_t));
    if (!svc_skeleton->ops)
    {
        AXIS2_LOG_ERROR(env->log, "[BigDataH2Bridge] Ops allocation failed");
        AXIS2_FREE(env->allocator, svc_skeleton_impl);
        return NULL;
    }

    svc_skeleton->ops->init = bigdata_h2_svc_skeleton_init;
    svc_skeleton->ops->invoke = bigdata_h2_svc_skeleton_invoke;
    svc_skeleton->ops->on_fault = bigdata_h2_svc_skeleton_on_fault;
    svc_skeleton->ops->free = bigdata_h2_svc_skeleton_free;

    AXIS2_LOG_INFO(env->log, "[BigDataH2Bridge] Service skeleton created successfully");
    return svc_skeleton;
}

/**
 * Service removal function
 */
AXIS2_EXTERN int AXIS2_CALL
axis2_remove_bigdata_h2(const axutil_env_t *env)
{
    AXIS2_LOG_INFO(env->log, "[BigDataH2Bridge] Service removed");
    return AXIS2_SUCCESS;
}