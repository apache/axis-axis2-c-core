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

#ifndef AXIOM_MIME_BODY_PART_H
#define AXIOM_MIME_BODY_PART_H

/**
 * @file axiom_mime_body_part.h
 * @brief axis2 mime_body_part interface
 */

#include <axis2_utils.h>
#include <axis2_error.h>
#include <axis2_utils_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axiom_data_handler.h>

#ifdef __cplusplus
extern "C"
{
#endif

   
typedef struct axiom_mime_body_part_ops axiom_mime_body_part_ops_t;
typedef struct axiom_mime_body_part axiom_mime_body_part_t;   

/** @defgroup axiom_mime_body_part
  * @ingroup axiom_mime_body_part
  * @{
  */

/** 
 * @brief ops struct
 * Encapsulator struct for ops of axiom_mime_body_part
 */
struct axiom_mime_body_part_ops
{
   axis2_status_t (AXIS2_CALL *
    add_header)(axiom_mime_body_part_t *mime_body_part, 
            const axis2_env_t *env, 
            const axis2_char_t *name, 
            const axis2_char_t *value); 
   
    axis2_status_t (AXIS2_CALL *
    set_data_handler)(axiom_mime_body_part_t *mime_body_part, 
            const axis2_env_t *env, 
            axiom_data_handler_t *data_handler); 

    axis2_status_t (AXIS2_CALL *
    write_to)(axiom_mime_body_part_t *mime_body_part, 
            const axis2_env_t *env,
            axis2_byte_t **output_stream, 
            int *output_stream_size); 

   /** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axiom_mime_body_part_t *mime_body_part, 
            const axis2_env_t *env);
};

struct axiom_mime_body_part
{
   axiom_mime_body_part_ops_t *ops;
   
};

/**
 * Creates mime_body_part struct
 * @return pointer to newly created mime_body_part
 */
AXIS2_EXTERN axiom_mime_body_part_t * AXIS2_CALL 
axiom_mime_body_part_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIOM_MIME_BODY_PART_FREE(mime_body_part, env) \
    ((mime_body_part)->ops->free (mime_body_part, env))

#define AXIOM_MIME_BODY_PART_ADD_HEADER(mime_body_part, env, name, value) \
    ((mime_body_part)->ops->add_header (mime_body_part, env, name, value))

#define AXIOM_MIME_BODY_PART_SET_DATA_HANDLER(mime_body_part, env, data_handler) \
    ((mime_body_part)->ops->set_data_handler (mime_body_part, env, data_handler))

#define AXIOM_MIME_BODY_PART_WRITE_TO(mime_body_part, env, output_stream, output_stream_size) \
    ((mime_body_part)->ops->write_to (mime_body_part, env, output_stream, output_stream_size))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIOM_MIME_BODY_PART_H */
