#include "util_stream_test.h"

void Testaxis2_stream_read(CuTest *tc) {

    char actual[10];
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_env_t *env = axis2_environment_create(allocator, NULL, NULL, NULL);
    char *expected = (char*) strdup("aaaaaaaaa");
    AXIS2_STREAM_READ(env->stream, actual, 10);
    CuAssertStrEquals(tc, expected, actual);
}

void Testaxis2_stream_write(CuTest *tc) {

    char actual[10];
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_env_t *env = axis2_environment_create(allocator, NULL, NULL, NULL);
    char *expected = (char*) strdup("aaaaaaaaa");
    AXIS2_STREAM_READ(env->stream, actual, 10);
    AXIS2_STREAM_WRITE(env->stream, actual, 10);
    
    CuAssertStrEquals(tc, expected, actual);
}
