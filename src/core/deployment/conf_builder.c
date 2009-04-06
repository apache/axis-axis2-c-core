
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
#include <axutil_class_loader.h>
#include <axis2_transport_in_desc.h>
#include <axis2_transport_out_desc.h>
#include <axutil_utils.h>
#include <axis2_transport_receiver.h>

struct axis2_conf_builder
{
    axis2_conf_t *conf;
    struct axis2_desc_builder *desc_builder;
};

static axis2_status_t axis2_conf_builder_process_disp_order(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_node_t * disp_order_node);

static axis2_status_t axis2_conf_builder_process_phase_orders(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_children_qname_iterator_t * phase_orders);

static axutil_array_list_t *axis2_conf_builder_get_phase_list(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_node_t * phase_orders_node);

static axis2_status_t axis2_conf_builder_process_transport_senders(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_children_qname_iterator_t * trs_senders);

static axis2_status_t axis2_conf_builder_process_transport_recvs(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_children_qname_iterator_t * trs_recvs);

static axis2_status_t AXIS2_CALL
axis2_conf_builder_process_default_module_versions(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_children_qname_iterator_t * module_versions);

AXIS2_EXTERN axis2_conf_builder_t *AXIS2_CALL
axis2_conf_builder_create(
    const axutil_env_t * env)
{
    axis2_conf_builder_t *conf_builder = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    conf_builder = (axis2_conf_builder_t *) AXIS2_MALLOC(env->allocator,
                                                         sizeof
                                                         (axis2_conf_builder_t));

    if (!conf_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    conf_builder->conf = NULL;

    return conf_builder;
}

AXIS2_EXTERN axis2_conf_builder_t *AXIS2_CALL
axis2_conf_builder_create_with_file_and_dep_engine_and_conf(
    const axutil_env_t * env,
    axis2_char_t * file,
    axis2_dep_engine_t * engine,
    axis2_conf_t * conf)
{
    axis2_conf_builder_t *conf_builder = NULL;

    conf_builder = (axis2_conf_builder_t *) axis2_conf_builder_create(env);
    if (!conf_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    conf_builder->desc_builder =
        axis2_desc_builder_create_with_file_and_dep_engine(env, file, engine);
    conf_builder->conf = conf;

    return conf_builder;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_conf_builder_free(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env)
{
    if (conf_builder->desc_builder)
    {
        axis2_desc_builder_free(conf_builder->desc_builder, env);
    }

    if (conf_builder)
    {
        AXIS2_FREE(env->allocator, conf_builder);
    }

    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_conf_builder_populate_conf(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env)
{
    axutil_qname_t *qparamst = NULL;
    axutil_qname_t *qmsgrecv = NULL;
    axutil_qname_t *qdisporder = NULL;
    axutil_qname_t *qmodulest = NULL;
    axutil_qname_t *qtransportsender = NULL;
    axutil_qname_t *qtransportrecv = NULL;
    axutil_qname_t *qphaseorder = NULL;
    axutil_qname_t *qdefmodver = NULL;
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
    axutil_param_t *param = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_conf_builder_populate_conf");
    conf_node = axis2_desc_builder_build_om(conf_builder->desc_builder, env);
    if (!conf_node)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Building om tree failed. Unable to continue");
        return AXIS2_FAILURE;
    }
    conf_element = axiom_node_get_data_element(conf_node, env);
    /* processing Paramters */
    /* Processing service level paramters */
    qparamst = axutil_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
    itr = axiom_element_get_children_with_qname(conf_element, env, qparamst,
                                                conf_node);
    axutil_qname_free(qparamst, env);
    axis2_desc_builder_process_params(conf_builder->desc_builder, env, itr,
                                      axis2_conf_get_param_container
                                      (conf_builder->conf, env),
                                      axis2_conf_get_param_container
                                      (conf_builder->conf, env));
    /* process Message Reciver */
    qmsgrecv = axutil_qname_create(env, AXIS2_MESSAGERECEIVER, NULL, NULL);
    msg_recvs = axiom_element_get_children_with_qname(conf_element, env,
                                                      qmsgrecv, conf_node);
    axutil_qname_free(qmsgrecv, env);
    while (axiom_children_qname_iterator_has_next(msg_recvs, env))
    {
        axiom_node_t *msg_recv_node = NULL;
        axiom_element_t *msg_recv_element = NULL;
        axis2_msg_recv_t *msg_recv = NULL;
        axiom_attribute_t *recv_name = NULL;
        axutil_qname_t *class_qname = NULL;
        axis2_char_t *class_name = NULL;

        msg_recv_node = (axiom_node_t *)
            axiom_children_qname_iterator_next(msg_recvs, env);
        msg_recv_element = (axiom_element_t *)
            axiom_node_get_data_element(msg_recv_node, env);
        msg_recv = axis2_desc_builder_load_msg_recv(conf_builder->desc_builder,
                                                    env, msg_recv_element);
        if (!msg_recv)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Message receiver loading failed. Unable to continue");
            return AXIS2_FAILURE;
        }

        class_qname = axutil_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
        recv_name = axiom_element_get_attribute(msg_recv_element, env, class_qname);
        axutil_qname_free(class_qname, env);
        class_name = axiom_attribute_get_value(recv_name, env);
        axis2_conf_add_msg_recv(conf_builder->conf, env, class_name, msg_recv);
    }

    /* processing Dispatching Order */
    qdisporder = axutil_qname_create(env, AXIS2_DISPATCH_ORDER, NULL, NULL);
    disp_order_element =
        axiom_element_get_first_child_with_qname(conf_element, env, qdisporder,
                                                 conf_node, &disp_order_node);
    axutil_qname_free(qdisporder, env);
    if (disp_order_element)
    {
        axis2_conf_builder_process_disp_order(conf_builder, env,
                                              disp_order_node);
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Found the custom disptaching"\
            " order and continue with that order");
    }
    else
    {
        status = axis2_conf_set_default_dispatchers(conf_builder->conf, env);
        if (!status)
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                "Setting default dispatchers to configuration failed, "\
                "unable to continue.");
            return AXIS2_FAILURE;
        }

        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "No custom dispatching order"\
            " found. Continue with the default dispatching order");
    }

    /* Process Module refs */
    qmodulest = axutil_qname_create(env, AXIS2_MODULEST, NULL, NULL);
    module_itr = axiom_element_get_children_with_qname(conf_element, env,
                                                       qmodulest, conf_node);
    axutil_qname_free(qmodulest, env);
    status =
        axis2_conf_builder_process_module_refs(conf_builder, env, module_itr);
    if (!status)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
            "Processing module ref's failed, unable to continue.");
        return AXIS2_FAILURE;
    }
    /* Proccessing Transport Sennders */
    qtransportsender =
        axutil_qname_create(env, AXIS2_TRANSPORTSENDER, NULL, NULL);
    trs_senders =
        axiom_element_get_children_with_qname(conf_element, env,
                                              qtransportsender, conf_node);
    axutil_qname_free(qtransportsender, env);
    status = axis2_conf_builder_process_transport_senders(conf_builder, env,
                                                          trs_senders);
    if (!status)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
            "Processing transport senders failed, unable to continue");
        return AXIS2_FAILURE;
    }

    /* Proccessing Transport Recivers */
    qtransportrecv =
        axutil_qname_create(env, AXIS2_TRANSPORTRECEIVER, NULL, NULL);
    trs_recvs =
        axiom_element_get_children_with_qname(conf_element, env, qtransportrecv,
                                              conf_node);
    axutil_qname_free(qtransportrecv, env);
    status = axis2_conf_builder_process_transport_recvs(conf_builder, env, trs_recvs);

    if (!status)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
            "Processing transport receivers failed, unable to continue");
        return AXIS2_FAILURE;
    }

    /* processing Phase orders */
    qphaseorder = axutil_qname_create(env, AXIS2_PHASE_ORDER, NULL, NULL);
    phase_orders = axiom_element_get_children_with_qname(conf_element, env,
                                                         qphaseorder,
                                                         conf_node);
    axutil_qname_free(qphaseorder, env);
    status = axis2_conf_builder_process_phase_orders(conf_builder, env, phase_orders);

    if (!status)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
            "Processing phase orders failed, unable to continue");
        return AXIS2_FAILURE;
    }

    /* Processing default module versions */
    qdefmodver = axutil_qname_create(env, AXIS2_DEFAULT_MODULE_VERSION, NULL,
                                     NULL);
    def_mod_versions = axiom_element_get_children_with_qname(conf_element,
                                                             env, qdefmodver,
                                                             conf_node);
    axutil_qname_free(qdefmodver, env);
    if (def_mod_versions)
    {
        status =
            axis2_conf_builder_process_default_module_versions(conf_builder,
                                                               env,
                                                               def_mod_versions);
        if (!status)
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                "Processing default module versions failed, unable to continue");
            return AXIS2_FAILURE;
        }
    }

    param =
        axutil_param_container_get_param(axis2_conf_get_param_container
                                         (conf_builder->conf, env), env,
                                         AXIS2_ENABLE_MTOM);
    if (param)
    {
        axis2_char_t *value = NULL;
        value = axutil_param_get_value(param, env);
        if (value)
        {
            axis2_conf_set_enable_mtom(conf_builder->conf, env,
                                       (!axutil_strcmp(value, AXIS2_VALUE_TRUE)));
        }
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_conf_builder_populate_conf");
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_conf_builder_process_module_refs(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_children_qname_iterator_t * module_refs)
{
    axis2_status_t status = AXIS2_SUCCESS;
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_conf_builder_process_module_refs");
    AXIS2_PARAM_CHECK(env->error, module_refs, AXIS2_FAILURE);

    while (axiom_children_qname_iterator_has_next(module_refs, env))
    {
        axiom_node_t *module_ref_node = NULL;
        axiom_element_t *module_ref_element = NULL;
        axutil_qname_t *qref = NULL;

        axiom_attribute_t *module_ref_att = NULL;
        module_ref_node = (axiom_node_t *)
            axiom_children_qname_iterator_next(module_refs, env);
        if (!module_ref_node)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Module ref node is empty. Unable to continue");
            return AXIS2_FAILURE;
        }
        module_ref_element = axiom_node_get_data_element(module_ref_node, env);
        qref = axutil_qname_create(env, AXIS2_REF, NULL, NULL);
        module_ref_att = axiom_element_get_attribute(module_ref_element, env,
                                                     qref);
        if (qref)
        {
            axutil_qname_free(qref, env);
        }

        if (module_ref_att)
        {
            axutil_qname_t *qrefname = NULL;
            axis2_char_t *ref_name = NULL;

            ref_name = axiom_attribute_get_value(module_ref_att, env);
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "Module %s found in axis2.xml", ref_name);
            qrefname = axutil_qname_create(env, ref_name, NULL, NULL);
            status =
                axis2_dep_engine_add_module(axis2_desc_builder_get_dep_engine
                                            (conf_builder->desc_builder, env),
                                            env, qrefname);
            if (qrefname)
            {
                axutil_qname_free(qrefname, env);
            }
        }
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_conf_builder_process_module_refs");
    return status;
}

static axis2_status_t
axis2_conf_builder_process_disp_order(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_node_t * disp_order_node)
{
    axiom_element_t *disp_order_element = NULL;
    axiom_children_qname_iterator_t *disps = NULL;
    axutil_qname_t *qdisp = NULL;
    axis2_bool_t found_disp = AXIS2_FALSE;
    axis2_phase_t *disp_phase = NULL;
    int count = 0;
    axis2_bool_t qname_itr_has_next = AXIS2_FALSE;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_conf_builder_process_disp_order");
    AXIS2_PARAM_CHECK(env->error, disp_order_node, AXIS2_FAILURE);

    disp_order_element = axiom_node_get_data_element(disp_order_node, env);
    qdisp = axutil_qname_create(env, AXIS2_DISPATCHER, NULL, NULL);
    disps = axiom_element_get_children_with_qname(disp_order_element,
                                                  env, qdisp, disp_order_node);
    axutil_qname_free(qdisp, env);
    disp_phase = axis2_phase_create(env, AXIS2_PHASE_DISPATCH);
    if (!disp_phase)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Dispatch phase creation failed. Unable to continue");
        return AXIS2_FAILURE;
    }
    if (disps)
    {
        qname_itr_has_next = axiom_children_qname_iterator_has_next(disps, env);
    }
    while (qname_itr_has_next)
    {
        axiom_node_t *disp_node = NULL;
        axiom_element_t *disp_element = NULL;
        axiom_attribute_t *disp_att = NULL;
        axis2_char_t *class_name = NULL;
        axis2_char_t *dll_name = NULL;
        axutil_qname_t *class_qname = NULL;
        axis2_disp_t *disp_dll = NULL;
        axutil_dll_desc_t *dll_desc = NULL;
        axutil_param_t *impl_info_param = NULL;
        axis2_handler_desc_t *handler_desc = NULL;
        axis2_handler_t *handler = NULL;

        found_disp = AXIS2_TRUE;
        disp_node = (axiom_node_t *)
            axiom_children_qname_iterator_next(disps, env);
        class_qname = axutil_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
        disp_att = axiom_element_get_attribute(disp_element, env, class_qname);
        axutil_qname_free(class_qname, env);
        if (!disp_att)
        {
            qname_itr_has_next = axiom_children_qname_iterator_has_next(disps,
                                                                        env);
            continue;
        }
        class_name = axiom_attribute_get_value(disp_att, env);
        dll_desc = axutil_dll_desc_create(env);
        dll_name = axutil_dll_desc_create_platform_specific_dll_name(dll_desc, env, class_name);
        axutil_dll_desc_set_name(dll_desc, env, dll_name);
        axutil_dll_desc_set_type(dll_desc, env, AXIS2_HANDLER_DLL);
        impl_info_param = axutil_param_create(env, class_name, NULL);
        if (!impl_info_param)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Parameter creation failed for %s. Unable to continue", class_name);

            axis2_phase_free(disp_phase, env);

            return AXIS2_FAILURE;
        }

        axutil_param_set_value(impl_info_param, env, dll_desc);
        axutil_param_set_value_free(impl_info_param, env,
                                    axutil_dll_desc_free_void_arg);
        axutil_class_loader_init(env);
        disp_dll = (axis2_disp_t *) axutil_class_loader_create_dll(env,
                                                           impl_info_param);

        handler = axis2_disp_get_base(disp_dll, env);
        handler_desc = axis2_handler_get_handler_desc(handler, env);
        axis2_handler_desc_add_param(handler_desc, env, impl_info_param);

        axis2_phase_add_handler_at(disp_phase, env, count, handler);
        count++;
        qname_itr_has_next = axiom_children_qname_iterator_has_next(disps, env);

    }

    if (!found_disp)
    {
        axis2_phase_free(disp_phase, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_DISPATCHER_FOUND, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No dispatcher found. Unable to continue");

        return AXIS2_FAILURE;
    }
    else
    {
        status =
            axis2_conf_set_dispatch_phase(conf_builder->conf, env, disp_phase);
        if (!status)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Setting dispatch phase failed. Unable to continue");

            axis2_phase_free(disp_phase, env);

            return status;
        }
    }

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Exit:axis2_conf_builder_process_disp_order");
    return AXIS2_SUCCESS;
}

/**
 * To process all the phase orders which are defined in axis2.xml retrieve each phase order node
 * from the iterator passed as parameter and from each phase order node retrieve phases list
 * defined for that phase order. Add the phases names into a array list and set it into the 
 * dep_engine->phases_info with the corresponding phase order name.
 * @param phase_orders
 */
static axis2_status_t
axis2_conf_builder_process_phase_orders(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_children_qname_iterator_t * phase_orders)
{
    axis2_phases_info_t *info = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Entry:axis2_conf_builder_process_phase_orders");

    AXIS2_PARAM_CHECK(env->error, phase_orders, AXIS2_FAILURE);

    info = axis2_dep_engine_get_phases_info(axis2_desc_builder_get_dep_engine(
                conf_builder->desc_builder, env), env);

    while (axiom_children_qname_iterator_has_next(phase_orders, env))
    {
        axiom_node_t *phase_orders_node = NULL;
        axiom_element_t *phase_orders_element = NULL;
        axiom_attribute_t *phase_orders_att = NULL;
        axutil_qname_t *qtype = NULL;
        axis2_char_t *flow_type = NULL;
        axutil_array_list_t *phase_list = NULL;

        phase_orders_node = (axiom_node_t *) axiom_children_qname_iterator_next(phase_orders, env);
        if (phase_orders_node)
        {
            phase_orders_element = axiom_node_get_data_element(phase_orders_node, env);
        }
        if (phase_orders_element)
        {
            qtype = axutil_qname_create(env, AXIS2_TYPE, NULL, NULL);
            phase_orders_att = axiom_element_get_attribute(phase_orders_element, env, qtype);
            axutil_qname_free(qtype, env);
        }

        if (phase_orders_att)
        {
            flow_type = axiom_attribute_get_value(phase_orders_att, env);
        }

        phase_list = axis2_conf_builder_get_phase_list(conf_builder, env, phase_orders_node);
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

        if (flow_type && !axutil_strcmp(AXIS2_IN_FLOW_START, flow_type))
        {
            axis2_phases_info_set_in_phases(info, env, phase_list);
        }
        else if (flow_type && !axutil_strcmp(AXIS2_IN_FAILTFLOW, flow_type))
        {
            axis2_phases_info_set_in_faultphases(info, env, phase_list);
        }
        else if (flow_type && !axutil_strcmp(AXIS2_OUT_FLOW_START, flow_type))
        {
            axis2_phases_info_set_out_phases(info, env, phase_list);
        }
        else if (flow_type && !axutil_strcmp(AXIS2_OUT_FAILTFLOW, flow_type))
        {
            axis2_phases_info_set_out_faultphases(info, env, phase_list);
        }
    }

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Exit:axis2_conf_builder_process_phase_orders");

    return AXIS2_SUCCESS;
}

/* From the phase order node passed as parameter retrieve all phase element nodes. From them extract
 * the phase name and add it to a array list. Return the array list.
 */
static axutil_array_list_t *
axis2_conf_builder_get_phase_list(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_node_t * phase_orders_node)
{
    axutil_array_list_t *phase_list = NULL;
    axiom_children_qname_iterator_t *phases = NULL;
    axutil_qname_t *qphase = NULL;
    axiom_element_t *phase_orders_element;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Entry:axis2_conf_builder_get_phase_list");

    AXIS2_PARAM_CHECK(env->error, phase_orders_node, NULL);

    phase_orders_element = axiom_node_get_data_element(phase_orders_node, env);
    if (!phase_orders_element)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_DATA_ELEMENT_IS_NULL, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Retrieving phase orders data element from phase orders node failed. Unable to continue");

        return NULL;
    }

    phase_list = axutil_array_list_create(env, 0);
    qphase = axutil_qname_create(env, AXIS2_PHASE, NULL, NULL);
    phases = axiom_element_get_children_with_qname(phase_orders_element, env, qphase, 
            phase_orders_node);

    axutil_qname_free(qphase, env);
    if (!phases)
    {
        axutil_array_list_free(phase_list, env);

        /* I guess this is not an error. So adding debug message*/
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "No phase node found in the phase orders node");

        return NULL;
    }

    while (axiom_children_qname_iterator_has_next(phases, env))
    {
        axiom_node_t *phase_node = NULL;
        axiom_element_t *phase_element = NULL;
        axiom_attribute_t *phase_att = NULL;
        axutil_qname_t *qattname = NULL;
        axis2_char_t *att_value = NULL;

        phase_node = (axiom_node_t *) axiom_children_qname_iterator_next(phases, env);
        if (phase_node)
        {
            phase_element = (axiom_element_t *) axiom_node_get_data_element(phase_node, env);
        }

        qattname = axutil_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        if (phase_element)
        {
            phase_att = axiom_element_get_attribute(phase_element, env, qattname);
        }

        if (phase_att)
        {
            att_value = axiom_attribute_get_value(phase_att, env);
        }

        if (att_value)
        {
            axutil_array_list_add(phase_list, env, att_value);
        }

        axutil_qname_free(qattname, env);
    }

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Exit:axis2_conf_builder_get_phase_list");

    return phase_list;
}

static axis2_status_t
axis2_conf_builder_process_transport_senders(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_children_qname_iterator_t * trs_senders)
{
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_conf_builder_process_transport_senders");
    while (axiom_children_qname_iterator_has_next(trs_senders, env))
    {
        axis2_transport_out_desc_t *transport_out = NULL;
        axiom_node_t *transport_node = NULL;
        axiom_element_t *transport_element = NULL;
        axiom_attribute_t *trs_name = NULL;
        axutil_qname_t *qattname = NULL;

        transport_node = (axiom_node_t *)
            axiom_children_qname_iterator_next(trs_senders, env);

        if (transport_node)
        {
            transport_element = (axiom_element_t *)
                axiom_node_get_data_element(transport_node, env);
            if (!transport_element)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Retrieving data element failed from the transport node."\
                    " Unable to continue");
                return AXIS2_FAILURE;
            }
        }
        else
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Transport node is empty. Unable to continue");
            return AXIS2_FAILURE;
        }

        /* getting trsnport Name */
        qattname = axutil_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        if (transport_element)
        {
            trs_name = axiom_element_get_attribute(transport_element, env,
                                                   qattname);
        }

        axutil_qname_free(qattname, env);
        if (trs_name)
        {
            axis2_char_t *name = NULL;
            axiom_attribute_t *trs_dll_att = NULL;
            axis2_char_t *dll_name = NULL;
            axis2_char_t *class_name = NULL;
            axiom_children_qname_iterator_t *itr = NULL;
            axutil_qname_t *qparamst = NULL;
            axutil_qname_t *qinflowst = NULL;
            axutil_qname_t *qoutflowst = NULL;
            axutil_qname_t *qinfaultflowst = NULL;
            axutil_qname_t *qoutfaultflowst = NULL;
            axutil_qname_t *qdllname = NULL;
            axiom_element_t *in_flow_element = NULL;
            axiom_node_t *in_flow_node = NULL;
            axiom_element_t *out_flow_element = NULL;
            axiom_node_t *out_flow_node = NULL;
            axiom_element_t *in_fault_flow_element = NULL;
            axiom_node_t *in_fault_flow_node = NULL;
            axiom_element_t *out_fault_flow_element = NULL;
            axiom_node_t *out_fault_flow_node = NULL;
            axutil_dll_desc_t *dll_desc = NULL;
            axutil_param_t *impl_info_param = NULL;
            void *transport_sender = NULL;
            axis2_char_t *path_qualified_dll_name = NULL;
            axis2_char_t *repos_name = NULL;
            axis2_char_t *temp_path = NULL;
            axis2_char_t *temp_path2 = NULL;
            axis2_char_t *temp_path3 = NULL;
            AXIS2_TRANSPORT_ENUMS transport_enum = AXIS2_TRANSPORT_ENUM_HTTP;
            /* AXIS2_TRANSPORT_ENUMS transport_enum = AXIS2_TRANSPORT_ENUM_HTTP
             * set to avoid C4701 on Windows
             */
            axis2_bool_t axis2_flag = AXIS2_FALSE;
            axutil_param_t *libparam;
            axis2_char_t *libdir=NULL;
            axis2_conf_t *conf; 

            conf = conf_builder->conf;
            axis2_flag = axis2_conf_get_axis2_flag (conf, env);

            name = axiom_attribute_get_value(trs_name, env);

            if (name)
            {
                if (!axutil_strcmp(name, AXIS2_TRANSPORT_HTTP))
                {
                    transport_enum = AXIS2_TRANSPORT_ENUM_HTTP;
                }
                else if (!axutil_strcmp(name, AXIS2_TRANSPORT_SMTP))
                {
                    transport_enum = AXIS2_TRANSPORT_ENUM_SMTP;
                }
                else if (!axutil_strcmp(name, AXIS2_TRANSPORT_XMPP))
                {
                    transport_enum = AXIS2_TRANSPORT_ENUM_XMPP;
                }
                else if (!axutil_strcmp(name, AXIS2_TRANSPORT_TCP))
                {
                    transport_enum = AXIS2_TRANSPORT_ENUM_TCP;
                }
                else if (!axutil_strcmp(name, AXIS2_TRANSPORT_HTTPS))
                {
                    transport_enum = AXIS2_TRANSPORT_ENUM_HTTPS;
                }
				else if (!axutil_strcmp(name, AXIS2_TRANSPORT_AMQP))
				{
					transport_enum = AXIS2_TRANSPORT_ENUM_AMQP;
				}
				else if (!axutil_strcmp(name, AXIS2_TRANSPORT_UDP))
				{
					transport_enum = AXIS2_TRANSPORT_ENUM_UDP;
				}
                else
                {
                    AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI,
                        "Transport name %s doesn't match with transport enum. "\
                        "Unable to continue", name);
                    return AXIS2_FAILURE;
                }

                transport_out =
                    axis2_transport_out_desc_create(env, transport_enum);
            }

            if (!transport_out)
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI,
                    "Transport_out value is NULL for transport %s", name);
                return AXIS2_FAILURE;
            }

            /* transport impl class */
            qdllname = axutil_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
            trs_dll_att = axiom_element_get_attribute(transport_element, env,
                                                      qdllname);
            axutil_qname_free(qdllname, env);
            if (!trs_dll_att)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_TRANSPORT_SENDER_ERROR,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                    "Transport dll name attribute is not set in the "\
                    "%s transport element node", name);
                return AXIS2_FAILURE;
            }
            class_name = axiom_attribute_get_value(trs_dll_att, env);
            impl_info_param = axutil_param_create(env, class_name, NULL);
            if (!impl_info_param)
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                    "Creating module dll name parameter failed for %s. Unable "\
                    "to continue", class_name);
                axis2_transport_out_desc_free(transport_out, env);
                return AXIS2_FAILURE;
            }
            dll_desc = axutil_dll_desc_create(env);
            dll_name =
                axutil_dll_desc_create_platform_specific_dll_name(dll_desc, env,
                                                                  class_name);
            if (!axis2_flag)
            {
                repos_name =
                    axis2_dep_engine_get_repos_path
                    (axis2_desc_builder_get_dep_engine
                     (conf_builder->desc_builder, env), env);
                temp_path = axutil_stracat(env, repos_name, AXIS2_PATH_SEP_STR);
                temp_path2 = axutil_stracat(env, temp_path, AXIS2_LIB_FOLDER);
                temp_path3 = axutil_stracat(env, temp_path2, AXIS2_PATH_SEP_STR);
                path_qualified_dll_name = axutil_stracat(env, temp_path3, dll_name);
                AXIS2_FREE(env->allocator, temp_path);
                AXIS2_FREE(env->allocator, temp_path2);
                AXIS2_FREE(env->allocator, temp_path3);
            }
            else
            {
                libparam = axis2_conf_get_param (conf, env, AXIS2_LIB_DIR);
                if (libparam)
                {
                    libdir = axutil_param_get_value (libparam, env);
                }

                if (!libdir)
                {
                    AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Specifying" \
                        "services and modules directories using axis2.xml but"\
                        " path of the library directory is not present");
                    return AXIS2_FAILURE;
                }
                path_qualified_dll_name = axutil_strcat (env, libdir, 
                                                         AXIS2_PATH_SEP_STR,
                                                         dll_name,
                                                         NULL);
            }

            axutil_dll_desc_set_name(dll_desc, env, path_qualified_dll_name);
            AXIS2_FREE(env->allocator, path_qualified_dll_name);

            axutil_dll_desc_set_type(dll_desc, env, AXIS2_TRANSPORT_SENDER_DLL);
            axutil_param_set_value(impl_info_param, env, dll_desc);
            axutil_param_set_value_free(impl_info_param, env,
                                        axutil_dll_desc_free_void_arg);
            axutil_class_loader_init(env);
            transport_sender =
                axutil_class_loader_create_dll(env, impl_info_param);
            axis2_transport_out_desc_add_param(transport_out, env,
                                               impl_info_param);

            if (!transport_sender)
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI,
                    "Transport sender is NULL for transport %s, unable to "\
                    "continue", name);
                axis2_transport_out_desc_free(transport_out, env);
                return status;
            }

            status =
                axis2_transport_out_desc_set_sender(transport_out, env,
                                                    transport_sender);
            if (!status)
            {
                axis2_transport_out_desc_free(transport_out, env);
                return status;
            }

            /* Process Parameters */
            /* Processing service level paramters */
            qparamst = axutil_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
            itr = axiom_element_get_children_with_qname(transport_element,
                                                        env, qparamst,
                                                        transport_node);
            axutil_qname_free(qparamst, env);
            status =
                axis2_desc_builder_process_params(conf_builder->desc_builder,
                                                  env, itr,
                                                  axis2_transport_out_desc_param_container
                                                  (transport_out, env),
                                                  axis2_conf_get_param_container
                                                  (conf_builder->conf, env));
            if (!status)
            {
                axis2_transport_out_desc_free(transport_out, env);
                return status;
            }
            /* process IN_FLOW */
            qinflowst =
                axutil_qname_create(env, AXIS2_IN_FLOW_START, NULL, NULL);
            in_flow_element =
                axiom_element_get_first_child_with_qname(transport_element, env,
                                                         qinflowst,
                                                         transport_node,
                                                         &in_flow_node);
            axutil_qname_free(qinflowst, env);
            if (in_flow_element)
            {
                axis2_transport_out_desc_free(transport_out, env);
                AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_IN_FLOW_NOT_ALLOWED_IN_TRS_OUT,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                    "Transport in flow element is not allowed in the out path");
                return AXIS2_FAILURE;
            }
            qoutflowst =
                axutil_qname_create(env, AXIS2_OUT_FLOW_START, NULL, NULL);
            out_flow_element =
                axiom_element_get_first_child_with_qname(transport_element, env,
                                                         qoutflowst,
                                                         transport_node,
                                                         &out_flow_node);
            axutil_qname_free(qoutflowst, env);
            if (out_flow_element)
            {
                axis2_flow_t *flow = NULL;

                flow =
                    axis2_desc_builder_process_flow(conf_builder->desc_builder,
                                                    env, out_flow_element,
                                                    axis2_conf_get_param_container
                                                    (conf_builder->conf, env),
                                                    out_flow_node);
                status =
                    axis2_transport_out_desc_set_out_flow(transport_out, env,
                                                          flow);
                if (!status)
                {
                    axis2_transport_out_desc_free(transport_out, env);
                    AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                        "Setting the out flow failed on the trasnport out "\
                        "description for the transport %s", name);
                    return status;
                }
            }

            /* process IN FAULT FLOW */
            qinfaultflowst = axutil_qname_create(env, AXIS2_IN_FAILTFLOW, NULL,
                                                 NULL);
            in_fault_flow_element =
                axiom_element_get_first_child_with_qname(transport_element,
                                                         env, qinfaultflowst,
                                                         transport_node,
                                                         &in_fault_flow_node);
            axutil_qname_free(qinfaultflowst, env);
            if (in_fault_flow_element)
            {
                axis2_transport_out_desc_free(transport_out, env);
                AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_IN_FLOW_NOT_ALLOWED_IN_TRS_OUT,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                    "A soap fault has occured in the in flow while "\
                    "processing transport senders. Unable to continue");
                return AXIS2_FAILURE;
            }

            qoutfaultflowst =
                axutil_qname_create(env, AXIS2_OUT_FAILTFLOW, NULL, NULL);
            out_fault_flow_element =
                axiom_element_get_first_child_with_qname(transport_element, env,
                                                         qoutfaultflowst,
                                                         transport_node,
                                                         &out_fault_flow_node);
            axutil_qname_free(qoutfaultflowst, env);
            if (out_fault_flow_element)
            {
                axis2_flow_t *flow = NULL;

                flow =
                    axis2_desc_builder_process_flow(conf_builder->desc_builder,
                                                    env, out_fault_flow_element,
                                                    axis2_conf_get_param_container
                                                    (conf_builder->conf, env),
                                                    out_fault_flow_node);
                status =
                    axis2_transport_out_desc_set_fault_out_flow(transport_out,
                                                                env, flow);
                if (!status)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "Setting the fault out flow into the transport out "\
                        "failed");
                    axis2_transport_out_desc_free(transport_out, env);
                    return status;
                }
            }

            /* adding to axis config */
            status = axis2_conf_add_transport_out(conf_builder->conf, env,
                                                  transport_out,
                                                  transport_enum);
            if (!status)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Adding transport out for %s into main configuration failed", 
                    name);
                axis2_transport_out_desc_free(transport_out, env);
                return status;
            }
        }
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_conf_builder_process_transport_senders");
    return AXIS2_SUCCESS;
}

static axis2_status_t
axis2_conf_builder_process_transport_recvs(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_children_qname_iterator_t * trs_recvs)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_conf_t *conf;
    axis2_bool_t axis2_flag = AXIS2_FALSE;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Entry:axis2_conf_builder_process_transport_recvs");
    AXIS2_PARAM_CHECK(env->error, trs_recvs, AXIS2_FAILURE);

    conf = conf_builder->conf;
    axis2_flag = axis2_conf_get_axis2_flag (conf, env);


    while (axiom_children_qname_iterator_has_next(trs_recvs, env))
    {
        axis2_transport_in_desc_t *transport_in = NULL;
        axiom_node_t *transport_node = NULL;
        axiom_element_t *transport_element = NULL;
        axiom_attribute_t *trs_name = NULL;
        axutil_qname_t *qattname = NULL;

        transport_node = (axiom_node_t *)
            axiom_children_qname_iterator_next(trs_recvs, env);
        if (transport_node)
        {
            transport_element = axiom_node_get_data_element(transport_node,
                                                            env);
            if (!transport_element)
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                    "Retrieving data element from the transport node failed.");
                return AXIS2_FAILURE;
            }
        }
        else
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                "Transport node is NULL. Unable to continue");
            return AXIS2_FAILURE;
        }

        /* getting transport Name */
        qattname = axutil_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        trs_name = axiom_element_get_attribute(transport_element, env,
                                               qattname);
        axutil_qname_free(qattname, env);

        if (trs_name)
        {
            axis2_char_t *name = NULL;
            axiom_attribute_t *trs_class_name = NULL;
            axiom_children_qname_iterator_t *itr = NULL;
            axutil_qname_t *class_qname = NULL;
            axutil_qname_t *qparamst = NULL;
            axutil_qname_t *qinflowst = NULL;
            axutil_qname_t *qoutflowst = NULL;
            axutil_qname_t *qinfaultflowst = NULL;
            axutil_qname_t *qoutfaultflowst = NULL;
            axiom_element_t *in_flow_element = NULL;
            axiom_node_t *in_flow_node = NULL;
            axiom_element_t *out_flow_element = NULL;
            axiom_node_t *out_flow_node = NULL;
            axiom_element_t *in_fault_flow_element = NULL;
            axiom_node_t *in_fault_flow_node = NULL;
            axiom_element_t *out_fault_flow_element = NULL;
            axiom_node_t *out_fault_flow_node = NULL;
            AXIS2_TRANSPORT_ENUMS transport_enum = AXIS2_TRANSPORT_ENUM_HTTP;
            /* AXIS2_TRANSPORT_ENUMS transport_enum = AXIS2_TRANSPORT_ENUM_HTTP
             * set to avoid C4701 on Windows
             */

            name = axiom_attribute_get_value(trs_name, env);
            if (name)
            {
                if (!axutil_strcmp(name, AXIS2_TRANSPORT_HTTP))
                {
                    transport_enum = AXIS2_TRANSPORT_ENUM_HTTP;
                }
                else if (!axutil_strcmp(name, AXIS2_TRANSPORT_SMTP))
                {
                    transport_enum = AXIS2_TRANSPORT_ENUM_SMTP;
                }
                else if (!axutil_strcmp(name, AXIS2_TRANSPORT_TCP))
                {
                    transport_enum = AXIS2_TRANSPORT_ENUM_TCP;
                }
                else if (!axutil_strcmp(name, AXIS2_TRANSPORT_HTTPS))
                {
                    transport_enum = AXIS2_TRANSPORT_ENUM_HTTPS;
                }
				else if (!axutil_strcmp(name, AXIS2_TRANSPORT_AMQP))
				{
					transport_enum = AXIS2_TRANSPORT_ENUM_AMQP;
				}
				else if (!axutil_strcmp(name, AXIS2_TRANSPORT_UDP))
				{
					transport_enum = AXIS2_TRANSPORT_ENUM_UDP;
				}
                else
                {
                    AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                        "Transport %s could not be recognized.", name);
                    return AXIS2_FAILURE;
                }

                transport_in = axis2_transport_in_desc_create(env,
                                                              transport_enum);
            }
            if (!transport_in)
            {
                AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                    "Creating trasport_in_desc for transport %s failed", name);
                return AXIS2_FAILURE;
            }

            /* transport impl class */
            class_qname = axutil_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
            trs_class_name = axiom_element_get_attribute(transport_element, env,
                                                         class_qname);
            axutil_qname_free(class_qname, env);

            if (trs_class_name)
            {
                axis2_char_t *class_name = NULL;
                axis2_char_t *dll_name = NULL;
                axutil_dll_desc_t *dll_desc = NULL;
                axutil_param_t *impl_info_param = NULL;
                axis2_transport_receiver_t *recv = NULL;
                axis2_status_t stat = AXIS2_FAILURE;
                axis2_char_t *path_qualified_dll_name = NULL;
                axis2_char_t *repos_name = NULL;
                axis2_char_t *temp_path = NULL;
                axis2_char_t *temp_path2 = NULL;
                axis2_char_t *temp_path3 = NULL;
                
                axutil_param_t *tnsparam;
                axis2_char_t *libpath;

                class_name = axiom_attribute_get_value(trs_class_name, env);
                impl_info_param = axutil_param_create(env, class_name, NULL);
                dll_desc = axutil_dll_desc_create(env);
                dll_name =
                    axutil_dll_desc_create_platform_specific_dll_name(dll_desc,
                                                                      env,
                                                                      class_name);
                if (!axis2_flag)
                {
                    /* Axis2 Configuration is not built using axis2.xml */
                    repos_name =
                        axis2_dep_engine_get_repos_path
                        (axis2_desc_builder_get_dep_engine
                         (conf_builder->desc_builder, env), env);
                    temp_path = axutil_stracat(env, repos_name, AXIS2_PATH_SEP_STR);
                    temp_path2 = axutil_stracat(env, temp_path, AXIS2_LIB_FOLDER);
                    temp_path3 =
                        axutil_stracat(env, temp_path2, AXIS2_PATH_SEP_STR);
                    path_qualified_dll_name =
                        axutil_stracat(env, temp_path3, dll_name);
                    AXIS2_FREE(env->allocator, temp_path);
                    AXIS2_FREE(env->allocator, temp_path2);
                    AXIS2_FREE(env->allocator, temp_path3);
                }
                else
                {
                    /* Axis2 Configuration is built using axis2.xml */
                    tnsparam = axis2_conf_get_param (conf, env, AXIS2_LIB_DIR);
                    if (tnsparam)
                    {
                        libpath = (axis2_char_t *)axutil_param_get_value (tnsparam, env);

                        if (libpath)
                        {
                            path_qualified_dll_name = axutil_strcat (env, libpath,
                                                                     AXIS2_PATH_SEP_STR,
                                                                     dll_name, NULL);
                        }
                    }
                }

                axutil_dll_desc_set_name(dll_desc, env,
                                         path_qualified_dll_name);
                AXIS2_FREE(env->allocator, path_qualified_dll_name);
                axutil_dll_desc_set_type(dll_desc, env,
                                         AXIS2_TRANSPORT_RECV_DLL);

                axutil_param_set_value(impl_info_param, env, dll_desc);
                axutil_param_set_value_free(impl_info_param, env,
                                            axutil_dll_desc_free_void_arg);
                axutil_class_loader_init(env);
                recv = (axis2_transport_receiver_t *)
                    axutil_class_loader_create_dll(env, impl_info_param);
                axis2_transport_in_desc_add_param(transport_in, env,
                                                  impl_info_param);

                if (!recv)
                {
                    AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI,
                        "Transport receiver loading failed for %s, "\
                        "unable to continue", dll_name);
                    axis2_transport_in_desc_free(transport_in, env);
                    return status;
                }

                stat = axis2_transport_in_desc_set_recv(transport_in, env,
                                                        recv);

                if (!stat)
                {
                    AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI,
                        "Setting transport receiver for transport %s into "\
                        "transport in description failed, unable to continue", 
                        name);
                    axis2_transport_in_desc_free(transport_in, env);
                    return stat;
                }

            }

            /* process Parameters */
            /* processing Paramters */
            /* Processing service level paramters */
            qparamst = axutil_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
            itr = axiom_element_get_children_with_qname(transport_element,
                                                        env, qparamst,
                                                        transport_node);
            axutil_qname_free(qparamst, env);
            status =
                axis2_desc_builder_process_params(conf_builder->desc_builder,
                                                  env, itr,
                                                  axis2_transport_in_desc_param_container
                                                  (transport_in, env),
                                                  axis2_conf_get_param_container
                                                  (conf_builder->conf, env));
            if (!status)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Processing transport receiver parameters failed. "\
                        "Unable to continue");
                axis2_transport_in_desc_free(transport_in, env);
                return status;
            }
            /* process OUT_FLOW */
            qoutflowst =
                axutil_qname_create(env, AXIS2_OUT_FLOW_START, NULL, NULL);
            out_flow_element =
                axiom_element_get_first_child_with_qname(transport_element, env,
                                                         qoutflowst,
                                                         transport_node,
                                                         &out_flow_node);
            axutil_qname_free(qoutflowst, env);
            if (out_flow_element)
            {
                axis2_transport_in_desc_free(transport_in, env);
                AXIS2_ERROR_SET(env->error, 
                    AXIS2_ERROR_OUT_FLOW_NOT_ALLOWED_IN_TRS_IN, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Out flow element not allowed in transport in path");
                return AXIS2_FAILURE;
            }

            qinflowst =
                axutil_qname_create(env, AXIS2_IN_FLOW_START, NULL, NULL);
            in_flow_element =
                axiom_element_get_first_child_with_qname(transport_element, env,
                                                         qinflowst,
                                                         transport_node,
                                                         &in_flow_node);
            axutil_qname_free(qinflowst, env);
            if (in_flow_element)
            {
                axis2_flow_t *flow = NULL;

                flow =
                    axis2_desc_builder_process_flow(conf_builder->desc_builder,
                                                    env, in_flow_element,
                                                    axis2_conf_get_param_container
                                                    (conf_builder->conf, env),
                                                    in_flow_node);
                status =
                    axis2_transport_in_desc_set_in_flow(transport_in, env,
                                                        flow);
                if (!status)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "Setting in flow into transport_in_desc of transport "\
                        "%s failed", name);
                    axis2_transport_in_desc_free(transport_in, env);
                    return status;
                }
            }

            qinfaultflowst = axutil_qname_create(env, AXIS2_IN_FAILTFLOW, NULL,
                                                 NULL);
            in_fault_flow_element =
                axiom_element_get_first_child_with_qname(transport_element,
                                                         env, qinfaultflowst,
                                                         transport_node,
                                                         &in_fault_flow_node);
            axutil_qname_free(qinfaultflowst, env);
            if (in_fault_flow_element)
            {
                axis2_flow_t *flow = NULL;

                flow =
                    axis2_desc_builder_process_flow(conf_builder->desc_builder,
                                                    env, in_fault_flow_element,
                                                    axis2_conf_get_param_container
                                                    (conf_builder->conf, env),
                                                    in_fault_flow_node);
                status =
                    axis2_transport_in_desc_set_fault_in_flow(transport_in, env,
                                                              flow);
                if (!status)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "Setting in fault flow into transport_in_desc of "\
                        "transport %s failed", name);
                    axis2_transport_in_desc_free(transport_in, env);
                    return status;
                }
            }

            qoutfaultflowst =
                axutil_qname_create(env, AXIS2_OUT_FAILTFLOW, NULL, NULL);
            out_fault_flow_element =
                axiom_element_get_first_child_with_qname(transport_element, env,
                                                         qoutfaultflowst,
                                                         transport_node,
                                                         &out_fault_flow_node);
            if (out_fault_flow_element)
            {
                AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_OUT_FLOW_NOT_ALLOWED_IN_TRS_IN,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Out flow element is not allowed in transport in path");
                return AXIS2_FAILURE;
            }

            /* adding to axis config */
            status = axis2_conf_add_transport_in(conf_builder->conf, env,
                                                 transport_in, transport_enum);
            if (!status)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Adding transport_in_desc for transport %s into main "\
                    "configuration failed.", name);
                axis2_transport_in_desc_free(transport_in, env);
                return status;
            }

            axutil_qname_free(qoutfaultflowst, env);

        }
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, 
        "Exit:axis2_conf_builder_process_transport_recvs");
    return AXIS2_SUCCESS;
}

static axis2_status_t AXIS2_CALL
axis2_conf_builder_process_default_module_versions(
    axis2_conf_builder_t * conf_builder,
    const axutil_env_t * env,
    axiom_children_qname_iterator_t * module_versions)
{
    AXIS2_PARAM_CHECK(env->error, module_versions, AXIS2_FAILURE);

    while (axiom_children_qname_iterator_has_next(module_versions, env))
    {
        axiom_element_t *om_element = NULL;
        axis2_char_t *name = NULL;
        axis2_char_t *default_version = NULL;
        axutil_qname_t *attribute_qname = NULL;

        om_element = (axiom_element_t *)
            axiom_children_qname_iterator_next(module_versions, env);
        if (!om_element)
        {
            continue;
        }
        attribute_qname = axutil_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        if (!attribute_qname)
        {
            continue;
        }
        name = axiom_element_get_attribute_value(om_element, env,
                                                 attribute_qname);
        axutil_qname_free(attribute_qname, env);
        attribute_qname = NULL;
        if (!name)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Attribute value is NULL for "\
                "attribute %s", AXIS2_ATTNAME);
            return AXIS2_FAILURE;
        }
        attribute_qname = axutil_qname_create(env,
                                              AXIS2_ATTRIBUTE_DEFAULT_VERSION,
                                              NULL, NULL);
        if (!attribute_qname)
        {
            continue;
        }
        default_version = axiom_element_get_attribute_value(om_element, env,
                                                            attribute_qname);
        axutil_qname_free(attribute_qname, env);
        attribute_qname = NULL;
        if (!default_version)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Attribute value is NULL for "\
                "attribute %s", AXIS2_ATTRIBUTE_DEFAULT_VERSION);
            return AXIS2_FAILURE;
        }
        axis2_conf_add_default_module_version(conf_builder->conf,
                                              env, name, default_version);
    }
    return AXIS2_SUCCESS;
}


