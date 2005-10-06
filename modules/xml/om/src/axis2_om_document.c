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


#include <axis2_om_document.h>
#include <stdlib.h>
#include <axis2_errno.h>
#include <axis2_stax_ombuilder.h>

axis2_om_document_t *axis2_om_document_create(axis2_om_node_t * root_ele,axis2_stax_om_builder_t * builder)
{

    axis2_om_document_t *doc = (axis2_om_document_t *) malloc(sizeof(axis2_om_document_t));
    if (!doc)
    {
		fprintf(stderr,"%d Error",AXIS2_ERROR_OM_MEMORY_ALLOCATION);
		return NULL;
    }
    doc->builder = builder;
    doc->root_element = root_ele;
    doc->first_child = NULL;
    doc->last_child = NULL;
    doc->char_set_encoding = CHAR_SET_ENCODING;
    doc->xml_version = XML_VERSION;
    doc->done = FALSE;
    if(builder)
    {
        builder->document=doc;
    }
    return doc;
}


void axis2_free_om_document(axis2_om_document_t * doc)
{
	if(doc)
	{
		if(doc->char_set_encoding)
			free(doc->char_set_encoding);
		if(doc->xml_version)
			free(doc->xml_version);
		/*   */		
	
	free(doc);
	}
}


void axis2_om_document_set_char_set_encoding(axis2_om_document_t *document,
					    const char *char_set_encoding)
{
    if (document)
    {
		if (document->char_set_encoding)
		{
			free(document->char_set_encoding);
		}
	document->char_set_encoding = strdup(char_set_encoding);
    }
}

void axis2_om_document_add_child(axis2_om_document_t * document,
				  axis2_om_node_t * child)
{
   if(!(document->root_element) && child)
   {
        document->root_element = child;
   
   }
   
   if(document->root_element && child)
   {
        axis2_om_node_add_child(document->root_element,child);
   }
   
}


void axis2_om_document_set_xmlversion(axis2_om_document_t *document,const char *xmlversion)
{
	if(document)
	{
		if(document->xml_version)
		{
			free(document->xml_version);
		}
		document->xml_version = strdup(xmlversion);
	}
}


void axis2_om_document_build_next(axis2_om_document_t *document)
{
   // printf("next");
	axis2_stax_om_builder_next(document->builder);
}


axis2_om_node_t *axis2_om_document_get_root_element(axis2_om_document_t *document)
{
        if(document && document->root_element)
        {
            return document->root_element;
        }
        else
        {   
            while(!document->root_element)
                axis2_om_document_build_next(document);
        }
        
        return document->root_element;
           
}

axis2_om_node_t *axis2_om_document_get_next_sibling(axis2_om_document_t *document)
{
    axis2_om_node_t *lastchild =document->builder->lastnode;
    if(document && document->builder && document->builder->lastnode)
    {
         
        if(!(lastchild->parent))
        {
            /* if parent is null there can't be siblings */
            return NULL;
        }
        while( !(lastchild->next_sibling) && !(lastchild->parent->done))
            axis2_om_document_build_next(document); 
    }
    return lastchild->next_sibling;
}

axis2_om_node_t *axis2_om_document_get_first_child(axis2_om_document_t *document)
{
    axis2_om_node_t *current_child = NULL;
    current_child = document->builder->lastnode;
    if(current_child)
    {
        while(!(current_child->first_child)&& !(current_child->done))
        {
           axis2_om_document_build_next(document);
        }
       if(current_child->first_child)
        return current_child->first_child;
        else
            return NULL;
        
    }
    
}

axis2_om_node_t *axis2_om_document_get_child(axis2_om_document_t *document)
{
    return document->builder->lastnode;
}
