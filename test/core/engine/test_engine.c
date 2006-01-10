#include <stdio.h>
#include <axis2_env.h>
#include <axis2_engine.h>
#include <axis2_conf_ctx.h>

int	axis2_test_engine_send()
{
	axis2_status_t *status = NULL;
	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);
	struct axis2_msg *msg = NULL;
    struct axis2_conf *conf = NULL;
	conf = axis2_conf_create(&env);

    struct axis2_conf_ctx *conf_ctx;
	struct axis2_msg_ctx *msg_ctx;
    conf_ctx = axis2_conf_ctx_create(&env, conf);
	axis2_transport_in_desc_t *in_desc = NULL;
	axis2_transport_out_desc_t *out_desc = NULL;

	msg_ctx = axis2_msg_ctx_create(&env, conf_ctx, NULL, NULL);	

	axis2_engine_t *engine = axis2_engine_create(&env, conf_ctx);

    axis2_engine_send(engine, &env, msg_ctx);
	if(status != AXIS2_SUCCESS )
	{
	    printf("ERROR\n");
	    return -1;
	}
}

int	axis2_test_engine_receive()
{
	axis2_status_t *status = NULL;
	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);
	struct axis2_msg *msg = NULL;
    struct axis2_conf *conf = NULL;
	conf = axis2_conf_create(&env);

    struct axis2_conf_ctx *conf_ctx;
	struct axis2_msg_ctx *msg_ctx;
    conf_ctx = axis2_conf_ctx_create(&env, conf);
	axis2_transport_in_desc_t *in_desc = NULL;
	axis2_transport_out_desc_t *out_desc = NULL;

	msg_ctx = axis2_msg_ctx_create(&env, conf_ctx, NULL, NULL);	

	axis2_engine_t *engine = axis2_engine_create(&env, conf_ctx);

    axis2_engine_receive(engine, &env, msg_ctx);
	if(status != AXIS2_SUCCESS )
	{
	    printf("ERROR\n");
	    return -1;
	}
}
int main()
{
	axis2_test_engine_send();
	axis2_test_engine_receive();
	return 0;
}
