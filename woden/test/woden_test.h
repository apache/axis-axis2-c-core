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

#ifndef XML_SCHEMA_TEST_H
#define XML_SCHEMA_TEST_H

#include <CuTest.h>
#include <stdio.h>

CuSuite* xml_schema_GetSuite();

void test_simple_type_schema_generation(CuTest *tc);
void test_any_attribute(CuTest *tc);
void test_block_content(CuTest *tc);
void test_circular_schema(CuTest *tc);
void test_schema_import1(CuTest *tc);
void test_schema_import2(CuTest *tc);
void test_mixed_content(CuTest *tc);
void test_local_elements(CuTest *tc);
void test_element_refs(CuTest *tc);
void test_forward_refs(CuTest *tc);
void test_local_unnamed_simple_type(CuTest *tc);
void test_simple_restriction(CuTest *tc);
void test_unqualified_schemas(CuTest *tc);
void test_two_schmes(CuTest *tc);

#endif /* XML_SCHEMA_TEST_H */
