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

#include <axis2c_stax_ombuilder.h>
#include <axis2c_node.h>
#include <stdlib.h>
#include <namespace.h>
#include <axis2c_om_element.h>
#include <axis2c_errno.h>
#include <xmlpullparser.h>

axis2c_stax_om_builder_t *create_stax_om_builder(XML_PullParser *parser)
{
	axis2c_om_document_t *document;
	axis2c_stax_om_builder_t *builder = (axis2c_stax_om_builder_t*)malloc(sizeof(axis2c_stax_om_builder_t));
	if(!builder)
	{
		fprintf(stderr," %d Error ",AXIS2C_ERROR_OM_MEMORY_ALLOCATION);
		return NULL;
	}
	builder->parser = parser;
	builder->cache  = true;
	builder->parser_accessed = false;
	builder->done   = false;
	builder->lastnode=NULL;

	document = axis2c_om_document_create(NULL,builder);
	if(document)
	{
		builder->document = document;
	}
	return builder;
}

axis2c_node_t *axis2c_stax_om_builder_create_om_element(
						axis2c_stax_om_builder_t *builder)
{
	axis2c_node_t *element_node;
	char *localname = XML_PullParser_getName(builder->parser);
	
	if(!(builder->lastnode))
	{	/*  */
		element_node = axis2c_om_element_create_with_builder(localname,NULL,NULL,builder);
		builder->document->root_element = element_node;
		axis2c_om_document_add_child(builder->document,element_node);
	}
	else if(builder->lastnode->done)
	{	/*  */
		element_node = axis2c_om_element_create_with_builder(localname,NULL,
				builder->lastnode->parent,builder);
		builder->lastnode->next_sibling = element_node;
		element_node->prev_sibling = builder->lastnode;
	}
	else 
	{	/* */
		element_node = axis2c_om_element_create_with_builder(localname,NULL,builder->lastnode,builder);
		builder->lastnode->first_child = element_node;
		element_node->parent = builder->lastnode;
	}

	// process namespace data
	// process attributes

return NULL;
}

axis2c_node_t *axis2c_stax_om_builder_create_om_comment(
						axis2c_stax_om_builder_t *builder)
{
	axis2c_node_t *node;
	if(!builder)
	{
		return NULL;
	}
	/*****/
	return NULL;
}


axis2c_node_t *axis2c_stax_om_builder_create_om_doctype(
						axis2c_stax_om_builder_t *builder)
{
	/******/
}


axis2c_node_t *axis2c_stax_om_builder_create_om_processing_instruction(
						axis2c_stax_om_builder_t *builder)
{
	/******/





}

void axis2c_stax_om_builder_end_element(axis2c_stax_om_builder_t *builder)
{	
	axis2c_node_t *parent;
	if(builder->lastnode->done)
	{
		parent = builder->lastnode->parent;
		parent->done = true;
		builder->lastnode = parent;
	}
	else
	{
		builder->lastnode->done = true;
	}
}


int axis2c_stax_om_builder_next(axis2c_stax_om_builder_t *builder)
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
			builder->lastnode = axis2c_stax_om_builder_create_om_element(builder);
		}
		break;
	case END_ELEMENT:
		{
		
		
		
		
		}
		break;
	case CHARACTER:
		{
	
		
		
		
		}
		break;
	case COMMENT:
		{
		
		
		
		
		
		
		}
		break;
	};
}


void axis2c_stax_om_builder_process_attributes(axis2c_stax_om_builder_t *builder,axis2c_node_t *element_node)
{
}


axis2c_node_t *axis2c_stax_om_builder_create_om_text(axis2c_stax_om_builder_t *builder)
{
}


void axis2c_stax_om_builder_discard(axis2c_stax_om_builder_t *builder,axis2c_node_t *element_node)
{

}


axis2c_node_t *axis2c_stax_om_builder_process_namespace_data(axis2c_stax_om_builder_t *builder,axis2c_node_t *element,int is_soap_element)
{
	int i=0;
	char *nsuri  = NULL;
	char *prefix = NULL;
	NAMESPACE *NS;
	axis2c_om_namespace_t *ns,*ns1;
	int namespace_count = XML_PullParser_getNamespaceCount (builder->parser);
	for( i = 0 ; i < namespace_count ; i++)
	{
		axis2c_om_element_declare_namespace_with_ns_uri_prefix(element,XML_PullParser_getNamespaceUri_by_number(builder->parser,i),XML_PullParser_getNamespacePrefix_by_number(builder->parser,i));
	}
	/* set own namespace */
	NS = XML_PullParser_getNamespace(builder->parser);
	nsuri  =XML_PullParser_getNamespaceUri(builder->parser,NS);
	prefix =XML_PullParser_getNamespacePrefix(builder->parser,NS);
	if(strcmp(nsuri,"") != 0)
	{
		if(!prefix)
		{
			
			ns =  axis2c_om_element_find_namespace(element,nsuri,"");
			if(!ns)
			{
				ns = axis2c_om_element_declare_namespace_with_ns_uri_prefix(element,nsuri,"");			
			}
			if(((axis2c_om_element_t*)element)->ns == ns)
			{
				axis2c_om_element_set_namespace(element,ns);
			}
		}
		else
		{
			ns = axis2c_om_element_find_namespace(element,nsuri,prefix);
			if(!ns)
			{
				ns1 = axis2c_om_namespace_create(nsuri,prefix);
				axis2c_om_element_set_namespace(element,ns);
			}
			else
			{
				axis2c_om_element_set_namespace(element,ns);
			}
		}
	}
}
