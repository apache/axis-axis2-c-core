
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

#include <axis2_phases_info.h>

struct axis2_phases_info
{
    axutil_array_list_t *in_phases;
    axutil_array_list_t *out_phases;
    axutil_array_list_t *in_faultphases;
    axutil_array_list_t *out_faultphases;
    axutil_hash_t *op_in_phases;
    axutil_hash_t *op_out_phases;
    axutil_hash_t *op_in_faultphases;
    axutil_hash_t *op_out_faultphases;

};

AXIS2_EXTERN axis2_phases_info_t *AXIS2_CALL
axis2_phases_info_create(
    const axutil_env_t * env)
{
    axis2_phases_info_t *phases_info = NULL;

    phases_info = (axis2_phases_info_t *) AXIS2_MALLOC(env->allocator,
                                                       sizeof
                                                       (axis2_phases_info_t));

    if (!phases_info)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

	memset ((void *)phases_info, 0, sizeof (axis2_phases_info_t));
    phases_info->op_in_phases = axutil_hash_make(env);
    phases_info->op_out_phases = axutil_hash_make(env);
    phases_info->op_in_faultphases = axutil_hash_make(env);
    phases_info->op_out_faultphases = axutil_hash_make(env);

    return phases_info;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_phases_info_free(
    axis2_phases_info_t * phases_info,
    const axutil_env_t * env)
{
    if (phases_info->in_phases)
    {
        axutil_array_list_free(phases_info->in_phases, env);
    }

    if (phases_info->out_phases)
    {
        axutil_array_list_free(phases_info->out_phases, env);
    }

    if (phases_info->in_faultphases)
    {
        axutil_array_list_free(phases_info->in_faultphases, env);
    }

    if (phases_info->out_faultphases)
    {
        axutil_array_list_free(phases_info->out_faultphases, env);
    }

    if (phases_info->op_in_phases)
    {
        axutil_hash_free(phases_info->op_in_phases, env);
    }

    if (phases_info->op_out_phases)
    {
        axutil_hash_free(phases_info->op_out_phases, env);
    }

    if (phases_info->op_in_faultphases)
    {
        axutil_hash_free(phases_info->op_in_faultphases, env);
    }

    if (phases_info->op_out_faultphases)
    {
        axutil_hash_free(phases_info->op_out_faultphases, env);
    }

    if (phases_info)
    {
        AXIS2_FREE(env->allocator, phases_info);
    }

    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phases_info_set_in_phases(
    axis2_phases_info_t * phases_info,
    const axutil_env_t * env,
    axutil_array_list_t * in_phases)
{
    AXIS2_PARAM_CHECK(env->error, in_phases, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK(env->error, phases_info, AXIS2_FAILURE);
	
    if (phases_info->in_phases)
    {
        axutil_array_list_free(phases_info->in_phases, env);
    }

    phases_info->in_phases = in_phases;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phases_info_set_out_phases(
    axis2_phases_info_t * phases_info,
    const axutil_env_t * env,
    axutil_array_list_t * out_phases)
{
    AXIS2_PARAM_CHECK(env->error, out_phases, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK(env->error, phases_info, AXIS2_FAILURE);

    if (phases_info->out_phases)
    {
        axutil_array_list_free(phases_info->out_phases, env);
        phases_info->out_phases = NULL;
    }
    phases_info->out_phases = out_phases;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phases_info_set_in_faultphases(
    axis2_phases_info_t * phases_info,
    const axutil_env_t * env,
    axutil_array_list_t * in_faultphases)
{
    AXIS2_PARAM_CHECK(env->error, in_faultphases, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK(env->error, phases_info, AXIS2_FAILURE);

    if (phases_info->in_faultphases)
    {
        axutil_array_list_free(phases_info->in_faultphases, env);
        phases_info->in_faultphases = NULL;
    }
    phases_info->in_faultphases = in_faultphases;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phases_info_set_out_faultphases(
    axis2_phases_info_t * phases_info,
    const axutil_env_t * env,
    axutil_array_list_t * out_faultphases)
{
    AXIS2_PARAM_CHECK(env->error, out_faultphases, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK(env->error, phases_info, AXIS2_FAILURE);

    if (phases_info->out_faultphases)
    {
        axutil_array_list_free(phases_info->out_faultphases, env);
        phases_info->out_faultphases = NULL;
    }
    phases_info->out_faultphases = out_faultphases;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_phases_info_get_in_phases(
    const axis2_phases_info_t * phases_info,
    const axutil_env_t * env)
{
	AXIS2_PARAM_CHECK(env->error, phases_info, NULL);
    return phases_info->in_phases;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_phases_info_get_out_phases(
    const axis2_phases_info_t * phases_info,
    const axutil_env_t * env)
{
	AXIS2_PARAM_CHECK(env->error, phases_info, NULL);
    return phases_info->out_phases;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_phases_info_get_in_faultphases(
    const axis2_phases_info_t * phases_info,
    const axutil_env_t * env)
{
	AXIS2_PARAM_CHECK(env->error, phases_info, NULL);
    return phases_info->in_faultphases;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_phases_info_get_out_faultphases(
    const axis2_phases_info_t * phases_info,
    const axutil_env_t * env)
{
	AXIS2_PARAM_CHECK(env->error, phases_info, NULL);
    return phases_info->out_faultphases;
}

/* Here we create the operation inflow as an array list and create phases for phases defined in
 * inflow of axis2.xml and add them into the array list.
 */
AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_phases_info_get_op_in_phases(
    const axis2_phases_info_t * phases_info,
    const axutil_env_t * env)
{
    struct axis2_phase *phase = NULL;
    int i = 0;
    int size = 0;
    axis2_char_t *phase_name = NULL;
    axutil_array_list_t *op_in_phases = NULL;
    axis2_status_t status = AXIS2_FAILURE;

	AXIS2_PARAM_CHECK(env->error, phases_info, NULL);

    op_in_phases = axutil_array_list_create(env, 0);
    if (!op_in_phases)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    if (!phases_info->in_phases)
    {
        return op_in_phases;
    }

    /* For each inflow phase name create a phase instance and add into the inflow. */
    size = axutil_array_list_size(phases_info->in_phases, env);
    for (i = 0; i < size; i++)
    {
        phase_name = (axis2_char_t *) axutil_array_list_get(phases_info->in_phases, env, i);

        if (!axutil_strcmp(AXIS2_PHASE_TRANSPORT_IN, phase_name) ||
            !axutil_strcmp(AXIS2_PHASE_PRE_DISPATCH, phase_name) ||
            !axutil_strcmp(AXIS2_PHASE_DISPATCH, phase_name) ||
            !axutil_strcmp(AXIS2_PHASE_POST_DISPATCH, phase_name))
        {
            /* We are not concerned here with system phases. */
        }
        else
        {
            /* This is the cause for jira AXIS2C-624. As solution I create new 
             * phases for each operation specific flow making the operation 
             * the owner of the flow and the phases contained within it*/
            /*phase = axutil_hash_get(phases_info->op_in_phases, phase_name, 
               AXIS2_HASH_KEY_STRING);
               if(!phase)
               {
               phase = axis2_phase_create(env, phase_name);
               axutil_hash_set(phases_info->op_in_phases, phase_name,
               AXIS2_HASH_KEY_STRING, phase);
               } */
            phase = axis2_phase_create(env, phase_name);
            status = axutil_array_list_add(op_in_phases, env, phase);
            if (AXIS2_SUCCESS != status)
            {
                int i = 0;
                int size = 0;

                axis2_phase_free(phase, env);
                phase = NULL;
                size = axutil_array_list_size(op_in_phases, env);
                for (i = 0; i < size; i++)
                {
                    phase = axutil_array_list_get(op_in_phases, env, i);
                    axis2_phase_free(phase, env);
                    phase = NULL;
                }
                axutil_array_list_free(op_in_phases, env);
                op_in_phases = NULL;
                return NULL;
            }
        }
    }

    return op_in_phases;
}

/* Here we create the operation outflow as an array list and create phases for phases defined in
 * outflow of axis2.xml and add them into the array list.
 */
AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_phases_info_get_op_out_phases(
    const axis2_phases_info_t * phases_info,
    const axutil_env_t * env)
{
    struct axis2_phase *phase = NULL;
    int i = 0;
    int size = 0;
    axis2_char_t *phase_name = NULL;
    axutil_array_list_t *op_out_phases = NULL;
    axis2_status_t status = AXIS2_FAILURE;

	AXIS2_PARAM_CHECK(env->error, phases_info, NULL);

    if (phases_info->out_phases)
    {
        size = axutil_array_list_size(phases_info->out_phases, env);
    }

    op_out_phases = axutil_array_list_create(env, 0);
    if (!op_out_phases)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    for (i = 0; i < size; i++)
    {
        phase_name = (axis2_char_t *) axutil_array_list_get(phases_info->out_phases, env, i);

        phase = axis2_phase_create(env, phase_name);
        status = axutil_array_list_add(op_out_phases, env, phase);
        if (AXIS2_SUCCESS != status)
        {
            int i = 0;
            int size = 0;

            axis2_phase_free(phase, env);
            phase = NULL;
            size = axutil_array_list_size(op_out_phases, env);
            for (i = 0; i < size; i++)
            {
                phase = axutil_array_list_get(op_out_phases, env, i);
                axis2_phase_free(phase, env);
                phase = NULL;
            }

            axutil_array_list_free(op_out_phases, env);
            op_out_phases = NULL;
            return NULL;
        }
    }

    return op_out_phases;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_phases_info_get_op_in_faultphases(
    const axis2_phases_info_t * phases_info,
    const axutil_env_t * env)
{
    int i = 0;
    int size = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *phase_name = NULL;
    axutil_array_list_t *op_in_faultphases = NULL;
    struct axis2_phase *phase = NULL;

	AXIS2_PARAM_CHECK(env->error, phases_info, NULL);

    if (!phases_info->in_faultphases)
    {
        return NULL;
    }

    size = axutil_array_list_size(phases_info->in_faultphases, env);
    if (0 == size)
    {
        return NULL;
    }

    op_in_faultphases = axutil_array_list_create(env, 0);
    if (!op_in_faultphases)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    for (i = 0; i < size; i++)
    {
        phase_name = (axis2_char_t *) axutil_array_list_get(phases_info->in_faultphases, env, i);

        phase = axis2_phase_create(env, phase_name);
        status = axutil_array_list_add(op_in_faultphases, env, phase);
        if (AXIS2_SUCCESS != status)
        {
            int i = 0;
            int size = 0;

            axis2_phase_free(phase, env);
            phase = NULL;
            size = axutil_array_list_size(op_in_faultphases, env);
            for (i = 0; i < size; i++)
            {
                phase = axutil_array_list_get(op_in_faultphases, env, i);
                axis2_phase_free(phase, env);
                phase = NULL;
            }
            axutil_array_list_free(op_in_faultphases, env);
            op_in_faultphases = NULL;
            return NULL;

        }
    }

    return op_in_faultphases;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_phases_info_get_op_out_faultphases(
    const axis2_phases_info_t * phases_info,
    const axutil_env_t * env)
{
    int i = 0;
    int size = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *phase_name = NULL;
    axutil_array_list_t *op_out_faultphases = NULL;
    struct axis2_phase *phase = NULL;

	AXIS2_PARAM_CHECK(env->error, phases_info, NULL);

    if (!phases_info->out_faultphases)
    {
        return NULL;
    }
    size = axutil_array_list_size(phases_info->out_faultphases, env);

    if (0 == size)
    {
        return NULL;
    }

    op_out_faultphases = axutil_array_list_create(env, 0);
    if (!op_out_faultphases)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    for (i = 0; i < size; i++)
    {
        phase_name = (axis2_char_t *) axutil_array_list_get(phases_info->out_faultphases, env, i);
        phase = axis2_phase_create(env, phase_name);
        status = axutil_array_list_add(op_out_faultphases, env, phase);
        if (AXIS2_SUCCESS != status)
        {
            int i = 0;
            int size = 0;

            axis2_phase_free(phase, env);
            phase = NULL;
            size = axutil_array_list_size(op_out_faultphases, env);
            for (i = 0; i < size; i++)
            {
                phase = axutil_array_list_get(op_out_faultphases, env, i);
                axis2_phase_free(phase, env);
                phase = NULL;
            }
            axutil_array_list_free(op_out_faultphases, env);
            op_out_faultphases = NULL;
            return NULL;

        }
    }

    return op_out_faultphases;
}

/* 
 * Get user defined phase instances for each flow defined in axis2.xml in an array list and add it 
 * into operation. This is called service builder, module builder and service client to add phases
 * into operations.
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phases_info_set_op_phases(
    axis2_phases_info_t * phases_info,
    const axutil_env_t * env,
    axis2_op_t * op_desc)
{
    axis2_status_t status = AXIS2_FAILURE;
    axutil_array_list_t *op_in_phases = NULL;
    axutil_array_list_t *op_out_phases = NULL;
    axutil_array_list_t *op_in_faultphases = NULL;
    axutil_array_list_t *op_out_faultphases = NULL;

    AXIS2_PARAM_CHECK(env->error, op_desc, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK(env->error, phases_info, AXIS2_FAILURE);

    op_in_phases = axis2_phases_info_get_op_in_phases(phases_info, env);
    if (!op_in_phases)
    {
        status = AXIS2_ERROR_GET_STATUS_CODE(env->error);
        /* op_in_phases cannot be NULL */
        return status;
    }

    op_out_phases = axis2_phases_info_get_op_out_phases(phases_info, env);
    if (!op_out_phases)
    {
        status = AXIS2_ERROR_GET_STATUS_CODE(env->error);
        /* op_out_phases cannot be NULL */
        return status;
    }

    op_in_faultphases = axis2_phases_info_get_op_in_faultphases(phases_info, env);
    op_out_faultphases = axis2_phases_info_get_op_out_faultphases(phases_info, env);

    status = axis2_op_set_in_flow(op_desc, env, op_in_phases);
    status = axis2_op_set_out_flow(op_desc, env, op_out_phases);
    if (op_in_faultphases)
    {
        status = axis2_op_set_fault_in_flow(op_desc, env, op_in_faultphases);
    }

    if (op_out_faultphases)
    {
        status = axis2_op_set_fault_out_flow(op_desc, env, op_out_faultphases);
    }

    return status;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_phases_info_copy_flow(
    const axutil_env_t * env,
    const axutil_array_list_t * flow_to_copy)
{
    int size = 0,
        i = 0;

    axutil_array_list_t *new_flow = NULL;
    if (flow_to_copy)
    {
        size =
            axutil_array_list_size((axutil_array_list_t *) flow_to_copy, env);
    }

    if (size > 0)
    {
        new_flow = axutil_array_list_create(env, 0);
        if (!new_flow)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }

    for (i = 0; i < size; i++)
    {
        void *item = axutil_array_list_get((axutil_array_list_t *) flow_to_copy, env, i);
        axis2_phase_increment_ref((axis2_phase_t *) item, env);
        axutil_array_list_add(new_flow, env, item);
    }

    return new_flow;
}

