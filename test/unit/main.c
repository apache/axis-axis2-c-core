#include <CuTest.h>
#include "core/phaseresolver/phaseresolver_test.h"
#include "core/description/description_test.h"
#include "core/engine/engine_test.h"
#include "wsdl/wsdl_test.h"

void RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, axis2_wsdlGetSuite());
    CuSuiteAddSuite(suite, axis2_engineGetSuite());
    
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
