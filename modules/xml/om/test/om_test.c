#include <CuTest.h>
#include <string.h>
#include <stdio.h>
#include <axis2c_node.h>

void Testdetach_node(CuTest *tc) {
	node_t* parent = create_node();
        node_t* prev_sibling = create_node();
        node_add_child(parent, prev_sibling);
        node_t* test_node = create_node();
        node_add_child(parent, test_node);
        node_t* next_sibling = create_node();
        node_add_child(parent, next_sibling);
        
        node_t* temp_parent = detach_node(test_node);
        puts("came");
        if(0 == temp_parent) puts("parent is null\n");
        node_t* expected = temp_parent->first_child;
        printf("came2");
        if(0 == expected) puts("expected is null\n");
        node_t* actual = next_sibling;

        CuAssertPtrEquals(tc, expected, actual); 
    }

    CuSuite* detach_nodeGetSuite() {
        CuSuite* suite = CuSuiteNew();
        SUITE_ADD_TEST(suite, Testdetach_node);
        return suite;
    }

 void RunAllTests(void) {
        CuString *output = CuStringNew();
        CuSuite* suite = CuSuiteNew();

        CuSuiteAddSuite(suite, detach_nodeGetSuite());

        CuSuiteRun(suite);
        CuSuiteSummary(suite, output);
        CuSuiteDetails(suite, output);
        printf("%s\n", output->buffer);
    }

    int main(void) {
        RunAllTests();
        return 0;
    }
