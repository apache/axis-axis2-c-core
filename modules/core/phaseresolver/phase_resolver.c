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
 
#include <axis2_phase_resolver.h>
    
/** 
 * @brief Wsdl phase holder struct impl
 * Phase Resolver
 * This class hold all the phases found in the services.xml and server.xml
 */ 
typedef struct axis2_phase_resolver_impl
{
	axis2_phase_resolver_t phase_resolver;
    /**
     * Field axisConfig
     */
    struct axis2_conf *axis2_config;

    /**
     * Field axisService
     */
    struct axis2_svc *svc;

    /**
     * Field phaseHolder
     */
    struct axis2_phase_holder *phase_holder;
    
} axis2_phase_resolver_impl_t;

#define AXIS2_INTF_TO_IMPL(phase_resolver) ((axis2_phase_resolver_impl_t *)phase_resolver)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_phase_resolver_free (
                axis2_phase_resolver_t *phase_resolver,
				axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_chains(axis2_phase_resolver_t *phase_resolver,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_module_op(axis2_phase_resolver_t *phase_resolver,
                                    axis2_env_t **env,
                                    struct axis2_op *op);

/**
 * this opeartion is used to build all the three cahins ,
 * so type varible is used to difrenciate them
 * type = 1 inflow
 * type = 2 out flow
 * type = 3 fault flow
 *
 * @param type
 */                                    
axis2_status_t
axis2_phase_resolver_build_execution_chains(axis2_phase_resolver_t *phase_resolver,
                                    axis2_env_t **env,
                                    int type,
                                    struct axis2_op *op);

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_transport_chains(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env);


axis2_status_t
axis2_phase_resolver_build_in_transport_chains(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_transport_in_desc *transport);

/**
 * Method buildTransportChains
 *
 * @param transport
 * @throws PhaseException
 */
axis2_status_t
axis2_phase_resolver_build_out_transport_chains(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_transport_out_desc *transport);


axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_globally(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_module_desc *module);

axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_svc_from_global(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_svc *svc,
                                                struct axis2_module_desc *module_desc);


axis2_status_t
axis2_phase_resolver_engage_to_global_chain(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_module_desc *module_desc);


axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_svc(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_svc *svc,
                                                struct axis2_module_desc *module_desc);
                                                    
axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_op(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                axis2_op_t *axis_op,
                                                axis2_module_desc_t *module_desc);

/************************** End of function prototypes ************************/

axis2_phase_resolver_t * AXIS2_CALL 
axis2_phase_resolver_create (axis2_env_t **env)
{
    axis2_phase_resolver_impl_t *phase_resolver_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	phase_resolver_impl = (axis2_phase_resolver_impl_t *) AXIS2_MALLOC((*env)->allocator,
			sizeof(axis2_phase_resolver_impl_t));
    
    if(NULL == phase_resolver_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
	
    phase_resolver_impl->phase_resolver.ops = NULL;
    phase_resolver_impl->axis2_config = NULL;
    phase_resolver_impl->svc = NULL;
    phase_resolver_impl->phase_holder = NULL;
    
	
    
	phase_resolver_impl->phase_resolver.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_phase_resolver_ops_t));
	if(NULL == phase_resolver_impl->phase_resolver.ops)
    {
        axis2_phase_resolver_free(&(phase_resolver_impl->phase_resolver), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    phase_resolver_impl->phase_resolver.ops->free =  
        axis2_phase_resolver_free;
	
    phase_resolver_impl->phase_resolver.ops->build_chains = 
        axis2_phase_resolver_build_chains;
    
    phase_resolver_impl->phase_resolver.ops->build_module_op = 
        axis2_phase_resolver_build_module_op;
    
    phase_resolver_impl->phase_resolver.ops->build_transport_chains =     
        axis2_phase_resolver_build_transport_chains;
    
    phase_resolver_impl->phase_resolver.ops->engage_module_globally = 
        axis2_phase_resolver_engage_module_globally;
        
    phase_resolver_impl->phase_resolver.ops->engage_module_to_svc_from_global = 
        axis2_phase_resolver_engage_module_to_svc_from_global;
        
    phase_resolver_impl->phase_resolver.ops->engage_module_to_svc = 
        axis2_phase_resolver_engage_module_to_svc;
        
    phase_resolver_impl->phase_resolver.ops->engage_module_to_op = 
        axis2_phase_resolver_engage_module_to_op;
    
  
	return &(phase_resolver_impl->phase_resolver);
}

axis2_phase_resolver_t * AXIS2_CALL 
axis2_phase_resolver_create_with_config (axis2_env_t **env, 
                                         struct axis2_conf *axis2_config)
{
    axis2_phase_resolver_impl_t *phase_resolver_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, axis2_config, NULL);
    
    phase_resolver_impl = (axis2_phase_resolver_impl_t *)
        axis2_phase_resolver_create(env);
    
    phase_resolver_impl->axis2_config = axis2_config;
    
    return &(phase_resolver_impl->phase_resolver);
}

axis2_phase_resolver_t * AXIS2_CALL 
axis2_phase_resolver_create_with_config_and_svc (axis2_env_t **env, 
                                                struct axis2_conf *axis2_config,
                                                struct axis2_svc *svc)
{
    axis2_phase_resolver_impl_t *phase_resolver_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, axis2_config, NULL);
    
    phase_resolver_impl = (axis2_phase_resolver_impl_t *)
        axis2_phase_resolver_create(env);
    
    if(!phase_resolver_impl)
    {
        return NULL;
    }
    phase_resolver_impl->axis2_config = axis2_config;
    
    phase_resolver_impl->svc = svc;
    
    return &(phase_resolver_impl->phase_resolver);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_phase_resolver_free (axis2_phase_resolver_t *phase_resolver, 
                            axis2_env_t **env)
{
    axis2_phase_resolver_impl_t *phase_resolver_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(phase_resolver, env, AXIS2_FAILURE);
    
    phase_resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);
    
	if(NULL != phase_resolver->ops)
    {
        AXIS2_FREE((*env)->allocator, phase_resolver->ops);
        phase_resolver->ops = NULL;
    }
    
    phase_resolver_impl->axis2_config = NULL;
    
    phase_resolver_impl->svc = NULL;
    
    if(NULL != phase_resolver_impl->phase_holder)
    {
        AXIS2_PHASE_HOLDER_FREE(phase_resolver_impl->phase_holder, env);
        phase_resolver_impl->phase_holder = NULL;
    }
    if(phase_resolver_impl)
    {
        AXIS2_FREE((*env)->allocator, phase_resolver_impl);
        phase_resolver_impl = NULL;
    }
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_chains(axis2_phase_resolver_t *phase_resolver,
                                    axis2_env_t **env) 
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_hash_index_t *index_i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_op_t *op = NULL;
    axis2_hash_t *ops = NULL;
    
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);
   
    if (!(resolver_impl->svc))
        return AXIS2_FAILURE;

    ops = AXIS2_SVC_GET_OPS(resolver_impl->svc, env);
    
    for (index_i = axis2_hash_first (ops, env); index_i; index_i = 
            axis2_hash_next (env, index_i))
    {
        void *v = NULL;
        int j = 0;
        axis2_hash_this (index_i, NULL, NULL, &v);
        op = (axis2_op_t *)v;
        for(j = 1; j < 5; j++)
        {
            status = axis2_phase_resolver_build_execution_chains(phase_resolver,
                env, j, op );
            
        }
    }

    return status;    
}

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_module_op(axis2_phase_resolver_t *phase_resolver,
                                    axis2_env_t **env,
                                    struct axis2_op *op)
{
    AXIS2_FUNC_PARAM_CHECK(phase_resolver, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op, AXIS2_FAILURE);
    int i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    for (i = 1; i < 5; i++) 
    {
        status = axis2_phase_resolver_build_execution_chains(phase_resolver, env, 
                    i, op);
    }
    return status;
}

axis2_status_t
axis2_phase_resolver_build_execution_chains(axis2_phase_resolver_t *phase_resolver,
                                    axis2_env_t **env,
                                    int type,
                                    struct axis2_op *op)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_array_list_t *all_handlers = NULL;
    axis2_array_list_t *moduleqnames = NULL;
    int i = 0;
    int size = 0;
    int status = AXIS2_FAILURE;
    struct axis2_flow *flow = NULL;    
    
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);
    
    /**************************************************************************/
    /********************* Handlers from   axis2.xml from modules *************/
    /**************************************************************************/
    
    moduleqnames = (axis2_array_list_t *) AXIS2_CONF_GET_ENGAGED_MODULES(
        resolver_impl->axis2_config, env);
    
    size = AXIS2_ARRAY_LIST_SIZE(moduleqnames, env);
    if(AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
    {
        return AXIS2_FAILURE;
    }
    
    for (i = 0; i < size; i++) 
    {
        axis2_qname_t *modulename = NULL;
        struct axis2_module_desc *module_desc = NULL;
            
        modulename = (axis2_qname_t *) AXIS2_ARRAY_LIST_GET(moduleqnames, env, i);
        module_desc = AXIS2_CONF_GET_MODULE(resolver_impl->axis2_config, env, 
            modulename);
        if (NULL != module_desc) 
        {
            switch (type) 
            {
                case AXIS2_INFLOW:
                {
                    flow = AXIS2_MODULE_DESC_GET_INFLOW(module_desc, env);
                    break;
                }
                case AXIS2_OUTFLOW:
                {
                    flow = AXIS2_MODULE_DESC_GET_OUTFLOW(module_desc, env);
                    break;
                }
                case AXIS2_FAULT_INFLOW:
                {
                    flow = AXIS2_MODULE_DESC_GET_FAULT_INFLOW(module_desc, env);
                    break;
                }
                case AXIS2_FAULT_OUTFLOW:
                {
                    flow = AXIS2_MODULE_DESC_GET_FAULT_OUTFLOW(module_desc, env);
                    break;
                }
            }
            status = AXIS2_SVC_ADD_TO_ENGAGED_MODULE_LIST(resolver_impl->svc, env, module_desc);
            if(AXIS2_FAILURE == status) 
            {
                return status;
            }
            status = AXIS2_OP_ADD_TO_ENGAGE_MODULE_LIST(op, env, module_desc);
            if(AXIS2_FAILURE == status) 
            {
                return status;
            }
            
        } else 
        {
                
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_MODULE_REF, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }

        if (NULL != flow) 
        {
            int j = 0;
            int count = 0;
            count = AXIS2_FLOW_GET_HANDLER_COUNT(flow, env);
            if(AXIS2_TRUE != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
            {
                return AXIS2_FAILURE;
            }
           
            
            if(!all_handlers)
            {
                return AXIS2_FAILURE;
            }

            for (j = 0; j < count; j++) 
            {
                struct axis2_handler_desc *metadata = NULL;
                axis2_char_t *phase_name = NULL;
                struct axis2_phase_rule *phase_rule = NULL;
                    
                metadata = AXIS2_FLOW_GET_HANDLER(flow, env, j);
                phase_rule = AXIS2_HANDLER_DESC_GET_RULES(metadata, env);
                phase_name = AXIS2_PHASE_RULE_GET_NAME(phase_rule, env);
                if(NULL != phase_name)
                {
                    return AXIS2_FAILURE;
                }
                if ((0 != AXIS2_STRCMP(AXIS2_PHASE_TRANSPORTIN, phase_name)) &&
                    (0 != AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name)) &&
                    (0 != AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) &&
                    (0 != AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                { 
                    if(!all_handlers)
                    {
                        all_handlers = axis2_array_list_create(env, 20);
                        if(!all_handlers)
                            return AXIS2_FAILURE;
                    }
                    status = AXIS2_ARRAY_LIST_ADD(all_handlers, env, metadata);
                    if(AXIS2_FAILURE == status)
                    {   
                        if(all_handlers)
                        {  
                            AXIS2_ARRAY_LIST_FREE(all_handlers, env);
                            all_handlers = NULL;                        
                        }                        
                        return status;
                    }
                } else 
                {
                    /**
                     *This handler is trying to added to system pre defined phases , but those handlers
                     * are already added to global chain which run irrespective of the service
                     *
                     */
                }
            }
        }

    }
    
    /**************************************************************************/
    /************************** SERVICE HANDLERS ******************************/
    /**************************************************************************/
    
    switch (type) 
    {
            
        case AXIS2_INFLOW:
        {
            flow = AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(resolver_impl->
                svc->wsdl_svc->wsdl_component, env, AXIS2_INFLOW_KEY);
            break;
        }
        case AXIS2_OUTFLOW:
        {
            flow = AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(resolver_impl->
                svc->wsdl_svc->wsdl_component, env, AXIS2_OUTFLOW_KEY);
            break;
        }
        case AXIS2_FAULT_INFLOW:
        {
            flow = AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(resolver_impl->
                svc->wsdl_svc->wsdl_component, env, AXIS2_IN_FAULTFLOW_KEY);
            break;
        }
        case AXIS2_FAULT_OUTFLOW:
        {
            flow = AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(resolver_impl->
                svc->wsdl_svc->wsdl_component, env, AXIS2_OUT_FAULTFLOW_KEY);
            break;
        }
    }
    
    if (NULL != flow) 
    {
        int j = 0;
        int count = 0;
        count = AXIS2_FLOW_GET_HANDLER_COUNT(flow, env);
        
        if(AXIS2_FAILURE == AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
        {
            if(all_handlers)
            {  
                AXIS2_ARRAY_LIST_FREE(all_handlers, env);
                all_handlers = NULL;                        
            }                        
            return AXIS2_FAILURE;              
        }
        for (j = 0; j < count; j++) 
        {
            struct axis2_handler_desc *metadata = NULL;
            axis2_char_t *phase_name = NULL;
                
            metadata = AXIS2_FLOW_GET_HANDLER(flow, env, j);
            phase_name = AXIS2_PHASE_RULE_GET_NAME(AXIS2_HANDLER_DESC_GET_RULES(metadata, 
                env), env);
            
            /* TODO change this in properway */
            if(0 == AXIS2_STRCMP(phase_name, ""))
            {
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_PHASE_IS_NOT_SPECIFED,
                    AXIS2_FAILURE);
                return AXIS2_FAILURE;
                
            } else if ((0 == AXIS2_STRCMP(AXIS2_PHASE_TRANSPORTIN, phase_name)) ||
                    (0 == AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name)) ||
                    (0 == AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) ||
                    (0 == AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
            {
                if(all_handlers)
                {  
                    AXIS2_ARRAY_LIST_FREE(all_handlers, env);
                    all_handlers = NULL;                        
                }
                AXIS2_ERROR_SET((*env)->error, 
                    AXIS2_ERROR_SERVICE_MODULE_CAN_NOT_REFER_GLOBAL_PHASE, AXIS2_FAILURE);
                return AXIS2_FAILURE;
                
            } else 
            {
                if(!all_handlers)
                {
                    all_handlers = axis2_array_list_create(env, 20);
                    if(!all_handlers)
                        return AXIS2_FAILURE;
                }

                status = AXIS2_ARRAY_LIST_ADD(all_handlers, env, metadata);
                
                if(AXIS2_FAILURE == status) 
                {
                    if(all_handlers)
                    {  
                        AXIS2_ARRAY_LIST_FREE(all_handlers, env);
                        all_handlers = NULL;                        
                    }
                    return status;
                }
            }
        }
    }
    else
    {
        /* no flows configured */
        return AXIS2_SUCCESS;
    }
    
    if(resolver_impl->phase_holder)
    {
        AXIS2_PHASE_HOLDER_FREE(resolver_impl->phase_holder, env);
    }
    
    switch (type) 
    {
        case AXIS2_INFLOW:
        {
            resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env, 
                AXIS2_OP_GET_REMAINING_PHASES_INFLOW(op, env));
            break;
        }
        case AXIS2_OUTFLOW:
        {
            resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env, 
                AXIS2_OP_GET_PHASES_OUTFLOW(op, env));
            break;
        }
        case AXIS2_FAULT_INFLOW:
        {
            resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env,
                AXIS2_OP_GET_PHASES_IN_FAULT_FLOW(op, env));
            break;
        }
        case AXIS2_FAULT_OUTFLOW:
        {
            resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env,
                AXIS2_OP_GET_PHASES_OUT_FAULT_FLOW(op, env));
            break;
        }
    }
    /* TODO There is no point of having this code. If no flows, it should
     * not come here. It should have returned when flows are null
    if(!all_handlers)
    {
        return AXIS2_SUCCESS;
    }
    */
    
    size = AXIS2_ARRAY_LIST_SIZE(all_handlers, env);
    if(AXIS2_TRUE != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
    {
        return AXIS2_FAILURE;
    }
    
    for (i = 0; i < size; i++) 
    {
        struct axis2_handler_desc *metadata = NULL;
            
        metadata = (struct axis2_handler_desc *)
            AXIS2_ARRAY_LIST_GET(all_handlers, env, i);
        status = AXIS2_PHASE_HOLDER_ADD_HANDLER(resolver_impl->phase_holder, env, metadata);
    }
    
    /* Free the locally created all_handlers list */
    if(all_handlers)
        AXIS2_ARRAY_LIST_FREE(all_handlers, env);
    
    return status;
}

axis2_status_t AXIS2_CALL
axis2_phase_resolver_build_transport_chains(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_hash_t *transports_in = NULL;
    axis2_hash_t *transports_out = NULL;
    axis2_hash_index_t *index_i = 0;
    struct axis2_transport_in_desc *transport_in = NULL;
    struct axis2_transport_out_desc *transport_out = NULL;
    void *v = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);
    
    transports_in = AXIS2_CONF_GET_TRANSPORTS_IN(
        resolver_impl->axis2_config, env);
    
    transports_out = AXIS2_CONF_GET_TRANSPORTS_OUT(
        resolver_impl->axis2_config, env);

    if(!transports_in || transports_out)
    {
        return AXIS2_FAILURE;
    }

    for (index_i = axis2_hash_first (transports_in, env); index_i; index_i = axis2_hash_next (env, index_i))
    {

        axis2_hash_this (index_i, NULL, NULL, &v);
        transport_in = (struct axis2_transport_in_desc *) v;
        status = axis2_phase_resolver_build_in_transport_chains(phase_resolver, env, transport_in);
        if(AXIS2_FAILURE == status)
            return status;
    }
    
    for (index_i = axis2_hash_first (transports_out, env); index_i; index_i = axis2_hash_next (env, index_i))
    {

        axis2_hash_this (index_i, NULL, NULL, &v);
        transport_out = (struct axis2_transport_out_desc *) v;
        status = axis2_phase_resolver_build_out_transport_chains(phase_resolver, env, transport_out);

    }
    
    return status;
}


axis2_status_t
axis2_phase_resolver_build_in_transport_chains(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_transport_in_desc *transport)
{
    int type = 0;
    int j = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_array_list_t *handlers = NULL;
    
    for (type = 1; type < 4; type++) 
    {
        struct axis2_flow *flow = NULL;
        struct axis2_phase *phase = NULL;

        switch (type) {
            case AXIS2_INFLOW:
            {
                flow = AXIS2_TRANSPORT_IN_DESC_GET_INFLOW(transport, env);
                phase = AXIS2_TRANSPORT_IN_DESC_GET_IN_PHASE(transport, env);
                break;
            }
            case AXIS2_FAULT_INFLOW:
            {
                flow = AXIS2_TRANSPORT_IN_DESC_GET_FAULTFLOW(transport, env);
                phase = AXIS2_TRANSPORT_IN_DESC_GET_FAULTPHASE(transport, env);
                break;
            }
        }
        if (NULL != flow) 
        {
            struct axis2_phase_holder *phase_holder = NULL;

            handlers = axis2_array_list_create(env, 20);
            for (j = 0; j < AXIS2_FLOW_GET_HANDLER_COUNT(flow, env); j++) 
            {
                struct axis2_handler_desc *metadata = NULL;

                metadata = AXIS2_FLOW_GET_HANDLER(flow, env, j);
                status = AXIS2_PHASE_RULE_SET_NAME(AXIS2_HANDLER_DESC_GET_RULES(metadata,
                    env), env, AXIS2_TRANSPORT_PHASE);
                
                if(AXIS2_FAILURE == status)
                {
                    if(handlers)
                        AXIS2_ARRAY_LIST_FREE(handlers, env);
                    return status;
                }
                status = AXIS2_ARRAY_LIST_ADD(handlers, env, metadata);
                if(AXIS2_FAILURE == status)
                {
                    if(handlers)
                        AXIS2_ARRAY_LIST_FREE(handlers, env);
                    return status;
                }
            }
            phase_holder = axis2_phase_holder_create(env);
            if(NULL == phase_holder)
            {
                if(handlers)
                    AXIS2_ARRAY_LIST_FREE(handlers, env);
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            
            status = AXIS2_PHASE_HOLDER_BUILD_TRANSPORT_HANDLER_CHAIN(phase_holder, env, 
                phase, handlers);
            if(phase_holder)
                AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
        } else 
        {
        }
    }
    if(handlers)
        AXIS2_ARRAY_LIST_FREE(handlers, env);
    return status;
}

axis2_status_t
axis2_phase_resolver_build_out_transport_chains(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_transport_out_desc *transport)
{
    int type = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_array_list_t *handlers = NULL;
        
    for (type = 1; type < 5; type++) 
    {
        struct axis2_flow *flow = NULL;
        struct axis2_phase *phase = NULL;
        
        switch (type) {
            case AXIS2_OUTFLOW:
            {
                flow = AXIS2_TRANSPORT_OUT_DESC_GET_OUTFLOW(transport, env);
                phase = AXIS2_TRANSPORT_OUT_DESC_GET_OUT_PHASE(transport, env);
                break;
            }
            case AXIS2_FAULT_OUTFLOW:
            {
                flow = AXIS2_TRANSPORT_OUT_DESC_GET_FAULTFLOW(transport, env);
                phase = AXIS2_TRANSPORT_OUT_DESC_GET_FAULTPHASE(transport, env);
                break;
            }
        }
        
        if (NULL != flow) 
        {
            struct axis2_phase_holder *phase_holder = NULL;
            int hndlr_count = 0;
            
            hndlr_count = AXIS2_FLOW_GET_HANDLER_COUNT(flow, env);
            if(AXIS2_TRUE != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
            {
                return AXIS2_FAILURE;
            }
            handlers = axis2_array_list_create(env, 20);
            int j = 0;
            for (j = 0; j < hndlr_count; j++) 
            {
                struct axis2_handler_desc *metadata = NULL;
                
                metadata = AXIS2_FLOW_GET_HANDLER(flow, env, j);
                status = AXIS2_PHASE_RULE_SET_NAME(AXIS2_HANDLER_DESC_GET_RULES(metadata,
                    env), env, AXIS2_TRANSPORT_PHASE);
                
                if(AXIS2_FAILURE == status)
                {
                    if(handlers)
                        AXIS2_ARRAY_LIST_FREE(handlers, env);
                    return status;
                }
                
                status = AXIS2_ARRAY_LIST_ADD(handlers, env, metadata);
                if(AXIS2_FAILURE == status)
                {
                    if(handlers)
                        AXIS2_ARRAY_LIST_FREE(handlers, env);
                    return status;
                }
            }
            phase_holder = axis2_phase_holder_create(env);
            if(NULL == phase_holder)
            {
                if(handlers)
                    AXIS2_ARRAY_LIST_FREE(handlers, env);
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            
            status = AXIS2_PHASE_HOLDER_BUILD_TRANSPORT_HANDLER_CHAIN(phase_holder, env, 
                phase, handlers);
            if(phase_holder)
                AXIS2_PHASE_HOLDER_FREE(phase_holder, env);
        } else 
        {
            /* Do nothing */
        }
    }
    if(handlers)
        AXIS2_ARRAY_LIST_FREE(handlers, env);
    return status;
}


axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_globally(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_module_desc *module)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_hash_t *svc_grps = NULL;
    axis2_hash_index_t *index_i = NULL;
    axis2_status_t status = AXIS2_FAILURE;
        
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);
    
    status = axis2_phase_resolver_engage_to_global_chain(phase_resolver, env, module);
    if(AXIS2_FAILURE == status)
        return status;
    svc_grps = AXIS2_CONF_GET_SVC_GRPS(resolver_impl->axis2_config, env);
    if(!svc_grps)
    {
        return AXIS2_FAILURE;
    }
    index_i = axis2_hash_first (svc_grps, env);
    while(NULL != index_i)
    {
        axis2_hash_t *svcs = NULL;
        struct axis2_svc_grp *svc_grp = NULL;
        void *v = NULL;
        axis2_hash_index_t *index_j = NULL;
        axis2_qname_t *mod_name = NULL; 

        axis2_hash_this (index_i, NULL, NULL, &v);
        svc_grp = (struct axis2_svc_grp *) v;
        svcs = AXIS2_SVC_GRP_GET_SVCS(svc_grp, env);    
        
        index_j = axis2_hash_first (svcs, env);
        while(NULL != index_j)
        {
            struct axis2_svc *svc = NULL;
            void *w = NULL;

            axis2_hash_this (index_i, NULL, NULL, &w);
            svc = (struct axis2_svc *) w;
                
            status = AXIS2_SVC_ADD_MODULE_OPS(svc, env, module, 
                resolver_impl->axis2_config);
            if(AXIS2_FAILURE == status)
            {
                return status;
            }
            status = axis2_phase_resolver_engage_module_to_svc_from_global(
                phase_resolver, env, svc, module);
            
            if(AXIS2_FAILURE == status)
            {
                return status;
            }
            
            status = AXIS2_SVC_ADD_TO_ENGAGED_MODULE_LIST(svc, env, module);
            if(AXIS2_FAILURE == status)
            {
                return status;
            }
            index_j = axis2_hash_next (env, index_j);
            
        }         
        mod_name = AXIS2_MODULE_DESC_GET_NAME(module, env);
        status = AXIS2_SVC_GRP_ADD_MODULE(svc_grp, env, 
            mod_name);
        
        if(AXIS2_FAILURE == status)
        {
            return status;
        }
        index_i = axis2_hash_next (env, index_i);
           
    }     
    return status;
}

axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_svc_from_global(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_svc *svc,
                                                struct axis2_module_desc *module_desc)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_hash_t *ops = NULL;
    axis2_bool_t engaged = AXIS2_FALSE;
    axis2_hash_index_t *index_i = NULL;
    int type = 0;
    axis2_status_t status = AXIS2_FAILURE;
    
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);
    
    ops = AXIS2_SVC_GET_OPS(svc, env);
    for (index_i = axis2_hash_first (ops, env); index_i; index_i = axis2_hash_next (env, index_i))
    {
        void *v = NULL;
        struct axis2_op *op_desc = NULL;
        axis2_array_list_t *modules = NULL;
        struct axis2_flow *flow = NULL;

        axis2_hash_this (index_i, NULL, NULL, &v);
        op_desc = (struct axis2_op *) v;
        modules = AXIS2_OP_GET_MODULES(op_desc, env);
        int j = 0;
        for(j = 0; j < AXIS2_ARRAY_LIST_SIZE(modules, env); j++)
        {
            struct axis2_module_desc *module_desc_l = NULL;

            module_desc_l = (struct axis2_module_desc *) 
                AXIS2_ARRAY_LIST_GET(modules, env, j);
            if(AXIS2_QNAME_EQUALS(AXIS2_MODULE_DESC_GET_NAME(module_desc_l, env),
                env, AXIS2_MODULE_DESC_GET_NAME(module_desc, env)))
            {
                engaged = AXIS2_TRUE;
                break;
            }
        }
        if(AXIS2_TRUE == engaged)
        {
            continue;
        }
        for(type = 1; type < 5; type++)
        {
            switch (type) {
                case AXIS2_INFLOW:
                {
                    resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env,
                        AXIS2_OP_GET_REMAINING_PHASES_INFLOW(op_desc, env));
                    break;
                }
                case AXIS2_OUTFLOW:
                {
                    resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env,
                        AXIS2_OP_GET_PHASES_OUTFLOW(op_desc, env));
                    break;
                }
                case AXIS2_FAULT_INFLOW:
                {
                    resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env,
                        AXIS2_OP_GET_PHASES_IN_FAULT_FLOW(op_desc, env));
                    break;
                }
                case AXIS2_FAULT_OUTFLOW:
                {
                    resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env,
                        AXIS2_OP_GET_PHASES_OUT_FAULT_FLOW(op_desc, env));
                    break;
                }
            }
            /******************************************************************/
            /******************* Modules refered by axis2.xml *****************/
            /******************************************************************/
            switch (type) 
            {
                case AXIS2_INFLOW:
                {
                    flow = AXIS2_MODULE_DESC_GET_INFLOW(module_desc, env);
                    break;
                }
                case AXIS2_OUTFLOW:
                {
                    flow = AXIS2_MODULE_DESC_GET_OUTFLOW(module_desc, env);
                    break;
                }
                case AXIS2_FAULT_INFLOW:
                {
                    flow = AXIS2_MODULE_DESC_GET_FAULT_INFLOW(module_desc, env);
                    break;
                }
                case AXIS2_FAULT_OUTFLOW:
                {
                    flow = AXIS2_MODULE_DESC_GET_FAULT_OUTFLOW(module_desc, env);
                    break;
                }
            }
            if (NULL != flow) 
            {
                for (j = 0; j < AXIS2_FLOW_GET_HANDLER_COUNT(flow, env); j++) 
                {
                    struct axis2_handler_desc *metadata = NULL;    
                    axis2_char_t *phase_name = NULL;

                    metadata = AXIS2_FLOW_GET_HANDLER(flow, env, j);
                    phase_name = AXIS2_PHASE_RULE_GET_NAME(
                        AXIS2_HANDLER_DESC_GET_RULES(metadata, env), env);
                    if ((0 != AXIS2_STRCMP(AXIS2_PHASE_TRANSPORTIN, phase_name)) &&
                        (0 != AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name)) &&
                        (0 != AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) &&
                        (0 != AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                    {
                        status = AXIS2_PHASE_HOLDER_ADD_HANDLER(resolver_impl->phase_holder,
                            env, metadata);
                        if(AXIS2_FAILURE == status)
                        {
                            return status;
                        }
                    }
                }
            }
        }
        status = AXIS2_OP_ADD_TO_ENGAGE_MODULE_LIST(op_desc, env, module_desc);
    }
    
    return status;
}


axis2_status_t
axis2_phase_resolver_engage_to_global_chain(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_module_desc *module_desc)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    int type = 0;
    
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);
    
    for (type = 1; type < 5; type++) 
    {
        struct axis2_flow *flow = NULL;

        switch (type) 
        {
            case AXIS2_INFLOW:
            {
                resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env,
                    AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(
                        resolver_impl->axis2_config, env));
                break;
            }
            case AXIS2_OUTFLOW:
            {
                resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env,
                    AXIS2_CONF_GET_OUTFLOW(resolver_impl->axis2_config,
                        env));
                break;
            }
            case AXIS2_FAULT_INFLOW:
            {
                resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env,
                    AXIS2_CONF_GET_IN_FAULT_FLOW(resolver_impl->axis2_config,
                        env));
                break;
            }
            case AXIS2_FAULT_OUTFLOW:
            {
                resolver_impl->phase_holder = axis2_phase_holder_create_with_phases(env,
                    AXIS2_CONF_GET_OUT_FAULT_FLOW(resolver_impl->axis2_config,
                        env));
                break;
            }
        }
        /**********************************************************************/
        /****************** Modules refered by axis2.xml **********************/
        /**********************************************************************/
        switch (type) 
        {
            case AXIS2_INFLOW:
            {
                flow = AXIS2_MODULE_DESC_GET_INFLOW(module_desc, env);
                break;
            }
            case AXIS2_OUTFLOW:
            {
                flow = AXIS2_MODULE_DESC_GET_OUTFLOW(module_desc, env);
                break;
            }
            case AXIS2_FAULT_INFLOW:
            {
                flow = AXIS2_MODULE_DESC_GET_FAULT_INFLOW(module_desc, env);
                break;
            }
            case AXIS2_FAULT_OUTFLOW:
            {
                flow = AXIS2_MODULE_DESC_GET_FAULT_OUTFLOW(module_desc, env);
                break;
            }
        }
        if (NULL != flow) 
        {
            int j = 0;
            for (j = 0; j < AXIS2_FLOW_GET_HANDLER_COUNT(flow, env); j++) 
            {
                struct axis2_handler_desc *metadata = NULL;
                axis2_char_t *phase_name = NULL;

                metadata = AXIS2_FLOW_GET_HANDLER(flow, env, j);
                phase_name = AXIS2_PHASE_RULE_GET_NAME(
                        AXIS2_HANDLER_DESC_GET_RULES(metadata, env), env);
                if ((0 != AXIS2_STRCMP(AXIS2_PHASE_TRANSPORTIN, phase_name)) &&
                    (0 != AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name)) &&
                    (0 != AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) &&
                    (0 != AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                {
                    AXIS2_PHASE_HOLDER_ADD_HANDLER(resolver_impl->phase_holder,
                        env, metadata);
          
                } 
                else 
                {
                    /**
                     * These handlers will go to op's handler chains , since the module
                     * try to add handlres to both sytem predefined phases and user defined phase
                     * so global module can do that. here the global module are the module which are
                     * reffred by axis2.xml
                     */
                }
            }
        }
    }
    
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_svc(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_svc *svc,
                                                struct axis2_module_desc *module_desc)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    axis2_hash_t *ops = NULL;
    axis2_bool_t engaged = AXIS2_FALSE;
    axis2_hash_index_t *index_i = NULL;
    void *v = NULL;
    int j = 0;
    axis2_status_t status = AXIS2_FAILURE;
        
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);
    
    ops = AXIS2_SVC_GET_OPS(resolver_impl->svc, env);
    if(!ops)
    {
        return AXIS2_FAILURE;
    }
    status = AXIS2_SVC_ADD_MODULE_OPS(resolver_impl->svc, env, module_desc, 
            resolver_impl->axis2_config);

    if(AXIS2_FAILURE == status)
    {
        return AXIS2_FAILURE;
    }
    for (index_i = axis2_hash_first (ops, env); index_i; index_i = 
            axis2_hash_next (env, index_i))
    {
        axis2_array_list_t *modules = NULL;
        struct axis2_op *op_desc = NULL;

        axis2_hash_this (index_i, NULL, NULL, &v);
        op_desc = (struct axis2_op *) v;
        modules = AXIS2_OP_GET_MODULES(op_desc, env);
        for(j = 0; j < AXIS2_ARRAY_LIST_SIZE(modules, env); j++)
        {
            struct axis2_module_desc *module_desc_l = NULL;

            module_desc_l = AXIS2_ARRAY_LIST_GET(modules, env, j);
            if(0 == AXIS2_QNAME_EQUALS(AXIS2_MODULE_DESC_GET_NAME(module_desc, env),
                    env, AXIS2_MODULE_DESC_GET_NAME(module_desc_l, env)))
            {
                engaged = AXIS2_TRUE;
                break;
            }
            if(AXIS2_FALSE == engaged)
            {
                status = axis2_phase_resolver_engage_module_to_op(
                    phase_resolver, env, op_desc, module_desc);
                if(AXIS2_FAILURE == status)
                    return status;
                
                status = AXIS2_OP_ADD_TO_ENGAGE_MODULE_LIST(op_desc, env, 
                    module_desc);
            }
        }

    }
    return status;
}


axis2_status_t AXIS2_CALL
axis2_phase_resolver_engage_module_to_op(axis2_phase_resolver_t *phase_resolver,
                                                axis2_env_t **env,
                                                struct axis2_op *axis_op,
                                                struct axis2_module_desc *module_desc)
{
    axis2_phase_resolver_impl_t *resolver_impl = NULL;
    int type = 0;
    
    AXIS2_FUNC_PARAM_CHECK(phase_resolver, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, axis_op, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_desc, AXIS2_FAILURE);
    resolver_impl = AXIS2_INTF_TO_IMPL(phase_resolver);
       
    for (type = 1; type < 5; type++) 
    {
        axis2_flow_t *flow = NULL;
        axis2_array_list_t *phases = NULL;
        switch (type) 
        {
            case AXIS2_INFLOW:
            {
                phases = AXIS2_OP_GET_REMAINING_PHASES_INFLOW(axis_op, env);
             
                resolver_impl->phase_holder = 
                    axis2_phase_holder_create_with_phases(env, phases);
                break;
            }
            case AXIS2_OUTFLOW:
            {
                phases = AXIS2_OP_GET_PHASES_OUTFLOW(axis_op, env);
                if(phases)
                {
                    resolver_impl->phase_holder = 
                        axis2_phase_holder_create_with_phases(env, phases);
                }
                break;
            }
            case AXIS2_FAULT_INFLOW:
            {
                phases = AXIS2_OP_GET_PHASES_IN_FAULT_FLOW(axis_op, env);
                resolver_impl->phase_holder = 
                    axis2_phase_holder_create_with_phases(env, phases);
                break;
            }
            case AXIS2_FAULT_OUTFLOW:
            {
                phases = AXIS2_OP_GET_PHASES_OUT_FAULT_FLOW(axis_op, env);
                resolver_impl->phase_holder = 
                    axis2_phase_holder_create_with_phases(env, phases);
                break;
            }
        }

        switch (type) 
        {
            case AXIS2_INFLOW:
            {
                flow = AXIS2_MODULE_DESC_GET_INFLOW(module_desc, env);
                break;
            }
            case AXIS2_OUTFLOW:
            {
                flow = AXIS2_MODULE_DESC_GET_OUTFLOW(module_desc, env);
                break;
            }
            case AXIS2_FAULT_INFLOW:
            {
                flow = AXIS2_MODULE_DESC_GET_FAULT_INFLOW(module_desc, env);
                break;
            }
            case AXIS2_FAULT_OUTFLOW:
            {
                flow = AXIS2_MODULE_DESC_GET_FAULT_OUTFLOW(module_desc, env);
                break;
            }
        }
        if (NULL != flow) 
        {
            int j = 0;
            
            for (j = 0; j < AXIS2_FLOW_GET_HANDLER_COUNT(flow, env); j++) 
            {
                axis2_handler_desc_t *metadata = NULL;
                axis2_char_t *phase_name = NULL;
                axis2_phase_rule_t *phase_rule = NULL;

                metadata = AXIS2_FLOW_GET_HANDLER(flow, env, j);
                phase_rule = AXIS2_HANDLER_DESC_GET_RULES(metadata, env);
                phase_name = AXIS2_PHASE_RULE_GET_NAME(phase_rule, env);
                if ((0 != AXIS2_STRCMP(AXIS2_PHASE_TRANSPORTIN, phase_name)) &&
                    (0 != AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name)) &&
                    (0 != AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) &&
                    (0 != AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name)))
                {
                    AXIS2_PHASE_HOLDER_ADD_HANDLER(resolver_impl->phase_holder, 
                        env, metadata);
           
                } 
                else 
                {
                    AXIS2_ERROR_SET((*env)->error, 
                        AXIS2_ERROR_SERVICE_MODULE_CAN_NOT_REFER_GLOBAL_PHASE, 
                            AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                    
                }
            }
        }
    }
    return AXIS2_SUCCESS;
}
