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

#include "axiom_mime_body_part.h"
#include <axiom_mime_part.h>
#include <axiom_data_handler.h>
#include <axutil_hash.h>

struct axiom_mime_body_part
{
    /* hash map to hold header name, value pairs */
    axutil_hash_t *header_map;
    axiom_data_handler_t *data_handler;
};

/* This method just create a mime_body_part. It does not 
 * fill the header map.
 */


AXIS2_EXTERN axiom_mime_body_part_t *AXIS2_CALL
axiom_mime_body_part_create(
    const axutil_env_t *env)
{
    axiom_mime_body_part_t *mime_body_part = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    mime_body_part = (axiom_mime_body_part_t *) AXIS2_MALLOC(env->allocator,
        sizeof(axiom_mime_body_part_t));

    if (!mime_body_part)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Cannot create MIME body part");
        return NULL;
    }

    mime_body_part->header_map = NULL;
    mime_body_part->data_handler = NULL;

    mime_body_part->header_map = axutil_hash_make(env);
    if (!(mime_body_part->header_map))
    {
        axiom_mime_body_part_free(mime_body_part, env);
        return NULL;
    }

    return mime_body_part;
}

/* This method will create the mime_body_part and fill the header map with 
 * default information. Default information are for binary attachments. 
 * Attachment information is taken from the information in data_handler in passed
 * om_text.
 */  

AXIS2_EXTERN axiom_mime_body_part_t *AXIS2_CALL 
axiom_mime_body_part_create_from_om_text(
    const axutil_env_t *env,    
    axiom_text_t *text)
{
    axiom_data_handler_t *data_handler = NULL;
    axiom_mime_body_part_t *mime_body_part = NULL;
    axis2_char_t *content_id = NULL;
    axis2_char_t *temp_content_id = NULL;
    const axis2_char_t *content_type = AXIOM_MIME_TYPE_OCTET_STREAM;

    mime_body_part = axiom_mime_body_part_create(env);
    if (!mime_body_part)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "MIME body part creation failed");
        return NULL;
    }
    
    /* Take the data_handler which is set by the sending applocation. */
    
    data_handler = axiom_text_get_data_handler(text, env);

    if (data_handler)
    {
        content_type = axiom_data_handler_get_content_type(data_handler, env);
    }

    AXIOM_MIME_BODY_PART_SET_DATA_HANDLER(mime_body_part, env, data_handler);
    content_id = (axis2_char_t *) "<";
    content_id = axutil_stracat(env, content_id,
        axiom_text_get_content_id(text, env));
    temp_content_id = axutil_stracat(env, content_id, ">");

    AXIS2_FREE(env->allocator, content_id);
    content_id = temp_content_id;

    /* Adding the content-id */
    AXIOM_MIME_BODY_PART_ADD_HEADER(mime_body_part, env,
        AXIOM_MIME_HEADER_CONTENT_ID, content_id);
    
    /* Adding the content-type */
    AXIOM_MIME_BODY_PART_ADD_HEADER(mime_body_part, env,
        AXIOM_MIME_HEADER_CONTENT_TYPE, axutil_strdup(env, content_type));
    
    /* Adding the content-transfer encoding */
    AXIOM_MIME_BODY_PART_ADD_HEADER(mime_body_part, env,
        AXIOM_MIME_HEADER_CONTENT_TRANSFER_ENCODING,
        axutil_strdup(env, AXIOM_MIME_CONTENT_TRANSFER_ENCODING_BINARY));

    return mime_body_part;
}


AXIS2_EXTERN void AXIS2_CALL
axiom_mime_body_part_free(
    axiom_mime_body_part_t *mime_body_part,
    const axutil_env_t *env)
{
    if (mime_body_part->header_map)
    {
        axutil_hash_index_t *hash_index = NULL;
        const void *key = NULL;
        void *value = NULL;
        for (hash_index = axutil_hash_first(mime_body_part->header_map, env);
             hash_index; hash_index = axutil_hash_next(env, hash_index))
        {
            axutil_hash_this(hash_index, &key, NULL, &value);
            if (value)
            {
                AXIS2_FREE(env->allocator, value);
            }
        }

        axutil_hash_free(mime_body_part->header_map, env);
        mime_body_part->header_map = NULL;
    }

    if (mime_body_part)
    {
        AXIS2_FREE(env->allocator, mime_body_part);
    }

    return;
}

/* This method will add a mime_header to the hash */


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_mime_body_part_add_header(
    axiom_mime_body_part_t *mime_body_part,
    const axutil_env_t *env,
    const axis2_char_t *name,
    const axis2_char_t *value)
{
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);

    if (!mime_body_part->header_map)
    {
        return AXIS2_FAILURE;
    }
    axutil_hash_set(mime_body_part->header_map, name,
                    AXIS2_HASH_KEY_STRING, value);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_mime_body_part_set_data_handler(
    axiom_mime_body_part_t *mime_body_part,
    const axutil_env_t *env,
    axiom_data_handler_t *data_handler)
{
    mime_body_part->data_handler = data_handler;
    return AXIS2_SUCCESS;
}


/* This method will fill the array_list with binary and binary_beader information.
 * If the binary is in a file this will not load the file to the memory. Because
 * that will cause performance degradation when the file size is large. Instead 
 * this will add file information to the list so that when writing the message 
 * through transport_sender it can send the file by chunk.
 */

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_mime_body_part_write_to_list(
    axiom_mime_body_part_t *mime_body_part,
    const axutil_env_t *env,
    axutil_array_list_t *list)
{
    axutil_hash_index_t *hash_index = NULL;
    const void *key = NULL;
    void *value = NULL;
    axis2_char_t *header_str = NULL;
    axis2_char_t *temp_header_str = NULL;
    int header_str_size = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axiom_mime_part_t *mime_header_part = NULL;


    /* We have the mime headers in the hash with thier keys 
     * So first concatenate them to a one string */ 
         
    for (hash_index = axutil_hash_first(mime_body_part->header_map, env);
         hash_index; hash_index = axutil_hash_next(env, hash_index))
    {
        axutil_hash_this(hash_index, &key, NULL, &value);
        if (key && value)
        {
            /* First conactenate to the already conacatenated stuff */
            
            temp_header_str =
                axutil_stracat(env, header_str, (axis2_char_t *) key);
            if (header_str)
            {
                AXIS2_FREE(env->allocator, header_str);
            }
            header_str = temp_header_str;
            temp_header_str = axutil_stracat(env, header_str, ": ");
            AXIS2_FREE(env->allocator, header_str);
            header_str = temp_header_str;
            
            /* Add the new stuff */
            temp_header_str =
                axutil_stracat(env, header_str, (axis2_char_t *) value);
            AXIS2_FREE(env->allocator, header_str);
            header_str = temp_header_str;
            
            /* Next header will be in a new line. So lets add it */
            
            temp_header_str = axutil_stracat(env, header_str, AXIS2_CRLF);
            AXIS2_FREE(env->allocator, header_str);
            header_str = temp_header_str;
        }
    }

    /* If there is a data handler that's mean there is an attachment. Attachment
     * will always start after an additional new line . So let's add it .*/
    
    if (mime_body_part->data_handler)
    {
        temp_header_str = axutil_stracat(env, header_str, AXIS2_CRLF);
        AXIS2_FREE(env->allocator, header_str);
        header_str = temp_header_str;
    }

    if (header_str)
    {
        header_str_size = axutil_strlen(header_str);
    }
    
    /* Now we have the complete mime_headers string for a particular mime part.
     * First wrap it as a mime_part_t .Then add it to the array list so
     * later through the transport this can be written to the wire. */
    
    mime_header_part = axiom_mime_part_create(env);
    
    if(mime_header_part)
    {
        mime_header_part->part = (axis2_byte_t *)header_str;
        mime_header_part->part_size = header_str_size;
        mime_header_part->type = AXIOM_MIME_PART_BUFFER;
    }    
    else
    {
        return AXIS2_FAILURE;
    }    
    
    axutil_array_list_add(list, env, mime_header_part);

    /* Then if the data_handler is there let's add the binary data, may be
     * buffer , may be file name and information. 
     */
     
    if (mime_body_part->data_handler)
    {
        status = axiom_data_handler_add_binary_data(mime_body_part->data_handler, env, list);
        if (status != AXIS2_SUCCESS)
        {
            return status;
        }
    }
    return AXIS2_SUCCESS;
}
