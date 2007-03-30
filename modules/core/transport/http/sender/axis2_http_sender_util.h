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

#ifndef AXIS2_HTTP_SENDER_UTIL_H
#define AXIS2_HTTP_SENDER_UTIL_H

#include <axutil_string.h>
#include <axis2_http_transport.h>
#include <string.h>
#include <axiom_output.h>
#include <axis2_op_ctx.h>
#include <axis2_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_http_client.h>
#include <axiom_xml_writer.h>
#include <axutil_property.h>
#include <axutil_param.h>
#include <axis2_types.h>
#include <axutil_generic_obj.h>

#ifdef __cplusplus
extern "C"
{
#endif

void
axis2_http_sender_util_add_header (
	const axutil_env_t *env,
	axis2_http_simple_request_t *request,
	axis2_char_t *header_name,
	const axis2_char_t *header_value);

#ifdef __cplusplus
}
#endif
#endif /* AXIS2_HTTP_SENDER_UTIL_H */
