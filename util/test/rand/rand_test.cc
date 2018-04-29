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

#include <gtest/gtest.h>

#include "../util/create_env.h"
#include <axutil_rand.h>

class TestRand: public ::testing::Test
{

    protected:
        void SetUp()
        {

            m_allocator = axutil_allocator_init(NULL);
            m_axis_log = axutil_log_create(m_allocator, NULL, NULL);
            m_error = axutil_error_create(m_allocator);

            m_env = axutil_env_create_with_error_log(m_allocator, m_error, m_axis_log);

        }

        void TearDown()
        {
            axutil_env_free(m_env);
        }


        axutil_allocator_t *m_allocator = NULL;
        axutil_env_t *m_env = NULL;
        axutil_error_t *m_error = NULL;
        axutil_log_t *m_axis_log = NULL;

};


/** @brief test_rand
 *   create random variable and get it's value
 */

TEST_F(TestRand, test_rand)
{

    int rand_number,rand_value,start = 2,end = 8,rand_range;
    unsigned seed = 10;

    rand_number = axutil_rand(&seed);
    printf("rand_number : %d\n", rand_number);
    rand_range = axutil_rand_with_range(&seed,start,end);
    printf("rand_range : %d\n", rand_range);
    ASSERT_NE(rand_range, -1);
    ASSERT_GE(rand_range, start);
    ASSERT_LE(rand_range, end);
    rand_value = axutil_rand_get_seed_value_based_on_time(m_env);
    printf("rand_based_on_time : %d\n", rand_value);


}
