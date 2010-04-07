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

#include <axiom_soap_builder.h>
#include "_axiom_soap_envelope.h"
#include "_axiom_soap_header.h"
#include "axiom_soap11_builder_helper.h"
#include "axiom_soap12_builder_helper.h"
#include <axiom_soap_const.h>
#include "_axiom_soap_body.h"
#include "_axiom_soap_header_block.h"
#include <axiom_stax_builder_internal.h>
#include "axiom_soap_builder_internal.h"
#include "_axiom_soap_fault.h"
#include <axutil_http_chunked_stream.h>

static axis2_status_t
axiom_soap_builder_identify_soap_version(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env,
    const axis2_char_t * soap_version_uri_from_transport);

static axis2_status_t
axiom_soap_builder_parse_headers(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env);

static axis2_status_t
axiom_soap_builder_process_namespace_data(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env,
    axiom_node_t * om_node);

static axis2_status_t
axiom_soap_builder_construct_soap_header(
    axiom_soap_builder_t *soap_builder,
    const axutil_env_t *env,
    axiom_node_t *om_node);

static axis2_status_t
axiom_soap_builder_construct_soap_body(
    axiom_soap_builder_t *soap_builder,
    const axutil_env_t *env,
    axiom_node_t *om_node);

static axis2_status_t
axiom_soap_builder_construct_soap_fault(
    axiom_soap_builder_t *soap_builder,
    const axutil_env_t *env,
    axiom_node_t *om_node);

static axis2_status_t
axiom_soap_builder_construct_xop_include(
    axiom_soap_builder_t *soap_builder,
    const axutil_env_t *env,
    axiom_node_t *om_element_node);

struct axiom_soap_builder
{
    axiom_stax_builder_t *om_builder;

    axiom_soap_envelope_t *soap_envelope;

    axis2_bool_t header_present;

    axis2_bool_t body_present;

    int element_level;

    axis2_bool_t processing_fault;

    axis2_bool_t processing_detail_elements;

    axis2_char_t *sender_fault_code;

    axis2_char_t *receiver_fault_code;

    axis2_bool_t processing_mandatory_fault_elements;

    void *builder_helper;

    axiom_namespace_t *envelope_ns;

    int soap_version;

    int last_node_status;

    axis2_bool_t done;

    axutil_hash_t *mime_body_parts;

    axiom_mime_parser_t *mime_parser;

    AXIS2_READ_INPUT_CALLBACK callback;

    void *callback_ctx;

};

typedef enum axis2_builder_last_node_states
{
    AXIS2_BUILDER_LAST_NODE_NULL = 0,
    AXIS2_BUILDER_LAST_NODE_NOT_NULL
} axis2_builder_last_node_states;

#define AXIS2_MAX_EVENT 100

AXIS2_EXTERN axiom_soap_builder_t *AXIS2_CALL
axiom_soap_builder_create(
    const axutil_env_t * env,
    axiom_stax_builder_t * stax_builder,
    const axis2_char_t * soap_version)
{
    axiom_soap_builder_t *soap_builder = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    if(!stax_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "STAX builder is invalid.");
        return NULL;
    }

    soap_builder = (axiom_soap_builder_t*)AXIS2_MALLOC(env->allocator, sizeof(axiom_soap_builder_t));
    if(!soap_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Insufficient memory to create soap builder");
        return NULL;
    }
    soap_builder = (axiom_soap_builder_t*)memset(soap_builder, 0, sizeof(axiom_soap_builder_t));
    soap_builder->soap_version = AXIOM_SOAP12;
    soap_builder->last_node_status = -1;
    soap_builder->om_builder = stax_builder;
    axiom_stax_builder_set_soap_builder(stax_builder, env, soap_builder);
    soap_builder->done = AXIS2_FALSE;

    status = axiom_soap_builder_identify_soap_version(soap_builder, env, soap_version);
    if(status != AXIS2_SUCCESS)
    {
        axiom_soap_builder_free(soap_builder, env);
        return NULL;
    }
    status = axiom_soap_builder_parse_headers(soap_builder, env);
    if(status != AXIS2_SUCCESS)
    {
        axiom_soap_builder_free(soap_builder, env);
        return NULL;
    }
    return soap_builder;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_soap_builder_free(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env)
{
    if(!soap_builder)
    {
        return;
    }
    if(soap_builder->builder_helper)
    {
        if(soap_builder->soap_version == AXIOM_SOAP11 && soap_builder->builder_helper)
        {
            axiom_soap11_builder_helper_free(
                (axiom_soap11_builder_helper_t *)(soap_builder->builder_helper), env);
            soap_builder->builder_helper = NULL;
        }
        else if(soap_builder->soap_version == AXIOM_SOAP12 && soap_builder->builder_helper)
        {
            axiom_soap12_builder_helper_free(
                (axiom_soap12_builder_helper_t *)(soap_builder->builder_helper), env);
            soap_builder->builder_helper = NULL;
        }
    }

    if(soap_builder->om_builder)
    {
        axiom_stax_builder_free(soap_builder->om_builder, env);
        soap_builder->om_builder = NULL;
    }

    if(soap_builder->mime_body_parts)
    {
        axutil_hash_index_t *hi = NULL;
        void *val = NULL;
        const void *key = NULL;
        for(hi = axutil_hash_first(soap_builder->mime_body_parts, env); hi; hi = axutil_hash_next(
            env, hi))
        {
            axutil_hash_this(hi, &key, NULL, &val);

            if(key)
            {
                AXIS2_FREE(env->allocator, (char *)key);
            }

            val = NULL;
            key = NULL;
        }

        axutil_hash_free(soap_builder->mime_body_parts, env);
        soap_builder->mime_body_parts = NULL;
    }

    if(soap_builder->mime_parser)
    {
        axiom_mime_parser_free(soap_builder->mime_parser, env);
        soap_builder->mime_parser = NULL;
    }

    if(soap_builder->callback_ctx)
    {
        axis2_callback_info_t *callback_info = NULL;

        callback_info = (axis2_callback_info_t *)(soap_builder->callback_ctx);
        if(callback_info)
        {
            if(callback_info->chunked_stream)
            {
                axutil_http_chunked_stream_free(callback_info->chunked_stream, env);
                callback_info->chunked_stream = NULL;
            }

            AXIS2_FREE(env->allocator, callback_info);
            callback_info = NULL;
            soap_builder->callback_ctx = NULL;
        }
    }

    if(soap_builder)
    {
        AXIS2_FREE(env->allocator, soap_builder);
        soap_builder = NULL;
    }

    return;
}

AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
axiom_soap_builder_get_soap_envelope(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env)
{
    int status = AXIS2_SUCCESS;

    if(!soap_builder)
    {
        return NULL;
    }
    if(!(soap_builder->om_builder))
    {
        return NULL;
    }
    while(!(soap_builder->soap_envelope) && !axiom_stax_builder_is_complete(
        soap_builder->om_builder, env))
    {
        status = axiom_soap_builder_next(soap_builder, env);
        if(status == AXIS2_FAILURE)
        {
            break;
        }
    }

    return soap_builder->soap_envelope;
}

AXIS2_EXTERN axiom_document_t *AXIS2_CALL
axiom_soap_builder_get_document(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env)
{
    if (!soap_builder)
    {
        return NULL;
    }
    if (soap_builder->om_builder)
    {
        return axiom_stax_builder_get_document(soap_builder->om_builder, env);
    }
    else
    return NULL;
}

/**
 * Builds next element of the OM structure
 * @param builder pointer to the SOAP Builder struct
 * @param env Environment. MUST NOT be NULL
 * @return AXIS2_SUCCESS if the next element is present else AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_builder_next(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env)
{
    int current_event = AXIS2_MAX_EVENT;
    int status = AXIS2_SUCCESS;

    AXIS2_ASSERT(soap_builder != NULL);
    AXIS2_ASSERT(soap_builder->om_builder != NULL);

    if(soap_builder->done)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Invalid soap builder");
        return AXIS2_FAILURE;
    }

    current_event = axiom_stax_builder_next_with_token(soap_builder->om_builder, env);
    if(current_event == -1)
    {
        /* there is an error. So, don't continue building it */
        soap_builder->done = AXIS2_TRUE;
        status = AXIS2_FAILURE;
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error occurred when building node");
    }

    /* for any other event types, return AXIS2_SUCCESS as the return value. */

    return status;
}

AXIS2_EXTERN axiom_node_t *AXIS2_CALL
axiom_soap_builder_get_document_element(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env)
{
    axiom_node_t *document_node = NULL;
    if(soap_builder->soap_envelope)
    {
        document_node = axiom_soap_envelope_get_base_node(soap_builder->soap_envelope, env);
    }

    return document_node;
}

axis2_status_t AXIS2_CALL
axiom_soap_builder_construct_node(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env,
    axiom_node_t * om_element_node)
{
    axiom_element_t *om_element = NULL;
    const axis2_char_t *ele_localname = NULL;
    int element_level = 0;
    int status = AXIS2_SUCCESS;
    axiom_node_t *parent = NULL;

    /* Check whether current node is the very first element, which is SOAP Envelope.
     * If last_node_status is AXIS2_BUILDER_LAST_NODE_NULL, then it means next node is SOAP Envelope
     */
    if(om_element_node == axiom_stax_builder_get_root_node(soap_builder->om_builder, env))
    {
        soap_builder->last_node_status = AXIS2_BUILDER_LAST_NODE_NULL;
    }
    else
    {
        soap_builder->last_node_status = AXIS2_BUILDER_LAST_NODE_NOT_NULL;
    }

    /* get OM element struct from node */
    om_element = (axiom_element_t *)axiom_node_get_data_element(om_element_node, env);
    if(!om_element)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot construct node. Invalid element");
        return AXIS2_FAILURE;
    }

    /* get element local name */
    ele_localname = axiom_element_get_localname(om_element, env);
    if(!ele_localname)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot construct node. Invalid element name");
        return AXIS2_FAILURE;
    }

    parent = axiom_node_get_parent(om_element_node, env);
    if(!parent)
    {
        /* parent node doesn't exist means, this should be SOAP Envelope */
        if(soap_builder->last_node_status != AXIS2_BUILDER_LAST_NODE_NULL)
        {
            /* this means, there are two elements in the first level */
            AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_SOAP_MESSAGE_FIRST_ELEMENT_MUST_CONTAIN_LOCAL_NAME, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Soap message can contain only one SOAP Envelope as the top level element");
            return AXIS2_FAILURE;
        }

        if(axutil_strcasecmp(ele_localname, AXIOM_SOAP_ENVELOPE_LOCAL_NAME) != 0)
        {
            AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_SOAP_MESSAGE_FIRST_ELEMENT_MUST_CONTAIN_LOCAL_NAME, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "SOAP message first element must be Envelope");
            return AXIS2_FAILURE;
        }

        /** create a null soap envelope struct */
        soap_builder->soap_envelope = axiom_soap_envelope_create_null(env);
        if(!soap_builder->soap_envelope)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create SOAP Envelope");
            return AXIS2_FAILURE;
        }

        /** wrap this OM node in it */
        axiom_soap_envelope_set_base_node(soap_builder->soap_envelope, env, om_element_node);
        axiom_soap_envelope_set_builder(soap_builder->soap_envelope, env, soap_builder);
        status = axiom_soap_builder_process_namespace_data(soap_builder, env, om_element_node);
        if(status != AXIS2_SUCCESS)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "SOAP Envelope is having invalid namespace");
        }

        return status;
    }

    /** a parent node exist , so not soap envelope. Can be either header/body/children of them */

    /* get element level of this OM element. Envelope will be in element_level 1. Body and Header
     * will be in element level 2. SOAP fault will be child of Body, so it will be in level 3*/
    element_level = axiom_stax_builder_get_element_level(soap_builder->om_builder, env);
    if(axiom_stax_builder_get_current_event(soap_builder->om_builder, env)
        == AXIOM_XML_READER_EMPTY_ELEMENT)
    {
        /* if it is an empty element, increase the element level. This is because the element level
         * is incremented by STAX builder only if <element> tag is identified. It will not be
         * incremented if <element/> tag is seen. But for our logic, even empty element should be
         * considered as full element. so, <element/> = <element> </element>. */
        ++element_level;
    }

    if(element_level == 2)
    {
        /* At this level, we can only have soap:Header or soap:Body */
        status = axiom_soap_builder_process_namespace_data(soap_builder, env, om_element_node);
        if(status != AXIS2_SUCCESS)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "invalid SOAP namespace");
            return AXIS2_FAILURE;
        }

        if(axutil_strcmp(ele_localname, AXIOM_SOAP_HEADER_LOCAL_NAME) == 0)
        {
            /* this is the soap header element */
            status = axiom_soap_builder_construct_soap_header(soap_builder, env, om_element_node);
        }
        else if(axutil_strcmp(ele_localname, AXIOM_SOAP_BODY_LOCAL_NAME) == 0)
        {
            /* This is the SOAP Body element */
            status = axiom_soap_builder_construct_soap_body(soap_builder, env, om_element_node);
        }
        else
        {
            AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "SOAP builder found a child element other than header or body in envelope element");
            return AXIS2_FAILURE;
        }
    }
    else if(element_level == 3)
    {
        axiom_element_t *parent_ele = NULL;
        axis2_char_t *parent_localname = NULL;

        parent_ele = (axiom_element_t *)axiom_node_get_data_element(parent, env);
        if(parent_ele)
        {
            parent_localname = axiom_element_get_localname(parent_ele, env);
        }

        if(!parent_localname)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot identify parent element local name");
            return AXIS2_FAILURE;
        }

        if(axutil_strcasecmp(parent_localname, AXIOM_SOAP_HEADER_LOCAL_NAME) == 0)
        {
            axiom_soap_header_block_t *header_block = NULL;
            axiom_soap_header_t *soap_header = NULL;

            soap_header = axiom_soap_envelope_get_header(soap_builder->soap_envelope, env);
            if(!soap_header)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot get SOAP Header from OM Envelope");
                return AXIS2_FAILURE;
            }

            header_block = axiom_soap_header_block_create(env);
            if(!header_block)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create SOAP Header block");
                return AXIS2_FAILURE;
            }

            axiom_soap_header_block_set_base_node(header_block, env, om_element_node);
            axiom_soap_header_set_header_block(soap_header, env, header_block);
            axiom_soap_header_block_set_soap_version(header_block, env, soap_builder->soap_version);
        }
        else if(axutil_strcasecmp(parent_localname, AXIOM_SOAP_BODY_LOCAL_NAME) == 0)
        {
            /* if the node is <xop:Include> or MTOM message */
            if(axutil_strcmp(ele_localname, AXIS2_XOP_INCLUDE) == 0)
            {
                return axiom_soap_builder_construct_xop_include(soap_builder, env, om_element_node);
            }

            if(axutil_strcasecmp(ele_localname, AXIOM_SOAP_BODY_FAULT_LOCAL_NAME) == 0)
            {
                return axiom_soap_builder_construct_soap_fault(soap_builder, env, om_element_node);
            }

            /* We don't need to process any other children of SOAP Body. */
        }
        else
        {
            AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "SOAP builder found a child element other than header or body in envelope element");
            return AXIS2_FAILURE;
        }

        return AXIS2_SUCCESS;
    }
    else if(element_level > 3)
    {
        if(soap_builder->processing_fault)
        {
            if(soap_builder->soap_version == AXIOM_SOAP11)
            {
                status = axiom_soap11_builder_helper_handle_event((axiom_soap11_builder_helper_t *)
                    soap_builder->builder_helper, env, om_element_node, element_level);
            }
            else if(soap_builder->soap_version == AXIOM_SOAP12)
            {
                status = axiom_soap12_builder_helper_handle_event((axiom_soap12_builder_helper_t *)
                    soap_builder->builder_helper, env, om_element_node, element_level);
            }
            else
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unknown SOAP Version");
                status = AXIS2_FAILURE;
            }
        }

        /* if the node is <xop:Include> or MTOM message */
        else if(axutil_strcmp(ele_localname, AXIS2_XOP_INCLUDE) == 0)
        {
            return axiom_soap_builder_construct_xop_include(soap_builder, env, om_element_node);
        }

        /* we don't need to consider any other elements at this level. */
    }
    return status;
}

static axis2_status_t
axiom_soap_builder_construct_soap_header(
    axiom_soap_builder_t *soap_builder,
    const axutil_env_t *env,
    axiom_node_t *om_node)
{
    axiom_soap_header_t *soap_header = NULL;
    if(soap_builder->header_present)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED,
            AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "SOAP builder encountered multiple headers");
        return AXIS2_FAILURE;
    }
    if(soap_builder->body_present)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER,
            AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "SOAP builder encountered body element first and header next");
        return AXIS2_FAILURE;
    }

    soap_builder->header_present = AXIS2_TRUE;
    soap_header = axiom_soap_header_create(env);
    if(!soap_header)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create SOAP Header");
        return AXIS2_FAILURE;
    }

    axiom_soap_header_set_base_node(soap_header, env, om_node);
    axiom_soap_envelope_set_header(soap_builder->soap_envelope, env, soap_header);
    axiom_soap_header_set_builder(soap_header, env, soap_builder);
    axiom_soap_header_set_soap_version(soap_header, env, soap_builder->soap_version);

    return AXIS2_SUCCESS;
}

static axis2_status_t
axiom_soap_builder_construct_soap_body(
    axiom_soap_builder_t *soap_builder,
    const axutil_env_t *env,
    axiom_node_t *om_node)
{
    axiom_soap_body_t *soap_body = NULL;
    if(soap_builder->body_present)
    {
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "SOAP builder multiple body elements encountered");
        return AXIS2_FAILURE;
    }

    soap_builder->body_present = AXIS2_TRUE;
    soap_body = axiom_soap_body_create(env);
    if(!soap_body)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create SOAP Body");
        return AXIS2_FAILURE;
    }

    axiom_soap_body_set_base_node(soap_body, env, om_node);
    axiom_soap_body_set_builder(soap_body, env, soap_builder);
    axiom_soap_envelope_set_body(soap_builder->soap_envelope, env, soap_body);

    return AXIS2_SUCCESS;
}

static axis2_status_t
axiom_soap_builder_construct_soap_fault(
    axiom_soap_builder_t *soap_builder,
    const axutil_env_t *env,
    axiom_node_t *om_node)
{
    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_fault_t *soap_fault = NULL;
    axiom_namespace_t *env_ns = NULL;

    env_ns = axiom_soap_envelope_get_namespace(soap_builder->soap_envelope, env);
    if(!env_ns)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot get soap envelope namespace");
        return AXIS2_FAILURE;
    }

    soap_body = axiom_soap_envelope_get_body(soap_builder->soap_envelope, env);
    if(!soap_body)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot get soap body from OM Envelope");
        return AXIS2_FAILURE;
    }

    soap_fault = axiom_soap_fault_create(env);
    if(!soap_fault)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create SOAP Fault structure");
        return AXIS2_FAILURE;
    }

    axiom_soap_fault_set_base_node(soap_fault, env, om_node);
    axiom_soap_body_set_fault(soap_body, env, soap_fault);
    axiom_soap_fault_set_builder(soap_fault, env, soap_builder);
    soap_builder->processing_fault = AXIS2_TRUE;
    soap_builder->processing_mandatory_fault_elements = AXIS2_TRUE;

    if(axutil_strcmp(AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI,
        axiom_namespace_get_uri(env_ns, env)) == 0)
    {
        soap_builder->builder_helper = axiom_soap12_builder_helper_create(env, soap_builder);
    }
    else if(axutil_strcmp(AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI,
        axiom_namespace_get_uri(env_ns, env)) == 0)
    {
        soap_builder->builder_helper = axiom_soap11_builder_helper_create(
            env, soap_builder,soap_builder->om_builder);
    }

    if(!soap_builder->builder_helper)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create soap builder helper");
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

static axis2_status_t
axiom_soap_builder_construct_xop_include(
    axiom_soap_builder_t *soap_builder,
    const axutil_env_t *env,
    axiom_node_t *om_element_node)
{
    axiom_namespace_t *ns = NULL;
    axis2_char_t *uri = NULL;
    axiom_element_t *om_element = NULL;
    axutil_qname_t *qname = NULL;
    axis2_char_t *id = NULL;

    om_element = (axiom_element_t *)axiom_node_get_data_element(om_element_node, env);
    ns = axiom_element_get_namespace(om_element, env, om_element_node);
    if(!ns)
    {
        /* this is not a xop:include element. so, we can safely return success */
        return AXIS2_SUCCESS;
    }

    uri = axiom_namespace_get_uri(ns, env);
    if((!uri) || (axutil_strcmp(uri, AXIS2_XOP_NAMESPACE_URI) != 0))
    {
        /* this is not a xop:include element. so, we can safely return success */
        return AXIS2_SUCCESS;
    }

    qname = axutil_qname_create(env, "href", NULL, NULL);
    if(!qname)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create qname with value href");
        return AXIS2_FAILURE;
    }

    id = axiom_element_get_attribute_value(om_element, env, qname);
    axutil_qname_free(qname, env);
    if(!id)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot get href ID of the xop:include element");
        return AXIS2_FAILURE;
    }

    if(!axutil_strstr(id, "cid:"))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "ID of xop:include doesn't include 'cid:' part");
        return AXIS2_FAILURE;
    }

    /* everything looks fine, so recursively build the OM tree, so that we can get data handlers */
    while(!axiom_node_is_complete(om_element_node, env))
    {
        axiom_stax_builder_next_with_token(soap_builder->om_builder, env);
    }

    if(soap_builder->mime_body_parts)
    {
        axiom_data_handler_t *data_handler = NULL;
        axis2_char_t *id_decoded = NULL;
        axiom_text_t *data_text = NULL;
        axiom_node_t *data_om_node = NULL;
        axiom_node_t *parent = NULL;

        /* Get the value of href id, after "cid:" */
        id += 4;
        id_decoded = axutil_strdup(env, id);
        axutil_url_decode(env, id_decoded, id_decoded);

        /* Find the data handler of given xop:include */
        data_handler = (axiom_data_handler_t *)axutil_hash_get(
            soap_builder-> mime_body_parts, (void *)id_decoded, AXIS2_HASH_KEY_STRING);
        if(!data_handler)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Cannot find data handler corresponding to id %s", id_decoded);
            AXIS2_FREE(env->allocator, id_decoded);
            return AXIS2_FAILURE;
        }

        /* remove the <xop:Include> element, and add the data handler as the child of
         * xop:include's parent */
        parent = axiom_node_get_parent(om_element_node, env);
        axiom_node_free_tree(om_element_node, env);
        data_text = axiom_text_create_with_data_handler(env, parent, data_handler, &data_om_node);
        axiom_text_set_content_id(data_text, env, id_decoded);
        axiom_stax_builder_set_lastnode(soap_builder->om_builder,env, parent);
        AXIS2_FREE(env->allocator, id_decoded);
    }
    else
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot find mime_body_part in soap builder");
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

/* check whether the namespace of given node is either SOAP11 or SOAP12 namespace */
static axis2_status_t
axiom_soap_builder_process_namespace_data(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env,
    axiom_node_t * om_node)
{
    axiom_element_t *om_ele = NULL;
    axiom_namespace_t *om_ns = NULL;

    om_ele = (axiom_element_t *)axiom_node_get_data_element(om_node, env);
    om_ns = axiom_element_get_namespace(om_ele, env, om_node);
    if(om_ns)
    {
        axis2_char_t *ns_uri = NULL;
        ns_uri = axiom_namespace_get_uri(om_ns, env);
        if(ns_uri)
        {
            if((axutil_strcmp(ns_uri, AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI) == 0)
                || (axutil_strcmp(ns_uri, AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI) == 0))
            {
                return AXIS2_SUCCESS;
            }
        }
    }

    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI, AXIS2_FAILURE);
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Invalid SOAP Namespace URI");
    return AXIS2_FAILURE;
}

static axis2_status_t
axiom_soap_builder_identify_soap_version(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env,
    const axis2_char_t * soap_version_uri_from_transport)
{
    axiom_namespace_t *om_ns = NULL;
    axiom_node_t *envelope_node = NULL;
    axiom_element_t *om_ele = NULL;
    axis2_char_t *ns_uri = NULL;

    soap_builder->soap_envelope = axiom_soap_builder_get_soap_envelope(soap_builder, env);
    if(soap_builder->soap_envelope == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOAP_MESSAGE_DOES_NOT_CONTAIN_AN_ENVELOPE,
            AXIS2_FAILURE);
        AXIS2_LOG_CRITICAL(env->log, AXIS2_LOG_SI,
            "SOAP message does not have a SOAP envelope element ");
        return AXIS2_FAILURE;
    }

    envelope_node = axiom_soap_envelope_get_base_node(soap_builder->soap_envelope, env);
    if(!envelope_node)
    {
        return AXIS2_FAILURE;
    }
    om_ele = (axiom_element_t *)axiom_node_get_data_element(envelope_node, env);
    if(!om_ele)
    {
        return AXIS2_FAILURE;
    }
    om_ns = axiom_element_get_namespace(om_ele, env, envelope_node);
    if(!om_ns)
    {
        return AXIS2_FAILURE;
    }
    ns_uri = axiom_namespace_get_uri(om_ns, env);

    if(ns_uri)
    {
        if(soap_version_uri_from_transport
            && axutil_strcmp(soap_version_uri_from_transport, ns_uri) != 0)
        {
            AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_TRANSPORT_LEVEL_INFORMATION_DOES_NOT_MATCH_WITH_SOAP, AXIS2_FAILURE);

            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "AXIS2_ERROR_TRANSPORT_LEVEL_INFORMATION_DOES_NOT_MATCH_WITH_SOAP");
            return AXIS2_FAILURE;
        }
        if(axutil_strcmp(AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, ns_uri) == 0)
        {
            soap_builder->soap_version = AXIOM_SOAP11;

            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Identified soap version is soap11");

            axiom_soap_envelope_set_soap_version_internal(soap_builder-> soap_envelope, env,
                soap_builder-> soap_version);

            return AXIS2_SUCCESS;
        }
        else if(axutil_strcmp(AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, ns_uri) == 0)
        {
            soap_builder->soap_version = AXIOM_SOAP12;

            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "identified soap version is soap12");

            axiom_soap_envelope_set_soap_version_internal(soap_builder-> soap_envelope, env,
                soap_builder-> soap_version);

            return AXIS2_SUCCESS;
        }
    }
    return AXIS2_FAILURE;
}

static axis2_status_t
axiom_soap_builder_parse_headers(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env)
{
    axiom_node_t *om_node = NULL;
    axiom_soap_header_t *soap_header = NULL;
    int status = AXIS2_SUCCESS;
    if(!soap_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Soap Builder is NULL");
        return AXIS2_FAILURE;
    }
    if(!soap_builder->soap_envelope)
    {
        return AXIS2_FAILURE;
    }
    soap_header = axiom_soap_envelope_get_header(soap_builder->soap_envelope, env);

    if(soap_header)
    {
        om_node = axiom_soap_header_get_base_node(soap_header, env);
        if(om_node)
        {
            while(!axiom_node_is_complete(om_node, env))
            {
                status = axiom_soap_builder_next(soap_builder, env);
                if(status == AXIS2_FAILURE)
                    return AXIS2_FAILURE;
            }
            /*HACK: to fix AXIS2C-129 - Samisa */
            /*
             axiom_stax_builder_set_element_level(
             soap_builder->om_builder, env, 1);
             */
        }
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_builder_set_bool_processing_mandatory_fault_elements(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env,
    axis2_bool_t value)
{
    soap_builder->processing_mandatory_fault_elements = value;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_builder_set_processing_detail_elements(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env,
    axis2_bool_t value)
{
    soap_builder->processing_detail_elements = value;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_soap_builder_is_processing_detail_elements(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env)
{
    return soap_builder->processing_detail_elements;
}

AXIS2_EXTERN int AXIS2_CALL
axiom_soap_builder_get_soap_version(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env)
{
    return soap_builder->soap_version;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_builder_set_mime_body_parts(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env,
    axutil_hash_t * map)
{
    soap_builder->mime_body_parts = map;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axiom_soap_builder_get_mime_body_parts(
    axiom_soap_builder_t * builder,
    const axutil_env_t * env)
{
    return builder->mime_body_parts;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_soap_builder_set_mime_parser(
    axiom_soap_builder_t * builder,
    const axutil_env_t * env,
    axiom_mime_parser_t *mime_parser)
{
    builder->mime_parser = mime_parser;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_soap_builder_set_callback_function(
    axiom_soap_builder_t * builder,
    const axutil_env_t * env,
    AXIS2_READ_INPUT_CALLBACK callback)
{
    builder->callback = callback;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_soap_builder_set_callback_ctx(
    axiom_soap_builder_t * builder,
    const axutil_env_t * env,
    void *callback_ctx)
{
    builder->callback_ctx = callback_ctx;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_builder_create_attachments(
    axiom_soap_builder_t * builder,
    const axutil_env_t * env,
    void *user_param,
    axis2_char_t *callback_name)
{
    axutil_hash_t *attachments_map = NULL;
    axis2_char_t *mime_boundary = NULL;

    if(builder->mime_parser)
    {
        if(builder->callback_ctx)
        {
            mime_boundary = axiom_mime_parser_get_mime_boundary(builder->mime_parser, env);

            if(mime_boundary)
            {
                if(callback_name)
                {
                    axiom_mime_parser_set_caching_callback_name(builder->mime_parser, env,
                        callback_name);
                }
                attachments_map = axiom_mime_parser_parse_for_attachments(builder->mime_parser,
                    env, builder->callback, builder->callback_ctx, mime_boundary, user_param);

                if(attachments_map)
                {
                    builder->mime_body_parts = attachments_map;
                    return AXIS2_SUCCESS;
                }
                else
                {
                    return AXIS2_FAILURE;
                }
            }
            else
            {
                return AXIS2_FAILURE;
            }
        }
        else
        {
            return AXIS2_FAILURE;
        }
    }
    else
    {
        return AXIS2_FAILURE;
    }
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_soap_builder_replace_xop(
    axiom_soap_builder_t * builder,
    const axutil_env_t * env,
    axiom_node_t *om_element_node,
    axiom_element_t *om_element)
{
    axiom_namespace_t *ns = NULL;
    axis2_bool_t is_replaced = AXIS2_FALSE;

    ns = axiom_element_get_namespace(om_element, env, om_element_node);
    if(ns)
    {
        axis2_char_t *uri = axiom_namespace_get_uri(ns, env);
        if(uri)
        {
            if(axutil_strcmp(uri, AXIS2_XOP_NAMESPACE_URI) == 0)
            {
                axutil_qname_t *qname = NULL;
                qname = axutil_qname_create(env, "href", NULL, NULL);
                if(qname)
                {
                    axis2_char_t *id = NULL;
                    id = axiom_element_get_attribute_value(om_element, env, qname);
                    if(id)
                    {
                        axis2_char_t *pos = NULL;
                        pos = axutil_strstr(id, "cid:");
                        if(pos)
                        {
                            axiom_data_handler_t *data_handler = NULL;
                            id += 4;

                            if(builder->mime_body_parts)
                            {

                                axis2_char_t *id_decoded = NULL;

                                id_decoded = axutil_strdup(env, id);

                                axutil_url_decode(env, id_decoded, id_decoded);

                                data_handler = (axiom_data_handler_t *)axutil_hash_get(
                                    builder-> mime_body_parts, (void *)id_decoded,
                                    AXIS2_HASH_KEY_STRING);
                                if(data_handler)
                                {
                                    axiom_text_t *data_text = NULL;
                                    axiom_node_t *data_om_node = NULL;
                                    axiom_node_t *parent = NULL;

                                    parent = axiom_node_get_parent(om_element_node, env);

                                    /*remove the <xop:Include> element */
                                    axiom_node_free_tree(om_element_node, env);

                                    data_text = axiom_text_create_with_data_handler(env, parent,
                                        data_handler, &data_om_node);

                                    axiom_text_set_content_id(data_text, env, id_decoded);
                                    /*axiom_stax_builder_set_lastnode
                                     (soap_builder->om_builder, env,
                                     parent);*/
                                    is_replaced = AXIS2_TRUE;
                                }
                                if(id_decoded)
                                {
                                    AXIS2_FREE(env->allocator, id_decoded);
                                }
                            }
                        }
                    }
                }
                axutil_qname_free(qname, env);
            }
        }
    }

    return is_replaced;
}

AXIS2_EXTERN axiom_stax_builder_t *AXIS2_CALL
axiom_soap_builder_get_om_builder(
    axiom_soap_builder_t * soap_builder,
    const axutil_env_t * env)
{
    if(!soap_builder)
    {
        return NULL;
    }
    return soap_builder->om_builder;
}

