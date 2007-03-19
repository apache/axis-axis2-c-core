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


#include <axis2_mep_client.h>
#include <axis2_options.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_engine.h>
#include <axiom_soap_const.h>
#include <axiom_soap_body.h>
#include <axis2_http_transport_utils.h>
#include <axis2_property.h>
#include <axis2_types.h>
#include <platforms/axis2_platform_auto_sense.h>

struct axis2_mep_client
{
    /** service context */
    axis2_svc_ctx_t *svc_ctx;
    /** message exchange pattern */
    axis2_char_t *mep;
    /** SOAP version URI */
    axis2_char_t *soap_version_uri;
    /** SOAP action */
    axis2_string_t *soap_action;
    /** WSA action  */
    axis2_char_t *wsa_action;
};

static axis2_char_t *AXIS2_CALL
axis2_get_transport_from_url(
    const axis2_char_t *url,
    const axis2_env_t *env);

AXIS2_EXTERN axis2_mep_client_t *AXIS2_CALL
axis2_mep_client_create(
    const axis2_env_t *env,
    axis2_svc_ctx_t *svc_ctx,
    const axis2_char_t *mep)
{
    axis2_mep_client_t *mep_client = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    mep_client = AXIS2_MALLOC(env->allocator, sizeof(axis2_mep_client_t));
    if (!mep_client)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    mep_client->svc_ctx = NULL;
    mep_client->mep = NULL;
    mep_client->soap_version_uri = NULL;
    mep_client->soap_action = NULL;
    mep_client->wsa_action = NULL;

    if (svc_ctx)
    {
        mep_client->svc_ctx = svc_ctx;
    }

    mep_client->soap_version_uri = axis2_strdup(AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, env);
    if (!(mep_client->soap_version_uri))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_mep_client_free(mep_client, env);
        return NULL;
    }

    if (mep)
    {
        mep_client->mep = axis2_strdup(mep, env);
        if (!(mep_client->mep))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            axis2_mep_client_free(mep_client, env);
            return NULL;
        }
    }

    return mep_client;
}

AXIS2_EXTERN axis2_string_t *AXIS2_CALL
axis2_mep_client_get_soap_action(
    const axis2_mep_client_t *mep_client,
    const axis2_env_t *env)
{
    return mep_client->soap_action;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_mep_client_prepare_invocation(
    axis2_mep_client_t *mep_client,
    const axis2_env_t *env,
    axis2_op_t *op,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_svc_t *svc = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    /* make sure operation's MEP is the same as given MEP */
    if (mep_client->mep)
    {
        if (axis2_strcmp(mep_client->mep, axis2_op_get_msg_exchange_pattern(op, env)) != 0)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_MEP_MISMATCH_IN_MEP_CLIENT, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    else
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_MEP_CANNOT_BE_NULL_IN_MEP_CLIENT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    /* If operation has a parent service get it */
    svc = axis2_op_get_parent(op, env);
    if (svc)
    {
         axis2_svc_ctx_set_svc(mep_client->svc_ctx, env, svc);
    }
    else
    {
        svc =  axis2_svc_ctx_get_svc(mep_client->svc_ctx, env);
        if (svc)
        {
            axis2_op_t *temp_op = NULL;
            const axis2_qname_t *op_qname = axis2_op_get_qname(op, env);
            temp_op = AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, op_qname);
            if (!temp_op)
            {
                AXIS2_SVC_ADD_OP(svc, env, op);
            }
        }
    }

    if (mep_client->wsa_action)
    {
         axis2_msg_ctx_set_wsa_action(msg_ctx, env, mep_client->wsa_action);
    }

    if (mep_client->soap_action)
    {
          axis2_msg_ctx_set_soap_action(msg_ctx, env, mep_client->soap_action);
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_msg_ctx_t *AXIS2_CALL
axis2_mep_client_prepare_soap_envelope(
    axis2_mep_client_t *mep_client,
    const axis2_env_t *env,
    axiom_node_t *to_send)
{
    axis2_msg_ctx_t *msg_ctx = NULL;
    axiom_soap_envelope_t *envelope = NULL;
    int soap_version = AXIOM_SOAP12;

    AXIS2_ENV_CHECK(env, NULL);

    if (mep_client->svc_ctx)
    {
        msg_ctx = axis2_msg_ctx_create(env,
                 axis2_svc_ctx_get_conf_ctx(mep_client->svc_ctx, env),
                 NULL, NULL);
    }

    if (!msg_ctx)
    {
        return NULL;
    }

    if (mep_client->soap_version_uri)
    {
        if (axis2_strcmp(mep_client->soap_version_uri,
                AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI) == 0)
            soap_version = AXIOM_SOAP11;
        else
            soap_version = AXIOM_SOAP12;
    }

    envelope = axiom_soap_envelope_create_default_soap_envelope(env, soap_version);
    if (!envelope)
    {
        return NULL;
    }

    if (to_send)
    {
        axiom_soap_body_t *soap_body = NULL;
        soap_body = axiom_soap_envelope_get_body(envelope, env);
        if (soap_body)
        {
            axiom_node_t *node = NULL;
            node = axiom_soap_body_get_base_node(soap_body, env);
            if (node)
            {
                AXIOM_NODE_ADD_CHILD(node, env, to_send);
            }
        }
    }

    axis2_msg_ctx_set_soap_envelope(msg_ctx, env, envelope);

    return msg_ctx;
}

AXIS2_EXTERN axis2_transport_out_desc_t *AXIS2_CALL
axis2_mep_client_infer_transport(
    axis2_mep_client_t *mep_client,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *epr)
{
    axis2_char_t *transport = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (epr)
    {
        const axis2_char_t *to_url = axis2_endpoint_ref_get_address(epr, env);

        transport = axis2_get_transport_from_url(to_url, env);
    }

    if (transport)
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        axis2_conf_t *conf = NULL;
        axis2_transport_out_desc_t *transport_out_desc = NULL;
		AXIS2_TRANSPORT_ENUMS transport_enum = 0;
        
		if (!axis2_strcmp(transport, "http"))
		{
			transport_enum = AXIS2_TRANSPORT_ENUM_HTTP;
		}
		else if (!axis2_strcmp (transport, "https"))
		{
			transport_enum = AXIS2_TRANSPORT_ENUM_HTTPS;
		}

        conf_ctx =  axis2_svc_ctx_get_conf_ctx(mep_client->svc_ctx, env);
        if (conf_ctx)
        {
            conf =  axis2_conf_ctx_get_conf(conf_ctx, env);
            if (conf)
            {
                transport_out_desc =  axis2_conf_get_transport_out(conf, env, transport_enum);
            }
        }

        AXIS2_FREE(env->allocator, transport);
        transport = NULL;
        return transport_out_desc;

    }

    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_CANNOT_INFER_TRANSPORT, AXIS2_FAILURE);
    return NULL;
}

AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
axis2_mep_client_create_default_soap_envelope(
    axis2_mep_client_t *mep_client,
    const axis2_env_t *env)
{
    axiom_soap_envelope_t *envelope = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (axis2_strcmp(AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, mep_client->soap_version_uri) == 0)
    {
        envelope = axiom_soap_envelope_create_with_soap_version_prefix(env, AXIOM_SOAP12, NULL);
    }

    if (axis2_strcmp(AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, mep_client->soap_version_uri) == 0)
    {
        envelope = axiom_soap_envelope_create_with_soap_version_prefix(env, AXIOM_SOAP11, NULL);
    }
    return envelope;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_mep_client_engage_module(
    axis2_mep_client_t *mep_client,
    const axis2_env_t *env,
    const axis2_qname_t *qname)
{
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (mep_client->svc_ctx)
    {
        conf_ctx =  axis2_svc_ctx_get_conf_ctx(mep_client->svc_ctx, env);
        if (conf_ctx)
        {
            conf =  axis2_conf_ctx_get_conf(conf_ctx, env);
            if (conf)
            {
                /*if it is already engaged do not engage it again*/
                if (!( axis2_conf_is_engaged(conf, env, qname)))
                {
                    return  axis2_conf_engage_module(conf, env, qname);
                }
            }
        }
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_mep_client_set_soap_version_uri(
    axis2_mep_client_t *mep_client,
    const axis2_env_t *env,
    const axis2_char_t *soap_version_uri)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (mep_client->soap_version_uri)
    {
        AXIS2_FREE(env->allocator, mep_client->soap_version_uri);
        mep_client->soap_version_uri = NULL;
    }

    if (soap_version_uri)
    {
        mep_client->soap_version_uri = axis2_strdup(soap_version_uri, env);
        if (!(mep_client->soap_version_uri))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_mep_client_set_soap_action(
    axis2_mep_client_t *mep_client,
    const axis2_env_t *env,
    axis2_string_t *soap_action)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (mep_client->soap_action)
    {
        axis2_string_free(mep_client->soap_action, env);
        mep_client->soap_action = NULL;
    }

    if (soap_action)
    {
        mep_client->soap_action = axis2_string_clone(soap_action, env);
        if (!(mep_client->soap_action))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_mep_client_set_wsa_action(
    axis2_mep_client_t *mep_client,
    const axis2_env_t *env,
    const axis2_char_t *wsa_action)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (mep_client->wsa_action)
    {
        AXIS2_FREE(env->allocator, mep_client->wsa_action);
        mep_client->wsa_action = NULL;
    }

    if (wsa_action)
    {
        mep_client->wsa_action = axis2_strdup(wsa_action, env);
        if (!(mep_client->wsa_action))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }

    return AXIS2_SUCCESS;
}

static axis2_char_t *AXIS2_CALL
axis2_get_transport_from_url(
    const axis2_char_t *url,
    const axis2_env_t *env)
{
    axis2_char_t *transport = NULL;
    const axis2_char_t *start = NULL;
    const axis2_char_t *end = NULL;
    AXIS2_PARAM_CHECK(env->error, url, AXIS2_FAILURE);
    start = url;
    end = url;
    while (end && (*end) != ':')
        end++;

    if ((*end) == ':')
    {
        const axis2_char_t *c = NULL;
        transport = AXIS2_MALLOC(env->allocator, (end - start + 1) * sizeof(char));
        if (!transport)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }

        for (c = start; c < end; c++)
            transport[c - start] = *c;
        transport[c-start] = '\0';
    }

    return transport;
}

AXIS2_EXTERN axis2_svc_ctx_t *AXIS2_CALL
axis2_mep_client_get_svc_ctx(
    const axis2_mep_client_t *mep_client,
    const axis2_env_t *env)
{
    return mep_client->svc_ctx;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_mep_client_free(
    axis2_mep_client_t *mep_client,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (mep_client->mep)
    {
        AXIS2_FREE(env->allocator, mep_client->mep);
    }

    if (mep_client->soap_version_uri)
    {
        AXIS2_FREE(env->allocator, mep_client->soap_version_uri);
    }

    if (mep_client->soap_action)
    {
        axis2_string_free(mep_client->soap_action, env);
    }

    if (mep_client->wsa_action)
    {
        AXIS2_FREE(env->allocator, mep_client->wsa_action);
    }

    AXIS2_FREE(env->allocator, mep_client);

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_msg_ctx_t *AXIS2_CALL
axis2_mep_client_two_way_send(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_engine_t *engine = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_msg_ctx_t *response = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_op_t *op = NULL;
    axiom_soap_envelope_t *response_envelope = NULL;
    axis2_property_t *property = NULL;
    long index = -1;
    axis2_bool_t wait_indefinitely = AXIS2_FALSE;

    AXIS2_ENV_CHECK(env, NULL);

    conf_ctx =  axis2_msg_ctx_get_conf_ctx(msg_ctx, env);
    engine = axis2_engine_create(env, conf_ctx);
    if (!engine)
        return NULL;
    property = axis2_msg_ctx_get_property(msg_ctx, env, AXIS2_TIMEOUT_IN_SECONDS, 
        AXIS2_FALSE);
    if(property)
    {
        axis2_char_t *value = axis2_property_get_value(property, env);
        if(value)
            index = AXIS2_ATOI(value);
        if(index == -1)
        {
            wait_indefinitely = AXIS2_TRUE;
            index = 1;
        }
    }

    status = axis2_engine_send(engine, env, msg_ctx);
    
     axis2_engine_free(engine, env);
    engine = NULL;
    
    if (status != AXIS2_SUCCESS)
        return NULL;

    op =  axis2_msg_ctx_get_op(msg_ctx, env);
    if (op)
    {
        /* handle one way case */
        const axis2_char_t *mep = axis2_op_get_msg_exchange_pattern(op, env);
        if (axis2_strcmp(mep, AXIS2_MEP_URI_OUT_ONLY) == 0 ||
            axis2_strcmp(mep, AXIS2_MEP_URI_ROBUST_OUT_ONLY) == 0)
        {
            return NULL;
        }
    }

    /* create the response */
    response = axis2_msg_ctx_create(env, conf_ctx,
             axis2_msg_ctx_get_transport_in_desc(msg_ctx, env),
             axis2_msg_ctx_get_transport_out_desc(msg_ctx, env));
    if (!response)
        return NULL;

    property =  axis2_msg_ctx_get_property(msg_ctx, env,AXIS2_TRANSPORT_IN, AXIS2_FALSE);
    if (property)
    {
         axis2_msg_ctx_set_property(response, env, AXIS2_TRANSPORT_IN, property,
                AXIS2_FALSE);
        property = NULL;
    }

    if (op)
    {
        axis2_op_register_op_ctx(op, env, response,  axis2_msg_ctx_get_op_ctx(msg_ctx, env));
    }
     axis2_msg_ctx_set_server_side(response, env, AXIS2_FALSE);
     axis2_msg_ctx_set_conf_ctx(response, env,  axis2_msg_ctx_get_conf_ctx(msg_ctx, env));
     axis2_msg_ctx_set_svc_grp_ctx(response, env,  axis2_msg_ctx_get_svc_grp_ctx(msg_ctx, env));

    /* If request is REST we assume the response is REST, so set the variable*/
     axis2_msg_ctx_set_doing_rest(response, env,  axis2_msg_ctx_get_doing_rest(msg_ctx, env));
	/* set response envelope */
    if (engine)
    {
         axis2_engine_free(engine, env);
        engine = NULL;
    }
 	response_envelope =  axis2_msg_ctx_get_response_soap_envelope (msg_ctx, env);
    if(response_envelope)
    {
        axis2_msg_ctx_set_soap_envelope(response, env, response_envelope);
        engine = axis2_engine_create(env, conf_ctx);
        if (engine)
        {
            status =  axis2_engine_receive(engine, env, response);
            if (status != AXIS2_SUCCESS)
                return NULL;
        }
    }
    else
    {
        while(!response_envelope && index > 0)
        {
            /*wait till the response arrives*/
            AXIS2_SLEEP(1);
            if(!wait_indefinitely)
                index--;
            response_envelope = axis2_msg_ctx_get_response_soap_envelope(
                msg_ctx, env);
        }
        /* if it is a two way message, then the status should be in error,
           else it is a one way message */
        if (response_envelope)
        {
            axis2_msg_ctx_set_soap_envelope(response, env, response_envelope);
            /* There could be a scenaria where the message has already passed
             * through the incoming phases. eg. Reliable Messaging 1.0 two
             * way single channel
             */
            property = axis2_msg_ctx_get_property(msg_ctx, env, 
                AXIS2_HANDLER_ALREADY_VISITED, AXIS2_FALSE);
            if(property)
            {
                axis2_char_t *value = axis2_property_get_value(property, env);
                if(0 == axis2_strcmp(AXIS2_VALUE_TRUE, value))
                {
                    return response;
                }
            }
            engine = axis2_engine_create(env, conf_ctx);
            if (engine)
            {
                status =  axis2_engine_receive(engine, env, response);
                if (status != AXIS2_SUCCESS)
                    return NULL;
            }
        }
        else
        {
            if (AXIS2_ERROR_GET_STATUS_CODE(env->error) != AXIS2_SUCCESS)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_BLOCKING_INVOCATION_EXPECTS_RESPONSE, AXIS2_FAILURE);
                if (engine)
                {
                     axis2_engine_free(engine, env);
                    engine = NULL;
                }
                 axis2_msg_ctx_free(response, env);
                return NULL;
            }
        }
    }

    /* property is NULL, and we set null for AXIS2_TRANSPORT_IN in msg_ctx to
    avoid double free of this property */
     axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_TRANSPORT_IN, property,
            AXIS2_FALSE);

    if (engine)
    {
         axis2_engine_free(engine, env);
        engine = NULL;
    }
    return response;
}

AXIS2_EXTERN axis2_msg_ctx_t *AXIS2_CALL
axis2_mep_client_receive(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_engine_t *engine = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_msg_ctx_t *response = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_op_t *op = NULL;
    axiom_soap_envelope_t *response_envelope = NULL;
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    /* create the response */
    response = axis2_msg_ctx_create(env, conf_ctx,
             axis2_msg_ctx_get_transport_in_desc(msg_ctx, env),
             axis2_msg_ctx_get_transport_out_desc(msg_ctx, env));
    if (!response)
        return NULL;

    property =  axis2_msg_ctx_get_property(msg_ctx, env, AXIS2_TRANSPORT_IN, AXIS2_FALSE);
    if (property)
    {
         axis2_msg_ctx_set_property(response, env, AXIS2_TRANSPORT_IN, property,
                AXIS2_FALSE);
        property = NULL;
    }

    op =  axis2_msg_ctx_get_op(msg_ctx, env);
    if (op)
    {
        axis2_op_register_op_ctx(op, env, response,  axis2_msg_ctx_get_op_ctx(msg_ctx, env));
    }
     axis2_msg_ctx_set_server_side(response, env, AXIS2_FALSE);
     axis2_msg_ctx_set_conf_ctx(response, env,  axis2_msg_ctx_get_conf_ctx(msg_ctx, env));
     axis2_msg_ctx_set_svc_grp_ctx(response, env,  axis2_msg_ctx_get_svc_grp_ctx(msg_ctx, env));

    /* If request is REST we assume the response is REST, so set the variable*/
     axis2_msg_ctx_set_doing_rest(response, env,  axis2_msg_ctx_get_doing_rest(msg_ctx, env));

 	response_envelope =  axis2_msg_ctx_get_response_soap_envelope (msg_ctx, env);
    if (response_envelope)
    {
        axis2_msg_ctx_set_soap_envelope(response, env, response_envelope);
        if (engine)
        {
             axis2_engine_free(engine, env);
            engine = NULL;
        }

        engine = axis2_engine_create(env, conf_ctx);
        if (engine)
        {
            status =  axis2_engine_receive(engine, env, response);
            if (status != AXIS2_SUCCESS)
            {
                return NULL;
            }
        }

    }
    else
    {
        /* if it is a two way message, then the status should be in error,
           else it is a one way message */
        if (AXIS2_ERROR_GET_STATUS_CODE(env->error) != AXIS2_SUCCESS)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_BLOCKING_INVOCATION_EXPECTS_RESPONSE, AXIS2_FAILURE);
            return NULL;
        }
    }

    /* property is NULL, and we set null for AXIS2_TRANSPORT_IN in msg_ctx to
    avoid double free of this property */
     axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_TRANSPORT_IN, property,
            AXIS2_FALSE);

    if (engine)
    {
         axis2_engine_free(engine, env);
        engine = NULL;
    }
    return response;
}
