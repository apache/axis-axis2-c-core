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
 
#include <w2c_url_processor.h>
#include <w2c_string.h>
#include <axis2_url.h>
#include <ctype.h>


static axis2_bool_t AXIS2_CALL
w2c_url_processor_is_token ( axis2_char_t *str );

static axis2_char_t* axis2_add_word_to_package_name
              ( axis2_char_t *package_name ,
                axis2_char_t *long_path,
                const axis2_env_t *env );

static const axis2_char_t *w2c_url_processor_c_tokens[]={ 
                      "auto", "break", "case", "axis2_char_t", "const", "continue",
                      "default", "do", "double", "else", "enum", "extern",
                      "float", "for", "goto", "if", "int", "long", "register",
                      "return", "short", "signed", "sizeof", "static",
                      "struct", "switch", "typedef", "union", "unsigned",
                      "void", "volatile", "while", NULL };

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    w2c_url_processor_make_package_name(
    const axis2_env_t *env,
    axis2_char_t *namespace)
{
    axis2_url_t *url = NULL;
    axis2_char_t *host_name = NULL;
    axis2_char_t *path = NULL;
    int index = 0;
    int path_length = 0;
    axis2_char_t *package_name = NULL;
    axis2_char_t *long_path = NULL;
    axis2_char_t *long_path_rev = NULL;
    axis2_char_t *ptr = NULL;

    url = axis2_url_parse_string (env, namespace );
    if ( ! url )
    {
        return axis2_strdup( namespace, env);
    }
    host_name = axis2_url_get_server ( url, env );
    path = axis2_url_get_path ( url, env );

    if ( ! host_name )
    {
        index = w2c_string_indexof ( host_name, ':');
        if ( index > -1 )
        {
            host_name = 
              axis2_string_substring_starting_at ( host_name, index );
        
            index =  w2c_string_indexof ( host_name, '/');
            if ( index > -1 )
            {
                host_name =
                 axis2_string_substring_ending_at ( host_name, index);
            }
        }
        else
        {
            host_name = namespace;
        }
    }
    
    if ( ! host_name || *host_name =='\0' )
    {
        return NULL;
    }

    long_path = (axis2_char_t* )axis2_strdup ( host_name,env );

    /** make path_name legal */    

    path_length = strlen ( long_path );    
    
    for ( long_path_rev = long_path + path_length ; ; long_path_rev -- )
    {
        if ( long_path -1 == long_path_rev ||
                 '.' == *long_path_rev ||':' == *long_path_rev )
        {
            /** go up one */
            long_path_rev ++;
            package_name =
             axis2_add_word_to_package_name( package_name , long_path_rev, env);

            if ( long_path_rev == long_path )  /** loop should be ended */
            {
                break;
            }
            else /** prepare for the next loop */
            {
                long_path_rev --;
                *long_path_rev = '\0';
                package_name = 
                    w2c_string_add_string ( package_name, ".", env);
            }
        }
    }
	AXIS2_FREE(env->allocator, long_path );    
  
    if ( NULL != path )
        path_length = strlen ( path );
    if ( 0 != path_length )
    {
        if ( '/' == path[path_length-1] )
        {
            path[ path_length -1] = '\0';
        }
        if ('/' ==  *path )
        {
            path ++;
        }
        
        long_path = ( axis2_char_t* )axis2_strdup( path, env );
        long_path = axis2_string_replace ( long_path, '-', '_' );
        long_path = axis2_string_replace ( long_path, ':', '_' );

        for ( ptr = long_path_rev = long_path; 
                         path_length ; ptr ++, path_length -- )
        {
            if ( '/' == *ptr || '\0' == *ptr)
            {
                *ptr ='\0';
                package_name =
                    w2c_string_add_string ( package_name, ".", env);               
                package_name =
                  axis2_add_word_to_package_name
                           ( package_name , long_path_rev, env);
                long_path_rev = ptr + 1;
            }
        }
		AXIS2_FREE(env->allocator, long_path );
    }
                
    return package_name;
}

/** internal methods */

static axis2_char_t* axis2_add_word_to_package_name
              ( axis2_char_t *package_name ,
                axis2_char_t *long_path,
                const axis2_env_t *env )
{
    /** make non keyword */
    if ( w2c_url_processor_is_token( long_path) )
    {
        package_name = w2c_string_add_string ( package_name,
                                "_", env );
    }
    /** starting with digit -> prefix _ */
    if ( isdigit(*long_path ) )
    {
        package_name = w2c_string_add_string ( package_name,
                                "_", env );
    }
    long_path = axis2_string_replace ( long_path, '.', '_' );
    package_name = w2c_string_add_string ( package_name, long_path, env );
   
    return package_name;
}

static axis2_bool_t AXIS2_CALL
w2c_url_processor_is_token ( axis2_char_t *str )
{
    int i = 0;
    while ( w2c_url_processor_c_tokens[i] )
    {
        if ( !strcmp (w2c_url_processor_c_tokens[i], str ) )
        {
            return AXIS2_TRUE;
        }
        i ++;
    }
    return AXIS2_FALSE;
}

