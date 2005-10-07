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

guththila_xml_pull_parser_t *
guththila_xml_pull_parser_create (guththila_reader_t * r)
{
  guththila_xml_pull_parser_t *parser =
    (guththila_xml_pull_parser_t *) malloc (sizeof (guththila_xml_pull_parser_t));
  parser->buffer = guththila_buffer_create (1024);
  parser->stack = guththila_stack_create ();
  parser->attrib = guththila_stack_create ();
  parser->namesp = guththila_stack_create ();
  parser->dep = guththila_stack_create ();
  parser->name = (guththila_token_t *) malloc (sizeof (guththila_token_t));
  parser->prefix = (guththila_token_t *) malloc (sizeof (guththila_token_t));
  parser->value = (guththila_token_t *) malloc (sizeof (guththila_token_t));
  parser->reader = r;
  parser->_next = 0;
  parser->offset = 0;
  parser->last = -1;
  parser->status = S_1;
  parser->unicode_state = None;
  return parser;
}


void
guththila_xml_pull_parser_free (guththila_xml_pull_parser_t * parser)
{
  if (parser->buffer)
    free ((void *) parser->buffer);
  if (parser->reader)
    free (parser->reader);
  if (parser->stack)
    guththila_stack_free (parser->stack);
  if (parser->attrib)
    guththila_stack_free (parser->attrib);
  if (parser->namesp)
    guththila_stack_free (parser->namesp);
  if (parser->dep)
    guththila_stack_free (parser->dep);
  free ((void *) parser);
}


int
guththila_xml_pull_parser_exception (char *file, int line)
{
  printf ("Throws guththila_xml_pull_parser_exception \t %s \t %d \n", file, line);
  // Samisa: we should define a set of error codes and a corresponding error message set
  // rather than printing the above line to the console, we should be appending the
  // above string to the error message corresponding to the current error code
  // please delete this comment once this is fixed
}


void
guththila_xml_pull_parser_relocate_tokens (guththila_xml_pull_parser_t * parser, int offset)
{
  guththila_element_t *el;
  int isize;
  isize = guththila_stack_size (parser->stack);
  el = guththila_stack_last (parser->stack);
  for (; isize > 0; isize--)
    {
      guththila_token_relocate (el->token, offset);
      el = el->prev;
    }
}


void
guththila_xml_pull_parser_shift (guththila_xml_pull_parser_t * parser)
{
  memmove (parser->buffer->buff, parser->buffer->buff + (parser->offset),
	   (parser->_next) - (parser->offset));
  parser->_next -= parser->offset;
  parser->last -= parser->offset;
  parser->offset = 0;
}


int
guththila_xml_pull_parser_read (guththila_xml_pull_parser_t * parser)
{
  int c = 0;
  if (parser->_next == parser->buffer->size)
    {
      if (parser->offset > 0)
	{
	  guththila_xml_pull_parser_relocate_tokens (parser, parser->offset);
	  guththila_xml_pull_parser_shift (parser);
	}
      else
	{
	  guththila_buffer_t *b;
	  b = parser->buffer;
	  parser->buffer = guththila_buffer_grow (parser->buffer);
	  guththila_xml_pull_parser_relocate_tokens (parser,
					 (b->size - parser->buffer->size));
	}
    }
  c =
    guththila_reader_read ((parser->buffer->buff), (parser->_next),
		 (parser->buffer->size) - (parser->_next), parser->reader);
  parser->last += c;
  return !c;
}


int
guththila_xml_pull_parser_from_utf16 (guththila_xml_pull_parser_t * parser, int eof)
{
  int c = parser->buffer->buff[parser->_next++];
  if (((parser->_next) > (parser->last)) && guththila_xml_pull_parser_read (parser))
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
guththila_xml_pull_parser_next_char(guththila_xml_pull_parser_t * parser, int eof)
{
  if (((parser->_next) > (parser->last)) && guththila_xml_pull_parser_read (parser))
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
    return guththila_xml_pull_parser_from_utf16 (parser, eof);
}


char *
guththila_xml_pull_parser_last_char (guththila_xml_pull_parser_t * parser)
{
  return parser->buffer->buff + (parser->_next - 1);
}


void
guththila_xml_pull_parser_open_token (guththila_xml_pull_parser_t * parser)
{
  guththila_token_t *t = (guththila_token_t *) malloc (sizeof (guththila_token_t));
  t->type = Unknown;
  t->start = guththila_xml_pull_parser_last_char (parser);
  guththila_stack_push (parser->stack, t, NULL);
}


void
guththila_xml_pull_parser_close_token (guththila_xml_pull_parser_t * parser, int t, int refer)
{
  guththila_element_t *e = guththila_stack_last (parser->stack);
  e->token->type = t;
  e->token->ref = refer;
  e->token->end = guththila_xml_pull_parser_last_char (parser) - 1;
}


int
guththila_xml_pull_parser_skip_spaces (guththila_xml_pull_parser_t * parser, int c)
{
  while (0x20 == c || 0x9 == c || 0xD == c || 0xA == c)
    c = guththila_xml_pull_parser_next_char(parser, 0);
  return c;
}


int
guththila_xml_pull_parser_process_eq (guththila_xml_pull_parser_t * parser, int c)
{
  int ic;
  if (0x3D == guththila_xml_pull_parser_skip_spaces (parser, c))
    {
      ic = guththila_xml_pull_parser_next_char(parser, 0);
      return guththila_xml_pull_parser_skip_spaces (parser, ic);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  return 0;
}


int
guththila_xml_pull_parser_process_version_info (guththila_xml_pull_parser_t * parser)
{
  int ic;
  int quote;
  int nc;
  ic = guththila_xml_pull_parser_next_char(parser, 0);
  ic = guththila_xml_pull_parser_skip_spaces (parser, ic);
  if (ic == 'v')
    {
      guththila_xml_pull_parser_open_token (parser);
      if ('e' == guththila_xml_pull_parser_next_char(parser, 0)
	  && 'r' == guththila_xml_pull_parser_next_char(parser, 0)
	  && 's' == guththila_xml_pull_parser_next_char(parser, 0)
	  && 'i' == guththila_xml_pull_parser_next_char(parser, 0)
	  && 'o' == guththila_xml_pull_parser_next_char(parser, 0)
	  && 'n' == guththila_xml_pull_parser_next_char(parser, 0))
	{
	  ic = guththila_xml_pull_parser_next_char(parser, 0);
	  guththila_xml_pull_parser_close_token (parser, _attribute, 0);
	  quote = guththila_xml_pull_parser_process_eq (parser, ic);
	  nc = guththila_xml_pull_parser_next_char(parser, 0);
	  /* 0, since we don't expect EOF line here */
	  guththila_xml_pull_parser_open_token (parser);
	  while (quote != nc)
	    nc = guththila_xml_pull_parser_next_char(parser, 0);
	  guththila_xml_pull_parser_close_token (parser, _attribute_value, 0);
	  nc = guththila_xml_pull_parser_next_char(parser, 0);
	  return guththila_xml_pull_parser_skip_spaces (parser, nc);
	}
      else
	guththila_xml_pull_parser_exception (p_FILE, LINE);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  return 0;
}


int
guththila_xml_pull_parser_process_encoding_decl (guththila_xml_pull_parser_t * parser)
{
  int quote;
  int nc;
  int ic;
  guththila_xml_pull_parser_open_token (parser);
  if ('n' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'c' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'o' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'd' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'i' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'n' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'g' == guththila_xml_pull_parser_next_char(parser, 0))
    {
      ic = guththila_xml_pull_parser_next_char(parser, 0);
      guththila_xml_pull_parser_close_token (parser, _attribute, 0);
      quote = guththila_xml_pull_parser_process_eq (parser, ic);
      nc = guththila_xml_pull_parser_next_char(parser, 0);
      guththila_xml_pull_parser_open_token (parser);
      while (quote != nc)
	nc = guththila_xml_pull_parser_next_char(parser, 0);
      guththila_xml_pull_parser_close_token (parser, _attribute_value, 0);
      nc = guththila_xml_pull_parser_next_char(parser, 0);
      return guththila_xml_pull_parser_skip_spaces (parser, nc);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  return 0;
}


int
guththila_xml_pull_parser_process_sd_decl (guththila_xml_pull_parser_t * parser)
{
  int quote = 0;
  int nc = 0;
  int ic = 0;
  guththila_xml_pull_parser_open_token (parser);
  if ('t' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'a' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'n' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'd' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'a' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'l' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'o' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'n' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'e' == guththila_xml_pull_parser_next_char(parser, 0))
    ic = guththila_xml_pull_parser_next_char(parser, 0);
  guththila_xml_pull_parser_close_token (parser, _attribute, 0);
  quote = guththila_xml_pull_parser_process_eq (parser, ic);
  nc = guththila_xml_pull_parser_next_char(parser, 0);
  guththila_xml_pull_parser_open_token (parser);
  while (quote != nc)
    {
      if ('y' == nc)
	{
	  if ('e' != guththila_xml_pull_parser_next_char(parser, 0)
	      || 's' != guththila_xml_pull_parser_next_char(parser, 0))
	    guththila_xml_pull_parser_exception (p_FILE, LINE);
	  else
	    nc = guththila_xml_pull_parser_next_char(parser, 0);
	}
      else if ('n' == nc)
	{
	  if ('o' != guththila_xml_pull_parser_next_char(parser, 0))
	    guththila_xml_pull_parser_exception (p_FILE, LINE);
	  else
	    nc = guththila_xml_pull_parser_next_char(parser, 0);
	}
      else
	guththila_xml_pull_parser_exception (p_FILE, LINE);
    }
  guththila_xml_pull_parser_close_token (parser, _attribute_value, 0);
  nc = guththila_xml_pull_parser_next_char(parser, 0);
  return guththila_xml_pull_parser_skip_spaces (parser, nc);
}


int
guththila_xml_pull_parser_process_xml_decl (guththila_xml_pull_parser_t * parser)
{
  int ix;
  if ('x' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'm' == guththila_xml_pull_parser_next_char(parser, 0)
      && 'l' == guththila_xml_pull_parser_next_char(parser, 0))
    {
      ix = guththila_xml_pull_parser_process_version_info (parser);

      if ('e' == ix)
	ix = guththila_xml_pull_parser_process_encoding_decl (parser);

      if ('s' == ix)
	ix = guththila_xml_pull_parser_process_sd_decl (parser);

      if ('?' == ix)
	{
	  if ('>' != guththila_xml_pull_parser_next_char(parser, 0))
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
guththila_xml_pull_parser_reset (guththila_xml_pull_parser_t * parser)
{
  parser->offset = parser->_next;
  parser->name = NULL;
  parser->prefix = NULL;
  parser->value = NULL;
  guththila_stack_clear (parser->attrib);
  guththila_stack_clear (parser->stack);
  if (guththila_event == GUTHTHILA_END_ELEMENT || guththila_event == GUTHTHILA_EMPTY_ELEMENT)
    guththila_xml_pull_parser_close_element (parser);
}


int
guththila_xml_pull_parser_is_space (int c)
{
  if (0x20 == c || 0xD == c || 0xA == c || 0x9 == c)
    return 1;
  else
    return 0;
}


int
guththila_xml_pull_parser_is_valid_starting_char (guththila_xml_pull_parser_t * parser, int c)
{
  if (isalpha (c) || '_' == c || ':' == c)
    return 1;
  else
    return 0;
}


int
guththila_xml_pull_parser_process_name (guththila_xml_pull_parser_t * parser)
{
  int c;
  char *x;
  x = guththila_xml_pull_parser_last_char (parser);
  if (!guththila_xml_pull_parser_is_valid_starting_char (parser, x[0]))
    guththila_xml_pull_parser_exception (p_FILE, LINE);

  guththila_xml_pull_parser_open_token (parser);
  c = guththila_xml_pull_parser_next_char(parser, 0);
  while (!
	 (guththila_xml_pull_parser_is_space (c) || '/' == c || '=' == c || '?' == c
	  || '>' == c))
    {
      if (':' == c)
	{
	  guththila_xml_pull_parser_close_token (parser, _prefix, 0);
	  c = guththila_xml_pull_parser_next_char(parser, 0);
	  guththila_xml_pull_parser_open_token (parser);
	}
      else
	c = guththila_xml_pull_parser_next_char(parser, 0);
    }
  guththila_xml_pull_parser_close_token (parser, _name, 0);

  return c;
}


int
guththila_xml_pull_parser_process_attribute_value (guththila_xml_pull_parser_t * parser, int quote)
{
  int c = 0;
  if ('\'' == quote || '\"' == quote)
    {
      c = guththila_xml_pull_parser_next_char(parser, 0);
      guththila_xml_pull_parser_open_token (parser);
      do
	{
	  c = guththila_xml_pull_parser_next_char(parser, 0);
	  if (quote == c)
	    {
	      guththila_xml_pull_parser_close_token (parser, _attribute_value, 0);
	      return guththila_xml_pull_parser_next_char(parser, 0);
	    }
	}
      while ('<' != c || '&' != c);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  return c;
}


int
guththila_xml_pull_parser_process_attribute (guththila_xml_pull_parser_t * parser, int c)
{
  int q;
  q = guththila_xml_pull_parser_skip_spaces (parser, c);
  q = guththila_xml_pull_parser_process_name (parser);
  q = guththila_xml_pull_parser_process_eq (parser, q);
  q = guththila_xml_pull_parser_process_attribute_value (parser, q);
  return q;
}


int
guththila_xml_pull_parser_processSTagOrEmptyElem (guththila_xml_pull_parser_t * parser)
{
  int c;
  c = guththila_xml_pull_parser_process_name (parser);
  c = guththila_xml_pull_parser_skip_spaces (parser, c);
  guththila_event = GUTHTHILA_START_ELEMENT;
  for (;;)
    {
      if ('/' == c)
	{
	  guththila_event = GUTHTHILA_EMPTY_ELEMENT;
	  if ('>' == guththila_xml_pull_parser_next_char(parser, 0))
	    return c;
	  else
	    guththila_xml_pull_parser_exception (p_FILE, LINE);
	}
      else if ('>' == c)
	return c;
      else
	c = guththila_xml_pull_parser_process_attribute (parser, c);
    }
}


int
guththila_xml_pull_parser_process_char_data (guththila_xml_pull_parser_t * parser)
{
  int c;
  int ref = 0;
  guththila_event = GUTHTHILA_CHARACTER;
  guththila_xml_pull_parser_open_token (parser);
  do
    {
      c = guththila_xml_pull_parser_next_char(parser, -1);

      if (c == '&')
	ref = 1;

      if (c == -1)
	{
	  guththila_xml_pull_parser_close_token (parser, _char_data, ref);
	  return 0;
	}
    }
  while (c != '<');
  guththila_xml_pull_parser_close_token (parser, _char_data, ref);
  return c;
}


int
guththila_xml_pull_parser_process_empty_tag (guththila_xml_pull_parser_t * parser)
{
  int c;
  c = guththila_xml_pull_parser_next_char(parser, 0);
  c = guththila_xml_pull_parser_process_name (parser);
  c = guththila_xml_pull_parser_skip_spaces (parser, c);
  if ('>' == c)
    guththila_event = GUTHTHILA_END_ELEMENT;
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  return c;
}


int
guththila_xml_pull_parser_process_pi (guththila_xml_pull_parser_t * parser)
{
  int c;
  guththila_event = GUTHTHILA_COMMENT;
  c = guththila_xml_pull_parser_next_char(parser, 0);
  while ('<' != c)
    c = guththila_xml_pull_parser_next_char(parser, 0);
  return c;
}


int
guththila_xml_pull_parser_process_comment (guththila_xml_pull_parser_t * parser)
{
  int c;
  if ('-' == guththila_xml_pull_parser_next_char(parser, 0)
      && '-' == guththila_xml_pull_parser_next_char(parser, 0))
    {
      c = guththila_xml_pull_parser_next_char(parser, 0);
      while (1)
	{
	  c = guththila_xml_pull_parser_next_char(parser, 0);
	  if ('-' == c)
	    {
	      c = guththila_xml_pull_parser_next_char(parser, 0);
	      if ('-' == c)
		{
		  if ('>' == guththila_xml_pull_parser_next_char(parser, 0))
		    {
		      guththila_event = GUTHTHILA_COMMENT;
		      while ('<' != c)
			{
			  c = guththila_xml_pull_parser_next_char(parser, -1);
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
      c = guththila_xml_pull_parser_next_char(parser, 0);
      while ('<' != c)
	c = guththila_xml_pull_parser_next_char(parser, -1);
    }
  return 0;
}


int
guththila_xml_pull_parser_tokenize (guththila_xml_pull_parser_t * parser)
{
  do
    {
      int c;
      c = guththila_xml_pull_parser_next_char(parser, -1);
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
		guththila_xml_pull_parser_process_xml_decl (parser);
		parser->status = S_3;
	      }
	    else if ('!' == c)
	      {
		guththila_xml_pull_parser_process_comment (parser);
		parser->status = S_4;
	      }
	    else
	      {
		guththila_xml_pull_parser_processSTagOrEmptyElem (parser);
		parser->status = S_3;
	      }
	  }
	  break;
	case S_3:
	  if ('<' == c)
	    parser->status = S_4;
	  else
	    {
	      c = guththila_xml_pull_parser_process_char_data (parser);
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
	      guththila_xml_pull_parser_process_empty_tag (parser);
	      parser->status = S_3;
	    }
	  else if ('?' == c)
	    guththila_xml_pull_parser_process_pi (parser);
	  else if ('!' == c)
	    guththila_xml_pull_parser_process_comment (parser);
	  else
	    {
	      guththila_xml_pull_parser_processSTagOrEmptyElem (parser);
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


int
guththila_xml_pull_parser_next (guththila_xml_pull_parser_t * parser)
{
  int ix;
  guththila_xml_pull_parser_reset (parser);
  ix = guththila_xml_pull_parser_tokenize (parser);
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
	ix = guththila_stack_size (parser->stack);
	for (; ix > 0; ix--)
	  {
	    e = guththila_stack_pull (parser->stack);
	    if (e->token->type == _attribute_value)
	      {
		att_value = e->token;
		e = guththila_stack_pull (parser->stack);
		ix--;
		att_name = e->token;
		guththila_xml_pull_parser_add_attribute (parser, att_name, att_value);
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
	is = guththila_stack_size (parser->stack);
	for (; is > 0 && guththila_stack_size (parser->stack); is--)
	  {
	    e = guththila_stack_pull (parser->stack);
	    /* be careful about memory leaks, when we pull it we get
	       seperate _element need to free it properly */
	    if (e->token->type == _attribute_value)
	      {
		value = e->token;
		e = guththila_stack_pull (parser->stack);
		is--;
		name = e->token;
		if (!guththila_token_compare
		    (e->token, "xmlns", 5, parser->unicode_state))
		  guththila_xml_pull_parser_add_namespace (parser, name, value);
		else
		  guththila_xml_pull_parser_add_attribute (parser, name, value);
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
		    (e->token, "xmlns", 5, parser->unicode_state))
		  {
		    ex = guththila_stack_pull (parser->attrib);
		    guththila_xml_pull_parser_add_namespace (parser, ex->attribute->name,
						 ex->attribute->value);
		  }
		else
		  {
		    if (parser->name)
		      parser->prefix = e->token;
		    else
		      {
			ex = guththila_stack_pull (parser->attrib);
			guththila_xml_pull_parser_add_attribute_with_prefix (parser,
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
	guththila_xml_pull_parser_open_element (parser);
      }
      break;
    case GUTHTHILA_END_ELEMENT:
      {
	guththila_element_t *e;
	e = guththila_stack_pull (parser->stack);
	if (e->token->type == _name)
	  parser->name = e->token;
	e = guththila_stack_pull (parser->stack);
	if (e->token->type == _prefix)
	  parser->prefix = e->token;
      }
      break;
    case GUTHTHILA_CHARACTER:
      {
	guththila_element_t *e;
	e = guththila_stack_pull (parser->stack);
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
guththila_xml_pull_parser_open_element (guththila_xml_pull_parser_t * parser)
{
  int ii;
  guththila_depth_t *m = (guththila_depth_t *) malloc (sizeof (guththila_depth_t));
  guththila_depth_t *l = NULL;
  guththila_element_t *e;
  ii = guththila_stack_size (parser->dep);
  if (!ii)
    {
      m->first = 0;
      m->total = guththila_stack_size (parser->namesp);
      m->count = m->total;
      guththila_stack_push_depth (parser->dep, m);
    }
  else
    {
      e = guththila_stack_pull_current (parser->dep);
      l = e->depth;
      m->first = l->first + l->count;
      m->total = guththila_stack_size (parser->namesp);
      m->count = m->total - l->total;
      guththila_stack_push_depth (parser->dep, m);
    }
}


void
guththila_xml_pull_parser_close_element (guththila_xml_pull_parser_t * parser)
{
  guththila_element_t *d;
  guththila_element_t *e;
  int ii;
  d = guththila_stack_pull (parser->dep);
  ii = d->depth->count;
  for (; ii > 0; ii--)
    {
      e = guththila_stack_pull (parser->namesp);
      if (e->namespace->name)
	free (e->namespace->name);
      if (e->namespace->uri)
	free (e->namespace->uri);
      free (e);
    }

}


void
guththila_xml_pull_parser_add_attribute (guththila_xml_pull_parser_t * parser, guththila_token_t * name,
			     guththila_token_t * value)
{
  guththila_attribute_t *att;
  att = (guththila_attribute_t *) malloc (sizeof (guththila_attribute_t));
  att->name = name;
  att->value = value;
  att->prefix = NULL;
  guththila_stack_push (parser->attrib, NULL, att);
}


void
guththila_xml_pull_parser_add_attribute_with_prefix (guththila_xml_pull_parser_t * parser,
					 guththila_token_t * prefix, guththila_token_t * name,
					 guththila_token_t * value)
{
  guththila_attribute_t *att;
  att = (guththila_attribute_t *) malloc (sizeof (guththila_attribute_t));
  att->name = name;
  att->value = value;
  att->prefix = prefix;
  guththila_stack_push (parser->attrib, NULL, att);
  /* _element can keep , tokens and attributes here token set to null */
}


void
guththila_xml_pull_parser_add_namespace (guththila_xml_pull_parser_t * parser, guththila_token_t * name,
			     guththila_token_t * uri)
{
  guththila_namespace_t *ns;
  ns = (guththila_namespace_t *) malloc (sizeof (guththila_namespace_t));
  ns->name = guththila_token_to_string (name, parser->unicode_state);
  ns->length = strlen (ns->name);
  ns->uri = guththila_token_to_string (uri, parser->unicode_state);
  ns->lengthuri = strlen (ns->uri);
  guththila_stack_push_namespace (parser->namesp, ns);
}


int
guththila_xml_pull_parser_get_attribute_count (guththila_xml_pull_parser_t * parser)
{
  return guththila_stack_size (parser->attrib);
}


guththila_attribute_t *
guththila_xml_pull_parser_get_attribute (guththila_xml_pull_parser_t * parser)
{
  guththila_element_t *e;
  e = guththila_stack_pull (parser->attrib);
  if (e->attribute)
    return e->attribute;
  else
    return NULL;
}


char *
guththila_xml_pull_parser_get_attribute_name (guththila_xml_pull_parser_t * parser, guththila_attribute_t * att)
{
  if (!att)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  if (att->name->end)
    return guththila_token_to_string (att->name, parser->unicode_state);
  return NULL;
}


char *
guththila_xml_pull_parser_get_attribute_value (guththila_xml_pull_parser_t * parser, guththila_attribute_t * att)
{
  if (!att)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  if (att->value->end)
    return guththila_token_to_string (att->value, parser->unicode_state);
  return NULL;
}


char *
guththila_xml_pull_parser_get_attribute_prefix (guththila_xml_pull_parser_t * parser, guththila_attribute_t * att)
{
  if (!att)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  if (att->prefix->end)
    return guththila_token_to_string (att->prefix, parser->unicode_state);
  return NULL;
}


char *
guththila_xml_pull_parser_get_attribute_prefix_by_number (guththila_xml_pull_parser_t * parser, int i)
{
  int ix = parser->attrib->pointer;
  guththila_element_t *e;
  guththila_attribute_t *att = NULL;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  else
    {
      e = guththila_stack_last (parser->attrib);
      for (; ix > i; ix--)
	{
	  if (e->prev)
	    e = e->prev;
	}
      if (e->attribute)
	att = e->attribute;
    }
  return guththila_token_to_string (att->prefix, parser->unicode_state);
}


char *
guththila_xml_pull_parser_get_attribute_name_by_number (guththila_xml_pull_parser_t * parser, int i)
{
  int ix = parser->attrib->pointer;
  guththila_element_t *e;
  guththila_attribute_t *att = NULL;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  else
    {
      e = guththila_stack_last (parser->attrib);
      for (; ix > i; ix--)
	{
	  if (e->prev)
	    e = e->prev;
	}
      if (e->attribute)
	att = e->attribute;
    }
  return guththila_token_to_string (att->name, parser->unicode_state);
}


char *
guththila_xml_pull_parser_get_attribute_value_by_number (guththila_xml_pull_parser_t * parser, int i)
{
  int ix = parser->attrib->pointer;
  guththila_element_t *e;
  guththila_attribute_t *att = NULL;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  else
    {
      e = guththila_stack_last (parser->attrib);
      for (; ix > i; ix--)
	{
	  if (e->prev)
	    e = e->prev;
	}
      if (e->attribute)
	att = e->attribute;
    }
  return guththila_token_to_string (att->value, parser->unicode_state);
}


char *
guththila_xml_pull_parser_get_name (guththila_xml_pull_parser_t * parser)
{
  if (parser->name->end)
    return guththila_token_to_string (parser->name, parser->unicode_state);
  return NULL;
}


char *
guththila_xml_pull_parser_get_value (guththila_xml_pull_parser_t * parser)
{
  if (parser->value->end)
    return guththila_token_to_string (parser->value, parser->unicode_state);
  return NULL;
}


char *
guththila_xml_pull_parser_get_prefix (guththila_xml_pull_parser_t * parser)
{
  if (parser->prefix)
    return guththila_token_to_string (parser->prefix, parser->unicode_state);
  return NULL;
}


int
guththila_xml_pull_parser_get_namespacecount (guththila_xml_pull_parser_t * parser)
{
  if (parser->namesp)
    return guththila_stack_size (parser->namesp);
  else
    return 0;
}


guththila_namespace_t *
guththila_xml_pull_parser_get_namespace (guththila_xml_pull_parser_t * parser)
{
  guththila_element_t *e;
  e = guththila_stack_pull_current (parser->namesp);
  if (e->namespace)
    return e->namespace;
  else
    return NULL;
}


char *
guththila_xml_pull_parser_get_namespace_prefix (guththila_xml_pull_parser_t * parser, guththila_namespace_t * ns)
{
  if (ns)
    return strdup (ns->name);
  else
    return NULL;
}


char *
guththila_xml_pull_parser_get_namespace_uri (guththila_xml_pull_parser_t * parser, guththila_namespace_t * ns)
{
  if (ns)
    return strdup (ns->uri);
  else
    return NULL;
}


char *
guththila_xml_pull_parser_get_namespace_prefix_by_number (guththila_xml_pull_parser_t * parser, int i)
{
  int ix = parser->namesp->pointer;
  guththila_element_t *e;
  guththila_namespace_t *ns = NULL;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  else
    {
      e = guththila_stack_get (parser->namesp, i);
      if (e->namespace)
	ns = e->namespace;
    }
  return strdup (ns->name);
}


char *
guththila_xml_pull_parser_get_namespace_uri_by_number (guththila_xml_pull_parser_t * parser, int i)
{
  int ix = parser->namesp->pointer;
  guththila_element_t *e;
  guththila_namespace_t *ns = NULL;
  if (i > ix)
    guththila_xml_pull_parser_exception (p_FILE, LINE);
  else
    {
      e = guththila_stack_get (parser->namesp, i);
      if (e->namespace)
	ns = e->namespace;
    }
  return strdup (ns->uri);
}


char *
guththila_xml_pull_parser_get_attribute_namespace_by_number (guththila_xml_pull_parser_t *parser, int i)
{
  char *att_prefix;
  int ii;
  int ix = parser->namesp->pointer;
  guththila_element_t *e;
  guththila_namespace_t *ns = NULL;
  att_prefix = guththila_xml_pull_parser_get_attribute_prefix_by_number (parser, i);
    
  for (ii = 0; ii <= ix; ii++)
    {
      e = guththila_stack_get (parser->namesp, ii);
      if (e)
	{
	  if (e->namespace && att_prefix)
	    {
	      ns = e->namespace;
	      if (!(strcmp (ns->name, att_prefix)))
		return strdup (ns->uri);
	    }
	}
    }
  return 0;
}
