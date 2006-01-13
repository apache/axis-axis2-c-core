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

	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);

    dep_engine = axis2_dep_engine_create_with_repos_name(&env, 
        ".");
    if(!dep_engine)
    {
        printf("dep engine is not created \n");
        return -1;
    }
    printf("came1\n");
    conf = AXIS2_DEP_ENGINE_LOAD(dep_engine, &env);
    printf("came2\n");
    in_phases = 
        AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(
            conf, &env);
    if(!in_phases)
    {
        printf("in phases up to and including post dispatch is NULL\n");
    }
    return 0;
}

int main()
{
		axis2_test_dep_engine_load();
		return 0;
}
