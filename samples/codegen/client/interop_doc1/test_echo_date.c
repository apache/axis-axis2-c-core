#include "axis2_InteropTestPortTypeDocService_stub.h"

int main(int argc, char** argv)
{
    axis2_env_t* env = NULL;
    axis2_char_t* client_home = NULL;

    axis2_char_t* endpoint_uri = NULL;

    axis2_stub_t* stub = NULL;

    /* variables use databinding */
    axis2_echoDate_t* echo_in = NULL;
    axis2_echoDateResponse_t* echo_out = NULL;

    axis2_date_time_t* echo_date = NULL;
    axis2_date_time_t* return_echo_date = NULL;

    axis2_char_t* send_date_str = NULL;
    axis2_char_t* return_date_str = NULL;

    endpoint_uri =
        "http://localhost:9090/axis2/services/interop_doc1";

    env = axis2_env_create_all("codegen_utest_blocking.log",
            AXIS2_LOG_LEVEL_TRACE);

    /* Set up deploy folder.*/
    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../../deploy";

    stub = axis2_InteropTestPortTypeDocService_stub_create(env,
            client_home , endpoint_uri);

    /* create the struct */
    echo_date = axis2_date_time_create(env); /* default to current date */
    send_date_str = AXIS2_DATE_TIME_SERIALIZE_DATE_TIME(echo_date, env);
    printf("sending date %s\n", send_date_str);

    /* create the input params using databinding */
    echo_in = axis2_echoDate_create(env);
    AXIS2_ECHODATE_SET_ARG_0_10(echo_in, env, echo_date);

    /* invoke the web service method*/
    echo_out = axis2_echoDate(stub, env, echo_in);

    /* return the output params using databinding */
    return_echo_date  = AXIS2_ECHODATERESPONSE_GET_ECHODATERETURN(echo_out, env);

    return_date_str = AXIS2_DATE_TIME_SERIALIZE_DATE_TIME(return_echo_date, env);
    printf("returned date %s\n", return_date_str);

    return 0;
}
