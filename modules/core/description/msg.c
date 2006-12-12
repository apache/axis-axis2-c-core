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

#include <axis2_msg.h>
#include <axis2_property.h>
#include <xml_schema_obj_collection.h>

typedef struct axis2_msg_impl
{
    axis2_msg_t msg;
    /** parent operation */
    axis2_op_t *parent;
    /** list of phases that represent the flow  */
    axis2_array_list_t *flow;
    /** name of the message */
    axis2_char_t *name;
    /** XML schema element qname */
    axis2_qname_t *element_qname;
    /** direction of message */
    axis2_char_t *direction;
}
axis2_msg_impl_t;

#define AXIS2_INTF_TO_IMPL(msg) ((axis2_msg_impl_t *)msg)

axis2_status_t AXIS2_CALL
axis2_msg_free(
    axis2_msg_t *msg,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_msg_add_param(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    axis2_param_t *param);

axis2_param_t *AXIS2_CALL
axis2_msg_get_param(
    const axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *name);

axis2_array_list_t *AXIS2_CALL
axis2_msg_get_all_params(
    const axis2_msg_t *msg,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_msg_is_param_locked(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *param_name);

axis2_status_t AXIS2_CALL
axis2_msg_set_parent(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    axis2_op_t *op);

axis2_op_t *AXIS2_CALL
axis2_msg_get_parent(
    const axis2_msg_t *msg,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
axis2_msg_get_flow(
    const axis2_msg_t *msg,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_msg_set_flow(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    axis2_array_list_t *flow);

const axis2_char_t *AXIS2_CALL
axis2_msg_get_direction(
    const axis2_msg_t *msg,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_msg_set_direction(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *direction);

const axis2_qname_t *AXIS2_CALL
axis2_msg_get_element_qname(
    const axis2_msg_t *msg,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_msg_set_element_qname(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_qname_t *element_qname);

const axis2_char_t *AXIS2_CALL
axis2_msg_get_name(
    const axis2_msg_t *msg,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_msg_set_name(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *name);

xml_schema_element_t *AXIS2_CALL
axis2_msg_get_schema_element(
    const axis2_msg_t *msg,
    const axis2_env_t *env);


AXIS2_EXTERN axis2_msg_t *AXIS2_CALL
axis2_msg_create(
    const axis2_env_t *env)
{
    axis2_msg_impl_t *msg_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    msg_impl = (axis2_msg_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_msg_impl_t));

    if (NULL == msg_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    msg_impl->msg.param_container = NULL;
    msg_impl->parent = NULL;
    msg_impl->flow = NULL;
    msg_impl->name = NULL;
    msg_impl->element_qname = NULL;
    msg_impl->direction = NULL;
    msg_impl->msg.ops = NULL;

    msg_impl->msg.param_container = (axis2_param_container_t *)
            axis2_param_container_create(env);
    if (NULL == msg_impl->msg.param_container)
    {
        axis2_msg_free(&(msg_impl->msg), env);
        return NULL;
    }


    msg_impl->flow = axis2_array_list_create(env, 0);
    if (NULL == msg_impl->flow)
    {
        axis2_msg_free(&(msg_impl->msg), env);
        return NULL;
    }

    msg_impl->msg.ops = AXIS2_MALLOC(env->allocator, sizeof(axis2_msg_ops_t));
    if (NULL == msg_impl->msg.ops)
    {
        axis2_msg_free(&(msg_impl->msg), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    msg_impl->msg.ops->free = axis2_msg_free;
    msg_impl->msg.ops->add_param = axis2_msg_add_param;
    msg_impl->msg.ops->get_param = axis2_msg_get_param;
    msg_impl->msg.ops->get_all_params = axis2_msg_get_all_params;
    msg_impl->msg.ops->is_param_locked = axis2_msg_is_param_locked;
    msg_impl->msg.ops->set_parent = axis2_msg_set_parent;
    msg_impl->msg.ops->get_parent = axis2_msg_get_parent;
    msg_impl->msg.ops->get_flow = axis2_msg_get_flow;
    msg_impl->msg.ops->set_flow = axis2_msg_set_flow;
    msg_impl->msg.ops->get_direction = axis2_msg_get_direction;
    msg_impl->msg.ops->set_direction = axis2_msg_set_direction;
    msg_impl->msg.ops->get_element_qname = axis2_msg_get_element_qname;
    msg_impl->msg.ops->set_element_qname = axis2_msg_set_element_qname;
    msg_impl->msg.ops->get_name = axis2_msg_get_name;
    msg_impl->msg.ops->set_name = axis2_msg_set_name;
    msg_impl->msg.ops->get_schema_element = axis2_msg_get_schema_element;

    return &(msg_impl->msg);
}

axis2_status_t AXIS2_CALL
axis2_msg_free(
    axis2_msg_t *msg,
    const axis2_env_t *env)
{
    axis2_msg_impl_t *msg_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg_impl = AXIS2_INTF_TO_IMPL(msg);

    if (msg_impl->flow)
    {
        int i = 0;
        int size = 0;
        size = AXIS2_ARRAY_LIST_SIZE(msg_impl->flow, env);
        for (i = 0; i < size; i++)
        {
            axis2_phase_t *phase = NULL;
            phase = (axis2_phase_t *)(AXIS2_ARRAY_LIST_GET(msg_impl->flow, env, 
                i));
            if (phase)
            {
                AXIS2_PHASE_FREE(phase, env);
                phase = NULL;
            }

        }
        AXIS2_ARRAY_LIST_FREE(msg_impl->flow, env);
        msg_impl->flow = NULL;
    }

    if (msg_impl->name)
    {
        AXIS2_FREE(env->allocator, msg_impl->name);
        msg_impl->name = NULL;
    }

    if (msg_impl->element_qname)
    {
        AXIS2_QNAME_FREE(msg_impl->element_qname, env);
        msg_impl->element_qname = NULL;
    }

    if (msg_impl->direction)
    {
        AXIS2_FREE(env->allocator, msg_impl->direction);
        msg_impl->direction = NULL;
    }

    if (msg->param_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(msg->param_container, env);
        msg->param_container = NULL;
    }

    msg_impl->parent = NULL;

    if (msg_impl->msg.ops)
    {
        AXIS2_FREE(env->allocator, msg_impl->msg.ops);
        msg_impl->msg.ops = NULL;
    }

    if (msg_impl)
    {
        AXIS2_FREE(env->allocator, msg_impl);
        msg_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_msg_add_param(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    axis2_param_t *param)
{
    axis2_msg_impl_t *msg_impl = NULL;
    axis2_char_t *param_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param, AXIS2_FALSE);
    msg_impl = AXIS2_INTF_TO_IMPL(msg);

    param_name = AXIS2_PARAM_GET_NAME(param, env);
    if (AXIS2_TRUE == axis2_msg_is_param_locked(msg, env, param_name))
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        return AXIS2_PARAM_CONTAINER_ADD_PARAM(msg->param_container, env, param);
    }

    return AXIS2_SUCCESS;
}

axis2_param_t *AXIS2_CALL
axis2_msg_get_param(
    const axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *param_name)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param_name, NULL);

    return AXIS2_PARAM_CONTAINER_GET_PARAM(msg->param_container, env, param_name);
}

axis2_array_list_t *AXIS2_CALL
axis2_msg_get_all_params(
    const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    axis2_msg_impl_t *msg_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, msg->param_container, AXIS2_FALSE);
    msg_impl = AXIS2_INTF_TO_IMPL(msg);

    return AXIS2_PARAM_CONTAINER_GET_PARAMS(msg->param_container, env);
}

axis2_status_t AXIS2_CALL
axis2_msg_set_parent(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    axis2_op_t *op)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg)->parent = op;
    return AXIS2_SUCCESS;
}

axis2_op_t *AXIS2_CALL
axis2_msg_get_parent(
    const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return AXIS2_INTF_TO_IMPL(msg)->parent;
}

axis2_array_list_t *AXIS2_CALL
axis2_msg_get_flow(
    const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(msg)->flow;
}

axis2_bool_t AXIS2_CALL
axis2_msg_is_param_locked(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *param_name)
{
    axis2_op_t *parent_l = NULL;
    axis2_param_t *param_l = NULL;
    axis2_bool_t locked = AXIS2_FALSE;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param_name, AXIS2_FALSE);

    /* checking the locked status in parent */
    parent_l = axis2_msg_get_parent(msg, env);
    if (parent_l)
    {
        locked = AXIS2_OP_IS_PARAM_LOCKED(parent_l, env, param_name);
    }
    if (AXIS2_TRUE == locked)
    {
        return AXIS2_TRUE;
    }
    else
    {
        param_l = axis2_msg_get_param(msg, env, param_name);
    }
    return (param_l  && AXIS2_PARAM_IS_LOCKED(param_l, env));
}

axis2_status_t AXIS2_CALL
axis2_msg_set_flow(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    axis2_array_list_t *flow)
{
    axis2_msg_impl_t *msg_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg_impl = AXIS2_INTF_TO_IMPL(msg);

    if (msg_impl->flow)
    {
        AXIS2_ARRAY_LIST_FREE(msg_impl->flow, env);
        msg_impl->flow = NULL;
    }
    if (flow)
    {
        msg_impl->flow = flow;
    }
    return AXIS2_SUCCESS;
}

const axis2_char_t *AXIS2_CALL
axis2_msg_get_direction(
    const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg)->direction;
}

axis2_status_t AXIS2_CALL
axis2_msg_set_direction(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *direction)
{
    axis2_msg_impl_t *msg_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg_impl = AXIS2_INTF_TO_IMPL(msg);

    if (msg_impl->direction)
    {
        AXIS2_FREE(env->allocator, msg_impl->direction);
        msg_impl->direction = NULL;
    }

    if (direction)
    {
        msg_impl->direction = AXIS2_STRDUP(direction, env);
        if (!(msg_impl->direction))
            return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

const axis2_qname_t *AXIS2_CALL
axis2_msg_get_element_qname(
    const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg)->element_qname;
}

axis2_status_t AXIS2_CALL
axis2_msg_set_element_qname(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_qname_t *element_qname)
{
    axis2_msg_impl_t *msg_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg_impl = AXIS2_INTF_TO_IMPL(msg);

    if (msg_impl->element_qname)
    {
        AXIS2_QNAME_FREE(msg_impl->element_qname, env);
        msg_impl->element_qname = NULL;
    }

    if (element_qname)
    {
        msg_impl->element_qname = AXIS2_QNAME_CLONE(element_qname, env);
        if (!(msg_impl->element_qname))
            return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}


xml_schema_element_t *AXIS2_CALL
axis2_msg_get_schema_element(
    const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    axis2_msg_impl_t *msg_impl = NULL;
    axis2_op_t *operation = NULL;
    axis2_svc_t *svc = NULL;
    axis2_array_list_t *schemas = NULL;
    int i = 0;
    int schemas_size = 0;

    AXIS2_ENV_CHECK(env, NULL);

    msg_impl = AXIS2_INTF_TO_IMPL(msg);
    operation = axis2_msg_get_parent(msg, env);
    svc = AXIS2_OP_GET_PARENT(operation, env);
    schemas = AXIS2_SVC_GET_ALL_SCHEMAS(svc, env);
    schemas_size = AXIS2_ARRAY_LIST_SIZE(schemas, env);
    for (i = 0; i < schemas_size; i++)
    {
        xml_schema_t *schema = NULL;
        xml_schema_obj_collection_t *schema_collection = NULL;

        schema = AXIS2_ARRAY_LIST_GET(schemas, env, i);
        schema_collection = XML_SCHEMA_GET_ITEMS(schema, env);
        if (schema_collection)
        {
            int count = 0;
            int j = 0;

            count = XML_SCHEMA_OBJ_COLLECTION_GET_COUNT(schema_collection, env);
            for (j = 0; j < count; j++)
            {
                xml_schema_obj_t *schema_obj = NULL;
                schema_obj = XML_SCHEMA_OBJ_COLLECTION_GET_ITEM(
                            schema_collection, env, j);
                if (NULL == schema_obj && XML_SCHEMA_ELEMENT ==
                        XML_SCHEMA_OBJ_GET_TYPE(schema_obj, env))
                {
                    axis2_qname_t *schema_qname = NULL;
                    schema_qname = XML_SCHEMA_ELEMENT_GET_QNAME(schema_obj, env);
                    if (msg_impl->element_qname && AXIS2_TRUE ==
                            AXIS2_QNAME_EQUALS(msg_impl->element_qname, env,
                                    schema_qname))
                    {
                        return (xml_schema_element_t*)schema_obj;
                    }
                }
            }
        }
    }
    return NULL;
}

const axis2_char_t *AXIS2_CALL
axis2_msg_get_name(
    const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg)->name;
}

axis2_status_t AXIS2_CALL
axis2_msg_set_name(
    axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *name)
{
    axis2_msg_impl_t *msg_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg_impl = AXIS2_INTF_TO_IMPL(msg);

    if (msg_impl->name)
    {
        AXIS2_FREE(env->allocator, msg_impl->name);
        msg_impl->name = NULL;
    }

    if (name)
    {
        msg_impl->name = AXIS2_STRDUP(name, env);
        if (!(msg_impl->name))
        {
            return AXIS2_FAILURE;
        }
    }

    return AXIS2_SUCCESS;
}
