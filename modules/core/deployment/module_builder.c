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
 
#include <axis2_module_builder.h>
#include <axis2_arch_file_data.h>
 
/**
 * This is to convert OM->ServiceDescrption , where it first create OM from 
 * services.xml and then populate service description by using OM
 */
typedef struct axis2_module_builder_impl
{
	axis2_module_builder_t module_builder;
    axis2_module_desc_t *module_desc;
    	
} axis2_module_builder_impl_t;

#define AXIS2_INTF_TO_IMPL(module_builder) \
    ((axis2_module_builder_impl_t *) module_builder)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_module_builder_free (axis2_module_builder_t *module_builder, 
                            axis2_env_t **env);


axis2_status_t AXIS2_CALL
axis2_module_builder_populate_module(axis2_module_builder_t *module_builder,
                                        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_module_builder_process_ops(axis2_module_builder_t *module_builder,
            axis2_env_t **env,
            axis2_om_children_qname_iterator_t *op_itr);
                                
/************************** End of function prototypes ************************/

axis2_module_builder_t * AXIS2_CALL 
axis2_module_builder_create (axis2_env_t **env)
{
    axis2_module_builder_impl_t *module_builder_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	module_builder_impl = (axis2_module_builder_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_module_builder_impl_t));
	
	
	if(NULL == module_builder_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    module_builder_impl->module_builder.ops = NULL;
    
	module_builder_impl->module_builder.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_module_builder_ops_t));
	if(NULL == module_builder_impl->module_builder.ops)
    {
        axis2_module_builder_free(&(module_builder_impl->module_builder), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	module_builder_impl->module_builder.ops->free = axis2_module_builder_free;
    module_builder_impl->module_builder.ops->populate_module = 
        axis2_module_builder_populate_module;
    
	return &(module_builder_impl->module_builder);
}

axis2_module_builder_t * AXIS2_CALL 
axis2_module_builder_create_with_file_and_dep_engine_and_module (axis2_env_t **env,
                                                axis2_char_t *file_name,
                                                axis2_dep_engine_t *dep_engine,
                                                axis2_module_desc_t *module_desc)
{
    axis2_module_builder_impl_t *builder_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	builder_impl = (axis2_module_builder_impl_t *) axis2_module_builder_create(env);
	if(NULL == builder_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    builder_impl->module_builder.desc_builder = 
        axis2_desc_builder_create_with_file_and_dep_engine(env, file_name,
            dep_engine);
    if(!builder_impl->module_builder.desc_builder)
    {
        axis2_module_builder_free(&(builder_impl->module_builder), env);
        return NULL;
    }
    builder_impl->module_desc = module_desc;
    return &(builder_impl->module_builder);   
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_module_builder_free (axis2_module_builder_t *module_builder, 
                            axis2_env_t **env)
{
    axis2_module_builder_impl_t *module_builder_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(module_builder, env, AXIS2_FAILURE);
    
    module_builder_impl = AXIS2_INTF_TO_IMPL(module_builder);
    
	if(NULL != module_builder->ops)
    {
        AXIS2_FREE((*env)->allocator, module_builder->ops);
        module_builder->ops = NULL;
    }
    
    if(module_builder_impl)
    {
        AXIS2_FREE((*env)->allocator, module_builder_impl);
        module_builder_impl = NULL;
    }
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_module_builder_populate_module(axis2_module_builder_t *module_builder,
                                        axis2_env_t **env)
{
    axis2_module_builder_impl_t *builder_impl = NULL;
    axis2_om_element_t *module_element = NULL;
    axis2_om_node_t *module_node = NULL;
    axis2_om_attribute_t *module_name_att = NULL;
    axis2_qname_t *qattname = NULL;
    axis2_qname_t *qdllname = NULL;
    axis2_qname_t *qparamst = NULL;
    axis2_qname_t *qinflowst = NULL;
    axis2_qname_t *qoutflowst = NULL;
    axis2_qname_t *qinfaultflow = NULL;
    axis2_qname_t *qoutfaultflow = NULL;
    axis2_qname_t *qopst = NULL; 
    axis2_om_attribute_t *module_dll_att = NULL;
    axis2_om_children_qname_iterator_t *itr = NULL;
    axis2_om_children_qname_iterator_t *op_itr = NULL;
    axis2_om_element_t *in_flow_element = NULL;
    axis2_om_node_t *in_flow_node = NULL;
    axis2_om_element_t *out_flow_element = NULL;
    axis2_om_node_t *out_flow_node = NULL;
    axis2_om_element_t *in_fault_flow_element = NULL;
    axis2_om_node_t *in_fault_flow_node = NULL;
    axis2_om_element_t *out_fault_flow_element = NULL;
    axis2_om_node_t *out_fault_flow_node = NULL;
    struct axis2_conf *parent = NULL;
    axis2_array_list_t *ops = NULL;
    axis2_param_container_t *parent_container = NULL;
    int size = 0;
    int i = 0;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(module_builder);
    
    module_node = AXIS2_DESC_BUILDER_BUILD_OM(module_builder->desc_builder, env);
    module_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(module_node, env);
    if(!module_element)
    {
        return AXIS2_FAILURE;
    }
    /* Setting Module Name */
    qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
    module_name_att = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(module_element, env, 
        qattname);
    if(qattname)
        AXIS2_QNAME_FREE(qattname, env);
    if(NULL != module_name_att)
    {
        axis2_char_t *module_name = NULL;
        
        module_name = AXIS2_OM_ATTRIBUTE_GET_VALUE(module_name_att, env);
        if(NULL != module_name && (0 != AXIS2_STRCMP("", module_name)))
        {
            axis2_qname_t *qmodule_name = NULL;
            
            qmodule_name = axis2_qname_create(env, module_name, NULL, NULL);
            AXIS2_MODULE_DESC_SET_NAME(builder_impl->module_desc, env, qmodule_name);
            if(qmodule_name)
                AXIS2_QNAME_FREE(qmodule_name, env);
        }
        else
        {
            axis2_arch_file_data_t *file_data = NULL;
            axis2_char_t *module_name = NULL;
            axis2_qname_t *module_qname = NULL;
            
            file_data = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(module_builder->
                desc_builder->engine, env);
            module_name = AXIS2_ARCH_FILE_DATA_GET_MODULE_NAME(file_data, env);
     
            module_qname = axis2_qname_create(env, module_name, NULL, NULL);
            AXIS2_MODULE_DESC_SET_NAME(builder_impl->module_desc, env, module_qname);
            if(module_qname)
                AXIS2_QNAME_FREE(module_qname, env);
        }
    }
    else
    {
        axis2_arch_file_data_t *file_data = NULL;
        axis2_char_t *module_name = NULL;
        axis2_qname_t *module_qname = NULL;
        
        file_data = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(module_builder->
            desc_builder->engine, env);
        module_name = AXIS2_ARCH_FILE_DATA_GET_MODULE_NAME(file_data, env);
        
        module_qname = axis2_qname_create(env, module_name, NULL, NULL);
        AXIS2_MODULE_DESC_SET_NAME(builder_impl->module_desc, env, module_qname);
        if(module_qname)
            AXIS2_QNAME_FREE(module_qname, env);
    }

    /* Setting Module Dll Name , if it is there */
    
    qdllname = axis2_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
    module_dll_att = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(module_element, env, qdllname);
    if(qdllname)
        AXIS2_QNAME_FREE(qdllname, env);
    
    if(NULL != module_dll_att)
    {
        axis2_char_t *module_dll_name = NULL;
        
        
        module_dll_name = AXIS2_OM_ATTRIBUTE_GET_VALUE(module_dll_att, env);
        if(NULL != module_dll_name && (0 != AXIS2_STRCMP("", module_dll_name)))
        {
            if(NULL != module_builder->desc_builder->engine)
            {
                axis2_arch_file_data_t *file_data = NULL;
                
                file_data = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(
                    module_builder->desc_builder->engine, env);
                AXIS2_ARCH_FILE_DATA_SET_MODULE_DLL_NAME(file_data, env, 
                    module_dll_name);
                
            }
        }
    }

    /* processing Paramters */
    /* Processing service level paramters */
    
    qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
    itr = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(module_element, env,
        qparamst, module_node);
    if(qparamst)
        AXIS2_QNAME_FREE(qparamst, env);

    parent = AXIS2_MODULE_DESC_GET_PARENT(builder_impl->module_desc, env);
    if(parent)
        parent_container = parent->param_container;
    AXIS2_DESC_BUILDER_PROCESS_PARAMS(module_builder->desc_builder, env,
        itr, builder_impl->module_desc->params, parent_container);

    /* process INFLOW */
    qinflowst = axis2_qname_create(env, AXIS2_INFLOWST, NULL, NULL);
    in_flow_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(module_element,
        env, qinflowst, module_node, &in_flow_node);
    if(qinflowst)
        AXIS2_QNAME_FREE(qinflowst, env);

    if(NULL != in_flow_element && NULL != in_flow_node)
    {
        axis2_flow_t *flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(module_builder->
            desc_builder, env, in_flow_element, builder_impl->module_desc->params,
                in_flow_node);
        AXIS2_MODULE_DESC_SET_INFLOW(builder_impl->module_desc, env, flow);
    }
    
    qoutflowst = axis2_qname_create(env, AXIS2_OUTFLOWST, NULL, NULL);
    out_flow_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(module_element,
        env, qoutflowst, module_node, &out_flow_node);
    if(qoutflowst)
        AXIS2_QNAME_FREE(qoutflowst, env);

    if(NULL != out_flow_element && NULL != out_flow_node)
    {
        axis2_flow_t *flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(module_builder->
            desc_builder, env, out_flow_element, builder_impl->module_desc->params,
                out_flow_node);
        AXIS2_MODULE_DESC_SET_OUTFLOW(builder_impl->module_desc, env, flow);
    }

    qinfaultflow = axis2_qname_create(env, AXIS2_IN_FAILTFLOW, NULL, NULL);
    in_fault_flow_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(module_element,
        env, qinfaultflow, module_node, &in_fault_flow_node);
    if(qinfaultflow)
        AXIS2_QNAME_FREE(qinfaultflow, env);

    if(NULL != in_fault_flow_element && NULL != in_fault_flow_node)
    {
        axis2_flow_t *flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(module_builder->
            desc_builder, env, in_fault_flow_element, builder_impl->module_desc->
                params, in_fault_flow_node);
        AXIS2_MODULE_DESC_SET_FAULT_INFLOW(builder_impl->module_desc, env, flow);
    }
    
    qoutfaultflow = axis2_qname_create(env, AXIS2_OUT_FAILTFLOW, NULL, NULL);
    out_fault_flow_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(module_element,
        env, qoutfaultflow, module_node, &out_fault_flow_node);
    if(qoutfaultflow)
        AXIS2_QNAME_FREE(qoutfaultflow, env);

    if(NULL != out_fault_flow_element && NULL != out_fault_flow_node)
    {
        axis2_flow_t *flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(module_builder->
            desc_builder, env, out_fault_flow_element, builder_impl->module_desc->
                params, out_fault_flow_node);
        AXIS2_MODULE_DESC_SET_FAULT_OUTFLOW(builder_impl->module_desc, env, flow);
    }

    /* processing Operations */
    qopst = axis2_qname_create(env, AXIS2_OPERATIONST, NULL, NULL);
    op_itr = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(module_element, env,
        qopst, module_node);
    if(qopst)
        AXIS2_QNAME_FREE(qopst, env);
    ops = axis2_module_builder_process_ops(module_builder, env, op_itr);
    size = AXIS2_ARRAY_LIST_SIZE(ops, env);
    for(i = 0; i < size; i++)
    {
        axis2_op_t *op = NULL;
        
        op = (axis2_op_t *) AXIS2_ARRAY_LIST_GET(ops, env, i);
        AXIS2_MODULE_DESC_ADD_OP(builder_impl->module_desc, env, op);
    }
    return AXIS2_SUCCESS;
}


axis2_array_list_t *AXIS2_CALL
axis2_module_builder_process_ops(axis2_module_builder_t *module_builder,
            axis2_env_t **env,
            axis2_om_children_qname_iterator_t *op_itr)
{
    axis2_module_builder_impl_t *builder_impl = NULL;
    axis2_array_list_t *ops = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, op_itr, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(module_builder);
    
    ops = axis2_array_list_create(env, 0);
    while(AXIS2_TRUE == AXIS2_OM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(op_itr, env))
    {
        axis2_om_element_t *op_element = NULL;
        axis2_om_node_t *op_node = NULL;
        axis2_om_attribute_t *op_name_att = NULL;
        axis2_om_attribute_t *op_mep_att = NULL;
        axis2_qname_t *qattname = NULL;
        axis2_char_t *mep_url = NULL;
        axis2_char_t *op_name = NULL;
        axis2_qname_t *qopname = NULL;
        axis2_qname_t *qmsgrecv = NULL;
        axis2_qname_t *qparamst = NULL;
        axis2_qname_t *qmodulest = NULL;
        axis2_qname_t *qmep = NULL;
        axis2_om_children_qname_iterator_t *params = NULL;
        axis2_om_children_qname_iterator_t *modules = NULL;
        axis2_om_element_t *recv_element = NULL;
        axis2_om_node_t *recv_node = NULL;
        axis2_phases_info_t *info = NULL;
        axis2_op_t *op_desc = NULL;
        
        op_node = (axis2_om_node_t *) AXIS2_OM_CHILDREN_QNAME_ITERATOR_NEXT(
            op_itr, env);
        op_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(op_node, env);
        /* getting operation name */
        qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        op_name_att = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(op_element, env, qattname);
        if(qattname)
            AXIS2_QNAME_FREE(qattname, env);

        if(NULL == op_name_att)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_OP_NAME_MISSING,
                AXIS2_FAILURE);
            return NULL;
        }
        qmep = axis2_qname_create(env, AXIS2_MEP, NULL, NULL);
        op_mep_att = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(op_element, env, qmep);
        if(qmep)
            AXIS2_QNAME_FREE(qmep, env);
        
        if(NULL != op_mep_att)
        {
            mep_url = AXIS2_OM_ATTRIBUTE_GET_VALUE(op_mep_att, env);
        }
        if(NULL == mep_url)
        {
            /* assuming in-out mep */
            op_desc = axis2_op_create(env); 
        }
        else
        {
            /* TODO op_descrip = AxisOperationFactory.getOperetionDescription(mepURL); 
             * we don't have a operation constructor taking mepURL as argument.
             * do we need this? */
            op_desc = axis2_op_create(env);
        }
        
        op_name = AXIS2_OM_ATTRIBUTE_GET_VALUE(op_name_att, env);
        qopname = axis2_qname_create(env, op_name, NULL, NULL);
        AXIS2_OP_SET_QNAME(op_desc, env, qopname);
        if(qopname)
            AXIS2_QNAME_FREE(qopname, env);

        /* Operation parameters */
        qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
        params = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(op_element, env, 
            qparamst, op_node);
        if(qparamst)
            AXIS2_QNAME_FREE(qparamst, env);

        AXIS2_DESC_BUILDER_PROCESS_PARAMS(module_builder->desc_builder, env,
            params, op_desc->param_container, builder_impl->module_desc->params);
        /* setting the mep of the operation */
        
        /* loading the message receivers */
        qmsgrecv = axis2_qname_create(env, AXIS2_MESSAGERECEIVER, NULL, NULL);
        recv_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(op_element, 
            env, qmsgrecv, op_node, &recv_node);
        if(qmsgrecv)
            AXIS2_QNAME_FREE(qmsgrecv, env);
        if(NULL != recv_element && NULL != recv_node)
        {
            axis2_msg_recv_t *msg_recv = NULL;
            
            msg_recv = AXIS2_DESC_BUILDER_LOAD_MSG_RECV(module_builder->
                desc_builder, env, recv_element);
            AXIS2_OP_SET_MSG_RECEIVER(op_desc, env, msg_recv);
        }
        else
        {
            axis2_msg_recv_t *msg_recv = NULL;
            /* setting default message reciver */
            msg_recv = AXIS2_DESC_BUILDER_LOAD_DEFAULT_MSG_RECV(module_builder->
                desc_builder, env);
            AXIS2_OP_SET_MSG_RECEIVER(op_desc, env,msg_recv);
            
        }
        /* Process Module Refs */
        qmodulest = axis2_qname_create(env, AXIS2_MODULEST, NULL, NULL);
        modules = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(op_element, env, 
            qmodulest, op_node);
        if(qmodulest)
            AXIS2_QNAME_FREE(qmodulest, env);
        AXIS2_DESC_BUILDER_PROCESS_OP_MODULE_REFS(module_builder->desc_builder,
            env, modules, op_desc);
        /* setting Operation phase */
        info = AXIS2_DEP_ENGINE_GET_PHASES_INFO(module_builder->desc_builder->
            engine, env);
        AXIS2_PHASES_INFO_SET_OP_PHASES(info, env, op_desc);
        
        /* adding operation */
        AXIS2_ARRAY_LIST_ADD(ops, env, op_desc); 
    }
    return ops;
}
