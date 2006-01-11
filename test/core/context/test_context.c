#include <axis2_conf_ctx.h>
#include <axis2_svc_grp.h>
#include <axis2.h>
#include <axis2_allocator.h>
#include <axis2_env.h>

void axis2_test_conf_ctx_init()
{
	struct axis2_conf *conf = NULL;
    struct axis2_svc_grp_ctx *svc_grp_ctx;
	struct axis2_svc_grp *svc_grp;
	struct axis2_conf_ctx *conf_ctx = NULL;
	struct axis2_svc_ctx *svc_ctx;
	struct axis2_svc *svc;
	struct axis2_qname *qname;
	struct axis2_op_ctx *op_ctx;
	struct axis2_op *op;
	struct axis2_hash_t *op_ctx_map;

	axis2_status_t status = AXIS2_FAILURE;
	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create (allocator);

	conf = axis2_conf_create(&env);

/*	axis2_hash_t *svc_ctx_map;
	axis2_hash_t *svc_grp_ctx_map;*/
	
	op = axis2_op_create(&env);

	conf_ctx = axis2_conf_ctx_create(&env, conf);

	svc_grp = axis2_svc_grp_create(&env);
	svc_grp_ctx = axis2_svc_grp_ctx_create(&env, svc_grp, conf_ctx);

	qname = axis2_qname_create(&env, "name1", NULL, NULL);
    svc = axis2_svc_create_with_qname(&env, qname);
    svc_ctx = axis2_svc_ctx_create(&env, svc, svc_grp_ctx);

	op = axis2_op_create(&env);
	op_ctx = axis2_op_ctx_create(&env, op, svc_ctx);
	op_ctx_map = AXIS2_CONF_CTX_GET_OP_CTX_MAP(conf_ctx, &env);
	char *key = "key";

	axis2_hash_set (op_ctx_map, key, ")	
	/*op_ctx_map  = AXIS2_CONF_CTX_GET_OP_CTX_MAP(conf_ctx, &env);
	svc_ctx_map = AXIS2_CONF_CTX_GET_SVC_CTX_MAP(conf_ctx, &env);
	svc_grp_ctx_map = AXIS2_CONF_CTX_GET_SVC_GRP_CTX_MAP(conf_ctx, &env);*/

	status = AXIS2_CONF_CTX_INIT(conf_ctx, &env, conf);

	if (status != AXIS2_SUCCESS)
	{
			printf("ERROR %d\n", status);
	}
    else
        printf("SUCCESS\n");

    AXIS2_CONF_CTX_FREE(conf_ctx, &env);
    AXIS2_CONF_FREE(conf, &env);
}

int main()
{
	axis2_test_conf_ctx_init();
	return 0;
}
