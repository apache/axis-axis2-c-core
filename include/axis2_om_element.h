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

#ifndef AXIS2_OM_ELEMENT_H
#define AXIS2_OM_ELEMENT_H

#include <axis2_om_namespace.h>
#include <axis2_om_attribute.h>
#include <axis2_om_output.h>
#include <axis2_om_node.h>
#include <apr.h>
#include <apr_hash.h>



struct axis2_om_element_s{
	axis2_om_namespace_t *ns;			/* current namespace*/
	char *localname;			
	int pns_counter;            /* prefix namespace counter*/
	apr_hash_t *attributes;     /* a hashtable for storing attributes */
	apr_hash_t *namespaces;		/* hashtable for storing namespaces*/
};


/*
*	Create an om element using localname and namespace and parent
*@param localname can't be null
*@param ns   namespace can be null
*@param parent can be null
*@return   Returns axis2_om_element_t pointer If there isn't enough memory null is returned
*/
axis2_om_element_t *axis2_om_element_create(axis2_om_node_t *parent,
						const char *localname,axis2_om_namespace_t *ns,
						axis2_om_node_t **node);

/**
 *	create an om element using a qname 
 * @param qname
 * @param parent 
 * @return axis2_om_element_t pointer or NULL if there isn't enough memory
 */


axis2_om_element_t *axis2_om_element_create_with_qname(axis2_om_node_t *parent,axis2_qname_t *qname
						,axis2_om_node_t **node);

axis2_om_element_t *axis2_om_element_create_with_builder(axis2_om_node_t *parent,
		const char *localname,axis2_om_namespace_t *ns,axis2_stax_om_builder_t *builder,axis2_om_node_t **node);
													

/*
*	Find a namespace in the scope of the document 
*	start to find from current element and go up the hierarchy
*/

axis2_om_namespace_t *axis2_om_element_find_namespace(
			axis2_om_node_t *element_node, const char *uri,const char *prefix);
/*
declare a namespace in current element (in the scope of this element )
*/

axis2_om_namespace_t *axis2_om_element_declare_namespace(axis2_om_node_t *element_node,axis2_om_namespace_t *ns);
/**
 *	find namespaces in the scope of current element
 */

axis2_om_namespace_t *axis2_om_element_find_declared_namespace(axis2_om_node_t *element_node,const char *uri,const char *prefix);

/**
 *	add an attribute to current element
 */

axis2_om_attribute_t *axis2_om_element_add_attribute(axis2_om_node_t *element_node,axis2_om_attribute_t *attribute);

/**
 *	return the requested attribute
 */

axis2_om_attribute_t *axis2_om_element_get_attribute(axis2_om_node_t *element_node,axis2_qname_t *qname);

/**
 *	add an attribute
 */

axis2_om_attribute_t *axis2_om_element_add_attribute_with_namespace(
		axis2_om_node_t* element_node,const char *attribute_name,const char *value,axis2_om_namespace_t *ns);

/**
 *
 */


static axis2_om_namespace_t *axis2_om_element_handle_namespace_with_qname(axis2_om_node_t *element_node,axis2_qname_t *qname);
/*
void om_element_remove_attribute(om_element_t *element,om_attribute_t *attribute);
*/

/**
 *	set the namespace of this element
 */

void axis2_om_element_set_namespace(axis2_om_node_t *element_node,axis2_om_namespace_t *ns);
/**
 *	declare a namespace in the scope of this element
 */

axis2_om_namespace_t *axis2_om_element_declare_namespace_with_ns_uri_prefix(axis2_om_node_t *element_node,const char* uri,const char *prefix);
/**
 *	Free Om element 
 *
 */


void axis2_free_om_element(axis2_om_element_t *element);
/**
 *	set localname
 */

void axis2_om_element_set_localname(axis2_om_node_t *element_node,const char *localname);

/**
 *	get Localname
 */
char *axis2_om_element_get_localname(axis2_om_node_t *element_node);

int axis2_om_element_serialize_start_part(axis2_om_element_t *element_node, axis2_om_output_t* om_output);

int axis2_om_element_serialize_end_part(axis2_om_element_t *element_node, axis2_om_output_t* om_output);

#endif	// AXIS2_OM_ELEMENT_H
