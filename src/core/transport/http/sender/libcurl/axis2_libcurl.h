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

#ifndef AXIS2_LIBCURL_H
#define AXIS2_LIBCURL_H
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <axis2_http_sender.h>
#include <axutil_string.h>
#include <axutil_types.h>
#include <axutil_generic_obj.h>
#include <axis2_const.h>
#include <axis2_util.h>
#include <axiom.h>

/* typedef struct axis2_libcurl axis2_libcurl_t; */
/* actually defined in axis2_http_sender.h as it is part of axis2/include */

AXIS2_EXTERN axis2_libcurl_t *AXIS2_CALL 
axis2_libcurl_create(
    const axutil_env_t * env);

AXIS2_EXTERN void AXIS2_CALL 
axis2_libcurl_free(
    axis2_libcurl_t *data,
    const axutil_env_t * env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_libcurl_send(
    axis2_libcurl_t *data,
    axiom_output_t * om_output,
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axiom_soap_envelope_t * out,
    const axis2_char_t * str_url,
    const axis2_char_t * soap_action);

#endif                          /* AXIS2_LIBCURL_H */
