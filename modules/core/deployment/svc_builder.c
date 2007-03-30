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

#include <axis2_svc_builder.h>
#include <axutil_class_loader.h>
#include <axis2_utils.h>
#include <axis2_property.h>

struct axis2_svc_builder
{
    axis2_svc_t *svc;
    struct axis2_desc_builder *desc_builder;
};

static axutil_array_list_t *
axis2_svc_builder_process_ops(axis2_svc_builder_t *svc_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *op_itr);

AXIS2_EXTERN axis2_svc_builder_t *AXIS2_CALL
axis2_svc_builder_create(const axis2_env_t *env)
{
    axis2_svc_builder_t *svc_builder = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc_builder = (axis2_svc_builder_t *) AXIS2_MALLOC(env->allocator,
        sizeof(axis2_svc_builder_t));


    if (! svc_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc_builder->desc_builder = NULL;
    svc_builder->svc = NULL;

    return svc_builder;
}

AXIS2_EXTERN axis2_svc_builder_t *AXIS2_CALL
axis2_svc_builder_create_with_file_and_dep_engine_and_svc(const axis2_env_t *env,
    axis2_char_t *file_name,
    struct axis2_dep_engine *dep_engine,
    axis2_svc_t *svc)
{
    axis2_svc_builder_t *svc_builder = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, file_name, NULL);
    AXIS2_PARAM_CHECK(env->error, dep_engine, NULL);
    AXIS2_PARAM_CHECK(env->error, svc, NULL);
    svc_builder = (axis2_svc_builder_t *) axis2_svc_builder_create(env);
    if (! svc_builder)
    {
        return NULL;
    }
    svc_builder->desc_builder =
        axis2_desc_builder_create_with_file_and_dep_engine(env, file_name,
            dep_engine);
    if (!svc_builder->desc_builder)
    {
        axis2_svc_builder_free(svc_builder, env);
        return NULL;
    }
    svc_builder->svc = svc;
    return svc_builder;
}

AXIS2_EXTERN axis2_svc_builder_t *AXIS2_CALL
axis2_svc_builder_create_with_dep_engine_and_svc(const axis2_env_t *env,
    axis2_dep_engine_t *dep_engine,
    axis2_svc_t *svc)
{
    axis2_svc_builder_t *svc_builder = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, dep_engine, NULL);
    AXIS2_PARAM_CHECK(env->error, svc, NULL);
    svc_builder = (axis2_svc_builder_t *) axis2_svc_builder_create(env);
    if (! svc_builder)
    {
        return NULL;
    }
    svc_builder->desc_builder =
        axis2_desc_builder_create_with_dep_engine(env, dep_engine);
    if (!svc_builder->desc_builder)
    {
        axis2_svc_builder_free(svc_builder, env);
        return NULL;
    }
    svc_builder->svc = svc;
    return svc_builder;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_svc_builder_free(axis2_svc_builder_t *svc_builder,
    const axis2_env_t *env)
{

    AXIS2_ENV_CHECK(env, void);

    if (svc_builder->desc_builder)
    {
        axis2_desc_builder_free(svc_builder->desc_builder, env);
    }

    svc_builder->svc = NULL;

    if (svc_builder)
    {
        AXIS2_FREE(env->allocator, svc_builder);
    }

    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_builder_populate_svc(axis2_svc_builder_t *svc_builder,
    const axis2_env_t *env,
    axiom_node_t *svc_node)
{
    axiom_element_t *svc_element = NULL;
    axiom_children_qname_iterator_t *itr = NULL;
    axiom_children_qname_iterator_t *operation_itr = NULL;
    axis2_qname_t *qparamst = NULL;
    axis2_qname_t *qdesc = NULL;
    axis2_qname_t *qmodulest = NULL;
    axis2_qname_t *qinflowst = NULL;
    axis2_qname_t *qoutflowst = NULL;
    axis2_qname_t *qin_faultflowst = NULL;
    axis2_qname_t *qout_faultflowst = NULL;
    axis2_qname_t *qopst = NULL;
    axis2_qname_t *qattname = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_svc_grp_t *parent = NULL;
    axiom_element_t *desc_element = NULL;
    axiom_node_t *desc_node = NULL;
    axiom_children_qname_iterator_t *module_refs = NULL;
    axiom_node_t *in_flow_node = NULL;
    axiom_element_t *in_flow_element = NULL;
    axiom_node_t *out_flow_node = NULL;
    axiom_element_t *out_flow_element = NULL;
    axiom_node_t *in_faultflow_node = NULL;
    axiom_element_t *in_faultflow_element = NULL;
    axiom_node_t *out_faultflow_node = NULL;
    axiom_element_t *out_faultflow_element = NULL;
    axiom_attribute_t *name_attr = NULL;
    axutil_array_list_t *ops = NULL;
    axis2_char_t *svc_name = NULL;
    axis2_char_t *class_name = NULL;
    axis2_char_t *svc_dll_name = NULL;
    axutil_dll_desc_t *dll_desc = NULL;
    axis2_param_t *impl_info_param = NULL;
    axis2_arch_file_data_t *arch_file_data = NULL;
    axis2_file_t *svc_folder = NULL;
    axis2_char_t *dll_path = NULL;
    axis2_char_t *svc_folder_path = NULL;
    int i = 0;
    int size = 0;
    AXIS2_TIME_T timestamp = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_node, AXIS2_FAILURE);

    svc_element = AXIOM_NODE_GET_DATA_ELEMENT(svc_node, env);
    /* Processing service level paramters */
    qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
    if (!qparamst)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    itr = axiom_element_get_children_with_qname(svc_element, env, qparamst,
        svc_node);
    axis2_qname_free(qparamst, env);
    qparamst = NULL;
    parent = axis2_svc_get_parent(svc_builder->svc, env);

    status = axis2_desc_builder_process_params(svc_builder->desc_builder, env,
        itr, 
        axis2_svc_get_param_container(svc_builder->svc, env), 
        axis2_svc_grp_get_param_container(parent, env));
    if (AXIS2_SUCCESS != status)
    {
        return status;
    }

    /* process service description */
    /* TODO this code is changed in new version of axis2. Until that logic
     * is incorporated I comment out this part and add my own logic to set svc
     * name
     */
	 /* -------------------------service description-------------------- */
    qdesc = axis2_qname_create(env, AXIS2_DESCRIPTION, NULL, NULL);
    desc_element = axiom_element_get_first_child_with_qname(svc_element, env,
        qdesc, svc_node, &desc_node);
    axis2_qname_free(qdesc, env) ;
    qdesc = NULL;
    if( desc_element)
    {
        axiom_element_t *desc_value_element = NULL;
        axiom_node_t *desc_value_node = NULL;
        axis2_char_t *description_text = NULL;

        desc_value_element = axiom_element_get_first_element(desc_element, 
            env, desc_node, &desc_value_node);
        description_text = axiom_element_get_text (desc_element, env, desc_node);
        if (description_text)
        {
            axis2_svc_set_svc_desc(svc_builder->svc, env, description_text);
        }
    }
	 /* --------------------services description end -------------------- */
    /* my logic to get set service name */
    qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
    name_attr = axiom_element_get_attribute(svc_element, env, qattname);
    svc_name = axiom_attribute_get_value(name_attr, env);
    axis2_svc_set_name(svc_builder->svc, env, svc_name);
    axis2_qname_free(qattname, env);

    /* create dll_desc and set it in a parameter. then set that param in param
     * container taken from svc 
     */
    dll_desc = axutil_dll_desc_create(env);
    impl_info_param = 
        axis2_param_container_get_param(      
            axis2_svc_get_param_container(svc_builder->svc, env),
            env,
            AXIS2_SERVICE_CLASS);
    if (!impl_info_param)
    {
        axutil_dll_desc_free(dll_desc, env);
        return AXIS2_FAILURE;
    }
    class_name = axis2_param_get_value(impl_info_param, env);
    svc_dll_name =
         axutil_dll_desc_create_platform_specific_dll_name(dll_desc, env,
             class_name);
    arch_file_data = axis2_dep_engine_get_current_file_item(
        axis2_desc_builder_get_dep_engine(svc_builder->desc_builder, env), 
            env);
    svc_folder = axis2_arch_file_data_get_file(arch_file_data, env);
    timestamp =  axis2_file_get_timestamp(svc_folder, env);
    axutil_dll_desc_set_timestamp(dll_desc, env, timestamp);
    svc_folder_path =  axis2_file_get_path(svc_folder, env);
    dll_path = axis2_strcat(env, svc_folder_path, AXIS2_PATH_SEP_STR,
        svc_dll_name, NULL);
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "dll path is : %s", dll_path);
    status =  axutil_dll_desc_set_name(dll_desc, env, dll_path);
    if (AXIS2_SUCCESS != status)
    {
        axutil_dll_desc_free(dll_desc, env);
        return status;
    }
    AXIS2_FREE(env->allocator, dll_path);
    dll_path = NULL;

    axutil_dll_desc_set_type(dll_desc, env, AXIS2_SVC_DLL);
    status = axis2_param_set_value(impl_info_param, env, dll_desc);
    axis2_param_set_value_free(impl_info_param, env, 
        axutil_dll_desc_free_void_arg);
    if (AXIS2_SUCCESS != status)
    {
        axutil_dll_desc_free(dll_desc, env);
        return status;
    }
    /* end of my logic */
    /* processing service wide modules which required to engage globally */
    qmodulest = axis2_qname_create(env, AXIS2_MODULEST, NULL, NULL);
    module_refs = axiom_element_get_children_with_qname(svc_element, env,
        qmodulest, svc_node);
    axis2_qname_free(qmodulest, env) ;
    qmodulest = NULL;
    status = axis2_svc_builder_process_module_refs(svc_builder, env, module_refs);
    if (AXIS2_SUCCESS != status)
    {
        return status;
    }

    /* process IN_FLOW */
    qinflowst = axis2_qname_create(env, AXIS2_IN_FLOW_START, NULL, NULL);
    in_flow_element = axiom_element_get_first_child_with_qname(svc_element,
        env, qinflowst, svc_node, &in_flow_node);
    axis2_qname_free(qinflowst, env) ;
    qinflowst = NULL;

    qoutflowst = axis2_qname_create(env, AXIS2_OUT_FLOW_START, NULL, NULL);
    out_flow_element = axiom_element_get_first_child_with_qname(svc_element,
        env, qoutflowst, svc_node, &out_flow_node);
    axis2_qname_free(qoutflowst, env) ;
    qoutflowst = NULL;

    qin_faultflowst = axis2_qname_create(env, AXIS2_IN_FAILTFLOW, NULL, NULL);
    in_faultflow_element = axiom_element_get_first_child_with_qname(svc_element,
        env, qin_faultflowst, svc_node, &in_faultflow_node);
    axis2_qname_free(qin_faultflowst, env) ;
    qin_faultflowst = NULL;

    qout_faultflowst = axis2_qname_create(env, AXIS2_OUT_FAILTFLOW, NULL, NULL);
    out_faultflow_element = axiom_element_get_first_child_with_qname(svc_element,
        env, qoutflowst, svc_node, &out_faultflow_node);
    axis2_qname_free(qout_faultflowst, env) ;
    qout_faultflowst = NULL;

    /* processing operations */
    qopst = axis2_qname_create(env, AXIS2_OPERATIONST, NULL, NULL);
    operation_itr = axiom_element_get_children_with_qname(svc_element, env,
            qopst, svc_node);
    axis2_qname_free(qopst, env) ;
    qopst = NULL;
    ops = axis2_svc_builder_process_ops(svc_builder, env, operation_itr);
    if (ops)
    {
        size = axutil_array_list_size(ops, env);
    }
    for (i = 0; i < size; i++)
    {
        axis2_op_t *op_desc = NULL;
        axutil_array_list_t *params = NULL;
        int j = 0;
        int sizej = 0;

        op_desc = (axis2_op_t *) axutil_array_list_get(ops, env, i);
        params = axis2_op_get_all_params(op_desc, env);
        /* Adding wsa-mapping into service */
        sizej = axutil_array_list_size(params, env);
        for (j = 0; j < sizej; j++)
        {
            axis2_param_t *param = NULL;
            axis2_char_t *param_name = NULL;

            param = axutil_array_list_get(params, env, j);
            param_name = axis2_param_get_name(param, env);
            if (0 == axis2_strcmp(param_name, AXIS2_WSA_MAPPING))
            {
                axis2_char_t *key = NULL;

                key = (axis2_char_t *) axis2_param_get_value(param, env);
                axis2_svc_add_mapping(svc_builder->svc, env, key, op_desc);
            }
        }
        axis2_svc_add_op(svc_builder->svc, env, op_desc);
    }
    axutil_array_list_free(ops, env);
    return AXIS2_SUCCESS;
}

static axutil_array_list_t *
axis2_svc_builder_process_ops(axis2_svc_builder_t *svc_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *op_itr)
{
    axutil_array_list_t *ops = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, op_itr, NULL);

    ops = axutil_array_list_create(env, 0);
    while (AXIS2_TRUE == axiom_children_qname_iterator_has_next(op_itr, env))
    {
        axiom_element_t *op_element = NULL;
        axiom_node_t *op_node = NULL;
        axiom_attribute_t *op_name_att = NULL;
        axiom_attribute_t *op_mep_att = NULL;
        axis2_qname_t *qmep = NULL;
        axis2_qname_t *qopname = NULL;
        axis2_qname_t *qparamst = NULL;
        axis2_qname_t *qmsgrecv = NULL;
        axis2_qname_t *qmodulest = NULL;
        axis2_qname_t *qattname = NULL;
        axis2_char_t *mep_url = NULL;
        axis2_char_t *op_name = NULL;
        axis2_op_t *op_desc = NULL;
        axiom_children_qname_iterator_t *params_itr = NULL;
        axiom_children_qname_iterator_t *module_itr = NULL;
        axiom_element_t *recv_element = NULL;
        axiom_node_t *recv_node = NULL;
        axis2_status_t status = AXIS2_FAILURE;
        struct axis2_dep_engine *dep_engine = NULL;

        op_node = axiom_children_qname_iterator_next(op_itr, env);
        /* getting operation name */
        op_element = AXIOM_NODE_GET_DATA_ELEMENT(op_node, env);
        qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        op_name_att = axiom_element_get_attribute(op_element, env, qattname);
        axis2_qname_free(qattname, env);
        qattname = NULL;
        if (! op_name_att)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_OP_NAME_MISSING,
                AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        /* set the mep of the operation */
        qmep = axis2_qname_create(env, AXIS2_MEP, NULL, NULL);
        op_mep_att = axiom_element_get_attribute(op_element, env, qmep);
        axis2_qname_free(qmep, env);
        qmep = NULL;
        if (op_mep_att)
        {
            mep_url = axiom_attribute_get_value(op_mep_att, env);
            /*
            TODO value has to be validate
            TODO
             op_descrip.setMessageExchangePattern(mep);
            */
        }
        op_name = axiom_attribute_get_value(op_name_att, env);
        qopname = axis2_qname_create(env, op_name, NULL, NULL);
        op_desc = axis2_op_create(env);
        if(mep_url)
        {
            axis2_op_set_msg_exchange_pattern(op_desc, env, mep_url);
        }
        axis2_op_set_qname(op_desc, env, qopname);

        axis2_qname_free(qopname, env);
        qopname = NULL;
        /* operation parameters */
        qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
        params_itr = axiom_element_get_children_with_qname(op_element, env,
                qparamst, op_node);
        axis2_qname_free(qparamst, env);
        qparamst = NULL;
        status = axis2_desc_builder_process_params(svc_builder->desc_builder,
            env, params_itr, 
            axis2_op_get_param_container(op_desc, env), 
            axis2_svc_get_param_container(svc_builder->svc, env));
        /* To process wsamapping */
        axis2_desc_builder_process_action_mappings(svc_builder->desc_builder, 
            env, op_node, op_desc);

        /* loading the message receivers */
        qmsgrecv = axis2_qname_create(env, AXIS2_MESSAGERECEIVER, NULL, NULL);
        recv_element = axiom_element_get_first_child_with_qname(op_element,
            env, qmsgrecv, op_node, &recv_node);
        axis2_qname_free(qmsgrecv, env);
        qmsgrecv = NULL;
        if (recv_element && NULL != recv_node)
        {
            axis2_msg_recv_t *msg_recv = NULL;
            msg_recv = axis2_desc_builder_load_msg_recv(svc_builder->desc_builder,
                 env, recv_element);
            axis2_op_set_msg_recv(op_desc, env, msg_recv);

        }
        else
        {
            axis2_msg_recv_t *msg_recv = NULL;
            /* setting the default messgae receiver */
            msg_recv = axis2_desc_builder_load_default_msg_recv(env);
            axis2_op_set_msg_recv(op_desc, env, msg_recv);
        }
        /* process module refs */
        qmodulest = axis2_qname_create(env, AXIS2_MODULEST, NULL, NULL);
        module_itr = axiom_element_get_children_with_qname(op_element, env,
            qmodulest, op_node);
        axis2_qname_free(qmodulest, env);
        qmodulest = NULL;
        status = axis2_desc_builder_process_op_module_refs(svc_builder->
            desc_builder, env, module_itr, op_desc);
        if (AXIS2_SUCCESS != status)
        {
            return AXIS2_FAILURE;
        }
        /* setting operation phase */
        dep_engine = 
            axis2_desc_builder_get_dep_engine(svc_builder->desc_builder, env);
        if (dep_engine)
        {
            axis2_phases_info_t *info =
                axis2_dep_engine_get_phases_info(dep_engine, env);
            axis2_phases_info_set_op_phases(info, env, op_desc);
        }

        /* adding operation */
        status = axutil_array_list_add(ops, env, op_desc);
    }

    return ops;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_builder_process_svc_module_conf(axis2_svc_builder_t *svc_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *module_confs,
    axis2_param_container_t *parent,
    axis2_svc_t *svc)
{
    while (AXIS2_TRUE == axiom_children_qname_iterator_has_next(module_confs, env))
    {
        axiom_element_t *module_conf_element = NULL;
        axiom_node_t *module_conf_node = NULL;
        axiom_attribute_t *module_name_att = NULL;
        axis2_qname_t *qattname = NULL;

        module_conf_node = axiom_children_qname_iterator_next(module_confs, env);
        module_conf_element = AXIOM_NODE_GET_DATA_ELEMENT(module_conf_node,
            env);
        qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        module_name_att = axiom_element_get_attribute(module_conf_element,
            env, qattname);
        axis2_qname_free(qattname, env);
        qattname = NULL;
        if (! module_name_att)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_MODULE_CONF,
                AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_builder_process_module_refs(axis2_svc_builder_t *svc_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *
    module_refs)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_refs, AXIS2_FAILURE);

    while (AXIS2_TRUE == axiom_children_qname_iterator_has_next(module_refs, env))
    {
        axiom_element_t *module_ref_element = NULL;
        axiom_node_t *module_ref_node = NULL;
        axiom_attribute_t *module_ref_att = NULL;
        axis2_qname_t *qref = NULL;

        module_ref_node = axiom_children_qname_iterator_next(module_refs, env);
        module_ref_element = AXIOM_NODE_GET_DATA_ELEMENT(module_ref_node,
            env);
        qref = axis2_qname_create(env, AXIS2_REF, NULL, NULL);
        module_ref_att = axiom_element_get_attribute(module_ref_element,
            env, qref);
        axis2_qname_free(qref, env);
        if (module_ref_att)
        {
            axis2_char_t *ref_name = NULL;
            axis2_qname_t *qrefname = NULL;

            ref_name = axiom_attribute_get_value(module_ref_att, env);
            qrefname = axis2_qname_create(env, ref_name, NULL, NULL);
            if (! axis2_dep_engine_get_module(
                axis2_desc_builder_get_dep_engine(svc_builder->desc_builder, env), 
                env, qrefname))
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_MODULE_NOT_FOUND,
                    AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            else
            {
                axis2_svc_add_module_qname(svc_builder->svc, env, qrefname);
            }
            axis2_qname_free(qrefname, env);
        }
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN struct axis2_desc_builder *AXIS2_CALL
axis2_svc_builder_get_desc_builder(const axis2_svc_builder_t *svc_builder,
    const axis2_env_t *env)
{
    return svc_builder->desc_builder;
}

