#include <stdio.h>
#include <axis2_hash.h>
#include "util_hash_test.h"
#include "axis2_string.h"
#include <stdlib.h>
#include <string.h>

typedef struct a
{
    char *value;
} a;

void Testaxis2_hash_get(CuTest *tc)
{
    printf("testing axis2_hash_get\n"); 
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *environment =
        axis2_env_create (allocator);

    axis2_hash_t *ht;
    a *actual, *expected;

    axis2_hash_index_t *i = 0;
    void *v = NULL;

    const char *key1 = "key1";

    actual = (a *) AXIS2_MALLOC(environment->allocator, sizeof (a));
    actual->value = AXIS2_STRDUP("value1", &environment);

    ht = axis2_hash_make (&environment);

    axis2_hash_set (ht, key1, AXIS2_HASH_KEY_STRING, actual);

    for (i = axis2_hash_first (ht, &environment); i; i = axis2_hash_next (&environment, i))
    {
        axis2_hash_this (i, NULL, NULL, &v);
    }

    expected = (a *) axis2_hash_get (ht, key1, AXIS2_HASH_KEY_STRING);
    CuAssertStrEquals(tc, expected->value, actual->value);
}

void Testaxis2_hash_while(CuTest *tc)
{
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *environment =
        axis2_env_create (allocator);

    axis2_hash_t *ht;
    a *actual = NULL;
    a *expected = NULL;
    a *entry1 = NULL;
    a *entry2 = NULL;
    a *entry3 = NULL;

    axis2_hash_index_t *index_i= 0;
    void *v = NULL;
    char *k = NULL;

    char *key = NULL;
    char *key1 = "key1";
    char *key2 = "key2";
    char *key3 = "key3";

    entry1 = (a *) AXIS2_MALLOC(environment->allocator, sizeof (a));
    entry2 = (a *) AXIS2_MALLOC(environment->allocator, sizeof (a));
    entry3 = (a *) AXIS2_MALLOC(environment->allocator, sizeof (a));
    
    expected = (a *) AXIS2_MALLOC(environment->allocator, sizeof (a));
    expected->value = AXIS2_STRDUP("value2", &environment);

    entry1->value = AXIS2_STRDUP("value1", &environment);

    ht = axis2_hash_make (&environment);

    axis2_hash_set (ht, key1, AXIS2_HASH_KEY_STRING, entry1);
    
    entry2->value = AXIS2_STRDUP("value2", &environment);
    
    axis2_hash_set (ht, key2, AXIS2_HASH_KEY_STRING, entry2);
    
    entry3->value = AXIS2_STRDUP("value3", &environment);
    
    axis2_hash_set (ht, key3, AXIS2_HASH_KEY_STRING, entry3);

    index_i = axis2_hash_first (ht, &environment);
    do 
    {
        axis2_hash_this (index_i, &k, NULL, &v);
        
        key = (char *)k;
        
        if(0 == AXIS2_STRCMP(key, "key2"))
        {
            actual = (a *) v;
            break;
        }
        
        index_i = axis2_hash_next (&environment, index_i);
    
    }while(NULL != index_i);
    
    printf("actual:%s\n",  actual->value);
    CuAssertStrEquals(tc, expected->value, actual->value);
    free(expected->value);
    free(expected);
    
    free(entry1->value);
    free(entry1);
    free(entry3->value);
    free(entry3);
    free(entry2->value);
    free(entry2);
    axis2_hash_free(ht, &environment);
}
