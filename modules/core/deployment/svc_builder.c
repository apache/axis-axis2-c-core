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
 
#include <axis2_svc_builder.h>

 /**
 * This struct is to convert OM->ServiceDescrption , where it first create OM 
 * from services.xml and then populate service description by using OM
 */
typedef struct axis2_svc_builder_impl
{
	axis2_svc_builder_t svc_builder;
    
    axis2_svc_t *svc;
    	
} axis2_svc_builder_impl_t;

#define AXIS2_INTF_TO_IMPL(svc_builder) \
    ((axis2_svc_builder_impl_t *) svc_builder)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_svc_builder_free (axis2_svc_builder_t *svc_builder, 
                            axis2_env_t **env);

/**
 * top most method that is used to populate service from corresponding OM
 */
axis2_status_t AXIS2_CALL
axis2_svc_builder_populate_svc(axis2_svc_builder_t *svc_builder,
                                axis2_env_t **env,
                                axis2_om_node_t *svc_node);

axis2_array_list_t *
axis2_svc_builder_process_ops(axis2_svc_builder_t *svc_builder,
                    axis2_env_t **env,
                    axis2_om_children_qname_iterator_t *op_itr);


axis2_status_t AXIS2_CALL
axis2_svc_builder_process_svc_module_conf(axis2_svc_builder_t *svc_builder,
                                axis2_env_t **env,
                                axis2_om_children_qname_iterator_t *module_confs,
                                axis2_param_container_t *parent, 
                                axis2_svc_t *svc);


/**
 * To get the list og modules that is requird to be engage globally
 * @param moduleRefs  <code>java.util.Iterator</code>
 */
axis2_status_t AXIS2_CALL
axis2_svc_builder_process_module_refs(axis2_svc_builder_t *svc_builder,
                                    axis2_env_t **env,
                                    axis2_om_children_qname_iterator_t *module_refs);

/************************** End of function prototypes ************************/

axis2_svc_builder_t * AXIS2_CALL 
axis2_svc_builder_create (axis2_env_t **env)
{
    axis2_svc_builder_impl_t *svc_builder_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	svc_builder_impl = (axis2_svc_builder_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_svc_builder_impl_t));
	
	
	if(NULL == svc_builder_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    svc_builder_impl->svc_builder.desc_builder = NULL;
    svc_builder_impl->svc = NULL;
    svc_builder_impl->svc_builder.ops = NULL;
    
	svc_builder_impl->svc_builder.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_svc_builder_ops_t));
	if(NULL == svc_builder_impl->svc_builder.ops)
    {
        axis2_svc_builder_free(&(svc_builder_impl->svc_builder), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	svc_builder_impl->svc_builder.ops->free = axis2_svc_builder_free;
    svc_builder_impl->svc_builder.ops->populate_svc = 
        axis2_svc_builder_populate_svc;
    svc_builder_impl->svc_builder.ops->process_module_refs = 
        axis2_svc_builder_process_module_refs;
    
	return &(svc_builder_impl->svc_builder);
}

axis2_svc_builder_t * AXIS2_CALL 
axis2_svc_builder_create_with_file_and_dep_engine_and_svc (axis2_env_t **env,
                                                axis2_char_t *file_name,
                                                struct axis2_dep_engine *dep_engine,
                                                axis2_svc_t *svc)
{
    axis2_svc_builder_impl_t *builder_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, file_name, NULL);
    AXIS2_PARAM_CHECK((*env)->error, dep_engine, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svc, NULL);
    builder_impl = (axis2_svc_builder_impl_t *) axis2_svc_builder_create(env);
    if(NULL == builder_impl)
    {
        return NULL;
    }
    builder_impl->svc_builder.desc_builder = 
        axis2_desc_builder_create_with_file_and_dep_engine(env, file_name,
            dep_engine);
    if(!builder_impl->svc_builder.desc_builder)
    {
        axis2_svc_builder_free(&(builder_impl->svc_builder), env);
        return NULL;
    }
    builder_impl->svc = svc;
    return &(builder_impl->svc_builder);   
}

axis2_svc_builder_t * AXIS2_CALL 
axis2_svc_builder_create_with_dep_engine_and_svc (axis2_env_t **env,
                                                axis2_dep_engine_t *dep_engine,
                                                axis2_svc_t *svc)
{
    axis2_svc_builder_impl_t *builder_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, dep_engine, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svc, NULL);
    builder_impl = (axis2_svc_builder_impl_t *) axis2_svc_builder_create(env);
    if(NULL == builder_impl)
    {
        return NULL;
    }
    builder_impl->svc_builder.desc_builder = 
        axis2_desc_builder_create_with_dep_engine(env, dep_engine);
    if(!builder_impl->svc_builder.desc_builder)
    {
        axis2_svc_builder_free(&(builder_impl->svc_builder), env);
        return NULL;
    }
    builder_impl->svc = svc;
    return &(builder_impl->svc_builder);   
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_svc_builder_free (axis2_svc_builder_t *svc_builder, 
                            axis2_env_t **env)
{
    axis2_svc_builder_impl_t *svc_builder_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc_builder, env, AXIS2_FAILURE);
    
    svc_builder_impl = AXIS2_INTF_TO_IMPL(svc_builder);
    
    if(svc_builder->desc_builder)
    {
        AXIS2_DESC_BUILDER_FREE(svc_builder->desc_builder, env);
        svc_builder->desc_builder = NULL;
    }
    
    svc_builder_impl->svc = NULL;
    
	if(NULL != svc_builder->ops)
    {
        AXIS2_FREE((*env)->allocator, svc_builder->ops);
        svc_builder->ops = NULL;
    }
    
    if(svc_builder_impl)
    {
        AXIS2_FREE((*env)->allocator, svc_builder_impl);
        svc_builder_impl = NULL;
    }
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_builder_populate_svc(axis2_svc_builder_t *svc_builder,
                                axis2_env_t **env,
                                axis2_om_node_t *svc_node)
{
    axis2_svc_builder_impl_t *builder_impl = NULL;
    axis2_om_element_t *svc_element = NULL;
    axis2_om_children_qname_iterator_t *itr = NULL;
    axis2_om_children_qname_iterator_t *operation_itr = NULL;
    /*axis2_om_children_qname_iterator_t *module_configs_itr = NULL; */
    axis2_qname_t *qparamst = NULL;
    axis2_qname_t *qdesc = NULL;
    axis2_qname_t *qmodulest = NULL;
    axis2_qname_t *qinflowst = NULL;
    axis2_qname_t *qoutflowst = NULL;
    axis2_qname_t *qin_faultflowst = NULL;
    axis2_qname_t *qout_faultflowst = NULL;
    axis2_qname_t *qopst = NULL;
    axis2_qname_t *qattname = NULL;
    /*axis2_qname_t *qmodule_config = NULL; */
    axis2_status_t status = AXIS2_FAILURE;
    axis2_svc_grp_t *parent = NULL;
    axis2_om_element_t *desc_element = NULL;
    axis2_om_node_t *desc_node = NULL;
    axis2_om_children_qname_iterator_t *module_refs = NULL;
    axis2_om_node_t *in_flow_node = NULL;
    axis2_om_element_t *in_flow_element = NULL;
    axis2_om_node_t *out_flow_node = NULL;
    axis2_om_element_t *out_flow_element = NULL;
    axis2_om_node_t *in_faultflow_node = NULL;
    axis2_om_element_t *in_faultflow_element = NULL;
    axis2_om_node_t *out_faultflow_node = NULL;
    axis2_om_element_t *out_faultflow_element = NULL;
    axis2_om_attribute_t *name_attr = NULL;
    axis2_array_list_t *ops = NULL;
    axis2_char_t *svc_name = NULL;
    int i = 0;
    int size = 0;
    
    AXIS2_FUNC_PARAM_CHECK(svc_builder, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_node, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(svc_builder);

    svc_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(svc_node, env);
    /* Processing service level paramters */
    qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
    if(!qparamst)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    itr = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(svc_element, env, qparamst,
        svc_node);
    parent = AXIS2_SVC_GET_PARENT(builder_impl->svc, env);
    status = AXIS2_DESC_BUILDER_PROCESS_PARAMS(svc_builder->desc_builder, env, 
        itr, builder_impl->svc->param_container, parent->param_container);
    if(AXIS2_SUCCESS != status)
    {
        AXIS2_QNAME_FREE(qparamst, env);
        qparamst = NULL;
        return AXIS2_FAILURE;
    }

    /* process service description */
    /* TODO this code is changed in new version of axis2 java. Until that logic
     * is incorporated I comment out this part and add my own logic to set svc
     * name
     */
    /*qdesc = axis2_qname_create(env, AXIS2_DESCRIPTION, NULL, NULL);
    desc_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(svc_element, env,
        qdesc, svc_node, &desc_node);
    if(NULL != desc_element)
    {
        axis2_om_element_t *desc_value_element = NULL;
        axis2_om_node_t *desc_value_node = NULL;
        axis2_xml_writer_t *xml_writer = NULL;
        axis2_om_output_t *om_output = NULL;
        
        desc_value_element = AXIS2_OM_ELEMENT_GET_FIRST_ELEMENT(desc_element, 
            env, desc_node, &desc_value_node);
        if(NULL != desc_value_element && NULL != desc_value_node)
        {
            axis2_char_t *svc_name = NULL;
            
            xml_writer = axis2_xml_writer_create_for_memory(env, NULL, 
                AXIS2_TRUE, 0);
            om_output = axis2_om_output_create(env, xml_writer);
            status = AXIS2_OM_NODE_SERIALIZE(desc_value_node, env, om_output);
            if(status != AXIS2_SUCCESS)
            {
                return AXIS2_FAILURE;
            }
            svc_name = AXIS2_XML_WRITER_GET_XML(xml_writer, env);
            AXIS2_SVC_SET_AXIS2_SVC_NAME(builder_impl->svc, env, 
                svc_name);
            AXIS2_OM_OUTPUT_FREE(om_output, env);
        }
        else
        {
            axis2_char_t *svc_name = NULL;
            
            svc_name = AXIS2_OM_ELEMENT_GET_TEXT(desc_element, env, desc_node);
            AXIS2_SVC_SET_AXIS2_SVC_NAME(builder_impl->svc, env, 
                svc_name);
        }
    }
    */
    /* my logic to get set service name */
    qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
    name_attr = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(svc_element, env, qattname);
    svc_name = AXIS2_OM_ATTRIBUTE_GET_VALUE(name_attr, env);
    AXIS2_SVC_SET_AXIS2_SVC_NAME(builder_impl->svc, env, svc_name);
    AXIS2_QNAME_FREE(qattname, env);
    /* end of my logic */
    /* processing service wide modules which required to engage globally */
    qmodulest = axis2_qname_create(env, AXIS2_MODULEST, NULL, NULL);
    module_refs = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(svc_element, env,
        qmodulest, svc_node);
    status = axis2_svc_builder_process_module_refs(svc_builder, env, module_refs);
    if(AXIS2_SUCCESS != status)
    {
        return AXIS2_FAILURE;
    }

    /* process INFLOW */
    qinflowst = axis2_qname_create(env, AXIS2_INFLOWST, NULL, NULL);
    in_flow_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(svc_element, 
        env, qinflowst, svc_node, &in_flow_node);
    
    if(in_flow_element != NULL && NULL != in_flow_node)
    {
        axis2_flow_t *flow = NULL;
        flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(svc_builder->desc_builder, env,
            in_flow_element, builder_impl->svc->param_container, in_flow_node);
        status = AXIS2_SVC_SET_INFLOW(builder_impl->svc, env, flow);
        if(AXIS2_FAILURE == status)
        {
            return AXIS2_FAILURE;
        }
    }

    qoutflowst = axis2_qname_create(env, AXIS2_OUTFLOWST, NULL, NULL);
    out_flow_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(svc_element,
        env, qoutflowst, svc_node, &out_flow_node);
    if(NULL != out_flow_element && NULL != out_flow_node)
    {
        axis2_flow_t *flow = NULL;
        flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(svc_builder->desc_builder, env,
            out_flow_element, builder_impl->svc->param_container, out_flow_node);
        status = AXIS2_SVC_SET_OUTFLOW(builder_impl->svc, env, flow);
        if(AXIS2_FAILURE == status)
        {
            return AXIS2_FAILURE;
        }
    }

    qin_faultflowst = axis2_qname_create(env, AXIS2_IN_FAILTFLOW, NULL, NULL);
    in_faultflow_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(svc_element, 
        env, qin_faultflowst, svc_node, &in_faultflow_node);
    
    if(in_faultflow_element != NULL && NULL != in_faultflow_node)
    {
        axis2_flow_t *flow = NULL;
        flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(svc_builder->desc_builder, env,
            in_faultflow_element, builder_impl->svc->param_container, 
                in_faultflow_node);
        status = AXIS2_SVC_SET_FAULT_INFLOW(builder_impl->svc, env, flow);
        if(AXIS2_FAILURE == status)
        {
            return AXIS2_FAILURE;
        }
    }

    qout_faultflowst = axis2_qname_create(env, AXIS2_OUT_FAILTFLOW, NULL, NULL);
    out_faultflow_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(svc_element,
        env, qoutflowst, svc_node, &out_faultflow_node);
    if(NULL != out_faultflow_element && NULL != out_faultflow_node)
    {
        axis2_flow_t *flow = NULL;
        flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(svc_builder->desc_builder, env,
            out_faultflow_element, builder_impl->svc->param_container, 
                out_faultflow_node);
        status = AXIS2_SVC_SET_FAULT_OUTFLOW(builder_impl->svc, env, flow);
        if(AXIS2_FAILURE == status)
        {
            return AXIS2_FAILURE;
        }
        
    }

    /* processing operations */
    qopst = axis2_qname_create(env, AXIS2_OPERATIONST, NULL, NULL);
    operation_itr = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(svc_element, env,
        qopst, svc_node);
    
    ops = axis2_svc_builder_process_ops(svc_builder, env, operation_itr);
    size = AXIS2_ARRAY_LIST_SIZE(ops, env);
    for(i = 0; i < size; i++)
    {
        axis2_op_t *op_desc = NULL;
        axis2_array_list_t *params = NULL;
        int j = 0;
        int size = 0;
        
        op_desc = (axis2_op_t *) AXIS2_ARRAY_LIST_GET(ops, env, i);
        params = AXIS2_OP_GET_PARAMS(op_desc, env);
        /* Adding wsa-mapping into service */
        size = AXIS2_ARRAY_LIST_SIZE(params, env);
        for(j = 0; j < size; j++)
        {
            axis2_param_t *param = NULL;
            axis2_char_t *param_name = NULL;
            
            param = AXIS2_ARRAY_LIST_GET(params, env, j);
            param_name = AXIS2_PARAM_GET_NAME(param, env);
            if(0 == AXIS2_STRCMP(param_name, AXIS2_WSA_ACTION))
            {
                axis2_char_t *key = NULL;
                
                key = (axis2_char_t *) AXIS2_PARAM_GET_VALUE(param, env);
                AXIS2_SVC_ADD_MAPPING(builder_impl->svc, env, key, op_desc);
            }
        }
        AXIS2_SVC_ADD_OP(builder_impl->svc, env, op_desc);   
    }
    /*
    qmodule_config = axis2_qname_create(env, AXIS2_MODULECONFIG, NULL, NULL);
    module_configs_itr = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(svc_element,
        env, qmodule_config, svc_node);
    status = axis2_svc_builder_process_svc_module_conf(svc_builder, env, 
        module_configs_itr, builder_impl->svc->param_container, builder_impl->svc);
    */
    AXIS2_QNAME_FREE(qparamst, env);
    /*AXIS2_QNAME_FREE(qdesc, env) ;*/
    AXIS2_QNAME_FREE(qmodulest, env) ;
    AXIS2_QNAME_FREE(qinflowst, env) ;
    AXIS2_QNAME_FREE(qoutflowst, env) ;
    AXIS2_QNAME_FREE(qin_faultflowst, env) ;
    AXIS2_QNAME_FREE(qout_faultflowst, env) ;
    AXIS2_QNAME_FREE(qopst, env) ;
    /*AXIS2_QNAME_FREE(qmodule_config, env) ; */
    return status;
}

axis2_array_list_t *
axis2_svc_builder_process_ops(axis2_svc_builder_t *svc_builder,
                    axis2_env_t **env,
                    axis2_om_children_qname_iterator_t *op_itr)
{
    axis2_svc_builder_impl_t *builder_impl = NULL;
    axis2_array_list_t *ops = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc_builder, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, op_itr, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(svc_builder);
    
    ops = axis2_array_list_create(env, 10);
    while(AXIS2_TRUE == AXIS2_OM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(op_itr, env))
    {
        axis2_om_element_t *op_element = NULL;
        axis2_om_node_t *op_node = NULL;
        axis2_om_attribute_t *op_name_att = NULL;
        axis2_om_attribute_t *op_mep_att = NULL;
        axis2_qname_t *qmep = NULL;
        axis2_qname_t *qopname = NULL;
        axis2_qname_t *qparamst = NULL;
        axis2_qname_t *qmsgrecv = NULL;
        axis2_qname_t *qmodulest = NULL;
        axis2_qname_t *qmodule_config = NULL;
        axis2_qname_t *qattname = NULL;
        axis2_char_t *mep_url = NULL;
        axis2_char_t *op_name = NULL;
        axis2_wsdl_op_t *wsdl_op = NULL;
        axis2_op_t *op_desc = NULL;
        axis2_om_children_qname_iterator_t *params_itr = NULL;
        axis2_om_children_qname_iterator_t *module_itr = NULL;
        axis2_om_element_t *recv_element = NULL;
        axis2_om_node_t *recv_node = NULL;
        axis2_status_t status = AXIS2_FAILURE;
        
        op_node = AXIS2_OM_CHILDREN_QNAME_ITERATOR_NEXT(op_itr, env);
        /* getting operation name */
        op_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(op_node, env);
        qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        op_name_att = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(op_element, env, qattname);
        if(NULL == op_name_att)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_OPERATION_NAME_MISSING,
                AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        /* set the mep of the operation */
        qmep = axis2_qname_create(env, AXIS2_MEP, NULL, NULL);
        op_mep_att = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(op_element, env, qmep);
        if(NULL != op_mep_att)
        {
            mep_url = AXIS2_OM_ATTRIBUTE_GET_VALUE(op_mep_att, env);
            /*
            TODO value has to be validate
            TODO
             op_descrip.setMessageExchangePattern(mep);
            */
        }
        op_name = AXIS2_OM_ATTRIBUTE_GET_VALUE(op_name_att, env);
        qopname = axis2_qname_create(env, op_name, NULL, NULL);
        
        wsdl_op = AXIS2_SVC_GET_WSDL_OP(builder_impl->svc, env, qopname);
        if(NULL == wsdl_op)
        {
            if(NULL == mep_url)
            {
                /* assumed MEP is in-out */
                op_desc = axis2_op_create(env);
            }
            else
            {
               /* TODO uncomment this when constructor is available */
               /*op_desc = axis2_op_create(env, mep_url); */
                op_desc = axis2_op_create(env);
            }
            AXIS2_OP_SET_QNAME(op_desc, env, qopname);
        }
        else
        {
            axis2_char_t *mep = NULL;
            
            /* craeting operation from existing operation */
            mep = AXIS2_WSDL_OP_GET_MSG_EXCHANGE_PATTERN(wsdl_op, env);
            if(NULL == mep)
            {
                op_desc = axis2_op_create_with_wsdl_op(env, wsdl_op);
            }
            else
            {
                /* TODO uncomment this when constructor is available */
               /*op_desc = axis2_op_create(env, mep); */
                op_desc = axis2_op_create(env);
                AXIS2_OP_SET_WSDL_OP(op_desc, env, wsdl_op);
            } 
        }
        /* operation parameters */
        qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
        params_itr = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(op_element, env,
            qparamst, op_node);
        status = AXIS2_DESC_BUILDER_PROCESS_PARAMS(svc_builder->desc_builder, 
            env, params_itr, op_desc->param_container, builder_impl->svc->
                param_container);        
        /* loading the message receivers */
        
        qmsgrecv = axis2_qname_create(env, AXIS2_MESSAGERECEIVER, NULL, NULL);
        recv_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(op_element, 
            env, qmsgrecv, op_node, &recv_node);
        if(NULL != recv_element && NULL != recv_node)
        {
            axis2_msg_recv_t *msg_recv = NULL;
            msg_recv = AXIS2_DESC_BUILDER_LOAD_MSG_RECV(svc_builder->desc_builder,
                env, recv_element);
            AXIS2_OP_SET_MSG_RECEIVER(op_desc, env, msg_recv);
            
        }
        else
        {
            axis2_msg_recv_t *msg_recv = NULL;
            /* setting the default messgae receiver */
            msg_recv = AXIS2_DESC_BUILDER_LOAD_DEFAULT_MSG_RECV(svc_builder->
                desc_builder, env);
            AXIS2_OP_SET_MSG_RECEIVER(op_desc, env, msg_recv);
        }
        /* process module refs */
        qmodulest = axis2_qname_create(env, AXIS2_MODULEST, NULL, NULL);
        module_itr = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(op_element, env,
            qmodulest, op_node);
        status = AXIS2_DESC_BUILDER_PROCESS_OP_MODULE_REFS(svc_builder->
            desc_builder, env, module_itr, op_desc);
        if(AXIS2_SUCCESS != status)
        {
            return AXIS2_FAILURE;
        }
        /* setting operation phase */
        if(NULL != svc_builder->desc_builder->engine)
        {
            axis2_phases_info_t *info = AXIS2_DEP_ENGINE_GET_PHASES_INFO(
                svc_builder->desc_builder->engine, env);
            AXIS2_PHASES_INFO_SET_OP_PHASES(info, env, op_desc);
        }
        
        /* adding operation */
        status = AXIS2_ARRAY_LIST_ADD(ops, env, op_desc);
        
        AXIS2_QNAME_FREE(qmep, env);
        AXIS2_QNAME_FREE(qopname, env);
        AXIS2_QNAME_FREE(qparamst, env);
        AXIS2_QNAME_FREE(qmsgrecv, env);
        AXIS2_QNAME_FREE(qmodulest, env);
        /*AXIS2_QNAME_FREE(qmodule_config, env);*/
        AXIS2_QNAME_FREE(qattname, env);
    }
    return ops;
}


axis2_status_t AXIS2_CALL
axis2_svc_builder_process_svc_module_conf(axis2_svc_builder_t *svc_builder,
                                axis2_env_t **env,
                                axis2_om_children_qname_iterator_t *module_confs,
                                axis2_param_container_t *parent, 
                                axis2_svc_t *svc)
{
    while(AXIS2_TRUE == AXIS2_OM_CHILDREN_ITERATOR_HAS_NEXT(module_confs, env))
    {
        axis2_om_element_t *module_conf_element = NULL;
        axis2_om_node_t *module_conf_node = NULL;
        axis2_om_attribute_t *module_name_att = NULL;
        axis2_qname_t *qattname = NULL;
        
        module_conf_node = AXIS2_OM_CHILDREN_ITERATOR_NEXT(module_confs, env);
        module_conf_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(module_conf_node,
            env);
        qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        module_name_att = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(module_conf_element,
            env, qattname);
        if(NULL == module_name_att)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_MODULE_CONF,
                AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_svc_builder_process_module_refs(axis2_svc_builder_t *svc_builder,
                                    axis2_env_t **env,
                                    axis2_om_children_qname_iterator_t *module_refs)
{
    axis2_svc_builder_impl_t *builder_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc_builder, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_refs, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(svc_builder);
    
    while(AXIS2_TRUE == AXIS2_OM_CHILDREN_ITERATOR_HAS_NEXT(module_refs, env))
    {
        axis2_om_element_t *module_ref_element = NULL;
        axis2_om_node_t *module_ref_node = NULL;
        axis2_om_attribute_t *module_ref_att = NULL;
        axis2_qname_t *qref = NULL;
        
        module_ref_node = AXIS2_OM_CHILDREN_ITERATOR_NEXT(module_refs, env);
        module_ref_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(module_ref_node,
            env);
        qref = axis2_qname_create(env, AXIS2_REF, NULL, NULL);
        module_ref_att = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(module_ref_element,
            env, qref);
        if(NULL != module_ref_att)
        {
            axis2_char_t *ref_name = NULL;
            axis2_qname_t *qrefname = NULL;
            
            ref_name = AXIS2_OM_ATTRIBUTE_GET_VALUE(module_ref_att, env);
            qrefname = axis2_qname_create(env, ref_name, NULL, NULL);
            if(NULL == AXIS2_DEP_ENGINE_GET_MODULE(svc_builder->desc_builder->
                engine, env, qrefname))
            {
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MODULE_NOT_FOUND,
                    AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            else
            {
                AXIS2_SVC_ADD_MODULE_REF(builder_impl->svc, env, qrefname);
            }
        }
    }
    return AXIS2_SUCCESS;
}
