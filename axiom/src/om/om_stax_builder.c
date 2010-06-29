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
#include "axiom_element_internal.h"
#include <axiom_text.h>
#include <axiom_processing_instruction.h>
#include <axiom_comment.h>
#include <axutil_string.h>
#include <axiom_xml_writer.h>
#include <axiom_doctype.h>
#include "axiom_node_internal.h"
#include "axiom_stax_builder_internal.h"
#include <axiom_soap_builder_internal.h>
#include "axiom_document_internal.h"

struct axiom_stax_builder
{
    /** pull parser instance used by the om_builder */
    axiom_xml_reader_t *parser;

    /** last node the om_builder found */
    axiom_node_t *lastnode;

    /** root node of the xml document */
    axiom_node_t *root_node;

    /** document associated with the om_builder */
    axiom_document_t *document;

    /** done building the document? */
    axis2_bool_t done;

    /** current event */
    int current_event;

    /** Indicate the  current element level. */
    int element_level;

    /** reference to the soap builder, to build soap releated elements */
    axiom_soap_builder_t *soap_builder;

    axutil_hash_t *declared_namespaces;
};

/**
 * Creates an stax builder
 * @param environment Environment. MUST NOT be NULL.
 * @param parser parser to be used with builder. The builder will take ownership of the parser.
 * @return a pointer to the newly created builder struct.
 */
AXIS2_EXTERN axiom_stax_builder_t *AXIS2_CALL
axiom_stax_builder_create(
    const axutil_env_t * env,
    axiom_xml_reader_t * parser)
{
    axiom_stax_builder_t *om_builder = NULL;

    if(!parser)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Invalid xml parser given for stax builder");
        return NULL;
    }

    om_builder = (axiom_stax_builder_t *)AXIS2_MALLOC(env->allocator, sizeof(axiom_stax_builder_t));
    if(!om_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Insufficient memory to create stax builder");
        return NULL;
    }

    om_builder->document = axiom_document_create(env, NULL, om_builder);
    if(!om_builder->document)
    {
        AXIS2_FREE(env->allocator, om_builder);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create axiom document");
        return NULL;
    }

    om_builder->declared_namespaces = axutil_hash_make(env);
    if(!om_builder->declared_namespaces)
    {
        AXIS2_FREE(env->allocator, om_builder);
        axiom_document_free(om_builder->document, env);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create namespace hashmap");
        return NULL;
    }

    om_builder->done = AXIS2_FALSE;
    om_builder->lastnode = NULL;
    om_builder->parser = parser;
    om_builder->current_event = -1;
    om_builder->root_node = NULL;
    om_builder->element_level = 0;
    om_builder->soap_builder = NULL;
    return om_builder;
}

/**
 * Free the build struct instance and its associated document,axiom tree.
 * @param builder pointer to builder struct
 * @param env environment, MUST NOT be NULL
 * @return status of the operation AXIS2_SUCCESS on success, AXIS2_FAILURE on error.
 */
AXIS2_EXTERN void AXIS2_CALL
axiom_stax_builder_free(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    axutil_hash_free(om_builder->declared_namespaces, env);
    axiom_document_free(om_builder->document, env);
    axiom_xml_reader_free(om_builder->parser, env);
    AXIS2_FREE(env->allocator, om_builder);
}

/* this will be called from root node to free the stax builder */
void AXIS2_CALL
axiom_stax_builder_free_internal(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
	if(om_builder)
	{
		axutil_hash_free(om_builder->declared_namespaces, env);
		axiom_xml_reader_free(om_builder->parser, env);
		axiom_document_free_self(om_builder->document, env);
		AXIS2_FREE(env->allocator, om_builder);
	}
}

/**
 * Give the responsibility of freeing the stax builder to the root node. This method will not
 * free anything. When the root node is freed, it will call axiom_stax_builder_free_internal
 * which will free the stax builder, document and xml reader.
 * @param builder pointer to builder struct
 * @param env environment, MUST NOT be NULL
 * @return status of the operation AXIS2_SUCCESS on success, AXIS2_FAILURE on error.
 */
AXIS2_EXTERN void AXIS2_CALL
axiom_stax_builder_free_self(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    if(om_builder->root_node)
    {
        axiom_node_assume_builder_ownership(om_builder->root_node, env);
    }
	else
	{
		axiom_stax_builder_free_internal(om_builder, env);
	}
}

/**
 * Gets the document associated with the builder
 * @param builder axiom_stax_builder
 * @param env environment
 * @return pointer to document struct associated with builder NULL if an error occurred.
 */
AXIS2_EXTERN axiom_document_t *AXIS2_CALL
axiom_stax_builder_get_document(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    return om_builder->document;
}

static axis2_status_t
axiom_stax_builder_process_attributes(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env,
    axiom_node_t * element_node)
{
    int i = 0;
    int attribute_count;
    axiom_element_t *om_ele = NULL;

    om_ele = (axiom_element_t *)axiom_node_get_data_element(element_node, env);
    attribute_count = axiom_xml_reader_get_attribute_count(om_builder->parser, env);
    for(i = 1; i <= attribute_count; ++i)
    {
        axiom_namespace_t *ns = NULL;
        axis2_char_t *uri = NULL;
        axis2_char_t *prefix = NULL;
        axis2_char_t *attr_name = NULL;
        axis2_char_t *attr_value = NULL;

        uri = axiom_xml_reader_get_attribute_namespace_by_number(om_builder-> parser, env, i);
        prefix = axiom_xml_reader_get_attribute_prefix_by_number(om_builder->parser, env, i);
        if(uri && (axutil_strcmp(uri, "") != 0))
        {
            ns = axiom_element_find_namespace(om_ele, env, element_node, uri, prefix);
            if(!ns)
            {
                /* if namespace is not defined yet, create it */
                ns = axiom_namespace_create(env, uri, prefix);
            }
        }
        if(uri)
        {
            axiom_xml_reader_xml_free(om_builder->parser, env, uri);
        }
        if(prefix)
        {
            axiom_xml_reader_xml_free(om_builder->parser, env, prefix);
        }

        attr_name = axiom_xml_reader_get_attribute_name_by_number(om_builder->parser, env, i);
        attr_value = axiom_xml_reader_get_attribute_value_by_number(om_builder->parser, env, i);

        if(attr_name)
        {
            axutil_string_t *attr_name_str = NULL;
            axutil_string_t *attr_value_str = NULL;
            axiom_attribute_t *attribute = NULL;

#ifdef AXIS2_LIBXML2_ENABLED
            attr_name_str = axutil_string_create(env, attr_name);
            attr_value_str = axutil_string_create(env, attr_value);
            axiom_xml_reader_xml_free(om_builder->parser, env, attr_name);
            axiom_xml_reader_xml_free(om_builder->parser, env, attr_value);
#else
            attr_name_str = axutil_string_create_assume_ownership(env, &attr_name);
            attr_value_str = axutil_string_create_assume_ownership(env, &attr_value);
#endif

            attribute = axiom_attribute_create_str(env, attr_name_str, attr_value_str, ns);
            axutil_string_free(attr_name_str, env);
            axutil_string_free(attr_value_str, env);

            if(!attribute)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create attribute");
                return AXIS2_FAILURE;
            }
            if(axiom_element_add_attribute(om_ele, env, attribute, element_node) != AXIS2_SUCCESS)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot add attribute to element");
                return AXIS2_FAILURE;
            }
        }
    }
    return AXIS2_SUCCESS;
}

static axiom_node_t *
axiom_stax_builder_create_om_text(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    axis2_char_t *temp_value = NULL;
    axutil_string_t *temp_value_str = NULL;
    axiom_node_t *node = NULL;
    axiom_node_t *parent = om_builder->lastnode;

    if(!parent)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create OM Text without a node");
        return NULL;
    }

    temp_value = axiom_xml_reader_get_value(om_builder->parser, env);
    if(!temp_value)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_VALUE_NULL, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Invalid OM Text value");
        return NULL;
    }

#ifdef AXIS2_LIBXML2_ENABLED
    temp_value_str = axutil_string_create(env, temp_value);
    axiom_xml_reader_xml_free(om_builder->parser, env, temp_value);
#else
    temp_value_str = axutil_string_create_assume_ownership(env, &temp_value);
#endif

    if(!temp_value_str)
    {
        /* axutil_string_create will have set an error number */
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create OM Text value");
        return NULL;
    }

    if(axiom_node_is_complete(parent, env))
    {
        parent = axiom_node_get_parent(om_builder->lastnode, env);
    }

    axiom_text_create_str(env, parent, temp_value_str, &node);
    axutil_string_free(temp_value_str, env);

    if(!node)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create axiom_text");
        return NULL;
    }
    axiom_node_set_builder(node, env, om_builder);
    axiom_node_set_complete(node, env, AXIS2_TRUE);
    om_builder->lastnode = node;
    return node;
}

static axis2_status_t
axiom_stax_builder_process_namespaces(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env,
    axiom_node_t * node,
    int is_soap_element)
{
    axis2_status_t status = AXIS2_SUCCESS;
    int namespace_count = 0;
    axiom_namespace_t *om_ns = NULL;
    axiom_element_t *om_ele = NULL;

    /* temp values */
    axis2_char_t *tmp_prefix = NULL;
    axis2_char_t *tmp_ns_prefix = NULL;
    axis2_char_t *tmp_ns_uri = NULL;
    axutil_string_t *tmp_ns_prefix_str = NULL;
    axutil_string_t *tmp_ns_uri_str = NULL;

    int i = 0;
    om_ele = (axiom_element_t *)axiom_node_get_data_element(node, env);

    namespace_count = axiom_xml_reader_get_namespace_count(om_builder->parser, env);
    for(i = 1; i <= namespace_count; ++i)
    {
        tmp_ns_prefix = axiom_xml_reader_get_namespace_prefix_by_number(om_builder->parser, env, i);
        tmp_ns_uri = axiom_xml_reader_get_namespace_uri_by_number(om_builder->parser, env, i);

#ifdef AXIS2_LIBXML2_ENABLED
        tmp_ns_prefix_str = axutil_string_create(env, tmp_ns_prefix);
        tmp_ns_uri_str = axutil_string_create(env, tmp_ns_uri);
#else
        tmp_ns_prefix_str = axutil_string_create_assume_ownership(env, &tmp_ns_prefix);
        tmp_ns_uri_str = axutil_string_create_assume_ownership(env, &tmp_ns_uri);
#endif

        if(!tmp_ns_prefix || axutil_strcmp(tmp_ns_prefix, "xmlns") == 0)
        {
            /* default namespace case. !temp_ns_prefix is for guththila */

            if(tmp_ns_prefix_str)
            {
                axutil_string_free(tmp_ns_prefix_str, env);
            }

            tmp_ns_prefix_str = axutil_string_create(env, "");
            om_ns = axiom_namespace_create_str(env, tmp_ns_uri_str, tmp_ns_prefix_str);
            if(!om_ns)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create namespace");
                return AXIS2_FAILURE;
            }

            status = axiom_element_declare_namespace(om_ele, env, node, om_ns);
            if(!status)
            {
                axiom_namespace_free(om_ns, env);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error when declaring namespace");
                return AXIS2_FAILURE;
            }
        }
        else
        {
            axis2_char_t *prefix = NULL;
            om_ns = axiom_namespace_create_str(env, tmp_ns_uri_str, tmp_ns_prefix_str);
            if(!om_ns)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create namespace");
                return AXIS2_FAILURE;
            }

            status = axiom_element_declare_namespace(om_ele, env, node, om_ns);
            prefix = axiom_namespace_get_prefix(om_ns, env);
            axutil_hash_set(om_builder->declared_namespaces, prefix, AXIS2_HASH_KEY_STRING, om_ns);
        }

        axutil_string_free(tmp_ns_uri_str, env);
        axutil_string_free(tmp_ns_prefix_str, env);
#ifdef AXIS2_LIBXML2_ENABLED
        axiom_xml_reader_xml_free(om_builder->parser, env, tmp_ns_uri);
        axiom_xml_reader_xml_free(om_builder->parser, env, tmp_ns_prefix);
#endif
    }

    /* set own namespace */
    tmp_prefix = axiom_xml_reader_get_prefix(om_builder->parser, env);
    if(tmp_prefix)
    {
        om_ns = axutil_hash_get(om_builder->declared_namespaces, tmp_prefix, AXIS2_HASH_KEY_STRING);

        if(om_ns)
        {
            axiom_element_set_namespace(om_ele, env, om_ns, node);
        }
        else
        {
            AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error when setting namespace");
            return AXIS2_FAILURE;
        }

        axiom_xml_reader_xml_free(om_builder->parser, env, tmp_prefix);
    }

    return status;
}

static axiom_node_t *
axiom_stax_builder_create_om_element(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env,
    axis2_bool_t is_empty)
{
    axiom_node_t *element_node = NULL;
    axiom_element_t *om_ele = NULL;
    axis2_char_t *temp_localname = NULL;
    axiom_node_t *parent = NULL;

    temp_localname = axiom_xml_reader_get_name(om_builder->parser, env);
    if(!temp_localname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot find name of the element");
        return NULL;
    }

    om_builder->element_level++;
    if(om_builder->lastnode)
    {
        if(axiom_node_is_complete(om_builder->lastnode, env))
        {
            /*previous node building is finished. This node should be a sibling of previous node */
            parent = axiom_node_get_parent(om_builder->lastnode, env);
        }
        else
        {
            /*previous node building is not finished. This should be child of previous node */
            parent = om_builder->lastnode;
        }
    }

    om_ele = axiom_element_create(env, parent, temp_localname, NULL, &element_node);
    axiom_xml_reader_xml_free(om_builder->parser, env, temp_localname);
    if((!om_ele) || (!element_node))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot create axiom element");
        return NULL;
    }
    axiom_node_set_builder(element_node, env, om_builder);
    axiom_element_set_is_empty(om_ele, env, is_empty);

    if(!om_builder->lastnode)
    {
        /* since last node is null, this should be the root node */
        om_builder->root_node = element_node;
        axiom_document_set_root_element(om_builder->document, env, element_node);
    }

    /* order of processing name spaces first (before processing attributes) is important */
    axiom_stax_builder_process_namespaces(om_builder, env, element_node, 0);
    axiom_stax_builder_process_attributes(om_builder, env, element_node);

    om_builder->lastnode = element_node;
    return element_node;
}

static axiom_node_t *
axiom_stax_builder_create_om_comment(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    axiom_node_t *comment_node = NULL;
    axis2_char_t *comment_value = NULL;
    axiom_node_t *parent = NULL;

    comment_value = axiom_xml_reader_get_value(om_builder->parser, env);
    if(!comment_value)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error in reading comment");
        return NULL;
    }

    if(axiom_node_is_complete(om_builder->lastnode, env))
    {
        /* Last node is completed means, this node should be a sibling of last node */
        parent = axiom_node_get_parent(om_builder->lastnode, env);
    }
    else
    {
        /* this node should be a child of last node */
        parent = om_builder->lastnode;
    }

    axiom_comment_create(env, parent, comment_value, &comment_node);
    axiom_xml_reader_xml_free(om_builder->parser,env,comment_value);
    if(!comment_node)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to create axiom element");
        return NULL;
    }

    axiom_node_set_builder(comment_node, env, om_builder);
    om_builder->element_level++;
    om_builder->lastnode = comment_node;

    return comment_node;
}


static axiom_node_t *
axiom_stax_builder_create_om_processing_instruction(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    axiom_node_t *pi_node = NULL;
    axis2_char_t *target = NULL;
    axis2_char_t *value = NULL;

    target = axiom_xml_reader_get_pi_target(om_builder->parser, env);
    value = axiom_xml_reader_get_pi_data(om_builder->parser, env);
    if(!target)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL, AXIS2_FAILURE);
        return NULL;
    }
    if(!om_builder->lastnode)
    {
        /* do nothing */
        axiom_xml_reader_xml_free(om_builder->parser, env, target);
        axiom_xml_reader_xml_free(om_builder->parser, env, value);
        return NULL;
    }
    else if(axiom_node_is_complete(om_builder->lastnode, env)
        || (axiom_node_get_node_type(om_builder->lastnode, env) == AXIOM_TEXT))
    {
        axiom_processing_instruction_create(env, axiom_node_get_parent(om_builder-> lastnode, env),
            target, value, &pi_node);

        axiom_node_set_next_sibling(om_builder->lastnode, env, pi_node);
        axiom_node_set_previous_sibling(pi_node, env, om_builder->lastnode);
    }
    else
    {
        axiom_processing_instruction_create(env, om_builder->lastnode, target, value, &pi_node);
        axiom_node_set_first_child(om_builder->lastnode, env, pi_node);
        axiom_node_set_parent(pi_node, env, om_builder->lastnode);
    }
    axiom_node_set_builder(pi_node, env, om_builder);
    om_builder->element_level++;

    if(target)
    {
        axiom_xml_reader_xml_free(om_builder->parser, env, target);
    }
    if(value)
    {
        axiom_xml_reader_xml_free(om_builder->parser, env, value);
    }

    om_builder->lastnode = pi_node;
    return pi_node;
}

static axis2_status_t
axiom_stax_builder_end_element(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    /* if last node is not set, that means end_element is called before start_element,
     * which is an error */
    if(!om_builder->lastnode)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Invalid XML. End element is received before start element.");
        return AXIS2_FAILURE;
    }

    om_builder->element_level--;
    if(axiom_node_is_complete(om_builder->lastnode, env))
    {
        /* Last node completed means, this end element should be parent of the last node. */
        axiom_node_t *parent = axiom_node_get_parent(om_builder->lastnode, env);
        if(parent)
        {
            axiom_node_set_complete(parent, env, AXIS2_TRUE);
            om_builder->lastnode = parent;
        }
    }
    else
    {
        axiom_node_set_complete((om_builder->lastnode), env, AXIS2_TRUE);
    }

    /* if we finish building the root node, then we can set the complete status of om_builder */
    if(axiom_node_is_complete(om_builder->root_node, env))
    {
        om_builder->done = AXIS2_TRUE;
    }

    return AXIS2_SUCCESS;
}

/**
  * moves the reader to next event and returns the token returned by the xml_reader ,
  * @param builder pointer to STAX builder struct to be used
  * @param environment Environment. MUST NOT be NULL.
  * @return next event axiom_xml_reader_event_types. Returns -1 on error
  */
int AXIS2_CALL
axiom_stax_builder_next_with_token(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    int token = 0;

    if(om_builder->done)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL, AXIS2_FAILURE);
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "OM is fully built. Nothing more to build");
        return -1;
    }

    token = axiom_xml_reader_next(om_builder->parser, env);
    om_builder->current_event = token;

    if(token == -1)
    {
        om_builder->done = AXIS2_TRUE;
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL, AXIS2_FAILURE);
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "-1 returned from the xml reader when reading xml");
        return -1;
    }

    switch(token)
    {
        case AXIOM_XML_READER_START_DOCUMENT:
        {
            /*Do nothing */
            break;
        }
        case AXIOM_XML_READER_START_ELEMENT:
        {
            if(!axiom_stax_builder_create_om_element(om_builder, env, AXIS2_FALSE))
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error in creating start element");
                return -1;
            }
            break;
        }
        case AXIOM_XML_READER_EMPTY_ELEMENT:
        {
            axis2_bool_t is_empty = AXIS2_TRUE;
#ifdef AXIS2_LIBXML2_ENABLED
            is_empty = AXIS2_FALSE;
#endif
            if(!axiom_stax_builder_create_om_element(om_builder, env, is_empty))
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error in creating empty element");
                return -1;
            }
            /* Note that we don't have a break here.
             * Let this to fall to AXIOM_XML_READER_END_ELEMENT case as well, since empty element
             * = start element logic + end element logic */
        }
        case AXIOM_XML_READER_END_ELEMENT:
        {
            if(axiom_stax_builder_end_element(om_builder, env) != AXIS2_SUCCESS)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error in processing end element");
                return -1;
            }
            break;
        }
        case AXIOM_XML_READER_SPACE:
        {
            /* ignore white space before the root element */
            if(om_builder->lastnode)
            {
                if(!axiom_stax_builder_create_om_text(om_builder, env))
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error in creating axiom text");
                    return -1;
                }
            }
            break;
        }
        case AXIOM_XML_READER_CHARACTER:
        {
            if(!axiom_stax_builder_create_om_text(om_builder, env))
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error in creating axiom text");
                return -1;
            }
            break;
        }
        case AXIOM_XML_READER_ENTITY_REFERENCE:
        {
            break;
        }
        case AXIOM_XML_READER_COMMENT:
        {
            /* if the comment is at the root level, we will omit it */
            if(om_builder->lastnode)
            {
                axis2_status_t status = AXIS2_FAILURE;
                if(axiom_stax_builder_create_om_comment(om_builder, env))
                {
                    status = axiom_stax_builder_end_element(om_builder, env);
                }

                if(status != AXIS2_SUCCESS)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error in creating axiom comment");
                    return -1;
                }
            }
            break;
        }
        case AXIOM_XML_READER_PROCESSING_INSTRUCTION:
        {
            if(axiom_stax_builder_create_om_processing_instruction(om_builder, env))
            {
                axiom_stax_builder_end_element(om_builder, env);
            }
           break;
        }
        case AXIOM_XML_READER_CDATA:
        case AXIOM_XML_READER_DOCUMENT_TYPE:
        default:
            break;
    }

    /* if stax builder is also a soap builder, build soap related elements */
    if(om_builder->soap_builder &&
        (token == AXIOM_XML_READER_START_ELEMENT || token == AXIOM_XML_READER_EMPTY_ELEMENT))
    {
        AXIS2_ASSERT(om_builder->lastnode != NULL);
        if(axiom_soap_builder_construct_node(om_builder->soap_builder, env, om_builder->lastnode)
            != AXIS2_SUCCESS)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error occurred when building soap node");
            om_builder->done = AXIS2_TRUE;
            return -1;
        }
    }
    return token;
}

/**
 internal function for soap om_builder only
 */
int AXIS2_CALL
axiom_stax_builder_get_current_event(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    return om_builder->current_event;
}

/**
 internal function for soap om_builder only
 */
axiom_node_t *AXIS2_CALL
axiom_stax_builder_get_lastnode(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    return om_builder->lastnode;
}

/**
 internal function for soap om_builder only
 */
axis2_bool_t AXIS2_CALL
axiom_stax_builder_is_complete(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    return om_builder->done;
}

/**
 internal function for soap om_builder only
 */
void AXIS2_CALL
axiom_stax_builder_set_lastnode(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env,
    axiom_node_t * om_node)
{
    om_builder->lastnode = om_node;
}

/**
 internal function for soap om_builder only
 */
int AXIS2_CALL
axiom_stax_builder_get_element_level(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    return om_builder->element_level;
}

/**
 internal function for soap om_builder only
 */
void AXIS2_CALL
axiom_stax_builder_set_element_level(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env,
    int element_level)
{
    om_builder->element_level = element_level;
}

void AXIS2_CALL
axiom_stax_builder_set_soap_builder(
    axiom_stax_builder_t *om_builder,
    const axutil_env_t *env,
    axiom_soap_builder_t *soap_builder)
{
    om_builder->soap_builder = soap_builder;
}

axiom_node_t *AXIS2_CALL
axiom_stax_builder_get_root_node(
    axiom_stax_builder_t *om_builder,
    const axutil_env_t * env)
{
    return om_builder->root_node;
}
#if 0
static axiom_node_t *
axiom_stax_builder_create_om_doctype(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    axiom_node_t *doctype_node = NULL;
    axis2_char_t *doc_value = NULL;

    doc_value = axiom_xml_reader_get_dtd(om_builder->parser, env);
    if(!doc_value)
    {
        return NULL;
    }
    if(!(om_builder->lastnode))
    {
        axiom_doctype_create(env, NULL, doc_value, &doctype_node);
        if(om_builder->document)
        {
            axiom_document_set_root_element(om_builder->document, env, doctype_node);
        }
    }
    om_builder->lastnode = doctype_node;
    axiom_xml_reader_xml_free(om_builder->parser, env, doc_value);
    return doctype_node;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_stax_builder_discard_current_element(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    axiom_node_t *element = NULL;
    axiom_node_t *prev_node = NULL;
    axiom_node_t *parent = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    element = om_builder->lastnode;

    if(axiom_node_is_complete(element, env) || !(om_builder->cache))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    om_builder->cache = AXIS2_FALSE;
    do
    {
        while(axiom_xml_reader_next(om_builder->parser, env) != AXIOM_XML_READER_END_ELEMENT)
            ;
    }
    while(!(axiom_node_is_complete(element, env)));

    /*All children of this element is pulled now */

    prev_node = axiom_node_get_previous_sibling(element, env);
    if(prev_node)
    {
        axiom_node_free_tree(axiom_node_get_next_sibling(prev_node, env), env);
        axiom_node_set_next_sibling(prev_node, env, NULL);
    }
    else
    {
        parent = axiom_node_get_parent(element, env);
        axiom_node_free_tree(axiom_node_get_first_child(parent, env), env);
        axiom_node_set_first_child(parent, env, NULL);
        om_builder->lastnode = parent;
    }
    om_builder->cache = AXIS2_TRUE;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_xml_reader_t *AXIS2_CALL
axiom_stax_builder_get_parser(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    return om_builder->parser;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_stax_builder_set_cache(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env,
    axis2_bool_t enable_cache)
{
    om_builder->cache = enable_cache;
}

axiom_node_t *AXIS2_CALL
axiom_stax_builder_next(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{
    int token = 0;
    axiom_node_t *node = NULL;

    do
    {
        if(om_builder->done)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL, AXIS2_FAILURE);
            return NULL;
        }

        token = axiom_xml_reader_next(om_builder->parser, env);
        if(token == -1)
        {
            return NULL;
        }

        om_builder->current_event = token;

        switch(token)
        {
            case AXIOM_XML_READER_START_DOCUMENT:
                /*Do nothing */
                break;

            case AXIOM_XML_READER_START_ELEMENT:
                node = axiom_stax_builder_create_om_element(om_builder, env, AXIS2_FALSE);
                break;

            case AXIOM_XML_READER_EMPTY_ELEMENT:

#ifdef AXIS2_LIBXML2_ENABLED
                node = axiom_stax_builder_create_om_element(om_builder, env, AXIS2_FALSE);
#else
                node = axiom_stax_builder_create_om_element(om_builder, env, AXIS2_TRUE);
#endif

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

                node = axiom_stax_builder_create_om_processing_instruction(om_builder, env);
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
    while(!node && !axiom_node_is_complete(om_builder->root_node, env));
    return node;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_stax_builder_free_self_original(
    axiom_stax_builder_t * om_builder,
    const axutil_env_t * env)
{

    axiom_node_t *temp_node = NULL;
    axiom_node_t *nodes[256];
    axiom_node_t *om_node = NULL;
    int count = 0;

    om_node = om_builder->root_node;

    nodes[count++] = om_node;

    if(om_node)
    {
        do
        {

            axiom_node_set_builder(om_node, env, NULL);

            temp_node = axiom_node_get_first_child(om_node, env);
            if(temp_node)
            {
                /* process the child node */
                om_node = temp_node;
                nodes[count++] = om_node;
            }
            else
            {
                /* no child node. check whether current node has any siblings */
                temp_node = axiom_node_get_next_sibling(om_node, env);
                if(temp_node)
                {
                    /*  process sibling nodes */
                    om_node = temp_node;
                    nodes[count - 1] = om_node;
                }
                else
                {
                    /* no siblings. so fallback to parent of current node and process */
                    while(count > 1 && !temp_node)
                    {
                        count--;
                        om_node = nodes[count - 1];
                        temp_node = axiom_node_get_next_sibling(om_node, env);
                    }
                    if(temp_node && count > 1)
                    {
                        om_node = temp_node;
                        nodes[count - 1] = om_node;
                    }
                    else
                    {
                        count--;
                    }
                }
            }
        }
        while(count > 0);
    }

    axutil_hash_free(om_builder->declared_namespaces, env);
    axiom_xml_reader_free(om_builder->parser, env);
    axiom_document_free_self(om_builder->document, env);
    AXIS2_FREE(env->allocator, om_builder);
}
#endif
