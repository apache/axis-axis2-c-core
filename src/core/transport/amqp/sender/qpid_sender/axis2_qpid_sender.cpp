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

#include <axis2_amqp_defines.h>
#include <axis2_qpid_sender.h>

Axis2QpidSender::Axis2QpidSender (string qpidBrokerIP,
								  int 	 qpidBrokerPort)
{
	this->qpidBrokerIP = qpidBrokerIP;
	this->qpidBrokerPort = qpidBrokerPort;
	this->dispatcher = NULL;
	this->responseContent = "";
	this->responseQueue.clear ();
}


Axis2QpidSender::~Axis2QpidSender (void)
{
	if (dispatcher)
		delete dispatcher;
}


bool Axis2QpidSender::CreateSession (void)
{
	bool status = false;

	try
	{
		/* Create Connection to Qpid Broker */
		connection.open (qpidBrokerIP, qpidBrokerPort);

		session = connection.newSession ();

		status = true;
	}
	catch (const std::exception& e)
	{
		connection.close ();
	}

	return status;
}


string Axis2QpidSender::Request (string messageContent)
{
	responseContent = "";
		
	bool sessionCreated = CreateSession ();
	if (sessionCreated)
	{
		/* Declare Private Queue */
		responseQueue << "client" << session.getId ().getName ();

		session.queueDeclare (arg::queue = responseQueue.str ());
		session.exchangeBind (arg::exchange = AXIS2_AMQP_EXCHANGE_DIRECT, 
							  arg::queue = responseQueue.str (),
							  arg::bindingKey = responseQueue.str ());

		dispatcher = new Dispatcher (session);
		
		listen ();

		/* Create Message */
		Message message;
		message.getDeliveryProperties ().setRoutingKey (AXIS2_AMQP_RECEIVER_QUEUE_BIND_KEY);
		message.getMessageProperties ().setReplyTo (ReplyTo (AXIS2_AMQP_EXCHANGE_DIRECT, responseQueue.str ()));
		message.setData (messageContent);

		async (session).messageTransfer (arg::content = message, arg::destination = AXIS2_AMQP_EXCHANGE_DIRECT);

		wait ();

		connection.close ();
	}

	return responseContent;
}


void Axis2QpidSender::Send (string messageContent, string to)
{
	bool sessionCreated = CreateSession ();
	if (sessionCreated)
	{
		Message message;

		message.getDeliveryProperties ().setRoutingKey (to);
		message.setData (messageContent);

		async(session).messageTransfer(arg::content = message, arg::destination = AXIS2_AMQP_EXCHANGE_DIRECT);

		connection.close ();
	}
}


void Axis2QpidSender::Send (string messageContent)
{
	bool sessionCreated = CreateSession ();
	if (sessionCreated)
	{
		Message message;

		message.setData (messageContent);

		async(session).messageTransfer(arg::content = message, arg::destination = AXIS2_AMQP_EXCHANGE_DIRECT);

		connection.close ();
	}
}


void Axis2QpidSender::received (Message& message)
{
	responseContent = message.getData ();

	dispatcher->stop ();
}


void Axis2QpidSender::listen (void)
{
	if (!dispatcher)
		return;

	session.messageSubscribe (arg::queue = responseQueue.str (), arg::destination = responseQueue.str ());

	session.messageFlow (arg::destination = responseQueue.str (), arg::unit = MESSAGE_CREDIT, arg::value = 1);
	session.messageFlow (arg::destination = responseQueue.str (), arg::unit = BYTE_CREDIT, arg::value = UNLIMITED_CREDIT);

	dispatcher->listen (responseQueue.str (), this);
}


void Axis2QpidSender::wait (void)
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

#include <axis2_amqp_defines.h>
#include <axis2_qpid_sender.h>

Axis2QpidSender::Axis2QpidSender (string qpidBrokerIP,
								  int 	 qpidBrokerPort)
{
	this->qpidBrokerIP = qpidBrokerIP;
	this->qpidBrokerPort = qpidBrokerPort;
	this->dispatcher = NULL;
	this->responseContent = "";
	this->responseQueue.clear ();
}


Axis2QpidSender::~Axis2QpidSender (void)
{
	if (dispatcher)
		delete dispatcher;
}


bool Axis2QpidSender::CreateSession (void)
{
	bool status = false;

	try
	{
		/* Create Connection to Qpid Broker */
		connection.open (qpidBrokerIP, qpidBrokerPort);

		session = connection.newSession ();

		status = true;
	}
	catch (const std::exception& e)
	{
		connection.close ();
	}

	return status;
}


string Axis2QpidSender::Request (string messageContent)
{
	responseContent = "";
		
	bool sessionCreated = CreateSession ();
	if (sessionCreated)
	{
		/* Declare Private Queue */
		responseQueue << "client" << session.getId ().getName ();

		session.queueDeclare (arg::queue = responseQueue.str ());
		session.exchangeBind (arg::exchange = AXIS2_AMQP_EXCHANGE_DIRECT, 
							  arg::queue = responseQueue.str (),
							  arg::bindingKey = responseQueue.str ());

		dispatcher = new Dispatcher (session);
		
		listen ();

		/* Create Message */
		Message message;
		message.getDeliveryProperties ().setRoutingKey (AXIS2_AMQP_RECEIVER_QUEUE_BIND_KEY);
		message.getMessageProperties ().setReplyTo (ReplyTo (AXIS2_AMQP_EXCHANGE_DIRECT, responseQueue.str ()));
		message.setData (messageContent);

		async (session).messageTransfer (arg::content = message, arg::destination = AXIS2_AMQP_EXCHANGE_DIRECT);

		wait ();

		connection.close ();
	}

	return responseContent;
}


void Axis2QpidSender::Send (string messageContent, string to)
{
	bool sessionCreated = CreateSession ();
	if (sessionCreated)
	{
		Message message;

		message.getDeliveryProperties ().setRoutingKey (to);
		message.setData (messageContent);

		async(session).messageTransfer(arg::content = message, arg::destination = AXIS2_AMQP_EXCHANGE_DIRECT);

		connection.close ();
	}
}


void Axis2QpidSender::Send (string messageContent)
{
	bool sessionCreated = CreateSession ();
	if (sessionCreated)
	{
		Message message;

		message.setData (messageContent);

		async(session).messageTransfer(arg::content = message, arg::destination = AXIS2_AMQP_EXCHANGE_DIRECT);

		connection.close ();
	}
}


void Axis2QpidSender::received (Message& message)
{
	responseContent = message.getData ();

	dispatcher->stop ();
}


void Axis2QpidSender::listen (void)
{
	if (!dispatcher)
		return;

	session.messageSubscribe (arg::queue = responseQueue.str (), arg::destination = responseQueue.str ());

	session.messageFlow (arg::destination = responseQueue.str (), arg::unit = MESSAGE_CREDIT, arg::value = 1);
	session.messageFlow (arg::destination = responseQueue.str (), arg::unit = BYTE_CREDIT, arg::value = UNLIMITED_CREDIT);

	dispatcher->listen (responseQueue.str (), this);
}


void Axis2QpidSender::wait (void)
{
	if (!dispatcher)
		return;

	dispatcher->run ();
}
