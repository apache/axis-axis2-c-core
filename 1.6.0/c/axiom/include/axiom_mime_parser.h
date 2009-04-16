
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

#ifndef AXIOM_MIME_PARSER_H
#define AXIOM_MIME_PARSER_H

/**
 * @file axiom_mime_parser.h
 * @brief axis2 mime_parser interface
 */

#include <axutil_utils.h>
#include <axutil_error.h>
#include <axutil_utils_defines.h>
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_string.h>
#include <axutil_hash.h>
#include <axiom_mime_const.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define AXIOM_MIME_PARSER_BUFFER_SIZE (1024 * 1024/2)
#define AXIOM_MIME_PARSER_MAX_BUFFERS 1000

#define AXIOM_MIME_PARSER_END_OF_MIME_MAX_COUNT 100


    typedef struct axiom_mime_parser axiom_mime_parser_t;

    /** @defgroup axiom_mime_parser Flow
      * @ingroup axiom_mime_parser
      * @{
      */

    /**
      * Parse and returns mime parts as a hash map
      * @param mime_parser the pointer for the mime parser struct 
      * @param env Environment. MUST NOT be NULL.
      * @param callback_ctx the callback context
      * @param mime_boundary the MIME boundary
      * @return mime parts as a hash map 
      */
    /*AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
    axiom_mime_parser_parse(
        axiom_mime_parser_t * mime_parser,
        const axutil_env_t * env,
        AXIS2_READ_INPUT_CALLBACK,
        void *callback_ctx,
        axis2_char_t * mime_boundary);*/

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_mime_parser_parse_for_soap(
        axiom_mime_parser_t * mime_parser,
        const axutil_env_t * env,
        AXIS2_READ_INPUT_CALLBACK callback,
        void *callback_ctx,
        axis2_char_t * mime_boundary);


    AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
    axiom_mime_parser_parse_for_attachments(
        axiom_mime_parser_t * mime_parser,
        const axutil_env_t * env,
        AXIS2_READ_INPUT_CALLBACK callback,
        void *callback_ctx,
        axis2_char_t * mime_boundary,
        void *user_param);


    /**
      * Returns mime parts as a hash map
      * @param mime_parser the pointer for the mime parser struct 
      * @param env Environment. MUST NOT be NULL.
      * @return mime parts as a hash map 
      */
    AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
    axiom_mime_parser_get_mime_parts_map(
        axiom_mime_parser_t * mime_parser,
        const axutil_env_t * env);

    /**
      * Deallocate memory. Free the mime parser struct
      * @param mime_parser the pointer for the mime parser struct 
      * @param env Environment. MUST NOT be NULL.
      * @return VOID
      */
    AXIS2_EXTERN void AXIS2_CALL
    axiom_mime_parser_free(
        axiom_mime_parser_t * mime_parser,
        const axutil_env_t * env);

    /**
      * Returns the length of the SOAP Body
      * @param mime_parser the pointer for the mime parser struct 
      * @param env Environment. MUST NOT be NULL.
      * @return the length of the SOAP Body
      */
    AXIS2_EXTERN int AXIS2_CALL
    axiom_mime_parser_get_soap_body_len(
        axiom_mime_parser_t * mime_parser,
        const axutil_env_t * env);

    /**
      * Get the SOAP Body as a string 
      * @param mime_parser the pointer for the mime parser struct 
      * @param env Environment. MUST NOT be NULL.
      * @return the SOAP Body as a string 
      */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_mime_parser_get_soap_body_str(
        axiom_mime_parser_t * mime_parser,
        const axutil_env_t * env);

    /**
      * Create a mime parser struct
      * @param env Environment. MUST NOT be NULL.
      * @return created mime parser 
      */
    AXIS2_EXTERN axiom_mime_parser_t *AXIS2_CALL
    axiom_mime_parser_create(
        const axutil_env_t * env);

    /**
      * Set the size of the chink buffer
      * @param mime_parser the pointer for the mime parser struct 
      * @param env Environment. MUST NOT be NULL.
      * @param size the size of the chink buffer
      * @return mime parts as a hash map 
      */
    AXIS2_EXTERN void AXIS2_CALL
    axiom_mime_parser_set_buffer_size(
        axiom_mime_parser_t * mime_parser,
        const axutil_env_t * env,
        int size);

    /**
      * Set maximum number of chunk buffers
      * @param mime_parser the pointer for the mime parser struct 
      * @param env Environment. MUST NOT be NULL.
      * @param num maximum number of chunk buffers
      * @return VOID
      */
    AXIS2_EXTERN void AXIS2_CALL
    axiom_mime_parser_set_max_buffers(
        axiom_mime_parser_t * mime_parser,
        const axutil_env_t * env,
        int num);

    
    /**
      * Set attachment dir specified in the axis2.xml
      * @param mime_parser the pointer for the mime parser struct 
      * @param env Environment. MUST NOT be NULL.
      * @param attachment_dir is string containg the directory path
      * @return VOID
      */

    AXIS2_EXTERN void AXIS2_CALL
    axiom_mime_parser_set_attachment_dir(
        axiom_mime_parser_t *mime_parser,
        const axutil_env_t *env,
        axis2_char_t *attachment_dir);


    /**
      * Set Caching callback name specified in the axis2.xml
      * @param mime_parser the pointer for the mime parser struct 
      * @param env Environment. MUST NOT be NULL.
      * @param callback_name is string containg the dll path
      * @return VOID
      */


    AXIS2_EXTERN void AXIS2_CALL
    axiom_mime_parser_set_caching_callback_name(
        axiom_mime_parser_t *mime_parser,
        const axutil_env_t *env,
        axis2_char_t *callback_name);


    AXIS2_EXTERN void AXIS2_CALL
    axiom_mime_parser_set_mime_boundary(
        axiom_mime_parser_t *mime_parser,
        const axutil_env_t *env,
        axis2_char_t *mime_boundary);


    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_mime_parser_get_mime_boundary(
        axiom_mime_parser_t *mime_parser,
        const axutil_env_t *env);




    /** @} */

#ifdef __cplusplus
}
#endif
#endif                          /* AXIOM_MIME_PARSER_H */
