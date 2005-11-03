#include "util_log_test.h"

void Testaxis2_log_write(CuTest *tc) 
{

    char actual[10];
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_stream_t *stream = axis2_stream_create(allocator, NULL);
    axis2_error_t *error = axis2_error_create(allocator);
    axis2_log_t *log  = axis2_log_create (allocator, NULL);
    axis2_env_t *env = axis2_environment_create_with_error_stream_log(allocator, error, stream, log);

    char *expected = strdup("aaaaaaaaa");
    AXIS2_STREAM_READ(env->stream, actual, 10);
    AXIS2_LOG_WRITE(env->log, actual, 10);
    CuAssertStrEquals(tc, expected, actual);
}
