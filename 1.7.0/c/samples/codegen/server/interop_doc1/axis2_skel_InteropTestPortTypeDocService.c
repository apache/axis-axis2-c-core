
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
 * axis2_skel_InteropTestPortTypeDocService.c
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis2/Java version: #axisVersion# #today#
 * axis2_skel_InteropTestPortTypeDocService Axis2/C skeleton for the axisService
 */

#include "axis2_skel_InteropTestPortTypeDocService.h"

/**
 * Auto generated function definition

  * @param param0

 */
adb_echoFloatResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoFloat(
    const axutil_env_t * env,
    adb_echoFloat_t * param0)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}

/**
 * Auto generated function definition

  * @param param2

 */
adb_echoVoidResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoVoid(
    const axutil_env_t * env,
    adb_echoVoid_t * param2)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}

/**
 * Auto generated function definition

  * @param param4

 */
adb_echoDateResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoDate(
    const axutil_env_t * env,
    adb_echoDate_t * param4)
{
    /* Todo fill this with the necessary business logic */
    adb_echoDate_t *echo_in = param4;
    adb_echoDateResponse_t *echo_out = NULL;

    axutil_date_time_t *echo_date = NULL;
    axis2_char_t *recieved_date_str = NULL;

    echo_date = adb_echoDate_get_arg_0_10(echo_in, env);

    recieved_date_str = axutil_date_time_serialize_date_time(echo_date, env);
    printf("echoing date %s\n", recieved_date_str);

    echo_out = adb_echoDateResponse_create(env);
    adb_echoDateResponse_set_echodatereturn(echo_out, env, echo_date);

    return echo_out;
}

/**
 * Auto generated function definition

  * @param param6

 */
adb_echoDecimalResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoDecimal(
    const axutil_env_t * env,
    adb_echoDecimal_t * param6)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}

/**
 * Auto generated function definition

  * @param param8

 */
adb_echoStringResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoString(
    const axutil_env_t * env,
    adb_echoString_t * param8)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}

/**
 * Auto generated function definition

  * @param param10

 */
adb_echoBase64Response_t *
axis2_skel_InteropTestPortTypeDocService_echoBase64(
    const axutil_env_t * env,
    adb_echoBase64_t * param10)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}

/**
 * Auto generated function definition

  * @param param12

 */
adb_echoFloatArrayResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoFloatArray(
    const axutil_env_t * env,
    adb_echoFloatArray_t * param12)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}

/**
 * Auto generated function definition

  * @param param14

 */
adb_echoIntegerArrayResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoIntegerArray(
    const axutil_env_t * env,
    adb_echoIntegerArray_t * param14)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}

/**
 * Auto generated function definition

  * @param param16

 */
adb_echoHexBinaryResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoHexBinary(
    const axutil_env_t * env,
    adb_echoHexBinary_t * param16)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}

/**
 * Auto generated function definition

  * @param param18

 */
adb_echoIntegerResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoInteger(
    const axutil_env_t * env,
    adb_echoInteger_t * param18)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}

/**
 * Auto generated function definition

  * @param param20

 */
adb_echoBooleanResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoBoolean(
    const axutil_env_t * env,
    adb_echoBoolean_t * param20)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}

/**
 * Auto generated function definition

  * @param param22

 */
adb_echoStringArrayResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoStringArray(
    const axutil_env_t * env,
    adb_echoStringArray_t * param22)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}

/**
 * Auto generated function definition

  * @param param24

 */
adb_echoStructArrayResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoStructArray(
    const axutil_env_t * env,
    adb_echoStructArray_t * param24)
{
    /* Todo fill this with the necessary business logic */
    adb_echoStructArray_t *echo_in = param24;
    adb_echoStructArrayResponse_t *echo_out = NULL;

    adb_SOAPStruct_t *echo_struct = NULL;
    int arr_size = 0;

    int i = 0;

    arr_size = adb_echoStructArray_sizeof_arg_0_7(echo_in, env);
    echo_out = adb_echoStructArrayResponse_create(env);
    for (i = 0; i < arr_size; i++)
    {
        echo_struct =
            adb_echoStructArray_get_arg_0_7_at(echo_in, env, i);
        printf("echoing turn %d\n string %s\n int %d\n float %f\n\n", i,
               adb_SOAPStruct_get_varString(echo_struct, env),
               adb_SOAPStruct_get_varInt(echo_struct, env),
               adb_SOAPStruct_get_varFloat(echo_struct, env));

        adb_echoStructArrayresponse_add_echoStructArrayreturn
                    (echo_out, env, echo_struct);
    }
    return echo_out;

}

/**
 * Auto generated function definition

  * @param param26

 */
adb_echoStructResponse_t *
axis2_skel_InteropTestPortTypeDocService_echoStruct(
    const axutil_env_t * env,
    adb_echoStruct_t * param26)
{
    /* Todo fill this with the necessary business logic */
    return NULL;
}
