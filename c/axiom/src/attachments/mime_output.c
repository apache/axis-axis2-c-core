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

#include <axiom_mime_output.h>
#include <axiom_data_handler.h>
#include "axiom_mime_body_part.h"
#include <axutil_string.h>
#include <axiom_text.h>
#include <axiom_mime_const.h>


static axis2_status_t
axiom_mime_output_write_body_part_to_list(
    axiom_mime_output_t *mime_output,
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axiom_mime_body_part_t *part);

static axis2_status_t
axiom_mime_output_write_mime_boundary(
    axiom_mime_output_t *mime_output,
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axis2_char_t *boundary);

static axis2_status_t
axiom_mime_output_finish_adding_parts(
    axiom_mime_output_t *mime_output,
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axis2_char_t *boundary);


AXIS2_EXTERN axiom_mime_output_t *AXIS2_CALL
axiom_mime_output_create(
    const axutil_env_t *env)
{
    axiom_mime_output_t *mime_output = NULL;
    mime_output = (axiom_mime_output_t *) AXIS2_MALLOC(env->allocator,
        sizeof(axiom_mime_output_t));

    if (!mime_output)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Cannot create MIME output");
        return NULL;
    }

    return mime_output;
}

AXIS2_EXTERN axiom_mime_output_part_t *AXIS2_CALL 
axiom_mime_output_part_create(
        const axutil_env_t *env)
{
    axiom_mime_output_part_t *mime_output_part = NULL;
    mime_output_part = AXIS2_MALLOC(env->allocator, sizeof(axiom_mime_output_part_t));

    if(mime_output_part)
    {
        mime_output_part->part = NULL;
        mime_output_part->file_name = NULL;
        mime_output_part->part_size = 0;
        mime_output_part->type = AXIOM_MIME_OUTPUT_PART_UNKNOWN;
        
        return mime_output_part;
    }    
    else
    {
        return NULL;
    }        
}        

AXIS2_EXTERN void AXIS2_CALL
axiom_mime_output_free(
    axiom_mime_output_t *mime_output,
    const axutil_env_t *env)
{
    if (mime_output)
    {
        AXIS2_FREE(env->allocator, mime_output);
    }
    return;
}


/* This method will create a mime_boundary buffer
 * and based on the buffer creates a mime_output part
 * This will be added to the array_list so later in the trasnport
 * this can be put to the wire. */

static axis2_status_t
axiom_mime_output_write_mime_boundary(
    axiom_mime_output_t *mime_output,
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axis2_char_t *boundary)
{
    axis2_byte_t *byte_buffer = NULL;
    axis2_byte_t *byte_stream = NULL;
    int size = 0;
    axiom_mime_output_part_t *boundary_part = NULL;
    
    boundary_part = axiom_mime_output_part_create(env);
    
    byte_buffer = (axis2_byte_t *)boundary;
    
    byte_stream =
        AXIS2_MALLOC(env->allocator, (size + 2) * sizeof(axis2_byte_t));
    if (!byte_stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Cannot create byte stream");
        return AXIS2_FAILURE;
    }

    byte_stream[0] = AXIOM_MIME_BOUNDARY_BYTE;
    byte_stream[1] = AXIOM_MIME_BOUNDARY_BYTE;

    memcpy(byte_stream + 2, byte_buffer, size);
    
    boundary_part->part = byte_stream;
    boundary_part->part_size = size + 2;
    boundary_part->type = AXIOM_MIME_OUTPUT_PART_BUFFER;
    
    axutil_array_list_add(list, env, boundary_part);

    return AXIS2_SUCCESS;
}

static axis2_status_t
axiom_mime_output_write_body_part_to_list(
    axiom_mime_output_t *mime_output,
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axiom_mime_body_part_t *part)
{
    axiom_mime_output_part_t *crlf1 = NULL;
    axiom_mime_output_part_t *crlf2 = NULL;
   
    /* There is a \r\n before the attachment data
     * So first add it
     */
    
    crlf1 = axiom_mime_output_part_create(env);
    
    crlf1->part = (axis2_byte_t *)axutil_strdup(env, AXIS2_CRLF);
    crlf1->part_size = 2;
    crlf1->type = AXIOM_MIME_OUTPUT_PART_BUFFER;
    
    axutil_array_list_add(list, env, crlf1);
    
    /* Write the attachemnt and realted haeders to the list */
    
    axiom_mime_body_part_write_to_list(part, env, list);

    /* Then add the next \r\n after the attachment */
    
    crlf2 = axiom_mime_output_part_create(env);
    
    crlf2->part = (axis2_byte_t *)axutil_strdup(env, AXIS2_CRLF);
    crlf2->part_size = 2;
    crlf2->type = AXIOM_MIME_OUTPUT_PART_BUFFER;
    
    axutil_array_list_add(list, env, crlf2);
      
    return AXIS2_SUCCESS;
}

static axis2_status_t
axiom_mime_output_finish_adding_parts(
    axiom_mime_output_t *mime_output,
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axis2_char_t *boundary)
{
    axis2_byte_t *byte_buffer = NULL;
    axis2_byte_t *byte_stream = NULL;
    int size = 0;
    axiom_mime_output_part_t *final_part = NULL;

    size = axutil_strlen(boundary);
    byte_buffer = (axis2_byte_t *)boundary;
    
    byte_stream =
        AXIS2_MALLOC(env->allocator, (size + 4) * sizeof(axis2_byte_t));
    if (!byte_stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Cannot create byte stream");
        return AXIS2_FAILURE;
    }

    /* Adding the starting -- */
    
    byte_stream[0] = AXIOM_MIME_BOUNDARY_BYTE;
    byte_stream[1] = AXIOM_MIME_BOUNDARY_BYTE;
    if (byte_buffer)
    {
        memcpy(byte_stream + 2, byte_buffer, size);
        AXIS2_FREE(env->allocator, byte_buffer);
        byte_buffer = NULL;
    }
    else
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
            "Byte buffer not available for writing");
    }

    /* Adding the final -- */
    
    byte_stream[size + 2] = AXIOM_MIME_BOUNDARY_BYTE;
    byte_stream[size + 3] = AXIOM_MIME_BOUNDARY_BYTE;
    
    /* Now we add this as an mime_output part to 
     * the list. */ 
    
    final_part = axiom_mime_output_part_create(env);
    
    if(!final_part)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Cannot create final_part");
        return AXIS2_FAILURE;
    }  
    
    final_part->part = byte_stream;
    final_part->part_size = size + 4;
    final_part->type = AXIOM_MIME_OUTPUT_PART_BUFFER;
    
    axutil_array_list_add(list, env, final_part);

    
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axiom_mime_output_get_content_type_for_mime(
    axiom_mime_output_t *mime_output,
    const axutil_env_t *env,
    axis2_char_t *boundary,
    axis2_char_t *content_id,
    axis2_char_t *char_set_encoding,
    const axis2_char_t *soap_content_type)
{
    axis2_char_t *content_type_string = NULL;
    axis2_char_t *temp_content_type_string = NULL;

    content_type_string = 
        axutil_strdup(env, AXIOM_MIME_TYPE_MULTIPART_RELATED);
    if (!content_type_string)
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
            "Creation of Content-Type string failed");
        return NULL;
    }
    temp_content_type_string = axutil_stracat(env, content_type_string, "; ");
    AXIS2_FREE(env->allocator, content_type_string);
    content_type_string = temp_content_type_string;
    if (boundary)
    {
        temp_content_type_string =
            axutil_stracat(env, content_type_string,
            AXIOM_MIME_HEADER_FIELD_BOUNDARY "=");
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
        temp_content_type_string =
            axutil_stracat(env, content_type_string, boundary);
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
        temp_content_type_string =
            axutil_stracat(env, content_type_string, "; ");
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
    }
    temp_content_type_string =
        axutil_stracat(env, content_type_string,
        AXIOM_MIME_HEADER_FIELD_TYPE "=\"" AXIOM_MIME_TYPE_XOP_XML "\"");
    AXIS2_FREE(env->allocator, content_type_string);
    content_type_string = temp_content_type_string;
    temp_content_type_string = axutil_stracat(env, content_type_string, "; ");
    AXIS2_FREE(env->allocator, content_type_string);
    content_type_string = temp_content_type_string;
    if (content_id)
    {
        temp_content_type_string =
            axutil_stracat(env, content_type_string, 
            AXIOM_MIME_HEADER_FIELD_START "=\"<");
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
        temp_content_type_string =
            axutil_stracat(env, content_type_string, content_id);
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
        temp_content_type_string =
            axutil_stracat(env, content_type_string, ">\"");
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
        temp_content_type_string =
            axutil_stracat(env, content_type_string, "; ");
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
    }
    if (soap_content_type)
    {
        temp_content_type_string =
            axutil_stracat(env, content_type_string, 
            AXIOM_MIME_HEADER_FIELD_START_INFO "=\"");
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
        temp_content_type_string =
            axutil_stracat(env, content_type_string, soap_content_type);
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
        temp_content_type_string =
            axutil_stracat(env, content_type_string, "\"; ");
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
    }
    if (char_set_encoding)
    {
        temp_content_type_string =
            axutil_stracat(env, content_type_string, 
            AXIOM_MIME_HEADER_FIELD_CHARSET "=\"");
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
        temp_content_type_string =
            axutil_stracat(env, content_type_string, char_set_encoding);
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
        temp_content_type_string =
            axutil_stracat(env, content_type_string, "\"");
        AXIS2_FREE(env->allocator, content_type_string);
        content_type_string = temp_content_type_string;
    }

    return content_type_string;
}


/* This method is the core of attachment sending
 * part. It will build each and every part and put them in
 * a array_list. Instead of a big buffer we pass the array_list
 * with small buffers and attachment locations . */


AXIS2_EXTERN axutil_array_list_t  *AXIS2_CALL
axiom_mime_output_create_part_list(
    axiom_mime_output_t *mime_output,
    const axutil_env_t *env,
    axis2_char_t *soap_body,
    axutil_array_list_t *binary_node_list,
    axis2_char_t *boundary,
    axis2_char_t *content_id,
    axis2_char_t *char_set_encoding,
    const axis2_char_t *soap_content_type)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *header_value = NULL;
    axis2_char_t *temp_header_value = NULL;
    axis2_char_t *content_id_string = NULL;
    axis2_char_t *temp_content_id_string = NULL;
    axiom_mime_body_part_t *root_mime_body_part = NULL;
    axis2_char_t *soap_body_buffer = NULL;
    axutil_array_list_t *part_list = NULL;
    axiom_mime_output_part_t *soap_part = NULL;
    
    part_list = axutil_array_list_create(env, 0);
    
    if(!part_list)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Cannot create part list array");
        return NULL;
    }    
    
    status = axiom_mime_output_write_mime_boundary(mime_output, 
            env, part_list, boundary);
    
    if (status != AXIS2_SUCCESS)
    {
        return NULL;
    }

    root_mime_body_part = axiom_mime_body_part_create(env);

    if (!root_mime_body_part)
    {
        return NULL;
    }

    /* Adding Content-Type Header */
    header_value = axutil_strdup(env, AXIOM_MIME_TYPE_XOP_XML 
        ";" AXIOM_MIME_HEADER_FIELD_CHARSET "=");
    temp_header_value = axutil_stracat(env, header_value, char_set_encoding);
    AXIS2_FREE(env->allocator, header_value);
    header_value = temp_header_value;
    temp_header_value = axutil_stracat(env, header_value, ";"
        AXIOM_MIME_HEADER_FIELD_TYPE "=\"");
    AXIS2_FREE(env->allocator, header_value);
    header_value = temp_header_value;
    temp_header_value = axutil_stracat(env, header_value, soap_content_type);
    AXIS2_FREE(env->allocator, header_value);
    header_value = temp_header_value;
    temp_header_value = axutil_stracat(env, header_value, "\";");
    AXIS2_FREE(env->allocator, header_value);
    header_value = temp_header_value;
    AXIOM_MIME_BODY_PART_ADD_HEADER(root_mime_body_part, env,
        AXIOM_MIME_HEADER_CONTENT_TYPE, header_value);

    /* Adding Content Transfer Encoding header */
    
    AXIOM_MIME_BODY_PART_ADD_HEADER(root_mime_body_part, env,
        AXIOM_MIME_HEADER_CONTENT_TRANSFER_ENCODING,
        axutil_strdup(env, AXIOM_MIME_CONTENT_TRANSFER_ENCODING_BINARY));

    /* Adding Content ID header */
    
    content_id_string = (axis2_char_t *) "<";
    content_id_string = axutil_stracat(env, content_id_string, content_id);
    temp_content_id_string = axutil_stracat(env, content_id_string, ">");
    AXIS2_FREE(env->allocator, content_id_string);
    content_id_string = temp_content_id_string;
    AXIOM_MIME_BODY_PART_ADD_HEADER(root_mime_body_part, env,
        AXIOM_MIME_HEADER_CONTENT_ID, content_id_string);

    /* Now first write the headers needed for SOAP */
    
    status = axiom_mime_output_write_body_part_to_list(mime_output, env, part_list,
            root_mime_body_part);
    
    if(status == AXIS2_FAILURE)
    {
        return NULL;
    }    
    
    /* Now add the SOAP body */

    AXIOM_MIME_BODY_PART_FREE(root_mime_body_part, env);
    root_mime_body_part = NULL;
    
    soap_part = axiom_mime_output_part_create(env);
    
    if(!soap_part)
    {
        return AXIS2_FAILURE;
    }    
    
    soap_part->part = (axis2_byte_t *)soap_body_buffer;
    soap_part->part_size = (int) axutil_strlen(soap_body_buffer);
    soap_part->type = AXIOM_MIME_OUTPUT_PART_BUFFER;
    
    axutil_array_list_add(part_list, env, soap_part);
    
    /* Now we need to add each binary attachment to the array_list */
    
    if (binary_node_list)
    {
        int j = 0;
        for (j = 0; j < axutil_array_list_size(binary_node_list, env); j++)
        {
            /* Getting each attachment text node from the node list */
            
            axiom_text_t *text = (axiom_text_t *) 
                axutil_array_list_get(binary_node_list, env, j);
            if (text)
            {
                axiom_mime_body_part_t *mime_body_part = NULL;
                mime_body_part = axiom_mime_body_part_create_from_om_text(env, text);
                
                /* Let's fill the mime_output_part arraylist with attachment data*/
                if(!mime_body_part)
                {
                    return NULL;
                }    
                
                status = axiom_mime_output_write_body_part_to_list(mime_output, 
                        env, part_list, mime_body_part);
                
                if(status == AXIS2_FAILURE);
                {
                    return NULL;
                }
                
                AXIOM_MIME_BODY_PART_FREE(mime_body_part, env);
                mime_body_part = NULL;
                
                /* Then we need to add a mime_boundary */
                status = axiom_mime_output_write_mime_boundary(mime_output, env, part_list, boundary);
                
                if(status == AXIS2_FAILURE)
                {
                    return NULL;
                }    
            }
        }
    }

    /* Now all the attachments and headers are added to the  list.
     * So let's add the final mime_boundary with -- at the end */
    
    axiom_mime_output_finish_adding_parts(mime_output, env, part_list, boundary);
    
    return part_list;    
}


