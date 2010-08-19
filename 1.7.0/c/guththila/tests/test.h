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

#ifndef _GUTHTHILA_TESTS_
#define _GUTHTHILA_TESTS_
#include <check.h>
#include <guththila.h>
#include "guththila_defines.h"

axutil_allocator_t *allocator;
guththila_reader_t *red;
axutil_env_t *env;
guththila_t *parser;

void setup(
    void);
void teardown(
    void);

Suite *guththila_suite(
    void);

Suite *guththila_attribute_suite(
    void);

#endif
