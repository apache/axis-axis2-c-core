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

#include "description_test.h"
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_env.h>

CuSuite* axis2_descriptionGetSuite() 
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Testaxis2_handler_desc_free); 
    SUITE_ADD_TEST(suite, Testaxis2_flow_add_handler); 
    SUITE_ADD_TEST(suite, Testaxis2_flow_free);
    SUITE_ADD_TEST(suite, Testaxis2_op_set_remaining_phases_inflow);
    SUITE_ADD_TEST(suite, Testaxis2_svc_add_module_ops);
    SUITE_ADD_TEST(suite, Testaxis2_svc_create_with_qname);
    return suite;
}

