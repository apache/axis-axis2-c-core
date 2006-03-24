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
 
 #include <axis2_xml_reader.h>
 #include <libxml/xmlreader.h>
 #include <axis2_defines.h>
 #include <axis2.h>
 #include <string.h>
 
/*************************** Function Prototypes ******************************/

#define AXIS2_ATTR_NS_MAX   20

 
int AXIS2_CALL
axis2_libxml2_reader_wrapper_next(axis2_xml_reader_t *parser,
                               axis2_env_t **env);
                 
axis2_status_t AXIS2_CALL 
axis2_libxml2_reader_wrapper_free(axis2_xml_reader_t *parser,
                           axis2_env_t **env);
           
int AXIS2_CALL
axis2_libxml2_reader_wrapper_get_attribute_count(axis2_xml_reader_t *parser,
                                          axis2_env_t **env);
        
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_attribute_name_by_number(axis2_xml_reader_t *parser,
                                                   axis2_env_t **env,
                                                   int i);
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_attribute_prefix_by_number(axis2_xml_reader_t *parser,
                                                     axis2_env_t **env,
                                                     int i);
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_attribute_value_by_number(axis2_xml_reader_t *parser,
                                                     axis2_env_t **env,
                                                     int i);
        
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_attribute_namespace_by_number
                                                    (axis2_xml_reader_t *parser,
                                                     axis2_env_t **env,
                                                     int i);

axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_value(axis2_xml_reader_t *parser,
                                axis2_env_t **env);
                                                      
int AXIS2_CALL 
axis2_libxml2_reader_wrapper_get_namespace_count(axis2_xml_reader_t *parser,
                                          axis2_env_t **env);
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_namespace_uri_by_number(axis2_xml_reader_t *parser,
                                                  axis2_env_t **env,
                                                  int i);
                                               
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_namespace_prefix_by_number(axis2_xml_reader_t *parser,
                                                	 axis2_env_t **env,
                                                	 int i); 
                                                
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_prefix(axis2_xml_reader_t *parser,
                                 axis2_env_t **env);   
        
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_name(axis2_xml_reader_t *parser,
                               axis2_env_t **env); 

axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_pi_target(axis2_xml_reader_t *parser,
                                    axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_pi_data(axis2_xml_reader_t *parser,
                                  axis2_env_t **env);
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_dtd(axis2_xml_reader_t *parser,
                                  axis2_env_t **env);                                  
                                  
axis2_status_t AXIS2_CALL
axis2_libxml2_reader_wrapper_xml_free(axis2_xml_reader_t *parser,
                               axis2_env_t **env,
                               void *data);
                               
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_get_char_set_encoding(axis2_xml_reader_t *parser,
                                           axis2_env_t **env);                               

axis2_status_t axis2_libxml2_reader_wrapper_fill_maps(axis2_xml_reader_t *parser,
                                            axis2_env_t **env);
                                            
void 
axis2_libxml2_reader_wrapper_error_handler(void *arg, 
                                           const char *msg, 
                                           int severities, 
                                           void *locator_ptr);                                            
                                            
static int 
axis2_libxml2_reader_wrapper_read_input_callback(void *ctx,
                                                 char *buffer,
                                                 int size);                                           
                                                 
static int 
axis2_libxml2_reader_wrapper_close_input_callback(void *ctx);
                                   
/************* End function parameters , axis2_libxml2_reader_wrapper_impl_t struct ***/

typedef struct axis2_libxml2_reader_wrapper_impl_t
{
	axis2_xml_reader_t parser;
	
	xmlTextReaderPtr reader;
	
	int current_event;
	int current_attribute_count;
    int current_namespace_count;
	int event_map[18];
	
	void* ctx;
    /* assuming that max ns and attri will be 20 */
    
    int namespace_map[AXIS2_ATTR_NS_MAX];
    int attribute_map[AXIS2_ATTR_NS_MAX];
    /******************************************/
    /* read callback function */
    int (*read_input_callback)(char *buffer, int size,void* ctx);

    int (*close_input_callback)(void *ctx);
    
} axis2_libxml2_reader_wrapper_impl_t;

/****************** End struct , Macro *****************************************/

#define AXIS2_INTF_TO_IMPL(p) ((axis2_libxml2_reader_wrapper_impl_t*)p)
#define AXIS2_IMPL_TO_INTF(p) &(p->parser)


/********************* End Macro  , event_map init function **********************/

static axis2_status_t 
axis2_libxml2_reader_wrapper_init_map(axis2_libxml2_reader_wrapper_impl_t *parser)
{   
    int i=0;
    if(parser)
    {
        for(i=0;i<18;i++)
           parser->event_map[i] = -1;
        
        parser->event_map[XML_READER_TYPE_ELEMENT] = 
                    AXIS2_XML_READER_START_ELEMENT;
        
        parser->event_map[XML_READER_TYPE_ELEMENT] = 
                    AXIS2_XML_READER_START_ELEMENT;
        
        parser->event_map[XML_READER_TYPE_DOCUMENT] = 
                    AXIS2_XML_READER_START_DOCUMENT;
        
        parser->event_map[XML_READER_TYPE_TEXT] = AXIS2_XML_READER_CHARACTER;
        
        parser->event_map[XML_READER_TYPE_CDATA] = AXIS2_XML_READER_CDATA;
        
        parser->event_map[XML_READER_TYPE_SIGNIFICANT_WHITESPACE] = 
                    AXIS2_XML_READER_SPACE;
        
        parser->event_map[XML_READER_TYPE_WHITESPACE] = AXIS2_XML_READER_SPACE;
        
        parser->event_map[XML_READER_TYPE_END_ELEMENT] = 
                    AXIS2_XML_READER_END_ELEMENT;
        
        parser->event_map [XML_READER_TYPE_ENTITY_REFERENCE] = 
                    AXIS2_XML_READER_ENTITY_REFERENCE;
        
        parser->event_map [XML_READER_TYPE_END_ENTITY] = AXIS2_XML_READER_SPACE;
        
        parser->event_map[XML_READER_TYPE_ENTITY] = AXIS2_XML_READER_SPACE;
            
        parser->event_map[XML_READER_TYPE_PROCESSING_INSTRUCTION] =
                    AXIS2_XML_READER_PROCESSING_INSTRUCTION;
                    
        parser->event_map[XML_READER_TYPE_COMMENT] = AXIS2_XML_READER_COMMENT;
        
        parser->event_map[XML_READER_TYPE_DOCUMENT_TYPE] =
                    AXIS2_XML_READER_DOCUMENT_TYPE;
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}




/*******************************************************************************/
AXIS2_DECLARE(axis2_xml_reader_t *)
axis2_xml_reader_create_for_file(axis2_env_t **env,
                                  char *filename,
                                  const axis2_char_t *encoding)
{
    
	axis2_libxml2_reader_wrapper_impl_t *wrapper_impl = NULL;
    AXIS2_ENV_CHECK( env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, filename, NULL);
    
    wrapper_impl = (axis2_libxml2_reader_wrapper_impl_t*)AXIS2_MALLOC((*env)->allocator,
         sizeof(axis2_libxml2_reader_wrapper_impl_t));
    if(!wrapper_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    wrapper_impl->reader =  xmlNewTextReaderFilename(filename);
    if(!(wrapper_impl->reader))
    {
        AXIS2_FREE((*env)->allocator, wrapper_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CREATING_XML_STREAM_READER, AXIS2_FAILURE);
        return NULL;
    }

    xmlTextReaderSetErrorHandler(wrapper_impl->reader, 
        (xmlTextReaderErrorFunc)axis2_libxml2_reader_wrapper_error_handler, 
        (*env));
    wrapper_impl->current_event = -1;
    
    axis2_libxml2_reader_wrapper_init_map(wrapper_impl);
	
    
    wrapper_impl->parser.ops = NULL;
    wrapper_impl->parser.ops = (axis2_xml_reader_ops_t*)AXIS2_MALLOC((*env)->allocator,
                                sizeof(axis2_xml_reader_ops_t));
    
    if(!(wrapper_impl->parser.ops))
    {
        xmlFreeTextReader(wrapper_impl->reader);
        AXIS2_FREE((*env)->allocator, wrapper_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
	/* ops */
	
	wrapper_impl->parser.ops->free = axis2_libxml2_reader_wrapper_free;
    wrapper_impl->parser.ops->next = axis2_libxml2_reader_wrapper_next;
    wrapper_impl->parser.ops->xml_free = axis2_libxml2_reader_wrapper_xml_free;
    
    wrapper_impl->parser.ops->get_attribute_count =
        axis2_libxml2_reader_wrapper_get_attribute_count;
    wrapper_impl->parser.ops->get_attribute_name_by_number =
        axis2_libxml2_reader_wrapper_get_attribute_name_by_number;
    wrapper_impl->parser.ops->get_attribute_value_by_number = 
        axis2_libxml2_reader_wrapper_get_attribute_value_by_number;
    wrapper_impl->parser.ops->get_attribute_namespace_by_number =
        axis2_libxml2_reader_wrapper_get_attribute_namespace_by_number;
    wrapper_impl->parser.ops->get_attribute_prefix_by_number =
        axis2_libxml2_reader_wrapper_get_attribute_prefix_by_number;
    
    wrapper_impl->parser.ops->get_value =
        axis2_libxml2_reader_wrapper_get_value;
    wrapper_impl->parser.ops->get_prefix =
        axis2_libxml2_reader_wrapper_get_prefix;
    wrapper_impl->parser.ops->get_name =
        axis2_libxml2_reader_wrapper_get_name;
        
        
    wrapper_impl->parser.ops->get_namespace_count =
        axis2_libxml2_reader_wrapper_get_namespace_count;
    wrapper_impl->parser.ops->get_namespace_prefix_by_number =
        axis2_libxml2_reader_wrapper_get_namespace_prefix_by_number;
    wrapper_impl->parser.ops->get_namespace_uri_by_number =
        axis2_libxml2_reader_wrapper_get_namespace_uri_by_number;
        
    wrapper_impl->parser.ops->get_pi_target =
        axis2_libxml2_reader_wrapper_get_pi_target;
    wrapper_impl->parser.ops->get_pi_data =
        axis2_libxml2_reader_wrapper_get_pi_data;
        
    wrapper_impl->parser.ops->get_dtd =
        axis2_libxml2_reader_wrapper_get_dtd;
    wrapper_impl->parser.ops->get_char_set_encoding =
        axis2_libxml2_reader_get_char_set_encoding;            
	return &(wrapper_impl->parser);
}

/************** create function for io callback function **********************/

AXIS2_DECLARE(axis2_xml_reader_t *)
axis2_xml_reader_create_for_memory(axis2_env_t **env,
                                    int (*read_input_callback)(char *buffer,int size,void *ctx),
                                    int (*close_input_callback)(void *ctx),
                                    void* ctx,
                                    const axis2_char_t *encoding)
{
    
	axis2_libxml2_reader_wrapper_impl_t *wrapper_impl = NULL;
    
    AXIS2_ENV_CHECK( env, NULL);
    
    if(!read_input_callback)
        return NULL;
        
    wrapper_impl = (axis2_libxml2_reader_wrapper_impl_t*)AXIS2_MALLOC((*env)->allocator,
         sizeof(axis2_libxml2_reader_wrapper_impl_t));
    if(!wrapper_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;   
    }
    wrapper_impl->close_input_callback = NULL;
    wrapper_impl->read_input_callback = NULL;
    wrapper_impl->read_input_callback = read_input_callback;
    wrapper_impl->close_input_callback = close_input_callback;
    wrapper_impl->ctx = ctx;
    if(wrapper_impl->close_input_callback)
    {
        wrapper_impl->reader =  
			xmlReaderForIO(axis2_libxml2_reader_wrapper_read_input_callback,
             			   axis2_libxml2_reader_wrapper_close_input_callback, 
						   wrapper_impl, NULL, encoding, XML_PARSE_RECOVER);
    }
    else
    {
         wrapper_impl->reader =  xmlReaderForIO(axis2_libxml2_reader_wrapper_read_input_callback,
                             NULL, wrapper_impl, NULL, encoding, XML_PARSE_RECOVER);
    }
    if(!(wrapper_impl->reader))
    {
        AXIS2_FREE((*env)->allocator, wrapper_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CREATING_XML_STREAM_READER, AXIS2_FAILURE);
        return NULL;
    }

    xmlTextReaderSetErrorHandler(wrapper_impl->reader, 
         (xmlTextReaderErrorFunc)axis2_libxml2_reader_wrapper_error_handler, 
         (*env));

    wrapper_impl->current_event = -1;
    
    axis2_libxml2_reader_wrapper_init_map(wrapper_impl);
	
    
    wrapper_impl->parser.ops = NULL;
    wrapper_impl->parser.ops = (axis2_xml_reader_ops_t*)AXIS2_MALLOC((*env)->allocator,
                                sizeof(axis2_xml_reader_ops_t));
    
    if(!(wrapper_impl->parser.ops))
    {
        xmlFreeTextReader(wrapper_impl->reader);
        AXIS2_FREE((*env)->allocator, wrapper_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

	
	wrapper_impl->parser.ops->free = axis2_libxml2_reader_wrapper_free;
    wrapper_impl->parser.ops->next = axis2_libxml2_reader_wrapper_next;
    wrapper_impl->parser.ops->xml_free = axis2_libxml2_reader_wrapper_xml_free;
    
    wrapper_impl->parser.ops->get_attribute_count =
        axis2_libxml2_reader_wrapper_get_attribute_count;
    wrapper_impl->parser.ops->get_attribute_name_by_number =
        axis2_libxml2_reader_wrapper_get_attribute_name_by_number;
    wrapper_impl->parser.ops->get_attribute_value_by_number = 
        axis2_libxml2_reader_wrapper_get_attribute_value_by_number;
    wrapper_impl->parser.ops->get_attribute_namespace_by_number =
        axis2_libxml2_reader_wrapper_get_attribute_namespace_by_number;
    wrapper_impl->parser.ops->get_attribute_prefix_by_number =
        axis2_libxml2_reader_wrapper_get_attribute_prefix_by_number;
    
    wrapper_impl->parser.ops->get_value =
        axis2_libxml2_reader_wrapper_get_value;
    wrapper_impl->parser.ops->get_prefix =
        axis2_libxml2_reader_wrapper_get_prefix;
    wrapper_impl->parser.ops->get_name =
        axis2_libxml2_reader_wrapper_get_name;
        
        
    wrapper_impl->parser.ops->get_namespace_count =
        axis2_libxml2_reader_wrapper_get_namespace_count;
    wrapper_impl->parser.ops->get_namespace_prefix_by_number =
        axis2_libxml2_reader_wrapper_get_namespace_prefix_by_number;
    wrapper_impl->parser.ops->get_namespace_uri_by_number =
        axis2_libxml2_reader_wrapper_get_namespace_uri_by_number;
        
    wrapper_impl->parser.ops->get_pi_target =
        axis2_libxml2_reader_wrapper_get_pi_target;
    wrapper_impl->parser.ops->get_pi_data =
        axis2_libxml2_reader_wrapper_get_pi_data;
        
    wrapper_impl->parser.ops->get_dtd =
        axis2_libxml2_reader_wrapper_get_dtd;
        
    wrapper_impl->parser.ops->get_char_set_encoding =
        axis2_libxml2_reader_get_char_set_encoding;            
	return &(wrapper_impl->parser);
}

/***************** create function for character buffer ************************/
AXIS2_DECLARE(axis2_xml_reader_t *)
axis2_xml_reader_create_for_buffer(axis2_env_t **env,
                                  const axis2_char_t *buffer,
                                  int size,
                                  const axis2_char_t *encoding)
{    
	axis2_libxml2_reader_wrapper_impl_t *wrapper_impl = NULL;
    
    AXIS2_ENV_CHECK( env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, buffer , AXIS2_FAILURE);
        
    wrapper_impl = (axis2_libxml2_reader_wrapper_impl_t*)AXIS2_MALLOC((*env)->allocator,
         sizeof(axis2_libxml2_reader_wrapper_impl_t));
    if(!wrapper_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;   
    }
    wrapper_impl->close_input_callback = NULL;
    wrapper_impl->read_input_callback = NULL;
	wrapper_impl->ctx = NULL;
	
    wrapper_impl->reader =  xmlReaderForMemory(buffer, size, 
								NULL, encoding, XML_PARSE_RECOVER);

    if(!(wrapper_impl->reader))
    {
        AXIS2_FREE((*env)->allocator, wrapper_impl);
        AXIS2_ERROR_SET((*env)->error, 
				AXIS2_ERROR_CREATING_XML_STREAM_READER, AXIS2_FAILURE);
        return NULL;
    }

    xmlTextReaderSetErrorHandler(wrapper_impl->reader, 
         (xmlTextReaderErrorFunc)axis2_libxml2_reader_wrapper_error_handler, 
         (*env));

    wrapper_impl->current_event = -1;
    
    axis2_libxml2_reader_wrapper_init_map(wrapper_impl);
    
    wrapper_impl->parser.ops = NULL;
    wrapper_impl->parser.ops = (axis2_xml_reader_ops_t*)AXIS2_MALLOC((*env)->allocator,
                                sizeof(axis2_xml_reader_ops_t));
    
    if(!(wrapper_impl->parser.ops))
    {
        xmlFreeTextReader(wrapper_impl->reader);
        AXIS2_FREE((*env)->allocator, wrapper_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

	
	wrapper_impl->parser.ops->free = axis2_libxml2_reader_wrapper_free;
    wrapper_impl->parser.ops->next = axis2_libxml2_reader_wrapper_next;
    wrapper_impl->parser.ops->xml_free = axis2_libxml2_reader_wrapper_xml_free;
    
    wrapper_impl->parser.ops->get_attribute_count =
        axis2_libxml2_reader_wrapper_get_attribute_count;
    wrapper_impl->parser.ops->get_attribute_name_by_number =
        axis2_libxml2_reader_wrapper_get_attribute_name_by_number;
    wrapper_impl->parser.ops->get_attribute_value_by_number = 
        axis2_libxml2_reader_wrapper_get_attribute_value_by_number;
    wrapper_impl->parser.ops->get_attribute_namespace_by_number =
        axis2_libxml2_reader_wrapper_get_attribute_namespace_by_number;
    wrapper_impl->parser.ops->get_attribute_prefix_by_number =
        axis2_libxml2_reader_wrapper_get_attribute_prefix_by_number;
    
    wrapper_impl->parser.ops->get_value =
        axis2_libxml2_reader_wrapper_get_value;
    wrapper_impl->parser.ops->get_prefix =
        axis2_libxml2_reader_wrapper_get_prefix;
    wrapper_impl->parser.ops->get_name =
        axis2_libxml2_reader_wrapper_get_name;
        
        
    wrapper_impl->parser.ops->get_namespace_count =
        axis2_libxml2_reader_wrapper_get_namespace_count;
    wrapper_impl->parser.ops->get_namespace_prefix_by_number =
        axis2_libxml2_reader_wrapper_get_namespace_prefix_by_number;
    wrapper_impl->parser.ops->get_namespace_uri_by_number =
        axis2_libxml2_reader_wrapper_get_namespace_uri_by_number;
        
    wrapper_impl->parser.ops->get_pi_target =
        axis2_libxml2_reader_wrapper_get_pi_target;
    wrapper_impl->parser.ops->get_pi_data =
        axis2_libxml2_reader_wrapper_get_pi_data;
        
    wrapper_impl->parser.ops->get_dtd =
        axis2_libxml2_reader_wrapper_get_dtd;
        
    wrapper_impl->parser.ops->get_char_set_encoding =
        axis2_libxml2_reader_get_char_set_encoding;            
	return &(wrapper_impl->parser);
}
/****************** end create functions ***************************************/

int AXIS2_CALL
axis2_libxml2_reader_wrapper_next(axis2_xml_reader_t *parser,
                               axis2_env_t **env)
{
    int ret_val = 0;
    int node = 2;
    int empty_check = 0;
    axis2_libxml2_reader_wrapper_impl_t *parser_impl; 
    AXIS2_ENV_CHECK(env, -1);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    /* if end_element event after empty element event is not required remove
        this if close 
    if(parser_impl->current_event == AXIS2_XML_READER_EMPTY_ELEMENT)
    {
         if the previous event was a empty element then create an end element
            event 
        parser_impl->current_event = AXIS2_XML_READER_END_ELEMENT;
        return AXIS2_XML_READER_END_ELEMENT;        
    }
    */
    ret_val = xmlTextReaderRead(parser_impl->reader);
    if(ret_val == 0)
    {
        AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "xml stream is over ");
    }
    if(ret_val == -1)
    {
        AXIS2_LOG_ERROR((*env)->log, AXIS2_LOG_SI, " error occured in reading xml stream ");
        return -1;
    }
   
    if(ret_val == 1)
    {
        node = xmlTextReaderNodeType(parser_impl->reader);
        parser_impl->current_event = parser_impl->event_map[node];
        parser_impl->current_attribute_count=0;
        parser_impl->current_namespace_count=0;
        
        if(node == XML_READER_TYPE_ELEMENT)
        {
            empty_check = xmlTextReaderIsEmptyElement(parser_impl->reader);
            axis2_libxml2_reader_wrapper_fill_maps(parser , env);
        }
        if(empty_check == 1)
        {   
            parser_impl->current_event =  AXIS2_XML_READER_EMPTY_ELEMENT;
            return AXIS2_XML_READER_EMPTY_ELEMENT;         
        }
        return parser_impl->event_map[node];
    }
    else
        return -1;
}
                 
axis2_status_t AXIS2_CALL 
axis2_libxml2_reader_wrapper_free(axis2_xml_reader_t *parser,
                           axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(AXIS2_INTF_TO_IMPL(parser)->reader)
    {
        xmlTextReaderClose(AXIS2_INTF_TO_IMPL(parser)->reader);
        xmlFreeTextReader(AXIS2_INTF_TO_IMPL(parser)->reader);
        xmlCleanupParser();
    }
    if(parser->ops)
    {
        AXIS2_FREE((*env)->allocator, parser->ops);
        parser->ops = NULL;
    }
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(parser));
    return AXIS2_SUCCESS;   
}
           
int AXIS2_CALL
axis2_libxml2_reader_wrapper_get_attribute_count(axis2_xml_reader_t *parser,
                                          axis2_env_t **env)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl=NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    if(parser_impl->current_event == AXIS2_XML_READER_START_ELEMENT ||
       parser_impl->current_event == AXIS2_XML_READER_EMPTY_ELEMENT )
        return parser_impl->current_attribute_count;
    else
        return 0;
}
        
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_attribute_name_by_number(axis2_xml_reader_t *parser,
                                                   axis2_env_t **env,
                                                   int i)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl;
    AXIS2_ENV_CHECK( env, NULL);
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
axis2_libxml2_reader_wrapper_get_attribute_prefix_by_number(axis2_xml_reader_t *parser,
                                                     axis2_env_t **env,
                                                     int i)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl = NULL;
    AXIS2_ENV_CHECK( env, NULL);
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
axis2_libxml2_reader_wrapper_get_attribute_value_by_number(axis2_xml_reader_t *parser,
                                                     axis2_env_t **env,
                                                     int i)
{
    
    axis2_libxml2_reader_wrapper_impl_t *parser_impl;
    AXIS2_ENV_CHECK( env, NULL);
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
axis2_libxml2_reader_wrapper_get_attribute_namespace_by_number
                                                    (axis2_xml_reader_t *parser,
                                                     axis2_env_t **env,
                                                     int i)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl;
    AXIS2_ENV_CHECK( env, NULL);
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
axis2_libxml2_reader_wrapper_get_value(axis2_xml_reader_t *parser,
                                axis2_env_t **env)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl = NULL;

    AXIS2_ENV_CHECK( env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    return (axis2_char_t*)xmlTextReaderValue(parser_impl->reader);
   
}
                                                      
int AXIS2_CALL 
axis2_libxml2_reader_wrapper_get_namespace_count(axis2_xml_reader_t *parser,
                                          axis2_env_t **env)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    if(parser_impl->current_event == AXIS2_XML_READER_START_ELEMENT ||
       parser_impl->current_event == AXIS2_XML_READER_EMPTY_ELEMENT)
        return parser_impl->current_namespace_count;
    else
        return 0;
}


axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_namespace_uri_by_number(axis2_xml_reader_t *parser,
                                                  axis2_env_t **env,
                                                  int i)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl = NULL;
    AXIS2_ENV_CHECK( env, NULL);
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
axis2_libxml2_reader_wrapper_get_namespace_prefix_by_number(axis2_xml_reader_t *parser,
                                                	 axis2_env_t **env,
                                                	 int i)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl = NULL;
    AXIS2_ENV_CHECK( env, NULL);
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
axis2_libxml2_reader_wrapper_get_prefix(axis2_xml_reader_t *parser,
                                 axis2_env_t **env)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl = NULL;
    AXIS2_ENV_CHECK( env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    xmlTextReaderMoveToElement(parser_impl->reader);
    return (axis2_char_t*)xmlTextReaderPrefix(parser_impl->reader);
}
        
axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_name(axis2_xml_reader_t *parser,
                               axis2_env_t **env)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl = NULL;
    AXIS2_ENV_CHECK( env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    xmlTextReaderMoveToElement(parser_impl->reader);
    return (axis2_char_t*)xmlTextReaderLocalName(parser_impl->reader);
    
}

axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_pi_target(axis2_xml_reader_t *parser,
                                    axis2_env_t **env)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl = NULL;
    AXIS2_ENV_CHECK( env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    if(parser_impl->current_event ==  AXIS2_XML_READER_PROCESSING_INSTRUCTION)
        return (axis2_char_t*)xmlTextReaderLocalName(parser_impl->reader);
    else
        return NULL;        
}

axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_dtd(axis2_xml_reader_t *parser,
                                  axis2_env_t **env)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl = NULL;
    AXIS2_ENV_CHECK( env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    if(parser_impl->current_event ==  AXIS2_XML_READER_DOCUMENT_TYPE)
        return (axis2_char_t*)xmlTextReaderLocalName(parser_impl->reader);
    else
        return NULL;        
}


axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_wrapper_get_pi_data(axis2_xml_reader_t *parser,
                                  axis2_env_t **env)
{
    axis2_libxml2_reader_wrapper_impl_t *parser_impl = NULL;
    AXIS2_ENV_CHECK( env, NULL);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    if(parser_impl->current_event ==  AXIS2_XML_READER_PROCESSING_INSTRUCTION)
         return (axis2_char_t*)xmlTextReaderValue(parser_impl->reader);
    else
    {
        return NULL;
    }    
}

axis2_status_t AXIS2_CALL
axis2_libxml2_reader_wrapper_xml_free(axis2_xml_reader_t *parser,
                               axis2_env_t **env,
                               void *data)
{
    AXIS2_ENV_CHECK( env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, data, AXIS2_FAILURE);
    xmlFree(data);
    return AXIS2_SUCCESS;
 }

axis2_char_t* AXIS2_CALL
axis2_libxml2_reader_get_char_set_encoding(axis2_xml_reader_t *parser,
                                           axis2_env_t **env)
{
    axis2_libxml2_reader_wrapper_impl_t *reader_impl = NULL;
    AXIS2_ENV_CHECK( env, NULL);
    reader_impl = AXIS2_INTF_TO_IMPL(parser);
    return xmlTextReaderConstEncoding(reader_impl->reader);
}                                           

axis2_status_t axis2_libxml2_reader_wrapper_fill_maps(axis2_xml_reader_t *parser,
                                            axis2_env_t **env)
{
    int libxml2_attribute_count = 0;
    int attr_count = 0;
    int ns_count = 0;
    int i=0;
    char *q_name = NULL;
    axis2_libxml2_reader_wrapper_impl_t *parser_impl = NULL;
    
    AXIS2_ENV_CHECK( env, AXIS2_FAILURE);
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    
    libxml2_attribute_count = xmlTextReaderAttributeCount(parser_impl->reader);
    if(libxml2_attribute_count == 0)
        return AXIS2_SUCCESS;
    
    for(i = 0;i < AXIS2_ATTR_NS_MAX ;i++)
    {
        parser_impl->namespace_map[i] = -1;
        parser_impl->attribute_map[i] = -1;        
    }
    
    for(i = 0; i < libxml2_attribute_count ; i++)
    {
        xmlTextReaderMoveToAttributeNo(parser_impl->reader,i);
        q_name = (char *) xmlTextReaderName(parser_impl->reader);
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
    return AXIS2_SUCCESS;
}


static int axis2_libxml2_reader_wrapper_read_input_callback(void *ctx,char *buffer,int size)
{
 return  ((axis2_libxml2_reader_wrapper_impl_t*)ctx)->read_input_callback(
        buffer, size,((axis2_libxml2_reader_wrapper_impl_t*)ctx)->ctx);
}

void
axis2_libxml2_reader_wrapper_error_handler(void *arg,
                                           const char *msg,
                                           int severities,
                                           void *locator_ptr)
{
    axis2_env_t *env = NULL;
    env = (axis2_env_t*)arg;
           
    switch(severities)
    {
        case XML_PARSER_SEVERITY_VALIDITY_WARNING :
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "%s VALIDITY WARNTING", msg);
        }
            break;
        case XML_PARSER_SEVERITY_VALIDITY_ERROR:
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "%s -- VALIDITY ERROR", msg);
        }
            break;
        case XML_PARSER_SEVERITY_WARNING:
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "%s -- VALIDITY ERROR", msg);
        }
            break;
        case XML_PARSER_SEVERITY_ERROR:
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "%s -- SEVERITY_ERROR", msg);
        }
            break;
        default:
            break;
     }
}

static int
axis2_libxml2_reader_wrapper_close_input_callback(void *ctx)
{
    return ((axis2_libxml2_reader_wrapper_impl_t*)ctx)->close_input_callback(((axis2_libxml2_reader_wrapper_impl_t*)ctx)->ctx);
}
