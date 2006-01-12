#include <axis2.h>
#include <axis2_env.h>
#include <axis2_engine.h>
#include <axis2_call.h>
#include <axis2_allocator.h>
#include <axis2_transport_listener.h>
#include <axis2_transport_in_desc.h>
#include <axis2_transport_out_desc.h>
#include <listener_manager.h>
#include <callback_recv.h>
#include <axis2_soap_body.h>
#include <axis2_msg_ctx.h>
#include <axis2_conf_ctx.h>

void axis2_test_call_invoke_blocking()
{
	struct axis2_conf_ctx *conf_ctx = NULL;
	struct axis2_msg_ctx *msg_ctx = NULL;
	struct axis2_msg_ctx *msg_ctx_res = NULL;
	struct axis2_op *op = NULL;
	struct axis2_qname *qname = NULL;
	struct axis2_svc *svc = NULL;
	struct axis2_svc_ctx *svc_ctx = NULL;
	struct axis2_svc_grp_ctx *svc_grp_ctx = NULL;
	struct axis2_call *call = NULL;
	struct axis2_svc_grp *svc_grp = NULL;
    struct axis2_conf *conf = NULL;
	
	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);

	conf = axis2_conf_create(&env);
	conf_ctx = axis2_conf_ctx_create(&env, conf);

	op  = axis2_op_create(&env);

	qname = axis2_qname_create(&env, "name1", NULL, NULL);
	svc = axis2_svc_create_with_qname(&env, qname);

	svc_grp = axis2_svc_grp_create(&env);
	svc_grp_ctx = axis2_svc_grp_ctx_create(&env, svc_grp, conf_ctx);

	svc_ctx = axis2_svc_ctx_create(&env, svc, svc_grp_ctx);

	call = axis2_call_create(&env, svc_ctx);

	msg_ctx = axis2_msg_ctx_create(&env, conf_ctx, NULL, NULL);
	
	msg_ctx_res = AXIS2_CALL_INVOKE_BLOCKING(call, &env, op, msg_ctx);

    if (msg_ctx_res)
        printf("axis2_test_call_invoke_blocking SUCCESS\n");
    else
        printf("axis2_test_call_invoke_blocking FAILURE\n");

    AXIS2_CONF_CTX_FREE(conf_ctx, &env);    
    AXIS2_CONF_FREE(conf, &env);    
    AXIS2_MSG_CTX_FREE(msg_ctx, &env);    
    AXIS2_QNAME_FREE(qname, &env);    
    AXIS2_SVC_GRP_CTX_FREE(svc_grp_ctx, &env);    
    AXIS2_SVC_CTX_FREE(svc_ctx, &env);    
    AXIS2_SVC_FREE(svc, &env);    
    AXIS2_OP_FREE(op, &env);    
    AXIS2_CALL_FREE(call, &env);    

}

int main()
{
	axis2_test_call_invoke_blocking();
	return 0;
}
