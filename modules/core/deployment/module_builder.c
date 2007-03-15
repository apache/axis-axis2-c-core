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

#include <axis2_module_builder.h>
#include <axis2_arch_file_data.h>

struct axis2_module_builder
{
    axis2_module_desc_t *module_desc;
    struct axis2_desc_builder *desc_builder;
};

static axis2_array_list_t *AXIS2_CALL
axis2_module_builder_process_ops(
    axis2_module_builder_t *module_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *op_itr);

AXIS2_EXTERN axis2_module_builder_t *AXIS2_CALL
axis2_module_builder_create(
    const axis2_env_t *env)
{
    axis2_module_builder_t *module_builder = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    module_builder = (axis2_module_builder_t *) AXIS2_MALLOC(env->
            allocator, sizeof(axis2_module_builder_t));


    if (NULL == module_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    return module_builder;
}

AXIS2_EXTERN axis2_module_builder_t *AXIS2_CALL
axis2_module_builder_create_with_file_and_dep_engine_and_module(
    const axis2_env_t *env,
    axis2_char_t *file_name,
    axis2_dep_engine_t *dep_engine,
    axis2_module_desc_t *module_desc)
{
    axis2_module_builder_t *module_builder = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    module_builder = (axis2_module_builder_t *) axis2_module_builder_create(env);
    if (NULL == module_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    module_builder->desc_builder =
        axis2_desc_builder_create_with_file_and_dep_engine(env, file_name,
                dep_engine);
    if (!module_builder->desc_builder)
    {
        axis2_module_builder_free(module_builder, env);
        return NULL;
    }
    module_builder->module_desc = module_desc;
    return module_builder;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_module_builder_free(
    axis2_module_builder_t *module_builder,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (module_builder->desc_builder)
    {
        AXIS2_DESC_BUILDER_FREE(module_builder->desc_builder, env);
    }

    if (module_builder)
    {
        AXIS2_FREE(env->allocator, module_builder);
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_module_builder_populate_module(
    axis2_module_builder_t *module_builder,
    const axis2_env_t *env)
{
    axiom_element_t *module_element = NULL;
    axiom_node_t *module_node = NULL;
    axis2_qname_t *qdllname = NULL;
    axis2_qname_t *qparamst = NULL;
    axis2_qname_t *qinflowst = NULL;
    axis2_qname_t *qoutflowst = NULL;
    axis2_qname_t *qinfaultflow = NULL;
    axis2_qname_t *qoutfaultflow = NULL;
    axis2_qname_t *qopst = NULL;
    axiom_attribute_t *module_dll_att = NULL;
    axiom_children_qname_iterator_t *itr = NULL;
    axiom_children_qname_iterator_t *op_itr = NULL;
    axiom_element_t *in_flow_element = NULL;
    axiom_node_t *in_flow_node = NULL;
    axiom_element_t *out_flow_element = NULL;
    axiom_node_t *out_flow_node = NULL;
    axiom_element_t *in_fault_flow_element = NULL;
    axiom_node_t *in_fault_flow_node = NULL;
    axiom_element_t *out_fault_flow_element = NULL;
    axiom_node_t *out_fault_flow_node = NULL;
    axis2_conf_t *parent = NULL;
    axis2_array_list_t *ops = NULL;
    axis2_param_container_t *parent_container = NULL;
    int size = 0;
    int i = 0;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "axis2_module_builder_populate_module start");

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    module_node = AXIS2_DESC_BUILDER_BUILD_OM(module_builder->desc_builder, env);
    module_element = AXIOM_NODE_GET_DATA_ELEMENT(module_node, env);
    if (!module_element)
    {
        return AXIS2_FAILURE;
    }
    /* Setting Module Name */
    /*qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
    module_name_att = AXIOM_ELEMENT_GET_ATTRIBUTE(module_element, env, 
        qattname);
    if(qattname)
        AXIS2_QNAME_FREE(qattname, env);
    if( module_name_att)
    {
        axis2_char_t *module_name = NULL;
        
        module_name = axiom_attribute_get_value(module_name_att, env);
        if( module_name && (0 != AXIS2_STRCMP("", module_name)))
        {
            axis2_qname_t *qmodule_name = NULL;
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Populate module %s", module_name);
        
            qmodule_name = axis2_qname_create(env, module_name, NULL, NULL);
            AXIS2_MODULE_DESC_SET_NAME(module_builder->module_desc, env, qmodule_name);
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
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Populate module %s", module_name);
     
            module_qname = axis2_qname_create(env, module_name, NULL, NULL);
            AXIS2_MODULE_DESC_SET_NAME(module_builder->module_desc, env, module_qname);
            if(module_qname)
                AXIS2_QNAME_FREE(module_qname, env);
        }
    }
    else
    */
    {
        axis2_arch_file_data_t *file_data = NULL;
        axis2_char_t *module_name = NULL;
        axis2_qname_t *module_qname = NULL;

        file_data = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(
                axis2_desc_builder_get_dep_engine(module_builder->desc_builder, env), 
                env);
        module_name = AXIS2_ARCH_FILE_DATA_GET_MODULE_NAME(file_data, env);

        module_qname = axis2_qname_create(env, module_name, NULL, NULL);
        axis2_module_desc_set_qname(module_builder->module_desc, env, module_qname);

        if (module_qname)
            AXIS2_QNAME_FREE(module_qname, env);
    }

    /* Setting Module Dll Name , if it is there */

    qdllname = axis2_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
    module_dll_att = AXIOM_ELEMENT_GET_ATTRIBUTE(module_element, env, qdllname);
    if (qdllname)
        AXIS2_QNAME_FREE(qdllname, env);

    if (module_dll_att)
    {
        axis2_char_t *class_name = NULL;


        class_name = axiom_attribute_get_value(module_dll_att, env);
        if (class_name && (0 != AXIS2_STRCMP("", class_name)))
        {
            axis2_dep_engine_t *dep_engine = 
                axis2_desc_builder_get_dep_engine(module_builder->desc_builder, env);
            if (dep_engine)
            {
                axis2_arch_file_data_t *file_data = NULL;

                file_data = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(
                            dep_engine, env);
                AXIS2_ARCH_FILE_DATA_SET_MODULE_DLL_NAME(file_data, env,
                        class_name);

            }
        }
    }

    /* Processing Paramters */
    /* Processing service level paramters */

    qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
    itr = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(module_element, env,
            qparamst, module_node);
    if (qparamst)
        AXIS2_QNAME_FREE(qparamst, env);

    parent = axis2_module_desc_get_parent(module_builder->module_desc, env);

    if (parent)
        parent_container = axis2_conf_get_param_container(parent, env);
    AXIS2_DESC_BUILDER_PROCESS_PARAMS(module_builder->desc_builder, env,
            itr, axis2_module_desc_get_param_container(module_builder->module_desc, env), 
            parent_container);

    /* Process IN_FLOW */
    qinflowst = axis2_qname_create(env, AXIS2_IN_FLOW_START, NULL, NULL);
    in_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(module_element,
            env, qinflowst, module_node, &in_flow_node);
    if (qinflowst)
        AXIS2_QNAME_FREE(qinflowst, env);

    if (in_flow_element && NULL != in_flow_node)
    {
        axis2_flow_t *flow = NULL;

        flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(module_builder->
                desc_builder, env, in_flow_element, 
                axis2_module_desc_get_param_container(module_builder->module_desc, env),
                in_flow_node);
        status = axis2_module_desc_set_in_flow(module_builder->module_desc, env, flow);

        if (AXIS2_SUCCESS != status)
        {
            if (flow)
                axis2_flow_free(flow, env);
            return status;
        }
    }

    qoutflowst = axis2_qname_create(env, AXIS2_OUT_FLOW_START, NULL, NULL);
    out_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(module_element,
            env, qoutflowst, module_node, &out_flow_node);
    if (qoutflowst)
        AXIS2_QNAME_FREE(qoutflowst, env);

    if (out_flow_element && NULL != out_flow_node)
    {
        axis2_flow_t *flow = NULL;

        flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(module_builder->
                desc_builder, env, out_flow_element, 
                axis2_module_desc_get_param_container(module_builder->module_desc, env),
                out_flow_node);
        status = axis2_module_desc_set_out_flow(module_builder->module_desc, env, flow);

        if (AXIS2_SUCCESS != status)
        {
            axis2_flow_free(flow, env);
            return status;
        }
    }

    qinfaultflow = axis2_qname_create(env, AXIS2_IN_FAILTFLOW, NULL, NULL);
    in_fault_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(module_element,
            env, qinfaultflow, module_node, &in_fault_flow_node);
    if (qinfaultflow)
        AXIS2_QNAME_FREE(qinfaultflow, env);

    if (in_fault_flow_element && NULL != in_fault_flow_node)
    {
        axis2_flow_t *flow = NULL;

        flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(module_builder->
                desc_builder, env, in_fault_flow_element, 
                axis2_module_desc_get_param_container(module_builder->module_desc, env),
                in_fault_flow_node);
        status = axis2_module_desc_set_fault_in_flow(module_builder->module_desc, env, flow);

        if (AXIS2_SUCCESS != status)
        {
            axis2_flow_free(flow, env);
            return status;
        }
    }

    qoutfaultflow = axis2_qname_create(env, AXIS2_OUT_FAILTFLOW, NULL, NULL);
    out_fault_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(module_element,
            env, qoutfaultflow, module_node, &out_fault_flow_node);
    if (qoutfaultflow)
        AXIS2_QNAME_FREE(qoutfaultflow, env);

    if (out_fault_flow_element && NULL != out_fault_flow_node)
    {
        axis2_flow_t *flow = NULL;

        flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(module_builder->
                desc_builder, env, out_fault_flow_element, 
                axis2_module_desc_get_param_container(module_builder->module_desc, env),
                out_fault_flow_node);
        status = axis2_module_desc_set_fault_out_flow(module_builder->module_desc,
                env, flow);
        if (AXIS2_SUCCESS != status)
        {
            axis2_flow_free(flow, env);
            return status;
        }
    }

    /* Processing Operations */
    qopst = axis2_qname_create(env, AXIS2_OPERATIONST, NULL, NULL);
    op_itr = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(module_element, env,
            qopst, module_node);
    if (qopst)
        AXIS2_QNAME_FREE(qopst, env);
    ops = axis2_module_builder_process_ops(module_builder, env, op_itr);
    size = axis2_array_list_size(ops, env);
    for (i = 0; i < size; i++)
    {
        axis2_op_t *op_desc = NULL;

        op_desc = (axis2_op_t *) axis2_array_list_get(ops, env, i);
        axis2_module_desc_add_op(module_builder->module_desc, env, op_desc);
    }
    axis2_array_list_free(ops, env);
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI,
            "axis2_module_builder_populate_module end");
    return AXIS2_SUCCESS;
}


static axis2_array_list_t *AXIS2_CALL
axis2_module_builder_process_ops(
    axis2_module_builder_t *module_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *op_itr)
{
    axis2_array_list_t *ops = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, op_itr, NULL);

    ops = axis2_array_list_create(env, 0);
    while (AXIS2_TRUE == axiom_children_qname_iterator_has_next(op_itr, env))
    {
        axiom_element_t *op_element = NULL;
        axiom_node_t *op_node = NULL;
        axiom_attribute_t *op_name_att = NULL;
        axiom_attribute_t *op_mep_att = NULL;
        axis2_qname_t *qattname = NULL;
        axis2_char_t *mep_url = NULL;
        axis2_char_t *op_name = NULL;
        axis2_qname_t *qopname = NULL;
        axis2_qname_t *qmsgrecv = NULL;
        axis2_qname_t *qparamst = NULL;
        axis2_qname_t *qmodulest = NULL;
        axis2_qname_t *qmep = NULL;
        axiom_children_qname_iterator_t *params = NULL;
        axiom_children_qname_iterator_t *modules = NULL;
        axiom_element_t *recv_element = NULL;
        axiom_node_t *recv_node = NULL;
        axis2_phases_info_t *info = NULL;
        axis2_op_t *op_desc = NULL;

        op_node = (axiom_node_t *) axiom_children_qname_iterator_next(
                    op_itr, env);
        op_element = AXIOM_NODE_GET_DATA_ELEMENT(op_node, env);
        /* getting operation name */
        qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        op_name_att = AXIOM_ELEMENT_GET_ATTRIBUTE(op_element, env, qattname);
        if (qattname)
            AXIS2_QNAME_FREE(qattname, env);

        if (NULL == op_name_att)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_OP_NAME_MISSING,
                    AXIS2_FAILURE);
            return NULL;
        }
        qmep = axis2_qname_create(env, AXIS2_MEP, NULL, NULL);
        op_mep_att = AXIOM_ELEMENT_GET_ATTRIBUTE(op_element, env, qmep);
        if (qmep)
            AXIS2_QNAME_FREE(qmep, env);

        if (op_mep_att)
        {
            mep_url = axiom_attribute_get_value(op_mep_att, env);
        }
        if (NULL == mep_url)
        {
            /* Assuming in-out mep */
            op_desc = axis2_op_create_from_module(env);
        }
        else
        {
            /* TODO
             * We don't have a operation constructor taking mepURL as argument.
             * do we need this? */
            op_desc = axis2_op_create_from_module(env);
        }

        op_name = axiom_attribute_get_value(op_name_att, env);
        qopname = axis2_qname_create(env, op_name, NULL, NULL);
        axis2_op_set_qname(op_desc, env, qopname);
        if (qopname)
            AXIS2_QNAME_FREE(qopname, env);

        /* Operation parameters */
        qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
        params = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(op_element, env,
                qparamst, op_node);
        if (qparamst)
            AXIS2_QNAME_FREE(qparamst, env);

        AXIS2_DESC_BUILDER_PROCESS_PARAMS(module_builder->desc_builder, env,
                params, 
                axis2_op_get_param_container(op_desc, env), 
                axis2_module_desc_get_param_container(module_builder->module_desc, env));
        /* To process wsamapping */
        AXIS2_DESC_BUILDER_PROCESS_ACTION_MAPPINGS(module_builder->desc_builder, 
            env, op_node, op_desc);

        /* setting the mep of the operation */

        /* loading the message receivers */
        qmsgrecv = axis2_qname_create(env, AXIS2_MESSAGERECEIVER, NULL, NULL);
        recv_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(op_element,
                env, qmsgrecv, op_node, &recv_node);
        if (qmsgrecv)
            AXIS2_QNAME_FREE(qmsgrecv, env);
        if (recv_element && NULL != recv_node)
        {
            axis2_msg_recv_t *msg_recv = NULL;

            msg_recv = AXIS2_DESC_BUILDER_LOAD_MSG_RECV(module_builder->
                    desc_builder, env, recv_element);
            axis2_op_set_msg_recv(op_desc, env, msg_recv);
        }
        else
        {
            axis2_msg_recv_t *msg_recv = NULL;
            /* setting default message reciver */
            msg_recv = axis2_desc_builder_load_default_msg_recv(env);
            axis2_op_set_msg_recv(op_desc, env, msg_recv);

        }
        /* Process Module Refs */
        qmodulest = axis2_qname_create(env, AXIS2_MODULEST, NULL, NULL);
        modules = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(op_element, env,
                qmodulest, op_node);
        if (qmodulest)
            AXIS2_QNAME_FREE(qmodulest, env);
        AXIS2_DESC_BUILDER_PROCESS_OP_MODULE_REFS(module_builder->desc_builder,
                env, modules, op_desc);
        /* setting Operation phase */
        info = AXIS2_DEP_ENGINE_GET_PHASES_INFO(
            axis2_desc_builder_get_dep_engine(module_builder->desc_builder, env),
            env);
        AXIS2_PHASES_INFO_SET_OP_PHASES(info, env, op_desc);

        /* adding operation */
        axis2_array_list_add(ops, env, op_desc);
    }
    return ops;
}

