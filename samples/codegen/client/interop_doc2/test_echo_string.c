#include "axis2_WSDLInteropTestDocLitService_stub.h"

int main(int argc, char** argv)
{
    axis2_env_t* env = NULL;
    axis2_char_t* client_home = NULL;

    axis2_char_t* endpoint_uri = NULL;

    axis2_stub_t* stub = NULL;

    /* variables use databinding */
    axis2_echoString_t* echo_in = NULL;
    axis2_echoStringResponse_t* echo_out = NULL;

    char* echo_str = "hello";
    char* return_echo_str = NULL;

    endpoint_uri = "http://localhost:9090/axis2/services/interop_doc2";

    env = axis2_env_create_all("codegen_utest_blocking.log", AXIS2_LOG_LEVEL_TRACE);

    /* Set up deploy folder.*/
    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../../deploy";

    stub = axis2_WSDLInteropTestDocLitService_stub_create(env, client_home , endpoint_uri);


    /* create the struct */

    /* create the input params using databinding */
    echo_in = axis2_echoString_create(env);
    AXIS2_ECHOSTRING_SET_PARAM0(echo_in, env, echo_str);

    /* invoke the web service method*/
    echo_out = axis2_echoString(stub, env, echo_in);

    /* return the output params using databinding */
    return_echo_str  = AXIS2_ECHOSTRUCTRESPONSE_GET_RETURN(echo_out, env);

    printf("returned string %s\n", return_echo_str);

    return 0;
}
