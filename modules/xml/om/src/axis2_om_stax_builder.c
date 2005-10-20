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
#include <axis2_om_element.h>
#include <axis2_om_text.h>
#include <guththila_xml_pull_parser.h>

const axis2_char_t XMLNS_URI[] ="http://www.w3.org/XML/1998/namespace";
const axis2_char_t XMLNS_PREFIX[]= "xml";

axis2_om_node_t *axis2_om_stax_builder_impl_next(axis2_environment_t *environment, axis2_om_stax_builder_t *builder);
axis2_status_t axis2_om_stax_builder_impl_discard_current_element(axis2_environment_t *environment, axis2_om_stax_builder_t *builder);

static int isnot_whitespace(axis2_char_t s[])
{
   
    int n=0;
     
    for(n = strlen(s)-1;n>=0;n--)
        if(s[n] !=' ' && s[n] !='\t' && s[n] != '\n')
            break;
    if(n== -1)
        n =0;
    
    return n;
}

static int trim(axis2_char_t s[])
{
    int n;
    for(n = strlen(s)-1;n>=0;n--)
        if(s[n] !=' ' && s[n] !='\t' && s[n] != '\n')
            break;
    s[n+1] = '\0';
  return n;
}

axis2_om_stax_builder_t *axis2_om_stax_builder_create(axis2_environment_t *environment, void *parser)
{
	axis2_om_stax_builder_t *builder = (axis2_om_stax_builder_t*)axis2_malloc(environment->allocator, sizeof(axis2_om_stax_builder_t));

	if (!builder)
    {
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
		return NULL;
    }
	
	builder->parser = NULL;
	if (parser)
		builder->parser = parser;
	else 
	{
		/* create the default Guththila pull parser */
		guththila_reader_t *reader = guththila_reader_create (stdin);
		
		if (!reader)
		{
			environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
			return NULL;
		}
		
		builder->parser = guththila_xml_pull_parser_create (reader);
		
		if (!builder->parser)
		{
			environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
			return NULL;
		}
	}	
		
	builder->cache   = AXIS2_TRUE;
	builder->parser_accessed = AXIS2_FALSE;
	builder->done   = AXIS2_FALSE;
	builder->lastnode = NULL;
    builder->document= NULL;
	
	/* operations */
    builder->ops = NULL;
    builder->ops = (axis2_om_stax_builder_ops_t*) axis2_malloc(environment->allocator, sizeof(axis2_om_stax_builder_t));
    
    if (!builder->ops)
    {
        guththila_xml_pull_parser_free(builder->parser);
		axis2_free(environment->allocator, builder);
		environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
		return NULL;
    }
	
	builder->ops->axis2_om_stax_builder_ops_next = axis2_om_stax_builder_impl_next;
	builder->ops->axis2_om_stax_builder_ops_discard_current_element = axis2_om_stax_builder_impl_discard_current_element;
	
	return builder;
}

axis2_status_t axis2_om_stax_builder_process_attributes(axis2_environment_t *environment, axis2_om_stax_builder_t *builder, axis2_om_node_t *element_node)
{
	int i = 0;
	axis2_om_attribute_t *attribute = NULL;
	axis2_om_namespace_t *ns = NULL;
	axis2_char_t *uri = NULL;
	axis2_char_t *prefix = NULL;
	axis2_status_t status = AXIS2_SUCCESS;
	
	int attribute_count = guththila_xml_pull_parser_get_attribute_count(builder->parser);
	for(i = 0 ; i < attribute_count ; i++)
	{
	    
		uri     = guththila_xml_pull_parser_get_attribute_namespace_by_number(builder->parser, i);
		prefix  = guththila_xml_pull_parser_get_attribute_prefix_by_number(builder->parser, i);
		
		if(uri){
		    if(axis2_strcmp(environment->string, uri," ") != 0);
		    {
			    ns = axis2_om_element_find_namespace(environment, element_node, uri, prefix);	
		    }
		}
		
		if(ns == NULL && prefix && uri	&& (axis2_strcmp(environment->string, prefix, XMLNS_PREFIX) == 0)
				&& (axis2_strcmp(environment->string, uri, XMLNS_URI) == 0))
		{
			ns = axis2_om_namespace_create(environment, XMLNS_URI, XMLNS_PREFIX);
			if (ns)
			{
				status = axis2_om_element_declare_namespace(environment, element_node, ns);
			}
			else
			{
				status = AXIS2_FAILURE;
			}
										
			ns = axis2_om_element_find_namespace(environment, element_node, uri, prefix);			
		}
		attribute = axis2_om_attribute_create(environment, guththila_xml_pull_parser_get_attribute_name_by_number(builder->parser,i), 
				guththila_xml_pull_parser_get_attribute_value_by_number(builder->parser,i), ns);
		status = axis2_om_element_add_attribute(environment, (axis2_om_element_t*)element_node->data_element, attribute);
					
	}
	
	return status;
	
}


axis2_om_node_t *axis2_om_stax_builder_create_om_text(axis2_environment_t *environment, axis2_om_stax_builder_t *builder)
{
    axis2_char_t *value = NULL;
	axis2_char_t *temp_value = NULL;
	axis2_om_node_t *node = NULL;

	if (!builder->lastnode)
	{
		environment->error->errorno = AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL;        
		return NULL;
	}
	
	temp_value = guththila_xml_pull_parser_get_value(builder->parser);
	
	if(!temp_value)
	{
		environment->error->errorno = AXIS2_ERROR_PULL_PARSER_VALUE_NULL;        
		return NULL;
	}
	else
	{
		value = axis2_strdup(environment->string, temp_value);
		axis2_free(environment->allocator, temp_value); /*should use guththila free here*/
	}
	/* Parser should fire a SPACE event here
	if(!isnot_whitespace(value))
	        return NULL;
	*/
	if(builder->lastnode->done)
	{
		axis2_om_text_create(environment, builder->lastnode->parent, value, &node);
					
	}
	else
	{  
		 axis2_om_text_create(environment, builder->lastnode, value, &node);
	}
	
	node->done = AXIS2_TRUE;	
	builder->lastnode = node;
	
	return node;	
}


axis2_status_t axis2_om_stax_builder_impl_discard_current_element(axis2_environment_t *environment, axis2_om_stax_builder_t *builder)
{
   axis2_om_node_t *element=NULL;
   axis2_om_node_t *prev_node=NULL;
   axis2_om_node_t *parent = NULL;
   element = builder->lastnode;
	
	if(element->done || !(builder->cache))
	{
		environment->error->errorno = AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD;
		return AXIS2_FAILURE;		
	}
	
	builder->cache = AXIS2_FALSE;
	do
	{   
		while(guththila_xml_pull_parser_next(builder->parser) != GUTHTHILA_END_ELEMENT);
	}while(!(element->done));
	
	/*All children of this element is pulled now*/
	
	prev_node = element->prev_sibling;
	if(prev_node)
	{
		axis2_om_node_free(environment, prev_node->next_sibling);
	    prev_node->next_sibling = NULL;
	
	}else
	{
	    parent = element->parent;
		axis2_om_node_free(environment, parent->first_child);
	    parent->first_child = NULL;
	    builder->lastnode = parent;
	}
	builder->cache = AXIS2_TRUE;
	
	return AXIS2_SUCCESS;
}

/*
axis2_status_t axis2_om_stax_builder_process_start_document(axis2_environment_t *environment, axis2_om_stax_builder_t* builder)
{
  
    ATTRIBUTE *a;
	axis2_char_t *p;
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
*/

axis2_status_t axis2_om_stax_builder_process_namespaces(axis2_environment_t *environment, axis2_om_stax_builder_t *builder, axis2_om_node_t *node, int is_soap_element)
{
	axis2_char_t *nsuri  = NULL;
	axis2_char_t *prefix = NULL;
	guththila_namespace_t *ns = NULL;
	axis2_status_t status = AXIS2_SUCCESS;	
	int namespace_count = 0;	
	axis2_om_namespace_t *om_ns = NULL;
	axis2_char_t *temp_prefix = NULL;
	
	namespace_count = guththila_xml_pull_parser_get_namespacecount (builder->parser);
	
	for( ;namespace_count > 0 ; namespace_count--)
	{
		om_ns = axis2_om_namespace_create(environment, guththila_xml_pull_parser_get_namespace_uri_by_number(builder->parser,namespace_count), 
				guththila_xml_pull_parser_get_namespace_prefix_by_number(builder->parser,namespace_count));
		if (om_ns)
		{
			status = axis2_om_element_declare_namespace(environment, node, om_ns);
		}
		else
		{
			/* something went wrong */
			return AXIS2_FAILURE;
		}	
	}
	/* set own namespace */
	temp_prefix = guththila_xml_pull_parser_get_prefix(builder->parser);
	
	if (temp_prefix)
	{
		om_ns =  axis2_om_element_find_namespace(environment, node, NULL, temp_prefix);
		
		if (om_ns)
		{
				((axis2_om_element_t*)(node->data_element))->ns = om_ns;
		}
		else
		{
			environment->error->errorno = AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE;
			return AXIS2_FAILURE;
		}
		
	}
	
	/*ns = guththila_xml_pull_parser_get_namespace(builder->parser);
	nsuri  = guththila_xml_pull_parser_get_namespace_uri(builder->parser, ns);
	prefix = guththila_xml_pull_parser_get_namespace_prefix(builder->parser, ns);
    
	if(nsuri && axis2_strcmp(environment->string, nsuri, "") != 0)
	{
		if(!prefix)
		{
			
			om_ns =  axis2_om_element_find_namespace(environment, node, nsuri, "");
			if(!om_ns)
			{
				om_ns = axis2_om_namespace_create(environment, nsuri, "");
				if (om_ns)
				{
					status = axis2_om_element_declare_namespace(environment, node, om_ns);
				}
				else
				{

					return AXIS2_FAILURE;
				}
			}
			
			if(((axis2_om_element_t*)(node->data_element))->ns != om_ns)
			{
				((axis2_om_element_t*)(node->data_element))->ns = om_ns;
			}
		}
		else
		{
			om_ns = axis2_om_element_find_namespace(environment, node, nsuri, prefix);
			
			if(!om_ns)
			{
				om_ns = axis2_om_namespace_create(environment, nsuri, prefix);
				if (om_ns)
				{
					status = axis2_om_element_declare_namespace(environment, node, om_ns);
				}
				else
				{
					return AXIS2_FAILURE;
				}
			}
			
			if(((axis2_om_element_t*)(node->data_element))->ns != om_ns)
			{
				((axis2_om_element_t*)(node->data_element))->ns = om_ns;
			}
		}
	}*/
	
	return status;
}

axis2_om_node_t *axis2_om_stax_builder_create_om_element(axis2_environment_t *environment, 
						axis2_om_stax_builder_t *builder)
{
	axis2_om_node_t *element_node;
	axis2_char_t *localname = NULL;
	
	axis2_char_t *temp_localname = guththila_xml_pull_parser_get_name (builder->parser);
	
	if (!temp_localname)
	{
		environment->error->errorno = AXIS2_ERROR_PULL_PARSER_ELEMENT_NULL;
        return NULL;
	}
	else
	{
		localname = axis2_strdup(environment->string, temp_localname);
		axis2_free(environment->allocator, temp_localname); /*we should be using guththila free here*/
	}
	
	if(!(builder->lastnode))
	{	
		axis2_om_element_create(environment, NULL,localname,NULL,&element_node);
		
		if (builder->document->root_element)
			axis2_om_node_free(environment, builder->document->root_element);
		builder->document->root_element = element_node;
	}
	else if(builder->lastnode->done)
	{	
		axis2_om_element_create(environment, builder->lastnode->parent, localname, NULL, &element_node);
		builder->lastnode->next_sibling = element_node;
		element_node->prev_sibling = builder->lastnode;
	}
	else 
	{	
		axis2_om_element_create(environment, builder->lastnode,localname,NULL,&element_node);
		builder->lastnode->first_child = element_node;
		element_node->parent = builder->lastnode;
	}
    axis2_om_stax_builder_process_attributes(environment, builder, element_node);
	axis2_om_stax_builder_process_namespaces(environment, builder, element_node, 0);
	
	builder->lastnode = element_node;

	return element_node;
}

axis2_om_node_t *axis2_om_stax_builder_create_om_comment( axis2_environment_t *environment, 
						axis2_om_stax_builder_t *builder)
{
	
	/* guththila does not support comments*/
	return NULL;
}


axis2_om_node_t *axis2_om_stax_builder_create_om_doctype(axis2_environment_t *environment, 
						axis2_om_stax_builder_t *builder)
{
	/*  guththila does not support doctype */
	return NULL;
}


axis2_om_node_t *axis2_om_stax_builder_create_om_processing_instruction(axis2_environment_t *environment, 
						axis2_om_stax_builder_t *builder)
{
	/* guththila does not support processing instrtuctions */
	return NULL;
}

axis2_status_t axis2_om_stax_builder_end_element(axis2_environment_t *environment, axis2_om_stax_builder_t *builder)
{	
	axis2_om_node_t *parent;
	if(builder->lastnode)
	{
    	if(builder->lastnode->done)
	    {
		    parent = builder->lastnode->parent;
			if (parent)
			{
				parent->done = AXIS2_TRUE;
				builder->lastnode = parent;
			}
	    }
	    else
	    {
		    builder->lastnode->done = AXIS2_TRUE;
	    }
	}
	
	return AXIS2_SUCCESS;
}


axis2_om_node_t *axis2_om_stax_builder_impl_next(axis2_environment_t *environment, axis2_om_stax_builder_t *builder)
{
	int token = 0;
	axis2_om_node_t *node = NULL;
	
	do 
	{
		if(builder->done)
		{
			environment->error->errorno = AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL;
			return NULL;		
		}
		
		token = guththila_xml_pull_parser_next (builder->parser);
	
		if(!(builder->cache))
		{
			return NULL;
		}
	   
	
		switch(token)
		{
		case GUTHTHILA_START_DOCUMENT:
			/*axis2_om_stax_builder_process_start_document(builder);*/
			/*Do nothing */
			break;
		case GUTHTHILA_START_ELEMENT:
			node = axis2_om_stax_builder_create_om_element(environment, builder);
			break;
		case GUTHTHILA_EMPTY_ELEMENT:
			node = axis2_om_stax_builder_create_om_element(environment, builder);
		case GUTHTHILA_END_ELEMENT:
			  axis2_om_stax_builder_end_element(environment, builder);
			break;
		case GUTHTHILA_SPACE:
			/* Do nothing*/
			break;
		case GUTHTHILA_CHARACTER:
			node = axis2_om_stax_builder_create_om_text(environment, builder);
			break;
		case GUTHTHILA_COMMENT:
			break;
		}
	} while (!node);
	
	return node;	
}

/*
axis2_status_t axis2_om_stax_builder_process_attributes(axis2_environment_t *environment, axis2_om_stax_builder_t *builder, axis2_om_node_t *element_node)
{
	int i=0;
	axis2_om_namespace_t *ns = NULL;
	axis2_char_t *uri = NULL;
	axis2_char_t *prefix = NULL;
	axis2_status_t status = AXIS2_SUCCESS;
	
	int attribute_count = guththila_xml_pull_parser_get_attribute_count(builder->parser);
	for(i = 0 ; i < attribute_count ; i++)
	{
	    
		uri     = guththila_xml_pull_parser_get_attribute_namespace_by_number(builder->parser,i);
		prefix  = guththila_xml_pull_parser_get_attribute_prefix_by_number(builder->parser,i);
		
		if(uri){
		    if(axis2_strcmp(environment->string, uri," ") != 0);
		    {
			    ns = axis2_om_element_find_namespace(element_node, uri, prefix);	
		    }
		}
		
		if(ns == NULL && prefix && uri	&& (axis2_strcmp(environment->string, prefix, XMLNS_PREFIX) == 0)
				&& (axis2_strcmp(environment->string, uri, XMLNS_URI) == 0))
		{
			ns = axis2_om_element_declare_namespace_with_ns_uri_prefix(element_node,
										XMLNS_URI, XMLNS_PREFIX);
			ns = axis2_om_element_find_namespace(element_node, uri, prefix);			
		}
		
		status = axis2_om_element_add_attribute_with_namespace(element_node, 
				guththila_xml_pull_parser_get_attribute_name_by_number(builder->parser,i), 
				guththila_xml_pull_parser_get_attribute_value_by_number(builder->parser,i), ns);	
	}
	
	return status;
	
}
*/



/*
axis2_status_t axis2_om_stax_builder_process_start_document(axis2_environment_t *environment, axis2_om_stax_builder_t* builder)
{
  
    ATTRIBUTE *a;
	axis2_char_t *p;
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
*/
