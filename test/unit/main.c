#include <CuTest.h>
#include "util/util_test.h"
#include "xml/om/om_test.h"
#include "core/phaseresolver/phaseresolver_test.h"
#include "core/description/description_test.h"

void RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, axis2_utilGetSuite());
    CuSuiteAddSuite(suite, axis2_omGetSuite());
    CuSuiteAddSuite(suite, axis2_phaseresolverGetSuite());
    CuSuiteAddSuite(suite, axis2_descriptionGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main(void) {
    RunAllTests();
    return 0;
}
