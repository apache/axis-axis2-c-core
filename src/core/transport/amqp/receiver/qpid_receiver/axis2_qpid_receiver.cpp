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
#include <qpid/client/SubscriptionManager.h>
#include <axis2_amqp_request_processor.h>
#include <axis2_amqp_defines.h>
#include <axis2_amqp_util.h>
#include <axis2_qpid_receiver_listener.h>
#include <axis2_qpid_receiver.h>
#include <list>

Axis2QpidReceiver::Axis2QpidReceiver(const axutil_env_t* env,
									 axis2_conf_ctx_t* conf_ctx)
{
	this->env = env;
	this->conf_ctx = conf_ctx;
}


Axis2QpidReceiver::~Axis2QpidReceiver(void)
{}


bool Axis2QpidReceiver::start(void)
{
	if (!conf_ctx)
		return false;
	
	Connection connection;
	axis2_bool_t serverSide = AXIS2_TRUE;
	
	serverSide = axis2_amqp_util_conf_ctx_get_server_side(conf_ctx, env);

	while (true)
	{
		try
		{
			std::list<string> queueNameList;
			string qpidBrokerIP = axis2_amqp_util_conf_ctx_get_qpid_broker_ip(
									conf_ctx, env);
			int qpidBrokerPort = axis2_amqp_util_conf_ctx_get_qpid_broker_port(
									conf_ctx, env);
			
			/* Check if Client Side and Resolve Dynamic Queue Name */
			if (serverSide == AXIS2_TRUE) /* Server side */
			{
				std::cout << "Connecting to Qpid Broker on " << qpidBrokerIP 
						  << ":" << qpidBrokerPort << " ... ";
			}

			/* Create Connection to Qpid Broker */
			connection.open(qpidBrokerIP, qpidBrokerPort);
			
			if (serverSide == AXIS2_TRUE) /* Server side */
			{
				/* Create queue for each service. Queue name is equal to service name */
				axis2_conf_t* conf = axis2_conf_ctx_get_conf(conf_ctx, env);
				if (!conf)
					return false;
				
				axutil_hash_t* serviceMap = axis2_conf_get_all_svcs(conf, env);
				if (!serviceMap)
					return false;
					
				axutil_hash_index_t* serviceHI = NULL;
				void* serviceValue = NULL;
				
				for (serviceHI = axutil_hash_first(serviceMap, env);
					 serviceHI; serviceHI = axutil_hash_next(env, serviceHI))
				{
					axutil_hash_this(serviceHI, NULL, NULL, &serviceValue);
					
					axis2_svc_t* service = (axis2_svc_t*)serviceValue;
					if (!service)
						return false;
					
					axis2_char_t* serviceName = axutil_qname_get_localpart(
							axis2_svc_get_qname(service, env), env);
					if (!serviceName)
						return false;
					
					queueNameList.push_back(serviceName);
				}

				std::cout << "CONNECTED" << std::endl;
			}
			else /* Client side separate listener in dual-channel case */
			{
				string queueName = axis2_amqp_util_conf_ctx_get_dual_channel_queue_name(
									conf_ctx, env);

				queueNameList.push_back(queueName);
			}
			
			/* Create new session */
			Session session = connection.newSession();

			/* Create Subscription manager */
			SubscriptionManager subscriptionManager(session);
			
			Axis2QpidReceiverListener qpidReceiverListener(env, conf_ctx);

			/* Subscribe to queues */
			while (!queueNameList.empty())
			{
				string queueName = queueNameList.front();

				session.queueDeclare(arg::queue = queueName, arg::autoDelete = true);
				session.exchangeBind(arg::exchange = AXIS2_AMQP_EXCHANGE_DIRECT,
									 arg::queue = queueName,
									 arg::bindingKey = queueName);
				
				subscriptionManager.subscribe(qpidReceiverListener, queueName);

				queueNameList.pop_front();
			}
	
			/* Listen and Wait */
			if (serverSide == AXIS2_TRUE) /* Server side */
			{
				std::cout << "Started Axis2 AMQP Server ..." << std::endl;
			}
			
			subscriptionManager.run();

			return true;
		}
		catch (const std::exception& e)
		{
			connection.close();

			if (serverSide == AXIS2_TRUE) /* Server side */
			{
				std::cout << "FAILED" << std::endl;
			}

			sleep(5);
		}
	}

	connection.close();

	return false;
}


bool Axis2QpidReceiver::shutdown(void)
{
	return true;
}
