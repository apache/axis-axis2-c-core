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

#include "util_test.h"
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_env.h>

CuSuite* axis2_utilGetSuite() 
{
    CuSuite* suite = CuSuiteNew();
   
    /* Samisa - need to remove this as we no longer have samples in the main build*/
    SUITE_ADD_TEST(suite, Testaxis2_class_loader_create_dll);
    SUITE_ADD_TEST(suite, Testaxis2_stream_write);
    SUITE_ADD_TEST(suite, Testaxis2_log_write);
    SUITE_ADD_TEST(suite, Testaxis2_hash_get);
    SUITE_ADD_TEST(suite, Testaxis2_hash_while);
    SUITE_ADD_TEST(suite, Testaxis2_array_list_get);
    SUITE_ADD_TEST(suite, Testaxis2_linked_list_get);
    SUITE_ADD_TEST(suite, Testaxis2_strndup);
    SUITE_ADD_TEST(suite, Testaxis2_strcat);
    SUITE_ADD_TEST(suite, Testaxis2_url_parse_string);
    SUITE_ADD_TEST(suite, Testaxis2_uri_parse_string);
    SUITE_ADD_TEST(suite, Testaxis2_uri_parse_relative);
    SUITE_ADD_TEST(suite, Testaxis2_uri_clone);
    SUITE_ADD_TEST(suite, Testaxis2_uri_get_path); 
    /* Samisa - need to remove this as we run make check before make install
    SUITE_ADD_TEST(suite, Testaxis2_dir_handler_list_dir); */
    SUITE_ADD_TEST(suite, Test_properties); 
    SUITE_ADD_TEST(suite, Test_date_time); 

    return suite;
}
