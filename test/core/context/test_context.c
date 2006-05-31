#include <axis2_conf_ctx.h>
#include <axis2_svc_grp.h>
#include <axis2_const.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <stdio.h>

void axis2_test_conf_ctx_init()
{
	struct axis2_conf *conf = NULL;
    struct axis2_svc_grp_ctx *svc_grp_ctx1 = NULL;
    struct axis2_svc_grp_ctx *svc_grp_ctx2 = NULL;
	struct axis2_svc_grp *svc_grp1 = NULL;
	struct axis2_svc_grp *svc_grp2 = NULL;
	struct axis2_conf_ctx *conf_ctx = NULL;
	struct axis2_svc_ctx *svc_ctx1 = NULL;
	struct axis2_svc_ctx *svc_ctx2 = NULL;
	struct axis2_svc *svc1 = NULL;
	struct axis2_svc *svc2 = NULL;
	struct axis2_qname *qname1 = NULL;
	struct axis2_qname *qname2 = NULL;
	struct axis2_op_ctx *op_ctx1 = NULL;
	struct axis2_op_ctx *op_ctx2 = NULL;
	struct axis2_op *op = NULL;
	struct axis2_hash_t *op_ctx_map = NULL;
	struct axis2_hash_t *svc_ctx_map = NULL;
	struct axis2_hash_t *svc_grp_ctx_map = NULL;

	axis2_status_t status = AXIS2_FAILURE;
	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create (allocator);

	conf = axis2_conf_create(env);
	
	op = axis2_op_create(env);

	conf_ctx = axis2_conf_ctx_create(env, conf);

	svc_grp1 = axis2_svc_grp_create(env);
	svc_grp2 = axis2_svc_grp_create(env);

	svc_grp_ctx1 = axis2_svc_grp_ctx_create(env, svc_grp1, conf_ctx);
	svc_grp_ctx2 = axis2_svc_grp_ctx_create(env, svc_grp2, conf_ctx);

	qname1 = axis2_qname_create(env, "name1", NULL, NULL);
	qname2 = axis2_qname_create(env, "name2", NULL, NULL);

    svc1 = axis2_svc_create_with_qname(env, qname1);
    svc2 = axis2_svc_create_with_qname(env, qname2);

    svc_ctx1 = axis2_svc_ctx_create(env, svc1, svc_grp_ctx1);
    svc_ctx2 = axis2_svc_ctx_create(env, svc2, svc_grp_ctx2);

	op = axis2_op_create(env);
	op_ctx1 = axis2_op_ctx_create(env, op, svc_ctx1);
	op_ctx2 = axis2_op_ctx_create(env, op, svc_ctx2);

	op_ctx_map = AXIS2_CONF_CTX_GET_OP_CTX_MAP(conf_ctx, env);

    if (op_ctx_map)
    {
        axis2_hash_set(op_ctx_map, "op_ctx1", AXIS2_HASH_KEY_STRING, op_ctx1);
        axis2_hash_set(op_ctx_map, "op_ctx2", AXIS2_HASH_KEY_STRING, op_ctx2);
    }

	svc_ctx_map = AXIS2_CONF_CTX_GET_SVC_CTX_MAP(conf_ctx, env);

	if (svc_ctx_map)
	{
		axis2_hash_set(svc_ctx_map, "svc_ctx1", AXIS2_HASH_KEY_STRING, svc_ctx1);
		axis2_hash_set(svc_ctx_map, "svc_ctx2", AXIS2_HASH_KEY_STRING, svc_ctx2);
	}

	svc_grp_ctx_map = AXIS2_CONF_CTX_GET_SVC_GRP_CTX_MAP(conf_ctx, env);

	if (svc_grp_ctx_map)
	{
		axis2_hash_set(svc_ctx_map, "svc_grp_ctx1", AXIS2_HASH_KEY_STRING, svc_grp_ctx1);
		axis2_hash_set(svc_ctx_map, "svc_grp_ctx2", AXIS2_HASH_KEY_STRING, svc_grp_ctx2);
	}

	status = AXIS2_CONF_CTX_INIT(conf_ctx, env, conf);

	if (status != AXIS2_SUCCESS)
	{
			printf("ERROR %d\n", status);
	}
    else
        printf("SUCCESS\n");

    AXIS2_CONF_CTX_FREE(conf_ctx, env);
}

int main()
{
	axis2_test_conf_ctx_init();
	return 0;
}
