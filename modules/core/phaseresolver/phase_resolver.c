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

#include <axis2_phase_resolver.h>
#include <axis2_property.h>

typedef struct axis2_phase_resolver_impl
{
    axis2_phase_resolver_t phase_resolver;

    /** axis2 configuration */
    axis2_conf_t *axis2_config;

    /** service */
    axis2_svc_t *svc;
}
axis2_phase_resolver_impl_t;

#define AXIS2_INTF_TO_IMPL(phase_resolver) ((axis2_phase_resolver_impl_t *)phase_resolver)

axis2_status_t AXIS2_CALL
axis2_phase_resolver_free(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_chains(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_module_op(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_op_t *op);

/**
 * All the engaged modules defined in axis2.xml and services.xml which are
 * engaged to operation passed as parameter. The operation passed is a new
 * operation for which no modules are yet engaged. First all engaged modules
 * in axis2_conf are retrieved and all handlers in their flows are added to
 * operation flows. These modules are added to services and operations engaged
 * module list. Then all handlers from services flows are retrieved and
 * added to operation flows.
 * Type variable is used to differentiate chains
 * type = 1 inflow
 * type = 2 out flow
 * type = 3 fault in flow
 * type = 4 fault out flow
 */
static axis2_status_t
axis2_phase_resolver_build_execution_chains(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    int type,
    axis2_op_t *op);

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_transport_chains(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env);

static axis2_status_t
axis2_phase_resolver_build_in_transport_chains(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_transport_in_desc_t *transport);

static axis2_status_t
axis2_phase_resolver_build_out_transport_chains(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_transport_out_desc_t *transport);

axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_globally(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_module_desc_t *module);

axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_svc_from_global(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_svc_t *svc,
    axis2_module_desc_t *module_desc);

static axis2_status_t
axis2_phase_resolver_engage_to_global_chain(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_module_desc_t *module_desc);

axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_svc(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_svc_t *svc,
    axis2_module_desc_t *module_desc);

axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_op(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_op_t *axis_op,
    axis2_module_desc_t *module_desc);

AXIS2_EXTERN axis2_phase_resolver_t *AXIS2_CALL
axis2_phase_resolver_create(
    const axis2_env_t *env)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    resolver_impl = (axis2_phase_resolver_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_phase_resolver_impl_t));

    if (NULL == resolver_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    resolver_impl->phase_resolver.ops = NULL;
    resolver_impl->axis2_config = NULL;
    resolver_impl->svc = NULL;

    resolver_impl->phase_resolver.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_phase_resolver_ops_t));
    if (NULL == resolver_impl->phase_resolver.ops)
    {
        axis2_phase_resolver_free(&(resolver_impl->phase_resolver), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    resolver_impl->phase_resolver.ops->free =  axis2_phase_resolver_free;

    resolver_impl->phase_resolver.ops->build_chains =
        axis2_phase_resolver_build_chains;

    resolver_impl->phase_resolver.ops->build_module_op =
        axis2_phase_resolver_build_module_op;

    resolver_impl->phase_resolver.ops->build_transport_chains =
        axis2_phase_resolver_build_transport_chains;

    resolver_impl->phase_resolver.ops->engage_module_globally =
        axis2_phase_resolver_engage_module_globally;

    resolver_impl->phase_resolver.ops->engage_module_to_svc_from_global =
        axis2_phase_resolver_engage_module_to_svc_from_global;

    resolver_impl->phase_resolver.ops->engage_module_to_svc =
        axis2_phase_resolver_engage_module_to_svc;

    resolver_impl->phase_resolver.ops->engage_module_to_op =
        axis2_phase_resolver_engage_module_to_op;


    return &(resolver_impl->phase_resolver);
}

AXIS2_EXTERN axis2_phase_resolver_t *AXIS2_CALL
axis2_phase_resolver_create_with_config(
    const axis2_env_t *env,
    axis2_conf_t *axis2_config)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, axis2_config, NULL);

    resolver_impl = (axis2_phase_resolver_impl_t *)
            axis2_phase_resolver_create(env);

    resolver_impl->axis2_config = axis2_config;

    return &(resolver_impl->phase_resolver);
}

AXIS2_EXTERN axis2_phase_resolver_t *AXIS2_CALL
axis2_phase_resolver_create_with_config_and_svc(
    const axis2_env_t *env,
    axis2_conf_t *axis2_config,
    axis2_svc_t *svc)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, axis2_config, NULL);

    resolver_impl = (axis2_phase_resolver_impl_t *)
            axis2_phase_resolver_create(env);

    if (!resolver_impl)
    {
        return NULL;
    }
    resolver_impl->axis2_config = axis2_config;

    resolver_impl->svc = svc;
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "svc name is:%s",
            AXIS2_SVC_GET_NAME(resolver_impl->svc, env));

    return &(resolver_impl->phase_resolver);
}

axis2_status_t AXIS2_CALL
axis2_phase_resolver_free(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);

    resolver_impl->axis2_config = NULL;

    resolver_impl->svc = NULL;

    if (phase_resolver->ops)
    {
        AXIS2_FREE(env->allocator, phase_resolver->ops);
        phase_resolver->ops = NULL;
    }

    if (resolver_impl)
    {
        AXIS2_FREE(env->allocator, resolver_impl);
        resolver_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_chains(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_hash_index_t *index_i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_op_t *op = NULL;
    axis2_hash_t *ops = NULL;

    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);

    if (!(resolver_impl->svc))
        return AXIS2_FAILURE;

    ops = AXIS2_SVC_GET_ALL_OPS(resolver_impl->svc, env);

    for (index_i = axis2_hash_first(ops, env); index_i; index_i =
                axis2_hash_next(env, index_i))
    {
        void *v = NULL;
        int j = 0;
        axis2_hash_this(index_i, NULL, NULL, &v);
        op = (axis2_op_t *)v;
        for (j = 1; j < 5; j++)
        {
            status = axis2_phase_resolver_build_execution_chains(phase_resolver,
                    env, j, op);
        }
    }

    return status;
}

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_module_op(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_op_t *op)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    int i = 0;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);


    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "op name is:%s",
            AXIS2_QNAME_GET_LOCALPART(axis2_op_get_qname(op, env),
                    env));
    for (i = 1; i < 5; i++)
    {
        status = axis2_phase_resolver_build_execution_chains(phase_resolver,
                env, i, op);
        if(!status)
            break;
    }
    return status;
}

static axis2_status_t
axis2_phase_resolver_build_execution_chains(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    int type,
    axis2_op_t *op)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_array_list_t *all_handlers = NULL;
    axis2_array_list_t *moduleqnames = NULL;
    int i = 0;
    int size = 0;
    int status = AXIS2_FAILURE;
    axis2_flow_t *flow = NULL;
    /*axis2_property_t *property = NULL;*/
    axis2_phase_holder_t *phase_holder = NULL;

    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);

    /* engage handlers from axis2.xml from modules */

    moduleqnames = AXIS2_CONF_GET_ALL_ENGAGED_MODULES(
                resolver_impl->axis2_config, env);

    size = axis2_array_list_size(moduleqnames, env);
    status = AXIS2_ERROR_GET_STATUS_CODE(env->error);
    if (AXIS2_SUCCESS != status)
    {
        return status;
    }

    for (i = 0; i < size; i++)
    {
        axis2_qname_t *modulename = NULL;
        axis2_module_desc_t *module_desc = NULL;

        modulename = (axis2_qname_t *) axis2_array_list_get(moduleqnames, env,
                i);
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "module name is:%s",
                AXIS2_QNAME_GET_LOCALPART(modulename, env));
        module_desc = AXIS2_CONF_GET_MODULE(resolver_impl->axis2_config, env,
                modulename);
        if (module_desc)
        {
            switch (type)
            {
                case AXIS2_IN_FLOW:
                {
                    flow = axis2_module_desc_get_in_flow(module_desc, env);
                    break;
                }
                case AXIS2_OUT_FLOW:
                {
                    flow = axis2_module_desc_get_out_flow(module_desc, env);
                    break;
                }
                case AXIS2_FAULT_IN_FLOW:
                {
                    flow = axis2_module_desc_get_fault_in_flow(module_desc, env);
                    break;
                }
                case AXIS2_FAULT_OUT_FLOW:
                {
                    flow = axis2_module_desc_get_fault_out_flow(module_desc, env);
                    break;
                }
            }
            /*status = AXIS2_SVC_ADD_TO_ENGAGED_MODULE_LIST(resolver_impl->svc,
                    env, module_desc);
            if (AXIS2_SUCCESS != status)
            {
                return status;
            }*/
            status = axis2_op_add_to_engaged_module_list(op, env, module_desc);
            if (AXIS2_SUCCESS != status)
            {
                return status;
            }

        }
        else
        {

            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_MODULE_REF,
                    AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }

        if (flow)
        {
            int j = 0;
            int count = 0;
            count = axis2_flow_get_handler_count(flow, env);
            if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
            {
                return AXIS2_ERROR_GET_STATUS_CODE(env->error);
            }


            for (j = 0; j < count; j++)
            {
                axis2_handler_desc_t *metadata = NULL;
                const axis2_char_t *phase_name = NULL;
                axis2_phase_rule_t *phase_rule = NULL;

                metadata = axis2_flow_get_handler(flow, env, j);
                phase_rule = axis2_handler_desc_get_rules(metadata, env);
                phase_name = axis2_phase_rule_get_name(phase_rule, env);
                if (NULL == phase_name)
                {
                    return AXIS2_FAILURE;
                }
                if ((0 != AXIS2_STRCMP(AXIS2_PHASE_TRANSPORT_IN, phase_name)) &&
                        (0 != AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name)) &&
                        (0 != AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) &&
                        (0 != AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                {
                    if (!all_handlers)
                    {
                        all_handlers = axis2_array_list_create(env, 0);
                        if (!all_handlers)
                            return AXIS2_FAILURE;
                    }
                    status = axis2_array_list_add(all_handlers, env, metadata);
                    if (AXIS2_SUCCESS != status)
                    {
                        if (all_handlers)
                        {
                            axis2_array_list_free(all_handlers, env);
                            all_handlers = NULL;
                        }
                        return status;
                    }
                }
                else
                {
                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "This handler is\
                            trying to added to system pre defined phases , but those\
                            handlers are already added to global chain which run\
                            irrespective of the service");
                }
            }
        }

    }


    /* process handlers form service.xml */
    flow = NULL;
    /*switch (type)
    {

        case AXIS2_IN_FLOW:
        {

            property = (axis2_property_t *)
                    AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(resolver_impl->
                            svc->wsdl_svc->wsdl_component, env, AXIS2_IN_FLOW_KEY);
            if (property)
                flow = (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
            break;
        }
        case AXIS2_OUT_FLOW:
        {
            property = (axis2_property_t *)
                    AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(resolver_impl->
                            svc->wsdl_svc->wsdl_component, env, AXIS2_OUT_FLOW_KEY);
            if (property)
                flow = (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
            break;
        }
        case AXIS2_FAULT_IN_FLOW:
        {
            property = (axis2_property_t *)
                    AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(resolver_impl->
                            svc->wsdl_svc->wsdl_component, env, AXIS2_IN_FAULTFLOW_KEY);
            if (property)
                flow = (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
            break;
        }
        case AXIS2_FAULT_OUT_FLOW:
        {
            property = (axis2_property_t *)
                    AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(resolver_impl->
                            svc->wsdl_svc->wsdl_component, env, AXIS2_OUT_FAULTFLOW_KEY);
            if (property)
                flow = (axis2_flow_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
            break;
        }
    }*/

    if (flow)
    {
        int j = 0;
        int count = 0;
        count = axis2_flow_get_handler_count(flow, env);

        if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
        {
            if (all_handlers)
            {
                axis2_array_list_free(all_handlers, env);
                all_handlers = NULL;
            }
            return AXIS2_ERROR_GET_STATUS_CODE(env->error);
        }
        for (j = 0; j < count; j++)
        {
            axis2_handler_desc_t *metadata = NULL;
            const axis2_char_t *phase_name = NULL;
            axis2_phase_rule_t *phase_rule = NULL;

            metadata = axis2_flow_get_handler(flow, env, j);
            phase_rule = axis2_handler_desc_get_rules(metadata, env);
            if (phase_rule)
            {
                phase_name = axis2_phase_rule_get_name(phase_rule, env);
            }

            /* TODO change this in proper way */
            if (!phase_name || (0 == AXIS2_STRCMP(phase_name, "")))
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_PHASE_IS_NOT_SPECIFED,
                        AXIS2_FAILURE);
                return AXIS2_FAILURE;

            }
            else if ((0 == AXIS2_STRCMP(AXIS2_PHASE_TRANSPORT_IN, phase_name)) ||
                    (0 == AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name)) ||
                    (0 == AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) ||
                    (0 == AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
            {
                if (all_handlers)
                {
                    axis2_array_list_free(all_handlers, env);
                    all_handlers = NULL;
                }
                AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_SERVICE_MODULE_CAN_NOT_REFER_GLOBAL_PHASE, AXIS2_FAILURE);
                return AXIS2_FAILURE;

            }
            else
            {
                if (!all_handlers)
                {
                    all_handlers = axis2_array_list_create(env, 0);
                    if (!all_handlers)
                        return AXIS2_FAILURE;
                }

                status = axis2_array_list_add(all_handlers, env, metadata);

                if (AXIS2_SUCCESS != status)
                {
                    if (all_handlers)
                    {
                        int size = 0;
                        int i = 0;
                        size = axis2_array_list_size(all_handlers, env);
                        for (i = 0; i < size; i++)
                        {
                            axis2_handler_desc_t *handler_desc = NULL;

                            handler_desc = axis2_array_list_get(all_handlers,
                                    env, i);
                            axis2_handler_desc_free(handler_desc, env);
                        }
                        axis2_array_list_free(all_handlers, env);
                        all_handlers = NULL;
                    }
                    return status;
                }
            }
        }
    }

    else if (!all_handlers)
    {
        /* no flows configured */
        return AXIS2_SUCCESS;
    }

    switch (type)
    {
        case AXIS2_IN_FLOW:
        {
            axis2_array_list_t *phase_list = NULL;

            phase_list = axis2_op_get_in_flow(op, env);
            phase_holder =
                axis2_phase_holder_create_with_phases(env, phase_list);
            break;
        }
        case AXIS2_OUT_FLOW:
        {
            axis2_array_list_t *phase_list = NULL;

            phase_list = axis2_op_get_out_flow(op, env);
            phase_holder =
                axis2_phase_holder_create_with_phases(env, phase_list);
            break;
        }
        case AXIS2_FAULT_IN_FLOW:
        {
            axis2_array_list_t *phase_list = NULL;

            phase_list = axis2_op_get_fault_in_flow(op, env);
            phase_holder =
                axis2_phase_holder_create_with_phases(env, phase_list);
            break;
        }
        case AXIS2_FAULT_OUT_FLOW:
        {
            axis2_array_list_t *phase_list = NULL;

            phase_list = axis2_op_get_fault_out_flow(op, env);
            phase_holder =
                axis2_phase_holder_create_with_phases(env, phase_list);
            break;
        }
    }

    size = axis2_array_list_size(all_handlers, env);
    if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
    {
        return AXIS2_ERROR_GET_STATUS_CODE(env->error);
    }

    for (i = 0; i < size; i++)
    {
        axis2_handler_desc_t *metadata = NULL;

        metadata = (axis2_handler_desc_t *)
                axis2_array_list_get(all_handlers, env, i);
        if (phase_holder)
        {
            status = AXIS2_PHASE_HOLDER_ADD_HANDLER(phase_holder,
                    env, metadata);
            if(!status)
                break;
        }
    }

    /* Free the locally created all_handlers list */
    if (all_handlers)
        axis2_array_list_free(all_handlers, env);
    if(phase_holder)
        AXIS2_PHASE_HOLDER_FREE(phase_holder, env);

    return status;
}

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_transport_chains(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_transport_in_desc_t **transports_in = NULL;
    axis2_transport_out_desc_t **transports_out = NULL;
    int index_i = 0;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);

    transports_in = AXIS2_CONF_GET_ALL_IN_TRANSPORTS(resolver_impl->axis2_config,
            env);
    if (!transports_in)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_TRANSPORT_IN_CONFIGURED,
                AXIS2_FAILURE);
        return AXIS2_SUCCESS;
    }

    transports_out = AXIS2_CONF_GET_ALL_OUT_TRANSPORTS(resolver_impl->axis2_config,
            env);
    if (!transports_out)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_TRANSPORT_OUT_CONFIGURED,
                AXIS2_FAILURE);
        return AXIS2_SUCCESS;
    }

    for (index_i = 0; index_i < AXIS2_TRANSPORT_ENUM_MAX; index_i++)
    {
        if (transports_in[index_i])
        {
            status = axis2_phase_resolver_build_in_transport_chains(phase_resolver,
                env, transports_in[index_i]);
            if (AXIS2_SUCCESS != status)
            {
                return status;
            }
        }
    }

    for (index_i = 0; index_i < AXIS2_TRANSPORT_ENUM_MAX; index_i++)
    {
        if (transports_out[index_i])
        {
            status = axis2_phase_resolver_build_out_transport_chains(phase_resolver,
                env, transports_out[index_i]);
            if (AXIS2_SUCCESS != status)
            {
                return status;
            }
        }
    }

    /* If transport in or transport out maps are not null but still they don't
     * have chains configured then we return success, because there are no
     * chain to process
     */
    return AXIS2_SUCCESS;
}


static axis2_status_t
axis2_phase_resolver_build_in_transport_chains(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_transport_in_desc_t *transport)
{
    int type = 0;
    int j = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_array_list_t *handlers = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, transport, AXIS2_FAILURE);

    for (type = 1; type < 4; type++)
    {
        axis2_flow_t *flow = NULL;
        axis2_phase_t *phase = NULL;

        switch (type)
        {
            case AXIS2_IN_FLOW:
            {
                flow = AXIS2_TRANSPORT_IN_DESC_GET_IN_FLOW(transport, env);
                phase = AXIS2_TRANSPORT_IN_DESC_GET_IN_PHASE(transport, env);
                break;
            }
            case AXIS2_FAULT_IN_FLOW:
            {
                flow = AXIS2_TRANSPORT_IN_DESC_GET_FAULT_IN_FLOW(transport, env);
                phase = AXIS2_TRANSPORT_IN_DESC_GET_FAULT_PHASE(transport, env);
                break;
            }
        }
        if (flow)
        {
            axis2_phase_holder_t *phase_holder = NULL;
            int size = 0;

            size = axis2_flow_get_handler_count(flow, env);
            handlers = axis2_array_list_create(env, 0);
            for (j = 0; j < size; j++)
            {
                axis2_handler_desc_t *metadata = NULL;
                axis2_phase_rule_t *rule = NULL;

                metadata = axis2_flow_get_handler(flow, env, j);
                rule = axis2_handler_desc_get_rules(metadata, env);
                if (rule)
                {
                    status = axis2_phase_rule_set_name(rule, env,
                            AXIS2_TRANSPORT_PHASE);
                }
                if (AXIS2_SUCCESS != status)
                {
                    if (handlers)
                    {
                        axis2_handler_desc_t *handler_d = NULL;
                        int i = 0;
                        int size = 0;

                        size = axis2_array_list_size(handlers, env);
                        for (i = 0; i < size; i++)
                        {
                            handler_d = axis2_array_list_get(handlers, env, i);
                            axis2_handler_desc_free(handler_d, env);
                        }
                        axis2_array_list_free(handlers, env);
                    }
                    return status;
                }
                status = axis2_array_list_add(handlers, env, metadata);
                if (AXIS2_SUCCESS != status)
                {
                    if (handlers)
                    {
                        axis2_handler_desc_t *handler_d = NULL;
                        int i = 0;
                        int size = 0;

                        size = axis2_array_list_size(handlers, env);
                        for (i = 0; i < size; i++)
                        {
                            handler_d = axis2_array_list_get(handlers, env, i);
                            axis2_handler_desc_free(handler_d, env);
                        }
                        axis2_array_list_free(handlers, env);
                    }
                    return status;
                }
            }
            phase_holder = axis2_phase_holder_create(env);
            if (!phase_holder)
            {
                if (handlers)
                {
                    axis2_handler_desc_t *handler_d = NULL;
                    int i = 0;
                    int size = 0;

                    size = axis2_array_list_size(handlers, env);
                    for (i = 0; i < size; i++)
                    {
                        handler_d = axis2_array_list_get(handlers, env, i);
                        axis2_handler_desc_free(handler_d, env);
                    }
                    axis2_array_list_free(handlers, env);
                }
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                        AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }

            status = AXIS2_PHASE_HOLDER_BUILD_TRANSPORT_HANDLER_CHAIN(
                        phase_holder, env, phase, handlers);
            if (phase_holder)
            {
                AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
            }
        }
        else
        {
            /* Do nothing */
        }
    }
    if (handlers)
    {
        axis2_array_list_free(handlers, env);
    }
    return status;
}

static axis2_status_t
axis2_phase_resolver_build_out_transport_chains(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_transport_out_desc_t *transport)
{
    int type = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_array_list_t *handlers = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, transport, AXIS2_FAILURE);
    for (type = 1; type < 5; type++)
    {
        axis2_flow_t *flow = NULL;
        axis2_phase_t *phase = NULL;

        switch (type)
        {
            case AXIS2_OUT_FLOW:
            {
                flow = AXIS2_TRANSPORT_OUT_DESC_GET_OUT_FLOW(transport, env);
                phase = AXIS2_TRANSPORT_OUT_DESC_GET_OUT_PHASE(transport, env);
                break;
            }
            case AXIS2_FAULT_OUT_FLOW:
            {
                flow = AXIS2_TRANSPORT_OUT_DESC_GET_FAULT_OUT_FLOW(transport, env);
                phase = AXIS2_TRANSPORT_OUT_DESC_GET_FAULT_PHASE(transport, env);
                break;
            }
        }

        if (flow)
        {
            axis2_phase_holder_t *phase_holder = NULL;
            int hndlr_count = 0;
            int j = 0;
            hndlr_count = axis2_flow_get_handler_count(flow, env);
            if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
            {
                return AXIS2_ERROR_GET_STATUS_CODE(env->error);
            }
            handlers = axis2_array_list_create(env, 0);

            for (j = 0; j < hndlr_count; j++)
            {
                axis2_handler_desc_t *metadata = NULL;
                axis2_phase_rule_t *rule = NULL;

                metadata = axis2_flow_get_handler(flow, env, j);


                rule = axis2_handler_desc_get_rules(metadata, env);
                if (rule)
                {
                    status = axis2_phase_rule_set_name(rule, env,
                            AXIS2_TRANSPORT_PHASE);
                }
                if (AXIS2_SUCCESS != status)
                {
                    if (handlers)
                    {
                        axis2_handler_desc_t *handler_d = NULL;
                        int i = 0;
                        int size = 0;

                        size = axis2_array_list_size(handlers, env);
                        for (i = 0; i < size; i++)
                        {
                            handler_d = axis2_array_list_get(handlers, env, i);
                            axis2_handler_desc_free(handler_d, env);
                        }
                        axis2_array_list_free(handlers, env);
                    }
                    return status;
                }

                status = axis2_array_list_add(handlers, env, metadata);
                if (AXIS2_FAILURE == status)
                {
                    if (handlers)
                    {
                        axis2_handler_desc_t *handler_d = NULL;
                        int i = 0;
                        int size = 0;

                        size = axis2_array_list_size(handlers, env);
                        for (i = 0; i < size; i++)
                        {
                            handler_d = axis2_array_list_get(handlers, env, i);
                            axis2_handler_desc_free(handler_d, env);
                        }
                        axis2_array_list_free(handlers, env);
                    }
                    return status;
                }
            }
            phase_holder = axis2_phase_holder_create(env);
            if (!phase_holder)
            {
                if (handlers)
                {
                    axis2_handler_desc_t *handler_d = NULL;
                    int i = 0;
                    int size = 0;

                    size = axis2_array_list_size(handlers, env);
                    for (i = 0; i < size; i++)
                    {
                        handler_d = axis2_array_list_get(handlers, env, i);
                        axis2_handler_desc_free(handler_d, env);
                    }
                    axis2_array_list_free(handlers, env);
                }
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }

            status = AXIS2_PHASE_HOLDER_BUILD_TRANSPORT_HANDLER_CHAIN(
                        phase_holder, env, phase, handlers);
            if (phase_holder)
                AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
        }
        else
        {
            /* Do nothing */
        }
    }
    if (handlers)
        axis2_array_list_free(handlers, env);
    return status;
}

axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_globally(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_module_desc_t *module_desc)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_hash_t *svc_grps = NULL;
    axis2_hash_index_t *index_i = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);

    status = axis2_phase_resolver_engage_to_global_chain(phase_resolver, env,
            module_desc);
    if (AXIS2_SUCCESS != status)
    {
        return status;
    }
    svc_grps = AXIS2_CONF_GET_ALL_SVC_GRPS(resolver_impl->axis2_config, env);
    if (!svc_grps)
    {
        return AXIS2_FAILURE;
    }
    for (index_i = axis2_hash_first(svc_grps, env); index_i; index_i =
                axis2_hash_next(env, index_i))
    {
        axis2_hash_t *svcs = NULL;
        axis2_svc_grp_t *svc_grp = NULL;
        void *v = NULL;
        axis2_hash_index_t *index_j = NULL;
        const axis2_qname_t *mod_name = NULL;

        axis2_hash_this(index_i, NULL, NULL, &v);
        svc_grp = (axis2_svc_grp_t *) v;
        svcs = AXIS2_SVC_GRP_GET_ALL_SVCS(svc_grp, env);

        for (index_j = axis2_hash_first(svcs, env); index_j; index_j =
                    axis2_hash_next(env, index_j))
        {
            axis2_svc_t *svc = NULL;
            void *w = NULL;

            axis2_hash_this(index_j, NULL, NULL, &w);
            svc = (axis2_svc_t *) w;
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "svc name is:%s",
                    AXIS2_SVC_GET_NAME(svc, env));

            status = AXIS2_SVC_ADD_MODULE_OPS(svc, env, module_desc,
                    resolver_impl->axis2_config);
            if (AXIS2_SUCCESS != status)
            {
                return status;
            }
            status = axis2_phase_resolver_engage_module_to_svc_from_global(
                        phase_resolver, env, svc, module_desc);

            if (AXIS2_SUCCESS != status)
            {
                return status;
            }

            /*status = AXIS2_SVC_ADD_TO_ENGAGED_MODULE_LIST(svc, env, module_desc);
            if (AXIS2_SUCCESS != status)
            {
                return status;
            }*/
        }
        mod_name = axis2_module_desc_get_qname(module_desc, env);
        status = AXIS2_SVC_GRP_ADD_MODULE_QNAME(svc_grp, env,
                mod_name);

        if (AXIS2_SUCCESS != status)
        {
            return status;
        }
    }
    return status;
}

axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_svc_from_global(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_svc_t *svc,
    axis2_module_desc_t *module_desc)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_hash_t *ops = NULL;
    axis2_bool_t engaged = AXIS2_FALSE;
    axis2_hash_index_t *index_i = NULL;
    int type = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_phase_holder_t *phase_holder = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);

    ops = AXIS2_SVC_GET_ALL_OPS(svc, env);
    if (!ops)
    {
        return AXIS2_FAILURE;
    }

    for (index_i = axis2_hash_first(ops, env); index_i;
            index_i = axis2_hash_next(env, index_i))
    {
        void *v = NULL;
        axis2_op_t *op_desc = NULL;
        int j = 0;
        axis2_array_list_t *modules = NULL;
        axis2_flow_t *flow = NULL;
        const axis2_qname_t *module_desc_qname = NULL;
        int size = 0;

        axis2_hash_this(index_i, NULL, NULL, &v);
        op_desc = (axis2_op_t *) v;
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "op name is:%s",
                AXIS2_QNAME_GET_LOCALPART(axis2_op_get_qname(op_desc, env), env));
        modules = axis2_op_get_all_modules(op_desc, env);
        module_desc_qname = axis2_module_desc_get_qname(module_desc, env);
        if (modules)
            size = axis2_array_list_size(modules, env);
        for (j = 0; j < size; j++)
        {
            axis2_module_desc_t *module_desc_l = NULL;
            const axis2_qname_t *module_desc_qname_l = NULL;

            module_desc_l = (axis2_module_desc_t *)
                    axis2_array_list_get(modules, env, j);

            module_desc_qname_l = axis2_module_desc_get_qname(module_desc_l ,
                    env);
            if (AXIS2_QNAME_EQUALS(module_desc_qname_l, env, module_desc_qname))
            {
                engaged = AXIS2_TRUE;
                break;
            }
        }
        
        if (AXIS2_TRUE == engaged)
        {
            continue;
        }
        
        for (type = 1; type < 5; type++)
        {
            switch (type)
            {
                case AXIS2_IN_FLOW:
                {
                    axis2_array_list_t *phase_list = NULL;

                    phase_list = axis2_op_get_in_flow(op_desc, env);
                    phase_holder =
                        axis2_phase_holder_create_with_phases(env, phase_list);
                    break;
                }
                case AXIS2_OUT_FLOW:
                {
                    axis2_array_list_t *phase_list = NULL;

                    phase_list = axis2_op_get_out_flow(op_desc, env);
                    phase_holder =
                        axis2_phase_holder_create_with_phases(env, phase_list);
                    break;
                }
                case AXIS2_FAULT_IN_FLOW:
                {
                    axis2_array_list_t *phase_list = NULL;

                    phase_list = axis2_op_get_fault_in_flow(op_desc, env);
                    phase_holder =
                        axis2_phase_holder_create_with_phases(env, phase_list);
                    break;
                }
                case AXIS2_FAULT_OUT_FLOW:
                {
                    axis2_array_list_t *phase_list = NULL;

                    phase_list = axis2_op_get_fault_out_flow(op_desc, env);
                    phase_holder =
                        axis2_phase_holder_create_with_phases(env, phase_list);
                    break;
                }
            }

            /* process modules referred by axis2.xml */

            switch (type)
            {
                case AXIS2_IN_FLOW:
                {
                    flow = axis2_module_desc_get_in_flow(module_desc, env);
                    break;
                }
                case AXIS2_OUT_FLOW:
                {
                    flow = axis2_module_desc_get_out_flow(module_desc, env);
                    break;
                }
                case AXIS2_FAULT_IN_FLOW:
                {
                    flow = axis2_module_desc_get_fault_in_flow(module_desc, env);
                    break;
                }
                case AXIS2_FAULT_OUT_FLOW:
                {
                    flow = axis2_module_desc_get_fault_out_flow(module_desc, env);
                    break;
                }
            }
            if (flow)
            {
                int handler_count = 0;

                handler_count  = axis2_flow_get_handler_count(flow, env);
                for (j = 0; j < handler_count; j++)
                {
                    axis2_handler_desc_t *metadata = NULL;
                    const axis2_char_t *phase_name = NULL;
                    axis2_phase_rule_t *phase_rule = NULL;

                    metadata = axis2_flow_get_handler(flow, env, j);
                    phase_rule = axis2_handler_desc_get_rules(metadata, env);
                    if (phase_rule)
                    {
                        phase_name = axis2_phase_rule_get_name(phase_rule, env);
                    }
                    if (!phase_name)
                    {
                        return AXIS2_FAILURE;
                    }
                    if ((0 != AXIS2_STRCMP(AXIS2_PHASE_TRANSPORT_IN, phase_name)) &&
                            (0 != AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name)) &&
                            (0 != AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) &&
                            (0 != AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                    {
                        if (phase_holder)
                        {
                            status = AXIS2_PHASE_HOLDER_ADD_HANDLER(
                                        phase_holder, env, metadata);
                            if(!status)
                            {
                                AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
                                return status;
                            }
                        }
                    }
                }
            }
            
            if(phase_holder)
                AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
        }
        status = axis2_op_add_to_engaged_module_list(op_desc, env, module_desc);
        if (AXIS2_SUCCESS != status)
        {
            return status;
        }
    }

    return AXIS2_SUCCESS;
}


static axis2_status_t
axis2_phase_resolver_engage_to_global_chain(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_module_desc_t *module_desc)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    int type = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_phase_holder_t *phase_holder = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);

    for (type = 1; type < 5; type++)
    {
        axis2_flow_t *flow = NULL;

        switch (type)
        {
            case AXIS2_IN_FLOW:
            {
                axis2_array_list_t *phase_list = NULL;

                phase_list =
                    AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(
                        resolver_impl->axis2_config, env);
                phase_holder =
                    axis2_phase_holder_create_with_phases(env, phase_list);
                if (!phase_holder) continue;
                break;
            }
            case AXIS2_OUT_FLOW:
            {
                axis2_array_list_t *phase_list = NULL;

                phase_list = AXIS2_CONF_GET_OUT_FLOW(resolver_impl->axis2_config,
                        env);
                phase_holder =
                    axis2_phase_holder_create_with_phases(env, phase_list);
                if (!phase_holder) continue;
                break;
            }
            case AXIS2_FAULT_IN_FLOW:
            {
                axis2_array_list_t *phase_list = NULL;

                phase_list = AXIS2_CONF_GET_IN_FAULT_FLOW(resolver_impl->
                        axis2_config, env);
                phase_holder =
                    axis2_phase_holder_create_with_phases(env, phase_list);
                if (!phase_holder) continue;
                break;
            }
            case AXIS2_FAULT_OUT_FLOW:
            {
                axis2_array_list_t *phase_list = NULL;

                phase_list = AXIS2_CONF_GET_OUT_FAULT_FLOW(resolver_impl->
                        axis2_config, env);
                phase_holder =
                    axis2_phase_holder_create_with_phases(env, phase_list);
                if (!phase_holder) continue;
                break;
            }
        }

        /* modules referred by axis2.xml */

        switch (type)
        {
            case AXIS2_IN_FLOW:
            {
                flow = axis2_module_desc_get_in_flow(module_desc, env);
                break;
            }
            case AXIS2_OUT_FLOW:
            {
                flow = axis2_module_desc_get_out_flow(module_desc, env);
                break;
            }
            case AXIS2_FAULT_IN_FLOW:
            {
                flow = axis2_module_desc_get_fault_in_flow(module_desc, env);
                break;
            }
            case AXIS2_FAULT_OUT_FLOW:
            {
                flow = axis2_module_desc_get_fault_out_flow(module_desc, env);
                break;
            }
        }
        if (flow)
        {
            int j = 0;
            for (j = 0; j < axis2_flow_get_handler_count(flow, env); j++)
            {
                axis2_handler_desc_t *metadata = NULL;
                const axis2_char_t *phase_name = NULL;
                axis2_phase_rule_t *phase_rule = NULL;

                metadata = axis2_flow_get_handler(flow, env, j);
                phase_rule = axis2_handler_desc_get_rules(metadata, env);
                if (phase_rule)
                {
                    phase_name = axis2_phase_rule_get_name(phase_rule, env);
                }
                if (!phase_name)
                {
                    return AXIS2_FAILURE;
                }
                if ((0 == AXIS2_STRCMP(AXIS2_PHASE_TRANSPORT_IN, phase_name)) ||
                        (0 == AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name)) ||
                        (0 == AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) ||
                        (0 == AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                {
                    status = AXIS2_PHASE_HOLDER_ADD_HANDLER(phase_holder, env, 
                            metadata);
                    if (!status)
                    {
                        AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
                        return status;
                    }

                }
                /*else
                {
                     * These handlers will go to op's handler chains , since 
                     * the module try to add handlers to both system predefined
                     * phases and user defined phase so global module can do 
                     * that. here the global module are the module which are
                     * referred by axis2.xml
                }*/
            }
        }
    
        if(phase_holder)    
            AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_svc(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_svc_t *svc,
    axis2_module_desc_t *module_desc)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_hash_t *ops = NULL;
    axis2_hash_index_t *index_i = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    const axis2_qname_t *module_d_qname = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);

    ops = AXIS2_SVC_GET_ALL_OPS(svc, env);
    if (!ops)
    {
        return AXIS2_FAILURE;
    }
    status = AXIS2_SVC_ADD_MODULE_OPS(svc, env, module_desc,
            resolver_impl->axis2_config);

    if (AXIS2_SUCCESS != status)
    {
        return status;
    }
    module_d_qname = axis2_module_desc_get_qname(module_desc, env);
    for (index_i = axis2_hash_first(ops, env); index_i; index_i =
                axis2_hash_next(env, index_i))
    {
        axis2_array_list_t *modules = NULL;
        axis2_op_t *op_desc = NULL;
        int size = 0;
        int j = 0;
        void *v = NULL;
        axis2_bool_t engaged = AXIS2_FALSE;

        axis2_hash_this(index_i, NULL, NULL, &v);
        op_desc = (axis2_op_t *) v;
        modules = axis2_op_get_all_modules(op_desc, env);
        if (modules)
            size = axis2_array_list_size(modules, env);
        for (j = 0; j < size; j++)
        {
            axis2_module_desc_t *module_desc_l = NULL;
            const axis2_qname_t *module_d_qname_l = NULL;

            module_desc_l = axis2_array_list_get(modules, env, j);
            module_d_qname_l = axis2_module_desc_get_qname(module_desc_l, env);
            if (AXIS2_TRUE == AXIS2_QNAME_EQUALS(module_d_qname, env,
                    module_d_qname_l))
            {
                engaged = AXIS2_TRUE;
                break;
            }
        }

        if (AXIS2_FALSE == engaged)
        {
            status = axis2_phase_resolver_engage_module_to_op(
                        phase_resolver, env, op_desc, module_desc);
            if (AXIS2_SUCCESS != status)
            {
                return status;
            }

            status = axis2_op_add_to_engaged_module_list(op_desc, env,
                    module_desc);
        }

    }
    return status;
}


axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_op(
    axis2_phase_resolver_t *phase_resolver,
    const axis2_env_t *env,
    axis2_op_t *axis_op,
    axis2_module_desc_t *module_desc)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    int type = 0;
    axis2_phase_holder_t *phase_holder = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, axis_op, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);

    for (type = 1; type < 5; type++)
    {
        axis2_flow_t *flow = NULL;
        axis2_array_list_t *phases = NULL;

        switch (type)
        {
            case AXIS2_IN_FLOW:
            {
                phases = axis2_op_get_in_flow(axis_op, env);
                break;
            }
            case AXIS2_OUT_FLOW:
            {
                phases = axis2_op_get_out_flow(axis_op, env);
                break;
            }
            case AXIS2_FAULT_IN_FLOW:
            {
                phases = axis2_op_get_fault_in_flow(axis_op, env);
                break;
            }
            case AXIS2_FAULT_OUT_FLOW:
            {
                phases = axis2_op_get_fault_out_flow(axis_op, env);
                break;
            }
        }

        if (phases)
        {
            phase_holder =
                axis2_phase_holder_create_with_phases(env, phases);
        }


        switch (type)
        {
            case AXIS2_IN_FLOW:
            {
                flow = axis2_module_desc_get_in_flow(module_desc, env);
                break;
            }
            case AXIS2_OUT_FLOW:
            {
                flow = axis2_module_desc_get_out_flow(module_desc, env);
                break;
            }
            case AXIS2_FAULT_IN_FLOW:
            {
                flow = axis2_module_desc_get_fault_in_flow(module_desc, env);
                break;
            }
            case AXIS2_FAULT_OUT_FLOW:
            {
                flow = axis2_module_desc_get_fault_out_flow(module_desc, env);
                break;
            }
        }

        if (flow && NULL != phase_holder)
        {
            int j = 0;
            int handler_count = 0;

            handler_count = axis2_flow_get_handler_count(flow, env);
            for (j = 0; j < handler_count; j++)
            {
                axis2_handler_desc_t *metadata = NULL;
                const axis2_char_t *phase_name = NULL;
                axis2_phase_rule_t *phase_rule = NULL;
                axis2_status_t status = AXIS2_FAILURE;

                metadata = axis2_flow_get_handler(flow, env, j);
                phase_rule = axis2_handler_desc_get_rules(metadata, env);
                phase_name = axis2_phase_rule_get_name(phase_rule, env);
                if ((0 != AXIS2_STRCMP(AXIS2_PHASE_TRANSPORT_IN, phase_name)) &&
                        (0 != AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name)) &&
                        (0 != AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) &&
                        (0 != AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                {
                    status = AXIS2_PHASE_HOLDER_ADD_HANDLER(phase_holder,
                            env, metadata);
                    if (AXIS2_SUCCESS != status)
                    {
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                "Handler inclusion failed for %s phase", phase_name);
                        AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
                        return status;
                    }

                }
                if ((0 == AXIS2_STRCMP(AXIS2_PHASE_TRANSPORT_IN, phase_name)) ||
                        (0 == AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name)) ||
                        (0 == AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) ||
                        (0 == AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                {
                    axis2_array_list_t *phase_list = NULL;
                    axis2_phase_holder_t *phase_holder = NULL;

                    phase_list =
                        AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(
                            resolver_impl->axis2_config, env);
                    if (phase_holder)
                    {
                        AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
                        phase_holder = NULL;
                    }
                    phase_holder =
                        axis2_phase_holder_create_with_phases(env, phase_list);

                    status = AXIS2_PHASE_HOLDER_ADD_HANDLER(phase_holder, env, metadata);
                    AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
                    phase_holder = NULL;
                    if (AXIS2_SUCCESS != status)
                    {
                        return status;
                    }
                }
            }
        }

        if(phase_holder)
        {
            AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
            phase_holder = NULL;
        }
    }
    
    return AXIS2_SUCCESS;
}

