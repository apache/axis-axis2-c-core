#include "../util/create_env.h"

axutil_env_t *env = NULL;
axis2_char_t * request = "This is a requset";
axis2_char_t * s = "<root>This is a & '""xml string</root>";
axis2_char_t c = 'c';

/** @brief test utils 
 *  test quote string  
 */

test_utils()
{   
    env = create_environment();
    axis2_char_t **op = (axis2_char_t **)axutil_parse_request_url_for_svc_and_op(env,request);
    axis2_char_t * quote_string = (axis2_char_t *)axutil_xml_quote_string(env,s,1);
    printf("The quote string is%s\n",(char *)quote_string);
    int hexit = axutil_hexit(c);
    printf("%d\n",hexit);
    if(op && quote_string)
    {
    printf("The test is SUCCESS\n"); 
    }
    if(!op || !quote_string)
    {
    printf("The test is FAIL");
    }
    return AXIS2_SUCCESS;
}
int main()
{
    int status = AXIS2_SUCCESS;
    env = create_environment();
    status = test_utils();
    if(status == AXIS2_FAILURE)
    {
        printf(" test  failed");
    }
    axutil_env_free(env);
    return 0;
}



