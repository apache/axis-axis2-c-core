#include "util_stream_test.h"

void Testaxis2_stream_ops_read(CuTest *tc) {

    char actual[10];
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_environment_t *env = axis2_environment_create(allocator, NULL, NULL, NULL);
    axis2_stream_read(env->stream, actual, 10);
    char *expected = strdup("aaaaaaaaa");
    CuAssertStrEquals(tc, expected, actual);
}

void Testaxis2_stream_ops_write(CuTest *tc) {

    char actual[10];
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_environment_t *env = axis2_environment_create(allocator, NULL, NULL, NULL);
    axis2_stream_read(env->stream, actual, 10);
    axis2_stream_write(env->stream, actual, 10);
    char *expected = strdup("aaaaaaaaa");
    CuAssertStrEquals(tc, expected, actual);
}
