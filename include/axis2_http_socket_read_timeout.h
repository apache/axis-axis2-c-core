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

#ifndef AXIS2_HTTP_SOCKET_READ_TIMEOUT_H
#define AXIS2_HTTP_SOCKET_READ_TIMEOUT_H

#include <axutil_env.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup axis2_http_socket_read_timeout HTTP Socket Read Timeout
 * @ingroup axis2_http
 * @{
 */

/**
 * HTTP socket read timeout global variable
 * This variable controls the socket read timeout for HTTP operations
 */
AXIS2_EXTERN int axis2_http_socket_read_timeout;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_HTTP_SOCKET_READ_TIMEOUT_H */