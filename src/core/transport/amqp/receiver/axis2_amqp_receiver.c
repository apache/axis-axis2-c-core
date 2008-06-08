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

#include <axis2_amqp_util.h>
#include <axis2_amqp_receiver.h>

static const axis2_transport_receiver_ops_t amqp_receiver_ops = {
    axis2_amqp_receiver_init,
    axis2_amqp_receiver_start,
    axis2_amqp_receiver_get_reply_to_epr,
    axis2_amqp_receiver_get_conf_ctx,
    axis2_amqp_receiver_is_running,
    axis2_amqp_receiver_stop,
    axis2_amqp_receiver_free};


axis2_transport_receiver_t* AXIS2_CALL
axis2_amqp_receiver_create (const axutil_env_t* env,
						    const axis2_char_t* repo,
              			    const axis2_char_t* qpid_broker_ip,
              			    int                 qpid_broker_port)
{
    AXIS2_ENV_CHECK (env, NULL);
    
	axis2_amqp_receiver_resource_pack_t* receiver_resource_pack = NULL;

    receiver_resource_pack = (axis2_amqp_receiver_resource_pack_t*)
							  AXIS2_MALLOC(env->allocator, 
									  	   sizeof (axis2_amqp_receiver_resource_pack_t));

    if (!receiver_resource_pack)
    {
        AXIS2_ERROR_SET (env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    receiver_resource_pack->receiver.ops = &amqp_receiver_ops;
    receiver_resource_pack->qpid_receiver = NULL;
    receiver_resource_pack->conf_ctx = NULL;
    receiver_resource_pack->conf_ctx_private = NULL;
    receiver_resource_pack->qpid_broker_ip = qpid_broker_ip ? qpid_broker_ip : 
											 AXIS2_QPID_BROKER_IP;
    receiver_resource_pack->qpid_broker_port = (qpid_broker_port != -1) ? 
											   qpid_broker_port : AXIS2_QPID_BROKER_PORT;

	if (repo)
    {
        /**
         * We first create a private conf ctx which is owned by this server
         * we only free this private conf context. We should never free the
         * receiver_impl->conf_ctx because it may be owned by any other object which
         * may lead to double free
         */
        receiver_resource_pack->conf_ctx_private = axis2_build_conf_ctx (env, repo);
        if (!receiver_resource_pack->conf_ctx_private)
        {
            axis2_amqp_receiver_free ((axis2_transport_receiver_t *)receiver_resource_pack, env);
            return NULL;
        }

        receiver_resource_pack->conf_ctx = receiver_resource_pack->conf_ctx_private;
    }

    return &(receiver_resource_pack->receiver);
}


axis2_status_t AXIS2_CALL 
axis2_amqp_receiver_init (axis2_transport_receiver_t* receiver,
						  const axutil_env_t* 		  env,
						  axis2_conf_ctx_t* 		  conf_ctx,
						  axis2_transport_in_desc_t*  in_desc)
{
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	axis2_amqp_receiver_resource_pack_t* receiver_resource_pack = NULL;

	receiver_resource_pack = AXIS2_AMQP_RECEIVER_TO_RESOURCE_PACK (receiver);
	receiver_resource_pack->conf_ctx = conf_ctx;
	receiver_resource_pack->qpid_broker_ip = axis2_amqp_util_get_conf_value_string (in_desc,
																	 		        env,
																	 		   		AXIS2_AMQP_CONF_QPID_BROKER_IP); 
	receiver_resource_pack->qpid_broker_port = axis2_amqp_util_get_conf_value_int (in_desc,
																			  	   env,
																			  	   AXIS2_AMQP_CONF_QPID_BROKER_PORT);
	
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_amqp_receiver_start (axis2_transport_receiver_t* receiver,
						   const axutil_env_t* 		   env)
{
	AXIS2_ENV_CHECK (env, AXIS2_FAILURE);

	axis2_status_t status = AXIS2_FAILURE;

	axis2_amqp_receiver_resource_pack_t* amqp_receiver_resource_pack = NULL;
	axis2_qpid_receiver_resource_pack_t* qpid_receiver_resource_pack = NULL;

	amqp_receiver_resource_pack = AXIS2_AMQP_RECEIVER_TO_RESOURCE_PACK (receiver);

	/* Create Qpid Receiver */
	qpid_receiver_resource_pack = axis2_qpid_receiver_create (env,
															  amqp_receiver_resource_pack->conf_ctx,
															  amqp_receiver_resource_pack->qpid_broker_ip,
															  amqp_receiver_resource_pack->qpid_broker_port);
	if (qpid_receiver_resource_pack)
	{
		amqp_receiver_resource_pack->qpid_receiver = qpid_receiver_resource_pack;

		status = axis2_qpid_receiver_start (qpid_receiver_resource_pack, env);
	}

	return status;
}


axis2_endpoint_ref_t* AXIS2_CALL 
axis2_amqp_receiver_get_reply_to_epr (axis2_transport_receiver_t* receiver,
									  const axutil_env_t* 		  env,
									  const axis2_char_t* 		  svc_name)
{
	return NULL;
}


axis2_conf_ctx_t* AXIS2_CALL 
axis2_amqp_receiver_get_conf_ctx (axis2_transport_receiver_t* receiver,
								  const axutil_env_t* 		  env)
{
	AXIS2_ENV_CHECK (env, NULL);

	return AXIS2_AMQP_RECEIVER_TO_RESOURCE_PACK (receiver)->conf_ctx;
}


axis2_bool_t AXIS2_CALL 
axis2_amqp_receiver_is_running (axis2_transport_receiver_t* receiver,
							    const axutil_env_t* 		env)
{
	return AXIS2_TRUE;
}


axis2_status_t AXIS2_CALL 
axis2_amqp_receiver_stop (axis2_transport_receiver_t* receiver,
						  const axutil_env_t* 		  env)
{
	return AXIS2_SUCCESS;
}


void AXIS2_CALL 
axis2_amqp_receiver_free (axis2_transport_receiver_t* receiver,
						  const axutil_env_t* 		  env)
{
	AXIS2_ENV_CHECK (env, void);

	axis2_amqp_receiver_resource_pack_t* receiver_resource_pack = NULL;
    receiver_resource_pack = AXIS2_AMQP_RECEIVER_TO_RESOURCE_PACK (receiver);
    
	if (receiver_resource_pack->qpid_receiver)
    {
        axis2_qpid_receiver_free (receiver_resource_pack->qpid_receiver, env);
        receiver_resource_pack->qpid_receiver = NULL;
    }

    if (receiver_resource_pack->conf_ctx_private)
    {
        axis2_conf_ctx_free (receiver_resource_pack->conf_ctx_private, env);
        receiver_resource_pack->conf_ctx_private = NULL;
    }

    receiver_resource_pack->conf_ctx = NULL; /* Do not free this. It may be owned by some other object */
    
	AXIS2_FREE (env->allocator, receiver_resource_pack);
}


/* Library Exports */

AXIS2_EXPORT int
#ifndef AXIS2_STATIC_DEPLOY
axis2_get_instance (
#else
axis2_amqp_receiver_get_instance (
#endif
	struct axis2_transport_receiver** inst,
	const axutil_env_t*				  env)
{
	int status = AXIS2_SUCCESS;

	*inst = axis2_amqp_receiver_create (env, NULL, NULL, -1);
	if (!(*inst))
	{
		status = AXIS2_FAILURE;
	}

	return status;
}


AXIS2_EXPORT int
#ifndef AXIS2_STATIC_DEPLOY
axis2_remove_instance (
#else
axis2_amqp_receiver_remove_instance (
#endif
	axis2_transport_receiver_t* inst,
	const axutil_env_t* 		env)
{
	if (inst)
	{
		axis2_transport_receiver_free (inst, env);
	}
	
	return AXIS2_SUCCESS;
}

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

#include <axis2_amqp_util.h>
#include <axis2_amqp_receiver.h>

static const axis2_transport_receiver_ops_t amqp_receiver_ops = {
    axis2_amqp_receiver_init,
    axis2_amqp_receiver_start,
    axis2_amqp_receiver_get_reply_to_epr,
    axis2_amqp_receiver_get_conf_ctx,
    axis2_amqp_receiver_is_running,
    axis2_amqp_receiver_stop,
    axis2_amqp_receiver_free};


axis2_transport_receiver_t* AXIS2_CALL
axis2_amqp_receiver_create (const axutil_env_t* env,
						    const axis2_char_t* repo,
              			    const axis2_char_t* qpid_broker_ip,
              			    int                 qpid_broker_port)
{
    AXIS2_ENV_CHECK (env, NULL);
    
	axis2_amqp_receiver_resource_pack_t* receiver_resource_pack = NULL;

    receiver_resource_pack = (axis2_amqp_receiver_resource_pack_t*)
							  AXIS2_MALLOC(env->allocator, 
									  	   sizeof (axis2_amqp_receiver_resource_pack_t));

    if (!receiver_resource_pack)
    {
        AXIS2_ERROR_SET (env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    receiver_resource_pack->receiver.ops = &amqp_receiver_ops;
    receiver_resource_pack->qpid_receiver = NULL;
    receiver_resource_pack->conf_ctx = NULL;
    receiver_resource_pack->conf_ctx_private = NULL;
    receiver_resource_pack->qpid_broker_ip = qpid_broker_ip ? qpid_broker_ip : 
											 AXIS2_QPID_BROKER_IP;
    receiver_resource_pack->qpid_broker_port = (qpid_broker_port != -1) ? 
											   qpid_broker_port : AXIS2_QPID_BROKER_PORT;

	if (repo)
    {
        /**
         * We first create a private conf ctx which is owned by this server
         * we only free this private conf context. We should never free the
         * receiver_impl->conf_ctx because it may be owned by any other object which
         * may lead to double free
         */
        receiver_resource_pack->conf_ctx_private = axis2_build_conf_ctx (env, repo);
        if (!receiver_resource_pack->conf_ctx_private)
        {
            axis2_amqp_receiver_free ((axis2_transport_receiver_t *)receiver_resource_pack, env);
            return NULL;
        }

        receiver_resource_pack->conf_ctx = receiver_resource_pack->conf_ctx_private;
    }

    return &(receiver_resource_pack->receiver);
}


axis2_status_t AXIS2_CALL 
axis2_amqp_receiver_init (axis2_transport_receiver_t* receiver,
						  const axutil_env_t* 		  env,
						  axis2_conf_ctx_t* 		  conf_ctx,
						  axis2_transport_in_desc_t*  in_desc)
{
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	axis2_amqp_receiver_resource_pack_t* receiver_resource_pack = NULL;

	receiver_resource_pack = AXIS2_AMQP_RECEIVER_TO_RESOURCE_PACK (receiver);
	receiver_resource_pack->conf_ctx = conf_ctx;
	receiver_resource_pack->qpid_broker_ip = axis2_amqp_util_get_conf_value_string (in_desc,
																	 		        env,
																	 		   		AXIS2_AMQP_CONF_QPID_BROKER_IP); 
	receiver_resource_pack->qpid_broker_port = axis2_amqp_util_get_conf_value_int (in_desc,
																			  	   env,
																			  	   AXIS2_AMQP_CONF_QPID_BROKER_PORT);
	
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_amqp_receiver_start (axis2_transport_receiver_t* receiver,
						   const axutil_env_t* 		   env)
{
	AXIS2_ENV_CHECK (env, AXIS2_FAILURE);

	axis2_status_t status = AXIS2_FAILURE;

	axis2_amqp_receiver_resource_pack_t* amqp_receiver_resource_pack = NULL;
	axis2_qpid_receiver_resource_pack_t* qpid_receiver_resource_pack = NULL;

	amqp_receiver_resource_pack = AXIS2_AMQP_RECEIVER_TO_RESOURCE_PACK (receiver);

	/* Create Qpid Receiver */
	qpid_receiver_resource_pack = axis2_qpid_receiver_create (env,
															  amqp_receiver_resource_pack->conf_ctx,
															  amqp_receiver_resource_pack->qpid_broker_ip,
															  amqp_receiver_resource_pack->qpid_broker_port);
	if (qpid_receiver_resource_pack)
	{
		amqp_receiver_resource_pack->qpid_receiver = qpid_receiver_resource_pack;

		status = axis2_qpid_receiver_start (qpid_receiver_resource_pack, env);
	}

	return status;
}


axis2_endpoint_ref_t* AXIS2_CALL 
axis2_amqp_receiver_get_reply_to_epr (axis2_transport_receiver_t* receiver,
									  const axutil_env_t* 		  env,
									  const axis2_char_t* 		  svc_name)
{
	return NULL;
}


axis2_conf_ctx_t* AXIS2_CALL 
axis2_amqp_receiver_get_conf_ctx (axis2_transport_receiver_t* receiver,
								  const axutil_env_t* 		  env)
{
	AXIS2_ENV_CHECK (env, NULL);

	return AXIS2_AMQP_RECEIVER_TO_RESOURCE_PACK (receiver)->conf_ctx;
}


axis2_bool_t AXIS2_CALL 
axis2_amqp_receiver_is_running (axis2_transport_receiver_t* receiver,
							    const axutil_env_t* 		env)
{
	return AXIS2_TRUE;
}


axis2_status_t AXIS2_CALL 
axis2_amqp_receiver_stop (axis2_transport_receiver_t* receiver,
						  const axutil_env_t* 		  env)
{
	return AXIS2_SUCCESS;
}


void AXIS2_CALL 
axis2_amqp_receiver_free (axis2_transport_receiver_t* receiver,
						  const axutil_env_t* 		  env)
{
	AXIS2_ENV_CHECK (env, void);

	axis2_amqp_receiver_resource_pack_t* receiver_resource_pack = NULL;
    receiver_resource_pack = AXIS2_AMQP_RECEIVER_TO_RESOURCE_PACK (receiver);
    
	if (receiver_resource_pack->qpid_receiver)
    {
        axis2_qpid_receiver_free (receiver_resource_pack->qpid_receiver, env);
        receiver_resource_pack->qpid_receiver = NULL;
    }

    if (receiver_resource_pack->conf_ctx_private)
    {
        axis2_conf_ctx_free (receiver_resource_pack->conf_ctx_private, env);
        receiver_resource_pack->conf_ctx_private = NULL;
    }

    receiver_resource_pack->conf_ctx = NULL; /* Do not free this. It may be owned by some other object */
    
	AXIS2_FREE (env->allocator, receiver_resource_pack);
}


/* Library Exports */

AXIS2_EXPORT int
#ifndef AXIS2_STATIC_DEPLOY
axis2_get_instance (
#else
axis2_amqp_receiver_get_instance (
#endif
	struct axis2_transport_receiver** inst,
	const axutil_env_t*				  env)
{
	int status = AXIS2_SUCCESS;

	*inst = axis2_amqp_receiver_create (env, NULL, NULL, -1);
	if (!(*inst))
	{
		status = AXIS2_FAILURE;
	}

	return status;
}


AXIS2_EXPORT int
#ifndef AXIS2_STATIC_DEPLOY
axis2_remove_instance (
#else
axis2_amqp_receiver_remove_instance (
#endif
	axis2_transport_receiver_t* inst,
	const axutil_env_t* 		env)
{
	if (inst)
	{
		axis2_transport_receiver_free (inst, env);
	}
	
	return AXIS2_SUCCESS;
}


