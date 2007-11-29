
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

#include "axis2_stub_WSDLInteropTestDocLitService.h"

int
main(
    int argc,
    char **argv)
{
    axutil_env_t *env = NULL;
    axis2_char_t *client_home = NULL;

    axis2_char_t *endpoint_uri = NULL;

    axis2_stub_t *stub = NULL;

    /* variables use databinding */
    adb_echoStruct_t *echo_in = NULL;
    adb_echoStructResponse_t *echo_out = NULL;
    adb_SOAPStruct_t *struct_in = NULL;
    adb_SOAPStruct_t *struct_out = NULL;

    float float_val = 11;
    int int_val = 10;
    char *string_val = "hello struct";

    int ret_int_val = 0;
    float ret_float_val = 0;
    char *ret_string_val = "";

    endpoint_uri = "http://localhost:9090/axis2/services/interop_doc2";

    env =
        axutil_env_create_all("codegen_utest_blocking.log",
                              AXIS2_LOG_LEVEL_TRACE);

    /* Set up deploy folder. */
    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../../deploy";

    stub = axis2_stub_create_WSDLInteropTestDocLitService(env,
                                                          client_home,
                                                          endpoint_uri);

    /* create the struct */
    struct_in = adb_SOAPStruct_create(env);
    adb_SOAPStruct_set_varFloat(struct_in, env, float_val);
    adb_SOAPStruct_set_varInt(struct_in, env, int_val);
    adb_SOAPStruct_set_varString(struct_in, env, string_val);

    /* create the input params using databinding */
    echo_in = adb_echoStruct_create(env);
    adb_echoStruct_set_param0(echo_in, env, struct_in);

    /* invoke the web service method */
    echo_out = axis2_stub_op_WSDLInteropTestDocLitService_echoStruct(stub, env, echo_in);

    /* return the output params using databinding */
    struct_out = adb_echoStructResponse_get_return(echo_out, env);
    /*struct_out = adb_echostruct_get_param0( ret_val, env ); */

    ret_float_val = adb_SOAPStruct_get_varFloat(struct_out, env);
    ret_int_val = adb_SOAPStruct_get_varInt(struct_out, env);
    ret_string_val = adb_SOAPStruct_get_varString(struct_out, env);

    printf("returned values \n");
    printf(" float %f\n", ret_float_val);
    printf(" int %d \n", ret_int_val);
    printf(" string %s \n", ret_string_val);

    return 0;
}
