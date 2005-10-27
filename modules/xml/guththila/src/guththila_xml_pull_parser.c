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

#include "guththila_xml_pull_parser.h"

static int
guththila_is_space (int c)
{
  return (0x20 == c || 0x9 == c || 0xD == c || 0xA == c);
}


GUTHTHILA_DECLARE(guththila_xml_pull_parser_t *)
guththila_xml_pull_parser_create (guththila_environment_t *environment,guththila_reader_t * r)
{
  guththila_xml_pull_parser_t *parser =
    (guththila_xml_pull_parser_t *) guththila_malloc (environment->allocator,sizeof (guththila_xml_pull_parser_t));
  parser->buffer = guththila_buffer_create (environment,1024);
  parser->stack = guththila_stack_create (environment);
  parser->attrib = guththila_stack_create (environment);
  parser->namesp = guththila_stack_create (environment);
  parser->dep = guththila_stack_create (environment);
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


GUTHTHILA_DECLARE(void)
guththila_xml_pull_parser_free (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  if (parser->buffer)
    guththila_buffer_free (environment,(void *) parser->buffer);
  if (parser->reader)
    guththila_free (environment->allocator,parser->reader);
  if (parser->stack)
    guththila_stack_free (environment,parser->stack);
  if (parser->attrib)
    guththila_stack_free (environment,parser->attrib);
  if (parser->namesp)
    guththila_stack_free (environment,parser->namesp);
  if (parser->dep)
    guththila_stack_free (environment,parser->dep);
  guththila_free (environment->allocator,(void *) parser);
}


int
guththila_xml_pull_parser_exception (guththila_char_t *file, int line)
{
  printf ("Throws guththila_xml_pull_parser_exception \t %s \t %d \n", file, line);
  /* Samisa: we should define a set of error codes and a corresponding error message set
   rather than printing the above line to the console, we should be appending the
   above string to the error message corresponding to the current error code
   please delete this comment once this is fixed
	*/
	return -1;
}


void
guththila_xml_pull_parser_relocate_tokens (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int offset)
{
  guththila_element_t *el;
  int isize;
  isize = guththila_stack_size (environment,parser->stack);
  el = guththila_stack_last (environment,parser->stack);
  for (; isize > 0; isize--)
    {
      guththila_token_relocate (environment,el->token, offset);
      el = el->prev;
    }
}


void
guththila_xml_pull_parser_shift (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  memmove (parser->buffer->buff, parser->buffer->buff + (parser->offset),
	   (parser->_next) - (parser->offset));
  parser->_next -= parser->offset;
  parser->last -= parser->offset;
  parser->offset = 0;
}


GUTHTHILA_DECLARE(int)
guththila_xml_pull_parser_read (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int c = 0;
  if (parser->_next == parser->buffer->size)
    {
      if (parser->offset > 0)
	{
	  guththila_xml_pull_parser_relocate_tokens (environment,parser, parser->offset);
	  guththila_xml_pull_parser_shift (environment,parser);
	}
      else
	{
	  guththila_buffer_t *b;
	  b = parser->buffer;
	  parser->buffer = guththila_buffer_grow (environment,parser->buffer);
	  guththila_xml_pull_parser_relocate_tokens (environment,parser,
					 (b->size - parser->buffer->size));
	}
    }
  c =
    guththila_reader_read (environment,(parser->buffer->buff), (parser->_next),
		 (parser->buffer->size) - (parser->_next), parser->reader);
  parser->last += c;
  return !c;
}


int
guththila_xml_pull_parser_from_utf16 (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int eof)
{
  int c = parser->buffer->buff[parser->_next++];
  if (((parser->_next) > (parser->last)) && guththila_xml_pull_parser_read (environment,parser))
    {
      if (eof)
	return -1;
      else
	guththila_xml_pull_parser_exception (p_FILE, LINE);
    }
  while (!c)
    c = parser->buffer->buff[parser->_next++];
  return c;
}


int
guththila_xml_pull_parser_next_char(guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int eof)
{
  if (((parser->_next) > (parser->last)) && guththila_xml_pull_parser_read (environment,parser))
    {
      if (eof)
	return -1;
      else
	guththila_xml_pull_parser_exception (p_FILE, LINE);
    }
  if (parser->_next == 0)
    {
      guththila_UTF8_char iu = (guththila_UTF8_char) parser->buffer->buff[parser->_next++];

      if (0xff == iu
	  && 0xfe == (guththila_UTF8_char) parser->buffer->buff[parser->_next++])
	{
	  parser->unicode_state = LE;
	}
      else if (0xfe == iu
	       && 0xff == (guththila_UTF8_char) parser->buffer->buff[parser->_next++])
	{
	  parser->unicode_state = BE;
	}
      else if (0xef == iu
	       && 0xbb == (guththila_UTF8_char) parser->buffer->buff[parser->_next++]
	       && 0xbf == (guththila_UTF8_char) parser->buffer->buff[parser->_next++])
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
    return guththila_xml_pull_parser_from_utf16 (environment,parser, eof);
}


guththila_char_t *
guththila_xml_pull_parser_last_char (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  return parser->buffer->buff + (parser->_next - 1);
}


void
guththila_xml_pull_parser_open_token (guththila_environment_t *environment, guththila_xml_pull_parser_t * parser)
{
  guththila_token_t *t = (guththila_token_t *) guththila_malloc(environment->allocator, sizeof (guththila_token_t));
  t->type = Unknown;
  t->start = guththila_xml_pull_parser_last_char (environment, parser);
  guththila_stack_push (environment,parser->stack, t, NULL);
}


void
guththila_xml_pull_parser_close_token (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int t, int refer)
{
  guththila_element_t *e = guththila_stack_last (environment, parser->stack);
  e->token->type = t;
  e->token->ref = refer;
  e->token->end = guththila_xml_pull_parser_last_char (environment, parser) - 1;
}


int
guththila_xml_pull_parser_skip_spaces (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int c)
{
  while (0x20 == c || 0x9 == c || 0xD == c || 0xA == c)
    c = guththila_xml_pull_parser_next_char(environment,parser, 0);
  return c;
}


int
guththila_xml_pull_parser_process_eq (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int c)
{
  int ic;
  if (0x3D == guththila_xml_pull_parser_skip_spaces (environment,parser, c))
    {
      ic = guththila_xml_pull_parser_next_char(environment,parser, 0);
      return guththila_xml_pull_parser_skip_spaces (environment,parser, ic);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  return 0;
}


int
guththila_xml_pull_parser_process_version_info (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int ic;
  int quote;
  int nc;
  ic = guththila_xml_pull_parser_next_char(environment,parser, 0);
  ic = guththila_xml_pull_parser_skip_spaces (environment,parser, ic);
  if (ic == 'v')
    {
      guththila_xml_pull_parser_open_token (environment,parser);
      if ('e' == guththila_xml_pull_parser_next_char(environment,parser, 0)
	  && 'r' == guththila_xml_pull_parser_next_char(environment,parser, 0)
	  && 's' == guththila_xml_pull_parser_next_char(environment,parser, 0)
	  && 'i' == guththila_xml_pull_parser_next_char(environment,parser, 0)
	  && 'o' == guththila_xml_pull_parser_next_char(environment,parser, 0)
	  && 'n' == guththila_xml_pull_parser_next_char(environment,parser, 0))
	{
	  ic = guththila_xml_pull_parser_next_char(environment,parser, 0);
	  guththila_xml_pull_parser_close_token (environment,parser, _attribute, 0);
	  quote = guththila_xml_pull_parser_process_eq (environment,parser, ic);
	  nc = guththila_xml_pull_parser_next_char(environment,parser, 0);
	  /* 0, since we don't expect EOF line here */
	  guththila_xml_pull_parser_open_token (environment,parser);
	  while (quote != nc)
	    nc = guththila_xml_pull_parser_next_char(environment,parser, 0);
	  guththila_xml_pull_parser_close_token (environment,parser, _attribute_value, 0);
	  nc = guththila_xml_pull_parser_next_char(environment,parser, 0);
	  return guththila_xml_pull_parser_skip_spaces (environment,parser, nc);
	}
      else
	guththila_xml_pull_parser_exception (p_FILE, LINE);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  return 0;
}


int
guththila_xml_pull_parser_process_encoding_decl (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int quote;
  int nc;
  int ic;
  guththila_xml_pull_parser_open_token (environment,parser);
  if ('n' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'c' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'o' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'd' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'i' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'n' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'g' == guththila_xml_pull_parser_next_char(environment,parser, 0))
    {
      ic = guththila_xml_pull_parser_next_char(environment,parser, 0);
      guththila_xml_pull_parser_close_token (environment,parser, _attribute, 0);
      quote = guththila_xml_pull_parser_process_eq (environment,parser, ic);
      nc = guththila_xml_pull_parser_next_char(environment,parser, 0);
      guththila_xml_pull_parser_open_token (environment,parser);
      while (quote != nc)
	nc = guththila_xml_pull_parser_next_char(environment,parser, 0);
      guththila_xml_pull_parser_close_token (environment,parser, _attribute_value, 0);
      nc = guththila_xml_pull_parser_next_char(environment,parser, 0);
      return guththila_xml_pull_parser_skip_spaces (environment,parser, nc);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  return 0;
}


int
guththila_xml_pull_parser_process_sd_decl (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int quote = 0;
  int nc = 0;
  int ic = 0;
  guththila_xml_pull_parser_open_token (environment,parser);
  if ('t' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'a' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'n' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'd' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'a' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'l' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'o' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'n' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'e' == guththila_xml_pull_parser_next_char(environment,parser, 0))
    ic = guththila_xml_pull_parser_next_char(environment,parser, 0);
  guththila_xml_pull_parser_close_token (environment,parser, _attribute, 0);
  quote = guththila_xml_pull_parser_process_eq (environment,parser, ic);
  nc = guththila_xml_pull_parser_next_char(environment,parser, 0);
  guththila_xml_pull_parser_open_token (environment,parser);
  while (quote != nc)
    {
      if ('y' == nc)
	{
	  if ('e' != guththila_xml_pull_parser_next_char(environment,parser, 0)
	      || 's' != guththila_xml_pull_parser_next_char(environment,parser, 0))
	    guththila_xml_pull_parser_exception (p_FILE, LINE);
	  else
	    nc = guththila_xml_pull_parser_next_char(environment,parser, 0);
	}
      else if ('n' == nc)
	{
	  if ('o' != guththila_xml_pull_parser_next_char(environment,parser, 0))
	    guththila_xml_pull_parser_exception (p_FILE, LINE);
	  else
	    nc = guththila_xml_pull_parser_next_char(environment,parser, 0);
	}
      else
	guththila_xml_pull_parser_exception (p_FILE, LINE);
    }
  guththila_xml_pull_parser_close_token (environment,parser, _attribute_value, 0);
  nc = guththila_xml_pull_parser_next_char(environment,parser, 0);
  return guththila_xml_pull_parser_skip_spaces (environment,parser, nc);
}


int
guththila_xml_pull_parser_process_xml_decl (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int ix;
  if ('x' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'm' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && 'l' == guththila_xml_pull_parser_next_char(environment,parser, 0))
    {
      ix = guththila_xml_pull_parser_process_version_info (environment,parser);

      if ('e' == ix)
	ix = guththila_xml_pull_parser_process_encoding_decl (environment,parser);

      if ('s' == ix)
	ix = guththila_xml_pull_parser_process_sd_decl (environment,parser);

      if ('?' == ix)
	{
	  if ('>' != guththila_xml_pull_parser_next_char(environment,parser, 0))
	    guththila_xml_pull_parser_exception (p_FILE, LINE);
	  else
	    guththila_event = GUTHTHILA_START_DOCUMENT;
	}
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  return 0;
}


void
guththila_xml_pull_parser_reset (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  parser->offset = parser->_next;
  parser->name = NULL;
  parser->prefix = NULL;
  parser->value = NULL;
  guththila_stack_clear (environment,parser->attrib);
  guththila_stack_clear (environment,parser->stack);
  if (guththila_event == GUTHTHILA_END_ELEMENT || guththila_event == GUTHTHILA_EMPTY_ELEMENT)
    guththila_xml_pull_parser_close_element(environment,parser);
}


int
guththila_xml_pull_parser_is_space (guththila_environment_t *environment,int c)
{
  if (0x20 == c || 0xD == c || 0xA == c || 0x9 == c)
    return 1;
  else
    return 0;
}


int
guththila_xml_pull_parser_is_valid_starting_char (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int c)
{
  if (isalpha (c) || '_' == c || ':' == c)
    return 1;
  else
    return 0;
}


int
guththila_xml_pull_parser_process_name (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int c;
  guththila_char_t *x;
  x = guththila_xml_pull_parser_last_char (environment,parser);
  if (!guththila_xml_pull_parser_is_valid_starting_char (environment,parser, x[0]))
    guththila_xml_pull_parser_exception (p_FILE, LINE);

  guththila_xml_pull_parser_open_token (environment,parser);
  c = guththila_xml_pull_parser_next_char(environment,parser, 0);
  while (!
	 (guththila_xml_pull_parser_is_space (environment,c) || '/' == c || '=' == c || '?' == c
	  || '>' == c))
    {
      if (':' == c)
	{
	  guththila_xml_pull_parser_close_token (environment,parser, _prefix, 0);
	  c = guththila_xml_pull_parser_next_char(environment,parser, 0);
	  guththila_xml_pull_parser_open_token (environment,parser);
	}
      else
	c = guththila_xml_pull_parser_next_char(environment,parser, 0);
    }
  guththila_xml_pull_parser_close_token (environment,parser, _name, 0);

  return c;
}


int
guththila_xml_pull_parser_process_attribute_value (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int quote)
{
  int c = 0;
  if ('\'' == quote || '\"' == quote)
    {
      c = guththila_xml_pull_parser_next_char(environment,parser, 0);
      guththila_xml_pull_parser_open_token (environment,parser);
      do
	{
	  c = guththila_xml_pull_parser_next_char(environment,parser, 0);
	  if (quote == c)
	    {
	      guththila_xml_pull_parser_close_token (environment,parser, _attribute_value, 0);
	      return guththila_xml_pull_parser_next_char(environment,parser, 0);
	    }
	}
      while ('<' != c || '&' != c);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  return c;
}


int
guththila_xml_pull_parser_process_attribute (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int c)
{
  int q;
  q = guththila_xml_pull_parser_skip_spaces (environment,parser, c);
  q = guththila_xml_pull_parser_process_name (environment,parser);
  q = guththila_xml_pull_parser_process_eq (environment,parser, q);
  q = guththila_xml_pull_parser_process_attribute_value (environment,parser, q);
  return q;
}


int
guththila_xml_pull_parser_processSTagOrEmptyElem (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int c;
  c = guththila_xml_pull_parser_process_name (environment,parser);
  c = guththila_xml_pull_parser_skip_spaces (environment,parser, c);
  guththila_event = GUTHTHILA_START_ELEMENT;
  for (;;)
    {
      if ('/' == c)
	{
	  guththila_event = GUTHTHILA_EMPTY_ELEMENT;
	  if ('>' == guththila_xml_pull_parser_next_char(environment,parser, 0))
	    return c;
	  else
	    guththila_xml_pull_parser_exception (p_FILE, LINE);
	}
      else if ('>' == c)
	return c;
      else{
		c = guththila_xml_pull_parser_process_attribute (environment,parser, c);
		c = guththila_xml_pull_parser_skip_spaces (environment,parser, c);
	  }
    }
}


int
guththila_xml_pull_parser_process_char_data (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int c;
  int ref = 0;
  int all_spaces = 1;
/*  int last_event = guththila_event; */
  guththila_event = GUTHTHILA_CHARACTER;
  guththila_xml_pull_parser_open_token (environment,parser);
  do
    {
      c = guththila_xml_pull_parser_next_char(environment,parser, -1);

	  if (c != '<')
		  all_spaces = (all_spaces && guththila_is_space(c));
		
      if (c == '&')
	ref = 1;

      if (c == -1)
	{
	  guththila_xml_pull_parser_close_token (environment,parser, _char_data, ref);
	  return 0;
	}
    }
  while (c != '<');

  if (all_spaces)
	  guththila_event = GUTHTHILA_SPACE;
  guththila_xml_pull_parser_close_token (environment,parser, _char_data, ref);
  return c;
}


int
guththila_xml_pull_parser_process_empty_tag (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int c;
  c = guththila_xml_pull_parser_next_char(environment,parser, 0);
  c = guththila_xml_pull_parser_process_name (environment,parser);
  c = guththila_xml_pull_parser_skip_spaces (environment,parser, c);
  if ('>' == c)
    guththila_event = GUTHTHILA_END_ELEMENT;
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  return c;
}


int
guththila_xml_pull_parser_process_pi (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int c;
  guththila_event = GUTHTHILA_COMMENT;
  c = guththila_xml_pull_parser_next_char(environment,parser, 0);
  while ('<' != c)
    c = guththila_xml_pull_parser_next_char(environment,parser, 0);
  return c;
}


int
guththila_xml_pull_parser_process_comment (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int c;
  if ('-' == guththila_xml_pull_parser_next_char(environment,parser, 0)
      && '-' == guththila_xml_pull_parser_next_char(environment,parser, 0))
    {
      c = guththila_xml_pull_parser_next_char(environment,parser, 0);
      while (1)
	{
	  c = guththila_xml_pull_parser_next_char(environment,parser, 0);
	  if ('-' == c)
	    {
	      c = guththila_xml_pull_parser_next_char(environment,parser, 0);
	      if ('-' == c)
		{
		  if ('>' == guththila_xml_pull_parser_next_char(environment,parser, 0))
		    {
		      guththila_event = GUTHTHILA_COMMENT;
		      while ('<' != c)
			{
			  c = guththila_xml_pull_parser_next_char(environment,parser, -1);
			  if (c == -1)
			    return c;
			}
		      return c;
		    }
		  else
		    guththila_xml_pull_parser_exception (p_FILE, LINE);
		}
	    }
	}
    }
  else
    {
      c = guththila_xml_pull_parser_next_char(environment,parser, 0);
      while ('<' != c)
	c = guththila_xml_pull_parser_next_char(environment,parser, -1);
    }
  return 0;
}


int
guththila_xml_pull_parser_tokenize (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  do
    {
      int c;
      c = guththila_xml_pull_parser_next_char(environment,parser, -1);
      if (c == -1)
	return -1;

      switch (parser->status)
	{
	case S_1:
	  if ('<' == c)
	    parser->status = S_2;
	  else
	    parser->status = S_0;
	  break;
	case S_2:
	  {
	    if ('?' == c)
	      {
		guththila_xml_pull_parser_process_xml_decl (environment,parser);
		parser->status = S_3;
	      }
	    else if ('!' == c)
	      {
		guththila_xml_pull_parser_process_comment (environment,parser);
		parser->status = S_4;
	      }
	    else
	      {
		guththila_xml_pull_parser_processSTagOrEmptyElem (environment,parser);
		parser->status = S_3;
	      }
	  }
	  break;
	case S_3:
	  if ('<' == c)
	    parser->status = S_4;
	  else
	    {
	      c = guththila_xml_pull_parser_process_char_data (environment,parser);
	      if ('<' == c)
		{
		  parser->status = S_4;
		  return guththila_event;
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
	      guththila_xml_pull_parser_process_empty_tag (environment,parser);
	      parser->status = S_3;
	    }
	  else if ('?' == c)
	    guththila_xml_pull_parser_process_pi (environment,parser);
	  else if ('!' == c)
	    guththila_xml_pull_parser_process_comment (environment,parser);
	  else
	    {
	      guththila_xml_pull_parser_processSTagOrEmptyElem (environment,parser);
	      parser->status = S_3;
	    }
	  break;
	case S_0:
	  guththila_xml_pull_parser_exception (p_FILE, LINE);
	}
    }
  while (parser->status != S_3);
  return guththila_event;
}


GUTHTHILA_DECLARE(int)
guththila_xml_pull_parser_next (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int ix;
  guththila_xml_pull_parser_reset (environment,parser);
  ix = guththila_xml_pull_parser_tokenize (environment,parser);
  switch (ix)
    {
    case -1:
      return -1;

    case GUTHTHILA_START_DOCUMENT:
      {
	int ix;
	guththila_element_t *e;
	guththila_token_t *att_name;
	guththila_token_t *att_value;
	ix = guththila_stack_size (environment,parser->stack);
	for (; ix > 0; ix--)
	  {
	    e = guththila_stack_pull (environment,parser->stack);
	    if (e->token->type == _attribute_value)
	      {
		att_value = e->token;
		e = guththila_stack_pull (environment,parser->stack);
		ix--;
		att_name = e->token;
		guththila_xml_pull_parser_add_attribute (environment,parser, att_name, att_value);
	      }
	  }
      }
      break;
    case GUTHTHILA_START_ELEMENT:
    case GUTHTHILA_EMPTY_ELEMENT:
      {
	int is = 0;
	guththila_element_t *e;
	guththila_token_t *name = NULL;
	guththila_token_t *value = NULL;
	is = guththila_stack_size (environment,parser->stack);
	for (; is > 0 && guththila_stack_size (environment,parser->stack); is--)
	  {
	    e = guththila_stack_pull (environment,parser->stack);
	    /* be careful about memory leaks, when we pull it we get
	       seperate _element need to free it properly */
	    if (e->token->type == _attribute_value)
	      {
		value = e->token;
		e = guththila_stack_pull (environment,parser->stack);
		is--;
		name = e->token;
		if (!guththila_token_compare
		    (environment,e->token, "xmlns", 5, parser->unicode_state))
		  guththila_xml_pull_parser_add_namespace (environment,parser, name, value);
		else
		  guththila_xml_pull_parser_add_attribute (environment,parser, name, value);
	      }
	    else if (e->token->type == _name)
	      {
		name = e->token;
		parser->name = name;
		parser->prefix = NULL;

	      }
	    else if (e->token->type == _prefix)
	      {
		guththila_element_t *ex;
		if (!guththila_token_compare
		    (environment,e->token, "xmlns", 5, parser->unicode_state))
		  {
		    ex = guththila_stack_pull (environment,parser->attrib);
		    guththila_xml_pull_parser_add_namespace (environment,parser, ex->attribute->name,
						 ex->attribute->value);
		  }
		else
		  {
		    if (parser->name)
		      parser->prefix = e->token;
		    else
		      {
			ex = guththila_stack_pull (environment,parser->attrib);
			guththila_xml_pull_parser_add_attribute_with_prefix (environment,parser,
								 e->token,
								 ex->
								 attribute->
								 name,
								 ex->
								 attribute->
								 value);
		      }
		  }
	      }
	    else
	      guththila_xml_pull_parser_exception (p_FILE, LINE);
	  }
	guththila_xml_pull_parser_open_element (environment,parser);
      }
      break;
    case GUTHTHILA_END_ELEMENT:
      {
	guththila_element_t *e;
	e = guththila_stack_pull (environment,parser->stack);
	if (e->token->type == _name)
	  parser->name = e->token;
	e = guththila_stack_pull (environment,parser->stack);
	if (e->token->type == _prefix)
	  parser->prefix = e->token;
      }
      break;
    case GUTHTHILA_CHARACTER:
      {
	guththila_element_t *e;
	e = guththila_stack_pull (environment,parser->stack);
	parser->value = e->token;
      }
      break;
    case GUTHTHILA_ENTITY_REFERANCE:
      break;
    case GUTHTHILA_COMMENT:
      break;

    };
  return guththila_event;
}


void
guththila_xml_pull_parser_open_element (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  int ii;
  guththila_depth_t *m = (guththila_depth_t *) guththila_malloc(environment->allocator,sizeof (guththila_depth_t));
  guththila_depth_t *l = NULL;
  guththila_element_t *e;
  ii = guththila_stack_size (environment,parser->dep);
  if (!ii)
    {
      m->first = 0;
      m->total = guththila_stack_size (environment,parser->namesp);
      m->count = m->total;
      guththila_stack_push_depth (environment,parser->dep, m);
    }
  else
    {
      e = guththila_stack_pull_current (environment,parser->dep);
      l = e->depth;
      m->first = l->first + l->count;
      m->total = guththila_stack_size (environment,parser->namesp);
      m->count = m->total - l->total;
      guththila_stack_push_depth (environment,parser->dep, m);
    }
}


void
guththila_xml_pull_parser_close_element (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  guththila_element_t *d;
  guththila_element_t *e;
  int ii;
  d = guththila_stack_pull (environment,parser->dep);
  ii = d->depth->count;
  for (; ii > 0; ii--)
    {
      e = guththila_stack_pull (environment,parser->namesp);
      if (e->namespace->name)
	  {
		free (e->namespace->name);
		  e->namespace->name = NULL;
	  }
      if (e->namespace->uri)
	  {
		free (e->namespace->uri);
		  e->namespace->uri = NULL;
	  }
	  if (e)
	  {
		  free (e);
		  e = NULL;
	  }
    }

}


void
guththila_xml_pull_parser_add_attribute (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, guththila_token_t * name,
			     guththila_token_t * value)
{
  guththila_attribute_t *att;
  att = (guththila_attribute_t *) guththila_malloc (environment->allocator,sizeof (guththila_attribute_t));
  att->name = name;
  att->value = value;
  att->prefix = NULL;
  guththila_stack_push (environment,parser->attrib, NULL, att);
}


void
guththila_xml_pull_parser_add_attribute_with_prefix (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser,
					 guththila_token_t * prefix, guththila_token_t * name,
					 guththila_token_t * value)
{
  guththila_attribute_t *att;
  att = (guththila_attribute_t *) guththila_malloc (environment->allocator,sizeof (guththila_attribute_t));
  att->name = name;
  att->value = value;
  att->prefix = prefix;
  guththila_stack_push (environment,parser->attrib, NULL, att);
  /* _element can keep , tokens and attributes here token set to null */
}


void
guththila_xml_pull_parser_add_namespace (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, guththila_token_t * name,
			     guththila_token_t * uri)
{
  guththila_namespace_t *ns;
  ns = (guththila_namespace_t *) guththila_malloc(environment->allocator,sizeof (guththila_namespace_t));
  ns->name = guththila_token_to_string (environment,name, parser->unicode_state);
  ns->length = guththila_strlen (environment->string,ns->name);
  ns->uri = guththila_token_to_string (environment,uri, parser->unicode_state);
  ns->lengthuri = guththila_strlen (environment->string,ns->uri);
  guththila_stack_push_namespace (environment,parser->namesp, ns);
}


GUTHTHILA_DECLARE(int)
guththila_xml_pull_parser_get_attribute_count (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  return guththila_stack_size (environment,parser->attrib);
}


GUTHTHILA_DECLARE(guththila_attribute_t *)
guththila_xml_pull_parser_get_attribute (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  guththila_element_t *e;
  e = guththila_stack_pull (environment,parser->attrib);
  if (e->attribute)
    return e->attribute;
  else
    return NULL;
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_attribute_name (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, guththila_attribute_t * att)
{
  if (!att)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  if (att->name->end)
    return guththila_token_to_string (environment,att->name, parser->unicode_state);
  return NULL;
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_attribute_value (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, guththila_attribute_t * att)
{
  if (!att)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  if (att->value->end)
    return guththila_token_to_string (environment,att->value, parser->unicode_state);
  return NULL;
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_attribute_prefix (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, guththila_attribute_t * att)
{
  if (!att)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  if (att->prefix->end)
    return guththila_token_to_string (environment,att->prefix, parser->unicode_state);
  return NULL;
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_attribute_prefix_by_number (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int i)
{
  int ix = parser->attrib->pointer;
  guththila_element_t *e;
  guththila_attribute_t *att = NULL;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  else
    {
      e = guththila_stack_last (environment,parser->attrib);
      for (; ix > i; ix--)
	{
	  if (e->prev)
	    e = e->prev;
	}
      if (e->attribute)
	att = e->attribute;
    }
  return guththila_token_to_string (environment,att->prefix, parser->unicode_state);
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_attribute_name_by_number (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int i)
{
  int ix = parser->attrib->pointer;
  guththila_element_t *e;
  guththila_attribute_t *att = NULL;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  else
    {
      e = guththila_stack_last (environment,parser->attrib);
      for (; ix > i; ix--)
	{
	  if (e->prev)
	    e = e->prev;
	}
      if (e->attribute)
	att = e->attribute;
    }
  return guththila_token_to_string (environment,att->name, parser->unicode_state);
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_attribute_value_by_number (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int i)
{
  int ix = parser->attrib->pointer;
  guththila_element_t *e;
  guththila_attribute_t *att = NULL;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  else
    {
      e = guththila_stack_last (environment,parser->attrib);
      for (; ix > i; ix--)
	{
	  if (e->prev)
	    e = e->prev;
	}
      if (e->attribute)
	att = e->attribute;
    }
  return guththila_token_to_string (environment,att->value, parser->unicode_state);
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_name (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  if (parser->name->end)
    return guththila_token_to_string (environment,parser->name, parser->unicode_state);
  return NULL;
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_value (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  if (parser->value->end)
    return guththila_token_to_string (environment,parser->value, parser->unicode_state);
  return NULL;
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_prefix (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  if (parser->prefix)
    return guththila_token_to_string (environment,parser->prefix, parser->unicode_state);
  return NULL;
}


GUTHTHILA_DECLARE(int)
guththila_xml_pull_parser_get_namespace_count (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  if (parser->namesp)
    return guththila_stack_size (environment,parser->namesp);
  else
    return 0;
}


GUTHTHILA_DECLARE(guththila_namespace_t *)
guththila_xml_pull_parser_get_namespace (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser)
{
  guththila_element_t *e;
  e = guththila_stack_pull_current (environment,parser->namesp);
  if (e->namespace)
    return e->namespace;
  else
    return NULL;
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_namespace_prefix (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, guththila_namespace_t * ns)
{
  if (ns)
    return guththila_strdup(environment->string, ns->name);
  else
    return NULL;
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_namespace_uri (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, guththila_namespace_t * ns)
{
  if (ns)
    return guththila_strdup(environment->string, ns->uri);
  else
    return NULL;
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_namespace_prefix_by_number (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int i)
{
  int ix = parser->namesp->pointer;
  guththila_element_t *e;
  guththila_namespace_t *ns = NULL;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  else
    {
      e = guththila_stack_get (environment,parser->namesp, i);
      if (e->namespace)
	ns = e->namespace;
    }
  return guththila_strdup(environment->string,ns->name);
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_namespace_uri_by_number (guththila_environment_t *environment,guththila_xml_pull_parser_t * parser, int i)
{
  int ix = parser->namesp->pointer;
  guththila_element_t *e;
  guththila_namespace_t *ns = NULL;
  if (i > ix)
      guththila_xml_pull_parser_exception (p_FILE, LINE);
  else
    {
      e = guththila_stack_get (environment,parser->namesp, i);
      if (e->namespace)
	ns = e->namespace;
    }
  return guththila_strdup(environment->string,ns->uri);
}


GUTHTHILA_DECLARE(guththila_char_t *)
guththila_xml_pull_parser_get_attribute_namespace_by_number (guththila_environment_t *environment,guththila_xml_pull_parser_t *parser, int i)
{
  guththila_char_t *att_prefix;
  int ii;
  int ix = parser->namesp->pointer;
  guththila_element_t *e;
  guththila_namespace_t *ns = NULL;
  att_prefix = guththila_xml_pull_parser_get_attribute_prefix_by_number (environment,parser, i);
    
  for (ii = 0; ii <= ix; ii++)
    {
      e = guththila_stack_get (environment,parser->namesp, ii);
      if (e)
	{
	  if (e->namespace && att_prefix)
	    {
	      ns = e->namespace;
	      if (!(guththila_strcmp (environment->string,ns->name, att_prefix)))
		return guththila_strdup(environment->string,ns->uri);
	    }
	}
    }
  return 0;
}
