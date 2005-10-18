#include "description_test.h"
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_environment.h>

CuSuite* axis2_descriptionGetSuite() {
    CuSuite* suite = CuSuiteNew();
    /*SUITE_ADD_TEST(suite, Testaxis2_stream_ops_read);*/
    return suite;
}

