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
