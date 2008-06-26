
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
#include <axutil_property.h>
#include <axis2_addr.h>
/*
 * It is important to understand the following relationships between the
 * functions defined here and else where.
 * axis2_phase_resolver_engage_module_globally->axis2_svc_add_module_ops->
 * ->axis2_phase_resolver_build_execution_chains_for_module_op->axis2_phase_resolver_build_execution_chains_for_op
 *  and
 * axis2_phase_resolver_engage_module_to_svc->axis2_svc_add_module_ops->
 * ->axis2_phase_resolver_build_execution_chains_for_module_op->axis2_phase_resolver_build_execution_chains_for_op
 */
struct axis2_phase_resolver
{

    /** axis2 configuration */
    axis2_conf_t *axis2_config;

    /** service */
    axis2_svc_t *svc;
};

static axis2_status_t axis2_phase_resolver_build_execution_chains_for_op(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    int type,
    axis2_op_t * op);

static axis2_status_t axis2_phase_resolver_add_module_handlers_to_system_defined_phases(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_module_desc_t * module_desc);

static axis2_status_t
axis2_phase_resolver_add_module_handlers_to_user_defined_phases(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    struct axis2_svc *svc,
    struct axis2_module_desc *module_desc);

/* Deprecated and no longer used */
static axis2_status_t axis2_phase_resolver_build_in_transport_chains(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_transport_in_desc_t * transport);

/* Deprecated and no longer used */
static axis2_status_t axis2_phase_resolver_build_out_transport_chains(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_transport_out_desc_t * transport);

static axis2_status_t
axis2_phase_resolver_add_to_handler_list(
    const axutil_env_t * env,
    axutil_array_list_t *handler_list,
    axis2_op_t *op,
    axis2_module_desc_t * module_desc,
    int type);

AXIS2_EXTERN axis2_phase_resolver_t *AXIS2_CALL
axis2_phase_resolver_create(
    const axutil_env_t * env)
{
    axis2_phase_resolver_t *phase_resolver = NULL;

    phase_resolver = (axis2_phase_resolver_t *) AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_phase_resolver_t));

    if (!phase_resolver)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory.");
        return NULL;
    }

    phase_resolver->axis2_config = NULL;
    phase_resolver->svc = NULL;

    return phase_resolver;
}

AXIS2_EXTERN axis2_phase_resolver_t *AXIS2_CALL
axis2_phase_resolver_create_with_config(
    const axutil_env_t * env,
    axis2_conf_t * axis2_config)
{
    axis2_phase_resolver_t *phase_resolver = NULL;

    AXIS2_PARAM_CHECK(env->error, axis2_config, NULL);

    phase_resolver = (axis2_phase_resolver_t *)
        axis2_phase_resolver_create(env);

    phase_resolver->axis2_config = axis2_config;

    return phase_resolver;
}

AXIS2_EXTERN axis2_phase_resolver_t *AXIS2_CALL
axis2_phase_resolver_create_with_config_and_svc(
    const axutil_env_t * env,
    axis2_conf_t * axis2_config,
    axis2_svc_t * svc)
{
    axis2_phase_resolver_t *phase_resolver = NULL;

    AXIS2_PARAM_CHECK(env->error, axis2_config, NULL);

    phase_resolver = (axis2_phase_resolver_t *)
        axis2_phase_resolver_create(env);

    if (!phase_resolver)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No Memory.");
        return NULL;
    }
    phase_resolver->axis2_config = axis2_config;

    phase_resolver->svc = svc;
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Service name is : %s",
                    axis2_svc_get_name(phase_resolver->svc, env));

    return phase_resolver;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_phase_resolver_free(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env)
{
    if (phase_resolver)
    {
        AXIS2_FREE(env->allocator, phase_resolver);
    }

    return;
}

/**
 * This is in general called to engage a module to the axis2 engine. In other words modules handlers 
 * are added into all global and operation specific phases appropriately. Where these handlers 
 * should go is determined by the module handler specific descriptions in module.xml file. Also 
 * module operations are added to service and built exeuction chains for those operations as well.
 * First add all the handlers defined for system phases are added into system phases. Then module
 * operations are added into each service. At the same time execution chains for these module
 * operations are built as well. Then handlers defined for user phases are added into user defined
 * pahses.
 */

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_globally(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_module_desc_t * module_desc)
{
    axis2_status_t status = AXIS2_FAILURE;
    axutil_qname_t *qname_addressing = NULL;
    axutil_hash_t *svcs = NULL;
    
    const axutil_qname_t *mod_qname = NULL;
    axis2_char_t *mod_name = NULL;
    axutil_hash_t *all_ops = NULL;
    axutil_hash_index_t *index_j = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Entry:axis2_phase_resolver_engage_module_globally");

    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);

    mod_qname = axis2_module_desc_get_qname(module_desc, env);
    mod_name = axutil_qname_get_localpart(mod_qname, env);

    /* Add module handlers into global phases */
    status = axis2_phase_resolver_add_module_handlers_to_system_defined_phases(phase_resolver, env, module_desc);

    if (AXIS2_SUCCESS != status)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Engaging module %s to global chain failed", mod_name);
        return status;
    }

    /* Module is engaged to all the services */
    svcs = axis2_conf_get_all_svcs(phase_resolver->axis2_config, env);
    if (!svcs)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "There are no services in the axis2 configuration");
        return AXIS2_FAILURE;
    }
 
    qname_addressing = axutil_qname_create(env, AXIS2_MODULE_ADDRESSING, NULL, NULL);

    for (index_j = axutil_hash_first(svcs, env); index_j; index_j = axutil_hash_next(env, index_j))
    {
        axis2_svc_t *svc = NULL;
        void *w = NULL;
        axis2_svc_grp_t *parent = NULL;
        const axis2_char_t *svc_name = NULL;
        const axis2_char_t *svc_grp_name = NULL;
        
        axutil_hash_this(index_j, NULL, NULL, &w);
        svc = (axis2_svc_t *) w;
        svc_name = axis2_svc_get_name(svc, env);

        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "svc name is:%s", svc_name);

        /* Module operations are added to service and built execution chains for operations. */
        status = axis2_svc_add_module_ops(svc, env, module_desc, phase_resolver->axis2_config);
        if (AXIS2_SUCCESS != status)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Adding module operations for module %s to service %s failed", mod_name, svc_name);
            axutil_qname_free(qname_addressing, env);
            return status;
        }

        /* Call this function to add module handlers into service operation phases */
        status = axis2_phase_resolver_add_module_handlers_to_user_defined_phases(phase_resolver, 
                env, svc, module_desc);

        if (AXIS2_SUCCESS != status)
        {
            axutil_qname_free(qname_addressing, env);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Engaging module %s to service %s failed", 
                    mod_name, svc_name);

            return status;
        }

        if (axutil_qname_equals(mod_qname, env, qname_addressing))
        {
            /* If addressing module then all operations which are not module 
             * operations with a wsa mapping parameter is added to the 
             * service's wsa-mapping list*/
            all_ops = axis2_svc_get_all_ops(svc, env);
            if (all_ops)
            {
                axutil_hash_index_t *hi = NULL;
                void *val = NULL;

                for (hi = axutil_hash_first(all_ops, env); hi;
                     hi = axutil_hash_next(env, hi))
                {
                    axutil_hash_this(hi, NULL, NULL, &val);

                    if (val)
                    {
                        if (!axis2_op_is_from_module((axis2_op_t *) val, env))
                        {
                            axis2_op_t *op_desc = NULL;
                            axutil_array_list_t *params = NULL;
                            int j = 0;
                            int sizej = 0;

                            op_desc = (axis2_op_t *)val;
                            params = axis2_op_get_all_params(op_desc, env);
                            /* Adding wsa-mapping into service */
                            sizej = axutil_array_list_size(params, env);
                            for (j = 0; j < sizej; j++)
                            {
                                axutil_param_t *param = NULL;
                                axis2_char_t *param_name = NULL;

                                param = axutil_array_list_get(params, env, j);
                                param_name = axutil_param_get_name(param, env);
                                if (!axutil_strcmp(param_name, AXIS2_WSA_MAPPING))
                                {
                                    axis2_char_t *key = NULL;
                                    key = (axis2_char_t *) axutil_param_get_value(param, env);
                                    axis2_svc_add_mapping(svc, env, key, op_desc);
                                }
                            }
                        }
                        val = NULL;
                    }
                }
            }
        }

        parent = axis2_svc_get_parent(svc, env);
        if (parent)
        {
            axutil_array_list_t *modules = NULL;
            int j = 0;
            int sizej = 0;
            axis2_bool_t add_to_group = AXIS2_TRUE;
            svc_grp_name = axis2_svc_grp_get_name(parent, env);

            modules = axis2_svc_grp_get_all_module_qnames(parent, env);
            sizej = axutil_array_list_size(modules, env);
            for (j = 0; j < sizej; j++)
            {
                axutil_qname_t *module = NULL;

                module = (axutil_qname_t *) axutil_array_list_get(modules, env, j);
                if (axutil_qname_equals(mod_qname, env, module))
                {
                    add_to_group = AXIS2_FALSE;
                    break;
                }
            }

            if (add_to_group)
            {
                status = axis2_svc_grp_add_module_qname(parent, env, mod_qname);
            }
        }

        if (AXIS2_SUCCESS != status)
        {
            axutil_qname_free(qname_addressing, env);

            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Adding module %s to service group %s failed", mod_name, svc_grp_name);

            return status;
        }
    }

    axutil_qname_free(qname_addressing, env);

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_phase_resolver_engage_module_globally");

    return status;
}

/**
 * This function is called to engage a module to a service specifically. In other words all module 
 * handlers defined for user phases are added into user defined phases and all module handlers 
 * defined for system defined phases are added into system defined phases. Note that user defined
 * phases are in the flows taken from operation and system defined phases are in the flows taken
 * from conf. Where each module handler should go is determined by module handler descriptions in 
 * module.xml file.
 * First we add the operations defined in the module into the service and built execution chains for 
 * them. Then for all the operations of the service we check whether the module 
 * already engaged to operation. If not engage it to service operation. Also if the module is newly 
 * engaged to operation add the module qnname to the engaged module list of the operation.
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_svc(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_svc_t * svc,
    axis2_module_desc_t * module_desc)
{
    axutil_hash_t *ops = NULL;
    axutil_hash_index_t *index_i = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    const axutil_qname_t *module_d_qname = NULL;
    axis2_char_t *modname_d = NULL;
    const axis2_char_t *svcname = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Entry:axis2_phase_resolver_engage_module_to_svc");

    module_d_qname = axis2_module_desc_get_qname(module_desc, env);
    modname_d = axutil_qname_get_localpart(module_d_qname, env); 
    svcname = axis2_svc_get_name(svc, env);

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Module %s will be engaged to %s", modname_d, svcname);

    ops = axis2_svc_get_all_ops(svc, env);
    if (!ops)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Service %s has no operation", svcname);

        return AXIS2_FAILURE;
    }

    /* Module operations are added to service and built execution chains */
    status = axis2_svc_add_module_ops(svc, env, module_desc, phase_resolver->axis2_config);

    if (AXIS2_SUCCESS != status)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Adding module operations from module %s into service %s failed", modname_d, svcname);

        return status;
    }

    for (index_i = axutil_hash_first(ops, env); index_i; index_i = axutil_hash_next(env, index_i))
    {
        axutil_array_list_t *modules = NULL;
        axis2_op_t *op_desc = NULL;
        int size = 0;
        int j = 0;
        void *v = NULL;
        axis2_bool_t engaged = AXIS2_FALSE;
        axis2_char_t *opname = NULL;

        axutil_hash_this(index_i, NULL, NULL, &v);
        op_desc = (axis2_op_t *) v;

        opname = axutil_qname_get_localpart(axis2_op_get_qname(op_desc, env), env);

        modules = axis2_op_get_all_modules(op_desc, env);
        if (modules)
        {
            size = axutil_array_list_size(modules, env);
        }

        for (j = 0; j < size; j++)
        {
            axis2_module_desc_t *module_desc_l = NULL;
            const axutil_qname_t *module_d_qname_l = NULL;

            module_desc_l = axutil_array_list_get(modules, env, j);
            module_d_qname_l = axis2_module_desc_get_qname(module_desc_l, env);
            if (axutil_qname_equals(module_d_qname, env, module_d_qname_l))
            {
                engaged = AXIS2_TRUE;
                status = AXIS2_SUCCESS;
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "Module %s already engaged to operation %s of service %s", modname_d, opname, 
                    svcname);

                break;
            }
        }

        if (!engaged)
        {
            status = axis2_phase_resolver_engage_module_to_op(phase_resolver, env, op_desc, 
                    module_desc);

            if (AXIS2_SUCCESS != status)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Engaging module %s to operation %s failed.", modname_d, opname);

                return status;
            }

            status = axis2_op_add_to_engaged_module_list(op_desc, env, module_desc);
        }

    }

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Exit:axis2_phase_resolver_engage_module_to_svc");

    return status;
}

/**
 * In this function all the handlers in each flow of the module description are added to the phases 
 * of the operation(user define phases) and phases of the conf(system defined phases). First handlers 
 * for system defined phases are added. Then handlers for operation specific phases are added.
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_op(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_op_t * axis_op,
    axis2_module_desc_t * module_desc)
{
    int type = 0;
    axis2_phase_holder_t *phase_holder = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_phase_resolver_engage_module_to_op");
    AXIS2_PARAM_CHECK(env->error, axis_op, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);

    for (type = 1; type < 5; type++)
    {
        axis2_flow_t *flow = NULL;
        axis2_char_t *flowname = NULL;
        axutil_array_list_t *phases = NULL;

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
            phase_holder = axis2_phase_holder_create_with_phases(env, phases);
        }

        switch (type)
        {
        case AXIS2_IN_FLOW:
            {
                flow = axis2_module_desc_get_in_flow(module_desc, env);
                flowname = "in flow";
                break;
            }
        case AXIS2_OUT_FLOW:
            {
                flow = axis2_module_desc_get_out_flow(module_desc, env);
                flowname = "out flow";
                break;
            }
        case AXIS2_FAULT_IN_FLOW:
            {
                flow = axis2_module_desc_get_fault_in_flow(module_desc, env);
                flowname = "fault in flow";
                break;
            }
        case AXIS2_FAULT_OUT_FLOW:
            {
                flow = axis2_module_desc_get_fault_out_flow(module_desc, env);
                flowname = "fault out flow";
                break;
            }
        }

        if (flow && phase_holder)
        {
            int j = 0;
            int handler_count = 0;

            handler_count = axis2_flow_get_handler_count(flow, env);
            for (j = 0; j < handler_count; j++)
            {
                /* For all handlers in the flow from the module description */
                axis2_handler_desc_t *metadata = NULL;
                const axis2_char_t *phase_name = NULL;
                axis2_phase_rule_t *phase_rule = NULL;
                const axutil_string_t *handlersname = NULL;
                const axis2_char_t *handlername = NULL;
                axis2_status_t status = AXIS2_FAILURE;

                metadata = axis2_flow_get_handler(flow, env, j);
                handlersname = axis2_handler_desc_get_name(metadata, env);
                handlername = axutil_string_get_buffer(handlersname, env);
                phase_rule = axis2_handler_desc_get_rules(metadata, env);
                phase_name = axis2_phase_rule_get_name(phase_rule, env);
                
                /* For user/operation specific phases */
                if ((axutil_strcmp(AXIS2_PHASE_TRANSPORT_IN, phase_name))
                    && (axutil_strcmp(AXIS2_PHASE_DISPATCH, phase_name)) &&
                    (axutil_strcmp(AXIS2_PHASE_POST_DISPATCH, phase_name))
                    && (axutil_strcmp(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                {
                    status = axis2_phase_holder_add_handler(phase_holder, env, metadata);
                    if (AXIS2_SUCCESS != status)
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "Handler %s inclusion failed for %s phase within flow %s. Phase might"\
                            "not available in axis2.xml", handlername, phase_name, phase_name, 
                            flowname);

                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "");
                        axis2_phase_holder_free(phase_holder, env);
                        return status;
                    }

                }

                /* For System defined phases */
                if ((!axutil_strcmp(AXIS2_PHASE_TRANSPORT_IN, phase_name))
                    || (!axutil_strcmp(AXIS2_PHASE_DISPATCH, phase_name)) ||
                    (!axutil_strcmp(AXIS2_PHASE_POST_DISPATCH, phase_name))
                    || (!axutil_strcmp(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                {
                    axutil_array_list_t *phase_list = NULL;
                    axis2_phase_holder_t *phase_holder = NULL;

                    phase_list = axis2_conf_get_in_phases_upto_and_including_post_dispatch
                        (phase_resolver->axis2_config, env);

                    if (phase_holder)
                    {
                        axis2_phase_holder_free(phase_holder, env);
                        phase_holder = NULL;
                    }

                    phase_holder = axis2_phase_holder_create_with_phases(env, phase_list);

                    status = axis2_phase_holder_add_handler(phase_holder, env, metadata);
                    axis2_phase_holder_free(phase_holder, env);
                    phase_holder = NULL;

                    if (AXIS2_SUCCESS != status)
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                            "Adding handler %s to phase %s within flow %s failed", handlername, 
                            phase_name, flowname);

                        return status;
                    }
                }
            }
        }

        if (phase_holder)
        {
            axis2_phase_holder_free(phase_holder, env);
            phase_holder = NULL;
        }
    }

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Exit:axis2_phase_resolver_engage_module_to_op");

    return AXIS2_SUCCESS;
}

/**
 * The caller function first set the service into the phase resolver. Then call this function to 
 * build execution chains for that services operations. Within this function it just call
 * axis2_phase_resolver_build_execution_chains_for_op() function to build exection chains for
 * each operation.
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_execution_chains_for_svc(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env)
{
    axutil_hash_index_t *index_i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_op_t *op = NULL;
    axutil_hash_t *ops = NULL;

    if (!(phase_resolver->svc))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No service set to phase resolver");
        return AXIS2_FAILURE;
    }

    ops = axis2_svc_get_all_ops(phase_resolver->svc, env);

    for (index_i = axutil_hash_first(ops, env); index_i; index_i = axutil_hash_next(env, index_i))
    {
        void *v = NULL;
        int j = 0;

        axutil_hash_this(index_i, NULL, NULL, &v);
        op = (axis2_op_t *) v;
        for (j = 1; j < 5; j++)
        {
            status = axis2_phase_resolver_build_execution_chains_for_op(phase_resolver, env, j, op);
        }
    }

    return status;
}

/**
 * For operation passed as parameter, build execution chains. To do this get all engaged modules
 * from the axis2 configuration and for each module get the all handlers to be add to the operation 
 * specific phases. Then for each operation specific phases add those handlers. It should be noted 
 * that by the time this function is called the module handlers are already added to system specific 
 * phases. This function is called from axis2_phase_resolver_build_execution_chains_for_svc() 
 * function and axis2_phase_resolver_build_execution_chains_for_module_op() function.
 */
static axis2_status_t
axis2_phase_resolver_build_execution_chains_for_op(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    int type,
    axis2_op_t *op)
{
    axutil_array_list_t *handler_list = NULL;
    axutil_array_list_t *moduleqnames = NULL;
    int i = 0;
    int size = 0;
    int status = AXIS2_FAILURE;
    axis2_char_t *flowname = NULL;
    axis2_phase_holder_t *phase_holder = NULL;
    axutil_array_list_t *engaged_module_list_for_parent_svc = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
            "Entry:axis2_phase_resolver_build_execution_chains_for_op");

    handler_list = axutil_array_list_create(env, 0);
    if (!handler_list)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory");
        return AXIS2_FAILURE;
    }

    /* Engage handlers from axis2.xml and from modules */
    moduleqnames = axis2_conf_get_all_engaged_modules(phase_resolver->axis2_config, env);

    size = axutil_array_list_size(moduleqnames, env);
    
    for (i = 0; i < size; i++)
    {
        axis2_char_t *modulename = NULL;
        axutil_qname_t *moduleqname = NULL;
        axis2_module_desc_t *module_desc = NULL;

        moduleqname = (axutil_qname_t *) axutil_array_list_get(moduleqnames, env, i);
        modulename = axutil_qname_get_localpart(moduleqname, env);
        
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Module name is:%s", modulename);

        module_desc = axis2_conf_get_module(phase_resolver->axis2_config, env, moduleqname);
        if(!module_desc)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_MODULE_REF, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Module description not found in axis2 configuration for name %s", modulename);

            if (handler_list)
            {
                axutil_array_list_free(handler_list, env);
            }

            return AXIS2_FAILURE;
        }

        status = axis2_phase_resolver_add_to_handler_list(env, handler_list, op, module_desc, type);
        if(AXIS2_SUCCESS != status)
        {
            if (handler_list)
            {
                axutil_array_list_free(handler_list, env);
            }
            return AXIS2_FAILURE;
        }

        axis2_op_add_to_engaged_module_list(op, env, module_desc);
    }

    engaged_module_list_for_parent_svc = axis2_svc_get_engaged_module_list(phase_resolver->svc, env);
    size = axutil_array_list_size(engaged_module_list_for_parent_svc, env);

    for (i = 0; i < size; i++)
    {
        axis2_char_t *modulename = NULL;
        axutil_qname_t *moduleqname = NULL;
        axis2_module_desc_t *module_desc = NULL;

        module_desc = axutil_array_list_get(engaged_module_list_for_parent_svc, env, i);
        if(!module_desc)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_MODULE_REF, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Module description not found in engaged module list for service %s", 
                axis2_svc_get_name(phase_resolver->svc, env));

            if (handler_list)
            {
                axutil_array_list_free(handler_list, env);
            }

            return AXIS2_FAILURE;
        }
        
        moduleqname = (axutil_qname_t *) axis2_module_desc_get_qname(module_desc, env);
        modulename = axutil_qname_get_localpart(moduleqname, env);
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Module name is:%s", modulename);

        status = axis2_phase_resolver_add_to_handler_list(env, handler_list, op, module_desc, type);
        if(AXIS2_SUCCESS != status)
        {
            if (handler_list)
            {
                axutil_array_list_free(handler_list, env);
            }
            return AXIS2_FAILURE;
        }

        axis2_op_add_to_engaged_module_list(op, env, module_desc);
    }

    if(0 == axutil_array_list_size(handler_list, env))
    {
        /* No flows configured */
        if (handler_list)
        {
            axutil_array_list_free(handler_list, env);
        }

        return AXIS2_SUCCESS;
    }

    switch (type)
    {
    case AXIS2_IN_FLOW:
        {
            axutil_array_list_t *phase_list = NULL;

            phase_list = axis2_op_get_in_flow(op, env);
            phase_holder = axis2_phase_holder_create_with_phases(env, phase_list);
            flowname = "in flow";
            break;
        }
    case AXIS2_OUT_FLOW:
        {
            axutil_array_list_t *phase_list = NULL;

            phase_list = axis2_op_get_out_flow(op, env);
            phase_holder = axis2_phase_holder_create_with_phases(env, phase_list);
            flowname = "out flow";
            break;
        }
    case AXIS2_FAULT_IN_FLOW:
        {
            axutil_array_list_t *phase_list = NULL;

            phase_list = axis2_op_get_fault_in_flow(op, env);
            phase_holder = axis2_phase_holder_create_with_phases(env, phase_list);
            flowname = "fault in flow";
            break;
        }
    case AXIS2_FAULT_OUT_FLOW:
        {
            axutil_array_list_t *phase_list = NULL;

            phase_list = axis2_op_get_fault_out_flow(op, env);
            phase_holder = axis2_phase_holder_create_with_phases(env, phase_list);
            flowname = "fault out flow";
            break;
        }
    }

    size = axutil_array_list_size(handler_list, env);
    for (i = 0; i < size; i++)
    {
        axis2_handler_desc_t *metadata = NULL;

        metadata = (axis2_handler_desc_t *) axutil_array_list_get(handler_list, env, i);
        if (phase_holder)
        {
            status = axis2_phase_holder_add_handler(phase_holder, env, metadata);
            if (!status)
            {
                break;
            }
        }
    }

    /* Free the locally created handler_list*/
    if (handler_list)
    {
        axutil_array_list_free(handler_list, env);
    }

    if (phase_holder)
    {
        axis2_phase_holder_free(phase_holder, env);
    }

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Exit:axis2_phase_resolver_build_execution_chains_for_op");

    return status;
}

/**
 * For module operation build execution chains. This is called by axis2_svc_add_module_ops() function.
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_execution_chains_for_module_op(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_op_t * op)
{
    int i = 0;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI,
                    "Entry:axis2_phase_resolver_build_execution_chains_for_module_op");
    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);

    for (i = 1; i < 5; i++)
    {
        status = axis2_phase_resolver_build_execution_chains_for_op(phase_resolver,
                                                             env, i, op);
        if (!status)
        {
            break;
        }
    }

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI,
                    "Exit:axis2_phase_resolver_build_execution_chains_for_module_op");
    return status;
}

/**
 * Take the phases for each flow from the axis2 configuration, take all the handlers of each flow 
 * from the module description and then each handler is added into the corresponding global phase. 
 * This function is called from  function axis2_phase_resolver_engage_module_globally() to add 
 * module handlers into global phases.
 */
static axis2_status_t
axis2_phase_resolver_add_module_handlers_to_system_defined_phases(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_module_desc_t * module_desc)
{
    int type = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_phase_holder_t *phase_holder = NULL;
    const axutil_qname_t *modqname = NULL;
    axis2_char_t *modname = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_phase_resolver_add_module_handlers_to_system_defined_phases");

    modqname = axis2_module_desc_get_qname(module_desc, env);
    modname = axutil_qname_get_localpart(modqname, env);
    for (type = 1; type < 5; type++)
    {
        axis2_flow_t *flow = NULL;
        axis2_char_t *flow_name = NULL;

        switch (type)
        {
        case AXIS2_IN_FLOW:
            {
                axutil_array_list_t *phase_list = NULL;

                phase_list =
                    axis2_conf_get_in_phases_upto_and_including_post_dispatch
                    (phase_resolver->axis2_config, env);
                phase_holder =
                    axis2_phase_holder_create_with_phases(env, phase_list);
                if (!phase_holder)
                    continue;
                break;
            }
        case AXIS2_OUT_FLOW:
            {
                axutil_array_list_t *phase_list = NULL;

                phase_list =
                    axis2_conf_get_out_flow(phase_resolver->axis2_config, env);
                phase_holder =
                    axis2_phase_holder_create_with_phases(env, phase_list);
                if (!phase_holder)
                    continue;
                break;
            }
        case AXIS2_FAULT_IN_FLOW:
            {
                axutil_array_list_t *phase_list = NULL;

                phase_list = axis2_conf_get_in_fault_flow(phase_resolver->
                                                          axis2_config, env);
                phase_holder =
                    axis2_phase_holder_create_with_phases(env, phase_list);
                if (!phase_holder)
                    continue;
                break;
            }
        case AXIS2_FAULT_OUT_FLOW:
            {
                axutil_array_list_t *phase_list = NULL;

                phase_list = axis2_conf_get_out_fault_flow(phase_resolver->
                                                           axis2_config, env);
                phase_holder =
                    axis2_phase_holder_create_with_phases(env, phase_list);
                if (!phase_holder)
                    continue;
                break;
            }
        }

        /* Modules referred by axis2.xml */
        switch (type)
        {
        case AXIS2_IN_FLOW:
            {
                flow = axis2_module_desc_get_in_flow(module_desc, env);
                flow_name = "in flow";
                break;
            }
        case AXIS2_OUT_FLOW:
            {
                flow = axis2_module_desc_get_out_flow(module_desc, env);
                flow_name = "out flow";
                break;
            }
        case AXIS2_FAULT_IN_FLOW:
            {
                flow = axis2_module_desc_get_fault_in_flow(module_desc, env);
                flow_name = "fault in flow";
                break;
            }
        case AXIS2_FAULT_OUT_FLOW:
            {
                flow = axis2_module_desc_get_fault_out_flow(module_desc, env);
                flow_name = "fault out flow";
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
                const axutil_string_t *handlersname = NULL;
                const axis2_char_t *handlername = NULL;

                metadata = axis2_flow_get_handler(flow, env, j);
                handlersname = axis2_handler_desc_get_name(metadata, env);
                handlername = axutil_string_get_buffer(handlersname, env);
                phase_rule = axis2_handler_desc_get_rules(metadata, env);
                if (phase_rule)
                {
                    phase_name = axis2_phase_rule_get_name(phase_rule, env);
                }
                if (!phase_name)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "Phase rules for handler %s has no name", handlername);
                    return AXIS2_FAILURE;
                }
                if ((!axutil_strcmp(AXIS2_PHASE_TRANSPORT_IN, phase_name))
                    || (!axutil_strcmp(AXIS2_PHASE_DISPATCH, phase_name)) ||
                    (!axutil_strcmp(AXIS2_PHASE_POST_DISPATCH, phase_name))
                    || (!axutil_strcmp(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                {
                    /* If a global phase add the module handler*/
                    status = axis2_phase_holder_add_handler(phase_holder, env,
                                                            metadata);
                    if (!status)
                    {
                        axis2_phase_holder_free(phase_holder, env);
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                            "Adding handler %s of module %s to phase %s of "\
                            "flow %s failed", handlername, modname, phase_name, 
                            flow_name);
                        return status;
                    }
                }
            }
        }
        if (phase_holder)
        {
            axis2_phase_holder_free(phase_holder, env);
        }
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_phase_resolver_add_module_handlers_to_system_defined_phases");
    return AXIS2_SUCCESS;
}

/**
 * For each operation of the service first check whether module is already engaged to operation. 
 * If not take each operations flows and add the module handlers into them appropriately. This 
 * function is called from function axis2_phase_resolver_engage_module_globally() to add handlers 
 * from module into each services all operations.
 */
static axis2_status_t
axis2_phase_resolver_add_module_handlers_to_user_defined_phases(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_svc_t * svc,
    axis2_module_desc_t * module_desc)
{
    axutil_hash_t *ops = NULL;
    axis2_bool_t engaged = AXIS2_FALSE;
    axutil_hash_index_t *index_i = NULL;
    int type = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_phase_holder_t *phase_holder = NULL;
    const axis2_char_t *svc_name = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_phase_resolver_add_module_handlers_to_user_defined_phases");

    AXIS2_PARAM_CHECK(env->error, svc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    svc_name = axis2_svc_get_name(svc, env);
    ops = axis2_svc_get_all_ops(svc, env);
    if (!ops)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No operations for the service %s", svc_name);
        return AXIS2_FAILURE;
    }

    for (index_i = axutil_hash_first(ops, env); index_i; index_i = axutil_hash_next(env, index_i))
    {
        void *v = NULL;
        axis2_op_t *op_desc = NULL;
        int j = 0;
        axutil_array_list_t *modules = NULL;
        axis2_flow_t *flow = NULL;
        axis2_char_t *flowname = NULL;
        const axutil_qname_t *module_desc_qname = NULL;
        axis2_char_t *module_desc_name = NULL;
        int size = 0;
        axis2_char_t *op_name = NULL;

        axutil_hash_this(index_i, NULL, NULL, &v);
        op_desc = (axis2_op_t *) v;
        op_name = axutil_qname_get_localpart(axis2_op_get_qname(op_desc, env), env);
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Operation name is : %s", op_name);

        /* Get all modules engaged to the operation */
        modules = axis2_op_get_all_modules(op_desc, env);
        module_desc_qname = axis2_module_desc_get_qname(module_desc, env);
        module_desc_name = axutil_qname_get_localpart(module_desc_qname, env);
        if (modules)
        {
            size = axutil_array_list_size(modules, env);
        }

        /* Checking whether module is already engaged to operation */
        for (j = 0; j < size; j++)
        {
            axis2_module_desc_t *module_desc_l = NULL;
            const axutil_qname_t *module_desc_qname_l = NULL;

            module_desc_l = (axis2_module_desc_t *) axutil_array_list_get(modules, env, j);

            module_desc_qname_l = axis2_module_desc_get_qname(module_desc_l, env);
            if (axutil_qname_equals(module_desc_qname_l, env, module_desc_qname))
            {
                engaged = AXIS2_TRUE;
                break;
            }
        }

        if (engaged)
        {
            continue;
        }

        for (type = 1; type < 5; type++)
        {
            switch (type)
            {
            case AXIS2_IN_FLOW:
                {
                    axutil_array_list_t *phase_list = NULL;

                    phase_list = axis2_op_get_in_flow(op_desc, env);
                    phase_holder = axis2_phase_holder_create_with_phases(env, phase_list);
                    break;
                }
            case AXIS2_OUT_FLOW:
                {
                    axutil_array_list_t *phase_list = NULL;

                    phase_list = axis2_op_get_out_flow(op_desc, env);
                    phase_holder = axis2_phase_holder_create_with_phases(env, phase_list);
                    break;
                }
            case AXIS2_FAULT_IN_FLOW:
                {
                    axutil_array_list_t *phase_list = NULL;

                    phase_list = axis2_op_get_fault_in_flow(op_desc, env);
                    phase_holder = axis2_phase_holder_create_with_phases(env, phase_list);
                    break;
                }
            case AXIS2_FAULT_OUT_FLOW:
                {
                    axutil_array_list_t *phase_list = NULL;

                    phase_list = axis2_op_get_fault_out_flow(op_desc, env);
                    phase_holder = axis2_phase_holder_create_with_phases(env, phase_list);
                    break;
                }
            }

            /* Process modules referred by axis2.xml */

            switch (type)
            {
            case AXIS2_IN_FLOW:
                {
                    flow = axis2_module_desc_get_in_flow(module_desc, env);
                    flowname = "in flow";
                    break;
                }
            case AXIS2_OUT_FLOW:
                {
                    flow = axis2_module_desc_get_out_flow(module_desc, env);
                    flowname = "out flow";
                    break;
                }
            case AXIS2_FAULT_IN_FLOW:
                {
                    flow = axis2_module_desc_get_fault_in_flow(module_desc, env);
                    flowname = "fault in flow";
                    break;
                }
            case AXIS2_FAULT_OUT_FLOW:
                {
                    flow = axis2_module_desc_get_fault_out_flow(module_desc, env);
                    flowname = "fault out flow";
                    break;
                }
            }

            if (flow)
            {
                int handler_count = 0;

                handler_count = axis2_flow_get_handler_count(flow, env);
                for (j = 0; j < handler_count; j++)
                {
                    axis2_handler_desc_t *metadata = NULL;
                    const axis2_char_t *phase_name = NULL;
                    axis2_phase_rule_t *phase_rule = NULL;
                    const axutil_string_t *handlersname = NULL;
                    const axis2_char_t *handlername = NULL;

                    metadata = axis2_flow_get_handler(flow, env, j);
                    handlersname = axis2_handler_desc_get_name(metadata, env);
                    handlername = axutil_string_get_buffer(handlersname, env);
                    phase_rule = axis2_handler_desc_get_rules(metadata, env);
                    if (phase_rule)
                    {
                        phase_name = axis2_phase_rule_get_name(phase_rule, env);
                    }
                    if (!phase_name)
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                            "Handler rules for the handler description %s within flow %s has no name", 
                            handlername, flowname);

                        return AXIS2_FAILURE;
                    }

                    /* If phase is not a system defined phase, add module handler to it */
                    if ((axutil_strcmp(AXIS2_PHASE_TRANSPORT_IN, phase_name)) && (axutil_strcmp(
                                    AXIS2_PHASE_DISPATCH, phase_name)) && (axutil_strcmp(
                                        AXIS2_PHASE_POST_DISPATCH, phase_name)) && (axutil_strcmp(
                                            AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                    {
                        if (phase_holder)
                        {
                            status = axis2_phase_holder_add_handler(phase_holder, env, metadata);
                            if (!status)
                            {
                                axis2_phase_holder_free(phase_holder, env);
                                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Adding handler desc %s to"\
                                        "phase %s within flow %s failed", handlername, phase_name, 
                                        flowname);

                                return status;
                            }
                        }
                    }
                }
            }

            if (phase_holder)
            {
                axis2_phase_holder_free(phase_holder, env);
            }
        }
        status = axis2_op_add_to_engaged_module_list(op_desc, env, module_desc);
        if (AXIS2_SUCCESS != status)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Adding module description %s to engaged "\
                    "module list of operation %s failed", module_desc_name, op_name);

            return status;
        }
    }

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_phase_resolver_add_module_handlers_to_user_defined_phases");

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_resolver_disengage_module_from_svc(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_svc_t * svc,
    axis2_module_desc_t * module_desc)
{
    axutil_hash_t *ops = NULL;
    axutil_hash_index_t *index_i = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    const axutil_qname_t *module_d_qname = NULL;
    const axis2_char_t *svc_name = axis2_svc_get_name(svc, env);
    axis2_char_t *modname_d = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_phase_resolver_disengage_module_from_svc");

    ops = axis2_svc_get_all_ops(svc, env);
    if (!ops)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Service %s has no operation", svc_name);
        return AXIS2_FAILURE;
    }

    module_d_qname = axis2_module_desc_get_qname(module_desc, env);
    modname_d = axutil_qname_get_localpart(module_d_qname, env);
    for (index_i = axutil_hash_first(ops, env); index_i; index_i = axutil_hash_next(env, index_i))
    {
        axutil_array_list_t *modules = NULL;
        axis2_op_t *op_desc = NULL;
        int size = 0;
        int j = 0;
        void *v = NULL;
        axis2_bool_t engaged = AXIS2_FALSE;
        const axutil_qname_t *opqname = NULL;
        axis2_char_t *opname = NULL;

        axutil_hash_this(index_i, NULL, NULL, &v);
        op_desc = (axis2_op_t *) v;
        opqname = axis2_op_get_qname(op_desc, env);
        opname = axutil_qname_get_localpart(opqname, env);
        modules = axis2_op_get_all_modules(op_desc, env);
        if (modules)
        {
            size = axutil_array_list_size(modules, env);
        }
        for (j = 0; j < size; j++)
        {
            axis2_module_desc_t *module_desc_l = NULL;
            const axutil_qname_t *module_d_qname_l = NULL;

            module_desc_l = axutil_array_list_get(modules, env, j);
            module_d_qname_l = axis2_module_desc_get_qname(module_desc_l, env);
            if (axutil_qname_equals(module_d_qname, env, module_d_qname_l))
            {
                engaged = AXIS2_TRUE;
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "Module %s already engaged.", modname_d);
                break;
            }
        }

        if (engaged)
        {
            status =
                axis2_phase_resolver_disengage_module_from_op(phase_resolver,
                                                              env, op_desc,
                                                              module_desc);
            if (AXIS2_SUCCESS != status)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Disengaging module %s from operation %s failed", modname_d, opname);
                return status;
            }

            status = axis2_op_remove_from_engaged_module_list(op_desc, env,
                                                              module_desc);
        }

    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_phase_resolver_disengage_module_from_svc");
    return status;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_resolver_disengage_module_from_op(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_op_t * axis_op,
    axis2_module_desc_t * module_desc)
{
    int type = 0;
    axis2_phase_holder_t *phase_holder = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_phase_resolver_disengage_module_from_op");
    AXIS2_PARAM_CHECK(env->error, axis_op, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);

    for (type = 1; type < 5; type++)
    {
        axis2_flow_t *flow = NULL;
        axis2_char_t *flowname = NULL;
        axutil_array_list_t *phases = NULL;

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
            phase_holder = axis2_phase_holder_create_with_phases(env, phases);
        }

        switch (type)
        {
        case AXIS2_IN_FLOW:
            {
                flow = axis2_module_desc_get_in_flow(module_desc, env);
                flowname = "in flow";
                break;
            }
        case AXIS2_OUT_FLOW:
            {
                flow = axis2_module_desc_get_out_flow(module_desc, env);
                flowname = "out flow";
                break;
            }
        case AXIS2_FAULT_IN_FLOW:
            {
                flow = axis2_module_desc_get_fault_in_flow(module_desc, env);
                flowname = "fault in flow";
                break;
            }
        case AXIS2_FAULT_OUT_FLOW:
            {
                flow = axis2_module_desc_get_fault_out_flow(module_desc, env);
                flowname = "fault out flow";
                break;
            }
        }

        if (flow && phase_holder)
        {
            int j = 0;
            int handler_count = 0;

            handler_count = axis2_flow_get_handler_count(flow, env);
            for (j = 0; j < handler_count; j++)
            {
                axis2_handler_desc_t *metadata = NULL;
                const axis2_char_t *phase_name = NULL;
                axis2_phase_rule_t *phase_rule = NULL;
                const axutil_string_t *handlersname = NULL;
                const axis2_char_t *handlername = NULL;
                axis2_status_t status = AXIS2_FAILURE;

                metadata = axis2_flow_get_handler(flow, env, j);
                handlersname = axis2_handler_desc_get_name(metadata, env);
                handlername = axutil_string_get_buffer(handlersname, env);
                phase_rule = axis2_handler_desc_get_rules(metadata, env);
                phase_name = axis2_phase_rule_get_name(phase_rule, env);
                if ((axutil_strcmp(AXIS2_PHASE_TRANSPORT_IN, phase_name))
                    && (axutil_strcmp(AXIS2_PHASE_DISPATCH, phase_name)) &&
                    (axutil_strcmp(AXIS2_PHASE_POST_DISPATCH, phase_name))
                    && (axutil_strcmp(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                {
                    status = axis2_phase_holder_remove_handler(phase_holder,
                                                               env, metadata);
                    if (AXIS2_SUCCESS != status)
                    {
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "Handler %s Removal failed for %s phase within flow %s", 
                            handlername, phase_name, flowname);
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "");
                        axis2_phase_holder_free(phase_holder, env);
                        return status;
                    }

                }
                if ((!axutil_strcmp(AXIS2_PHASE_TRANSPORT_IN, phase_name))
                    || (!axutil_strcmp(AXIS2_PHASE_DISPATCH, phase_name)) ||
                    (!axutil_strcmp(AXIS2_PHASE_POST_DISPATCH, phase_name))
                    || (!axutil_strcmp(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                {
                    axutil_array_list_t *phase_list = NULL;
                    axis2_phase_holder_t *phase_holder = NULL;

                    phase_list =
                        axis2_conf_get_in_phases_upto_and_including_post_dispatch
                        (phase_resolver->axis2_config, env);
                    if (phase_holder)
                    {
                        axis2_phase_holder_free(phase_holder, env);
                        phase_holder = NULL;
                    }
                    phase_holder =
                        axis2_phase_holder_create_with_phases(env, phase_list);

                    status =
                        axis2_phase_holder_remove_handler(phase_holder, env,
                                                          metadata);
                    axis2_phase_holder_free(phase_holder, env);
                    phase_holder = NULL;
                    if (AXIS2_SUCCESS != status)
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                            "Removing handler %s from phase %s within flow %s failed", 
                            handlername, phase_name, flowname);
                        return status;
                    }
                }
            }
        }

        if (phase_holder)
        {
            axis2_phase_holder_free(phase_holder, env);
            phase_holder = NULL;
        }
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_phase_resolver_disengage_module_from_op");
    return AXIS2_SUCCESS;
}

/* This function is deprecated and no longer used */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_transport_chains(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env)
{
    axis2_transport_in_desc_t **transports_in = NULL;
    axis2_transport_out_desc_t **transports_out = NULL;
    int index_i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_phase_resolver_build_transport_chains");

    transports_in =
        axis2_conf_get_all_in_transports(phase_resolver->axis2_config, env);
    if (!transports_in)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_TRANSPORT_IN_CONFIGURED,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "No transport in descriptions configured");
        return AXIS2_SUCCESS;
    }

    transports_out =
        axis2_conf_get_all_out_transports(phase_resolver->axis2_config, env);
    if (!transports_out)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_TRANSPORT_OUT_CONFIGURED,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "No transport out descriptions configured");
        return AXIS2_SUCCESS;
    }

    for (index_i = 0; index_i < AXIS2_TRANSPORT_ENUM_MAX; index_i++)
    {
        if (transports_in[index_i])
        {
            status =
                axis2_phase_resolver_build_in_transport_chains(phase_resolver,
                                                               env,
                                                               transports_in
                                                               [index_i]);
            if (AXIS2_SUCCESS != status)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Building transport in chains failed");
                return status;
            }
        }
    }

    for (index_i = 0; index_i < AXIS2_TRANSPORT_ENUM_MAX; index_i++)
    {
        if (transports_out[index_i])
        {
            status =
                axis2_phase_resolver_build_out_transport_chains(phase_resolver,
                                                                env,
                                                                transports_out
                                                                [index_i]);
            if (AXIS2_SUCCESS != status)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Building transport out chains failed");
                return status;
            }
        }
    }

    /* If transport in or transport out maps are not null but still they don't
     * have chains configured then we return success, because there are no
     * chains to process.
     */
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_phase_resolver_build_transport_chains");
    return AXIS2_SUCCESS;
}

/**
 * This function is called from function 
 * axis2_phase_resolver_build_transport_chains().
 * This function is deprecated and no longer used.
 */
static axis2_status_t
axis2_phase_resolver_build_in_transport_chains(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_transport_in_desc_t * transport)
{
    int type = 0;
    int j = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axutil_array_list_t *handlers = NULL;
    AXIS2_TRANSPORT_ENUMS transport_enum = 
        axis2_transport_in_desc_get_enum(transport, env);;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_phase_resolver_build_in_transport_chains"); 
    AXIS2_PARAM_CHECK(env->error, transport, AXIS2_FAILURE);

    for (type = 1; type < 4; type++)
    {
        axis2_flow_t *flow = NULL;
        axis2_char_t *flowname = NULL;
        axis2_phase_t *phase = NULL;

        switch (type)
        {
        case AXIS2_IN_FLOW:
            {
                flow = axis2_transport_in_desc_get_in_flow(transport, env);
                phase = axis2_transport_in_desc_get_in_phase(transport, env);
                flowname = "in flow";
                break;
            }
        case AXIS2_FAULT_IN_FLOW:
            {
                flow =
                    axis2_transport_in_desc_get_fault_in_flow(transport, env);
                phase = axis2_transport_in_desc_get_fault_phase(transport, env);
                flowname = "fault in flow";
                break;
            }
        }
        if (flow)
        {
            axis2_phase_holder_t *phase_holder = NULL;
            int size = 0;

            size = axis2_flow_get_handler_count(flow, env);
            handlers = axutil_array_list_create(env, 0);
            for (j = 0; j < size; j++)
            {
                axis2_handler_desc_t *metadata = NULL;
                axis2_phase_rule_t *rule = NULL;
                const axis2_char_t *handlername = NULL;
                const axutil_string_t *handlersname = NULL;

                metadata = axis2_flow_get_handler(flow, env, j);
                handlersname = axis2_handler_desc_get_name(metadata, env);
                handlername = axutil_string_get_buffer(handlersname, env);
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

                        size = axutil_array_list_size(handlers, env);
                        for (i = 0; i < size; i++)
                        {
                            handler_d = axutil_array_list_get(handlers, env, i);
                            axis2_handler_desc_free(handler_d, env);
                        }
                        axutil_array_list_free(handlers, env);
                    }
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "Setting name %s to phase rules for handler %s failed"\
                        "for in transport %d within flow %s", AXIS2_TRANSPORT_PHASE, 
                        handlername, transport_enum, flowname);
                    return status;
                }
                status = axutil_array_list_add(handlers, env, metadata);
                if (AXIS2_SUCCESS != status)
                {
                    if (handlers)
                    {
                        axis2_handler_desc_t *handler_d = NULL;
                        int i = 0;
                        int size = 0;

                        size = axutil_array_list_size(handlers, env);
                        for (i = 0; i < size; i++)
                        {
                            handler_d = axutil_array_list_get(handlers, env, i);
                            axis2_handler_desc_free(handler_d, env);
                        }
                        axutil_array_list_free(handlers, env);
                    }
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "Adding handler %s from in transport %d to handler "\
                        "list failed within flow %s", handlername, transport_enum,
                        flowname);
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

                    size = axutil_array_list_size(handlers, env);
                    for (i = 0; i < size; i++)
                    {
                        handler_d = axutil_array_list_get(handlers, env, i);
                        axis2_handler_desc_free(handler_d, env);
                    }
                    axutil_array_list_free(handlers, env);
                }
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory");
                return AXIS2_FAILURE;
            }

            status =
                axis2_phase_holder_build_transport_handler_chain(phase_holder,
                                                                 env, phase,
                                                                 handlers);
            if (phase_holder)
            {
                axis2_phase_holder_free(phase_holder, env);
            }
        }
        else
        {
            /* Do nothing */
        }
    }
    if (handlers)
    {
        axutil_array_list_free(handlers, env);
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_phase_resolver_build_in_transport_chains"); 
    return status;
}

/**
 * This function is called from function
 * axis2_phase_resolver_build_transport_chains().
 * This is deprecated and no longer used.
 */
static axis2_status_t
axis2_phase_resolver_build_out_transport_chains(
    axis2_phase_resolver_t * phase_resolver,
    const axutil_env_t * env,
    axis2_transport_out_desc_t * transport)
{
    int type = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axutil_array_list_t *handlers = NULL;
    AXIS2_TRANSPORT_ENUMS transport_enum = 
        axis2_transport_out_desc_get_enum(transport, env);;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_phase_resolver_build_out_transport_chains"); 
    AXIS2_PARAM_CHECK(env->error, transport, AXIS2_FAILURE);
    for (type = 1; type < 5; type++)
    {
        axis2_flow_t *flow = NULL;
        axis2_char_t *flowname = NULL;
        axis2_phase_t *phase = NULL;

        switch (type)
        {
        case AXIS2_OUT_FLOW:
            {
                flow = axis2_transport_out_desc_get_out_flow(transport, env);
                phase = axis2_transport_out_desc_get_out_phase(transport, env);
                flowname = "out flow";
                break;
            }
        case AXIS2_FAULT_OUT_FLOW:
            {
                flow =
                    axis2_transport_out_desc_get_fault_out_flow(transport, env);
                phase =
                    axis2_transport_out_desc_get_fault_phase(transport, env);
                flowname = "fault out flow";
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
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "");
                return AXIS2_ERROR_GET_STATUS_CODE(env->error);
            }
            handlers = axutil_array_list_create(env, 0);

            for (j = 0; j < hndlr_count; j++)
            {
                axis2_handler_desc_t *metadata = NULL;
                axis2_phase_rule_t *rule = NULL;
                const axis2_char_t *handlername = NULL;
                const axutil_string_t *handlersname = NULL;

                metadata = axis2_flow_get_handler(flow, env, j);
                handlersname = axis2_handler_desc_get_name(metadata, env);
                handlername = axutil_string_get_buffer(handlersname, env);

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

                        size = axutil_array_list_size(handlers, env);
                        for (i = 0; i < size; i++)
                        {
                            handler_d = axutil_array_list_get(handlers, env, i);
                            axis2_handler_desc_free(handler_d, env);
                        }
                        axutil_array_list_free(handlers, env);
                    }
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "Setting name %s to phase rules for handler %s failed"\
                        "for out transport %d within flow %s", AXIS2_TRANSPORT_PHASE, 
                        handlername, transport_enum, flowname);
                    return status;
                }

                status = axutil_array_list_add(handlers, env, metadata);
                if (AXIS2_FAILURE == status)
                {
                    if (handlers)
                    {
                        axis2_handler_desc_t *handler_d = NULL;
                        int i = 0;
                        int size = 0;

                        size = axutil_array_list_size(handlers, env);
                        for (i = 0; i < size; i++)
                        {
                            handler_d = axutil_array_list_get(handlers, env, i);
                            axis2_handler_desc_free(handler_d, env);
                        }
                        axutil_array_list_free(handlers, env);
                    }
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "Adding handler %s from out transport %d to handler "\
                        "list failed within flow %s", handlername, transport_enum,
                        flowname);
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

                    size = axutil_array_list_size(handlers, env);
                    for (i = 0; i < size; i++)
                    {
                        handler_d = axutil_array_list_get(handlers, env, i);
                        axis2_handler_desc_free(handler_d, env);
                    }
                    axutil_array_list_free(handlers, env);
                }
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory");
                return AXIS2_FAILURE;
            }

            status =
                axis2_phase_holder_build_transport_handler_chain(phase_holder,
                                                                 env, phase,
                                                                 handlers);
            if (phase_holder)
            {
                axis2_phase_holder_free(phase_holder, env);
            }
        }
        else
        {
            /* Do nothing */
        }
    }
    if (handlers)
    {
        axutil_array_list_free(handlers, env);
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_phase_resolver_build_out_transport_chains"); 
    return status;
}


/**
 * This function is called from axis2_phase_resolver_build_execution_chains_for_op() function.
 */
static axis2_status_t
axis2_phase_resolver_add_to_handler_list(
    const axutil_env_t * env,
    axutil_array_list_t *handler_list,
    axis2_op_t *op,
    axis2_module_desc_t * module_desc,
    int type)
{
    axis2_flow_t *flow = NULL;
    axis2_char_t *flowname = NULL;
    const axutil_qname_t *opqname = NULL;
    axis2_char_t *opname = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    opqname = axis2_op_get_qname(op, env);
    opname = axutil_qname_get_localpart(opqname, env);

    switch (type)
    {
    case AXIS2_IN_FLOW:
        {
            flow = axis2_module_desc_get_in_flow(module_desc, env);
            flowname = "in flow";
            break;
        }
    case AXIS2_OUT_FLOW:
        {
            flow = axis2_module_desc_get_out_flow(module_desc, env);
            flowname = "out flow";
            break;
        }
    case AXIS2_FAULT_IN_FLOW:
        {
            flow = axis2_module_desc_get_fault_in_flow(module_desc, env);
            flowname = "fault in flow";
            break;
        }
    case AXIS2_FAULT_OUT_FLOW:
        {
            flow = axis2_module_desc_get_fault_out_flow(module_desc, env);
            flowname = "fault out flow";
            break;
        }
    }

    if (flow)
    {
        int j = 0;
        int count = 0;

        count = axis2_flow_get_handler_count(flow, env);
        if (AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE(env->error))
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Getting hanlder count for the flow %s failed", 
                    flowname);

            return AXIS2_ERROR_GET_STATUS_CODE(env->error);
        }

        for (j = 0; j < count; j++)
        {
            axis2_handler_desc_t *metadata = NULL;
            const axis2_char_t *phase_name = NULL;
            axis2_phase_rule_t *phase_rule = NULL;
            const axutil_string_t *handlername = NULL;
            const axis2_char_t *handlername_buff = NULL;

            metadata = axis2_flow_get_handler(flow, env, j);
            handlername = axis2_handler_desc_get_name(metadata, env);
            handlername_buff = axutil_string_get_buffer(handlername, env);
            phase_rule = axis2_handler_desc_get_rules(metadata, env);
            phase_name = axis2_phase_rule_get_name(phase_rule, env);
            if (!phase_name)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "Phase rules name null for the handler description %s within flow %s", 
                        handlername_buff, flowname);

                return AXIS2_FAILURE;
            }

            /* If user defined phases */
            if ((axutil_strcmp(AXIS2_PHASE_TRANSPORT_IN, phase_name))
                && (axutil_strcmp(AXIS2_PHASE_DISPATCH, phase_name)) &&
                (axutil_strcmp(AXIS2_PHASE_POST_DISPATCH, phase_name))
                && (axutil_strcmp(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
            { 
                status = axutil_array_list_add(handler_list, env, metadata);
                if (AXIS2_SUCCESS != status)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "Adding handler description %s failed for phase %s within flow %s", 
                        handlername_buff, phase_name, flowname);

                    return status;
                }
            }
            else
            {
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "Trying to add this handler %s to system pre defined phases , but those "\
                    "handlers are already added to global chain which run irrespective of the service", 
                        handlername_buff);
            }
        }
    }

    return AXIS2_SUCCESS;
}

