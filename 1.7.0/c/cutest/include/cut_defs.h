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

#ifndef CUT_DEFS_H
#define CUT_DEFS_H

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define CUT_LOG_LEVEL AXIS2_LOG_LEVEL_DEBUG
#define CUT_LOG_HEADER "************* testing %s **************"

#define CUT_ASSERT(value) \
  cut_assert((value), __LINE__, #value, __FILE__);

#define CUT_ASSERT_RETURN(value) \
  if (!cut_assert((value), __LINE__, #value, __FILE__)) return;

#define CUT_RETURN_ON_FAILURE(cr) \
    if (cut_nb_assert_failures) return cr;

#define CUT_ASSERT_PTR_EQUAL(ptr,value,bret) \
  if (!cut_ptr_equal((void*)(ptr), (void*)(value), 1, __LINE__, __FILE__) && bret) return;

#define CUT_ASSERT_PTR_NOT_EQUAL(ptr,value,bret) \
  if (!cut_ptr_equal((void*)(ptr), (void*)(value), 0, __LINE__, __FILE__) && bret) return;

#define CUT_ASSERT_INT_EQUAL(value,expected,bret) \
  if (!cut_int_equal((long)(value), (long)(expected), 1, __LINE__, __FILE__) && bret) return;

#define CUT_ASSERT_INT_NOT_EQUAL(value,expected,bret) \
  if (!cut_int_equal((long)(value), (long)(expected), 0, __LINE__, __FILE__) && bret) return;

#define CUT_ASSERT_STR_EQUAL(value,expected,bret) \
  if (!cut_str_equal(value, expected, 1, __LINE__, __FILE__) && bret) return;

#define CUT_ASSERT_STR_NOT_EQUAL(value,expected,bret) \
  if (!cut_str_equal(value, expected, 0, __LINE__, __FILE__) && bret) return;

static int cut_nb_asserts = 0;
static int cut_nb_assert_failures = 0;
static axutil_env_t *cut_axis2_env = NULL;

static int cut_strcmp(const char *str1, const char *str2)
{
    if (str1 == NULL || str2 == NULL)
	{
	    return str1 == str2;
	}
	return strcmp(str1, str2);
}

static int cut_assert(int bValue, 
               unsigned int uiLine,
               char strCondition[], 
               char strFile[])
{
    ++cut_nb_asserts;
    if (bValue) return bValue;
    ++cut_nb_assert_failures;
    printf("Assertion Failure %s %d: %s\n", strFile, uiLine, strCondition);
    AXIS2_LOG_ERROR(cut_axis2_env->log, strFile, uiLine, "Assertion Failure : %s", strCondition);
	return bValue;
}

static int cut_ptr_equal(void *ptr, void *value, int equal,
               unsigned int uiLine,
               char strFile[])
{
    ++cut_nb_asserts;
    if ( (ptr == value) == equal) return 1;
    ++cut_nb_assert_failures;
    printf("Assertion Failure %s %d: pointer %sexpected <%s>, found <%s>\n", strFile, uiLine, 
	    equal?"":"not ", (char *) value, (char*) ptr);
    AXIS2_LOG_ERROR(cut_axis2_env->log, strFile, uiLine, 
	    "Assertion Failure : pointer %sexpected <%p>, found <%p>", equal?"":"not ", value, ptr);
	return 0;
}

static int cut_int_equal(long value, long expected, int equal,
               unsigned int uiLine,
               char strFile[])
{
    ++cut_nb_asserts;
    if ((expected == value)== equal) return 1;
    ++cut_nb_assert_failures;
    printf("Assertion Failure %s %d: integer %sexpected <%ld>, found <%ld>\n", strFile, uiLine, 
	    equal?"":"not ", expected, value);
    AXIS2_LOG_ERROR(cut_axis2_env->log, strFile, uiLine, 
	    "Assertion Failure : integer %sexpected <%ld>, found <%ld>", equal?"":"not ", expected,value);
	return 0;
}

static int cut_str_equal(const char *value, const char *expected, int equal,
               unsigned int uiLine,
               char strFile[])
{
    ++cut_nb_asserts;
    if ( (cut_strcmp(expected,value) == 0) == equal) return 1;
    ++cut_nb_assert_failures;
    printf("Assertion Failure %s %d: string %sexpected <%s>, found <%s>\n", strFile, uiLine, 
	    equal?"":"not ", expected, value);
    AXIS2_LOG_ERROR(cut_axis2_env->log, strFile, uiLine, 
	    "Assertion Failure : string %sexpected <%s>, found <%s>", equal?"":"not ", expected,value);
	return 0;
}

axutil_env_t *cut_setup_env(char* testName)
{
    cut_axis2_env = axutil_env_create_all("unit_tests.log", CUT_LOG_LEVEL);
    if (testName != NULL)
    {
        printf("\n");
        printf(CUT_LOG_HEADER, testName);
        printf("\n\n");
        if (cut_axis2_env) 
        {
            AXIS2_LOG_INFO(cut_axis2_env->log, CUT_LOG_HEADER, testName);
        }
    }
    return cut_axis2_env;
}

#ifdef __cplusplus
}
#endif

#endif                          /* CUT_DEFS_H */
