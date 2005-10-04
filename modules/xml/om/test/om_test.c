#include <CuTest.h>
#include <string.h>
#include <stdio.h>
#include <axis2_om_node.h>

void Testnode_detach(CuTest *tc) {
	axis2_om_node_t* parent = axis2_om_node_create();
    axis2_om_node_t* prev_sibling = axis2_om_node_create();
    axis2_om_node_add_child(parent, prev_sibling);
    axis2_om_node_t* test_node = axis2_om_node_create();
    axis2_om_node_add_child(parent, test_node);
    axis2_om_node_t* next_sibling = axis2_om_node_create();
    axis2_om_node_add_child(parent, next_sibling);
        
    axis2_om_node_t* temp_parent = axis2_om_node_detach(test_node);
    puts("came");
    if(0 == temp_parent) puts("parent is null\n");
    axis2_om_node_t* expected = temp_parent->first_child;
    printf("came2");
    if(0 == expected) puts("expected is null\n");
        axis2_om_node_t* actual = next_sibling;

    CuAssertPtrEquals(tc, expected, actual); 
}

CuSuite* node_detachGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Testnode_detach);
    return suite;
}

 void RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, node_detachGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main(void) {
    RunAllTests();
    return 0;
}
