#include <CuTest.h>
#include <stdio.h>
#include "w2c_engine_test.h"
#include "w2c_writer_test.h"

void RunAllTests(void)
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, (CuSuite*)w2c_engineGetSuite());
    CuSuiteAddSuite(suite, (CuSuite*)w2c_writerGetSuite());

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
