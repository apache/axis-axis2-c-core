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

#ifndef GUTHTHILA_STREAM_H
#define GUTHTHILA_STREAM_H

#include <guththila_allocator.h>

#ifdef __cplusplus
extern "C" {
#endif

struct guththila_stream;
struct guththila_stream_ops;

 GUTHTHILA_DECLARE_DATA typedef struct guththila_stream_ops {
    int (GUTHTHILA_CALL *guththila_stream_read)(void *buffer, size_t count);
    int (GUTHTHILA_CALL *guththila_stream_write)(const void *buffer, size_t count);
} guththila_stream_ops_t;

typedef struct guththila_stream {
    struct guththila_stream_ops *ops;
} guththila_stream_t;

GUTHTHILA_DECLARE(guththila_stream_t*) guththila_stream_create(guththila_allocator_t* allocator, guththila_stream_ops_t* operations);

#define guththila_stream_read(stream, buffer, count) ((stream)->ops->guththila_stream_read(buffer, count))
#define guththila_stream_write(stream, buffer, count) ((stream)->ops->guththila_stream_write(buffer, count))

#ifdef __cplusplus
}
#endif
        
#endif /* GUTHTHILA_STREAM_H */
