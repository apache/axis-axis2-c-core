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

#ifndef AXIS2_MIME_OUTPUT_H
#define AXIS2_MIME_OUTPUT_H

/**
 * @file axis2_mime_output.h
 * @brief axis2 mime_output interface
 */

#include <axis2_utils.h>
#include <axis2_error.h>
#include <axis2_utils_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>


#ifdef __cplusplus
extern "C"
{
#endif

/* This should be moved to header file later axis2_utils_defines.h*/    
#define axis2_byte_t char
	
typedef struct axis2_mime_output_ops axis2_mime_output_ops_t;
typedef struct axis2_mime_output axis2_mime_output_t;	
	

/** @defgroup axis2_mime_output Flow
  * @ingroup axis2_mime_output
  * @{
  */

/** 
 * @brief Flow ops struct
 * Encapsulator struct for ops of axis2_mime_output
 */
struct axis2_mime_output_ops
{

    axis2_byte_t* (AXIS2_CALL *complete)(axis2_mime_output_t *mime_output,
                            axis2_env_t **env, 
                            axis2_byte_t **output_stream,
							int *output_stream_size,
                            axis2_char_t *soap_body_buffer,
                            axis2_array_list_t *binary_node_list,
                            axis2_char_t *boundary, 
                            axis2_char_t *content_id,
                            axis2_char_t *char_set_encoding,
                            axis2_char_t *soap_content_type);

    axis2_char_t* (AXIS2_CALL *get_content_type_for_mime)(axis2_mime_output_t *mime_output, axis2_env_t **env, 
                        axis2_char_t *boundary, axis2_char_t *content_id, axis2_char_t *char_set_encoding, axis2_char_t *soap_content_type);
    
	/** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *free) (axis2_mime_output_t *mime_output,
                            axis2_env_t **env);

};

/** 
 * @brief Flow struct
 *	Flow  
 */ 
struct axis2_mime_output
{
	axis2_mime_output_ops_t *ops;
};

/**
 * Creates mime_output struct
 * @return pointer to newly created mime_output
 */
AXIS2_DECLARE(axis2_mime_output_t *) 
axis2_mime_output_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_MIME_OUTPUT_FREE(mime_output, env) ((mime_output)->ops->free (mime_output, env))

#define AXIS2_MIME_OUTPUT_COMPLETE(mime_output, env, output_stream, output_stream_size, soap_body_buffer, binary_node_list, boundary, content_id, char_set_encoding, soap_content_type) \
((mime_output)->ops->complete(mime_output, env, output_stream, output_stream_size, soap_body_buffer, binary_node_list, boundary, content_id, char_set_encoding, soap_content_type))

#define AXIS2_MIME_OUTPUT_GET_CONTENT_TYPE_FOR_MIME(mime_output, env, boundary, content_id, char_set_encoding, soap_content_type) \
((mime_output)->ops->get_content_type_for_mime(mime_output, env, boundary, content_id, char_set_encoding, soap_content_type))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_MIME_OUTPUT_H */
