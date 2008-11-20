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

#include <axiom_soap.h>
#include <axis2_transport_in_desc.h>
#include <axis2_amqp_defines.h>
#include <axis2_amqp_util.h>
#include <axis2_amqp_receiver.h>
#include <axis2_amqp_sender.h>

static const axis2_transport_sender_ops_t amqp_sender_ops = {
	axis2_amqp_sender_init,
	axis2_amqp_sender_invoke,
	axis2_amqp_sender_clean_up,
	axis2_amqp_sender_free};

AXIS2_EXTERN axis2_transport_sender_t* AXIS2_CALL
axis2_amqp_sender_create(const axutil_env_t* env)
{
	AXIS2_ENV_CHECK(env, NULL);

	axis2_amqp_sender_resource_pack_t* sender_resource_pack = NULL;

	sender_resource_pack = (axis2_amqp_sender_resource_pack_t*)
							AXIS2_MALLOC(env->allocator,
										  sizeof(axis2_amqp_sender_resource_pack_t));

	if (!sender_resource_pack)
	{
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return NULL;
	}

	sender_resource_pack->sender.ops = &amqp_sender_ops;
	sender_resource_pack->conf_ctx = NULL;

	return &(sender_resource_pack->sender);
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL 
axis2_amqp_sender_init(
	axis2_transport_sender_t* sender,
	const axutil_env_t* env,
	axis2_conf_ctx_t* conf_ctx,
	axis2_transport_out_desc_t* out_desc)
{
	axis2_amqp_sender_resource_pack_t* sender_resource_pack = NULL;
    sender_resource_pack = AXIS2_AMQP_SENDER_TO_RESOURCE_PACK(sender);

	sender_resource_pack->conf_ctx = conf_ctx;

	return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL 
axis2_amqp_sender_invoke(
	axis2_transport_sender_t* sender,
	const axutil_env_t* env,
	axis2_msg_ctx_t* msg_ctx)
{
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

	axiom_soap_envelope_t* request_soap_envelope = NULL;
	axiom_xml_writer_t* xml_writer = NULL;
	axiom_output_t* request_om_output = NULL;
	axis2_char_t* request_content = NULL;
	axiom_soap_envelope_t* response_soap_envelope = NULL;
	axis2_bool_t is_server = AXIS2_TRUE;
	axis2_bool_t is_soap_11 = AXIS2_FALSE;
	axutil_string_t* content_type = NULL;
	const axis2_char_t* soap_action = NULL;
	axis2_bool_t do_mtom = AXIS2_FALSE;
	axis2_bool_t status = AXIS2_FAILURE;

	request_soap_envelope = axis2_msg_ctx_get_soap_envelope(msg_ctx, env);
	
	xml_writer = axiom_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0, 
					AXIS2_XML_PARSER_TYPE_BUFFER);
	if (!xml_writer)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to Create XML Writer");
		return AXIS2_FAILURE;
	}

	request_om_output = axiom_output_create(env, xml_writer);
	if (!request_om_output)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to Create OM Output");
		axiom_xml_writer_free(xml_writer, env);
		xml_writer = NULL;
		return AXIS2_FAILURE;
	}

	is_soap_11 = axis2_msg_ctx_get_is_soap_11(msg_ctx, env);

	/* Set SOAP version */
	axiom_output_set_soap11(request_om_output, env, is_soap_11);

	/* Content-Type */
	if (AXIS2_TRUE == is_soap_11)
	{
		/* SOAP1.1 */
		content_type = axutil_string_create(env, AXIS2_AMQP_HEADER_ACCEPT_TEXT_XML);
	}
	else
	{
		/* SOAP1.2 */
		content_type = axutil_string_create(env, AXIS2_AMQP_HEADER_ACCEPT_APPL_SOAP);
	}

	/* SOAP action */
	soap_action = axutil_string_get_buffer(axis2_msg_ctx_get_soap_action(msg_ctx, env), env);

	if (!soap_action)
		soap_action = "";

	/* Handle MTOM */
	do_mtom = axis2_msg_ctx_get_doing_mtom(msg_ctx, env);

	axiom_output_set_do_optimize(request_om_output, env, do_mtom);
	axiom_soap_envelope_serialize(request_soap_envelope, env, request_om_output, AXIS2_FALSE);

	if (do_mtom)
	{
		axis2_status_t mtom_status = AXIS2_FAILURE;
		axutil_array_list_t* mime_parts = NULL;

		mtom_status = axiom_output_flush(request_om_output, env);

		if (mtom_status == AXIS2_SUCCESS)
		{
			mime_parts = axiom_output_get_mime_parts(request_om_output, env);
			if (!mime_parts)
			{
				AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
						"Unable to create the mime part list from request_om_output");
			
				return AXIS2_FAILURE;
			}
			else
			{
				axis2_msg_ctx_set_mime_parts(msg_ctx, env, mime_parts);
			}
		}
		
		content_type = axutil_string_create(env, 
				axiom_output_get_content_type(request_om_output, env));
	}

	request_content = (axis2_char_t*)axiom_xml_writer_get_xml(xml_writer, env);
	if (!request_content)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to Serialize the SOAP Envelope");
		return AXIS2_FAILURE;
	}
	
	is_server = axis2_msg_ctx_get_server_side(msg_ctx, env);

	if (is_server)
	{
		status = axis2_qpid_server_send(request_content, env, 
				axutil_string_get_buffer(content_type, env), soap_action, msg_ctx);
	}
	else
	{
		axis2_op_t* op = NULL;
		const axis2_char_t* mep = NULL;
		
		op = axis2_msg_ctx_get_op(msg_ctx, env);
		mep = axis2_op_get_msg_exchange_pattern(op, env);

		if (axutil_strcmp(mep, AXIS2_MEP_URI_OUT_ONLY) == 0 || 
			axutil_strcmp(mep, AXIS2_MEP_URI_ROBUST_OUT_ONLY) == 0) /* One-way */
		{
			status = axis2_qpid_client_send_robust(request_content, env, 
					axutil_string_get_buffer(content_type, env), soap_action, msg_ctx);

			/* Set Status Code in msg_ctx */
			axis2_msg_ctx_set_status_code(msg_ctx, env, status);
		}
		else
		{
			if (AXIS2_TRUE == axis2_amqp_util_msg_ctx_get_use_separate_listener(msg_ctx, env)) /* Dual Channel */
			{
				axis2_amqp_sender_resource_pack_t* sender_resource_pack = NULL;
				sender_resource_pack = AXIS2_AMQP_SENDER_TO_RESOURCE_PACK(sender);
				
				if (!sender_resource_pack->conf_ctx)
					return AXIS2_FAILURE;
					
				axis2_char_t* reply_to_queue_name = NULL;
				reply_to_queue_name = axis2_amqp_util_conf_ctx_get_dual_channel_queue_name(
						sender_resource_pack->conf_ctx, env);

				if (!reply_to_queue_name)
					return AXIS2_FAILURE;
				
				status = axis2_qpid_client_send_dual(request_content, env, reply_to_queue_name, 
						axutil_string_get_buffer(content_type, env), soap_action, msg_ctx);
			}
			else /* Single Channel */
			{
				axis2_amqp_binary_data_buffer_t* binary_data_buffer = NULL;
				binary_data_buffer = axis2_qpid_client_send_receive(request_content, env, 
						axutil_string_get_buffer(content_type, env), soap_action, msg_ctx);

				if (binary_data_buffer)
				{
					response_soap_envelope = axis2_amqp_util_get_soap_envelope(binary_data_buffer, env, msg_ctx);
				
					if (response_soap_envelope)
						axis2_msg_ctx_set_response_soap_envelope(msg_ctx, env, response_soap_envelope);

					AXIS2_FREE(env->allocator, binary_data_buffer->data);
					AXIS2_FREE(env->allocator, binary_data_buffer->content_type);
					AXIS2_FREE(env->allocator, binary_data_buffer);

					status = AXIS2_SUCCESS;
				}
			}
        }
	}

	if (content_type)
		axutil_string_free(content_type, env);
	
	return status;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL 
axis2_amqp_sender_clean_up(
	axis2_transport_sender_t* sender,
	const axutil_env_t* env,
	axis2_msg_ctx_t* msg_ctx)
{
	return AXIS2_SUCCESS;
}


AXIS2_EXTERN void AXIS2_CALL 
axis2_amqp_sender_free(
	axis2_transport_sender_t* sender,
	const axutil_env_t* env)
{
	AXIS2_ENV_CHECK(env, void);

	axis2_amqp_sender_resource_pack_t* sender_resource_pack = NULL;
	sender_resource_pack = AXIS2_AMQP_SENDER_TO_RESOURCE_PACK(sender);

	AXIS2_FREE(env->allocator, sender_resource_pack);
}

/* Library Exports */

AXIS2_EXPORT int 
#ifndef AXIS2_STATIC_DEPLOY
axis2_get_instance(
#else
axis2_amqp_sender_get_instance(
#endif
    struct axis2_transport_sender** inst,
    const axutil_env_t* env)
{
    int status = AXIS2_SUCCESS;

    *inst = axis2_amqp_sender_create(env);
    if (!(*inst))
    {
        status = AXIS2_FAILURE;
    }

    return status;
}


AXIS2_EXPORT int
#ifndef AXIS2_STATIC_DEPLOY
axis2_remove_instance(
#else
axis2_amqp_sender_remove_instance(
#endif
    axis2_transport_sender_t* inst,
    const axutil_env_t* env)
{
    if (inst)
    {
        axis2_transport_sender_free(inst, env);
    }

    return AXIS2_SUCCESS;
}

