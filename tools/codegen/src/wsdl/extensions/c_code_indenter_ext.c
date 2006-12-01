/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <w2c_extension.h>
#include <w2c_string.h>
#include <axis2_string.h>


typedef struct w2c_c_code_indenter_ext_impl
{
    w2c_extension_t extension;

} w2c_c_code_indenter_ext_impl_t;

#define W2C_INTF_TO_IMPL(extension) \
        ((w2c_c_code_indenter_ext_impl_t*) extension)

void format_file( axis2_char_t *filename,
                  const axis2_env_t *env);

/************************* implmentations ********************************/
axis2_status_t AXIS2_CALL
w2c_c_code_indenter_ext_free(w2c_extension_t *extension,
       const axis2_env_t *env)
{
    w2c_c_code_indenter_ext_impl_t *extension_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    extension_impl = W2C_INTF_TO_IMPL(extension);

    if(extension->ops)
    {
        AXIS2_FREE(env->allocator, extension->ops);
    }
    if(extension_impl)
    {
        AXIS2_FREE(env->allocator, extension_impl);
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_c_code_indenter_ext_engage(w2c_extension_t *extension,
       const axis2_env_t *env,
       w2c_engine_configuration_t *conf)
{
    w2c_c_code_indenter_ext_impl_t *extension_impl = NULL;
    axis2_bool_t indent_code = AXIS2_FALSE;
    axis2_char_t *lang = NULL;
    axis2_array_list_t *filenames = NULL;
    int i = 0, size = 0;
    axis2_char_t *filename = NULL;
    int length = 0;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    extension_impl = W2C_INTF_TO_IMPL(extension);

    indent_code = W2C_ENGINE_CONFIGURATION_GET_INDENT_CODE( conf, env);
    lang = W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LANGUAGE( conf, env);
    filenames = W2C_ENGINE_CONFIGURATION_GET_FILENAMES( conf, env);

    /* this executes only with following configuration */
    if ( indent_code && AXIS2_STRCMP( lang, "c") == 0)
    {
        size = AXIS2_ARRAY_LIST_SIZE( filenames, env);
        for ( i = 0; i < size; i ++ )
        {
            filename = AXIS2_ARRAY_LIST_GET( filenames, env, i);
            length = AXIS2_STRLEN( filename);
            /* take only .c or .h files */
            if( filename[length-1] == 'c' || filename[length-1] == 'h' ||
                    filename[length-2] == '.' )
            {
                format_file( filename, env);
            }
        }
    }
    return AXIS2_SUCCESS;
}

/****************** standard create and delete for DLL ************************/
AXIS2_EXPORT int
axis2_get_instance(w2c_extension_t **inst,
                   const axis2_env_t *env)
{
    w2c_c_code_indenter_ext_impl_t *extension_impl = NULL;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    extension_impl = (w2c_c_code_indenter_ext_impl_t*)AXIS2_MALLOC( env-> allocator, 
                                          sizeof(w2c_c_code_indenter_ext_impl_t) );
 
    if(NULL == extension_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return AXIS2_FAILURE;
    }
    
    extension_impl->extension.ops = 
                AXIS2_MALLOC (env->allocator, sizeof(w2c_extension_ops_t));
    if(NULL == extension_impl->extension.ops)
    {
        w2c_c_code_indenter_ext_free(&(extension_impl->extension), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }   
    extension_impl->extension.ops->free = 
                      w2c_c_code_indenter_ext_free;
    extension_impl->extension.ops->engage = 
                      w2c_c_code_indenter_ext_engage;

    *inst = &(extension_impl->extension);

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int
axis2_remove_instance(w2c_extension_t *inst,
                      const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    if (inst)
    {
        status = W2C_EXTENSION_FREE(inst, env);
    }
    return status;
}

/** the following part seperated to handle code indenting algorithm */

/* function declarations */
void format_file( axis2_char_t *filename,
                  const axis2_env_t *env);
axis2_char_t* read_file ( axis2_char_t *filename,
                  const axis2_env_t *env);
void write_file ( axis2_char_t *filename,
                  axis2_char_t *buffer,
                  const axis2_env_t *env);

typedef struct custom_allocator
{
     int allocated;
     int index;
     char* buffer;
} custom_allocator_t;

void add_string( const axis2_env_t* env,
                      custom_allocator_t* allocator,
                      char* string);

void add_char( const axis2_env_t* env,
                      custom_allocator_t* allocator,
                      char c,
                      int turns);

#define SPACES_FOR_TAB 4

/* implementations */
void format_file( axis2_char_t *filename,
                  const axis2_env_t *env)
{
    custom_allocator_t * allocator  = NULL;
    axis2_char_t* in_buffer = NULL;
    axis2_char_t* out_buffer = NULL;
    char* p = NULL;
    int tabs = 0;

    /** formatting parameters */
    char *line_start = NULL;
    char *line_end = NULL;
    int start_blanks = 0;
    int early_line_start_blanks = 0;
    int continue_from_early_line = 0;
    int line_ended_with_semicolon = 1;
    int line_ended_with_bracket = 0;
    int line_ended_with_backslash = 0;
    int line_ended_with_curly_bracket = 0; /* close} or open{ */
    int tabs_off = 0;
    int is_comment = 0;
    int multi_line_comment = 0;
    int shift = 0;
    int early_line_blank = 0;
    int comment_just_finished = 0;
    int early_line_macro = 0;
   
    allocator = (custom_allocator_t*) AXIS2_MALLOC(env->
        allocator, sizeof(custom_allocator_t));
    allocator-> buffer = NULL;
    allocator-> allocated = 0;
    allocator-> index = 0;
    
    in_buffer = read_file( filename, env);

    p = in_buffer;
    for ( p = in_buffer, tabs = 0 ; *p ; p ++)
    {
        /** identify new_line */
        if ( p == in_buffer || *(p-1) == '\n')
        {
            /* assign new tabs off value from previous lines */
            tabs += tabs_off;
            if (tabs < 0 ) tabs = 0;
            tabs_off = 0;

            /** some decision making) */
             if ( !early_line_blank) 
            {
                if ( line_ended_with_semicolon )
                {
                    continue_from_early_line = 0;
                }
                else if ( line_ended_with_bracket)
                {
                    continue_from_early_line = 0;
                }
                else if ( line_ended_with_curly_bracket)
                {
                    continue_from_early_line = 0;
                }
                else if ( line_ended_with_backslash)
                {
                    continue_from_early_line = 1;
                }
                else
                {
                    /** some decision 
                     * if it is not a semicolon or bracket it continues*/
                    continue_from_early_line = 1;
                }
   
                /* more adjusment */
                if ( comment_just_finished)
                {
                    continue_from_early_line = 0;
                }
                if ( multi_line_comment)
                {
                    continue_from_early_line = 1;
                }
                if ( early_line_macro && !line_ended_with_backslash )
                {
                    continue_from_early_line = 0;
                }
            }
            else
            {
               /** big decision */
               continue_from_early_line = 0;
            }
            start_blanks = 0;
            /* count starter blanks */
            for( ; *p && (*p == ' ' || *p == '\t'); p ++)
            {
                start_blanks ++;
            }

            line_start = p;
            line_end = NULL;
            /** omit things until the end */
            for( ; *p && *p !='\n'; p ++)
            {
                /** following set handle comments */
                if ( !is_comment && *p == '/' &&
                        *(p+1) == '*')
                {
                    is_comment = 1;
                    multi_line_comment = 1;
                }
                else if ( is_comment && *p == '*' && *(p+1) == '/' )
                {
                    is_comment = 0;
                    multi_line_comment = 0;
                }
                else if ( !is_comment && *p == '/' && *(p+1) == '/' )
                {
                    is_comment = 1;
                }

                /* intending */
                else if ( !is_comment && *p == '{' )
                {
                    tabs_off ++;
                }
                else if ( !is_comment && *p == '}')
                {
                    tabs --;
                    if (tabs < 0 ) tabs = 0;
                }

                /** check for the valid end */
                if ( *p != ' ' && *p!='\t')
                {
                    line_end = p;
                }
            }
            *p = '\0';
            if ( line_end )
            {
                line_ended_with_semicolon = ( ';' == *line_end );
                line_ended_with_bracket = (')' == *line_end);
                line_ended_with_backslash= ( '\\' == *line_end);
                comment_just_finished = ( '/' == *line_end &&
                                    line_end -1 != line_start &&
                                    '*' == *(line_end-1) );
                line_ended_with_curly_bracket = ( ('{' == *line_end) 
                                                || ('}' == *line_end) );
            }
            early_line_blank = ('\0' == *line_start);

            early_line_macro = ('#' == *line_start);
            
            if( !multi_line_comment)
            {
                is_comment = 0;
            }
            /* the following works only for generated code 
             * given that code is relatively well formatted
             */
            if ( continue_from_early_line)
            {
                shift = start_blanks - early_line_start_blanks;
                if (shift > 0)
                { 
                    add_char ( env, allocator, ' ', shift);
                }
            }
            else if(!early_line_blank)
            {
                early_line_start_blanks = start_blanks;
            }
            if ( !continue_from_early_line)
            {
                start_blanks = 0;
            }

            if ( tabs > 0 )
            {
                add_char ( env, allocator, ' ', tabs * SPACES_FOR_TAB );
            }
            add_string ( env, allocator, line_start);
            add_char (env, allocator, '\n', 1);
            *p = '\n';
        }
    }

    *(allocator-> buffer + allocator-> index) = '\0'; 
    out_buffer = allocator-> buffer; 
    write_file( filename, out_buffer, env); 
}

axis2_char_t* read_file ( axis2_char_t *filename,
                  const axis2_env_t *env)
{
    const int MAX_SIZE=100;
    int nread = 0;
    FILE *f = NULL;
    axis2_char_t *out_stream = NULL;
    int ncount = 0;

    out_stream = (axis2_char_t*) AXIS2_MALLOC( env-> allocator,
                                    sizeof(axis2_char_t)* MAX_SIZE );
    if (out_stream == NULL )
    {
        return NULL;
    }

    f = fopen ( filename, "r+");
    if ( f == NULL )
    {
		AXIS2_FREE(env->allocator, out_stream );
        return NULL;
    }
    do{
        nread = fread ( out_stream + ncount, sizeof(axis2_char_t), MAX_SIZE, f);
        ncount += nread;
        out_stream = (axis2_char_t*) AXIS2_REALLOC( env-> allocator,
                    out_stream,
                    sizeof(axis2_char_t)* (MAX_SIZE + ncount ) );
        if ( out_stream == NULL )
        {
            return NULL;
        }
    }while ( nread == MAX_SIZE );

    out_stream[ncount] = '\0';
    fclose(f);

    return out_stream;
}

void write_file ( axis2_char_t *filename,
                  axis2_char_t *buffer,
                  const axis2_env_t *env)
{
    FILE *f = NULL;
    f = fopen( filename, "w+");
    fprintf( f, "%s",buffer);
}

void add_string( const axis2_env_t* env,
                      custom_allocator_t* allocator,
                      char* string)
{
    int additional_len = 0;
    void* dest = NULL;
    void* src = NULL;
    int count = 0;

    additional_len = AXIS2_STRLEN ( string ) +1;
    if ( allocator-> index + additional_len  >= allocator-> allocated )
    {
        if ( allocator-> allocated == 0 )
        {
            allocator-> buffer =
                    AXIS2_MALLOC ( env-> allocator, additional_len );
        }
        else
        {
            allocator-> buffer =
                    AXIS2_REALLOC ( env-> allocator, allocator-> buffer,
                                allocator-> allocated + additional_len);
        }
        allocator-> allocated += additional_len;
    }

    /* copy memory */
    dest = allocator-> buffer + allocator-> index;
    src = string;
    count = additional_len; /* this is with the terminating zero */
    memcpy ( dest, src, count );

    allocator-> index += count -1;
}


void add_char( const axis2_env_t* env,
                      custom_allocator_t* allocator,
                      char c,
                      int turns)
{
    int additional_len = 0;

    additional_len = turns + 1;
    if ( allocator-> index + additional_len  >= allocator-> allocated )
    {
        if ( allocator-> allocated == 0 )
        {
            allocator-> buffer =
                    AXIS2_MALLOC ( env-> allocator, additional_len );
        }
        else
        {
            allocator-> buffer =
                    AXIS2_REALLOC ( env-> allocator, allocator-> buffer,
                                allocator-> allocated + additional_len);
        }
        allocator-> allocated += additional_len;
    }

    /* copy memory */
    memset ( allocator-> buffer + allocator-> index, c, turns );

    allocator-> index += turns;

}

