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

#include <axiom_stax_builder.h>
#include <axiom_element.h>
#include <axiom_text.h>
#include <axiom_processing_instruction.h>
#include <axiom_comment.h>
#include <axis2_string.h>
#include <axiom_xml_writer.h>
#include <axiom_doctype.h>
#include "axiom_node_internal.h"
#include "axiom_stax_builder_internal.h"

struct axiom_stax_builder
{
    /** pull parser instance used by the om_builder */
    axiom_xml_reader_t *parser;
    /** last node the om_builder found */
    axiom_node_t *lastnode;

    axiom_node_t *root_node;
    /** document associated with the om_builder */
    axiom_document_t *document;
    /** done building the document? */
    axis2_bool_t done;
    /** parser was accessed? */
    axis2_bool_t parser_accessed;
    /** caching enabled? */
    axis2_bool_t cache;
    /** current event */
    int current_event;

    int element_level;
    axis2_hash_t *declared_namespaces;
};

AXIS2_EXTERN axiom_stax_builder_t *AXIS2_CALL
axiom_stax_builder_create(const axis2_env_t *env,
    axiom_xml_reader_t *parser)
{
    axiom_stax_builder_t *om_builder = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, parser, NULL);

    om_builder = (axiom_stax_builder_t *)AXIS2_MALLOC(env->allocator,
        sizeof(axiom_stax_builder_t));

    if (!om_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    om_builder->cache = AXIS2_TRUE;
    om_builder->parser_accessed = AXIS2_FALSE;
    om_builder->done = AXIS2_FALSE;
    om_builder->lastnode = NULL;
    om_builder->document = NULL;
    om_builder->parser = parser;
    om_builder->current_event = -1;
    om_builder->root_node = NULL;
    om_builder->element_level = 0;

    om_builder->declared_namespaces = axis2_hash_make(env);

    om_builder->document = axiom_document_create(env, NULL, om_builder);
    if (!om_builder->document)
    {
        AXIS2_FREE(env->allocator, om_builder);
        return NULL;
    }

    return om_builder;
}


axis2_status_t
axiom_stax_builder_process_attributes(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env,
    axiom_node_t *element_node)
{
    int i = 0;
    int attribute_count;
    axiom_attribute_t *attribute = NULL;
    axiom_namespace_t *ns = NULL;
    axis2_char_t *uri = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *attr_name = NULL;
    axis2_char_t *attr_value = NULL;

    axis2_status_t status = AXIS2_SUCCESS;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element_node, AXIS2_FAILURE);

    attribute_count = AXIOM_XML_READER_GET_ATTRIBUTE_COUNT(om_builder->parser,
        env);
    for (i = 1; i <= attribute_count ; i++)
    {
        axiom_element_t *temp_ele =  NULL;

        uri =
            AXIOM_XML_READER_GET_ATTRIBUTE_NAMESPACE_BY_NUMBER(om_builder->parser,
                env, i);

        prefix = AXIOM_XML_READER_GET_ATTRIBUTE_PREFIX_BY_NUMBER(om_builder->parser,
            env, i);
        if (uri)
        {
            if (axis2_strcmp(uri, "") != 0)
            {
                axiom_element_t *om_ele = NULL;
                om_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(element_node, env);
                if (om_ele)
                {
                    ns = AXIOM_ELEMENT_FIND_NAMESPACE(om_ele ,
                        env , element_node, uri, prefix);

                    /* newly added to handle "xml:*" attributes" (AXIS2_STRCMP(prefix, "xml") == 0) && */
                    if (!ns)
                    {
                        ns = axiom_namespace_create(env, uri, prefix);
                    }
                }
            }
        }

        attr_name = AXIOM_XML_READER_GET_ATTRIBUTE_NAME_BY_NUMBER(
            om_builder->parser, env, i);

        attr_value = AXIOM_XML_READER_GET_ATTRIBUTE_VALUE_BY_NUMBER(
            om_builder->parser, env, i);
        if (attr_name)
        {
            attribute = axiom_attribute_create(env, attr_name, attr_value, ns);
            if (!attribute)
            {
                return AXIS2_FAILURE;
            }

            temp_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(element_node, env);
            if (temp_ele)
            {
                status = AXIOM_ELEMENT_ADD_ATTRIBUTE(temp_ele,
                    env, attribute, element_node);
            }
        }
        if (uri)
        {
            AXIOM_XML_READER_XML_FREE(om_builder->parser, env, uri);
        }
        if (prefix)
        {
            AXIOM_XML_READER_XML_FREE(om_builder->parser, env, prefix);
        }
        if (attr_name)
        {
            AXIOM_XML_READER_XML_FREE(om_builder->parser, env, attr_name);
        }
        if (attr_value)
        {
            AXIOM_XML_READER_XML_FREE(om_builder->parser, env, attr_value);
        }
        ns = NULL;
    }
    return status;
}

axiom_node_t *
axiom_stax_builder_create_om_text(axiom_stax_builder_t * om_builder,
    const axis2_env_t *env)
{
    axis2_char_t *temp_value = NULL;
    axiom_node_t *node = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!om_builder->lastnode)
    {
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL, AXIS2_FAILURE);
        return NULL;
    }
    temp_value = AXIOM_XML_READER_GET_VALUE(om_builder->parser, env);

    if (!temp_value)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_VALUE_NULL,
            AXIS2_FAILURE);
        return NULL;
    }

    if (AXIOM_NODE_IS_COMPLETE(om_builder->lastnode, env))
    {
        axiom_text_create(env,
            AXIOM_NODE_GET_PARENT(om_builder->lastnode, env),
            temp_value, &node);

    }
    else
    {
        axiom_text_create(env, om_builder->lastnode, temp_value, &node);
    }

    axiom_node_set_complete(node , env, AXIS2_TRUE);
    om_builder->lastnode = node;

    AXIOM_XML_READER_XML_FREE(om_builder->parser , env, temp_value);
    return node;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_stax_builder_discard_current_element(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    axiom_node_t *element = NULL;
    axiom_node_t *prev_node = NULL;
    axiom_node_t *parent = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    element = om_builder->lastnode;

    if (AXIOM_NODE_IS_COMPLETE(element, env) || !(om_builder->cache))
    {
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    om_builder->cache = AXIS2_FALSE;
    do
    {
        while (AXIOM_XML_READER_NEXT(om_builder->parser, env)
            != AXIOM_XML_READER_END_ELEMENT);
    }
    while (!(AXIOM_NODE_IS_COMPLETE(element, env)));

    /*All children of this element is pulled now */

    prev_node = AXIOM_NODE_GET_PREVIOUS_SIBLING(element, env);
    if (prev_node)
    {
        AXIOM_NODE_FREE_TREE(AXIOM_NODE_GET_NEXT_SIBLING(prev_node, env), env);
        axiom_node_set_next_sibling(prev_node, env, NULL);
    }
    else
    {
        parent = AXIOM_NODE_GET_PARENT(element, env);
        AXIOM_NODE_FREE_TREE(AXIOM_NODE_GET_FIRST_CHILD(parent, env), env);
        axiom_node_set_first_child(parent, env, NULL);
        om_builder->lastnode = parent;
    }
    om_builder->cache = AXIS2_TRUE;

    return AXIS2_SUCCESS;
}

axis2_status_t
axiom_stax_builder_process_namespaces(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env,
    axiom_node_t *node,
    int is_soap_element)
{
    axis2_status_t status = AXIS2_SUCCESS;
    int namespace_count = 0;
    axiom_namespace_t *om_ns = NULL;
    /* temp values */
    axis2_char_t *temp_prefix = NULL;
    axis2_char_t *temp_ns_prefix = NULL;
    axis2_char_t *temp_ns_uri    = NULL;

    int i = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    namespace_count = AXIOM_XML_READER_GET_NAMESPACE_COUNT(om_builder->parser, env);
    for (i = 1; i <= namespace_count; i++)
    {
        temp_ns_prefix = AXIOM_XML_READER_GET_NAMESPACE_PREFIX_BY_NUMBER(
            om_builder->parser, env, i);

        temp_ns_uri = AXIOM_XML_READER_GET_NAMESPACE_URI_BY_NUMBER(
            om_builder->parser, env , i);

        if (!temp_ns_prefix || AXIS2_STRCMP(temp_ns_prefix, "xmlns") == 0)
        {
            /** default namespace case */
            /** !temp_ns_prefix is for guththila */
            axiom_element_t *om_ele = NULL;
            om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(node, env);

            om_ns = axiom_namespace_create(env, temp_ns_uri, "");
            if (!om_ns || !om_ele)
            {
                return AXIS2_FAILURE;
            }

            status = AXIOM_ELEMENT_DECLARE_NAMESPACE(om_ele, env, node, om_ns);

            if (!status)
            {
                axiom_namespace_free(om_ns, env);
                om_ns = NULL;
            }
        }
        else
        {
            axiom_element_t *om_ele = NULL;
            axis2_char_t *prefix = NULL;
            om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(node, env);

            om_ns = axiom_namespace_create(env, temp_ns_uri, temp_ns_prefix);
            if (!om_ns || !om_ele)
            {
                return AXIS2_FAILURE;
            }

            status = axiom_element_declare_namespace_assume_param_ownership(om_ele,
                env, om_ns);
            prefix = axiom_namespace_get_prefix(om_ns, env);
            axis2_hash_set(om_builder->declared_namespaces,
                prefix, AXIS2_HASH_KEY_STRING, om_ns);
        }
        AXIOM_XML_READER_XML_FREE(om_builder->parser, env, temp_ns_prefix);
        AXIOM_XML_READER_XML_FREE(om_builder->parser, env, temp_ns_uri);
        if (!om_ns)
        {
            /* something went wrong */
            return AXIS2_FAILURE;
        }
    }
    /* set own namespace */
    temp_prefix = AXIOM_XML_READER_GET_PREFIX(om_builder->parser, env);
    if (temp_prefix)
    {
        om_ns = axis2_hash_get(om_builder->declared_namespaces,
            temp_prefix, AXIS2_HASH_KEY_STRING);

        if (om_ns)
        {
            axiom_element_t *om_ele = NULL;
            om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(node, env);
            if (om_ele)
            {
                axiom_element_set_namespace_assume_param_ownership(om_ele, env, om_ns);
            }
        }
        else
        {
            AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    if (temp_prefix)
    {
        AXIOM_XML_READER_XML_FREE(om_builder->parser, env, temp_prefix);
    }
    return status;
}

axiom_node_t *
axiom_stax_builder_create_om_element(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    axiom_node_t *element_node = NULL;
    axiom_element_t *om_ele = NULL;
    axis2_char_t *temp_localname = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, om_builder, NULL);

    temp_localname  = AXIOM_XML_READER_GET_NAME(om_builder->parser, env);

    if (!temp_localname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL,
            AXIS2_FAILURE);
        return NULL;
    }

    om_builder->element_level++;

    if (!(om_builder->lastnode))
    {
        om_ele = axiom_element_create(env , NULL, temp_localname, NULL, &element_node);
        if (!om_ele)
        {
            return NULL;
        }

        om_builder->root_node = element_node;

        axiom_node_set_builder(element_node, env, om_builder);

        if (om_builder->document)
        {
            axiom_node_set_document(element_node, env, om_builder->document);
            axiom_document_set_root_element(om_builder->document,
                env, element_node);
        }
    }
    else if (AXIOM_NODE_IS_COMPLETE(om_builder->lastnode, env))
    {
        om_ele = axiom_element_create(env,
            AXIOM_NODE_GET_PARENT(om_builder->lastnode, env),
            temp_localname, NULL, &element_node);
        if (!om_ele)
        {
            return NULL;
        }

        if (element_node)
        {
            axiom_node_set_next_sibling(om_builder->lastnode,
                env, element_node);
            axiom_node_set_previous_sibling(element_node,
                env, om_builder->lastnode);
            axiom_node_set_document(element_node,
                env, om_builder->document);
            axiom_node_set_builder(element_node,
                env, om_builder);
        }
    }
    else
    {
        om_ele = axiom_element_create(env, om_builder->lastnode,
            temp_localname, NULL, &element_node);
        if (element_node)
        {
            axiom_node_set_first_child(om_builder->lastnode,
                env, element_node);
            axiom_node_set_parent(element_node , env, om_builder->lastnode);
            axiom_node_set_document(element_node, env, om_builder->document);
            axiom_node_set_builder(element_node, env, om_builder);
        }
    }
    if (temp_localname)
    {
        AXIOM_XML_READER_XML_FREE(om_builder->parser , env, temp_localname);
    }

    /** order of processing namespaces first is important */
    axiom_stax_builder_process_namespaces(om_builder, env, element_node, 0);
    axiom_stax_builder_process_attributes(om_builder, env, element_node);

    om_builder->lastnode = element_node;

    return element_node;
}


axiom_node_t *
axiom_stax_builder_create_om_comment(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    axiom_node_t *comment_node = NULL;
    axis2_char_t *comment_value = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    comment_value  = AXIOM_XML_READER_GET_VALUE(om_builder->parser, env);
    if (!comment_value)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL,
            AXIS2_FAILURE);
        return NULL;
    }

    if (!(om_builder->lastnode))
    {
        /* do nothing */
        AXIOM_XML_READER_XML_FREE(om_builder->parser , env, comment_value);
        return NULL;
    }
    else if (AXIOM_NODE_IS_COMPLETE(om_builder->lastnode, env))
    {
        axiom_comment_create(env,
            AXIOM_NODE_GET_PARENT(om_builder->lastnode, env),
            comment_value , &comment_node);

        axiom_node_set_next_sibling(om_builder->lastnode, env, comment_node);
        axiom_node_set_previous_sibling(comment_node , env,
            om_builder->lastnode);
        axiom_node_set_builder(comment_node, env, om_builder);
        axiom_node_set_document(comment_node, env, om_builder->document);
    }
    else
    {
        axiom_comment_create(env, om_builder->lastnode,
            comment_value , &comment_node);

        axiom_node_set_first_child(om_builder->lastnode , env, comment_node);
        axiom_node_set_parent(comment_node , env, om_builder->lastnode);
        axiom_node_set_builder(comment_node, env, om_builder);
        axiom_node_set_document(comment_node, env, om_builder->document);
    }

    om_builder->element_level++;
    AXIOM_XML_READER_XML_FREE(om_builder->parser , env, comment_value);

    om_builder->lastnode = comment_node;

    return comment_node;
}

axiom_node_t *
axiom_stax_builder_create_om_doctype(axiom_stax_builder_t * om_builder,
    const axis2_env_t *env)
{
    axiom_node_t *doctype_node = NULL;
    axis2_char_t *doc_value = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    doc_value = AXIOM_XML_READER_GET_DTD(om_builder->parser, env);
    if (!doc_value)
    {
        return NULL;
    }
    if (!(om_builder->lastnode))
    {
        axiom_doctype_create(env, NULL, doc_value, &doctype_node);
        if (om_builder->document)
        {
            axiom_document_set_root_element(om_builder->document,
                env, doctype_node);
        }
    }
    om_builder->lastnode = doctype_node;
    AXIOM_XML_READER_XML_FREE(om_builder->parser, env, doc_value);
    return doctype_node;
}

axiom_node_t *
axiom_stax_builder_create_om_processing_instruction(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    axiom_node_t *pi_node = NULL;
    axis2_char_t *target = NULL;
    axis2_char_t *value  = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    target = AXIOM_XML_READER_GET_PI_TARGET(om_builder->parser, env);
    value  = AXIOM_XML_READER_GET_PI_DATA(om_builder->parser, env);
    if (!target)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL,
            AXIS2_FAILURE);
        return NULL;
    }
    if (!(om_builder->lastnode))
    {
        /* do nothing */
        AXIOM_XML_READER_XML_FREE(om_builder->parser , env, target);
        AXIOM_XML_READER_XML_FREE(om_builder->parser , env, value);
        return NULL;
    }
    else if (AXIOM_NODE_IS_COMPLETE(om_builder->lastnode, env) ||
        (AXIOM_NODE_GET_NODE_TYPE(om_builder->lastnode, env) == AXIOM_TEXT))
    {
        axiom_processing_instruction_create(env,
            AXIOM_NODE_GET_PARENT(om_builder->lastnode, env),
            target, value, &pi_node);

        axiom_node_set_next_sibling(om_builder->lastnode, env, pi_node);
        axiom_node_set_previous_sibling(pi_node , env, om_builder->lastnode);
    }
    else
    {
        axiom_processing_instruction_create(env, om_builder->lastnode,
            target, value, &pi_node);
        axiom_node_set_first_child(om_builder->lastnode , env, pi_node);
        axiom_node_set_parent(pi_node , env, om_builder->lastnode);
    }

    om_builder->element_level++;

    if (target)
    {
        AXIOM_XML_READER_XML_FREE(om_builder->parser , env, target);
    }
    if (value)
    {
        AXIOM_XML_READER_XML_FREE(om_builder->parser , env, value);
    }

    om_builder->lastnode = pi_node;
    return pi_node;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_stax_builder_end_element(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    axiom_node_t *parent = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_builder->element_level--;

    if (om_builder->lastnode)
    {
        if (AXIOM_NODE_IS_COMPLETE((om_builder->lastnode), env))
        {
            parent = AXIOM_NODE_GET_PARENT((om_builder->lastnode), env);
            if (parent)
            {
                axiom_node_set_complete(parent, env, AXIS2_TRUE);
                om_builder->lastnode = parent;
            }
        }
        else
        {
            axiom_node_set_complete((om_builder->lastnode), env, AXIS2_TRUE);
        }
    }
    if (om_builder->root_node)
    {
        if (AXIOM_NODE_IS_COMPLETE(om_builder->root_node , env))
        {
            om_builder->done = AXIS2_TRUE;
        }
    }
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axiom_node_t * AXIS2_CALL
axiom_stax_builder_next(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    int token = 0;
    axiom_node_t *node = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    if (!om_builder->parser)
    {
        return NULL;
    }
    do
    {
        if (om_builder->done)
        {
            AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL, AXIS2_FAILURE);
            return NULL;
        }

        token = AXIOM_XML_READER_NEXT(om_builder->parser, env);
        if (token == -1)
        {
            return NULL;
        }

        om_builder->current_event = token;

        if (!(om_builder->cache))
        {
            return NULL;
        }

        switch (token)
        {
            case AXIOM_XML_READER_START_DOCUMENT:
                /*Do nothing */
                break;

            case AXIOM_XML_READER_START_ELEMENT:
                node = axiom_stax_builder_create_om_element(om_builder, env);
                break;

            case AXIOM_XML_READER_EMPTY_ELEMENT:
                node = axiom_stax_builder_create_om_element(om_builder, env);

            case AXIOM_XML_READER_END_ELEMENT:
                axiom_stax_builder_end_element(om_builder, env);
                break;


            case AXIOM_XML_READER_SPACE:
                node = axiom_stax_builder_create_om_text(om_builder, env);
                break;

            case AXIOM_XML_READER_CHARACTER:
                node = axiom_stax_builder_create_om_text(om_builder, env);
                break;

            case AXIOM_XML_READER_ENTITY_REFERENCE:
                break;

            case AXIOM_XML_READER_COMMENT:

                node = axiom_stax_builder_create_om_comment(om_builder, env);
                    axiom_stax_builder_end_element(om_builder, env);
                break;

            case AXIOM_XML_READER_PROCESSING_INSTRUCTION:

                node = 
                    axiom_stax_builder_create_om_processing_instruction(om_builder,
                        env);
                axiom_stax_builder_end_element(om_builder, env);
                break;

            case AXIOM_XML_READER_CDATA:
                break;

            case AXIOM_XML_READER_DOCUMENT_TYPE:
                break;

            default:
                break;
        }
    }
    while (!node);
    return node;
}


AXIS2_EXTERN axis2_status_t
AXIS2_CALL axiom_stax_builder_free(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (!om_builder)
    {
        return AXIS2_FAILURE;
    }

    if (om_builder->parser)
    {
        AXIOM_XML_READER_FREE(om_builder->parser, env);
        om_builder->parser = NULL;
    }

    if (om_builder->declared_namespaces)
    {
        axis2_hash_free(om_builder->declared_namespaces, env);
        om_builder->declared_namespaces = NULL;
    }

    if (om_builder->document)
    {
        axiom_document_free(om_builder->document, env);
        om_builder->document = NULL;
    }
    else
    {
        if (om_builder->root_node)
        {
            AXIOM_NODE_FREE_TREE(om_builder->root_node, env);
            om_builder->root_node = NULL;
        }
    }
    AXIS2_FREE(env->allocator, om_builder);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_document_t* AXIS2_CALL
axiom_stax_builder_get_document(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    return om_builder->document;
}

/**
    This is an internal function
*/

AXIS2_EXTERN int AXIS2_CALL
axiom_stax_builder_get_current_event(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    AXIS2_PARAM_CHECK(env->error, om_builder, AXIS2_FAILURE);
    return om_builder->current_event;
}

/**
 This is an internal function
*/
AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_stax_builder_get_lastnode(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    AXIS2_PARAM_CHECK(env->error, om_builder, NULL);
    return om_builder->lastnode;
}

/**
This is an internal function
*/
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_stax_builder_is_complete(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return om_builder->done;
}

/**
This is an internal function to be used by soap om_builder only
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_stax_builder_set_lastnode(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env,
    axiom_node_t *om_node)
{
    AXIS2_PARAM_CHECK(env->error, om_node, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_builder, AXIS2_FAILURE);

    om_builder->lastnode = om_node;
    return AXIS2_SUCCESS;
}

/**
internal function for soap om_builder only
*/
AXIS2_EXTERN int AXIS2_CALL
axiom_stax_builder_get_element_level(axiom_stax_builder_t* om_builder,
    const axis2_env_t *env)
{
    AXIS2_PARAM_CHECK(env->error, om_builder, -1);
    return om_builder->element_level;
}

/**
internal function for soap om_builder only
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_stax_builder_set_element_level(axiom_stax_builder_t* om_builder,
    const axis2_env_t *env,
    int element_level)
{
    AXIS2_PARAM_CHECK(env->error, om_builder, AXIS2_FAILURE);
    om_builder->element_level = element_level;
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axiom_stax_builder_next_with_token(axiom_stax_builder_t *om_builder,
    const axis2_env_t *env)
{
    int token = 0;
    void *val = NULL;

    if (!om_builder)
    {
        return -1;
    }

    if (om_builder->done)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL, AXIS2_FAILURE);
        return -1;
    }

    if (!om_builder->parser)
    {
        return -1;
    }

    token = AXIOM_XML_READER_NEXT(om_builder->parser, env);

    if (token == -1)
    {
        om_builder->done = AXIS2_TRUE;
        return -1;
    }

    if (!(om_builder->cache))
    {
        return -1;
    }
    switch (token)
    {
        case AXIOM_XML_READER_START_DOCUMENT:
            /*Do nothing */
            break;

        case AXIOM_XML_READER_START_ELEMENT:
            val = axiom_stax_builder_create_om_element(om_builder, env);
            if (!val)
            {
                return -1;
            }
            break;

        case AXIOM_XML_READER_EMPTY_ELEMENT:
            val = axiom_stax_builder_create_om_element(
                om_builder, env);
            if (!val)
            {
                return -1;
            }
        case AXIOM_XML_READER_END_ELEMENT:
            axiom_stax_builder_end_element(om_builder, env);
            break;

        case AXIOM_XML_READER_SPACE:
            /* Do nothing */
            break;

        case AXIOM_XML_READER_CHARACTER:
            val = axiom_stax_builder_create_om_text(om_builder, env);
            if (!val)
            {
                return -1;
            }
            break;

        case AXIOM_XML_READER_ENTITY_REFERENCE:
            break;

        case AXIOM_XML_READER_COMMENT:
            val = axiom_stax_builder_create_om_comment(om_builder, env);
            if (!val)
            {
                return -1;
            }
            axiom_stax_builder_end_element(om_builder, env);
            break;

        case AXIOM_XML_READER_PROCESSING_INSTRUCTION:
            val = axiom_stax_builder_create_om_processing_instruction(om_builder,
                env);
            if (!val)
            {
                return -1;
            }
            axiom_stax_builder_end_element(om_builder, env);
            break;

        case AXIOM_XML_READER_CDATA:
            break;

        case AXIOM_XML_READER_DOCUMENT_TYPE:
            break;

        default:
            break;
    }
    return token;
}


