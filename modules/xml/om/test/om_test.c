#include <strutil.h>
#include <CuTest.h>
#include <string.h>
#include <stdio.h>
#include <node.h>

void Testnode_detach(CuTest *tc) {
	node_t test_node = malloc(sizeof(node_t));
        CuAssertStrEquals(tc, expected, actual);
    }

    CuSuite* StrUtilGetSuite() {
        CuSuite* suite = CuSuiteNew();
        SUITE_ADD_TEST(suite, TestStrToUpper);
        return suite;
    }

 void RunAllTests(void) {
        CuString *output = CuStringNew();
        CuSuite* suite = CuSuiteNew();

        CuSuiteAddSuite(suite, StrUtilGetSuite());

        CuSuiteRun(suite);
        CuSuiteSummary(suite, output);
        CuSuiteDetails(suite, output);
        printf("%s\n", output->buffer);
    }

    int main(void) {
        RunAllTests();
        return 0;
    }
