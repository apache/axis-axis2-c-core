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

#include <axis2_om_element.h>
#include <axis2_om_attribute.h>
#include <axis2_om_namespace.h>


axis2_om_namespace_t *
    AXIS2_CALL axis2_om_element_impl_find_namespace (axis2_environment_t * environment,
                                           axis2_om_node_t * node,
                                           const axis2_char_t * uri,
                                           const axis2_char_t * prefix);
                                           
axis2_status_t AXIS2_CALL axis2_om_element_impl_declare_namespace (axis2_environment_t *
                                                        environment,
                                                        axis2_om_node_t *
                                                        node,
                                                        axis2_om_namespace_t *
                                                        ns);
                                                        
axis2_om_namespace_t *
    AXIS2_CALL axis2_om_element_impl_find_namespace_with_qname (axis2_environment_t *
                                                      environment,
                                                      axis2_om_node_t *
                                                      element,
                                                      axis2_qname_t * qname);
/*
axis2_om_namespace_t *
    AXIS2_CALL axis2_om_element_impl_find_declared_namespace (axis2_environment_t *
                                                    environment,
                                                    struct axis2_om_element
                                                    *element,
                                                    const axis2_char_t * uri,
                                                    const axis2_char_t *
                                                    prefix);
*/

axis2_status_t AXIS2_CALL axis2_om_element_impl_add_attribute (axis2_environment_t *
                                                    environment,
                                                    struct axis2_om_element
                                                    *element,
                                                    axis2_om_attribute_t *
                                                    attribute);
                                                    
axis2_om_attribute_t * AXIS2_CALL axis2_om_element_impl_get_attribute (axis2_environment_t
                                                           * environment,
                                                           struct
                                                           axis2_om_element
                                                           *element,
                                                           axis2_qname_t *
                                                           qname);
                                                           
axis2_status_t AXIS2_CALL axis2_om_element_impl_free (axis2_environment_t * environment,
                                           struct axis2_om_element *element);
                                               
axis2_status_t AXIS2_CALL axis2_om_element_impl_serialize_start_part (axis2_environment_t
                                                           * environment,
                                                           axis2_om_element_t
                                                           * element,
                                                           axis2_om_output_t *
                                                           om_output);
                                           
axis2_status_t AXIS2_CALL axis2_om_element_impl_serialize_end_part (axis2_environment_t *
                                                         environment,
                                                         axis2_om_element_t *
                                                         element,
                                                         axis2_om_output_t *
                                                         om_output);


axis2_om_element_t *
axis2_om_element_create (axis2_environment_t * environment,
                         axis2_om_node_t * parent,
                         const axis2_char_t * localname,
                         axis2_om_namespace_t * ns, axis2_om_node_t ** node)
{
    axis2_om_element_t *element;
    if (!localname || !node)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }

    (*node) = axis2_om_node_create (environment);
    if (!(*node))
    {
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
    element =
        (axis2_om_element_t *) axis2_malloc (environment->allocator,
                                             sizeof (axis2_om_element_t));

    if (!element)
    {
        axis2_om_node_free (environment, (*node));
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
    element->ns = NULL;
    element->localname = NULL;
    element->attributes = NULL;
    element->namespaces = NULL;

    element->localname =
        (char *) axis2_strdup (environment->string, localname);
    if (!element->localname)
    {
        axis2_free (environment->allocator, element);
        axis2_om_node_free (environment, (*node));
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }


    if (parent)
    {
        (*node)->parent = parent;
        axis2_om_node_add_child (environment, parent, (*node));
    }

    (*node)->done = AXIS2_FALSE;
    (*node)->node_type = AXIS2_OM_ELEMENT;
    (*node)->data_element = element;

    if (ns)
    {
        element->ns =
            axis2_om_element_impl_find_namespace (environment, *node, ns->uri,
                                                  ns->prefix);
        if (!(element->ns))
        {
            if (axis2_om_element_impl_declare_namespace
                (environment, *node, ns) == AXIS2_SUCCESS)
                element->ns = ns;
        }
    }

    element->ops = NULL;
    element->ops =
        (axis2_om_element_ops_t *) axis2_malloc (environment->allocator,
                                                 sizeof
                                                 (axis2_om_element_ops_t));

    if (!element->ops)
    {
        axis2_free (environment->allocator, element->localname);
        axis2_free (environment->allocator, element);
        axis2_om_node_free (environment, (*node));
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    /* operations */
    element->ops->axis2_om_element_ops_find_namespace =
        axis2_om_element_impl_find_namespace;
    element->ops->axis2_om_element_ops_declare_namespace =
        axis2_om_element_impl_declare_namespace;
    element->ops->axis2_om_element_ops_find_namespace_with_qname =
        axis2_om_element_impl_find_namespace_with_qname;
    element->ops->axis2_om_element_ops_add_attribute =
        axis2_om_element_impl_add_attribute;
    element->ops->axis2_om_element_ops_get_attribute =
        axis2_om_element_impl_get_attribute;
    element->ops->axis2_om_element_ops_free = axis2_om_element_impl_free;
    /*element->ops->axis2_om_element_ops_find_declared_namespace =
        axis2_om_element_impl_find_declared_namespace;*/
    element->ops->axis2_om_element_ops_serialize_start_part =
        axis2_om_element_impl_serialize_start_part;
    element->ops->axis2_om_element_ops_serialize_end_part =
        axis2_om_element_impl_serialize_end_part;

    return element;

}

axis2_om_element_t *
axis2_om_element_create_with_qname (axis2_environment_t * environment,
                                    axis2_om_node_t * parent,
                                    axis2_qname_t * qname,
                                    axis2_om_node_t ** node)
{
    axis2_om_element_t *element = NULL;

    if (!qname || !(*node))
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;            /* can't create an element */
    }

    /* TODO:handle namespace in the following */
    element =
        axis2_om_element_create (environment, parent, qname->localpart, NULL,
                                 node);
    if (*node)
    {
        if (((axis2_om_element_t *) ((*node)->data_element))->ns)
        {
            axis2_om_namespace_t *ns =
                ((axis2_om_element_t *) ((*node)->data_element))->ns;
            element->ns =
                axis2_om_element_impl_find_namespace (environment, *node,
                                                      ns->uri, ns->prefix);
            if (!(element->ns))
            {
                if (axis2_om_element_impl_declare_namespace
                    (environment, *node, ns) == AXIS2_SUCCESS)
                    element->ns = ns;
            }
        }
    }

    return element;
}

axis2_om_namespace_t *
axis2_om_element_impl_find_namespace (axis2_environment_t * environment,
                                      axis2_om_node_t * node,
                                      const axis2_char_t * uri,
                                      const axis2_char_t * prefix)
{
    void *ns = NULL;
    axis2_hash_index_t *hashindex;
    axis2_om_element_t *element = NULL;

    if (!node)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }
    if (!(node->data_element) || node->node_type != AXIS2_OM_ELEMENT)
    {
        /* wrong element type or null node */
        environment->error->errorno = AXIS2_ERROR_INVALID_NODE_TYPE;
        return NULL;
    }

    element = (axis2_om_element_t *) (node->data_element);
    if (!prefix || axis2_strcmp (environment->string, prefix, "") == 0)
    {
        for (hashindex = axis2_hash_first (environment, element->namespaces);
             hashindex; hashindex = axis2_hash_next (hashindex))
        {
            axis2_hash_this (hashindex, NULL, NULL, &ns);
            if (ns
                && axis2_strcmp (environment->string,
                                 ((axis2_om_namespace_t *) (ns))->uri,
                                 uri) == 0)
            {
                return (axis2_om_namespace_t *) (ns);
            }
        }
    }

    if (element->namespaces)
        ns = axis2_hash_get (element->namespaces, prefix,
                             AXIS2_HASH_KEY_STRING);

    if (ns)
    {
        return ns;
    }
    else if ((node->parent != NULL) &&
             (node->parent->node_type == AXIS2_OM_ELEMENT))
    {
        return axis2_om_element_impl_find_namespace (environment,
                                                     node->parent, uri,
                                                     prefix);
    }

    return NULL;
}

axis2_status_t
axis2_om_element_impl_declare_namespace (axis2_environment_t * environment,
                                         axis2_om_node_t * node,
                                         axis2_om_namespace_t * ns)
{
    axis2_om_namespace_t *declared_ns = NULL;
    axis2_om_element_t *element = NULL;
    if (!node || !ns)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }

    declared_ns =
        axis2_om_element_impl_find_namespace (environment, node, ns->uri,
                                              ns->prefix);

    if (declared_ns)
    {
        /*Namespace already declared, so return */
        return AXIS2_SUCCESS;
    }

    element = (axis2_om_element_t *) (node->data_element);

    if (!element)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }

    if (!(element->namespaces))
    {
        element->namespaces = axis2_hash_make (environment);
        if (!(element->namespaces))
            return AXIS2_FAILURE;
    }

    if (ns->prefix)
        axis2_hash_set (element->namespaces, ns->prefix,
                        AXIS2_HASH_KEY_STRING, ns);
    else
        axis2_hash_set (element->namespaces, "default", AXIS2_HASH_KEY_STRING,
                        ns);

    return AXIS2_SUCCESS;
}

/*
axis2_om_namespace_t *
axis2_om_element_impl_find_declared_namespace (axis2_environment_t *
                                               environment,
                                               struct axis2_om_element *
                                               element,
                                               const axis2_char_t * uri,
                                               const axis2_char_t * prefix)
{
    axis2_hash_index_t *hash_index = NULL;
    void *ns = NULL;
    if (!element || !(element->namespaces))
    {
        return NULL;
    }
    if (!prefix || axis2_strcmp (environment->string, prefix, "") == 0)
    {
        for (hash_index = axis2_hash_first (environment, element->namespaces);
             hash_index; hash_index = axis2_hash_next (hash_index))
        {
            axis2_hash_this (hash_index, NULL, NULL, &ns);
            if (axis2_strcmp
                (environment->string, ((axis2_om_namespace_t *) (ns))->uri,
                 uri))
            {
                return (axis2_om_namespace_t *) (ns);
            }
        }
        return NULL;
    }
    ns = axis2_hash_get (element->namespaces, prefix, AXIS2_HASH_KEY_STRING);
    if (ns)
        return (axis2_om_namespace_t *) ns;
    else
        return NULL;

}
*/

axis2_om_namespace_t *
axis2_om_element_impl_find_namespace_with_qname (axis2_environment_t *
                                                 environment,
                                                 axis2_om_node_t * element,
                                                 axis2_qname_t * qname)
{
    if (!element || !qname)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }

    if (qname->namespace_uri)
    {
        return axis2_om_element_impl_find_namespace (environment, element,
                                                     qname->namespace_uri,
                                                     qname->prefix);
    }
    else
    {
        return NULL;
    }
}

axis2_status_t
axis2_om_element_impl_add_attribute (axis2_environment_t * environment,
                                     struct axis2_om_element * element,
                                     axis2_om_attribute_t * attribute)
{
    axis2_qname_t *qname = NULL;

    if (!element || !attribute)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }

    if (!(element->attributes))
    {
        element->attributes = axis2_hash_make (environment);
        if (!(element->attributes))
            return AXIS2_FAILURE;
    }

    qname = axis2_om_attribute_get_qname (environment, attribute);
    if (qname)
        axis2_hash_set (element->attributes, qname, sizeof (axis2_qname_t),
                        attribute);

    return ((qname) ? AXIS2_SUCCESS : AXIS2_FAILURE);
}

axis2_om_attribute_t *
axis2_om_element_impl_get_attribute (axis2_environment_t * environment,
                                     struct axis2_om_element * element,
                                     axis2_qname_t * qname)
{
    if (!element || !qname)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }

    return (axis2_om_attribute_t
            *) (axis2_hash_get (element->attributes, qname,
                                sizeof (axis2_qname_t)));
}

axis2_status_t
axis2_om_element_impl_free (axis2_environment_t * environment,
                            struct axis2_om_element *element)
{
    if (!element)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }

    if (element)
    {
        if (element->localname)
        {
            axis2_free (environment->allocator, element->localname);
        }
        if (element->ns)
        {
            /* it is the responsibility of the element where the namespace is declared to free it */
        }
        if (element->attributes)
        {
            /* TODO: free attributes */
            /*need to iterate and free individual attributes */
        }
        if (element->namespaces)
        {
            /*TODO: free namespaces */
            /*need to eterate and free individual namespaces */
        }
        axis2_free (environment->allocator, element->ops);
        axis2_free (environment->allocator, element);
    }
    return AXIS2_SUCCESS;
}

axis2_status_t
axis2_om_element_impl_serialize_start_part (axis2_environment_t * environment,
                                            axis2_om_element_t * element,
                                            axis2_om_output_t * om_output)
{
    int status = AXIS2_SUCCESS;
    if (!element || !om_output)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }


    if (element->ns && element->ns->uri && element->ns->prefix)
        status =
            axis2_om_output_write (environment, om_output, AXIS2_OM_ELEMENT,
                                   3, element->localname, element->ns->uri,
                                   element->ns->prefix);
    else if (element->ns && element->ns->uri)
        status =
            axis2_om_output_write (environment, om_output, AXIS2_OM_ELEMENT,
                                   2, element->localname, element->ns->uri);
    else
        status =
            axis2_om_output_write (environment, om_output, AXIS2_OM_ELEMENT,
                                   1, element->localname);

    /* serialize attributes */
    if (element->attributes)
    {
        axis2_hash_index_t *hi;
        void *val;
        for (hi = axis2_hash_first (environment, element->attributes); hi;
             hi = axis2_hash_next (hi))
        {
            axis2_hash_this (hi, NULL, NULL, &val);

            if (val)
                status =
                    axis2_om_attribute_serialize (environment,
                                                  (axis2_om_attribute_t *)
                                                  val, om_output);
            else
            {
                status = AXIS2_FAILURE;
            }
        }
    }

    /* serialize namespaces */
    if (element->namespaces)
    {
        axis2_hash_index_t *hi;
        void *val;
        for (hi = axis2_hash_first (environment, element->namespaces); hi;
             hi = axis2_hash_next (hi))
        {
            axis2_hash_this (hi, NULL, NULL, &val);

            if (val)
                status =
                    axis2_om_namespace_serialize (environment,
                                                  (axis2_om_namespace_t *)
                                                  val, om_output);
            else
            {
                status = AXIS2_FAILURE;
            }
        }
    }

    return status;
}

axis2_status_t
axis2_om_element_impl_serialize_end_part (axis2_environment_t * environment,
                                          axis2_om_element_t * element,
                                          axis2_om_output_t * om_output)
{
    int status = AXIS2_SUCCESS;

    if (!om_output)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }

    status =
        axis2_om_output_write (environment, om_output, AXIS2_OM_ELEMENT, 0);
    return status;
}
