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
#include <axis2c_om_attribute.h>
#include <axis2c_node.h>
#include <apr.h>
#include <apr_hash.h>



typedef struct axis2c_om_element_t{
	axis2c_om_namespace_t *ns;			// current namespace
	char *localname;			
	int pns_counter;            // prefix namespace counter
	apr_hash_t *attributes;     // a hashtable for storing attributes 
	apr_hash_t *namespaces;		// hashtable for storing namespaces
}axis2c_om_element_t;


/*
*	Create an om element using localname and namespace and parent
*@param localname can't be null
*@param ns   namespace can be null
*@param parent can be null
*@return   Returns axis2c_node_t pointer If there isn't enough memory null is returned
*/
axis2c_node_t *axis2c_om_element_create(const char *localname,axis2c_om_namespace_t *ns
										,axis2c_node_t *parent);

/**
 *	create an om element using a qname 
 * @param qname
 * @param parent 
 * @return axis2c_node_t pointer or NULL if there isn't enough memory
 */


axis2c_node_t *axis2c_om_element_create_with_qname(axis2c_qname_t *qname,axis2c_node_t *parent);

axis2c_node_t *axis2c_om_element_create_with_builder(const char *localname
					,axis2c_om_namespace_t *ns,axis2c_node_t *parent,axis2c_stax_om_builder_t *builder);
													

/*
*	Find a namespace in the scope of the document 
*	start to find from current element and go up the hierarchy
*/

axis2c_om_namespace_t *axis2c_om_element_find_namespace(
			axis2c_node_t *element_node, const char *uri,const char *prefix);
/*
declare a namespace in current element (in the scope of this element )
*/

axis2c_om_namespace_t *axis2c_om_element_declare_namespace(axis2c_node_t *element_node,axis2c_om_namespace_t *ns);
/**
 *	find namespaces in the scope of current element
 */

axis2c_om_namespace_t *axis2c_om_element_find_declared_namespace(axis2c_node_t *element_node,const char *uri,const char *prefix);

/**
 *	add an attribute to current element
 */

axis2c_om_attribute_t *axis2c_om_element_add_attribute(axis2c_node_t *element_node,axis2c_om_attribute_t *attribute);

/**
 *	return the requested attribute
 */

axis2c_om_attribute_t *axis2c_om_element_get_attribute(axis2c_node_t *element_node,axis2c_qname_t *qname);

/**
 *	add an attribute
 */

axis2c_om_attribute_t *axis2c_om_element_add_attribute_with_namespace(
		axis2c_node_t* element_node,const char *attribute_name,const char *value,axis2c_om_namespace_t *ns);

/**
 *
 */


static axis2c_om_namespace_t *axis2c_om_element_handle_namespace_with_qname(axis2c_node_t *element_node,axis2c_qname_t *qname);
/*
void om_element_remove_attribute(om_element_t *element,om_attribute_t *attribute);
*/

/**
 *	set the namespace of this element
 */

void axis2c_om_element_set_namespace(axis2c_node_t *element_node,axis2c_om_namespace_t *ns);
/**
 *	declare a namespace in the scope of this element
 */

axis2c_om_namespace_t *axis2c_om_element_declare_namespace_with_ns_uri_prefix(axis2c_node_t *element_node,const char* uri,const char *prefix);
/**
 *	Free Om element 
 *
 */


void axis2c_free_om_element(axis2c_om_element_t *element);
/**
 *	set localname
 */

void axis2c_om_element_set_localname(axis2c_node_t *element_node,const char *localname);

/**
 *	get Localname
 */
char *axis2c_om_element_get_localname(axis2c_node_t *element_node);



#endif	// AXISC_OM_ELEMENT_H
