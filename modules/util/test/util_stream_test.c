#include "util_stream_test.h"

void Testaxis2_stream_read(CuTest *tc) {

    char actual[10];
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_stream_t *stream = axis2_stream_create(allocator, NULL);
    axis2_error_t *error = axis2_error_create(allocator);
    axis2_env_t *env = axis2_environment_create_with_error_stream(allocator, error, stream);
    char *expected = (char*) strdup("aaaaaaaaa");
    AXIS2_STREAM_READ(env->stream, actual, 10);
    CuAssertStrEquals(tc, expected, actual);
}

void Testaxis2_stream_write(CuTest *tc) {

    char actual[10];
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_stream_t *stream = axis2_stream_create(allocator, NULL);
    axis2_error_t *error = axis2_error_create(allocator);
    axis2_env_t *env = axis2_environment_create_with_error_stream(allocator, error, stream);
    char *expected = (char*) strdup("aaaaaaaaa");
    AXIS2_STREAM_READ(env->stream, actual, 10);
    AXIS2_STREAM_WRITE(env->stream, actual, 10);
    
    CuAssertStrEquals(tc, expected, actual);
}
