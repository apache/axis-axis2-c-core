#include <axutil_url.h>
#include "../util/create_env.h"

axutil_env_t *env = NULL;
axutil_url_t * url;
axis2_char_t *url_str = "https://issues.apache.org/jira/secure/ManageAttachments.jspa?id=12386356";
axis2_char_t *set_protocol = "file";
axis2_char_t *get_protocol;
axis2_char_t *set_server = "www.yahoo.com";
axis2_char_t *get_server;
axis2_port_t set_port = 80;
axis2_port_t get_port;
axis2_char_t *set_path = "/bar/";
axis2_char_t *get_path;

/** @brief test uri 
 * create URL and get the values of it's components  
 */

axis2_status_t test_uri()
{   
    env = create_environment();
    url = (axutil_url_t *)axutil_url_create(env,"http","www.google.com",8090,"/foo/");
    url = axutil_url_parse_string(env,url_str);
    axutil_url_set_protocol(url,env,set_protocol);
    get_protocol = axutil_url_get_protocol(url,env);
    axutil_url_set_server(url,env,set_server);
    axutil_url_clone(url,env);
    axutil_url_to_uri(url,env);
    get_server = axutil_url_get_server(url,env);
    axutil_url_set_port(url,env,set_port);
    get_port = axutil_url_get_port(url,env);
    axutil_url_set_path(url,env,set_path);
    get_path = axutil_url_get_path(url,env);
    printf("The protocol is %s\n",get_protocol);
    printf("The server is %s\n",get_server);
    printf("The port is %d\n",get_port);
    printf("The path is %s\n",get_path);
    if(url)
    {
    printf("The test is SUCCESS\n"); 
    }
    if(!url)
    {
    printf("The test is FAIL");
    }
    axutil_url_free(url,env);
    return AXIS2_SUCCESS;
}
int main()
{
    int status = AXIS2_SUCCESS;
    env = create_environment();
    status = test_uri();
    if(status == AXIS2_FAILURE)
    {
        printf(" test  failed");
    }
    axutil_env_free(env);
    return 0;
}



