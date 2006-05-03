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

#include <axis2_op_client.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_mep_client.h>
#include <axis2_uuid_gen.h>
#include <listener_manager.h> /*TODO:change too axis2_listener_manager.h*/

typedef struct axis2_op_client_impl
{
    /** op_client base struct */
    axis2_op_client_t op_client;

	axis2_mep_client_t *base;

	axis2_svc_ctx_t *svc_ctx;

	axis2_options_t *options;

	axis2_op_ctx_t *op_ctx;

	axis2_callback_t *callback;

	axis2_bool_t completed;

} axis2_op_client_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(op_client) ((axis2_op_client_impl_t *)op_client)

/** private function prototypes */
static void axis2_op_client_init_ops(axis2_op_client_t *op_client);
static axis2_msg_ctx_t* axis2_op_client_invoke_blocking(axis2_op_client_impl_t *op_client_impl,
															axis2_env_t **env,
															axis2_msg_ctx_t *mc);
/** public function prototypes */
axis2_status_t AXIS2_CALL 
axis2_op_client_set_options(struct axis2_op_client *op_client,
                    			axis2_env_t **env,
                    			axis2_options_t *options);

axis2_options_t* AXIS2_CALL 
axis2_op_client_get_options(struct axis2_op_client *op_client,
                    			axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_op_client_add_msg_ctx(struct axis2_op_client *op_client,
                        				axis2_env_t **env,
                        				axis2_msg_ctx_t *mc);


axis2_msg_ctx_t* AXIS2_CALL 
axis2_op_client_get_msg_ctx(struct axis2_op_client *op_client,
                            			axis2_env_t **env,
                            			axis2_char_t *message_label);

axis2_status_t AXIS2_CALL 
axis2_op_client_set_callback(struct axis2_op_client *op_client,
                    			axis2_env_t **env,
                    			axis2_callback_t *callback);

axis2_status_t AXIS2_CALL 
axis2_op_client_execute(struct axis2_op_client *op_client,
                			axis2_env_t **env,
                			axis2_bool_t block);

axis2_status_t AXIS2_CALL 
axis2_op_client_reset(struct axis2_op_client *op_client,
						axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_op_client_compelete(struct axis2_op_client *op_client,
                			axis2_env_t **env,
                			axis2_msg_ctx_t *mc);

axis2_op_ctx_t* AXIS2_CALL 
axis2_op_client_get_operation_context(struct axis2_op_client *op_client);

axis2_status_t AXIS2_CALL
axis2_op_client_free(struct axis2_op_client *op_client, 
						axis2_env_t **env);

axis2_op_client_t* AXIS2_CALL 
axis2_op_client_create(axis2_env_t **env, axis2_op_t *op,
						axis2_svc_ctx_t *svc_ctx,
						axis2_options_t *options)
{
    axis2_op_client_impl_t *op_client_impl = NULL;
	axis2_char_t *mep_uri = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, op, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svc_ctx, NULL);
    AXIS2_PARAM_CHECK((*env)->error, options, NULL);
    
    op_client_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_op_client_impl_t) );
    if (!op_client_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

	/** initialize data */
    op_client_impl->base = NULL;
	op_client_impl->svc_ctx = NULL;
	op_client_impl->options = NULL;
	op_client_impl->op_ctx = NULL;
    op_client_impl->callback = NULL;
    op_client_impl->completed = AXIS2_FALSE;
    
	op_client_impl->options = options;
	op_client_impl->svc_ctx = svc_ctx;
    
    op_client_impl->op_ctx = axis2_op_ctx_create(env, op, 
        op_client_impl->svc_ctx);
    if (!(op_client_impl->op_ctx))
    {
        axis2_op_client_free(&(op_client_impl->op_client), env);
		return NULL;
    }
    
	mep_uri = AXIS2_OP_GET_MSG_EXCHANGE_PATTERN(op, env);

	if (!mep_uri)
	{
		axis2_op_client_free(&(op_client_impl->op_client), env);
		return NULL;
	}

	op_client_impl->base = axis2_mep_client_create(env, svc_ctx, mep_uri);
    if (!(op_client_impl->base))
    {
        axis2_op_client_free(&(op_client_impl->op_client), env);
        return NULL;
    }

    /* initialize ops */    
    op_client_impl->op_client.ops  = 
        AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_op_client_ops_t) );
    
    if (!op_client_impl->op_client.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_CALLBACK_FREE(&(op_client_impl->op_client), env);
        return NULL;        
    }

	axis2_op_client_init_ops(&(op_client_impl->op_client));
    
	return &(op_client_impl->op_client);
}


axis2_status_t AXIS2_CALL 
axis2_op_client_set_options(struct axis2_op_client *op_client,
                    			axis2_env_t **env,
                    			axis2_options_t *options)
{
    axis2_op_client_impl_t *op_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	op_client_impl = AXIS2_INTF_TO_IMPL(op_client);
	
	if (op_client_impl->options)
	{
		AXIS2_OPTIONS_FREE(op_client_impl->options, env);
	}
	op_client_impl->options = options;

	return AXIS2_SUCCESS;	
}

axis2_options_t* AXIS2_CALL 
axis2_op_client_get_options(struct axis2_op_client *op_client,
                    			axis2_env_t **env)
{
    axis2_op_client_impl_t *op_client_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

	op_client_impl = AXIS2_INTF_TO_IMPL(op_client);
	
	return op_client_impl->options;	
}

axis2_status_t AXIS2_CALL 
axis2_op_client_add_msg_ctx(struct axis2_op_client *op_client,
                        				axis2_env_t **env,
                        				axis2_msg_ctx_t *mc)
{
    axis2_op_client_impl_t *op_client_impl = NULL;
 	axis2_msg_ctx_t *out_msg_ctx = NULL, *in_msg_ctx = NULL;
	axis2_hash_t *msg_ctx_map = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	op_client_impl = AXIS2_INTF_TO_IMPL(op_client);
	
	msg_ctx_map = AXIS2_OP_CTX_GET_MSG_CTX_MAP(op_client_impl->op_ctx, env);

	out_msg_ctx = axis2_hash_get(msg_ctx_map, AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE, 
									AXIS2_HASH_KEY_STRING);
	in_msg_ctx = axis2_hash_get(msg_ctx_map, AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE,
									AXIS2_HASH_KEY_STRING);

	if (out_msg_ctx && in_msg_ctx)
	{
		/*TODO:error - completed*/
		return AXIS2_FAILURE;
	}

	if (!out_msg_ctx)
	{
		axis2_hash_set(msg_ctx_map, AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE, AXIS2_HASH_KEY_STRING, mc);
	}
	else
	{
		axis2_hash_set(msg_ctx_map, AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE, AXIS2_HASH_KEY_STRING, mc);
		AXIS2_OP_CTX_SET_IS_COMPLETE(op_client_impl->op_ctx, env, AXIS2_TRUE);
	}
	return AXIS2_SUCCESS;
}


axis2_msg_ctx_t* AXIS2_CALL 
axis2_op_client_get_msg_ctx(struct axis2_op_client *op_client,
                            			axis2_env_t **env,
                            			axis2_char_t *message_label)
{

    axis2_op_client_impl_t *op_client_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);

	op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

	if (!message_label)
	{
        /*AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);*/
		return NULL;
	}
	return AXIS2_OP_CTX_GET_MSG_CTX(op_client_impl->op_ctx, env, message_label); 
}

axis2_status_t AXIS2_CALL 
axis2_op_client_set_callback(struct axis2_op_client *op_client,
                    			axis2_env_t **env,
                    			axis2_callback_t *callback)
{
    axis2_op_client_impl_t *op_client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

	if (op_client_impl->callback)
	{
		AXIS2_CALLBACK_FREE(op_client_impl->callback, env);
	}

	op_client_impl->callback = callback;	
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_op_client_execute(struct axis2_op_client *op_client,
                			axis2_env_t **env,
                			axis2_bool_t block)
{
    axis2_op_client_impl_t *op_client_impl = NULL;
   	axis2_conf_ctx_t *conf_ctx = NULL;
   	axis2_msg_ctx_t *msg_ctx = NULL;
	axis2_msg_info_headers_t *msg_info_headers = NULL;
	
	axis2_transport_out_desc_t *transport_out = NULL;
	axis2_transport_in_desc_t *transport_in = NULL;

	axis2_status_t status = AXIS2_FAILURE;
	axis2_op_t *op = NULL;
	axis2_char_t *msg_id = NULL;
	
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

	if (op_client_impl->completed)
	{
		/**TODO:error completed*/
		return AXIS2_FAILURE;
	}
	
	conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(op_client_impl->svc_ctx, env);
	msg_ctx = axis2_op_client_get_msg_ctx(op_client, env, 
		AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE); 

	if (!msg_ctx)
	{
		/*TODO:error msg ctx is null*/
		return AXIS2_FAILURE;
	}
	
	msg_info_headers = AXIS2_OPTIONS_GET_MSG_INFO_HEADERS(op_client_impl->options,
							env);	
	AXIS2_MSG_CTX_SET_MSG_INFO_HEADERS(msg_ctx, env, msg_info_headers);
	
	/** 
	if the transport to use for sending is not specified, try to find it
    from the URL
	*/
	transport_out = AXIS2_OPTIONS_GET_TRANSPORT_OUT(op_client_impl->options,
						env);
	if (!transport_out)
	{
		axis2_endpoint_ref_t *to_epr = NULL;
		to_epr = AXIS2_OPTIONS_GET_TO(op_client_impl->options, env);
		if (!to_epr)
			to_epr = AXIS2_MSG_CTX_GET_TO(msg_ctx, env);
		transport_out = AXIS2_MEP_CLIENT_INFER_TRANSPORT(op_client_impl->base, env, to_epr);
	}
	
	if (!transport_out)
	{
		return AXIS2_FAILURE;
	}

	if (!(AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(msg_ctx, env)))
		AXIS2_MSG_CTX_SET_TRANSPORT_OUT_DESC(msg_ctx, env, transport_out);
        

	transport_in = AXIS2_OPTIONS_GET_TRANSPORT_IN(op_client_impl->options, env);
	if (!transport_in)
	{
        axis2_conf_ctx_t *conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(op_client_impl->svc_ctx, env);
        if (conf_ctx)
        {
            axis2_conf_t *conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
            if (conf)
            {
                transport_in = AXIS2_CONF_GET_TRANSPORT_IN(conf, env,
                        AXIS2_TRANSPORT_OUT_DESC_GET_QNAME(transport_out, env));
            }
        }
    }
	
	if (!(AXIS2_MSG_CTX_GET_TRANSPORT_IN_DESC(msg_ctx, env)))
    {
        AXIS2_MSG_CTX_SET_TRANSPORT_IN_DESC(msg_ctx, env, transport_in);
    }

	/*TODO:check - soap_action*/

	/*TODO:check - add reference parametes*/	

	op = AXIS2_OP_CTX_GET_OP(op_client_impl->op_ctx, env);
	if (!op)
		return AXIS2_FAILURE;

	status = AXIS2_MEP_CLIENT_PREPARE_INVOCATION(op_client_impl->base, env, op, msg_ctx);
	if (status != AXIS2_SUCCESS)
		return AXIS2_FAILURE;
	
	msg_id = (axis2_char_t*)axis2_uuid_gen(env);
	AXIS2_MSG_CTX_SET_MESSAGE_ID(msg_ctx, env, msg_id);
    if(NULL != msg_id)
    {
		AXIS2_FREE((*env)->allocator, msg_id);
		msg_id = NULL;
	}		

	if (AXIS2_OPTIONS_IS_USE_SEPERATE_LISTENER(op_client_impl->options, env))
	{
	}
	else
	{
		if (block)
		{
			axis2_op_ctx_t *op_ctx = NULL;
        	axis2_msg_ctx_t *response_mc = NULL;
        	axis2_char_t *address = NULL;
        	axis2_char_t *epr_address = NULL;
			axis2_property_t *property = NULL;

        	/* Usual Request-Response Sync implemetation */
        	property = axis2_property_create(env);
        	AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
        	epr_address = AXIS2_ENDPOINT_REF_GET_ADDRESS(
					AXIS2_OPTIONS_GET_TO(op_client_impl->options, env), env);
        	address = AXIS2_STRDUP(epr_address, env);
        	AXIS2_PROPERTY_SET_VALUE(property, env, address);
        	AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env,
                AXIS2_TRANSPORT_URL, property, AXIS2_FALSE);
        	/*AXIS2_MSG_CTX_SET_TO(msg_ctx, env, call_impl->to);*/
        	AXIS2_MSG_CTX_SET_SVC_CTX(msg_ctx, env, op_client_impl->svc_ctx);
        	AXIS2_MSG_CTX_SET_CONF_CTX(msg_ctx, env, 
                AXIS2_SVC_CTX_GET_CONF_CTX(op_client_impl->svc_ctx, env));
            AXIS2_MSG_CTX_SET_OP_CTX(msg_ctx, env, op_client_impl->op_ctx);

			/*TODO: check this if it is necessary*/
        	/*op_ctx = axis2_op_ctx_create(env, op, op_client_impl->svc_ctx);
        	AXIS2_OP_REGISTER_OP_CTX(op, env, msg_ctx, op_ctx);*/

        	/*Send the SOAP Message and receive a response */
        	response_mc = axis2_mep_client_two_way_send(env, msg_ctx);
        	if (!response_mc)
            	return AXIS2_FAILURE;
			axis2_op_client_add_msg_ctx(&(op_client_impl->op_client), env, response_mc);	
		}
		else
		{
		}
	}
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_op_client_reset(struct axis2_op_client *op_client,
							axis2_env_t **env)
{
    axis2_op_client_impl_t *op_client_impl = NULL;

	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	
	op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

	if (!op_client_impl->completed)
		return AXIS2_FAILURE;

	op_client_impl->completed = AXIS2_FALSE;

	/*TODO:check
	if (op_client_impl->op_ctx)
		AXIS2_OP_CTX_FREE(op_client_impl->op_ctx, env);
	*/
	op_client_impl->op_ctx = NULL;

	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_op_client_compelete(struct axis2_op_client *op_client,
                			axis2_env_t **env,
                			axis2_msg_ctx_t *mc)
{
    axis2_op_client_impl_t *op_client_impl = NULL;
	axis2_conf_ctx_t *conf_ctx = NULL;
	axis2_listener_manager_t *listener_manager = NULL;
	axis2_char_t *transport = NULL;

	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	
	op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

	conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(mc, env);

	if (!conf_ctx)
		return AXIS2_FAILURE;

	/*TODO:uncomment when implemented
	listener_manager = AXIS2_CONF_CTX_GET_LISTENER_MANAGER(conf_ctx, env);
	*/
	if (!listener_manager)
		return AXIS2_FAILURE;
	/*TODO:set transport*/
	return AXIS2_LISTENER_MANAGER_STOP(listener_manager, env, transport);
}

axis2_op_ctx_t* AXIS2_CALL 
axis2_op_client_get_operation_context(struct axis2_op_client *op_client)
{
    axis2_op_client_impl_t *op_client_impl = NULL;

	op_client_impl = AXIS2_INTF_TO_IMPL(op_client);
	
	return op_client_impl->op_ctx;
}
	
axis2_status_t AXIS2_CALL
axis2_op_client_free(struct axis2_op_client *op_client,
		                        axis2_env_t **env)
{
	axis2_op_client_impl_t *op_client_impl = NULL;

	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	op_client_impl = AXIS2_INTF_TO_IMPL(op_client);

	/*TODO:free*/

	return AXIS2_SUCCESS;
}
/** private functions - implementation */

static void axis2_op_client_init_ops(axis2_op_client_t *op_client)
{
	op_client->ops->set_options = axis2_op_client_set_options;
	op_client->ops->get_options = axis2_op_client_get_options;
	op_client->ops->add_msg_ctx = axis2_op_client_add_msg_ctx;
	op_client->ops->get_msg_ctx = axis2_op_client_get_msg_ctx;
	op_client->ops->set_callback = axis2_op_client_set_callback;
	op_client->ops->execute = axis2_op_client_execute;
	op_client->ops->reset = axis2_op_client_reset;
	op_client->ops->compelete = axis2_op_client_compelete;
	op_client->ops->get_operation_context = axis2_op_client_get_operation_context;
	op_client->ops->free = axis2_op_client_free;
}

static axis2_msg_ctx_t* axis2_op_client_invoke_blocking(axis2_op_client_impl_t *op_client_impl, 
										axis2_env_t **env,
										axis2_msg_ctx_t *mc)
{
	return NULL;
}
