 
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
#ifndef GUTHTHILA_H
#define GUTHTHILA_H

#include <guththila_defines.h>
#include <guththila_token.h>
#include <guththila_reader.h>
#include <guththila_xml_writer.h>
#include <guththila_attribute.h>
#include <guththila_namespace.h>
#include <guththila_buffer.h>
#include <guththila_stack.h>
#include <guththila_error.h>

#include <axutil_utils.h>

/*
All the functions in this library does not check weather the given arguments are NULL.
It is the responsblity of the user to check weather the arguments contain NULL values.
*/
EXTERN_C_START()  

enum guththila_status
{
    S_0 = 0, 
	S_1, 
	S_2, 
	S_3
};

enum guththila_UTF16_endianess
{
    None = 1, 
	LE, 
	BE 
};

typedef enum guththila_type
{
    type_file_name = 0, 
	type_memory_buffer, 
	type_reader, 
	type_io
} guththila_type_t;

enum guththila_event_types
{
    GUTHTHILA_START_DOCUMENT =0, 
	GUTHTHILA_END_ELEMENT, 
	GUTHTHILA_CHARACTER,
    GUTHTHILA_ENTITY_REFERANCE, 
	GUTHTHILA_COMMENT,
    GUTHTHILA_SPACE, 
	GUTHTHILA_START_ELEMENT,
    GUTHTHILA_EMPTY_ELEMENT
};

typedef struct guththila_s
{    
    guththila_tok_list_t tokens; /* Token cache */

    guththila_buffer_t buffer;  /* Holding incoming xml string */  

    guththila_reader_t *reader; /* Reading the data */

    guththila_token_t *prefix; /* Prefix of the xml element */

    guththila_token_t *name; /* xml element local name */

    guththila_token_t *value; /* text of a xml element */    

    guththila_stack_t elem; /* elements are put in a stack */

    guththila_stack_t attrib; /* Attributes are put in a stack */

    guththila_stack_t namesp; /* namespaces are put in a stack */    

    int status;

    int guththila_event; /* Current event */

    size_t next;    /* Keep track of the position in the xml string */

    int last_start; /* Keep track of the starting position of the last token */

    guththila_token_t *temp_prefix; /* Temporery location for prefixes */

    guththila_token_t *temp_name;   /* Temporery location for names */
    
    guththila_token_t *temp_tok;   /* We don't know this until we close it */
} guththila_t;

/* 
 * An element will contain one of these things if it has namespaces 
 * */
typedef struct guththila_elem_namesp_s
{
    guththila_namespace_t *namesp; /* Array of namespaces */
    int no;                        /*Number of namespace in the element */
    int size;                      /* Allocated size */
} guththila_elem_namesp_t;

/*
 * Element.
 */
typedef struct guththila_element_s
{
    guththila_token_t *name;    /* local name */

    guththila_token_t *prefix;  /* prefix */
    
	int is_namesp;              /* Positive if a namespace is present */
} guththila_element_t;

/* Initialize the parser */
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_init(guththila_t * m, void *reader, 
			   const axutil_env_t * env);

/* Uninitialize the parser */
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_un_init(guththila_t * m, const axutil_env_t * env);

/* Still not used */
typedef void(GUTHTHILA_CALL * guththila_error_func)(void *arg, 
													const guththila_char_t *msg, 
													guththila_error_level level, 
													void *locator);

/* 
 * Parse the xml and return an event. If something went wrong it will return -1. 
 * The events are of the type guththila_event_types. According to the event 
 * user can get the required information using the appriate functions.
 * @param g pointer to a guththila_t structure
 * @param env the environment
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_next(guththila_t * g, const axutil_env_t * env);

/*
 * Return the number of attributes in the current element. 
 * @param g pointer to a guththila_t structure
 * @param env the environment
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_get_attribute_count(guththila_t * g, const axutil_env_t * env);

/*
 * Return the attribute name.
 * @param g pointer to a guththila_t structure
 * @param att pointer to a attribute
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t * GUTHTHILA_CALL
guththila_get_attribute_name(guththila_t * g, guththila_attr_t * att, 
							 const axutil_env_t * env);

/*
 * Return the attribute value.
 * @param g pointer to a guththila_t structure
 * @param att pointer to a attribute
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t * GUTHTHILA_CALL  
guththila_get_attribute_value(guththila_t * g, 
							  guththila_attr_t * att, 
							  const axutil_env_t * env);

/*
 * Return the attribute prefix.
 * @param g pointer to a guththila_t structure
 * @param att pointer to a attribute
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_attribute_prefix(guththila_t * g, 
							   guththila_attr_t * att, 
							   const axutil_env_t * env);

/* 
 * Return the attribute 
 * @param g pointer to a guththila_t structure
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_attr_t *GUTHTHILA_CALL  
guththila_get_attribute(guththila_t * g, const axutil_env_t * env);

/*
 * Return the name of the attribute by the attribute bumber. 
 * First attribute will be 1.
 * @param g pointer to a guththila_t structure
 * @param index position of the attribute
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_attribute_name_by_number(guththila_t * g, int index, 
									   const axutil_env_t *env);

/*
 * Return the attribute value by number.
 * First attribute will be 1.
 * @param g pointer to a guththila_t structure
 * @param index position of the attribute
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_attribute_value_by_number(guththila_t * g, int index, 
										const axutil_env_t *env);

/* 
 * Return the prefix of the attribute.
 * First attribute will be 1.
 * @param g pointer to a guththila_t structure
 * @param index position of the attribute
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_attribute_prefix_by_number(guththila_t * g, int index, 
										 const axutil_env_t *env);

/*
 * Return the name of the element.
 * @param g pointer to a guththila_t structure
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_name(guththila_t * g, const axutil_env_t * env);

/*
 * Return the prefix of the element.
 * @param g pointer to a guththila_t structure
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_prefix(guththila_t * g, const axutil_env_t * env);

/*
 * Return the text of the element.
 * @param g pointer to a guththila_t structure
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_value(guththila_t * g, const axutil_env_t * env);

/*
 * Return the namespace of the element.
 * @param g pointer to a guththila_t structure
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_namespace_t *GUTHTHILA_CALL  
guththila_get_namespace(guththila_t * g, const axutil_env_t * env);

/*
 * Return the number of namespaces in the element.
 * @param g pointer to a guththila_t structure
 * @param env the environment
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_get_namespace_count(guththila_t * g, const axutil_env_t * env);

/*
 * Return the namespace uri of the given namespace.
 * @param g pointer to a guththila_t structure
 * @param ns pointer to a namespace
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t * GUTHTHILA_CALL
guththila_get_namespace_uri(guththila_t * g, guththila_namespace_t * ns, 
							const axutil_env_t * env);

/* 
 * Return the prefix of the namespace.
 * @param g pointer to a guththila_t structure
 * @param ns pointer to a namespace
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_namespace_prefix(guththila_t * p, guththila_namespace_t * ns, 
							   const axutil_env_t * env);

/*
 * Return the prefix of the namespace at the given position.
 * First namespace will have the value 1.
 * @param g pointer to a guththila_t structure
 * @param index position of the namespace
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_namespace_prefix_by_number(guththila_t * g, int index, 
										 const axutil_env_t *env);

/*
 * Get the uri of the namespace at the given position.
 * First namespace will have the value 1.
 * @param g pointer to a guththila_t structure
 * @param index position of the namespace
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_namespace_uri_by_number(guththila_t * g, int index, 
									  const axutil_env_t *env);

/*
 * Get the attribute namespace of the attribute at the given position.
 * @param g pointer to a guththila_t structure
 * @param index position of the namespace
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_attribute_namespace_by_number(guththila_t *g, int index, 
											const axutil_env_t *env);

/*
 * Get the encoding. at the moment we don't support UNICODE
 * @param g pointer to a guththila_t structure
 * @param env the environment
 */
GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL  
guththila_get_encoding(guththila_t * p, const axutil_env_t * env);

/* 
 * To do. Implement a proper error handling mechanism.
 * @param g pointer to a guththila_t structure
 * @param env the environment
 */
GUTHTHILA_EXPORT void GUTHTHILA_CALL
guththila_set_error_handler(guththila_t * m, guththila_error_func, 
							const axutil_env_t * env);
EXTERN_C_END() 
#endif  

