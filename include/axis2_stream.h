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

struct axis2_stream;
struct axis2_stream_ops;

typedef struct axis2_stream_ops {
    int (*axis2_stream_read)(void *buffer, size_t count);
    int (*axis2_stream_write)(const void *buffer, size_t count);
} axis2_stream_ops_t;

typedef struct axis2_stream {
    struct axis2_stream_ops *ops;
} axis2_stream_t;

axis2_stream_t *axis2_stream_create(axis2_allocator_t* allocator, axis2_stream_ops_t* operations);

#define axis2_stream_read(stream, buffer, count) ((stream)->ops->axis2_stream_read(buffer, count))
#define axis2_stream_write(stream, buffer, count) ((stream)->ops->axis2_stream_write(buffer, count))
        
#endif /* AXIS2_STREAM_H */
