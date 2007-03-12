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

#include <axis2_conf_builder.h>
#include <axis2_disp.h>
#include <axis2_conf.h>
#include <axis2_class_loader.h>
#include <axis2_transport_in_desc.h>
#include <axis2_transport_out_desc.h>
#include <axis2_utils.h>
#include <axis2_transport_receiver.h>

typedef struct axis2_conf_builder_impl
{
    axis2_conf_builder_t conf_builder;
    axis2_conf_t *conf;

}
axis2_conf_builder_impl_t;

#define AXIS2_INTF_TO_IMPL(conf_builder) \
    ((axis2_conf_builder_impl_t *) conf_builder)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_conf_builder_free(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_conf_builder_populate_conf(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_conf_builder_process_module_refs(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *module_refs);

static axis2_status_t
axis2_conf_builder_process_disp_order(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_node_t *disp_order);

static axis2_status_t
axis2_conf_builder_process_phase_orders(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *phase_orders);

static axis2_array_list_t *
axis2_conf_builder_get_phase_list(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_node_t *phase_orders);

static axis2_status_t
axis2_conf_builder_process_transport_senders(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *trs_senders);

static axis2_status_t
axis2_conf_builder_process_transport_recvs(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *trs_recvs);

axis2_status_t AXIS2_CALL
axis2_conf_builder_process_default_module_versions(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *module_versions);

/************************** End of function prototypes ************************/

AXIS2_EXTERN axis2_conf_builder_t *AXIS2_CALL
axis2_conf_builder_create(
    const axis2_env_t *env)
{
    axis2_conf_builder_impl_t *conf_builder_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    conf_builder_impl = (axis2_conf_builder_impl_t *) AXIS2_MALLOC(env->
            allocator, sizeof(axis2_conf_builder_impl_t));


    if (NULL == conf_builder_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    conf_builder_impl->conf = NULL;
    conf_builder_impl->conf_builder.ops = NULL;

    conf_builder_impl->conf_builder.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_conf_builder_ops_t));
    if (NULL == conf_builder_impl->conf_builder.ops)
    {
        axis2_conf_builder_free(&(conf_builder_impl->conf_builder), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    conf_builder_impl->conf_builder.ops->free = axis2_conf_builder_free;
    conf_builder_impl->conf_builder.ops->populate_conf =
        axis2_conf_builder_populate_conf;
    conf_builder_impl->conf_builder.ops->process_module_refs =
        axis2_conf_builder_process_module_refs;

    return &(conf_builder_impl->conf_builder);
}

AXIS2_EXTERN axis2_conf_builder_t *AXIS2_CALL
axis2_conf_builder_create_with_file_and_dep_engine_and_conf(
    const axis2_env_t *env,
    axis2_char_t *file,
    axis2_dep_engine_t *engine,
    axis2_conf_t *conf)
{
    axis2_conf_builder_impl_t *builder_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    builder_impl = (axis2_conf_builder_impl_t *) axis2_conf_builder_create(
                env);
    if (NULL == builder_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    builder_impl->conf_builder.desc_builder =
        axis2_desc_builder_create_with_file_and_dep_engine(env, file, engine);
    builder_impl->conf = conf;

    return &(builder_impl->conf_builder);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_conf_builder_free(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env)
{
    axis2_conf_builder_impl_t *conf_builder_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    conf_builder_impl = AXIS2_INTF_TO_IMPL(conf_builder);

    if (conf_builder->desc_builder)
    {
        AXIS2_DESC_BUILDER_FREE(conf_builder->desc_builder, env);
        conf_builder->desc_builder = NULL;
    }

    if (conf_builder->ops)
    {
        AXIS2_FREE(env->allocator, conf_builder->ops);
        conf_builder->ops = NULL;
    }

    if (conf_builder_impl)
    {
        AXIS2_FREE(env->allocator, conf_builder_impl);
        conf_builder_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_builder_populate_conf(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env)
{
    axis2_conf_builder_impl_t *builder_impl = NULL;
    axis2_qname_t *qparamst = NULL;
    axis2_qname_t *qmsgrecv = NULL;
    axis2_qname_t *qdisporder = NULL;
    axis2_qname_t *qmodulest = NULL;
    axis2_qname_t *qtransportsender = NULL;
    axis2_qname_t *qtransportrecv = NULL;
    axis2_qname_t *qphaseorder = NULL;
    axis2_qname_t *qdefmodver = NULL;
    axiom_children_qname_iterator_t *itr = NULL;
    axiom_children_qname_iterator_t *msg_recvs = NULL;
    axiom_children_qname_iterator_t *module_itr = NULL;
    axiom_children_qname_iterator_t *trs_senders = NULL;
    axiom_children_qname_iterator_t *trs_recvs = NULL;
    axiom_children_qname_iterator_t *phase_orders = NULL;
    axiom_children_qname_iterator_t *def_mod_versions = NULL;
    axiom_element_t *conf_element = NULL;
    axiom_node_t *conf_node = NULL;
    axiom_element_t *disp_order_element = NULL;
    axiom_node_t *disp_order_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_param_t *param = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(conf_builder);

    conf_node  = AXIS2_DESC_BUILDER_BUILD_OM(conf_builder->desc_builder, env);
    if (!conf_node)
    {
        return AXIS2_FAILURE;
    }
    conf_element = AXIOM_NODE_GET_DATA_ELEMENT(conf_node, env);
    /* processing Paramters */
    /* Processing service level paramters */
    qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
    itr = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(conf_element, env, qparamst,
            conf_node);
    AXIS2_QNAME_FREE(qparamst, env);
    AXIS2_DESC_BUILDER_PROCESS_PARAMS(conf_builder->desc_builder, env, itr,
            builder_impl->conf->param_container, builder_impl->conf->param_container);
    /* process Message Reciver */
    qmsgrecv = axis2_qname_create(env, AXIS2_MESSAGERECEIVER, NULL, NULL);
    msg_recvs = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(conf_element, env,
            qmsgrecv, conf_node);
    AXIS2_QNAME_FREE(qmsgrecv, env);
    while (AXIS2_TRUE == AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(msg_recvs, env))
    {
        axiom_node_t *msg_recv_node = NULL;
        axiom_element_t *msg_recv_element = NULL;
        axis2_msg_recv_t *msg_recv = NULL;
        axiom_attribute_t *mep_att = NULL;
        axis2_qname_t *qmep = NULL;
        axis2_char_t *att_value = NULL;

        msg_recv_node = (axiom_node_t *)
                AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(msg_recvs, env);
        msg_recv_element = (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(msg_recv_node, env);
        msg_recv = AXIS2_DESC_BUILDER_LOAD_MSG_RECV(conf_builder->desc_builder,
                env, msg_recv_element);
        if (!msg_recv)
        {
            return AXIS2_FAILURE;
        }
        qmep = axis2_qname_create(env, AXIS2_MEP, NULL, NULL);
        mep_att = AXIOM_ELEMENT_GET_ATTRIBUTE(msg_recv_element, env, qmep);
        att_value = AXIOM_ATTRIBUTE_GET_VALUE(mep_att, env);
        AXIS2_CONF_ADD_MSG_RECV(builder_impl->conf, env, att_value, msg_recv);
        AXIS2_QNAME_FREE(qmep, env);
    }

    /* processing Dispatching Order */
    qdisporder = axis2_qname_create(env, AXIS2_DISPATCH_ORDER, NULL, NULL);
    disp_order_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(
                conf_element, env, qdisporder, conf_node, &disp_order_node);
    AXIS2_QNAME_FREE(qdisporder, env);
    if (disp_order_element)
    {
        axis2_conf_builder_process_disp_order(conf_builder, env, disp_order_node);
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "found the custom disptaching \
                order and continue with that order");
    }
    else
    {
        status = AXIS2_CONF_SET_DEFAULT_DISPATCHERS(builder_impl->conf, env);
        if (AXIS2_SUCCESS != status)
        {
            return AXIS2_FAILURE;
        }
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "no custom dispatching order \
                found continue with default dispatcing order");
    }

    /* Process Module refs */
    qmodulest = axis2_qname_create(env, AXIS2_MODULEST, NULL, NULL);
    module_itr = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(conf_element, env,
            qmodulest, conf_node);
    AXIS2_QNAME_FREE(qmodulest, env);
    status = axis2_conf_builder_process_module_refs(conf_builder, env, module_itr);
    if (AXIS2_SUCCESS != status)
    {
        return AXIS2_FAILURE;
    }
    /* Proccessing Transport Sennders */
    qtransportsender = axis2_qname_create(env, AXIS2_TRANSPORTSENDER, NULL, NULL);
    trs_senders = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(conf_element, env,
            qtransportsender, conf_node);
    AXIS2_QNAME_FREE(qtransportsender, env);
    axis2_conf_builder_process_transport_senders(conf_builder, env, trs_senders);

    /* Proccessing Transport Recivers */
    qtransportrecv = axis2_qname_create(env, AXIS2_TRANSPORTRECEIVER, NULL, NULL);
    trs_recvs = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(conf_element, env,
            qtransportrecv, conf_node);
    AXIS2_QNAME_FREE(qtransportrecv, env);
    axis2_conf_builder_process_transport_recvs(conf_builder, env, trs_recvs);

    /* processing Phase orders */
    qphaseorder = axis2_qname_create(env, AXIS2_PHASE_ORDER, NULL, NULL);
    phase_orders = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(conf_element, env,
            qphaseorder, conf_node);
    AXIS2_QNAME_FREE(qphaseorder, env);
    axis2_conf_builder_process_phase_orders(conf_builder, env, phase_orders);

    /* Processing default module versions */
    qdefmodver = axis2_qname_create(env, AXIS2_DEFAULT_MODULE_VERSION, NULL,
            NULL);
    def_mod_versions = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(conf_element,
            env, qdefmodver, conf_node);
    AXIS2_QNAME_FREE(qdefmodver, env);
    if (def_mod_versions)
    {
        status = axis2_conf_builder_process_default_module_versions(conf_builder
                , env, def_mod_versions);
        if (AXIS2_FAILURE == status)
        {
            return AXIS2_FAILURE;
        }
    }
    /* TODO processing Axis Storages */

    param = axis2_param_container_get_param(builder_impl->conf->param_container, 
        env, AXIS2_ENABLE_MTOM);
    if (param)
    {
        axis2_char_t *value = NULL;
        value = AXIS2_PARAM_GET_VALUE(param, env);
        if (value)
        {
            axis2_conf_set_enable_mtom(builder_impl->conf, env, 
                (AXIS2_STRCMP(value, AXIS2_VALUE_TRUE) == 0));
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_builder_process_module_refs(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *module_refs)
{
    axis2_conf_builder_impl_t *builder_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_refs, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(conf_builder);

    while (AXIS2_TRUE == AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(module_refs,
            env))
    {
        axiom_node_t *module_ref_node = NULL;
        axiom_element_t *module_ref_element = NULL;
        axis2_qname_t *qref = NULL;

        axiom_attribute_t *module_ref_att = NULL;
        module_ref_node = (axiom_node_t *)
                AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(module_refs, env);
        if (!module_ref_node)
        {
            return AXIS2_FAILURE;
        }
        module_ref_element = AXIOM_NODE_GET_DATA_ELEMENT(module_ref_node, env);
        qref = axis2_qname_create(env, AXIS2_REF, NULL, NULL);
        module_ref_att = AXIOM_ELEMENT_GET_ATTRIBUTE(module_ref_element, env,
                qref);
        if (qref)
            AXIS2_QNAME_FREE(qref, env);
        if (module_ref_att)
        {
            axis2_qname_t *qrefname = NULL;
            axis2_char_t *ref_name = NULL;

            ref_name = AXIOM_ATTRIBUTE_GET_VALUE(module_ref_att, env);
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "module %s found in axis2.xml", ref_name);
            qrefname = axis2_qname_create(env, ref_name, NULL, NULL);
            status = AXIS2_DEP_ENGINE_ADD_MODULE(conf_builder->desc_builder->
                    engine, env, qrefname);
            if (qrefname)
                AXIS2_QNAME_FREE(qrefname, env);
        }
    }
    return status;
}

static axis2_status_t
axis2_conf_builder_process_disp_order(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_node_t *disp_order_node)
{
    axis2_conf_builder_impl_t *builder_impl = NULL;
    axiom_element_t *disp_order_element = NULL;
    axiom_children_qname_iterator_t *disps = NULL;
    axis2_qname_t *qdisp = NULL;
    axis2_bool_t found_disp = AXIS2_FALSE;
    axis2_phase_t *disp_phase = NULL;
    int count = 0;
    axis2_bool_t qname_itr_has_next = AXIS2_FALSE;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, disp_order_node, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(conf_builder);

    disp_order_element = AXIOM_NODE_GET_DATA_ELEMENT(
                disp_order_node, env);
    qdisp = axis2_qname_create(env, AXIS2_DISPATCHER, NULL, NULL);
    disps = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(
                disp_order_element, env, qdisp, disp_order_node);
    AXIS2_QNAME_FREE(qdisp, env);
    disp_phase = axis2_phase_create(env, AXIS2_PHASE_DISPATCH);
    if (NULL == disp_phase)
    {
        return AXIS2_FAILURE;
    }
    if (disps)
    {
        qname_itr_has_next = AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(disps,
                env);
    }
    while (AXIS2_TRUE == qname_itr_has_next)
    {
        axiom_node_t *disp_node = NULL;
        axiom_element_t *disp_element = NULL;
        axiom_attribute_t *disp_att = NULL;
        axis2_char_t *class_name = NULL;
        axis2_char_t *dll_name = NULL;
        axis2_qname_t *class_qname = NULL;
        axis2_disp_t *disp_dll = NULL;
        axis2_dll_desc_t *dll_desc = NULL;
        axis2_param_t *impl_info_param = NULL;
        axis2_handler_desc_t *handler_desc = NULL;
        axis2_handler_t *handler = NULL;

        found_disp = AXIS2_TRUE;
        disp_node = (axiom_node_t *)
                AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(disps, env);
        class_qname = axis2_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
        disp_att = AXIOM_ELEMENT_GET_ATTRIBUTE(disp_element, env, class_qname);
        AXIS2_QNAME_FREE(class_qname, env);
        if (!disp_att)
        {
            qname_itr_has_next = AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(disps,
                    env);
            continue;
        }
        class_name = AXIOM_ATTRIBUTE_GET_VALUE(disp_att, env);
        dll_desc = axis2_dll_desc_create(env);
        dll_name =
             axis2_dll_desc_create_platform_specific_dll_name(dll_desc, env,
                    class_name);
        /* TODO
         * set full dll path here instead of dll lib name only */
         axis2_dll_desc_set_name(dll_desc, env, dll_name);
         axis2_dll_desc_set_type(dll_desc, env, AXIS2_HANDLER_DLL);
        impl_info_param = axis2_param_create(env, class_name, NULL);
        if (!impl_info_param)
        {
            AXIS2_PHASE_FREE(disp_phase, env);
            return AXIS2_FAILURE;
        }
        AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc);
        axis2_param_set_value_free(impl_info_param, env, 
            axis2_dll_desc_free_void_arg);
        axis2_class_loader_init(env);
        disp_dll = (axis2_disp_t *) axis2_class_loader_create_dll(env,
                impl_info_param);

        handler = AXIS2_DISP_GET_BASE(disp_dll, env);
        handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(handler, env);
        axis2_handler_desc_add_param(handler_desc, env, impl_info_param);

        /*disptachClas.getHandlerDesc().setParent(axisConfiguration); */
        AXIS2_PHASE_ADD_HANDLER_AT(disp_phase, env, count, handler);
        count ++;
        qname_itr_has_next = AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(disps,
                env);

    }

    if (AXIS2_TRUE != found_disp)
    {
        AXIS2_PHASE_FREE(disp_phase, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_DISPATCHER_FOUND,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        status = AXIS2_CONF_SET_DISPATCH_PHASE(builder_impl->conf, env, disp_phase);
        if (AXIS2_SUCCESS != status)
        {
            AXIS2_PHASE_FREE(disp_phase, env);
            return status;
        }
    }
    return AXIS2_SUCCESS;
}

/**
 * To process all the phase orders which are defined in axis2.xml
 * @param phase_orders
 */
static axis2_status_t
axis2_conf_builder_process_phase_orders(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *phase_orders)
{
    axis2_conf_builder_impl_t *builder_impl = NULL;
    axis2_phases_info_t *info = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, phase_orders, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(conf_builder);

    info = AXIS2_DEP_ENGINE_GET_PHASES_INFO(conf_builder->desc_builder->engine,
            env);
    while (AXIS2_TRUE == AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(phase_orders, env))
    {
        axiom_node_t *phase_orders_node = NULL;
        axiom_element_t *phase_orders_element = NULL;
        axiom_attribute_t *phase_orders_att = NULL;
        axis2_qname_t *qtype = NULL;
        axis2_char_t *flow_type = NULL;
        axis2_array_list_t *phase_list = NULL;

        phase_orders_node = (axiom_node_t *) AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(
                    phase_orders, env);
        if (phase_orders_node)
        {
            phase_orders_element = AXIOM_NODE_GET_DATA_ELEMENT(phase_orders_node,
                    env);
        }
        if (phase_orders_element)
        {
            qtype = axis2_qname_create(env, AXIS2_TYPE, NULL, NULL);

            phase_orders_att = AXIOM_ELEMENT_GET_ATTRIBUTE(phase_orders_element,
                    env, qtype);
            AXIS2_QNAME_FREE(qtype, env);
        }

        if (phase_orders_att)
        {
            flow_type = AXIOM_ATTRIBUTE_GET_VALUE(phase_orders_att, env);
        }

        phase_list = axis2_conf_builder_get_phase_list(conf_builder, env,
                phase_orders_node);
        if (!phase_list)
        {
            axis2_status_t status_code = AXIS2_FAILURE;
            status_code = AXIS2_ERROR_GET_STATUS_CODE(env->error);
            if (AXIS2_SUCCESS != status_code)
            {
                return status_code;
            }
            return AXIS2_SUCCESS;
        }
        if (flow_type && 0 == AXIS2_STRCMP(AXIS2_IN_FLOW_START, flow_type))
        {
            AXIS2_PHASES_INFO_SET_IN_PHASES(info, env, phase_list);
        }
        else if (flow_type && 0 == AXIS2_STRCMP(AXIS2_IN_FAILTFLOW, flow_type))
        {
            AXIS2_PHASES_INFO_SET_IN_FAULTPHASES(info, env, phase_list);
        }
        else if (flow_type && 0 == AXIS2_STRCMP(AXIS2_OUT_FLOW_START, flow_type))
        {
            AXIS2_PHASES_INFO_SET_OUT_PHASES(info, env, phase_list);
        }
        else if (flow_type && 0 == AXIS2_STRCMP(AXIS2_OUT_FAILTFLOW, flow_type))
        {
            AXIS2_PHASES_INFO_SET_OUT_FAULTPHASES(info, env, phase_list);
        }
    }
    return AXIS2_SUCCESS;
}


static axis2_array_list_t *
axis2_conf_builder_get_phase_list(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_node_t *phase_orders_node)
{
    axis2_conf_builder_impl_t *builder_impl = NULL;
    axis2_array_list_t *phase_list = NULL;
    axiom_children_qname_iterator_t *phases = NULL;
    axis2_qname_t *qphase = NULL;
    axiom_element_t *phase_orders_element;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, phase_orders_node, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(conf_builder);

    phase_orders_element = AXIOM_NODE_GET_DATA_ELEMENT(phase_orders_node,
            env);
    if (!phase_orders_element)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_DATA_ELEMENT_IS_NULL,
                AXIS2_FAILURE);
        return NULL;
    }
    phase_list = axis2_array_list_create(env, 0);
    qphase = axis2_qname_create(env, AXIS2_PHASE, NULL, NULL);
    phases = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(phase_orders_element, env,
            qphase, phase_orders_node);
    AXIS2_QNAME_FREE(qphase, env);
    if (!phases)
    {
        axis2_array_list_free(phase_list, env);
        return NULL;
    }

    while (AXIS2_TRUE == AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(phases, env))
    {
        axiom_node_t *phase_node = NULL;
        axiom_element_t *phase_element = NULL;
        axiom_attribute_t *phase_att = NULL;
        axis2_qname_t *qattname = NULL;
        axis2_char_t *att_value = NULL;

        phase_node = (axiom_node_t *) AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(
                    phases, env);
        if (phase_node)
        {
            phase_element = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(
                        phase_node, env);
        }

        qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        if (phase_element)
        {
            phase_att = AXIOM_ELEMENT_GET_ATTRIBUTE(phase_element, env,
                    qattname);
        }
        if (phase_att)
        {
            att_value = AXIOM_ATTRIBUTE_GET_VALUE(phase_att, env);
        }
        if (att_value)
        {
            axis2_array_list_add(phase_list, env, att_value);
        }

        AXIS2_QNAME_FREE(qattname, env);
    }
    return phase_list;
}

static axis2_status_t
axis2_conf_builder_process_transport_senders(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *trs_senders)
{
    axis2_conf_builder_impl_t *builder_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, trs_senders, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(conf_builder);

    while (AXIS2_TRUE == AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(trs_senders,
            env))
    {
        axis2_transport_out_desc_t *transport_out = NULL;
        axiom_node_t *transport_node = NULL;
        axiom_element_t *transport_element = NULL;
        axiom_attribute_t *trs_name = NULL;
        axis2_qname_t *qattname = NULL;

        transport_node = (axiom_node_t *)
                AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(trs_senders, env);

        if (transport_node)
        {
            transport_element = (axiom_element_t*)
                    AXIOM_NODE_GET_DATA_ELEMENT(transport_node, env);
            if (!transport_element)
            {
                return AXIS2_FAILURE;
            }
        }
        else
        {
            return AXIS2_FAILURE;
        }

        /* getting trsnport Name */
        qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        if (transport_element)
        {
            trs_name = AXIOM_ELEMENT_GET_ATTRIBUTE(transport_element, env,
                    qattname);
        }
        AXIS2_QNAME_FREE(qattname, env);
        if (trs_name)
        {
            axis2_char_t *name = NULL;
            axiom_attribute_t *trs_dll_att = NULL;
            axis2_char_t *dll_name = NULL;
            axis2_char_t *class_name = NULL;
            axiom_children_qname_iterator_t *itr = NULL;
            axis2_qname_t *qparamst = NULL;
            axis2_qname_t *qinflowst = NULL;
            axis2_qname_t *qoutflowst = NULL;
            axis2_qname_t *qinfaultflowst = NULL;
            axis2_qname_t *qoutfaultflowst = NULL;
            axis2_qname_t *qdllname = NULL;
            axiom_element_t *in_flow_element = NULL;
            axiom_node_t *in_flow_node = NULL;
            axiom_element_t *out_flow_element = NULL;
            axiom_node_t *out_flow_node = NULL;
            axiom_element_t *in_fault_flow_element = NULL;
            axiom_node_t *in_fault_flow_node = NULL;
            axiom_element_t *out_fault_flow_element = NULL;
            axiom_node_t *out_fault_flow_node = NULL;
            axis2_dll_desc_t *dll_desc = NULL;
            axis2_param_t *impl_info_param = NULL;
            void *transport_sender = NULL;
            axis2_char_t *path_qualified_dll_name = NULL;
            axis2_char_t *repos_name = NULL;
            axis2_char_t *temp_path = NULL;
            axis2_char_t *temp_path2 = NULL;
            axis2_char_t *temp_path3 = NULL;
            AXIS2_TRANSPORT_ENUMS transport_enum;

            name = AXIOM_ATTRIBUTE_GET_VALUE(trs_name, env);
            if (name)
            {
                if (axis2_strcmp(name, AXIS2_TRANSPORT_HTTP) == 0)
                    transport_enum = AXIS2_TRANSPORT_ENUM_HTTP;
                else if (axis2_strcmp(name, AXIS2_TRANSPORT_XMPP) == 0)
                    transport_enum = AXIS2_TRANSPORT_ENUM_XMPP;
                else if (axis2_strcmp(name, AXIS2_TRANSPORT_SMTP) == 0)
                    transport_enum = AXIS2_TRANSPORT_ENUM_SMTP;
                else if (axis2_strcmp(name, AXIS2_TRANSPORT_TCP) == 0)
                    transport_enum = AXIS2_TRANSPORT_ENUM_TCP;
                else if (axis2_strcmp(name, AXIS2_TRANSPORT_HTTPS) == 0)
                    transport_enum = AXIS2_TRANSPORT_ENUM_HTTPS;
                else
                    return AXIS2_FAILURE;
                
                transport_out = axis2_transport_out_desc_create(env, transport_enum);
            }

            if (NULL == transport_out)
            {
                return AXIS2_FAILURE;
            }

            /* transport impl class */
            qdllname = axis2_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
            trs_dll_att = AXIOM_ELEMENT_GET_ATTRIBUTE(transport_element, env,
                    qdllname);
            AXIS2_QNAME_FREE(qdllname, env);
            if (NULL == trs_dll_att)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_TRANSPORT_SENDER_ERROR,
                        AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            class_name = AXIOM_ATTRIBUTE_GET_VALUE(trs_dll_att, env);
            impl_info_param = axis2_param_create(env, class_name, NULL);
            if (!impl_info_param)
            {
                axis2_transport_out_desc_free(transport_out, env);
                return AXIS2_FAILURE;
            }
            dll_desc = axis2_dll_desc_create(env);
            dll_name =
                 axis2_dll_desc_create_platform_specific_dll_name(dll_desc, env,
                        class_name);

            repos_name = AXIS2_DEP_ENGINE_GET_REPOS_PATH(conf_builder->
                    desc_builder->engine, env);
            temp_path = AXIS2_STRACAT(repos_name, AXIS2_PATH_SEP_STR, env);
            temp_path2 = AXIS2_STRACAT(temp_path, AXIS2_LIB_FOLDER, env);
            temp_path3 = AXIS2_STRACAT(temp_path2, AXIS2_PATH_SEP_STR, env);
            path_qualified_dll_name = AXIS2_STRACAT(temp_path3, dll_name, env);
            AXIS2_FREE(env->allocator, temp_path);
            AXIS2_FREE(env->allocator, temp_path2);
            AXIS2_FREE(env->allocator, temp_path3);
             axis2_dll_desc_set_name(dll_desc, env, path_qualified_dll_name);
            AXIS2_FREE(env->allocator, path_qualified_dll_name);
             axis2_dll_desc_set_type(dll_desc, env, AXIS2_TRANSPORT_SENDER_DLL);
            AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc);
            axis2_param_set_value_free(impl_info_param, env, 
                axis2_dll_desc_free_void_arg);
            axis2_class_loader_init(env);
            transport_sender = axis2_class_loader_create_dll(env, impl_info_param);
            axis2_transport_out_desc_add_param(transport_out, env,
                    impl_info_param);
            status = axis2_transport_out_desc_set_sender(transport_out, env,
                    transport_sender);
            if (AXIS2_SUCCESS != status)
            {
                axis2_transport_out_desc_free(transport_out, env);
                return status;
            }

            /* Process Parameters */
            /* Processing service level paramters */
            qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
            itr = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(transport_element,
                    env, qparamst, transport_node);
            AXIS2_QNAME_FREE(qparamst, env);
            status = AXIS2_DESC_BUILDER_PROCESS_PARAMS(conf_builder->desc_builder, env,
                    itr, 
                    axis2_transport_out_desc_param_container(transport_out, env), builder_impl->conf->
                    param_container);
            if (AXIS2_SUCCESS != status)
            {
                axis2_transport_out_desc_free(transport_out, env);
                return status;
            }
            /* process IN_FLOW */
            qinflowst = axis2_qname_create(env, AXIS2_IN_FLOW_START, NULL, NULL);
            in_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(
                        transport_element, env, qinflowst, transport_node, &in_flow_node);
            AXIS2_QNAME_FREE(qinflowst, env);
            if (in_flow_element)
            {
                axis2_transport_out_desc_free(transport_out, env);
                AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_IN_FLOW_NOT_ALLOWED_IN_TRS_OUT, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            qoutflowst = axis2_qname_create(env, AXIS2_OUT_FLOW_START, NULL, NULL);
            out_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(
                        transport_element, env, qoutflowst, transport_node, &out_flow_node);
            AXIS2_QNAME_FREE(qoutflowst, env);
            if (out_flow_element)
            {
                axis2_flow_t *flow = NULL;

                flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(conf_builder->desc_builder,
                        env, out_flow_element, builder_impl->conf->param_container,
                        out_flow_node);
                status = axis2_transport_out_desc_set_out_flow(transport_out, env, flow);
                if (AXIS2_SUCCESS != status)
                {
                    axis2_transport_out_desc_free(transport_out, env);
                    return status;
                }
            }

            /* process IN FAULT FLOW */
            qinfaultflowst = axis2_qname_create(env, AXIS2_IN_FAILTFLOW, NULL,
                    NULL);
            in_fault_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(
                        transport_element, env, qinfaultflowst, transport_node,
                        &in_fault_flow_node);
            AXIS2_QNAME_FREE(qinfaultflowst, env);
            if (in_fault_flow_element)
            {
                axis2_transport_out_desc_free(transport_out, env);
                AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_IN_FLOW_NOT_ALLOWED_IN_TRS_OUT, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }

            qoutfaultflowst = axis2_qname_create(env, AXIS2_OUT_FAILTFLOW, NULL,
                    NULL);
            out_fault_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(
                        transport_element, env, qoutfaultflowst, transport_node,
                        &out_fault_flow_node);
            AXIS2_QNAME_FREE(qoutfaultflowst, env);
            if (out_fault_flow_element)
            {
                axis2_flow_t *flow = NULL;

                flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(conf_builder->desc_builder,
                        env, out_fault_flow_element, builder_impl->conf->param_container,
                        out_fault_flow_node);
                status = axis2_transport_out_desc_set_fault_out_flow(transport_out, env, flow);
                if (AXIS2_SUCCESS != status)
                {
                    axis2_transport_out_desc_free(transport_out, env);
                    return status;
                }
            }

            /* adding to axis config */
            status = AXIS2_CONF_ADD_TRANSPORT_OUT(builder_impl->conf, env,
                    transport_out, transport_enum);
            if (AXIS2_SUCCESS != status)
            {
                axis2_transport_out_desc_free(transport_out, env);
                return status;
            }
        }
    }
    
    return AXIS2_SUCCESS;
}


static axis2_status_t
axis2_conf_builder_process_transport_recvs(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *trs_recvs)
{
    axis2_conf_builder_impl_t *builder_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, trs_recvs, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(conf_builder);

    while (AXIS2_TRUE == AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(trs_recvs,
            env))
    {
        axis2_transport_in_desc_t *transport_in = NULL;
        axiom_node_t *transport_node = NULL;
        axiom_element_t *transport_element = NULL;
        axiom_attribute_t *trs_name = NULL;
        axis2_qname_t *qattname = NULL;

        transport_node = (axiom_node_t *)
                AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(trs_recvs, env);
        if (transport_node)
        {
            transport_element = AXIOM_NODE_GET_DATA_ELEMENT(transport_node,
                    env);
            if (!transport_element)
            {
                return AXIS2_FAILURE;
            }
        }
        else
        {
            return AXIS2_FAILURE;
        }

        /* getting transport Name */
        qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        trs_name = AXIOM_ELEMENT_GET_ATTRIBUTE(transport_element, env,
                qattname);
        AXIS2_QNAME_FREE(qattname, env);

        if (trs_name)
        {
            axis2_char_t *name = NULL;
            axiom_attribute_t *trs_class_name = NULL;
            axiom_children_qname_iterator_t *itr = NULL;
            axis2_qname_t *class_qname = NULL;
            axis2_qname_t *qparamst = NULL;
            axis2_qname_t *qinflowst = NULL;
            axis2_qname_t *qoutflowst = NULL;
            axis2_qname_t *qinfaultflowst = NULL;
            axis2_qname_t *qoutfaultflowst = NULL;
            axiom_element_t *in_flow_element = NULL;
            axiom_node_t *in_flow_node = NULL;
            axiom_element_t *out_flow_element = NULL;
            axiom_node_t *out_flow_node = NULL;
            axiom_element_t *in_fault_flow_element = NULL;
            axiom_node_t *in_fault_flow_node = NULL;
            axiom_element_t *out_fault_flow_element = NULL;
            axiom_node_t *out_fault_flow_node = NULL;
            AXIS2_TRANSPORT_ENUMS transport_enum;

            name = AXIOM_ATTRIBUTE_GET_VALUE(trs_name, env);
            if (name)
            {
                if (axis2_strcmp(name, AXIS2_TRANSPORT_HTTP) == 0)
                    transport_enum = AXIS2_TRANSPORT_ENUM_HTTP;
                else if (axis2_strcmp(name, AXIS2_TRANSPORT_XMPP) == 0)
                    transport_enum = AXIS2_TRANSPORT_ENUM_XMPP;
                else if (axis2_strcmp(name, AXIS2_TRANSPORT_SMTP) == 0)
                    transport_enum = AXIS2_TRANSPORT_ENUM_SMTP;
                else if (axis2_strcmp(name, AXIS2_TRANSPORT_TCP) == 0)
                    transport_enum = AXIS2_TRANSPORT_ENUM_TCP;
                else if (axis2_strcmp(name, AXIS2_TRANSPORT_HTTPS) == 0)
                    transport_enum = AXIS2_TRANSPORT_ENUM_HTTPS;
                else
                    return AXIS2_FAILURE;

                transport_in = axis2_transport_in_desc_create(env,
                    transport_enum);
            }
            if (!transport_in)
            {
                return AXIS2_FAILURE;
            }

            /* transport impl class */
            class_qname = axis2_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
            trs_class_name = AXIOM_ELEMENT_GET_ATTRIBUTE(transport_element, env,
                    class_qname);
            AXIS2_QNAME_FREE(class_qname, env);

            if (trs_class_name)
            {
                axis2_char_t *class_name = NULL;
                axis2_char_t *dll_name = NULL;
                axis2_dll_desc_t *dll_desc = NULL;
                axis2_param_t *impl_info_param = NULL;
                axis2_transport_receiver_t *recv = NULL;
                axis2_status_t stat = AXIS2_FAILURE;
                axis2_char_t *path_qualified_dll_name = NULL;
                axis2_char_t *repos_name = NULL;
                axis2_char_t *temp_path = NULL;
                axis2_char_t *temp_path2 = NULL;
                axis2_char_t *temp_path3 = NULL;

                class_name = AXIOM_ATTRIBUTE_GET_VALUE(trs_class_name, env);
                impl_info_param = axis2_param_create(env, class_name, NULL);
                dll_desc = axis2_dll_desc_create(env);
                dll_name =
                     axis2_dll_desc_create_platform_specific_dll_name(dll_desc,
                            env, class_name);

                repos_name = AXIS2_DEP_ENGINE_GET_REPOS_PATH(conf_builder->
                        desc_builder->engine, env);
                temp_path = AXIS2_STRACAT(repos_name, AXIS2_PATH_SEP_STR, env);
                temp_path2 = AXIS2_STRACAT(temp_path, AXIS2_LIB_FOLDER, env);
                temp_path3 = AXIS2_STRACAT(temp_path2, AXIS2_PATH_SEP_STR, env);
                path_qualified_dll_name = AXIS2_STRACAT(temp_path3, dll_name, env);
                AXIS2_FREE(env->allocator, temp_path);
                AXIS2_FREE(env->allocator, temp_path2);
                AXIS2_FREE(env->allocator, temp_path3);

                 axis2_dll_desc_set_name(dll_desc, env, path_qualified_dll_name);
                AXIS2_FREE(env->allocator, path_qualified_dll_name);
                 axis2_dll_desc_set_type(dll_desc, env, AXIS2_TRANSPORT_RECV_DLL);

                AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc);
                axis2_param_set_value_free(impl_info_param, env, 
                    axis2_dll_desc_free_void_arg);
                axis2_class_loader_init(env);
                recv = (axis2_transport_receiver_t *)
                        axis2_class_loader_create_dll(env, impl_info_param);
                axis2_transport_in_desc_add_param(transport_in, env,
                        impl_info_param);
                stat = axis2_transport_in_desc_set_recv(transport_in, env,
                        recv);
            }

            /* process Parameters */
            /* processing Paramters */
            /* Processing service level paramters */
            qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
            itr = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(transport_element,
                    env, qparamst, transport_node);
            AXIS2_QNAME_FREE(qparamst, env);
            status = AXIS2_DESC_BUILDER_PROCESS_PARAMS(conf_builder->desc_builder, env,
                    itr, 
                    axis2_transport_in_desc_param_container(transport_in, env),
                    builder_impl->conf->param_container);
            if (AXIS2_SUCCESS != status)
            {
                axis2_transport_in_desc_free(transport_in, env);
                return status;
            }
            /* process OUT_FLOW */
            qoutflowst = axis2_qname_create(env, AXIS2_OUT_FLOW_START, NULL, NULL);
            out_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(
                        transport_element, env, qoutflowst, transport_node, &out_flow_node);
            AXIS2_QNAME_FREE(qoutflowst, env);
            if (out_flow_element)
            {
                axis2_transport_in_desc_free(transport_in, env);
                AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_OUT_FLOW_NOT_ALLOWED_IN_TRS_IN, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }

            qinflowst = axis2_qname_create(env, AXIS2_IN_FLOW_START, NULL, NULL);
            in_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(
                        transport_element, env, qinflowst, transport_node, &in_flow_node);
            AXIS2_QNAME_FREE(qinflowst, env);
            if (in_flow_element)
            {
                axis2_flow_t *flow = NULL;

                flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(conf_builder->
                        desc_builder, env, in_flow_element, builder_impl->conf->
                        param_container, in_flow_node);
                status = axis2_transport_in_desc_set_in_flow(transport_in, env,
                        flow);
                if (AXIS2_SUCCESS != status)
                {
                    axis2_transport_in_desc_free(transport_in, env);
                    return status;
                }
            }

            qinfaultflowst = axis2_qname_create(env, AXIS2_IN_FAILTFLOW, NULL,
                    NULL);
            in_fault_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(
                        transport_element, env, qinfaultflowst, transport_node,
                        &in_fault_flow_node);
            AXIS2_QNAME_FREE(qinfaultflowst, env);
            if (in_fault_flow_element)
            {
                axis2_flow_t *flow = NULL;

                flow = AXIS2_DESC_BUILDER_PROCESS_FLOW(conf_builder->desc_builder,
                        env, in_fault_flow_element, builder_impl->conf->
                        param_container, in_fault_flow_node);
                status = axis2_transport_in_desc_set_fault_in_flow(transport_in,
                        env, flow);
                if (AXIS2_SUCCESS != status)
                {
                    axis2_transport_in_desc_free(transport_in, env);
                    return status;
                }
            }

            qoutfaultflowst = axis2_qname_create(env, AXIS2_OUT_FAILTFLOW, NULL,
                    NULL);
            out_fault_flow_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(
                        transport_element, env, qoutfaultflowst, transport_node,
                        &out_fault_flow_node);
            if (out_fault_flow_element)
            {
                AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_OUT_FLOW_NOT_ALLOWED_IN_TRS_IN, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }

            /* adding to axis config */
            status = AXIS2_CONF_ADD_TRANSPORT_IN(builder_impl->conf, env,
                    transport_in, transport_enum);
            if (AXIS2_SUCCESS != status)
            {
                axis2_transport_in_desc_free(transport_in, env);
                return status;
            }

            AXIS2_QNAME_FREE(qoutfaultflowst, env);

        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_builder_process_default_module_versions(
    axis2_conf_builder_t *conf_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *module_versions)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_versions, AXIS2_FAILURE);

    while (AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(module_versions, env))
    {
        axiom_element_t *om_element = NULL;
        axis2_char_t *name = NULL;
        axis2_char_t *default_version = NULL;
        axis2_qname_t *attribute_qname = NULL;

        om_element = (axiom_element_t *)
                AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(module_versions, env);
        if (NULL == om_element)
        {
            continue;
        }
        attribute_qname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        if (NULL == attribute_qname)
        {
            continue;
        }
        name = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE(om_element, env,
                attribute_qname);
        AXIS2_QNAME_FREE(attribute_qname, env);
        attribute_qname = NULL;
        if (NULL == name)
        {
            return AXIS2_FAILURE;
        }
        attribute_qname = axis2_qname_create(env,
                AXIS2_ATTRIBUTE_DEFAULT_VERSION, NULL, NULL);
        if (NULL == attribute_qname)
        {
            continue;
        }
        default_version = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE(om_element, env,
                attribute_qname);
        AXIS2_QNAME_FREE(attribute_qname, env);
        attribute_qname = NULL;
        if (NULL == default_version)
        {
            return AXIS2_FAILURE;
        }
        AXIS2_CONF_ADD_DEFAULT_MODULE_VERSION(
            AXIS2_INTF_TO_IMPL(conf_builder)->conf, env, name,
            default_version);
    }
    return AXIS2_SUCCESS;
}

