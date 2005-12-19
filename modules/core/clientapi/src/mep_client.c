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

#include <axis2_mep_client.h>
#include <axis2.h>
#include <axis2_hash.h>

typedef struct axis2_mep_client_impl
{
    /** context base struct */
    axis2_mep_client_t mep_client;
    /** service context */
    axis2_svc_ctx_t *svc_ctx;
    /** message exchange pattern */
    axis2_char_t *mep;
    /** SOAP version URI */
    axis2_char_t *soap_version_uri;
    /** SOAP action */
    axis2_char_t *soap_action;
    /** WSA action  */
    axis2_char_t *wsa_action;
} axis2_mep_client_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(mep_client) ((axis2_mep_client_impl_t *)mep_client)

axis2_char_t* AXIS2_CALL axis2_mep_client_get_soap_action(struct axis2_mep_client *mep_client, axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_mep_client_prepare_invocation(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_op_t *op, axis2_msg_ctx_t *msg_ctx);
axis2_msg_ctx_t* AXIS2_CALL axis2_mep_client_prepare_soap_envelope(struct axis2_mep_client *mep_client, 
    axis2_env_t **env, 
    axis2_om_node_t *to_send);
axis2_transport_out_desc_t* AXIS2_CALL axis2_mep_client_infer_transport(struct axis2_mep_client *mep_client, axis2_env_t **env, 
            axis2_endpoint_ref_t *epr);
/* TODO axis2_soap_envelope_t* AXIS2_CALL axis2_mep_client_create_default_soap_envelope(struct axis2_mep_client *mep_client, 
    axis2_env_t **env);*/
axis2_status_t AXIS2_CALL axis2_mep_client_engage_module(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_qname_t *qname);
axis2_status_t AXIS2_CALL axis2_mep_client_set_soap_version_uri(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_char_t *soap_version_uri);
axis2_status_t AXIS2_CALL axis2_mep_client_set_soap_action(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_char_t *soap_action);
axis2_status_t AXIS2_CALL axis2_mep_client_set_wsa_action(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_char_t *wsa_action);
axis2_char_t* AXIS2_CALL axis2_get_transport_from_url(axis2_char_t *url, axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_mep_client_free (struct axis2_mep_client *mep_client, 
                                   axis2_env_t **env);

axis2_mep_client_t* AXIS2_CALL axis2_mep_client_create(axis2_env_t **env, axis2_svc_ctx_t *svc_ctx, axis2_char_t *mep)
{
    axis2_mep_client_impl_t *mep_client_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    mep_client_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_mep_client_impl_t) );
    if (!mep_client_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    mep_client_impl->mep_client.ops = NULL;
    mep_client_impl->svc_ctx = NULL;
    mep_client_impl->mep = NULL;
    mep_client_impl->soap_version_uri = NULL; 
    mep_client_impl->soap_action = NULL;
    mep_client_impl->wsa_action = NULL;

    if (svc_ctx)
    {
        mep_client_impl->svc_ctx = svc_ctx;
    }
    
    /* TODO mep_client_impl->soap_version_uri = AXIS2_STRDUP((*env)->allocator, AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI);
    if (!(mep_client_impl->soap_version_uri))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_mep_client_free(&(mep_client_impl->mep_client), env);
        return NULL;        
    }    */
    
    if (mep)
    {
        mep_client_impl->mep = AXIS2_STRDUP(mep, env);
        if (!(mep_client_impl->mep))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            axis2_mep_client_free(&(mep_client_impl->mep_client), env);
            return NULL;
        }
    }
    
    /* initialize ops */    
    mep_client_impl->mep_client.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_mep_client_ops_t) );
    if (!mep_client_impl->mep_client.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_mep_client_free(&(mep_client_impl->mep_client), env);
        return NULL;        
    }

    mep_client_impl->mep_client.ops->get_soap_action = axis2_mep_client_get_soap_action;
    mep_client_impl->mep_client.ops->prepare_invocation = axis2_mep_client_prepare_invocation;
    mep_client_impl->mep_client.ops->prepare_soap_envelope = axis2_mep_client_prepare_soap_envelope;
    mep_client_impl->mep_client.ops->infer_transport = axis2_mep_client_infer_transport;
    /* TODO mep_client_impl->mep_client.ops->create_default_soap_envelope = axis2_mep_client_create_default_soap_envelope;*/
    mep_client_impl->mep_client.ops->engage_module = axis2_mep_client_engage_module;
    mep_client_impl->mep_client.ops->set_soap_version_uri = axis2_mep_client_set_soap_version_uri;
    mep_client_impl->mep_client.ops->set_soap_action = axis2_mep_client_set_soap_action;
    mep_client_impl->mep_client.ops->set_wsa_action = axis2_mep_client_set_wsa_action;
    mep_client_impl->mep_client.ops->free = axis2_mep_client_free;

    return &(mep_client_impl->mep_client);
}

axis2_char_t* AXIS2_CALL axis2_mep_client_get_soap_action(struct axis2_mep_client *mep_client, axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(mep_client, env, NULL);
    return AXIS2_INTF_TO_IMPL(mep_client)->soap_action;
}

axis2_status_t AXIS2_CALL axis2_mep_client_prepare_invocation(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_op_t *op, axis2_msg_ctx_t *msg_ctx)
{
    axis2_mep_client_impl_t *mep_client_impl = NULL;
    axis2_svc_t *svc = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(mep_client, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    
    mep_client_impl = AXIS2_INTF_TO_IMPL(mep_client);

    /* make sure operation's MEP is the same as given MEP */
    if (mep_client_impl->mep)
    {
        if (AXIS2_STRCMP(mep_client_impl->mep, AXIS2_OP_GET_MSG_EXCHANGE_PATTERN(op, env)) != 0)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MEP_MISMATCH_IN_MEP_CLIENT, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    else
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MEP_CANNOT_BE_NULL_IN_MEP_CLIENT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    /* if operation not alrady added, add it */
    svc = AXIS2_SVC_CTX_GET_SVC(mep_client_impl->svc_ctx, env);
    if (svc)
    {
        axis2_op_t *temp_op = NULL;
        axis2_qname_t *op_qname = AXIS2_OP_GET_QNAME(op, env);
        temp_op = AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, op_qname);
        if (!temp_op)
        {
            AXIS2_SVC_ADD_OP(svc, env, op);
        }
    }
    
    if (mep_client_impl->wsa_action) 
    {
        AXIS2_MSG_CTX_SET_WSA_ACTION(msg_ctx, env, mep_client_impl->wsa_action);
    }
    
    if (mep_client_impl->soap_action)
    {
        AXIS2_MSG_CTX_SET_SOAP_ACTION(msg_ctx, env, mep_client_impl->soap_action);
    }
    
    return AXIS2_SUCCESS;
}

axis2_msg_ctx_t* AXIS2_CALL axis2_mep_client_prepare_soap_envelope(struct axis2_mep_client *mep_client, 
    axis2_env_t **env, 
    axis2_om_node_t *to_send)  
{
    axis2_mep_client_impl_t *mep_client_impl = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    /* TODO axis2_soap_envelope_t *envelope = NULL;*/
    
    AXIS2_FUNC_PARAM_CHECK(mep_client, env, NULL);
    
    mep_client_impl = AXIS2_INTF_TO_IMPL(mep_client);
    
    msg_ctx = axis2_msg_ctx_create(env, AXIS2_SVC_CTX_GET_CONF_CTX(mep_client_impl->svc_ctx, env), NULL, NULL);
    if (!msg_ctx)
    {
        return NULL;
    }
    
    /* TODO envelope = axis2_soap_envelope_create(env);
    if (!envelope)
    {
        return NULL;
    }*/
    
    if (to_send) 
    {
        /* TODO axis2_soap_body_t *soap_body = NULL;
        soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(envelope, env);
        if (soap_body)
        {
            AXIS2_SOAP_BODY_ADD_CHILD(soap_body, env, to_send);
        }       
        */
    }
    
    /* TODO AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, envelope);*/
   
    return msg_ctx;
}

axis2_transport_out_desc_t* AXIS2_CALL axis2_mep_client_infer_transport(struct axis2_mep_client *mep_client, axis2_env_t **env, 
            axis2_endpoint_ref_t *epr)  
{
    axis2_mep_client_impl_t *mep_client_impl = NULL;
    axis2_char_t *transport = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(mep_client, env, NULL);
    
    mep_client_impl = AXIS2_INTF_TO_IMPL(mep_client);
    
    if (epr)
    {
        axis2_char_t *to_url = AXIS2_ENDPOINT_REF_GET_ADDRESS(epr, env);
        
        transport = axis2_get_transport_from_url(to_url, env);        
    }

    if (transport) 
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        axis2_conf_t *conf = NULL;
        axis2_qname_t *qname = NULL;
        axis2_transport_out_desc_t *transport_out_desc = NULL;
        
        qname = axis2_qname_create(env, transport, NULL, NULL);
        if (!qname)
            return NULL;
        
        conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(mep_client_impl->svc_ctx, env);
        if (conf_ctx)
        {
            conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
            if (conf)
            {
                transport_out_desc = AXIS2_CONF_GET_TRANSPORT_OUT(conf, env, qname);
            }
        }
        
        AXIS2_QNAME_FREE(qname, env);
        return transport_out_desc;        

    } 
    
    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CANNOT_INFER_TRANSPORT, AXIS2_FAILURE);
    return NULL;
}

/* TODO axis2_soap_envelope_t* AXIS2_CALL axis2_mep_client_create_default_soap_envelope(struct axis2_mep_client *mep_client, 
    axis2_env_t **env)  
{
    axis2_soap_factory fac = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(mep_client, env, NULL);
    
    if (AXIS2_STRCMP(SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, soap_version_uri) == 0) 
    {
        /*fac = OMAbstractFactory.getSOAP12Factory();*/
    /*} 
    else if (AXIS2_STRCMP(SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, soap_version_uri) == 0) 
    {
        /*fac = OMAbstractFactory.getSOAP11Factory();*/
    /*} else 
    {
        AXIS2_ERROR_SET( (*env)->error, AXIS2_ERROR_INVALID_SOAP_VERSION, AXIS2_FAILURE);
        return NULL;
    }
    
    return AXIS2_SOAP_FACTORY_GET_DEFAULT_ENVELOPE(fac, env);
}*/

axis2_status_t AXIS2_CALL axis2_mep_client_engage_module(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_qname_t *qname)  
{
    axis2_mep_client_impl_t *mep_client_impl = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(mep_client, env, AXIS2_FAILURE);
    
    mep_client_impl = AXIS2_INTF_TO_IMPL(mep_client);
    
    if (mep_client_impl->svc_ctx)
    {
        conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(mep_client_impl->svc_ctx, env);
        if (conf_ctx)
        {
            conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
            if (conf)
            {
                /*if it is already engeged do not engege it again*/
                if (!(AXIS2_CONF_IS_ENGAGED(conf, env, qname))) 
                {
                    /* TODO return AXIS2_CONF_ENGAGE_MODULE(conf, env, qname);*/
                }
            }
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_mep_client_set_soap_version_uri(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_char_t *soap_version_uri) 
{
    axis2_mep_client_impl_t *mep_client_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(mep_client, env, AXIS2_FAILURE);
    
    mep_client_impl = AXIS2_INTF_TO_IMPL(mep_client);
    
    if (mep_client_impl->soap_version_uri)
    {
        AXIS2_FREE((*env)->allocator, mep_client_impl->soap_version_uri);
        mep_client_impl->soap_version_uri = NULL;
    }
    
    if (soap_version_uri)
    {
        mep_client_impl->soap_version_uri = AXIS2_STRDUP(soap_version_uri, env);
        if (!(mep_client_impl->soap_version_uri))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_mep_client_set_soap_action(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_char_t *soap_action) 
{
    axis2_mep_client_impl_t *mep_client_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(mep_client, env, AXIS2_FAILURE);
    
    mep_client_impl = AXIS2_INTF_TO_IMPL(mep_client);
    
    if (mep_client_impl->soap_action)
    {
        AXIS2_FREE((*env)->allocator, mep_client_impl->soap_action);
        mep_client_impl->soap_action = NULL;
    }
    
    if (soap_action)
    {
        mep_client_impl->soap_action = AXIS2_STRDUP(soap_action, env);
        if (!(mep_client_impl->soap_action))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_mep_client_set_wsa_action(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_char_t *wsa_action) 
{
    axis2_mep_client_impl_t *mep_client_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(mep_client, env, AXIS2_FAILURE);
    
    mep_client_impl = AXIS2_INTF_TO_IMPL(mep_client);
    
    if (mep_client_impl->wsa_action)
    {
        AXIS2_FREE((*env)->allocator, mep_client_impl->wsa_action);
        mep_client_impl->wsa_action = NULL;
    }
    
    if (wsa_action)
    {
        mep_client_impl->wsa_action = AXIS2_STRDUP(wsa_action, env);
        if (!(mep_client_impl->wsa_action))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL axis2_get_transport_from_url(axis2_char_t *url, axis2_env_t **env)
{
    axis2_char_t *transport = NULL;
    axis2_char_t *start = NULL;
    axis2_char_t *end = NULL;
    AXIS2_PARAM_CHECK((*env)->error, url, AXIS2_FAILURE);
    start = url;
    end = url;
    while ( end && (*end) != ':' )
        end++;
    
    if ((*end) == ':')
    {    
        axis2_char_t *c = NULL;
        transport = AXIS2_MALLOC( (*env)->allocator, (end - start + 1 ) * sizeof(char) );
        if (!transport)
        { 
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;        
        }
        
        for (c = start; c < end; c++)
            transport[c - start] = *c;
        transport[c-start] = '\0';
    }
    
    return transport;    
}

axis2_status_t AXIS2_CALL axis2_mep_client_free (struct axis2_mep_client *mep_client, 
                                   axis2_env_t **env)
{
    axis2_mep_client_impl_t *mep_client_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(mep_client, env, AXIS2_FAILURE);
    
    mep_client_impl = AXIS2_INTF_TO_IMPL(mep_client);
    
    if (mep_client_impl->mep_client.ops)
    {
        AXIS2_FREE((*env)->allocator, mep_client_impl->mep_client.ops);
        mep_client_impl->mep_client.ops = NULL;
    }
    
    if (mep_client_impl->mep)
    {
        AXIS2_FREE((*env)->allocator, mep_client_impl->mep);
        mep_client_impl->mep = NULL;
    }
    
    if (mep_client_impl->soap_version_uri)
    {
        AXIS2_FREE((*env)->allocator, mep_client_impl->soap_version_uri);
        mep_client_impl->soap_version_uri = NULL;
    }
    
    if (mep_client_impl->soap_action)
    {
        AXIS2_FREE((*env)->allocator, mep_client_impl->soap_action);
        mep_client_impl->soap_action = NULL;
    }
    
    if (mep_client_impl->wsa_action)
    {
        AXIS2_FREE((*env)->allocator, mep_client_impl->wsa_action);
        mep_client_impl->wsa_action = NULL;
    }

    AXIS2_FREE((*env)->allocator, mep_client_impl);
    mep_client_impl = NULL;
    
    return AXIS2_SUCCESS;
}
