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

#ifndef AXIS2_SSL_UTILS_H
#define AXIS2_SSL_UTILS_H

#include <openssl/ssl.h>
#include <platforms/axis2_platform_auto_sense.h>
#include <axis2_const.h>
#include <axis2_defines.h>


#ifdef __cplusplus
extern "C"
{
#endif
BIO *bio_err = 0;

AXIS2_DECLARE (SSL_CTX *)
axis2_ssl_utils_initialize_ctx(axis2_env_t **env);

AXIS2_DECLARE (SSL *)
axis2_ssl_utils_initialize_ssl(axis2_env_t **env, SSL_CTX *ctx, 
                        axis2_socket_t socket);
                        
AXIS2_DECLARE (axis2_status_t)
axis2_ssl_utils_cleanup_ssl(axis2_env_t **env, SSL_CTX *ctx, SSL *ssl);

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_SSL_UTILS_H */
