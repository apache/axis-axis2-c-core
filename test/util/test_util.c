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
#include <axis2_error_default.h>
#include <axis2_stream_default.h>

typedef struct a
{
    axis2_char_t *value;
} a;

axis2_env_t *test_init()
{
	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_error_t *error = axis2_error_create (allocator);
	axis2_stream_t *stream = axis2_stream_create (allocator, NULL);
    axis2_env_t *env = axis2_env_create_with_error_stream (allocator, error, stream);
	return env;
}

int test_hash_get (axis2_env_t *env)
{
    axis2_hash_t *ht;
    a *a1, *a2, *a3, *a4;

    axis2_hash_index_t *i = 0;
    void *v = NULL;

    char *key1 = "key1";
    char *key2 = "key2";
    char *key3 = "key3";
    char *key4 = "key4";

    a1 = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    a2 = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    a3 = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    a4 = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));


    a1->value = AXIS2_STRDUP("value1", &env);
    a2->value = AXIS2_STRDUP("value2", &env);
    a3->value = AXIS2_STRDUP("value3", &env);
    a4->value = AXIS2_STRDUP("value4", &env);

    ht = axis2_hash_make (&env);

    axis2_hash_set (ht, key1, AXIS2_HASH_KEY_STRING, a1);
    axis2_hash_set (ht, key2, AXIS2_HASH_KEY_STRING, a2);
    axis2_hash_set (ht, key3, AXIS2_HASH_KEY_STRING, a3);
    axis2_hash_set (ht, key4, AXIS2_HASH_KEY_STRING, a4);

    for (i = axis2_hash_first (ht, &env); i; i = axis2_hash_next (&env, i))
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
int test_file_diff(axis2_env_t *env)
{
	/*axis2_char_t *expected_file_name = AXIS2_STRDUP("expected", &env);
    axis2_char_t *actual_file_name = AXIS2_STRDUP("actual", &env);	*/
	axis2_char_t *expected_file_name = strdup("expected");
    axis2_char_t *actual_file_name = strdup("actual");	
    axis2_file_diff(env, expected_file_name, actual_file_name);
	return 0;
}

char* test_funct_for_test_env_null(axis2_env_t **env)
{
	if(NULL == *env)
	{
		axis2_allocator_t *allocator = axis2_allocator_init (NULL);
		axis2_error_t *error = axis2_error_create (allocator);
		axis2_stream_t *stream = axis2_stream_create (allocator, NULL);
        *env = axis2_env_create_with_error_stream (allocator, error, stream);
		AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
		AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_ENVIRONMENT_IS_NULL);		
		return NULL;
	}
	return "env not null, so be happy";	
}

int test_env_null()
{
	axis2_env_t *env;
	/* Suppose we did properly initialized env here */
	/* But here we mistakenly make env to null */
	env = NULL;
	/*Now we call an axis2 mock function called
	 * test_funct_for_test_env_null
	 */
	char *msg = test_funct_for_test_env_null(&env);
	int status = axis2_env_check_status(&env);
	if(AXIS2_SUCCESS == status)
		printf("%s\n", msg);
	else
		printf("status code is:%d\n", status);
	return 0;
}

int main(void)
{
	axis2_env_t *env = test_init();
	test_file_diff(env);
	test_hash_get(env);
	test_env_null();

	return 0;	
}
