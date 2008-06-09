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

#include <axiom.h>
#include <axis2_amqp_util.h>

axis2_char_t* AXIS2_CALL
axis2_amqp_util_get_conf_value_string (axis2_transport_in_desc_t* in_desc,
						  	           const axutil_env_t* 	   	  env,
						               const axis2_char_t*	 	  param_name)
{
	axutil_param_t* param = NULL;
    axis2_char_t* 	value = NULL;

	param = (axutil_param_t*)
			axutil_param_container_get_param (axis2_transport_in_desc_param_container (in_desc, env),
											  env,
											  param_name);
    if (param)
    {
		value = axutil_param_get_value (param, env);
    }

	return value;
}


int AXIS2_CALL
axis2_amqp_util_get_conf_value_int (axis2_transport_in_desc_t* in_desc,
						            const axutil_env_t* 	   env,
						            const axis2_char_t*		   param_name)
{
	axis2_char_t* value_str = NULL;
	int 		  value = 0;

	value_str = axis2_amqp_util_get_conf_value_string (in_desc, env, param_name);
	if (value_str)
	{
		value = atoi (value_str);
	}

	return value;
}


axiom_soap_envelope_t* AXIS2_CALL
axis2_amqp_util_get_soap_envelope (const axis2_char_t* content,
								   const axutil_env_t* env)
{
	axiom_xml_reader_t* xml_reader = NULL;
	axiom_stax_builder_t* stax_builder = NULL;
	axiom_soap_builder_t* soap_builder = NULL;
	axiom_soap_envelope_t* soap_envelope = NULL;

	xml_reader = axiom_xml_reader_create_for_memory (env,
                                                     (axis2_char_t*)content,
                                                     axutil_strlen (content),
                                                     NULL,
                                                     AXIS2_XML_PARSER_TYPE_BUFFER);
    if (!xml_reader)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Failed to Create XML Reader");
        return NULL;
    }

    stax_builder = axiom_stax_builder_create (env, xml_reader);
    if (!stax_builder)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Failed to Create StAX Builder");
        return NULL;
    }

    soap_builder = axiom_soap_builder_create (env,
                                              stax_builder,
                                              AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI);
    if (!soap_builder)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Failed to Create SOAP Builder");
        return NULL;
    }
	
	soap_envelope = axiom_soap_builder_get_soap_envelope (soap_builder, env);

	return soap_envelope;
}


axutil_url_t* AXIS2_CALL
axis2_amqp_util_get_qpid_broker_url (axis2_msg_ctx_t*    msg_ctx,
									 const axutil_env_t* env)
{
	axis2_endpoint_ref_t* endpoint_ref = NULL;
	const axis2_char_t* endpoint_ref_str = NULL;

	endpoint_ref = axis2_msg_ctx_get_to (msg_ctx, env);
	if (!endpoint_ref)
		return NULL;
	
	endpoint_ref_str = axis2_endpoint_ref_get_address (endpoint_ref, env);
	if (!endpoint_ref_str)
		return NULL;
	
	return axutil_url_parse_string (env, endpoint_ref_str);
}


axis2_char_t* AXIS2_CALL
axis2_amqp_util_get_qpid_broker_ip (axis2_msg_ctx_t*    msg_ctx,
		                            const axutil_env_t* env)
{
	axutil_url_t* endpoint_url = axis2_amqp_util_get_qpid_broker_url (msg_ctx, env);

	if (!endpoint_url)
		return NULL;

	return axutil_url_get_host (endpoint_url, env);
}


int AXIS2_CALL
axis2_amqp_util_get_qpid_broker_port (axis2_msg_ctx_t*    msg_ctx,
		                              const axutil_env_t* env)
{
	axutil_url_t* endpoint_url = axis2_amqp_util_get_qpid_broker_url (msg_ctx, env);

	if (!endpoint_url)
		return -1;

	return axutil_url_get_port (endpoint_url, env);;
}
