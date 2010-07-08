#include "../util/create_env.h"
#include <axutil_utils.h>
#include <cut_defs.h>

axis2_char_t * request = "This is a request";
axis2_char_t * s = "<root>This is a & in xml string</root>";
axis2_char_t c = 'c';

/** @brief test utils 
 *  test quote string  
 */

void test_utils(axutil_env_t *env)
{
    axis2_char_t **op, *quote_string;
    int hexit;
    op = axutil_parse_request_url_for_svc_and_op(env,request);
	CUT_ASSERT_PTR_NOT_EQUAL(op, NULL, 0);
    quote_string = axutil_xml_quote_string(env,s,1);
    CUT_ASSERT_STR_EQUAL(quote_string, "&lt;root&gt;This is a &amp; in xml string&lt;/root&gt;", 0);
    hexit = axutil_hexit(c);
    CUT_ASSERT_INT_EQUAL(hexit,12, 0);
}
int main()
{
    axutil_env_t *env = cut_setup_env("util");
	CUT_ASSERT(env != NULL);
	if (env) {
       test_utils(env);
       axutil_env_free(env);
    }
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}



