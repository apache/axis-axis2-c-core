#include "util_string_test.h"
#include <axis2_env.h>
#include <axis2_error_default.h>
#include <axis2_log_default.h>
#include <stdlib.h>

void Testaxis2_strndup(CuTest *tc) 
{
    axis2_char_t *actual = NULL;
    int n = 3;
    axis2_char_t *expected = "dam";
    
    printf("******************************************\n");
    printf("testing axis2_strndup\n");
    printf("******************************************\n");
    
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_error_t *error = axis2_error_create(allocator);
    const axis2_env_t *env = axis2_env_create_with_error(allocator, error);

    actual = axis2_strndup("damitha", n, env);
    printf("actual:%s\n", actual);

    CuAssertStrEquals(tc, expected, actual);
}
