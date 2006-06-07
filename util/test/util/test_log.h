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

#ifndef _TEST_LOG_H_
#define _TEST_LOG_H_

#include <axis2_env.h>

void run_test_log();
const axis2_env_t *create_env_with_error_log();
void test_axis2_log_write(const axis2_env_t *env);
void test_axis2_log_debug(const axis2_env_t *env);
void test_axis2_log_debug_off(const axis2_env_t *env);
void test_axis2_log_info(const axis2_env_t *env);
void test_axis2_log_info_off(const axis2_env_t *env);
void test_axis2_log_warning(const axis2_env_t *env);
void test_axis2_log_warning_off(const axis2_env_t *env);
void test_axis2_log_error(const axis2_env_t *env);
void test_axis2_log_critical(const axis2_env_t *env);

#endif /* _TEST_LOG_H_ */

