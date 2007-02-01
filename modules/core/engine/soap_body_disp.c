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


#include <axis2_disp.h>
#include <axis2_handler_desc.h>
#include <axis2_string.h>
#include <axis2_svc.h>
#include <axis2_const.h>
#include <axis2_conf_ctx.h>
#include <axis2_utils.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_body.h>

const axis2_char_t *AXIS2_SOAP_MESSAGE_BODY_DISP_NAME  = "soap_message_body_based_dispatcher";

axis2_status_t AXIS2_CALL
axiom_soap_body_disp_invoke(
    axis2_handler_t *handler,
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx);

axis2_svc_t *AXIS2_CALL
axiom_soap_body_disp_find_svc(
    axis2_msg_ctx_t *msg_ctx,
    const axis2_env_t *env);


axis2_op_t *AXIS2_CALL
axiom_soap_body_disp_find_op(
    axis2_msg_ctx_t *msg_ctx,
    const axis2_env_t *env,
    axis2_svc_t *svc);


axis2_disp_t *AXIS2_CALL
axiom_soap_body_disp_create(
    const axis2_env_t *env)
{
    axis2_disp_t *disp = NULL;
    axis2_handler_t *handler = NULL;
    axis2_string_t *name = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    name = axis2_string_create_const(env, (axis2_char_t**)&AXIS2_SOAP_MESSAGE_BODY_DISP_NAME);

    disp = axis2_disp_create(env, name);
    if (!disp)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    handler = AXIS2_DISP_GET_BASE(disp, env);
    if (!handler)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
        return NULL;
    }

    handler->ops->invoke = axiom_soap_body_disp_invoke;

    axis2_string_free(name, env);

    return disp;
}

axis2_svc_t *AXIS2_CALL
axiom_soap_body_disp_find_svc(
    axis2_msg_ctx_t *msg_ctx,
    const axis2_env_t *env)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axis2_svc_t *svc = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
    if (soap_envelope)
    {
        axiom_soap_body_t *soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
        if (soap_body)
        {
            axiom_node_t *body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(soap_body, env);
            if (body_node)
            {
                axiom_node_t *body_first_child_node =
                    AXIOM_NODE_GET_FIRST_ELEMENT(body_node, env);

                if (body_first_child_node)
                {
                    if (AXIOM_NODE_GET_NODE_TYPE(body_first_child_node, env) == AXIOM_ELEMENT)
                    {
                        axiom_element_t *element = NULL;
                        element = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(body_first_child_node, env);
                        if (element)
                        {
                            axiom_namespace_t *ns =
                                AXIOM_ELEMENT_GET_NAMESPACE(element, env, body_first_child_node);
                            if (ns)
                            {
                                axis2_char_t *uri = AXIOM_NAMESPACE_GET_URI(ns, env);
                                if (uri)
                                {
                                    axis2_char_t **url_tokens = NULL;
                                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                            "Checking for service using SOAP message body's first child's namespace URI : %s", uri);

                                    url_tokens = axis2_parse_request_url_for_svc_and_op(env, uri);

                                    if (url_tokens)
                                    {
                                        if (url_tokens[0])
                                        {
                                            axis2_conf_ctx_t *conf_ctx = NULL;

                                            conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
                                            if (conf_ctx)
                                            {
                                                axis2_conf_t *conf = NULL;
                                                conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
                                                if (conf)
                                                {
                                                    svc = AXIS2_CONF_GET_SVC(conf, env, url_tokens[0]);
                                                    if (svc)
                                                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                                                "Service found using SOAP message body's first child's namespace URI");
                                                }
                                            }
                                            AXIS2_FREE(env->allocator, url_tokens[0]);
                                        }

                                        AXIS2_FREE(env->allocator, url_tokens);
                                        url_tokens = NULL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return svc;
}

axis2_op_t *AXIS2_CALL
axiom_soap_body_disp_find_op(
    axis2_msg_ctx_t *msg_ctx,
    const axis2_env_t *env,
    axis2_svc_t *svc)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axis2_op_t *op = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, svc, NULL);

    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
    if (soap_envelope)
    {
        axiom_soap_body_t *soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
        if (soap_body)
        {
            axiom_node_t *body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(soap_body, env);
            if (body_node)
            {
                axiom_node_t *body_first_child_node =
                    AXIOM_NODE_GET_FIRST_ELEMENT(body_node, env);

/*				while (AXIOM_NODE_GET_NODE_TYPE(body_first_child_node, env) != AXIOM_ELEMENT)
				body_first_child_node = AXIOM_NODE_GET_NEXT_SIBLING (body_first_child_node, env);*/

                if (body_first_child_node)
                {
                    if (AXIOM_NODE_GET_NODE_TYPE(body_first_child_node, env) == AXIOM_ELEMENT)
                    {
                        axiom_element_t *element = NULL;
                        element = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(body_first_child_node, env);
                        if (element)
                        {
                            axis2_char_t *element_name = AXIOM_ELEMENT_GET_LOCALNAME(element, env);
                            if (element_name)
                            {
                                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "Checking for operation using SOAP message body's first child's local name : %s",
                                        element_name);

                                op = AXIS2_SVC_GET_OP_WITH_NAME(svc, env, element_name);

                                if (op)
                                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                            "Operation found using SOAP message body's first child's local name");

                            }
                        }
                    }
                }
            }
        }
    }
    return op;
}

axis2_status_t AXIS2_CALL
axiom_soap_body_disp_invoke(
    axis2_handler_t * handler,
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    axis2_msg_ctx_set_find_svc(msg_ctx, env, axiom_soap_body_disp_find_svc);
    axis2_msg_ctx_set_find_op(msg_ctx, env, axiom_soap_body_disp_find_op);

    return axis2_disp_invoke(handler, env, msg_ctx);
}
