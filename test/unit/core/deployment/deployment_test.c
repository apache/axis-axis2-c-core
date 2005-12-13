#include "deployment_test.h"
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_env.h>

CuSuite* axis2_deploymentGetSuite() {
    CuSuite* suite = CuSuiteNew();
    /*SUITE_ADD_TEST(suite, Testaxis2_phases_info_get_op_in_phases );*/
    return suite;
}

