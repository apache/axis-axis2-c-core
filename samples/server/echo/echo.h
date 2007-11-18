
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
#ifndef CALC_H
#define CALC_H

#include <axis2_svc_skeleton.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axiom_text.h>
#include <axiom_node.h>
#include <axiom_element.h>


/* USER_ERROR_NO_# defined wrt USER_ERROR_CODES_START*/
#define USER_ERROR_NO_1 (USER_ERROR_CODES_START + 1) 
#define USER_ERROR_NO_2 (USER_ERROR_CODES_START + 2)
#define USER_ERROR_NO_3 (USER_ERROR_CODES_START + 3)
#define USER_ERROR_NO_4 (USER_ERROR_CODES_START + 4)

axiom_node_t *axis2_echo_echo(
    const axutil_env_t * env,
    axiom_node_t * node);

#endif                          /* CALC_H */
