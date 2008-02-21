#include <axutil_uri.h>
#include "../util/create_env.h"
/** @brief test uri 
 *  * create URI and get the values of it's components  
 *   */
axis2_status_t test_uri(axutil_env_t *env)
{   
    axis2_char_t * uri_str = "http://user:pass@example.com:80/foo?bar#item5";
    axis2_char_t * host = "home.netscape.com:443";
    axis2_char_t * uri_str_base = "http://user:pass@example.com:80/foo?bar";
    axis2_char_t * scheme_str = "http";
    axutil_uri_t * base = NULL;
    axutil_uri_t * hostinfo = NULL;
    axutil_uri_t * uri = NULL;
    axutil_uri_t * clone = NULL;
    axutil_uri_t * rel = NULL;
    axis2_char_t * protocol = NULL;
    axis2_char_t * server = NULL;
    axis2_char_t * path = NULL;
    axis2_port_t scheme_port;
    axis2_port_t port;

    hostinfo = axutil_uri_parse_hostinfo(env,host);
    if(hostinfo)
    {
        printf("The host information of uri is %s\n",axutil_uri_to_string(hostinfo,env,0));
    }
    else
    {
        printf("Test hostinfo faild\n");
    } 
    
    scheme_port = axutil_uri_port_of_scheme(scheme_str); 
    if(scheme_port)
    {
        printf("port of scheme is %u\n", scheme_port);
    }
    else
    {
        printf("Test port failed\n");
    }
    
    uri = axutil_uri_parse_string(env,uri_str);    
    if(uri)
    {
        printf("The uri is %s\n",axutil_uri_to_string(uri,env,0));
        axutil_uri_free(uri, env);
    }
    else     
    { 
         return AXIS2_FAILURE;
    }

    base = axutil_uri_parse_string(env,uri_str_base);
    if(base)
    {
         printf("The base of uri is %s\n",axutil_uri_to_string(base,env,0));
    }
    else 
    {
       printf("Test base failed\n");
    }

    clone = axutil_uri_clone(uri,env);
    if(clone)
    {
        printf("The clone of uri is %s\n",axutil_uri_to_string(clone,env,0));
        axutil_uri_free(clone,env);
    }
    else
    {
        printf("Test clone failed");
    }
    
    rel = axutil_uri_resolve_relative(env,base,clone);
    if(rel)
    {
        printf("The resolved relative uri is %s\n",axutil_uri_to_string(rel,env,0));
    }
    else
    {
        printf("Test resolve relative failed");
    }
    
    protocol = axutil_uri_get_protocol(uri,env);
    if (!protocol)
    {
        axutil_uri_free(uri,env);
        return AXIS2_FAILURE;
    }
    
    server = axutil_uri_get_server(uri,env);
    if (!server)
    {
        axutil_uri_free(uri,env);
        return AXIS2_FAILURE;
    }
    
    port = axutil_uri_get_port(uri,env);
    if (!port)
    {
        axutil_uri_free(uri,env);
        return AXIS2_FAILURE;
    }
    
    path = axutil_uri_get_path(uri,env);
    if (!path)
    {
        axutil_uri_free(uri,env);
        return AXIS2_FAILURE;
    }
   
    printf("The protocol is %s\n",protocol);
    printf("The server is %s \n",server);
    printf("The port is %u \n",port);
    printf("The path is %s\n",path); 
    axutil_uri_free(uri,env);
    return AXIS2_SUCCESS;
}

int main()
{   
    int status = AXIS2_SUCCESS;
    axutil_env_t *env = NULL;
    
    env = create_environment();
    status = test_uri(env);
    
    if(status == AXIS2_FAILURE)
    {
        printf("The Test failed");
    }
    axutil_env_free(env);
    
    return 0;
}




