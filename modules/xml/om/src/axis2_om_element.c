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

#include <axis2_errno.h>
#include <axis2_om_element.h>
#include <axis2_om_attribute.h>
#include <axis2_om_namespace.h>
#include <stdlib.h>

static apr_pool_t *om_pool;

axis2_om_element_t *
axis2_om_element_create (axis2_om_node_t * parent,const char *localname,
			axis2_om_namespace_t * ns,axis2_om_node_t *ele_node)
{
    axis2_om_node_t *node;
    axis2_om_element_t *element;
    if (!localname)
    {
        fprintf (stderr, "Localname can't be null");
        return NULL;
    }

    node = axis2_node_create ();
    if (!node)
    {
        fprintf (stderr, "%d Error", AXIS2_ERROR_OM_MEMORY_ALLOCATION);
        return NULL;
    }
    element = (axis2_om_element_t *) malloc (sizeof (axis2_om_element_t));

    if (!element)
    {
        axis2_node_free (node);
        return NULL;
    }
    element->ns = NULL;
    element->localname = strdup (localname);
    element->pns_counter = 0;
    element->attributes = NULL;
    element->namespaces = NULL;

    if (parent)
    {
        node->parent = parent;
        axis2_node_add_child (parent, node);
    }
    node->done = true;
    node->element_type = AXIS2_OM_ELEMENT;
    node->data_element = element;
    axis2_om_element_set_namespace (node, ns);
	ele_node = node;
    return element;

}


/* create an om_element using qname and parent */
axis2_om_element_t *
axis2_om_element_create_with_qname (axis2_om_node_t * parent,
				axis2_qname_t * qname,axis2_om_node_t *ele_node)
{
	axis2_om_element_t *element;
    axis2_om_node_t *node = NULL;;
    if (!qname)
    {
        return NULL;            /* can't create an element */
    }
     element = axis2_om_element_create (parent,qname->localpart, NULL,node);
    if (node)
    {
        ((axis2_om_element_t *) (node->data_element))->ns =
            axis2_om_element_handle_namespace_with_qname (node, qname);
    }
	ele_node = node;
    return element;
}

axis2_om_element_t *
axis2_om_element_create_with_builder (axis2_om_node_t * parent,
				const char *localname,axis2_om_namespace_t * ns
                              ,axis2_stax_om_builder_t * builder,axis2_om_node_t *ele_node)
{
    axis2_om_node_t *node;
    axis2_om_element_t *element;
    if (!localname)
    {
        fprintf (stderr, "Localname can't be null");
        return NULL;
    }

    node = axis2_node_create ();
    if (!node)
    {
        fprintf (stderr, "%d Error", AXIS2_ERROR_OM_MEMORY_ALLOCATION);
        return NULL;
    }
    element = (axis2_om_element_t *) malloc (sizeof (axis2_om_element_t));

    if (!element)
    {
        axis2_node_free (node);
        return NULL;
    }
    element->localname = strdup (localname);
    element->attributes = NULL;
    element->pns_counter = 0;
    element->namespaces = NULL;
    element->ns = NULL;

    node->builder = builder;
    node->data_element = element;
    node->done = false;
    node->element_type = AXIS2_OM_ELEMENT;

    if (parent)
    {
        node->parent = parent;
        axis2_node_add_child (parent, node);
    }
    axis2_om_element_set_namespace (node, ns);
	ele_node = node;
    return element;
}

axis2_om_namespace_t *
axis2_om_element_find_namespace (axis2_om_node_t
                                 * element_node, const char *uri,
                                 const char *prefix)
{
    axis2_om_namespace_t *ns = NULL;

    if (!element_node)
    {
        return NULL;
    }
    if (!(element_node->data_element)
        || element_node->element_type != AXIS2_OM_ELEMENT)
    {
        /* wrong element type or null node */
        return NULL;
    }

    ns = axis2_om_element_find_declared_namespace (element_node, uri, prefix);
    if (!ns)
    {
        return ns;
    }
    if ((element_node->parent != NULL) &&
        (element_node->parent->element_type == AXIS2_OM_ELEMENT))
    {
        axis2_om_element_find_namespace (element_node->parent, uri, prefix);
    }
    return NULL;
}


/* declare a namespace for this om element */

axis2_om_namespace_t *
axis2_om_element_declare_namespace (axis2_om_node_t *
                                    element_node, axis2_om_namespace_t * ns)
{
    apr_status_t status;
    axis2_om_element_t *element = NULL;
    if (!element_node || !ns || !element_node->data_element
        || element_node->element_type != AXIS2_OM_ELEMENT)
    {
        return NULL;
    }

    element = (axis2_om_element_t *) (element_node->data_element);

    if (!element->namespaces)
    {
        if (!om_pool)
        {
            status = apr_pool_create (&om_pool, NULL);
        }

        element->namespaces = apr_hash_make (om_pool);
    }

    apr_hash_set (element->namespaces, ns->prefix, APR_HASH_KEY_STRING, ns);

    return ns;
}


axis2_om_namespace_t *
axis2_om_element_declare_namespace_with_ns_uri_prefix (axis2_om_node_t *
                                                       element_node,
                                                       const char *uri,
                                                       const char *prefix)
{
    axis2_om_namespace_t *nsp = NULL;
    nsp = axis2_om_namespace_create (uri, prefix);
    if (nsp)
    {
        return axis2_om_element_declare_namespace (element_node, nsp);
    }
    return NULL;
}

/*
*	checks for the namespace in the current om element 
*   can be used to retrive a prefix of a known namespace uri
*
*/
axis2_om_namespace_t *
axis2_om_element_find_declared_namespace (axis2_om_node_t * element_node,
                                          const char *uri, const char *prefix)
{
    void *ns = NULL;
    apr_hash_index_t *hashindex;
    axis2_om_element_t *element = NULL;

    if (!element_node || !ns
        || element_node->element_type != AXIS2_OM_ELEMENT)
    {
        return NULL;
    }

    element = (axis2_om_element_t *) (element_node->data_element);
    if (!prefix || strcmp (prefix, "") == 0)
    {
        for (hashindex = apr_hash_first (om_pool, element->namespaces);
             hashindex; hashindex = apr_hash_next (hashindex))
        {
            apr_hash_this (hashindex, NULL, NULL, &ns);
            if (strcmp (((axis2_om_namespace_t *) (ns))->uri, uri))
            {
                return (axis2_om_namespace_t *) (ns);
            }
        }
    }
    ns = apr_hash_get (element->namespaces, prefix, APR_HASH_KEY_STRING);
    return (axis2_om_namespace_t *) ns;
}

/*
*	This will find a namespace with the given uri and prefix, in the scope of the docuemnt.
* This will start to find from the current element and goes up in the hiararchy until this finds one.
*
*/



static axis2_om_namespace_t *
axis2_om_element_handle_namespace_with_qname (axis2_om_node_t * element_node,
                                              axis2_qname_t * qname)
{
    axis2_om_namespace_t *pns = NULL;
    char *ns_uri = qname->ns_uri;
    if (ns_uri != NULL)
    {
        pns =
            axis2_om_element_find_namespace (element_node, ns_uri,
                                             qname->prefix);
            /**
             * What is left now is
             *  1. nsURI = null & parent != null, but ns = null
             *  2. nsURI != null, (parent doesn't have an ns with given URI), but ns = null
             */
        if (pns == NULL)
        {
            if (qname->prefix)
            {
                pns =
                    axis2_om_element_declare_namespace_with_ns_uri_prefix
                    (element_node, ns_uri, qname->prefix);
            }
        }
    }
    return NULL;
}

static axis2_om_namespace_t *
axis2_om_element_handle_namespace (axis2_om_node_t
                                   * element_node, axis2_om_namespace_t * ns)
{
    axis2_om_namespace_t *ns1 = NULL;
    if (!ns || !element_node)
    {
        return NULL;
    }
    ns1 = axis2_om_element_find_namespace (element_node, ns->uri, ns->prefix);

    if (!ns1)
    {
        ns1 = axis2_om_element_declare_namespace (element_node, ns);
    }
    return ns1;
}


axis2_om_attribute_t *
axis2_om_element_add_attribute (axis2_om_node_t * element_node,
                                axis2_om_attribute_t * attr)
{
    apr_status_t status;
    axis2_qname_t *qname = NULL;
    axis2_om_element_t *element = NULL;

    if (!element_node || element_node->element_type != AXIS2_OM_ELEMENT)
    {
        return NULL;
    }
    element = (axis2_om_element_t *) (element_node->data_element);

    if (!(element->attributes))
    {
        if (!om_pool)
        {
            status = apr_pool_create (&om_pool, NULL);
        }
        element->attributes = apr_hash_make (om_pool);
    }

    qname = axis2_om_attribute_get_qname (attr);

    apr_hash_set (element->attributes, qname, sizeof (axis2_qname_t), attr);

    return attr;
}

axis2_om_attribute_t *
axis2_om_element_get_attribute (axis2_om_node_t * element_node,
                                axis2_qname_t * qname)
{
    char *key = NULL;
    axis2_om_element_t *element = NULL;
    if (!element_node || !qname
        || element_node->element_type != AXIS2_OM_ELEMENT)
    {
        return NULL;
    }
    element = (axis2_om_element_t *) (element_node->data_element);

    return (axis2_om_attribute_t
            *) (apr_hash_get (element->attributes, qname,
                              sizeof (axis2_qname_t)));
}

/*
*  The node passed to the method should have the data element as of type OM_ELEMENT
*/

axis2_om_attribute_t *
axis2_om_element_add_attribute_with_namespace (axis2_om_node_t * element_node,
                                               const char *attribute_name,
                                               const char *value,
                                               axis2_om_namespace_t * ns)
{
    axis2_om_namespace_t *namespace1 = NULL;
    if (!element_node || element_node->element_type != AXIS2_OM_ELEMENT)
    {
        return NULL;
    }
    if (ns)
    {
        namespace1 = axis2_om_element_find_namespace (element_node, ns->uri,
                                                      ns->prefix);
        if (namespace1 == NULL)
        {
            return NULL;
        }
    }
    return axis2_om_element_add_attribute (element_node,
                                           axis2_om_attribute_create
                                           (attribute_name, value, ns));
}


void
axis2_om_element_set_namespace (axis2_om_node_t * node,
                                axis2_om_namespace_t * ns)
{
    axis2_om_namespace_t *nsp = NULL;
    if (ns && node && (node->data_element))
    {
        nsp = axis2_om_element_handle_namespace (node, ns);
    }
    ((axis2_om_element_t *) (node->data_element))->ns = nsp;
    nsp = NULL;
}


void
axis2_free_om_element (axis2_om_element_t * element)
{
    if (element)
    {
        if (element->localname)
        {
            free (element->localname);
        }
        if (element->ns)
        {
            axis2_om_namespace_free (element->ns);
        }
        if (element->attributes)
        {

        }
        if (element->namespaces)
        {

        }
        free (element);
    }
}

void
axis2_om_element_set_localname (axis2_om_node_t * element_node,
                                const char *localname)
{
    axis2_om_element_t *element = NULL;
    if (!element_node || element_node->element_type != AXIS2_OM_ELEMENT)
    {
        return;
    }
    element = (axis2_om_element_t *) (element_node->data_element);

    if (element->localname)
    {
        free (element->localname);
    }
    element->localname = strdup (localname);
}

char *
axis2_om_element_get_localname (axis2_om_node_t * element_node)
{
    if (!element_node || element_node->element_type != AXIS2_OM_ELEMENT)
    {
        return NULL;
    }
    return ((axis2_om_element_t *) (element_node->data_element))->localname;
}

int
axis2_om_element_serialize_start_part (axis2_om_element_t * element_node,
                                       axis2_om_output_t * om_output)
{
    // TODO : handle null pointer errors
    if (element_node->ns && element_node->ns->uri && element_node->ns->prefix)
        axis2_om_output_write (om_output, AXIS2_OM_ELEMENT, 3,
                               element_node->localname, element_node->ns->uri,
                               element_node->ns->prefix);
    else if (element_node->ns && element_node->ns->uri)
        axis2_om_output_write (om_output, AXIS2_OM_ELEMENT, 2,
                               element_node->localname,
                               element_node->ns->uri);
    else
        axis2_om_output_write (om_output, AXIS2_OM_ELEMENT, 1,
                               element_node->localname);


}

int
axis2_om_element_serialize_end_part (axis2_om_element_t * element_node,
                                     axis2_om_output_t * om_output)
{
    // TODO : handle null pointer errors
    axis2_om_output_write (om_output, AXIS2_OM_ELEMENT, 0);
}
