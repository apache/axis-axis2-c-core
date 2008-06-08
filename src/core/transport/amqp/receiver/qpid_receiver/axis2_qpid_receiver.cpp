/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      tcp://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2_amqp_request_processor.h>
#include <axis2_amqp_defines.h>
#include <axis2_qpid_receiver.h>

#define AXIS2_AMQP_RECEIVER_QUEUE_NAME	"axis2.amqp.receiver.queue"

Axis2QpidReceiver::Axis2QpidReceiver (const axutil_env_t* env,
									  axis2_conf_ctx_t*   conf_ctx,
									  string 			  qpidBrokerIP,
									  int 				  qpidBrokerPort)
{
	this->env = env;
	this->conf_ctx = conf_ctx;
	this->qpidBrokerIP = qpidBrokerIP;
	this->qpidBrokerPort = qpidBrokerPort;
	this->dispatcher = NULL;
}


Axis2QpidReceiver::~Axis2QpidReceiver (void)
{
	if (dispatcher)
		delete dispatcher;
}


bool Axis2QpidReceiver::start (void)
{
	bool status = false;

	while (true)
	{
		try
		{
			/* Create Connection to Qpid Broker */
			std::cout << "Connecting to Qpid Broker on " << qpidBrokerIP
					  << ":" << qpidBrokerPort << " ... ";
			
			connection.open (qpidBrokerIP, qpidBrokerPort);

			session = connection.newSession ();

			string request_queue = AXIS2_AMQP_RECEIVER_QUEUE_NAME;

			session.queueDeclare (arg::queue = request_queue);
			session.exchangeBind (arg::exchange = AXIS2_AMQP_EXCHANGE_DIRECT,
								  arg::queue = request_queue,
								  arg::bindingKey = AXIS2_AMQP_RECEIVER_QUEUE_BIND_KEY);

			/* Create Dispatcher */
			dispatcher = new Dispatcher (session);

			status = true;

			std::cout << "CONNECTED" << std::endl;

			break;
		}
		catch (const std::exception& e)
		{
			std::cout << "FAILED" << std::endl;
			connection.close ();

			sleep (5);
		}
	}

	/* Listen and Wait */
	if (status)
	{
		std::cout << "Started Axis2 AMQP Server ..." << std::endl;

		listen ();
		wait ();
	}

	return status;
}


bool Axis2QpidReceiver::shutdown (void)
{
	connection.close ();

	return true;
}


void Axis2QpidReceiver::received (Message& message)
{
	AXIS2_ENV_CHECK (env, void);

	axis2_amqp_request_processor_resource_pack_t* request_data = NULL;
#ifdef AXIS2_SVR_MULTI_THREADED
	axutil_thread_t* worker_thread = NULL;
#endif

	request_data = (axis2_amqp_request_processor_resource_pack_t*)
					AXIS2_MALLOC (env->allocator, 
								  sizeof (axis2_amqp_request_processor_resource_pack_t));

	if (!request_data)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Memory Allocation Error");
		return;
	}

	request_data->env = (axutil_env_t*)env;
	request_data->conf_ctx = conf_ctx;

	/* Create a Local Copy of Request Content */
	std::string message_data = message.getData ();
	axis2_char_t* request_content = (axis2_char_t*)AXIS2_MALLOC (env->allocator,
																 message_data.size () + 1);
	strcpy (request_content, message_data.c_str ());

	request_data->request_content = request_content;

	/* Set ReplyTo */
	request_data->reply_to = NULL;
	if (message.getMessageProperties ().hasReplyTo ())
	{
		/* Create a Local Copy of ReplyTo */
		std::string reply_to_tmp = message.getMessageProperties ().getReplyTo ().getRoutingKey ();
		axis2_char_t* reply_to = (axis2_char_t*)AXIS2_MALLOC (env->allocator,
															  reply_to_tmp.size () + 1);
		strcpy (reply_to, reply_to_tmp.c_str ());

		request_data->reply_to = reply_to;
	}

	/* Set Qpid Broker IP/Port */
	request_data->qpid_broker_ip = (axis2_char_t*)qpidBrokerIP.c_str ();
	request_data->qpid_broker_port = qpidBrokerPort;

#ifdef AXIS2_SVR_MULTI_THREADED
	worker_thread = axutil_thread_pool_get_thread (env->thread_pool,
												   axis2_amqp_request_processor_thread_function,
												   (void*)request_data);

	if (!worker_thread)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Failed to Create Thread");
		return;
	}

	axutil_thread_pool_thread_detach (env->thread_pool, worker_thread);
#else
	axis2_amqp_request_processor_thread_function (NULL, (void*)request_data);
#endif
}


void Axis2QpidReceiver::listen (void)
{
	if (!dispatcher)
		return;

	string request_queue = AXIS2_AMQP_RECEIVER_QUEUE_NAME;

	session.messageSubscribe (arg::queue = request_queue, arg::destination = request_queue);

	session.messageFlow (arg::destination = request_queue, arg::unit = MESSAGE_CREDIT, arg::value = 1);
	session.messageFlow (arg::destination = request_queue, arg::unit = BYTE_CREDIT, arg::value = UNLIMITED_CREDIT);

	dispatcher->listen (request_queue, this);
}


void Axis2QpidReceiver::wait (void)
{
	if (!dispatcher)
		return;

	dispatcher->run ();
}
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      tcp://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2_amqp_request_processor.h>
#include <axis2_amqp_defines.h>
#include <axis2_qpid_receiver.h>

#define AXIS2_AMQP_RECEIVER_QUEUE_NAME	"axis2.amqp.receiver.queue"

Axis2QpidReceiver::Axis2QpidReceiver (const axutil_env_t* env,
									  axis2_conf_ctx_t*   conf_ctx,
									  string 			  qpidBrokerIP,
									  int 				  qpidBrokerPort)
{
	this->env = env;
	this->conf_ctx = conf_ctx;
	this->qpidBrokerIP = qpidBrokerIP;
	this->qpidBrokerPort = qpidBrokerPort;
	this->dispatcher = NULL;
}


Axis2QpidReceiver::~Axis2QpidReceiver (void)
{
	if (dispatcher)
		delete dispatcher;
}


bool Axis2QpidReceiver::start (void)
{
	bool status = false;

	while (true)
	{
		try
		{
			/* Create Connection to Qpid Broker */
			std::cout << "Connecting to Qpid Broker on " << qpidBrokerIP
					  << ":" << qpidBrokerPort << " ... ";
			
			connection.open (qpidBrokerIP, qpidBrokerPort);

			session = connection.newSession ();

			string request_queue = AXIS2_AMQP_RECEIVER_QUEUE_NAME;

			session.queueDeclare (arg::queue = request_queue);
			session.exchangeBind (arg::exchange = AXIS2_AMQP_EXCHANGE_DIRECT,
								  arg::queue = request_queue,
								  arg::bindingKey = AXIS2_AMQP_RECEIVER_QUEUE_BIND_KEY);

			/* Create Dispatcher */
			dispatcher = new Dispatcher (session);

			status = true;

			std::cout << "CONNECTED" << std::endl;

			break;
		}
		catch (const std::exception& e)
		{
			std::cout << "FAILED" << std::endl;
			connection.close ();

			sleep (5);
		}
	}

	/* Listen and Wait */
	if (status)
	{
		std::cout << "Started Axis2 AMQP Server ..." << std::endl;

		listen ();
		wait ();
	}

	return status;
}


bool Axis2QpidReceiver::shutdown (void)
{
	connection.close ();

	return true;
}


void Axis2QpidReceiver::received (Message& message)
{
	AXIS2_ENV_CHECK (env, void);

	axis2_amqp_request_processor_resource_pack_t* request_data = NULL;
#ifdef AXIS2_SVR_MULTI_THREADED
	axutil_thread_t* worker_thread = NULL;
#endif

	request_data = (axis2_amqp_request_processor_resource_pack_t*)
					AXIS2_MALLOC (env->allocator, 
								  sizeof (axis2_amqp_request_processor_resource_pack_t));

	if (!request_data)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Memory Allocation Error");
		return;
	}

	request_data->env = (axutil_env_t*)env;
	request_data->conf_ctx = conf_ctx;

	/* Create a Local Copy of Request Content */
	std::string message_data = message.getData ();
	axis2_char_t* request_content = (axis2_char_t*)AXIS2_MALLOC (env->allocator,
																 message_data.size () + 1);
	strcpy (request_content, message_data.c_str ());

	request_data->request_content = request_content;

	/* Set ReplyTo */
	request_data->reply_to = NULL;
	if (message.getMessageProperties ().hasReplyTo ())
	{
		/* Create a Local Copy of ReplyTo */
		std::string reply_to_tmp = message.getMessageProperties ().getReplyTo ().getRoutingKey ();
		axis2_char_t* reply_to = (axis2_char_t*)AXIS2_MALLOC (env->allocator,
															  reply_to_tmp.size () + 1);
		strcpy (reply_to, reply_to_tmp.c_str ());

		request_data->reply_to = reply_to;
	}

	/* Set Qpid Broker IP/Port */
	request_data->qpid_broker_ip = (axis2_char_t*)qpidBrokerIP.c_str ();
	request_data->qpid_broker_port = qpidBrokerPort;

#ifdef AXIS2_SVR_MULTI_THREADED
	worker_thread = axutil_thread_pool_get_thread (env->thread_pool,
												   axis2_amqp_request_processor_thread_function,
												   (void*)request_data);

	if (!worker_thread)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Failed to Create Thread");
		return;
	}

	axutil_thread_pool_thread_detach (env->thread_pool, worker_thread);
#else
	axis2_amqp_request_processor_thread_function (NULL, (void*)request_data);
#endif
}


void Axis2QpidReceiver::listen (void)
{
	if (!dispatcher)
		return;

	string request_queue = AXIS2_AMQP_RECEIVER_QUEUE_NAME;

	session.messageSubscribe (arg::queue = request_queue, arg::destination = request_queue);

	session.messageFlow (arg::destination = request_queue, arg::unit = MESSAGE_CREDIT, arg::value = 1);
	session.messageFlow (arg::destination = request_queue, arg::unit = BYTE_CREDIT, arg::value = UNLIMITED_CREDIT);

	dispatcher->listen (request_queue, this);
}


void Axis2QpidReceiver::wait (void)
{
	if (!dispatcher)
		return;

	dispatcher->run ();
}
