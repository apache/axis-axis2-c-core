#include "om_test.h"
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <om_node_test.h>

CuSuite* axis2_omGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Testaxis2_om_node_add_child);
    SUITE_ADD_TEST(suite, Testaxis2_om_node_set_parent);
    SUITE_ADD_TEST(suite,Testaxis2_om_node_insert_sibling_after);
    SUITE_ADD_TEST(suite,Testaxis2_om_node_insert_sibling_before);
    SUITE_ADD_TEST(suite,Testaxis2_om_node_detach);

    return suite;
}
