/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ADD_H
#define ADD_H


#include <axis2_svc_skeleton.h>
#include <axis2_log_default.h>
#include <axis2_error_default.h>
#include <axis2_om_text.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>

axis2_om_node_t *axis2_add_add(axis2_env_t **env, 
    axis2_om_node_t *node, 
    axis2_msg_ctx_t *msg_ctx);

#endif /* ADD_H*/
