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

#include <axiom_document_internal.h>
#include <axiom_stax_builder_internal.h>
#include <axutil_string.h>

struct axiom_document
{
    /** root element */
    axiom_node_t *root_element;

    /** builder of the document */
    struct axiom_stax_builder *builder;

};

axiom_document_t *AXIS2_CALL
axiom_document_create(
    const axutil_env_t * env,
    axiom_node_t * root,
    axiom_stax_builder_t * builder)
{
    axiom_document_t *document = NULL;
    document = (axiom_document_t *)AXIS2_MALLOC(env->allocator, sizeof(axiom_document_t));
    if(!document)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Insufficient memory to create axiom document");
        return NULL;
    }

    document->builder = builder;
    document->root_element = root;
    return document;
}

void AXIS2_CALL
axiom_document_free(
    axiom_document_t * document,
    const axutil_env_t * env)
{
    if(document->root_element)
    {
        axiom_node_free_tree(document->root_element, env);
    }
    AXIS2_FREE(env->allocator, document);
}

void AXIS2_CALL
axiom_document_free_self(
    axiom_document_t * document,
    const axutil_env_t * env)
{
    AXIS2_FREE(env->allocator, document);
}

axis2_status_t AXIS2_CALL
axiom_document_set_root_element(
    axiom_document_t * document,
    const axutil_env_t * env,
    axiom_node_t * node)
{
    if(document->root_element)
    {
        axiom_node_free_tree(document->root_element, env);
    }

    document->root_element = node;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_node_t *AXIS2_CALL
axiom_document_get_root_element(
    axiom_document_t * document,
    const axutil_env_t * env)
{
    if(!document->root_element)
    {
        /* force to build the root node */
        int token;
        do{
            token = axiom_stax_builder_next_with_token(document->builder, env);
            if(token == -1)
            {
                AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_INVALID_DOCUMENT_STATE_ROOT_NULL, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to get root node");
                return NULL;
            }
        }while(token != AXIOM_XML_READER_START_ELEMENT && !document->root_element);
    }

    return document->root_element;
}

AXIS2_EXTERN axiom_node_t *AXIS2_CALL
axiom_document_build_all(
    axiom_document_t *document,
    const axutil_env_t *env)
{
    do
    {
        int token = axiom_stax_builder_next_with_token(document->builder, env);
        if((token == -1) && (!axiom_node_is_complete(document->root_element, env)))
        {
            /* if returned token is "invalid token" and root node is not fully completed,
             * this means there is an error occurred */
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Error occurred when building all nodes of document");
            return NULL;
        }
    }
    while(document->root_element && !axiom_node_is_complete(document->root_element, env));

    return document->root_element;
}

AXIS2_EXTERN axiom_node_t *AXIS2_CALL
axiom_document_build_next(
    axiom_document_t * document,
    const axutil_env_t * env)
{
    axiom_node_t *return_node = NULL;
    axiom_node_t *last_node = axiom_stax_builder_get_lastnode(document->builder, env);
    while(document->root_element && !axiom_node_is_complete(document->root_element, env))
    {
        int token = axiom_stax_builder_next_with_token(document->builder, env);
        if(token == -1)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed to create next node");
            return_node = NULL;
            break;
        }
        return_node = axiom_stax_builder_get_lastnode(document->builder, env);
        if(last_node != return_node)
        {
            /* this is a new node created */
            break;
        }
    }

    return return_node;
}

#if 0
AXIS2_EXTERN axiom_stax_builder_t *AXIS2_CALL
axiom_document_get_builder(
    axiom_document_t * document,
    const axutil_env_t * env)
{
    return document->builder;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_document_set_builder(
    axiom_document_t * document,
    const axutil_env_t * env,
    axiom_stax_builder_t * builder)
{
    document->builder = builder;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_document_serialize(
    axiom_document_t * document,
    const axutil_env_t * env,
    axiom_output_t * om_output)
{
    if(!document)
        return AXIS2_FAILURE;

    if(!(document->root_element))
    {
        axiom_document_get_root_element(document, env);
    }
    if(document->root_element)
    {
        return axiom_node_serialize(document->root_element, env, om_output);
    }
    else
    {
        return AXIS2_FAILURE;
    }
}

axiom_node_t *AXIS2_CALL
axiom_document_build_next(
    axiom_document_t * document,
    const axutil_env_t * env)
{
    if(document->root_element && axiom_node_is_complete(document->root_element, env))
    {
        return NULL; /* Nothing wrong but done with pulling */
    }

    return axiom_stax_builder_next(document->builder, env);
}
#endif
