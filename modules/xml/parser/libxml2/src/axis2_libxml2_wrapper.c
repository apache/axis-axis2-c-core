/*
 *   Copyright 2004,2005 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */
 
 #include <axis2_pull_parser.h>
 #include <libxml/xmlreader.h>
 #include <axis2_defines.h>
 #include <axis2.h>
 #include <string.h>
 
/*************************** Function Prototypes ******************************/

#define AXIS2_ATTR_NS_MAX   20

 
int AXIS2_CALL
axis2_libxml2_wrapper_next(axis2_pull_parser_t *parser,
                               axis2_env_t **env);
                 
axis2_status_t AXIS2_CALL 
axis2_libxml2_wrapper_free(axis2_pull_parser_t *parser,
                           axis2_env_t **env);
           
int AXIS2_CALL
axis2_libxml2_wrapper_get_attribute_count(axis2_pull_parser_t *parser,
                                          axis2_env_t **env);
        
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_attribute_name_by_number(axis2_pull_parser_t *parser,
                                                   axis2_env_t **env,
                                                   int i);
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_attribute_prefix_by_number(axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i);
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_attribute_value_by_number(axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i);
        
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_attribute_namespace_by_number
                                                    (axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i);

axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_value(axis2_pull_parser_t *parser,
                                axis2_env_t **env);
                                                      
int AXIS2_CALL 
axis2_libxml2_wrapper_get_namespace_count(axis2_pull_parser_t *parser,
                                          axis2_env_t **env);
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_namespace_uri_by_number(axis2_pull_parser_t *parser,
                                                  axis2_env_t **env,
                                                  int i);
                                               
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_namespace_prefix_by_number(axis2_pull_parser_t *parser,
                                                	 axis2_env_t **env,
                                                	 int i); 
                                                
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_prefix(axis2_pull_parser_t *parser,
                                 axis2_env_t **env);   
        
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_name(axis2_pull_parser_t *parser,
                               axis2_env_t **env); 

axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_pi_target(axis2_pull_parser_t *parser,
                                    axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_pi_data(axis2_pull_parser_t *parser,
                                  axis2_env_t **env);
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_dtd(axis2_pull_parser_t *parser,
                                  axis2_env_t **env);                                  
                                  
axis2_status_t AXIS2_CALL
axis2_libxml2_wrapper_xml_free(axis2_pull_parser_t *parser,
                               axis2_env_t **env,
                               void *data);

void axis2_libxml2_wrapper_fill_maps(axis2_pull_parser_t *parser,
                                            axis2_env_t **env);
                                   
/************* End function parameters , axis2_libxml2_wrapper_impl_t struct ***/

typedef struct axis2_libxml2_wrapper_impl_t
{
	axis2_pull_parser_t parser;
	
	xmlTextReaderPtr reader;
	int current_event;
	int current_attribute_count;
    int current_namespace_count;
	int event_map[18];
    /* assuming that max ns and attri will be 20 */
    
    int namespace_map[AXIS2_ATTR_NS_MAX];
    int attribute_map[AXIS2_ATTR_NS_MAX];
}axis2_libxml2_wrapper_impl_t;

/****************** End struct , Macro *****************************************/

#define AXIS2_INTF_TO_IMPL(p) ((axis2_libxml2_wrapper_impl_t*)p)
#define AXIS2_IMPL_TO_INTF(p) &(p->parser)


/********************* End Macro  , event_map init function **********************/

static axis2_status_t axis2_libxml2_wrapper_init_map(
                                      axis2_libxml2_wrapper_impl_t *parser)
{   
    int i=0;
    if(parser)
    {
        for(i=0;i<18;i++)
           parser->event_map[i] = -1;
        
        parser->event_map[XML_READER_TYPE_ELEMENT] = 
                    AXIS2_PULL_PARSER_START_ELEMENT;
        
        parser->event_map[XML_READER_TYPE_ELEMENT] = 
                    AXIS2_PULL_PARSER_START_ELEMENT;
        
        parser->event_map[XML_READER_TYPE_DOCUMENT] = 
                    AXIS2_PULL_PARSER_START_DOCUMENT;
        
        parser->event_map[XML_READER_TYPE_TEXT] = AXIS2_PULL_PARSER_CHARACTER;
        
        parser->event_map[XML_READER_TYPE_CDATA] = AXIS2_PULL_PARSER_CDATA;
        
        parser->event_map[XML_READER_TYPE_SIGNIFICANT_WHITESPACE] = 
                    AXIS2_PULL_PARSER_SPACE;
        
        parser->event_map[XML_READER_TYPE_WHITESPACE] = AXIS2_PULL_PARSER_SPACE;
        
        parser->event_map[XML_READER_TYPE_END_ELEMENT] = 
                    AXIS2_PULL_PARSER_END_ELEMENT;
        
        parser->event_map [XML_READER_TYPE_ENTITY_REFERENCE] = 
                    AXIS2_PULL_PARSER_ENTITY_REFERANCE;
        
        parser->event_map [XML_READER_TYPE_END_ENTITY] = AXIS2_PULL_PARSER_SPACE;
        
        parser->event_map[XML_READER_TYPE_ENTITY] = AXIS2_PULL_PARSER_SPACE;
            
        parser->event_map[XML_READER_TYPE_PROCESSING_INSTRUCTION] =
                    AXIS2_PULL_PARSER_PROCESSING_INSTRUCTION;
                    
        parser->event_map[XML_READER_TYPE_COMMENT] = AXIS2_PULL_PARSER_COMMENT;
        
        parser->event_map[XML_READER_TYPE_DOCUMENT_TYPE] =
                    AXIS2_PULL_PARSER_DOCUMENT_TYPE;
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}




/*******************************************************************************/
AXIS2_DECLARE(axis2_pull_parser_t *)
axis2_pull_parser_create_for_file(axis2_env_t **env,char *filename)
{
    
	axis2_libxml2_wrapper_impl_t *wrapper_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(filename, env, NULL);
    
    wrapper_impl = (axis2_libxml2_wrapper_impl_t*)AXIS2_MALLOC((*env)->allocator,
         sizeof(axis2_libxml2_wrapper_impl_t));
    if(!wrapper_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    wrapper_impl->reader =  xmlNewTextReaderFilename(filename);
    if(!(wrapper_impl->reader))
    {
        AXIS2_FREE((*env)->allocator, wrapper_impl);
      /*  AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CREATING_LIBXML_READER, NULL);
        */
    }
	
    
    wrapper_impl->current_event = -1;
    
    axis2_libxml2_wrapper_init_map(wrapper_impl);
	
    
    wrapper_impl->parser.ops = NULL;
    wrapper_impl->parser.ops = (axis2_pull_parser_ops_t*)AXIS2_MALLOC((*env)->allocator,
                                sizeof(axis2_pull_parser_ops_t));
    
    if(!(wrapper_impl->parser.ops))
    {
        xmlFreeTextReader(wrapper_impl->reader);
        AXIS2_FREE((*env)->allocator, wrapper_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
	/* operations */
	
	wrapper_impl->parser.ops->free = axis2_libxml2_wrapper_free;
    wrapper_impl->parser.ops->next = axis2_libxml2_wrapper_next;
    wrapper_impl->parser.ops->xml_free = axis2_libxml2_wrapper_xml_free;
    
    wrapper_impl->parser.ops->get_attribute_count =
        axis2_libxml2_wrapper_get_attribute_count;
    wrapper_impl->parser.ops->get_attribute_name_by_number =
        axis2_libxml2_wrapper_get_attribute_name_by_number;
    wrapper_impl->parser.ops->get_attribute_value_by_number = 
        axis2_libxml2_wrapper_get_attribute_value_by_number;
    wrapper_impl->parser.ops->get_attribute_namespace_by_number =
        axis2_libxml2_wrapper_get_attribute_namespace_by_number;
    wrapper_impl->parser.ops->get_attribute_prefix_by_number =
        axis2_libxml2_wrapper_get_attribute_prefix_by_number;
    
    wrapper_impl->parser.ops->get_value =
        axis2_libxml2_wrapper_get_value;
    wrapper_impl->parser.ops->get_prefix =
        axis2_libxml2_wrapper_get_prefix;
    wrapper_impl->parser.ops->get_name =
        axis2_libxml2_wrapper_get_name;
        
        
    wrapper_impl->parser.ops->get_namespace_count =
        axis2_libxml2_wrapper_get_namespace_count;
    wrapper_impl->parser.ops->get_namespace_prefix_by_number =
        axis2_libxml2_wrapper_get_namespace_prefix_by_number;
    wrapper_impl->parser.ops->get_namespace_uri_by_number =
        axis2_libxml2_wrapper_get_namespace_uri_by_number;
        
    wrapper_impl->parser.ops->get_pi_target =
        axis2_libxml2_wrapper_get_pi_target;
    wrapper_impl->parser.ops->get_pi_data =
        axis2_libxml2_wrapper_get_pi_data;
        
    wrapper_impl->parser.ops->get_dtd =
        axis2_libxml2_wrapper_get_dtd;    
	return &(wrapper_impl->parser);
}

/************** create function for io callback function **********************/
/*
AXIS2_DECLARE(axis2_pull_parser_t *)
axis2_pull_parser_create_for_memory(axis2_env_t **env,int )
{
    
	axis2_libxml2_wrapper_impl_t *wrapper_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(filename, env, NULL);
    
    wrapper_impl = (axis2_libxml2_wrapper_impl_t*)AXIS2_MALLOC((*env)->allocator,
         sizeof(axis2_libxml2_wrapper_impl_t));
    if(!wrapper_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    wrapper_impl->reader =  xmlNewTextReaderFilename(filename);
    if(!(wrapper_impl->reader))
    {
        AXIS2_FREE((*env)->allocator, wrapper_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CREATING_LIBXML_READER, NULL);
        
    }
	
    
    wrapper_impl->current_event = -1;
    
    axis2_libxml2_wrapper_init_map(wrapper_impl);
	
    
    wrapper_impl->parser.ops = NULL;
    wrapper_impl->parser.ops = (axis2_pull_parser_ops_t*)AXIS2_MALLOC((*env)->allocator,
                                sizeof(axis2_pull_parser_ops_t));
    
    if(!(wrapper_impl->parser.ops))
    {
        xmlFreeTextReader(wrapper_impl->reader);
        AXIS2_FREE((*env)->allocator, wrapper_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }

	
	wrapper_impl->parser.ops->free = axis2_libxml2_wrapper_free;
    wrapper_impl->parser.ops->next = axis2_libxml2_wrapper_next;
    wrapper_impl->parser.ops->xml_free = axis2_libxml2_wrapper_xml_free;
    
    wrapper_impl->parser.ops->get_attribute_count =
        axis2_libxml2_wrapper_get_attribute_count;
    wrapper_impl->parser.ops->get_attribute_name_by_number =
        axis2_libxml2_wrapper_get_attribute_name_by_number;
    wrapper_impl->parser.ops->get_attribute_value_by_number = 
        axis2_libxml2_wrapper_get_attribute_value_by_number;
    wrapper_impl->parser.ops->get_attribute_namespace_by_number =
        axis2_libxml2_wrapper_get_attribute_namespace_by_number;
    wrapper_impl->parser.ops->get_attribute_prefix_by_number =
        axis2_libxml2_wrapper_get_attribute_prefix_by_number;
    
    wrapper_impl->parser.ops->get_value =
        axis2_libxml2_wrapper_get_value;
    wrapper_impl->parser.ops->get_prefix =
        axis2_libxml2_wrapper_get_prefix;
    wrapper_impl->parser.ops->get_name =
        axis2_libxml2_wrapper_get_name;
        
        
    wrapper_impl->parser.ops->get_namespace_count =
        axis2_libxml2_wrapper_get_namespace_count;
    wrapper_impl->parser.ops->get_namespace_prefix_by_number =
        axis2_libxml2_wrapper_get_namespace_prefix_by_number;
    wrapper_impl->parser.ops->get_namespace_uri_by_number =
        axis2_libxml2_wrapper_get_namespace_uri_by_number;
	return &(wrapper_impl->parser);
}
	
*/	



int AXIS2_CALL
axis2_libxml2_wrapper_next(axis2_pull_parser_t *parser,
                               axis2_env_t **env)
{
    int ret_val = 0;
    int node = 2;
    int empty_check = 0;
    axis2_libxml2_wrapper_impl_t *parser_impl; 
     
    AXIS2_FUNC_PARAM_CHECK(parser,env, -1);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    /* if end_element event after empty element event is not required remove
        this if close */
    if(parser_impl->current_event == AXIS2_PULL_PARSER_EMPTY_ELEMENT)
    {
        /* if the previous event was a empty element then create an end element
            event */
        parser_impl->current_event = AXIS2_PULL_PARSER_END_ELEMENT;
        return AXIS2_PULL_PARSER_END_ELEMENT;        
    }
    
    ret_val = xmlTextReaderRead(parser_impl->reader);
    
    if(ret_val == 1)
    {
        node = xmlTextReaderNodeType(parser_impl->reader);
        parser_impl->current_event = parser_impl->event_map[node];
        parser_impl->current_attribute_count=0;
        parser_impl->current_namespace_count=0;
        
        if(node == XML_READER_TYPE_ELEMENT)
        {
            empty_check = xmlTextReaderIsEmptyElement(parser_impl->reader);
            axis2_libxml2_wrapper_fill_maps(parser , env);
        }
        if(empty_check == 1)
        {
            parser_impl->current_event =  AXIS2_PULL_PARSER_EMPTY_ELEMENT;
            return AXIS2_PULL_PARSER_EMPTY_ELEMENT;         
        }
        return parser_impl->event_map[node];
    }
    else
        return -1;
}
                 
axis2_status_t AXIS2_CALL 
axis2_libxml2_wrapper_free(axis2_pull_parser_t *parser,
                           axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env, AXIS2_FAILURE);
    
    if(AXIS2_INTF_TO_IMPL(parser)->reader)
        xmlFreeTextReader(AXIS2_INTF_TO_IMPL(parser)->reader);
    if(parser->ops)
        AXIS2_FREE((*env)->allocator, parser->ops);
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(parser));
    return AXIS2_SUCCESS;   
}
           
int AXIS2_CALL
axis2_libxml2_wrapper_get_attribute_count(axis2_pull_parser_t *parser,
                                          axis2_env_t **env)
{
    axis2_libxml2_wrapper_impl_t *parser_impl=NULL;
    AXIS2_FUNC_PARAM_CHECK(parser,env, AXIS2_FAILURE);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    if(parser_impl->current_event == AXIS2_PULL_PARSER_START_ELEMENT)
        return parser_impl->current_attribute_count;
    else
        return 0;
}
        
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_attribute_name_by_number(axis2_pull_parser_t *parser,
                                                   axis2_env_t **env,
                                                   int i)
{
    axis2_libxml2_wrapper_impl_t *parser_impl;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    
    if(parser_impl->current_attribute_count >0 && 
        parser_impl->current_attribute_count >= i)
    {
        int ret = xmlTextReaderMoveToAttributeNo(parser_impl->reader,
                                                 parser_impl->attribute_map[i]);
        if(ret == 1)
            return (axis2_char_t*)xmlTextReaderLocalName(parser_impl->reader);
        else
            return NULL;
    }
    return NULL;
}

axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_attribute_prefix_by_number(axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i)
{
    axis2_libxml2_wrapper_impl_t *parser_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    
    if(parser_impl->current_attribute_count >0 && 
        parser_impl->current_attribute_count >= i)
    {
        int ret = xmlTextReaderMoveToAttributeNo(parser_impl->reader,
                                                 parser_impl->attribute_map[i]);
        if(ret == 1)
            return (axis2_char_t*)xmlTextReaderPrefix(parser_impl->reader);
        else
            return NULL;
    }
    return NULL;   
}


axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_attribute_value_by_number(axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i)
{
    
    axis2_libxml2_wrapper_impl_t *parser_impl;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    
    if(parser_impl->current_attribute_count >0 && 
        parser_impl->current_attribute_count >= i)
    {
        int ret = xmlTextReaderMoveToAttributeNo(parser_impl->reader,
                                                 parser_impl->attribute_map[i]);
        if(ret == 1)
            return (axis2_char_t*)xmlTextReaderValue(parser_impl->reader);
        else
            return NULL;
    }
    return NULL; 
}
        
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_attribute_namespace_by_number
                                                    (axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i)
{
    axis2_libxml2_wrapper_impl_t *parser_impl;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    
    if(parser_impl->current_attribute_count >0 && 
        parser_impl->current_attribute_count >= i )
    {
        int ret = xmlTextReaderMoveToAttributeNo(parser_impl->reader,
                                                 parser_impl->attribute_map[i]);
        
        
        if(ret == 1)
            return (axis2_char_t*)xmlTextReaderNamespaceUri(parser_impl->reader);
        else
            return NULL;
    }
    return NULL;     
}

axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_value(axis2_pull_parser_t *parser,
                                axis2_env_t **env)
{
    axis2_libxml2_wrapper_impl_t *parser_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    /*xmlTextReaderMoveToElement(parser_impl->reader); */
    return (axis2_char_t*)xmlTextReaderValue(parser_impl->reader);
}
                                                      
int AXIS2_CALL 
axis2_libxml2_wrapper_get_namespace_count(axis2_pull_parser_t *parser,
                                          axis2_env_t **env)
{
    axis2_libxml2_wrapper_impl_t *parser_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(parser,env, AXIS2_FAILURE);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    if(parser_impl->current_event == AXIS2_PULL_PARSER_START_ELEMENT)
        return parser_impl->current_namespace_count;
    else
        return 0;
}


axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_namespace_uri_by_number(axis2_pull_parser_t *parser,
                                                  axis2_env_t **env,
                                                  int i)
{
    axis2_libxml2_wrapper_impl_t *parser_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    
    if(parser_impl->current_namespace_count >0 && 
        parser_impl->current_namespace_count >= i )
    {
        
        int ret = xmlTextReaderMoveToAttributeNo(parser_impl->reader,
                                                 parser_impl->namespace_map[i]);
        if(ret == 1)
            return (axis2_char_t*)xmlTextReaderValue(parser_impl->reader);
        else
            return NULL;
    }
    return NULL; 
}
                                               
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_namespace_prefix_by_number(axis2_pull_parser_t *parser,
                                                	 axis2_env_t **env,
                                                	 int i)
{
    axis2_libxml2_wrapper_impl_t *parser_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    
    if(parser_impl->current_namespace_count >0 && 
        parser_impl->current_namespace_count >= i ) 
    {   
        int ret = xmlTextReaderMoveToAttributeNo(parser_impl->reader,
                                                 parser_impl->namespace_map[i]);
        
        if(ret == 1)
            return (axis2_char_t*)xmlTextReaderLocalName(parser_impl->reader);
        else
            return NULL;
    } 
    return NULL; 

    
    
}
                                                
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_prefix(axis2_pull_parser_t *parser,
                                 axis2_env_t **env)
{
    axis2_libxml2_wrapper_impl_t *parser_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    xmlTextReaderMoveToElement(parser_impl->reader);
    return (axis2_char_t*)xmlTextReaderPrefix(parser_impl->reader);
}
        
axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_name(axis2_pull_parser_t *parser,
                               axis2_env_t **env)
{
    axis2_libxml2_wrapper_impl_t *parser_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    /*xmlTextReaderMoveToElement(parser_impl->reader);*/
    return (axis2_char_t*)xmlTextReaderLocalName(parser_impl->reader);
    
}

axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_pi_target(axis2_pull_parser_t *parser,
                                    axis2_env_t **env)
{
    axis2_libxml2_wrapper_impl_t *parser_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    if(parser_impl->current_event ==  AXIS2_PULL_PARSER_PROCESSING_INSTRUCTION)
        return (axis2_char_t*)xmlTextReaderLocalName(parser_impl->reader);
    else
        return NULL;        
}

axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_dtd(axis2_pull_parser_t *parser,
                                  axis2_env_t **env)
{
    axis2_libxml2_wrapper_impl_t *parser_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    if(parser_impl->current_event ==  AXIS2_PULL_PARSER_DOCUMENT_TYPE)
        return (axis2_char_t*)xmlTextReaderLocalName(parser_impl->reader);
    else
        return NULL;        
}


axis2_char_t* AXIS2_CALL
axis2_libxml2_wrapper_get_pi_data(axis2_pull_parser_t *parser,
                                  axis2_env_t **env)
{
    axis2_libxml2_wrapper_impl_t *parser_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    if(parser_impl->current_event ==  AXIS2_PULL_PARSER_PROCESSING_INSTRUCTION)
         return (axis2_char_t*)xmlTextReaderValue(parser_impl->reader);
    else
    {
        return NULL;
    }    
}

axis2_status_t AXIS2_CALL
axis2_libxml2_wrapper_xml_free(axis2_pull_parser_t *parser,
                               axis2_env_t **env,
                               void *data)
{
    AXIS2_FUNC_PARAM_CHECK(parser, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, data, AXIS2_FAILURE);
    xmlFree(data);
    return AXIS2_SUCCESS;
 }


void axis2_libxml2_wrapper_fill_maps(axis2_pull_parser_t *parser,
                                            axis2_env_t **env)
{
    int libxml2_attribute_count = 0;
    int attr_count = 0;
    int ns_count = 0;
    int i=0;
    char *q_name = NULL;
    axis2_libxml2_wrapper_impl_t *parser_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(parser, env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    
    libxml2_attribute_count = xmlTextReaderAttributeCount(parser_impl->reader);
    if(libxml2_attribute_count == 0)
        return;
    
    for(i = 0;i < AXIS2_ATTR_NS_MAX ;i++)
    {
        parser_impl->namespace_map[i] = -1;
        parser_impl->attribute_map[i] = -1;        
    }
    
    for(i = 0; i < libxml2_attribute_count ; i++)
    {
        xmlTextReaderMoveToAttributeNo(parser_impl->reader,i);
        q_name = xmlTextReaderName(parser_impl->reader);
        if(q_name != NULL)
        {    
           if((strcmp(q_name, "xmlns") == 0) || (strncmp(q_name, "xmlns:",6) == 0))
           {
                /* found a namespace */
               ns_count++;
               parser_impl->namespace_map[ns_count] = i;
           }
           else
           {
                /* found an attribute */       
               attr_count++;
               parser_impl->attribute_map[attr_count] = i;
           }
           
           xmlFree(q_name);
           q_name = NULL;
        }
        
        parser_impl->current_attribute_count = attr_count;
        parser_impl->current_namespace_count = ns_count;
    }
    return;
}
