
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

#include <stdio.h>
#include <string.h>
#include <axutil_error_default.h>
#include <axutil_log.h>
#include <axutil_log_default.h>
#include <axutil_allocator.h>
#include <axutil_utils.h>
#include "test_thread.h"
#include <cut_defs.h>

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
    CUT_ASSERT_PTR_NOT_EQUAL(control, NULL, 0);

    thread_lock =
        axutil_thread_mutex_create(allocator, AXIS2_THREAD_MUTEX_DEFAULT);
    CUT_ASSERT_PTR_NOT_EQUAL(thread_lock, NULL, 0);
    /* To avoid warning of not using cut_str_equal */
    CUT_ASSERT_STR_EQUAL("", "", 0);

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
    CUT_ASSERT(value==1);
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
    i = AXIS2_MALLOC(allocator, sizeof(int));
    *i = 5;
    param_data = -1;
    t1 = axutil_thread_create(allocator, NULL, test_function, (void *) i);
    CUT_ASSERT_PTR_NOT_EQUAL(t1, NULL, 0);
    AXIS2_SLEEP(1);
    CUT_ASSERT_INT_EQUAL(param_data, *i, 0);


    j = AXIS2_MALLOC(allocator, sizeof(int));
    *j = 25;
    param_data = -1;
    t2 = axutil_thread_create(allocator, NULL, test_function, (void *) j);
    CUT_ASSERT_PTR_NOT_EQUAL(t2, NULL, 0);
    AXIS2_SLEEP(1);
    CUT_ASSERT_INT_EQUAL(param_data, *j, 0);

    rv = axutil_thread_join(t1);
    CUT_ASSERT_INT_EQUAL(rv, AXIS2_SUCCESS, 0);

    rv = axutil_thread_join(t2);
    CUT_ASSERT_INT_EQUAL(rv, AXIS2_SUCCESS, 0);

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
    CUT_ASSERT_PTR_NOT_EQUAL(attr, NULL, 1);
    rv = axutil_threadattr_detach_set(attr, 1);
    CUT_ASSERT_INT_EQUAL(rv, AXIS2_SUCCESS, 1);
    t3 = axutil_thread_create(allocator, attr, test_function2, NULL);
    CUT_ASSERT_PTR_NOT_EQUAL(t3, NULL, 1);

    /*
     * thread is already detached - should return AXIS2_FAILURE
     */
    rv = axutil_thread_detach(t3);
    CUT_ASSERT_INT_EQUAL(rv, AXIS2_FAILURE, 1);
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
    CUT_ASSERT_PTR_NOT_EQUAL(attr, NULL, 1);

    t4 = axutil_thread_create(allocator, attr, test_function2, NULL);
    CUT_ASSERT_PTR_NOT_EQUAL(t4, NULL, 1);
    /*
     * thread is not detached yet - should return AXIS2_SUCCESS
     */
    rv = axutil_thread_detach(t4);
    CUT_ASSERT_INT_EQUAL(rv, AXIS2_SUCCESS, 1);
}

void
check_locks(
    )
{
    CUT_ASSERT_INT_EQUAL(x, 2, 0);
}

void
check_thread_once(
    )
{
    CUT_ASSERT_INT_EQUAL(value, 1, 0);
}

void
run_test_thread(
    const axutil_env_t * env)
{
    test_thread_init(env);
    test_axutil_thread_create(env);
    check_locks();
    check_thread_once();
    test_axutil_thread_detach(env);
    test_axutil_thread_detach2(env);

#if defined (WIN32)
    Sleep(1000);                /*to give time for detached threads to execute */
#else
    sleep(2);
#endif

    axutil_thread_mutex_destroy(thread_lock);
}

int
main(
    void)
{
    axutil_env_t *env = cut_setup_env("util thread");
	CUT_ASSERT(env != NULL);
	if (env) {
       run_test_thread(env);
       axutil_env_free(env);
    }
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}
