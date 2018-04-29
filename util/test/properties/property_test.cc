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

#include <axutil_env.h>
#include "../util/create_env.h"
#include <axutil_string.h>
#include <axutil_properties.h>

void printProperties(axutil_properties_t * properties, axutil_env_t *env)
{
    axutil_hash_t* all_properties = NULL;
    all_properties = axutil_properties_get_all(properties,env);
    if(all_properties)
    {
       axutil_hash_index_t *hi = NULL;
       axis2_char_t *key = NULL;
       axis2_char_t *value = NULL;
       for(hi = axutil_hash_first(all_properties, env); hi; hi = axutil_hash_next(env, hi))
        {
          axutil_hash_this(hi, (const void **)&key, NULL, (void **)&value);
            if(key)
            {
                if(!value)
                {
                    value = (axis2_char_t *) axutil_strdup(env, "");
                }
                printf("%s=%s\n", key, value);
            }
        }
    }
}

class TestProperties: public ::testing::Test
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


/** @brief test properties
  * read file and give the output
  */
TEST_F(TestProperties, test_properties)
{
    axutil_hash_t* all_properties = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t* input_filename = "test.doc";
    axutil_properties_t * properties = NULL;
    axis2_status_t  store_properties ;
    axis2_status_t  load_properties ;
    axis2_char_t * key = "key";
    axis2_char_t * value = "value";
    FILE *input = fopen("input.doc","rb");
    FILE *output = fopen("output.doc","wb");

    ASSERT_NE(input, nullptr);
    ASSERT_NE(output, nullptr);

    properties = axutil_properties_create(m_env);
    ASSERT_NE(properties, nullptr);

    status = axutil_properties_set_property(properties,m_env, key, value);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    printProperties(properties, m_env);

	store_properties = AXIS2_SUCCESS;
/* Not used, program aborts on Windows with MSVC (Visual Studio 2008 Express Edition)
	store_properties = axutil_properties_store(properties,env,output);
    if(!store_properties)
    {
        printf("Can not store the properties\n");
        axutil_properties_free(properties,env);
        return AXIS2_FAILURE;
    }
    else 
    printf("The test axutil_properties_store is successfull\n");
*/
    load_properties = axutil_properties_load(properties,m_env,input_filename);   
    ASSERT_NE(load_properties, 0);

    printProperties(properties, m_env);

    all_properties = axutil_properties_get_all(properties,m_env);
    ASSERT_NE(all_properties, nullptr);

    axutil_properties_free(properties,m_env);

}

