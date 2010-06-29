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

#include <axis2_handler_desc.h>
#include <axutil_qname.h>
#include <axis2_svc.h>
#include <axiom_soap_header.h>
#include <axiom_soap_body.h>
#include <axis2_addr.h>
#include <axiom_soap_header_block.h>
#include <axis2_endpoint_ref.h>
#include <axutil_property.h>
#include <stdio.h>

axis2_status_t AXIS2_CALL
axis2_addr_out_handler_invoke(
    struct axis2_handler * handler,
    const axutil_env_t * env,
    struct axis2_msg_ctx * msg_ctx);

axis2_status_t
axis2_addr_out_handler_add_to_soap_header(
    const axutil_env_t * env,
    axis2_endpoint_ref_t * endpoint_ref,
    const axis2_char_t * type,
    axiom_soap_header_t * soap_header,
    const axis2_char_t * addr_ns);

axis2_status_t
axis2_addr_out_handler_add_to_header(
    const axutil_env_t * env,
    axis2_endpoint_ref_t * epr,
    axiom_node_t ** parent_node,
    const axis2_char_t * addr_ns);

axis2_status_t
axis2_addr_out_handler_process_any_content_type(
    const axutil_env_t * env,
    axis2_any_content_type_t * reference_values,
    axiom_node_t * parent_ele_node,
    const axis2_char_t * addr_ns);

axiom_node_t *
axis2_addr_out_handler_process_string_info(
    const axutil_env_t * env,
    const axis2_char_t * value,
    const axis2_char_t * type,
    axiom_soap_header_t ** soap_header,
    const axis2_char_t * addr_ns,
	axis2_bool_t set_must_understand);

AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
axis2_addr_out_handler_create(
    const axutil_env_t * env,
    axutil_string_t * name)
{
    axis2_handler_t *handler = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    handler = axis2_handler_create(env);
    if(!handler)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory. Cannot create addressing out handler");
        return NULL;
    }

    axis2_handler_set_invoke(handler, env, axis2_addr_out_handler_invoke);

    return handler;
}

axis2_status_t AXIS2_CALL
axis2_addr_out_handler_invoke(
    struct axis2_handler * handler,
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx)
{
    axis2_char_t *addr_ver_from_msg_ctx = NULL;
    const axis2_char_t *addr_ns = NULL;
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    axis2_ctx_t *ctx = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axiom_node_t *soap_header_node = NULL;
    axiom_element_t *soap_header_ele = NULL;
    axis2_endpoint_ref_t *epr_to = NULL;
    axis2_endpoint_ref_t *epr_reply_to = NULL;
    axis2_endpoint_ref_t *epr_from = NULL;
    axis2_endpoint_ref_t *epr_fault_to = NULL;
    axutil_property_t *property = NULL;
    const axis2_char_t *wsa_action = NULL;
	axis2_bool_t set_must_understand = AXIS2_FALSE;
	axutil_property_t *must_understand_prop;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    AXIS2_LOG_INFO(env->log, "Starting addressing out handler");

    soap_envelope = axis2_msg_ctx_get_soap_envelope(msg_ctx, env);
    if(!soap_envelope)
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "No SOAP envelope. Stop processing addressing");
        return AXIS2_SUCCESS; /* Can happen in case of ONE-WAY services/clients */
    }

    msg_info_headers = axis2_msg_ctx_get_msg_info_headers(msg_ctx, env);

    if(msg_info_headers)
    {
        wsa_action = axis2_msg_info_headers_get_action(msg_info_headers, env);
    }
    else
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "No addressing in use");
        return AXIS2_SUCCESS; /* No addressing in use */
    }

    if(!wsa_action || !*wsa_action)
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "No action present. Stop processing addressing");
        return AXIS2_SUCCESS; /* If no action present, assume no addressing in use */
    }


    ctx = axis2_msg_ctx_get_base(msg_ctx, env);
    property = axis2_ctx_get_property(ctx, env, AXIS2_WSA_VERSION);

    if(property)
    {
        addr_ver_from_msg_ctx = axutil_property_get_value(property, env);
        property = NULL;
    }

	must_understand_prop = axis2_msg_ctx_get_property(msg_ctx, env, AXIS2_ADDR_ADD_MUST_UNDERSTAND_TO_ADDR_HEADERS);
	if(must_understand_prop)
	{
		axis2_char_t *value = axutil_property_get_value(must_understand_prop, env);
		if(axutil_strcmp(value, AXIS2_VALUE_TRUE) == 0)
			set_must_understand = AXIS2_TRUE;
	}

    /* Setting version 1.0 as the default addressing namespace */
    addr_ns = AXIS2_WSA_NAMESPACE;

    if(addr_ver_from_msg_ctx)
    {
        if(!axutil_strcmp(AXIS2_WSA_NAMESPACE_SUBMISSION, addr_ver_from_msg_ctx))
        {
            addr_ns = AXIS2_WSA_NAMESPACE_SUBMISSION;
        }
    }
    else if(axis2_msg_ctx_get_op_ctx(msg_ctx, env))
    {
        axis2_op_ctx_t *op_ctx = NULL;
        axis2_msg_ctx_t *in_msg_ctx = NULL;
        op_ctx = axis2_msg_ctx_get_op_ctx(msg_ctx, env);

        if(op_ctx)
        {
            in_msg_ctx = axis2_op_ctx_get_msg_ctx(op_ctx, env, AXIS2_WSDL_MESSAGE_LABEL_IN);
        }

        if(in_msg_ctx)
        {
            axis2_ctx_t *in_ctx = NULL;
            in_ctx = axis2_msg_ctx_get_base(in_msg_ctx, env);

            property = axis2_ctx_get_property(in_ctx, env, AXIS2_WSA_VERSION);
            if(property)
            {
                addr_ns = axutil_property_get_value(property, env);
                property = NULL;
            }

            if(!addr_ns || !*addr_ns)
            {
                addr_ns = AXIS2_WSA_NAMESPACE;
            }
        }
    }

    soap_header = axiom_soap_envelope_get_header(soap_envelope, env);
    if(!soap_header)
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "No SOAP header. Stop processing addressing");
        return AXIS2_SUCCESS; /* No SOAP header, so no point proceeding */
    }
    else
    {
        /* By this time, we definitely have some addressing information to be sent. This is because,
         * we have tested at the start of this whether msg_info_headers are null or not.
         * So rather than declaring addressing namespace in each and every addressing header, lets
         * define that in the Header itself.
         */
        const axis2_char_t *action = NULL;
        const axis2_char_t *address = NULL;
        const axis2_char_t *svc_group_context_id = NULL;
        const axis2_char_t *message_id = NULL;
        axis2_relates_to_t *relates_to = NULL;
        axiom_node_t *relates_to_header_node = NULL;
        axiom_element_t *relates_to_header_ele = NULL;
        axiom_namespace_t *addressing_namespace = NULL;

        soap_header_node = axiom_soap_header_get_base_node(soap_header, env);
        soap_header_ele = (axiom_element_t *)axiom_node_get_data_element(soap_header_node, env);

        addressing_namespace = axiom_namespace_create(env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);

        axiom_element_declare_namespace(soap_header_ele, env, soap_header_node,
            addressing_namespace);

        epr_to = axis2_msg_info_headers_get_to(msg_info_headers, env);

        if(epr_to)
        {
            axiom_soap_body_t *body = axiom_soap_envelope_get_body(soap_envelope, env);

            if(body)
            {
                /* In case of a SOAP fault, we got to send the response to
                 the adress specified by FaultTo */
                if(axiom_soap_body_has_fault(body, env))
                {
                    axis2_endpoint_ref_t *epr_fault_to = axis2_msg_info_headers_get_fault_to(
                        msg_info_headers, env);
                    if(epr_fault_to)
                    {
                        const axis2_char_t *fault_to_address = axis2_endpoint_ref_get_address(
                            epr_fault_to, env);
                        if(fault_to_address)
                        {
                            if(axutil_strcmp(AXIS2_WSA_NONE_URL, fault_to_address)
                                && axutil_strcmp(AXIS2_WSA_NONE_URL_SUBMISSION, fault_to_address))
                            {
                                axis2_endpoint_ref_set_address(epr_to, env, fault_to_address);
                            }
                        }
                    }
                }
            }

            address = axis2_endpoint_ref_get_address(epr_to, env);
            if(address && *address)
            {
                axiom_node_t *to_header_block_node = NULL;
                axiom_soap_header_block_t *to_header_block = NULL;
                axutil_array_list_t *ref_param_list = NULL;
                int size = 0;

                to_header_block = axiom_soap_header_add_header_block(soap_header, env,
                    AXIS2_WSA_TO, addressing_namespace);
				if(set_must_understand == AXIS2_TRUE)
				{
					axiom_soap_header_block_set_must_understand_with_bool(to_header_block, env, AXIS2_TRUE);
				}
				to_header_block_node = axiom_soap_header_block_get_base_node(to_header_block, env);
                if(to_header_block_node)
                {
                    axiom_element_t *to_header_block_element = NULL;
                    to_header_block_element = (axiom_element_t *)axiom_node_get_data_element(
                        to_header_block_node, env);
                    if(to_header_block_element)
                    {
                        axiom_element_set_text(to_header_block_element, env, address,
                            to_header_block_node);
                    }
                }

                ref_param_list = axis2_endpoint_ref_get_ref_param_list(epr_to, env);

                size = axutil_array_list_size(ref_param_list, env);
                if(ref_param_list && size > 0)
                {
                    axiom_soap_header_block_t *reference_header_block = NULL;
                    axiom_node_t *reference_node = NULL;
                    int i = 0;

                    for(i = 0; i < size; i++)
                    {
                        axiom_node_t *temp_node = NULL;

                        temp_node = (axiom_node_t *)axutil_array_list_get(ref_param_list, env, i);
                        if(temp_node)
                        {
                            axiom_element_t *temp_ele = NULL;

                            temp_ele = axiom_node_get_data_element(temp_node, env);
                            if(temp_ele)
                            {
                                reference_header_block = axiom_soap_header_add_header_block(
                                    soap_header, env, axiom_element_get_localname(temp_ele, env),
                                    axiom_element_get_namespace(temp_ele, env, temp_node));
								if(set_must_understand)
								{
									axiom_soap_header_block_set_must_understand_with_bool(reference_header_block, env, AXIS2_TRUE);
								}

                                reference_node = axiom_soap_header_block_get_base_node(
                                    reference_header_block, env);
                                if(reference_node)
                                {
                                    axiom_element_t *reference_ele = NULL;
                                    reference_ele = (axiom_element_t *)axiom_node_get_data_element(
                                        reference_node, env);

                                    if(reference_ele)
                                    {
                                        axiom_namespace_t *addr_ns_obj = NULL;
                                        axiom_attribute_t *reference_attr = NULL;

                                        addr_ns_obj = axiom_namespace_create(env, addr_ns,
                                            AXIS2_WSA_DEFAULT_PREFIX);
                                        reference_attr = axiom_attribute_create(env,
                                        /*"isReferenceParameter"*/
                                        AXIS2_WSA_IS_REFERENCE_PARAMETER_ATTRIBUTE, "true",
                                            addr_ns_obj);
                                        axiom_element_add_attribute(reference_ele, env,
                                            reference_attr, reference_node);
                                        axiom_element_set_text(reference_ele, env,
                                            axiom_element_get_text(temp_ele, env, temp_node),
                                            reference_node);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }/* if(epr_to) */

		axiom_namespace_free(addressing_namespace, env);

        action = axis2_msg_info_headers_get_action(msg_info_headers, env);
        if(action && *action)
        {
            axis2_addr_out_handler_process_string_info(env, action, AXIS2_WSA_ACTION, &soap_header,
				addr_ns, set_must_understand);
        }

        epr_reply_to = axis2_msg_info_headers_get_reply_to(msg_info_headers, env);
        if(!epr_reply_to)
        {
            const axis2_char_t *anonymous_uri = NULL;
            axis2_bool_t anonymous = axis2_msg_info_headers_get_reply_to_anonymous(
                msg_info_headers, env);
            axis2_bool_t none = axis2_msg_info_headers_get_reply_to_none(msg_info_headers, env);
            if(!axutil_strcmp(addr_ns, AXIS2_WSA_NAMESPACE_SUBMISSION))
            {
                if(none)
                {
                    anonymous_uri = AXIS2_WSA_NONE_URL_SUBMISSION;
                }
                else if(anonymous)
                {
                    anonymous_uri = AXIS2_WSA_ANONYMOUS_URL_SUBMISSION;
                }
            }
            else
            {
                if(none)
                {
                    anonymous_uri = AXIS2_WSA_NONE_URL;
                }
                else if(anonymous)
                {
                    anonymous_uri = AXIS2_WSA_ANONYMOUS_URL;
                }
            }

            if(anonymous_uri)
            {
                epr_reply_to = axis2_endpoint_ref_create(env, anonymous_uri);
            }

            if(epr_reply_to)
            {
                axis2_msg_info_headers_set_reply_to(msg_info_headers, env, epr_reply_to);
            }
        }

        /* add the service group id as a reference parameter */
        svc_group_context_id = axutil_string_get_buffer(axis2_msg_ctx_get_svc_grp_ctx_id(msg_ctx,
            env), env);

        axis2_addr_out_handler_add_to_soap_header(env, epr_reply_to, AXIS2_WSA_REPLY_TO,
            soap_header, addr_ns);

        epr_from = axis2_msg_info_headers_get_from(msg_info_headers, env);

        if(epr_from)
        {
            axis2_addr_out_handler_add_to_soap_header(env, epr_from, AXIS2_WSA_FROM, soap_header,
                addr_ns);

        }

        epr_fault_to = axis2_msg_info_headers_get_fault_to(msg_info_headers, env);
        if(!epr_fault_to)
        {
            const axis2_char_t *anonymous_uri = NULL;
            axis2_bool_t anonymous = axis2_msg_info_headers_get_fault_to_anonymous(
                msg_info_headers, env);
            axis2_bool_t none = axis2_msg_info_headers_get_fault_to_none(msg_info_headers, env);
            if(!axutil_strcmp(addr_ns, AXIS2_WSA_NAMESPACE_SUBMISSION))
            {
                if(none)
                {
                    anonymous_uri = AXIS2_WSA_NONE_URL_SUBMISSION;
                }
                else if(anonymous)
                {
                    anonymous_uri = AXIS2_WSA_ANONYMOUS_URL_SUBMISSION;
                }
            }
            else
            {
                if(none)
                    anonymous_uri = AXIS2_WSA_NONE_URL;
                else if(anonymous)
                    anonymous_uri = AXIS2_WSA_ANONYMOUS_URL;
            }

            if(anonymous_uri)
            {
                epr_fault_to = axis2_endpoint_ref_create(env, anonymous_uri);
            }
        }

        if(epr_fault_to)
        {
            /* optional */
            axis2_addr_out_handler_add_to_soap_header(env, epr_fault_to, AXIS2_WSA_FAULT_TO,
                soap_header, addr_ns);
        }

        message_id = axis2_msg_info_headers_get_message_id(msg_info_headers, env);

        if(message_id)
        {
            axis2_addr_out_handler_process_string_info(env, message_id, AXIS2_WSA_MESSAGE_ID,
				&soap_header, addr_ns, set_must_understand);
        }

        relates_to = axis2_msg_info_headers_get_relates_to(msg_info_headers, env);

        if(relates_to)
        {
            const axis2_char_t *value = NULL;
            value = axis2_relates_to_get_value(relates_to, env);
            relates_to_header_node = axis2_addr_out_handler_process_string_info(env, value,
				AXIS2_WSA_RELATES_TO, &soap_header, addr_ns, set_must_understand);
        }

        if(relates_to_header_node)
        {
            const axis2_char_t *relationship_type = NULL;
            relationship_type = axis2_relates_to_get_relationship_type(relates_to, env);
            if(relationship_type && *relationship_type)
            {
                axiom_attribute_t *om_attr = NULL;
                axiom_namespace_t *addr_ns_obj = NULL;
                axiom_namespace_t *dec_ns = NULL;
                relates_to_header_ele = (axiom_element_t *)axiom_node_get_data_element(
                    relates_to_header_node, env);
                if(relates_to_header_ele)
                {
                    dec_ns = axiom_element_find_declared_namespace(relates_to_header_ele, env,
                        addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
                    if(dec_ns)
                    {
                        addr_ns_obj = dec_ns;
                    }
                    else
                    {
                        addr_ns_obj
                            = axiom_namespace_create(env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
                    }

                    if(!axutil_strcmp(addr_ns, AXIS2_WSA_NAMESPACE_SUBMISSION))
                    {
                        om_attr = axiom_attribute_create(env,
                            AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE,
                            AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE_SUBMISSION,
                            addr_ns_obj);
                    }
                    else
                    {
                        om_attr = axiom_attribute_create(env,
                            AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE,
                            AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE, addr_ns_obj);
                    }

                    axiom_element_add_attribute(relates_to_header_ele, env, om_attr,
                        relates_to_header_node);

                    dec_ns = axiom_element_find_declared_namespace(relates_to_header_ele, env,
                        addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
                    if(!dec_ns)
                    {
                        dec_ns = axiom_element_find_namespace(relates_to_header_ele, env,
                            relates_to_header_node, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
                        if(dec_ns)
                        {
                            axiom_namespace_free(addr_ns_obj, env);
                            addr_ns_obj = NULL;
                            axiom_attribute_set_namespace(om_attr, env, dec_ns);
                        }
                    }
                }

            }
        }
    }
    return AXIS2_SUCCESS;
}

axiom_node_t *
axis2_addr_out_handler_process_string_info(
    const axutil_env_t * env,
    const axis2_char_t * value,
    const axis2_char_t * type,
    axiom_soap_header_t ** soap_header_p,
    const axis2_char_t * addr_ns,
	axis2_bool_t set_must_understand)
{
    axiom_soap_header_t *soap_header = NULL;
    axiom_soap_header_block_t *header_block = NULL;
    axiom_node_t *header_block_node = NULL;
    axiom_element_t *header_block_ele = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, value, NULL);
    AXIS2_PARAM_CHECK(env->error, type, NULL);
    AXIS2_PARAM_CHECK(env->error, soap_header_p, NULL);
    AXIS2_PARAM_CHECK(env->error, addr_ns, NULL);

    soap_header = *(soap_header_p);

    if(value && *value)
    {
        axiom_namespace_t *addr_ns_obj = NULL;
        addr_ns_obj = axiom_namespace_create(env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
        header_block = axiom_soap_header_add_header_block(soap_header, env, type, addr_ns_obj);
        header_block_node = axiom_soap_header_block_get_base_node(header_block, env);
        header_block_ele = (axiom_element_t *)axiom_node_get_data_element(header_block_node, env);
        if(header_block_ele)
        {
            axiom_namespace_t *dec_ns = NULL;
            axiom_element_set_text(header_block_ele, env, value, header_block_node);
            dec_ns = axiom_element_find_declared_namespace(header_block_ele, env, addr_ns,
                AXIS2_WSA_DEFAULT_PREFIX);
            if(!dec_ns)
            {
                axiom_namespace_free(addr_ns_obj, env);
                addr_ns_obj = NULL;
            }
        }
		if(set_must_understand)
		{
			axiom_soap_header_block_set_must_understand_with_bool(header_block, env, AXIS2_TRUE);
		}
    }
    return header_block_node;
}

axis2_status_t
axis2_addr_out_handler_add_to_soap_header(
    const axutil_env_t * env,
    axis2_endpoint_ref_t * endpoint_ref,
    const axis2_char_t * type,
    axiom_soap_header_t * soap_header,
    const axis2_char_t * addr_ns)
{
    axiom_soap_header_block_t *header_block = NULL;
    const axis2_char_t *address = NULL;
    axutil_array_list_t *ref_param_list = NULL;
    axutil_array_list_t *meta_data_list = NULL;
    axutil_array_list_t *extension_list = NULL;
    axiom_node_t *header_block_node = NULL;
    axiom_node_t *header_node = NULL;
    axiom_namespace_t *addr_ns_obj = NULL;
    int size = 0;

    AXIS2_PARAM_CHECK(env->error, endpoint_ref, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, soap_header, AXIS2_FAILURE);

    header_node = axiom_soap_header_get_base_node(soap_header, env);

    addr_ns_obj = axiom_namespace_create(env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
    header_block = axiom_soap_header_add_header_block(soap_header, env, type, addr_ns_obj);
    if(addr_ns_obj)
    {
        axiom_namespace_free(addr_ns_obj, env);
        addr_ns_obj = NULL;
    }

    address = axis2_endpoint_ref_get_address(endpoint_ref, env);
    if(address && *address)
    {
        axiom_node_t *hb_node = NULL;
        axiom_element_t *hb_ele = NULL;
        axiom_node_t *address_node = NULL;
        axiom_element_t *address_ele = NULL;
        hb_node = axiom_soap_header_block_get_base_node(header_block, env);
        hb_ele = (axiom_element_t *)axiom_node_get_data_element(hb_node, env);

        addr_ns_obj = axiom_namespace_create(env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);

        address_ele = axiom_element_create(env, hb_node, EPR_ADDRESS, addr_ns_obj, &address_node);
		axiom_namespace_free(addr_ns_obj, env);
        if(address_ele)
        {
            axiom_namespace_t *dec_ns = NULL;
            axiom_element_set_text(address_ele, env, address, address_node);
            dec_ns = axiom_element_find_declared_namespace(address_ele, env, addr_ns,
                AXIS2_WSA_DEFAULT_PREFIX);
        }
    }

    header_block_node = axiom_soap_header_block_get_base_node(header_block, env);
    axis2_addr_out_handler_add_to_header(env, endpoint_ref, &header_block_node, addr_ns);

    ref_param_list = axis2_endpoint_ref_get_ref_param_list(endpoint_ref, env);
    if(ref_param_list && axutil_array_list_size(ref_param_list, env) > 0)
    {
        axiom_node_t *reference_node = NULL;
        axiom_element_t *reference_ele = NULL;
        axutil_array_list_t *ref_attribute_list = NULL;
        int i = 0;

        addr_ns_obj = axiom_namespace_create(env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);

        reference_ele = axiom_element_create(env, header_block_node, EPR_REFERENCE_PARAMETERS,
            addr_ns_obj, &reference_node);
		axiom_namespace_free(addr_ns_obj, env);

        ref_attribute_list = axis2_endpoint_ref_get_ref_attribute_list(endpoint_ref, env);
        if(ref_attribute_list)
        {
            int j = 0;

            size = axutil_array_list_size(ref_attribute_list, env);
            for(j = 0; j < size; j++)
            {
                axiom_attribute_t *attr = (axiom_attribute_t *)axutil_array_list_get(
                    ref_attribute_list, env, j);
                if(attr)
                {
                    axiom_element_add_attribute(reference_ele, env, attr, reference_node);
                }
            }
        }

        size = axutil_array_list_size(ref_param_list, env);
        for(i = 0; i < size; i++)
        {
            axiom_node_t *ref_node = (axiom_node_t *)axutil_array_list_get(ref_param_list, env, i);
            if(ref_node)
            {
                axiom_node_add_child(reference_node, env, ref_node);
            }
        }

    }

    meta_data_list = axis2_endpoint_ref_get_metadata_list(endpoint_ref, env);
    if(meta_data_list && axutil_array_list_size(meta_data_list, env) > 0)
    {
        axiom_node_t *reference_node = NULL;
        axiom_element_t *reference_ele = NULL;
        axutil_array_list_t *meta_attribute_list = NULL;
        int i = 0;

        if(!reference_node) /* may be we alredy created this in ref params block */
        {
            addr_ns_obj = axiom_namespace_create(env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);

            reference_ele = axiom_element_create(env, header_block_node, AXIS2_WSA_METADATA,
                addr_ns_obj, &reference_node);
			axiom_namespace_free(addr_ns_obj, env);
        }

        meta_attribute_list = axis2_endpoint_ref_get_metadata_attribute_list(endpoint_ref, env);
        if(meta_attribute_list)
        {
            int j = 0;
            size = axutil_array_list_size(meta_attribute_list, env);
            for(j = 0; j < size; j++)
            {
                axiom_attribute_t *attr = (axiom_attribute_t *)axutil_array_list_get(
                    meta_attribute_list, env, j);
                if(attr)
                {
                    axiom_element_add_attribute(reference_ele, env, attr, reference_node);
                }
            }
        }

        size = axutil_array_list_size(meta_data_list, env);
        for(i = 0; i < size; i++)
        {
            axiom_node_t *ref_node = (axiom_node_t *)axutil_array_list_get(meta_data_list, env, i);
            if(ref_node)
            {
                axiom_node_add_child(reference_node, env, ref_node);
            }
        }
    }

    extension_list = axis2_endpoint_ref_get_extension_list(endpoint_ref, env);
    if(extension_list && axutil_array_list_size(extension_list, env) > 0)
    {
        int i = 0;

        size = axutil_array_list_size(extension_list, env);
        for(i = 0; i < size; i++)
        {
            axiom_node_t *ref_node = (axiom_node_t *)axutil_array_list_get(extension_list, env, i);
            if(ref_node)
            {
                axiom_node_add_child(header_block_node, env, ref_node);
            }
        }
    }

    return AXIS2_SUCCESS;
}

axis2_status_t
axis2_addr_out_handler_add_to_header(
    const axutil_env_t * env,
    axis2_endpoint_ref_t * epr,
    axiom_node_t ** parent_node_p,
    const axis2_char_t * addr_ns)
{
    axiom_node_t *parent_node = NULL;
    const axutil_qname_t *interface_qname = NULL;
    axiom_node_t *interface_node = NULL;
    axiom_element_t *interface_ele = NULL;
    const axis2_char_t *element_localname = NULL;
    axis2_svc_name_t *service_name = NULL;
    axiom_namespace_t *addr_ns_obj = NULL;

    AXIS2_PARAM_CHECK(env->error, epr, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, parent_node_p, AXIS2_FAILURE);

    parent_node = *(parent_node_p);

    interface_qname = axis2_endpoint_ref_get_interface_qname(epr, env);

    if(interface_qname)
    {
        axis2_char_t *text = NULL;
        axis2_char_t *qname_prefix = NULL;
        axis2_char_t *qname_localpart = NULL;

        addr_ns_obj = axiom_namespace_create(env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);

        if(!axutil_strcmp(addr_ns, AXIS2_WSA_NAMESPACE_SUBMISSION))
        {
            element_localname = EPR_PORT_TYPE;
        }
        else
        {
            element_localname = AXIS2_WSA_INTERFACE_NAME;
        }

        interface_ele = axiom_element_create(env, parent_node, element_localname, addr_ns_obj,
            &interface_node);
        qname_prefix = axutil_qname_get_prefix(interface_qname, env);
        qname_localpart = axutil_qname_get_localpart(interface_qname, env);

        text = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (axutil_strlen(qname_prefix)
            + axutil_strlen(qname_localpart) + 2));
        sprintf(text, "%s:%s", qname_prefix, qname_localpart);
        axiom_element_set_text(interface_ele, env, text, interface_node);
        AXIS2_FREE(env->allocator, text);
        if(interface_ele)
        {
            axiom_namespace_t *dec_ns = NULL;
            dec_ns = axiom_element_find_declared_namespace(interface_ele, env, addr_ns,
                AXIS2_WSA_DEFAULT_PREFIX);
            if(!dec_ns)
            {
                axiom_namespace_free(addr_ns_obj, env);
                addr_ns_obj = NULL;
            }
        }

    }

    service_name = axis2_endpoint_ref_get_svc_name(epr, env);
    return AXIS2_SUCCESS;
}

axis2_status_t
axis2_addr_out_handler_process_any_content_type(
    const axutil_env_t * env,
    axis2_any_content_type_t * reference_values,
    axiom_node_t * parent_ele_node,
    const axis2_char_t * addr_ns)
{
    axutil_hash_t *value_ht = NULL;
    axutil_hash_index_t *hash_index = NULL;

    if(reference_values)
    {
        const void *key = NULL;
        void *val = NULL;
        axis2_ssize_t len = 0;
        value_ht = axis2_any_content_type_get_value_map(reference_values, env);
        if(!value_ht)
        {
            return AXIS2_FAILURE;
        }

        for(hash_index = axutil_hash_first(value_ht, env); hash_index; hash_index
            = axutil_hash_next(env, hash_index))
        {
            axutil_hash_this(hash_index, &key, &len, &val);
            if(key)
            {
                axiom_node_t *node = NULL;
                axiom_element_t *ele = NULL;
                ele = axiom_element_create(env, parent_ele_node, key, NULL, &node);
                if(ele)
                {
                    if(!axutil_strcmp(AXIS2_WSA_NAMESPACE, addr_ns))
                    {
                        axiom_namespace_t *addr_ns_obj = NULL;
                        axiom_attribute_t *att = NULL;
                        addr_ns_obj
                            = axiom_namespace_create(env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
                        att = axiom_attribute_create(env,
                            AXIS2_WSA_IS_REFERENCE_PARAMETER_ATTRIBUTE,
                            AXIS2_WSA_TYPE_ATTRIBUTE_VALUE, addr_ns_obj);
                    }
                    axiom_element_set_text(ele, env, val, node);
                }
            }
        }
    }
    return AXIS2_SUCCESS;
}
