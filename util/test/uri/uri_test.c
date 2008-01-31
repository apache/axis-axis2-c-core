#include <axutil_uri.h>
#include "../util/create_env.h"

axutil_env_t *env = NULL;
axutil_uri_t * uri;
axis2_char_t *uri_str = "http://user:pass@example.com:80/foo?bar#item5";
axis2_char_t * hostinfo = "http://user:pass@example.com:80";
axutil_uri_t * base;
axis2_char_t * uri_str_base = "http://user:pass@example.com:80/foo?bar";
axis2_char_t * scheme_str = "http";
unsigned flags;
axis2_char_t *protocol;
axis2_char_t *server;
axis2_port_t port;
axis2_char_t *path;

/** @brief test uri 
 * create URI and get the values of it's components  
 */

test_uri()
{   
    env = create_environment();
    uri = (axutil_uri_t *)axutil_uri_create(env);
    base = (axutil_uri_t *)axutil_uri_create(env);
    base = axutil_uri_parse_string(env,uri_str_base);
    axutil_uri_parse_hostinfo(env,hostinfo);
    axutil_uri_parse_relative(env,base,(char *)uri);
    axutil_uri_port_of_scheme(scheme_str); 
    axutil_uri_parse_relative(env,base,(char *)uri);
    axutil_uri_clone(uri,env);
    uri = axutil_uri_parse_string(env,uri_str);
    axutil_uri_resolve_relative(env,base,uri);
    printf("The uri is %s\n",axutil_uri_to_string(uri,env,flags));
    protocol = axutil_uri_get_protocol(uri,env);
    server = axutil_uri_get_server(uri,env);
    port = axutil_uri_get_port(uri,env);
    path = axutil_uri_get_path(uri,env);
    printf("The protocol is %s\n",protocol);
    printf("The server is %s \n",server);
    printf("The port is %d \n",(unsigned short)port);
    printf("The path is %s\n",path);
    if(uri)
    {
    printf("The test is SUCCESS\n"); 
    }
    if(!uri)
    {
    printf("The test is FAIL");
    }
    axutil_uri_free(uri,env);
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




