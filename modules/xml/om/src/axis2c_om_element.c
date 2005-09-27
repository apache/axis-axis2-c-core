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

#include <axis2c_om_element.h>
#include <axis2c_om_attribute.h>
#include <stdlib.h>

axis2c_node_t *axis2c_create_om_element(const char *localname,
				 axis2c_om_namespace_t * ns)
{
    axis2c_node_t *node = axis2c_create_node();
    if (node)
    {
	om_element_t *element =
	    (om_element_t *) malloc(sizeof(om_element_t));
	if (element)
	{
	    element->localname = strdup(localname);
	    element->ns = ns;
	    element->pns_counter = 0;
	    element->attributes = NULL;
	    element->namespaces = NULL;
	}
	else
	{
	    //could not allocate memory
	    free(node);
	    return NULL;
	}

	node->element_type = OM_ELEMENT;
	node->data_element = element;
	return node;
    }
    return NULL;
}

// create an om element using localname namespace and parent element

axis2c_node_t *axis2c_create_om_element_with_parent(const char *localname,
					     axis2c_om_namespace_t * ns,
					     axis2c_node_t * parent)
{
    axis2c_node_t *curr_node = axis2c_create_om_element(localname, ns);
    if (!curr_node)
    {
	//unable to create an element 
	return NULL;
    }
    curr_node->done = TRUE;
    if (!parent)
	return curr_node;

    curr_node->parent = parent;
    axis2c_node_add_child(parent, curr_node);

    return curr_node;
}

// create an om_element using qname and parent 
axis2c_node_t *axis2c_create_om_element_with_qname_parent(qname_t * qname,
						   axis2c_node_t * parent)
{
    axis2c_node_t *node = NULL;;
    if (!qname)
    {
	return NULL;		// can't create an element

    }
    node =
	axis2c_create_om_element_with_parent(qname->localpart, NULL,
					     parent);
    if (node)
    {
	((om_element_t *) (node->data_element))->ns =
	    axis2c_om_element_handle_namespace_with_qname(node, qname);
    }
    return node;
}


axis2c_om_namespace_t *axis2c_om_element_find_namespace(node_t * element_node,
						 const char *uri,
						 const char *prefix)
{
    //om_element_t *ele=NULL;
    axis2c_om_namespace_t *ns = NULL;
    if (!element_node)
    {
	return NULL;
    }
    if (!(element_node->data_element)
	|| element_node->element_type != OM_ELEMENT)
    {
	// wrong element type or null node

	return NULL;
    }
    //ele = (om_element_t *)(element_node->data_element);


    ns = axis2c_om_element_find_declared_namespace(element_node, uri,
						   prefix);
    if (!ns)
    {
	return ns;
    }
    if ((element_node->parent != NULL)
	&& (element_node->parent->element_type == OM_ELEMENT))
    {
	axis2c_om_element_find_namespace(element_node->parent, uri,
					 prefix);
    }
    return NULL;
}


// declare a namespace for this om element

axis2c_om_namespace_t *axis2c_om_element_declare_namespace(node_t * element_node,
						    axis2c_om_namespace_t * ns)
{
    apr_status_t status;
    om_element_t *element = NULL;
    if (!element_node || !ns || !element_node->data_element
	|| element_node->element_type != OM_ELEMENT)
    {
	return NULL;
    }
    element = (om_element_t *) (element_node->data_element);
    if (!element->namespaces)
    {
	if (!om_pool)
	{
	    status = apr_pool_create(&om_pool, NULL);
	}
	element->namespaces = apr_hash_make(om_pool);
    }

    apr_hash_set(element->namespaces, ns->prefix, APR_HASH_KEY_STRING, ns);
    return ns;
}


axis2c_om_namespace_t
    *axis2c_om_element_declare_namespace_with_ns_uri_prefix(node_t *
							    element_node,
							    const char
							    *uri,
							    const char
							    *prefix)
{
    axis2c_om_namespace_t *nsp = NULL;
    nsp = axis2c_create_om_namespace(uri, prefix);
    if (nsp)
	return axis2c_om_element_declare_namespace(element_node, nsp);
    return NULL;
}

/*
*	checks for the namespace in the current om element 
*   can be used to retrive a prefix of a known namespace uri
*
*/
axis2c_om_namespace_t *axis2c_om_element_find_declared_namespace(node_t *
							  element_node,
							  const char *uri,
							  const char
							  *prefix)
{
    void *ns = NULL;
    om_element_t *element = NULL;
    if (!element_node || !ns || element_node->element_type != OM_ELEMENT)
    {
	return NULL;
    }
    element = (om_element_t *) (element_node->data_element);
    if (!prefix || strcmp(prefix, "") == 0)
    {
	// should traverse through the namespaces 


    }
    ns = apr_hash_get(element->namespaces, prefix, APR_HASH_KEY_STRING);
    return (void *) ns;
}

/*
*	This will find a namespace with the given uri and prefix, in the scope of the docuemnt.
* This will start to find from the current element and goes up in the hiararchy until this finds one.
*
*/



static axis2c_om_namespace_t *axis2c_om_element_handle_namespace_with_qname(node_t
								     *
								     element_node,
								     qname_t
								     *
								     qname)
{
    axis2c_om_namespace_t *pns = NULL;
    char *ns_uri = qname->ns_uri;
    if (ns_uri != NULL)
    {
	pns =
	    axis2c_om_element_find_namespace(element_node, ns_uri,
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
		    axis2c_om_element_declare_namespace_with_ns_uri_prefix
		    (element_node, ns_uri, qname->prefix);
	    }
	}
    }
    return NULL;
}







static axis2c_om_namespace_t *axis2c_om_element_handle_namespace(node_t *
							  element_node,
							  axis2c_om_namespace_t *
							  ns)
{
    axis2c_om_namespace_t *ns1 = NULL;
    if (!ns || !element_node)
    {
	// namespace null handle condition

    }
    ns1 =
	axis2c_om_element_find_namespace(element_node, ns->uri,
					 ns->prefix);

    if (!ns1)
    {
	ns1 = axis2c_om_element_declare_namespace(element_node, ns);
    }
    return ns1;
}


om_attribute_t *axis2c_om_element_add_attribute(node_t * element_node,
						om_attribute_t * attr)
{
    apr_status_t status;
    qname_t *t = NULL;
    om_element_t *element = NULL;
    char *key = NULL;

    if (!element_node || element_node->element_type != OM_ELEMENT)
    {
	//fprintf(stderr,"Error");
	return NULL;
    }
    element = (om_element_t *) (element_node->data_element);

    if (!(element->attributes))
    {
	if (!om_pool)
	{
	    status = apr_pool_create(&om_pool, NULL);
	    // error handling should go hear
	}
	element->attributes = apr_hash_make(om_pool);
    }

    t = axis2c_om_attribute_get_qname(attr);
    key = axis2c_construct_key_from_qname(t);
    apr_hash_set(element->attributes, key, APR_HASH_KEY_STRING, attr);
    return attr;
}

om_attribute_t *axis2c_om_element_get_attribute(node_t * element_node,
						qname_t * qname)
{
    char *key = NULL;
    om_element_t *element = NULL;
    if (!element_node || !qname
	|| element_node->element_type != OM_ELEMENT)
    {
	return NULL;
    }
    element = (om_element_t *) (element_node->data_element);
    key = axis2c_construct_key_from_qname(qname);
    return (om_attribute_t
	    *) (apr_hash_get(element->attributes, key,
			     APR_HASH_KEY_STRING));
}




static char *axis2c_construct_key_from_qname(qname_t * qname)
{

    // concatanation order is localpart prefix namespaceuri
    char *key = NULL;
    size_t i = 0;
    if (!qname)
    {
	//fprintf(stderr,"Error  null value for qname ");
	return NULL;
    }
    else
    {
	if (qname->localpart)
	    i += strlen(qname->localpart);
	if (qname->ns_uri)
	    i += strlen(qname->ns_uri);
	if (qname->prefix)
	    i += strlen(qname->prefix);
	key = (char *) calloc(i + 1, sizeof(char));
	if (qname->localpart)
	    strcat(key, qname->localpart);
	if (qname->prefix)
	    strcat(key, qname->prefix);
	if (qname->ns_uri)
	    strcat(key, qname->ns_uri);
	return key;

    }
    return NULL;
}

/*
*  The node passed to the method should have the data element as of type OM_ELEMENT
*
*
*
*/

om_attribute_t *axis2c_om_attribute_add_attribute_with_namespace(node_t *
								 element_node,
								 const char
								 *attribute_name,
								 const char
								 *value,
								 axis2c_om_namespace_t
								 * ns)
{
    axis2c_om_namespace_t *t = NULL;
    if (!element_node)
    {
	return NULL;

    }
    else if (element_node->element_type != OM_ELEMENT)
    {
	return NULL;
    }


    if (ns)
    {
	t = axis2c_om_element_find_namespace(element_node, ns->uri,
					     ns->prefix);
	if (t == NULL)
	{
	    //fprintf(stderr,"prefix %s   ns_uri  %s   This namespace is  not in the scope of this element",ns->prefix,ns->uri);
	    return NULL;
	}
    }
    return axis2c_om_element_add_attribute(element_node,
					   axis2c_create_om_attribute
					   (attribute_name, value, ns));
}

/*
void om_element_remove_attribute(om_element_t *element,om_attribute_t *attribute)
{
	qname_t *qname=NULL;
	char* key=NULL;
    if(!element || !(element->attributes))
	{
		// error handling 
		return NULL;
	}
	qname = om_attribute_get_qname(attribute);
	if(qname)
	{
		key=construct_key_from_qname(qname);
		//apr_hash_
	
	
	
	}






}
*/

void axis2c_om_element_set_namespace(node_t * node, axis2c_om_namespace_t * ns)
{
    axis2c_om_namespace_t *nsp = NULL;
    if (ns && node && (node->data_element))
    {
	nsp = axis2c_om_element_handle_namespace(node, ns);
    }
    ((om_element_t *) node->data_element)->ns = nsp;
}


void axis2c_free_om_element(om_element_t * element)
{
    if (element)
    {
	if (element->localname)
	    free(element->localname);
	if (element->ns)
	    axis2c_free_om_namespace(element->ns);
	if (element->attributes)
	{
	    // should find out how to free hashtable
	}
	if (element->namespaces)
	{

	    // same as above

	}
	free(element);
    }
}

void axis2c_om_element_set_localname(node_t * element_node,
				     const char *localname)
{
    om_element_t *element = NULL;
    if (!element_node || element_node->element_type != OM_ELEMENT)
    {
	// not correct element or null pointer
	return;
    }
    element = (om_element_t *) (element_node->data_element);
    if (element->localname)
    {
	free(element->localname);
    }
    element->localname = strdup(localname);
}

char *axis2c_om_element_get_localname(node_t * element_node)
{
    if (!element_node || element_node->element_type != OM_ELEMENT)
    {
	return NULL;
    }
    return strdup(((om_element_t *) (element_node->data_element))->
		  localname);
}
