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

#include <tcpmon_session.h>
#include <tcpmon_entry.h>

axis2_status_t tcpmon_session_add_new_entry(
    tcpmon_session_t * session,
    const axutil_env_t * env,
    tcpmon_entry_t * entry);

TCPMON_SESSION_TRANS_ERROR_FUNCT tcpmon_session_get_on_trans_fault(
    tcpmon_session_t * session,
    const axutil_env_t * env);

TCPMON_SESSION_NEW_ENTRY_FUNCT tcpmon_session_get_on_new_entry(
    tcpmon_session_t * session,
    const axutil_env_t * env);
