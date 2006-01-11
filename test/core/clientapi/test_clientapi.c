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
	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);
    struct axis2_conf *conf = NULL;
	conf = axis2_conf_create(&env);
	struct axis2_conf_ctx *conf_ctx;
	struct axis2_msg_ctx *msg_ctx;
	struct axis2_op *op;
	struct axis2_qname *qname;
	struct axis2_svc *svc;
	struct axis2_svc_ctx *svc_ctx;
	struct axis2_svc_grp_ctx *svc_grp_ctx;
	struct axis2_call *call;
	struct axis2_svc_grp *svc_grp;
	
	conf_ctx = axis2_conf_ctx_create(&env, conf);

	op  = axis2_op_create(&env);

	qname = axis2_qname_create(&env, "name1", NULL, NULL);

	svc_grp = axis2_svc_grp_create(&env);
	svc = axis2_svc_create_with_qname(&env, qname);
	svc_grp_ctx = axis2_svc_grp_ctx_create(&env, svc_grp, conf_ctx);

	svc_ctx = axis2_svc_ctx_create(&env, svc, svc_grp_ctx);

	call = axis2_call_create(&env, svc_ctx);
	msg_ctx = axis2_call_invoke_blocking(call, &env, op, msg_ctx);
}

int main()
{
	axis2_test_call_invoke_blocking();
	return 0;
}
