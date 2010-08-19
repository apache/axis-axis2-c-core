
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
#include <cut_defs.h>
#include "axutil_log.h"
#include "test_thread.h"
#include "test_log.h"
#include "test_md5.h"
#include "test_string.c"

extern void
run_test_string(
    axutil_env_t * env);
	
typedef struct a
{
    axis2_char_t *value;
}
a;

void
test_hash_get(
    const axutil_env_t * env)
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

    a1 = (a *) AXIS2_MALLOC(env->allocator, sizeof(a));
    a2 = (a *) AXIS2_MALLOC(env->allocator, sizeof(a));
    a3 = (a *) AXIS2_MALLOC(env->allocator, sizeof(a));
    a4 = (a *) AXIS2_MALLOC(env->allocator, sizeof(a));

    a1->value = axutil_strdup(env, "value1");
    a2->value = axutil_strdup(env, "value2");
    a3->value = axutil_strdup(env, "value3");
    a4->value = axutil_strdup(env, "value4");

    ht = axutil_hash_make(env);

    axutil_hash_set(ht, key1, AXIS2_HASH_KEY_STRING, a1);
    axutil_hash_set(ht, key2, AXIS2_HASH_KEY_STRING, a2);
    axutil_hash_set(ht, key3, AXIS2_HASH_KEY_STRING, a3);
    axutil_hash_set(ht, key4, AXIS2_HASH_KEY_STRING, a4);

    axutil_hash_set(ht, key2, AXIS2_HASH_KEY_STRING, NULL);
    axutil_hash_set(ht, key2, AXIS2_HASH_KEY_STRING, a2);
    for (i = axutil_hash_first(ht, env); i; i = axutil_hash_next(env, i))
    {
        axutil_hash_this(i, NULL, NULL, &v);
        printf("\n %s \n", ((a *) v)->value);
    }

    CUT_ASSERT_STR_EQUAL(
           ((a *) axutil_hash_get(ht, key1, AXIS2_HASH_KEY_STRING))->value, "value1", 0);

    CUT_ASSERT_STR_EQUAL(
           ((a *) axutil_hash_get(ht, key2, AXIS2_HASH_KEY_STRING))->value, "value2", 0);

    CUT_ASSERT_STR_EQUAL(
           ((a *) axutil_hash_get(ht, key3, AXIS2_HASH_KEY_STRING))->value, "value3", 0);

    CUT_ASSERT_STR_EQUAL(
           ((a *) axutil_hash_get(ht, key4, AXIS2_HASH_KEY_STRING))->value, "value4", 0);

    axutil_hash_free(ht, env);
    AXIS2_FREE(env->allocator, a1->value);
    AXIS2_FREE(env->allocator, a2->value);
    AXIS2_FREE(env->allocator, a3->value);
    AXIS2_FREE(env->allocator, a4->value);
    AXIS2_FREE(env->allocator, a1);
    AXIS2_FREE(env->allocator, a2);
    AXIS2_FREE(env->allocator, a3);
    AXIS2_FREE(env->allocator, a4);
}

void
test_axutil_dir_handler_list_service_or_module_dirs(axutil_env_t *env)
{
    int i,
     isize;
    axutil_file_t *file = NULL;
    axis2_char_t *filename = NULL;

    axis2_char_t *pathname = axutil_strdup(env, "/tmp/test/");

    axutil_array_list_t *arr_folders =
        axutil_dir_handler_list_service_or_module_dirs(env, pathname);
    if (arr_folders == NULL)
    {
        printf("List of folders is NULL\n");
        return;
    }

    isize = axutil_array_list_size(arr_folders, env);
    printf("Folder array size = %d \n", isize);

    for (i = 0; i < isize; ++i)
    {
        file = (axutil_file_t *) axutil_array_list_get(arr_folders, env, i);
        filename = axutil_file_get_name(file, env);
        printf("filename = %s \n", filename);
    }
    printf
        ("----end of test_axutil_dir_handler_list_service_or_module_dirs----\n");

}

/**
  * This test is intended to test whether given two files are equal or not.
  * Spaces and new lines are ignored in comparing
  */
void test_file_diff(
    const axutil_env_t * env)
{
}

void
test_array_list(
    const axutil_env_t * env)
{
    axutil_array_list_t *al;
    a *entry = NULL;
    int size;

    al = axutil_array_list_create(env, 1);
	CUT_ASSERT_PTR_NOT_EQUAL(al, NULL, 1);
    CUT_ASSERT_INT_EQUAL(axutil_array_list_size(al, env), 0, 0);

    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof(a));
    entry->value = axutil_strdup(env, "value1");
    axutil_array_list_add(al, env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof(a));
    entry->value = axutil_strdup(env, "value2");
    axutil_array_list_add(al, env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof(a));
    entry->value = axutil_strdup(env, "value3");
    axutil_array_list_add(al, env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof(a));
    entry->value = axutil_strdup(env, "value4");
    axutil_array_list_add(al, env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof(a));
    entry->value = axutil_strdup(env, "value5");
    axutil_array_list_add(al, env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof(a));
    entry->value = axutil_strdup(env, "value6");
    axutil_array_list_add(al, env, (void *) entry);

    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof(a));
    entry->value = axutil_strdup(env, "value7");
    axutil_array_list_set(al, env, 3, (void *) entry);
    axutil_array_list_remove(al, env, 2);

    entry = (a *) axutil_array_list_get(al, env, 0);
    CUT_ASSERT_STR_EQUAL(entry->value, "value1", 0);

    entry = (a *) axutil_array_list_get(al, env, 2);
    CUT_ASSERT_STR_EQUAL(entry->value, "value7", 0);
    size = axutil_array_list_size(al, env);
    CUT_ASSERT_INT_EQUAL(size, 5, 0);
}

void
test_uuid_gen(
    const axutil_env_t * env)
{
    char *uuid = NULL;
    printf("starting uuid_gen test...\n");
    uuid = axutil_uuid_gen(env);
    printf("Generated UUID 1:%s\n", uuid);
    AXIS2_FREE(env->allocator, uuid);
    uuid = axutil_uuid_gen(env);
    printf("Generated UUID 2:%s\n", uuid);
    AXIS2_FREE(env->allocator, uuid);
    printf("finished uuid_gen test...\n");
}

void
test_log_write(void)
{
    axutil_error_t *error = NULL;
    axutil_log_t *log22 = NULL;
    axutil_env_t *env = NULL;
    axutil_allocator_t *allocator = axutil_allocator_init(NULL);
    CUT_ASSERT_PTR_NOT_EQUAL(allocator, NULL, 1);
    error = axutil_error_create(allocator);
    CUT_ASSERT_PTR_NOT_EQUAL(error, NULL, 1);
    log22 = axutil_log_create(allocator, NULL, NULL);
    CUT_ASSERT_PTR_NOT_EQUAL(log22, NULL, 1);
    log22->level = AXIS2_LOG_LEVEL_DEBUG;
    env = axutil_env_create_with_error_log(allocator, error, log22);
    CUT_ASSERT_PTR_NOT_EQUAL(env, NULL, 1);

    AXIS2_LOG_CRITICAL(env->log, AXIS2_LOG_SI, "log1 %s", "test1");
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "log2 %d", 2);
    AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "log3 %s", "test3");
    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "log4 %s %s", "info1", "info2");
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "log5 %s %d", "test", 5);
    axutil_env_free(env);

}

int
main(
    void)
{
    axutil_env_t *env = cut_setup_env("util");
    CUT_ASSERT(env != NULL);
    if (env) 
    {
        test_hash_get(env);
        test_file_diff(env);
        test_array_list(env);
        test_uuid_gen(env);
        test_md5(env);
        run_test_log();
        run_test_string(env);
        test_axutil_dir_handler_list_service_or_module_dirs(env);
        axutil_env_free(env);
    }
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}

