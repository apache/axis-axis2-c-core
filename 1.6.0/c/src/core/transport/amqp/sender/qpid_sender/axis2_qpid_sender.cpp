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

#include <qpid/client/Connection.h>
#include <qpid/client/Session.h>
#include <qpid/client/Message.h>
#include <qpid/client/SubscriptionManager.h>
#include <qpid/sys/Time.h>
#include <axis2_amqp_defines.h>
#include <axiom_mime_part.h>
#include <axis2_qpid_sender.h>
#include <fstream>

using namespace std;
using namespace qpid::client;
using namespace qpid::framing;

Axis2QpidSender::Axis2QpidSender(string qpidBrokerIP, int qpidBrokerPort, const axutil_env_t* env)
{
	this->qpidBrokerIP = qpidBrokerIP;
	this->qpidBrokerPort = qpidBrokerPort;
	this->env = env;
	this->responseContent = "";
	this->responseContentType = "";
}


Axis2QpidSender::~Axis2QpidSender(void)
{}


bool Axis2QpidSender::SendReceive(string messageContent, string toQueueName, bool isSOAP11, 
		string contentType, string soapAction, axutil_array_list_t* mime_parts, int timeout)
{
	bool status = false;
	this->responseContent = "";
	this->responseContentType = "";
	
	try
	{
		Connection connection;
		connection.open(qpidBrokerIP, qpidBrokerPort);
		
		Session session = connection.newSession();

		/* Declare Private Queue */
		string replyToQueueName = AXIS2_AMQP_TEMP_QUEUE_NAME_PREFIX;
		replyToQueueName.append(axutil_uuid_gen(env));

		session.queueDeclare(arg::queue = replyToQueueName, arg::autoDelete = true);
		session.exchangeBind(arg::exchange = AXIS2_AMQP_EXCHANGE_DIRECT, 
							 arg::queue = replyToQueueName,
							 arg::bindingKey = replyToQueueName);

		/* Create Message */
		Message reqMessage;
		
		reqMessage.getDeliveryProperties().setRoutingKey(toQueueName);
		reqMessage.getMessageProperties().setReplyTo(ReplyTo(AXIS2_AMQP_EXCHANGE_DIRECT, replyToQueueName));

		reqMessage.getHeaders().setString(AXIS2_AMQP_HEADER_CONTENT_TYPE, contentType);
		reqMessage.getHeaders().setString(AXIS2_AMQP_HEADER_SOAP_ACTION, soapAction);

		if (mime_parts)
		{
			string mimeBody;
			GetMimeBody(mime_parts, mimeBody);
			
			messageContent.clear();/* MIME parts include SOAP envelop */
			
			messageContent.append(mimeBody);
		}

		reqMessage.setData(messageContent);
	
		async(session).messageTransfer(arg::content = reqMessage, arg::destination = AXIS2_AMQP_EXCHANGE_DIRECT);
		
		/* Create subscription manager */
		SubscriptionManager subscriptionManager(session);
		
		Message resMessage;
		qpid::sys::Duration reqTimeout(timeout * AXIS2_AMQP_NANOSEC_PER_MILLISEC);

		if (subscriptionManager.get(resMessage, replyToQueueName, reqTimeout))
		{
			responseContent = resMessage.getData();
			responseContentType = resMessage.getHeaders().getString(AXIS2_AMQP_HEADER_CONTENT_TYPE);
		
			status = true;
		}

		connection.close();
	}
	catch (const std::exception& e)
	{
	}

	return status;
}


bool Axis2QpidSender::Send(string messageContent, string toQueueName, string replyToQueueName, 
		bool isSOAP11, string contentType, string soapAction, axutil_array_list_t* mime_parts)
{
	bool status = false;

	try
	{
		Connection connection;
		connection.open(qpidBrokerIP, qpidBrokerPort);
		
		Session session = connection.newSession();

		/* Create Message */ 
		Message message;

		message.getDeliveryProperties().setRoutingKey(toQueueName);

		if (!replyToQueueName.empty()) /* Client dual-channel */
		{
			message.getMessageProperties().setReplyTo(ReplyTo(AXIS2_AMQP_EXCHANGE_DIRECT, replyToQueueName));

			session.queueDeclare(arg::queue = replyToQueueName);
			session.exchangeBind(arg::exchange = AXIS2_AMQP_EXCHANGE_DIRECT,
								 arg::queue = replyToQueueName,
							 	 arg::bindingKey = replyToQueueName);
		}
		
		message.getHeaders().setString(AXIS2_AMQP_HEADER_CONTENT_TYPE, contentType);
		message.getHeaders().setString(AXIS2_AMQP_HEADER_SOAP_ACTION, soapAction);

		if (mime_parts)
		{
			string mimeBody;
			GetMimeBody(mime_parts, mimeBody);
		
			messageContent.clear();/* MIME parts include SOAP envelop */

			messageContent.append(mimeBody);
		}

		message.setData(messageContent);

		async(session).messageTransfer(arg::content = message, arg::destination = AXIS2_AMQP_EXCHANGE_DIRECT);

		connection.close();

		status = true;
	}
	catch (const std::exception& e)
	{
	}

	return status;
}


void Axis2QpidSender::GetMimeBody(axutil_array_list_t* mime_parts, string& mimeBody)
{
	int i = 0;
	axiom_mime_part_t *mime_part = NULL;
	axis2_status_t status = AXIS2_SUCCESS;

	if (!mime_parts)
		return;

	for (i = 0; i < axutil_array_list_size(mime_parts, env); i++)
	{
		mime_part = (axiom_mime_part_t *)axutil_array_list_get(mime_parts, env, i);

		if (mime_part->type == AXIOM_MIME_PART_BUFFER)
		{
			mimeBody.append(mime_part->part, mime_part->part_size);
		}
		else if (mime_part->type == AXIOM_MIME_PART_FILE)
		{
			int length;
			char* buffer;

			ifstream file;
			file.open(mime_part->file_name, ios::binary);

			file.seekg(0, ios::end);
			length = file.tellg();
			file.seekg(0, ios::beg);

			buffer = new char[length];

			file.read(buffer, length);
			file.close();

			mimeBody.append(buffer, length);

			delete [] buffer;
		}
		else
		{
			AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unknown mime type");
			return;
		}
            
		if (status == AXIS2_FAILURE)
		{
			break;
		}
	}
}
