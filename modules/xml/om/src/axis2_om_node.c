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

#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>

axis2_status_t AXIS2_CALL axis2_om_node_impl_free (axis2_environment_t * environment,
                                        axis2_om_node_t * node);
axis2_status_t AXIS2_CALL axis2_om_node_impl_add_child (axis2_environment_t *
                                             environment,
                                             axis2_om_node_t * parent,
                                             axis2_om_node_t * child);
axis2_om_node_t * AXIS2_CALL axis2_om_node_impl_detach (axis2_environment_t * environment,
                                            axis2_om_node_t * node_to_detach);
axis2_status_t AXIS2_CALL axis2_om_node_impl_set_parent (axis2_environment_t *
                                              environment,
                                              axis2_om_node_t * node,
                                              axis2_om_node_t * parent);
axis2_status_t AXIS2_CALL axis2_om_node_impl_insert_sibling_after (axis2_environment_t *
                                                        environment,
                                                        axis2_om_node_t *
                                                        node,
                                                        axis2_om_node_t *
                                                        node_to_insert);
axis2_status_t AXIS2_CALL axis2_om_node_impl_insert_sibling_before (axis2_environment_t *
                                                         environment,
                                                         axis2_om_node_t *
                                                         node,
                                                         axis2_om_node_t *
                                                         node_to_insert);
axis2_om_node_t * AXIS2_CALL axis2_om_node_impl_get_first_child (axis2_environment_t *
                                                     environment,
                                                     axis2_om_node_t *
                                                     parent_node);
axis2_om_node_t * AXIS2_CALL axis2_om_node_impl_get_next_child (axis2_environment_t *
                                                    environment,
                                                    axis2_om_node_t *
                                                    parent_node);
axis2_status_t AXIS2_CALL axis2_om_node_impl_serialize (axis2_environment_t *
                                             environment,
                                             axis2_om_node_t * om_node,
                                             axis2_om_output_t * om_output);


AXIS2_DECLARE(axis2_om_node_t *)
axis2_om_node_create (axis2_environment_t * environment)
{
    axis2_om_node_t *node =
        (axis2_om_node_t *) axis2_malloc (environment->allocator,
                                          sizeof (axis2_om_node_t));
    if (!node)
    {
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
    node->ops =
        (axis2_om_node_ops_t *) axis2_malloc (environment->allocator,
                                              sizeof (axis2_om_node_ops_t));
    if (!(node->ops))
    {
        axis2_free (environment->allocator, node);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
    /* assign fucn pointers */
    node->ops->axis2_om_node_ops_add_child = axis2_om_node_impl_add_child;
    node->ops->axis2_om_node_ops_free = axis2_om_node_impl_free;
    node->ops->axis2_om_node_ops_detach = axis2_om_node_impl_detach;
    node->ops->axis2_om_node_ops_get_first_child =
        axis2_om_node_impl_get_first_child;
    node->ops->axis2_om_node_ops_insert_sibling_after =
        axis2_om_node_impl_insert_sibling_after;
    node->ops->axis2_om_node_ops_insert_sibling_before =
        axis2_om_node_impl_insert_sibling_before;
    node->ops->axis2_om_node_ops_set_parent = axis2_om_node_impl_set_parent;
    node->ops->axis2_om_node_ops_serialize = axis2_om_node_impl_serialize;
    node->ops->axis2_om_node_ops_get_next_child =
        axis2_om_node_impl_get_next_child;

    node->first_child = NULL;
    node->last_child = NULL;
    node->next_sibling = NULL;
    node->prev_sibling = NULL;
    node->parent = NULL;
    node->node_type = AXIS2_OM_INVALID;
    node->done = AXIS2_FALSE;
    node->data_element = NULL;
    node->current_child = NULL;
    return node;
}

/**
 *  This free fucntion will free an om_element and all the children contained in it
 *  before calling this function first free 
 */
axis2_status_t AXIS2_CALL
axis2_om_node_impl_free (axis2_environment_t * environment,
                         axis2_om_node_t * node)
{

    if (!node)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }
    if (node->first_child)
    {
        while (node->first_child)
        {
            axis2_om_node_t *child_node = NULL;
            child_node = axis2_om_node_detach (environment, node->first_child);
            axis2_om_node_free (environment, child_node);
        }
    }
    switch (node->node_type)
    {
    case AXIS2_OM_ELEMENT:
        axis2_om_element_free(environment, (axis2_om_element_t*)node->data_element);
        break;

    case AXIS2_OM_COMMENT:
        /* axis2_om_comment_free(environment,(axis2_om_comment_t*)node->data_element) */
        break;
    case AXIS2_OM_DOCTYPE:
        /* axis2_om_doctype_free(environment,(axis2_om_doctype_t*)node->data_element) */
        break;
    case AXIS2_OM_PROCESSING_INSTRUCTION:
        /* axis2_om_prcessing_instruction_free(environment,(axis2_om_processing_instruction)node->data_element) */
        break;
    case AXIS2_OM_TEXT:
        axis2_om_text_free(environment,(axis2_om_text_t*)node->data_element);
        break;

    default:
        break;
    }

    axis2_free (environment->allocator, node->ops);
    axis2_free (environment->allocator, node);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_om_node_impl_add_child (axis2_environment_t * environment,
                              axis2_om_node_t * parent,
                              axis2_om_node_t * child)
{
    if (!parent || !child)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }
    if (parent->first_child == NULL)
    {
        parent->first_child = child;
    }
    else
    {
        parent->last_child->next_sibling = child;
        child->prev_sibling = parent->last_child;
    }

    child->parent = parent;
    parent->last_child = child;
    return AXIS2_SUCCESS;
}



axis2_om_node_t * AXIS2_CALL
axis2_om_node_impl_detach (axis2_environment_t * environment,
                           axis2_om_node_t * node_to_detach)
{
    axis2_om_node_t *parent = NULL;

    if (!node_to_detach || !(node_to_detach->parent))
    {
        /* nodes that do not have a parent can't be detached  */
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }

    parent = node_to_detach->parent;

    if ((node_to_detach->prev_sibling) == NULL)
    {
        parent->first_child = node_to_detach->next_sibling;
    }
    else
    {
        node_to_detach->prev_sibling->next_sibling =
            node_to_detach->next_sibling;
    }
    if ((node_to_detach->next_sibling))
    {
        node_to_detach->next_sibling->prev_sibling =
            node_to_detach->prev_sibling;
    }

    node_to_detach->parent = NULL;
    return node_to_detach;
}

axis2_status_t AXIS2_CALL
axis2_om_node_impl_set_parent (axis2_environment_t * environment,
                               axis2_om_node_t * node,
                               axis2_om_node_t * parent)
{
    if (!parent || !node)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }
    if (parent == node->parent)
    {                           /* same parent already exist */
        return AXIS2_SUCCESS;
    }
    /* if a new parent is assigned in  place of existing 
     *  one first the node should  be detached  
     */

    if (!(node->parent))
    {
        axis2_om_node_detach (environment, node);

    }
    node->parent = parent;
    return AXIS2_SUCCESS;
}

/**
 * This will insert a sibling just after the current information item
 * @param node the node in consideration
 * @param nodeto_insert the node that will be inserted
 */

axis2_status_t AXIS2_CALL
axis2_om_node_impl_insert_sibling_after (axis2_environment_t * environment,
                                         axis2_om_node_t * node,
                                         axis2_om_node_t * node_to_insert)
{
    if (!node || !node_to_insert)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }
    node_to_insert->parent = node->parent;
    node_to_insert->prev_sibling = node;

    if (node->next_sibling)
    {
        node->next_sibling->prev_sibling = node_to_insert;
    }
    node_to_insert->next_sibling = node->next_sibling;
    node->next_sibling = node_to_insert;
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_om_node_impl_insert_sibling_before (axis2_environment_t * environment,
                                          axis2_om_node_t * node,
                                          axis2_om_node_t * node_to_insert)
{
    if (!node || !node_to_insert)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }

    node_to_insert->parent = node->parent;

    node_to_insert->prev_sibling = node->prev_sibling;
    node_to_insert->next_sibling = node;

    if (!(node->prev_sibling))
    {
        node->parent->first_child = node_to_insert;
    }
    else
    {
        node->prev_sibling->next_sibling = node_to_insert;
    }
    node->prev_sibling = node_to_insert;
    return AXIS2_SUCCESS;
}

axis2_om_node_t* AXIS2_CALL
axis2_om_node_impl_get_first_child (axis2_environment_t * environment,
                                    axis2_om_node_t * parent_node)
{
    /**  */
    if (!parent_node)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }
    if (parent_node->first_child)
    {
        parent_node->current_child = parent_node->first_child;
        return parent_node->first_child;
    }

    return NULL;
}

axis2_om_node_t* AXIS2_CALL
axis2_om_node_impl_get_next_child (axis2_environment_t * environment,
                                   axis2_om_node_t * parent_node)
{
    axis2_om_node_t *node = NULL;
    if (parent_node && !(parent_node->first_child))
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }

    if (parent_node && parent_node->first_child
        && !(parent_node->current_child))
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_ITERATOR_STATE;
        return NULL;
    }
    if (parent_node->current_child->next_sibling)
    {
        node = parent_node->current_child->next_sibling;
        parent_node->current_child = node;
        return node;
    }
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_om_node_impl_serialize (axis2_environment_t * environment,
                              axis2_om_node_t * om_node,
                              axis2_om_output_t * om_output)
{
    axis2_om_node_t *child_node = NULL;
    int status = AXIS2_SUCCESS;

    if (!om_node || !om_output)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }


    switch (om_node->node_type)
    {
    case AXIS2_OM_ELEMENT:
        status =
            axis2_om_element_serialize_start_part (environment,
                                                   (axis2_om_element_t *)
                                                   om_node->data_element,
                                                   om_output);
        if (status != AXIS2_SUCCESS)
            return status;
        break;
    case AXIS2_OM_TEXT:
        status =
            axis2_om_text_serialize (environment,
                                     (axis2_om_text_t *) om_node->
                                     data_element, om_output);
        return status;
        break;
    
    default:
        break;
    }


    child_node = axis2_om_node_get_first_child (environment, om_node);

    while (child_node)
    {
        status = axis2_om_node_serialize (environment, child_node, om_output);
        if (status != AXIS2_SUCCESS)
            return status;
        child_node = axis2_om_node_get_next_child (environment, om_node);
    }

    switch (om_node->node_type)
    {
    case AXIS2_OM_ELEMENT:
        status =
            axis2_om_element_serialize_end_part (environment,
                                                 (axis2_om_element_t *)
                                                 om_node->data_element,
                                                 om_output);
        if (status != AXIS2_SUCCESS)
            return status;
        break;
    
    default:
        break;
    }

    return status;

    return AXIS2_SUCCESS;
}
