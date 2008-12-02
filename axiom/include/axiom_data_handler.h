
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

#ifndef AXIOM_DATA_HANDLER_H
#define AXIOM_DATA_HANDLER_H

/**
 * @file axiom_data_handler.h
 * @brief axis2 data_handler interface
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

    typedef enum axiom_data_handler_type
    {
        AXIOM_DATA_HANDLER_TYPE_FILE,
        AXIOM_DATA_HANDLER_TYPE_BUFFER,
        AXIOM_DATA_HANDLER_TYPE_CALLBACK
    } axiom_data_handler_type_t;

    typedef struct axiom_data_handler axiom_data_handler_t;

    /** @defgroup axiom_data_handler Flow
      * @ingroup axiom_data_handler
      * @{
      */

    /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
     * @return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_data_handler_get_content_type(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env);
	
    /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
	 * @param mime type, 
     * @return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_data_handler_set_content_type(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env,
		const axis2_char_t *mime_type);

    /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
     * @return bool whether attachment is cached or not
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axiom_data_handler_get_cached(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env);
	
    /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
	 * @param cached, 
     * @return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
     */
    AXIS2_EXTERN void AXIS2_CALL
    axiom_data_handler_set_cached(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env,
		axis2_bool_t cached);


    /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
     * @return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
     */
    AXIS2_EXTERN axis2_byte_t *AXIS2_CALL

    axiom_data_handler_get_input_stream(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env);

    /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
     * @return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
     */
    AXIS2_EXTERN int AXIS2_CALL
    axiom_data_handler_get_input_stream_len(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env);

    /**
     * The data_handler is responsible for memory occupied by the stream 
     * returned
     * @param output_stream parameter to store reference to output byte stream.
     * @param output_stream_size parameter to store reference to output byte 
     * stream length
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_data_handler_read_from(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env,
        axis2_byte_t ** output_stream,
        int *output_stream_size);

    /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
     * @return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL

    axiom_data_handler_set_binary_data(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env,
        axis2_byte_t * input_stream,
        int input_stream_len);

    /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
     * @return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_data_handler_write_to(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env);

    /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
     * @return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_data_handler_set_file_name(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env,
        axis2_char_t * file_name);

    /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
     * @return file name, in the case of file type data handler.
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_data_handler_get_file_name(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env);

    /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
     * @return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
     */
    AXIS2_EXTERN void AXIS2_CALL
    axiom_data_handler_free(
        axiom_data_handler_t * data_handler,
        const axutil_env_t * env);

    /**
     * Creates data_handler struct
     * @return pointer to newly created data_handler
     */
    AXIS2_EXTERN axiom_data_handler_t *AXIS2_CALL
    axiom_data_handler_create(
        const axutil_env_t * env,
        const axis2_char_t * file_name,
        const axis2_char_t * mime_type);

    /* Add the binary to the array_list
     * @param data_handler, a pointer to data handler struct
     * data_handler, a pointer to data handler struct
     * list, a pointer to an array_list which containing some message parts need 
     * to be written to the wire
     * data_handler, a pointer to data handler struct
     */ 
    
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_data_handler_add_binary_data(
        axiom_data_handler_t *data_handler,
        const axutil_env_t *env,
        axutil_array_list_t *list);
   
 
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axiom_data_handler_get_mime_id(
        axiom_data_handler_t *data_handler,
        const axutil_env_t *env);

     /**
     * @param data_handler, a pointer to data handler struct
     * @param env environment, MUST NOT be NULL.
     * @param mime id, 
     * @return status code, AXIS2_SUCCESS on success and AXIS2_FAILURE on error.
     */

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_data_handler_set_mime_id(
        axiom_data_handler_t *data_handler,
        const axutil_env_t *env,
        const axis2_char_t *mime_id);


    AXIS2_EXTERN axiom_data_handler_type_t AXIS2_CALL
    axiom_data_handler_get_data_handler_type(
        axiom_data_handler_t *data_handler,
        const axutil_env_t *env);

    AXIS2_EXTERN void AXIS2_CALL
    axiom_data_handler_set_data_handler_type(
        axiom_data_handler_t *data_handler,
        const axutil_env_t *env,
        axiom_data_handler_type_t data_handler_type);

    AXIS2_EXTERN void *AXIS2_CALL
    axiom_data_handler_get_user_param(
        axiom_data_handler_t *data_handler,
        const axutil_env_t *env);

    AXIS2_EXTERN void AXIS2_CALL
    axiom_data_handler_set_user_param(
        axiom_data_handler_t *data_handler,
        const axutil_env_t *env,
        void *user_param);

    
    /** @} */

#ifdef __cplusplus
}
#endif
#endif                          /* AXIOM_DATA_HANDLER_H */
