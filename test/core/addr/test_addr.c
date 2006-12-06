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

#include <axis2_msg_info_headers.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_endpoint_ref.h>
#include <stdio.h>

int axis2_test_msg_info_headers()
{
    axis2_endpoint_ref_t *to = NULL;
    axis2_endpoint_ref_t *from = NULL;
    axis2_endpoint_ref_t *reply_to = NULL;
    axis2_endpoint_ref_t *fault_to = NULL;
    axis2_endpoint_ref_t *axis2_endpoint_ref = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_msg_info_headers_t *axis2_msg_info_headers = NULL;
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    const axis2_env_t *env = axis2_env_create(allocator);
    const axis2_char_t *action = "test action";
    const axis2_char_t *get_action = NULL;
    to = axis2_endpoint_ref_create(env, "to");
    from = axis2_endpoint_ref_create(env, "from");
    reply_to = axis2_endpoint_ref_create(env, "reply to");
    fault_to = axis2_endpoint_ref_create(env, "fault to");

    axis2_msg_info_headers = axis2_msg_info_headers_create(env, NULL, action);
    if (axis2_msg_info_headers)
        printf("SUCCESS axis2_msg_info_headers_create\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_CREATE\n");
        return -1;
    }

    status = AXIS2_MSG_INFO_HEADERS_SET_TO(axis2_msg_info_headers, env, to);

    if (status == AXIS2_SUCCESS)
        printf("SUCCESS AXIS2_MSG_INFO_HEADERS_SET_TO\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_SET_TO");
        return -1;
    }

    axis2_endpoint_ref = AXIS2_MSG_INFO_HEADERS_GET_TO(axis2_msg_info_headers, env);

    if (axis2_endpoint_ref)
        printf("SUCCESS AXIS2_MSG_INFO_HEADERS_GET_TO\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_GET_TO\n");
        return -1;
    }

    status = AXIS2_FAILURE;
    status = AXIS2_MSG_INFO_HEADERS_SET_FROM(axis2_msg_info_headers, env, from);

    if (status)
        printf("SUCCESS AXIS2_MSG_INFO_HEADERS_SET_FROM\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_SET_FROM\n");
        return -1;
    }

    axis2_endpoint_ref = NULL;
    axis2_endpoint_ref = AXIS2_MSG_INFO_HEADERS_GET_FROM(axis2_msg_info_headers, env);

    if (axis2_endpoint_ref)
        printf("SUCCESS AXIS2_MSG_INFO_HEADERS_GET_FROM\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_GET_FROM\n");
        return -1;
    }



    axis2_endpoint_ref = NULL;
    axis2_endpoint_ref = AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO(axis2_msg_info_headers, env);

    if (status)
        printf("SUCCESS AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO\n");
        return -1;
    }

    status = AXIS2_FAILURE;
    status = AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO(axis2_msg_info_headers, env, reply_to);

    if (status)
        printf("SUCCESS AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO\n");
        return -1;
    }

    status = AXIS2_FAILURE;
    status = AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(axis2_msg_info_headers, env, fault_to);
    if (status)
        printf("SUCCESS AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO\n");
        return -1;
    }

    axis2_endpoint_ref = NULL;
    axis2_endpoint_ref = AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO(axis2_msg_info_headers, env);
    if (axis2_endpoint_ref)
        printf("SUCCESS AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO\n");
        return -1;
    }


    get_action = AXIS2_MSG_INFO_HEADERS_GET_ACTION(axis2_msg_info_headers, env);
    if (get_action)
        printf("SUCCESS AXIS2_MSG_INFO_HEADERS_GET_ACTION\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_GET_ACTION\n");
        return -1;
    }

    status = AXIS2_FAILURE;
    status = AXIS2_MSG_INFO_HEADERS_SET_ACTION(axis2_msg_info_headers, env, action);
    if (status)
        printf("SUCCESS AXIS2_MSG_INFO_HEADERS_SET_ACTION\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_SET_ACTION\n");
    }


    status = AXIS2_FAILURE;
    status = AXIS2_MSG_INFO_HEADERS_FREE(axis2_msg_info_headers, env);
    if (status)
        printf("SUCCESS AXIS2_MSG_INFO_HEADERS_FREE\n");
    else
    {
        printf("ERROR AXIS2_MSG_INFO_HEADERS_FREE\n");
    }
    return 0;
}

int main()
{
    axis2_test_msg_info_headers();
    return 0;
}
