#include <stdio.h>
#include <axis2_op.h>
#include <axis2_module_desc.h>
#include <axis2_phases_info.h>
#include <stdio.h>
#include <axis2_op.h>
#include <axis2_dep_engine.h>
#include <axis2_env.h>
#include <axis2_allocator.h>

int axis2_test_dep_engine_load()
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_conf_t *conf = NULL;
    axis2_array_list_t *in_phases = NULL;
    axis2_char_t *axis2c_home = NULL;

	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dep_engine = axis2_dep_engine_create_with_repos_name(&env, 
        axis2c_home);
    if(!dep_engine)
    {
        printf("dep engine is not created \n");
        return -1;
    }
    conf = AXIS2_DEP_ENGINE_LOAD(dep_engine, &env);
    if (!conf)
    {
        printf("conf is NULL\n)");
        return -1;
    }
    in_phases = 
        AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(
            conf, &env);
    if(!in_phases)
    {
        printf("in phases up to and including post dispatch is NULL\n");
    }
    else
    {
        printf("dep engine load is successfull\n");
    }
    return 0;
}

int axis2_test_engine_conf_builder_populate_conf()
{
	axis2_status_t status = AXIS2_FAILURE;
    axis2_conf_t *conf = NULL;
    axis2_conf_builder_t *builder = NULL;
    axis2_char_t *conf_name = NULL;
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_char_t *axis2c_home = NULL;

    printf("testing conf_builder_populate_conf method \n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);
	conf = axis2_conf_create(&env);
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");

    dep_engine = axis2_dep_engine_create_with_repos_name(&env, 
        axis2c_home);

    conf_name = AXIS2_STRACAT (axis2c_home, "/axis2.xml", &env);
    builder = axis2_conf_builder_create_with_file_and_dep_engine_and_conf(&env,
            conf_name, dep_engine, conf);
    if(!builder)
    {
        printf("conf builder is not created \n");
        return -1;
    }
    status = AXIS2_CONF_BUILDER_POPULATE_CONF(builder, &env);   
    if(AXIS2_SUCCESS != status)
    {
        printf("Populate conf is not successfule. Test failed \n");
    }
    else
    {
        printf("conf is populated. Test is a success \n");
    }
    AXIS2_FREE(env->allocator, conf_name);
    return 0; 
}

int main()
{
    axis2_test_engine_conf_builder_populate_conf();
    axis2_test_dep_engine_load();
	return 0;
}
