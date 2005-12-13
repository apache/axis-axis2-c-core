#ifndef TEST_SVC_H
#define TEST_SVC_H

#include <string.h>
#include <stdio.h>
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

struct axis2_svc;
struct axis2_op;
struct axis2_module_desc;
struct axis2_wsdl_op;


void Testaxis2_svc_add_module_ops(CuTest *tc);

#endif /* TEST_SVC_H*/
