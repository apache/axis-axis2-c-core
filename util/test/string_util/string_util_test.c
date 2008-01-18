#include <string.h>   
#include "../util/create_env.h"
#include <axutil_array_list.h>

axutil_env_t *env = NULL;
axis2_char_t * in =  "this is a test string";
int delim = ' ';

/** @brief test string 
 *  tokenize a string  
 */

test_string()
{   
    env = create_environment();
    axutil_array_list_t * tokenize = (axutil_array_list_t *)axutil_tokenize(env, in, delim);
    axutil_array_list_t * first_token = (axutil_array_list_t *)axutil_first_token(env,in,delim);
    axutil_array_list_t * last_token = (axutil_array_list_t *)axutil_last_token(env,in,delim);
    void * token  = axutil_array_list_get(tokenize,env,4);
    printf("%s\n",(char *)token);
    void * first_token_string = axutil_array_list_get(first_token,env,1);
    printf("%s\n",(char *)first_token_string);
    void * last_token_string = axutil_array_list_get(last_token,env,1);
    printf("%s\n",(char *)last_token_string);

    if(tokenize && first_token && last_token)
    {
    printf("The test is SUCCESS\n"); 
    }
    if(!tokenize || !first_token || !last_token)
    {
    printf("The test is FAIL");
    }
    return AXIS2_SUCCESS;
}
int main()
{
    int status = AXIS2_SUCCESS;
    env = create_environment();
    status = test_string();
    if(status == AXIS2_FAILURE)
    {
        printf(" test  failed");
    }
    axutil_env_free(env);
    return 0;
}



