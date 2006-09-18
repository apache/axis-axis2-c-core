

/**
 * axis2_stub_interop_test_port_type_doc_service.c
 *
 * This file was auto-generated from WSDL for "InteropTestPortTypeDocService|http://soapinterop.org" service
 * by the Apache Axis2/C version: #axisVersion# #today#
 */

#include "axis2_stub_interop_test_port_type_doc_service.h"

/**
 * axis2_stub_interop_test_port_type_doc_service C implementation
 */

axis2_stub_t*
axis2_stub_interop_test_port_type_doc_service_create(const axis2_env_t *env,
        axis2_char_t *client_home,
        axis2_char_t *endpoint_uri)
{
    axis2_stub_t *stub = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    AXIS2_FUNC_PARAM_CHECK(client_home, env, NULL)

    if (NULL == endpoint_uri)
    {
        endpoint_uri = axis2_stub_interop_test_port_type_doc_service_get_endpoint_uri_from_wsdl(env);
    }

    endpoint_ref = axis2_endpoint_ref_create(env, endpoint_uri);

    stub = axis2_stub_create_with_endpoint_ref_and_client_home(env, endpoint_ref, client_home);
    axis2_stub_interop_test_port_type_doc_service_populate_services(stub, env);
    return stub;
}


void axis2_stub_interop_test_port_type_doc_service_populate_services(axis2_stub_t *stub, const axis2_env_t *env)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_qname_t *svc_qname =  NULL;
    axis2_qname_t *op_qname =  NULL;
    axis2_svc_t *svc = NULL;
    axis2_op_t *op = NULL;

    /* Modifying the Service */
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    svc = (axis2_svc_t*)AXIS2_SVC_CLIENT_GET_AXIS_SERVICE(svc_client, env);
    axis2_qname_create(env, "InteropTestPortTypeDocService" , NULL, NULL);
    AXIS2_SVC_SET_QNAME(svc, env, svc_qname);

    /* creating the operations*/



    op_qname = axis2_qname_create(env,
            "echoString" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoStringArray" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoInteger" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoIntegerArray" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoFloat" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoFloatArray" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoStruct" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoStructArray" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoVoid" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoBase64" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoDate" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoHexBinary" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoDecimal" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "echoBoolean" ,
            "http://soapinterop.org",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);


}

/**
 *return end point picked from wsdl
 */
axis2_char_t*
axis2_stub_interop_test_port_type_doc_service_get_endpoint_uri_from_wsdl(const axis2_env_t *env)
{
    axis2_char_t *endpoint_uri = NULL;
    /* set the address from here */

    endpoint_uri = "http://localhost/axis/InteropBaseDoc";

    return endpoint_uri;
}



/**
 * auto generated method signature
 * for "echoString|http://soapinterop.org" operation.
 * @param content0
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_string(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content0)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content0;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoString" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoString|http://soapinterop.org" operation.
  * @param content0
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_string_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content0,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content0;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoStringArray|http://soapinterop.org" operation.
 * @param content2
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_string_array(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content2)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content2;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoStringArray" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoStringArray|http://soapinterop.org" operation.
  * @param content2
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_string_array_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content2,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content2;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoInteger|http://soapinterop.org" operation.
 * @param content4
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_integer(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content4)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content4;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoInteger" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoInteger|http://soapinterop.org" operation.
  * @param content4
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_integer_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content4,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content4;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoIntegerArray|http://soapinterop.org" operation.
 * @param content6
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_integer_array(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content6)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content6;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoIntegerArray" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoIntegerArray|http://soapinterop.org" operation.
  * @param content6
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_integer_array_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content6,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content6;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoFloat|http://soapinterop.org" operation.
 * @param content8
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_float(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content8)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content8;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoFloat" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoFloat|http://soapinterop.org" operation.
  * @param content8
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_float_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content8,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content8;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoFloatArray|http://soapinterop.org" operation.
 * @param content10
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_float_array(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content10)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content10;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoFloatArray" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoFloatArray|http://soapinterop.org" operation.
  * @param content10
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_float_array_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content10,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content10;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoStruct|http://soapinterop.org" operation.
 * @param content12
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_struct(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content12)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content12;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoStruct" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoStruct|http://soapinterop.org" operation.
  * @param content12
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_struct_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content12,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content12;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoStructArray|http://soapinterop.org" operation.
 * @param content14
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_struct_array(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content14)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content14;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoStructArray" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoStructArray|http://soapinterop.org" operation.
  * @param content14
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_struct_array_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content14,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content14;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoVoid|http://soapinterop.org" operation.
 * @param content16
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_void(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content16)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content16;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoVoid" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoVoid|http://soapinterop.org" operation.
  * @param content16
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_void_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content16,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content16;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoBase64|http://soapinterop.org" operation.
 * @param content18
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_base64(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content18)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content18;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoBase64" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoBase64|http://soapinterop.org" operation.
  * @param content18
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_base64_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content18,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content18;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoDate|http://soapinterop.org" operation.
 * @param content20
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_date(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content20)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content20;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoDate" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoDate|http://soapinterop.org" operation.
  * @param content20
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_date_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content20,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content20;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoHexBinary|http://soapinterop.org" operation.
 * @param content22
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_hex_binary(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content22)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content22;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoHexBinary" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoHexBinary|http://soapinterop.org" operation.
  * @param content22
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_hex_binary_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content22,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content22;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoDecimal|http://soapinterop.org" operation.
 * @param content24
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_decimal(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content24)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content24;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoDecimal" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoDecimal|http://soapinterop.org" operation.
  * @param content24
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_decimal_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content24,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content24;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "echoBoolean|http://soapinterop.org" operation.
 * @param content26
 */
axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_boolean(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content26)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content26;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return NULL;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "echoBoolean" ,
            "http://soapinterop.org",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "echoBoolean|http://soapinterop.org" operation.
  * @param content26
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_interop_test_port_type_doc_service_echo_boolean_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content26,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content26;


    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    if (NULL == options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return;
    }
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    soap_action = AXIS2_OPTIONS_GET_ACTION(options, env);
    if (NULL == soap_action)
    {
        soap_action = "";
        AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
    }

    AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_COMPLETE(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    AXIS2_CALLBACK_SET_ON_ERROR(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


