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


#ifndef __TOKEN__INCLUDED__
#define __TOKEN__INCLUDED__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct token
{
  int type;
  char *start;
  char *end;
  int last;
  int size;
  int ref;
}TOKEN;


enum _type
  {
    Unknown = 1,
    Name,
    Value,
    Attribute,
    AttValue,
    Prefix,
    CharData
  };

enum _type TYPE;


TOKEN* Token_createTokenBuffer(int size);
void Token_freeTokenBuffer (TOKEN *tok);
int Token_length (TOKEN *tok);
void Token_Exception ();
TOKEN* Token_append (TOKEN *tok);
TOKEN* Token_grow (TOKEN *tok);
TOKEN* Token_last (TOKEN *tok);
int Token_count (TOKEN *tok);
char* Token_toString (TOKEN *tok, int unicode);
void Token_relocate (TOKEN *tok, int offset);
int Token_compare (TOKEN *tok, const char *st, int n, int unicode_state);
char *Token_convert_utf16_to_utf8 (char *buffer, int length);
int Token_length_utf16 (unsigned int utf16_ch);
char *Token_build_utf8 (unsigned int utf16_ch, int length);
char *Token_char_ref (char *buffer);
#endif
