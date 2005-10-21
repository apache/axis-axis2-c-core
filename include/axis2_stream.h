/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain count copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_STREAM_H
#define AXIS2_STREAM_H

#include <axis2_allocator.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_stream;
    struct axis2_stream_ops;

/**
 * @defgroup axis2_stream Stream
 * @ingroup axis2_util 
 * @{
 */

  /** 
    * \brief Axis2 stream operations struct
    *
    * Encapsulator struct for operations of axis2_stream
    */
    typedef struct axis2_stream_ops
    {
      /**
        * reads from stream
        * @param buffer buffer into which the content is to be read
        * @param size size of the buffer
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        AXIS2_DECLARE(axis2_status_t) (*axis2_stream_ops_read) (void *buffer, size_t count);
      /**
        * writes into stream
        * @param buffer buffer to be written
        * @param size size of the buffer
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        AXIS2_DECLARE(axis2_status_t) (*axis2_stream_ops_write) (const void *buffer, size_t count);
    } axis2_stream_ops_t;

  /** 
    * \brief Axis2 Stream struct
    *
    * Stream is the encapsulating struct for all stream related operations
    */
    typedef struct axis2_stream
    {
        /** Stream related operations */
        struct axis2_stream_ops *ops;
    } axis2_stream_t;

  /**
    * Creates a stream struct
    * @param allocator allocator to be used. Mandatory, cannot be NULL
    * @param operations operations struct. Optional, can be NULL. If null default operations would be assigned.
    * @return pointer to the newly created log struct 
    */
    AXIS2_DECLARE(axis2_stream_t *) axis2_stream_create (axis2_allocator_t * allocator,
                                         axis2_stream_ops_t * operations);

#define axis2_stream_read(stream, buffer, count) ((stream)->ops->axis2_stream_ops_read(buffer, count))
#define axis2_stream_write(stream, buffer, count) ((stream)->ops->axis2_stream_ops_write(buffer, count))

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_STREAM_H */
