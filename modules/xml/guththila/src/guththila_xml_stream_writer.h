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
 
#ifndef GUTHTHILA_XML_STREAM_WRITER_H
#define GUTHTHILA_XML_STREAM_WRITER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "guththila_errno.h"

#define  GUTHTHILA_DEFAULT_NS_PREFIX    ""

           

/**
 * guththila_xml_stream_writer_t - Stream Writer Structure
 */
typedef struct guththila_xml_stream_writer guththila_xml_stream_writer_t;


                   
guththila_xml_stream_writer_t* guththila_create_xml_stream_writer(FILE* writer, char* encoding, int is_prefix_defaulting);
  
  int guththila_xml_stream_writer_end_start_element(guththila_xml_stream_writer_t* stream_writer);
  

  int guththila_xml_stream_writer_write_start_element(guththila_xml_stream_writer_t* stream_writer, char* local_name);
  

  int guththila_xml_stream_writer_write_start_element_with_namespace(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* namespace_uri);
  

  int guththila_xml_stream_writer_write_start_element_with_namespace_prefix(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* namespace_uri, char* prefix);


  int guththila_xml_stream_writer_write_empty_element(guththila_xml_stream_writer_t* stream_writer, char* local_name);


  int guththila_xml_stream_writer_write_empty_element_with_namespace(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* namespace_uri);



  int guththila_xml_stream_writer_write_empty_element_with_namespace_prefix(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* namespace_uri, char* prefix);


  int guththila_xml_stream_writer_write_end_element(guththila_xml_stream_writer_t* stream_writer);
    
  int guththila_xml_stream_writer_write_end_document(guththila_xml_stream_writer_t* stream_writer);


  int guththila_xml_stream_writer_write_attribute(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* value);


  int guththila_xml_stream_writer_write_attribute_with_namespace(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* value, char* namespace_uri);
    

  int guththila_xml_stream_writer_write_attribute_with_namespace_prefix(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* value, char* namespace_uri, char* prefix);
    

  int guththila_xml_stream_writer_write_namespace(guththila_xml_stream_writer_t* stream_writer, char* prefix, char* namespace_uri);


  int guththila_xml_stream_writer_write_default_namespace(guththila_xml_stream_writer_t* stream_writer, char* namespace_uri);


  int guththila_xml_stream_writer_write_comment(guththila_xml_stream_writer_t* stream_writer, char* data);
    
  int guththila_xml_stream_writer_write_processing_instruction(guththila_xml_stream_writer_t* stream_writer, char* target);
    

  int guththila_xml_stream_writer_write_processing_instruction_data(guththila_xml_stream_writer_t* stream_writer, char* target, char* data);


  int guththila_xml_stream_writer_write_cdata(guththila_xml_stream_writer_t* stream_writer, char* data);
    

  int guththila_xml_stream_writer_write_dtd(guththila_xml_stream_writer_t* stream_writer, char* dtd);
    
  int guththila_xml_stream_writer_write_entity_ref(guththila_xml_stream_writer_t* stream_writer, char* name);

  int guththila_xml_stream_writer_write_start_document(guththila_xml_stream_writer_t* stream_writer);

  int guththila_xml_stream_writer_write_start_document_with_version(guththila_xml_stream_writer_t* stream_writer, char* version);


  int guththila_xml_stream_writer_write_start_document_with_version_encoding(guththila_xml_stream_writer_t* stream_writer, char* encoding, char* version);
    

  int guththila_xml_stream_writer_write_characters(guththila_xml_stream_writer_t* stream_writer, char* text);


  char* guththila_xml_stream_writer_get_prefix(guththila_xml_stream_writer_t* stream_writer, char* uri);

  int guththila_xml_stream_writer_set_prefix(guththila_xml_stream_writer_t* stream_writer, char* prefix, char* uri);


  int guththila_xml_stream_writer_set_default_prefix(guththila_xml_stream_writer_t* stream_writer, char* uri);


  int guththila_xml_stream_writer_write_encoded(guththila_xml_stream_writer_t* stream_writer, char* text, int in_attr);


#endif /*GUTHTHILA_XML_STREAM_WRITER_H*/
