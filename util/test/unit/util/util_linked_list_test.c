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

#include <axis2_linked_list.h>
#include <axis2_linked_list.h>
#include <axis2_env.h>
#include "util_linked_list_test.h"
#include "axis2_string.h"
#include <stdio.h>

typedef struct a
{
    char *value;
} a;

void Testaxis2_linked_list_get(CuTest *tc)
{
    axis2_linked_list_t *al;
    a *actual, *expected;
    a *actual2;
    
    printf("**************************************\n");
    printf("testing axis2_linked_list_get  method \n");
    printf("**************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create(allocator);

    actual = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    actual2 = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));

    actual->value = AXIS2_STRDUP("value1", env);
    actual2->value = AXIS2_STRDUP("value2", env);

    al = axis2_linked_list_create (env);

    AXIS2_LINKED_LIST_ADD (al, env, (void*)actual);
    AXIS2_LINKED_LIST_ADD (al, env, (void*)actual);
    AXIS2_LINKED_LIST_ADD (al, env, (void*)actual);
    AXIS2_LINKED_LIST_ADD (al, env, (void*)actual);
    AXIS2_LINKED_LIST_SET (al, env, 3, (void*)actual2);
    AXIS2_LINKED_LIST_REMOVE_AT_INDEX (al, env, 2);

    expected = (a *) AXIS2_LINKED_LIST_GET (al, env, 2);
    CuAssertStrEquals(tc, expected->value, actual2->value);
}
