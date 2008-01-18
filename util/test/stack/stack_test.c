#include "../util/create_env.h"
#include <axutil_stack.h>

test_stack(axutil_env_t * env, char * value)
{
    axutil_stack_t * stack = NULL;
    stack = axutil_stack_create(env);
    axutil_stack_pop(stack,env);
    axutil_stack_push(stack,env,(void *)value);
    axutil_stack_size(stack,env);
    axutil_stack_get(stack,env);
    void * get_value = axutil_stack_get_at(stack,env,0);
    printf("The value of stack is %s",(char *)get_value);
    if(stack && get_value)
    {
    printf("The test is SUCCESS\n");
    }
    if(!stack || !get_value)
    {
    printf("The test is FAIL\n");
    }
    axutil_stack_free(stack,env);
    return AXIS2_SUCCESS;
}
int main()
{
    char value[10] = "test\n";
    int status = AXIS2_SUCCESS;
    axutil_env_t *env = NULL;
    env = create_environment();
    status = test_stack(env,value);
    if(status == AXIS2_FAILURE)
    {
        printf(" test  failed");
    }
    axutil_env_free(env);
    return 0;
}

