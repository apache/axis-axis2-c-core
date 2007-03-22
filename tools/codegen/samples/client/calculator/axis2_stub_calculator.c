/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/**
 * axis2_stub_calculator.c
 *
 * This file was auto-generated from WSDL for "Calculator|http://localhost/axis/Calculator" service
 * by the Apache Axis2/C version: #axisVersion# #today#
 */

#include "axis2_stub_calculator.h"

/**
 * axis2_stub_calculator C implementation
 */

axis2_stub_t*
axis2_stub_calculator_create(const axis2_env_t *env,
        axis2_char_t *client_home,
        axis2_char_t *endpoint_uri)
{
    axis2_stub_t *stub = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    AXIS2_FUNC_PARAM_CHECK(client_home, env, NULL)

    if (! endpoint_uri)
    {
        endpoint_uri = axis2_stub_calculator_get_endpoint_uri_from_wsdl(env);
    }

    endpoint_ref = axis2_endpoint_ref_create(env, endpoint_uri);

    stub = axis2_stub_create_with_endpoint_ref_and_client_home(env, endpoint_ref, client_home);
    axis2_stub_calculator_populate_services(stub, env);
    return stub;
}


void axis2_stub_calculator_populate_services(axis2_stub_t *stub, const axis2_env_t *env)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_qname_t *svc_qname =  NULL;
    axis2_qname_t *op_qname =  NULL;
    axis2_svc_t *svc = NULL;
    axis2_op_t *op = NULL;

    /* Modifying the Service */
    svc_client = axis2_stub_get_svc_client(stub, env);
    svc = (axis2_svc_t*)AXIS2_SVC_CLIENT_GET_AXIS_SERVICE(svc_client, env);
    axis2_qname_create(env, "Calculator" , NULL, NULL);
    AXIS2_SVC_SET_QNAME(svc, env, svc_qname);

    /* creating the operations*/



    op_qname = axis2_qname_create(env,
            "add" ,
            "http://localhost/axis/Calculator",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    axis2_op_set_msg_exchange_pattern(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "sub" ,
            "http://localhost/axis/Calculator",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    axis2_op_set_msg_exchange_pattern(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "mul" ,
            "http://localhost/axis/Calculator",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    axis2_op_set_msg_exchange_pattern(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);



    op_qname = axis2_qname_create(env,
            "div" ,
            "http://localhost/axis/Calculator",
            NULL);
    op = axis2_op_create_with_qname(env, op_qname);

    axis2_op_set_msg_exchange_pattern(op, env, AXIS2_MEP_URI_OUT_IN);

    AXIS2_SVC_ADD_OP(svc, env, op);


}

/**
 *return end point picked from wsdl
 */
axis2_char_t*
axis2_stub_calculator_get_endpoint_uri_from_wsdl(const axis2_env_t *env)
{
    axis2_char_t *endpoint_uri = NULL;
    /* set the address from here */

    endpoint_uri = "http://localhost/axis/Calculator";

    return endpoint_uri;
}



/**
 * auto generated method signature
 * for "add|http://localhost/axis/Calculator" operation.
 * @param content0
 */
axiom_node_t* axis2_stub_calculator_add(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content0)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content0;


    options = axis2_stub_get_options(stub, env);
    if (! options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                axis2_error_get_message(env->error));
        return NULL;
    }
    svc_client = axis2_stub_get_svc_client(stub, env);
    soap_action = axis2_options_get_action(options, env);
    if (! soap_action)
    {
        soap_action = "Calculator#add";
        axis2_options_set_action(options, env, soap_action);
    }

    axis2_options_set_soap_version(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "add" ,
            "http://localhost/axis/Calculator",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "add|http://localhost/axis/Calculator" operation.
  * @param content0
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_calculator_add_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content0,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content0;


    options = axis2_stub_get_options(stub, env);
    if (! options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                axis2_error_get_message(env->error));
        return;
    }
    svc_client = axis2_stub_get_svc_client(stub, env);
    soap_action = axis2_options_get_action(options, env);
    if (! soap_action)
    {
        soap_action = "Calculator#add";
        axis2_options_set_action(options, env, soap_action);
    }

    axis2_options_set_soap_version(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    axis2_callback_set_on_complete(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    axis2_callback_set_on_error(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "sub|http://localhost/axis/Calculator" operation.
 * @param content2
 */
axiom_node_t* axis2_stub_calculator_sub(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content2)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content2;


    options = axis2_stub_get_options(stub, env);
    if (! options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                axis2_error_get_message(env->error));
        return NULL;
    }
    svc_client = axis2_stub_get_svc_client(stub, env);
    soap_action = axis2_options_get_action(options, env);
    if (! soap_action)
    {
        soap_action = "Calculator#sub";
        axis2_options_set_action(options, env, soap_action);
    }

    axis2_options_set_soap_version(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "sub" ,
            "http://localhost/axis/Calculator",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "sub|http://localhost/axis/Calculator" operation.
  * @param content2
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_calculator_sub_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content2,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content2;


    options = axis2_stub_get_options(stub, env);
    if (! options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                axis2_error_get_message(env->error));
        return;
    }
    svc_client = axis2_stub_get_svc_client(stub, env);
    soap_action = axis2_options_get_action(options, env);
    if (! soap_action)
    {
        soap_action = "Calculator#sub";
        axis2_options_set_action(options, env, soap_action);
    }

    axis2_options_set_soap_version(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    axis2_callback_set_on_complete(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    axis2_callback_set_on_error(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "mul|http://localhost/axis/Calculator" operation.
 * @param content4
 */
axiom_node_t* axis2_stub_calculator_mul(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content4)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content4;


    options = axis2_stub_get_options(stub, env);
    if (! options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                axis2_error_get_message(env->error));
        return NULL;
    }
    svc_client = axis2_stub_get_svc_client(stub, env);
    soap_action = axis2_options_get_action(options, env);
    if (! soap_action)
    {
        soap_action = "Calculator#mul";
        axis2_options_set_action(options, env, soap_action);
    }

    axis2_options_set_soap_version(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "mul" ,
            "http://localhost/axis/Calculator",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "mul|http://localhost/axis/Calculator" operation.
  * @param content4
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_calculator_mul_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content4,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content4;


    options = axis2_stub_get_options(stub, env);
    if (! options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                axis2_error_get_message(env->error));
        return;
    }
    svc_client = axis2_stub_get_svc_client(stub, env);
    soap_action = axis2_options_get_action(options, env);
    if (! soap_action)
    {
        soap_action = "Calculator#mul";
        axis2_options_set_action(options, env, soap_action);
    }

    axis2_options_set_soap_version(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    axis2_callback_set_on_complete(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    axis2_callback_set_on_error(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


/**
 * auto generated method signature
 * for "div|http://localhost/axis/Calculator" operation.
 * @param content6
 */
axiom_node_t* axis2_stub_calculator_div(axis2_stub_t *stub, const axis2_env_t *env  ,
        axiom_node_t* content6)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;
    axiom_node_t *ret_node = NULL;

    const axis2_char_t *soap_action = NULL;
    axis2_qname_t *op_qname =  NULL;
    axiom_node_t *payload = NULL;

    payload = content6;


    options = axis2_stub_get_options(stub, env);
    if (! options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                axis2_error_get_message(env->error));
        return NULL;
    }
    svc_client = axis2_stub_get_svc_client(stub, env);
    soap_action = axis2_options_get_action(options, env);
    if (! soap_action)
    {
        soap_action = "Calculator#div";
        axis2_options_set_action(options, env, soap_action);
    }

    axis2_options_set_soap_version(options, env, AXIOM_SOAP11);

    op_qname = axis2_qname_create(env,
            "div" ,
            "http://localhost/axis/Calculator",
            NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload);



    return ret_node;

}

/**
  * auto generated method signature for asynchronous invocations
  * for "div|http://localhost/axis/Calculator" operation.
  * @param content6
  * @param on_complete callback to handle on complete
  * @param on_error callback to handle on error
  */

void axis2_stub_calculator_div_start(axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content6,
        axis2_status_t(AXIS2_CALL *on_complete)(struct axis2_callback *, const axis2_env_t *) ,
        axis2_status_t(AXIS2_CALL *on_error)(struct axis2_callback *, const axis2_env_t *, int))
{

    axis2_callback_t *callback = NULL;

    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    const axis2_char_t *soap_action = NULL;
    axiom_node_t *payload = NULL;


    payload = content6;


    options = axis2_stub_get_options(stub, env);
    if (! options)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "options is null in stub: Error code:"
                " %d :: %s", env->error->error_number,
                axis2_error_get_message(env->error));
        return;
    }
    svc_client = axis2_stub_get_svc_client(stub, env);
    soap_action = axis2_options_get_action(options, env);
    if (! soap_action)
    {
        soap_action = "Calculator#div";
        axis2_options_set_action(options, env, soap_action);
    }

    axis2_options_set_soap_version(options, env, AXIOM_SOAP11);


    callback = axis2_callback_create(env);
    /* Set our on_complete fucntion pointer to the callback object */
    axis2_callback_set_on_complete(callback, on_complete);
    /* Set our on_error function pointer to the callback object */
    axis2_callback_set_on_error(callback, on_error);

    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback);
}


