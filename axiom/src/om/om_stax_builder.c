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
/**************************** function prototypes *****************************/
/********************************* axiom_stax_builder_impl_t struct ********/
typedef struct axiom_stax_builder_impl_t
{
    axiom_stax_builder_t om_stax_builder;
    /** pull parser instance used by the builder */
    axiom_xml_reader_t *parser;
    /** last node the builder found */
    axiom_node_t *lastnode;

    axiom_node_t *root_node;
    /** document associated with the builder */
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

}
axiom_stax_builder_impl_t;

static const axiom_stax_builder_ops_t axiom_stax_builder_ops_var = {
    0
};

/************************************** Macro *********************************/

#define AXIS2_INTF_TO_IMPL(builder) ((axiom_stax_builder_impl_t*)builder)

/******************************************************************************/


AXIS2_EXTERN axiom_stax_builder_t * AXIS2_CALL
axiom_stax_builder_create(const axis2_env_t *env,
        axiom_xml_reader_t *parser)
{
    axiom_stax_builder_impl_t *builder = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, parser, NULL);

    builder = (axiom_stax_builder_impl_t *)AXIS2_MALLOC(
                env->allocator, sizeof(axiom_stax_builder_impl_t));

    if (!builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    builder->cache = AXIS2_TRUE;
    builder->parser_accessed = AXIS2_FALSE;
    builder->done = AXIS2_FALSE;
    builder->lastnode = NULL;
    builder->document = NULL;
    builder->parser = parser;
    builder->current_event = -1;
    builder->root_node = NULL;
    builder->element_level = 0;

    builder->document = axiom_document_create(env, NULL, &(builder->om_stax_builder));
    if (!builder->document)
    {
        AXIS2_FREE(env->allocator, builder);
        return NULL;
    }

    /* ops */
    builder->om_stax_builder.ops = &axiom_stax_builder_ops_var;

    return &(builder->om_stax_builder);
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

    axiom_stax_builder_impl_t *builder_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element_node, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(om_builder);


    attribute_count = AXIOM_XML_READER_GET_ATTRIBUTE_COUNT(
                builder_impl->parser, env);
    for (i = 1; i <= attribute_count ; i++)
    {
        axiom_element_t *temp_ele =  NULL;

        uri =  AXIOM_XML_READER_GET_ATTRIBUTE_NAMESPACE_BY_NUMBER(
                    builder_impl->parser, env, i);

        prefix = AXIOM_XML_READER_GET_ATTRIBUTE_PREFIX_BY_NUMBER(
                    builder_impl->parser, env, i);
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
                    builder_impl->parser, env, i);

        attr_value = AXIOM_XML_READER_GET_ATTRIBUTE_VALUE_BY_NUMBER(
                    builder_impl->parser, env, i);
        if (attr_name)
        {
            attribute = axiom_attribute_create(env, attr_name, attr_value, ns);
            if (!attribute)
                return AXIS2_FAILURE;

            temp_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(element_node, env);
            if (temp_ele)
            {
                status = AXIOM_ELEMENT_ADD_ATTRIBUTE(temp_ele, env, attribute, element_node);
            }
        }
        if (uri)
            AXIOM_XML_READER_XML_FREE(builder_impl->parser, env, uri);
        if (prefix)
            AXIOM_XML_READER_XML_FREE(builder_impl->parser, env, prefix);
        if (attr_name)
            AXIOM_XML_READER_XML_FREE(builder_impl->parser, env, attr_name);
        if (attr_value)
            AXIOM_XML_READER_XML_FREE(builder_impl->parser, env, attr_value);

        ns = NULL;
    }
    return status;
}


axiom_node_t *
axiom_stax_builder_create_om_text(axiom_stax_builder_t * om_stax_builder,
        const axis2_env_t *env)
{
    axis2_char_t *temp_value = NULL;
    axiom_node_t *node = NULL;
    axiom_stax_builder_impl_t *builder = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);

    if (!builder->lastnode)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL, AXIS2_FAILURE);
        return NULL;
    }
    temp_value = AXIOM_XML_READER_GET_VALUE(builder->parser, env);

    if (!temp_value)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_VALUE_NULL, AXIS2_FAILURE);
        return NULL;
    }

    if (AXIOM_NODE_IS_COMPLETE(builder->lastnode, env))
    {
        axiom_text_create(env,
                AXIOM_NODE_GET_PARENT(builder->lastnode, env),
                temp_value, &node);

    }
    else
    {
        axiom_text_create(env, builder->lastnode, temp_value, &node);
    }

    axiom_node_set_complete(node , env, AXIS2_TRUE);
    builder->lastnode = node;

    AXIOM_XML_READER_XML_FREE(builder->parser , env, temp_value);
    return node;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_stax_builder_discard_current_element(axiom_stax_builder_t *om_stax_builder,
        const axis2_env_t *env)
{
    axiom_node_t *element = NULL;
    axiom_node_t *prev_node = NULL;
    axiom_node_t *parent = NULL;
    axiom_stax_builder_impl_t *builder = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);

    element = builder->lastnode;

    if (AXIOM_NODE_IS_COMPLETE(element, env) || !(builder->cache))
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD, AXIS2_FAILURE);

        return AXIS2_FAILURE;
    }

    builder->cache = AXIS2_FALSE;
    do
    {
        while (AXIOM_XML_READER_NEXT(builder->parser, env)
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
        builder->lastnode = parent;
    }
    builder->cache = AXIS2_TRUE;

    return AXIS2_SUCCESS;
}

axis2_status_t
axiom_stax_builder_process_namespaces(axiom_stax_builder_t *om_stax_builder,
        const axis2_env_t *env,
        axiom_node_t *node,
        int is_soap_element)
{
    axis2_status_t status = AXIS2_SUCCESS;
    int namespace_count = 0;
    axiom_namespace_t *om_ns = NULL;
    axiom_namespace_t *temp_ns = NULL;
    /* temp values */
    axis2_char_t *temp_prefix = NULL;
    axis2_char_t *temp_ns_prefix = NULL;
    axis2_char_t *temp_ns_uri    = NULL;

    axiom_stax_builder_impl_t *builder = NULL;
    int i = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);

    namespace_count = AXIOM_XML_READER_GET_NAMESPACE_COUNT(builder->parser, env);
    for (i = 1; i <= namespace_count; i++)
    {
        temp_ns_prefix = AXIOM_XML_READER_GET_NAMESPACE_PREFIX_BY_NUMBER(
                    builder->parser, env, i);

        temp_ns_uri = AXIOM_XML_READER_GET_NAMESPACE_URI_BY_NUMBER(
                    builder->parser, env , i);

        if (!temp_ns_prefix || AXIS2_STRCMP(temp_ns_prefix, "xmlns") == 0)
        {
            /** default namespace case */
            /** !temp_ns_prefix is for guththila */
            axiom_element_t *om_ele = NULL;
            om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(node, env);

            om_ns = axiom_namespace_create(env, temp_ns_uri, "");
            if (!om_ns || !om_ele)
                return AXIS2_FAILURE;

            status = AXIOM_ELEMENT_DECLARE_NAMESPACE(om_ele, env, node, om_ns);

            temp_ns = AXIOM_ELEMENT_FIND_DECLARED_NAMESPACE(om_ele, env, temp_ns_uri, NULL);
            /*
            if(temp_ns)
            {
               AXIOM_ELEMENT_SET_NAMESPACE (om_ele, env, om_ns, node); 
            } 
            */
            if (!temp_ns)
            {
                AXIOM_NAMESPACE_FREE(om_ns, env);
                om_ns = NULL;
            }
        }
        else
        {
            axiom_element_t *om_ele = NULL;
            om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(node, env);

            om_ns = axiom_namespace_create(env, temp_ns_uri, temp_ns_prefix);
            if (!om_ns || !om_ele)
                return AXIS2_FAILURE;

            status = AXIOM_ELEMENT_DECLARE_NAMESPACE(om_ele, env, node, om_ns);
            /*
             temp_ns = AXIOM_ELEMENT_FIND_DECLARED_NAMESPACE(om_ele, 
                             env, temp_ns_uri,temp_ns_prefix);
            */
        }
        AXIOM_XML_READER_XML_FREE(builder->parser, env, temp_ns_prefix);
        AXIOM_XML_READER_XML_FREE(builder->parser, env, temp_ns_uri);
        if (!om_ns)
        {
            /* something went wrong */
            return AXIS2_FAILURE;
        }
    }
    /* set own namespace */
    temp_prefix = AXIOM_XML_READER_GET_PREFIX(builder->parser, env);
    if (temp_prefix)
    {
        om_ns = AXIOM_ELEMENT_FIND_NAMESPACE(
                    (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(node, env),
                    env, node, NULL, temp_prefix);

        if (om_ns)
        {
            axiom_element_t *om_ele = NULL;
            om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(node, env);
            if (om_ele)
                AXIOM_ELEMENT_SET_NAMESPACE(om_ele, env, om_ns, node);
        }
        else
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    if (temp_prefix)
        AXIOM_XML_READER_XML_FREE(builder->parser, env, temp_prefix);

    return status;
}

axiom_node_t *
axiom_stax_builder_create_om_element(axiom_stax_builder_t *om_stax_builder,
        const axis2_env_t *env)
{
    axiom_node_t *element_node = NULL;
    axiom_element_t *om_ele = NULL;
    axiom_stax_builder_impl_t *builder_impl = NULL;
    axis2_char_t *temp_localname = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, om_stax_builder, NULL);

    builder_impl = AXIS2_INTF_TO_IMPL(om_stax_builder);

    temp_localname  = AXIOM_XML_READER_GET_NAME(builder_impl->parser, env);

    if (!temp_localname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL, AXIS2_FAILURE);
        return NULL;
    }

    builder_impl->element_level++;

    if (!(builder_impl->lastnode))
    {
        om_ele = axiom_element_create(env , NULL, temp_localname, NULL, &element_node);
        if (!om_ele)
            return NULL;

        builder_impl->root_node = element_node;

        axiom_node_set_builder(element_node, env, om_stax_builder);

        if (builder_impl->document)
        {
            axiom_node_set_document(element_node, env, builder_impl->document);
            AXIOM_DOCUMENT_SET_ROOT_ELEMENT(builder_impl->document, env, element_node);
        }
    }
    else if (AXIOM_NODE_IS_COMPLETE(builder_impl->lastnode, env))
    {
        axiom_element_t *om_ele = NULL;

        om_ele = axiom_element_create(env,
                AXIOM_NODE_GET_PARENT(builder_impl->lastnode, env),
                temp_localname, NULL, &element_node);
        if (!om_ele)
            return NULL;

        if (element_node)
        {
            axiom_node_set_next_sibling(builder_impl->lastnode, env, element_node);
            axiom_node_set_previous_sibling(element_node , env, builder_impl->lastnode);
            axiom_node_set_document(element_node, env, builder_impl->document);
            axiom_node_set_builder(element_node, env, om_stax_builder);
        }
    }
    else
    {
        axiom_element_t *om_ele = NULL;
        om_ele = axiom_element_create(env, builder_impl->lastnode,
                temp_localname, NULL, &element_node);
        if (element_node)
        {
            axiom_node_set_first_child(builder_impl->lastnode , env, element_node);
            axiom_node_set_parent(element_node , env, builder_impl->lastnode);
            axiom_node_set_document(element_node, env, builder_impl->document);
            axiom_node_set_builder(element_node, env, om_stax_builder);
        }
    }
    if (temp_localname)
        AXIOM_XML_READER_XML_FREE(builder_impl->parser , env, temp_localname);

    /** order of processing namespaces first is important */
    axiom_stax_builder_process_namespaces(om_stax_builder, env, element_node, 0);
    axiom_stax_builder_process_attributes(om_stax_builder, env, element_node);

    builder_impl->lastnode = element_node;

    return element_node;
}


axiom_node_t *
axiom_stax_builder_create_om_comment(axiom_stax_builder_t *builder,
        const axis2_env_t *env)
{
    axiom_node_t *comment_node = NULL;
    axiom_stax_builder_impl_t *builder_impl = NULL;
    axis2_char_t *comment_value = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    comment_value  = AXIOM_XML_READER_GET_VALUE(builder_impl->parser, env);
    if (!comment_value)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL, AXIS2_FAILURE);
        return NULL;
    }

    if (!(builder_impl->lastnode))
    {
        /* do nothing */
        AXIOM_XML_READER_XML_FREE(builder_impl->parser , env, comment_value);
        return NULL;
    }
    else if (AXIOM_NODE_IS_COMPLETE(builder_impl->lastnode, env))
    {
        axiom_comment_create(env,
                AXIOM_NODE_GET_PARENT(builder_impl->lastnode, env),
                comment_value , &comment_node);

        axiom_node_set_next_sibling(builder_impl->lastnode, env, comment_node);
        axiom_node_set_previous_sibling(comment_node , env, builder_impl->lastnode);
		axiom_node_set_builder(comment_node, env, builder);
		axiom_node_set_document(comment_node, env, builder_impl->document);
    }
    else
    {
        axiom_comment_create(env, builder_impl->lastnode,
                comment_value , &comment_node);

        axiom_node_set_first_child(builder_impl->lastnode , env, comment_node);
        axiom_node_set_parent(comment_node , env, builder_impl->lastnode);
		axiom_node_set_builder(comment_node, env, builder);
		axiom_node_set_document(comment_node, env, builder_impl->document);
    }

    builder_impl->element_level++;

    AXIOM_XML_READER_XML_FREE(builder_impl->parser , env, comment_value);

    builder_impl->lastnode = comment_node;

    return comment_node;

}


axiom_node_t *
axiom_stax_builder_create_om_doctype(axiom_stax_builder_t * builder,
        const axis2_env_t *env)
{
    axiom_stax_builder_impl_t *builder_impl;
    axiom_node_t *doctype_node = NULL;
    axis2_char_t *doc_value = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    doc_value = AXIOM_XML_READER_GET_DTD(builder_impl->parser, env);
    if (!doc_value)
        return NULL;
    if (!(builder_impl->lastnode))
    {
        axiom_doctype_create(env, NULL, doc_value, &doctype_node);
        if (builder_impl->document)
        {
            AXIOM_DOCUMENT_SET_ROOT_ELEMENT(builder_impl->document,
                    env, doctype_node);
        }
    }
    builder_impl->lastnode = doctype_node;
    AXIOM_XML_READER_XML_FREE(builder_impl->parser, env, doc_value);
    return doctype_node;
}


axiom_node_t *
axiom_stax_builder_create_om_processing_instruction(axiom_stax_builder_t *builder,
        const axis2_env_t *env)
{
    axiom_node_t *pi_node = NULL;
    axiom_stax_builder_impl_t *builder_impl = NULL;
    axis2_char_t *target = NULL;
    axis2_char_t *value  = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);



    target = AXIOM_XML_READER_GET_PI_TARGET(builder_impl->parser, env);
    value  = AXIOM_XML_READER_GET_PI_DATA(builder_impl->parser, env);
    if (!target)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL, AXIS2_FAILURE);
        return NULL;
    }
    if (!(builder_impl->lastnode))
    {
        /* do nothing */
        AXIOM_XML_READER_XML_FREE(builder_impl->parser , env, target);
        AXIOM_XML_READER_XML_FREE(builder_impl->parser , env, value);
        return NULL;
    }
    else if (AXIOM_NODE_IS_COMPLETE(builder_impl->lastnode, env) ||
            (AXIOM_NODE_GET_NODE_TYPE(builder_impl->lastnode, env) == AXIOM_TEXT))
    {
        axiom_processing_instruction_create(env,
                AXIOM_NODE_GET_PARENT(builder_impl->lastnode, env),
                target, value, &pi_node);

        axiom_node_set_next_sibling(builder_impl->lastnode, env, pi_node);
        axiom_node_set_previous_sibling(pi_node , env, builder_impl->lastnode);
    }
    else
    {
        axiom_processing_instruction_create(env, builder_impl->lastnode,
                target, value, &pi_node);
        axiom_node_set_first_child(builder_impl->lastnode , env, pi_node);
        axiom_node_set_parent(pi_node , env, builder_impl->lastnode);
    }

    builder_impl->element_level++;

    if (target)
        AXIOM_XML_READER_XML_FREE(builder_impl->parser , env, target);
    if (value)
        AXIOM_XML_READER_XML_FREE(builder_impl->parser , env, value);

    builder_impl->lastnode = pi_node;
    return pi_node;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_stax_builder_end_element(axiom_stax_builder_t *om_stax_builder,
        const axis2_env_t *env)
{
    axiom_node_t *parent = NULL;
    axiom_stax_builder_impl_t *builder = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);

    builder->element_level--;

    if (builder->lastnode)
    {
        if (AXIOM_NODE_IS_COMPLETE((builder->lastnode), env))
        {
            parent = AXIOM_NODE_GET_PARENT((builder->lastnode), env);
            if (parent)
            {
                axiom_node_set_complete(parent, env, AXIS2_TRUE);
                builder->lastnode = parent;
            }
        }
        else
        {
            axiom_node_set_complete((builder->lastnode), env, AXIS2_TRUE);
        }
    }
    if (builder->root_node)
    {
        if (AXIOM_NODE_IS_COMPLETE(builder->root_node , env))
        {
            builder->done = AXIS2_TRUE;
        }
    }
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axiom_node_t * AXIS2_CALL
axiom_stax_builder_next(axiom_stax_builder_t *om_stax_builder,
        const axis2_env_t *env)
{
    int token = 0;
    axiom_stax_builder_impl_t *builder_impl = NULL;
    axiom_node_t *node = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    builder_impl = AXIS2_INTF_TO_IMPL(om_stax_builder);
    if (!builder_impl->parser)
        return NULL;
    do
    {
        if (builder_impl->done)
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL, AXIS2_FAILURE);
            return NULL;
        }

        token = AXIOM_XML_READER_NEXT(builder_impl->parser, env);

        if (token == -1)
            return NULL;

        builder_impl->current_event = token;

        if (!(builder_impl->cache))
        {
            return NULL;
        }

        switch (token)
        {
            case AXIOM_XML_READER_START_DOCUMENT:
                /*Do nothing */
                break;

            case AXIOM_XML_READER_START_ELEMENT:
                node = axiom_stax_builder_create_om_element(
                            om_stax_builder, env);
                break;

            case AXIOM_XML_READER_EMPTY_ELEMENT:
                node = axiom_stax_builder_create_om_element(
                            om_stax_builder, env);

            case AXIOM_XML_READER_END_ELEMENT:
                axiom_stax_builder_end_element(om_stax_builder, env);
                break;


            case AXIOM_XML_READER_SPACE:
                /* Do nothing */
                break;

            case AXIOM_XML_READER_CHARACTER:
                node = axiom_stax_builder_create_om_text(om_stax_builder, env);
                break;

            case AXIOM_XML_READER_ENTITY_REFERENCE:
                break;

            case AXIOM_XML_READER_COMMENT:

                node = axiom_stax_builder_create_om_comment(om_stax_builder, env);
                axiom_stax_builder_end_element(om_stax_builder, env);

                break;

            case AXIOM_XML_READER_PROCESSING_INSTRUCTION:

                node = axiom_stax_builder_create_om_processing_instruction(
                            om_stax_builder , env);
                axiom_stax_builder_end_element(om_stax_builder, env);

                break;

            case AXIOM_XML_READER_CDATA:
                break;

            case AXIOM_XML_READER_DOCUMENT_TYPE:
                /* node = axiom_stax_builder_create_om_doctype(om_stax_builder, env);
                */
                break;

            default:
                break;
        }
    }
    while (!node);
    return node;
}


AXIS2_EXTERN axis2_status_t
AXIS2_CALL axiom_stax_builder_free(axiom_stax_builder_t *builder,
        const axis2_env_t *env)
{
    axiom_stax_builder_impl_t *builder_impl =  NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (!builder)
        return AXIS2_FAILURE;

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    if (builder_impl->parser)
    {
        AXIOM_XML_READER_FREE(builder_impl->parser, env);
        builder_impl->parser = NULL;
    }
    if (builder_impl->document)
    {
        AXIOM_DOCUMENT_FREE(builder_impl->document, env);
        builder_impl->document = NULL;
    }
    else
    {
        if (builder_impl->root_node)
        {
            AXIOM_NODE_FREE_TREE(builder_impl->root_node, env);
            builder_impl->root_node = NULL;
        }
    }
    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(builder));
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axiom_document_t* AXIS2_CALL
axiom_stax_builder_get_document(axiom_stax_builder_t *builder,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(builder)->document;
}

/**
    This is an internal function
*/

AXIS2_EXTERN int AXIS2_CALL
axiom_stax_builder_get_current_event(axiom_stax_builder_t *builder,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, -1);
    AXIS2_PARAM_CHECK(env->error, builder, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(builder)->current_event;
}
/**
 This is an internal function
*/
AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_stax_builder_get_lastnode(axiom_stax_builder_t *builder,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, builder, NULL);
    return AXIS2_INTF_TO_IMPL(builder)->lastnode;
}
/**
This is an internal function
*/
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_stax_builder_is_complete(axiom_stax_builder_t *builder,
        const axis2_env_t *env)
{
    axiom_stax_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return builder_impl->done;
}
/**
This is an internal function to be used by soap builder only
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_stax_builder_set_lastnode(axiom_stax_builder_t *builder,
        const axis2_env_t *env,
        axiom_node_t *om_node)
{
    axiom_stax_builder_impl_t *builder_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, om_node, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, builder, AXIS2_FAILURE);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->lastnode = om_node;
    return AXIS2_SUCCESS;
}
/**
internal function for soap builder only
*/
AXIS2_EXTERN int AXIS2_CALL
axiom_stax_builder_get_element_level(axiom_stax_builder_t* builder,
        const axis2_env_t *env)
{
    axiom_stax_builder_impl_t *builder_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, builder, -1);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return builder_impl->element_level;
}
/**
internal function for soap builder only
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_stax_builder_set_element_level(axiom_stax_builder_t* builder,
        const axis2_env_t *env,
        int element_level)
{
    axiom_stax_builder_impl_t *builder_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, builder, AXIS2_FAILURE);

    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->element_level = element_level;
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axiom_stax_builder_next_with_token(axiom_stax_builder_t *builder,
        const axis2_env_t *env)
{

    int token = 0;
    void *val = NULL;

    axiom_stax_builder_impl_t *builder_impl = NULL;

    if (!builder) return -1;

    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    if (builder_impl->done)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL, AXIS2_FAILURE);
        return -1;
    }

    if (!builder_impl->parser) return -1;

    token = AXIOM_XML_READER_NEXT(builder_impl->parser, env);

    if (token == -1)
    {
        builder_impl->done = AXIS2_TRUE;
        return -1;
    }

    if (!(builder_impl->cache))
    {
        return -1;
    }
    switch (token)
    {
        case AXIOM_XML_READER_START_DOCUMENT:
            /*Do nothing */
            break;

        case AXIOM_XML_READER_START_ELEMENT:
            val = axiom_stax_builder_create_om_element(
                        builder, env);
            if (!val)
                return -1;

            break;

        case AXIOM_XML_READER_EMPTY_ELEMENT:
            val = axiom_stax_builder_create_om_element(
                        builder, env);
            if (!val)
                return -1;
        case AXIOM_XML_READER_END_ELEMENT:
            axiom_stax_builder_end_element(builder, env);
            break;


        case AXIOM_XML_READER_SPACE:
            /* Do nothing */
            break;

        case AXIOM_XML_READER_CHARACTER:
            val = axiom_stax_builder_create_om_text(builder, env);
            if (!val)
                return -1;
            break;

        case AXIOM_XML_READER_ENTITY_REFERENCE:
            break;

        case AXIOM_XML_READER_COMMENT:
            val = axiom_stax_builder_create_om_comment(builder, env);
            if (!val)
                return -1;
            axiom_stax_builder_end_element(builder, env);
            break;

        case AXIOM_XML_READER_PROCESSING_INSTRUCTION:
            val = axiom_stax_builder_create_om_processing_instruction(
                        builder , env);
            if (!val)
                return -1;
            axiom_stax_builder_end_element(builder, env);
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


