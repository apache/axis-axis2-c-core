#include <stdio.h>
#include <axis2_op.h>
#include <axis2_module_desc.h>
#include <axis2_phases_info.h>
#include <axis2_env.h>
#include <axis2_allocator.h>

struct axis2_module_desc *create_module_desc(axis2_env_t **env);

int axis2_test_op_engage_module()
{
    struct axis2_module_desc *moduleref= NULL;

	axis2_status_t status = AXIS2_FAILURE;
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);

	struct axis2_op *op = axis2_op_create(&env);
    moduleref = axis2_module_desc_create(&env);

    status = AXIS2_OP_ENGAGE_MODULE(op, &env, moduleref);

	if(status != AXIS2_SUCCESS )
	{
		printf("ERROR %d\n", status);
	    /*return -1;*/
	}

	AXIS2_OP_FREE(op, &env);
	AXIS2_MODULE_DESC_FREE(moduleref, &env);
	axis2_env_free(env);
    return 0;
}
int axis2_test_svc_add_module_ops()
{
	struct axis2_svc *svc = NULL;
	struct axis2_qname *qname = NULL;
    struct axis2_module_desc *module_desc = NULL;
	struct axis2_conf *axis2_config = NULL;

	axis2_status_t status = AXIS2_FAILURE;

	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);
	
	qname = axis2_qname_create(&env, "name1", NULL, NULL);
	svc = axis2_svc_create_with_qname(&env, qname);
	module_desc = axis2_module_desc_create(&env);
	axis2_config = axis2_conf_create(&env);
	status = AXIS2_SVC_ADD_MODULE_OPS(svc, &env, module_desc, axis2_config);
	if(status != AXIS2_SUCCESS)
	{
	    printf("axis2_test_description_add_module_ops ERROR %d\n", status);
	}
	else
	    printf("axis2_test_add_module_ops SUCCESS\n");

	AXIS2_SVC_FREE(svc, &env);
	AXIS2_QNAME_FREE(qname, &env);
	AXIS2_MODULE_DESC_FREE(module_desc, &env);
	AXIS2_CONF_FREE(axis2_config, &env);
	
	return 0;
}

int axis2_test_svc_engage_module()
{
	struct axis2_svc *svc = NULL;
	struct axis2_qname *qname = NULL;
	struct axis2_module_desc *moduleref = NULL;
	struct axis2_conf *axis2_config = NULL;
	
	axis2_status_t status = AXIS2_FAILURE;

	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);

	qname = axis2_qname_create(&env, "name1", NULL, NULL);
	svc = axis2_svc_create_with_qname(&env, qname);
	moduleref = axis2_module_desc_create(&env);
	axis2_config = axis2_conf_create(&env);
	
	status = AXIS2_SVC_ENGAGE_MODULE(svc, &env, moduleref, axis2_config); 

	if(status != AXIS2_SUCCESS)
	{
	    printf("axis2_test_description_svc_engage_module ERROR %d\n", status);
	}
	else
	    printf("axis2_test_svc_engage_module SUCCESS\n");

	AXIS2_SVC_FREE(svc, &env);
	AXIS2_QNAME_FREE(qname, &env);
	AXIS2_MODULE_DESC_FREE(moduleref, &env);
	AXIS2_CONF_FREE(axis2_config, &env);

	return 0;
}

int main()
{
		axis2_test_op_engage_module();
		axis2_test_svc_add_module_ops();
		axis2_test_svc_engage_module();
		return 0;
}
