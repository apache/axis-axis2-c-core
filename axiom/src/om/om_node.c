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

#include "axiom_node_internal.h"
#include <axiom_element.h>
#include <axiom_text.h>
#include <axiom_data_source.h>
#include <axiom_comment.h>
#include <axiom_processing_instruction.h>
#include <axiom_doctype.h>
#include <axiom_document.h>
#include <axiom_stax_builder.h>

struct axiom_node
{
    /** document only availble if build through builder */
    struct axiom_document *om_doc;

    axiom_stax_builder_t *builder;
    /** parent node */
    axiom_node_t *parent;
    /** previous sibling */
    axiom_node_t *prev_sibling;
    /** next sibling */
    axiom_node_t *next_sibling;
    /** first child */
    axiom_node_t *first_child;
    /** last child */
    axiom_node_t *last_child;
    /** node type, indicates the type stored in data_element */
    axiom_types_t node_type;
    /** done true means that this node is completely built , false otherwise */
    int done;

    /** instances of an om struct, whose type is defined by node type */
    void *data_element;

};

AXIS2_EXTERN axiom_node_t * AXIS2_CALL
axiom_node_create(const axis2_env_t *env)
{
    axiom_node_t *node = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    node = (axiom_node_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axiom_node_t));
    if (!node)
    {
        env->error->error_number = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
   
    node->first_child = NULL;
    node->last_child = NULL;
    node->next_sibling = NULL;
    node->prev_sibling = NULL;
    node->parent = NULL;
    node->node_type = AXIOM_INVALID;
    node->done = AXIS2_FALSE;
    node->data_element = NULL;
    node->om_doc = NULL;
    node->builder = NULL;

    return node;
}

/**
 *  This free fucntion will free an om_element and all the children contained in it
 *  before calling this function
*/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_free(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    axiom_node_t *child_node = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (!om_node)
        return AXIS2_FAILURE;

    if (om_node->first_child)
    {
        while ((om_node->first_child))
        {
            child_node = AXIOM_NODE_DETACH(om_node->first_child, env);
            if (child_node)
            {
                AXIOM_NODE_FREE_TREE(child_node , env);
                child_node = NULL;
            }
        }
    }

    if (om_node->node_type == AXIOM_ELEMENT)
    {
        if (om_node->data_element)
            AXIOM_ELEMENT_FREE((axiom_element_t*)(om_node->data_element), env);
    }
    else if (om_node->node_type == AXIOM_COMMENT)
    {
        if (om_node->data_element)
            AXIOM_COMMENT_FREE((axiom_comment_t*)(om_node->data_element), env);
    }
    else if (om_node->node_type == AXIOM_DOCTYPE)
    {
        /*AXIOM_DOCTYPE_FREE((axiom_doctype_t*)(om_node->data_element), env);*/
    }
    else if (om_node->node_type == AXIOM_PROCESSING_INSTRUCTION)
    {
        if (om_node->data_element)
            AXIOM_PROCESSING_INSTRUCTION_FREE(
                (axiom_processing_instruction_t*)(om_node->data_element), env);
    }
    else if (om_node->node_type == AXIOM_TEXT)
    {
        if (om_node->data_element)
            AXIOM_TEXT_FREE((axiom_text_t*)(om_node->data_element), env);
    }
    
    AXIS2_FREE(env->allocator, om_node);

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_add_child(axiom_node_t *om_node,
        const axis2_env_t *env,
        axiom_node_t *child)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, child, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_node, AXIS2_FAILURE);

    if (child->parent)
    {
        child = AXIOM_NODE_DETACH(child, env);
    }

    if (om_node->first_child  == NULL)
    {
        om_node->first_child = child;
    }
    else
    {
        axiom_node_t *last_sib = NULL;
        last_sib  = om_node->last_child;
        if (last_sib)
        {
            last_sib->next_sibling = child;
            child->prev_sibling = last_sib;
        }
    }

    child->parent  = om_node;
    om_node->last_child = child;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_node_detach(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    axiom_node_t *parent = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    parent = om_node->parent;

    AXIS2_PARAM_CHECK(env->error, parent, NULL);


    if (!(om_node->prev_sibling))
    {
        parent->first_child =
            om_node->next_sibling;
    }
    else
    {
        axiom_node_t *prev_sib = NULL;
        prev_sib  = om_node->prev_sibling;
        if (prev_sib)
        {
            prev_sib->next_sibling = om_node->next_sibling;
        }
    }

    if (om_node->next_sibling)
    {
        axiom_node_t *next_sibling = NULL;
        next_sibling = om_node->next_sibling;
        if (next_sibling)
            next_sibling->prev_sibling = om_node->prev_sibling;
    }

    if ((parent->last_child) &&
            ((parent->last_child) == om_node))
    {
        parent->last_child = om_node->prev_sibling;
    }

    om_node->parent = NULL;
    om_node->prev_sibling = NULL;
    om_node->next_sibling = NULL;
    om_node->builder = NULL;
    return om_node;

}
/**
Internal function , only used in om and soap
not to be used by users

*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_set_parent(axiom_node_t *om_node,
        const axis2_env_t *env,
        axiom_node_t * parent)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (!om_node)
        return AXIS2_FAILURE;

    AXIS2_PARAM_CHECK(env->error, parent, AXIS2_FAILURE);

    if (parent == om_node->parent)
    {   /* same parent already exist */
        return AXIS2_SUCCESS;
    }
    /* if a new parent is assigned in  place of existing
     *  one first the node should  be detached  
     */
    if (om_node->parent)
    {
        om_node =  axiom_node_detach(om_node, env);
    }

    om_node->parent = parent;

    return AXIS2_SUCCESS;
}


/**
 * This will insert a sibling just after the current information item
 * @param node the node in consideration
 * @param nodeto_insert the node that will be inserted
 */

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_insert_sibling_after(axiom_node_t * om_node,
        const axis2_env_t *env,
        axiom_node_t * node_to_insert)
{
    axiom_node_t *next_sib = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    AXIS2_PARAM_CHECK(env->error, node_to_insert, AXIS2_FAILURE);

    node_to_insert->parent =
        om_node->parent;

    node_to_insert->prev_sibling = om_node;

    next_sib = om_node->next_sibling;

    if (next_sib)
    {
        next_sib->prev_sibling = node_to_insert;
    }

    node_to_insert->next_sibling =
        om_node->next_sibling;

    om_node->next_sibling = node_to_insert;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_insert_sibling_before(axiom_node_t *om_node,
        const axis2_env_t *env,
        axiom_node_t * node_to_insert)
{
    axiom_node_t *prev_sibling = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, node_to_insert, AXIS2_FAILURE);

    node_to_insert->parent =
        om_node->parent ;

    node_to_insert->prev_sibling =
        om_node->prev_sibling;

    node_to_insert->next_sibling = om_node;

    prev_sibling = om_node->prev_sibling;

    if (!prev_sibling)
    {
        axiom_node_t *parent = om_node->parent;
        parent->first_child = node_to_insert;
    }
    else
    {
        axiom_node_t *prev_sibling = om_node->prev_sibling;
        if (prev_sibling)
            prev_sibling->next_sibling = node_to_insert;
    }
    om_node->prev_sibling = node_to_insert;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_serialize(axiom_node_t *om_node,
        const axis2_env_t *env,
        axiom_output_t *om_output)
{

    int status = AXIS2_SUCCESS;
    axiom_node_t *temp_node = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (!om_node)
        return AXIS2_SUCCESS;

    AXIS2_PARAM_CHECK(env->error, om_output, AXIS2_FAILURE);

    if (om_node->node_type == AXIOM_ELEMENT)
    {
        if (om_node->data_element)
        {
            status = AXIOM_ELEMENT_SERIALIZE_START_PART(
                        (axiom_element_t *)(om_node->data_element),
                        env,
                        om_output,
                        om_node);
        }
        if (status != AXIS2_SUCCESS)
            return status;
    }
    else if (om_node->node_type == AXIOM_DATA_SOURCE)
    {
        if (om_node->data_element)
        {
            status = axiom_data_source_serialize(
                        (axiom_data_source_t*)(om_node->data_element),
                        env, om_output);
        }
        if (status != AXIS2_SUCCESS)
            return status;
    }
    else if (om_node->node_type == AXIOM_TEXT)
    {
        if (om_node->data_element)
        {
            status = AXIOM_TEXT_SERIALIZE(
                        (axiom_text_t*)(om_node->data_element),
                        env, om_output);
        }
        if (status != AXIS2_SUCCESS)
            return status;
    }
    else if (om_node->node_type == AXIOM_COMMENT)
    {
        if (om_node->data_element)
        {
            status = AXIOM_COMMENT_SERIALIZE(
                        (axiom_comment_t*)(om_node->data_element),
                        env, om_output);
        }
        if (status != AXIS2_SUCCESS)
            return status;
    }
    else if (om_node->node_type == AXIOM_DOCTYPE)
    {
        if (om_node->data_element)
        {
            status = AXIOM_DOCTYPE_SERIALIZE(
                        (axiom_doctype_t*)(om_node->data_element),
                        env, om_output);
        }
        if (status != AXIS2_SUCCESS)
            return status;
    }
    else if (om_node->node_type == AXIOM_PROCESSING_INSTRUCTION)
    {
        if (om_node->data_element)
        {
            status = AXIOM_PROCESSING_INSTRUCTION_SERIALIZE(
                        (axiom_processing_instruction_t*)(om_node->data_element),
                        env, om_output);
        }

        if (status != AXIS2_SUCCESS)
            return status;
    }


    temp_node = axiom_node_get_first_child(om_node, env);
    /* serialize children of this node */
    while (temp_node)
    {
        status =  axiom_node_serialize(temp_node, env, om_output);
        temp_node = AXIOM_NODE_GET_NEXT_SIBLING(temp_node, env);
    }

    if (om_node->node_type == AXIOM_ELEMENT)
    {
        if (om_node->data_element)
        {
            status = AXIOM_ELEMENT_SERIALIZE_END_PART(
                        (axiom_element_t *)(om_node->data_element),
                        env, om_output);
        }
        if (status != AXIS2_SUCCESS)
            return status;
    }

    return status;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_node_get_parent(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return om_node->parent;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_node_get_first_child(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    int token = 0;
    if (! om_node)
        return NULL;

    AXIS2_ENV_CHECK(env, NULL);
    /**********************************************************/
    while (!(om_node->first_child) && !(om_node->done)
            && om_node->builder)
    {
        token = AXIOM_STAX_BUILDER_NEXT_WITH_TOKEN(om_node->builder, env);
        if (token == -1)
            return NULL;
    }
    /**********************************************************/
    return om_node->first_child;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_node_get_first_element(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    int token = 0;
	axiom_node_t *first_element;
    if (! om_node)
        return NULL;

    AXIS2_ENV_CHECK(env, NULL);
    /**********************************************************/
    while (!(om_node->first_child) && !(om_node->done)
            && om_node->builder)
    {
        token = AXIOM_STAX_BUILDER_NEXT_WITH_TOKEN(om_node->builder, env);
        if (token == -1)
            return NULL;
    }
    /**********************************************************/
	first_element = om_node->first_child;

	while (first_element && (AXIOM_NODE_GET_NODE_TYPE(first_element, env) != AXIOM_ELEMENT))
		first_element = AXIOM_NODE_GET_NEXT_SIBLING (first_element, env);

    return first_element;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_node_get_last_child(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return om_node->last_child;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_node_get_previous_sibling(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return om_node->prev_sibling;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axiom_node_get_next_sibling(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    int token = 0;

    if (!om_node)
        return NULL;

    AXIS2_ENV_CHECK(env, NULL);
    
    while (!(om_node->next_sibling) && om_node->parent &&
            om_node->builder && !(AXIOM_NODE_IS_COMPLETE(om_node->parent, env)))
    {
        token = AXIOM_STAX_BUILDER_NEXT_WITH_TOKEN(om_node->builder, env);
        if (token == -1)
            return NULL;
    }

    return om_node->next_sibling;
}

AXIS2_EXTERN axiom_types_t AXIS2_CALL
axiom_node_get_node_type(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIOM_INVALID);
    return om_node->node_type;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_node_is_complete(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return om_node->done;

}

AXIS2_EXTERN struct axiom_document* AXIS2_CALL
            axiom_node_get_document(axiom_node_t *om_node,
                    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return om_node->om_doc;
}

AXIS2_EXTERN void* AXIS2_CALL
axiom_node_get_data_element(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return om_node->data_element;
}

/**
  internal function , not to be used by users
  only sets the first_child link because this is needed by builder
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_set_first_child(axiom_node_t *om_node, const axis2_env_t *env,
        axiom_node_t *first_child)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_node, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, first_child, AXIS2_FAILURE);
    /** set the parent */
    axiom_node_set_parent(first_child, env, om_node);

    om_node->first_child = first_child;
    return AXIS2_SUCCESS;
}
/**
  internal function not to be used by users
  only sets the previous sibling link as it is needed by builders

*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_set_previous_sibling(axiom_node_t *om_node,
        const axis2_env_t *env,
        axiom_node_t *prev_sibling)
{

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_node, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, prev_sibling, AXIS2_FAILURE);

    om_node->prev_sibling = prev_sibling;
    return AXIS2_SUCCESS;
}

/**
  internal function, not to be used by users
  only sets the next sibling link;
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_set_next_sibling(axiom_node_t *om_node,
        const axis2_env_t *env,
        axiom_node_t *next_sibling)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_node, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, next_sibling, AXIS2_FAILURE);
    om_node->next_sibling = next_sibling;
    return AXIS2_SUCCESS;
}

/**
   internal function not to be used by users
   sets the node type only used in soap and om
*/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_set_node_type(axiom_node_t *om_node,
        const axis2_env_t *env,
        axiom_types_t type)
{

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_node, AXIS2_FAILURE);
    om_node->node_type = type;
    return AXIS2_SUCCESS;
}
/**
   internal function , not to be used by users
   only used in om and soap
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_set_data_element(axiom_node_t *om_node,
        const axis2_env_t *env,
        void* data_element)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_node, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, data_element, AXIS2_FAILURE);
    om_node->data_element = data_element;
    return AXIS2_SUCCESS;
}


/**
 internal function not to be used by users
 only sets the build status

*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_set_complete(axiom_node_t *om_node,
        const axis2_env_t *env,
        axis2_bool_t done)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_node, AXIS2_FAILURE);
    om_node->done = done;
    return AXIS2_SUCCESS;
}
/**
 internal function not to be used by users
 only used by om builder

*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_set_document(axiom_node_t *om_node,
        const axis2_env_t *env,
        struct axiom_document *om_doc)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_node, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_doc, AXIS2_FAILURE);
    om_node->om_doc = om_doc;
    return AXIS2_SUCCESS;
}

/**
 internal function only sets the builder reference ,
 should not be used by user

*/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_node_set_builder(axiom_node_t *om_node,
        const axis2_env_t *env,
        axiom_stax_builder_t* builder)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, builder, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_node, AXIS2_FAILURE);
    om_node->builder = builder;
    return AXIS2_SUCCESS;

}
/**
 * This is an internal function
 */
AXIS2_EXTERN axiom_stax_builder_t * AXIS2_CALL
axiom_node_get_builder(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    if (!om_node)
        return NULL;
    return om_node->builder;
}

AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axiom_node_to_string(axiom_node_t *om_node,
        const axis2_env_t *env)
{

    int status = AXIS2_SUCCESS;
    axiom_output_t *om_output = NULL;
    axiom_xml_writer_t *xml_writer = NULL;
    axis2_char_t *xml = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, om_node, NULL);

    xml_writer = axiom_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0,
            AXIS2_XML_PARSER_TYPE_BUFFER);
    if (!xml_writer)
        return NULL;

    om_output = axiom_output_create(env, xml_writer);
    if (!om_output)
    {
        AXIOM_XML_WRITER_FREE(xml_writer, env);
        return NULL;
    }
    status = AXIOM_NODE_SERIALIZE(om_node, env, om_output);
    if (status == AXIS2_SUCCESS)
    {
        xml = axis2_strdup((axis2_char_t*)AXIOM_XML_WRITER_GET_XML(xml_writer, env), env);
    }
    AXIOM_OUTPUT_FREE(om_output, env);
    return xml;
}
