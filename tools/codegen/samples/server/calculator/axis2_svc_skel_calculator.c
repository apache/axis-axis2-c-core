

/**
 * axis2_svc_skel_calculator.c
 *
 * This file was auto-generated from WSDL for "Calculator|http://localhost/axis/Calculator" service
 * by the Apache Axis2 version: #axisVersion# #today#
 *  axis2_svc_skel_calculator
 */

#include "axis2_skel_calculator.h"
#include <axis2_svc_skeleton.h>
#include <axis2_array_list.h>
#include <stdio.h>

/**
 * functions prototypes
 */

/* On fault, handle the fault */
axiom_node_t* AXIS2_CALL
axis2_svc_skel_calculator_on_fault(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env, axiom_node_t *node);

/* Free the service */
int AXIS2_CALL
axis2_svc_skel_calculator_free(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env);

/* This method invokes the right service method */
axiom_node_t* AXIS2_CALL
axis2_svc_skel_calculator_invoke(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env,
        axiom_node_t *node,
        axis2_msg_ctx_t *msg_ctx);

/* Initializing the environment  */
int AXIS2_CALL
axis2_svc_skel_calculator_init(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env);

/* Create the service  */
axis2_svc_skeleton_t* AXIS2_CALL
axis2_svc_skel_calculator_create(const axis2_env_t *env);

/**
 * Implementations for the functions
 */

axis2_svc_skeleton_t* AXIS2_CALL
axis2_svc_skel_calculator_create(const axis2_env_t *env)
{
    axis2_svc_skeleton_t *svc_skeleton = NULL;
    svc_skeleton = AXIS2_MALLOC(env->allocator,
            sizeof(axis2_svc_skeleton_t));


    svc_skeleton->ops = AXIS2_MALLOC(
                env->allocator, sizeof(axis2_svc_skeleton_ops_t));

    svc_skeleton->func_array = NULL;

    svc_skeleton->ops->free = axis2_svc_skel_calculator_free;
    svc_skeleton->ops->init = axis2_svc_skel_calculator_init;
    svc_skeleton->ops->invoke = axis2_svc_skel_calculator_invoke;
    svc_skeleton->ops->on_fault = axis2_svc_skel_calculator_on_fault;

    return svc_skeleton;
}


int AXIS2_CALL
axis2_svc_skel_calculator_init(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env)
{
    svc_skeleton->func_array = axis2_array_list_create(env, 10);

    AXIS2_ARRAY_LIST_ADD(svc_skeleton->func_array, env, "add");

    AXIS2_ARRAY_LIST_ADD(svc_skeleton->func_array, env, "sub");

    AXIS2_ARRAY_LIST_ADD(svc_skeleton->func_array, env, "mul");

    AXIS2_ARRAY_LIST_ADD(svc_skeleton->func_array, env, "div");


    /* Any initialization stuff of axis2_skel_calculator goes here */
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axis2_svc_skel_calculator_free(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env)
{
    if (svc_skeleton->func_array)
    {
        AXIS2_ARRAY_LIST_FREE(svc_skeleton->func_array, env);
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
axis2_svc_skel_calculator_invoke(axis2_svc_skeleton_t *svc_skeleton,
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
                axis2_char_t *op_name = AXIOM_ELEMENT_GET_LOCALNAME(element, env);
                if (op_name)
                {


                    if (AXIS2_STRCMP(op_name, "add") == 0)
                    {
                        return  axis2_skel_calculator_add(env  ,
                                content_node);


                    }


                    if (AXIS2_STRCMP(op_name, "sub") == 0)
                    {
                        return  axis2_skel_calculator_sub(env  ,
                                content_node);


                    }


                    if (AXIS2_STRCMP(op_name, "mul") == 0)
                    {
                        return  axis2_skel_calculator_mul(env  ,
                                content_node);


                    }


                    if (AXIS2_STRCMP(op_name, "div") == 0)
                    {
                        return  axis2_skel_calculator_div(env  ,
                                content_node);


                    }

                }
            }
        }
    }
    printf("axis2_svc_skel_calculator service ERROR: invalid OM parameters in request\n");
    return content_node;
}

axiom_node_t* AXIS2_CALL
axis2_svc_skel_calculator_on_fault(axis2_svc_skeleton_t *svc_skeleton,
        const axis2_env_t *env, axiom_node_t *node)
{
    axiom_node_t *error_node = NULL;
    axiom_node_t* text_node = NULL;
    axiom_element_t *error_ele = NULL;
    error_ele = axiom_element_create(env, node, "axis2_svc_skel_calculatorError", NULL,
            &error_node);
    AXIOM_ELEMENT_SET_TEXT(error_ele, env, "axis2_svc_skel_calculatorfailed",
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
    *inst = axis2_svc_skel_calculator_create(env);

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



