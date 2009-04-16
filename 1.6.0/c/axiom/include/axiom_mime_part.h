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

#ifndef AXIOM_MIME_PART_H
#define AXIOM_MIME_PART_H

/**
 * @file axiom_mime_part.h
 * @brief axis2 mime_part interface
 */

#include <axutil_utils.h>
#include <axutil_error.h>
#include <axutil_utils_defines.h>
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_string.h>
#include <axutil_array_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_mime_part_t axiom_mime_part_t;
    
    
    /* An enum to keep different mime_part types */

    typedef enum axiom_mime_part_type_t
    {

        /** Char buffer */
        AXIOM_MIME_PART_BUFFER = 0,
        
        /* A file */
        AXIOM_MIME_PART_FILE,

        /* User specified callback */
        AXIOM_MIME_PART_CALLBACK,
        
        /* unknown type*/
        AXIOM_MIME_PART_UNKNOWN
        
    } axiom_mime_part_type_t;
    
    struct axiom_mime_part_t
    {
        /* This is when the mime part is a buffer.
         * This will be null when the part is a file */
        axis2_byte_t *part;

        /* This is to keep file name when the part is a file
         * NULL when the part is a buffer */
        axis2_char_t *file_name;

        /* Size of the part. In the case of buffer this is 
         * the buffer size and in the case of file this is 
           the file size */
        int part_size;    

        /* This is one from the above defined enum */
        axiom_mime_part_type_t type;

        /* This is required in the case of the callback */
        void *user_param;
    };




    AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
    axiom_mime_part_get_content_type_for_mime(
        const axutil_env_t * env,
        axis2_char_t * boundary,
        axis2_char_t * content_id,
        axis2_char_t * char_set_encoding,
        const axis2_char_t * soap_content_type);

    
    /**
     * Creates mime_part struct
     * @return pointer to newly created mime_part
     */
    
    AXIS2_EXTERN axiom_mime_part_t *AXIS2_CALL 
    axiom_mime_part_create(
        const axutil_env_t *env);

    /* This method will create the output part 
     * list.*/

    AXIS2_EXTERN axutil_array_list_t  *AXIS2_CALL
    axiom_mime_part_create_part_list(
        const axutil_env_t *env,
        axis2_char_t *soap_body,
        axutil_array_list_t *binary_node_list,
        axis2_char_t *boundary,
        axis2_char_t *content_id,
        axis2_char_t *char_set_encoding,
        const axis2_char_t *soap_content_type);


    AXIS2_EXTERN void AXIS2_CALL
    axiom_mime_part_free(
        axiom_mime_part_t *mime_part,
        const axutil_env_t *env);

    


    /** @} */

#ifdef __cplusplus
}
#endif
#endif                          /* AXIOM_MIME_PART_H */
