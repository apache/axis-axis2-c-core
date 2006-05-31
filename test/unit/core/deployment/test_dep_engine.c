#include "test_dep_engine.h"
#include <axis2_env.h>
#include <stdio.h>
#include <string.h>
#include <axis2_dep_engine.h>

void Testaxis2_dep_engine_free(CuTest *tc)
{
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_status_t actual = AXIS2_FAILURE;
    axis2_status_t expected = AXIS2_SUCCESS;
    axis2_char_t *axis2c_home = NULL;
	
    printf("**************************************\n");
    printf("testing axis2_dep_engine_free  method \n");
    printf("**************************************\n");
    
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create (allocator);
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
	
    dep_engine = axis2_dep_engine_create_with_repos_name(env, axis2c_home
        );
    if(!dep_engine)
    {
        printf("dep engine is not created \n");
        return;
    }
    actual = AXIS2_DEP_ENGINE_FREE(dep_engine, env);
    CuAssertIntEquals(tc, expected, actual);
	axis2_env_free(env);
}


void Testaxis2_dep_engine_create(CuTest *tc)
{
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_dep_engine_t *dep_engine = NULL;

    printf("****************************************\n");
    printf("testing axis2_dep_engine_create  method \n");
    printf("****************************************\n");

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    dep_engine = axis2_dep_engine_create(env);

	CuAssertPtrNotNull(tc,dep_engine);

    if (dep_engine)
        AXIS2_DEP_ENGINE_FREE(dep_engine, env);
    axis2_env_free(env);
}

void Testaxis2_dep_engine_create_with_repos_name(CuTest *tc)
{
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_char_t *axis2c_home = NULL;

    printf("********************************************************\n");
    printf("testing axis2_dep_engine_create_with_repos_name  method \n");
    printf("********************************************************\n");

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
	
    dep_engine = axis2_dep_engine_create_with_repos_name(env,axis2c_home);

	CuAssertPtrNotNull(tc, dep_engine);

    if (dep_engine)
        AXIS2_DEP_ENGINE_FREE(dep_engine, env);

    axis2_env_free(env);
}


void Testaxis2_dep_engine_load(CuTest *tc)
{

    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_conf_t *conf_actual = NULL;
    axis2_char_t *axis2c_home = NULL;
    axis2_dep_engine_t *dep_engine = NULL;

    printf("**************************************\n");
    printf("testing axis2_dep_engine_load  method \n");
    printf("**************************************\n");

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");

    dep_engine = axis2_dep_engine_create_with_repos_name(env, axis2c_home);
    if (dep_engine)
        conf_actual = AXIS2_DEP_ENGINE_LOAD(dep_engine, env);

	CuAssertPtrNotNull(tc, conf_actual);

    if (conf_actual)
        AXIS2_DEP_ENGINE_FREE(dep_engine, env);
    
    axis2_env_free(env);
}

