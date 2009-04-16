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

#ifndef AXIS2_QPID_RECEIVER_H
#define AXIS2_QPID_RECEIVER_H

#include <axutil_env.h>
#include <axis2_conf_init.h>

class Axis2QpidReceiver
{
	public:
		Axis2QpidReceiver(const axutil_env_t* env,
						  axis2_conf_ctx_t* conf_ctx);
		~Axis2QpidReceiver(void);

		bool start(void);
		bool shutdown(void);

	private:
		const axutil_env_t* env;
		axis2_conf_ctx_t*   conf_ctx;
};

#endif
