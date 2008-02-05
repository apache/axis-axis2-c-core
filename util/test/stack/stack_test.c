#include "../util/create_env.h"
#include <axutil_stack.h>

axis2_status_t test_stack(axutil_env_t * env, char * value)
{
    axutil_stack_t * stack = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    stack = axutil_stack_create(env);
    void * get_value = NULL;
    if (!stack)
    {
        printf("Creation of stack failed");
        return AXIS2_FAILURE;
    }
    status = axutil_stack_push(stack,env,(void *)value);
    if (status != AXIS2_SUCCESS)
    {
        printf("Adding value to stack failed");
        axutil_stack_free(stack,env);
        return AXIS2_FAILURE;
    }
    if (axutil_stack_size(stack,env) != 1)
    {
        printf("Incorrect size of stack is reported");
        axutil_stack_free(stack,env);
        return AXIS2_FAILURE;
    }
    if (value != (char *)axutil_stack_get(stack,env))
    {
        printf("Stack returns incorrect object");
        axutil_stack_free(stack,env);
        return AXIS2_FAILURE;
    }
    get_value = axutil_stack_get_at(stack,env,0);
    printf("The value of stack is %s",(char *)get_value);
    if (value != (char *)axutil_stack_pop(stack,env))
    {
        printf("Stack returns incorrect object");
        axutil_stack_free(stack,env);
        return AXIS2_FAILURE;
    }
    if (axutil_stack_size(stack,env) != 0)
    {
        printf("Incorrect size of stack is reported");
        axutil_stack_free(stack,env);
        return AXIS2_FAILURE;
    }
    if(stack)
    {
        axutil_stack_free(stack,env);
        printf("The test is SUCCESSFUL\n");
        return AXIS2_SUCCESS;
    }
    
    return AXIS2_FAILURE;
}
int main()
{
    char value[10] = "test\n";
    int status = AXIS2_SUCCESS;
    axutil_env_t *env = NULL;
    env = create_environment();
    status = test_stack(env,value);
    if(status != AXIS2_SUCCESS)
    {
        printf("The test failed");
    }
    axutil_env_free(env);
    return 0;
}

