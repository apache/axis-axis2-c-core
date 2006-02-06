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

    printf("******************************************\n");
    printf("testing axis2_op_engage_module\n");
    printf("******************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);
	struct axis2_op *op = axis2_op_create(&env);

    moduleref = axis2_module_desc_create(&env);

    status = AXIS2_OP_ENGAGE_MODULE(op, &env, moduleref);

	if(status != AXIS2_SUCCESS )
	{
	    AXIS2_OP_FREE(op, &env);
	    AXIS2_MODULE_DESC_FREE(moduleref, &env);
		printf("ERROR %d\n", status);
	    /*return -1;*/
	}

	AXIS2_OP_FREE(op, &env);
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

    
	printf("******************************************\n");
    printf("testing axis2_svc_add_module_ops\n");
    printf("******************************************\n");

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

    
    printf("******************************************\n");
    printf("testing axis2_svc_engage_module\n");
    printf("******************************************\n");

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

int axis2_test_svc_get_op()
{
	struct axis2_svc *svc = NULL;
	struct axis2_qname *qname = NULL;
	struct axis2_hash_t *ops = NULL;
	struct axis2_op *op = NULL;
	axis2_status_t status = NULL;


    printf("******************************************\n");
    printf("testing axis2_svc_get_op\n");
    printf("******************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);
	qname = axis2_qname_create(&env, "op1", NULL, NULL);	
	op = axis2_op_create_with_qname(&env, qname);	
	qname = axis2_qname_create(&env, "svc1", NULL, NULL);	
	svc = axis2_svc_create_with_qname(&env, qname);

	status = AXIS2_SVC_ADD_OP(svc, &env, op);
	
    qname = axis2_qname_create(&env, "op2", NULL, NULL);	
	op = axis2_op_create_with_qname(&env, qname);	
	status = AXIS2_SVC_ADD_OP(svc, &env, op);
	
    ops = AXIS2_SVC_GET_OPS(svc, &env);

	if (ops)
		printf("SUCCESS AXIS2_SVC_GET_OPS\n");
	else
	{
		printf("ERROR AXIS2_SVC_GET_OPS\n");
		return -1;
	}

             if(ops)
             {
                printf("ops count = %d\n", axis2_hash_count(ops));

                axis2_hash_index_t *hi2 = NULL;
                void *op2 = NULL;
                axis2_char_t *oname = NULL;
                int count = 0;

                for(hi2 = axis2_hash_first(ops, &env); hi2; hi2 = axis2_hash_next(&env, hi2))
                {
                    printf ("count = %d \n", count++);
                    AXIS2_SVC_GET_OPS(svc, &env);
                    if (!(hi2))
                        break;
                    axis2_hash_this(hi2, NULL, NULL, &op2);
                    if (op2)
                    {
                        axis2_qname_t *qname = NULL;
                        qname = AXIS2_OP_GET_QNAME((axis2_op_t *)op2, &env);
					    oname = AXIS2_QNAME_GET_LOCALPART(qname, &env);
                        printf("op name = %s\n", oname);
                    }
                }    
             }
             else
                printf("ops count = zero\n");
	

	return 0;
}

int main()
{
		axis2_test_op_engage_module();
		axis2_test_svc_add_module_ops();
		axis2_test_svc_engage_module();
		axis2_test_svc_get_op();
		return 0;
}
