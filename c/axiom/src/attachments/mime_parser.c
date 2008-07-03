
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

#include <axiom_mime_parser.h>
#include <axutil_string.h>
#include <axiom_data_handler.h>
#include <stdio.h>
#include <axiom_caching_callback.h>
#include <axutil_class_loader.h>
#include <axutil_http_chunked_stream.h>

struct axiom_mime_parser
{
    axutil_hash_t *mime_parts_map;
    int soap_body_len;
    axis2_char_t *soap_body_str;
    int buffer_size;
    int max_buffers;
    axiom_caching_callback_t *caching_callback;
    axis2_char_t *callback_name;
    int total;
};

struct axiom_search_info
{
    /*String need to be searched*/
    const axis2_char_t *search_str;

    /*The buffers and the lengths need to be searched*/
    axis2_char_t *buffer1;
    int len1;
    axis2_char_t *buffer2;
    int len2;

    /*Flag to keep what type of search is this buffer has done*/
    axis2_bool_t primary_search;

    /*The offset where we found the pattern entirely in one buffer*/
    int match_len1;

    /*when pattern contains in two buffers the length of partial pattern
    in buffer2 */
    int match_len2;

    /*Whether we need caching or not*/
    axis2_bool_t cached;

    /*A pointer to a user provided storage to which we cache the attachment*/
    void *handler;

    /* Size of the binary when writing to the buffer*/
    int binary_size;
};

typedef struct axiom_search_info axiom_search_info_t;


#define AXIOM_MIME_PARSER_CONTENT_ID "content-id"
#define AXIOM_MIME_PARSER_CONTENT_TYPE "content-type"

#define AXIOM_MIME_PARSER_END_OF_MIME_MAX_COUNT 100

static axis2_char_t *axiom_mime_parser_search_for_soap(
    const axutil_env_t * env,
    AXIS2_READ_INPUT_CALLBACK callback,
    void *callback_ctx,
    int *buf_num,
    int *len_array,
    axis2_char_t **buf_array,
    axiom_search_info_t *search_info,
    int size,
    axis2_char_t *mime_boundary,
    axiom_mime_parser_t *mime_parser);


static axis2_char_t *axiom_mime_parser_search_for_crlf(
    const axutil_env_t * env,
    AXIS2_READ_INPUT_CALLBACK callback,
    void *callback_ctx,
    int *buf_num,
    int *len_array,
    axis2_char_t **buf_array,
    axiom_search_info_t *search_info,
    int size,
    axiom_mime_parser_t *mime_parser);

static int axiom_mime_parser_calculate_part_len(
    const axutil_env_t *env,
    int buf_num,
    int *len_list,
    int maker,
    axis2_char_t *pos,
    axis2_char_t *buf
);

static axis2_char_t * axiom_mime_parser_create_part (
    const axutil_env_t *env,
    int part_len,
    int buf_num,
    int *len_list,
    int marker,
    axis2_char_t *pos,
    axis2_char_t **buf_list,
    axiom_mime_parser_t *mime_parser);

static axis2_char_t *axiom_mime_parser_search_string(
    axiom_search_info_t *search_info,
    const axutil_env_t *env);

static axis2_char_t *axiom_mime_parser_search_for_attachment(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t * env,
    AXIS2_READ_INPUT_CALLBACK callback,
    void *callback_ctx,
    int *buf_num,
    int *len_array,
    axis2_char_t **buf_array,
    axiom_search_info_t *search_info,
    int size,
    axis2_char_t *mime_boundary);

static axis2_status_t
axiom_mime_parser_process_mime_headers(
    const axutil_env_t *env,
    axiom_mime_parser_t *mime_parser,
    axis2_char_t *mime_headers,
    axis2_char_t *mime_binary,
    int mime_binary_len,
    axis2_bool_t cached);

static void axiom_mime_parser_clear_buffers(
    const axutil_env_t *env,
    axis2_char_t **buf_list,
    int free_from,
    int free_to);

static axis2_status_t axiom_mime_parser_cache_to_buffer(
    const axutil_env_t *env,
    axis2_char_t *buf,
    int buf_len,
    axiom_search_info_t *search_info,
    axiom_mime_parser_t *mime_parser);


static void* axiom_mime_parser_initiate_callback(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env);

static axis2_bool_t axiom_mime_parser_is_more_data(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    axis2_callback_info_t *callback_info);

AXIS2_EXTERN axiom_mime_parser_t *AXIS2_CALL
axiom_mime_parser_create(
    const axutil_env_t * env)
{
    axiom_mime_parser_t *mime_parser = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    mime_parser = (axiom_mime_parser_t *) AXIS2_MALLOC(env->allocator,
                                                       sizeof
                                                       (axiom_mime_parser_t));

    if (!mime_parser)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    mime_parser->mime_parts_map = NULL;
    mime_parser->soap_body_len = 0;
    mime_parser->soap_body_str = NULL;  /* shallow copy */
    mime_parser->buffer_size = 1;
    mime_parser->max_buffers = AXIOM_MIME_PARSER_MAX_BUFFERS;
    mime_parser->caching_callback = NULL;
    mime_parser->callback_name = NULL;
    mime_parser->total = 0;

    mime_parser->mime_parts_map = axutil_hash_make(env);
    if (!(mime_parser->mime_parts_map))
    {
        axiom_mime_parser_free(mime_parser, env);
        return NULL;
    }

    return mime_parser;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_mime_parser_free(
    axiom_mime_parser_t * mime_parser,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    /* This map is passed on to SOAP builder, and SOAP builder take over the
       ownership of the map */
    if(mime_parser->caching_callback)
    {
        axutil_param_t *param = NULL;
        param = mime_parser->caching_callback->param;

        AXIOM_CACHING_CALLBACK_FREE(mime_parser->caching_callback, env);
        mime_parser->caching_callback = NULL;

        if(param)
        {
            axutil_param_free(param, env);
            param = NULL;
        }
    }

    if (mime_parser)
    {
        AXIS2_FREE(env->allocator, mime_parser);
    }

    return;
}

AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axiom_mime_parser_parse(
    axiom_mime_parser_t * mime_parser,
    const axutil_env_t * env,
    AXIS2_READ_INPUT_CALLBACK callback,
    void *callback_ctx,
    axis2_char_t * mime_boundary)
{
    int size = 0;
    axis2_char_t *soap_str = NULL;
    int soap_len = 0;
    axis2_char_t *mime_headers = NULL;
    int mime_headers_len = 0;
    int temp_mime_boundary_size = 0;
    axis2_char_t *temp_mime_boundary = NULL;
    axis2_char_t **buf_array = NULL;
    int *len_array = NULL;
    int buf_num = 0;
    axis2_char_t *pos = NULL;
    axiom_search_info_t *search_info = NULL;
    int part_start = 0;
    axis2_bool_t end_of_mime = AXIS2_FALSE;
    int count = 0; 
    int mime_binary_len = 0;
    axis2_char_t *mime_binary = NULL;
    int len = 0;
    axis2_status_t status = AXIS2_FAILURE; 
    axis2_char_t *buffer = NULL;
    int malloc_len = 0;
    axis2_callback_info_t *callback_info = NULL;

    callback_info = (axis2_callback_info_t *)callback_ctx;

    size = AXIOM_MIME_PARSER_BUFFER_SIZE * (mime_parser->buffer_size); 

    /*An array to keep the set of buffers*/

    buf_array = AXIS2_MALLOC(env->allocator,
        sizeof(axis2_char_t *) * (mime_parser->max_buffers));

    mime_parser->total = mime_parser->total + sizeof(axis2_char_t *) * (mime_parser->max_buffers);

    if (!buf_array)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Failed in creating buffer array");
        return NULL;
    }

    /*Keeps the corresponding lengths of buffers in buf_array*/

    len_array = AXIS2_MALLOC(env->allocator,
        sizeof(int) * (mime_parser->max_buffers));

    mime_parser->total = mime_parser->total + sizeof(int) * (mime_parser->max_buffers);

    if (!len_array)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Failed in creating length array");
        return NULL;
    }

    temp_mime_boundary = axutil_stracat(env, "--", mime_boundary);
    temp_mime_boundary_size = strlen(mime_boundary) + 2;

    /*This struct keeps the pre-post search informations*/
    search_info = AXIS2_MALLOC(env->allocator,
        sizeof(axiom_search_info_t));

    mime_parser->total = mime_parser->total + sizeof(axiom_search_info_t);

    buf_array[buf_num] = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_char_t) * (size + 1));

    mime_parser->total = mime_parser->total + sizeof(axis2_char_t) * (size + 1);

    if(buf_array[buf_num])
    {
        len = callback(buf_array[buf_num], size, (void *) callback_ctx);
    }
    if(len > 0)
    {
        len_array[buf_num] = len;
    }
    else
    {
        return NULL;
    }

    /*starting buffer for the current search*/
    part_start = buf_num;

    /*We are passing the address of the buf_num , beacause that value 
    is changing inside the method.*/

    pos = axiom_mime_parser_search_for_crlf(env, callback, callback_ctx, &buf_num,
            len_array, buf_array, search_info, size, mime_parser);

    if(!pos)
    {
        return NULL;
    }   

    /*The patteren contains in one buffer*/

    if((search_info->match_len2 == 0))
    {
        /*Readjusting the buffers for the next search and discarding the prevoius 
        buffers*/

        malloc_len = buf_array[buf_num] + len_array[buf_num] - pos - 4;
        if(malloc_len < 0)
        {
            return NULL;
        }
        else    
        {
            buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                size + 1));

            mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(size + 1);

            if(malloc_len > 0)
            {
                memcpy(buffer, pos + 4, malloc_len);
            }

            if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
            {
                len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);           
            }
            axiom_mime_parser_clear_buffers(env, buf_array, part_start, buf_num);
            if(len >= 0)
            {
                buf_array[buf_num] = buffer;
                len_array[buf_num] = malloc_len + len;                    
            }
        }
    }

    /*The pattern divides among two buffers*/
        
    else if(search_info->match_len2 > 0)
    {
        malloc_len = len_array[buf_num] - search_info->match_len2;      
        if(malloc_len < 0)
        {
            return NULL;
        } 
        else
        {
            buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                size + 1));

            mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(size + 1);

            if(malloc_len > 0)
            {
                memcpy(buffer, buf_array[buf_num] + search_info->match_len2, malloc_len);        
            }
            if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
            {
                len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);
            }
            axiom_mime_parser_clear_buffers(env, buf_array, part_start, buf_num);
            if(len >= 0)
            {
                buf_array[buf_num] = buffer;
                len_array[buf_num] = malloc_len + len; 
            }
        }
    }
    else
    {
        return NULL;
    }

    /*Resetting the previous search data and getting ready 
      for the next search */

    part_start = buf_num;
    pos = NULL;
    malloc_len = 0;
 
    search_info->match_len1 = 0;
    search_info->match_len2 = 0;

    /*In order to extract the soap envelope we need to search for the first
      --MIMEBOUNDARY  */

    pos = axiom_mime_parser_search_for_soap(env, callback, callback_ctx, &buf_num,
          len_array, buf_array, search_info, size, temp_mime_boundary, mime_parser);

    if(!pos)
    {
        return NULL;
    }
    
    if(search_info->match_len2 == 0)
    {
        /*Calculating the length of the SOAP str*/

        soap_len = axiom_mime_parser_calculate_part_len (
            env, buf_num, len_array, part_start, pos, buf_array[buf_num]);
        if(soap_len > 0)
        {
            soap_str = axiom_mime_parser_create_part(
            env, soap_len, buf_num, len_array, part_start, pos, buf_array, mime_parser);
            if(!soap_str)
            {
                return NULL;
            }

            malloc_len = len_array[buf_num] - search_info->match_len1 - temp_mime_boundary_size;
            if(malloc_len < 0)
            {
                return NULL;
            }    
            else
            {
                buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                    size + 1));

                mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(size + 1);

                memset(buffer, 0, size + 1);
                if(malloc_len > 0)
                {
                    memcpy(buffer, pos + temp_mime_boundary_size, malloc_len);
                }
                if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
                {
                    len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);
                }

                axiom_mime_parser_clear_buffers(env, buf_array, part_start, buf_num);
                if(len >= 0)
                {
                    buf_array[buf_num] = buffer;
                    len_array[buf_num] = malloc_len + len;
                }
                printf("Malloc_len after soap : %d \n", malloc_len);
            }
        }     
        else
        {
            return NULL;
        }
    }    
    else if(search_info->match_len2 > 0)
    {
        soap_len = axiom_mime_parser_calculate_part_len (
            env, buf_num - 1, len_array, part_start, pos, buf_array[buf_num - 1]);

        if(soap_len > 0)
        {
            soap_str = axiom_mime_parser_create_part(
            env, soap_len, buf_num - 1, len_array, part_start, pos, buf_array, mime_parser);
            if(!soap_str)
            {
                return NULL;
            }

            malloc_len = len_array[buf_num] - search_info->match_len2;
            if(malloc_len < 0)
            {
                return NULL;
            }    
            else
            {
                buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                    size + 1));

                mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(size + 1);

                if(malloc_len > 0)
                {
                    memcpy(buffer, buf_array[buf_num] + search_info->match_len2, malloc_len);
                }

                if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
                {
                    len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);
                }
                axiom_mime_parser_clear_buffers(env, buf_array, part_start, buf_num);
                if(len >= 0)
                {
                    buf_array[buf_num] = buffer;
                    len_array[buf_num] = malloc_len + len;
                }
            }
        }     
        else
        {
            return NULL;
        }
    }

    mime_parser->soap_body_str = soap_str;
    mime_parser->soap_body_len = soap_len;

    printf("\n\n%s\n\n", soap_str);

    /*<SOAP></SOAP>--MIMEBOUNDARY
      mime_headr1:.......
      mime_headr2:....

      Binarstart.................
      ...............--MIMEBOUNDARY      
 */
    

    while (!end_of_mime && count < AXIOM_MIME_PARSER_END_OF_MIME_MAX_COUNT)
    {
        /*First we will search for \r\n\r\n*/
 
        search_info->match_len1 = 0;
        search_info->match_len2 = 0;
        pos = NULL;
        part_start = buf_num;

        malloc_len = 0;

        /*We need this count in order to handle cases where the recieving 
        stream does not contain --*/

        count++;       
 
        printf("before crlf method %d\n", len_array[buf_num]);
 
        pos = axiom_mime_parser_search_for_crlf(env, callback, callback_ctx, &buf_num,
            len_array, buf_array, search_info, size, mime_parser);

        printf("after crlf method %d\n", len_array[buf_num]);

        if(!pos)
        {
            return NULL;
        }
        
        /*The pattern contains in one buffer*/
        if(search_info->match_len2 == 0)
        {
            /*We found it . so lets seperates the details of this binary into 
              mime headers.*/

            mime_headers_len = axiom_mime_parser_calculate_part_len (
                env, buf_num, len_array, part_start, pos, buf_array[buf_num]);
            if(mime_headers_len > 0)
            {
                printf("before calculate len method %d\n", len_array[buf_num]);
                mime_headers = axiom_mime_parser_create_part(
                    env, mime_headers_len, buf_num, len_array, part_start, pos, buf_array, mime_parser);
                if(!mime_headers)
                {
                    return NULL;
                }
                printf("after calculate len method %d\n", len_array[buf_num]);
                malloc_len = buf_array[buf_num] + len_array[buf_num] - pos - 4;

                /*This should be > 0 , > 0 means there is some part to copy = 0 means
                there is nothing to copy*/
                if(malloc_len < 0)
                {
                    return NULL;
                }

                else
                {
                    buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                        size + 1));

                    mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(size + 1);
                    

                    if(malloc_len > 0)
                    {
                        memcpy(buffer, pos + 4, malloc_len);
                    }
                    
                    if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
                    {
                        len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);
                    }

                    axiom_mime_parser_clear_buffers(env, buf_array, part_start, buf_num);
                    if(len >= 0)
                    {
                        buf_array[buf_num] = buffer;
                        len_array[buf_num] = malloc_len + len;
                    }
                }
            }     
            else
            {
                return NULL;
            }
        }    

        else if(search_info->match_len2 > 0)
        {
            /*Now we extract the mime headers */

            mime_headers_len = axiom_mime_parser_calculate_part_len (
                env, buf_num - 1, len_array, part_start, pos, buf_array[buf_num - 1]);

            if(mime_headers_len > 0)
            {
                mime_headers = axiom_mime_parser_create_part(
                    env, soap_len, buf_num - 1, len_array, part_start, pos, buf_array, mime_parser);
                if(!mime_headers)
                {
                    return NULL;
                }

                malloc_len = len_array[buf_num] - search_info->match_len2;
                if(malloc_len < 0)
                {
                    return NULL;
                }
                else
                {
                    buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                        size + 1));

                    mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(size + 1);

                    if(malloc_len > 0)
                    {
                        memcpy(buffer, buf_array[buf_num] + search_info->match_len2, malloc_len);
                    }
                    if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
                    {
                        len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);
                    }
                    axiom_mime_parser_clear_buffers(env, buf_array, part_start, buf_num);
                    if(len >= 0)
                    {
                        buf_array[buf_num] = buffer;
                        len_array[buf_num] = malloc_len + len;
                    }
                }
            }     
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }

        pos = NULL;

        search_info->match_len1 = 0;
        search_info->match_len2 = 0;

        part_start = buf_num;
        malloc_len = 0;

        /*We extract the mime headers. So lets search for the attachment.*/

        pos = axiom_mime_parser_search_for_attachment(mime_parser, env, callback, callback_ctx, &buf_num,
          len_array, buf_array, search_info, size, temp_mime_boundary);

        if(pos)
        {
            /*If it is small we are not caching. Hence the attachment 
              is in memory. So store it in a buffer. */

            if(!search_info->cached)
            {
                if(search_info->match_len2 == 0)
                {
                    mime_binary_len = axiom_mime_parser_calculate_part_len (
                        env, buf_num, len_array, part_start, pos, buf_array[buf_num]);
                    if(mime_binary_len > 0)
                    {
                        mime_binary = axiom_mime_parser_create_part(
                            env, mime_binary_len, buf_num, len_array, part_start, pos, buf_array, mime_parser);
                        if(!mime_binary)
                        {
                            return NULL;
                        }
                    }     
                    else
                    {
                        return NULL;
                    }
                }
        
                else if(search_info->match_len2 > 0)
                {
                    mime_binary_len = axiom_mime_parser_calculate_part_len (
                        env, buf_num - 1, len_array, part_start, pos, buf_array[buf_num - 1]);

                    if(mime_binary_len > 0)
                    {
                        mime_headers = axiom_mime_parser_create_part(
                            env, soap_len, buf_num - 1, len_array, part_start, pos, buf_array, mime_parser);
                        if(!mime_headers)
                        {
                            return NULL;
                        }
                    }
                    else
                    {
                        return NULL;
                    }
                }
            }
           
            if(search_info->match_len2 == 0)
            {
                malloc_len = len_array[buf_num] - search_info->match_len1 - temp_mime_boundary_size;
                if(malloc_len < 0)
                {
                    return NULL;
                }
                else
                {
                    buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                        size + 1));

                    mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(size + 1);

                    if(malloc_len > 0)
                    {
                        memcpy(buffer, pos + temp_mime_boundary_size, malloc_len);
                    }
                    
                    /*When the last buffer only containing -- we know this is the end 
                     of the attachments. Hence we don't need to read again*/

                    if(malloc_len != 2)
                    {
                        if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
                        {
                            len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);        
                        }
                        if(len >= 0)
                        {
                            len_array[buf_num] = malloc_len + len;
                        }
                    }
                    else
                    {
                        len_array[buf_num] = malloc_len;
                    }
                    axiom_mime_parser_clear_buffers(env, buf_array, part_start, buf_num);
                    buf_array[buf_num] = buffer;
                }
            }   
            else if(search_info->match_len2 > 0)
            {
                malloc_len = len_array[buf_num] - search_info->match_len2;

                if(malloc_len < 0)
                {
                    return NULL;
                }
                else
                {
                    buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                        size + 1));

                    mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(size + 1);

                    if(malloc_len > 0)
                    {
                        memcpy(buffer, buf_array[buf_num] + search_info->match_len2, malloc_len);
                    }
                    if(malloc_len != 2)
                    {
                        if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
                        {
                            len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);
                        }
                        if(len >= 0)
                        {
                            len_array[buf_num] = malloc_len + len;
                        }
                    }
                    else
                    {
                        len_array[buf_num] = malloc_len;
                    }
                    axiom_mime_parser_clear_buffers(env, buf_array, part_start, buf_num);
                    buf_array[buf_num] = buffer;
                }
            }              
        }
        else
        {
            return NULL;
        }

        /*We have the attachment now either cached or not. So lets process the mime headers.*/        

        if((search_info->cached) && (!mime_parser->caching_callback))
        {
            mime_binary = (axis2_char_t *)search_info->handler;
            mime_binary_len = search_info->binary_size;
        }

        status = axiom_mime_parser_process_mime_headers(env, mime_parser, mime_headers,
            mime_binary, mime_binary_len, search_info->cached);

        /*Check wether we encounter --MIMEBOUNDARY-- to find the end of mime*/

        if(buf_array[buf_num])
        {
            end_of_mime = (AXIOM_MIME_BOUNDARY_BYTE == *(buf_array[buf_num])) &&
                            (AXIOM_MIME_BOUNDARY_BYTE == *(buf_array[buf_num] + 1));
        }

        if(mime_headers)
        {
            AXIS2_FREE(env->allocator, mime_headers);
            mime_headers = NULL;
        }    

        if(status != AXIS2_SUCCESS)
        {
            return NULL;
        }
    }   

    if (buf_array)
    {
        AXIS2_FREE(env->allocator, buf_array);
        buf_array = NULL;
    }

    if (len_array)
    {
        AXIS2_FREE(env->allocator, len_array);
        len_array = NULL;
    }

    if(temp_mime_boundary)
    {
        AXIS2_FREE(env->allocator, temp_mime_boundary);
        temp_mime_boundary = NULL;
    }
    
    if(search_info)
    {
        AXIS2_FREE(env->allocator, search_info);
        search_info = NULL;
    }

    printf("Memory in mime parser--- %d \n", mime_parser->total);

    return mime_parser->mime_parts_map;

}

/*This method will search for \r\n\r\n */

static axis2_char_t *axiom_mime_parser_search_for_crlf(
    const axutil_env_t * env,
    AXIS2_READ_INPUT_CALLBACK callback,
    void *callback_ctx,
    int *buf_num,
    int *len_array,
    axis2_char_t **buf_array,
    axiom_search_info_t *search_info,
    int size,
    axiom_mime_parser_t *mime_parser)
{
    axis2_char_t *found = NULL;
    int len = 0;    

    search_info->search_str = "\r\n\r\n";
    search_info->buffer1 = NULL;
    search_info->buffer2 = NULL;
    search_info->len1 = 0;
    search_info->len2 = 0;
    search_info->match_len1 = 0;
    search_info->match_len2 = 0;
    search_info->primary_search = AXIS2_FALSE;
    search_info->cached = AXIS2_FALSE;
    search_info->handler = NULL;
    search_info->binary_size = 0;

    /*First do a search in the first buffer*/

    if(buf_array[*buf_num])
    {
        search_info->buffer1 = buf_array[*buf_num];
        search_info->len1 = len_array[*buf_num];
        found = axiom_mime_parser_search_string(search_info, env);
    }

    while(!found)
    {
        /*Let's read another buffer and do a boundary search in both*/

        *buf_num = *buf_num + 1;
        buf_array[*buf_num] = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (size + 1));

        mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(size + 1);


        if(buf_array[*buf_num])
        {
            len = callback(buf_array[*buf_num], size, (void *) callback_ctx);
        }
        if(len > 0)
        {
            len_array[*buf_num] = len;
            search_info->buffer2 = buf_array[*buf_num];
            search_info->len2 = len;
            found = axiom_mime_parser_search_string(search_info, env);
        }
        else
        {
            break;
        }
        if(!found)
        {
            /*Let's do a full search in the second buffer*/

            search_info->buffer1 = buf_array[*buf_num];
            search_info->len1 = len_array[*buf_num];
            search_info->primary_search = AXIS2_FALSE;
            search_info->buffer2 = NULL;
            search_info->len2 = 0;
            found = axiom_mime_parser_search_string(search_info, env);
        }
    }

    return found;
}



static axis2_char_t *axiom_mime_parser_search_for_soap(
    const axutil_env_t * env,
    AXIS2_READ_INPUT_CALLBACK callback,
    void *callback_ctx,
    int *buf_num,
    int *len_array,
    axis2_char_t **buf_array,
    axiom_search_info_t *search_info,
    int size,
    axis2_char_t *mime_boundary,
    axiom_mime_parser_t *mime_parser)
{
    axis2_char_t *found = NULL;
    int len = 0;    

    search_info->search_str = mime_boundary;
    search_info->buffer1 = NULL;
    search_info->buffer2 = NULL;
    search_info->len1 = 0;
    search_info->len2 = 0;
    search_info->match_len1 = 0;
    search_info->match_len2 = 0;
    search_info->primary_search = AXIS2_FALSE;

    if(buf_array[*buf_num])
    {
        search_info->buffer1 = buf_array[*buf_num];
        search_info->len1 = len_array[*buf_num];
        found = axiom_mime_parser_search_string(search_info, env);
    }

    while(!found)
    {
        *buf_num = *buf_num + 1;
        buf_array[*buf_num] = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (size + 1));

        mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(size + 1);


        if(buf_array[*buf_num])
        {
            len = callback(buf_array[*buf_num], size, (void *) callback_ctx);
        }
        if(len > 0)
        {
            len_array[*buf_num] = len;
            search_info->buffer2 = buf_array[*buf_num];
            search_info->len2 = len;
            found = axiom_mime_parser_search_string(search_info, env);
        }
        else
        {
            break;
        }
        if(!found)
        {
            search_info->buffer1 = buf_array[*buf_num];
            search_info->len1 = len_array[*buf_num];
            search_info->primary_search = AXIS2_FALSE;
            search_info->buffer2 = NULL;
            search_info->len2 = 0;
            found = axiom_mime_parser_search_string(search_info, env);
        }
    }

    return found;
}

/*The caching is done in this function. Caching happens when we did not 
  find the mime_boundary in initial two buffers. So the maximum size
  that we are keeping in memory is 2 * size. This size can be configurable from
  the aixs.xml. The caching may starts when the search failed with the
  second buffer.  */

static axis2_char_t *axiom_mime_parser_search_for_attachment(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t * env,
    AXIS2_READ_INPUT_CALLBACK callback,
    void *callback_ctx,
    int *buf_num,
    int *len_array,
    axis2_char_t **buf_array,
    axiom_search_info_t *search_info,
    int size,
    axis2_char_t *mime_boundary)
{
    axis2_char_t *found = NULL;
    int len = 0;   
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *temp = NULL;    
    int temp_length = 0;

    search_info->search_str = mime_boundary;
    search_info->buffer1 = NULL;
    search_info->buffer2 = NULL;
    search_info->len1 = 0;
    search_info->len2 = 0;
    search_info->match_len1 = 0;
    search_info->match_len2 = 0;
    search_info->primary_search = AXIS2_FALSE;
    search_info->cached = AXIS2_FALSE;
    search_info->handler = NULL;

    /*First search in the incoming buffer*/

    if(buf_array[*buf_num])
    {
        search_info->buffer1 = buf_array[*buf_num];
        search_info->len1 = len_array[*buf_num];
        found = axiom_mime_parser_search_string(search_info, env);
    }

    while(!found)
    {
        if(search_info->cached)
        {
            if(!(search_info->handler))
            {
                if(mime_parser->callback_name)
                {
                    if(!mime_parser->caching_callback)
                    {
                        search_info->handler = axiom_mime_parser_initiate_callback(mime_parser, env);
                        if(!(search_info->handler))
                        {
                            return NULL;
                        }
                    }
                }
            }

            /*So lets cache the previous buffer which has undergone the
              full search and the partial search.  */

            if(mime_parser->caching_callback)
            {
                status = AXIOM_CACHING_CALLBACK_CACHE(mime_parser->caching_callback, 
                    env, buf_array[*buf_num - 1], len_array[*buf_num - 1], search_info->handler);
            }
            else if(!mime_parser->callback_name)
            {
                status = axiom_mime_parser_cache_to_buffer(env, buf_array[*buf_num - 1], 
                    len_array[*buf_num - 1], search_info, mime_parser);
            }
            else
            {
                return NULL;
            }

            if(status == AXIS2_FAILURE)
            {
                return NULL;
            }
            /*Here we interchange the buffers.*/

            temp = buf_array[*buf_num - 1];    
            buf_array[*buf_num - 1] = buf_array[*buf_num];
            buf_array[*buf_num] = temp;
            temp_length = len_array[*buf_num - 1];
            len_array[*buf_num - 1] = len_array[*buf_num];
            len_array[*buf_num] = temp_length;
            if(buf_array[*buf_num])
            {
                /*The cached buffer is the one which get filled.*/

                printf("requested: %d\n", size);
                len = callback(buf_array[*buf_num], size, (void *) callback_ctx);
                printf("read length: %d\n", len);
            }
        }

        /*Caching is not started yet so we will create a new buffer and fill*/
        else
        {
            *buf_num = *buf_num + 1;
            buf_array[*buf_num] = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (size + 1));

            mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(size + 1);

            if(buf_array[*buf_num])
            {
                printf("Filling to the malloc buf\n");
                printf("requested: %d\n", size);
                len = callback(buf_array[*buf_num], size, (void *) callback_ctx);
                printf("read length: %d\n", len);
            }
        }
        
        /*Doing  a complete search in newly cretaed buffer*/       
 
        if(len > 0)
        {
            len_array[*buf_num] = len;
            search_info->buffer2 = buf_array[*buf_num];
            search_info->len2 = len;
            found = axiom_mime_parser_search_string(search_info, env);
        }
        else
        {
            printf("\nNo data \n");
            break;
        }
                
        if(!found)
        {
            /*So now we must start caching*/
            search_info->buffer1 = buf_array[*buf_num];
            search_info->len1 = len_array[*buf_num];
            search_info->primary_search = AXIS2_FALSE;
            search_info->buffer2 = NULL;
            search_info->len2 = 0;
            found = axiom_mime_parser_search_string(search_info, env);
            if(!found)
            {
                search_info->cached = AXIS2_TRUE;   
            }
        }
    }

    /*Caching of the final buffer*/

    if(search_info->cached)
    {
        if(search_info->match_len2 == 0)
        {
            if(mime_parser->caching_callback)
            {
                status = AXIOM_CACHING_CALLBACK_CACHE(mime_parser->caching_callback,
                    env, buf_array[*buf_num - 1], len_array[*buf_num - 1], search_info->handler);
                if(status == AXIS2_SUCCESS)
                {
                    status = AXIOM_CACHING_CALLBACK_CACHE(mime_parser->caching_callback,
                        env, buf_array[*buf_num], found - buf_array[*buf_num], search_info->handler);
                }
            }
            else if(!mime_parser->callback_name)
            {
                status = axiom_mime_parser_cache_to_buffer(env, buf_array[*buf_num - 1],
                    len_array[*buf_num - 1], search_info, mime_parser);
                if(status == AXIS2_SUCCESS)
                {
                    status = axiom_mime_parser_cache_to_buffer(env, buf_array[*buf_num], 
                        found - buf_array[*buf_num], search_info, mime_parser);
                }
            }
        }
        else if(search_info->match_len2 > 0)
        {
            /*Here the curent buffer has partial mime boundary. So we need 
            to cache only the previous buffer. */
            if(mime_parser->caching_callback)
            {
                status = AXIOM_CACHING_CALLBACK_CACHE(mime_parser->caching_callback,
                    env, buf_array[*buf_num - 1], search_info->match_len1, search_info->handler);
            }
            else if(!mime_parser->callback_name)
            {
                status = axiom_mime_parser_cache_to_buffer(env, buf_array[*buf_num - 1],
                    search_info->match_len1, search_info, mime_parser);
            }
        }
        else
        {
            return NULL;
        }

        if(status == AXIS2_FAILURE)
        {
            return NULL;
        }
    }

    printf("Caching is over \n");
    if(search_info->handler && mime_parser->caching_callback)
    {
        status = AXIOM_CACHING_CALLBACK_CLOSE_HANDLER(mime_parser->caching_callback, env,
            search_info->handler);
        if(status == AXIS2_FAILURE)
        {
            return NULL;
        }
    }
    return found;
}


/*following two functions are used to extract important information 
  for the buffer list. eg: SOAP, MIME_HEADERS*/

/*marker is the starting buffer of the required 
     part  */

static int axiom_mime_parser_calculate_part_len (
    const axutil_env_t *env,
    int buf_num,
    int *len_list,
    int marker,
    axis2_char_t *pos,
    axis2_char_t *buf
)
{
    int part_len = 0;    
    int i = 0;

    for(i = marker; i < buf_num; i++)
    {
        part_len += len_list[i];
    }

    part_len = part_len + (pos - buf);
    
    return part_len;
}

static axis2_char_t * axiom_mime_parser_create_part(
    const axutil_env_t *env,
    int part_len,
    int buf_num,
    int *len_list,
    int marker,
    axis2_char_t *pos,
    axis2_char_t **buf_list,
    axiom_mime_parser_t *mime_parser)
{
    /*We will copy the set of buffers which contains the rrquired part.
     This part can be the SOAP message , mime headers or the mime 
     binary in the case of none cahced.*/

    axis2_char_t *part_str = NULL;
    int i = 0;
    int temp = 0;

    part_str = AXIS2_MALLOC(env->allocator, sizeof(char) * (part_len + 1));
    
    mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(part_len + 1);

    if (!part_str)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Failed in creating buffer");
        return NULL;
    }

    for (i = marker; i < buf_num; i++)
    {
        if (buf_list[i])
        {
            memcpy(part_str + temp, buf_list[i], len_list[i]);
            temp += len_list[i];
        }
    }

    memcpy(part_str + temp, buf_list[i], pos - buf_list[i]);

    part_str[part_len] = '\0';

    return part_str; 
}

AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axiom_mime_parser_get_mime_parts_map(
    axiom_mime_parser_t * mime_parser,
    const axutil_env_t * env)
{
    return mime_parser->mime_parts_map;
}

AXIS2_EXTERN int AXIS2_CALL
axiom_mime_parser_get_soap_body_len(
    axiom_mime_parser_t * mime_parser,
    const axutil_env_t * env)
{
    return mime_parser->soap_body_len;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_mime_parser_get_soap_body_str(
    axiom_mime_parser_t * mime_parser,
    const axutil_env_t * env)
{
    return mime_parser->soap_body_str;
}

/*This is the new search function. This will first do a
search for the entire search string.Then will do a search 
for the partial string which can be divided among two buffers.*/


static axis2_char_t *axiom_mime_parser_search_string(
    axiom_search_info_t *search_info,
    const axutil_env_t *env)
{
    axis2_char_t *pos = NULL;
    axis2_char_t *old_pos = NULL;
    axis2_char_t *found = NULL;
    int str_length = 0;
    int search_length = 0;

    str_length = strlen(search_info->search_str);

    /*First lets search the entire buffer*/
    if(!search_info->primary_search)
    {
        old_pos = search_info->buffer1;

        do
        {
            /*find the first byte. We need to adhere to this
             approach rather than straightaway using strstr
             because the buffer1 can be containg binary data*/

            pos = NULL;

            search_length = search_info->buffer1 + search_info->len1
                                - old_pos - str_length + 1;

            if(search_length < 0)
            {
                printf("\nSearch lenth is mimus\n");
                break;
            }

            if (old_pos)
            {
                /*pos = memchr(old_pos, *(search_info->search_str),
                        search_info->buffer1 + search_info->len1
                        - old_pos - str_length + 1);*/
                pos = memchr(old_pos, *(search_info->search_str),
                        search_length);
            }

            /* found it so lets check the remaining */

            if (pos)
            {
                found = axutil_strstr(pos, search_info->search_str);
                if(found)
                {
                    search_info->match_len1 = found - search_info->buffer1;
                    printf("Hit primary search\n");
                    break;
                }
                else
                {
                    old_pos = pos + 1;
                }
            }
        }
        while (pos);
    }

    search_info->primary_search = AXIS2_TRUE;

    if(found)
    {
        return found;
    }

    /*So we didn't find the string in the buffer
     lets check whether it is divided in two buffers*/

    else
    {
        int offset = 0;
        pos = NULL;
        old_pos = NULL;
        found = NULL;
        search_length = 0;

        if(search_info->buffer2)
        {
            old_pos = search_info->buffer1 + search_info->len1 - str_length + 1;
            do
            {
                /*First check the starting byte*/
                pos = NULL;
                found = NULL;

                search_length = search_info->buffer1 + search_info->len1 - old_pos;

                if(search_length < 0)
                {
                    break;
                }

                /*pos = memchr(old_pos, *(search_info->search_str),
                search_info->buffer1 + search_info->len1 - old_pos);*/

                pos = memchr(old_pos, *(search_info->search_str), search_length);

                if(pos)
                {
                    offset = search_info->buffer1 + search_info->len1 - pos;

                    /*First match the beginng to offset in buffer1*/

                    if(offset > 0)
                    {
                        if(memcmp(pos, search_info->search_str, offset)
                          == 0)
                        {
                            found = pos;
                        }

                        /*We found something in buffer1 so lets match the
                          remaining in buffer2*/

                        if(found)
                        {
                            if(memcmp(search_info->buffer2, search_info->search_str + offset,
                               str_length - offset ) == 0)
                            {
                                search_info->match_len2 = str_length - offset;
                                search_info->match_len1 = found - search_info->buffer1;
                                printf("Hit secondary search\n");
                                break;
                            }
                            else
                            {
                                old_pos = pos + 1;
                            }
                        }
                        else
                        {
                            old_pos = pos + 1;
                        }
                    }
                }
            }
            while(pos);
            search_info->primary_search = AXIS2_FALSE;

            return found;
        }
        else
        {
            return NULL;
        }
    }
}

/*This will parse the string containing the mime header
information and extract necessary details.*/


static axis2_status_t 
axiom_mime_parser_process_mime_headers(
    const axutil_env_t *env,
    axiom_mime_parser_t *mime_parser,
    axis2_char_t *mime_headers,
    axis2_char_t *mime_binary,
    int mime_binary_len,
    axis2_bool_t cached)
{
	if (mime_parser->mime_parts_map)
    {
        axis2_char_t *id = NULL;
        axis2_char_t *type = NULL;
        axis2_char_t *pos = NULL;        

        /* Get the MIME ID */
        if (mime_headers)
        {
            id = axutil_strcasestr(mime_headers, AXIOM_MIME_HEADER_CONTENT_ID);
            type = axutil_strcasestr(mime_headers,
                AXIOM_MIME_HEADER_CONTENT_TYPE);
            if (type)
            {
                axis2_char_t *end = NULL;
                axis2_char_t *temp_type = NULL;
                type += axutil_strlen(AXIOM_MIME_HEADER_CONTENT_TYPE);
                while (type && *type && *type != ':')
                {
                    type++;
                }
                type++;
                while (type && *type && *type == ' ')
                {
                    type++;
                }
                end = type;
                while (end && *end && !isspace(*end))
                {
                    end++;
                }
                if ((end - type) > 0)
                {
                    temp_type = AXIS2_MALLOC(env->allocator,
                        sizeof(axis2_char_t) * ((end - type) + 1));
                    if (!temp_type)
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "No memory. Failed in creating Content-Type");
                        return AXIS2_FAILURE;
                    }
                    memcpy(temp_type, type, (end - type));
                    temp_type[end - type] = '\0';
                    type = temp_type;
                }
            }
        }
        if (id)
        {
            id += axutil_strlen(AXIOM_MIME_HEADER_CONTENT_ID);
            while (id && *id && *id != ':')
            {
                id++;
            }
            if (id)
            {
                while (id && *id && *id != '<')
                {
                    id++;
                }
                id++;
                pos = axutil_strstr(id, ">");
                if (pos)
                {
                    axis2_char_t *mime_id = NULL;
                    int mime_id_len = 0;
                    mime_id_len = (int)(pos - id);
                    mime_id = AXIS2_MALLOC(env->allocator,
                        sizeof(axis2_char_t) * mime_id_len + 1); 
                    /* The MIME ID will be freed by the SOAP builder */
                    if (mime_id)
                    {
                        axiom_data_handler_t *data_handler = NULL;
                        memcpy(mime_id, id, mime_id_len);
                        mime_id[mime_id_len] = '\0';
                        data_handler =
                            axiom_data_handler_create(env, NULL, type);

                        AXIS2_FREE(env->allocator, type);

                        if(!mime_binary && cached)
                        {
                            axiom_data_handler_set_cached(data_handler, env, AXIS2_TRUE);    
                        }
                        else if(mime_binary)
                        {
                            axiom_data_handler_set_binary_data(data_handler,
                                env, mime_binary, mime_binary_len - 2);
                        }
                        else
                        {
                            return AXIS2_FAILURE;
                        }
                        axutil_hash_set(mime_parser->mime_parts_map,
                                mime_id, AXIS2_HASH_KEY_STRING, data_handler);
                    }
                    else
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "No memory. Failed in creating MIME ID");
                        return AXIS2_FAILURE;
                    }
                }
            }
        }
        else
        {
            /*axis2_char_t temp_boundry[1024];
            sprintf(temp_boundry, "--%s--", mime_boundary);
            if (body_mime && axutil_strstr(body_mime, temp_boundry))
            {
                break;
            }*/
            return AXIS2_FAILURE;
        }
        return AXIS2_SUCCESS;
    }	
    else
    {
        return AXIS2_FAILURE;
    }    
}

/*This is used to free some unwanted buffers. For example we did
not want the buffers which contains the data before the soap
envelope starts. */

static void axiom_mime_parser_clear_buffers(
    const axutil_env_t *env,
    axis2_char_t **buf_list,
    int free_from,
    int free_to)
{
    int i = 0;

    for(i = free_from; i <= free_to; i++)
    {
        if(buf_list[i])
        {
            AXIS2_FREE(env->allocator, buf_list[i]);
            buf_list[i] = NULL;
        }   
    }
    return;
}


static axis2_status_t axiom_mime_parser_cache_to_buffer(
    const axutil_env_t *env,
    axis2_char_t *buf,
    int buf_len,
    axiom_search_info_t *search_info,
    axiom_mime_parser_t *mime_parser)
{
    axis2_char_t *data_buffer = NULL;
    axis2_char_t *temp_buf = NULL;
    int mime_binary_len = 0;

    temp_buf = (axis2_char_t *)search_info->handler;
    mime_binary_len = search_info->binary_size + buf_len;

    if(mime_binary_len > 0)
    {
        data_buffer = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_char_t) * (mime_binary_len));

        mime_parser->total = mime_parser->total + sizeof(axis2_char_t) *(mime_binary_len);        


        if(data_buffer)
        {
            if(temp_buf && search_info->binary_size > 0)
            {
                memcpy(data_buffer, temp_buf, search_info->binary_size);
                AXIS2_FREE(env->allocator, temp_buf);
                temp_buf = NULL;
            }
            memcpy(data_buffer + (search_info->binary_size), 
                buf, buf_len);
            search_info->binary_size = mime_binary_len;
            search_info->handler = (void *)data_buffer;

            return AXIS2_SUCCESS;
        }
        else
        {
            return AXIS2_FAILURE;
        }
    }
    else
    {
        return AXIS2_FAILURE;
    }
}


AXIS2_EXTERN void AXIS2_CALL
axiom_mime_parser_set_buffer_size(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    int size)
{
    mime_parser->buffer_size = size;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_mime_parser_set_max_buffers(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    int num)
{
    mime_parser->max_buffers = num;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_mime_parser_set_caching_callback_name(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    axis2_char_t *callback_name)
{
    mime_parser->callback_name = callback_name;
}

static void* axiom_mime_parser_initiate_callback(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env)
{
    axutil_dll_desc_t *dll_desc = NULL;
    axutil_param_t *impl_info_param = NULL;
    void *ptr = NULL;

    if(mime_parser->callback_name)
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Trying to load module = %s", 
                mime_parser->callback_name);
        dll_desc = axutil_dll_desc_create(env);
        axutil_dll_desc_set_name(dll_desc, env, mime_parser->callback_name);
        impl_info_param = axutil_param_create(env, NULL, dll_desc);
        /*Set the free function*/
        axutil_param_set_value_free(impl_info_param, env, axutil_dll_desc_free_void_arg);
        axutil_class_loader_init(env);
        ptr = axutil_class_loader_create_dll(env, impl_info_param);

        if (!ptr)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Unable to load the module %s. ERROR", mime_parser->callback_name);
            return NULL;
        }

        mime_parser->caching_callback =  (axiom_caching_callback_t *)ptr;
        mime_parser->caching_callback->param = impl_info_param;

        return AXIOM_CACHING_CALLBACK_INIT_HANDLER(mime_parser->caching_callback, env);
    }

    else
    {
        return NULL;
    }

}

static axis2_bool_t axiom_mime_parser_is_more_data(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    axis2_callback_info_t *callback_info)
{
    if(callback_info->chunked_stream)
    {
        if(axutil_http_chunked_stream_get_end_of_chunks(
            callback_info->chunked_stream, env))
        {
            return AXIS2_FALSE;
        }
        else
        {
            return AXIS2_TRUE;
        }
    }

    else if(callback_info->unread_len == 0)
    {
        return AXIS2_FALSE;
    }
    else
    {
        return AXIS2_TRUE;
    }
}
