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

#ifndef AXIS2_QPID_SENDER_H
#define AXIS2_QPID_SENDER_H

#include <axis2_util.h>
#include <sstream>
#include <string>

using std::string;

class Axis2QpidSender
{
	public:
		Axis2QpidSender(string qpidBrokerIP, int qpidBrokerPort, const axutil_env_t* env);
		~Axis2QpidSender(void);

		bool SendReceive(string messageContent, string toQueueName, bool isSOAP11, 
				string contentType, string soapAction, axutil_array_list_t* mime_parts, int timeout);
		bool Send(string messageContent, string toQueueName, string replyToQueueName, bool isSOAP11, 
				string contentType, string soapAction, axutil_array_list_t* mime_parts);
		
		string 		 responseContent;
		string		 responseContentType;

	private:
		void GetMimeBody(axutil_array_list_t* mime_parts, string& mimeBody);

		string		  		 qpidBrokerIP;
		int			  		 qpidBrokerPort;
		const axutil_env_t*  env;
};

#endif
