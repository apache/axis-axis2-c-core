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

#ifndef AXIS2_DATA_HANDLER_H
#define AXIS2_DATA_HANDLER_H

/**
 * @file axis2_data_handler.h
 * @brief axis2 data_handler interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_handler_desc.h>


#ifdef __cplusplus
extern "C"
{
#endif


/* This should be moved to header file later axis2_defines.h*/    
#define axis2_byte_t char
	
typedef struct axis2_data_handler_ops axis2_data_handler_ops_t;
typedef struct axis2_data_handler axis2_data_handler_t;	

/*
      private final DataSource ds;
      private final DataFlavor flavor;
      private CommandMap commandMap;
      private DataContentHandler dch;
*/

/** @defgroup axis2_data_handler Flow
  * @ingroup axis2_data_handler
  * @{
  */

/** 
 * @brief Flow ops struct
 * Encapsulator struct for ops of axis2_data_handler
 */
struct axis2_data_handler_ops
{
	axis2_char_t *(AXIS2_CALL *
    get_content_type)(axis2_data_handler_t *data_handler, 
        axis2_env_t **env); 
	
	axis2_byte_t* (AXIS2_CALL *
    get_input_stream)(axis2_data_handler_t *data_handler, 
        axis2_env_t **env); 

    axis2_status_t (AXIS2_CALL *
    read_from)(axis2_data_handler_t *data_handler, 
        axis2_env_t **env, 
        axis2_byte_t** output_stream, 
        int *output_stream_size);

    axis2_status_t (AXIS2_CALL *
    set_binary_data)(axis2_data_handler_t *data_handler, 
        axis2_env_t **env, 
        axis2_byte_t* input_stream, 
        int input_stream_len);

    axis2_status_t (AXIS2_CALL *
    write_to)(axis2_data_handler_t *data_handler, 
        axis2_env_t **env);

    axis2_status_t ( AXIS2_CALL *
    set_file_name )(
        axis2_data_handler_t *data_handler, 
        axis2_env_t **env, 
        axis2_char_t* file_name);
    
    axis2_status_t (AXIS2_CALL *
    free) (axis2_data_handler_t *data_handler, 
        axis2_env_t **env);
};

/** 
 * @brief Flow struct
 *	Flow  
 */ 
struct axis2_data_handler
{
	axis2_data_handler_ops_t *ops;
	
	/*
    axis2_char_t type;
    *void handler;	
	*/
};

/**
 * Creates data_handler struct
 * @return pointer to newly created data_handler
 */
AXIS2_DECLARE(axis2_data_handler_t *) 
axis2_data_handler_create (axis2_env_t **env, axis2_char_t *file_name, axis2_char_t *mime_type);

/*************************** Function macros **********************************/

#define AXIS2_DATA_HANDLER_FREE(data_handler, env) \
    ((data_handler)->ops->free (data_handler, env))

#define AXIS2_DATA_HANDLER_GET_CONTENT_TYPE(data_handler, env) \
    ((data_handler)->ops->get_content_type (data_handler, env))

#define AXIS2_DATA_HANDLER_GET_INPUT_STREAM(data_handler, env) \
    ((data_handler)->ops->get_input_stream (data_handler, env))

#define AXIS2_DATA_HANDLER_READ_FROM(data_handler, env, output_stream, output_stream_size) \
    ((data_handler)->ops->read_from (data_handler, env, output_stream, output_stream_size))

#define AXIS2_DATA_HANDLER_SET_BINARY_DATA(data_handler, env, input_stream, input_stream_size) \
    ((data_handler)->ops->set_binary_data (data_handler, env, input_stream, input_stream_size))

#define AXIS2_DATA_HANDLER_WRITE_TO(data_handler, env) \
    ((data_handler)->ops->write_to (data_handler, env))

#define AXIS2_DATA_HANDLER_SET_FILE_NAME(data_handler, env, file_name) \
    ((data_handler)->ops->set_file_name (data_handler, env, file_name))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_DATA_HANDLER_H */
