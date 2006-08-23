/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2_phase_holder.h>

/**
 * @brief Wsdl phase holder struct impl
 * Phase Holder
 * This class hold all the phases found in the services.xml and server.xml
 */
typedef struct axis2_phase_holder_impl
{
    axis2_phase_holder_t phase_holder;
    axis2_array_list_t *phase_list;
}
axis2_phase_holder_impl_t;

#define AXIS2_INTF_TO_IMPL(phase_holder) ((axis2_phase_holder_impl_t *)phase_holder)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_phase_holder_free (
    axis2_phase_holder_t *phase_holder,
    const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_phase_holder_is_phase_exist(
    axis2_phase_holder_t *phase_holder,
    const axis2_env_t *env,
    axis2_char_t *phase_name);

axis2_status_t AXIS2_CALL
axis2_phase_holder_add_handler(
    axis2_phase_holder_t *phase_holder,
    const axis2_env_t *env,
    axis2_handler_desc_t *handler);

axis2_phase_t *AXIS2_CALL
axis2_phase_holder_get_phase(
    const axis2_phase_holder_t *phase_holder,
    const axis2_env_t *env,
    axis2_char_t *phase_name);

axis2_status_t AXIS2_CALL
axis2_phase_holder_build_transport_handler_chain(
    axis2_phase_holder_t *phase_holder,
    const axis2_env_t *env,
    axis2_phase_t *phase,
    axis2_array_list_t *handlers);

/************************** End of function prototypes ************************/

AXIS2_EXTERN axis2_phase_holder_t *AXIS2_CALL
axis2_phase_holder_create(
    const axis2_env_t *env)
{
    axis2_phase_holder_impl_t *phase_holder_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    phase_holder_impl = (axis2_phase_holder_impl_t *) AXIS2_MALLOC(env->
            allocator, sizeof(axis2_phase_holder_impl_t));

    if(NULL == phase_holder_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    phase_holder_impl->phase_list = NULL;
    phase_holder_impl->phase_holder.ops = NULL;

    phase_holder_impl->phase_holder.ops =
        AXIS2_MALLOC (env->allocator, sizeof(axis2_phase_holder_ops_t));
    if(NULL == phase_holder_impl->phase_holder.ops)
    {
        axis2_phase_holder_free(&(phase_holder_impl->phase_holder), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    phase_holder_impl->phase_holder.ops->free = axis2_phase_holder_free;
    phase_holder_impl->phase_holder.ops->is_phase_exist =
        axis2_phase_holder_is_phase_exist;
    phase_holder_impl->phase_holder.ops->add_handler =
        axis2_phase_holder_add_handler;
    phase_holder_impl->phase_holder.ops->get_phase =
        axis2_phase_holder_get_phase;
    phase_holder_impl->phase_holder.ops->build_transport_handler_chain =
        axis2_phase_holder_build_transport_handler_chain;

    return &(phase_holder_impl->phase_holder);
}

AXIS2_EXTERN axis2_phase_holder_t *AXIS2_CALL
axis2_phase_holder_create_with_phases(
    const axis2_env_t *env, 
    axis2_array_list_t *phases)
{
    axis2_phase_holder_impl_t *phase_holder_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!phases)
        return NULL;
    /*AXIS2_PARAM_CHECK(env->error, phases, NULL);*/

    phase_holder_impl = (axis2_phase_holder_impl_t *)axis2_phase_holder_create(env);

    phase_holder_impl->phase_list = phases;

    return &(phase_holder_impl->phase_holder);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_phase_holder_free(
    axis2_phase_holder_t *phase_holder,
    const axis2_env_t *env)
{
    axis2_phase_holder_impl_t *phase_holder_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    phase_holder_impl = AXIS2_INTF_TO_IMPL(phase_holder);

    /*if(NULL != phase_holder_impl->phase_list)
    {
        AXIS2_ARRAY_LIST_FREE(phase_holder_impl->phase_list, env);
        phase_holder_impl->phase_list = NULL;
    }*/

    if(NULL != phase_holder->ops)
    {
        AXIS2_FREE(env->allocator, phase_holder->ops);
        phase_holder->ops = NULL;
    }

    if(phase_holder_impl)
    {
        AXIS2_FREE(env->allocator, phase_holder_impl);
        phase_holder_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

/**
 * Method isPhaseExist
 *
 * @param phaseName
 * @return
 */
axis2_bool_t AXIS2_CALL
axis2_phase_holder_is_phase_exist(
    axis2_phase_holder_t *phase_holder,
    const axis2_env_t *env,
    axis2_char_t *phase_name)
{
    int size = 0;
    int i = 0;
    axis2_phase_t *phase = NULL;
    axis2_phase_holder_impl_t *phase_holder_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, phase_name, AXIS2_FALSE);
    phase_holder_impl = AXIS2_INTF_TO_IMPL(phase_holder);

    size = AXIS2_ARRAY_LIST_SIZE(phase_holder_impl->phase_list, env);

    for (i = 0; i < size; i++)
    {
        const axis2_char_t *phase_name_l = NULL;

        phase = (axis2_phase_t *) AXIS2_ARRAY_LIST_GET(phase_holder_impl->
                phase_list, env, i);
        phase_name_l = AXIS2_PHASE_GET_NAME(phase, env);
        if (0 == AXIS2_STRCMP(phase_name_l, phase_name))
        {
            return AXIS2_TRUE;
        }
    }
    return AXIS2_FALSE;
}

/**
 * Method addHandler
 *
 * @param handler
 */
axis2_status_t AXIS2_CALL
axis2_phase_holder_add_handler(
    axis2_phase_holder_t *phase_holder,
    const axis2_env_t *env,
    axis2_handler_desc_t *handler)
{
    axis2_char_t *phase_name = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "axis2_phase_holder_add_handler start");
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, handler, AXIS2_FAILURE);

    phase_name = AXIS2_PHASE_RULE_GET_NAME(
                AXIS2_HANDLER_DESC_GET_RULES(handler, env), env);
    if (AXIS2_TRUE == axis2_phase_holder_is_phase_exist(phase_holder, env,
            phase_name))
    {
        axis2_phase_t *phase = NULL;

        phase = axis2_phase_holder_get_phase(phase_holder, env, phase_name);
        status = AXIS2_PHASE_ADD_HANDLER_DESC(phase, env, handler);
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Add handler %s to phase %s",
                AXIS2_QNAME_TO_STRING(AXIS2_HANDLER_DESC_GET_QNAME(handler, env), env), phase_name);
    }
    else
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_PHASE, AXIS2_FAILURE);
        status = AXIS2_FAILURE;
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "axis2_phase_holder_add_handler end status = %s",
            status ? "SUCCESS" : "FAILURE");
    return status;
}

/**
 * this method is used to get the actual phase object given in the phase array list
 *
 * @param phaseName
 * @return
 */
axis2_phase_t *AXIS2_CALL
axis2_phase_holder_get_phase(
    const axis2_phase_holder_t *phase_holder,
    const axis2_env_t *env,
    axis2_char_t *phase_name)
{
    int size = 0;
    int i = 0;
    axis2_phase_holder_impl_t *phase_holder_impl = NULL;
    axis2_phase_t *phase = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, phase_name, NULL);

    phase_holder_impl = AXIS2_INTF_TO_IMPL(phase_holder);

    size = AXIS2_ARRAY_LIST_SIZE(phase_holder_impl->phase_list, env);

    for (i = 0; i < size; i++)
    {
        const axis2_char_t *phase_name_l = NULL;
        phase = (axis2_phase_t *) AXIS2_ARRAY_LIST_GET(phase_holder_impl->
                phase_list, env, i);
        phase_name_l = AXIS2_PHASE_GET_NAME(phase, env);
        if(0 == AXIS2_STRCMP(phase_name_l, phase_name))
        {
            return phase;
        }
    }

    return NULL;
}

/**
 * This method is to build the transport phase , here load the corresponding handlers and added them
 * in to correct phase
 *
 * @param phase
 * @param handlers
 */
axis2_status_t AXIS2_CALL
axis2_phase_holder_build_transport_handler_chain(
    axis2_phase_holder_t *phase_holder,
    const axis2_env_t *env,
    axis2_phase_t *phase,
    axis2_array_list_t *handlers)
{
    axis2_handler_t *handler = NULL;
    int size = 0;
    int status = AXIS2_FAILURE;
    int i = 0;
    axis2_handler_desc_t *handler_desc = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, phase, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, handlers, AXIS2_FAILURE);

    size = AXIS2_ARRAY_LIST_SIZE(handlers, env);

    for (i = 0; i < size; i++)
    {
        handler_desc = (axis2_handler_desc_t *) AXIS2_ARRAY_LIST_GET(
                    handlers, env, i);
        status = AXIS2_HANDLER_INIT(handler, env, handler_desc);
        if(AXIS2_FAILURE == status)
            return status;

        status = AXIS2_HANDLER_DESC_SET_HANDLER(handler_desc, env, handler);
        if(AXIS2_FAILURE == status)
            return status;

        status = AXIS2_PHASE_ADD_HANDLER(phase, env, handler);
    }
    return status;
}
