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
 
#ifndef AXIS2_HTTP_CHUNKED_STREAM_H
#define AXIS2_HTTP_CHUNKED_STREAM_H

/**
 * @file axis2_http_chunked_stream.h
 * @brief axis2 HTTP Chunked Stream
 */

#include <axis2.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_stream.h>


#ifdef __cplusplus
extern "C" 
{
#endif
/** 
  * @ingroup axis2_core_transport_http
  * @{
  */

typedef struct axis2_http_chunked_stream_ops axis2_http_chunked_stream_ops_t;
typedef struct axis2_http_chunked_stream axis2_http_chunked_stream_t;
    
    
/** 
 * @brief HTTP Chunked Stream ops struct
 * Encapsulator struct for ops of axis2_http_chunked_stream
 */
AXIS2_DECLARE_DATA struct axis2_http_chunked_stream_ops
{
	int (AXIS2_CALL *read) (axis2_http_chunked_stream_t *chunked_stream,
						axis2_env_t **env, void *buffer, size_t count);
	
	int (AXIS2_CALL *write) (axis2_http_chunked_stream_t *chunked_stream, 
						axis2_env_t **env, const void *buffer, 
						size_t count);
	int (AXIS2_CALL *get_current_chunk_size) 
                        (axis2_http_chunked_stream_t *chunked_stream, 
                        axis2_env_t **env);
	axis2_status_t (AXIS2_CALL *write_last_chunk) 
						(axis2_http_chunked_stream_t *chunked_stream, 
                        axis2_env_t **env);
	axis2_status_t (AXIS2_CALL *free) 
                        (axis2_http_chunked_stream_t *chunked_stream, 
                        axis2_env_t **env);
};

/**
 * @brief HTTP Chunked Stream struct
 * Axis2 HTTP Chunked Stream
 */
AXIS2_DECLARE_DATA struct axis2_http_chunked_stream
{
	axis2_http_chunked_stream_ops_t *ops;
};


AXIS2_DECLARE(axis2_http_chunked_stream_t *) 
axis2_http_chunked_stream_create(axis2_env_t **env, axis2_stream_t* stream);
/********************* Start of function macros	***************************/

#define AXIS2_HTTP_CHUNKED_STREAM_READ(chunked_stream, env, buffer, count) \
                        ((chunked_stream)->ops->read(chunked_stream, env, \
						buffer, count))
#define AXIS2_HTTP_CHUNKED_STREAM_WRITE(chunked_stream, env, buffer, count) \
						((chunked_stream)->ops->write(chunked_stream, env, \
						buffer, count))
#define AXIS2_HTTP_CHUNKED_GET_CURRENT_CHUNK_SIZE(chunked_stream, env) \
                        ((chunked_stream)->ops->get_current_chunk_size \
						(chunked_stream, env))
#define AXIS2_HTTP_CHUNKED_STREAM_WRITE_LAST_CHUNK(chunked_stream, env) \
						((chunked_stream)->ops->write_last_chunk(chunked_stream\
						, env))
#define AXIS2_HTTP_CHUNKED_STREAM_FREE(chunked_stream, env) \
                        ((chunked_stream)->ops->free(chunked_stream, env))

/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_HTTP_CHUNKED_STREAM_H */
