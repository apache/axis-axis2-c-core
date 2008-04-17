#include <string.h>   
#include "../util/create_env.h"
#include <axutil_string_util.h>
#include <axutil_array_list.h>

/** @brief test string 
 *  tokenize a string  
 */

axis2_status_t test_string(axutil_env_t *env)
{   
    int delim = ' ';
    void *token = NULL;
    void *last_token_string = NULL;
    void *first_token_string = NULL;
    axutil_array_list_t * first_token, * last_token;
    axis2_char_t * in =  "this is a test string";
    
    axutil_array_list_t * tokenize = axutil_tokenize(env, in, delim);
    if(tokenize)
    {
        token  = axutil_array_list_get(tokenize,env,4);
        printf("The test axutil_tokenize is successfull\n");
        printf("The tokenize string is %s\n",(char *)token);
    }
    else 
        return AXIS2_FAILURE;

    first_token = axutil_first_token(env,in,delim);
    if(first_token)
    {
        first_token_string = axutil_array_list_get(first_token,env,1);
        printf("The test axutil_first_token is successfull\n");
        printf("First token string is %s\n",(char *)first_token_string);
    }
    else
        return AXIS2_FAILURE;
    
    last_token = axutil_last_token(env,in,delim);
    if(last_token)
    {
        last_token_string = axutil_array_list_get(last_token,env,1);
        printf("The test axutil_last_token is successfull\n");
        printf("Last token string is %s\n",(char *)last_token_string);
    }
    else
        return AXIS2_FAILURE;

    return AXIS2_SUCCESS;
}
int main()
{
    axutil_env_t *env = NULL;
    int status = AXIS2_SUCCESS;
    env = create_environment();
    status = test_string(env);
    if(status == AXIS2_FAILURE)
    {
        printf("build  failed");
    }
    axutil_env_free(env);
    return 0;
}



