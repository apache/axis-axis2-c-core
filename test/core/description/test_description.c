#include <stdio.h>
#include <axis2_op.h>
#include <axis2_module_desc.h>
#include <axis2_phases_info.h>
#include <axis2_env.h>

struct axis2_module_desc *create_module_desc(axis2_env_t **env);

int axis2_test_op_engage_module()
{
	axis2_status_t status = AXIS2_FAILURE;
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);
	struct axis2_op *op = axis2_op_create(&env);
    struct axis2_module_desc *moduleref= NULL;

    moduleref = axis2_module_desc_create(&env);

    status = AXIS2_OP_ENGAGE_MODULE(op, &env, moduleref);

	if(status != AXIS2_SUCCESS )
	{
		printf("ERROR %d\n", status);
	    /*return -1;*/
	}

	AXIS2_OP_FREE(op, &env);
	axis2_env_free(env);
    return 0;
}

int main()
{
		axis2_test_op_engage_module();
		return 0;
}
