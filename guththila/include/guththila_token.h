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
 */


#ifndef GUTHTHILA_TOKEN_H
#define GUTHTHILA_TOKEN_H

/* #include "guththila_environment.h" */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <axis2_env.h>
#include <guththila_defines.h>


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


AXIS2_EXTERN guththila_token_t *
guththila_token_create_token_buffer (axis2_env_t * environment,
                                     int size);
                                     
AXIS2_EXTERN void
guththila_token_free (axis2_env_t * environment,
					  guththila_token_t * tok);
AXIS2_EXTERN int
guththila_token_length (axis2_env_t * environment,
                        guththila_token_t * tok);
                        
AXIS2_EXTERN void
guththila_token_exception ();

AXIS2_EXTERN guththila_token_t *
guththila_token_append (axis2_env_t * environment,
                        guththila_token_t * tok);
                        
AXIS2_EXTERN guththila_token_t *
guththila_token_grow (axis2_env_t * environment,
                      guththila_token_t * tok);
                      
AXIS2_EXTERN guththila_token_t *
guththila_token_last (axis2_env_t * environment,
                      guththila_token_t * tok);
                      
AXIS2_EXTERN int
guththila_token_count (axis2_env_t * environment,
                       guththila_token_t * tok);
                       
AXIS2_EXTERN guththila_char_t *
guththila_token_to_string (axis2_env_t * environment,
                           guththila_token_t * tok,
                           int unicode);
                           
AXIS2_EXTERN void
guththila_token_relocate (axis2_env_t * environment,
                          guththila_token_t * tok,
                          int offset);
                          
                          
AXIS2_EXTERN int
guththila_token_compare (axis2_env_t * environment,
                         guththila_token_t * tok,
                         const guththila_char_t * st,
                         int n, 
                         int unicode_state);
                         
                         
AXIS2_EXTERN guththila_char_t *
guththila_token_convert_utf16_to_utf8 (axis2_env_t * environment,
                                       guththila_char_t * buffer,
                                       int length);
                                       
AXIS2_EXTERN int
guththila_token_length_utf16 (axis2_env_t * environment,
                              unsigned int utf16_ch);
                              
                              
AXIS2_EXTERN guththila_char_t *
guththila_token_build_utf8 (axis2_env_t * environment,
                            unsigned int utf16_ch,
                            int length);
                            
AXIS2_EXTERN guththila_char_t *
guththila_token_char_ref (axis2_env_t * environment,
                          guththila_char_t * buffer);
                          

#endif /* GUTHTHILA_TOKEN_H */





