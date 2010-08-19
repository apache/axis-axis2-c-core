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

#include "create_env.h"

axutil_env_t * create_environment()
{
    axutil_allocator_t *allocator = NULL;
    axutil_log_t *log = NULL;
    axutil_error_t *error = NULL;
    axutil_env_t *env = NULL;
    allocator = axutil_allocator_init(NULL);
    log = axutil_log_create(allocator, NULL, NULL);

    error = axutil_error_create(allocator);
    env = axutil_env_create_with_error_log(allocator, error, log);
    return env;
}


