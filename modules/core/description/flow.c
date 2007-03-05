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

#include <axis2_flow.h>

struct axis2_flow
{
    axis2_array_list_t *list;
};

AXIS2_EXTERN axis2_flow_t *AXIS2_CALL
axis2_flow_create(
    const axis2_env_t *env)
{
    axis2_flow_t *flow = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    flow = (axis2_flow_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_flow_t));

    if (NULL == flow)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    flow->list = NULL;

    flow->list = axis2_array_list_create(env, 20);
    if (NULL == flow->list)
    {
        axis2_flow_free(flow, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    return flow;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_flow_free(
    axis2_flow_t *flow,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (flow->list)
    {
        int i = 0;
        int size = 0;

        size = AXIS2_ARRAY_LIST_SIZE(flow->list, env);
        for (i = 0; i < size; i++)
        {
            axis2_handler_desc_t *handler_desc = NULL;

            handler_desc = (axis2_handler_desc_t *) AXIS2_ARRAY_LIST_GET(
                        flow->list, env, i);
            AXIS2_HANDLER_DESC_FREE(handler_desc, env);
        }
        AXIS2_ARRAY_LIST_FREE(flow->list, env);
        flow->list = NULL;
    }

    if (flow)
    {
        AXIS2_FREE(env->allocator, flow);
        flow = NULL;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_flow_free_void_arg(
    void *flow,
    const axis2_env_t *env)
{
    axis2_flow_t *flow_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    flow_l = (axis2_flow_t *) flow;
    return axis2_flow_free(flow_l, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_flow_add_handler(
    axis2_flow_t *flow,
    const axis2_env_t *env,
    axis2_handler_desc_t *handler)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, handler, AXIS2_FAILURE);

    if (!flow->list)
    {
        flow->list = axis2_array_list_create(env, 0);
        if (NULL == flow->list)
        {
            axis2_flow_free(flow, env);
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_ARRAY_LIST_ADD(flow->list, env, handler);
}

AXIS2_EXTERN axis2_handler_desc_t *AXIS2_CALL
axis2_flow_get_handler(
    const axis2_flow_t *flow,
    const axis2_env_t *env,
    const int index)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_ARRAY_LIST_GET(flow->list, env, index);
}

AXIS2_EXTERN int AXIS2_CALL
axis2_flow_get_handler_count(
    const axis2_flow_t *flow,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return AXIS2_ARRAY_LIST_SIZE(flow->list, env);
}

