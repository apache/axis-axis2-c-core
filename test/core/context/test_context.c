#include <stdio.h>
#include <axis2_conf_ctx.h>
#include <axis2_svc_grp.h>
#include <axis2.h>
#include <axis2_allocator.h>
#include <axis2_env.h>

int axis2_test_conf_ctx_init()
{
    axis2_status_t *status = NULL;
	axis2_allocator_t *allocator = axis2_allocator_init (NULL);

    axis2_env_t *env = axis2_env_create (allocator);

	struct axis2_conf * axis2_config;
	struct axis2_conf *conf = NULL;
	conf = axis2_conf_create(&env);

	struct axis2_conf_ctx *conf_ctx;

	conf_ctx = axis2_conf_ctx_create(&env, conf);

	status = axis2_conf_ctx_init(conf_ctx, &env, conf);

	if (status != AXIS2_SUCCESS)
	{
			printf("Error");
			return -1;
	}
    axis2_env_free(env);
}

int main()
{
	axis2_test_conf_ctx_init();
	return 0;
}
