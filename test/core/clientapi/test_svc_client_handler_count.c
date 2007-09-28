#include <stdio.h>
#include <axis2_util.h>
#include <axis2_client.h>

int
main(
    )
{
    axutil_env_t *env =
        axutil_env_create_all("hello_client.log", AXIS2_LOG_LEVEL_TRACE);
    const int TIMES = 1000;
    int i;
    for (i = 1; i <= TIMES; ++i)
    {
        printf("%d\n", i);
        axis2_svc_client_t *svc_client =
            axis2_svc_client_create(env, AXIS2_GETENV("AXIS2C_HOME"));
        axis2_svc_client_free(svc_client, env);
    }
    axutil_env_free(env);
    return 0;
}
