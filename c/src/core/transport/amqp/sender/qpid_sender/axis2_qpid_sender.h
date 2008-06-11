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

#include <qpid/client/Connection.h>
#include <qpid/client/Dispatcher.h>
#include <qpid/client/Session.h>
#include <qpid/client/Message.h>
#include <qpid/client/MessageListener.h>

#include <sstream>
#include <string>

using namespace qpid::client;
using namespace qpid::framing;
using std::stringstream;
using std::string;

class Axis2QpidSender : public MessageListener
{
	public:
		Axis2QpidSender  (string qpidBrokerIP,
						  int    qpidBrokerPort);
		~Axis2QpidSender (void);

		/* Out-In */
		string 		 Request (string messageContent);			 /* Used by Clients */
		
		/* Out Only */
		void 		 Send	 (string messageContent, string to); /* Used by Server */
		void 		 Send	 (string messageContent); 			 /* Used by Clients */

	private:
		bool	CreateSession  (void);

		virtual void received  (Message& message);
		virtual void listen	   (void);
		virtual void wait	   (void);
    
		string		  	qpidBrokerIP;
		int			  	qpidBrokerPort;
		Connection    	connection;
		Dispatcher*		dispatcher;
		Session		  	session;
		string 			responseContent;
		stringstream 	responseQueue;
};

#endif
