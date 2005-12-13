#include "phaseresolver_test.h"
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_env.h>

CuSuite* axis2_phaseresolverGetSuite() 
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Testaxis2_phase_resolver_engage_module_to_op);
    return suite;
}

