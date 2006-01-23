#include "engine_test.h"
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include "test_conf.h"

CuSuite* axis2_engineGetSuite() 
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Testaxis2_conf_set_default_dispatchers);
    return suite;
}

