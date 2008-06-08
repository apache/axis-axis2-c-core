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

#ifndef AXIS2_AMQP_UTIL_H
#define AXIS2_AMQP_UTIL_H

#include <axis2_transport_in_desc.h>
#include <axutil_param_container.h>
#include <axiom_soap.h>
#include <axis2_conf_ctx.h>
#include <axutil_url.h>

axis2_char_t* AXIS2_CALL
axis2_amqp_util_get_conf_value_string (axis2_transport_in_desc_t* in_desc,
						  	      	   const axutil_env_t* 	   	  env,
						               const axis2_char_t*		  param_name);

int AXIS2_CALL
axis2_amqp_util_get_conf_value_int (axis2_transport_in_desc_t* in_desc,
						       		const axutil_env_t* 	   env,
						       		const axis2_char_t*		   param_name);

axiom_soap_envelope_t* AXIS2_CALL
axis2_amqp_util_get_soap_envelope (const axis2_char_t* content,
								   const axutil_env_t* env);

axutil_url_t* AXIS2_CALL
axis2_amqp_util_get_qpid_broker_url (axis2_msg_ctx_t*    msg_ctx,
									 const axutil_env_t* env);

axis2_char_t* AXIS2_CALL
axis2_amqp_util_get_qpid_broker_ip (axis2_msg_ctx_t*    msg_ctx,
									const axutil_env_t* env);

int AXIS2_CALL
axis2_amqp_util_get_qpid_broker_port (axis2_msg_ctx_t*    msg_ctx,
									  const axutil_env_t* env);

#endif
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

#ifndef AXIS2_AMQP_UTIL_H
#define AXIS2_AMQP_UTIL_H

#include <axis2_transport_in_desc.h>
#include <axutil_param_container.h>
#include <axiom_soap.h>
#include <axis2_conf_ctx.h>
#include <axutil_url.h>

axis2_char_t* AXIS2_CALL
axis2_amqp_util_get_conf_value_string (axis2_transport_in_desc_t* in_desc,
						  	      	   const axutil_env_t* 	   	  env,
						               const axis2_char_t*		  param_name);

int AXIS2_CALL
axis2_amqp_util_get_conf_value_int (axis2_transport_in_desc_t* in_desc,
						       		const axutil_env_t* 	   env,
						       		const axis2_char_t*		   param_name);

axiom_soap_envelope_t* AXIS2_CALL
axis2_amqp_util_get_soap_envelope (const axis2_char_t* content,
								   const axutil_env_t* env);

axutil_url_t* AXIS2_CALL
axis2_amqp_util_get_qpid_broker_url (axis2_msg_ctx_t*    msg_ctx,
									 const axutil_env_t* env);

axis2_char_t* AXIS2_CALL
axis2_amqp_util_get_qpid_broker_ip (axis2_msg_ctx_t*    msg_ctx,
									const axutil_env_t* env);

int AXIS2_CALL
axis2_amqp_util_get_qpid_broker_port (axis2_msg_ctx_t*    msg_ctx,
									  const axutil_env_t* env);

#endif
