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

#include <stdio.h>
#include <axis2_util.h>
#include <axis2_client.h>

int
main(
    )
{
    axutil_env_t *env =
        axutil_env_create_all("hello_client.log", AXIS2_LOG_LEVEL_TRACE);
    const int TIMES = 1000;
    int i;
    for (i = 1; i <= TIMES; ++i)
    {
        axis2_svc_client_t *svc_client =
            axis2_svc_client_create(env, AXIS2_GETENV("AXIS2C_HOME"));
        axis2_svc_client_free(svc_client, env);
        printf("%d\n", i);
    }
    axutil_env_free(env);
    return 0;
}
