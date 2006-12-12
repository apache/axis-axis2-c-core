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
 * axis2_skel_interop_test_port_type_doc_service.c
 *
 * This file was auto-generated from WSDL for "InteropTestPortTypeDocService|http://soapinterop.org" service
 * by the Apache Axis2/C version: #axisVersion# #today#
 * axis2_skel_interop_test_port_type_doc_service Axis2/C skeleton for the axisService
 */

#include "axis2_skel_interop_test_port_type_doc_service.h"


/**
 * auto generated function definition signature
 * for "echoString|http://soapinterop.org" operation.
 * @param content0
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_string(const axis2_env_t *env  ,
        axiom_node_t* content0)
{
    /* TODO fill this with the necessary business logic */
    axiom_node_t* req = content0;
    axiom_node_t* node = NULL;
    axiom_element_t* ele = NULL;
    axis2_char_t* text =  NULL;

    axiom_node_t *op_node = NULL;
    axiom_element_t* op_ele = NULL;
    axiom_node_t* value_node = NULL;
    axiom_element_t * value_ele = NULL;
    axiom_namespace_t *ns1 = NULL;

    if (NULL == req)
    {
        printf("request null\n");
        return NULL;
    }
    node = AXIOM_NODE_GET_FIRST_CHILD(req, env);
    ele = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    text = AXIOM_ELEMENT_GET_TEXT(ele, env, node);

    printf("echoing string: %s\n", text);

    ns1 = axiom_namespace_create(env, "http://soapinterop.org", "ns1");
    op_ele = axiom_element_create(env, NULL, "echoStringResponse", ns1, &op_node);

    value_ele = axiom_element_create(env, op_node, "arg", NULL, &value_node);
    AXIOM_ELEMENT_SET_TEXT(value_ele, env, text, value_node);

    return op_node;
}

/**
 * auto generated function definition signature
 * for "echoStringArray|http://soapinterop.org" operation.
 * @param content2
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_string_array(const axis2_env_t *env  ,
        axiom_node_t* content2)
{
    /* TODO fill this with the necessary business logic */
    return NULL;
}

/**
 * auto generated function definition signature
 * for "echoInteger|http://soapinterop.org" operation.
 * @param content4
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_integer(const axis2_env_t *env  ,
        axiom_node_t* content4)
{
    /* TODO fill this with the necessary business logic */
    axiom_node_t* req = content4;
    axiom_node_t* node = NULL;
    axiom_element_t* ele = NULL;
    axis2_char_t* text =  NULL;

    axiom_node_t *op_node = NULL;
    axiom_element_t* op_ele = NULL;
    axiom_node_t* value_node = NULL;
    axiom_element_t * value_ele = NULL;
    axiom_namespace_t *ns1 = NULL;

    if (NULL == req)
    {
        printf("request null\n");
        return NULL;
    }
    node = AXIOM_NODE_GET_FIRST_CHILD(req, env);
    ele = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    text = AXIOM_ELEMENT_GET_TEXT(ele, env, node);

    printf("echoing integer: %s\n", text);

    ns1 = axiom_namespace_create(env, "http://soapinterop.org", "ns1");
    op_ele = axiom_element_create(env, NULL, "echoIntegerResponse", ns1, &op_node);

    value_ele = axiom_element_create(env, op_node, "arg", NULL, &value_node);
    AXIOM_ELEMENT_SET_TEXT(value_ele, env, text, value_node);

    return op_node;
}

/**
 * auto generated function definition signature
 * for "echoIntegerArray|http://soapinterop.org" operation.
 * @param content6
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_integer_array(const axis2_env_t *env  ,
        axiom_node_t* content6)
{
    /* TODO fill this with the necessary business logic */
    return NULL;
}

/**
 * auto generated function definition signature
 * for "echoFloat|http://soapinterop.org" operation.
 * @param content8
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_float(const axis2_env_t *env  ,
        axiom_node_t* content8)
{
    /* TODO fill this with the necessary business logic */
    axiom_node_t* req = content8;
    axiom_node_t* node = NULL;
    axiom_element_t* ele = NULL;
    axis2_char_t* text =  NULL;

    axiom_node_t *op_node = NULL;
    axiom_element_t* op_ele = NULL;
    axiom_node_t* value_node = NULL;
    axiom_element_t * value_ele = NULL;
    axiom_namespace_t *ns1 = NULL;

    if (NULL == req)
    {
        printf("request null\n");
        return NULL;
    }
    node = AXIOM_NODE_GET_FIRST_CHILD(req, env);
    ele = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    text = AXIOM_ELEMENT_GET_TEXT(ele, env, node);

    printf("echoing float: %s\n", text);

    ns1 = axiom_namespace_create(env, "http://soapinterop.org", "ns1");
    op_ele = axiom_element_create(env, NULL, "echoFloatResponse", ns1, &op_node);

    value_ele = axiom_element_create(env, op_node, "arg", NULL, &value_node);
    AXIOM_ELEMENT_SET_TEXT(value_ele, env, text, value_node);

    return op_node;
}

/**
 * auto generated function definition signature
 * for "echoFloatArray|http://soapinterop.org" operation.
 * @param content10
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_float_array(const axis2_env_t *env  ,
        axiom_node_t* content10)
{
    /* TODO fill this with the necessary business logic */
    return NULL;
}

/**
 * auto generated function definition signature
 * for "echoStruct|http://soapinterop.org" operation.
 * @param content12
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_struct(const axis2_env_t *env  ,
        axiom_node_t* content12)
{
    /* TODO fill this with the necessary business logic */
    return NULL;
}

/**
 * auto generated function definition signature
 * for "echoStructArray|http://soapinterop.org" operation.
 * @param content14
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_struct_array(const axis2_env_t *env  ,
        axiom_node_t* content14)
{
    /* TODO fill this with the necessary business logic */
    return NULL;
}

/**
 * auto generated function definition signature
 * for "echoVoid|http://soapinterop.org" operation.
 * @param content16
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_void(const axis2_env_t *env  ,
        axiom_node_t* content16)
{
    /* TODO fill this with the necessary business logic */
    return NULL;
}

/**
 * auto generated function definition signature
 * for "echoBase64|http://soapinterop.org" operation.
 * @param content18
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_base64(const axis2_env_t *env  ,
        axiom_node_t* content18)
{
    /* TODO fill this with the necessary business logic */
    return NULL;
}

/**
 * auto generated function definition signature
 * for "echoDate|http://soapinterop.org" operation.
 * @param content20
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_date(const axis2_env_t *env  ,
        axiom_node_t* content20)
{
    /* TODO fill this with the necessary business logic */
    return NULL;
}

/**
 * auto generated function definition signature
 * for "echoHexBinary|http://soapinterop.org" operation.
 * @param content22
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_hex_binary(const axis2_env_t *env  ,
        axiom_node_t* content22)
{
    /* TODO fill this with the necessary business logic */
    return NULL;
}

/**
 * auto generated function definition signature
 * for "echoDecimal|http://soapinterop.org" operation.
 * @param content24
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_decimal(const axis2_env_t *env  ,
        axiom_node_t* content24)
{
    /* TODO fill this with the necessary business logic */
    return NULL;
}

/**
 * auto generated function definition signature
 * for "echoBoolean|http://soapinterop.org" operation.
 * @param content26
 */
axiom_node_t* axis2_skel_interop_test_port_type_doc_service_echo_boolean(const axis2_env_t *env  ,
        axiom_node_t* content26)
{
    /* TODO fill this with the necessary business logic */
    return NULL;
}

