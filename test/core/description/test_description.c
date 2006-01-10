#include <stdio.h>
#include <axis2_op.h>
#include <axis2_module_desc.h>
#include <axis2_phases_info.h>
#include <axis2_env.h>

struct axis2_module_desc *create_module_desc(axis2_env_t **env);

int test_op_engage_module()
{
	int size = 0;
	axis2_status_t *status = NULL;
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);
	struct axis2_op *op = axis2_op_create(&env);
    struct axis2_module_desc *moduleref= NULL;

    moduleref = axis2_module_desc_create(&env);

    status = axis2_op_engage_module(op, &env, moduleref);

	/*printf("%d\n", AXIS2_ERROR_GET_STATUS_CODE((env)->error));*/

	if(status != AXIS2_SUCCESS )
	{
		printf("ERROR %d\n", status);
	    return -1;
	}

	axis2_op_free (op, &env);
	axis2_env_free(env);
}

int main()
{
		test_op_engage_module();
		return 0;
}
