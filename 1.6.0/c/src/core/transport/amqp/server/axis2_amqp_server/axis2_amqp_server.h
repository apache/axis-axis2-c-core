/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      tcp://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_AMQP_SERVER_H
#define AXIS2_AMQP_SERVER_H

#include <axis2_amqp_defines.h>
#include <platforms/axutil_platform_auto_sense.h>

axutil_env_t*
init_server_env(axutil_allocator_t* allocator,
		const axis2_char_t* log_file_name);

void 
server_exit(int status);

void
show_usage(axis2_char_t* prog_name);

#ifndef WIN32

void
sig_handler(int signal);

#endif

#endif
