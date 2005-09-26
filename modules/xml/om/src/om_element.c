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



node_t* create_om_element(const char* localname,om_namespace_t* ns)
{
	node_t* node=create_node();
	if(!(node))
	{
		om_element_t* element=(om_element_t*)malloc(sizeof(om_element_t));
		if(element)
		{
			element->localname = strdup(localname);
			element->ns		   = ns;
		}
		else
		{
			//could not allocate memory
			free(node);
			return NULL;
		}
		
		node->element_type=OM_ELEMENT;
		return node;
	}
return NULL;	
}
// create an om element using localname namespace and parent element

node_t *create_om_element_with_parent(const char *localname,om_namespace_t *ns,node_t *parent)
{
	node_t *curr_node = create_om_element(localname,ns);
	if(!curr_node)
	{
		//unable to create an element 
		return NULL;
	}
	if(!parent)
		return curr_node;

	curr_node->parent = parent;
	node_add_child(parent,curr_node);
	curr_node->done =TRUE;
	return curr_node;
}

// create an om_element using qname and parent 
node_t *create_om_element_with_qname_parent(qname_t *qname,node_t *parent)
{
	node_t *node=NULL;;
	if(!qname)
	{
		return NULL; // can't create an element
	
	}
	node=create_om_element_with_parent(qname->localpart,NULL,parent);
	if(node)
	{
		((om_element_t*)(node->data_element))->ns= om_element_handle_namespace_with_qname(node,qname);
	}
	return node;
}
om_namespace_t *om_element_declare_namespace_with_ns_uri_prefix(om_element_t *element,const char* uri,const char *prefix)
{
	om_namespace_t *nsp=NULL;
	nsp=create_om_namespace(uri,prefix);
	if(nsp)
		return om_element_declare_namespace(element,nsp);
	return NULL;
}






// declare a namespace for this om element

om_namespace_t *om_element_declare_namespace(om_element_t *element,om_namespace_t *ns)
{
	apr_status_t status;
	
	if(!element || !ns)
	{
		return NULL;
	}
		
	
	if(!element->namespaces)
		{
			if(!om_pool)
			{
				status=apr_pool_create(&om_pool,NULL);
			}
			element->namespaces = apr_hash_make(om_pool);
    	}
		
		apr_hash_set(element->namespaces,ns->prefix,APR_HASH_KEY_STRING,ns);
		return ns;
}

/*
*	checks for the namespace in the current om element 
*   can be used to retrive a prefix of a known namespace uri
*
*/
om_namespace_t *om_element_find_declared_namespace(om_element_t *element,const char *uri,const char *prefix)
{
	void *ns=NULL;
	if(!element || !ns)
	{
		return NULL;
	}
	if(!prefix || strcmp(prefix,"")== 0)
	{
		// should traverse through the namespaces 
	
	
	}
	ns=apr_hash_get(element->namespaces,prefix,APR_HASH_KEY_STRING);
	return (void*)ns;
}

/*
*	This will find a namespace with the given uri and prefix, in the scope of the docuemnt.
* This will start to find from the current element and goes up in the hiararchy until this finds one.
*
*/

om_namespace_t *om_element_find_namespace(node_t *element_node,const char *uri,const char *prefix)
{
	om_element_t *ele=NULL;
	om_namespace_t *ns=NULL;
	if(!element_node)
	{
		return NULL;
	}
	if(!(element_node->data_element) || element_node->element_type != OM_ELEMENT)
	{
		// wrong element type or null node
	
		return NULL;
	}
	ele = (om_element_t *)(element_node->data_element);

	
	ns = om_element_find_declared_namespace(ele,uri,prefix);
	if(!ns)
	{
		return ns;
	}
	if((element_node->parent != NULL) && (element_node->parent->element_type == OM_ELEMENT) )
	{
		om_element_find_namespace(element_node->parent,uri,prefix);
	}
return NULL;
}

static om_namespace_t *om_element_handle_namespace_with_qname(node_t *element_node,qname_t *qname)
{
	om_namespace_t *pns=NULL;
	char *ns_uri = qname->ns_uri;
	if(ns_uri != NULL)
	{
		pns = om_element_find_namespace(element_node,ns_uri,qname->prefix);
			/**
             * What is left now is
             *  1. nsURI = null & parent != null, but ns = null
             *  2. nsURI != null, (parent doesn't have an ns with given URI), but ns = null
             */
		if(pns == NULL)
		{
			if(qname->prefix)
			{
				pns = om_element_declare_namespace_with_ns_uri_prefix((om_element_t*)element_node->data_element,ns_uri,qname->prefix);
			}
		}
	}
	return NULL;
}







static om_namespace_t* om_element_handle_namespace(node_t *element_node,om_namespace_t *ns)
{
	om_namespace_t *ns1=NULL;
	if(!ns || !element_node)
	{
		// namespace null handle condition
		
	}
	ns1 = om_element_find_namespace( element_node,ns->uri,ns->prefix);

	if(!ns1)
	{
		ns1 = om_element_declare_namespace((om_element_t*)(element_node->data_element),ns);
	}
	return ns1; 
}


om_attribute_t* om_element_add_attribute(om_element_t *element,om_attribute_t *attr)
{
	apr_status_t status;
	qname_t *t=NULL;
	char *key=NULL;
	if(!element)
	{
		//fprintf(stderr,"Error");
		return NULL;
	}
	if(!(element->attributes))
	{
		if(!om_pool)
		{
		status = apr_pool_create(&om_pool,NULL);
		// error handling should go hear
		}
		element->attributes = apr_hash_make(om_pool);       
	}
	t=om_attribute_get_qname(attr);
	key=construct_key_from_qname(t);
	apr_hash_set(element->attributes,key,APR_HASH_KEY_STRING,attr);
	return attr;
}



static char* construct_key_from_qname(qname_t *qname)
{

	// concatanation order is localpart prefix namespaceuri
	char *key=NULL;
	size_t i=0;
	if(!qname){
		//fprintf(stderr,"Error  null value for qname ");
		return NULL;
	}else{
		if(qname->localpart)
			i+=strlen(qname->localpart);
		if(qname->ns_uri)
			i+=strlen(qname->ns_uri);
		if(qname->prefix)
			i+=strlen(qname->prefix);
		key = (char *)calloc(i+1,sizeof(char));
		if(qname->localpart)
			strcat(key,qname->localpart);
		if(qname->prefix)
			strcat(key,qname->prefix);
		if(qname->ns_uri)
			strcat(key,qname->ns_uri);
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

om_attribute_t *om_attribute_add_attribute_with_namespace(node_t *element_node,const char *attribute_name,const char *value,om_namespace_t *ns)
{	om_namespace_t *t=NULL;
	if(!element_node)
	{
		return NULL;
	
	}else if(element_node->element_type != OM_ELEMENT)
	{
		return NULL;
	}

	
	if(ns)
	{
		t=om_element_find_namespace(element_node,ns->uri,ns->prefix);	
		if(t == NULL)
		{
			//fprintf(stderr,"prefix %s   ns_uri  %s   This namespace is  not in the scope of this element",ns->prefix,ns->uri);
			return NULL;	
		}
	}
	return om_element_add_attribute((om_element_t*)element_node->data_element,create_om_attribute(attribute_name,value,ns));
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

void om_element_set_namespace(node_t* node,om_namespace_t *ns)
{
	om_namespace_t *nsp=NULL;
	if(ns && node  && (node->data_element))
	{
		nsp = om_element_handle_namespace(node,ns);
    }
	((om_element_t*)node->data_element)->ns = nsp;
}



