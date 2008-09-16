
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
 
#include <axis2_const.h>

typedef struct axis2_cgi_request
{
    axis2_char_t *server_name;
    axis2_char_t *server_port;
    axis2_char_t *script_name;
    axis2_char_t *path_info;
    axis2_char_t *server_protocol;
    axis2_char_t *content_length;
    axis2_char_t *content_type;
    axis2_char_t *request_method;
    axis2_char_t *remote_addr;
    axis2_char_t *soap_action;
    axis2_char_t *query_string;
} axis2_cgi_request_t;
