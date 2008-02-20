#include "../util/create_env.h"

/** @brief test_rand 
 *   create random variable and get it's value 
 */

axis2_status_t test_rand(axutil_env_t *env)
{    
    int rand_number,rand_value,start = 2,end = 8,rand_range;
    unsigned seed = 10;
    
    rand_number = axutil_rand(&seed);
    if(!rand_number)
    {
        printf("Test axutil_rand failed\n");         
    }
    else
    {
        printf("Test axutil_rand is successfull\n");
        printf("The random value is %d\n",rand_number);
    }
    
    rand_range = axutil_rand_with_range(&seed,start,end);
    if(rand_range == -1)
    {
        printf("Test axutil_rand_with_range failed\n");
    }
    else
    {
        printf("Test axutil_rand_with_range is successfull\n");
        printf("The random seed value is %d\n",rand_range);
    }
    
    rand_value = axutil_rand_get_seed_value_based_on_time(env);
    if(!rand_value)
    {
        printf("The test axutil_rand_get_seed_value_based_on_time failed\n");
    }
    else
    {
        printf("Test axutil_rand_get_seed_value_based_on_time is successfull\n");
        printf("The random range is %d\n",rand_value);
    }
 
    return AXIS2_SUCCESS;
}

int main()
{
    int status = AXIS2_SUCCESS;
    axutil_env_t *env = NULL;

    env = create_environment();
    status = test_rand(env);

    if(status == AXIS2_FAILURE)
    {
        printf("Test  failed\n");
    }
    axutil_env_free(env);
    return 0;
}


