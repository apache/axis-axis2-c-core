#include "deployment_test.h"
#include "test_dep_engine.h"
#include <axis2_allocator.h>
#include <axis2_env.h>

CuSuite* axis2_deploymentGetSuite() 
{
    CuSuite* suite = CuSuiteNew();
	
	/**
	  * Test cases for axis2_phases_info
	  */
    /*SUITE_ADD_TEST(suite, Testaxis2_phases_info_get_op_in_phases );*/

	/**
	  * Test cases for axis2_dep_engine
	  */
    SUITE_ADD_TEST(suite, Testaxis2_dep_engine_free );
	SUITE_ADD_TEST(suite,Testaxis2_dep_engine_create);
	/*SUITE_ADD_TEST(suite,Testaxis2_dep_engine_create_with_repos_name);*/
	SUITE_ADD_TEST(suite,Testaxis2_dep_engine_load);

    return suite;
}
