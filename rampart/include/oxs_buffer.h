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

#ifndef OXS_BUFFER_H
#define OXS_BUFFER_H


/**
  * @file oxs_buffer.h
  * @brief 
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_util.h>
#include <oxs_axiom.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup  
 * @ingroup 
 * @{
 */

#define OXS_BUFFER_INITIAL_SIZE 1024


typedef struct _oxs_buffer                    oxs_buffer,
                                *oxs_buffer_ptr;


/**
* Allocate mode for the buffer
* oxs_alloc_mode_exact : Minimizes the allocated memory size
* oxs_alloc_mode_double : Minimizes number of Malloc calls
*/
typedef enum {
    oxs_alloc_mode_exact = 0,
    oxs_alloc_mode_double
} oxs_AllocMode;

/**
* Buffer to hold data
* @data : pointer to buffer data
* @size : size of the buffer data
* @max_size : allocated size of the buffer
* @alloc_mode : Mode of the allcoation
*/
typedef struct _oxs_buffer{
    unsigned char* data;
    unsigned int size;
    unsigned int max_size;
    oxs_AllocMode alloc_mode;
}; 

AXIS2_EXTERN oxs_buffer_ptr AXIS2_CALL
oxs_create_buffer(const axis2_env_t *env, unsigned int size);

AXIS2_EXTERN oxs_buffer_ptr AXIS2_CALL
oxs_buffer_initialize(const axis2_env_t *env ,oxs_buffer_ptr buf,  unsigned int size);

AXIS2_EXTERN int AXIS2_CALL
oxs_free_buffer(const axis2_env_t *env, oxs_buffer_ptr buf);

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_remove_head(const axis2_env_t *env, oxs_buffer_ptr buf, unsigned int size);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_buffer_remove_tail(const axis2_env_t *env, oxs_buffer_ptr buf, unsigned int size);

AXIS2_EXTERN oxs_buffer_ptr AXIS2_CALL
oxs_string_to_buffer(const axis2_env_t *env, axis2_char_t* string);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_buffer_to_string(const axis2_env_t *env, oxs_buffer_ptr buf);

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_append(const axis2_env_t *env, oxs_buffer_ptr buf, unsigned char* data, unsigned int size);

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_prepend(const axis2_env_t *env, oxs_buffer_ptr buf, unsigned char* data, unsigned int size);

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_set_size(const axis2_env_t *env, oxs_buffer_ptr buf, unsigned int size);

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_set_max_size(const axis2_env_t *env, oxs_buffer_ptr buf, unsigned int size);

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_read_file(const axis2_env_t *env, oxs_buffer_ptr buf, const char* filename);

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_base64_node_content_read(const axis2_env_t *env, oxs_buffer_ptr buf, axiom_node_t *node);
/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_BUFFER_H */
