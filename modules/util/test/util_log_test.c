#include "util_log_test.h"

void Testaxis2_log_write(CuTest *tc) 
{

    char actual[10];
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_env_t *env = axis2_environment_create(allocator);
    char *expected = strdup("aaaaaaaaa");
    AXIS2_STREAM_READ(env->stream, actual, 10);
    AXIS2_LOG_WRITE(env->log, actual, 10);
    CuAssertStrEquals(tc, expected, actual);
}
