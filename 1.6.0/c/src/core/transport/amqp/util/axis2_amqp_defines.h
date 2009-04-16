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

#ifndef AXIS2_AMQP_DEFINES_H
#define AXIS2_AMQP_DEFINES_H

#include <axiom_mime_const.h>

#define AXIS2_AMQP_EXCHANGE_DIRECT			 		 "amq.direct"

#define AXIS2_AMQP_CONF_QPID_BROKER_IP				 "qpid_broker_ip"
#define AXIS2_AMQP_CONF_QPID_BROKER_PORT			 "qpid_broker_port"
#define AXIS2_AMQP_CONF_QPID_REQUEST_TIMEOUT		 "request_timeout"

#define AXIS2_QPID_DEFAULT_BROKER_IP    			 "127.0.0.1"
#define AXIS2_QPID_DEFAULT_BROKER_PORT  			 5672
#define AXIS2_QPID_DEFAULT_REQUEST_TIMEOUT  		 500
#define AXIS2_QPID_NULL_CONF_INT	  			 	 -1

#define AXIS2_AMQP_CONF_CTX_PROPERTY_BROKER_IP		 "qpid_broker_ip"
#define AXIS2_AMQP_CONF_CTX_PROPERTY_BROKER_PORT	 "qpid_broker_port"
#define AXIS2_AMQP_CONF_CTX_PROPERTY_REQUEST_TIMEOUT "request_timeout"
#define AXIS2_AMQP_CONF_CTX_PROPERTY_QUEUE_NAME		 "queue_name"

#define AXIS2_AMQP_MSG_CTX_PROPERTY_REPLY_TO		 "qpid_reply_to"

#define AXIS2_AMQP_HEADER_ACCEPT_TEXT_XML 			 "text/xml"
#define AXIS2_AMQP_HEADER_ACCEPT_APPL_SOAP 			 "application/soap+xml"
#define AXIS2_AMQP_HEADER_ACCEPT_MULTIPART_RELATED 	 AXIOM_MIME_TYPE_MULTIPART_RELATED
#define AXIS2_AMQP_HEADER_CONTENT_TYPE_MIME_BOUNDARY "boundary"
#define AXIS2_AMQP_HEADER_SOAP_ACTION 			     "SOAPAction"
#define AXIS2_AMQP_HEADER_CONTENT_TYPE 			     "Content-Type"

#define AXIS2_AMQP_TEMP_QUEUE_NAME_PREFIX 			 "TempQueue"

#define AXIS2_AMQP_SERVER_LOG_FILE_NAME 			 "axis2_amqp_server.log"
#define AXIS2_AMQP_SERVER_REPO_PATH     			 "../"

#define AXIS2_AMQP_EPR_PREFIX						 "amqp:"
#define AXIS2_AMQP_EPR_SERVICE_PREFIX				 "services"
#define AXIS2_AMQP_EPR_ANON_SERVICE_NAME			 "__ANONYMOUS_SERVICE__"

#define AXIS2_AMQP_EQ 								 '='
#define AXIS2_AMQP_SEMI_COLON 						 ';'
#define AXIS2_AMQP_ESC_NULL 						 '\0'
#define AXIS2_AMQP_DOUBLE_QUOTE 					 '"'
#define AXIS2_AMQP_B_SLASH 							 '\\'

#define AXIS2_AMQP_NANOSEC_PER_MILLISEC				 1000*1000

#endif
