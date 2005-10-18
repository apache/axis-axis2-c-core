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
#include <axis2_hash.h>

struct axis2_om_element;
struct axis2_om_element_ops;

typedef struct axis2_om_element_ops
{
    /*
    *	Find a namespace in the scope of the document 
    *	start to find from current element and go up the hierarchy
    */
    
    axis2_om_namespace_t *(*axis2_om_element_ops_find_namespace)( axis2_environment_t *environment, 
                axis2_om_node_t *node, const axis2_char_t *uri,const axis2_char_t *prefix);
    /*
    declare a namespace in current element (in the scope of this element )
    */
    
    axis2_status_t (*axis2_om_element_ops_declare_namespace)(axis2_environment_t *environment, struct axis2_om_element *element, axis2_om_namespace_t *ns);
    /**
     *	find namespaces in the scope of current element
     */
    
    /*axis2_om_namespace_t *(*axis2_om_element_ops_find_declared_namespace)(axis2_environment_t *environment, struct axis2_om_element *element, const axis2_char_t *uri,const axis2_char_t *prefix);*/
    /**
     *
     */
    
    axis2_om_namespace_t *(*axis2_om_element_ops_find_namespace_with_qname)(axis2_environment_t *environment, axis2_om_node_t *node, axis2_qname_t *qname);
    
    /**
     *	add an attribute to current element
     * @param ns namespace Optional
     */
    
    axis2_status_t (*axis2_om_element_ops_add_attribute)(axis2_environment_t *environment, struct axis2_om_element *element, axis2_om_attribute_t *attribute);
    
    /**
     *	return the requested attribute
     */
    
    axis2_om_attribute_t *(*axis2_om_element_ops_get_attribute)(axis2_environment_t *environment, struct axis2_om_element *element, axis2_qname_t *qname);
    
    
    /**
     *	declare a namespace in the scope of this element
     */
    
    /*axis2_om_namespace_t *(*axis2_om_element_ops_declare_namespace_with_ns_uri_prefix)(axis2_environment_t *environment, struct axis2_om_element *element, const axis2_char_t* uri,const axis2_char_t *prefix);*/
    /**
     *	Free Om element 
     *
     */
    
    axis2_status_t (*axis2_om_element_ops_free)(axis2_environment_t *environment, struct axis2_om_element *element);
    
    axis2_status_t (*axis2_om_element_ops_serialize_start_part)(axis2_environment_t *environment, struct axis2_om_element *element, axis2_om_output_t* om_output);
    
    axis2_status_t (*axis2_om_element_ops_serialize_end_part)(axis2_environment_t *environment, struct axis2_om_element *element, axis2_om_output_t* om_output);

} axis2_om_element_ops_t;

typedef struct axis2_om_element{
	axis2_om_element_ops_t *ops;
    /** Element's namespace */
    axis2_om_namespace_t *ns;
    /** Element's local name */
	axis2_char_t *localname;	
    /** List of attributes */
	axis2_hash_t *attributes;     
    /** List of namespaces */
	axis2_hash_t *namespaces;		
} axis2_om_element_t;


/*
*	Create an om element using localname and namespace and parent
*@param localname can't be null
*@param ns   namespace can be null
*@param parent can be null
*@return   Returns axis2_om_element_t pointer If there isn't enough memory null is returned
*/
axis2_om_element_t *axis2_om_element_create(axis2_environment_t *environment, axis2_om_node_t *parent,
						const axis2_char_t *localname, axis2_om_namespace_t *ns,
						axis2_om_node_t **node);

/**
 *	create an om element using a qname 
 * @param qname
 * @param parent 
 * @return axis2_om_element_t pointer or NULL if there isn't enough memory
 */


axis2_om_element_t *axis2_om_element_create_with_qname(axis2_environment_t *environment, axis2_om_node_t *parent,axis2_qname_t *qname
						,axis2_om_node_t **node);

#define axis2_om_element_find_namespace(environment, node, uri, prefix) ((node->data_element)->ops->axis2_om_element_ops_find_namespace(environment, node, uri, prefix))
#define axis2_om_element_declare_namespace(environment, element, ns) ((element)->ops->axis2_om_element_ops_declare_namespace(environment, element, ns) )
#define axis2_om_element_find_namespace_with_qname (environment, element, qname) ((element)->ops->axis2_om_element_ops_find_namespace_with_qname (environment, element, qname))
#define axis2_om_element_add_attribute(environment, element, attribute) ((element)->ops->axis2_om_element_ops_add_attribute(environment, element, attribute))
#define axis2_om_element_get_attribute(environment, element, qname) ((element)->ops->axis2_om_element_ops_get_attribute(environment, element, qname))
#define axis2_om_element_free(environment, element) ((element)->ops->axis2_om_element_ops_free(environment, element);)
#define axis2_om_element_serialize_start_part(environment, element, om_output) ((element)->ops->axis2_om_element_ops_serialize_start_part(environment, element, om_output))
#define axis2_om_element_serialize_end_part(environment, element, om_output) ((element)->ops->axis2_om_element_ops_serialize_end_part(environment, element, om_output))


#endif	/* AXIS2_OM_ELEMENT_H */
