#include <stdio.h>
#include <axis2_env.h>
#include <axis2_engine.h>
#include <axis2_conf_ctx.h>
#include <axis2_msg_ctx.h>
#include <axis2_conf_builder.h>

int   axis2_test_engine_send()
{
   axis2_status_t status = AXIS2_FAILURE;
   axis2_allocator_t *allocator = axis2_allocator_init (NULL);
   const axis2_env_t *env = axis2_env_create (allocator);
    struct axis2_conf *conf = NULL;
   conf = axis2_conf_create(env);

    struct axis2_conf_ctx *conf_ctx;
   struct axis2_msg_ctx *msg_ctx;
   struct axis2_op_ctx *op_ctx;
   struct axis2_op *op;
   struct axis2_svc *svc;
   struct axis2_svc_ctx *svc_ctx;
   struct axis2_svc_grp *svc_grp;
   struct axis2_svc_grp_ctx *svc_grp_ctx;
   struct axis2_qname *qname;

    conf_ctx = axis2_conf_ctx_create(env, conf);
   
   svc_grp = axis2_svc_grp_create(env);
   svc_grp_ctx = axis2_svc_grp_ctx_create(env, svc_grp, conf_ctx);
   
   qname = axis2_qname_create(env, "name1", NULL, NULL);
   svc = axis2_svc_create_with_qname(env, qname);
   svc_ctx = axis2_svc_ctx_create(env, svc, svc_grp_ctx);

   op  = axis2_op_create(env);
   op_ctx = axis2_op_ctx_create(env, op, svc_ctx);

   msg_ctx = axis2_msg_ctx_create(env, conf_ctx, NULL, NULL);   

    AXIS2_MSG_CTX_SET_CONF_CTX(msg_ctx, env, conf_ctx);
   AXIS2_MSG_CTX_SET_OP_CTX(msg_ctx, env, op_ctx);
   AXIS2_MSG_CTX_SET_SVC_CTX(msg_ctx, env, svc_ctx);

   axis2_engine_t *engine = axis2_engine_create(env, conf_ctx);
    status = AXIS2_ENGINE_SEND(engine, env, msg_ctx);
   if(status != AXIS2_SUCCESS )
   {
       printf("axis2_test_engine_send ERROR %d\n", status);
   }
    else
        printf("axis2_test_engine_send SUCCESS\n");

    AXIS2_CONF_CTX_FREE(conf_ctx, env);
    AXIS2_MSG_CTX_FREE(msg_ctx, env);
    AXIS2_QNAME_FREE(qname, env);
    AXIS2_SVC_GRP_CTX_FREE(svc_grp_ctx, env);
    AXIS2_SVC_CTX_FREE(svc_ctx, env);
    AXIS2_SVC_FREE(svc, env);
    AXIS2_OP_CTX_FREE(op_ctx, env);
    AXIS2_OP_FREE(op, env);
    AXIS2_ENGINE_FREE(engine, env);
    return 0;
}

int   axis2_test_engine_receive()
{
   axis2_status_t status = AXIS2_FAILURE;
   axis2_allocator_t *allocator = axis2_allocator_init (NULL);
   const axis2_env_t *env = axis2_env_create (allocator);
    axis2_conf_t *conf = NULL;
   conf = axis2_conf_create(env);

    struct axis2_conf_ctx *conf_ctx;
   struct axis2_msg_ctx *msg_ctx;
    conf_ctx = axis2_conf_ctx_create(env, conf);

   msg_ctx = axis2_msg_ctx_create(env, conf_ctx, NULL, NULL);   

   axis2_engine_t *engine = axis2_engine_create(env, conf_ctx);

    status = AXIS2_ENGINE_RECEIVE(engine, env, msg_ctx);
   if(status != AXIS2_SUCCESS )
   {
       printf("axis2_test_engine_receive ERROR %d\n", status);
   }
    else
        printf("axis2_test_engine_receive SUCCESS\n");
    AXIS2_CONF_CTX_FREE(conf_ctx, env);
    AXIS2_MSG_CTX_FREE(msg_ctx, env);
    AXIS2_ENGINE_FREE(engine, env);
    return 0;
}


int main()
{
   axis2_test_engine_send();
   axis2_test_engine_receive();
   return 0;
}
