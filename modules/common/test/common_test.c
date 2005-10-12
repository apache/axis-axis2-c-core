#include <CuTest.h>
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_environment.h>

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

void Testaxis2_log_ops_write(CuTest *tc) {

    char actual[10];
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_environment_t *env = axis2_environment_create(allocator, NULL, NULL, NULL);
    axis2_stream_read(env->stream, actual, 10);
    axis2_log_write(env->log, actual, 10);
    char *expected = strdup("aaaaaaaaa");
    CuAssertStrEquals(tc, expected, actual);
}

CuSuite* axis2_commonGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Testaxis2_stream_ops_read);
    SUITE_ADD_TEST(suite, Testaxis2_stream_ops_write);
    SUITE_ADD_TEST(suite, Testaxis2_log_ops_write);
    return suite;
}

void RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, axis2_commonGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main(void) {
    RunAllTests();
    return 0;
}
