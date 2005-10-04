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

#include <axis2_stax_ombuilder.h>
#include <axis2_om_node.h>
#include <stdlib.h>
#include <namespace.h>
#include <axis2_om_element.h>
#include <axis2_errno.h>
#include <xmlpullparser.h>

axis2_stax_om_builder_t *create_stax_om_builder(XML_PullParser *parser)
{
	axis2_om_document_t *document;
	axis2_stax_om_builder_t *builder = (axis2_stax_om_builder_t*)malloc(sizeof(axis2_stax_om_builder_t));
	if(!builder)
	{
		fprintf(stderr," %d Error ",AXIS2_ERROR_OM_MEMORY_ALLOCATION);
		return NULL;
	}
	builder->parser = parser;
	builder->cache  = TRUE;
	builder->parser_accessed = FALSE;
	builder->done   = FALSE;
	builder->lastnode=NULL;

	document = axis2_om_document_create(NULL,builder);
	if(document)
	{
		builder->document = document;
	}
	return builder;
}

axis2_om_node_t *axis2_stax_om_builder_create_om_element(
						axis2_stax_om_builder_t *builder)
{
	axis2_om_node_t *element_node;
	char *localname = XML_PullParser_getName(builder->parser);
	
	if(!(builder->lastnode))
	{	/*  */
		axis2_om_element_create_with_builder(NULL,localname,NULL,builder,element_node);
		builder->document->root_element = element_node;
		axis2_om_document_add_child(builder->document,element_node);
	}
	else if(builder->lastnode->done)
	{	/*  */
		 axis2_om_element_create_with_builder(builder->lastnode->parent,localname,NULL
				,builder,element_node);
		builder->lastnode->next_sibling = element_node;
		element_node->prev_sibling = builder->lastnode;
	}
	else 
	{	/* */
		axis2_om_element_create_with_builder(builder->lastnode,localname,NULL,builder,element_node);
		builder->lastnode->first_child = element_node;
		element_node->parent = builder->lastnode;
	}

	// process namespace data
	// process attributes

return NULL;
}

axis2_om_node_t *axis2_stax_om_builder_create_om_comment(
						axis2_stax_om_builder_t *builder)
{
	
	/* guththila does not support yet*/
	return NULL;
}


axis2_om_node_t *axis2_stax_om_builder_create_om_doctype(
						axis2_stax_om_builder_t *builder)
{
	/*  guththila does not support yet */
}


axis2_om_node_t *axis2_stax_om_builder_create_om_processing_instruction(
						axis2_stax_om_builder_t *builder)
{
	/* guththila does not support yet */





}

void axis2_stax_om_builder_end_element(axis2_stax_om_builder_t *builder)
{	
	axis2_om_node_t *parent;
	if(builder->lastnode->done)
	{
		parent = builder->lastnode->parent;
		parent->done = TRUE;
		builder->lastnode = parent;
	}
	else
	{
		builder->lastnode->done = TRUE;
	}
}


int axis2_stax_om_builder_next(axis2_stax_om_builder_t *builder)
{
	int token = 0;
	if(builder->done)
	{
		fprintf(stderr,"%d Errpr",-1);
		return NULL;
	}
	
	token = XML_PullParser_next (builder->parser);
	if(!(builder->cache))
	{
		return token;
	}


	switch(token)
	{
	case START_DOCUMENT:
		{
		
		}
		break;

	case START_ELEMENT:
		{
			builder->lastnode = axis2_stax_om_builder_create_om_element(builder);
		}
		break;
	case END_ELEMENT:
		{
		
			axis2_stax_om_builder_end_element(builder);
	
		
		
		}
		break;
	case CHARACTER:
		{
			builder->lastnode = axis2_stax_om_builder_create_om_text(builder);
		
		
		
		}
		break;
	case COMMENT:
		{
				
		
		
		}
		break;
		
		
	};
}


void axis2_stax_om_builder_process_attributes(axis2_stax_om_builder_t *builder,axis2_om_node_t *element_node)
{
	int i=0;
	axis2_om_namespace_t *ns=NULL;
	char *uri=NULL;
	char *prefix=NULL;
	
	int attribute_count = XML_PullParser_getAttributeCount(builder->parser);
	for(i=0 ; i < attribute_count ; i++)
	{
			
		uri     = XML_PullParser_getAttributeNamespace_by_number(builder->parser,i);
		prefix  = XML_PullParser_getAttributePrefix_by_number(builder->parser,i);
		if(strcmp(uri,"") != 0)
		{
			ns = axis2_om_element_find_namespace(element_node,uri,prefix);	
		}
		
		if(ns == NULL && (prefix != NULL) && 	(uri != NULL)
				&& (strcmp(prefix,XMLNS_PREFIX) == 0)
				&& (strcmp(uri,XMLNS_URI) == 0))
		{
			axis2_om_element_declare_namespace_with_ns_uri_prefix(element_node,
										XMLNS_URI,XMLNS_PREFIX);
			ns = axis2_om_element_find_namespace(element_node,uri,prefix);
			
			
		}
		axis2_om_element_add_attribute_with_namespace(element_node
				,XML_PullParser_getAttributeName_by_number(builder->parser,i)
				,XML_PullParser_getAttributeValue_by_number(builder->parser,i),ns);	
	}
}


axis2_om_node_t *axis2_stax_om_builder_create_om_text(axis2_stax_om_builder_t *builder)
{
	axis2_om_node_t *node;
	if(builder->lastnode)
	{
		fprintf(stderr," ERROR");
		return NULL;
	}
	if(builder->lastnode->done)
	{
		axis2_om_text_create(builder->lastnode->parent,
			XML_PullParser_getValue(builder->parser),node);		
	}
	else
	{
		axis2_om_text_create(builder->lastnode,
			XML_PullParser_getValue(builder->parser),node);		
		
	}
	return node;	
}


void axis2_stax_om_builder_discard(axis2_stax_om_builder_t *builder,axis2_om_node_t *element_node)
{
/*	axis2_om_node_t *element;
	if(element_node->done || !(builder->cache))
	{
		fprintf(stderr," Error");
		return;		
	}
	
		cache = FALSE;
		do
		(
			while(XML_PullParser_next(builder->parser) != END_ELEMENT);
				
		}while(
				
	 
	
	
	
	
	
	
	
	
	
	
*/	
	
	
}


axis2_om_node_t *axis2_stax_om_builder_process_namespace_data(axis2_stax_om_builder_t *builder,axis2_om_node_t *element,int is_soap_element)
{
	int i=0;
	char *nsuri  = NULL;
	char *prefix = NULL;
	NAMESPACE *NS;
	axis2_om_namespace_t *ns,*ns1;
	int namespace_count = XML_PullParser_getNamespaceCount (builder->parser);
	for( i = 0 ; i < namespace_count ; i++)
	{
		axis2_om_element_declare_namespace_with_ns_uri_prefix(element,XML_PullParser_getNamespaceUri_by_number(builder->parser,i),XML_PullParser_getNamespacePrefix_by_number(builder->parser,i));
	}
	/* set own namespace */
	NS = XML_PullParser_getNamespace(builder->parser);
	nsuri  =XML_PullParser_getNamespaceUri(builder->parser,NS);
	prefix =XML_PullParser_getNamespacePrefix(builder->parser,NS);
	if(strcmp(nsuri,"") != 0)
	{
		if(!prefix)
		{
			
			ns =  axis2_om_element_find_namespace(element,nsuri,"");
			if(!ns)
			{
				ns = axis2_om_element_declare_namespace_with_ns_uri_prefix(element,nsuri,"");			
			}
			if(((axis2_om_element_t*)element)->ns == ns)
			{
				axis2_om_element_set_namespace(element,ns);
			}
		}
		else
		{
			ns = axis2_om_element_find_namespace(element,nsuri,prefix);
			if(!ns)
			{
				ns1 = axis2_om_namespace_create(nsuri,prefix);
				axis2_om_element_set_namespace(element,ns);
			}
			else
			{
				axis2_om_element_set_namespace(element,ns);
			}
		}
	}
}

char *axis2_stax_om_builder_get_attribute_prefix(axis2_stax_om_builder_t *builder,int i)
{
	return XML_PullParser_getAttributePrefix_by_number(builder->parser,i);
}



char *axis2_stax_om_builder_get_attribute_name(axis2_stax_om_builder_t *builder,int i)
{
	return XML_PullParser_getAttributeName_by_number(builder->parser,i);
}

int axis2_stax_om_builder_get_attribute_count(axis2_stax_om_builder_t *builder)
{
	return XML_PullParser_getAttributeCount(builder->parser);	
}

char *axis2_stax_om_builder_get_attribute_namespace(axis2_stax_om_builder_t *builder,int i)
{
	XML_PullParser_getAttributeNamespace_by_number(	builder->parser,i);
}
