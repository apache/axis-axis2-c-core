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

#include "guththila_environment.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct guththila_token_s
{
    int type;
    guththila_char_t *start;
    guththila_char_t *end;
    int last;
    int size;
    int ref;
} guththila_token_t;


enum guththila_token_type
{
    Unknown = 1,
    _name,
    _value,
    _attribute,
    _attribute_value,
    _prefix,
    _char_data
};


GUTHTHILA_DECLARE (guththila_token_t *)
guththila_token_create_token_buffer (guththila_environment_t * environment,
                                     int size);
GUTHTHILA_DECLARE (void)
guththila_token_free_token_buffer (guththila_environment_t * environment,
                                   guththila_token_t * tok);
GUTHTHILA_DECLARE (int)
guththila_token_length (guththila_environment_t * environment,
                        guththila_token_t * tok);
GUTHTHILA_DECLARE (void)
guththila_token_exception ();
GUTHTHILA_DECLARE (guththila_token_t *)
guththila_token_append (guththila_environment_t * environment,
                        guththila_token_t * tok);
GUTHTHILA_DECLARE (guththila_token_t *)
guththila_token_grow (guththila_environment_t * environment,
                      guththila_token_t * tok);
GUTHTHILA_DECLARE (guththila_token_t *)
guththila_token_last (guththila_environment_t * environment,
                      guththila_token_t * tok);
GUTHTHILA_DECLARE (int)
guththila_token_count (guththila_environment_t * environment,
                       guththila_token_t * tok);
GUTHTHILA_DECLARE (guththila_char_t *)
guththila_token_to_string (guththila_environment_t * environment,
                           guththila_token_t * tok, int unicode);
GUTHTHILA_DECLARE (void)
guththila_token_relocate (guththila_environment_t * environment,
                          guththila_token_t * tok, int offset);
GUTHTHILA_DECLARE (int)
guththila_token_compare (guththila_environment_t * environment,
                         guththila_token_t * tok, const guththila_char_t * st,
                         int n, int unicode_state);
GUTHTHILA_DECLARE (guththila_char_t *)
guththila_token_convert_utf16_to_utf8 (guththila_environment_t * environment,
                                       guththila_char_t * buffer, int length);
GUTHTHILA_DECLARE (int)
guththila_token_length_utf16 (guththila_environment_t * environment,
                              unsigned int utf16_ch);
GUTHTHILA_DECLARE (guththila_char_t *)
guththila_token_build_utf8 (guththila_environment_t * environment,
                            unsigned int utf16_ch, int length);
GUTHTHILA_DECLARE (guththila_char_t *)
guththila_token_char_ref (guththila_environment_t * environment,
                          guththila_char_t * buffer);

#endif /* GUTHTHILA_TOKEN_H */
