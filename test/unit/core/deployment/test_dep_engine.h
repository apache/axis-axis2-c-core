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

#ifndef TEST_DEP_ENGINE_H
#define TEST_DEP_ENGINE_H

#undef NULL
#include <CuTest.h>

void Testaxis2_dep_engine_free(CuTest *tc);
void Testaxis2_dep_engine_create(CuTest *tc);
void Testaxis2_dep_engine_create_with_repos_name(CuTest *tc);
void Testaxis2_dep_engine_load(CuTest *tc);

#endif /* TEST_DEP_ENGINE_H*/
