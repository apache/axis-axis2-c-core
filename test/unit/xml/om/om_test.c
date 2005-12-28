#include "om_test.h"
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <om_node_test.h>

CuSuite* axis2_omGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Testaxis2_om_node_add_child);
    printf("came1\n");
    SUITE_ADD_TEST(suite, Testaxis2_om_node_set_parent);
    printf("came1\n");
    SUITE_ADD_TEST(suite,Testaxis2_om_node_insert_sibling_after);
    printf("came1\n");
    SUITE_ADD_TEST(suite,Testaxis2_om_node_insert_sibling_before);
    printf("came1\n");
    SUITE_ADD_TEST(suite,Testaxis2_om_node_detach);
    return suite;
}
