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


#ifndef GUTHTHILA_TOKEN_H
#define GUTHTHILA_TOKEN_H
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


guththila_token_t* guththila_token_create_token_buffer(int size);
void guththila_token_free_token_buffer (guththila_token_t *tok);
int guththila_token_length (guththila_token_t *tok);
void guththila_token_exception ();
guththila_token_t* guththila_token_append (guththila_token_t *tok);
guththila_token_t* guththila_token_grow (guththila_token_t *tok);
guththila_token_t* guththila_token_last (guththila_token_t *tok);
int guththila_token_count (guththila_token_t *tok);
char* guththila_token_to_string (guththila_token_t *tok, int unicode);
void guththila_token_relocate (guththila_token_t *tok, int offset);
int guththila_token_compare (guththila_token_t *tok, const char *st, int n, int unicode_state);
char *guththila_token_convert_utf16_to_utf8 (char *buffer, int length);
int guththila_token_length_utf16 (unsigned int utf16_ch);
char *guththila_token_build_utf8 (unsigned int utf16_ch, int length);
char *guththila_token_char_ref (char *buffer);

#endif /* GUTHTHILA_TOKEN_H */
