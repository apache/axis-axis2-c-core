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

#include <stdio.h>
#include <axis2_hash.h>
#include <axis2_string.h>
#include <axis2_file_diff.h>

typedef struct a
{
    axis2_char_t *value;
} a;

axis2_environment_t *test_init()
{
	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_environment_t *env =
    	axis2_environment_create (allocator, NULL, NULL, NULL, NULL);
	return env;
}

int test_hash_get (axis2_environment_t *environment)
{
    axis2_hash_t *ht;
    a *a1, *a2, *a3, *a4;

    axis2_hash_index_t *i = 0;
    void *v = NULL;

    char *key1 = "key1";
    char *key2 = "key2";
    char *key3 = "key3";
    char *key4 = "key4";

    a1 = (a *) axis2_malloc(environment->allocator, sizeof (a));
    a2 = (a *) axis2_malloc(environment->allocator, sizeof (a));
    a3 = (a *) axis2_malloc(environment->allocator, sizeof (a));
    a4 = (a *) axis2_malloc(environment->allocator, sizeof (a));


    a1->value = axis2_strdup(environment->string, "value1");
    a2->value = axis2_strdup(environment->string, "value2");
    a3->value = axis2_strdup(environment->string, "value3");
    a4->value = axis2_strdup(environment->string, "value4");

    ht = axis2_hash_make (environment);

    axis2_hash_set (ht, key1, AXIS2_HASH_KEY_STRING, a1);
    axis2_hash_set (ht, key2, AXIS2_HASH_KEY_STRING, a2);
    axis2_hash_set (ht, key3, AXIS2_HASH_KEY_STRING, a3);
    axis2_hash_set (ht, key4, AXIS2_HASH_KEY_STRING, a4);

    for (i = axis2_hash_first (environment, ht); i; i = axis2_hash_next (i))
    {

        axis2_hash_this (i, NULL, NULL, &v);

        printf ("\n %s \n", ((a *) v)->value);
    }

    printf ("\n demo get %s ",
            ((a *) axis2_hash_get (ht, key1, AXIS2_HASH_KEY_STRING))->value);

    printf ("\n demo get %s ",
            ((a *) axis2_hash_get (ht, key2, AXIS2_HASH_KEY_STRING))->value);

    printf ("\n demo get %s ",
            ((a *) axis2_hash_get (ht, key3, AXIS2_HASH_KEY_STRING))->value);

    printf ("\n demo get %s \n",
            ((a *) axis2_hash_get (ht, key4, AXIS2_HASH_KEY_STRING))->value);

    return 0;
}

/**
  * This test is intended to test whether given two files are equal or not.
  * Spaces and new lines are ignored in comparing
  */
int test_file_diff(axis2_environment_t *env)
{
	axis2_char_t *expected_file_name = axis2_strdup(env->string, "expected");
    axis2_char_t *actual_file_name = axis2_strdup(env->string, "actual");	
    axis2_file_diff(env, expected_file_name, actual_file_name);
}

int main(void)
{
	axis2_environment_t *env = test_init();
	test_file_diff(env);
	test_hash_get(env); 
}
