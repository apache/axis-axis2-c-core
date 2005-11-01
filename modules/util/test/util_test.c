#include "util_test.h"
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_environment.h>

CuSuite* axis2_utilGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Testaxis2_stream_read);
    SUITE_ADD_TEST(suite, Testaxis2_stream_write);
    SUITE_ADD_TEST(suite, Testaxis2_log_write);
    SUITE_ADD_TEST(suite, Testaxis2_hash_get);
    return suite;
}

