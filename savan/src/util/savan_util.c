/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
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

#include <axis2_msg_info_headers.h>
#include <axiom_soap.h>

#include <savan_util.h>
#include <savan_error.h>

/******************************************************************************/

savan_message_types_t AXIS2_CALL
savan_util_get_message_type(
    axis2_msg_ctx_t *msg_ctx,
    const axis2_env_t *env)
{
    const axis2_char_t *action = NULL;
    axis2_msg_info_headers_t *info_header = NULL;

    info_header =  axis2_msg_ctx_get_msg_info_headers(msg_ctx, env);
    if (!info_header)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[savan] Cannot extract message "
            "info headers"); 
        return SAVAN_MSG_TYPE_UNKNOWN;
    }
    
    action = axis2_msg_info_headers_get_action(info_header, env);
    if( ! action)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[savan] Cannot extract soap "
            "action"); 
        AXIS2_ERROR_SET(env->error, SAVAN_ERROR_SOAP_ACTION_NULL, AXIS2_FAILURE);
        return SAVAN_MSG_TYPE_UNKNOWN;
    }
    
    if (AXIS2_STRCMP(action, SAVAN_ACTIONS_SUB) == 0)
        return SAVAN_MSG_TYPE_SUB;
    else if (AXIS2_STRCMP(action, SAVAN_ACTIONS_SUB_RESPONSE) == 0)
        return SAVAN_MSG_TYPE_SUB_RESPONSE;
    else if (AXIS2_STRCMP(action, SAVAN_ACTIONS_UNSUB) == 0)
        return SAVAN_MSG_TYPE_UNSUB;
    else if (AXIS2_STRCMP(action, SAVAN_ACTIONS_UNSUB_RESPONSE) == 0)
        return SAVAN_MSG_TYPE_UNSUB_RESPONSE;
    else if (AXIS2_STRCMP(action, SAVAN_ACTIONS_GET_STATUS) == 0)
        return SAVAN_MSG_TYPE_GET_STATUS;
    else if (AXIS2_STRCMP(action, SAVAN_ACTIONS_GET_STATUS_RESPONSE) == 0)
        return SAVAN_MSG_TYPE_GET_STATUS_RESPONSE;
    else if (AXIS2_STRCMP(action, SAVAN_ACTIONS_RENEW) == 0)
        return SAVAN_MSG_TYPE_RENEW;
    else if (AXIS2_STRCMP(action, SAVAN_ACTIONS_RENEW_RESPONSE) == 0)
        return SAVAN_MSG_TYPE_RENEW_RESPONSE;
    
    return SAVAN_MSG_TYPE_UNKNOWN;
}

/******************************************************************************/

axis2_char_t * AXIS2_CALL
savan_util_get_subscription_id_from_msg(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_char_t *sub_id = NULL;
    axiom_soap_envelope_t *envelope = NULL;
    axiom_soap_header_t *header = NULL;
    axis2_qname_t *qname = NULL;
    axiom_node_t *header_node = NULL;
    axiom_node_t *id_node = NULL;
    axiom_element_t *header_elem = NULL;
    axiom_element_t *id_elem = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[savan][util] "
        "get sub id from msg...");
    
    /* Get soap envelop and extract the subscription id */

    envelope =  axis2_msg_ctx_get_soap_envelope(msg_ctx, env);
    if (!envelope)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[savan] Failed to extract the "
            "soap envelop");
        return NULL;
    }
    
    header = AXIOM_SOAP_ENVELOPE_GET_HEADER(envelope, env);
    if (!header)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[savan] Failed to extract the "
            "soap header"); 
        return NULL;
    }
    
    /* Get header element from header node */
    header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(header, env);
    header_elem = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(header_node, env);
    
    /* Get Identifier element from header */
    qname = axis2_qname_create(env, ELEM_NAME_ID, EVENTING_NAMESPACE, NULL);
    id_elem = axiom_element_get_first_child_with_qname(header_elem, env, qname,
        header_node, &id_node);
    AXIS2_QNAME_FREE(qname, env);
    
    /* Now read the id */
    sub_id = axiom_element_get_text(id_elem, env, id_node);
    
    return sub_id;    
}

/******************************************************************************/

savan_subscriber_t * AXIS2_CALL
savan_util_get_subscriber_from_msg(
        const axis2_env_t *env,
        axis2_msg_ctx_t *msg_ctx,
        axis2_char_t *sub_id)
{
    axis2_hash_t *store = NULL;
    savan_subscriber_t *subscriber = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[savan][util] "
        "get subscriber from msg...");

    /* Extract subscription id from msg if not already given */
    if (!sub_id)
        sub_id = savan_util_get_subscription_id_from_msg(env, msg_ctx);

    store = savan_util_get_subscriber_store(env, msg_ctx);
    if (!store)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[savan] Subscriber store is "
            "null"); 
        return NULL;
    }

    subscriber = axis2_hash_get(store, sub_id, AXIS2_HASH_KEY_STRING);
    
    return subscriber;
}

/******************************************************************************/

axis2_hash_t * AXIS2_CALL
savan_util_get_subscriber_store(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_svc_t *svc = NULL;
    axis2_param_t *param = NULL;
    axis2_hash_t *store = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[savan][util] "
        "get subscriber store...");

    svc =  axis2_msg_ctx_get_svc(msg_ctx, env);
    if (!svc)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[savan] Failed to extract the "
            "service"); 
        return NULL;
    }

    param = AXIS2_SVC_GET_PARAM(svc, env, SUBSCRIBER_STORE);
    if (!param)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[savan] Failed to extract the "
            "subscriber store param"); 
        return NULL;
    }

    store = (axis2_hash_t*)AXIS2_PARAM_GET_VALUE(param, env);

    return store;
}

/******************************************************************************/

axis2_char_t * AXIS2_CALL
savan_util_get_expiry_time(
    const axis2_env_t *env)
{
    /* TODO: decide how to set expiry time */
    
    return NULL;
}

/******************************************************************************/

axis2_char_t * AXIS2_CALL
savan_util_get_renewed_expiry_time(
    const axis2_env_t *env,
    axis2_char_t *expiry)
{
    /* TODO: decide how to renew expiry time */
    
    return NULL;
}
