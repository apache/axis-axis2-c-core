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

#include <axis2_svc_client.h>
#include <axis2.h>
#include <axis2_hash.h>
#include <callback_recv.h>
#include <axis2_soap.h>
#include <axis2_soap_body.h>
#include <listener_manager.h>
#include <axis2_module_desc.h>
#include <axis2_array_list.h>
#include <axis2_options.h>

typedef struct axis2_svc_client_impl
{
    /** svc_client base struct */
    axis2_svc_client_t svc_client;

	axis2_svc_t *svc;

	axis2_conf_t *conf;

	axis2_conf_ctx_t *conf_ctx;

	axis2_svc_ctx_t *svc_ctx;

	axis2_options_t *options;

	axis2_options_t *override_options;

	axis2_array_list_t *headers;
	
	/** for receiving the async messages */
	axis2_callback_recv_t *callback_recv;	

} axis2_svc_client_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(svc_client) ((axis2_svc_client_impl_t *)svc_client)

/** private functions */
static axis2_bool_t initialize_transport(axis2_env_t **env, 
									axis2_svc_client_impl_t *svc_client_impl,
									axis2_conf_ctx_t *conf_ctx,
									axis2_char_t *client_home);
static axis2_bool_t axis2_svc_client_init_data(axis2_env_t **env,
									axis2_svc_client_impl_t *svc_client_impl);
static void axis2_svc_client_init_ops(axis2_svc_client_t *svc_client);
static axis2_svc_t* axis2_create_annonymous_svc(axis2_env_t **env);
static axis2_bool_t fill_soap_envelope(axis2_env_t **env, axis2_svc_client_impl_t *svc_client_impl,
                                axis2_msg_ctx_t *msg_ctx, axis2_om_node_t *payload);

/** public funcitons */
axis2_svc_t* AXIS2_CALL 
axis2_svc_client_get_axis_service(struct axis2_svc_client *svc_client);

axis2_status_t AXIS2_CALL 
axis2_svc_client_set_options(struct axis2_svc_client *svc_client,
                axis2_env_t **env,
                axis2_options_t *options);

axis2_options_t* AXIS2_CALL 
axis2_svc_client_get_options(struct axis2_svc_client *svc_client);

axis2_status_t AXIS2_CALL 
axis2_svc_client_set_override_options(struct axis2_svc_client *svc_client,
                        axis2_env_t **env,
                        axis2_options_t *override_options);

axis2_options_t* AXIS2_CALL 
axis2_svc_client_get_override_options(struct axis2_svc_client *svc_client);

axis2_status_t AXIS2_CALL 
axis2_svc_client_engage_module(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *module_name);

axis2_status_t AXIS2_CALL 
axis2_svc_client_disengage_module(struct axis2_svc_client *svc_client,
                        axis2_env_t **env,
                        axis2_qname_t *module_name);

axis2_status_t AXIS2_CALL 
axis2_svc_client_add_header(struct axis2_svc_client *svc_client,
                				axis2_env_t **env,
                				axis2_om_node_t *header);

axis2_status_t AXIS2_CALL 
axis2_svc_client_remove_headers(struct axis2_svc_client *svc_client,
                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_svc_client_add_header_with_text(struct axis2_svc_client *svc_client,
                        axis2_env_t **env,
                        axis2_qname_t *header_name,
                        axis2_char_t *header_text);

axis2_status_t AXIS2_CALL 
axis2_svc_client_send_robust(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_om_node_t *payload);


axis2_status_t AXIS2_CALL 
axis2_svc_client_send_robust_with_operation(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *operation,
                    axis2_om_node_t *payload);

void AXIS2_CALL 
axis2_svc_client_fire_and_forget(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_om_node_t *payload);


void AXIS2_CALL 
axis2_svc_client_fire_and_forget_with_operation(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *operation,
                    axis2_om_node_t *payload);

axis2_om_node_t* AXIS2_CALL 
axis2_svc_client_send_receive(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_om_node_t *payload);


axis2_om_node_t* AXIS2_CALL 
axis2_svc_client_send_receive_with_operation(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *operation,
                    axis2_om_node_t *payload);


void AXIS2_CALL 
axis2_svc_client_send_receive_non_blocking(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_om_node_t *payload,
                    axis2_callback_t *callback);

void AXIS2_CALL 
axis2_svc_client_send_receive_non_blocking_with_operation(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *operation,
                    axis2_om_node_t *payload,
                    axis2_callback_t *callback);

axis2_op_client_t* AXIS2_CALL 
axis2_svc_client_create_client(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *operation);

axis2_status_t AXIS2_CALL 
axis2_svc_client_finalize_invoke(struct axis2_svc_client *svc_client,
                        axis2_env_t **env);
    
axis2_endpoint_ref_t* AXIS2_CALL 
axis2_svc_client_get_my_epr(struct axis2_svc_client *svc_client,
                axis2_env_t **env,
                axis2_char_t *transport);

axis2_endpoint_ref_t* AXIS2_CALL 
axis2_svc_client_get_target_epr(struct axis2_svc_client *svc_client,
                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_svc_client_set_target_epr(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_endpoint_ref_t *target_epr);
    
axis2_svc_ctx_t* AXIS2_CALL 
axis2_svc_client_get_svc_ctx(struct axis2_svc_client *svc_client,
                    axis2_env_t **env);


axis2_svc_client_t* AXIS2_CALL
axis2_svc_client_create(axis2_env_t **env, axis2_char_t *client_home)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
   	svc_client_impl = AXIS2_INTF_TO_IMPL(axis2_svc_client_create_with_conf_ctx_and_svc
											(env, NULL, NULL, client_home));
	
	if (!svc_client_impl)
		return NULL;
 
	return &(svc_client_impl->svc_client);
}

axis2_svc_client_t* AXIS2_CALL
axis2_svc_client_create_with_conf_ctx_and_svc(axis2_env_t **env,
                                         	axis2_conf_ctx_t *conf_ctx,
                                         	axis2_svc_t *svc,
											axis2_char_t *client_home)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
	axis2_svc_grp_t *svc_grp = NULL;
	axis2_svc_grp_ctx_t *svc_grp_ctx = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc_client_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_svc_client_impl_t));
    if (!svc_client_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /** initialize private data to NULL, create options */
    if (!axis2_svc_client_init_data(env, svc_client_impl))
	{
		axis2_svc_client_free(&(svc_client_impl->svc_client), env);
		retrun NULL;
	}
	
	if (!initialize_transport(env, svc_client_impl, conf_ctx, client_home))
	{
		axis2_svc_client_free(&(svc_client_impl->svc_client), env);
		return NULL;
	}

	svc_client_impl->conf = AXIS2_CONF_CTX_GET_CONF(svc_client_impl->conf_ctx, env);
	
	if (svc)
	{
		svc_client_impl->svc = svc;
	}
	else
	{
		if (NULL == (svc_client_impl->svc = axis2_create_annonymous_svc(env)))
		{
			axis2_svc_client_free(&(svc_client_impl->svc_client), env);
			return NULL;
		}
	}
	
	/** add the service to the config context if it isn't in there already */
	if (NULL == AXIS2_CONF_GET_SVC(svc_client_impl->conf, env, 
					AXIS2_SVC_GET_AXIS2_SVC_NAME(svc_client_impl->svc, env)))
	{
		AXIS2_CONF_ADD_SVC(svc_client_impl->conf, env, svc_client_impl->svc);
	}

    /** create a service context for myself: create a new service group
     context and then get the service context for myself as I'll need that
     later for stuff that I gotta do
	 */
	svc_grp = AXIS2_SVC_GET_PARENT(svc_client_impl->svc, env);
    if (!svc_grp)
        return NULL;
    
	svc_grp_ctx = AXIS2_SVC_GRP_GET_SVC_GRP_CTX(svc_grp, env, 
					svc_client_impl->conf_ctx);
    if (!svc_grp_ctx)
        return NULL;
    
	AXIS2_CONF_CTX_REGISTER_SVC_GRP_CTX(svc_client_impl->conf_ctx, env, 
		AXIS2_SVC_GET_AXIS2_SVC_NAME(svc_client_impl->svc, env), svc_grp_ctx);
    
	svc_client_impl->svc_ctx = AXIS2_SVC_GRP_CTX_GET_SVC_CTX(svc_grp_ctx, env,
		        AXIS2_SVC_GET_AXIS2_SVC_NAME(svc_client_impl->svc, env));
    
    /* create ops */
    svc_client_impl->svc_client.ops  =
        AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_svc_client_ops_t));

    if (!svc_client_impl->svc_client.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_svc_client_free(&(svc_client_impl->svc_client), env);
        return NULL;
    }

    /** initialize ops */
    axis2_svc_client_init_ops(&(svc_client_impl->svc_client));

    return &(svc_client_impl->svc_client);
}



axis2_svc_t* AXIS2_CALL 
axis2_svc_client_get_axis_service(struct axis2_svc_client *svc_client)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    
   	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
	return svc_client_impl->svc;

}

axis2_status_t AXIS2_CALL 
axis2_svc_client_set_options(struct axis2_svc_client *svc_client,
                axis2_env_t **env,
                axis2_options_t *options)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
	
	/*TODO:double check whether we need to free*/
	if (svc_client_impl->options)
		axis2_options_free(svc_client_impl->options, env);
   	svc_client_impl->options = options;
	return AXIS2_SUCCESS;	
}

axis2_options_t* AXIS2_CALL 
axis2_svc_client_get_options(struct axis2_svc_client *svc_client)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	return svc_client_impl->options;
}

axis2_status_t AXIS2_CALL 
axis2_svc_client_set_override_options(struct axis2_svc_client *svc_client,
                        axis2_env_t **env,
                        axis2_options_t *override_options)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	if (svc_client_impl->override_options)
		axis2_options_free(svc_client_impl->override_options, env);

	svc_client_impl->override_options = override_options;

	return AXIS2_SUCCESS;
}

axis2_options_t* AXIS2_CALL 
axis2_svc_client_get_override_options(struct axis2_svc_client *svc_client)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;

    svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	return svc_client_impl->override_options;
}

axis2_status_t AXIS2_CALL 
axis2_svc_client_engage_module(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *module_name)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
	axis2_module_desc_t *module = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	module = AXIS2_CONF_GET_MODULE(svc_client_impl->conf, env, module_name);

	if (module)
	{
		return AXIS2_SVC_ENGAGE_MODULE(svc_client_impl->svc, env, module,
										svc_client_impl->conf);
	}
	return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL 
axis2_svc_client_disengage_module(struct axis2_svc_client *svc_client,
                        axis2_env_t **env,
                        axis2_qname_t *module_name)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
	axis2_module_desc_t *module = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	module = AXIS2_CONF_GET_MODULE(svc_client_impl->conf, env, module_name);

	/**TODO:uncomment once axis2_svc_disengage_module is implemented
	if (module)
	{
		return AXIS2_SVC_DISENGAGE_MODULE(svc_client_impl->svc, env, module);
		
	}
	*/

	return AXIS2_FAILURE;

}

axis2_status_t AXIS2_CALL 
axis2_svc_client_add_header(struct axis2_svc_client *svc_client,
                				axis2_env_t **env,
                				axis2_om_node_t *header)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	if (!svc_client_impl->headers)
	{
		svc_client_impl->headers = axis2_array_list_create(env, 0);
		if(!svc_client_impl->headers)
		{
			AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
			return AXIS2_FAILURE;
		}
	}
	AXIS2_ARRAY_LIST_ADD(svc_client_impl->headers, env, header);

	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_svc_client_remove_headers(struct axis2_svc_client *svc_client,
                    axis2_env_t **env)
{
    axis2_svc_client_impl_t *svc_client_impl = NULL;
	int i = 0, size = 0;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	if (!svc_client_impl->headers)
		return AXIS2_FAILURE;
	
	size = AXIS2_ARRAY_LIST_SIZE(svc_client_impl->headers, env);
	for (i = 0; i < size; i++)
	{
		axis2_om_node_t *node = NULL;
		node = AXIS2_ARRAY_LIST_GET(svc_client_impl->headers, env, i);
		if (node)
		{
			AXIS2_OM_NODE_FREE_TREE(node, env);
			node = NULL;
		}
	}
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_svc_client_add_header_with_text(struct axis2_svc_client *svc_client,
                        axis2_env_t **env,
                        axis2_qname_t *header_name,
                        axis2_char_t *header_text)
{
	axis2_om_node_t *node_name, *node_text;
	axis2_om_element_t *element = NULL;
	axis2_om_text_t *text = NULL;
    axis2_svc_client_impl_t *svc_client_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	element = axis2_om_element_create_with_qname(env, NULL, header_name, &node_name);
	if (!element)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return AXIS2_FAILURE;
	}
	text = axis2_om_text_create(env, node_name, header_text, &node_text);
	if (!text)
	{
		AXIS2_OM_ELEMENT_FREE(element, env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return AXIS2_FAILURE;
	}
	return axis2_svc_client_add_header(&(svc_client_impl->svc_client), env,
			node_name);
}

axis2_status_t AXIS2_CALL 
axis2_svc_client_send_robust(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_om_node_t *payload)
{
	return AXIS2_FAILURE;
}


axis2_status_t AXIS2_CALL 
axis2_svc_client_send_robust_with_operation(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *operation,
                    axis2_om_node_t *payload)
{
	return AXIS2_FAILURE;
}

void AXIS2_CALL 
axis2_svc_client_fire_and_forget(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_om_node_t *payload)
{

}

void AXIS2_CALL 
axis2_svc_client_fire_and_forget_with_operation(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *operation,
                    axis2_om_node_t *payload)
{

}

axis2_om_node_t* AXIS2_CALL 
axis2_svc_client_send_receive(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_om_node_t *payload)
{
	axis2_svc_client_impl_t *svc_client_impl = NULL;
	axis2_qname_t *op = NULL;
	AXIS2_ENV_CHECK(env, NULL);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
	op = axis2_qname_create(env, AXIS2_ANON_OUT_IN_OP, NULL, NULL);
	
	return axis2_svc_client_send_receive_with_operation(
			&(svc_client_impl->svc_client), env, op, payload);
}


axis2_om_node_t* AXIS2_CALL 
axis2_svc_client_send_receive_with_operation(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *operation,
                    axis2_om_node_t *payload)
{
	axis2_svc_client_impl_t *svc_client_impl = NULL;

	AXIS2_ENV_CHECK(env, NULL);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	if (AXIS2_OPTIONS_IS_USE_SEPERATE_LISTENER(svc_client_impl->options, env))
	{
		return NULL;
	}
	else
	{
		axis2_op_client_t *mep_client = NULL;
		axis2_msg_ctx_t *res_msg_ctx = NULL;
		axis2_msg_ctx_t *mc = NULL;
		axis2_soap_envelope_t *soap_envelope = NULL;
		axis2_soap_body_t *soap_body = NULL;
		axis2_om_node_t *soap_node = NULL;

		mc = axis2_msg_ctx_create(env, 
				AXIS2_SVC_CTX_GET_CONF_CTX(svc_client_impl->svc_ctx, env), NULL, NULL);
		if (!fill_soap_envelope(env, svc_client_impl, mc, payload))
			return NULL;
		
		mep_client = axis2_svc_client_create_client(&(svc_client_impl->svc_client), env, operation);
		
		AXIS2_OP_CLIENT_ADD_MSG_CTX(mep_client, env, mc);
		AXIS2_OP_CLIENT_EXECUTE(mep_client, env, AXIS2_TRUE);
		res_msg_ctx = AXIS2_OP_CTX_GET_MSG_CTX(mep_client, env, AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE);
		
		if (!res_msg_ctx)
        {
			return NULL;
		}

        soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(res_msg_ctx, env);
        if (!soap_envelope)
        {
			return NULL;
		}
        soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);

        if (!soap_body)
        {
			return NULL;
		}
        
        soap_node = AXIS2_SOAP_BODY_GET_BASE_NODE(soap_body, env);
        if (!soap_node)
        {
			return NULL;
		}
        return AXIS2_OM_NODE_GET_FIRST_CHILD(soap_node, env);
	}
}


void AXIS2_CALL 
axis2_svc_client_send_receive_non_blocking(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_om_node_t *payload,
                    axis2_callback_t *callback)
{

}

void AXIS2_CALL 
axis2_svc_client_send_receive_non_blocking_with_operation(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *operation,
                    axis2_om_node_t *payload,
                    axis2_callback_t *callback)
{

}

axis2_op_client_t* AXIS2_CALL 
axis2_svc_client_create_client(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_qname_t *operation)
{
	axis2_op_t *op = NULL;
	axis2_op_client_t *op_client = NULL;
	axis2_svc_client_impl_t *svc_client_impl = NULL;

	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	op = AXIS2_SVC_GET_OP_WITH_QNAME(svc_client_impl->svc, env, operation);
	if (!op)
	{
		/*TODO:error - svc does not have the operation*/
		return NULL;
	}
	op_client = axis2_op_client_create(env, op, svc_client_impl->svc_ctx, 
			svc_client_impl->options);
	/**
	 if overide options have been set, that means we need to make sure
     those options override the options of even the operation client. So,
     what we do is switch the parents around to make that work.
	*/
	if (!svc_client_impl->override_options)
	{
		AXIS2_OPTIONS_SET_PARENT(svc_client_impl->override_options, env, 
			AXIS2_OPERATION_CLIENT_GET_OPTIONS(op_client, env));
		AXIS2_OPERATION_CLIENT_SET_OPTIONS(op_client, env, svc_client_impl->override_options);
	}

	return op_client;	
}

axis2_status_t AXIS2_CALL 
axis2_svc_client_finalize_invoke(struct axis2_svc_client *svc_client,
                        axis2_env_t **env)
{
	axis2_svc_client_impl_t *svc_client_impl = NULL;
	axis2_listener_manager_t *listener_manager = NULL;
	axis2_char_t *transport = NULL;

	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	/*TODO:uncomment once implemented
	listener_manager = AXIS2_CONF_CTX_GET_LISTENER_MANAGER(svc_client_impl->conf_ctx,
			env);
			*/

	if (!listener_manager)
	{
		/*TODO:error - listener manager not found*/
		return AXIS2_FAILURE;
	}
	/*TODO:set transport*/
	return AXIS2_LISTENER_MANAGER_STOP(listener_manager, env, transport);
}
    
axis2_endpoint_ref_t* AXIS2_CALL 
axis2_svc_client_get_my_epr(struct axis2_svc_client *svc_client,
                axis2_env_t **env,
                axis2_char_t *transport)
{
	axis2_svc_client_impl_t *svc_client_impl = NULL;

	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
	/*TODO:implement-issue - there's not get_my_epr in svc_ctx*/
	
	return NULL;
}

axis2_endpoint_ref_t* AXIS2_CALL 
axis2_svc_client_get_target_epr(struct axis2_svc_client *svc_client,
                    axis2_env_t **env)
{
	axis2_svc_client_impl_t *svc_client_impl = NULL;

	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
	/*TODO:implement-issue - there's not get_target_epr in svc_ctx*/

	return NULL;
}

axis2_status_t AXIS2_CALL 
axis2_svc_client_set_target_epr(struct axis2_svc_client *svc_client,
                    axis2_env_t **env,
                    axis2_endpoint_ref_t *target_epr)
{
	axis2_svc_client_impl_t *svc_client_impl = NULL;

	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);
	/*TODO:implement-issue - there's not set_my_epr in svc_ctx*/

	return AXIS2_FAILURE;
}
    
axis2_svc_ctx_t* AXIS2_CALL 
axis2_svc_client_get_svc_ctx(struct axis2_svc_client *svc_client,
                    axis2_env_t **env)
{
	axis2_svc_client_impl_t *svc_client_impl = NULL;

	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	svc_client_impl = AXIS2_INTF_TO_IMPL(svc_client);

	return svc_client_impl->svc_ctx;
}




/** private function implementation */

static axis2_bool_t initialize_transport(axis2_env_t **env,
                                    axis2_svc_client_impl_t *svc_client_impl,
                                    axis2_conf_ctx_t *conf_ctx,
									axis2_char_t *client_home)
{
	axis2_listener_manager_t *listener_manager = NULL;

	if (conf_ctx)
	{
		svc_client_impl->conf_ctx = conf_ctx;
		/*TODO:uncomment once implemented
		listener_manager = AXIS2_CONF_CTX_GET_LISTENER_MANAGER(conf_ctx, env);
		*/
    	if (!listener_manager)
    	{
		    listener_manager = axis2_listener_manager_create(env);
			if (!listener_manager)
			{
				AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        		return AXIS2_FALSE;
			}
			/*TODO:uncomment once implemented
			AXIS2_LISTENER_MANAGER_INIT(listener_manager, conf_ctx);
			*/
    	}

	}
	else
	{
		svc_client_impl->conf_ctx = build_client_conf_ctx(env, client_home);
		if (!svc_client_impl->conf_ctx)
		{
			AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
			return AXIS2_FALSE;
		}
		listener_manager = axis2_listener_manager_create(env);
		if (!listener_manager)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FALSE;
        }
		/*TODO:uncomment when implemented
		AXIS2_LISTENER_MANAGER_INIT(listener_manager, svc_client_impl->conf_ctx);
		*/
	}
	return AXIS2_TRUE;
}

static axis2_bool_t axis2_svc_client_init_data(axis2_env_t **env,
								axis2_svc_client_impl_t *svc_client_impl)
{
    svc_client_impl->svc = NULL;

    svc_client_impl->conf_ctx = NULL;

    svc_client_impl->svc_ctx = NULL;

	svc_client_impl->options = AXIS2_MALLOC((*env)->allocator, sizeof(axis2_options_t));
    if (!svc_client_impl->options)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FALSE;
	}

    svc_client_impl->override_options = NULL;

    svc_client_impl->headers = NULL;

    svc_client_impl->callback_recv = axis2_callback_recv_create(env);
    if (!(svc_client_impl->callback_recv))
    {
        axis2_svc_client_free(&(svc_client_impl->svc_client), env);
        return AXIS2_FALSE;
    }

	return AXIS2_TRUE;
}

static void axis2_svc_client_init_ops(axis2_svc_client_t *svc_client)
{

	svc_client->ops->get_axis_service = axis2_svc_client_get_axis_service;
	svc_client->ops->set_options = axis2_svc_client_set_options;
	svc_client->ops->get_options = axis2_svc_client_get_options;
	svc_client->ops->set_override_options = axis2_svc_client_set_override_options;
	svc_client->ops->get_override_options = axis2_svc_client_get_override_options;
	svc_client->ops->engage_module = axis2_svc_client_engage_module;
	svc_client->ops->disengage_module = axis2_svc_client_disengage_module;
	svc_client->ops->add_header = axis2_svc_client_add_header;
	svc_client->ops->remove_headers = axis2_svc_client_remove_headers;
	svc_client->ops->add_header_with_text = axis2_svc_client_add_header_with_text;
	svc_client->ops->send_robust = axis2_svc_client_send_robust;
	svc_client->ops->send_robust_with_operation = axis2_svc_client_send_robust_with_operation;
	svc_client->ops->fire_and_forget = axis2_svc_client_fire_and_forget;
	svc_client->ops->fire_and_forget_with_operation = axis2_svc_client_fire_and_forget_with_operation;
	svc_client->ops->send_receive = axis2_svc_client_send_receive;
	svc_client->ops->send_receive_with_operation = axis2_svc_client_send_receive_with_operation;
	svc_client->ops->send_receive_non_blocking = axis2_svc_client_send_receive_non_blocking;
	svc_client->ops->send_receive_non_blocking_with_operation = axis2_svc_client_send_receive_non_blocking_with_operation;
	svc_client->ops->create_client = axis2_svc_client_create_client;
	svc_client->ops->finalize_invoke = axis2_svc_client_finalize_invoke;
	svc_client->ops->get_my_epr = axis2_svc_client_get_my_epr;
	svc_client->ops->get_target_epr = axis2_svc_client_get_target_epr;
	svc_client->ops->set_target_epr = axis2_svc_client_set_target_epr;
	svc_client->ops->get_svc_ctx = axis2_svc_client_get_svc_ctx;
}

/**
  * Create an anonymous axisService with one (anonymous) operation each for
  * each MEP that I support dealing with anonymously using the convenience
  * APIs.
  *
  * @return the minted anonymous service
  */

static axis2_svc_t* axis2_create_annonymous_svc(axis2_env_t **env)
{
	/**
	now add anonymous operations to the axis2 service for use with the
    shortcut client API. NOTE: We only add the ones we know we'll use
    later in the convenience API; if you use
    this constructor then you can't expect any magic!
	*/
	axis2_qname_t *tmp_qname;
	axis2_svc_t *svc;
	axis2_op_t *op_out_in, *op_out_only, *op_robust_out_only;

	tmp_qname = axis2_qname_create(env, AXIS2_ANON_SERVICE, NULL, NULL);
	
	if (!tmp_qname)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return NULL;
	}

	svc = axis2_svc_create_with_qname(env, tmp_qname);	
	
	if (!svc)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return NULL;
	}
	AXIS2_QNAME_FREE(tmp_qname, env);

	tmp_qname = axis2_qname_create(env, AXIS2_ANON_OUT_IN_OP, NULL, NULL);

    if (!tmp_qname)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
	op_out_in = axis2_op_create_with_qname(env, tmp_qname);
	AXIS2_QNAME_FREE(tmp_qname, env);

			
	tmp_qname = axis2_qname_create(env, AXIS2_ANON_OUT_ONLY_OP, NULL, NULL);

    if (!tmp_qname)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
	op_out_only = axis2_op_create_with_qname(env, tmp_qname);
	AXIS2_QNAME_FREE(tmp_qname, env);

	tmp_qname = axis2_qname_create(env, AXIS2_ANON_ROBUST_OUT_ONLY_OP, NULL, NULL);

    if (!tmp_qname)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
	op_robust_out_only = axis2_op_create_with_qname(env, tmp_qname);
	AXIS2_QNAME_FREE(tmp_qname, env);

	if (!op_out_in || !op_out_only || !op_robust_out_only)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		if (op_out_in)
		{
			AXIS2_OP_FREE(op_out_in, env);
		}
		if (op_out_only)
		{
			AXIS2_OP_FREE(op_out_only, env);
		}
		if (op_robust_out_only)
		{
			AXIS2_OP_FREE(op_robust_out_only, env);
		}

        return NULL;
    }

	AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op_out_in, env, AXIS2_MEP_URI_OUT_IN);
	AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op_out_only, env, AXIS2_MEP_URI_OUT_ONLY);
	AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op_robust_out_only, env, AXIS2_MEP_URI_ROBUST_OUT_ONLY);
	
	AXIS2_SVC_ADD_OP(svc, env, op_out_in);
	AXIS2_SVC_ADD_OP(svc, env, op_out_only);
	AXIS2_SVC_ADD_OP(svc, env, op_robust_out_only);
	return svc;	
}

static axis2_bool_t fill_soap_envelope(axis2_env_t **env, axis2_svc_client_impl_t *svc_client_impl,
								axis2_msg_ctx_t *msg_ctx, axis2_om_node_t *payload)
{
	axis2_char_t *soap_version_uri;
	int soap_version;
	axis2_soap_envelope_t *envelope = NULL;

	soap_version_uri = AXIS2_OPTIONS_GET_SOAP_VERSION_URI(svc_client_impl->options, env);

    if (soap_version_uri)
    {
		return AXIS2_FALSE;
	}

    if (AXIS2_STRCMP(soap_version_uri,
                AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI) == 0)
    	soap_version = AXIS2_SOAP11;
    else
    	soap_version = AXIS2_SOAP12;
    

    envelope = axis2_soap_envelope_create_default_soap_envelope(env, soap_version);
    if (!envelope)
    {
        return AXIS2_FALSE;
    }

    if (payload)
    {
        axis2_soap_body_t *soap_body = NULL;
        soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(envelope, env);
        if (soap_body)
        {
            axis2_om_node_t *node = NULL;
            node = AXIS2_SOAP_BODY_GET_BASE_NODE(soap_body, env);
            if (node)
            {
                AXIS2_OM_NODE_ADD_CHILD(node, env, payload);
            }
        }
    }

    AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, envelope);
	return AXIS2_TRUE;
}

