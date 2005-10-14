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

#include <axis2_om_stax_builder.h>
#include <axis2_om_node.h>
#include <stdlib.h>
#include <namespace.h>
#include <axis2_om_element.h>
#include <axis2_errno.h>
#include <xmlpullparser.h>
#include <axis2_om_text.h>

const char XMLNS_URI[] ="http://www.w3.org/XML/1998/namespace";
const char XMLNS_PREFIX[]= "xml";

static int isnot_whitespace(char s[])
{
   
    int n=0;
     
    for(n = strlen(s)-1;n>=0;n--)
        if(s[n] !=' ' && s[n] !='\t' && s[n] != '\n')
            break;
    if(n== -1)
        n =0;
    
    return n;
}

static int trim(char s[])
{
    int n;
    for(n = strlen(s)-1;n>=0;n--)
        if(s[n] !=' ' && s[n] !='\t' && s[n] != '\n')
            break;
    s[n+1] = '\0';
  return n;
}

axis2_om_stax_builder_t *axis2_om_stax_builder_create(XML_PullParser *parser)
{
	axis2_om_document_t *document;
	axis2_om_stax_builder_t *builder = (axis2_om_stax_builder_t*)malloc(sizeof(axis2_om_stax_builder_t));
	if(!builder)
	{
		fprintf(stderr," %d Error ",AXIS2_ERROR_OM_MEMORY_ALLOCATION);
		return NULL;
	}
	builder->parser  = parser;
	builder->cache   = TRUE;
	builder->parser_accessed = FALSE;
	builder->done   = FALSE;
	builder->lastnode = NULL;
    builder->document= NULL;
	return builder;
}


axis2_om_node_t *axis2_om_stax_builder_create_om_element(
						axis2_om_stax_builder_t *builder)
{
	axis2_om_node_t *element_node;
	char *localname = XML_PullParser_getName(builder->parser);
	
	if(!(builder->lastnode))
	{	/*  */
		axis2_om_element_create(NULL,localname,NULL,&element_node);
		builder->document->root_element = element_node;
		
		//axis2_om_document_add_child(builder->document,element_node);
	}
	else if(builder->lastnode->done)
	{	/*  */
		axis2_om_element_create(builder->lastnode->parent,localname,NULL,&element_node);
		builder->lastnode->next_sibling = element_node;
		element_node->prev_sibling = builder->lastnode;
	}
	else 
	{	/* */
		axis2_om_element_create(builder->lastnode,localname,NULL,&element_node);
		builder->lastnode->first_child = element_node;
		element_node->parent = builder->lastnode;
	}
    axis2_om_stax_builder_process_attributes(builder,element_node);
	axis2_om_stax_builder_process_namespace_data(builder,element_node,0);
	
	// process attributes

return element_node;
}

axis2_om_node_t *axis2_om_stax_builder_create_om_comment(
						axis2_om_stax_builder_t *builder)
{
	
	/* guththila does not support yet*/
	return NULL;
}


axis2_om_node_t *axis2_om_stax_builder_create_om_doctype(
						axis2_om_stax_builder_t *builder)
{
	/*  guththila does not support yet */
	return NULL;
}


axis2_om_node_t *axis2_om_stax_builder_create_om_processing_instruction(
						axis2_om_stax_builder_t *builder)
{
	/* guththila does not support yet */
	return NULL;
}

void axis2_om_stax_builder_end_element(axis2_om_stax_builder_t *builder)
{	
	axis2_om_node_t *parent;
	if(builder->lastnode)
	{
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
}


int axis2_om_stax_builder_next(axis2_om_stax_builder_t *builder)
{
	int token = 0;

	
	if(builder->done)
	{
		fprintf(stderr,"%d Error",-1);
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
		
		axis2_om_stax_builder_process_start_document(builder);
		}
		break;

	case START_ELEMENT:
		{
		    
		builder->lastnode = axis2_om_stax_builder_create_om_element(builder);
		}
		break;
	case END_ELEMENT:
		{
		  axis2_om_stax_builder_end_element(builder);
	   	}
		break;
	case CHARACTER:
		{
		
		builder->lastnode = axis2_om_stax_builder_create_om_text(builder);
		}
		break;
	case COMMENT:
		{
				
		
		
		}
		break;
		
		
	};
	
}


void axis2_om_stax_builder_process_attributes(axis2_om_stax_builder_t *builder,axis2_om_node_t *element_node)
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
		//printf("\nAttribute count %d %s %s\n",attribute_count,uri,prefix);
		
		if(uri){
		    if(strcmp(uri," ") != 0);
		    {
			    ns = axis2_om_element_find_namespace(element_node,uri,prefix);	
		    }
		}
		
		if(ns == NULL && prefix && uri	&& (strcmp(prefix,XMLNS_PREFIX) == 0)
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


axis2_om_node_t *axis2_om_stax_builder_create_om_text(axis2_om_stax_builder_t *builder)
{
    char *value;
	axis2_om_node_t *node=NULL;
	//axis2_om_text_t *t,*t1;
	value = XML_PullParser_getValue(builder->parser);
	

	
	
	if(!value || !(builder->lastnode) )
	{
	         return builder->lastnode;
	}
	
	if(!isnot_whitespace(value))
	        return builder->lastnode;
	
	if(builder->lastnode->done)
	{
		axis2_om_text_create(builder->lastnode->parent,
			value,&node);
					
	}
	else
	{  
		 axis2_om_text_create(builder->lastnode,value,&node);
	}
	return node;	
}


void axis2_om_stax_builder_discard_element(axis2_om_stax_builder_t *builder)
{
   axis2_om_node_t *element=NULL;
   axis2_om_node_t *prev_node=NULL;
   axis2_om_node_t *parent = NULL;
   element = builder->lastnode;
	
	if(element->done || !(builder->cache))
	{
		fprintf(stderr," Error");
		return;		
	}
	
	builder->cache = FALSE;
	do
	{   
		while(XML_PullParser_next(builder->parser) != END_ELEMENT);
	}while(!(element->done));
	
	//All children of this element is build 
	
	prev_node = element->prev_sibling;
	if(prev_node)
	{
	    prev_node->next_sibling = NULL;
	
	}else
	{
	    parent = element->parent;
	    parent->first_child = NULL;
	    builder->lastnode = parent;
	}
	builder->cache = TRUE;
}

void axis2_om_stax_builder_process_start_document(axis2_om_stax_builder_t* builder)
{
    /* skiping */
    ATTRIBUTE *a;
	char *p;
	 int ix;
    if(!(builder->document))
    {
        return;
    }
   
    ix = XML_PullParser_getAttributeCount (builder->parser);
    for (; ix > 0; ix--)
    {
		a = XML_PullParser_getAttribute (builder->parser);
		p = XML_PullParser_getAttributeName (builder->parser, a);
		free (p);
		p = XML_PullParser_getAttributeValue (builder->parser, a);
    	free (p);
    }
}







axis2_om_node_t *axis2_om_stax_builder_process_namespace_data(axis2_om_stax_builder_t *builder,axis2_om_node_t *element,int is_soap_element)
{
	int i=0;
	char *nsuri  = NULL;
	char *prefix = NULL;
	char *uri=NULL;
	char *prefixi=NULL;
	NAMESPACE *NS;
	
	
	int namespace_count;
	
	
	axis2_om_namespace_t *ns,*ns1;
	//e = Stack_last (builder->parser->dep);
	//d = e->depth->count;
	namespace_count = XML_PullParser_getNamespaceCount (builder->parser);
	
	for( ;namespace_count >0 ; namespace_count--)
	{
	   // uri = XML_PullParser_getNamespaceUri_by_number(builder->parser,namespace_count);
	   // prefix = XML_PullParser_getNamespacePrefix_by_number(builder->parser,namespace_count);
		axis2_om_element_declare_namespace_with_ns_uri_prefix(element,XML_PullParser_getNamespaceUri_by_number(builder->parser,namespace_count),XML_PullParser_getNamespacePrefix_by_number(builder->parser,namespace_count));
	
	}
	// set own namespace //
	
	NS = XML_PullParser_getNamespace(builder->parser);
	nsuri  =XML_PullParser_getNamespaceUri(builder->parser,NS);
	prefix =XML_PullParser_getNamespacePrefix(builder->parser,NS);
    
	if(strcmp(nsuri," ") != 0)
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
				axis2_om_element_set_namespace(element,ns1);
				
			}
			else
			{
			    
				axis2_om_element_set_namespace(element,ns);
			}
		}
	}
	
}

char *axis2_om_stax_builder_get_attribute_prefix(axis2_om_stax_builder_t *builder,int i)
{
	return XML_PullParser_getAttributePrefix_by_number(builder->parser,i);
}



char *axis2_om_stax_builder_get_attribute_name(axis2_om_stax_builder_t *builder,int i)
{
	return XML_PullParser_getAttributeName_by_number(builder->parser,i);
}

int axis2_om_stax_builder_get_attribute_count(axis2_om_stax_builder_t *builder)
{
	return XML_PullParser_getAttributeCount(builder->parser);	
}

char *axis2_om_stax_builder_get_attribute_namespace(axis2_om_stax_builder_t *builder,int i)
{
	XML_PullParser_getAttributeNamespace_by_number(	builder->parser,i);
}

