#include <CuTest.h>
#include <stdio.h>
#include "w2c_util_test.h"
#include "w2c_conf_test.h"

void RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, (CuSuite*)w2c_utilGetSuite());
    CuSuiteAddSuite(suite, (CuSuite*)w2c_confGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main(void) {
    RunAllTests();
    return 0;
}
