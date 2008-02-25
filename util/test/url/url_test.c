#include <axutil_url.h>
#include "../util/create_env.h"

/** @brief test url 
 * create URL and get the values of it's components  
 */

axis2_status_t test_url(axutil_env_t *env)
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
    if(url)
    {   
        printf("The url is created \n");
    }
    else
    {
         return AXIS2_FAILURE;
    }
    
    status = axutil_url_set_protocol(url,env,set_protocol);
    
    if (status == AXIS2_SUCCESS)
        printf("The test 1 is successful\n");
    else
        printf("The test 1 failed\n") ;
    
    status = axutil_url_set_server(url,env,set_server);
    
    if (status == AXIS2_SUCCESS)
        printf("The test 2 is successful\n");
    else
        printf("The test 2 failed\n") ;
    
    status = axutil_url_set_port(url,env,set_port);
    
    if (status == AXIS2_SUCCESS)
        printf("The test 3 is successful\n");
    else
        printf("The test 3 failed\n") ;
    
    status = axutil_url_set_path(url,env,set_path);
    
    if (status == AXIS2_SUCCESS)
        printf("The test 4 is successful\n");
    else
        printf("The test 4 failed\n") ;
    
    get_protocol = axutil_url_get_protocol(url,env);
    
    if (!get_protocol)
    {
        axutil_url_free(url,env);
        return AXIS2_FAILURE;
    }
    else 
    {
        printf("The protocol is %s\n",get_protocol);
    }

    get_server = axutil_url_get_server(url,env);
    
    if (!get_server)
    {
        axutil_url_free(url,env);
        return AXIS2_FAILURE;
    }
    else
    {
        printf("The server is %s\n",get_server);
    }
        
    get_port = axutil_url_get_port(url,env);
    
    if (!get_port)
    {
        axutil_url_free(url,env);
        return AXIS2_FAILURE;
    }
    else 
    {
        printf("The port is %d\n",get_port);
    }
 
    get_path = axutil_url_get_path(url,env);
    
    if (!get_path)
    {
        axutil_url_free(url,env);
        return AXIS2_FAILURE;
    }
    else
    {
        printf("The path is %s\n",get_path);
    }

    axutil_url_free(url,env);
    return AXIS2_SUCCESS;
}

int main()
{
    int status = AXIS2_SUCCESS;
    axutil_env_t *env = NULL;
    
    env = create_environment();
    status = test_url(env);
    
    if(status == AXIS2_FAILURE)
    {
        printf("Test failed");
    }
    axutil_env_free(env);
    
    return 0;
}



