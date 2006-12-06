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


#include <axiom_document.h>
#include <axiom_stax_builder.h>
#include <axis2_string.h>

axis2_status_t AXIS2_CALL
axiom_document_free(axiom_document_t * document,
        const axis2_env_t *env);

axiom_node_t * AXIS2_CALL
axiom_document_build_next(axiom_document_t *document,
        const axis2_env_t *env);

axiom_node_t * AXIS2_CALL
axiom_document_get_root_element(axiom_document_t *document,
        const axis2_env_t *env);

axis2_status_t  AXIS2_CALL
axiom_document_set_root_element(axiom_document_t *document,
        const axis2_env_t *env,
        axiom_node_t *node);

axiom_node_t* AXIS2_CALL
axiom_document_build_all(axiom_document_t *document,
        const axis2_env_t *env);

axiom_stax_builder_t* AXIS2_CALL
axiom_document_get_builder(axiom_document_t *document,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axiom_document_serialize(axiom_document_t *document,
        const axis2_env_t *env,
        axiom_output_t *om_output);

/********************************* end of function pointers ******************/

typedef struct axiom_document_impl_t
{
    axiom_document_t om_document;
    /** root element */
    axiom_node_t *root_element;
    /** last child */
    axiom_node_t *last_child;
    /** first child */
    axiom_node_t *first_child;
    /** done building the document */
    axis2_bool_t done;
    /** builder of the document */
    struct axiom_stax_builder *builder;
    /** char set encoding */
    axis2_char_t *char_set_encoding;
    /** XML version */
    axis2_char_t *xml_version;
}
axiom_document_impl_t;

/************************************ Macro ************************************/

#define AXIS2_INTF_TO_IMPL(document) ((axiom_document_impl_t*)document)

/*******************************************************************************/

AXIS2_EXTERN axiom_document_t * AXIS2_CALL
axiom_document_create(const axis2_env_t *env,
        axiom_node_t *root,
        axiom_stax_builder_t *builder)
{
    axiom_document_impl_t *document = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    document = (axiom_document_impl_t *) AXIS2_MALLOC(
                env->allocator, sizeof(axiom_document_impl_t));

    if (!document)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    document->builder = builder;
    document->root_element = root;
    document->first_child = root;
    document->last_child = root;
    document->xml_version = NULL;
    document->char_set_encoding = NULL;
    document->done = AXIS2_FALSE;
    document->om_document.ops = NULL;

    document->char_set_encoding = (axis2_char_t *) AXIS2_STRDUP(CHAR_SET_ENCODING, env);
    if (!document->char_set_encoding)
    {
        AXIS2_FREE(env->allocator, document);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    document->xml_version = (axis2_char_t *) AXIS2_STRDUP(XML_VERSION, env);
    if (!document->xml_version)
    {

        AXIS2_FREE(env->allocator, document->char_set_encoding);
        AXIS2_FREE(env->allocator, document);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    document->om_document.ops = (axiom_document_ops_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axiom_document_ops_t));
    if (!document->om_document.ops)
    {

        AXIS2_FREE(env->allocator, document->char_set_encoding);
        AXIS2_FREE(env->allocator, document->xml_version);
        AXIS2_FREE(env->allocator, document);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    document->om_document.ops->free = axiom_document_free;
    document->om_document.ops->build_next = axiom_document_build_next;
    document->om_document.ops->get_root_element = axiom_document_get_root_element;
    document->om_document.ops->set_root_element = axiom_document_set_root_element;
    document->om_document.ops->build_all = axiom_document_build_all;
    document->om_document.ops->get_builder = axiom_document_get_builder;
    document->om_document.ops->serialize = axiom_document_serialize;

    return &(document->om_document);
}

axis2_status_t AXIS2_CALL
axiom_document_free(axiom_document_t *om_document,
        const axis2_env_t *env)
{
    axiom_document_impl_t *document = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    document = AXIS2_INTF_TO_IMPL(om_document);

    if (document->char_set_encoding)
    {
        AXIS2_FREE(env->allocator, document->char_set_encoding);
        document->char_set_encoding = NULL;
    }
    if (document->xml_version)
    {
        AXIS2_FREE(env->allocator, document->xml_version);
        document->xml_version = NULL;
    }
    if (document->root_element)
    {

        AXIOM_NODE_FREE_TREE(document->root_element, env);
        document->root_element = NULL;
    }
    if (document->om_document.ops)
    {
        AXIS2_FREE(env->allocator, document->om_document.ops);
        document->om_document.ops = NULL;
    }
    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(document));
    return AXIS2_SUCCESS;
}


axiom_node_t* AXIS2_CALL
axiom_document_build_next(axiom_document_t *om_document,
        const axis2_env_t *env)
{
    axiom_document_impl_t *document = NULL;
    axiom_node_t  *last_child = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    document = AXIS2_INTF_TO_IMPL(om_document);
    if (!document->builder)
        return NULL;

    if (!(document->root_element))
    {
        last_child = AXIOM_STAX_BUILDER_NEXT(document->builder, env);
        if (last_child)
        {
            document->last_child = last_child;
            document->root_element = last_child;
        }
        return last_child;
    }
    else if ((document->root_element) &&
            (AXIOM_NODE_IS_COMPLETE(document->root_element, env) == AXIS2_TRUE))
        return NULL;            /* Nothing wrong but done with pulling */

    last_child = AXIOM_STAX_BUILDER_NEXT(document->builder, env);
    if (last_child)
        document->last_child = last_child;
    return last_child;
}


axiom_node_t * AXIS2_CALL
axiom_document_get_root_element(axiom_document_t * document,
        const axis2_env_t *env)
{
    axiom_node_t *node = NULL;
    axiom_document_impl_t *doc_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    doc_impl = AXIS2_INTF_TO_IMPL(document);

    if (doc_impl->root_element)
    {
        return doc_impl->root_element;
    }
    else
    {
        node = axiom_document_build_next(document, env);

        if (doc_impl->root_element)
        {

            return doc_impl->root_element;
        }
        else
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_INVALID_DOCUMENT_STATE_ROOT_NULL, AXIS2_FAILURE);
            return NULL;
        }
    }
    return NULL;
}

axis2_status_t AXIS2_CALL
axiom_document_set_root_element(axiom_document_t *document,
        const axis2_env_t *env,
        axiom_node_t *node)
{
    int status = AXIS2_SUCCESS;
    axiom_document_impl_t *document_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, node, AXIS2_FAILURE);

    document_impl = AXIS2_INTF_TO_IMPL(document);

    if (document_impl->root_element)
    {
        status = AXIOM_NODE_FREE_TREE(document_impl->root_element, env);
        if (status == AXIS2_SUCCESS)
        {
            document_impl->root_element = node;
            return AXIS2_SUCCESS;
        }
        return AXIS2_FAILURE;
    }
    else
    {
        document_impl->root_element = node;
    }
    return AXIS2_SUCCESS;
}

axiom_node_t* AXIS2_CALL
axiom_document_build_all(struct axiom_document *document,
        const axis2_env_t *env)
{
    axiom_document_impl_t *doc_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    if (!document)
        return NULL;
    doc_impl = AXIS2_INTF_TO_IMPL(document);
    if (!doc_impl->root_element)
    {
        axiom_document_get_root_element(document, env);
    }
    if (doc_impl->root_element)
    {
        do
        {
            axiom_node_t *ret_val = NULL;
            ret_val = AXIOM_DOCUMENT_BUILD_NEXT(document, env);
            if (!ret_val)
            {
                if (AXIOM_NODE_IS_COMPLETE(doc_impl->root_element, env)
                        == AXIS2_TRUE)
                {
                    /** document is completly build */
                    return doc_impl->root_element;
                }
                else
                {
                    /** error occured */
                    return NULL;
                }
            }
        }
        while (!AXIOM_NODE_IS_COMPLETE(doc_impl->root_element, env));
        return doc_impl->root_element;
    }
    else
        return NULL;
}

axiom_stax_builder_t* AXIS2_CALL
axiom_document_get_builder(axiom_document_t *document,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(document)->builder;
}

axis2_status_t AXIS2_CALL
axiom_document_serialize(axiom_document_t *document,
        const axis2_env_t *env,
        axiom_output_t *om_output)
{
    axiom_document_impl_t *document_impl = NULL;
    if (!document)
        return AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    document_impl = AXIS2_INTF_TO_IMPL(document);
    if (!(document_impl->root_element))
    {
        axiom_document_get_root_element(document, env);
    }
    if (document_impl->root_element)
    {
        return AXIOM_NODE_SERIALIZE(document_impl->root_element,
                env, om_output);
    }
    else
    {
        return AXIS2_FAILURE;
    }
}
