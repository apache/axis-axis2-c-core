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

#include <axis2_raw_xml_in_out_msg_recv.h>
#include <string.h>
#include <axiom_element.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_header.h>
#include <axiom_soap_body.h>
#include <axiom_soap_fault.h>


/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_raw_xml_in_out_msg_recv_invoke_business_logic_sync(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_msg_ctx_t *new_msg_ctx);

/************************** End of function prototypes ************************/

AXIS2_EXTERN axis2_msg_recv_t *AXIS2_CALL
axis2_raw_xml_in_out_msg_recv_create(
    const axis2_env_t *env)
{
    axis2_msg_recv_t *msg_recv = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, NULL);
    msg_recv = axis2_msg_recv_create(env);
    if (NULL == msg_recv)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    status = AXIS2_MSG_RECV_SET_SCOPE(msg_recv, env, AXIS2_APPLICATION_SCOPE);
    if (AXIS2_TRUE != status)
    {
        AXIS2_MSG_RECV_FREE(msg_recv, env);
        return NULL;
    }
    msg_recv->ops->invoke_in_out_business_logic_sync =
        axis2_raw_xml_in_out_msg_recv_invoke_business_logic_sync;
    msg_recv->ops->receive = msg_recv->ops->receive_sync;
    return msg_recv;
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_raw_xml_in_out_msg_recv_invoke_business_logic_sync(
    axis2_msg_recv_t *msg_recv,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_msg_ctx_t *new_msg_ctx)
{
    axis2_svc_skeleton_t *svc_obj = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_op_t *op_desc = NULL;
    const axis2_char_t *style = NULL;
    axiom_node_t *om_node = NULL;
    axiom_element_t *om_element = NULL;
    axis2_char_t *local_name = NULL;
    axiom_node_t *result_node = NULL;
    axiom_node_t *body_content_node = NULL;
    axiom_element_t *body_content_element = NULL;
    axiom_soap_envelope_t *default_envelope = NULL;
    axiom_soap_body_t *out_body = NULL;
    axiom_soap_header_t *out_header = NULL;
    axiom_soap_fault_t *soap_fault = NULL;
    axiom_node_t *out_node = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_bool_t skel_invoked = AXIS2_FALSE;
    const axis2_char_t *soap_ns = AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    int soap_version = AXIOM_SOAP12;
    axiom_namespace_t *env_ns = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, new_msg_ctx, AXIS2_FAILURE);

    /* get the implementation class for the Web Service */
    svc_obj = AXIS2_MSG_RECV_GET_IMPL_OBJ(msg_recv, env, msg_ctx);

    if (!svc_obj)
    {
        const axis2_char_t *svc_name = NULL;
        axis2_svc_t *svc = AXIS2_MSG_CTX_GET_SVC(msg_ctx, env);

        if (svc)
        {
            svc_name = AXIS2_SVC_GET_NAME(svc, env);
        }
        else
        {
            svc_name = "unknown";
        }

        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Impl object for service '%s' not set in message receiver. %d :: %s",
                svc_name,
                env->error->error_number,
                AXIS2_ERROR_GET_MESSAGE(env->error));
        status = AXIS2_FAILURE;
    }
    else
    {
        op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
        op_desc = AXIS2_OP_CTX_GET_OP(op_ctx, env);

        style = AXIS2_OP_GET_STYLE(op_desc, env);
        if (0 == AXIS2_STRCMP(AXIS2_STYLE_DOC, style))
        {
            axiom_soap_envelope_t *envelope = NULL;
            axiom_soap_body_t *body = NULL;

            envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
            body = AXIOM_SOAP_ENVELOPE_GET_BODY(envelope, env);
            om_node = AXIOM_SOAP_BODY_GET_BASE_NODE(body, env);
            om_element = AXIOM_NODE_GET_DATA_ELEMENT(om_node, env);
            om_node = AXIOM_NODE_GET_FIRST_CHILD(om_node, env);
        }
        else if (0 == AXIS2_STRCMP(AXIS2_STYLE_RPC, style))
        {
            axiom_soap_envelope_t *envelope = NULL;
            axiom_soap_body_t *body = NULL;
            axiom_node_t *op_node = NULL;
            axiom_element_t *op_element = NULL;

            envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
            body = AXIOM_SOAP_ENVELOPE_GET_BODY(envelope, env);
            op_node = AXIOM_SOAP_BODY_GET_BASE_NODE(body, env);
            op_element = AXIOM_NODE_GET_DATA_ELEMENT(op_node, env);
            if (NULL != op_element)
            {
                local_name = AXIOM_ELEMENT_GET_LOCALNAME(op_element, env);
                if (NULL != local_name)
                {
                    axis2_array_list_t *function_arr = NULL;
                    int i = 0;
                    int size = 0;
                    axis2_bool_t matches = AXIS2_FALSE;

                    function_arr = svc_obj->func_array;
                    if (function_arr)
                    {
                        size = AXIS2_ARRAY_LIST_SIZE(function_arr, env);
                    }

                    for (i = 0; i < size; i++)
                    {
                        axis2_char_t *function_name = NULL;

                        function_name = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(
                                    function_arr, env, i);
                        if (0 == AXIS2_STRCMP(function_name, local_name))
                        {
                            matches = AXIS2_TRUE;

                        }
                    }

                    if (AXIS2_TRUE == matches)
                    {
                        om_node = AXIOM_NODE_GET_FIRST_CHILD(op_node, env);
                        om_element = AXIOM_NODE_GET_DATA_ELEMENT(om_node, env);
                    }
                    else
                    {
                        AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_OM_ELEMENT_MISMATCH, AXIS2_FAILURE);
                        status = AXIS2_FAILURE;
                    }
                }
                else
                {
                    AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_OM_ELEMENT_INVALID_STATE, AXIS2_FAILURE);
                    status = AXIS2_FAILURE;
                }
            }
            else
            {
                AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_RPC_NEED_MATCHING_CHILD, AXIS2_FAILURE);
                status = AXIS2_FAILURE;
            }
        }
        else
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_UNKNOWN_STYLE, AXIS2_FAILURE);
            status = AXIS2_FAILURE;
        }

        if (status == AXIS2_SUCCESS)
        {
            skel_invoked = AXIS2_TRUE;
            result_node = AXIS2_SVC_SKELETON_INVOKE(svc_obj, env, om_node, new_msg_ctx);
            AXIS2_SVC_SKELETON_FREE(svc_obj, env);
        }

        if (result_node)
        {
            if (0 == AXIS2_STRCMP(style, AXIS2_STYLE_RPC))
            {
                axiom_namespace_t *ns = NULL;
                axis2_char_t *res_name = NULL;

                res_name = AXIS2_STRACAT(local_name, "Response", env);
                ns = axiom_namespace_create(env, "http://soapenc/", "res");
                if (!ns)
                {
                    status = AXIS2_FAILURE;
                }
                else
                {
                    body_content_element = axiom_element_create(env, NULL, res_name,
                            ns, &body_content_node);
                    AXIOM_NODE_ADD_CHILD(body_content_node, env, result_node);
                }

            }
            else
            {
                body_content_node = result_node;
            }
        }
        else
        {
            status = AXIS2_ERROR_GET_STATUS_CODE(env->error);
        }
    }

    if (msg_ctx && AXIS2_MSG_CTX_GET_IS_SOAP_11(msg_ctx, env))
    {
        soap_ns = AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI; /* default is 1.2 */
        soap_version = AXIOM_SOAP11;
    }

    if (AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(new_msg_ctx, env))
    {
        /* service implementation has set the envelope,
           useful when setting a SOAP fault.
           No need to further process */
        return AXIS2_SUCCESS;
    }

    /* create the soap envelope here*/
    env_ns = axiom_namespace_create(env, soap_ns, "soapenv");
    if (!env_ns)
    {
        return AXIS2_FAILURE;
    }

    default_envelope = axiom_soap_envelope_create(env, env_ns);

    if (!default_envelope)
    {
        return AXIS2_FAILURE;
    }

    out_header = axiom_soap_header_create_with_parent(env, default_envelope);
    if (!out_header)
    {
        return AXIS2_FAILURE;
    }

    out_body = axiom_soap_body_create_with_parent(env, default_envelope);
    if (!out_body)
    {
        return AXIS2_FAILURE;
    }

    out_node = AXIOM_SOAP_BODY_GET_BASE_NODE(out_body, env);
    if (!out_node)
    {
        return AXIS2_FAILURE;
    }

    if (status != AXIS2_SUCCESS)
    {
        /* something went wrong. set a SOAP Fault*/
        const axis2_char_t *fault_value_str = "env:Sender";
        const axis2_char_t *fault_reason_str = NULL;
        const axis2_char_t *err_msg = NULL;

        if (!skel_invoked)
            fault_value_str = "env:Receiver";

        err_msg = AXIS2_ERROR_GET_MESSAGE(env->error);
        if (err_msg)
        {
            fault_reason_str = err_msg;
        }
        else
        {
            fault_reason_str = "Something went wrong";
        }

        soap_fault = axiom_soap_fault_create_default_fault(env, out_body,
                fault_value_str, fault_reason_str, soap_version);
    }

    if (body_content_node)
    {
        AXIOM_NODE_ADD_CHILD(out_node , env, body_content_node);
        status = AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(new_msg_ctx, env, default_envelope);
    }
    else if (soap_fault)
    {
        AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(new_msg_ctx, env, default_envelope);
        status = AXIS2_FAILURE; /* if there is a failure we have to return a failure code */
    }
    else
    {
        /* we should free the memory as the envelope is not used, one way case */
        AXIOM_SOAP_ENVELOPE_FREE(default_envelope, env);
        default_envelope = NULL;
    }


    return status;
}

/**
 * Following block distinguish the exposed part of the dll.
 */


AXIS2_EXPORT int axis2_get_instance(
    struct axis2_msg_recv **inst,
    const axis2_env_t *env)
{
    *inst = axis2_raw_xml_in_out_msg_recv_create(env);
    if (!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int axis2_remove_instance(
    struct axis2_msg_recv *inst,
    const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
    if (inst)
    {
        status = AXIS2_MSG_RECV_FREE(inst, env);
    }
    return status;
}
