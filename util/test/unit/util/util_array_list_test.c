#include <stdio.h>
#include <axis2_array_list.h>
#include <string.h>
#include "util_array_list_test.h"
#include "axis2_string.h"
typedef struct a
{
    char *value;
} a;

void Testaxis2_array_list_get(CuTest *tc)
{
    axis2_array_list_t *al;
    a *actual, *expected;
    a *actual2;

    printf("*************************************\n");
    printf("testing axis2_array_list_get  method \n");
    printf("*************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create(allocator);

    actual = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));
    actual2 = (a *) AXIS2_MALLOC(env->allocator, sizeof (a));


    actual->value = AXIS2_STRDUP("value1", &env);
    actual2->value = AXIS2_STRDUP("value2", &env);


    al = axis2_array_list_create (&env, 10);

    AXIS2_ARRAY_LIST_ADD (al, &env, (void*)actual);
    AXIS2_ARRAY_LIST_ADD (al, &env, (void*)actual);
    AXIS2_ARRAY_LIST_ADD (al, &env, (void*)actual);
    AXIS2_ARRAY_LIST_ADD (al, &env, (void*)actual);
    AXIS2_ARRAY_LIST_SET (al, &env, 3, (void*)actual2);
    AXIS2_ARRAY_LIST_REMOVE (al, &env, 2);

    expected = (a *) AXIS2_ARRAY_LIST_GET (al, &env, 2);
    CuAssertStrEquals(tc, expected->value, actual2->value);
}
