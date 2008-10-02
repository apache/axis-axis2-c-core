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

#include <axis2_rm_assertion_builder.h>

/*private functions*/



static axis2_status_t AXIS2_CALL
axis2_rm_assertion_builder_process_sandesha2_assertions(
    const axutil_env_t *env,
    axis2_rm_assertion_t *rm_assertion,
    axiom_node_t *node,
    axiom_element_t *element);

static axis2_status_t AXIS2_CALL
axis2_rm_assertion_builder_populate_for_10(
    const axutil_env_t *env,
    axis2_rm_assertion_t *rm_assertion,
    axiom_node_t *rm_assertion_node,
    axiom_element_t *rm_assertion_element);

static axis2_status_t AXIS2_CALL
axis2_rm_assertion_builder_populate_for_11(
    const axutil_env_t *env,
    axis2_rm_assertion_t *rm_assertion,
    axiom_node_t *rm_assertion_node,
    axiom_element_t *rm_assertion_element);

static axis2_status_t AXIS2_CALL
axis2_rm_assertion_builder_process_delivery_assuarance(
    const axutil_env_t *env,
    axis2_rm_assertion_t *rm_assertion,
    axiom_node_t *da_node,
    axiom_element_t *da_element);



/* This functions retrives a rm_assertion axiom_node 
 * which may be 1.0 or 1.1 and return a rm_assertion
 * struct */


AXIS2_EXTERN neethi_assertion_t *AXIS2_CALL
axis2_rm_assertion_builder_build(
    const axutil_env_t *env,
    axiom_node_t *rm_assertion_node,
    axiom_element_t *rm_assertion_ele)
{
    axis2_rm_assertion_t *rm_assertion = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axiom_children_iterator_t *children_iter = NULL;
    neethi_assertion_t *assertion = NULL;
    axis2_char_t *ns = NULL;
    axutil_qname_t *node_qname = NULL;

    node_qname = axiom_element_get_qname(rm_assertion_ele, env, rm_assertion_node);
    if(!node_qname)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[neethi] Cannot get qname from element");
        return NULL;
    }

    ns = axutil_qname_get_uri(node_qname, env);
    if(!ns)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[neethi] Cannot get namespace from element.");
        return NULL;
    }

    rm_assertion = axis2_rm_assertion_create(env);
    if (!rm_assertion)
    {
        return NULL;
    }

    /* First we check whether this is in 1.0 or 1.1 
     * namespace. Then we called the appropriate builder */

    children_iter = axiom_element_get_children(rm_assertion_ele, env, rm_assertion_node);

    if(!axutil_strcmp(ns, AXIS2_RM_POLICY_10_NS))
    {
        status = axis2_rm_assertion_builder_populate_for_10(env, rm_assertion, 
            rm_assertion_node, rm_assertion_ele);

        axiom_children_iterator_reset(children_iter, env);

    }
    else if(!axutil_strcmp(ns, AXIS2_RM_POLICY_11_NS))
    {
        status = axis2_rm_assertion_builder_populate_for_11(env, rm_assertion, 
            rm_assertion_node, rm_assertion_ele);
    }

    if(status == AXIS2_FAILURE)
    {
        axis2_rm_assertion_free(rm_assertion, env);
        rm_assertion = NULL;
        return NULL;
    }

    /*children_iter = axiom_element_get_children(rm_assertion_ele, env, rm_assertion_node);*/
    if (children_iter)
    {
        while (axiom_children_iterator_has_next(children_iter, env))
        {
            axiom_node_t *node = NULL;
            axiom_element_t *ele = NULL;
            axis2_char_t *local_name = NULL;
            axutil_qname_t *node_qname = NULL;

            node = axiom_children_iterator_next(children_iter, env);
            if (node)
            {
                if (axiom_node_get_node_type(node, env) == AXIOM_ELEMENT)
                {
                    ele =
                        (axiom_element_t *) axiom_node_get_data_element(node,
                                                                        env);
                    node_qname = axiom_element_get_qname(ele, env, node);
                    if(!node_qname)
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "[neethi] Cannot get qname from element %s.", local_name);
                        return NULL;
                    }
                    ns = axutil_qname_get_uri(node_qname, env);
                    if(!ns)
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "[neethi] Cannot get namespace from element %s.", local_name);
                        return NULL;
                    }
                    if(!axutil_strcmp(ns, AXIS2_SANDESHA2_NS))
                    {
                        status = axis2_rm_assertion_builder_process_sandesha2_assertions(
                            env, rm_assertion, node, ele);    
                        if(status == AXIS2_FAILURE)
                        {
                            axis2_rm_assertion_free(rm_assertion, env);
                            rm_assertion = NULL;
                            return NULL;
                        }
                    }
                }
            }
        }
    }
    assertion =
        neethi_assertion_create_with_args(env, (AXIS2_FREE_VOID_ARG)axis2_rm_assertion_free,
                                          rm_assertion,
                                          ASSERTION_TYPE_RM_ASSERTION);
    return assertion;
}

static axis2_status_t AXIS2_CALL
axis2_rm_assertion_builder_populate_for_10(
    const axutil_env_t *env,
    axis2_rm_assertion_t *rm_assertion,
    axiom_node_t *rm_assertion_node,
    axiom_element_t *rm_assertion_element)
{
    axiom_children_iterator_t *children_iter = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    /* In rm 1.0 it is just child elements which inside 
     * rm_assertion contains all the properties. */


    children_iter = axiom_element_get_children(rm_assertion_element, 
        env, rm_assertion_node);
    if (children_iter)
    {
        while (axiom_children_iterator_has_next(children_iter, env))
        {
            axiom_node_t *node = NULL;
            axiom_element_t *ele = NULL;
            axis2_char_t *local_name = NULL;
            node = axiom_children_iterator_next(children_iter, env);
            if (node)
            {
                if (axiom_node_get_node_type(node, env) == AXIOM_ELEMENT)
                {
                    ele =
                        (axiom_element_t *) axiom_node_get_data_element(node,
                                                                        env);
                    if (ele)
                    {
                        axutil_qname_t *node_qname = NULL;
                        axis2_char_t *ns = NULL;
                        node_qname = axiom_element_get_qname(ele, env, node);
                        if(!node)
                        {
                            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "[neethi] Cannot get qname from element");
                            return AXIS2_FAILURE;
                        }

                        ns = axutil_qname_get_uri(node_qname, env);
                        if(!ns)
                        {
                            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                "[neethi] Cannot get namespace from element.");
                            return AXIS2_FAILURE;
                        }
                        
                        if(axutil_strcmp(ns, AXIS2_RM_POLICY_10_NS))
                        {
                            continue;
                        }

                        local_name = axiom_element_get_localname(ele, env);
                        if (local_name)
                        {

                            if(!axutil_strcmp(local_name, AXIS2_RM_INACTIVITY_TIMEOUT))
                            {
                                axis2_char_t *inactivity_timeout  = NULL;
                                axutil_qname_t *qname =NULL;

                                qname = axutil_qname_create(env, "Milliseconds", NULL, NULL);    

                                inactivity_timeout = axiom_element_get_attribute_value(ele, env, qname);
                                if(qname)
                                {
                                    axutil_qname_free(qname, env);
                                    qname = NULL;
                                }
                                status = axis2_rm_assertion_set_inactivity_timeout(
                                    rm_assertion, env, inactivity_timeout);
                            }
                            else if(!axutil_strcmp(local_name, AXIS2_RM_BASE_RETRANSMISSION_INTERVAL))
                            {
                                axis2_char_t *rti = NULL;
                                axutil_qname_t *qname =NULL;
                                qname = axutil_qname_create(env, "Milliseconds", NULL, NULL);

                                rti = axiom_element_get_attribute_value(ele, env, qname);
                                if(qname)
                                {
                                    axutil_qname_free(qname, env);
                                    qname = NULL;
                                }
                                status = axis2_rm_assertion_set_retrans_interval(
                                    rm_assertion, env, rti);
                            }
                            else if(!axutil_strcmp(local_name, AXIS2_RM_EXPONENTIAL_BACK_OFF))
                            {
                                status = axis2_rm_assertion_set_is_exp_backoff(rm_assertion, env, AXIS2_TRUE);
                            }
                            else if(!axutil_strcmp(local_name, AXIS2_RM_ACKNOWLEDGEMENT_INTERVAL))
                            {
                                axis2_char_t *ack_interval = NULL;
                                axutil_qname_t *qname =NULL;

                                qname = axutil_qname_create(env, "Milliseconds", NULL, NULL);

                                ack_interval = axiom_element_get_attribute_value(ele, env, qname);
                                if(qname)
                                {
                                    axutil_qname_free(qname, env);
                                    qname = NULL;
                                }
                                status = axis2_rm_assertion_set_ack_interval(
                                    rm_assertion, env, ack_interval);
                            }
                            else
                            {
                                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                    "[neethi] Unknown Assertion %s ", local_name);
                                return AXIS2_FAILURE;
                            }
                        }
                    }
                }
            }
        }
    }
    return status;
}

/* In rm 1.1 rm_assertion contains a policy element as 
 * a child element. We are not creating a policy object 
 * for policy element. Becasue then the processing become 
 * complex.So we just parse the axiom model */


static axis2_status_t AXIS2_CALL
axis2_rm_assertion_builder_populate_for_11(
    const axutil_env_t *env,
    axis2_rm_assertion_t *rm_assertion,
    axiom_node_t *rm_assertion_node,
    axiom_element_t *rm_assertion_element)
{
    axiom_children_iterator_t *children_iter = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axiom_node_t *child_node = NULL;
    axiom_element_t *child_element = NULL;


    child_node = axiom_node_get_first_element(rm_assertion_node, env);
    if (child_node)
    {
        child_element =
            (axiom_element_t *) axiom_node_get_data_element(child_node, env);
        if (child_element)
        {
            children_iter = axiom_element_get_children(child_element, 
                env, child_node);
            if (children_iter)
            {
                while (axiom_children_iterator_has_next(children_iter, env))
                {
                    axiom_node_t *node = NULL;
                    axiom_element_t *ele = NULL;
                    axis2_char_t *local_name = NULL;
                    node = axiom_children_iterator_next(children_iter, env);
                    if (node)
                    {
                        if (axiom_node_get_node_type(node, env) == AXIOM_ELEMENT)
                        {
                            ele =
                                (axiom_element_t *) axiom_node_get_data_element(node,
                                                                        env);
                            if (ele)
                            {
                                axutil_qname_t *node_qname = NULL;
                                axis2_char_t *ns = NULL;
                                node_qname = axiom_element_get_qname(ele, env, node);
                                if(!node)
                                {
                                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                    "[neethi] Cannot get qname from element");
                                    return AXIS2_FAILURE;
                                }

                                ns = axutil_qname_get_uri(node_qname, env);
                                if(!ns)
                                {
                                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                        "[neethi] Cannot get namespace from element.");
                                    return AXIS2_FAILURE;
                                }

                                if(axutil_strcmp(ns, AXIS2_RM_POLICY_11_NS))
                                {
                                    continue;
                                }
                                local_name = axiom_element_get_localname(ele, env);
                                if (local_name)
                                {
                                    if(!axutil_strcmp(local_name, AXIS2_RM_SEQUENCE_STR)) 
                                    {
                                        status = axis2_rm_assertion_set_is_sequence_str(rm_assertion, env, AXIS2_TRUE);
                                    }   
                                    else if(!axutil_strcmp(local_name, AXIS2_RM_SEQUENCE_TRANSPORT_SECURITY))
                                    {
                                        status = axis2_rm_assertion_set_is_sequence_transport_security(rm_assertion, env, AXIS2_TRUE);
                                    }
                                    else if(!axutil_strcmp(local_name, AXIS2_RM_DELIVERY_ASSURANCE))
                                    {
                                        status = axis2_rm_assertion_builder_process_delivery_assuarance(
                                           env, rm_assertion, node, ele);                                        
                                    }
                                    else
                                    {
                                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                        "[neethi] Unknown Assertion %s ", local_name);
                                        return AXIS2_FAILURE;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }   
    }   

    return status;
}


static axis2_status_t AXIS2_CALL
axis2_rm_assertion_builder_process_delivery_assuarance(
    const axutil_env_t *env,
    axis2_rm_assertion_t *rm_assertion,
    axiom_node_t *da_node,
    axiom_element_t *da_element)
{
    axiom_children_iterator_t *children_iter = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axiom_node_t *child_node = NULL;
    axiom_element_t *child_element = NULL;

    child_node = axiom_node_get_first_element(da_node, env);
    if (child_node)
    {
        child_element =
            (axiom_element_t *) axiom_node_get_data_element(child_node, env);
        if (child_element)
        {
            children_iter = axiom_element_get_children(child_element, 
                env, child_node);
            if (children_iter)
            {
                while (axiom_children_iterator_has_next(children_iter, env))
                {
                    axiom_node_t *node = NULL;
                    axiom_element_t *ele = NULL;
                    axis2_char_t *local_name = NULL;
                    node = axiom_children_iterator_next(children_iter, env);
                    if (node)
                    {
                        if (axiom_node_get_node_type(node, env) == AXIOM_ELEMENT)
                        {
                            ele =
                                (axiom_element_t *) axiom_node_get_data_element(node,
                                                                        env);
                            if (ele)
                            {
                                local_name = axiom_element_get_localname(ele, env);
                                if (local_name)
                                {
                                    if(!axutil_strcmp(local_name, AXIS2_RM_EXACTLY_ONCE)) 
                                    {
                                        status = axis2_rm_assertion_set_is_exactly_once(rm_assertion, env, AXIS2_TRUE);
                                    }   
                                    else if(!axutil_strcmp(local_name, AXIS2_RM_AT_LEAST_ONCE))
                                    {
                                        status = axis2_rm_assertion_set_is_atleast_once(rm_assertion, env, AXIS2_TRUE);
                                    }
                                    else if(!axutil_strcmp(local_name, AXIS2_RM_AT_MOST_ONCE))
                                    {
                                        status = axis2_rm_assertion_set_is_atmost_once(rm_assertion, env, AXIS2_TRUE);
                                    }                                    
                                    else if(!axutil_strcmp(local_name, AXIS2_RM_IN_ORDER))
                                    {
                                        status = axis2_rm_assertion_set_is_inorder(rm_assertion, env, AXIS2_TRUE);                                                               }       
                                    else
                                    {
                                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                                            "[neethi] Unknown Assertion %s ", local_name);
                                        return AXIS2_FAILURE;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }   
    }    
    return status;
}



static axis2_status_t AXIS2_CALL
axis2_rm_assertion_builder_process_sandesha2_assertions(
    const axutil_env_t *env,
    axis2_rm_assertion_t *rm_assertion,
    axiom_node_t *node,
    axiom_element_t *element)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *local_name = NULL;

    if (element)
    {
        local_name = axiom_element_get_localname(element, env);
        if (local_name)
        {
            if(!axutil_strcmp(local_name, AXIS2_RM_BASE_RETRANSMISSION_INTERVAL))
            {
                axis2_char_t *rti = NULL;
                rti = axiom_element_get_text(element, env, node);
                return axis2_rm_assertion_set_retrans_interval(rm_assertion, env, rti);
            }
            else if(!axutil_strcmp(local_name, AXIS2_RM_INACTIVITY_TIMEOUT))
            {
                axis2_char_t *inactivity_timeout = NULL;
                inactivity_timeout = axiom_element_get_text(element, env, node);
                return axis2_rm_assertion_set_inactivity_timeout(rm_assertion, env, inactivity_timeout);
            }
            else if(!axutil_strcmp(local_name, AXIS2_RM_ACKNOWLEDGEMENT_INTERVAL))
            {
                axis2_char_t *ack_interval = NULL;
                ack_interval = axiom_element_get_text(element, env, node);
                return axis2_rm_assertion_set_ack_interval(rm_assertion, env, ack_interval);
            }
            else if(!axutil_strcmp(local_name, AXIS2_RM_STORAGE_MANAGER))
            {
                axis2_char_t *storage_mgr = NULL;
                storage_mgr = axiom_element_get_text(element, env, node);
                return axis2_rm_assertion_set_storage_mgr(rm_assertion, env, storage_mgr);
            }
            else if(!axutil_strcmp(local_name, AXIS2_RM_SANDESHA2_DB))
            {
                axis2_char_t *sandesha2_db = NULL;
                sandesha2_db = axiom_element_get_text(element, env, node);
                return axis2_rm_assertion_set_sandesha2_db(rm_assertion, env, sandesha2_db);
            }
            else if(!axutil_strcmp(local_name, AXIS2_RM_MESSAGE_TYPES_TO_DROP))
            {
                axis2_char_t *message_types_to_drop = NULL;
                message_types_to_drop = axiom_element_get_text(element, env, node);
                return axis2_rm_assertion_set_message_types_to_drop(rm_assertion, env, message_types_to_drop);
            }
            else if(!axutil_strcmp(local_name, AXIS2_RM_MAX_RETRANS_COUNT))
            {
                axis2_char_t *retrans_count = NULL;
                retrans_count = axiom_element_get_text(element, env, node);
                return axis2_rm_assertion_set_max_retrans_count(rm_assertion, env, retrans_count);
            }
            else if(!axutil_strcmp(local_name, AXIS2_RM_SENDER_SLEEP_TIME))
            {
                axis2_char_t *sender_sleep_time = NULL;
                sender_sleep_time = axiom_element_get_text(element, env, node);
                return axis2_rm_assertion_set_sender_sleep_time(rm_assertion, env, sender_sleep_time);
            }
            else if(!axutil_strcmp(local_name, AXIS2_RM_INVOKER_SLEEP_TIME))
            {
                axis2_char_t *invoker_sleep_time = NULL;
                invoker_sleep_time = axiom_element_get_text(element, env, node);
                return axis2_rm_assertion_set_invoker_sleep_time(rm_assertion, env, invoker_sleep_time);
            }
            else if(!axutil_strcmp(local_name, AXIS2_RM_POLLING_WAIT_TIME))
            {
                axis2_char_t *polling_wait_time = NULL;
                polling_wait_time = axiom_element_get_text(element, env, node);
                return axis2_rm_assertion_set_polling_wait_time(rm_assertion, env, polling_wait_time);
            }
            else if(!axutil_strcmp(local_name, AXIS2_RM_TERMINATE_DELAY))
            {
                axis2_char_t *terminate_delay = NULL;
                terminate_delay = axiom_element_get_text(element, env, node);
                return axis2_rm_assertion_set_terminate_delay(rm_assertion, env, terminate_delay);
            }
            else
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "[neethi] Unknown Assertion %s ", local_name);
                return AXIS2_FAILURE;
            }
        }
    }
    return status;
}


