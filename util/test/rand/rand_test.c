#include "../util/create_env.h"

int rand_number,rand_value,sleep_time,start = 2,end = 8,rand_range;
unsigned seed = 10;
unsigned int *seedp = &seed;

/** @brief test_rand 
 *   create random variable and get it's value 
 */

axis2_status_t test_rand(axutil_env_t *env)
{
    rand_number = axutil_rand(seedp);
    rand_range = axutil_rand_with_range(seedp,start,end);
    rand_value = axutil_rand_get_seed_value_based_on_time(env);
    printf("The random value is %d\n",rand_number);
    printf("The random seed value is %d\n",rand_value);
    printf("The random range is %d\n",rand_range);
    if(rand_number && rand_range && rand_value)
    {
    printf("The test is SUCCESS\n");
    }
    if(rand_number || rand_range || rand_value)
    {
    printf("The test is FAIL");
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
        printf(" test  failed");
    }
    axutil_env_free(env);
    return 0;
}

