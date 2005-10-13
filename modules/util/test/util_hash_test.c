#include "util_hash_test.h"

typedef struct a
{
    char *value;
} a;

void Testaxis2_hash_ops_get(CuTest *tc)
{    
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_environment_t *environment =
        axis2_environment_create (allocator, NULL, NULL, NULL);

    axis2_hash_t *ht;
    a *actual, *expected;

    axis2_hash_index_t *i = 0;
    void *v = NULL;

    char *key1 = "key1";

    actual = (a *) axis2_malloc(environment->allocator, sizeof (a));


    actual->value = axis2_strdup(environment->allocator, "value1");


    ht = axis2_hash_make (environment);

    axis2_hash_set (ht, key1, AXIS2_HASH_KEY_STRING, actual);

    for (i = axis2_hash_first (environment, ht); i; i = axis2_hash_next (i))
    {

        axis2_hash_this (i, NULL, NULL, &v);

    }

    expected = (a *) axis2_hash_get (ht, key1, AXIS2_HASH_KEY_STRING);
    CuAssertStrEquals(tc, expected->value, actual->value);
}

