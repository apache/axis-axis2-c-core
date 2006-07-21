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

#include <guththila_xml_pull_parser.h>

static int
guththila_is_space (int c)
{
  return (0x20 == c || 0x9 == c || 0xD == c || 0xA == c);
}


AXIS2_EXTERN guththila_xml_pull_parser_t *
guththila_xml_pull_parser_create (axis2_env_t * environment,
                                  guththila_reader_t * r)
{
  guththila_xml_pull_parser_t *parser =
    (guththila_xml_pull_parser_t *) AXIS2_MALLOC (
						  environment->allocator, sizeof (guththila_xml_pull_parser_t));
                
  parser->buffer = guththila_buffer_create (environment, 1024);
  parser->stack = axis2_stack_create (environment);
  parser->attrib = axis2_stack_create (environment);
  parser->namesp = axis2_stack_create (environment);
  parser->dep = axis2_stack_create (environment);
  parser->name = NULL;
  parser->prefix = NULL;
  parser->value = NULL;
  parser->reader = r;
  parser->_next = 0;
  parser->offset = 0;
  parser->last = -1;
  parser->status = S_1;
  parser->unicode_state = None;

  return parser;
}


AXIS2_EXTERN void
guththila_xml_pull_parser_free (axis2_env_t * environment,
                                guththila_xml_pull_parser_t * parser)
{
  if (parser->buffer)
    guththila_buffer_free (environment, (void *) parser->buffer);
  if (parser->stack)
    AXIS2_STACK_FREE (parser->stack, environment);
  if (parser->attrib)
    AXIS2_STACK_FREE (parser->attrib, environment);
  if (parser->namesp)
     AXIS2_STACK_FREE (parser->namesp, environment);
  if (parser->dep)
    AXIS2_STACK_FREE (parser->dep, environment);
  AXIS2_FREE (environment->allocator, (void *) parser);
}


AXIS2_EXTERN int
guththila_xml_pull_parser_exception (guththila_char_t * file, int line, int error_code)
{
  printf ("Throws guththila_xml_pull_parser_exception \t %s \t %d \n", file,
	  line);
  /* Samisa: we should define a set of error codes and a corresponding error message set
     rather than printing the above line to the console, we should be appending the
     above string to the error message corresponding to the current error code
     please delete this comment once this is fixed
  */

  /* Dinesh: Modified exception function to print line number , file
     and return error code for particular error. If we feel that
     file and line number is not necessary lets remove them later.*/
  return error_code;
}


AXIS2_EXTERN void
guththila_xml_pull_parser_relocate_tokens 
(axis2_env_t *environment,
 guththila_xml_pull_parser_t *parser,
 int offset)
{
  guththila_token_t *el = NULL;
  int isize;
  isize = AXIS2_STACK_SIZE (parser->stack, environment);
  el = (guththila_token_t *) AXIS2_STACK_GET_AT (parser->stack, environment, isize);
  for (; isize > 0; isize--)
    {
      guththila_token_relocate (environment, el, offset);
      el = (guththila_token_t *) AXIS2_STACK_GET_AT (parser->stack, environment, isize);
    }
}


AXIS2_EXTERN void
guththila_xml_pull_parser_shift (axis2_env_t * environment,
                                 guththila_xml_pull_parser_t * parser)
{
  memmove (parser->buffer->buff, parser->buffer->buff + (parser->offset),
	   (parser->_next) - (parser->offset));
  parser->_next -= parser->offset;
  parser->last -= parser->offset;
  parser->offset = 0;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_read (axis2_env_t * environment,
                                guththila_xml_pull_parser_t * parser)
{
  int c = 0;
  if (parser->_next == parser->buffer->size)
    {
      if (parser->offset > 0)
        {
	  guththila_xml_pull_parser_relocate_tokens (environment, parser,
						     parser->offset);
	  guththila_xml_pull_parser_shift (environment, parser);
        }
      else
        {
	  int b = parser->buffer->size;
	  parser->buffer =
	    guththila_buffer_grow (environment, parser->buffer);
	  guththila_xml_pull_parser_relocate_tokens (
            environment, parser, (parser->buffer->size - b));
        }
    }
  c = guththila_reader_read (environment, (parser->buffer->buff),
			     (parser->_next),
			     (parser->buffer->size) - (parser->_next),
			     parser->reader);
  parser->last += c;
  return !c;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_from_utf16 (axis2_env_t * environment,
                                      guththila_xml_pull_parser_t * parser,
                                      int eof)
{
  int c = parser->buffer->buff[parser->_next++];
  if (((parser->_next) > (parser->last))
      && guththila_xml_pull_parser_read (environment, parser))
    {
      if (eof)
	return -1;
      else
	guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_UNEXPECTED_UTF16_EOF);
    }
  while (!c)
    c = parser->buffer->buff[parser->_next++];
  return c;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_next_char (axis2_env_t * environment,
                                     guththila_xml_pull_parser_t * parser,
                                     int eof)
{
  if (((parser->_next) > (parser->last))
      && guththila_xml_pull_parser_read (environment, parser))
    {
      if (eof)
	return -1;
      else
	guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_UNEXPECTED_EOF);
    }
  if (parser->_next == 0)
    {
      guththila_UTF8_char iu =
	(guththila_UTF8_char) parser->buffer->buff[parser->_next++];

      if (0xff == iu
	  && 0xfe ==
	  (guththila_UTF8_char) parser->buffer->buff[parser->_next++])
        {
	  parser->unicode_state = LE;
        }
      else if (0xfe == iu
	       && 0xff ==
	       (guththila_UTF8_char) parser->buffer->buff[parser->_next++])
        {
	  parser->unicode_state = BE;
        }
      else if (0xef == iu
	       && 0xbb ==
	       (guththila_UTF8_char) parser->buffer->buff[parser->_next++]
	       && 0xbf ==
	       (guththila_UTF8_char) parser->buffer->buff[parser->_next++])
        {
	  parser->unicode_state = None;
        }
      else
        {
	  parser->unicode_state = None;
	  parser->_next = 0;
        }
    }

  if (parser->unicode_state == None)
    return parser->buffer->buff[parser->_next++];
  else
    return guththila_xml_pull_parser_from_utf16 (environment, parser,
						 eof);
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_last_char (axis2_env_t * environment,
                                     guththila_xml_pull_parser_t * parser)
{
  return parser->buffer->buff + (parser->_next - 1);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_open_token (axis2_env_t * environment,
                                      guththila_xml_pull_parser_t * parser)
{
  guththila_token_t *t =
    (guththila_token_t *) AXIS2_MALLOC (environment->allocator,
					sizeof (guththila_token_t));
  t->type = Unknown;
  t->start = guththila_xml_pull_parser_last_char (environment, parser);
  AXIS2_STACK_PUSH (parser->stack, environment, t);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_close_token (axis2_env_t * environment,
                                       guththila_xml_pull_parser_t * parser,
                                       int t, int refer)
{
  guththila_token_t *token =
    (guththila_token_t *) AXIS2_STACK_GET (parser->stack, environment);
  token->type = t;
  token->ref = refer;
  token->end =
    guththila_xml_pull_parser_last_char (environment, parser) - 1;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_skip_spaces (axis2_env_t * environment,
                                       guththila_xml_pull_parser_t * parser,
                                       int c)
{
  while (0x20 == c || 0x9 == c || 0xD == c || 0xA == c)
    c = guththila_xml_pull_parser_next_char (environment, parser, 0);
  return c;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_eq (axis2_env_t * environment,
                                      guththila_xml_pull_parser_t * parser,
                                      int c)
{
  int ic;
  if (0x3D ==
      guththila_xml_pull_parser_skip_spaces (environment, parser, c))
    {
      ic = guththila_xml_pull_parser_next_char (environment, parser, 0);
      return guththila_xml_pull_parser_skip_spaces (environment, parser,
						    ic);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_PROCESS_EQUAL);
  return 0;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_version_info (axis2_env_t *
                                                environment,
                                                guththila_xml_pull_parser_t *
                                                parser)
{
  int ic;
  int quote;
  int nc;
  ic = guththila_xml_pull_parser_next_char (environment, parser, 0);
  ic = guththila_xml_pull_parser_skip_spaces (environment, parser, ic);
  if (ic == 'v')
    {
      guththila_xml_pull_parser_open_token (environment, parser);
      if ('e' ==
	  guththila_xml_pull_parser_next_char (environment, parser, 0)
	  && 'r' == guththila_xml_pull_parser_next_char (environment,
							 parser, 0)
	  && 's' == guththila_xml_pull_parser_next_char (environment,
							 parser, 0)
	  && 'i' == guththila_xml_pull_parser_next_char (environment,
							 parser, 0)
	  && 'o' == guththila_xml_pull_parser_next_char (environment,
							 parser, 0)
	  && 'n' == guththila_xml_pull_parser_next_char (environment,
							 parser, 0))
        {
	  ic = guththila_xml_pull_parser_next_char (environment, parser, 0);
	  guththila_xml_pull_parser_close_token (environment, parser,
						 _attribute, 0);
	  quote =
	    guththila_xml_pull_parser_process_eq (environment, parser,
						  ic);
	  nc = guththila_xml_pull_parser_next_char (environment, parser, 0);
	  /* 0, since we don't expect EOF line here */
	  guththila_xml_pull_parser_open_token (environment, parser);
	  while (quote != nc)
	    nc = guththila_xml_pull_parser_next_char (environment, parser,
						      0);
	  guththila_xml_pull_parser_close_token (environment, parser,
						 _attribute_value, 0);
	  nc = guththila_xml_pull_parser_next_char (environment, parser, 0);
	  return guththila_xml_pull_parser_skip_spaces (environment, parser,
							nc);
        }
      else
	guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_INCORRECT_VERSION_INFO);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_VERSION_INFO_NOT_FOUND);
  return 0;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_encoding_decl (axis2_env_t *
                                                 environment,
                                                 guththila_xml_pull_parser_t *
                                                 parser)
{
  int quote;
  int nc;
  int ic;
  guththila_xml_pull_parser_open_token (environment, parser);
  if ('n' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'c' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'o' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'd' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'i' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'n' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'g' == guththila_xml_pull_parser_next_char (environment, parser,
						     0))
    {
      ic = guththila_xml_pull_parser_next_char (environment, parser, 0);
      guththila_xml_pull_parser_close_token (environment, parser,
					     _attribute, 0);
      quote =
	guththila_xml_pull_parser_process_eq (environment, parser, ic);
      nc = guththila_xml_pull_parser_next_char (environment, parser, 0);
      guththila_xml_pull_parser_open_token (environment, parser);
      while (quote != nc)
	nc = guththila_xml_pull_parser_next_char (environment, parser, 0);
      guththila_xml_pull_parser_close_token (environment, parser,
					     _attribute_value, 0);
      nc = guththila_xml_pull_parser_next_char (environment, parser, 0);
      return guththila_xml_pull_parser_skip_spaces (environment, parser,
						    nc);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_ENCODING_DECLARATION_ERROR);
  return 0;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_sd_decl (axis2_env_t *
                                           environment,
                                           guththila_xml_pull_parser_t *
                                           parser)
{
  int quote = 0;
  int nc = 0;
  int ic = 0;
  guththila_xml_pull_parser_open_token (environment, parser);
  if ('t' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'a' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'n' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'd' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'a' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'l' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'o' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'n' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'e' == guththila_xml_pull_parser_next_char (environment, parser,
						     0))
    ic = guththila_xml_pull_parser_next_char (environment, parser, 0);
  guththila_xml_pull_parser_close_token (environment, parser, _attribute,
					 0);
  quote = guththila_xml_pull_parser_process_eq (environment, parser, ic);
  nc = guththila_xml_pull_parser_next_char (environment, parser, 0);
  guththila_xml_pull_parser_open_token (environment, parser);
  while (quote != nc)
    {
      if ('y' == nc)
        {
	  if ('e' !=
	      guththila_xml_pull_parser_next_char (environment, parser, 0)
	      || 's' != guththila_xml_pull_parser_next_char (environment,
							     parser, 0))
	    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_STANDALONE_ERROR_IN_YES);
	  else
	    nc = guththila_xml_pull_parser_next_char (environment, parser,
						      0);
        }
      else if ('n' == nc)
        {
	  if ('o' !=
	      guththila_xml_pull_parser_next_char (environment, parser, 0))
	    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_STANDALONE_ERROR_IN_NO);
	  else
	    nc = guththila_xml_pull_parser_next_char (environment, parser,
						      0);
        }
      else
	guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_STANDALONE_ERROR_YES_OR_NO_NOT_AVAILABLE);
    }
  guththila_xml_pull_parser_close_token (environment, parser,
					 _attribute_value, 0);
  nc = guththila_xml_pull_parser_next_char (environment, parser, 0);
  return guththila_xml_pull_parser_skip_spaces (environment, parser, nc);
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_xml_decl (axis2_env_t *
                                            environment,
                                            guththila_xml_pull_parser_t *
                                            parser)
{
  int ix;
  if ('x' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'm' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && 'l' == guththila_xml_pull_parser_next_char (environment, parser,
						     0))
    {
      ix = guththila_xml_pull_parser_process_version_info (environment,
							   parser);

      if ('e' == ix)
	ix = guththila_xml_pull_parser_process_encoding_decl (environment,
							      parser);

      if ('s' == ix)
	ix = guththila_xml_pull_parser_process_sd_decl (environment,
							parser);

      if ('?' == ix)
        {
	  if ('>' !=
	      guththila_xml_pull_parser_next_char (environment, parser, 0))
	    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_MISSING_GREATER_SIGN_IN_XML_DECLARATION);
	  else
	    parser->guththila_event = GUTHTHILA_START_DOCUMENT;
        }
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_INCORRECT_XML_DECLARATION);
  return 0;
}


AXIS2_EXTERN void
guththila_xml_pull_parser_reset (axis2_env_t * environment,
                                 guththila_xml_pull_parser_t * parser)
{
  parser->offset = parser->_next;
  parser->name = NULL;
  parser->prefix = NULL;
  parser->value = NULL;
/*   guththila_stack_clear (environment, parser->attrib); */
/*   guththila_stack_clear (environment, parser->stack); */
  if (parser->guththila_event == GUTHTHILA_END_ELEMENT
      || parser->guththila_event == GUTHTHILA_EMPTY_ELEMENT)
    guththila_xml_pull_parser_close_element (environment, parser);
}


AXIS2_EXTERN int
guththila_xml_pull_parser_is_space (axis2_env_t * environment,
                                    int c)
{
  if (0x20 == c || 0xD == c || 0xA == c || 0x9 == c)
    return 1;
  else
    return 0;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_is_valid_starting_char (axis2_env_t *
                                                  environment,
                                                  guththila_xml_pull_parser_t
                                                  * parser, int c)
{
  if (isalpha (c) || '_' == c || ':' == c)
    return 1;
  else
    return 0;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_name (axis2_env_t * environment,
                                        guththila_xml_pull_parser_t * parser)
{
  int c;
  guththila_char_t *x;
  x = guththila_xml_pull_parser_last_char (environment, parser);
  if (!guththila_xml_pull_parser_is_valid_starting_char
      (environment, parser, x[0]))
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_INVALID_NAME_STARTING_CHARACTER);

  guththila_xml_pull_parser_open_token (environment, parser);
  c = guththila_xml_pull_parser_next_char (environment, parser, 0);
  while (!
	 (guththila_xml_pull_parser_is_space (environment, c) || '/' == c
	  || '=' == c || '?' == c || '>' == c))
    {
      if (':' == c)
        {
	  guththila_xml_pull_parser_close_token (environment, parser,
						 _prefix, 0);
	  c = guththila_xml_pull_parser_next_char (environment, parser, 0);
	  guththila_xml_pull_parser_open_token (environment, parser);
        }
      else
	c = guththila_xml_pull_parser_next_char (environment, parser, 0);
    }
  guththila_xml_pull_parser_close_token (environment, parser, _name, 0);

  return c;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_attribute_value (axis2_env_t *
                                                   environment,
                                                   guththila_xml_pull_parser_t
                                                   * parser, int quote)
{
  int c = 0;
  if ('\'' == quote || '\"' == quote)
    {
      c = guththila_xml_pull_parser_next_char (environment, parser, 0);
      guththila_xml_pull_parser_open_token (environment, parser);
      do
        {
	  c = guththila_xml_pull_parser_next_char (environment, parser, 0);
	  if (quote == c)
            {
	      guththila_xml_pull_parser_close_token (environment, parser,
						     _attribute_value, 0);
	      return guththila_xml_pull_parser_next_char (environment,
							  parser, 0);
            }
        }
      while ('<' != c || '&' != c);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE,  GUTHTHILA_ERROR_QUOTES_NOT_FOUND_BEFORE_ATTRIBUTE_VALUE );
  return c;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_attribute (axis2_env_t *
                                             environment,
                                             guththila_xml_pull_parser_t *
                                             parser, int c)
{
  int q;
  q = guththila_xml_pull_parser_skip_spaces (environment, parser, c);
  q = guththila_xml_pull_parser_process_name (environment, parser);
  q = guththila_xml_pull_parser_process_eq (environment, parser, q);
  q = guththila_xml_pull_parser_process_attribute_value (environment,
							 parser, q);
  return q;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_processSTagOrEmptyElem (axis2_env_t *
                                                  environment,
                                                  guththila_xml_pull_parser_t
                                                  * parser)
{
  int c;
  c = guththila_xml_pull_parser_process_name (environment, parser);
  c = guththila_xml_pull_parser_skip_spaces (environment, parser, c);
  parser->guththila_event = GUTHTHILA_START_ELEMENT;
  for (;;)
    {
      if ('/' == c)
        {
	  parser->guththila_event = GUTHTHILA_EMPTY_ELEMENT;
	  if ('>' ==
	      guththila_xml_pull_parser_next_char (environment, parser, 0))
	    return c;
	  else
	    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_EMPTY_ELEMENT_NOT_CLOSED);
        }
      else if ('>' == c)
	return c;
      else
        {
	  c = guththila_xml_pull_parser_process_attribute (environment,
							   parser, c);
	  c = guththila_xml_pull_parser_skip_spaces (environment, parser,
						     c);
        }
    }
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_char_data (axis2_env_t *
                                             environment,
                                             guththila_xml_pull_parser_t *
                                             parser)
{
  int c;
  int ref = 0;
  /*     int all_spaces = 1; */
  /*  int last_event = parser->guththila_event; */
  parser->guththila_event = GUTHTHILA_CHARACTER;
  guththila_xml_pull_parser_open_token (environment, parser);
  do
    {
      c = guththila_xml_pull_parser_next_char (environment, parser, -1);

      /* if (c != '<') */
      /*             all_spaces = (all_spaces && guththila_is_space (c)); */

      if (c == '&')
	ref = 1;

      if (c == -1)
        {
	  guththila_xml_pull_parser_close_token (environment, parser,
						 _char_data, ref);
	  return 0;
        }
    }
  while (c != '<');

  
    /*  if (all_spaces) */
    /*         parser->guththila_event = GUTHTHILA_SPACE; */
    guththila_xml_pull_parser_close_token (environment, parser, _char_data,
                                           ref);
    return c;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_end_tag (axis2_env_t *
					   environment,
					   guththila_xml_pull_parser_t *
					   parser)
{
  int c;
  c = guththila_xml_pull_parser_next_char (environment, parser, 0);
  c = guththila_xml_pull_parser_process_name (environment, parser);
  c = guththila_xml_pull_parser_skip_spaces (environment, parser, c);
  if ('>' == c)
    parser->guththila_event = GUTHTHILA_END_ELEMENT;
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_END_TAG_NOT_CLOSED);
  return c;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_pi (axis2_env_t * environment,
                                      guththila_xml_pull_parser_t * parser)
{
  int c;
  parser->guththila_event = GUTHTHILA_COMMENT;
  c = guththila_xml_pull_parser_next_char (environment, parser, 0);
  while ('<' != c)
    c = guththila_xml_pull_parser_next_char (environment, parser, 0);
  return c;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_process_comment (axis2_env_t *
                                           environment,
                                           guththila_xml_pull_parser_t *
                                           parser)
{
  int c;
  if ('-' == guththila_xml_pull_parser_next_char (environment, parser, 0)
      && '-' == guththila_xml_pull_parser_next_char (environment, parser,
						     0))
    {
      c = guththila_xml_pull_parser_next_char (environment, parser, 0);
      while (1)
        {
	  c = guththila_xml_pull_parser_next_char (environment, parser, 0);
	  if ('-' == c)
            {
	      c = guththila_xml_pull_parser_next_char (environment, parser,
						       0);
	      if ('-' == c)
                {
		  if ('>' ==
		      guththila_xml_pull_parser_next_char (environment,
							   parser, 0))
                    {
		      parser->guththila_event = GUTHTHILA_COMMENT;
		      while ('<' != c)
                        {
			  c = guththila_xml_pull_parser_next_char
			    (environment, parser, -1);
			  if (c == -1)
			    return c;
                        }
		      return c;
                    }
		  else
		    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_MORE_HYPENS_OCCURED_IN_COMMENT);
                }
            }
        }
    }
  else
    {
      c = guththila_xml_pull_parser_next_char (environment, parser, 0);
      while ('<' != c)
	c = guththila_xml_pull_parser_next_char (environment, parser, -1);
    }
  return 0;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_tokenize (axis2_env_t * environment,
                                    guththila_xml_pull_parser_t * parser)
{
  do
    {
      int c;
      c = guththila_xml_pull_parser_next_char (environment, parser, -1);
      if (c == -1)
	return -1;

      switch (parser->status)
        {
        case S_1:
	  while (isspace(c))
            {
	      c = guththila_xml_pull_parser_next_char (environment, parser, -1);
		if (c == -1)
		  return -1;
		  }

            if ('<' == c)
	      parser->status = S_2;
            else
	      parser->status = S_0;
            break;
        case S_2:
	  {
	    if ('?' == c)
	      {
		guththila_xml_pull_parser_process_xml_decl (environment,
							    parser);
		parser->status = S_3;
	      }
	    else if ('!' == c)
	      {
		guththila_xml_pull_parser_process_comment (environment,
							   parser);
		parser->status = S_4;
	      }
	    else
	      {
		guththila_xml_pull_parser_processSTagOrEmptyElem
		  (environment, parser);
		parser->status = S_3;
	      }
	  }
	  break;
        case S_3:
	  if ('<' == c)
	    parser->status = S_4;
	  else
            {
	      c = guththila_xml_pull_parser_process_char_data (environment,
							       parser);
	      if ('<' == c)
                {
		  parser->status = S_4;
		  return parser->guththila_event;
                }
	      else if ('\0' == c)
		parser->status = S_3;
	      else
		parser->status = S_0;
            }
	  break;
        case S_4:
	  if ('/' == c)
            {
	      guththila_xml_pull_parser_process_end_tag (environment,
							 parser);
	      parser->status = S_3;
            }
	  else if ('?' == c)
	    guththila_xml_pull_parser_process_pi (environment, parser);
	  else if ('!' == c)
	    guththila_xml_pull_parser_process_comment (environment,
						       parser);
	  else
            {
	      guththila_xml_pull_parser_processSTagOrEmptyElem (environment,
								parser);
	      parser->status = S_3;
            }
	  break;
        case S_0:
	  guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_TOKENIZE_ERROR);
        }
    }
  while (parser->status != S_3);
  return parser->guththila_event;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_next (axis2_env_t * environment,
                                guththila_xml_pull_parser_t * parser)
{
  int ix;
  guththila_xml_pull_parser_reset (environment, parser);
  ix = guththila_xml_pull_parser_tokenize (environment, parser);
  switch (ix)
    {
    case -1:
      return -1;

    case GUTHTHILA_START_DOCUMENT:
      {
	int ix;
	guththila_token_t *token;
	guththila_token_t *att_name;
	guththila_token_t *att_value;
	ix = AXIS2_STACK_SIZE (parser->stack, environment);
	for (; ix > 0; ix--)
	  {
	    token = (guththila_token_t *)AXIS2_STACK_POP (parser->stack, environment);
	    if (token->type == _attribute_value)
	      {
		att_value = token;
	    token = (guththila_token_t *)AXIS2_STACK_POP (parser->stack, environment);
	    ix--;
		att_name = token;
		guththila_xml_pull_parser_add_attribute (environment,
							 parser, att_name,
							 att_value);
	      }
	  }
      }
      break;
    case GUTHTHILA_START_ELEMENT:
    case GUTHTHILA_EMPTY_ELEMENT:
      {
	int is = 0;
	guththila_token_t *token;
	guththila_token_t *name = NULL;
	guththila_token_t *value = NULL;
	is = AXIS2_STACK_SIZE (parser->stack, environment);
	for (;
	     is > 0 && 	AXIS2_STACK_SIZE (parser->stack, environment);
	     is--)
	  {
	    token = (guththila_token_t *) AXIS2_STACK_POP (parser->stack, environment);
	    /* be careful about memory leaks, when we pull it we get
	       seperate _element need to free it properly */
	    if (token->type == _attribute_value)
	      {
		value = token;
		token = (guththila_token_t *) AXIS2_STACK_POP (parser->stack, environment);
		is--;
		name = token;
		if (!guththila_token_compare
		    (environment, token, "xmlns", 5,
		     parser->unicode_state))
		  guththila_xml_pull_parser_add_namespace (environment,
							   parser, name,
							   value);
		else
		  guththila_xml_pull_parser_add_attribute (environment,
							   parser, name,
							   value);
	      }
	    else if (token->type == _name)
	      {
		name = token;
		parser->name = name;
		parser->prefix = NULL;

	      }
	    else if (token->type == _prefix)
	      {
		guththila_attribute_t *attribute;
		if (!guththila_token_compare
		    (environment, token, "xmlns", 5,
		     parser->unicode_state))
		  {
		    attribute = (guththila_attribute_t *) AXIS2_STACK_POP ( parser->attrib, environment);
		    guththila_xml_pull_parser_add_namespace (environment,
							     parser,
							     attribute->
							     name,
							     attribute->
							     value);
		  }
		else
		  {
		    if (parser->name)
		      parser->prefix = token;
		    else
		      {
			attribute = (guththila_attribute_t *) AXIS2_STACK_POP ( parser->attrib, environment);
			guththila_xml_pull_parser_add_attribute_with_prefix
			  (environment, parser, token,
			   attribute->name, attribute->value);
		      }
		  }
	      }
	    else
	      guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_INVALID_TOKEN_TYPE);
	  }
	guththila_xml_pull_parser_open_element (environment, parser);
      }
      break;
    case GUTHTHILA_END_ELEMENT:
      {
	guththila_token_t *token;
	token = (guththila_token_t *) AXIS2_STACK_POP (parser->stack, environment);
	if (token->type == _name)
	  parser->name = token;

	if (AXIS2_STACK_SIZE (parser->stack, environment))
	  {
	    token = (guththila_token_t *) AXIS2_STACK_POP (parser->stack, environment);
	    if (token->type == _prefix)
	      parser->prefix = token;
	  }
      }
      break;
    case GUTHTHILA_CHARACTER:
      {
	guththila_token_t *token;
	token = (guththila_token_t *) AXIS2_STACK_POP (parser->stack, environment);
	parser->value = token;
      }
      break;
    case GUTHTHILA_ENTITY_REFERANCE:
      break;
    case GUTHTHILA_COMMENT:
      break;

    };
  return parser->guththila_event;
}


AXIS2_EXTERN void
guththila_xml_pull_parser_open_element (axis2_env_t * environment,
                                        guththila_xml_pull_parser_t * parser)
{
  int ii;
  guththila_depth_t *m =
    (guththila_depth_t *) AXIS2_MALLOC (environment->allocator,
					sizeof (guththila_depth_t));
  guththila_depth_t *l = NULL;
  guththila_depth_t *depth;
  ii = AXIS2_STACK_SIZE (parser->dep, environment);
  if (!ii)
    {
      m->first = 0;
      m->total = AXIS2_STACK_SIZE (parser->dep, environment);
      m->count = m->total;
/*       guththila_stack_push_depth (environment, parser->dep, m); */
      AXIS2_STACK_PUSH (parser->dep, environment, m);
    }
  else
    {
      depth = (guththila_depth_t *) AXIS2_STACK_GET (parser->dep, environment);
      l = depth;
      m->first = l->first + l->count;
      m->total = AXIS2_STACK_SIZE (parser->namesp, environment);
      m->count = m->total - l->total;
      AXIS2_STACK_PUSH (parser->dep, environment, m);
    }
}


AXIS2_EXTERN void
guththila_xml_pull_parser_close_element (axis2_env_t *
                                         environment,
                                         guththila_xml_pull_parser_t * parser)
{
  guththila_depth_t *depth;
  guththila_namespace_t *namespace;
  int ii;
  depth = (guththila_depth_t *) AXIS2_STACK_POP (parser->dep, environment);
  ii = depth->count;
  for (; ii > 0; ii--)
    {
      namespace = (guththila_namespace_t *) AXIS2_STACK_POP (parser->namesp, environment);
      if (namespace->name)
        {
	  AXIS2_FREE (environment->allocator, namespace->name);
	  namespace->name = NULL;
        }
      if (namespace->uri)
        {
	  AXIS2_FREE (environment->allocator, namespace->uri);
	  namespace->uri = NULL;
        }
      
    }

}


AXIS2_EXTERN void
guththila_xml_pull_parser_add_attribute (axis2_env_t *
                                         environment,
                                         guththila_xml_pull_parser_t * parser,
                                         guththila_token_t * name,
                                         guththila_token_t * value)
{
  guththila_attribute_t *att;
  att =
    (guththila_attribute_t *) AXIS2_MALLOC (environment->allocator,
					    sizeof
					    (guththila_attribute_t));
  att->name = name;
  att->value = value;
  att->prefix = NULL;
/*   guththila_stack_push (environment, parser->attrib, NULL, att); */
  AXIS2_STACK_PUSH (parser->attrib, environment, att);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_add_attribute_with_prefix (axis2_env_t *
                                                     environment,
                                                     guththila_xml_pull_parser_t
                                                     * parser,
                                                     guththila_token_t *
                                                     prefix,
                                                     guththila_token_t * name,
                                                     guththila_token_t *
                                                     value)
{
  guththila_attribute_t *att;
  att =
    (guththila_attribute_t *) AXIS2_MALLOC (environment->allocator,
					    sizeof
					    (guththila_attribute_t));
  att->name = name;
  att->value = value;
  att->prefix = prefix;
  AXIS2_STACK_PUSH (parser->attrib, environment, att);
/*   guththila_stack_push (environment, parser->attrib, NULL, att); */
  /* _element can keep , tokens and attributes here token set to null */
}


AXIS2_EXTERN void
guththila_xml_pull_parser_add_namespace (axis2_env_t *
                                         environment,
                                         guththila_xml_pull_parser_t * parser,
                                         guththila_token_t * name,
                                         guththila_token_t * uri)
{
  guththila_namespace_t *ns;
  ns = (guththila_namespace_t *) AXIS2_MALLOC (environment->allocator,
					       sizeof
					       (guththila_namespace_t));
  ns->name =
    guththila_token_to_string (environment, name, parser->unicode_state);
  ns->length = AXIS2_STRLEN ( ns->name);
  ns->uri =
    guththila_token_to_string (environment, uri, parser->unicode_state);
  ns->lengthuri = AXIS2_STRLEN ( ns->uri);
  AXIS2_STACK_PUSH (parser->namesp, environment, ns);
/*   guththila_stack_push_namespace (environment, parser->namesp, ns); */
}


AXIS2_EXTERN int
guththila_xml_pull_parser_get_attribute_count (axis2_env_t *
                                               environment,
                                               guththila_xml_pull_parser_t *
                                               parser)
{
  return AXIS2_STACK_SIZE (parser->attrib, environment);
}


AXIS2_EXTERN guththila_attribute_t *
guththila_xml_pull_parser_get_attribute (axis2_env_t *
                                         environment,
                                         guththila_xml_pull_parser_t * parser)
{
  guththila_attribute_t *attribute;
  attribute = (guththila_attribute_t *) AXIS2_STACK_POP (parser->attrib, environment);
  if (attribute)
    return attribute;
  else
    return NULL;
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_attribute_name (axis2_env_t *
                                              environment,
                                              guththila_xml_pull_parser_t *
                                              parser,
                                              guththila_attribute_t * att)
{
  if (!att)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_NULL_ATTRIBUTE_NAME);
  if (att->name->end)
    return guththila_token_to_string (environment, att->name,
				      parser->unicode_state);
  return NULL;
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_attribute_value (axis2_env_t *
                                               environment,
                                               guththila_xml_pull_parser_t *
                                               parser,
                                               guththila_attribute_t * att)
{
  if (!att)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_NULL_ATTRIBUTE_VALUE);
  if (att->value->end)
    return guththila_token_to_string (environment, att->value,
				      parser->unicode_state);
  return NULL;
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_attribute_prefix (axis2_env_t *
                                                environment,
                                                guththila_xml_pull_parser_t *
                                                parser,
                                                guththila_attribute_t * att)
{
  if (!att)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_NULL_ATTRIBUTE_PREFIX);
  if (att->prefix->end)
    return guththila_token_to_string (environment, att->prefix,
				      parser->unicode_state);
  return NULL;
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_attribute_prefix_by_number
(axis2_env_t * environment, guththila_xml_pull_parser_t * parser, int i)
{
 /*   int ix = parser->attrib->pointer; */
  int ix = AXIS2_STACK_SIZE (parser->attrib, environment);
  guththila_attribute_t *attribute;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_REQUESTED_NUMBER_GREATER_THAN_STACK_SIZE);
  else
    {
/*       e = guththila_stack_last (environment, parser->attrib); */
      attribute = (guththila_attribute_t *) AXIS2_STACK_GET_AT (parser->attrib, environment, i);
    }
  return guththila_token_to_string (environment, attribute->prefix,
				    parser->unicode_state);
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_attribute_name_by_number
(axis2_env_t * environment, guththila_xml_pull_parser_t * parser, int i)
{
  /* int ix = parser->attrib->pointer; */
  int ix = AXIS2_STACK_SIZE (parser->attrib, environment);
  guththila_attribute_t *attribute;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_REQUESTED_NUMBER_GREATER_THAN_STACK_SIZE);
  else
    {
/*       e = guththila_stack_last (environment, parser->attrib); */
      attribute = (guththila_attribute_t *) AXIS2_STACK_GET_AT (parser->attrib, environment, i);
    }
  return guththila_token_to_string (environment, attribute->name,
				    parser->unicode_state);
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_attribute_value_by_number
(axis2_env_t * environment, guththila_xml_pull_parser_t * parser, int i)
{
/*   int ix = parser->attrib->pointer; */
  int ix = AXIS2_STACK_SIZE (parser->attrib, environment);
  guththila_attribute_t *attribute;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_REQUESTED_NUMBER_GREATER_THAN_STACK_SIZE);
  else
    {
/*       e = guththila_stack_last (environment, parser->attrib); */
      attribute = (guththila_attribute_t *) AXIS2_STACK_GET_AT (parser->attrib, environment, i);
    }
  return guththila_token_to_string (environment, attribute->value,
				    parser->unicode_state);
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_name (axis2_env_t * environment,
                                    guththila_xml_pull_parser_t * parser)
{
  if (parser->name->end)
    return guththila_token_to_string (environment, parser->name,
				      parser->unicode_state);
  return NULL;
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_value (axis2_env_t * environment,
                                     guththila_xml_pull_parser_t * parser)
{
  if (parser->value->end)
    return guththila_token_to_string (environment, parser->value,
				      parser->unicode_state);
  else
    return NULL;
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_prefix (axis2_env_t * environment,
                                      guththila_xml_pull_parser_t * parser)
{
  if (parser->prefix)
    return guththila_token_to_string (environment, parser->prefix,
				      parser->unicode_state);
  return NULL;
}


AXIS2_EXTERN int
guththila_xml_pull_parser_get_namespace_count (axis2_env_t *
                                               environment,
                                               guththila_xml_pull_parser_t *
                                               parser)
{
  if (parser->namesp)
    return AXIS2_STACK_SIZE (parser->namesp, environment);
  else
    return 0;
}


AXIS2_EXTERN guththila_namespace_t *
guththila_xml_pull_parser_get_namespace (axis2_env_t *
                                         environment,
                                         guththila_xml_pull_parser_t * parser)
{
  guththila_namespace_t *namespace;
/*   e = guththila_stack_pull_current (environment, parser->namesp); */
 namespace = (guththila_namespace_t *) AXIS2_STACK_GET (parser->namesp, environment);
  if (namespace)
    return namespace;
  else
    return NULL;
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_namespace_prefix (axis2_env_t *
                                                environment,
                                                guththila_xml_pull_parser_t *
                                                parser,
                                                guththila_namespace_t * ns)
{
  if (ns)
    return AXIS2_STRDUP (ns->name, environment);
  else
    return NULL;
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_namespace_uri (axis2_env_t *
                                             environment,
                                             guththila_xml_pull_parser_t *
                                             parser,
                                             guththila_namespace_t * ns)
{
  if (ns)
    return AXIS2_STRDUP (ns->uri, environment);
  else
    return NULL;
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_namespace_prefix_by_number
(axis2_env_t * environment, guththila_xml_pull_parser_t * parser, int i)
{
/*   int ix = parser->namesp->pointer; */
  int ix = AXIS2_STACK_SIZE (parser->namesp, environment);
  guththila_namespace_t *ns;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_REQUESTED_NUMBER_GREATER_THAN_STACK_SIZE);
  else
    {
/*       e = guththila_stack_get (environment, parser->namesp, i); */
      ns = (guththila_namespace_t *) AXIS2_STACK_GET_AT (parser->namesp, environment, i);
    }
  return AXIS2_STRDUP (ns->name, environment);
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_namespace_uri_by_number (axis2_env_t
                                                       * environment,
                                                       guththila_xml_pull_parser_t
                                                       * parser, int i)
{
/*   int ix = parser->namesp->pointer; */
  int ix = AXIS2_STACK_SIZE (parser->namesp, environment);
  guththila_namespace_t *ns = NULL;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_ERROR_REQUESTED_NUMBER_GREATER_THAN_STACK_SIZE);
  else
    {
/*       e = guththila_stack_get (environment, parser->namesp, i); */
      ns = (guththila_namespace_t *) AXIS2_STACK_GET_AT (parser->namesp, environment, i);
    }
  return AXIS2_STRDUP (ns->uri, environment);
}


AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_attribute_namespace_by_number
(axis2_env_t * environment, guththila_xml_pull_parser_t * parser, int i)
{
  guththila_char_t *att_prefix;
  int ii;
/*   int ix = parser->namesp->pointer; */
  int ix = AXIS2_STACK_SIZE (parser->namesp, environment);
  guththila_namespace_t *ns = NULL;
  att_prefix =
    guththila_xml_pull_parser_get_attribute_prefix_by_number (environment,
							      parser, i);

  for (ii = 0; ii <= ix; ii++)
    {
/*       e = guththila_stack_get (environment, parser->namesp, ii); */
      ns = (guththila_namespace_t *) AXIS2_STACK_GET_AT (parser->namesp, environment, ii);
      if (ns)
        {
	  if (ns && att_prefix)
            {
	      if (!
		  (AXIS2_STRCMP
		   ( ns->name, att_prefix)))
		return AXIS2_STRDUP (ns->uri, environment);
            }
        }
    }
  return 0;
}

AXIS2_EXTERN guththila_char_t *
guththila_xml_pull_parser_get_encoding(axis2_env_t *environment,
				       guththila_xml_pull_parser_t *parser)
{
  /* parser will find character encoding base on Byte Order Mark
     (BOM). */
  switch (parser->unicode_state)
    {
    case None:
      return "UTF-8";
    case BE:
      return "UTF-16";
    case LE:
      return "UTF-16";
    default:
      return (guththila_char_t *) GUTHTHILA_ERROR_INVALID_ENCODING_DECLARATION;
    };
}
