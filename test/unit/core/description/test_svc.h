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

#ifndef TEST_SVC_H
#define TEST_SVC_H

#undef NULL
#include <CuTest.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_hash.h>
#include <axis2_string.h>
#include <axis2_op.h>
#include <axis2_module_desc.h>
#include <axis2_op.h>
#include <axis2_flow.h>
#include <axis2_wsdl_op.h>
#include <axis2_phase.h>
#include <axis2_svc.h>

struct axis2_svc;
struct axis2_op;
struct axis2_module_desc;
struct axis2_wsdl_op;
struct axis2_conf;
struct axis2_flow;
struct axis2_phase;

void Testaxis2_svc_add_module_ops(CuTest *tc);
void Testaxis2_svc_create_with_qname(CuTest *tc);

#endif /* TEST_SVC_H*/
