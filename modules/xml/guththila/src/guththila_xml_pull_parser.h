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
 *	
 * @author Dinesh Premalal (xydinesh@gmail.com, premalwd@cse.mrt.ac.lk)	
 */

#ifndef GUTHTHILA_XML_PULL_PARSER_H
#define GUTHTHILA_XML_PULL_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "guththila_buffer.h"
#include "guththila_reader.h"
#include "guththila_token.h"
#include "guththila_namespace.h"
#include "guththila_depth.h"
#include "guththila_stack.h"
#include "guththila_unicode.h"

#define p_FILE __FILE__		/* file name */
#define LINE __LINE__		/* line number */

/*NOTE:
 *-----
 *The above defined p_FILE and LINE should provide as parameters when
 *   throwing Xml_PullParser_Exception
 */

enum status
{
  S_0 = 0,
  S_1,
  S_2,
  S_3,
  S_4
};

enum event_types
{
  START_DOCUMENT = 10,
  START_guththila_element_t = 20,
  END_guththila_element_t = 30,
  EMPTY_guththila_element_t = 40,
  CHARACTER = 50,
  ENTITY_REFERANCE = 60,
  COMMENT = 70
};
enum event_types EVENT;


typedef struct guththila_xml_pull_parser_s
{
  guththila_buffer_t *buffer;
  guththila_reader_t *reader;
  guththila_token_t *prefix;
  guththila_token_t *name;
  guththila_token_t *value;
  guththila_stack_t *stack;
  guththila_stack_t *attrib;
  guththila_stack_t *namesp;
  guththila_stack_t *dep;
  int _next;
  int offset;
  int last;
  int unicode_state;
  enum status STATUS;
} guththila_xml_pull_parser_t;


int guththila_xml_pull_parser_read (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_next_char(guththila_xml_pull_parser_t * p, int eof);
int guththila_xml_pull_parser_tokenize (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_exception (char *s, int line);
int guththila_xml_pull_parser_skip_spaces (guththila_xml_pull_parser_t * p, int c);
int guththila_xml_pull_parser_process_xml_decl (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_process_version_info (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_process_encoding_decl (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_process_sd_decl (guththila_xml_pull_parser_t * p);
void guththila_xml_pull_parser_open_token (guththila_xml_pull_parser_t * p);
void guththila_xml_pull_parser_close_token (guththila_xml_pull_parser_t * p, int t, int refer);
char *guththila_xml_pull_parser_last_char (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_process_eq (guththila_xml_pull_parser_t * p, int c);
void guththila_xml_pull_parser_add_attribute (guththila_xml_pull_parser_t * p, guththila_token_t * name,
				  guththila_token_t * value);
void guththila_xml_pull_parser_add_attribute_with_prefix (guththila_xml_pull_parser_t * p,
					      guththila_token_t * prefix, guththila_token_t * name,
					      guththila_token_t * value);
int guththila_xml_pull_parser_process_char_data (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_process_space_tag_or_empty_element (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_process_comment (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_process_pi (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_process_empty_tag (guththila_xml_pull_parser_t * p);
void guththila_xml_pull_parser_reset (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_process_name (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_process_attribute (guththila_xml_pull_parser_t * p, int c);
int guththila_xml_pull_parser_process_attribute_value (guththila_xml_pull_parser_t * p, int c);
int guththila_xml_pull_parser_is_space (int c);
void guththila_xml_pull_parser_relocate_tokens (guththila_xml_pull_parser_t * p, int offset);
void guththila_xml_pull_parser_shift (guththila_xml_pull_parser_t * p);
void guththila_xml_pull_parser_add_namespace (guththila_xml_pull_parser_t * p, guththila_token_t * name,
				  guththila_token_t * uri);
void guththila_xml_pull_parser_open_element (guththila_xml_pull_parser_t * p);
void guththila_xml_pull_parser_close_element (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_from_utf16 (guththila_xml_pull_parser_t * p, int eof);
int guththila_xml_pull_parser_is_valid_starting_char (guththila_xml_pull_parser_t * p, int c);

int guththila_xml_pull_parser_next (guththila_xml_pull_parser_t * p);
guththila_xml_pull_parser_t *guththila_xml_pull_parser_create (guththila_reader_t * r);
void guththila_xml_pull_parser_free ();
int guththila_xml_pull_parser_get_attribute_count (guththila_xml_pull_parser_t * p);
char *guththila_xml_pull_parser_get_attribute_name (guththila_xml_pull_parser_t * p, guththila_attribute_t * att);
char *guththila_xml_pull_parser_get_attribute_value (guththila_xml_pull_parser_t * p, guththila_attribute_t * att);
char *guththila_xml_pull_parser_get_attribute_prefix (guththila_xml_pull_parser_t * p, guththila_attribute_t * att);
guththila_attribute_t *guththila_xml_pull_parser_get_attribute (guththila_xml_pull_parser_t * p);
char *guththila_xml_pull_parser_get_attribute_name_by_number (guththila_xml_pull_parser_t * p, int i);
char *guththila_xml_pull_parser_get_attribute_value_by_number (guththila_xml_pull_parser_t * p, int i);
char *guththila_xml_pull_parser_get_attribute_prefix_by_number (guththila_xml_pull_parser_t * p, int i);
char *guththila_xml_pull_parser_get_name (guththila_xml_pull_parser_t * p);
char *guththila_xml_pull_parser_get_prefix (guththila_xml_pull_parser_t * p);
char *guththila_xml_pull_parser_get_value (guththila_xml_pull_parser_t * p);
guththila_namespace_t *guththila_xml_pull_parser_get_namespace (guththila_xml_pull_parser_t * p);
int guththila_xml_pull_parser_get_namespacecount (guththila_xml_pull_parser_t * p);
char *guththila_xml_pull_parser_get_namespace_uri (guththila_xml_pull_parser_t * p, guththila_namespace_t * ns);
char *guththila_xml_pull_parser_get_namespace_prefix (guththila_xml_pull_parser_t * p, guththila_namespace_t * ns);
char *guththila_xml_pull_parser_get_namespace_prefix_by_number (guththila_xml_pull_parser_t * p, int i);
char *guththila_xml_pull_parser_get_namespace_uri_by_number (guththila_xml_pull_parser_t * p, int i);
char *guththila_xml_pull_parser_get_attribute_namespace_by_number (guththila_xml_pull_parser_t *p, int i);

#endif /* GUTHTHILA_XML_PULL_PARSER_H */
