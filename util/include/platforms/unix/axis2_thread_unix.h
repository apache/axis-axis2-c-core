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

#ifndef AXIS2_THREAD_UNIX_H
#define AXIS2_THREAD_UNIX_H

#include <axis2_thread.h>
#include <pthread.h>

#define SHELL_PATH "/bin/sh"

typedef pthread_t axis2_os_thread_t; /* Native thread */

struct axis2_thread_t {
    pthread_t *td;
    void *data;
    axis2_thread_start_t func;
};

struct axis2_threadattr_t {
    pthread_attr_t attr;
};

struct axis2_threadkey_t {
    pthread_key_t key;
};

struct axis2_thread_once_t {
    pthread_once_t once;
};

/*************************Thread locking functions*****************************/
struct axis2_thread_mutex_t {
    axutil_allocator_t *allocator;
    pthread_mutex_t mutex;
};

#endif  /* AXIS2_THREAD_UNIX_H */
