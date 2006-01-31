
#include "wsdl_test.h"
CuSuite* axis2_wsdlGetSuite()
{
	CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite,Testaxis2_wsdl_component_set_component_properties); 
	return suite;
}

