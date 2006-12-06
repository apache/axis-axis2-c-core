/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <w2c_string.h>
#include <axis2_string.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* hoping this all ascii */
#define W2C_STRING_IS_CAPITAL(c) (c) >= 'A' && (c) <= 'Z'
#define W2C_STRING_IS_SIMPLE(c) (c) >= 'a' && (c) <= 'z'


AXIS2_EXTERN int AXIS2_CALL
w2c_string_indexof( axis2_char_t* str, axis2_char_t c )
{
    int i;
    for ( i = 0; *str != '\0' ; str ++, i ++ )
    {
        if ( *str == c )
        {
            return i;
        }
    }
    return -1;
}

AXIS2_EXTERN int AXIS2_CALL
w2c_string_indexof_cs( axis2_char_t* str, axis2_char_t* cs )
{
    int i = 0;
    axis2_char_t* temp_cs = cs;
    int starting_i = -1;
    for ( i = 0; *str != '\0' && *temp_cs!= '\0' ; str ++, i ++ )
    {
        if ( *str == *temp_cs )
        {
            temp_cs ++;
            if ( starting_i < 0 ) 
            {
                starting_i = i;
            }
        }
        else if ( starting_i > -1 )
        {
            starting_i = -1;
            temp_cs = cs;
        }
    }
    if ( starting_i > -1 && *temp_cs =='\0' )
    {
        return starting_i;
    }
    return -1;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_string_add_string ( axis2_char_t* string1, axis2_char_t* string2,
                        const axis2_env_t* env)
{
    int current_len = 0;
    int additional_len = 0;
    axis2_char_t* new_location = NULL;

    additional_len = strlen ( string2 );
    if ( 0 == additional_len )
    {
        return string1;
    }

    if ( NULL == string1 )
    {
        string1 = AXIS2_MALLOC( env-> allocator, 
                   sizeof(axis2_char_t) * (additional_len +1 ) );
    }
    else
    {
        current_len = strlen ( string1 );
        string1 = AXIS2_REALLOC( env-> allocator,
                 string1, sizeof(axis2_char_t) *
                 (current_len + additional_len + 1 ) );
    }
    new_location = string1 + sizeof(axis2_char_t) * current_len ;
    memcpy ( new_location , string2, sizeof(axis2_char_t) * additional_len );

    string1[ additional_len + current_len ] = '\0';

    return string1;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_string_make_c_simple_name( axis2_char_t *name,
                               const axis2_env_t *env)
{
    char *p = NULL;
    int len = 0;
    char *ret_val = NULL;
    int new_index = 0;
    int earlier_capital = 0;

    name = (axis2_char_t*)AXIS2_STRDUP( name, env); /* so constansts are allowd */
    len = AXIS2_STRLEN( name);
    ret_val = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, 
                                        sizeof(axis2_char_t) * len * 2);

    for ( p = name, new_index = 0, earlier_capital = 0; *p != '\0'; p ++ )
    {
        if ( W2C_STRING_IS_CAPITAL(*p) )
        {
            if ( new_index != 0 && ret_val[new_index-1] != '_' && !earlier_capital)
                ret_val[new_index++] = '_';
            ret_val[new_index++] = tolower(*p);
            earlier_capital = 1;
        }
        else
        {
            earlier_capital = 0;
            ret_val[new_index++] = *p;
        }
    }
    ret_val[new_index] = '\0';

    AXIS2_FREE( env-> allocator, name);
    return ret_val;
}
    
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_string_make_c_macro_name( axis2_char_t *name,
                               const axis2_env_t *env)
{
    char *p = NULL;
    char *ret_val = NULL;

    ret_val = w2c_string_make_c_simple_name( name, env);

    for ( p = ret_val; *p != '\0'; p ++ )
    {
        *p = toupper(*p);
    }
    return ret_val;
}

AXIS2_EXTERN axis2_char_t*
w2c_string_make_key_from_qname( axis2_qname_t *qname, 
                                     const axis2_env_t *env)
{
    axis2_char_t *local = NULL;
    axis2_char_t *uri = NULL;
    axis2_char_t *key = NULL;

    local = AXIS2_QNAME_GET_LOCALPART( qname, env);
    uri = AXIS2_QNAME_GET_URI( qname, env);
    
    key = AXIS2_STRDUP( local, env);
    if ( uri != NULL && *uri != '\0' ) /* it should not be empty*/
    {
        key = w2c_string_add_string( key, "|", env);
        key = w2c_string_add_string( key, uri, env);
    }
    return key;
}

