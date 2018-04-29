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

#include <axutil_linked_list.h>
#include "../util/create_env.h"


class TestLinkedList: public ::testing::Test
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


TEST_F(TestLinkedList, test_link_list)
{
    char *first_item = "first entry";
    char *second_item = "secnd entry";
    char *third_item = "third entry";
    char *last_item = "last entry";
    char *array = "test";

    axutil_linked_list_t * linked_list = NULL;
    int index_of_item;
	int index_of_last_item;
	entry_t * entry;
	void *get_item;
	axis2_status_t status;
	axis2_bool_t bresult;
	void **array_from_list;

	linked_list = axutil_linked_list_create(m_env);
    ASSERT_NE(linked_list, nullptr);
    status = axutil_linked_list_add_first(linked_list,m_env,(void *)first_item);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    bresult = axutil_linked_list_contains(linked_list,m_env,(void *)second_item);
	ASSERT_EQ(bresult, AXIS2_FALSE);
    status = axutil_linked_list_add(linked_list,m_env,(void *)third_item);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    status = axutil_linked_list_add_last(linked_list,m_env,(void *)last_item);
    ASSERT_EQ(status, AXIS2_SUCCESS);
 	ASSERT_EQ(axutil_linked_list_size(linked_list,m_env), 3);
    index_of_item = axutil_linked_list_index_of(linked_list,m_env,third_item);
    ASSERT_EQ(index_of_item, 1);
    index_of_last_item = axutil_linked_list_last_index_of(linked_list,m_env,last_item);
    ASSERT_EQ(index_of_last_item, 2);
    entry = axutil_linked_list_get_entry(linked_list,m_env,0);
    ASSERT_NE(entry, nullptr);
    get_item = axutil_linked_list_get(linked_list,m_env,1);
    ASSERT_NE(get_item, nullptr);
	ASSERT_EQ(strcmp((char*)get_item, third_item), 0);
    get_item = axutil_linked_list_set(linked_list,m_env,1,(void *)array);
    ASSERT_NE(get_item, nullptr);
	ASSERT_EQ(strcmp((char*)get_item, third_item), 0);
    array_from_list = axutil_linked_list_to_array(linked_list,m_env);
	ASSERT_NE(array_from_list, nullptr);
    status = axutil_linked_list_add_at_index(linked_list,m_env,1,(void *)second_item);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    get_item = axutil_linked_list_remove_at_index(linked_list,m_env,1);
	ASSERT_NE(get_item, nullptr);
    bresult = axutil_linked_list_check_bounds_inclusive(linked_list,m_env,1);
	ASSERT_EQ(bresult, AXIS2_TRUE);
    status = axutil_linked_list_remove_entry(linked_list,m_env,entry);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    get_item = axutil_linked_list_remove_first(linked_list,m_env);
	ASSERT_NE(get_item, nullptr);
    get_item = axutil_linked_list_remove_last(linked_list,m_env);
	ASSERT_NE(get_item, nullptr);
	ASSERT_EQ(axutil_linked_list_size(linked_list,m_env), 0);

    bresult = axutil_linked_list_remove(linked_list,m_env,(void *)third_item);
 	ASSERT_EQ(bresult, AXIS2_FALSE);

    axutil_linked_list_free(linked_list,m_env);
}

