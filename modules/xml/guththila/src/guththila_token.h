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


#ifndef GUTHTHILA_guththila_token_t_H
#define GUTHTHILA_guththila_token_t_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct guththila_token_s
{
  int type;
  char *start;
  char *end;
  int last;
  int size;
  int ref;
} guththila_token_t;


enum guththila_token_type
  {
    Unknown = 1,
    Name,
    Value,
    Attribute,
    AttValue,
    Prefix,
    CharData
  };


guththila_token_t* Token_createTokenBuffer(int size);
void Token_freeTokenBuffer (guththila_token_t *tok);
int Token_length (guththila_token_t *tok);
void Token_Exception ();
guththila_token_t* Token_append (guththila_token_t *tok);
guththila_token_t* Token_grow (guththila_token_t *tok);
guththila_token_t* Token_last (guththila_token_t *tok);
int Token_count (guththila_token_t *tok);
char* Token_toString (guththila_token_t *tok, int unicode);
void Token_relocate (guththila_token_t *tok, int offset);
int Token_compare (guththila_token_t *tok, const char *st, int n, int unicode_state);
char *Token_convert_utf16_to_utf8 (char *buffer, int length);
int Token_length_utf16 (unsigned int utf16_ch);
char *Token_build_utf8 (unsigned int utf16_ch, int length);
char *Token_char_ref (char *buffer);

#endif /* GUTHTHILA_guththila_token_t_H */
