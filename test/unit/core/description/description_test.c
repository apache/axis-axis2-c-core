#include "description_test.h"
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_env.h>

CuSuite* axis2_descriptionGetSuite() 
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Testaxis2_handler_desc_free); 
    SUITE_ADD_TEST(suite, Testaxis2_flow_add_handler); 
    SUITE_ADD_TEST(suite, Testaxis2_flow_free);
    SUITE_ADD_TEST(suite, Testaxis2_op_set_remaining_phases_inflow);
    SUITE_ADD_TEST(suite, Testaxis2_svc_add_module_ops);
    SUITE_ADD_TEST(suite, Testaxis2_svc_create_with_qname);
    return suite;
}

