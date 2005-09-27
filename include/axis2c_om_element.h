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

#ifndef _AXISC_OM_ELEMENT_H_
#define _AXISC_OM_ELEMENT_H_

#include <axis2c_om_namespace.h>
#include <axis2c_om_container.h>
#include <axis2c_om_attribute.h>
#include <axis2c_node.h>
#include <apr.h>
#include <apr_hash.h>

struct om_element_s;
typedef struct om_element_s om_element_t;

struct om_element_s {
    axis2c_om_namespace_t *ns;	// current namespace
    char *localname;
    //int done;
    int pns_counter;		// prefix namespace counter
    apr_hash_t *attributes;	// a hashtable for storing attributes 
    apr_hash_t *namespaces;	// hashtable for storing namespaces
};

void free_om_element(node_t * element_node);
/*
*	Create an om element using localname and namespace
*
*/
node_t *create_om_element(const char *localname, axis2c_om_namespace_t * ns);
/*
*	create om element using localname namespace and parent element
*
*/
node_t *create_om_element_with_parent(const char *localname,
				      axis2c_om_namespace_t * ns,
				      node_t * parent);


/*
*	Find a namespace in the scope of the document 
*	start to find from current element and go up the hierarchy
*
*/
node_t *create_om_element_with_qname_parent(qname_t * qname,
					    node_t * parent);






axis2c_om_namespace_t *om_element_find_namespace(node_t * element_node,
						 const char *uri,
						 const char *prefix);


// declare a namespace in current element
axis2c_om_namespace_t *om_element_declare_namespace(om_element_t * element,
						    axis2c_om_namespace_t * ns);

axis2c_om_namespace_t *om_element_find_declared_namespace(om_element_t *
							  element_node,
							  const char *uri,
							  const char
							  *prefix);

om_attribute_t *om_element_add_attribute(om_element_t * element,
					 om_attribute_t * attribute);

om_attribute_t *om_attribute_add_attribute_with_namespace(node_t *
							  element_node,
							  const char
							  *attribute_name,
							  const char
							  *value,
							  axis2c_om_namespace_t *
							  ns);

static char *construct_key_from_qname(qname_t * qname);


static axis2c_om_namespace_t *om_element_handle_namespace_with_qname(node_t
								     *
								     element_node,
								     qname_t
								     *
								     qname);
//void om_element_remove_attribute(om_element_t *element,om_attribute_t *attribute);

void om_element_set_namespace(node_t * element_node, axis2c_om_namespace_t * ns);

axis2c_om_namespace_t
    *om_element_declare_namespace_with_nsuri_prefix(om_element_t * element,
						    const char *uri,
						    const char *prefix);







#endif				// _AXISC_OM_ELEMENT
