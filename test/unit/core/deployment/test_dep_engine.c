#include "test_dep_engine.h"
#include <axis2_dep_engine.h>

void Testaxis2_dep_engine_free(CuTest *tc)
{
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_status_t actual = AXIS2_FAILURE;
    axis2_status_t expected = AXIS2_SUCCESS;
    
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create (allocator);
    
    dep_engine = axis2_dep_engine_create_with_repos_name(&env, 
        "/home/damitha/programs/axis2c");
    if(!dep_engine)
    {
        printf("dep engine is not created \n");
        return -1;
    }
    actual = AXIS2_DEP_ENGINE_FREE(dep_engine, &env);
    CuAssertIntEquals(tc, expected, actual);
}

