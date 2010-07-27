#include <axutil_url.h>
#include <cut_defs.h>
#include "../util/create_env.h"

/** @brief test url 
 * create URL and get the values of it's components  
 */

void test_url(axutil_env_t *env)
{
    axutil_url_t * url;
    axis2_char_t *url_str = "https://issues.apache.org/jira/secure/ManageAttachments.jspa?id=12386356";
    axis2_char_t *set_server = "www.yahoo.com";
    axis2_char_t *set_protocol = "file";
    axis2_char_t *set_path = "/bar/";
    axis2_port_t set_port = 80;
    axis2_char_t *get_protocol;
    axis2_char_t *get_server;
    axis2_port_t get_port;
    axis2_char_t *get_path;
    axis2_status_t status;

    url = axutil_url_parse_string(env,url_str);
    CUT_ASSERT_PTR_NOT_EQUAL(url, NULL, 1);

    status = axutil_url_set_protocol(url,env,set_protocol);   
    CUT_ASSERT_INT_EQUAL(status, AXIS2_SUCCESS, 0);
    status = axutil_url_set_server(url,env,set_server);    
    CUT_ASSERT_INT_EQUAL(status, AXIS2_SUCCESS, 0);
    status = axutil_url_set_port(url,env,set_port);    
    CUT_ASSERT_INT_EQUAL(status, AXIS2_SUCCESS, 0);
	status = axutil_url_set_path(url,env,set_path);   
    CUT_ASSERT_INT_EQUAL(status, AXIS2_SUCCESS, 0);
  
    get_protocol = axutil_url_get_protocol(url,env);
 	CUT_ASSERT_STR_EQUAL(get_protocol, set_protocol, 0);

    get_server = axutil_url_get_server(url,env);
	CUT_ASSERT_STR_EQUAL(get_server, "www.yahoo.com:80", 0);
    
    get_port = axutil_url_get_port(url,env);
    CUT_ASSERT_INT_EQUAL(get_port,set_port,0);
 
    get_path = axutil_url_get_path(url,env);
	CUT_ASSERT_STR_EQUAL(get_path, set_path, 0);

    axutil_url_free(url,env);
}

int main()
{
    axutil_env_t *env = cut_setup_env("Url");
	CUT_ASSERT(env != NULL);
	if (env) {
        test_url(env);    
        axutil_env_free(env);
     }
    CUT_RETURN_ON_FAILURE(-1);  
    return 0;
}



