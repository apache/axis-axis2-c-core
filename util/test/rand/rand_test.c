#include "../util/create_env.h"
#include <axutil_rand.h>
#include <cut_defs.h>

/** @brief test_rand 
 *   create random variable and get it's value 
 */

void test_rand(axutil_env_t *env)
{    
    int rand_number,rand_value,start = 2,end = 8,rand_range;
    unsigned seed = 10;
    
    rand_number = axutil_rand(&seed);
    printf("rand_number : %d\n", rand_number);    
    rand_range = axutil_rand_with_range(&seed,start,end);
    printf("rand_range : %d\n", rand_range);    
    CUT_ASSERT(rand_range != -1);
    CUT_ASSERT(rand_range >= start && rand_range <= end);
    rand_value = axutil_rand_get_seed_value_based_on_time(env);
    printf("rand_based_on_time : %d\n", rand_value);    
    
    /* To avoid warning of not using cut_ptr_equal */
    CUT_ASSERT_PTR_EQUAL(NULL, NULL, 0);
    /* To avoid warning of not using cut_int_equal */
    CUT_ASSERT_INT_EQUAL(0, 0, 0);
    /* To avoid warning of not using cut_str_equal */
    CUT_ASSERT_STR_EQUAL("", "", 0);

}

int main()
{
    axutil_env_t *env = cut_setup_env("Rand");
	CUT_ASSERT(env != NULL);
	if (env) {
        test_rand(env);
        axutil_env_free(env);
    }
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}


