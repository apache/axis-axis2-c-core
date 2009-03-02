
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
#include <ctype.h>    
#include <axutil_http_chunked_stream.h>
#include <axiom_mtom_caching_callback.h>
#include <axutil_class_loader.h>
#include <axutil_url.h>

struct axiom_mime_parser
{
    /* This will keep the attachment and its info*/
    axutil_hash_t *mime_parts_map;

    /* This is the actual SOAP part len */
    int soap_body_len;

    /* The SOAP part of the message */
    axis2_char_t *soap_body_str;

    /* The size of the buffer we give to the callback to
     * read data */
    int buffer_size;

    /* The number of buffers */
    int max_buffers;

    /* The attachment dir name, in the case of caching */
    axis2_char_t *attachment_dir;

    /*A pointer to the caching callback */
    axiom_mtom_caching_callback_t *mtom_caching_callback;

    /* The caching callback name specified */
    axis2_char_t *callback_name;

    axis2_char_t **buf_array;

    int *len_array;

    int current_buf_num;

    axis2_bool_t end_of_mime;

    axis2_char_t *mime_boundary;

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
    axis2_char_t *mime_boundary,
    axis2_char_t *mime_id,
    void *user_param);

static axis2_status_t axiom_mime_parser_store_attachment(
    const axutil_env_t *env,
    axiom_mime_parser_t *mime_parser,
    axis2_char_t *mime_id,
    axis2_char_t *mime_type,
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


static axis2_bool_t axiom_mime_parser_is_more_data(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    axis2_callback_info_t *callback_info);

static axis2_char_t *
    axiom_mime_parser_process_mime_headers(
    const axutil_env_t *env,
    axiom_mime_parser_t *mime_parser,
    axis2_char_t **mime_id,
    axis2_char_t *mime_headers);

static axis2_status_t 
axiom_mime_parser_cache_to_file(
    const axutil_env_t* env,
    axis2_char_t *buf,
    int buf_len,
    void *handler);

static void* axiom_mime_parser_initiate_callback(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    axis2_char_t *mime_id,
    void *user_param);



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
    mime_parser->attachment_dir = NULL;
    mime_parser->mtom_caching_callback = NULL;
    mime_parser->callback_name = NULL;
    mime_parser->buf_array = NULL;
    mime_parser->len_array = NULL;
    mime_parser->current_buf_num = 0;
    mime_parser->end_of_mime = AXIS2_FALSE;
    mime_parser->mime_boundary = NULL;

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

    /* The map is passed on to SOAP builder, and SOAP builder take over the
       ownership of the map */

    /* We will unload the callback at the end */

    if(mime_parser->mtom_caching_callback)
    {
        axutil_param_t *param = NULL;
        param = mime_parser->mtom_caching_callback->param;

        AXIOM_MTOM_CACHING_CALLBACK_FREE(mime_parser->mtom_caching_callback, env);
        mime_parser->mtom_caching_callback = NULL;

        if(param)
        {
            axutil_param_free(param, env);
            param = NULL;
        }
    }

    if(mime_parser->buf_array)
    {
        AXIS2_FREE(env->allocator, mime_parser->buf_array);
        mime_parser->buf_array = NULL;
    }
    
    if(mime_parser->len_array)
    {
        AXIS2_FREE(env->allocator, mime_parser->len_array);
        mime_parser->len_array = NULL;
    }

    if (mime_parser)
    {
        AXIS2_FREE(env->allocator, mime_parser);
    }

    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_mime_parser_parse_for_soap(
    axiom_mime_parser_t * mime_parser,
    const axutil_env_t * env,
    AXIS2_READ_INPUT_CALLBACK callback,
    void *callback_ctx,
    axis2_char_t * mime_boundary)
{
    int size = 0;
    axis2_char_t *soap_str = NULL;
    int soap_len = 0;
    int temp_mime_boundary_size = 0;
    axis2_char_t *temp_mime_boundary = NULL;
    axis2_char_t **buf_array = NULL;
    int *len_array = NULL;
    int buf_num = 0;
    axis2_char_t *pos = NULL;
    axiom_search_info_t *search_info = NULL;
    int part_start = 0;
    axis2_bool_t end_of_mime = AXIS2_FALSE;
    int len = 0;
    axis2_char_t *buffer = NULL;
    int malloc_len = 0;
    axis2_callback_info_t *callback_info = NULL;

    callback_info = (axis2_callback_info_t *)callback_ctx;

    /* The user will specify the mime_parser->buffer_size */

    size = AXIOM_MIME_PARSER_BUFFER_SIZE * (mime_parser->buffer_size); 

    /*An array to keep the set of buffers*/

    buf_array = AXIS2_MALLOC(env->allocator,
        sizeof(axis2_char_t *) * (mime_parser->max_buffers));

    if (!buf_array)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Failed in creating buffer array");
        return AXIS2_FAILURE;
    }

    /*Keeps the corresponding lengths of buffers in buf_array*/

    len_array = AXIS2_MALLOC(env->allocator,
        sizeof(int) * (mime_parser->max_buffers));


    if (!len_array)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Failed in creating length array");
        return AXIS2_FAILURE;
    }

    mime_parser->buf_array = buf_array;
    mime_parser->len_array = len_array;

    temp_mime_boundary = axutil_stracat(env, "--", mime_boundary);
    temp_mime_boundary_size = strlen(mime_boundary) + 2;

    /*This struct keeps the pre-post search informations*/
    search_info = AXIS2_MALLOC(env->allocator,
        sizeof(axiom_search_info_t));

    /* The first buffer is created */
    buf_array[buf_num] = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_char_t) * (size + 1));

    /* The buffer is filled from the callback */

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
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Error reading from the stream");
        return AXIS2_FAILURE;
    }

    /*starting buffer for the current search*/
    part_start = buf_num;

    /*We are passing the address of the buf_num , beacause that value 
    is changing inside the method.*/

    /* Following call to the method will search first \r\n\r\n */

    pos = axiom_mime_parser_search_for_crlf(env, callback, callback_ctx, &buf_num,
            len_array, buf_array, search_info, size, mime_parser);

    if(!pos)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Error in the message.");
        return AXIS2_FAILURE;
    }   

    /* The patteren contains in one buffer */

    if((search_info->match_len2 == 0))
    {
        /*Readjusting the buffers for the next search and discarding the prevoius 
        buffers*/

        /* We need the remaining part in the buffer after the \r\n\r\n*/

        malloc_len = buf_array[buf_num] + len_array[buf_num] - pos - 4;
        if(malloc_len < 0)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Error in parsing.");
            return AXIS2_FAILURE;
        }
        else    
        {
            /* Here we will create a new buffer of predefined size fill the 
             * first portion from the remaining part after previous search
             * and then fill the remaining from the callback */

            buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                size + 1));

            if(malloc_len > 0)
            {
                memcpy(buffer, pos + 4, malloc_len);
            }

            /* Here we need to check for more data, because if the message is too small
             * comapred to the reading size there may be no data in the stream , instead
             * all the remaining data may be in the buffer.And if there are no more data
             * we will set the len to be 0. Otherwise len_array will contain wrong lenghts.
             */            

            if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
            {
                /* There is more data so fill the remaining from the  stream*/

                len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);           
            }
            else
            {
                len = 0;
            }            

            /* We do not need the data in the previous buffers once we found a particular
             * string and after worked with those buffers */
    
            axiom_mime_parser_clear_buffers(env, buf_array, part_start, buf_num);

            /* Adding the new buffer to the buffer list */

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
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Error in parsing.");
            return AXIS2_FAILURE;
        } 
        else
        {
            buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                size + 1));

            /* Here the buf_num is the second buffer. We will copy the remaining data
             * after the partial string in the second buffer */

            if(malloc_len > 0)
            {
                memcpy(buffer, buf_array[buf_num] + search_info->match_len2, malloc_len);        
            }
            if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
            {
                len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);
            }
            else
            {
                len = 0;
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
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Error in parsing.");
        return AXIS2_FAILURE;
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
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Error while searching for the SOAP part ");
        return AXIS2_FAILURE;
    }
    
    if(search_info->match_len2 == 0)
    {
        /*Calculating the length of the SOAP str*/

        soap_len = axiom_mime_parser_calculate_part_len (
            env, buf_num, len_array, part_start, pos, buf_array[buf_num]);
        if(soap_len > 0)
        {
            /* Get the SOAP string from the starting and end buffers containing 
             * the SOAP  part */

            soap_str = axiom_mime_parser_create_part(
            env, soap_len, buf_num, len_array, part_start, pos, buf_array, mime_parser);
            if(!soap_str)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Error while creating the SOAP part from the message ");
                return AXIS2_FAILURE;
            }

            malloc_len = len_array[buf_num] - search_info->match_len1 - temp_mime_boundary_size;
            if(malloc_len < 0)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Error in parsing for mime.");
                return AXIS2_FAILURE;
            }    
            else
            {
                /* This will fill the new buffer with remaining data after the
                 * SOAP */

                buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                    size + 1));

                memset(buffer, 0, size + 1);
                if(malloc_len > 0)
                {
                    memcpy(buffer, pos + temp_mime_boundary_size, malloc_len);
                }
                if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
                {
                    len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);
                }
                else
                {
                    len = 0;
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
            return AXIS2_FAILURE;
        }
    }    

    /* This is the condition where the --MIMEBOUNDARY is devided among two
     * buffers */

    else if(search_info->match_len2 > 0)
    {
        soap_len = axiom_mime_parser_calculate_part_len (
            env, buf_num - 1, len_array, part_start, pos, buf_array[buf_num - 1]);

        if(soap_len > 0)
        {
            /* Here we pass buf_num-1 becasue buf_num does not have any thing we want to
             * for this particular part. It begins with the latter part of the search string */

            soap_str = axiom_mime_parser_create_part(
            env, soap_len, buf_num - 1, len_array, part_start, pos, buf_array, mime_parser);
            if(!soap_str)
            {
                return AXIS2_FAILURE;
            }

            malloc_len = len_array[buf_num] - search_info->match_len2;
            if(malloc_len < 0)
            {
                return AXIS2_FAILURE;
            }    
            else
            {
                buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                    size + 1));

                if(malloc_len > 0)
                {
                    memcpy(buffer, buf_array[buf_num] + search_info->match_len2, malloc_len);
                }

                if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
                {
                    len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);
                }
                else
                {
                    len = 0;
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
            return AXIS2_FAILURE;
        }
    }

    mime_parser->soap_body_str = soap_str;
    mime_parser->soap_body_len = soap_len;
    mime_parser->current_buf_num = buf_num;

    /* There are multipart/related messages which does not contain attachments 
     * The only mime_part is the soap envelope. So for those messages the mime
     * boundary after the soap will end up with --
     * So we will check that here and if we found then the logic inside the 
     * while loop will not be executed */

   end_of_mime = (AXIOM_MIME_BOUNDARY_BYTE == *(buf_array[buf_num])) &&
                            (AXIOM_MIME_BOUNDARY_BYTE == *(buf_array[buf_num] + 1));
    if(end_of_mime)
    {
        AXIS2_FREE(env->allocator, buf_array[buf_num]);
        buf_array[buf_num] = NULL;
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

    mime_parser->end_of_mime = end_of_mime;

    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axiom_mime_parser_parse_for_attachments(
    axiom_mime_parser_t * mime_parser,
    const axutil_env_t * env,
    AXIS2_READ_INPUT_CALLBACK callback,
    void *callback_ctx,
    axis2_char_t * mime_boundary,
    void *user_param)
{
    int count = 0;
    axiom_search_info_t *search_info = NULL;
    axis2_char_t *pos = NULL;
    int part_start = 0;
    axis2_char_t **buf_array = NULL;
    int *len_array = NULL;
    int buf_num = 0;
    int size = 0;
    int malloc_len = 0;
    axis2_callback_info_t *callback_info = NULL;
    axis2_char_t *temp_mime_boundary = NULL;
    int temp_mime_boundary_size = 0;
    axis2_bool_t end_of_mime = AXIS2_FALSE;

    callback_info = (axis2_callback_info_t *)callback_ctx;

    search_info = AXIS2_MALLOC(env->allocator,
        sizeof(axiom_search_info_t));

    size = AXIOM_MIME_PARSER_BUFFER_SIZE * (mime_parser->buffer_size);

    buf_array = mime_parser->buf_array;
    len_array = mime_parser->len_array;
    buf_num = mime_parser->current_buf_num;


    /*<SOAP></SOAP>--MIMEBOUNDARY
      mime_headr1:.......
      mime_headr2:....

      Binarstart.................
      ...............--MIMEBOUNDARY      
    */
    
    /* This loop will extract all the attachments in the message. The condition
     * with the count is needed because if the sender not marked the end of the 
     * attachment wiht -- then this loop may run infinitely. To prevent that
     * this additional condition has been put */

    temp_mime_boundary = axutil_stracat(env, "--", mime_boundary);
    temp_mime_boundary_size = strlen(mime_boundary) + 2;


    while ((!(mime_parser->end_of_mime)) && count < AXIOM_MIME_PARSER_END_OF_MIME_MAX_COUNT)
    {
        /*First we will search for \r\n\r\n*/
        axis2_char_t *mime_id = NULL;
        axis2_char_t *mime_type = NULL; 
        int mime_headers_len = 0;
        int mime_binary_len = 0;
        axis2_char_t *mime_binary = NULL;
        axis2_char_t *mime_headers = NULL;
        axis2_char_t *buffer = NULL;
        int len = 0;    
        axis2_status_t status = AXIS2_FAILURE;
     
        search_info->match_len1 = 0;
        search_info->match_len2 = 0;
        pos = NULL;
        part_start = buf_num;

        malloc_len = 0;

        count++;       
 
        pos = axiom_mime_parser_search_for_crlf(env, callback, callback_ctx, &buf_num,
            len_array, buf_array, search_info, size, mime_parser);

        if(!pos)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Error in parsing for mime.");
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
                mime_headers = axiom_mime_parser_create_part(
                    env, mime_headers_len, buf_num, 
                    len_array, part_start, pos, 
                    buf_array, mime_parser);

                if(!mime_headers)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Error in parsing for mime headers.");
                    return NULL;
                }
                malloc_len = buf_array[buf_num] + len_array[buf_num] - pos - 4;

                /*This should be > 0 , > 0 means there is some part to copy = 0 means
                there is nothing to copy*/
                if(malloc_len < 0)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Error in parsing for mime headers");
                    return NULL;
                }

                else
                {
                    buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (
                        size + 1));

                    if(malloc_len > 0)
                    {
                        memcpy(buffer, pos + 4, malloc_len);
                    }
                    
                    if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
                    {
                        len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);
                    }
                    else
                    {
                        len = 0;
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
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Error in parsing for mime headers.");
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
                    env, mime_headers_len, buf_num - 1, len_array, part_start, pos, buf_array, mime_parser);
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

                    if(malloc_len > 0)
                    {
                        memcpy(buffer, buf_array[buf_num] + search_info->match_len2, malloc_len);
                    }
                    if(axiom_mime_parser_is_more_data(mime_parser, env, callback_info))
                    {
                        len = callback(buffer + malloc_len, size - malloc_len, (void *) callback_ctx);
                    }
                    else
                    {
                        len = 0;
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

        mime_type = axiom_mime_parser_process_mime_headers(env, mime_parser, &mime_id, mime_headers);
        
        if(!mime_id )
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Error in parsing for mime headers.Mime id did not find");
            return NULL;
        }
        
        if(!mime_type)
        {
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "Mime type did not find");
        }

        /*We extract the mime headers. So lets search for the attachment.*/

        pos = axiom_mime_parser_search_for_attachment(mime_parser, env, callback, callback_ctx, &buf_num,
          len_array, buf_array, search_info, size, temp_mime_boundary, mime_id, user_param);

        if(pos)
        {
            /*If it is small we are not caching. Hence the attachment 
              is in memory. So store it in a buffer. */

            if(!search_info->cached)
            {
                if(search_info->match_len2 == 0)
                {
                    /* mime_binary contains the attachment when it does not 
                     * cached */

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
                        mime_binary = axiom_mime_parser_create_part(
                            env, mime_binary_len, buf_num - 1, len_array, part_start, pos, buf_array, mime_parser);
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
            }
           
            /* The functionality below is common when it is cached or not. It deals with remaining
             * after a particualr attachment, Those may be related to a end of mime_boundary or 
             * another attachment */

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
                        else
                        {
                            len = 0;
                        }
                        if(len >= 0)
                        {
                            len_array[buf_num] = malloc_len + len;
                        }
                    }

                    /* This means there is another attachment */
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
                        else
                        {
                            len = 0;
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

        /*We have the attachment now either cached or not. So lets put it in the mime_parts 
         * hash map with the mime_id. Remember at this moment we have already processed the 
         * mime_headers and mime_id is already there */        

        /* In the case user has not specified the callback or the attachment dir . So we cached it to a memory
         * buffer. Hence the data_handler type we need to create is different */

        if((search_info->cached) && (!mime_parser->attachment_dir) && 
            (!mime_parser->callback_name))
        {
            mime_binary = (axis2_char_t *)search_info->handler;
            mime_binary_len = search_info->binary_size;
        }

        /* Storing the attachment in the hash map with the id*/

        status = axiom_mime_parser_store_attachment(env, mime_parser, mime_id,
            mime_type, mime_binary, mime_binary_len, search_info->cached);

        /*Check wether we encounter --MIMEBOUNDARY-- to find the end of mime*/

        if(buf_array[buf_num])
        {
            /* Here we check for the end of mime */            

            end_of_mime = (AXIOM_MIME_BOUNDARY_BYTE == *(buf_array[buf_num])) &&
                            (AXIOM_MIME_BOUNDARY_BYTE == *(buf_array[buf_num] + 1));
            if(end_of_mime)
            {
                AXIS2_FREE(env->allocator, buf_array[buf_num]);
                buf_array[buf_num] = NULL;
            }
            mime_parser->end_of_mime = end_of_mime;
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

    /*Do the necessary cleaning */

    /*if (buf_array)
    {
        AXIS2_FREE(env->allocator, buf_array);
        buf_array = NULL;
    }

    if (len_array)
    {
        AXIS2_FREE(env->allocator, len_array);
        len_array = NULL;
    }*/

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

/* This method will search for the mime_boundary after the SOAP part 
 * of the message */

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
    
    /* What we need to search is the mime_boundary */

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

        /* Inside this search primary_search flag will be set to TRUE */
    }

    while(!found)
    {
        /* We need to create the second buffer and do the search for the 
         * mime_boundary in the both the buffers */

        *buf_num = *buf_num + 1;
        buf_array[*buf_num] = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (size + 1));

        if(buf_array[*buf_num])
        {
            len = callback(buf_array[*buf_num], size, (void *) callback_ctx);
        }
        if(len > 0)
        {
            /* In this search we are matching end part of the first
             * buffer and starting part of the previous buffer */
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
  second buffer.  
  In this logic first we will search for a callback to cache. If it is not 
  there then we will search for a directory to save the file. If it is also
  not there then the attachment will be in memory.      
*/

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
    axis2_char_t *mime_boundary,
    axis2_char_t *mime_id,
    void *user_param)
{
    axis2_char_t *found = NULL;
    int len = 0;   
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *temp = NULL;    
    int temp_length = 0;
    axis2_char_t *file_name = NULL;

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
            if(mime_parser->callback_name)
            {
                if(!(search_info->handler))
                {
                    /* If the callback is not loaded yet then we load it*/
                    if(!mime_parser->mtom_caching_callback)
                    {
                        search_info->handler = axiom_mime_parser_initiate_callback(mime_parser, env, mime_id, user_param);
                        if(!(search_info->handler))
                        {
                            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "Caching Callback is not loaded");
                            return NULL;
                        }
                    }
                }
                /*So lets cache the previous buffer which has undergone the
                  full search and the partial search.  */

                if(mime_parser->mtom_caching_callback)
                {
                    /* Caching callback is loaded. So we can cache the previous buffer */
                    status = AXIOM_MTOM_CACHING_CALLBACK_CACHE(mime_parser->mtom_caching_callback,
                        env, buf_array[*buf_num - 1], len_array[*buf_num - 1], search_info->handler);
                }
            }

            else if(mime_parser->attachment_dir)
            {
                if(!(search_info->handler))
                {
                    /* If the File is not opened yet we will open it*/
                    
                    axis2_char_t *encoded_mime_id = NULL;

                    /* Some times content-ids urls, hence we need to encode them 
                     * becasue we can't create files with / */

                    encoded_mime_id = AXIS2_MALLOC(env->allocator,
                        (sizeof(axis2_char_t))* (strlen(mime_id)));
                    memset(encoded_mime_id, 0, strlen(mime_id));
                    encoded_mime_id = axutil_url_encode(env, encoded_mime_id,
                        mime_id, strlen(mime_id));
                    if(!encoded_mime_id)
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "Mime Id encoding failed");
                        return NULL;
                    }

                    file_name = axutil_stracat(env, mime_parser->attachment_dir, 
                        encoded_mime_id);
                    AXIS2_FREE(env->allocator, encoded_mime_id);
                    encoded_mime_id = NULL;
            
                    if(!file_name)
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "Caching file name creation error");
                        return NULL;
                    }
                    search_info->handler = (void *)fopen(file_name, "ab+");
                    if(!(search_info->handler))
                    {
                            return NULL;
                    }
                }
            
                /*So lets cache the previous buffer which has undergone the
                full search and the partial search.  */

                status = axiom_mime_parser_cache_to_file(env, buf_array[*buf_num - 1], 
                    len_array[*buf_num - 1], search_info->handler);            
            }

            else
            {
                /* Here the user has not specified the caching File location. So we are 
                 * not going to cache. Instead we store the attachment in the buffer */
                status = axiom_mime_parser_cache_to_buffer(env, buf_array[*buf_num - 1], 
                    len_array[*buf_num - 1], search_info, mime_parser);
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
                len = callback(buf_array[*buf_num], size, (void *) callback_ctx);
            }
        }

        /*Size of the data in memory not yet risen to the caching threasold
         *So we can create the second buffer */
        else
        {
            *buf_num = *buf_num + 1;
            buf_array[*buf_num] = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (size + 1));

            if(buf_array[*buf_num])
            {
                len = callback(buf_array[*buf_num], size, (void *) callback_ctx);
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
            break;
        }

        /* Now there are two buffers. If the searching string is not 
         * here then we must cache the first buffer */
                
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
                /* So at the begining of the next search we start
                 * that only after caching this data */
                search_info->cached = AXIS2_TRUE;   
            }
        }
    }

    /* Here we are out of the loop. If there is no error then this means 
     * the searching string is found */

    if(search_info->cached)
    {
        /* If the attachment is cached then we need to cache the 
         * final buffer */

        if(search_info->match_len2 == 0)
        {
            /* This is the case where we found the whole string in one buffer 
             * So we need to cache previous buffer and the data up to the starting
             * point of the search string in the current buffer */

            if(mime_parser->mtom_caching_callback)
            {
                status = AXIOM_MTOM_CACHING_CALLBACK_CACHE(mime_parser->mtom_caching_callback,
                    env, buf_array[*buf_num - 1], len_array[*buf_num - 1], search_info->handler);
                if(status == AXIS2_SUCCESS)
                {
                    status = AXIOM_MTOM_CACHING_CALLBACK_CACHE(mime_parser->mtom_caching_callback,
                        env, buf_array[*buf_num], found - buf_array[*buf_num], search_info->handler);
                }
            }

            else if(mime_parser->attachment_dir)
            {
                status = axiom_mime_parser_cache_to_file(env, buf_array[*buf_num - 1], 
                    len_array[*buf_num - 1], search_info->handler);
                if(status == AXIS2_SUCCESS)
                {
                    status = axiom_mime_parser_cache_to_file(env, buf_array[*buf_num], 
                        found - buf_array[*buf_num], search_info->handler);
                }
            }
            
            /* If the callback or a file is not there then the data is appended to the buffer */

            else
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

            if(mime_parser->mtom_caching_callback)
            {
                status = AXIOM_MTOM_CACHING_CALLBACK_CACHE(mime_parser->mtom_caching_callback,
                    env, buf_array[*buf_num - 1], search_info->match_len1, search_info->handler);
            }

            else if(mime_parser->attachment_dir)
            {
                status = axiom_mime_parser_cache_to_file(env, buf_array[*buf_num - 1], 
                    search_info->match_len1, search_info->handler);
            }
            else
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

    /* Parsing is done so lets close the relative handlers */

    if(search_info->handler)
    {
        if(mime_parser->mtom_caching_callback)
        {
            status = AXIOM_MTOM_CACHING_CALLBACK_CLOSE_HANDLER(mime_parser->mtom_caching_callback, env,
                search_info->handler);
            if(status == AXIS2_FAILURE)
            {
                return NULL;
            }
        }

        else if(mime_parser->attachment_dir)
        {
            if(fclose((FILE *)(search_info->handler)) == 0)
            {
                status = AXIS2_SUCCESS;
            }
            else
            {
                status = AXIS2_FAILURE;
            }

            AXIS2_FREE(env->allocator, file_name);
            file_name = NULL;        

            if(status == AXIS2_FAILURE)
            {
                return NULL;
            }
        }
    }
    return found;
}


/*following two functions are used to extract important information 
  from the buffer list. eg: SOAP, MIME_HEADERS*/

/*marker is the starting buffer of the required 
     part and pos is the end point of that part  */

static int axiom_mime_parser_calculate_part_len (
    const axutil_env_t *env,
    int buf_num,
    int *len_list,
    int marker,
    axis2_char_t *pos,
    axis2_char_t *buf)
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
    /*We will copy the set of buffers which contains the required part.
     This part can be the SOAP message , mime headers or the mime 
     binary in the case of none cahced.*/

    axis2_char_t *part_str = NULL;
    int i = 0;
    int temp = 0;

    part_str = AXIS2_MALLOC(env->allocator, sizeof(char) * (part_len + 1));
    

    if (!part_str)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Failed in creating buffer");
        return NULL;
    }

    /* Copy from the part starting buffer to the 
     * curent buffer */ 

    for (i = marker; i < buf_num; i++)
    {
        if (buf_list[i])
        {
            memcpy(part_str + temp, buf_list[i], len_list[i]);
            temp += len_list[i];
        }
    }
    /* Finally we are copying from the final portion */

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
                break;
            }

            if (old_pos)
            {
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

            /* We will set this to AXIS2_FALSE so when the next time this
             * search method is called it will do a full search first for buffer1 */
            search_info->primary_search = AXIS2_FALSE;

            return found;
        }
        else
        {
            return NULL;
        }
    }
}


/* This method creates a data_handler out of the attachment 
 * and store the data_handler in the mime_parts map */

static axis2_status_t 
axiom_mime_parser_store_attachment(
    const axutil_env_t *env,
    axiom_mime_parser_t *mime_parser,
    axis2_char_t *mime_id,
    axis2_char_t *mime_type,
    axis2_char_t *mime_binary,
    int mime_binary_len,
    axis2_bool_t cached)
{
	if (mime_parser->mime_parts_map)
    {
        if (mime_id)
        {
            axiom_data_handler_t *data_handler = NULL;

            /* Handling the case where attachment is cached using a callback */

            if(mime_parser->callback_name && cached)
            {
                data_handler = axiom_data_handler_create(env, 
                    NULL, mime_type);
                if(data_handler)
                {
                    axiom_data_handler_set_cached(data_handler, env, AXIS2_TRUE);
                    axiom_data_handler_set_data_handler_type(data_handler, env, 
                        AXIOM_DATA_HANDLER_TYPE_CALLBACK);
                }
            }

            /* Handling the case where attachment is cached to a file*/

            else if(mime_parser->attachment_dir && cached)
            {
                axis2_char_t *attachment_location = NULL;
                axis2_char_t *encoded_mime_id = NULL;

                /* Some times content-ids urls, hence we need to encode them 
                 * becasue we can't create files with / */

                encoded_mime_id = AXIS2_MALLOC(env->allocator, 
                    (sizeof(axis2_char_t))* (strlen(mime_id)));
                memset(encoded_mime_id, 0, strlen(mime_id));
                encoded_mime_id = axutil_url_encode(env, encoded_mime_id, 
                    mime_id, strlen(mime_id));
                if(!encoded_mime_id)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Mime Id encoding failed");
                    return AXIS2_FAILURE;
                }

                attachment_location = axutil_stracat(env, 
                    mime_parser->attachment_dir, encoded_mime_id);

                AXIS2_FREE(env->allocator, encoded_mime_id);
                encoded_mime_id = NULL;    

                if(attachment_location)
                {

                    data_handler = axiom_data_handler_create(env, 
                        attachment_location, mime_type);
                    if(data_handler)
                    {
                        axiom_data_handler_set_cached(data_handler, env, AXIS2_TRUE);
                        
                    }
                    AXIS2_FREE(env->allocator, attachment_location);
                    attachment_location = NULL; 
                }
            }

            /* Attachment is in memory, either it is small to be cached or
             * user does not provided the attachment cached directory */

            else if(mime_binary)
            {
                data_handler = axiom_data_handler_create(env, 
                    NULL, mime_type);
                if(data_handler)
                {
                    axiom_data_handler_set_binary_data(data_handler,
                        env, mime_binary, mime_binary_len - 2);
                }
            }
            axiom_data_handler_set_mime_id(data_handler, env, mime_id);            

            axutil_hash_set(mime_parser->mime_parts_map,
                    mime_id, AXIS2_HASH_KEY_STRING, data_handler);
            if(mime_type)
            {
                AXIS2_FREE(env->allocator, mime_type);
            }
        }
        else
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Mime Id or Mime rype not found");
            return AXIS2_FAILURE;
            /*axis2_char_t temp_boundry[1024];
            sprintf(temp_boundry, "--%s--", mime_boundary);
            if (body_mime && axutil_strstr(body_mime, temp_boundry))
            {
                break;
            }*/
        }
        return AXIS2_SUCCESS;
    }	
    else
    {
        return AXIS2_FAILURE;
    }    
}

/* This method will process the mime_headers for a particualr
   attacment and return the mime_id  */


static axis2_char_t *
    axiom_mime_parser_process_mime_headers(
    const axutil_env_t *env,
    axiom_mime_parser_t *mime_parser,
    axis2_char_t **mime_id,
    axis2_char_t *mime_headers)
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
            while (end && *end && !isspace((int)*end))
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
                    return NULL;
                }
                memcpy(temp_type, type, (end - type));
                temp_type[end - type] = '\0';
                type = temp_type;
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
                    int mime_id_len = 0;
                    mime_id_len = (int)(pos - id);
                    *mime_id = AXIS2_MALLOC(env->allocator,
                        sizeof(axis2_char_t) * mime_id_len + 1); 
                    /* The MIME ID will be freed by the SOAP builder */
                    if (*mime_id)
                    {
                        memcpy(*mime_id, id, mime_id_len);
                        (*mime_id)[mime_id_len] = '\0';
                    }
                    else
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "No memory. Failed in creating MIME ID");
                        return NULL;
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
            return NULL;
        }
        return type;
    }
    else
    {
        return NULL;
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


/* Instead of caching to a file this method will cache it
 * to a buffer */

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
axiom_mime_parser_set_attachment_dir(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    axis2_char_t *attachment_dir)
{
    mime_parser->attachment_dir = attachment_dir;
}

/* Set the path of the caching callnack to be loaded */

AXIS2_EXTERN void AXIS2_CALL
axiom_mime_parser_set_caching_callback_name(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    axis2_char_t *callback_name)
{
    mime_parser->callback_name = callback_name;
}

AXIS2_EXTERN void AXIS2_CALL
axiom_mime_parser_set_mime_boundary(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    axis2_char_t *mime_boundary)
{
    mime_parser->mime_boundary = mime_boundary;
}


AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_mime_parser_get_mime_boundary(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env)
{
    return mime_parser->mime_boundary;
}

/* Load the caching callback dll */


static void* axiom_mime_parser_initiate_callback(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    axis2_char_t *mime_id,
    void *user_param)
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

        mime_parser->mtom_caching_callback =  (axiom_mtom_caching_callback_t *)ptr;
        mime_parser->mtom_caching_callback->param = impl_info_param;
        mime_parser->mtom_caching_callback->user_param = user_param;

        return AXIOM_MTOM_CACHING_CALLBACK_INIT_HANDLER(mime_parser->mtom_caching_callback, env, mime_id);
    }

    else
    {
        return NULL;
    }

}



/* This method will tell whether there are more data in the 
 * stream */

static axis2_bool_t axiom_mime_parser_is_more_data(
    axiom_mime_parser_t *mime_parser,
    const axutil_env_t *env,
    axis2_callback_info_t *callback_info)
{
    /* In the case of axutil_http_chunked stream it is the 
     * end of chunk */

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

    /* When we are using content length or any wrapped 
     * stream it will be the unread_length */

    else if(callback_info->unread_len == 0)
    {
        return AXIS2_FALSE;
    }
    else
    {
        return AXIS2_TRUE;
    }
}

static axis2_status_t 
axiom_mime_parser_cache_to_file(
    const axutil_env_t* env,
    axis2_char_t *buf,
    int buf_len,
    void *handler)
{
    int len = 0;
    FILE *fp = NULL;

    fp = (FILE *)handler;

    len = fwrite(buf, 1, buf_len, fp);
    if(len < 0)
    {
        return AXIS2_FAILURE;
    }
    else
    {
        return AXIS2_SUCCESS;
    }
}


