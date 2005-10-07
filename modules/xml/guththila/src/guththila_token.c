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


#include "guththila_token.h"
#include "unicode.h"

TOKEN *
Token_createTokenBuffer (int size)
{
  TOKEN *tok = (TOKEN *) malloc (sizeof(TOKEN)*size);
  tok->size = size;
  return tok;
}


void
Token_freeTokenBuffer (TOKEN *tok)
{
  if (tok)
    free (tok);
}


void 
Token_Exception ()
{
  printf ("Throws new token Exception");
  exit (0);
}


int
Token_length (TOKEN *tok)
{
  if (tok->end)
    return (tok->end) - (tok->start) + 1;
  else
    Token_Exception ();
  return 0;
}


TOKEN *
Token_grow (TOKEN *tok)
{
  tok->size <<= 1;
  tok = (TOKEN *) realloc (tok, sizeof(TOKEN)*tok->size);
  return tok;
}


TOKEN *
Token_append (TOKEN *tok)
{
  if (++ (tok->last) > (tok->size))
    Token_grow (tok);
  return &tok[tok->last];
}


TOKEN *
Token_last (TOKEN *tok)
{
  if (tok->last < 0)
    Token_Exception ();
  return &tok[tok->last];
}


int
Token_count (TOKEN *tok)
{
  return tok->last;
}

char*
Token_char_ref (char *buffer)
{
  int len;
  int ii;
  int ix;
  len = strlen (buffer);
  char *ref_buffer = (char *) malloc (len+1);
  for (ii = 0, ix = 0; ii < len; ii++, ix++)
    {
      if (buffer[ii] == '&')
	{
	  if (buffer[ii+1] == 'a' 
	      && buffer[ii+2] == 'm'
	      && buffer[ii+3] == 'p'
	      && buffer[ii+4] == ';')
	    {
	      ref_buffer[ix] = '&';
	      ii += 4;
	    }
	  else if (buffer[ii+1] == 'g'
		   && buffer[ii+2] == 't'
		   && buffer[ii+3] == ';')
	    {
	      ref_buffer[ix] = '>';
	      ii += 3;
	    }
	  else if (buffer[ii+1] == 'l'
		   && buffer[ii+2] == 't'
		   && buffer[ii+3] == ';')
	    {
	      ref_buffer[ix] = '<';
	      ii += 3;
	    }
	  else if (buffer[ii+1] == 'q'
		   && buffer[ii+2] == 'u'
		   && buffer[ii+3] == 'o'
		   && buffer[ii+4] == 't'
		   && buffer[ii+5] == ';')
	    {
	      ref_buffer[ix] = '"';
	      ii +=5;
	    }
	  else if (buffer[ii+1] == 'a'
		   && buffer[ii+2] == 'p'
		   && buffer[ii+3] == 'o'
		   && buffer[ii+4] == 's'
		   && buffer[ii+5] == ';')
	    {
	      ref_buffer[ix] = '\'';
	      ii +=5;
	    }
	  else 
	    Token_Exception ();
	}
      else
	ref_buffer[ix] = buffer[ii];
    }
  return ref_buffer;
}


char *
Token_toString (TOKEN *tok, int unicode)
{
  if (tok)
    {
      if (unicode == None)
	{
	  int length;
	  char *buffer;
	  length = Token_length (tok);
	  buffer = (char *) malloc (length + 1);
	  memcpy (buffer, tok->start, length);
	  buffer[length] = 0;
	  if (tok->ref)
	    Token_char_ref (buffer);
	  else
	    return buffer;
	}
      else
	{
	  int length;
	  char *buffer;
	  length = Token_length (tok);
	  buffer = (char *) malloc (length + 1);
	  memcpy (buffer, tok->start, length);
	  buffer[length] = 0;
	  return Token_convert_utf16_to_utf8 (buffer, length);
	}
    }
  else
    return NULL;
}


void
Token_relocate (TOKEN *tok, int offset)
{
  tok->start -= offset;
  tok->end -= offset;
}


int
Token_compare (TOKEN *tok, const char *s, int n, int unicode_state)
{
  if (unicode_state == None)
    return strncmp (tok->start, s, n);
  else
    {
      char *buffer;
      buffer = Token_toString (tok, unicode_state);
      return strncmp (buffer, s, n);
    }
}


int
Token_length_utf16 (unsigned int utf16_ch)
{
  int length;

  if (0x80 > utf16_ch)
    length = 1;
  else if (0x800 > utf16_ch)
    length = 2;
  else if (0x10000 > utf16_ch)
    length = 3;
  else if (0x200000 > utf16_ch)
    length = 4;
  else if (0x4000000 > utf16_ch)
    length = 5;
  else
    length = 6;

  return length;
}


char*
Token_build_utf8 (unsigned int utf16_ch, int length)
{
  UTF8_char mask = 0;
  int ii = 0;
  char *buffer = (char *) calloc (length + 1, 1);
  if (length == 1)
    buffer[0] = utf16_ch;
  else
    {
      switch (length)
	{
	case 1:
	  break;
	case 2:
	  mask = 0xc0;
	  break;
	case 3:
	  mask = 0xe0;
	  break;
	case 4:
	  mask = 0xf0;
	  break;
	case 5:
	  mask = 0xf8;
	  break;
	case 6:
	  mask = 0xfc;
	  break;
	};

      for (ii = length - 1; ii > 0; ii--)
	{
	  buffer[ii] = (utf16_ch & 0x3f) | 0x80;
	  utf16_ch >>= 6;
	}

      buffer[0] = utf16_ch | mask;
      buffer[length] = 0;
    }
  return buffer;
}


char *
Token_convert_utf16_to_utf8 (char *buffer, int length)
{
  unsigned int utf16_char = 0;
  int length_utf16 = 0;
  int total_length = 0;
  char *input_buffer = buffer;
  char *output_buffer = (char *) calloc (1,  1);
  char *output_char = 0;
  int ii = 0;
  for (ii = 0; length > ii ; )
    {
      utf16_char = *((UTF16_char *)&input_buffer[ii]);
      ii += 2;
      length_utf16 = Token_length_utf16 (utf16_char);
      output_char = Token_build_utf8 (utf16_char, length_utf16);
      total_length += length_utf16;
      output_buffer = strcat (output_buffer, output_char);
    }
  return output_buffer;
}


