#include "util_log_test.h"

void Testaxis2_log_ops_write(CuTest *tc) 
{

    char actual[10];
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_env_t *env = axis2_environment_create(allocator, NULL, NULL, NULL, NULL);
    char *expected = strdup("aaaaaaaaa");
    axis2_stream_read(env->stream, actual, 10);
    axis2_log_write(env->log, actual, 10);
    CuAssertStrEquals(tc, expected, actual);
}
