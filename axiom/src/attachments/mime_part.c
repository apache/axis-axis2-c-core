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

#include <axiom_mime_part.h>
#include <axiom_data_handler.h>
#include "axiom_mime_body_part.h"
#include <axutil_string.h>
#include <axiom_text.h>
#include <axiom_mime_const.h>


static axis2_status_t
axiom_mime_part_write_body_part_to_list(
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axiom_mime_body_part_t *part,
    axis2_char_t *boundary);

static axis2_status_t
axiom_mime_part_write_mime_boundary(
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axis2_char_t *boundary);

static axis2_status_t
axiom_mime_part_finish_adding_parts(
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axis2_char_t *boundary);

/* This method will create a mime_part 
 * A mime part will encapsulate a buffer 
 * a file or a callback to load the attachment 
   which needs to be send */

AXIS2_EXTERN axiom_mime_part_t *AXIS2_CALL 
axiom_mime_part_create(
        const axutil_env_t *env)
{
    axiom_mime_part_t *mime_part = NULL;
    mime_part = AXIS2_MALLOC(env->allocator, sizeof(axiom_mime_part_t));

    if(mime_part)
    {
        mime_part->part = NULL;
        mime_part->file_name = NULL;
        mime_part->part_size = 0;
        mime_part->type = AXIOM_MIME_PART_UNKNOWN;
        mime_part->user_param = NULL; 
       
        return mime_part;
    }    
    else
    {
        return NULL;
    }        
}        

/* Frees the mime_part */

AXIS2_EXTERN void AXIS2_CALL
axiom_mime_part_free(
    axiom_mime_part_t *mime_part,
    const axutil_env_t *env)
{
    if (mime_part)
    {
        if(mime_part->type == AXIOM_MIME_PART_BUFFER)
        {
            if(mime_part->part)
            {
                AXIS2_FREE(env->allocator, mime_part->part);
                mime_part->part = NULL;
            }
        }
        else if(mime_part->type == AXIOM_MIME_PART_FILE)
        {
            if(mime_part->file_name)
            {
                AXIS2_FREE(env->allocator, mime_part->file_name);
                mime_part->file_name = NULL;
            }
        }
        
        AXIS2_FREE(env->allocator, mime_part);
        mime_part = NULL;
    }
    return;
}


/* This method will create a mime_boundary buffer
 * and based on the buffer creates a mime_part. 
 * This will be added to the array_list so later in the trasnport
 * this can be put to the wire. */

static axis2_status_t
axiom_mime_part_write_mime_boundary(
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axis2_char_t *boundary)
{
    axis2_byte_t *byte_buffer = NULL;
    axis2_byte_t *byte_stream = NULL;
    int size = 0;
    axiom_mime_part_t *boundary_part = NULL;
    
    boundary_part = axiom_mime_part_create(env);
    
    byte_buffer = (axis2_byte_t *)boundary;
    size = axutil_strlen(boundary);
    
    byte_stream =
        AXIS2_MALLOC(env->allocator, (size + 2) * sizeof(axis2_byte_t));
    if (!byte_stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Cannot create byte stream");
        return AXIS2_FAILURE;
    }

    /* Mime boundary is always in the following form
       --MimeBoundary */

    byte_stream[0] = AXIOM_MIME_BOUNDARY_BYTE;
    byte_stream[1] = AXIOM_MIME_BOUNDARY_BYTE;

    memcpy(byte_stream + 2, byte_buffer, size);
    
    boundary_part->part = byte_stream;
    boundary_part->part_size = size + 2;
    boundary_part->type = AXIOM_MIME_PART_BUFFER;
    
    axutil_array_list_add(list, env, boundary_part);

    return AXIS2_SUCCESS;
}



/* This method will add the attachment file related information 
   to the list. It will create a mime_part from those information
   and add to the list. If there are not data_handlers in the mime_body
   then this method just add the headers. */


static axis2_status_t
axiom_mime_part_write_body_part_to_list(
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axiom_mime_body_part_t *part,
    axis2_char_t *boundary)
{
    axiom_mime_part_t *crlf1 = NULL;
    axiom_mime_part_t *crlf2 = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    /* We are adding accoarding to the following format here.
     * --MimeBoundary
     * mime_header1
     * mime_header2
     * mime_header3 */   

    status = axiom_mime_part_write_mime_boundary(
        env, list, boundary);
    
    if(status != AXIS2_SUCCESS)
    {
        return status;
    }

    /* Then we will add the new line charator after 
     * the mime_boundary */  
 
    crlf1 = axiom_mime_part_create(env);
    
    crlf1->part = (axis2_byte_t *)axutil_strdup(env, AXIS2_CRLF);
    crlf1->part_size = 2;
    crlf1->type = AXIOM_MIME_PART_BUFFER;
    
    axutil_array_list_add(list, env, crlf1);
    
    /*This method will fill the list with mime_headers and 
     *if there is an attachment with attachment details*/
    
    axiom_mime_body_part_write_to_list(part, env, list);

    /* Then add the next \r\n after the attachment */
    
    crlf2 = axiom_mime_part_create(env);
    
    crlf2->part = (axis2_byte_t *)axutil_strdup(env, AXIS2_CRLF);
    crlf2->part_size = 2;
    crlf2->type = AXIOM_MIME_PART_BUFFER;
    
    axutil_array_list_add(list, env, crlf2);
      
    return AXIS2_SUCCESS;
}


/* This methos will add the final mime_boundary
 * It is in --MimeBoundary-- format */

static axis2_status_t
axiom_mime_part_finish_adding_parts(
    const axutil_env_t *env,
    axutil_array_list_t *list,
    axis2_char_t *boundary)
{
    axis2_byte_t *byte_buffer = NULL;
    axis2_byte_t *byte_stream = NULL;
    int size = 0;
    axiom_mime_part_t *final_part = NULL;

    size = axutil_strlen(boundary);
    byte_buffer = (axis2_byte_t *)boundary;
    
    /* There is -- before and after so the length of the
     * actual part is mime_boundary_len + 4 */

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
    }
    else
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
            "Byte buffer not available for writing");
    }

    /* Adding the final -- */
    
    byte_stream[size + 2] = AXIOM_MIME_BOUNDARY_BYTE;
    byte_stream[size + 3] = AXIOM_MIME_BOUNDARY_BYTE;
    
    /* Now we add this as an mime_part to 
     * the list. */ 
    
    final_part = axiom_mime_part_create(env);
    
    if(!final_part)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Cannot create final_part");
        return AXIS2_FAILURE;
    }  
    
    final_part->part = byte_stream;
    final_part->part_size = size + 4;
    final_part->type = AXIOM_MIME_PART_BUFFER;
    
    axutil_array_list_add(list, env, final_part);

    
    return AXIS2_SUCCESS;
}


/* This is the method which creates the content-type string 
   which is in the HTTP header or in mime_headers*/


AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axiom_mime_part_get_content_type_for_mime(
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
 * an array_list. Instead of a big buffer we pass the array_list
 * with small buffers and attachment locations. */


AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axiom_mime_part_create_part_list(
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
    axiom_mime_part_t *soap_part = NULL;
    
    part_list = axutil_array_list_create(env, 0);
    
    if(!part_list)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Cannot create part list array");
        return NULL;
    }    

    /* This mime_body part just keeps the mime_headers of the 
     * SOAP part. Since it is not created from an axiom_text
     * this will not contain an attachment*/
    
    root_mime_body_part = axiom_mime_body_part_create(env);

    if (!root_mime_body_part)
    {
        return NULL;
    }

    /* In order to understand the following code which creates 
     * mime_headers go through the code with a sample mtom message */


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

    /* Now first insert the headers needed for SOAP */
    
    /* After calling this method we have mime_headers of the SOAP envelope
     * as a mime_part in the array_list */

    status = axiom_mime_part_write_body_part_to_list(env, part_list,
            root_mime_body_part, boundary);
    
    if(status == AXIS2_FAILURE)
    {
        return NULL;
    }    
    
    /* Now add the SOAP body */

    AXIOM_MIME_BODY_PART_FREE(root_mime_body_part, env);
    root_mime_body_part = NULL;
    
    soap_part = axiom_mime_part_create(env);
    
    if(!soap_part)
    {
        return NULL;
    }    

    /* The atachment's mime_boundary will start after a new line charator */

    soap_body_buffer = axutil_stracat(env, soap_body, AXIS2_CRLF); 
   
    soap_part->part = (axis2_byte_t *)soap_body_buffer;
    soap_part->part_size = (int) axutil_strlen(soap_body_buffer);
    soap_part->type = AXIOM_MIME_PART_BUFFER;
    
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
                
                /* Let's fill the mime_part arraylist with attachment data*/
                if(!mime_body_part)
                {
                    return NULL;
                }    
                
                /* This call will create mime_headers for the attachment and put 
                 * them to the array_list. Then put the attachment file_name to the 
                 * list */                    
                
                status = axiom_mime_part_write_body_part_to_list(env, 
                    part_list, mime_body_part, boundary);
                
                if(status == AXIS2_FAILURE)
                {
                    return NULL;
                }
                
                AXIOM_MIME_BODY_PART_FREE(mime_body_part, env);
                mime_body_part = NULL;
            }
        }
    }

    /* Now we have the SOAP message, all the attachments and headers are added to the  list.
     * So let's add the final mime_boundary with -- at the end */
    
    status = axiom_mime_part_finish_adding_parts(env, part_list, boundary);
    if(status == AXIS2_FAILURE)
    {
        return NULL;
    }
    return part_list;    
}


