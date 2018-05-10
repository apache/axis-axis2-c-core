
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

#include <stdio.h>
#include <string.h>
#include <axutil_error_default.h>
#include <axutil_log.h>
#include <axutil_log_default.h>
#include <axutil_allocator.h>
#include <axutil_utils.h>
#include "test_thread.h"

const axutil_env_t *env = NULL;
static axutil_thread_mutex_t *thread_lock = NULL;
static axutil_thread_once_t *control = NULL;
static int x = 0;
static int value = 0;
static int param_data;

static axutil_thread_t *t1 = NULL;
static axutil_thread_t *t2 = NULL;

/*for detach tests*/
static axutil_thread_t *t3 = NULL;
static axutil_thread_t *t4 = NULL;

void
init_func(
    void)
{
    value++;
}

void
test_thread_init(
    const axutil_env_t * env)
{
    axutil_allocator_t *allocator = NULL;

    allocator = env->allocator;

    control = axutil_thread_once_init(allocator);
    ASSERT_NE(control, nullptr);

    thread_lock =
        axutil_thread_mutex_create(allocator, AXIS2_THREAD_MUTEX_DEFAULT);
    ASSERT_NE(thread_lock, nullptr);
}

void *AXIS2_CALL
test_function(
    axutil_thread_t * td,
    void *param)
{
    int i  = *((int *) param);
    printf("thread data = %d \n", i);
	param_data = i;

    axutil_thread_once(control, init_func);
    EXPECT_EQ(value, 1);
    axutil_thread_mutex_lock(thread_lock);
    printf("x = %d \n", ++x);
    axutil_thread_mutex_unlock(thread_lock);

    /*axutil_thread_exit(td, env->allocator); */

    return (void *) 1;
}

void
test_axutil_thread_create(
    const axutil_env_t * env)
{
    axis2_status_t rv = AXIS2_FAILURE;
    axutil_allocator_t *allocator = NULL;
    int *i = NULL,
        *j = NULL;

    allocator = env->allocator;
    i = (int*)AXIS2_MALLOC(allocator, sizeof(int));
    *i = 5;
    param_data = -1;
    t1 = axutil_thread_create(allocator, NULL, test_function, (void *) i);
    ASSERT_NE(t1, nullptr);
    AXIS2_SLEEP(1);
    ASSERT_EQ(param_data, *i);


    j = (int*)AXIS2_MALLOC(allocator, sizeof(int));
    *j = 25;
    param_data = -1;
    t2 = axutil_thread_create(allocator, NULL, test_function, (void *) j);
    ASSERT_NE(t2, nullptr);
    AXIS2_SLEEP(1);
    ASSERT_EQ(param_data, *j);

    rv = axutil_thread_join(t1);
    ASSERT_EQ(rv, AXIS2_SUCCESS);

    rv = axutil_thread_join(t2);
    ASSERT_EQ(rv, AXIS2_SUCCESS);

}

void *AXIS2_CALL
test_function2(
    axutil_thread_t * td,
    void *param)
{
    printf("thread \n");
    return (void *) 1;
}

void
test_axutil_thread_detach(
    const axutil_env_t * env)
{
    axutil_threadattr_t *attr = NULL;
    axutil_allocator_t *allocator = NULL;
    axis2_status_t rv = AXIS2_FAILURE;

    allocator = env->allocator;
    attr = axutil_threadattr_create(allocator);
    ASSERT_NE(attr, nullptr);
    rv = axutil_threadattr_detach_set(attr, 1);
    ASSERT_EQ(rv, AXIS2_SUCCESS);
    t3 = axutil_thread_create(allocator, attr, test_function2, NULL);
    ASSERT_NE(t3, nullptr);

    /*
     * thread is already detached - should return AXIS2_FAILURE
     */
    rv = axutil_thread_detach(t3);
    ASSERT_EQ(rv, AXIS2_FAILURE);
    AXIS2_FREE(allocator, attr);
 }

void
test_axutil_thread_detach2(
    const axutil_env_t * env)
{
    axutil_threadattr_t *attr = NULL;
    axutil_allocator_t *allocator = NULL;
    axis2_status_t rv = AXIS2_FAILURE;

    allocator = env->allocator;
    attr = axutil_threadattr_create(allocator);
    ASSERT_NE(attr, nullptr);

    t4 = axutil_thread_create(allocator, attr, test_function2, NULL);
    ASSERT_NE(t4, nullptr);
    /*
     * thread is not detached yet - should return AXIS2_SUCCESS
     */
    rv = axutil_thread_detach(t4);
    ASSERT_EQ(rv, AXIS2_SUCCESS);
    AXIS2_FREE(allocator, attr);
}

void
check_locks(
    )
{
    ASSERT_EQ(x, 2);
}

void
check_thread_once(
    )
{
    ASSERT_EQ(value, 1);
}

class TestThread: public ::testing::Test
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


TEST_F(TestThread, test_thread)
{

    test_thread_init(m_env);
    test_axutil_thread_create(m_env);
    check_locks();
    check_thread_once();
    test_axutil_thread_detach(m_env);
    test_axutil_thread_detach2(m_env);

#if defined (WIN32)
    Sleep(1000);                /*to give time for detached threads to execute */
#else
    sleep(2);
#endif

    axutil_thread_mutex_destroy(thread_lock);
}
