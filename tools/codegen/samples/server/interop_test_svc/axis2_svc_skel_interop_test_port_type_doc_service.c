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
 * axis2_svc_skel_interop_test_port_type_doc_service.c
 *
 * This file was auto-generated from WSDL for "InteropTestPortTypeDocService|http://soapinterop.org" service
 * by the Apache Axis2 version: #axisVersion# #today#
 *  axis2_svc_skel_interop_test_port_type_doc_service
 */

#include "axis2_skel_interop_test_port_type_doc_service.h"
#include <axis2_svc_skeleton.h>
#include <axis2_array_list.h>
#include <stdio.h>

/**
 * functions prototypes
 */

/* On fault, handle the fault */
axiom_node_t* AXIS2_CALL
axis2_svc_skel_interop_test_port_type_doc_service_on_fault(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env, axiom_node_t *node);

/* Free the service */
int AXIS2_CALL
axis2_svc_skel_interop_test_port_type_doc_service_free(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env);

/* This method invokes the right service method */
axiom_node_t* AXIS2_CALL
axis2_svc_skel_interop_test_port_type_doc_service_invoke(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env,
        axiom_node_t *node,
        axis2_msg_ctx_t *msg_ctx);

/* Initializing the environment  */
int AXIS2_CALL
axis2_svc_skel_interop_test_port_type_doc_service_init(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env);

/* Create the service  */
axis2_svc_skeleton_t* AXIS2_CALL
axis2_svc_skel_interop_test_port_type_doc_service_create(const axis2_env_t *env);

/**
 * Implementations for the functions
 */

axis2_svc_skeleton_t* AXIS2_CALL
axis2_svc_skel_interop_test_port_type_doc_service_create(const axis2_env_t *env)
{
    axis2_svc_skeleton_t *svc_skeleton = NULL;
    svc_skeleton = AXIS2_MALLOC(env->allocator,
            sizeof(axis2_svc_skeleton_t));


    svc_skeleton->ops = AXIS2_MALLOC(
                env->allocator, sizeof(axis2_svc_skeleton_ops_t));

    svc_skeleton->func_array = NULL;

    svc_skeleton->ops->free = axis2_svc_skel_interop_test_port_type_doc_service_free;
    svc_skeleton->ops->init = axis2_svc_skel_interop_test_port_type_doc_service_init;
    svc_skeleton->ops->invoke = axis2_svc_skel_interop_test_port_type_doc_service_invoke;
    svc_skeleton->ops->on_fault = axis2_svc_skel_interop_test_port_type_doc_service_on_fault;

    return svc_skeleton;
}


int AXIS2_CALL
axis2_svc_skel_interop_test_port_type_doc_service_init(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env)
{
    svc_skeleton->func_array = axis2_array_list_create(env, 10);

    axis2_array_list_add(svc_skeleton->func_array, env, "echoString");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoStringArray");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoInteger");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoIntegerArray");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoFloat");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoFloatArray");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoStruct");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoStructArray");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoVoid");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoBase64");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoDate");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoHexBinary");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoDecimal");

    axis2_array_list_add(svc_skeleton->func_array, env, "echoBoolean");


    /* Any initialization stuff of axis2_skel_interop_test_port_type_doc_service goes here */
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axis2_svc_skel_interop_test_port_type_doc_service_free(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env)
{
    if (svc_skeleton->func_array)
    {
        axis2_array_list_free(svc_skeleton->func_array, env);
        svc_skeleton->func_array = NULL;
    }

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
axis2_svc_skel_interop_test_port_type_doc_service_invoke(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env,
        axiom_node_t *content_node,
        axis2_msg_ctx_t *msg_ctx)
{
    /* depending on the function name invoke the
     * corresponding  method
     */

    axiom_element_t *element = NULL;

    if (content_node)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(content_node, env) == AXIOM_ELEMENT)
        {
            element = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(content_node, env);
            if (element)
            {
                axis2_char_t *op_name = axiom_element_get_localname(element, env);
                if (op_name)
                {


                    if (axis2_strcmp(op_name, "echoString") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_string(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoStringArray") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_string_array(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoInteger") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_integer(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoIntegerArray") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_integer_array(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoFloat") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_float(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoFloatArray") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_float_array(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoStruct") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_struct(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoStructArray") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_struct_array(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoVoid") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_void(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoBase64") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_base64(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoDate") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_date(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoHexBinary") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_hex_binary(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoDecimal") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_decimal(env  ,
                                content_node);


                    }


                    if (axis2_strcmp(op_name, "echoBoolean") == 0)
                    {
                        return  axis2_skel_interop_test_port_type_doc_service_echo_boolean(env  ,
                                content_node);


                    }

                }
            }
        }
    }
    printf("axis2_svc_skel_interop_test_port_type_doc_service service ERROR: invalid OM parameters in request\n");
    return content_node;
}

axiom_node_t* AXIS2_CALL
axis2_svc_skel_interop_test_port_type_doc_service_on_fault(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env, axiom_node_t *node)
{
    axiom_node_t *error_node = NULL;
    axiom_node_t* text_node = NULL;
    axiom_element_t *error_ele = NULL;
    error_ele = axiom_element_create(env, node, "axis2_svc_skel_interop_test_port_type_doc_serviceError", NULL,
            &error_node);
    axiom_element_set_text(error_ele, env, "axis2_svc_skel_interop_test_port_type_doc_servicefailed",
            text_node);
    return error_node;
}


/**
 * Following block distinguish the exposed part of the dll.
  */

AXIS2_EXTERN int AXIS2_CALL
axis2_get_instance(struct axis2_svc_skeleton **inst,
        const axis2_env_t *env)
{
    *inst = axis2_svc_skel_interop_test_port_type_doc_service_create(env);

    if (!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_remove_instance(axis2_svc_skeleton_t *inst,
        const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
    if (inst)
    {
        status = AXIS2_SVC_SKELETON_FREE(inst, env);
    }
    return status;
}



