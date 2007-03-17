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
#include "axis2_svc_skeleton.h"
#include "add.h"
#include <axis2_array_list.h>
#include <stdio.h>

int AXIS2_CALL
add_free(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env);

/*
 * This method invokes the right service method
 */
axiom_node_t* AXIS2_CALL
add_invoke(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env,
        axiom_node_t *node,
        axis2_msg_ctx_t *msg_ctx);

int AXIS2_CALL add_init(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env);


AXIS2_EXTERN axis2_svc_skeleton_t * AXIS2_CALL
axis2_add_create(const axis2_env_t *env)
{
    axis2_svc_skeleton_t *svc_skeleton = NULL;
    svc_skeleton = AXIS2_MALLOC(env->allocator,
            sizeof(axis2_svc_skeleton_t));


    svc_skeleton->ops = AXIS2_MALLOC(
                env->allocator, sizeof(axis2_svc_skeleton_ops_t));

    svc_skeleton->func_array = NULL;

    svc_skeleton->ops->free = add_free;
    svc_skeleton->ops->init = add_init;
    svc_skeleton->ops->invoke = add_invoke;
    /*svc_skeleton->ops->on_fault = add_on_fault;*/

    return svc_skeleton;
}

int AXIS2_CALL
add_init(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env)
{
    svc_skeleton->func_array = axis2_array_list_create(env, 0);
    axis2_array_list_add(svc_skeleton->func_array, env, "add");

    /* Any initialization stuff of add goes here */
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
add_free(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env)
{
    if (svc_skeleton->ops)
    {
        AXIS2_FREE(env->allocator, svc_skeleton->ops);
        svc_skeleton->ops = NULL;
    }

    if (svc_skeleton)
    {
        AXIS2_FREE(env->allocator, svc_skeleton);
        svc_skeleton = NULL;
    }
    return AXIS2_SUCCESS;
}

/*
 * This method invokes the right service method
 */
axiom_node_t* AXIS2_CALL
add_invoke(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env,
        axiom_node_t *node,
        axis2_msg_ctx_t *msg_ctx)
{
    /* Depending on the function name invoke the
     *  corresponding add method
     */
    if (node)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(node, env) == AXIOM_ELEMENT)
        {
            axiom_element_t *element = NULL;
            element = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(node, env);
            if (element)
            {
                axis2_char_t *op_name = axiom_element_get_localname(element, env);
                if (op_name)
                {
                    if (axis2_strcmp(op_name, "add") == 0)
                        return axis2_add_add(env, node, msg_ctx);
                }
            }
        }
    }

    printf("Math service ERROR: invalid OM parameters in request\n");

    /** TODO: return a SOAP fault here */
    return node;
}

/**
 * Following block distinguish the exposed part of the dll.
 */

AXIS2_EXPORT int axis2_get_instance(struct axis2_svc_skeleton **inst,
        const axis2_env_t *env)
{
    *inst = axis2_add_create(env);
    if (!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int axis2_remove_instance(axis2_svc_skeleton_t *inst,
        const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
    if (inst)
    {
        status = AXIS2_SVC_SKELETON_FREE(inst, env);
    }
    return status;
}

