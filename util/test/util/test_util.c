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
#include <axis2_array_list.h>
#include <platforms/axis2_platform_auto_sense.h>
#include <axis2_uuid_gen.h>
#include <axis2_log_default.h>
#include <axis2_log.h>
#include <axis2_dir_handler.h>
#include <axis2_file.h>
#include "axis2_log.h"
#include "test_thread.h"
#include <test_log.h>

typedef struct a
{
    axis2_char_t *value;
} a;

const axis2_env_t *test_init()
{
   axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_error_t *error = axis2_error_create (allocator);
    const axis2_env_t *env = axis2_env_create_with_error(allocator, error);
   return env;
}

int test_hash_get (const axis2_env_t *env)
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


    a1->value = AXIS2_STRDUP("value1", env);
    a2->value = AXIS2_STRDUP("value2", env);
    a3->value = AXIS2_STRDUP("value3", env);
    a4->value = AXIS2_STRDUP("value4", env);

    ht = axis2_hash_make (env);

    axis2_hash_set (ht, key1, AXIS2_HASH_KEY_STRING, a1);
    axis2_hash_set (ht, key2, AXIS2_HASH_KEY_STRING, a2);
    axis2_hash_set (ht, key3, AXIS2_HASH_KEY_STRING, a3);
    axis2_hash_set (ht, key4, AXIS2_HASH_KEY_STRING, a4);

    axis2_hash_set (ht, key2, AXIS2_HASH_KEY_STRING, NULL);
    axis2_hash_set (ht, key2, AXIS2_HASH_KEY_STRING, a2);
    for (i = axis2_hash_first (ht, env); i; i = axis2_hash_next (env, i))
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

    axis2_hash_free(ht, env);
    AXIS2_FREE(env->allocator, a1->value);
    AXIS2_FREE(env->allocator, a2->value);
    AXIS2_FREE(env->allocator, a3->value);
    AXIS2_FREE(env->allocator, a4->value);
    AXIS2_FREE(env->allocator, a1);
    AXIS2_FREE(env->allocator, a2);
    AXIS2_FREE(env->allocator, a3);
    AXIS2_FREE(env->allocator, a4);
    return 0;
}

void test_axis2_dir_handler_list_service_or_module_dirs()
{
    int i,isize;
    axis2_file_t *file = NULL;
    axis2_char_t *filename = NULL;
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_error_t *error = axis2_error_create (allocator);
    const axis2_env_t *env = axis2_env_create_with_error(allocator, error);

    axis2_char_t *pathname = AXIS2_STRDUP("/tmp/test/",env);

    axis2_array_list_t *arr_folders = AXIS2_DIR_HANDLER_LIST_SERVICE_OR_MODULE_DIRS(env,pathname);
    if (arr_folders == NULL)
    {
        printf("List of folders is NULL\n");
        return ;
    }


    isize = AXIS2_ARRAY_LIST_SIZE(arr_folders,env);
    printf("Folder array size = %d \n",isize);

    for (i =0;i<isize;++i)
    {
        file = (axis2_file_t*)AXIS2_ARRAY_LIST_GET(arr_folders,env,i);
        filename = AXIS2_FILE_GET_NAME(file,env);
        printf("filename = %s \n",filename);
    }
    printf("----end of test_axis2_dir_handler_list_service_or_module_dirs----\n");

}


/**
  * This test is intended to test whether given two files are equal or not.
  * Spaces and new lines are ignored in comparing
  */
int test_file_diff(const axis2_env_t *env)
{
   axis2_char_t *expected_file_name = AXIS2_STRDUP("expected", env);
    axis2_char_t *actual_file_name = AXIS2_STRDUP("actual", env);   
    axis2_file_diff(env, expected_file_name, actual_file_name);
   return 0;
}

char* test_funct_for_test_env_null(const axis2_env_t *env)
{
   if(NULL == *env)
   {
      axis2_allocator_t *allocator = axis2_allocator_init (NULL);
      axis2_error_t *error = axis2_error_create (allocator);
        *env = axis2_env_create_with_error(allocator, error);
      AXIS2_ERROR_SET_STATUS_CODE(env->error, AXIS2_FAILURE);
      AXIS2_ERROR_SET_ERROR_NUMBER(env->error, AXIS2_ERROR_ENVIRONMENT_IS_NULL);   
      return AXIS2_STRDUP("env is null!!!", env);
   }
    
    AXIS2_ERROR_SET_STATUS_CODE(env->error, AXIS2_SUCCESS);
   return AXIS2_STRDUP("env not null, so be happy", env);   
}

int test_env_null()
{
   const axis2_env_t *env = NULL;
   char *msg;
   int status;
   /* Suppose we did properly initialized env here */
   /* But here we mistakenly make env to null */
   env = NULL;
   /*Now we call an axis2 mock function called
    * test_funct_for_test_env_null
    */
   msg = test_funct_for_test_env_null(env);
   status = axis2_env_check_status(env);
   if(AXIS2_SUCCESS == status)
      printf("%s\n", msg);
   else
      printf("status code is:%d\n", status);

    AXIS2_FREE((env)->allocator, msg);
   return 0;
}

void test_array_list(const axis2_env_t *env)
{
    axis2_array_list_t *al;
    a *entry = NULL;
    int size;
    
    al = axis2_array_list_create (env, 1);
    printf("list size %d\n", AXIS2_ARRAY_LIST_SIZE (al, env));
    
    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    entry->value = AXIS2_STRDUP("value1", env);
    AXIS2_ARRAY_LIST_ADD (al, env, (void*)entry);
    
    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    entry->value = AXIS2_STRDUP("value2", env);
    AXIS2_ARRAY_LIST_ADD (al, env, (void*)entry);
    
    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    entry->value = AXIS2_STRDUP("value3", env);
    AXIS2_ARRAY_LIST_ADD (al, env, (void*)entry);
    
    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    entry->value = AXIS2_STRDUP("value4", env);
    AXIS2_ARRAY_LIST_ADD (al, env, (void*)entry);
    
    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    entry->value = AXIS2_STRDUP("value5", env);
    AXIS2_ARRAY_LIST_ADD (al, env, (void*)entry);

    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    entry->value = AXIS2_STRDUP("value6", env);
    AXIS2_ARRAY_LIST_ADD (al, env, (void*)entry);

    entry = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    entry->value = AXIS2_STRDUP("value7", env);
    AXIS2_ARRAY_LIST_SET (al, env, 3, (void*)entry);
    AXIS2_ARRAY_LIST_REMOVE (al, env, 2);
    
    entry = (a *) AXIS2_ARRAY_LIST_GET (al, env, 0);
    printf("entry->value:%s\n", entry->value);
    
    entry = (a *) AXIS2_ARRAY_LIST_GET (al, env, 2);
    printf("entry->value:%s\n", entry->value);
    size = AXIS2_ARRAY_LIST_SIZE (al, env);
    printf("list size %d\n", AXIS2_ARRAY_LIST_SIZE (al, env));
    
}


void test_uuid_gen(const axis2_env_t *env)
{
    char *uuid = NULL;
    printf("starting uuid_gen test...\n");
    uuid = axis2_uuid_gen(env);
    printf("Generated UUID 1:%s\n", uuid);
    AXIS2_FREE(env->allocator, uuid);
    uuid = axis2_uuid_gen(env);
    printf("Generated UUID 2:%s\n", uuid);
    AXIS2_FREE(env->allocator, uuid);
    printf("finished uuid_gen test...\n");
}

void test_log_write()
{
   char msg[10];
   printf("start of test_log_write\n\n");
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
   if (!allocator)
   {
      printf("allocator is NULL\n");
      return;
   }
   axis2_error_t *error = axis2_error_create(allocator);
   if (!error)
   {
      printf("cannot create error\n");
      return;
   }
    axis2_log_t *log22  = axis2_log_create (allocator, NULL,NULL);
   if (!log22)
   {
      printf("cannot create log\n");
      return;
   }
   log22->level = AXIS2_LOG_LEVEL_DEBUG;

    const axis2_env_t *env = axis2_env_create_with_error_log(allocator, error, log22);
   if (!env)
   {
      printf("cannot create env with error and log\n");
      return;
   }
   strcpy(msg,"abcd test123");
   AXIS2_LOG_WRITE(env->log,msg,AXIS2_LOG_LEVEL_ERROR);

   AXIS2_LOG_CRITICAL(env->log,AXIS2_LOG_SI,"log1 %s","test1");
   AXIS2_LOG_ERROR(env->log,AXIS2_LOG_SI,"log2 %d",2);
   AXIS2_LOG_WARNING(env->log,AXIS2_LOG_SI,"log3 %s","test3");
   AXIS2_LOG_INFO(env->log,AXIS2_LOG_SI,"log4 %s %s","info1","info2");
   AXIS2_LOG_DEBUG(env->log,AXIS2_LOG_SI,"log5 %s %d","test",5);
    printf("end of test_log_write \n\n");
   
}

int main(void)
{
   const axis2_env_t *env = test_init();
   test_file_diff(env);
   test_hash_get(env);
   test_env_null(); 
    test_array_list(env);
    test_uuid_gen(env);
   run_test_log();
   test_axis2_dir_handler_list_service_or_module_dirs();
   axis2_allocator_t *allocator = env->allocator;
    axis2_env_free(env);
    axis2_allocator_free(allocator);
   return 0;   
}
