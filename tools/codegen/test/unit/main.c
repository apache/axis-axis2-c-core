#include <CuTest.h>
#include "util/w2c_util_test.h"
#include "util/w2c_conf_test.h"
#include <stdio.h>

void RunAllTests(void)
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, w2c_utilGetSuite());
    CuSuiteAddSuite(suite, w2c_confGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main(void)
{
    RunAllTests();
    return 0;
}
