/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <axis2_om_document.h>
#include <axis2_om_stax_builder.h>

axis2_status_t AXIS2_CALL axis2_om_document_impl_free (axis2_environment_t * environment,
                                            axis2_om_document_t * document);
axis2_status_t AXIS2_CALL axis2_om_document_impl_add_child (axis2_environment_t *
                                                 environment,
                                                 axis2_om_document_t *
                                                 document,
                                                 axis2_om_node_t * child);
axis2_om_node_t * AXIS2_CALL axis2_om_document_impl_build_next (axis2_environment_t *
                                                    environment,
                                                    axis2_om_document_t *
                                                    document);
axis2_om_node_t * AXIS2_CALL axis2_om_document_impl_get_root_element (axis2_environment_t
                                                          * environment,
                                                          axis2_om_document_t
                                                          * document);
axis2_om_node_t * AXIS2_CALL axis2_om_document_impl_get_next_sibling (axis2_environment_t
                                                          * environment,
                                                          axis2_om_document_t
                                                          * document);
axis2_om_node_t * AXIS2_CALL axis2_om_document_impl_get_first_child (axis2_environment_t *
                                                         environment,
                                                         axis2_om_document_t *
                                                         document);
axis2_om_node_t * AXIS2_CALL axis2_om_document_impl_get_next_child (axis2_environment_t *
                                                        environment,
                                                        axis2_om_document_t *
                                                        document);

axis2_om_document_t *
AXIS2_CALL axis2_om_document_create (axis2_environment_t * environment,
                          axis2_om_node_t * root,
                          axis2_om_stax_builder_t * builder)
{

    axis2_om_document_t *document =
        (axis2_om_document_t *) axis2_malloc (environment->allocator,
                                              sizeof (axis2_om_document_t));

    if (!document)
    {
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
    document->builder = builder;
    document->root_element = root;
    document->first_child = NULL;
    document->last_child = NULL;

    document->char_set_encoding = NULL;
    document->char_set_encoding =
        (axis2_char_t *) axis2_strdup (environment->string,
                                       CHAR_SET_ENCODING);

    if (!document->char_set_encoding)
    {
        axis2_free (environment->allocator, document);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    document->xml_version = NULL;
    document->xml_version =
        (axis2_char_t *) axis2_strdup (environment->string, XML_VERSION);
    if (!document->xml_version)
    {
        axis2_free (environment->allocator, document);
        axis2_free (environment->allocator, document->char_set_encoding);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    document->done = AXIS2_FALSE;

    if (builder)
    {
        builder->document = document;
    }

    /* operations */
    document->ops = NULL;
    document->ops =
        (axis2_om_document_ops_t *) axis2_malloc (environment->allocator,
                                                  sizeof
                                                  (axis2_om_document_ops_t));

    if (!document->ops)
    {
        axis2_free (environment->allocator, document);
        axis2_free (environment->allocator, document->char_set_encoding);
        axis2_free (environment->allocator, document->xml_version);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    document->ops->axis2_om_document_ops_free = axis2_om_document_impl_free;
    document->ops->axis2_om_document_ops_add_child =
        axis2_om_document_impl_add_child;
    document->ops->axis2_om_document_ops_build_next =
        axis2_om_document_impl_build_next;
    document->ops->axis2_om_document_ops_get_root_element =
        axis2_om_document_impl_get_root_element;
    document->ops->axis2_om_document_ops_get_next_sibling =
        axis2_om_document_impl_get_next_sibling;
    document->ops->axis2_om_document_ops_get_first_child =
        axis2_om_document_impl_get_first_child;
    document->ops->axis2_om_document_ops_get_next_child =
        axis2_om_document_impl_get_next_child;
    return document;
}


axis2_status_t
axis2_om_document_impl_free (axis2_environment_t * environment,
                             axis2_om_document_t * document)
{
    if (document)
    {
        if (document->char_set_encoding)
            axis2_free (environment->allocator, document->char_set_encoding);
        if (document->xml_version)
            axis2_free (environment->allocator, document->xml_version);

        (document->root_element)->ops->axis2_om_node_ops_free (environment,
                                                               document->
                                                               root_element);

        axis2_free (environment->allocator, document);
    }
    return AXIS2_SUCCESS;
}


axis2_status_t
axis2_om_document_impl_add_child (axis2_environment_t * environment,
                                  axis2_om_document_t * document,
                                  axis2_om_node_t * child)
{
    if (!document || !child)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }

    if (!(document->root_element) && child)
    {
        document->root_element = child;
        return AXIS2_SUCCESS;

    }

    if (document->root_element && child)
    {
        return axis2_om_node_add_child (environment, document->root_element,
                                        child);
    }

    return AXIS2_FAILURE;

}

axis2_om_node_t *
axis2_om_document_impl_build_next (axis2_environment_t * environment,
                                   axis2_om_document_t * document)
{
    if (!document)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }

    if (!(document->root_element))
    {
        return axis2_om_stax_builder_next (environment, document->builder);
    }
    else if (document->root_element->done)
        return NULL;            /* Nothing wrong but done with pulling */

    return axis2_om_stax_builder_next (environment, document->builder);
}


axis2_om_node_t *
axis2_om_document_impl_get_root_element (axis2_environment_t * environment,
                                         axis2_om_document_t * document)
{
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_om_node_t *node = NULL;
    if (document && document->root_element)
    {
        return document->root_element;
    }
    else
    {
        node =
            (document)->ops->axis2_om_document_ops_build_next (environment,
                                                               document);
        if (document->root_element)
            return document->root_element;
        else
        {
            if (status != AXIS2_SUCCESS)
                environment->error->errorno = AXIS2_ERROR_INVALID_DOCUMENT_STATE_ROOT_NULL;
            return NULL;
        }
    }
}

axis2_om_node_t *
axis2_om_document_impl_get_next_sibling (axis2_environment_t * environment,
                                         axis2_om_document_t * document)
{
    axis2_om_node_t *lastchild = NULL;
    axis2_om_node_t *current_node = NULL;
    if (document && document->builder && document->builder->lastnode)
    {
        lastchild = document->builder->lastnode;
        if (!(lastchild->parent))
        {
            return NULL;        /* if parent is null there can't be siblings */
        }
        while (!(lastchild->next_sibling) && !(lastchild->parent->done))
            current_node = axis2_om_document_build_next (environment, document);
        return lastchild->next_sibling;
    }
    return NULL;
}

axis2_om_node_t *
axis2_om_document_impl_get_first_child (axis2_environment_t * environment,
                                        axis2_om_document_t * document)
{
    axis2_om_node_t *current_child = NULL;
    axis2_om_node_t *current_node = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    current_child = document->builder->lastnode;
    if (current_child)
    {
        while (!(current_child->first_child) && !(current_child->done))
        {
            current_node = axis2_om_document_build_next (environment, document);
        }

        if (current_child->first_child)
            return current_child->first_child;
        else
        {
            if (status != AXIS2_SUCCESS)
                environment->error->errorno = status;
            return NULL;
        }
    }
    return NULL;
}

axis2_om_node_t *
axis2_om_document_impl_get_next_child (axis2_environment_t * environment,
                                       axis2_om_document_t * document)
{
    axis2_om_node_t *current_child = NULL;
    axis2_om_node_t *current_node = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    current_child = document->builder->lastnode;
    if (current_child && current_child->parent->first_child)
    {
        current_node = axis2_om_document_build_next (environment, document);

        if (current_child->last_child)
            return current_child->last_child;
        else
        {
            if (status != AXIS2_SUCCESS)
                environment->error->errorno = status;
            return NULL;
        }
    }
    else
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_ITERATOR_STATE;
        return NULL;
    }
}
