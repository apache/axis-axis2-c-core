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

#ifndef AXIS2C_OM_ELEMENT_H
#define AXIS2C_OM_ELEMENT_H

#include <axis2c_om_namespace.h>
#include <axis2c_om_container.h>
#include <axis2c_om_attribute.h>
#include <axis2c_node.h>
#include <apr.h>
#include <apr_hash.h>

typedef struct axis2c_om_element_t {
    axis2c_om_namespace_t *ns;	// current namespace
    char *localname;
    //int done;
    int pns_counter;		// prefix namespace counter
    apr_hash_t *attributes;	// a hashtable for storing attributes 
    apr_hash_t *namespaces;	// hashtable for storing namespaces
} axis2c_om_element_t;

void axis2c_free_om_element(axis2c_om_element_t* element_node);
/*
*	Create an om element using localname and namespace
*
*/
axis2c_node_t *axis2c_create_om_element(const char *localname,
					axis2c_om_namespace_t * ns);
/*
*	create om element using localname namespace and parent element
*
*/
axis2c_node_t *axis2c_create_om_element_with_parent(const char *localname,
						    axis2c_om_namespace_t *
						    ns,
						    axis2c_node_t *
						    parent);


/*
*	Find a namespace in the scope of the document 
*	start to find from current element and go up the hierarchy
*
*/
axis2c_node_t *axis2c_create_om_element_with_qname_parent(axis2c_qname_t *
							  qname,
							  axis2c_node_t *
							  parent);






axis2c_om_namespace_t *axis2c_om_element_find_namespace(axis2c_om_element_t*
							element_node,
							const char *uri,
							const char
							*prefix);


// declare a namespace in current element
axis2c_om_namespace_t
    *axis2c_om_element_declare_namespace(axis2c_om_element_t * element,
					 axis2c_om_namespace_t * ns);

axis2c_om_namespace_t
    *axis2c_om_element_find_declared_namespace(axis2c_om_element_t *
					       element_node,
					       const char *uri, const char
					       *prefix);

axis2c_om_attribute_t *axis2c_om_element_add_attribute(axis2c_om_element_t
						       * element,
						       axis2c_om_attribute_t
						       * attribute);

axis2c_om_attribute_t
    *axis2c_om_attribute_add_attribute_with_namespace(axis2c_node_t *
						      element_node,
						      const char
						      *attribute_name, const char
						      *value,
						      axis2c_om_namespace_t
						      * ns);

static char *axis2c_construct_key_from_qname(axis2c_qname_t * qname);


static axis2c_om_namespace_t
    *axis2c_om_element_handle_namespace_with_qname(axis2c_node_t *
						   element_node,
						   axis2c_qname_t * qname);
//void om_element_remove_attribute(om_element_t *element,om_attribute_t *attribute);

void axis2c_om_element_set_namespace(axis2c_node_t * element_node,
				     axis2c_om_namespace_t * ns);

axis2c_om_namespace_t
    *
axis2c_om_element_declare_namespace_with_nsuri_prefix(axis2c_om_element_t *
						      element,
						      const char *uri,
						      const char *prefix);







#endif				// AXIS2C_OM_ELEMENT
