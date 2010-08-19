
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

#include "axis2_stub_InteropTestPortTypeDocService.h"

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
    adb_echoStructArray_t *echo_in = NULL;
    adb_echoStructArrayResponse_t *echo_out = NULL;

    adb_SOAPStruct_t *echo_struct = NULL;
    int arr_size = 0;
    int ret_arr_size = 0;

    int i = 0;  /* for for loop */

    endpoint_uri = "http://localhost:9090/axis2/services/interop_doc1";

    env = axutil_env_create_all("codegen_utest_blocking.log",
                                AXIS2_LOG_LEVEL_TRACE);

    /* Set up deploy folder. */
    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../../deploy";

    stub = axis2_stub_create_InteropTestPortTypeDocService(env,
                                                           client_home,
                                                           endpoint_uri);

    /* create the struct */
    echo_in = adb_echoStructArray_create(env);

    /* create the struct array */
    echo_struct = adb_SOAPStruct_create(env);
    adb_SOAPStruct_set_varString(echo_struct, env, "sturct0");
    adb_SOAPStruct_set_varInt(echo_struct, env, 0);
    adb_SOAPStruct_set_varFloat(echo_struct, env, 0);
    
    adb_echoStructArray_add_arg_0_7(echo_in, env, echo_struct);

    echo_struct = adb_SOAPStruct_create(env);
    adb_SOAPStruct_set_varString(echo_struct, env, "sturct1");
    adb_SOAPStruct_set_varInt(echo_struct, env, 10);
    adb_SOAPStruct_set_varFloat(echo_struct, env, 100);
    
    adb_echoStructArray_add_arg_0_7(echo_in, env, echo_struct);

    echo_struct = adb_SOAPStruct_create(env);
    adb_SOAPStruct_set_varString(echo_struct, env, "sturct2");
    adb_SOAPStruct_set_varInt(echo_struct, env, 20);
    adb_SOAPStruct_set_varFloat(echo_struct, env, 200);
    
    adb_echoStructArray_add_arg_0_7(echo_in, env, echo_struct);

    /* invoke the web service method */
    echo_out = axis2_stub_op_InteropTestPortTypeDocService_echoStruct(stub, env, echo_in);

    ret_arr_size = adb_echoStructArrayResponse_sizeof_echoStructArrayReturn(echo_out, env);
    for (i = 0; i < ret_arr_size; i++)
    {
        echo_struct = adb_echoStructArrayResponse_get_echoStructArrayReturn(echo_out, env);
        printf("recieved turn %d \n string %s\n int %d\n float %f\n\n", i,
               adb_SOAPStruct_get_varString(echo_struct, env),
               adb_SOAPStruct_get_varInt(echo_struct, env),
               adb_SOAPStruct_get_varFloat(echo_struct, env));
    }
    return 0;
}
