
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
#include <axutil_hash.h>
#include <axutil_string.h>
#include <axutil_error_default.h>
#include <axutil_array_list.h>
#include <platforms/axutil_platform_auto_sense.h>
#include <axutil_uuid_gen.h>
#include <axutil_log_default.h>
#include <axutil_log.h>
#include <axutil_dir_handler.h>
#include <axutil_file.h>
#include "axutil_log.h"
#include "test_thread.h"
#include "test_log.h"
#include "test_md5.h"

extern void
run_test_string(
    axutil_env_t * env);

typedef struct a
{
    axis2_char_t *value;
}
a;

class TestUtil: public ::testing::Test
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


TEST_F(TestUtil, test_hash_get)
{

    axutil_hash_t *ht;
    a *a1,
    *a2,
    *a3,
    *a4;

    axutil_hash_index_t *i = 0;
    void *v = NULL;

    char *key1 = "key1";
    char *key2 = "key2";
    char *key3 = "key3";
    char *key4 = "key4";

    a1 = (a *) AXIS2_MALLOC(m_env->allocator, sizeof(a));
    a2 = (a *) AXIS2_MALLOC(m_env->allocator, sizeof(a));
    a3 = (a *) AXIS2_MALLOC(m_env->allocator, sizeof(a));
    a4 = (a *) AXIS2_MALLOC(m_env->allocator, sizeof(a));

    a1->value = (axis2_char_t*) axutil_strdup(m_env, "value1");
    a2->value = (axis2_char_t*) axutil_strdup(m_env, "value2");
    a3->value = (axis2_char_t*) axutil_strdup(m_env, "value3");
    a4->value = (axis2_char_t*) axutil_strdup(m_env, "value4");

    ht = axutil_hash_make(m_env);

    axutil_hash_set(ht, key1, AXIS2_HASH_KEY_STRING, a1);
    axutil_hash_set(ht, key2, AXIS2_HASH_KEY_STRING, a2);
    axutil_hash_set(ht, key3, AXIS2_HASH_KEY_STRING, a3);
    axutil_hash_set(ht, key4, AXIS2_HASH_KEY_STRING, a4);

    axutil_hash_set(ht, key2, AXIS2_HASH_KEY_STRING, NULL);
    axutil_hash_set(ht, key2, AXIS2_HASH_KEY_STRING, a2);
    for (i = axutil_hash_first(ht, m_env); i; i = axutil_hash_next(m_env, i))
    {
        axutil_hash_this(i, NULL, NULL, &v);
        printf("\n %s \n", ((a *) v)->value);
    }

    ASSERT_STREQ(((a *) axutil_hash_get(ht, key1, AXIS2_HASH_KEY_STRING))->value, "value1");
    ASSERT_STREQ(((a *) axutil_hash_get(ht, key2, AXIS2_HASH_KEY_STRING))->value, "value2");
    ASSERT_STREQ(((a *) axutil_hash_get(ht, key3, AXIS2_HASH_KEY_STRING))->value, "value3");
    ASSERT_STREQ(((a *) axutil_hash_get(ht, key4, AXIS2_HASH_KEY_STRING))->value, "value4");

    axutil_hash_free(ht, m_env);
    AXIS2_FREE(m_env->allocator, a1->value);
    AXIS2_FREE(m_env->allocator, a2->value);
    AXIS2_FREE(m_env->allocator, a3->value);
    AXIS2_FREE(m_env->allocator, a4->value);
    AXIS2_FREE(m_env->allocator, a1);
    AXIS2_FREE(m_env->allocator, a2);
    AXIS2_FREE(m_env->allocator, a3);
    AXIS2_FREE(m_env->allocator, a4);
}

TEST_F(TestUtil, test_axutil_dir_handler_list_service_or_module_dirs)
{
    int i,
     isize;
    axutil_file_t *file = NULL;
    axis2_char_t *filename = NULL;

    axis2_char_t *pathname = (axis2_char_t*) axutil_strdup(m_env, "/tmp/test/");

    axutil_array_list_t *arr_folders =
        axutil_dir_handler_list_service_or_module_dirs(m_env, pathname);
    if (arr_folders == NULL)
    {
        AXIS2_FREE(m_env->allocator, pathname);
        printf("List of folders is NULL\n");
        return;
    }

    isize = axutil_array_list_size(arr_folders, m_env);
    printf("Folder array size = %d \n", isize);

    for (i = 0; i < isize; ++i)
    {
        file = (axutil_file_t *) axutil_array_list_get(arr_folders, m_env, i);
        filename = axutil_file_get_name(file, m_env);
        printf("filename = %s \n", filename);
    }
    printf
        ("----end of test_axutil_dir_handler_list_service_or_module_dirs----\n");

    AXIS2_FREE(m_env->allocator, pathname);
}

/**
  * This test is intended to test whether given two files are equal or not.
  * Spaces and new lines are ignored in comparing
  */
TEST_F(TestUtil, test_file_diff)
{
    /* TODO */
}

TEST_F(TestUtil, test_array_list)
{

    axutil_array_list_t *al;
    a *entry = NULL;
    a *tmpentry = NULL;
    int size;

    al = axutil_array_list_create(m_env, 1);
	ASSERT_NE(al, nullptr);
    ASSERT_EQ(axutil_array_list_size(al, m_env), 0);

    entry = (a *) AXIS2_MALLOC(m_env->allocator, sizeof(a));
    entry->value = (axis2_char_t*) axutil_strdup(m_env, "value1");
    axutil_array_list_add(al, m_env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(m_env->allocator, sizeof(a));
    entry->value = (axis2_char_t*) axutil_strdup(m_env, "value2");
    axutil_array_list_add(al, m_env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(m_env->allocator, sizeof(a));
    entry->value = (axis2_char_t*) axutil_strdup(m_env, "value3");
    axutil_array_list_add(al, m_env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(m_env->allocator, sizeof(a));
    entry->value = (axis2_char_t*) axutil_strdup(m_env, "value4");
    axutil_array_list_add(al, m_env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(m_env->allocator, sizeof(a));
    entry->value = (axis2_char_t*) axutil_strdup(m_env, "value5");
    axutil_array_list_add(al, m_env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(m_env->allocator, sizeof(a));
    entry->value = (axis2_char_t*) axutil_strdup(m_env, "value6");
    axutil_array_list_add(al, m_env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(m_env->allocator, sizeof(a));
    entry->value = (axis2_char_t*) axutil_strdup(m_env, "value7");
    size = axutil_array_list_size(al, m_env);
    ASSERT_EQ(size, 6);

    tmpentry = (a *) axutil_array_list_get(al, m_env, 3);
    axutil_array_list_set(al, m_env, 3, (void *) entry);
    size = axutil_array_list_size(al, m_env);
    ASSERT_EQ(size, 6);
    AXIS2_FREE(m_env->allocator, tmpentry->value);
    AXIS2_FREE(m_env->allocator, tmpentry);
    tmpentry = NULL;

    tmpentry = (a *) axutil_array_list_get(al, m_env, 2);
    axutil_array_list_remove(al, m_env, 2);
    size = axutil_array_list_size(al, m_env);
    ASSERT_EQ(size, 5);
    AXIS2_FREE(m_env->allocator, tmpentry->value);
    AXIS2_FREE(m_env->allocator, tmpentry);
    tmpentry = NULL;

    entry = (a *) axutil_array_list_get(al, m_env, 0);
    ASSERT_STREQ(entry->value, "value1");

    entry = (a *) axutil_array_list_get(al, m_env, 2);
    ASSERT_STREQ(entry->value, "value7");
    size = axutil_array_list_size(al, m_env);
    ASSERT_EQ(size, 5);

    while (size = axutil_array_list_size(al, m_env)) {
        entry = (a *) axutil_array_list_remove(al, m_env, 0);
        AXIS2_FREE(m_env->allocator, entry->value);
        AXIS2_FREE(m_env->allocator, entry);
    }

    axutil_array_list_free(al, m_env);
}


TEST_F(TestUtil, test_uuid_gen)
{
    char *uuid = NULL;
    printf("starting uuid_gen test...\n");
    uuid = axutil_uuid_gen(m_env);
    printf("Generated UUID 1:%s\n", uuid);
    AXIS2_FREE(m_env->allocator, uuid);
    uuid = axutil_uuid_gen(m_env);
    printf("Generated UUID 2:%s\n", uuid);
    AXIS2_FREE(m_env->allocator, uuid);
    printf("finished uuid_gen test...\n");
}


TEST_F(TestUtil, test_log_write)
{
    axutil_error_t *error = NULL;
    axutil_log_t *log22 = NULL;
    axutil_env_t *env = NULL;
    axutil_allocator_t *allocator = axutil_allocator_init(NULL);
    ASSERT_NE(allocator, nullptr);
    error = axutil_error_create(allocator);
    ASSERT_NE(error, nullptr);
    log22 = axutil_log_create(allocator, NULL, NULL);
    ASSERT_NE(log22, nullptr);
    log22->level = AXIS2_LOG_LEVEL_DEBUG;
    env = axutil_env_create_with_error_log(allocator, error, log22);
    ASSERT_NE(env, nullptr);

    AXIS2_LOG_CRITICAL(env->log, AXIS2_LOG_SI, "log1 %s", "test1");
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "log2 %d", 2);
    AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "log3 %s", "test3");
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "log4 %s %s", "info1", "info2");
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "log5 %s %d", "test", 5);
    axutil_env_free(env);

}

