#include <axis2_util.h>
#include <axiom.h>
#include <axis2_client.h>
#include <axis2_http_transport.h>

axiom_node_t *
build_yahoo_rest_payload (const axis2_env_t *env, axis2_char_t *string);

void 
format_output (const axis2_env_t *env, axiom_node_t *ret_node);

void 
format_output_one (const axis2_env_t *env, axiom_node_t *child_node);

int
print_help ();

int main (int argc, char *argv[])
{
    const axis2_env_t *env = NULL;
    const axis2_char_t *address = NULL;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_options_t *options = NULL;
    const axis2_char_t *client_home = NULL;
    axis2_svc_client_t* svc_client = NULL;
    axiom_node_t *payload = NULL;
    axiom_node_t *ret_node = NULL;
    axis2_property_t *rest_property = NULL;
	axis2_property_t *get_property = NULL;
	axis2_char_t *search_string = NULL;

	if (argc > 1)
	{

		if (!strcmp (argv[1], "-h") || !strcmp (argv[1], "--help"))
		{
			print_help ();
		}
		else
			search_string = argv[1];
	}

    env = axis2_env_create_all("yahoo_rest_search.log", AXIS2_LOG_LEVEL_TRACE);
	address = "http://search.yahooapis.com/WebSearchService/V1/webSearch";

	printf ("using endpoint %s \n", address);

    endpoint_ref = axis2_endpoint_ref_create(env, address);

    options = axis2_options_create(env);
    AXIS2_OPTIONS_SET_TO(options, env, endpoint_ref);

    rest_property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_VALUE(rest_property, env, axis2_strdup (AXIS2_VALUE_TRUE, env));
    AXIS2_OPTIONS_SET_PROPERTY(options, env, AXIS2_ENABLE_REST,
            rest_property);
	get_property = axis2_property_create(env);
	AXIS2_PROPERTY_SET_VALUE(get_property, env, axis2_strdup(AXIS2_HTTP_HEADER_GET, env));
	AXIS2_OPTIONS_SET_PROPERTY(options, env, AXIS2_HTTP_METHOD, get_property);

    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home || !strcmp (client_home, ""))
        client_home = "../..";

    /* Create service client */
    svc_client = axis2_svc_client_create(env, client_home);
    if (!svc_client)
    {
        printf("Error creating service client, Please check AXIS2C_HOME again\n");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
		  return -1;
    }

    /* Set service client options */
    AXIS2_SVC_CLIENT_SET_OPTIONS(svc_client, env, options);

    /* Build the SOAP request message payload using OM API.*/
    payload = build_yahoo_rest_payload (env, search_string);

    /* Send request */
    ret_node = AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, env, payload);

    if (ret_node)
    {
		format_output (env, ret_node);
        printf("\necho client invoke SUCCESSFUL!\n");
    }
    else
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        printf("echo client invoke FAILED!\n");
    }

    if (svc_client)
    {
        AXIS2_SVC_CLIENT_FREE(svc_client, env);
        svc_client = NULL;
    }
    return 0;
}

axiom_node_t *
build_yahoo_rest_payload (const axis2_env_t *env, axis2_char_t *string)
{
	axiom_node_t *root_node;
	axiom_node_t *appid_node;
	axiom_node_t *query_node;
	axiom_element_t *appid_element;
	axiom_element_t *query_element;
	axiom_element_t *root_element;
	root_node = axiom_node_create (env);
	appid_node = axiom_node_create (env);
	query_node = axiom_node_create (env);
	root_element = axiom_element_create (env, NULL, "yahoo_rest_search", NULL, &root_node);
	appid_element = axiom_element_create (env, root_node, "appid", NULL, &appid_node);
	AXIOM_ELEMENT_SET_TEXT (appid_element, env, "YahooDemo",  appid_node); 
	query_element = axiom_element_create (env, root_node, "query", NULL, &query_node);
	if (string)
		AXIOM_ELEMENT_SET_TEXT (query_element, env, string,  query_node); 
	else
		AXIOM_ELEMENT_SET_TEXT (query_element, env, "finance",  query_node); 
	return root_node;
}

void 
format_output (const axis2_env_t *env, axiom_node_t *node)
{
	axiom_node_t *child_node;
	child_node = AXIOM_NODE_GET_FIRST_CHILD (node, env);
	while (AXIOM_NODE_IS_COMPLETE (node, env) && child_node)
	{
		printf ("\n\n");
		format_output_one (env, child_node);
		child_node = AXIOM_NODE_GET_NEXT_SIBLING (child_node, env);
	}


}

void
format_output_one (const axis2_env_t *env, axiom_node_t *node)
{
	axiom_node_t *child_node;
	child_node = AXIOM_NODE_GET_FIRST_CHILD (node, env);
	while (AXIOM_NODE_IS_COMPLETE (node, env) && child_node)
	{
        axis2_char_t *om_str = AXIOM_NODE_TO_STRING (child_node, env);
        if (om_str) 
        {
    		printf ("\t%s\n", om_str);
            AXIS2_FREE(env->allocator, om_str);
        }
		child_node = AXIOM_NODE_GET_NEXT_SIBLING (child_node, env);
	}
}


int
print_help ()
{
	printf ("./yahoosearch string_to_search \n");
	exit (0);
	return 0;
}
