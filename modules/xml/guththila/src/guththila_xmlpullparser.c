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

#include "guththila_xmlpullparser.h"

XML_PullParser *
XML_PullParser_createPullParser (READER * r)
{
  XML_PullParser *parser =
    (XML_PullParser *) malloc (sizeof (XML_PullParser));
  parser->buffer = Buffer_createBuffer (1024);
  parser->stack = Stack_createStack ();
  parser->attrib = Stack_createStack ();
  parser->namesp = Stack_createStack ();
  parser->dep = Stack_createStack ();
  parser->name = (TOKEN *) malloc (sizeof (TOKEN));
  parser->prefix = (TOKEN *) malloc (sizeof (TOKEN));
  parser->value = (TOKEN *) malloc (sizeof (TOKEN));
  parser->reader = r;
  parser->_next = 0;
  parser->offset = 0;
  parser->last = -1;
  parser->STATUS = S_1;
  parser->unicode_state = None;
  return parser;
}


void
XML_PullParser_freePullParser (XML_PullParser * parser)
{
  if (parser->buffer)
    free ((void *) parser->buffer);
  if (parser->reader)
    free (parser->reader);
  if (parser->stack)
    Stack_free (parser->stack);
  if (parser->attrib)
    Stack_free (parser->attrib);
  if (parser->namesp)
    Stack_free (parser->namesp);
  if (parser->dep)
    Stack_free (parser->dep);
  free ((void *) parser);
}


int
XML_PullParser_Exception (char *file, int line)
{
  printf ("Throws XML_PullParser_Exception \t %s \t %d \n", file, line);
  exit (0);
}


void
XML_PullParser_relocateTokens (XML_PullParser * parser, int offset)
{
  ELEMENT *el;
  int isize;
  isize = Stack_size (parser->stack);
  el = Stack_last (parser->stack);
  for (; isize > 0; isize--)
    {
      Token_relocate (el->token, offset);
      el = el->prev;
    }
}


void
XML_PullParser_shift (XML_PullParser * parser)
{
  memmove (parser->buffer->buff, parser->buffer->buff + (parser->offset),
	   (parser->_next) - (parser->offset));
  parser->_next -= parser->offset;
  parser->last -= parser->offset;
  parser->offset = 0;
}


int
XML_PullParser_read (XML_PullParser * parser)
{
  int c = 0;
  if (parser->_next == parser->buffer->size)
    {
      if (parser->offset > 0)
	{
	  XML_PullParser_relocateTokens (parser, parser->offset);
	  XML_PullParser_shift (parser);
	}
      else
	{
	  BUFFER *b;
	  b = parser->buffer;
	  parser->buffer = Buffer_grow (parser->buffer);
	  XML_PullParser_relocateTokens (parser,
					 (b->size - parser->buffer->size));
	}
    }
  c =
    Reader_read ((parser->buffer->buff), (parser->_next),
		 (parser->buffer->size) - (parser->_next), parser->reader);
  parser->last += c;
  return !c;
}


int
XML_PullParser_from_utf16 (XML_PullParser * parser, int eof)
{
  int c = parser->buffer->buff[parser->_next++];
  if (((parser->_next) > (parser->last)) && XML_PullParser_read (parser))
    {
      if (eof)
	return -1;
      else
	XML_PullParser_Exception (p_FILE, LINE);
    }
  while (!c)
    c = parser->buffer->buff[parser->_next++];
  return c;
}


int
XML_PullParser_nextChar (XML_PullParser * parser, int eof)
{
  if (((parser->_next) > (parser->last)) && XML_PullParser_read (parser))
    {
      if (eof)
	return -1;
      else
	XML_PullParser_Exception (p_FILE, LINE);
    }
  if (parser->_next == 0)
    {
      UTF8_char iu = (UTF8_char) parser->buffer->buff[parser->_next++];

      if (0xff == iu
	  && 0xfe == (UTF8_char) parser->buffer->buff[parser->_next++])
	{
	  parser->unicode_state = LE;
	}
      else if (0xfe == iu
	       && 0xff == (UTF8_char) parser->buffer->buff[parser->_next++])
	{
	  parser->unicode_state = BE;
	}
      else if (0xef == iu
	       && 0xbb == (UTF8_char) parser->buffer->buff[parser->_next++]
	       && 0xbf == (UTF8_char) parser->buffer->buff[parser->_next++])
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
    return XML_PullParser_from_utf16 (parser, eof);
}


char *
XML_PullParser_lastChar (XML_PullParser * parser)
{
  return parser->buffer->buff + (parser->_next - 1);
}


void
XML_PullParser_openToken (XML_PullParser * parser)
{
  TOKEN *t = (TOKEN *) malloc (sizeof (TOKEN));
  t->type = Unknown;
  t->start = XML_PullParser_lastChar (parser);
  Stack_push (parser->stack, t, NULL);
}


void
XML_PullParser_closeToken (XML_PullParser * parser, int t, int refer)
{
  ELEMENT *e = Stack_last (parser->stack);
  e->token->type = t;
  e->token->ref = refer;
  e->token->end = XML_PullParser_lastChar (parser) - 1;
}


int
XML_PullParser_skipS (XML_PullParser * parser, int c)
{
  while (0x20 == c || 0x9 == c || 0xD == c || 0xA == c)
    c = XML_PullParser_nextChar (parser, 0);
  return c;
}


int
XML_PullParser_processEq (XML_PullParser * parser, int c)
{
  int ic;
  if (0x3D == XML_PullParser_skipS (parser, c))
    {
      ic = XML_PullParser_nextChar (parser, 0);
      return XML_PullParser_skipS (parser, ic);
    }
  else
    XML_PullParser_Exception (p_FILE, LINE);
  return 0;
}


int
XML_PullParser_processVersionInfo (XML_PullParser * parser)
{
  int ic;
  int quote;
  int nc;
  ic = XML_PullParser_nextChar (parser, 0);
  ic = XML_PullParser_skipS (parser, ic);
  if (ic == 'v')
    {
      XML_PullParser_openToken (parser);
      if ('e' == XML_PullParser_nextChar (parser, 0)
	  && 'r' == XML_PullParser_nextChar (parser, 0)
	  && 's' == XML_PullParser_nextChar (parser, 0)
	  && 'i' == XML_PullParser_nextChar (parser, 0)
	  && 'o' == XML_PullParser_nextChar (parser, 0)
	  && 'n' == XML_PullParser_nextChar (parser, 0))
	{
	  ic = XML_PullParser_nextChar (parser, 0);
	  XML_PullParser_closeToken (parser, Attribute, 0);
	  quote = XML_PullParser_processEq (parser, ic);
	  nc = XML_PullParser_nextChar (parser, 0);
	  /* 0, since we don't expect EOF line here */
	  XML_PullParser_openToken (parser);
	  while (quote != nc)
	    nc = XML_PullParser_nextChar (parser, 0);
	  XML_PullParser_closeToken (parser, AttValue, 0);
	  nc = XML_PullParser_nextChar (parser, 0);
	  return XML_PullParser_skipS (parser, nc);
	}
      else
	XML_PullParser_Exception (p_FILE, LINE);
    }
  else
    XML_PullParser_Exception (p_FILE, LINE);
  return 0;
}


int
XML_PullParser_processEncodingDecl (XML_PullParser * parser)
{
  int quote;
  int nc;
  int ic;
  XML_PullParser_openToken (parser);
  if ('n' == XML_PullParser_nextChar (parser, 0)
      && 'c' == XML_PullParser_nextChar (parser, 0)
      && 'o' == XML_PullParser_nextChar (parser, 0)
      && 'd' == XML_PullParser_nextChar (parser, 0)
      && 'i' == XML_PullParser_nextChar (parser, 0)
      && 'n' == XML_PullParser_nextChar (parser, 0)
      && 'g' == XML_PullParser_nextChar (parser, 0))
    {
      ic = XML_PullParser_nextChar (parser, 0);
      XML_PullParser_closeToken (parser, Attribute, 0);
      quote = XML_PullParser_processEq (parser, ic);
      nc = XML_PullParser_nextChar (parser, 0);
      XML_PullParser_openToken (parser);
      while (quote != nc)
	nc = XML_PullParser_nextChar (parser, 0);
      XML_PullParser_closeToken (parser, AttValue, 0);
      nc = XML_PullParser_nextChar (parser, 0);
      return XML_PullParser_skipS (parser, nc);
    }
  else
    XML_PullParser_Exception (p_FILE, LINE);
  return 0;
}


int
XML_PullParser_processSDDecl (XML_PullParser * parser)
{
  int quote = 0;
  int nc = 0;
  int ic = 0;
  XML_PullParser_openToken (parser);
  if ('t' == XML_PullParser_nextChar (parser, 0)
      && 'a' == XML_PullParser_nextChar (parser, 0)
      && 'n' == XML_PullParser_nextChar (parser, 0)
      && 'd' == XML_PullParser_nextChar (parser, 0)
      && 'a' == XML_PullParser_nextChar (parser, 0)
      && 'l' == XML_PullParser_nextChar (parser, 0)
      && 'o' == XML_PullParser_nextChar (parser, 0)
      && 'n' == XML_PullParser_nextChar (parser, 0)
      && 'e' == XML_PullParser_nextChar (parser, 0))
    ic = XML_PullParser_nextChar (parser, 0);
  XML_PullParser_closeToken (parser, Attribute, 0);
  quote = XML_PullParser_processEq (parser, ic);
  nc = XML_PullParser_nextChar (parser, 0);
  XML_PullParser_openToken (parser);
  while (quote != nc)
    {
      if ('y' == nc)
	{
	  if ('e' != XML_PullParser_nextChar (parser, 0)
	      || 's' != XML_PullParser_nextChar (parser, 0))
	    XML_PullParser_Exception (p_FILE, LINE);
	  else
	    nc = XML_PullParser_nextChar (parser, 0);
	}
      else if ('n' == nc)
	{
	  if ('o' != XML_PullParser_nextChar (parser, 0))
	    XML_PullParser_Exception (p_FILE, LINE);
	  else
	    nc = XML_PullParser_nextChar (parser, 0);
	}
      else
	XML_PullParser_Exception (p_FILE, LINE);
    }
  XML_PullParser_closeToken (parser, AttValue, 0);
  nc = XML_PullParser_nextChar (parser, 0);
  return XML_PullParser_skipS (parser, nc);
}


int
XML_PullParser_processXmlDecl (XML_PullParser * parser)
{
  int ix;
  if ('x' == XML_PullParser_nextChar (parser, 0)
      && 'm' == XML_PullParser_nextChar (parser, 0)
      && 'l' == XML_PullParser_nextChar (parser, 0))
    {
      ix = XML_PullParser_processVersionInfo (parser);

      if ('e' == ix)
	ix = XML_PullParser_processEncodingDecl (parser);

      if ('s' == ix)
	ix = XML_PullParser_processSDDecl (parser);

      if ('?' == ix)
	{
	  if ('>' != XML_PullParser_nextChar (parser, 0))
	    XML_PullParser_Exception (p_FILE, LINE);
	  else
	    EVENT = START_DOCUMENT;
	}
    }
  else
    XML_PullParser_Exception (p_FILE, LINE);
  return 0;
}


void
XML_PullParser_reset (XML_PullParser * parser)
{
  parser->offset = parser->_next;
  parser->name = NULL;
  parser->prefix = NULL;
  parser->value = NULL;
  Stack_clear (parser->attrib);
  Stack_clear (parser->stack);
  if (EVENT == END_ELEMENT || EVENT == EMPTY_ELEMENT)
    XML_PullParser_closeElement (parser);
}


int
XML_PullParser_isS (int c)
{
  if (0x20 == c || 0xD == c || 0xA == c || 0x9 == c)
    return 1;
  else
    return 0;
}


int
XML_PullParser_is_valid_starting_char (XML_PullParser * parser, int c)
{
  if (isalpha (c) || '_' == c || ':' == c)
    return 1;
  else
    return 0;
}


int
XML_PullParser_processName (XML_PullParser * parser)
{
  int c;
  char *x;
  x = XML_PullParser_lastChar (parser);
  if (!XML_PullParser_is_valid_starting_char (parser, x[0]))
    XML_PullParser_Exception (p_FILE, LINE);

  XML_PullParser_openToken (parser);
  c = XML_PullParser_nextChar (parser, 0);
  while (!
	 (XML_PullParser_isS (c) || '/' == c || '=' == c || '?' == c
	  || '>' == c))
    {
      if (':' == c)
	{
	  XML_PullParser_closeToken (parser, Prefix, 0);
	  c = XML_PullParser_nextChar (parser, 0);
	  XML_PullParser_openToken (parser);
	}
      else
	c = XML_PullParser_nextChar (parser, 0);
    }
  XML_PullParser_closeToken (parser, Name, 0);

  return c;
}


int
XML_PullParser_processAttValue (XML_PullParser * parser, int quote)
{
  int c = 0;
  if ('\'' == quote || '\"' == quote)
    {
      c = XML_PullParser_nextChar (parser, 0);
      XML_PullParser_openToken (parser);
      do
	{
	  c = XML_PullParser_nextChar (parser, 0);
	  if (quote == c)
	    {
	      XML_PullParser_closeToken (parser, AttValue, 0);
	      return XML_PullParser_nextChar (parser, 0);
	    }
	}
      while ('<' != c || '&' != c);
    }
  else
    XML_PullParser_Exception (p_FILE, LINE);
  return c;
}


int
XML_PullParser_processAttribute (XML_PullParser * parser, int c)
{
  int q;
  q = XML_PullParser_skipS (parser, c);
  q = XML_PullParser_processName (parser);
  q = XML_PullParser_processEq (parser, q);
  q = XML_PullParser_processAttValue (parser, q);
  return q;
}


int
XML_PullParser_processSTagOrEmptyElem (XML_PullParser * parser)
{
  int c;
  c = XML_PullParser_processName (parser);
  c = XML_PullParser_skipS (parser, c);
  EVENT = START_ELEMENT;
  for (;;)
    {
      if ('/' == c)
	{
	  EVENT = EMPTY_ELEMENT;
	  if ('>' == XML_PullParser_nextChar (parser, 0))
	    return c;
	  else
	    XML_PullParser_Exception (p_FILE, LINE);
	}
      else if ('>' == c)
	return c;
      else
	c = XML_PullParser_processAttribute (parser, c);
    }
}


int
XML_PullParser_processCharData (XML_PullParser * parser)
{
  int c;
  int ref = 0;
  EVENT = CHARACTER;
  XML_PullParser_openToken (parser);
  do
    {
      c = XML_PullParser_nextChar (parser, -1);

      if (c == '&')
	ref = 1;

      if (c == -1)
	{
	  XML_PullParser_closeToken (parser, CharData, ref);
	  return 0;
	}
    }
  while (c != '<');
  XML_PullParser_closeToken (parser, CharData, ref);
  return c;
}


int
XML_PullParser_processETag (XML_PullParser * parser)
{
  int c;
  c = XML_PullParser_nextChar (parser, 0);
  c = XML_PullParser_processName (parser);
  c = XML_PullParser_skipS (parser, c);
  if ('>' == c)
    EVENT = END_ELEMENT;
  else
    XML_PullParser_Exception (p_FILE, LINE);
  return c;
}


int
XML_PullParser_processPI (XML_PullParser * parser)
{
  int c;
  EVENT = COMMENT;
  c = XML_PullParser_nextChar (parser, 0);
  while ('<' != c)
    c = XML_PullParser_nextChar (parser, 0);
  return c;
}


int
XML_PullParser_processComment (XML_PullParser * parser)
{
  int c;
  if ('-' == XML_PullParser_nextChar (parser, 0)
      && '-' == XML_PullParser_nextChar (parser, 0))
    {
      c = XML_PullParser_nextChar (parser, 0);
      while (1)
	{
	  c = XML_PullParser_nextChar (parser, 0);
	  if ('-' == c)
	    {
	      c = XML_PullParser_nextChar (parser, 0);
	      if ('-' == c)
		{
		  if ('>' == XML_PullParser_nextChar (parser, 0))
		    {
		      EVENT = COMMENT;
		      while ('<' != c)
			{
			  c = XML_PullParser_nextChar (parser, -1);
			  if (c == -1)
			    return c;
			}
		      return c;
		    }
		  else
		    XML_PullParser_Exception (p_FILE, LINE);
		}
	    }
	}
    }
  else
    {
      c = XML_PullParser_nextChar (parser, 0);
      while ('<' != c)
	c = XML_PullParser_nextChar (parser, -1);
    }
  return 0;
}


int
XML_PullParser_tokenize (XML_PullParser * parser)
{
  do
    {
      int c;
      c = XML_PullParser_nextChar (parser, -1);
      if (c == -1)
	return -1;

      switch (parser->STATUS)
	{
	case S_1:
	  if ('<' == c)
	    parser->STATUS = S_2;
	  else
	    parser->STATUS = S_0;
	  break;
	case S_2:
	  {
	    if ('?' == c)
	      {
		XML_PullParser_processXmlDecl (parser);
		parser->STATUS = S_3;
	      }
	    else if ('!' == c)
	      {
		XML_PullParser_processComment (parser);
		parser->STATUS = S_4;
	      }
	    else
	      {
		XML_PullParser_processSTagOrEmptyElem (parser);
		parser->STATUS = S_3;
	      }
	  }
	  break;
	case S_3:
	  if ('<' == c)
	    parser->STATUS = S_4;
	  else
	    {
	      c = XML_PullParser_processCharData (parser);
	      if ('<' == c)
		{
		  parser->STATUS = S_4;
		  return EVENT;
		}
	      else if ('\0' == c)
		parser->STATUS = S_3;
	      else
		parser->STATUS = S_0;
	    }
	  break;
	case S_4:
	  if ('/' == c)
	    {
	      XML_PullParser_processETag (parser);
	      parser->STATUS = S_3;
	    }
	  else if ('?' == c)
	    XML_PullParser_processPI (parser);
	  else if ('!' == c)
	    XML_PullParser_processComment (parser);
	  else
	    {
	      XML_PullParser_processSTagOrEmptyElem (parser);
	      parser->STATUS = S_3;
	    }
	  break;
	case S_0:
	  XML_PullParser_Exception (p_FILE, LINE);
	}
    }
  while (parser->STATUS != S_3);
  return EVENT;
}


int
XML_PullParser_next (XML_PullParser * parser)
{
  int ix;
  XML_PullParser_reset (parser);
  ix = XML_PullParser_tokenize (parser);
  switch (ix)
    {
    case -1:
      return -1;

    case START_DOCUMENT:
      {
	int ix;
	ELEMENT *e;
	TOKEN *att_name;
	TOKEN *att_value;
	ix = Stack_size (parser->stack);
	for (; ix > 0; ix--)
	  {
	    e = Stack_pull (parser->stack);
	    if (e->token->type == AttValue)
	      {
		att_value = e->token;
		e = Stack_pull (parser->stack);
		ix--;
		att_name = e->token;
		XML_PullParser_addAttribute (parser, att_name, att_value);
	      }
	  }
      }
      break;
    case START_ELEMENT:
    case EMPTY_ELEMENT:
      {
	int is = 0;
	ELEMENT *e;
	TOKEN *name = NULL;
	TOKEN *value = NULL;
	is = Stack_size (parser->stack);
	for (; is > 0 && Stack_size (parser->stack); is--)
	  {
	    e = Stack_pull (parser->stack);
	    /* be careful about memory leaks, when we pull it we get
	       seperate Element need to free it properly */
	    if (e->token->type == AttValue)
	      {
		value = e->token;
		e = Stack_pull (parser->stack);
		is--;
		name = e->token;
		if (!Token_compare
		    (e->token, "xmlns", 5, parser->unicode_state))
		  XML_PullParser_addNamespace (parser, name, value);
		else
		  XML_PullParser_addAttribute (parser, name, value);
	      }
	    else if (e->token->type == Name)
	      {
		name = e->token;
		parser->name = name;
		parser->prefix = NULL;

	      }
	    else if (e->token->type == Prefix)
	      {
		ELEMENT *ex;
		if (!Token_compare
		    (e->token, "xmlns", 5, parser->unicode_state))
		  {
		    ex = Stack_pull (parser->attrib);
		    XML_PullParser_addNamespace (parser, ex->attribute->name,
						 ex->attribute->value);
		  }
		else
		  {
		    if (parser->name)
		      parser->prefix = e->token;
		    else
		      {
			ex = Stack_pull (parser->attrib);
			XML_PullParser_addAttribute_with_prefix (parser,
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
	      XML_PullParser_Exception (p_FILE, LINE);
	  }
	XML_PullParser_openElement (parser);
      }
      break;
    case END_ELEMENT:
      {
	ELEMENT *e;
	e = Stack_pull (parser->stack);
	if (e->token->type == Name)
	  parser->name = e->token;
	e = Stack_pull (parser->stack);
	if (e->token->type == Prefix)
	  parser->prefix = e->token;
      }
      break;
    case CHARACTER:
      {
	ELEMENT *e;
	e = Stack_pull (parser->stack);
	parser->value = e->token;
      }
      break;
    case ENTITY_REFERANCE:
      break;
    case COMMENT:
      break;

    };
  return EVENT;
}


void
XML_PullParser_openElement (XML_PullParser * parser)
{
  int ii;
  DEPTH *m = (DEPTH *) malloc (sizeof (DEPTH));
  DEPTH *l = NULL;
  ELEMENT *e;
  ii = Stack_size (parser->dep);
  if (!ii)
    {
      m->first = 0;
      m->total = Stack_size (parser->namesp);
      m->count = m->total;
      Stack_push_depth (parser->dep, m);
    }
  else
    {
      e = Stack_pull_current (parser->dep);
      l = e->depth;
      m->first = l->first + l->count;
      m->total = Stack_size (parser->namesp);
      m->count = m->total - l->total;
      Stack_push_depth (parser->dep, m);
    }
}


void
XML_PullParser_closeElement (XML_PullParser * parser)
{
  ELEMENT *d;
  ELEMENT *e;
  int ii;
  d = Stack_pull (parser->dep);
  ii = d->depth->count;
  for (; ii > 0; ii--)
    {
      e = Stack_pull (parser->namesp);
      if (e->namespace->name)
	free (e->namespace->name);
      if (e->namespace->uri)
	free (e->namespace->uri);
      free (e);
    }

}


void
XML_PullParser_addAttribute (XML_PullParser * parser, TOKEN * name,
			     TOKEN * value)
{
  ATTRIBUTE *att;
  att = (ATTRIBUTE *) malloc (sizeof (ATTRIBUTE));
  att->name = name;
  att->value = value;
  att->prefix = NULL;
  Stack_push (parser->attrib, NULL, att);
}


void
XML_PullParser_addAttribute_with_prefix (XML_PullParser * parser,
					 TOKEN * prefix, TOKEN * name,
					 TOKEN * value)
{
  ATTRIBUTE *att;
  att = (ATTRIBUTE *) malloc (sizeof (ATTRIBUTE));
  att->name = name;
  att->value = value;
  att->prefix = prefix;
  Stack_push (parser->attrib, NULL, att);
  /* Element can keep , tokens and attributes here token set to null */
}


void
XML_PullParser_addNamespace (XML_PullParser * parser, TOKEN * name,
			     TOKEN * uri)
{
  NAMESPACE *ns;
  ns = (NAMESPACE *) malloc (sizeof (NAMESPACE));
  ns->name = Token_toString (name, parser->unicode_state);
  ns->length = strlen (ns->name);
  ns->uri = Token_toString (uri, parser->unicode_state);
  ns->lengthuri = strlen (ns->uri);
  Stack_push_namespace (parser->namesp, ns);
}


int
XML_PullParser_getAttributeCount (XML_PullParser * parser)
{
  return Stack_size (parser->attrib);
}


ATTRIBUTE *
XML_PullParser_getAttribute (XML_PullParser * parser)
{
  ELEMENT *e;
  e = Stack_pull (parser->attrib);
  if (e->attribute)
    return e->attribute;
  else
    return NULL;
}


char *
XML_PullParser_getAttributeName (XML_PullParser * parser, ATTRIBUTE * att)
{
  if (!att)
    XML_PullParser_Exception (p_FILE, LINE);
  if (att->name->end)
    return Token_toString (att->name, parser->unicode_state);
  return NULL;
}


char *
XML_PullParser_getAttributeValue (XML_PullParser * parser, ATTRIBUTE * att)
{
  if (!att)
    XML_PullParser_Exception (p_FILE, LINE);
  if (att->value->end)
    return Token_toString (att->value, parser->unicode_state);
  return NULL;
}


char *
XML_PullParser_getAttributePrefix (XML_PullParser * parser, ATTRIBUTE * att)
{
  if (!att)
    XML_PullParser_Exception (p_FILE, LINE);
  if (att->prefix->end)
    return Token_toString (att->prefix, parser->unicode_state);
  return NULL;
}


char *
XML_PullParser_getAttributePrefix_by_number (XML_PullParser * parser, int i)
{
  int ix = parser->attrib->pointer;
  ELEMENT *e;
  ATTRIBUTE *att = NULL;
  if (i > ix)
    XML_PullParser_Exception (p_FILE, LINE);
  else
    {
      e = Stack_last (parser->attrib);
      for (; ix > i; ix--)
	{
	  if (e->prev)
	    e = e->prev;
	}
      if (e->attribute)
	att = e->attribute;
    }
  return Token_toString (att->prefix, parser->unicode_state);
}


char *
XML_PullParser_getAttributeName_by_number (XML_PullParser * parser, int i)
{
  int ix = parser->attrib->pointer;
  ELEMENT *e;
  ATTRIBUTE *att = NULL;
  if (i > ix)
    XML_PullParser_Exception (p_FILE, LINE);
  else
    {
      e = Stack_last (parser->attrib);
      for (; ix > i; ix--)
	{
	  if (e->prev)
	    e = e->prev;
	}
      if (e->attribute)
	att = e->attribute;
    }
  return Token_toString (att->name, parser->unicode_state);
}


char *
XML_PullParser_getAttributeValue_by_number (XML_PullParser * parser, int i)
{
  int ix = parser->attrib->pointer;
  ELEMENT *e;
  ATTRIBUTE *att = NULL;
  if (i > ix)
    XML_PullParser_Exception (p_FILE, LINE);
  else
    {
      e = Stack_last (parser->attrib);
      for (; ix > i; ix--)
	{
	  if (e->prev)
	    e = e->prev;
	}
      if (e->attribute)
	att = e->attribute;
    }
  return Token_toString (att->value, parser->unicode_state);
}


char *
XML_PullParser_getName (XML_PullParser * parser)
{
  if (parser->name->end)
    return Token_toString (parser->name, parser->unicode_state);
  return NULL;
}


char *
XML_PullParser_getValue (XML_PullParser * parser)
{
  if (parser->value->end)
    return Token_toString (parser->value, parser->unicode_state);
  return NULL;
}


char *
XML_PullParser_getPrefix (XML_PullParser * parser)
{
  if (parser->prefix)
    return Token_toString (parser->prefix, parser->unicode_state);
  return NULL;
}


int
XML_PullParser_getNamespaceCount (XML_PullParser * parser)
{
  if (parser->namesp)
    return Stack_size (parser->namesp);
  else
    return 0;
}


NAMESPACE *
XML_PullParser_getNamespace (XML_PullParser * parser)
{
  ELEMENT *e;
  e = Stack_pull_current (parser->namesp);
  if (e->namespace)
    return e->namespace;
  else
    return NULL;
}


char *
XML_PullParser_getNamespacePrefix (XML_PullParser * parser, NAMESPACE * ns)
{
  if (ns)
    return strdup (ns->name);
  else
    return NULL;
}


char *
XML_PullParser_getNamespaceUri (XML_PullParser * parser, NAMESPACE * ns)
{
  if (ns)
    return strdup (ns->uri);
  else
    return NULL;
}


char *
XML_PullParser_getNamespacePrefix_by_number (XML_PullParser * parser, int i)
{
  int ix = parser->namesp->pointer;
  ELEMENT *e;
  NAMESPACE *ns = NULL;
  if (i > ix)
    XML_PullParser_Exception (p_FILE, LINE);
  else
    {
      e = Stack_get (parser->namesp, i);
      if (e->namespace)
	ns = e->namespace;
    }
  return strdup (ns->name);
}


char *
XML_PullParser_getNamespaceUri_by_number (XML_PullParser * parser, int i)
{
  int ix = parser->namesp->pointer;
  ELEMENT *e;
  NAMESPACE *ns = NULL;
  if (i > ix)
    XML_PullParser_Exception (p_FILE, LINE);
  else
    {
      e = Stack_get (parser->namesp, i);
      if (e->namespace)
	ns = e->namespace;
    }
  return strdup (ns->uri);
}


char *
XML_PullParser_getAttributeNamespace_by_number (XML_PullParser *parser, int i)
{
  char *att_prefix;
  int ii;
  int ix = parser->namesp->pointer;
  ELEMENT *e;
  NAMESPACE *ns = NULL;
  att_prefix = XML_PullParser_getAttributePrefix_by_number (parser, i);
    
  for (ii = 0; ii <= ix; ii++)
    {
      e = Stack_get (parser->namesp, ii);
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
