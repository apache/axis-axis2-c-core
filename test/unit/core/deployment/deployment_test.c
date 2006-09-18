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

#include "deployment_test.h"
#include "test_dep_engine.h"
#include <axis2_allocator.h>
#include <axis2_env.h>

CuSuite* axis2_deploymentGetSuite()
{
    CuSuite* suite = CuSuiteNew();

    /**
      * Test cases for axis2_phases_info
      */
    /*SUITE_ADD_TEST(suite, Testaxis2_phases_info_get_op_in_phases );*/

    /**
      * Test cases for axis2_dep_engine
      */
    SUITE_ADD_TEST(suite, Testaxis2_dep_engine_free);
    SUITE_ADD_TEST(suite, Testaxis2_dep_engine_create);
    SUITE_ADD_TEST(suite, Testaxis2_dep_engine_create_with_repos_name);
    SUITE_ADD_TEST(suite, Testaxis2_dep_engine_load);

    return suite;
}
