
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef GUTHTHILA_XML_WRITER_H
#define GUTHTHILA_XML_WRITER_H

#include <guththila_token.h>
#include <guththila_defines.h>
#include <guththila_buffer.h>
#include <guththila.h>
#include <axutil_utils.h>

EXTERN_C_START()
#define GUTHTHILA_XML_WRITER_TOKEN

/*
Design notes:-
namesp member of guththila_xml_writer_s is populated with malloc created objects.
Providing a list for this seems expensive because most of the times only few
namespaces are present in a XML document.

element member of guththila_xml_writer_s must be povided the list capablity. This
is particularly important in very deep XML documents.
*/
typedef enum guththila_writer_type_s
{
    GUTHTHILA_WRITER_FILE = 1,
    GUTHTHILA_WRITER_MEMORY
} guththila_writer_type_t;

typedef struct guththila_writer_s
{
    short type;
    FILE *out_stream;
    guththila_buffer_t *buffer;
    int next;
}
guththila_writer_t;

typedef enum guththila_writer_status_s
{
    /*Started writing a non empty element */
    START = 1,
    /*Started writing a empty element */
    START_EMPTY,
    /*We are in a position to begin wrting an element */
    BEGINING
} guththila_writer_status_t;

/*Main structure which provides the writer capability*/
typedef struct guththila_xml_writer_s
{
    guththila_stack_t element;
    guththila_stack_t namesp;
    guththila_writer_t *writer;
#ifdef GUTHTHILA_XML_WRITER_TOKEN
    guththila_tok_list_t tok_list;
#endif
    /* Type of this writer. Can be file writer or memory writer */
    guththila_writer_type_t type;

    FILE *out_stream;
    guththila_buffer_t buffer;
    guththila_writer_status_t status;
    int next;
} guththila_xml_writer_t;

/*TODO: we need to came up with common implementation of followng two structures in writer and reader*/

/*
This is a private structure for keeping track of the elements. When we start to write an element this structure will be pop
*/
typedef struct guththila_xml_writer_element_s
{
#ifdef GUTHTHILA_XML_WRITER_TOKEN
    guththila_token_t *prefix;
    guththila_token_t *name;
#else
    guththila_char_t *prefix;
    guththila_char_t *name;
#endif
    /* contains the number of the stack which holds the namespaces
       for this element. When we close this element all the namespaces 
       that are below this should also must be closed */
    int name_sp_stack_no;
}
guththila_xml_writer_element_t;

typedef struct guththila_xml_writer_namesp_s
{
    /* These are double pointers because a single element may contain multple
       namespace declarations */
#ifdef GUTHTHILA_XML_WRITER_TOKEN
    guththila_token_t **name;
    guththila_token_t **uri;
#else
    guththila_char_t **name;
    guththila_char_t **uri;
#endif
    int no;             /*number of namespaces */
    int size;
}
guththila_xml_writer_namesp_t;

#define GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE 4

/*Writer functions*/

/* 
 * Create a writer which writes to a file.
 * @param file_name name of the file
 * @param env pointer to the environment
 */
GUTHTHILA_EXPORT guththila_xml_writer_t *GUTHTHILA_CALL
guththila_create_xml_stream_writer(
    char *file_name,
    const axutil_env_t * env);

/* 
 * Create a writer which writes to a memory buffer.
 * @param env pointer to the environment
 */
GUTHTHILA_EXPORT guththila_xml_writer_t *GUTHTHILA_CALL
guththila_create_xml_stream_writer_for_memory(
    const axutil_env_t * env);

/* 
 * Jus write what ever the content in the buffer. If the writer was in 
 * a start of a element it will close it.
 * @param wr pointer to the writer
 * @param buff buffer containing the data
 * @param size size of the buffer
 * @param env pointer to the environment
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_to_buffer(
    guththila_xml_writer_t * wr,
    char *buff,
    int size,
    const axutil_env_t * env);

/*
 * Write the name space with the given prifix and namespace.
 * @param wr pointer to the writer
 * @param prefix prefix of the namespace
 * @param uri uri of the namespace
 * @param env pointer to the environment 
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_namespace(
    guththila_xml_writer_t * wr,
    char *prefix,
    char *uri,
    const axutil_env_t * env);

/*
 * Write the name space with the given prifix and namespace.
 * @param wr pointer to the writer
 * @param prefix prefix of the namespace
 * @param uri uri of the namespace
 * @param local_name name of the attribute
 * @param value value of the attribute
 * @param env pointer to the environment 
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_do_write_attribute_with_prefix_and_namespace(
    guththila_xml_writer_t * wr,
    char *prefix,
    char *uri,
    char *local_name,
    char *value,
    const axutil_env_t * env);


/*
 * Write the start document element with the xml version and encoding.
 * @param wr pointer to the writer
 * @param env pointer to the environment 
 * @param encoding encoding of the XML.
 * @param version xml version
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_start_document(
    guththila_xml_writer_t * wr,
    const axutil_env_t * env,
    char *encoding,
    char *version);

/*
 * Write the start element.
 * @param wr pointer to the writer
 * @param name name of the element
 * @param env pointer to the environment 
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_start_element(
    guththila_xml_writer_t * wr,
    char *name,
    const axutil_env_t * env);

/*
 * Write the end element. 
 * @param wr pointer to the writer
 * @param env pointer to the environment 
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_end_element(
    guththila_xml_writer_t * wr,
    const axutil_env_t * env);

/*
 * Not implemented.
 * @param wr pointer to the writer
 * @param env pointer to the environment 
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_close(
    guththila_xml_writer_t * wr,
    const axutil_env_t * env);

/*
 * Write the text content of a element. 
 * @param wr pointer to the writer
 * @param buff character string
 * @param env pointer to the environment 
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_characters(
    guththila_xml_writer_t * wr,
    char *buff,
    const axutil_env_t * env);

/*
 * Write comment with the given text data. 
 * @param wr pointer to the writer
 * @param buff character string
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_comment(
    guththila_xml_writer_t * wr,
    char *buff,
    const axutil_env_t * env);

/*
 * Write scape character. 
 * @param wr pointer to the writer
 * @param buff character string
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_escape_character(
    guththila_xml_writer_t * wr,
    char *buff,
    const axutil_env_t * env);

/*
 * Start to write an empty element with the given name. 
 * @param wr pointer to the writer
 * @param name name of the element
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_empty_element(
    guththila_xml_writer_t * wr,
    char *name,
    const axutil_env_t * env);

/*
 * Write a defualt namespace. 
 * @param wr pointer to the writer
 * @param uri uri of the namespace
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_default_namespace(
    guththila_xml_writer_t * wr,
    char *uri,
    const axutil_env_t * env);

/*
 * Write a attribute with the given name and value. 
 * @param wr pointer to the writer
 * @param localname name of the attribute
 * @param value value of the attribute
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_attribute(
    guththila_xml_writer_t * wr,
    char *localname,
    char *value,
    const axutil_env_t * env);

/*
 * Write a attribute with the given name and value and namespace. 
 * @param wr pointer to the writer
 * @param prefix prefix of the attribute
 * @param namespace_uri uri of the namespace
 * @param localname name of the attribute
 * @param value value of the attribute
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_write_attribute_with_prefix_and_namespace(
    guththila_xml_writer_t * wr,
    char *prefix,
    char *namespace_uri,
    char *localname,
    char *value,
    const axutil_env_t * env);

/*
 * Write a attribute with the given name, value and prefix. If the prefix 
 * is not defined previously as a namespace this method will fail. 
 * @param wr pointer to the writer
 * @param prefix prefix of the attribute
 * @param localname name of the attribute
 * @param value value of the attribute
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_attribute_with_prefix(
    guththila_xml_writer_t * wr,
    char *prefix,
    char *localname,
    char *value,
    const axutil_env_t * env);

/*
 * Write a attribute with the given name, value and namespace uri.  
 * If the namespace is not defined previously as a namespace this 
 * method will fail. The prefix corresponding to the namespace uri 
 * will be used. 
 * @param wr pointer to the writer
 * @param namesp namespace uri
 * @param localname name of the attribute
 * @param value value of the attribute
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_attribute_with_namespace(
    guththila_xml_writer_t * wr,
    char *namesp,
    char *localname,
    char *value,
    const axutil_env_t * env);

/*
 * Write a start element with prefix and namespace. If the namespace is not 
 * defined previoully new namespace will be written. 
 * @param wr pointer to the writer
 * @param prefix prefix of the attribute
 * @param namespace_uri uri
 * @param localname name of the attribute
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_write_start_element_with_prefix_and_namespace(
    guththila_xml_writer_t * wr,
    char *prefix,
    char *namespace_uri,
    char *local_name,
    const axutil_env_t * env);

/*
 * Write a start element with the namespace. If the namespace is not 
 * defined previously method will fail. 
 * @param wr pointer to the writer
 * @param namespace_uri uri
 * @param localname name of the attribute
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_write_start_element_with_namespace(
    guththila_xml_writer_t * wr,
    char *namespace_uri,
    char *local_name,
    const axutil_env_t * env);

/*
 * Write a start element with the prefix. If the prefix is not 
 * defined previously method will fail. 
 * @param wr pointer to the writer
 * @param namespace_uri uri
 * @param localname name of the attribute
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_write_start_element_with_prefix(
    guththila_xml_writer_t * wr,
    char *prefix,
    char *local_name,
    const axutil_env_t * env);

/*
 * Write a empty element with prefix and namespace. If the namespace is not 
 * defined previoully new namespace will be written. 
 * @param wr pointer to the writer
 * @param prefix prefix of the attribute
 * @param namespace_uri uri
 * @param localname name of the attribute
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_write_empty_element_with_prefix_and_namespace(
    guththila_xml_writer_t * wr,
    char *prefix,
    char *namespace_uri,
    char *local_name,
    const axutil_env_t * env);

/*
 * Write a empty element with the namespace. If the namespace is not 
 * defined previously method will fail. 
 * @param wr pointer to the writer
 * @param namespace_uri uri
 * @param localname name of the attribute
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_write_empty_element_with_namespace(
    guththila_xml_writer_t * wr,
    char *namespace_uri,
    char *local_name,
    const axutil_env_t * env);

/*
 * Write a empty element with the prefix. If the prefix is not 
 * defined previously method will fail. 
 * @param wr pointer to the writer
 * @param namespace_uri uri
 * @param localname name of the attribute
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_write_empty_element_with_prefix(
    guththila_xml_writer_t * wr,
    char *prefix,
    char *local_name,
    const axutil_env_t * env);

/*
 * Close all the elements that were started by writing the end elements.
 * @param wr pointer to the writer
 * @param env pointer to the environment
 */ 
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_end_document(
    guththila_xml_writer_t * wr,
    const axutil_env_t * env);
/* 
 * Write a new element with the name, then write the characters as text, 
 * then close the element and write a new line.
 * @param wr pointer to the writer
 * @element_name name of the element
 * @characters text of the new element
 * @param env pointer to the environment
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL guththila_write_line(
    guththila_xml_writer_t * wr,
    char *element_name,
    char *characters,
    const axutil_env_t * env);

/*
 * Get the memory buffer that is written.  
 * @param wr pointer to the writer
 * @param env pointer to the environment
 * @return memory buffer
 */
GUTHTHILA_EXPORT char *GUTHTHILA_CALL guththila_get_memory_buffer(
    guththila_xml_writer_t * wr,
    const axutil_env_t * env);

/*
 * Get the size of the memory buffer. 
 * @param wr pointer to the writer
 * @param env pointer to the environment
 * @return size of the buffer
 */
GUTHTHILA_EXPORT unsigned int GUTHTHILA_CALL
guththila_get_memory_buffer_size(
    guththila_xml_writer_t * wr,
    const axutil_env_t * env);

/*
 * Free the writer. 
 * @param wr pointer to the writer
 * @param env pointer to the environment
 */
GUTHTHILA_EXPORT void GUTHTHILA_CALL guththila_xml_writer_free(
    guththila_xml_writer_t * wr,
    const axutil_env_t * env);
/*
 * Get the prefix for the namespace.
 * @param wr pointer to the writer
 * @namespace namespace uri
 * @param env pointer to the environment
 * @return prefix for the namspace uri
 */
GUTHTHILA_EXPORT char *GUTHTHILA_CALL guththila_get_prefix_for_namespace(
    guththila_xml_writer_t * wr,
    char *namespace,
    const axutil_env_t * env);

EXTERN_C_END()
#endif
